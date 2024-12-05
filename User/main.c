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
	while (1)
	{
	}
}