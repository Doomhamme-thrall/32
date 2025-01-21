#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "serial.h"
#include "LED.h"
#include "string.h"
#include "pwm.h"
#include "dma.h"
#include "ad.h"
#include "jy901s.h"
#include "balance.h"
#include "motion.h"
#include "w25q64.h"
#include "rtc.h"

SerialData_t serial_data;

int main(void)
{
	OLED_Init();
	if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET)
	{
		OLED_ShowString(1, 1, "WWDG Reset");
		Delay_ms(500);
		OLED_ShowString(1, 1, "          ");
		RCC_ClearFlag();
	}
	else
	{
		OLED_ShowString(2, 1, "Reset");
		Delay_ms(500);
		OLED_ShowString(2, 1, "     ");
	}

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	WWDG_SetWindowValue(0x40 | 0x21);
	WWDG_Enable(0x40 | 54);

	while (1)
	{

		Delay_ms(45);
		WWDG_SetCounter(0x50 | 54);
	}
}
