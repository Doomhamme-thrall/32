#include "stm32f10x.h" // Device header

void spi_w_ss(uint8_t value)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, value);
}

void spi_start(void)
{
    spi_w_ss(0);
}

void spi_stop(void)
{
    spi_w_ss(1);
}

void spi_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    // NSS
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // SCK,MOSI
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // MISO
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                        // SPI主机模式
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   // 双线全双工
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; // 波特率预分
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                         // 第一个时钟沿捕获数据
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                           // 时钟极性
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                    // 8位数据帧
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                   // 高位先行
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                            // 软件控制NSS
    SPI_InitStructure.SPI_CRCPolynomial = 7;                             // CRC多项式
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_Cmd(SPI1, ENABLE);

    spi_w_ss(1);
}

//SPI数据交换函数
uint8_t spi_swap(uint8_t data)
{
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET)
        ;

    SPI_I2S_SendData(SPI1, data);

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET)
        ;

    return SPI_I2S_ReceiveData(SPI1);
}