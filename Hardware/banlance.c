#include "stm32f10x.h"
#include "balance.h"
#include "pwm.h"

// PWM限制范围
#define PWM_STOP 1500
#define PWM_MAX 1650
#define PWM_MIN 1350
#define ROLL_ANGLE_MAX 30.0f
// 平衡控制参数
static BalanceParams_t balance_params = {0};
float target_roll_angle = 0.0f;

// 初始化平衡控制器
void Balance_Init(float kp, float kd)
{
    balance_params.kp = kp;
    balance_params.kd = kd;
    balance_params.prev_error = 0.0f; // 初始化上一次误差
}

// 平衡控制函数
void Balance_Calculate(float current_roll_angle, float current_roll_rate, int RS, uint16_t pwm[4])
{
    float target_roll_angle = ROLL_ANGLE_MAX * RS / 100.0f;
    float error = target_roll_angle - current_roll_angle;

    // 使用 PD 控制计算补偿值
    float compensation = balance_params.kp * error - balance_params.kd * current_roll_rate;

    // 计算左右推进器 PWM 值
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

    // 设置 PWM 输出
    pwm[1] = left_pwm;
    pwm[2] = right_pwm;

    // 更新上一次误差
    balance_params.prev_error = error;
}
