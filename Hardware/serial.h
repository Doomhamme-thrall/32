#ifndef __SERIAL_H
#define __SERIAL_H
#include <stdio.h>

typedef struct
{
    uint8_t LS;
    uint8_t RS;
    uint8_t LT;
    uint8_t RT;
    uint8_t A;
    uint8_t B;
    uint8_t X;
    uint8_t Y;
} SerialData_t;

#define RX_BUFFER_SIZE 33 // 环形缓冲区大小
#define FRAME_SIZE 11     // 每帧数据长度

void serial1_init(void);
void serial1_init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);
void Serial_ProcessDMA(void);
void Serial_GetData(SerialData_t *data);

#endif
