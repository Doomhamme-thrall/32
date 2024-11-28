#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include "string.h"
#include "serial3.h"
#include "pwm.h"

uint8_t RxData;

int main(void)
{
	OLED_Init();
	OLED_ShowString(1, 1, "PWM:");
	OLED_ShowString(1, 7, "%");
	Serial_Init();
	serial3_Init();
	pwm_init();

	Serial_SendByte('a');

	while (1)
	{
		if (Serial_GetRxFlag() == 1)
		{
			RxData = Serial_GetRxData();
			Serial_SendByte(RxData);
			int duty = (int)RxData;
			OLED_ShowHexNum(1, 5, duty, 2);
			pwm_setcompare(duty);
		}
	}
}
