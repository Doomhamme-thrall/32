#ifndef __serial3_h__
#define __serial3_h__

uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);

void serial3_init(void);
void serial3_sendbyte(uint8_t byte);
void serial3_sendstring(char *string);

#endif
