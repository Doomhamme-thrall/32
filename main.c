#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include "string.h"
#include "pwm.h"

uint8_t RxData;
// extern char Serial_RxPacket[];
extern uint8_t Serial_RxFlag;

int main(void)
{
	serial1_init();
	PWM_Init();
	while (1)
	{
		if (Serial_GetRxFlag() == 1) // 如果接收到数据包
		{
			PWM_Set(Serial_RxPacket);
		}
	}
}