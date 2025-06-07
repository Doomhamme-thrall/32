#include "stm32f10x.h"
#include "motion.h"

// PWM 限制范围
#define PWM_STOP 1500
#define PWM_MAX 1650
#define PWM_MIN 1350
#define SPEED_MAX 2.0f
#define YAW_RATE_MAX 180.0f

PID_Params speed_pid = {0};
PID_Params yaw_pid = {0};
float target_speed = 0.0f;
float target_yaw_rate = 0.0f;

static void tim2_init(void);

// 运动控制PID初始化
// kp1:推进PID的比例系数
// ki1:推进PID的积分系数
// kd1:推进PID的微分系数
// kp2:偏航PID的比例系数
// ki2:偏航PID的积分系数
// kd2:偏航PID的微分系数
void Motion_init(float kp1, float ki1, float kd1, float kp2, float ki2, float kd2)
{
    speed_pid.kp = kp1;
    speed_pid.ki = ki1;
    speed_pid.kd = kd1;
    yaw_pid.kp = kp2;
    yaw_pid.ki = ki2;
    yaw_pid.kd = kd2;
    tim2_init();
}

// 运动控制函数
// current_acceleration:当前加速度 由陀螺仪读取
// current_yaw_rate : 当前偏航角速度 由陀螺仪读取
// RT LT LS:右扳机 左扳机 左摇杆
void Motion_Calculate(float current_acceleration, float current_yaw_rate, uint8_t RT, uint8_t LT, uint8_t LS, uint16_t pwm[4])
{
    target_speed = SPEED_MAX * RT / 100.0;
    target_yaw_rate = YAW_RATE_MAX * LS / 100.0;

    static float current_speed = 0.0f;
    static float dt = 0;
    int prev_time = 0;
    dt = (TIM2->CNT - prev_time) / 10000.0f;
    prev_time = TIM2->CNT;

    // 通过加速度积分得到速度
    current_speed += current_acceleration * dt;

    // 速度PID计算
    float speed_error = target_speed - current_speed;
    speed_pid.integral += speed_error * dt;
    float speed_derivative = (speed_error - speed_pid.prev_error) / dt;
    speed_pid.prev_error = speed_error;
    float speed_output = speed_pid.kp * speed_error + speed_pid.ki * speed_pid.integral + speed_pid.kd * speed_derivative;

    // 偏航角速度PID计算
    float yaw_error = target_yaw_rate - current_yaw_rate;
    yaw_pid.integral += yaw_error * dt;
    float yaw_derivative = (yaw_error - yaw_pid.prev_error) / dt;
    yaw_pid.prev_error = yaw_error;
    float yaw_output = yaw_pid.kp * yaw_error + yaw_pid.ki * yaw_pid.integral + yaw_pid.kd * yaw_derivative;

    // 计算左右推进器PWM值
    int16_t left_pwm = PWM_STOP + (int16_t)speed_output - (int16_t)yaw_output;
    int16_t right_pwm = PWM_STOP + (int16_t)speed_output + (int16_t)yaw_output;

    // 限制PWM值范围
    if (left_pwm > PWM_MAX)
        left_pwm = PWM_MAX;
    if (left_pwm < PWM_MIN)
        left_pwm = PWM_MIN;
    if (right_pwm > PWM_MAX)
        right_pwm = PWM_MAX;
    if (right_pwm < PWM_MIN)
        right_pwm = PWM_MIN;

    pwm[0] = left_pwm;
    pwm[3] = right_pwm;
}

// TIM2初始化
// 用于计算dt
static void tim2_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = 71999; // 1ms自增一次
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}