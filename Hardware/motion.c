#include "motion.h"
#include "pwm.h"

// 运动控制 PID 参数
static PID_t accel_pid = {0};
static PID_t yaw_pid = {0};

// 初始化运动控制器
void Motion_Init(float kp, float ki, float kd)
{
    accel_pid.kp = kp;
    accel_pid.ki = ki;
    accel_pid.kd = kd;
    accel_pid.target_value = 0.0f;
    accel_pid.prev_error = 0.0f;
    accel_pid.integral = 0.0f;

    yaw_pid.kp = kp;
    yaw_pid.ki = ki;
    yaw_pid.kd = kd;
    yaw_pid.target_value = 0.0f;
    yaw_pid.prev_error = 0.0f;
    yaw_pid.integral = 0.0f;
}

// PID 计算函数
static float PID_Calculate(PID_t *pid, float current_value)
{
    float error = pid->target_value - current_value;

    // 积分和微分计算
    pid->integral += error;
    float derivative = error - pid->prev_error;

    // PID 输出
    float output = pid->kp * error + pid->ki * pid->integral + pid->kd * derivative;

    // 更新上一次误差
    pid->prev_error = error;

    return output;
}

// 运动控制函数
void Motion_Control(float target_accel, float target_angle, float current_accel, float current_yaw_rate, int16_t pwm_output[4])
{
    // 设置目标值
    accel_pid.target_value = target_accel;
    yaw_pid.target_value = target_angle;

    // 计算加速度和方向输出
    float accel_output = PID_Calculate(&accel_pid, current_accel);
    float yaw_output = PID_Calculate(&yaw_pid, current_yaw_rate);

    // 基础 PWM 值
    int16_t base_pwm = 1500 + (int16_t)accel_output;

    // 差速计算
    int16_t diff_pwm = (int16_t)yaw_output;

    // 计算左右推进器 PWM
    int16_t left_pwm = base_pwm - diff_pwm;
    int16_t right_pwm = base_pwm + diff_pwm;

    // 限制 PWM 范围
    left_pwm = (left_pwm > 2000) ? 2000 : (left_pwm < 1000) ? 1000: left_pwm;
    right_pwm = (right_pwm > 2000) ? 2000 : (right_pwm < 1000) ? 1000: right_pwm;

    // 设置 PWM 输出
    pwm_output[0] = left_pwm;
    pwm_output[3] = right_pwm;
}
