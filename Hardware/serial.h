#ifndef __SERIAL_H
#define __SERIAL_H
#include <stdio.h>


// 外部变量和函数
extern Serial3_Data_t serial3_rx_data;
extern uint8_t Serial_TxPacket[];
extern int Serial_RxPacket[100];

#define RX_BUFFER_SIZE 64 // 环形缓冲区大小
#define FRAME_SIZE 23     // 每帧数据长度

// 接收到的解析后的数据
typedef struct
{
    float f1, f2, f3, f4;   // 浮点数
    uint8_t i1, i2, i3, i4; // 整数
} Serial3_Data_t;

uint8_t Serial_GetRxFlag(void);
void serial1_init(void);
void serial3_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);
void serial3_process_dma(void);

#endif
