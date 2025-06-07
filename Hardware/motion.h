#ifndef MOTION_H
#define MOTION_H

#include "stm32f10x.h"

typedef struct
{
    float kp;         // 比例增益
    float ki;         // 积分增益
    float kd;         // 微分增益
    float prev_error; // 当前误差
    float integral;   // 积分项
} PID_Params;

void Motion_init(float kp1, float ki1, float kd1, float kp2, float ki2, float kd2);

void Motion_Calculate(float current_acceleration, float current_yaw_rate, uint8_t RT, uint8_t LT, uint8_t LS, uint16_t pwm[4]);

#endif // MOTION_H
