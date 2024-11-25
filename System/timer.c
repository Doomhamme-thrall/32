#include "stm32f10x.h"                  // Device header

void timer_init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  //打开TIM2
	
	TIM_InternalClockConfig(TIM2);                       //选择内部时钟
	
	TIM_TimeBaseInitTypeDef TimTimeBaseInitScture;
	TimTimeBaseInitScture.TIM_ClockDivision=TIM_CKD_DIV1;    //采样时钟分频 
	TimTimeBaseInitScture.TIM_CounterMode=TIM_CounterMode_Up;//上升计数
	TimTimeBaseInitScture.TIM_Period=9999;					 //计数周期
	TimTimeBaseInitScture.TIM_Prescaler=7199;                //计数分频
	TimTimeBaseInitScture.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TimTimeBaseInitScture);
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);               //开启更新中断到NVIC通路
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);        //中断分组
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;      //指定通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;           //开启
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2; //抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;        //响应优先级
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM2,ENABLE);
}

//void Tim2_IRQHandler(void){
//	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
//}
