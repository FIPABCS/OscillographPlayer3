#include "EdgeDetection.h"
#include "Vector.h"
#include "SafeArray.h"
#include "PointQueue.h"

#include <malloc.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

//Debug
#include <stdio.h>
//Debug/

#define LowGray 0
#define HighGray 255

DefineSafeArray(uint8_t, UInt8)
DefineSafeArray(float, Float)
DefineSafeArray(Vector,	Vect)
DefineSafeArray(bool, Bool)

//高斯模糊权重
static float GaussianWeight(int distance, float sigma)
{
	const float sqrtTwicePi = 2.51f;

	float sigmaSqare = sigma * sigma;
	float distantSqare = (float)distance * distance;

	return 1.0f / (sqrtTwicePi * sigma * expf(1.0f * distantSqare / 2.0f * sigmaSqare));
}

//降噪（高斯模糊）
static bool NoiseReductionGaussian(SafeArrayUInt8* grayMap, float sigma)
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

	int width = grayMap->width;
	int height = grayMap->height;

	float* horizonWeightArray = (float*)malloc((int64_t)width * height * sizeof(float));
	if (!horizonWeightArray)
	{
		return false;
	}
	SafeArrayFloat horizonWeight = { horizonWeightArray,width,height };

	float tempGray = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			tempGray = 0;
			for (int xTP = 0; xTP < diameter; xTP++)
			{
				tempGray += GetUInt8(grayMap, x - radius + xTP, y) * weight[xTP];
			}

			SetFloat(&horizonWeight, x, y, tempGray);
		}
	}
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			tempGray = 0;
			for (int yTP = 0; yTP < diameter; yTP++)
			{
				tempGray += GetFloat(&horizonWeight, x , y - radius + yTP) * weight[yTP];
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

//降噪（三角模糊）
//static bool NoiseReductionTriangle(SafeArrayUInt8* grayMap, int radius)
//{
//	int width = grayMap->width;
//	int height = grayMap->height;
//
//	const int maxRadius = 2895;
//	radius = radius <= maxRadius ? radius : maxRadius;
//	int maxPixelNum = (radius + 1) * (radius + 1);
//
//	int weightSum = 0, outSum = 0, inSum = 0;
//	float* horizonWeightArray = (float*)malloc((int64_t)width * height * sizeof(float));
//	if (!horizonWeightArray)
//	{
//		return false;
//	}
//
//	SafeArrayFloat horizonWeight = { horizonWeightArray,width,height };
//	for (int y = 0; y < height; y++)
//	{
//		weightSum = outSum = inSum = 0;
//		for (int xTP = -radius; xTP <= radius; xTP++)
//		{
//			weightSum += GetUInt8(grayMap,xTP,y) * (radius - abs(xTP) + 1);
//			if (xTP <= 0)
//			{
//				outSum += GetUInt8(grayMap, xTP, y);
//			}
//			else
//			{
//				inSum += GetUInt8(grayMap, xTP, y);
//			}
//		}
//
//		for (int x = 0; x < width; x++)
//		{
//			SetFloat(&horizonWeight, x, y, (float)weightSum / (float)maxPixelNum);
//
//			inSum += GetUInt8(grayMap, x + radius + 1, y);
//
//			weightSum -= outSum;
//			weightSum += inSum;
//
//			outSum += GetUInt8(grayMap, x + 1, y);
//
//			outSum -= GetUInt8(grayMap, x - radius, y);
//			inSum -= GetUInt8(grayMap, x + 1, y);
//		}
//	}
//
//	for (int x = 0; x < width; x++)
//	{
//		weightSum = outSum = inSum = 0;
//		for (int yTP = -radius; yTP <= radius; yTP++)
//		{
//			weightSum += (int)GetFloat(&horizonWeight, x, yTP) * (radius - abs(yTP) + 1);
//			if (yTP <= 0)
//			{
//				outSum += (int)GetFloat(&horizonWeight, x, yTP);
//			}
//			else
//			{
//				inSum += (int)GetFloat(&horizonWeight, x, yTP);
//			}
//		}
//
//		for (int y = 0; y < height; y++)
//		{
//			SetUInt8(grayMap, x, y, (uint8_t)(weightSum / maxPixelNum));
//
//			inSum += (int)GetFloat(&horizonWeight, x, y + radius + 1);
//
//			weightSum -= outSum;
//			weightSum += inSum;
//
//			outSum += (int)GetFloat(&horizonWeight, x, y + 1);
//
//			outSum -= (int)GetFloat(&horizonWeight, x, y - radius);
//			inSum -= (int)GetFloat(&horizonWeight, x, y + 1);
//		}
//	}
//
//	free(horizonWeightArray);
//	horizonWeightArray = NULL;
//
//	return true;
//}

//采样
static void Sample(SafeArrayUInt8* grayMap, float stepLength)
{
	if (stepLength < 1) { stepLength = 1; }

	int width = grayMap->width,
		height = grayMap->height,
		widthNew = (int)(width / stepLength);
	uint8_t* grayArray = grayMap->items;

	int xNew = 0, yNew = 0;
	for (int yOld = 0,indexNew=0; yOld < height; yNew++, yOld = (int)(yNew * stepLength))
	{
		xNew = 0;
		indexNew = widthNew * yNew;
		for (int xOld = 0; xOld < width; xNew++,indexNew++, xOld = (int)(xNew * stepLength))
		{
			grayArray[indexNew] = GetUInt8(grayMap, xOld, yOld);
			//SetUInt8(grayMap, xNew, yNew, GetUInt8(grayMap, xOld, yOld));
		}
	}

	grayMap->width = xNew;
	grayMap->height = yNew;

	return;
}

//计算灰度梯度
static void CalculateGradient(const SafeArrayUInt8* grayMap,SafeArrayVect* gradMap)
{
	int width = grayMap->width;
	int height = grayMap->height;

	int dX = 0, dY = 0;
	Vector thisGradient = { 0,none };
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			dX = (GetUInt8(grayMap, x + 1, y + 1) + 2 * GetUInt8(grayMap, x + 1, y) + GetUInt8(grayMap, x + 1, y - 1))
				- (GetUInt8(grayMap, x - 1, y + 1) + 2 * GetUInt8(grayMap, x - 1, y) + GetUInt8(grayMap, x - 1, y - 1));

			dY = (GetUInt8(grayMap, x + 1, y - 1) + 2 * GetUInt8(grayMap, x, y - 1) + GetUInt8(grayMap, x - 1, y - 1))
				- (GetUInt8(grayMap, x + 1, y + 1) + 2 * GetUInt8(grayMap, x, y + 1) + GetUInt8(grayMap, x - 1, y + 1));

			thisGradient.value = (uint8_t)fminf((float)HighGray, sqrtf((float)((dX * dX) + (dY * dY))) / 4.0f);
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
		return GetVect(gradMap, x, y).value > GetVect(gradMap, x + 1, y).value
			&& GetVect(gradMap, x, y).value > GetVect(gradMap, x - 1, y).value;
	case rightup:
		return GetVect(gradMap, x, y).value > GetVect(gradMap, x + 1, y - 1).value
			&& GetVect(gradMap, x, y).value > GetVect(gradMap, x - 1, y + 1).value;
	case up:
		return GetVect(gradMap, x, y).value > GetVect(gradMap, x, y + 1).value
			&& GetVect(gradMap, x, y).value > GetVect(gradMap, x, y - 1).value;
	case leftup:
		return GetVect(gradMap, x, y).value > GetVect(gradMap, x - 1, y - 1).value
			&& GetVect(gradMap, x, y).value > GetVect(gradMap, x + 1, y + 1).value;
	}

	return false;
}

//非极值点抑制
static void NonMaximumInhibit(const SafeArrayVect* gradMap, SafeArrayUInt8* grayMap)
{
	int width = grayMap->width;
	int height = grayMap->height;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (!IsMaximum(gradMap, x, y))
			{
				SetUInt8(grayMap, x, y, LowGray);
			}
			else
			{
				SetUInt8(grayMap, x, y, GetVect(gradMap, x, y).value);
			}
		}
	}

	return;
}

//双阈值及孤立弱边缘抑制
static bool DoubleThresholdAndConnect(SafeArrayUInt8* grayMap,SafeArrayUInt8* edgeMap, uint8_t highThreshold, uint8_t lowThreshold)
{
	int width = grayMap->width,
		height = grayMap->height;
	
	PointQueue needCheck;
	uint64_t bufferLength = (uint64_t)(width * height);
	Point* needCheckArray = (Point*)malloc(bufferLength * sizeof(Point));
	if(!needCheckArray)
	{
		return false;
	}
	InitQueue(&needCheck, needCheckArray, bufferLength);

	int thisGray = LowGray;
	for (int y = 0;y < height;y++)
	{
		for (int x = 0;x < width;x++)
		{
			thisGray = GetUInt8(grayMap, x, y);
			if (thisGray < lowThreshold)
			{
				SetUInt8(grayMap, x, y, LowGray);
			}
			else if (thisGray > highThreshold)
			{
				SetUInt8(grayMap, x, y, HighGray);
				Enqueue(&needCheck, (Point) { x, y });
			}
		}
	}

	bool* checkArray = (bool*)calloc((int64_t)width * height, sizeof(bool));
	if (!checkArray)
	{
		return false;
	}
	SafeArrayBool checked={ checkArray,width,height };

	int xTP[8] = { 0,1,1,1,0,-1,-1,-1 },
		yTP[8] = { 1,1,0,-1,-1,-1,0,1 };

	while (Length(&needCheck))
	{
		Point thisPoint = Dequeue(&needCheck);
		int x = thisPoint.x, 
			y = thisPoint.y;

		SetUInt8(edgeMap, x, y, HighGray);

		for (int i = 0;i < 8;i++)
		{
			int xCheck = x + xTP[i],
				yCheck = y + yTP[i];

			uint8_t checkGray = GetUInt8(grayMap, xCheck, yCheck);

			if (GetBool(&checked, xCheck, yCheck))
			{
				continue;
			}
			SetBool(&checked, xCheck, yCheck, true);

			if (checkGray == LowGray)
			{
				SetUInt8(edgeMap, xCheck, yCheck, LowGray);
				continue;
			}
			Enqueue(&needCheck, (Point) { xCheck, yCheck });
		}
	}

	free(checkArray);
	checkArray = NULL;

	return true;
}

//双阈值
//static void Threshold(SafeArrayUInt8* grayMap, uint8_t highThreshold, uint8_t lowThreshold)
//{
//	int width = grayMap->width;
//	int height = grayMap->height;
//
//	uint8_t thisGray = LowGray;
//	
//	for (int y = 0; y < height; y++)
//	{
//		for (int x = 0; x < width; x++)
//		{
//			thisGray = GetUInt8(grayMap, x, y);
//
//			if (thisGray >= highThreshold) { SetUInt8(grayMap, x, y, HighGray); }
//			else if (thisGray <= lowThreshold) { SetUInt8(grayMap, x, y, LowGray); }
//		}
//	}
//
//	return;
//}
//
////非孤立弱边缘抑制
//static void IndependentWeakEdgeInhibit(const SafeArrayUInt8* grayMap, SafeArrayUInt8* edgeMap)
//{
//	int width = grayMap->width;
//	int height = grayMap->height;
//
//	int xTP[8] = { 0,1,1,1,0,-1,-1,-1 };
//	int yTP[8] = { 1,1,0,-1,-1,-1,0,1 };
//	int isUsableWeakEdge = false;
//	uint8_t thisGray = LowGray;
//	for (int y = 0; y < height; y++)
//	{
//		for (int x = 0; x < width; x++)
//		{
//			thisGray = GetUInt8(grayMap, x, y);
//			if(thisGray!=LowGray&&thisGray!=HighGray)
//			{
//				isUsableWeakEdge = false;
//				for (int i = 0; i < 8; i++)
//				{
//					if (GetUInt8(grayMap, x + xTP[i], y + yTP[i]) == HighGray)
//					{
//						isUsableWeakEdge = true;
//						break;
//					}
//				}
//				SetUInt8(edgeMap, x, y, isUsableWeakEdge ? HighGray : LowGray);
//			}
//			else
//			{
//				SetUInt8(edgeMap, x, y, thisGray);
//			}
//		}
//	}
//}

HEAD void CallingConvertion EdgeDetection(uint8_t* grayImage, int width, int height,
	float stepLength, uint8_t lowThreshold, uint8_t highThreshold, uint8_t* edgeArray)
{
	SafeArrayUInt8 grayMap = { grayImage,width,height };

	if (!NoiseReductionGaussian(&grayMap, (float)stepLength / 2.0f))
	{
		edgeArray = NULL;
		return;
	}

	if(stepLength > 1)
	{
		Sample(&grayMap, stepLength);
		width = grayMap.width;
		height = grayMap.height;
	}

	Vector* gradArray = (Vector*)malloc((int64_t)width * height * sizeof(Vector));
	if (!gradArray)
	{
		edgeArray = NULL;
		return;
	}
	SafeArrayVect gradMap = { gradArray,width,height };

	CalculateGradient(&grayMap, &gradMap);

	NonMaximumInhibit(&gradMap, &grayMap);

	free(gradArray);
	gradArray = NULL;

	SafeArrayUInt8 edgeMap = { edgeArray,width,height };

	if (!DoubleThresholdAndConnect(&grayMap, &edgeMap, highThreshold, lowThreshold))
	{
		edgeArray = NULL;
		return;
	}
	
	return;
}