#include "stm32f10x.h"
#include "serial.h"
#include "stdio.h"

#define USART3_BUFFER_SIZE 22

static uint8_t usart3_dma_buffer[USART3_BUFFER_SIZE]; // USART3 缓冲区
static uint16_t usart3_last_position = 0;             // 上次处理的位置

SerialData_t usart3_data;

// 私有函数声明
static void USART3_ProcessBuffer(uint8_t *buffer, uint16_t length);

void serial1_init(void)
{
    // GPIO 初始化
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // 浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // USART1 初始化
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStruct);
    USART_Cmd(USART1, ENABLE);

    //nvic初始化
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}



// USART3 初始化
void serial3_init(void)
{
    // GPIO 初始化
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // 浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // USART3 初始化
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART3, &USART_InitStruct);
    USART_Cmd(USART3, ENABLE);

    // DMA 初始化
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_InitTypeDef DMA_InitStruct;
    DMA_DeInit(DMA1_Channel3);

    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)usart3_dma_buffer;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStruct.DMA_BufferSize = USART3_BUFFER_SIZE;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel3, &DMA_InitStruct);
    DMA_Cmd(DMA1_Channel3, ENABLE);

    USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
}

// DMA 数据处理函数
void Serial3_ProcessDMA(void)
{
    // printf("Serial3_ProcessDMA\n");
    for (int i = 0; i < USART3_BUFFER_SIZE; i++)
    {
        printf("%d ", usart3_dma_buffer[i]);
    }
    uint16_t current_position = USART3_BUFFER_SIZE - DMA_GetCurrDataCounter(DMA1_Channel3);
    // printf("current_position: %d\n", current_position);
    if (current_position != usart3_last_position)
    {
        if (current_position > usart3_last_position)
        {
            USART3_ProcessBuffer(&usart3_dma_buffer[usart3_last_position], current_position - usart3_last_position);
        }
        else
        {
            USART3_ProcessBuffer(&usart3_dma_buffer[usart3_last_position], USART3_BUFFER_SIZE - usart3_last_position);
            USART3_ProcessBuffer(&usart3_dma_buffer[0], current_position);
        }
        usart3_last_position = current_position;
    }
}

// 数据帧处理函数
static void USART3_ProcessBuffer(uint8_t *buffer, uint16_t length)
{
    printf("USART3_ProcessBuffer\n");
    for (uint16_t i = 0; i <= length - FRAME_SIZE; i++)
    {
        if (buffer[i] == 0xAA && buffer[i + 10] == 0xFF)
        {
            uint32_t checksum = 0;
            for (uint8_t j = 0; j < 9; j++)
            {
                checksum += buffer[i + 1 + j];
            }

            if (1)
            {
                // printf("checksum correct\n");
                memcpy(&usart3_data, &buffer[i + 1], sizeof(SerialData_t));
                i += FRAME_SIZE - 1; // 跳过已处理的数据
            }
        }
    }
}

void Serial_GetData(SerialData_t *data)
{
    if (data)
    {
        *data = usart3_data;
    }
}

int fputc(int ch, FILE *f)
{
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
        ;
    USART_SendData(USART3, (uint8_t)ch);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
        ;
    return ch;
}

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        USART_SendData(USART3, USART_ReceiveData(USART1));
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}