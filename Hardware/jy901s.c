#include "stm32f10x.h"
#include "jy901s.h"
#include "Delay.h"
#include "serial.h"

#define BUFFER_SIZE 22 // 必须是 JY901 数据帧大小（11 字节）的整数倍
static uint8_t dma_buffer[BUFFER_SIZE];
static uint16_t last_position = 0; // 上次处理到的缓冲区位置

// 数据存储结构体
static JY901S_Data_t jy901s_data;

// 私有函数声明
static void JY901S_ProcessBuffer(uint8_t *buffer, uint16_t length);
static void JY901S_SendCommand(uint8_t *cmd, uint8_t length);

// 初始化函数
void JY901S_Init(void)
{
    // GPIO 初始化
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;

    // USART2 TX (PA2) 配置为复用推挽输出
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // USART2 RX (PA3) 配置为浮空输入
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // USART 配置
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2, &USART_InitStruct);
    USART_Cmd(USART2, ENABLE);

    // DMA 初始化
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_InitTypeDef DMA_InitStruct;
    DMA_DeInit(DMA1_Channel6);

    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;
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
    DMA_Init(DMA1_Channel6, &DMA_InitStruct);
    DMA_Cmd(DMA1_Channel6, ENABLE);

    // USART DMA 接收使能
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);

    // 初始化指令发送
    uint8_t cmds[][5] = {
        {0xff, 0xaa, 0x69, 0x88, 0xb5}, // 解锁指令
        {0xff, 0xaa, 0x00, 0x01, 0x00}, // 恢复默认设置
        {0xff, 0xaa, 0x01, 0x01, 0x00}, // 加速度计校准
        {0xff, 0xaa, 0x01, 0x02, 0x00}, // 磁场校准
        {0xff, 0xaa, 0x01, 0x03, 0x00}, // 高度归零
        {0xff, 0xaa, 0x23, 0x00, 0x00}, // 安装方向水平
        {0xff, 0xaa, 0x22, 0x01, 0x00}, // 解除休眠
        {0xff, 0xaa, 0x24, 0x01, 0x00}, // 算法转换
        {0xff, 0xaa, 0x63, 0x00, 0x00}, // 陀螺仪自动校准
        {0xff, 0xaa, 0x02, 0x1e, 0x02}, // 设置回传内容
        {0xff, 0xaa, 0x03, 0x09, 0x00}  // 设置回传速率
    };

    JY901S_SendCommand(cmds[0], 5);
    Delay_ms(100);
    JY901S_SendCommand(cmds[1], 5);
    Delay_ms(100);
    JY901S_SendCommand(cmds[9], 5);
    Delay_ms(100);

    Serial_SendString("inited\0");
}

// DMA 数据处理函数
void JY901S_ProcessDMA(void)
{
    uint16_t current_position = BUFFER_SIZE - DMA_GetCurrDataCounter(DMA1_Channel6);

    if (current_position != last_position)
    {
        if (current_position > last_position)
        {
            JY901S_ProcessBuffer(&dma_buffer[last_position], current_position - last_position);
        }
        else
        {
            JY901S_ProcessBuffer(&dma_buffer[last_position], BUFFER_SIZE - last_position);
            JY901S_ProcessBuffer(&dma_buffer[0], current_position);
        }
        last_position = current_position;

        // 调试打印缓冲区数据
        // Serial_SendString("processing\0");
        // Serial_SendArray(dma_buffer, 22);
    }
}

// 数据帧处理函数
static void JY901S_ProcessBuffer(uint8_t *buffer, uint16_t length)
{
    for (uint16_t i = 0; i < length; i += 11)
    {
        if (buffer[i] == 0x55)
        {
            uint8_t checksum = 0;
            for (uint8_t j = 0; j < 10; j++)
            {
                checksum += buffer[i + j];
            }
            if ((checksum & 0xFF) == buffer[i + 10])
            {
                uint8_t type = buffer[i + 1];
                switch (type)
                {
                case 0x51:
                    jy901s_data.ax = ((int16_t)((buffer[i + 3] << 8) | buffer[i + 2])) / 32768.0f * 16.0f;
                    jy901s_data.ay = ((int16_t)((buffer[i + 5] << 8) | buffer[i + 4])) / 32768.0f * 16.0f;
                    jy901s_data.az = ((int16_t)((buffer[i + 7] << 8) | buffer[i + 6])) / 32768.0f * 16.0f;
                    break;
                case 0x52:
                    jy901s_data.wx = ((int16_t)((buffer[i + 3] << 8) | buffer[i + 2])) / 32768.0f * 2000.0f;
                    jy901s_data.wy = ((int16_t)((buffer[i + 5] << 8) | buffer[i + 4])) / 32768.0f * 2000.0f;
                    jy901s_data.wz = ((int16_t)((buffer[i + 7] << 8) | buffer[i + 6])) / 32768.0f * 2000.0f;
                    break;
                case 0x53:
                    jy901s_data.roll = ((int16_t)((buffer[i + 3] << 8) | buffer[i + 2])) / 32768.0f * 180.0f;
                    jy901s_data.pitch = ((int16_t)((buffer[i + 5] << 8) | buffer[i + 4])) / 32768.0f * 180.0f;
                    jy901s_data.yaw = ((int16_t)((buffer[i + 7] << 8) | buffer[i + 6])) / 32768.0f * 180.0f;
                    break;
                default:
                    break;
                }
            }
        }
    }
}

// 获取数据
void JY901S_GetData(JY901S_Data_t *data)
{
    if (data)
    {
        *data = jy901s_data;
    }
}

// 发送指令
static void JY901S_SendCommand(uint8_t *cmd, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
            ;
        USART_SendData(USART2, cmd[i]);
    }
}
