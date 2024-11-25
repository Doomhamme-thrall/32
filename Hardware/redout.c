#include "stm32f10x.h"                  // Device header

int CountSensor_Count=0;

void redout_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //打开GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);   //打开AFIO
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       //模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;          //引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);              //A or B
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource10);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line =EXTI_Line10;             //线路号
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;              //开启中断
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    //中断模式
	EXTI_InitStructure.EXTI_Trigger =EXTI_Trigger_Falling; //下降触发
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);        //中断分组
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;      //指定通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;           //开启
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;        //响应优先级
	NVIC_Init(&NVIC_InitStruct);
}

int redout_get(){
	return CountSensor_Count;
}

void EXTI15_10_IRQHandler(void){                //中断函数
	if(EXTI_GetITStatus(EXTI_Line10)==SET){
		CountSensor_Count++;
		EXTI_ClearITPendingBit(EXTI_Line10); //清除标志位
	}
}
