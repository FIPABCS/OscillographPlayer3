#include "PostProcess.h"

#include "Point.h"
#include "PointQueue.h"
#include "Defines.h"
#include "SafeArray.h"
#include "EdgeRefineJudge.h"

#include <stdbool.h>
#include <stdint.h>
#include <malloc.h>

DefineSafeArray(bool, Bool)

//双阈值及孤立弱边缘抑制
bool DoubleThresholdAndConnect(SafeArrayUInt8* grayMap, SafeArrayUInt8* edgeMap, uint8_t highThreshold, uint8_t lowThreshold)
{
	uint16_t width = grayMap->width,
		height = grayMap->height;

	PointQueue needCheck;
	uint64_t bufferLength = (uint64_t)(width * height);
	Point* needCheckArray = (Point*)malloc(bufferLength * sizeof(Point));
	if (!needCheckArray)
	{
		return false;
	}
	InitQueue(&needCheck, needCheckArray, bufferLength);

	uint8_t thisGray = LowGray;
	for (uint16_t y = 0;y < height;y++)
	{
		for (uint16_t x = 0;x < width;x++)
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
	SafeArrayBool checked = { checkArray,width,height };

	int xTP[8] = { 0,1,1,1,0,-1,-1,-1 },
		yTP[8] = { 1,1,0,-1,-1,-1,0,1 };

	while (QueueLength(&needCheck))
	{
		Point thisPoint = Dequeue(&needCheck);
		int
			x = thisPoint.x,
			y = thisPoint.y;

		SetUInt8(edgeMap, x, y, HighGray);

		for (int i = 0;i < 8;i++)
		{
			int xCheck = (int)x + xTP[i],
				yCheck = (int)y + yTP[i];

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
			Enqueue(&needCheck, (Point) { (uint16_t)xCheck, (uint16_t)yCheck });
		}
	}

	free(checkArray);
	checkArray = NULL;

	return true;
}

//边缘细化
bool EdgeRefinement(SafeArrayUInt8* edgeMap)
{
	uint16_t
		height = edgeMap->height,
		width = edgeMap->width;
	uint64_t mapArea = (uint64_t)height * width;

	Point* needInhibitPointArray = (Point*)malloc(mapArea * sizeof(Point));
	PointQueue needInhibitPoint;
	InitQueue(&needInhibitPoint, needInhibitPointArray, mapArea);

	bool needInhibit;
	do
	{
		needInhibit = false;

		for (int i = 0;i < 2;i++)
		{
			for (int y = 0;y < height;y++)
			{
				for (int x = 0;x < width;x++)
				{
					if (GetUInt8(edgeMap, x, y) != HighGray) { continue; }

					if (NeedInhibit(edgeMap, x, y, i))
					{
						Enqueue(&needInhibitPoint, (Point) { x, y });
					}
				}
			}
			needInhibit |= (bool)QueueLength(&needInhibitPoint);
			while (QueueLength(&needInhibitPoint))
			{
				Point inhibitPoint = Dequeue(&needInhibitPoint);
				SetUInt8(edgeMap, inhibitPoint.x, inhibitPoint.y, LowGray);
			}
		}
	} while (needInhibit);

	free(needInhibitPointArray);

	return true;
}