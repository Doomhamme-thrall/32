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

int main(void)
{
	// PWM_Init();
	// OLED_Init();
	serial1_init();
	// serial3_init();
	JY901S_Init();
	//	OLED_ShowNum(2,2,33,2);
	JY901S_Data_t sensor_data;
	SerialData_t serial3_data;

	while (1)
	{
		// JY901S_ProcessDMA();
		// JY901S_GetData(&sensor_data);
		Serial_ProcessDMA();
		Serial_GetData(&serial3_data);
		// printf("Value1: %.2f Value2: %.2f Value3: %.2f Value4: %.2f\n", serial3_data.value1, serial3_data.value2, serial3_data.value3, serial3_data.value4);
		Delay_s(5);
		
		// 打印传感器数据
		// printf("Ax: %.2f Ay: %.2f Az: %.2f\n", sensor_data.ax, sensor_data.ay, sensor_data.az);
		// printf("Wx: %.2f Wy: %.2f Wz: %.2f\n", sensor_data.wx, sensor_data.wy, sensor_data.wz);
		// printf("Roll: %.2f Pitch: %.2f Yaw: %.2f\n", sensor_data.roll, sensor_data.pitch, sensor_data.yaw);
	}
}