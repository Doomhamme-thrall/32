#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include "string.h"
#include "pwm.h"
#include "dma.h"
#include "ad.h"
#include "mpu6050.h"
#include "MPU6050_Reg.h"

uint8_t RxData;
// extern char Serial_RxPacket[];
extern uint8_t Serial_RxFlag;
uint8_t id;
int16_t AX, AY, AZ, GX, GY, GZ;

int main(void)
{
	// serial1_init();
	// PWM_Init();
	OLED_Init();

	mpu_init();
	//	OLED_ShowNum(2,2,33,2);

	id = mpu_read(MPU6050_WHO_AM_I);
	OLED_ShowHexNum(1, 1, id, 2);

	while (1)
	{
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ); // 获取MPU6050的数据
		OLED_ShowSignedNum(2, 1, AX, 5);			   // OLED显示数据
		OLED_ShowSignedNum(3, 1, AY, 5);
		OLED_ShowSignedNum(4, 1, AZ, 5);
		OLED_ShowSignedNum(2, 8, GX, 5);
		OLED_ShowSignedNum(3, 8, GY, 5);
		OLED_ShowSignedNum(4, 8, GZ, 5);
		Delay_ms(100);
	}
}