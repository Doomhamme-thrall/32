#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "redout.h"
#include "encoder.h"
#include "timer.h"
#include "pwm.h"
#include "ic.h"
#include "serial.h"
#include "stdio.h"
#include "string.h"

int main(void)
{
	OLED_Init();
	LED_Init();
	serial_init();
	pwm_init();
	Delay_s(2);
	pwm_setcompare(1000);
	while (1)
	{
//		if (flag == 1)
//		{
//			
//			OLED_ShowString(4, 1, rxpack);
//			
//			if(strcmp(rxpack,"LED_ON")==0){
//				LED1_ON();
//			}
//			if(strcmp(rxpack,"LED_OFF")==0){
//				LED1_OFF();
//			}
//			flag=0;
//		}
	}
}

void TIM2_IRQHandler()
{

	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
