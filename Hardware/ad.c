#include "stm32f10x.h" // Device header

uint16_t ad_value[4];

void ad_init(void)
{
    // ADC初始化
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // 模拟输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5); // ADC1,通道0,序列1，采样时间239.5周期

    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  // 独立模式
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              // 右对齐
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 软件触发
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                        // 是否扫描
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                  // 是否连续
    ADC_InitStructure.ADC_NbrOfChannel = 2;                             // 序列数量
    ADC_Init(ADC1, &ADC_InitStructure);

    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;             // 外设基地址
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 数据宽度
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            // 外设地址是否自增
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ad_value;                  // 存储器基地址
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         // 存储器数据宽度
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     // 存储器地址是否自增
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          // 外设作为源端还是目标端
    DMA_InitStructure.DMA_BufferSize = 2;                                       // 传输次数
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             // 是否自动重装
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                // 是否软件触发
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                         // 优先级
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1, ENABLE);

    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    // ADC校准
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1) == SET)
        ;
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET)
        ;
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}