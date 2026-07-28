#pragma once

#include "Defines.h"

#include <stdbool.h>
#include <stdint.h>

static bool guoHallJudgeEven[256] = {
	true, true, true, true, true, true, true, false, true, true, false, false, true, true,
	false, false, true, true, true, true, true, true, true, true, true, true, false, false,
	false, true, false, false, true, true, true, true, true, true, true, true, false, true,
	false, false, false, true, false, false, true, true, true, true, true, true, true, true,
	false, true, false, false, false, true, false, false, true, true, true, true, true, true,
	true, true, true, true, true, true, true, true, true, true, true, true, true, true,
	true, true, true, true, true, true, true, true, true, true, true, true, true, true,
	true, true, true, true, true, true, false, true, false, true, false, true, false, true,
	false, true, true, true, true, true, true, true, false, true, false, true, false, true,
	false, true, true, true, true, false, true, true, true, false, true, true, true, false,
	true, true, true, false, true, true, true, true, true, true, true, true, true, true,
	true, false, true, true, true, false, true, true, true, true, true, true, true, true,
	true, true, true, true, true, true, true, true, true, true, true, true, true, true,
	true, true, true, true, true, true, true, true, true, true, true, true, true, true,
	true, true, true, true, true, true, true, true, true, true, true, true, true, true,
	true, true, true, true, true, true, true, true, true, true, true, true, true, true,
	true, true, true, true, true, true, true, true, true, true, true, true, true, true,
	true, true, true, true, true, true, true, true, true, true, true, true, true, true,
	true, true, true, true };

static bool guoHallJudgeOdd[256] = {
	true, true, true, true, true, true, true, false, true, true, true, true, true, true,
	true, true, true, true, true, true, true, true, true, true, true, true, true, true,
	true, true, true, true, true, true, true, true, true, true, true, true, true, true,
	true, true, true, true, true, true, true, true, true, true, true, true, true, true,
	false, true, true, true, true, true, true, true, true, true, true, true, true, true,
	true, true, true, true, true, true, true, true, true, true, true, true, true, true,
	true, true, true, true, true, true, true, true, true, true, true, true, true, true,
	true, true, true, true, true, true, true, true, true, true, true, true, true, true,
	false, true, true, true, true, true, true, true, false, true, true, true, true, true,
	true, true, true, true, false, false, true, true, false, false, true, true, true, true,
	true, true, true, true, true, true, true, true, true, true, true, true, true, true,
	true, true, true, true, true, true, false, false, false, false, true, true, false, false,
	true, true, true, true, true, true, true, true, false, false, false, false, true, true,
	true, true, false, false, true, true, true, true, true, true, true, false, false, false,
	true, true, false, false, true, true, true, true, true, true, true, true, true, true,
	true, true, true, true, true, true, true, true, true, true, true, true, true, true,
	false, false, false, false, true, true, false, false, true, true, true, true, true, true,
	true, true, false, false, false, false, true, true, true, true, false, false, true, true,
	true, true, true, true };

inline bool NeedInhibit(SafeArrayUInt8* edgeMap, int x, int y, int iterCount)
{
	if (GetUInt8(edgeMap, x, y) != HighGray) { return false; }

	int
		xTP[8] = { -1,0,1,1,1,0,-1,-1 },
		yTP[8] = { 1,1,1,0,-1,-1,-1,0 };
	uint8_t	neighborVal = 0;
	bool buffer;

	for (int i = 0;i < 8;i++)
	{
		buffer = GetUInt8(edgeMap, x + xTP[i], y + yTP[i]) == HighGray ? true : false;

		neighborVal |= (buffer << i);
	}

	return !(iterCount & 1 ? guoHallJudgeOdd[neighborVal] : guoHallJudgeEven[neighborVal]);
}
