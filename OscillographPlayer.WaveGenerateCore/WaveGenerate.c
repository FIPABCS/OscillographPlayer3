#pragma once

#include "WaveGenerate.h"
#include "SafeArray.h"
#include "PointQueue.h"

#include <stdbool.h>
#include <stdint.h>
#include <malloc.h>

#define HighGray 255
#define LowGray 0

DefineSafeArray(uint8_t, UInt8)
DefineSafeArray(bool, Bool)

//排列边缘像素
static bool SortEdgePoint(SafeArrayUInt8* edgeMap, PointQueue* edgePoint)
{
	int width = edgeMap->width,
		height = edgeMap->height;

	uint64_t bufferLength = (uint64_t)width * height;
	Point* needCheckArray = (Point*)malloc(bufferLength * sizeof(Point));
		 
	if (!needCheckArray)
	{
		return false;
	}
	PointQueue needCheck;
	InitQueue(&needCheck, &needCheckArray, bufferLength);
	

	bool* isCheckedArray = (bool*)calloc((int64_t)width * height, sizeof(bool));
	if (!isCheckedArray)
	{
		return false;
	}
	SafeArrayBool isChecked = { &isChecked,width,height };

	Enqueue(&needCheck, (Point) { width / 2, height / 2 });
	int xTP[4] = { 1,0,-1,0 },
		yTP[4] = { 0,1,0,-1 },
		curtX = 0, curtY = 0,
		checkX = 0, checkY = 0;
	Point curtPoint = { 0,0 };
	while (Length(&needCheck))
	{
		curtPoint = Dequeue(&needCheck);
		curtX = curtPoint.x;
		curtY = curtPoint.y;

		if (GetBool(&isChecked, curtX, curtY))
		{
			continue;
		}
		SetBool(&isChecked, curtX, curtY, true);

		if (GetBool(edgeMap, curtX, curtY) == HighGray)
		{
			Enqueue(&edgePoint, (Point) { curtX, curtY });
		}

		for (int i = 0;i < 4;i++)
		{
			checkX = curtX + xTP[i];
			checkY = curtY + yTP[i];

			if (GetBool(&isChecked, checkX, checkY))
			{
				continue;
			}
			SetBool(&isChecked, checkX, checkY, true);

			Enqueue(&needCheck, (Point) { checkX, checkY });
		}
	}

	return true;
}

HEAD bool CallingConvertion WaveGenerate(uint8_t* edgeArray,int width,int height, int frameRate, int sampleRate, int16_t* waveArray)
{
	SafeArrayUInt8 edgeMap = { edgeArray,width,height };

	uint64_t bufferLength = (uint64_t)width * height;
	Point* edgePointArray = (Point*)malloc(bufferLength * sizeof(Point));
	if (!edgePointArray)
	{
		return false;
	}
	PointQueue edgePoint;
	InitQueue(&edgePoint, &edgePointArray, bufferLength);

	if (!SortEdgePoint(&edgeMap, &edgePoint))
	{
		return false;
	}
}