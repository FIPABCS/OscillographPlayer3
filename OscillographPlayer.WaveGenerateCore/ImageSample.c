#include "ImageSample.h"

#include <stdint.h>

//采样
void Sample(SafeArrayUInt8* grayMap, float stepLength)
{
	if (stepLength < 1) { stepLength = 1; }

	uint16_t width = grayMap->width,
		height = grayMap->height,
		widthNew = (uint16_t)(width / stepLength);
	uint8_t* grayArray = grayMap->items;

	uint16_t xNew = 0, yNew = 0;
	for (int yOld = 0, indexNew = 0; yOld < height; yNew++, yOld = (int)(yNew * stepLength))
	{
		xNew = 0;
		indexNew = widthNew * yNew;
		for (int xOld = 0; xOld < width; xNew++, indexNew++, xOld = (int)(xNew * stepLength))
		{
			grayArray[indexNew] = GetUInt8(grayMap, xOld, yOld);
			//SetUInt8(grayMap, xNew, yNew, GetUInt8(grayMap, xOld, yOld));
		}
	}

	grayMap->width = xNew;
	grayMap->height = yNew;

	return;
}