#ifndef __w25q64_h__
#define __w25q64_h__
#include "stm32f10x.h"

void w25q64_init(void);
uint8_t w25q64_read_id(uint8_t *mid, uint16_t *did);
void w25q64_page_write(uint32_t address, uint8_t *dataarray, uint16_t count);
void w25q64_sector_erase(uint32_t address);
void w25q64_read(uint32_t address, uint8_t *dataarray, uint32_t count);

#endif