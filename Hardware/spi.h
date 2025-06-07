#ifndef __spi_h__
#define __spi_h__

#include "stm32f10x.h"

void spi_init(void);
uint8_t spi_swap(uint8_t data);

void spi_start(void);
void spi_stop(void);

#endif