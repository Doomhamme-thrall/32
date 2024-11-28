#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include "string.h"
#include "serial3.h"
#include "pwm.h"

uint8_t RxData;
extern char Serial_RxPacket[];
extern uint8_t Serial_RxFlag;

int main(void)
{
	OLED_Init();
//	OLED_ShowString(1, 1, "PWM:");
//	OLED_ShowString(1, 7, "%");
	Serial_Init();
	serial3_Init();
	pwm_init();
	pwm_setcompare(0);

	while (1)
	{
		if (Serial_RxFlag == 1) // 如果接收到数据包
		{
			OLED_ShowString(4, 1, "                ");
			OLED_ShowString(4, 1, Serial_RxPacket); // OLED清除指定位置，并显示接收到的数据包

			/*将收到的数据包与预设的指令对比，以此决定将要执行的操作*/
			if (strcmp(Serial_RxPacket, "pwm_half") == 0) // 如果收到pwm_half指令
			{
				pwm_setcompare(50);					  // 设置占空比为50
				Serial_SendString("PWM_HALF_OK\r\n"); // 串口回传一个字符串PWM_HALF_OK
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "PWM_HALF_OK"); // OLED清除指定位置，并显示PWM_HALF_OK
			}
			else if (strcmp(Serial_RxPacket, "pwm_full") == 0) // 如果收到pwm_full指令
			{
				pwm_setcompare(100);				  // 设置占空比为100
				Serial_SendString("PWM_FULL_OK\r\n"); // 串口回传一个字符串PWM_FULL_OK
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "PWM_FULL_OK"); // OLED清除指定位置，并显示PWM_FULL_OK
			}
			else if (strcmp(Serial_RxPacket, "pwm_off") == 0) // 如果收到pwm_off指令
			{
				pwm_setcompare(0);					 // 设置占空比为0
				Serial_SendString("PWM_OFF_OK\r\n"); // 串口回传一个字符串PWM_OFF_OK
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "PWM_OFF_OK"); // OLED清除指定位置，并显示PWM_OFF_OK
			}
			else // 上述所有条件均不满足，即收到了未知指令
			{
				Serial_SendString("ERROR_COMMAND\r\n"); // 串口回传一个字符串ERROR_COMMAND
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "ERROR_COMMAND"); // OLED清除指定位置，并显示ERROR_COMMAND
			}

			Serial_RxFlag = 0; // 处理完成后，需要将接收数据包标志位清零，否则将无法接收后续数据包
		}
	}
}