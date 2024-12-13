#include "stm32f10x.h" // Device header

// 简易控制函数，不使用PID
// 用于测量MAX_SPEED，MAX_YAW_RATE，MAX_ROLL_ANGLE等
void measure_calculate(uint8_t A, uint8_t B, uint8_t X, uint8_t Y, uint16_t pwm[4])
{
    pwm[0] = 1500 * A;
    pwm[1] = 1500 * B;
    pwm[2] = 1500 * X;
    pwm[3] = 1500 * Y;
}