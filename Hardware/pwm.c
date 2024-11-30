#include "stm32f10x.h"

/**
 * @brief  初始化PWM
 *
 * TIM1,PE9、PE11、PE13、PE14以及OC1、OC2、OC3、OC4
 */
void pwm_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); // 打开TIM1

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); // 打开GPIOE

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;										// 模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_13; // 引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;									// 速度
	GPIO_Init(GPIOB, &GPIO_InitStructure);												// A or B

	TIM_InternalClockConfig(TIM1); // 选择内部时钟

	TIM_TimeBaseInitTypeDef TimTimeBaseInitScture;
	TimTimeBaseInitScture.TIM_ClockDivision = TIM_CKD_DIV1;		// 采样时钟分频
	TimTimeBaseInitScture.TIM_CounterMode = TIM_CounterMode_Up; // 上升计数
	TimTimeBaseInitScture.TIM_Period = 1999;					// 计数周期ARR
	TimTimeBaseInitScture.TIM_Prescaler = 7199;					// 计数分频PSC
	TimTimeBaseInitScture.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TimTimeBaseInitScture);

	TIM_OCInitTypeDef Tim_OCInitStructure;
	TIM_OCStructInit(&Tim_OCInitStructure);
	Tim_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	Tim_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
	Tim_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	Tim_OCInitStructure.TIM_Pulse = 1500;	 // CCR  *计数频率=高电平时间
	TIM_OC4Init(TIM1, &Tim_OCInitStructure); // 初始化通道
	TIM_OC3Init(TIM1, &Tim_OCInitStructure); // 初始化通道
	TIM_OC2Init(TIM1, &Tim_OCInitStructure); // 初始化通道
	TIM_OC1Init(TIM1, &Tim_OCInitStructure); // 初始化通道

	TIM_Cmd(TIM1, ENABLE);
}

/**
 * @brief  设置PE9占空比
 */
void pwm_setcompare1(int compare)
{
	TIM_SetCompare1(TIM1, compare);
}

/**
 * @brief  设置PE10占空比
 *
 */
void pwm_setcompare2(int compare)
{
	TIM_SetCompare2(TIM1, compare);
}

/**
 * @brief  设置PE11占空比
 *
 */
void pwm_setcompare3(int compare)
{
	TIM_SetCompare3(TIM1, compare);
}

/**
 * @brief  设置PE13占空比
 *
 */
void pwm_setcompare4(int compare)
{
	TIM_SetCompare4(TIM1, compare);
}

/**
 * @brief  设置TIM1的预分频
 *
 */
void pwm_setprescaler(int prescaler)
{
	TIM_PrescalerConfig(TIM1, prescaler, TIM_PSCReloadMode_Immediate);
}