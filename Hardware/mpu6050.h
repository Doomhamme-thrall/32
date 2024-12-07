#ifndef __mpu6050_h__
#define __mpu6050_h__

void MPU6050_WaitEvent(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT);
void mpu_init(void);
uint8_t mpu_read(uint8_t address);
void mpu_write(uint8_t address, uint8_t data);
uint8_t MPU6050_GetID(void);
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                     int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);
#endif