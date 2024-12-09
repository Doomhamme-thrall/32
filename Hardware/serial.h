#ifndef __SERIAL_H
#define __SERIAL_H
#include <stdio.h>

typedef struct
{
    uint8_t value1;
    uint8_t value2;
    uint8_t value3;
    uint8_t value4;
    uint8_t int1;
    uint8_t int2;
    uint8_t int3;
    uint8_t int4;
} SerialData_t;

#define RX_BUFFER_SIZE 33 // 环形缓冲区大小
#define FRAME_SIZE 11     // 每帧数据长度

// 接收到的解析后的数据

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
