#pragma once

#include "Point.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct ContinueLine
{
	uint64_t headIdx, tailIdx, bufferLength;
	uint8_t connectEndpointNum;
	Point* items;
} ContinueLine;

inline void InitLine(ContinueLine* line,Point* linePoints,uint64_t bufferLength)
{
	uint64_t middleIdx = (uint64_t)(bufferLength / 2);
	line->headIdx = middleIdx - 1;
	line->tailIdx = middleIdx;
	line->bufferLength = bufferLength;
	line->connectEndpointNum = 0;
	line->items = linePoints;

	return;
}

inline bool AddLineHead(ContinueLine* line, Point point)
{
	if (line->headIdx <= 0)
	{
		return false;
	}
	line->items[line->headIdx] = point;
	line->headIdx--;

	return true;
}

inline bool AddLineTail(ContinueLine* line, Point point)
{
	if (line->tailIdx >= line->bufferLength - 1)
	{
		return false;
	}
	line->items[line->tailIdx] = point;
	line->tailIdx++;
	
	return true;
}

inline uint64_t LineLength(const ContinueLine* line)
{
	return line->tailIdx - line->headIdx;
}