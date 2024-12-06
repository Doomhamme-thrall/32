#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include "string.h"
#include "pwm.h"
#include "dma.h"

uint8_t RxData;
// extern char Serial_RxPacket[];
extern uint8_t Serial_RxFlag;

uint8_t dataa[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
uint8_t datab[8] = {0, 0, 0, 0, 0, 0, 0, 0};

int main(void)
{
	serial1_init();
	PWM_Init();
	OLED_Init();

	OLED_ShowHexNum(1, 1, dataa[0], 2);
	OLED_ShowHexNum(1, 3, dataa[1], 2);
	OLED_ShowHexNum(1, 5, dataa[2], 2);
	OLED_ShowHexNum(1, 7, dataa[3], 2);

	OLED_ShowHexNum(2, 1, datab[0], 2);
	OLED_ShowHexNum(2, 3, datab[1], 2);
	OLED_ShowHexNum(2, 5, datab[2], 2);
	OLED_ShowHexNum(2, 7, datab[3], 2);

	dma_init((uint32_t)dataa, (uint32_t)datab, 4);
	dma_transport();

	OLED_ShowHexNum(3, 1, dataa[0], 2);
	OLED_ShowHexNum(3, 3, dataa[1], 2);
	OLED_ShowHexNum(3, 5, dataa[2], 2);
	OLED_ShowHexNum(3, 7, dataa[3], 2);

	OLED_ShowHexNum(4, 1, datab[0], 2);
	OLED_ShowHexNum(4, 3, datab[1], 2);
	OLED_ShowHexNum(4, 5, datab[2], 2);
	OLED_ShowHexNum(4, 7, datab[3], 2);
	while (1)
	{
		if (Serial_GetRxFlag() == 1) // 如果接收到数据包
		{
			PWM_Set(Serial_RxPacket);
		}
	}
}