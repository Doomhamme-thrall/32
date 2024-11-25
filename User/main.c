#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "redout.h"
#include "encoder.h"
#include "timer.h"
#include "pwm.h"
#include "ic.h"

int num=1;
int a=1;

int main(void)
{
	pwm_init();
	OLED_Init();
	ic_init();
	pwm_setcompare(75);
	
	while (1)
	{	
		OLED_ShowNum(1,1,ic_getfreq(),5);
		OLED_ShowNum(2,1,ic_getduty(),2);
	}
}

void TIM2_IRQHandler(){
	num++;
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}
