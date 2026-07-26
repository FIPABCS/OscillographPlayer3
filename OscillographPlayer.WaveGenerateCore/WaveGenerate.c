#pragma once

#include "Point.h"
#include "WaveGenerate.h"
#include "PointQueue.h"
#include "ArrangeSamples.h"
#include "SortEdgePoint.h"

#include <stdbool.h>
#include <stdint.h>
#include <malloc.h>

static inline int16_t ConvertToInt16(int num)
{
	if (num < INT16_MIN) { num = INT16_MIN; }
	else if (num > INT16_MAX) { num = INT16_MAX; }

	return (int16_t)num;
}

//标准化输出
static void StandardOutputArray(PointQueue* arrangedPoint, uint16_t width, uint16_t height, 
	bool horizontalFlip,bool verticalFlip, int16_t* dataArray)
{
	int
		xMove = width / 2,
		yMove = height / 2,
		horiCoe = horizontalFlip ? -1 : 1,
		veriCoe = verticalFlip ? -1 : 1;

	for (int i = 0;QueueLength(arrangedPoint);)
	{
		Point curtPoint = Dequeue(arrangedPoint);

		dataArray[i] = ConvertToInt16((curtPoint.x - xMove) * horiCoe);
		i++;
		dataArray[i] = ConvertToInt16((curtPoint.y - yMove) * veriCoe);
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

	Point startPoint = { (int)width / 2,(int)height / 2 };
	if (!SortEdgePoint(&edgeMap, startPoint, &edgePoint))
	{
		free(edgePointArray);
		return false;
	}

	Point* arrangedPointArray = (Point*)malloc(sampleInFrame * sizeof(Point));
	if (!arrangedPointArray) 
	{
		free(edgePointArray);
		return false; 
	}
	PointQueue arrangedPoint;
	InitQueue(&arrangedPoint, arrangedPointArray, sampleInFrame);

	ArrangeSamples(&edgePoint, sampleInFrame, arrangeMethod, &arrangedPoint);

	StandardOutputArray(&arrangedPoint, width, height, horizontalFlip, verticalFlip, waveArray);

	free(edgePointArray);
	free(arrangedPointArray);

	return true;
}

HEAD void CallingConvertion AmplitudeMaximization(int16_t* waveArray, int length)
{
	int16_t maxAmplitude = 0;
	for (int i = 0;i < length;i++)
	{
		int16_t thisAmplitude = waveArray[i];
		if (maxAmplitude < thisAmplitude) { maxAmplitude = thisAmplitude; }
	}

	float maxCoe = (float)INT16_MAX / maxAmplitude;

	for (int i = 0;i < length;i++)
	{
		waveArray[i] = (int16_t)(waveArray[i] * maxCoe);
	}

	return;
}
