#include "stm32f10x.h" // Device header

uint32_t flash_read_word(uint32_t address)
{
    return *((__IO uint32_t *)(address));
}

uint16_t flash_read_half_word(uint32_t address)
{
    return *((__IO uint16_t *)(address));
}

uint8_t flash_read_byte(uint32_t address)
{
    return *((__IO uint8_t *)(address));
}

void flash_erase_all(void)
{
    FLASH_Unlock();
    FLASH_EraseAllPages();
    FLASH_Lock();
}

void flash_erase_page(uint32_t page_address)
{
    FLASH_Unlock();
    FLASH_ErasePage(page_address);
    FLASH_Lock();
}

void flash_program_word(uint32_t address, uint32_t data)
{
    FLASH_Unlock();
    FLASH_ProgramWord(address, data);
    FLASH_Lock();
}

void flash_program_half_word(uint32_t address, uint16_t data)
{
    FLASH_Unlock();
    FLASH_ProgramHalfWord(address, data);
    FLASH_Lock();
}
