#include "stm32f10x.h" // Device header
#include <stdio.h>
#include <stdarg.h>
#include "LED.H"
#include "pwm.h"

char Serial_RxPacket[100];
uint8_t Serial_RxFlag;

void serial1_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE);
}

void serial3_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART3, ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
    USART_SendData(USART3, Byte);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
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

// uint32_t Serial_Pow(uint32_t X, uint32_t Y)
// {
//     uint32_t Result = 1;
//     while (Y--)
//     {
//         Result *= X;
//     }
//     return Result;
// }

// void Serial_SendNumber(uint32_t Number, uint8_t Length)
// {
//     uint8_t i;
//     for (i = 0; i < Length; i++)
//     {
//         Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
//     }
// }

int fputc(int ch, FILE *f)
{
    Serial_SendByte(ch);
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

uint8_t Serial_GetRxFlag(void)
{
    if (Serial_RxFlag == 1)
    {
        Serial_RxFlag = 0;
        return 1;
    }
    return 0;
}

//void USART1_IRQHandler(void)
//{
//    static uint8_t RxState = 0;                          // 定义表示当前状态机状态的静态变量
//    static uint8_t pRxPacket = 0;                        // 定义表示当前接收数据位置的静态变量
//    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) // 判断是否是USART1的接收事件触发的中断
//    {
//        uint8_t RxData = USART_ReceiveData(USART1); // 读取数据寄存器，存放在接收的数据变量

//        /*使用状态机的思路，依次处理数据包的不同部分*/

//        /*当前状态为0，接收数据包包头*/
//        if (RxState == 0)
//        {
//            if (RxData == 0xFF) // 如果数据确实是包头
//            {
//                RxState = 1;   // 置下一个状态
//                pRxPacket = 0; // 数据包的位置归零
//            }
//        }
//        /*当前状态为1，接收数据包数据*/
//        else if (RxState == 1)
//        {
//            Serial_RxPacket[pRxPacket] = RxData; // 将数据存入数据包数组的指定位置
//            pRxPacket++;                         // 数据包的位置自增
//            if (pRxPacket >= 4)                  // 如果收够4个数据
//            {
//                RxState = 2; // 置下一个状态
//            }
//        }
//        /*当前状态为2，接收数据包包尾*/
//        else if (RxState == 2)
//        {
//            if (RxData == 0xFE) // 如果数据确实是包尾部
//            {
//                RxState = 0;       // 状态归0
//                Serial_RxFlag = 1; // 接收数据包标志位置1，成功接收一个数据包
//            }
//        }

//        USART_ClearITPendingBit(USART1, USART_IT_RXNE); // 清除标志位
//    }
//}

void USART3_IRQHandler(void)
{
    static uint8_t RxState = 0;                          // 定义表示当前状态机状态的静态变量
    static uint8_t pRxPacket = 0;                        // 定义表示当前接收数据位置的静态变量
    if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET) // 判断是否是USART3的接收事件触发的中断
    {
        uint8_t RxData = USART_ReceiveData(USART3); // 读取数据寄存器，存放在接收的数据变量

        /*使用状态机的思路，依次处理数据包的不同部分*/

        /*当前状态为0，接收数据包包头*/
        if (RxState == 0)
        {
            if (RxData == 0xFF) // 如果数据确实是包头
            {
                RxState = 1;   // 置下一个状态
                pRxPacket = 0; // 数据包的位置归零
            }
        }
        /*当前状态为1，接收数据包数据*/
        else if (RxState == 1)
        {
            Serial_RxPacket[pRxPacket] = RxData; // 将数据存入数据包数组的指定位置
            pRxPacket++;                         // 数据包的位置自增
            if (pRxPacket >= 4)                  // 如果收够4个数据
            {
                RxState = 2; // 置下一个状态
            }
        }
        /*当前状态为2，接收数据包包尾*/
        else if (RxState == 2)
        {
            if (RxData == 0xFE) // 如果数据确实是包尾部
            {
                RxState = 0;       // 状态归0
                Serial_RxFlag = 1; // 接收数据包标志位置1，成功接收一个数据包
            }
        }

        USART_ClearITPendingBit(USART3, USART_IT_RXNE); // 清除标志位
    }
}

 void USART1_IRQHandler(void)
 {
     PWM_Init();
 }