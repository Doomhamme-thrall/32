#include "stm32f10x.h"                  // Device header
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


int main(void)
{
	OLED_Init();
	serial_init();
	while (1)
	{
	}
}

void TIM2_IRQHandler(){
	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}
