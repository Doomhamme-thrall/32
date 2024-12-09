#ifndef JY901S_H
#define JY901S_H

#include "stm32f10x.h"

// 数据结构定义
typedef struct
{
	float ax, ay, az;		// 加速度 (g)
	float wx, wy, wz;		// 角速度 (°/s)
	float roll, pitch, yaw; // 角度 (°)
} JY901S_Data_t;

void JY901S_Init(void);
void JY901S_ProcessDMA(void);
void JY901S_GetData(JY901S_Data_t *data);

#endif // JY901S_H
