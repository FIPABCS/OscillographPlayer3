#pragma once

#include "GaussianBlur.h"
#include "Defines.h"
#include "SafeArray.h"

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <malloc.h>

DefineSafeArray(float, Float)

//高斯模糊权重
static float GaussianWeight(int distance, float sigma)
{
	const float sqrtTwicePi = 2.51f;

	float sigmaSqare = sigma * sigma;
	float distantSqare = (float)distance * distance;

	return 1.0f / (sqrtTwicePi * sigma * expf(1.0f * distantSqare / 2.0f * sigmaSqare));
}

//高斯模糊
bool GaussianBlur(SafeArrayUInt8* grayMap, float sigma)
{
	if (sigma < 1) { sigma = 1.0f; }

	int radius = (int)(sigma * 3),
		diameter = radius * 2 + 1;

	float* weight = (float*)malloc(diameter * sizeof(float));
	if (!weight)
	{
		return false;
	}
	for (int i = 0;i < diameter;i++)
	{
		weight[i] = GaussianWeight(abs(radius - i), sigma);
	}
	float weightSum = 0;
	for (int i = 0;i < diameter;i++)
	{
		weightSum += weight[i];
	}
	float normalization = 1.0f / weightSum;
	for (int i = 0;i < diameter;i++)
	{
		weight[i] *= normalization;
	}

	uint16_t width = grayMap->width;
	uint16_t height = grayMap->height;

	float* horizonWeightArray = (float*)malloc((int64_t)width * height * sizeof(float));
	if (!horizonWeightArray)
	{
		return false;
	}
	SafeArrayFloat horizonWeight = { horizonWeightArray,width,height };

	float tempGray = 0;
	for (uint16_t y = 0; y < height; y++)
	{
		for (uint16_t x = 0; x < width; x++)
		{
			tempGray = 0;
			for (int xTP = 0; xTP < diameter; xTP++)
			{
				tempGray += GetUInt8(grayMap, x - radius + xTP, y) * weight[xTP];
			}

			SetFloat(&horizonWeight, x, y, tempGray);
		}
	}
	for (uint16_t y = 0; y < height; y++)
	{
		for (uint16_t x = 0; x < width; x++)
		{
			tempGray = 0;
			for (int yTP = 0; yTP < diameter; yTP++)
			{
				tempGray += GetFloat(&horizonWeight, x, y - radius + yTP) * weight[yTP];
			}

			if (tempGray < LowGray)
			{
				tempGray = LowGray;
			}
			else if (tempGray > HighGray)
			{
				tempGray = HighGray;
			}

			SetUInt8(grayMap, x, y, (uint8_t)tempGray);
		}
	}

	free(horizonWeightArray);
	horizonWeightArray = NULL;
	free(weight);
	weight = NULL;

	return true;
}