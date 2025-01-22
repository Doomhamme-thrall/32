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
#include "flash.h"

int main(void)
{
	OLED_Init();
	OLED_ShowHexNum(2, 1, flash_read_half_word(0x1FFFF7E0), 4);
	OLED_ShowHexNum(3, 1, flash_read_half_word(0x1FFFF7EB), 4);
	while (1)
	{

		// printf("1");
	}
}
