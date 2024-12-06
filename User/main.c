#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include "string.h"
#include "pwm.h"
#include "dma.h"
#include "ad.h"

uint8_t RxData;
// extern char Serial_RxPacket[];
extern uint8_t Serial_RxFlag;

int main(void)
{
	// serial1_init();
	// PWM_Init();
	OLED_Init();
	ad_init();

	while (1)
	{
		OLED_ShowNum(1, 1, ad_value[0], 4);

		Delay_ms(100);
	}
}