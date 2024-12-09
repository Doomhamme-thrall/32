#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include "string.h"
#include "pwm.h"
#include "dma.h"
#include "ad.h"
#include "jy901s.h"

uint8_t RxData;
// extern char Serial_RxPacket[];
extern uint8_t Serial_RxFlag;

int main(void)
{
	serial1_init();
	serial3_Init();
	PWM_Init();
	Motion_Init(kp,ki,kd);
	Balance_Init(kp,kd);
	// OLED_Init();

	JY901S_Init();
	//	OLED_ShowNum(2,2,33,2);
	JY901S_Data_t sensor_data;
	while (1)
	{

		JY901S_ProcessDMA();
		JY901S_GetData(&sensor_data);

		Balance_Control(float current_value, float current_rate, int16_t pwm_output[4]);
		Motion_Control(float target_accel, float target_angle, float current_accel, float current_yaw_rate, int16_t pwm_output[4]);
		// 打印传感器数据
		// printf("Ax: %.2f Ay: %.2f Az: %.2f\n", sensor_data.ax, sensor_data.ay, sensor_data.az);
		// printf("Wx: %.2f Wy: %.2f Wz: %.2f\n", sensor_data.wx, sensor_data.wy, sensor_data.wz);
		printf("Roll: %.2f Pitch: %.2f Yaw: %.2f\n", sensor_data.roll, sensor_data.pitch, sensor_data.yaw);
	}
}