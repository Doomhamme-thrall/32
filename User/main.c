#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include "string.h"
#include "pwm.h"

uint8_t RxData;
extern char Serial_RxPacket[];
extern uint8_t Serial_RxFlag;

int main(void)
{	
	PWM_Init();
	// Delay_s(2);
	//	pwm_setcompare1(1450)
	while (1)
	{
		//		if (Serial_GetRxFlag() == 1)
		//		{
		//			OLED_ShowHexNum(4, 1, Serial_RxPacket[0], 2);
		//			OLED_ShowHexNum(4, 4, Serial_RxPacket[1], 2);
		//			OLED_ShowHexNum(4, 7, Serial_RxPacket[2], 2);
		//			OLED_ShowHexNum(4, 10, Serial_RxPacket[3], 2);
		//		}
	}
}