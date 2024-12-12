#ifndef BALANCE_H
#define BALANCE_H

#include "stm32f10x.h"

// 平衡控制参数结构
typedef struct
{
    float kp;                // 比例增益
    float kd;                // 微分增益
    float prev_error;        // 上一次误差
} BalanceParams_t;

// 初始化平衡控制器
void Balance_Init(float kp, float kd);

// 平衡控制函数
void Balance_Calculate(float current_roll_angle, float current_roll_rate, int RS, uint16_t pwm[4]);

#endif // BALANCE_H
