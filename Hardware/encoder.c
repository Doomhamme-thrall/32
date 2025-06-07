#include "stm32f10x.h"                  // Device header
int count=0;
int temp;
void encoder_init(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //打开GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);   //打开AFIO
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       //模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;          //引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //速度
	GPIO_Init(GPIOB, &GPIO_InitStructure);              //A or B
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line =EXTI_Line0 | EXTI_Line1;             //线路号
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;              //开启中断
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    //中断模式
	EXTI_InitStructure.EXTI_Trigger =EXTI_Trigger_Falling; //下降触发
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);        //中断分组
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;      //指定通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;           //开启
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;	//响应优先级
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;      //指定通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;           //开启
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;        //响应优先级
	NVIC_Init(&NVIC_InitStruct);
}

int encoder_get(){
	temp=count;
	count=0;
	return temp;
}

void EXTI0_IRQHandler(void){
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0){
		count--;
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}

void EXTI1_IRQHandler(void){
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0){
		count++;
	}
	EXTI_ClearITPendingBit(EXTI_Line1);
}
