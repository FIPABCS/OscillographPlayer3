#include "WaveGenerate.h"
//Model
#include "Point.h"
#include "PointQueue.h"
#include "Defines.h"
//Service
#include "ArrangeSamples.h"
#include "SortEdgePoint.h"
#include "StandardOutputArray.h"

#include <stdbool.h>
#include <stdint.h>
#include <malloc.h>

HEAD bool CallingConvertion WaveGenerate(uint8_t* edgeArray, uint16_t width, uint16_t height,
	ArrangeMethods arrangeMethod, int sampleInFrame, bool horizontalFlip, bool verticalFlip,
	int16_t* waveArray)
{
	SafeArrayRBUInt8 edgeMap = { edgeArray,width,height };

	uint64_t bufferLength = (uint64_t)width * height;
	Point* edgePointArray = (Point*)malloc(bufferLength * sizeof(Point));
	if (!edgePointArray)
	{
		return false;
	}
	PointQueue edgePoint;
	InitQueue(&edgePoint, edgePointArray, bufferLength);

	Point centerPoint = { (int)width / 2,(int)height / 2 };
	if (!SortEdgePoint(&edgeMap, centerPoint, &edgePoint))
	{
		free(edgePointArray);
		return false;
	}

	if (!QueueLength(&edgePoint))
	{
		Enqueue(&edgePoint, centerPoint);
	}

	int arrangedPointArrayBufferLength = sampleInFrame + 1;
	Point* arrangedPointArray = (Point*)malloc(arrangedPointArrayBufferLength * sizeof(Point));
	if (!arrangedPointArray)
	{
		free(edgePointArray);
		return false;
	}
	PointQueue arrangedPoint;
	InitQueue(&arrangedPoint, arrangedPointArray, arrangedPointArrayBufferLength);

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