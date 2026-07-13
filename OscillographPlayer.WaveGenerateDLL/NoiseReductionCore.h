#pragma once

//定义降噪核，包括权重数组和直径
typedef struct
{
	float* weight;
	int radius;
} NoiseReductionCore;