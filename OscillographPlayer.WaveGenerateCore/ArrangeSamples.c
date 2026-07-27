#include "ArrangeSamples.h"
#include "PointQueue.h"
#include "Point.h"

//排布采样点（按点重复）【此处sampleInFrame为一帧时间内对应的音频采样数量】
static void ArrangeByPoint(PointQueue* edgePoint, int sampleInFrame, PointQueue* arrangedPoint)
{
	float repeatTime = sampleInFrame / (float)QueueLength(edgePoint);

	int outCount = 0;
	Point curtPoint = { 0,0 };
	for (int srcCount = 1;QueueLength(edgePoint) && outCount < sampleInFrame;srcCount++)
	{
		curtPoint = Dequeue(edgePoint);

		for (;outCount < (int)(srcCount * repeatTime) && outCount < sampleInFrame;outCount++)
		{
			Enqueue(arrangedPoint, curtPoint);
		}
	}

	if (outCount < sampleInFrame)
	{
		for (;outCount < sampleInFrame;outCount++)
		{
			Enqueue(arrangedPoint, curtPoint);
		}
	}

	return;
}

//排布采样点（按帧重复）【此处sampleInFrame为一帧时间内对应的音频采样数量】
static void ArrangeByFrame(PointQueue* edgePoint, int sampleInFrame, PointQueue* arrangedPoint)
{
	for (int outCount = 0;outCount < sampleInFrame;outCount++)
	{
		Point curtPoint = Dequeue(edgePoint);

		Enqueue(arrangedPoint, curtPoint);

		Enqueue(edgePoint, curtPoint);
	}

	return;
}

//排布采样点（汇总方法）
void ArrangeSamples(PointQueue* edgePoint, int sampleInFrame, ArrangeMethods arrangeMethod, PointQueue* arrangedPoint)
{
	static void (*arrangeSamples)(PointQueue * edgePoint, int sampleInFrame, PointQueue * arrangedPoint);
	switch (arrangeMethod)
	{
	case ByPoint:
		arrangeSamples = ArrangeByPoint;
		break;
	case ByFrame:
		arrangeSamples = ArrangeByFrame;
		break;
	}

	arrangeSamples(edgePoint, sampleInFrame, arrangedPoint);

	return;
}