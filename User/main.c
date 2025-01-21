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
	rtc_init();
	RTC_SetAlarm(RTC_GetCounter() + 10);
	PWR_WakeUpPinCmd(ENABLE);

	OLED_ShowNum(1, 1, SystemCoreClock, 10);
	while (1)
	{
		OLED_ShowNum(2, 1, RTC_GetCounter(), 10);
		OLED_ShowNum(3, 1, RTC_GetFlagStatus(RTC_FLAG_ALR), 1);

		OLED_ShowString(4, 1, "Hello, World");
		Delay_ms(100);
		OLED_ShowString(4, 1, "            ");
		Delay_ms(100);

		PWR_EnterSTANDBYMode();
		
	}
}
