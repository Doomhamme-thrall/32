#include "stm32f10x.h" // Device header

void PWM_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // GPIOE 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  // TIM1 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 19999;                   // 50 Hz
    TIM_TimeBaseStructure.TIM_Prescaler = 71;                   // 预分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 时钟分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // PWM 模式 1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; // 不使用互补输出
    TIM_OCInitStructure.TIM_Pulse = 1500;                            //  ARR
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        // 高电平有效
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;      // 不使用互补极性
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;     // 闲置状态
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

    TIM_OC1Init(TIM1, &TIM_OCInitStructure); // 通道 1 (PE9)
    TIM_OC2Init(TIM1, &TIM_OCInitStructure); // 通道 2 (PE11)
    TIM_OC3Init(TIM1, &TIM_OCInitStructure); // 通道 3 (PE13)
    TIM_OC4Init(TIM1, &TIM_OCInitStructure); // 通道 4 (PE14)

    TIM_Cmd(TIM1, ENABLE);

    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void PWM_Set(uint16_t *compare)
{
    TIM_SetCompare1(TIM1, compare[0]);
    TIM_SetCompare2(TIM1, compare[1]);
    TIM_SetCompare3(TIM1, compare[2]);
    TIM_SetCompare4(TIM1, compare[3]);
}