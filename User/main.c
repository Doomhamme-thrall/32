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

int main(void)
{

	PWM_Init();
	serial1_init();
	serial3_init();
	JY901S_Init();
	Balance_Init(1, 1);
	Motion_init(0, 0, 0, 0, 0, 0);
	uint16_t pwm[] = {1500, 1500, 1500, 1500};
	JY901S_Data_t sensor_data;
	SerialData_t serial3_data;
	while (1)
	{
		JY901S_ProcessDMA();
		JY901S_GetData(&sensor_data);
		Serial_ProcessDMA();
		Serial_GetData(&serial3_data);
		// Balance_Calculate(sensor_data.roll, sensor_data.wy, serial3_data.RS, pwm);
		// Motion_Calculate(sensor_data.ay, sensor_data.wy, serial3_data.RT, serial3_data.LT, serial3_data.LS, pwm);
		measure_calculate(serial3_data.A, serial3_data.B, serial3_data.X, serial3_data.Y, pwm);
		PWM_Set(pwm);

		// 打印手柄数据
		// printf("RT:%d ", serial3_data.RT);
		// printf("LT:%d ", serial3_data.LT);
		// printf("RS:%d ", serial3_data.RS);
		// printf("LS:%d ", serial3_data.LS);
		printf("A:%d ", serial3_data.A);
		printf("B:%d ", serial3_data.B);
		printf("X:%d ", serial3_data.X);
		printf("Y:%d ,", serial3_data.Y);
		//打印传感器数据
		// printf("roll: %f ", sensor_data.roll);
		// printf("pitch: %f ", sensor_data.pitch);
		// printf("yaw: %f ", sensor_data.yaw);
		// printf("ax: %f ", sensor_data.ax);
		// printf("ay: %f ", sensor_data.ay);
		// printf("az: %f ", sensor_data.az);
		// printf("wx: %f ", sensor_data.wx);
		// printf("wy: %f ", sensor_data.wy);
		// printf("wz: %f ", sensor_data.wz);
		
	}
}