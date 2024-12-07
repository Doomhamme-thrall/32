#include "stm32f10x.h" // Device header
#include <string.h>


uint8_t cmd1[] = {0XFF, 0XAA, 0X00, 0X01, 0X00};//恢复默认设置
uint8_t cmd2[] = {0XFF, 0XAA, 0X00, 0X01, 0X00};//加速度计校准
uint8_t cmd3[] = {0XFF, 0XAA, 0X00, 0X01, 0X00};//磁场校准
uint8_t cmd4[] = {0XFF, 0XAA, 0X00, 0X01, 0X00};//高度归零
uint8_t cmd5[] = {0XFF, 0XAA, 0X00, 0X01, 0X00};//安装方向水平
uint8_t cmd6[] = {0XFF, 0XAA, 0X00, 0X01, 0X00};//解除休眠
uint8_t cmd7[] = {0XFF, 0XAA, 0X00, 0X01, 0X00};//算法转换
uint8_t cmd8[] = {0XFF, 0XAA, 0X00, 0X01, 0X00};//陀螺仪自动校准
uint8_t cmd9[] = {0XFF, 0XAA, 0X00, 0X01, 0X00};//设置回传内容
uint8_t cmd9[] = {0XFF, 0XAA, 0X00, 0X01, 0X00}; // 设置回传速率

struct STime stcTime;
struct SAcc stcAcc;
struct SGyro stcGyro;
struct SAngle stcAngle;
struct SMag stcMag;
struct SDStatus stcDStatus;
struct SPress stcPress;
struct SLonLat stcLonLat;
struct SGPSV stcGPSV;
struct SQ stcQ;

void jy901s_init(void)
{
    // 打开GPIOA和USART2时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    // 配置PA2 (USART2_TX)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置PA3 (USART2_RX)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure;
    // 配置USART2
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);
    USART_Cmd(USART2, ENABLE);

    // 恢复默认设置
    jy901s_cmd(cmd1);
}


void jy901s_cmd(uint8_t *cmd)
{
    for (int i = 0; i < 5; i++)
    {
        USART_SendData(USART2, cmd[i]);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
            ;
    }
}

void uart3_read_data(unsigned char ucData)
{
    static unsigned char ucRxBuffer[256];
    static unsigned char ucRxCount = 0;

    ucRxBuffer[ucRxCount++] = ucData; // 将收到的数据存入缓冲区中
    if (ucRxBuffer[0] != 0x55)        // 数据头
    {
        ucRxCount = 0;
        return;
    }
    if (ucRxCount < 11)
    {
        return;
    } // 数据不满11个，则返回
    else
    {
        switch (ucRxBuffer[1]) // 判断数据是哪种数据，然后将其拷贝到对应的结构体中，有些数据包需要通过上位机打开对应的输出后，才能接收到这个数据包的数据
        {
        // memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据结构体里面，从而实现数据的解析。
        case 0x50:
            memcpy(&stcTime, &ucRxBuffer[2], 8);
            break;
        case 0x51:
            memcpy(&stcAcc, &ucRxBuffer[2], 8);
            break;
        case 0x52:
            memcpy(&stcGyro, &ucRxBuffer[2], 8);
            break;
        case 0x53:
            memcpy(&stcAngle, &ucRxBuffer[2], 8);
            break;
        case 0x54:
            memcpy(&stcMag, &ucRxBuffer[2], 8);
            break;
        case 0x55:
            memcpy(&stcDStatus, &ucRxBuffer[2], 8);
            break;
        case 0x56:
            memcpy(&stcPress, &ucRxBuffer[2], 8);
            break;
        case 0x57:
            memcpy(&stcLonLat, &ucRxBuffer[2], 8);
            break;
        case 0x58:
            memcpy(&stcGPSV, &ucRxBuffer[2], 8);
            break;
        case 0x59:
            memcpy(&stcQ, &ucRxBuffer[2], 8);
            break;
        }
        ucRxCount = 0; // 清空缓存区
    }
}