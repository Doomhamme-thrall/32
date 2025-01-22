#ifndef __flash_h__
#define __flash_h__
#include "stm32f10x.h"

uint32_t flash_read_word(uint32_t address);
uint16_t flash_read_half_word(uint32_t address);
uint8_t flash_read_byte(uint32_t address);
void flash_erase_all(void);
void flash_erase_page(uint32_t page_address);
void flash_program_word(uint32_t address, uint32_t data);
void flash_program_half_word(uint32_t address, uint16_t data);

#endif