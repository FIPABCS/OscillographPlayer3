#pragma once

//定义权重数组和模糊直径为降噪核
typedef struct
{
	float* weight;
	int radius;
} NoiseReductionCore;