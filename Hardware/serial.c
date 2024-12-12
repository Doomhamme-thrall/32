#include "stm32f10x.h" // Device header
#include <stdio.h>
#include <stdarg.h>
#include "LED.H"
#include "pwm.h"
#include "serial.h"

#define FRAME_SIZE 11
#define BUFFER_SIZE 33 // 必须是 FRAME_SIZE 的整数倍
static uint8_t dma_buffer[BUFFER_SIZE];
static uint16_t last_position = 0;

static SerialData_t serial_data;

void serial3_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART3, &USART_InitStructure);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART3, ENABLE);
}

void serial1_init(void)
{
    // 启用GPIOA和USART1的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);

    // 初始化GPIO结构体
    GPIO_InitTypeDef GPIO_InitStruct;

    // 配置USART1 TX (PA9)为复用推挽输出
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 配置USART1 RX (PA10)为浮空输入
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 配置USART1
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No; // 奇校验
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStruct);
    USART_Cmd(USART1, ENABLE);

    // DMA初始化
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_InitTypeDef DMA_InitStruct;
    DMA_DeInit(DMA1_Channel5);

    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)dma_buffer;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStruct.DMA_BufferSize = BUFFER_SIZE;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel5, &DMA_InitStruct);
    DMA_Cmd(DMA1_Channel5, ENABLE);

    // USART DMA接收使能
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
}

static void ProcessBuffer(uint8_t *buffer, uint16_t length)
{
    // printf("\rprocess buffer\n ");

    for (uint16_t i = 0; i <= length - FRAME_SIZE; i++)
    {
        if (buffer[i] == 0xAA && buffer[i + 10] == 0xFF)
        {
            //            printf("package found  \n");
            uint32_t checksum = 0;
            for (uint8_t j = 0; j < 9; j++)
            {
                checksum += buffer[i + j];
            }

            if (checksum % 256 == buffer[i + 9])
            {
                printf("checksum correct\n");
                memcpy(&serial_data, &buffer[i + 1], sizeof(SerialData_t));
                i += FRAME_SIZE - 1; // 跳过已处理的数据
            }
            else
            {
                // printf("checksum incorrect\n");
            }
        }
    }
}

// 处理 DMA 数据
void Serial_ProcessDMA(void)
{
    //     printf("\rprocess dma\n ");
    //     for (int i = 0; i < 46; i++)
    //     {
    //         printf("%d ", dma_buffer[i]);
    //     }
    uint16_t current_position = BUFFER_SIZE - DMA_GetCurrDataCounter(DMA1_Channel5);
    // printf("\rcurrent_position1: %d\n ", current_position);
    if (current_position != last_position)
    {
        // printf("\rcurrent_position2: %d\n ", current_position);
        if (current_position > last_position)
        {
            ProcessBuffer(&dma_buffer[last_position], current_position - last_position);
        }
        else
        {
            ProcessBuffer(&dma_buffer[last_position], BUFFER_SIZE - last_position);
            ProcessBuffer(&dma_buffer[0], current_position);
        }
        last_position = current_position;
    }
}

// 获取解码后的数据
void Serial_GetData(SerialData_t *data)
{
    // printf("get data\n");
    if (data)
    {
        // printf("%d ", serial_data.value1);
        // printf("%d ", serial_data.value2);
        // printf("%d ", serial_data.value3);
        // printf("%d ", serial_data.value4);
        // printf("%d ", serial_data.int1);
        // printf("%d ", serial_data.int2);
        // printf("%d ", serial_data.int3);
        // printf("%d ", serial_data.int4);
        *data = serial_data;
    }
}

void Serial_SendByte(uint8_t Byte)
{
    USART_SendData(USART1, Byte);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        ;
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
    uint16_t i;
    for (i = 0; i < Length; i++)
    {
        Serial_SendByte(Array[i]);
    }
}

void Serial_SendString(char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)
    {
        Serial_SendByte(String[i]);
    }
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--)
    {
        Result *= X;
    }
    return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
    }
}

int fputc(int ch, FILE *f)
{
    // 等待 USART1 数据寄存器为空
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        ;
    // 发送字符
    USART_SendData(USART1, (uint8_t)ch);

    // 等待传输完成
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        ;

    return ch;
}

void Serial_Printf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    Serial_SendString(String);
}

// 串口中断处理函数
void USART3_IRQHandler(void)
{
    if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
    {
        uint8_t data = USART_ReceiveData(USART3);
        Serial_SendByte(data);
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}