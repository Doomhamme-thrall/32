#include "stm32f10x.h" // Device header

uint16_t esize;

void dma_init(uint32_t addra, uint32_t addrb, uint16_t size)
{
    esize=size;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = addra;                       // 外设基地址
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 数据宽度
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;         // 外设地址是否自增
    DMA_InitStructure.DMA_MemoryBaseAddr = addrb;                           // 存储器基地址
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         // 存储器数据宽度
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 // 存储器地址是否自增
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                      // 外设作为源端还是目标端
    DMA_InitStructure.DMA_BufferSize = size;                                // 传输次数
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           // 是否自动重装
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;                             // 是否软件触发
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                     // 优先级
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1, DISABLE);
}

void dma_transport(void)
{   
    DMA_Cmd(DMA1_Channel1, DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel1, esize);
    DMA_Cmd(DMA1_Channel1, ENABLE);

    while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
    DMA_ClearFlag(DMA1_FLAG_TC1);
}