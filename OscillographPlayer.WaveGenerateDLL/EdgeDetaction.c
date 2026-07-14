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

static void CalculateGradient(SafeArrayUChar* grayMap,SafeArrayVect* gradMap)
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

			thisGradient.value = (unsigned char)(sqrtf((dX * dX) + (dY * dY)) / 4);
			thisGradient.direction = GetDirection(dX, dY);

			SetVect(gradMap, x, y, thisGradient);
		}
	}
}

//HEAD unsigned char* CallingConvertion EdgeDetection(unsigned char* grayMap, int width, int height,
//	float sigma, unsigned char lowThreshold, unsigned char highThreshold)
//{
//
//}

