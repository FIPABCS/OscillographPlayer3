#include "EdgeDetection.h"
//Models
#include "Vector.h"
#include "Defines.h"
//Services
#include "GaussianBlur.h"
#include "NonMaximumInhibit.h"
#include "PostProcess.h"
#include "ImageSample.h"

#include <malloc.h>
#include <stdint.h>
#include <stdbool.h>

HEAD bool CallingConvertion EdgeDetection(uint8_t* grayImage, uint16_t width, uint16_t height,
	float stepLength, uint8_t lowThreshold, uint8_t highThreshold, uint8_t* edgeArray)
{
	SafeArrayUInt8 grayMap = { grayImage,width,height };

	if (!GaussianBlur(&grayMap, (float)stepLength / 2.0f))
	{
		return false;
	}

	if (stepLength > 1)
	{
		Sample(&grayMap, stepLength);
		width = grayMap.width;
		height = grayMap.height;
	}

	Vector* gradArray = (Vector*)malloc((int64_t)width * height * sizeof(Vector));
	if (!gradArray)
	{
		return false;
	}
	SafeArrayVect gradMap = { gradArray,width,height };

	CalculateGradient(&grayMap, &gradMap);

	NonMaximumInhibit(&gradMap, &grayMap);

	free(gradArray);
	gradArray = NULL;

	SafeArrayUInt8 edgeMap = { edgeArray,width,height };

	if (!DoubleThresholdAndConnect(&grayMap, &edgeMap, highThreshold, lowThreshold))
	{
		return false;
	}

	if (!EdgeRefinement(&edgeMap))
	{
		return false;
	}

	return true;
}