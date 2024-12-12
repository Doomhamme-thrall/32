#ifndef MOTION_H
#define MOTION_H

#include "stm32f10x.h"

typedef struct
{
    float kp;
    float ki;
    float kd;
    float prev_error;
    float integral;
} PID_Params;

// 运动控制PID初始化
//  参数:
//    kp1 - 推进PID的比例系数
//    ki1 - 推进PID的积分系数
//    kd1 - 推进PID的微分系数
//    kp2 - 偏航PID的比例系数
//    ki2 - 偏航PID的积分系数
//    kd2 - 偏航PID的微分系数
void Motion_init(float kp1, float ki1, float kd1, float kp2, float ki2, float kd2);

// 运动控制函数
// 参数：前进功率(0-100)，后退功率(0-100)，转向幅度(-100~100)
void Motion_Calculate(float current_acceleration, float current_yaw_rate, int RT, int LT, int LS, uint16_t pwm[4]);

#endif // MOTION_H
