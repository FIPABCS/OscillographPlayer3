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
	uint16_t width = edgeMap->width,
		height = edgeMap->height;

	uint64_t bufferLength = (uint64_t)width * height;
	Point* needCheckArray = (Point*)malloc(bufferLength * sizeof(Point));
	if (!needCheckArray)
	{
		return false;
	}
	PointQueue needCheck;
	InitQueue(&needCheck, needCheckArray, bufferLength);

	bool* isCheckedArray = (bool*)calloc((int64_t)width * height, sizeof(bool));
	if (!isCheckedArray)
	{
		free(needCheckArray);
		return false;
	}
	SafeArrayBool isChecked = { isCheckedArray,width,height };

	Enqueue(&needCheck, (Point) { width / 2, height / 2 });
	int xTP[4] = { 1,0,-1,0 },
		yTP[4] = { 0,1,0,-1 };
	Point curtPoint = { 0,0 };
	while (Length(&needCheck))
	{
		curtPoint = Dequeue(&needCheck);
		uint16_t curtX = curtPoint.x,
			     curtY = curtPoint.y;

		if (GetBool(&isChecked, curtX, curtY))
		{
			continue;
		}
		SetBool(&isChecked, curtX, curtY, true);

		if (GetUInt8(edgeMap, curtX, curtY) == HighGray)
		{
			Enqueue(edgePoint, (Point) { curtX, curtY });
		}

		for (int i = 0;i < 4;i++)
		{
			int checkX = (int)curtX + xTP[i];
			int checkY = (int)curtY + yTP[i];

			if (GetBool(&isChecked, checkX, checkY))
			{
				continue;
			}
			SetBool(&isChecked, checkX, checkY, true);

			Enqueue(&needCheck, (Point) { (uint16_t)checkX, (uint16_t)checkY });
		}
	}

	free(isCheckedArray);
	free(needCheckArray);

	return true;
}

//排布采样点（按点重复）【此处sampleInFrame为一帧时间内对应的音频采样数量，一个坐标占用两个采样点】
static void ArrangeByPoint(PointQueue* edgePoint, int sampleInFrame, uint16_t* arrangedArray)
{
	float repetTime = sampleInFrame / (Length(edgePoint) * 2.0f);
	
	int arrIdx = 0;
	for (int cycleTime = 1; arrIdx < sampleInFrame && Length(edgePoint) > 0;cycleTime++)
	{
		Point curtPoint = Dequeue(edgePoint);
		while (arrIdx <= (int)((float)cycleTime * repetTime) && arrIdx < sampleInFrame)
		{
			arrangedArray[arrIdx] = curtPoint.x;
			arrIdx++;
			arrangedArray[arrIdx] = curtPoint.y;
			arrIdx++;
		}
	}

	if (arrIdx < sampleInFrame)
	{
		int lastIdx = arrIdx;
		while (arrIdx < sampleInFrame)
		{
			arrangedArray[arrIdx] = arrangedArray[lastIdx - 1];
			arrIdx++;
			arrangedArray[arrIdx] = arrangedArray[lastIdx];
			arrIdx++;
		}
	}

	return;
}

//排布采样点（按帧重复）【此处sampleInFrame为一帧时间内对应的音频采样数量，一个坐标占用两个采样点】
static void ArrangeByFrame(PointQueue* edgePoint, int sampleInFrame, uint16_t* arrangedArray)
{
	for (int arrIdx = 0; arrIdx < sampleInFrame;)
	{
		Point curtPoint = Dequeue(edgePoint);

		arrangedArray[arrIdx] = curtPoint.x;
		arrIdx++;
		arrangedArray[arrIdx] = curtPoint.y;
		arrIdx++;

		Enqueue(edgePoint, curtPoint);
	}

	return;
}


//排布采样点（汇总方法）
static void ArrangeSamples(PointQueue* edgePoint, int sampleInFrame, ArrangeMethods arrangeMethod, uint16_t* arrangedArray)
{
	static void (*arrangeSamples)(PointQueue * edgePoint, int sampleInFrame, uint16_t * arrangedArray);
	switch (arrangeMethod)
	{
	case ByPoint:
		arrangeSamples = ArrangeByPoint;
		break;
	case ByFrame:
		arrangeSamples = ArrangeByFrame;
		break;
	}

	arrangeSamples(edgePoint, sampleInFrame, arrangedArray);

	return;
}

//标准化输出
static void StandardOutputArray(uint16_t* arrangedArray, int length, uint16_t width, uint16_t height, bool horizontalFlip,bool verticalFlip, int16_t* outputArray)
{
	int xMove = width / 2,
		yMove = height / 2;

	int16_t horiCoe = horizontalFlip ? -1 : 1,
			veriCoe = verticalFlip ? -1 : 1;

	for (int i = 0;i < length;)
	{
		int temp = (int)arrangedArray[i] - xMove;
		outputArray[i] = horiCoe * (int16_t)temp;
		i++;
		temp = (int)arrangedArray[i] - yMove;
		outputArray[i] = veriCoe * (int16_t)temp;	
		i++;
	}

	return;
}

HEAD bool CallingConvertion WaveGenerate(uint8_t* edgeArray, uint16_t width, uint16_t height,
	ArrangeMethods arrangeMethod, int sampleInFrame, bool horizontalFlip, bool verticalFlip,
	int16_t* waveArray)
{
	SafeArrayUInt8 edgeMap = { edgeArray,width,height };

	uint64_t bufferLength = (uint64_t)width * height;
	Point* edgePointArray = (Point*)malloc(bufferLength * sizeof(Point));
	if (!edgePointArray)
	{
		return false;
	}
	PointQueue edgePoint;
	InitQueue(&edgePoint, edgePointArray, bufferLength);

	if (!SortEdgePoint(&edgeMap, &edgePoint))
	{
		free(edgePointArray);
		return false;
	}

	uint16_t* arrangedArray = (uint16_t*)malloc(sampleInFrame * sizeof(uint16_t));
	if (!arrangedArray)
	{
		free(edgePointArray);
		return false;
	}

	ArrangeSamples(&edgePoint, sampleInFrame, arrangeMethod, arrangedArray);

	StandardOutputArray(arrangedArray, sampleInFrame, width, height, horizontalFlip, verticalFlip, waveArray);

	free(edgePointArray);
	free(arrangedArray);

	return true;
}
