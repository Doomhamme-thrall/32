#include "balance.h"
#include "pwm.h"

// 平衡控制参数
static BalanceParams_t balance_params = {0};

// 初始化平衡控制器
void Balance_Init(float kp, float kd)
{
    balance_params.kp = kp;
    balance_params.kd = kd;
    balance_params.target_value = 0.0f; // 默认目标值为 0
    balance_params.prev_error = 0.0f;   // 初始化上一次误差
}

// 平衡控制函数
void Balance_Control(float current_value, float current_rate, int16_t pwm_output[4])
{
    // 计算误差
    float error = balance_params.target_value - current_value;

    // 使用 PD 控制计算补偿值
    float compensation = balance_params.kp * error - balance_params.kd * current_rate;

    // 计算左右推进器 PWM 值
    int16_t left_pwm = 1500 + (int16_t)compensation;
    int16_t right_pwm = 1500 - (int16_t)compensation;

    // 限制 PWM 范围
    left_pwm = (left_pwm > 2000) ? 2000 : (left_pwm < 1000) ? 1000
                                                            : left_pwm;
    right_pwm = (right_pwm > 2000) ? 2000 : (right_pwm < 1000) ? 1000
                                                               : right_pwm;

    // 设置 PWM 输出
    pwm_output[1] = left_pwm;
    pwm_output[2] = right_pwm;

    // 更新上一次误差
    balance_params.prev_error = error;
}
