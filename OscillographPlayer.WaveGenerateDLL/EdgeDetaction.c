#include "EdgeDetection.h"
#include "NoiseReductionCore.h"
#include "Vector.h"

#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "SafeArray.h"
DefineSafeArray(unsigned char, UChar)
DefineSafeArray(float, Float)
DefineSafeArray(Vector,	Vect)
DefineSafeArray(bool, Bool)

//降噪
static bool NoiseReduction(SafeArrayUChar* grayMap,const NoiseReductionCore* core)
{
	int radius = core->radius;
	int diameter = radius * 2 + 1;
	float* weight = core->weight;

	int width = grayMap->width;
	int height = grayMap->height;

	SafeArrayFloat* horizonWeight = (SafeArrayFloat*)malloc((int64_t)(width * height) * sizeof(SafeArrayUChar));
	if (!horizonWeight)
	{
		return false;
	}
	horizonWeight->width = width;
	horizonWeight->height = height;

	float tempGray = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			for (int xTP = 0; xTP < diameter; xTP++)
			{
				tempGray += GetUChar(grayMap, x - radius + xTP, y) * weight[xTP];
			}

			if (tempGray < 0)
			{
				tempGray = 0;
			}
			else if(tempGray > 255)
			{
				tempGray = 255;
			}

			SetFloat(horizonWeight, x, y, tempGray);
		}
	}
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			for (int yTP = 0; yTP < diameter; yTP++)
			{
				tempGray += GetFloat(horizonWeight, x , y - radius + yTP) * weight[yTP];
			}

			if (tempGray < 0)
			{
				tempGray = 0;
			}
			else if (tempGray > 255)
			{
				tempGray = 255;
			}

			SetUChar(grayMap, x, y, (unsigned char)tempGray);
		}
	}

	free(horizonWeight);
	horizonWeight = NULL;
	weight = NULL;

	return true;
}

//缩小
static void Shrink(SafeArrayUChar* grayMap, float skip)
{
	int width = grayMap->width;
	int height = grayMap->height;

	int newWidth = 1, newHeight = 1;
	for (int xNew=0,xOld=0; xOld * skip < height; xNew++,xOld=xNew*skip)
	{
		for (int yNew=0,yOld=0; xOld * skip < width; yNew++,yOld=yNew*skip)
		{
			SetUChar(grayMap, xNew, yNew, GetUChar(grayMap, xOld, yOld));
		}
	}

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

	return;
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

HEAD unsigned char* CallingConvertion EdgeDetection(unsigned char* grayMap, int width, int height,
	float sigma, unsigned char lowThreshold, unsigned char highThreshold,
	const NoiseReductionCore* noiseReductionCore)
{

}

