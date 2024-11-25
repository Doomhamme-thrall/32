#ifndef __serial_h__
#define __serial_h__
void serial_init(void);
void serial_sendbyte(uint8_t byte);
void serial_sendsdsring(char *string);
uint32_t power(uint32_t x,uint32_t y);
void serial_sendnum(uint32_t num,uint8_t length);
uint8_t serial_recdata(void);
#endif
