#pragma once

#include <stdint.h>
#include "Point.h"

typedef struct PointQueue
{
	uint64_t headIdx, tailIdx, bufferLength;
	Point* items;
} PointQueue;

inline void InitQueue(PointQueue* queue,Point* queueBuffer,uint64_t bufferLength)
{
	queue->headIdx = 0;
	queue->tailIdx = 0;
	queue->bufferLength = bufferLength;
	queue->items = queueBuffer;

	return;
}

inline void Enqueue(PointQueue* queue, Point point)
{
	queue->items[queue->tailIdx] = point;

	if (queue->tailIdx == queue->bufferLength - 1)
	{
		queue->tailIdx = 0;
	}
	else
	{
		queue->tailIdx++;
	}

	return;
}

inline Point Dequeue(PointQueue* queue)
{
	Point point = queue->items[queue->headIdx];

	if (queue->headIdx == queue->bufferLength - 1)
	{
		queue->headIdx = 0;
	}
	else
	{
		queue->headIdx++;
	}

	return point;
}

inline uint64_t QueueLength(const PointQueue* queue)
{
	uint64_t headIdx = queue->headIdx,
		tailIdx = queue->tailIdx;

	if (headIdx <= tailIdx)
	{
		return tailIdx - headIdx;
	}
	else
	{
		return queue->bufferLength - (headIdx - tailIdx);
	}
}