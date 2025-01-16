#ifndef __SERIAL_H
#define __SERIAL_H

#include "stm32f10x.h"

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

#define USART3_BUFFER_SIZE 22
#define FRAME_SIZE 11

void serial3_init(void);
void Serial3_ProcessDMA(void);
void Serial_GetData(SerialData_t *data);

#endif
