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
		Balance_Calculate(sensor_data.roll, sensor_data.wy, serial3_data.RS, pwm);
		Motion_Calculate(sensor_data.ay, sensor_data.wy, serial3_data.RT, serial3_data.LT, serial3_data.LS, pwm);
		PWM_Set(pwm);

		// 打印手柄数据
		// printf("%d ", serial3_data.RT);
		// printf("%d ", serial3_data.LT);
		// printf("%d ", serial3_data.RS);
		// printf("%d ", serial3_data.LS);
		// printf("%d ", serial3_data.A);
		// printf("%d ", serial3_data.B);
		// printf("%d ", serial3_data.X);
		// printf("%d ,", serial3_data.Y);
		
	}
}