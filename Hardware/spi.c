#include "stm32f10x.h" // Device header

void spi_w_ss(uint8_t value){
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, value);
}

void spi_w_sck(uint8_t value){
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, value);
}

void spi_w_mosi(uint8_t value){
    GPIO_WriteBit(GPIOA, GPIO_Pin_7, value);
}

uint8_t spi_r_miso(void){
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
}

void spi_start(void){
    spi_w_ss(0);
}

void spi_stop(void){
    spi_w_ss(1);
}

void spi_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    spi_w_ss(1);
    spi_w_sck(0);
}

uint8_t spi_swap(uint8_t data)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        spi_w_mosi(data & 0x80);
        data <<= 1;
        spi_w_sck(1);
        if(spi_r_miso()==1){
            data |= 0x01;
        }
        spi_w_sck(0);
    }
    
    return data;
}