#include "stm32f10x.h" // Device header
#include "MPU6050_Reg.h"

void MPU6050_WaitEvent(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT)
{
    uint32_t Timeout;
    Timeout = 10000;                                   // 给定超时计数时间
    while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS) // 循环等待指定事件
    {
        Timeout--;        // 等待时，计数值自减
        if (Timeout == 0) // 自减到0后，等待超时
        {
            /*超时的错误处理代码，可以添加到此处*/
            break; // 跳出等待，不等了
        }
    }
}

void mpu_write(uint8_t RegAddress, uint8_t Data)
{
    I2C_GenerateSTART(I2C2, ENABLE);                       // 硬件I2C生成起始条件
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); // 等待EV5

    I2C_Send7bitAddress(I2C2, 0xD0, I2C_Direction_Transmitter);          // 硬件I2C发送从机地址，方向为发送
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); // 等待EV6

    I2C_SendData(I2C2, RegAddress);                              // 硬件I2C发送寄存器地址
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING); // 等待EV8

    I2C_SendData(I2C2, Data);                                   // 硬件I2C发送数据
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // 等待EV8_2

    I2C_GenerateSTOP(I2C2, ENABLE); // 硬件I2C生成终止条件
}

uint8_t mpu_read(uint8_t RegAddress)
{
    uint8_t Data;

    I2C_GenerateSTART(I2C2, ENABLE);                       // 硬件I2C生成起始条件
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); // 等待EV5

    I2C_Send7bitAddress(I2C2, 0xD0, I2C_Direction_Transmitter);          // 硬件I2C发送从机地址，方向为发送
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); // 等待EV6

    I2C_SendData(I2C2, RegAddress);                             // 硬件I2C发送寄存器地址
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // 等待EV8_2

    I2C_GenerateSTART(I2C2, ENABLE);                       // 硬件I2C生成重复起始条件
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); // 等待EV5

    I2C_Send7bitAddress(I2C2, 0xD0, I2C_Direction_Receiver);          // 硬件I2C发送从机地址，方向为接收
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED); // 等待EV6

    I2C_AcknowledgeConfig(I2C2, DISABLE); // 在接收最后一个字节之前提前将应答失能
    I2C_GenerateSTOP(I2C2, ENABLE);       // 在接收最后一个字节之前提前申请停止条件

    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED); // 等待EV7
    Data = I2C_ReceiveData(I2C2);                            // 接收数据寄存器

    I2C_AcknowledgeConfig(I2C2, ENABLE); // 将应答恢复为使能，为了不影响后续可能产生的读取多字节操作

    return Data;
}

void mpu_init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;                                // i2c模式
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;                        // 快速模式下占空比
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;                                 // 作为从机时的地址
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;                               // 是否应答
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // 作为从机响应几位地址
    I2C_InitStructure.I2C_ClockSpeed = 50000;                                // 时钟速度
    I2C_Init(I2C2, &I2C_InitStructure);
    I2C_Cmd(I2C2, ENABLE);

    mpu_write(MPU6050_PWR_MGMT_1, 0x01);   // 电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
    mpu_write(MPU6050_PWR_MGMT_2, 0x00);   // 电源管理寄存器2，保持默认值0，所有轴均不待机
    mpu_write(MPU6050_SMPLRT_DIV, 0x09);   // 采样率分频寄存器，配置采样率
    mpu_write(MPU6050_CONFIG, 0x06);       // 配置寄存器，配置DLPF
    mpu_write(MPU6050_GYRO_CONFIG, 0x18);  // 陀螺仪配置寄存器，选择满量程为±2000°/s
    mpu_write(MPU6050_ACCEL_CONFIG, 0x18); // 加速度计配置寄存器，选择满量程为±16g
}

void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                     int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
    uint8_t DataH, DataL; // 定义数据高8位和低8位的变量

    DataH = mpu_read(MPU6050_ACCEL_XOUT_H); // 读取加速度计X轴的高8位数据
    DataL = mpu_read(MPU6050_ACCEL_XOUT_L); // 读取加速度计X轴的低8位数据
    *AccX = (DataH << 8) | DataL;           // 数据拼接，通过输出参数返回

    DataH = mpu_read(MPU6050_ACCEL_YOUT_H); // 读取加速度计Y轴的高8位数据
    DataL = mpu_read(MPU6050_ACCEL_YOUT_L); // 读取加速度计Y轴的低8位数据
    *AccY = (DataH << 8) | DataL;           // 数据拼接，通过输出参数返回

    DataH = mpu_read(MPU6050_ACCEL_ZOUT_H); // 读取加速度计Z轴的高8位数据
    DataL = mpu_read(MPU6050_ACCEL_ZOUT_L); // 读取加速度计Z轴的低8位数据
    *AccZ = (DataH << 8) | DataL;           // 数据拼接，通过输出参数返回

    DataH = mpu_read(MPU6050_GYRO_XOUT_H); // 读取陀螺仪X轴的高8位数据
    DataL = mpu_read(MPU6050_GYRO_XOUT_L); // 读取陀螺仪X轴的低8位数据
    *GyroX = (DataH << 8) | DataL;         // 数据拼接，通过输出参数返回

    DataH = mpu_read(MPU6050_GYRO_YOUT_H); // 读取陀螺仪Y轴的高8位数据
    DataL = mpu_read(MPU6050_GYRO_YOUT_L); // 读取陀螺仪Y轴的低8位数据
    *GyroY = (DataH << 8) | DataL;         // 数据拼接，通过输出参数返回

    DataH = mpu_read(MPU6050_GYRO_ZOUT_H); // 读取陀螺仪Z轴的高8位数据
    DataL = mpu_read(MPU6050_GYRO_ZOUT_L); // 读取陀螺仪Z轴的低8位数据
    *GyroZ = (DataH << 8) | DataL;         // 数据拼接，通过输出参数返回
}