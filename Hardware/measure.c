#include "stm32f10x.h" // Device header
#include "pwm.h"

#define PWM_MAX 1650
#define PWM_MIN 1350

// 简易控制函数，不使用PID
// 用于测量MAX_SPEED，MAX_YAW_RATE，MAX_ROLL_ANGLE以及均衡推进器的最大推力
// void measure_calculate(uint8_t A, uint8_t B, uint8_t X, uint8_t Y, uint16_t pwm[4])
// {
//     pwm[0] = 1500 + A*50;
//     pwm[1] = 1500 + B*50;
//     pwm[2] = 1500 + X*50;
//     pwm[3] = 1500 + Y*50;
// }

void measure_calculate(uint8_t LS, uint8_t RS, uint8_t LT, uint8_t RT, uint16_t pwm[4])
{
    // 计算推进功率
    int power = (RT - LT);
    int differ = LS - 100;
    int up_dowm = RS - 100;
    // 计算转向控制
    int16_t left_pwm = 1500 + power - differ;
    int16_t right_pwm = 1500 + power + differ;

    // 计算沉浮控制
    int16_t up_pwm = 1500 + up_dowm;
    int16_t down_pwm = 1500 + up_dowm;

    // 限制PWM值范围
    if (left_pwm > PWM_MAX)
        left_pwm = PWM_MAX;
    if (left_pwm < PWM_MIN)
        left_pwm = PWM_MIN;
    if (right_pwm > PWM_MAX)
        right_pwm = PWM_MAX;
    if (right_pwm < PWM_MIN)
        right_pwm = PWM_MIN;
    if (up_pwm > PWM_MAX)
        up_pwm = PWM_MAX;
    if (up_pwm < PWM_MIN)
        up_pwm = PWM_MIN;
    if (down_pwm > PWM_MAX)
        down_pwm = PWM_MAX;
    if (down_pwm < PWM_MIN)
        down_pwm = PWM_MIN;

    pwm[0] = left_pwm;
    pwm[1] = up_pwm;
    pwm[2] = down_pwm;
    pwm[3] = right_pwm;
}