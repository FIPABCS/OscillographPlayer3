#include "EdgeDetection.h"
#include "SafeArray.h"

#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

//降噪
static bool NoiseReduction(SafeArrayUchar* grayMap, NoiseReductionCore* core)
{
	int radius = core->radius;
	int diameter = radius * 2 + 1;
	float* weight = core->weight;

	int width = grayMap->width;
	int height = grayMap->height;

	SafeArrayUchar* horizonWeight = (SafeArrayUchar*)malloc(width * height * sizeof(SafeArrayUchar));
	if (!horizonWeight)
	{
		return false;
	}
	
	float tempGray = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			for (int xTP = 0; xTP < diameter; xTP++)
			{
				tempGray += Get(grayMap, x - radius + xTP, y) * weight[xTP];
			}

			Set(horizonWeight, x, y, tempGray);
		}
	}
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			for (int yTP = 0; yTP < diameter; yTP++)
			{
				tempGray += Get(grayMap, x , y - radius + yTP) * weight[yTP];
			}

			Set(grayMap, x, y, tempGray);
		}
	}

	return true;
}

HEAD unsigned char* CallingConvertion EdgeDetection(unsigned char* grayMap, int width, int height,
	float sigma, unsigned char lowThreshold, unsigned char highThreshold)
{

}

