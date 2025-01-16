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

uint8_t mid;
uint16_t did;

int main(void)
{
	OLED_Init();

	uint8_t data_write[4] = {0xa1, 0x22, 0x03, 0x04};
	uint8_t data_read[4] = {0};

	w25q64_init();
	w25q64_read_id(&mid, &did);

	OLED_ShowHexNum(1, 1, mid, 2);
	OLED_ShowHexNum(1, 4, did, 4);

	w25q64_sector_erase(0x000000);
	w25q64_page_write(0x000000, data_write, 4);

	w25q64_read(0x000000, data_read, 4);

	OLED_ShowHexNum(2, 1, data_read[0], 2);
	OLED_ShowHexNum(2, 4, data_read[1], 2);
	OLED_ShowHexNum(2, 7, data_read[2], 2);
	OLED_ShowHexNum(2, 10, data_read[3], 2);

	// OLED_ShowHexNum(3, 1, data_write[0], 2);
	// OLED_ShowHexNum(3, 4, data_write[1], 2);
	// OLED_ShowHexNum(3, 7, data_write[2], 2);
	// OLED_ShowHexNum(3, 10, data_write[3], 2);

	while (1)
	{

	}
}