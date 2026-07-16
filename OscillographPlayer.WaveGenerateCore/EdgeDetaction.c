#include "EdgeDetection.h"
#include "Vector.h"
#include "SafeArray.h"

#include <malloc.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

DefineSafeArray(unsigned char, UChar)
DefineSafeArray(float, Float)
DefineSafeArray(Vector,	Vect)
DefineSafeArray(bool, Bool)

//降噪（高斯模糊）
//static bool NoiseReductionGaussian(SafeArrayUChar* grayMap,const NoiseReductionCore* core)
//{
//	int radius = core->radius;
//	int diameter = radius * 2 + 1;
//	float* weight = core->weight;
//
//	int width = grayMap->width;
//	int height = grayMap->height;
//
//	SafeArrayFloat* horizonWeight = (SafeArrayFloat*)malloc((int64_t)(width * height) * sizeof(SafeArrayUChar));
//	if (!horizonWeight)
//	{
//		return false;
//	}
//	horizonWeight->width = width;
//	horizonWeight->height = height;
//
//	float tempGray = 0;
//	for (int y = 0; y < height; y++)
//	{
//		for (int x = 0; x < width; x++)
//		{
//			for (int xTP = 0; xTP < diameter; xTP++)
//			{
//				tempGray += GetUChar(grayMap, x - radius + xTP, y) * weight[xTP];
//			}
//
//			if (tempGray < 0)
//			{
//				tempGray = 0;
//			}
//			else if(tempGray > 255)
//			{
//				tempGray = 255;
//			}
//
//			SetFloat(horizonWeight, x, y, tempGray);
//		}
//	}
//	for (int y = 0; y < height; y++)
//	{
//		for (int x = 0; x < width; x++)
//		{
//			for (int yTP = 0; yTP < diameter; yTP++)
//			{
//				tempGray += GetFloat(horizonWeight, x , y - radius + yTP) * weight[yTP];
//			}
//
//			if (tempGray < 0)
//			{
//				tempGray = 0;
//			}
//			else if (tempGray > 255)
//			{
//				tempGray = 255;
//			}
//
//			SetUChar(grayMap, x, y, (unsigned char)tempGray);
//		}
//	}
//
//	free(horizonWeight);
//	horizonWeight = NULL;
//	weight = NULL;
//
//	return true;
//}

//降噪（三角模糊）
static bool NoiseReductionTriangle(SafeArrayUChar* grayMap, int radius)
{
	int width = grayMap->width;
	int height = grayMap->height;

	const int maxRadius = 2895;
	radius = radius <= maxRadius ? radius : maxRadius;
	int maxPixelNum = (radius + 1) * (radius + 1);

	int weightSum = 0, outSum = 0, inSum = 0;
	unsigned char* horizonWeightArray = (unsigned char*)malloc((int64_t)(width * height) * sizeof(unsigned char));
	if (!horizonWeightArray)
	{
		return false;
	}

	SafeArrayUChar horizonWeight = { horizonWeightArray,width,height };
	for (int y = 0; y < height; y++)
	{
		weightSum = outSum = inSum = 0;
		for (int xTP = -radius; xTP <= radius; xTP++)
		{
			weightSum += GetUChar(grayMap,xTP,y) * (radius - abs(xTP) + 1);
			if (xTP <= 0)
			{
				outSum += GetUChar(grayMap, xTP, y);
			}
			else
			{
				inSum += GetUChar(grayMap, xTP, y);
			}
		}

		for (int x = 0; x < width; x++)
		{
			SetUChar(&horizonWeight, x, y, weightSum / maxPixelNum);

			inSum += GetUChar(grayMap, x + radius + 1, y);

			weightSum -= outSum;
			weightSum += inSum;

			outSum += GetUChar(grayMap, x + 1, y);

			outSum -= GetUChar(grayMap, x - radius, y);
			inSum -= GetUChar(grayMap, x + 1, y);
		}
	}

	for (int x = 0; x < width; x++)
	{
		weightSum = outSum = inSum = 0;
		for (int yTP = -radius; yTP <= radius; yTP++)
		{
			weightSum += GetUChar(&horizonWeight, x, yTP) * (radius - abs(yTP) + 1);
			if (yTP <= 0)
			{
				outSum += GetUChar(&horizonWeight, x, yTP);
			}
			else
			{
				inSum += GetUChar(&horizonWeight, x, yTP);
			}
		}

		for (int y = 0; y < height; y++)
		{
			SetUChar(grayMap, x, y, weightSum / maxPixelNum);

			inSum += GetUChar(&horizonWeight, x, y + radius + 1);

			weightSum -= outSum;
			weightSum += inSum;

			outSum += GetUChar(&horizonWeight, x, y + 1);

			outSum -= GetUChar(&horizonWeight, x, y - radius);
			inSum -= GetUChar(&horizonWeight, x, y + 1);
		}
	}

	free(horizonWeightArray);
	horizonWeightArray = NULL;

	return true;
}

//缩小
static void Shrink(SafeArrayUChar* grayMap, float stepLength)
{
	int width = grayMap->width;
	int height = grayMap->height;

	int xNew = 0, yNew = 0;
	for (int yOld = 0; yOld * stepLength < height; yNew++)
	{
		yOld = (int)(yNew * stepLength);
		xNew = 0;

		for (int xOld = 0; xOld * stepLength < width; xNew++)
		{
			xOld = (int)(xNew * stepLength);

			SetUChar(grayMap, xNew, yNew, GetUChar(grayMap, xOld, yOld));
		}
	}

	grayMap->width = xNew + 1;
	grayMap->height = yNew + 1;

	return;
}

//计算灰度梯度
static void CalculateGradient(const SafeArrayUChar* grayMap,SafeArrayVect* gradMap)
{
	int width = grayMap->width;
	int height = grayMap->height;

	int dX = 0, dY = 0;
	Vector thisGradient = { 0,none };
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			dX = (GetUChar(grayMap, x + 1, y + 1) + 2 * GetUChar(grayMap, x + 1, y) + GetUChar(grayMap, x + 1, y - 1))
				- (GetUChar(grayMap, x - 1, y + 1) + 2 * GetUChar(grayMap, x - 1, y) + GetUChar(grayMap, x - 1, y - 1));

			dY = (GetUChar(grayMap, x + 1, y + 1) + 2 * GetUChar(grayMap, x, y + 1) + GetUChar(grayMap, x - 1, y + 1))
				- (GetUChar(grayMap, x + 1, y - 1) + 2 * GetUChar(grayMap, x, y - 1) + GetUChar(grayMap, x - 1, y - 1));

			thisGradient.value = (unsigned char)(sqrtf((float)((dX * dX) + (dY * dY))) / 4);
			thisGradient.direction = GetDirection(dX, dY);

			SetVect(gradMap, x, y, thisGradient);
		}
	}

	return;
}

//确认极值点
static inline bool IsMaximum(const SafeArrayVect* gradMap, int x, int y)
{
	switch (GetVect(gradMap, x, y).direction)
	{
	case none:
		return false;
	case right:
		return GetVect(gradMap, x, y).value >= GetVect(gradMap, x + 1, y).value
			&& GetVect(gradMap, x, y).value >= GetVect(gradMap, x - 1, y).value;
	case rightup:
		return GetVect(gradMap, x, y).value >= GetVect(gradMap, x + 1, y - 1).value
			&& GetVect(gradMap, x, y).value >= GetVect(gradMap, x - 1, y + 1).value;
	case up:
		return GetVect(gradMap, x, y).value >= GetVect(gradMap, x, y + 1).value
			&& GetVect(gradMap, x, y).value >= GetVect(gradMap, x, y - 1).value;
	case leftup:
		return GetVect(gradMap, x, y).value >= GetVect(gradMap, x - 1, y - 1).value
			&& GetVect(gradMap, x, y).value >= GetVect(gradMap, x + 1, y + 1).value;
	}

	return none;
}

//非极值点抑制
static void NonMaximumInhibit(const SafeArrayVect* gradMap, SafeArrayUChar* grayMap)
{
	int width = grayMap->width;
	int height = grayMap->height;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (!IsMaximum(gradMap, x, y))
			{
				SetUChar(grayMap, x, y, 0);
			}
		}
	}

	return;
}

//双阈值及非孤立弱边缘抑制
static void Threshold(const SafeArrayUChar* grayMap, SafeArrayBool* edgeMap, unsigned char highThreshold, unsigned char lowThreshold)
{
	int width = grayMap->width;
	int height = grayMap->height;

	unsigned char thisGray = 0;
	unsigned char xTP[8] = { 0,1,1,1,0,-1,-1,-1 };
	unsigned char yTP[8] = { 1,1,0,-1,-1,-1,0,1 };
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			thisGray = GetUChar(grayMap, x, y);

			if (thisGray >= highThreshold) { SetBool(edgeMap, x, y, true); }
			else if (thisGray <= lowThreshold) { SetBool(edgeMap, x, y, true); }
			else
			{
				for (int i = 0; i < 8; i++)
				{
					if (GetUChar(grayMap, x + xTP[i], y + yTP[i]) >= highThreshold)
					{
						SetBool(edgeMap, x, y, false);
					}
					else
					{
						SetBool(edgeMap, x, y, true);
					}
				}
			}
		}
	}

	return;
}

HEAD bool* CallingConvertion EdgeDetection(unsigned char* grayImage, int width, int height,
	float stepLength, unsigned char lowThreshold, unsigned char highThreshold)
{
	//float sigma = shrinkRate / 2;

	SafeArrayUChar grayMap = { grayImage,width,height };

	if (!NoiseReductionTriangle(&grayMap, (int)(stepLength * 3 / 2)))
	{
		return NULL;
	}

	Shrink(&grayMap, stepLength);

	Vector* gradArray = (Vector*)malloc((int64_t)(width * height) * sizeof(Vector));
	if (!gradArray)
	{
		return NULL;
	}
	SafeArrayVect gradMap = { gradArray,width,height };

	CalculateGradient(&grayMap, &gradMap);

	NonMaximumInhibit(&gradMap, &grayMap);

	free(gradArray);
	gradArray = NULL;

	bool* edgeArray = (bool*)malloc((int64_t)(width * height) * sizeof(bool));
	if (!edgeArray)
	{
		return NULL;
	}
	SafeArrayBool edgeMap = { edgeArray,width,height };

	Threshold(&grayMap, &edgeMap, highThreshold, lowThreshold);

	return edgeArray;
}