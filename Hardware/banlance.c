#include "stm32f10x.h"
#include "balance.h"
#include "pwm.h"

#define PWM_STOP 1500
#define PWM_MAX 1650
#define PWM_MIN 1350
#define ROLL_ANGLE_MAX 30.0f

static BalanceParams_t balance_params = {0};
float target_roll_angle = 0.0f;

// 平衡初始化函数
// kp: 比例增益
// kd: 微分增益
void Balance_Init(float kp, float kd)
{
    balance_params.kp = kp;
    balance_params.kd = kd;
    balance_params.prev_error = 0.0f; // 初始化上一次误差
}

// 平衡控制函数
// current_roll_angle: 当前ROLL角
// current_roll_rate: 当前ROLL角速度
// RS: 右摇杆Y轴
void Balance_Calculate(float current_roll_angle, float current_roll_rate, uint8_t RS, uint16_t pwm[4])
{
    target_roll_angle = ROLL_ANGLE_MAX * RS / 100.0f;
    float error = target_roll_angle - current_roll_angle;

    // PD计算
    float compensation = balance_params.kp * error - balance_params.kd * current_roll_rate;

    // 左右PWM计算
    uint16_t left_pwm = 1500 + (int16_t)compensation;
    uint16_t right_pwm = 1500 - (int16_t)compensation;

    if (left_pwm > PWM_MAX)
        left_pwm = PWM_MAX;
    if (left_pwm < PWM_MIN)
        left_pwm = PWM_MIN;
    if (right_pwm > PWM_MAX)
        right_pwm = PWM_MAX;
    if (right_pwm < PWM_MIN)
        right_pwm = PWM_MIN;
    pwm[1] = left_pwm;
    pwm[2] = right_pwm;

    balance_params.prev_error = error;
}
