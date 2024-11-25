#include "stm32f10x.h"                  // Device header

void ic_init(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  //打开TIM2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //打开GPIOA
		
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       //模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;          //引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);              //A or B
	
	TIM_InternalClockConfig(TIM3);                       //选择内部时钟
	
	TIM_TimeBaseInitTypeDef TimTimeBaseInitScture;
	TimTimeBaseInitScture.TIM_ClockDivision=TIM_CKD_DIV1;    //采样时钟分频 
	TimTimeBaseInitScture.TIM_CounterMode=TIM_CounterMode_Up;//上升计数
	TimTimeBaseInitScture.TIM_Period=65536-1;				 //计数周期ARR
	TimTimeBaseInitScture.TIM_Prescaler=72-1;                //计数分频PSC
	TimTimeBaseInitScture.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TimTimeBaseInitScture);
	
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_1;                //输入捕获通道
	TIM_ICInitStruct.TIM_ICFilter=0XF;					       //滤波
	TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Rising;     //上升沿触发
	TIM_ICInitStruct.TIM_ICPrescaler=TIM_ICPSC_DIV1;           //分频
	TIM_ICInitStruct.TIM_ICSelection=TIM_ICSelection_DirectTI; //数据选择器
	TIM_PWMIConfig(TIM3,&TIM_ICInitStruct);
	
	TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);                //TRGI触发源
	TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Reset);             //从模式为重置
	
	TIM_Cmd(TIM3,ENABLE);
}

uint32_t ic_getfreq(){
	return 1000000/TIM_GetCapture1(TIM3);
}

uint32_t ic_getduty(){
	return TIM_GetCapture2(TIM3)*100/TIM_GetCapture1(TIM3);
}