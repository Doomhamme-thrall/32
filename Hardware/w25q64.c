#include "stm32f10x.h" // Device header
#include "spi.h"

#define W25Q64_WRITE_ENABLE 0x06
#define W25Q64_WRITE_DISABLE 0x04
#define W25Q64_READ_STATUS_REGISTER_1 0x05
#define W25Q64_READ_STATUS_REGISTER_2 0x35
#define W25Q64_WRITE_STATUS_REGISTER 0x01
#define W25Q64_PAGE_PROGRAM 0x02
#define W25Q64_QUAD_PAGE_PROGRAM 0x32
#define W25Q64_BLOCK_ERASE_64KB 0xD8
#define W25Q64_BLOCK_ERASE_32KB 0x52
#define W25Q64_SECTOR_ERASE_4KB 0x20
#define W25Q64_CHIP_ERASE 0xC7
#define W25Q64_ERASE_SUSPEND 0x75
#define W25Q64_ERASE_RESUME 0x7A
#define W25Q64_POWER_DOWN 0xB9
#define W25Q64_HIGH_PERFORMANCE_MODE 0xA3
#define W25Q64_CONTINUOUS_READ_MODE_RESET 0xFF
#define W25Q64_RELEASE_POWER_DOWN_HPM_DEVICE_ID 0xAB
#define W25Q64_MANUFACTURER_DEVICE_ID 0x90
#define W25Q64_READ_UNIQUE_ID 0x4B
#define W25Q64_JEDEC_ID 0x9F
#define W25Q64_READ_DATA 0x03
#define W25Q64_FAST_READ 0x0B
#define W25Q64_FAST_READ_DUAL_OUTPUT 0x3B
#define W25Q64_FAST_READ_DUAL_IO 0xBB
#define W25Q64_FAST_READ_QUAD_OUTPUT 0x6B
#define W25Q64_FAST_READ_QUAD_IO 0xEB
#define W25Q64_OCTAL_WORD_READ_QUAD_IO 0xE3

#define W25Q64_DUMMY_BYTE 0xFF

void w25q64_init(void)
{
    spi_init();
}

void w25q64_read_id(uint8_t *MID, uint16_t *DID)
{
    spi_start();
    spi_swap(W25Q64_JEDEC_ID);
    *MID = spi_swap(W25Q64_DUMMY_BYTE);
    // print("MID: %x\n", *MID);
    *DID = spi_swap(W25Q64_DUMMY_BYTE);
    // print("DID: %x\n", *DID);
    *DID <<= 8;
    *DID |= spi_swap(W25Q64_DUMMY_BYTE);
    // print("DID: %x\n", *DID);
    spi_stop();
}

void w25q64_w_enable(void)
{
    spi_start();
    spi_swap(W25Q64_WRITE_ENABLE);
    spi_stop();
}

void w25q64_waitbusy(void)
{
    spi_start();
    spi_swap(W25Q64_READ_STATUS_REGISTER_1);
    while ((spi_swap(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)
        ;
    spi_stop();
}

void w25q64_page_write(uint32_t address, uint8_t *dataarray, uint16_t count)
{
    uint16_t i;
    w25q64_w_enable();
    spi_start();
    spi_swap(W25Q64_PAGE_PROGRAM);
    spi_swap(address >> 16);
    spi_swap(address >> 8);
    spi_swap(address);
    for (i = 0; i < count; i++)
    {
        spi_swap(dataarray[i]);
    }
    spi_stop();
    w25q64_waitbusy();
}

void w25q64_sector_erase(uint32_t address)
{
    w25q64_w_enable();
    spi_start();
    spi_swap(W25Q64_SECTOR_ERASE_4KB);
    spi_swap(address >> 16);
    spi_swap(address >> 8);
    spi_swap(address);
    spi_stop();
    w25q64_waitbusy();
}

void w25q64_read(uint32_t address, uint8_t *dataarray, uint32_t count)
{
    uint16_t i;
    spi_start();
    spi_swap(W25Q64_READ_DATA);
    spi_swap(address >> 16);
    spi_swap(address >> 8);
    spi_swap(address);
    for (i = 0; i < count; i++)
    {
        dataarray[i] = spi_swap(W25Q64_DUMMY_BYTE);
    }
}
