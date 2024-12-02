#include "stm32f10x.h" // Device header

void PWM_Init(void)
{
    // 1. 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 开启GPIOB时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  // 开启TIM3时钟

    // 2. 配置PB1为复用推挽输出
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;         // PB1引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO速度50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 3. 配置TIM3定时器
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 999;                     // 自动重装载值 (周期)
    TIM_TimeBaseStructure.TIM_Prescaler = 71;                   // 预分频值 (1MHz计数频率)
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 时钟分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // 4. 配置TIM3输出比较通道
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             // PWM模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 输出使能
    TIM_OCInitStructure.TIM_Pulse = 500;                          // 占空比 (设置为50%)
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     // 高电平有效
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);                      // 初始化TIM3通道4
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);

//    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); // 使能预装载寄存器
//	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

    // 5. 启动TIM3定时器
    TIM_Cmd(TIM3, ENABLE); // 使能TIM3

    // 6. 启动TIM3主输出
    TIM_CtrlPWMOutputs(TIM3, ENABLE);
}