#include "stm32f10x.h"

void pwm_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 打开TIM2

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 打开GPIOA

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	  // 模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		  // 引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);			  // A or B

	TIM_InternalClockConfig(TIM2); // 选择内部时钟

	TIM_TimeBaseInitTypeDef TimTimeBaseInitScture;
	TimTimeBaseInitScture.TIM_ClockDivision = TIM_CKD_DIV1;		// 采样时钟分频
	TimTimeBaseInitScture.TIM_CounterMode = TIM_CounterMode_Up; // 上升计数
	TimTimeBaseInitScture.TIM_Period = 99;					// 计数周期ARR
	TimTimeBaseInitScture.TIM_Prescaler = 719;					// 计数分频PSC
	TimTimeBaseInitScture.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TimTimeBaseInitScture);

	TIM_OCInitTypeDef Tim_OCInitStructure;
	TIM_OCStructInit(&Tim_OCInitStructure);
	Tim_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	Tim_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
	Tim_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	Tim_OCInitStructure.TIM_Pulse = 20;	 // CCR  *计数频率=高电平时间
	TIM_OC2Init(TIM2, &Tim_OCInitStructure); // 初始化通道

	Tim_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	Tim_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
	Tim_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	Tim_OCInitStructure.TIM_Pulse = 1500;	 // CCR  *计数频率=高电平时间
	TIM_OC1Init(TIM2, &Tim_OCInitStructure); // 初始化通道

	TIM_Cmd(TIM2, ENABLE);
}

void pwm_setcompare(int compare)
{
	TIM_SetCompare2(TIM2, compare);
}

void pwm_setprescaler(int prescaler)
{
	TIM_PrescalerConfig(TIM2, prescaler, TIM_PSCReloadMode_Immediate);
}