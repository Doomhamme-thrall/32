#ifndef __SERIAL_H
#define __SERIAL_H
#include <stdio.h>

extern uint8_t Serial_TxPacket[];
extern int Serial_RxPacket[100];
uint8_t Serial_GetRxFlag(void);
void Serial_SendPacket(void);

void serial1_init(void);
void serial3_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);


#endif
