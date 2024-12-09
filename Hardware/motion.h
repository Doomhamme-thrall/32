#ifndef MOTION_H
#define MOTION_H

#include "stm32f10x.h"

// PID 参数结构
typedef struct
{
    float target_value; // 目标值
    float kp;           // 比例增益
    float ki;           // 积分增益
    float kd;           // 微分增益
    float prev_error;   // 上一次误差
    float integral;     // 累积误差
} PID_t;

// 初始化运动控制器
void Motion_Init(float kp, float ki, float kd);

// 运动控制函数
void Motion_Control(float target_accel, float target_angle, float current_accel, float current_yaw_rate, int16_t pwm_output[4]);

#endif // MOTION_H
