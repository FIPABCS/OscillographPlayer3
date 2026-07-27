#include "NonMaximumInhibit.h"

#include "Vector.h"
#include "Defines.h"

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

//确认极值点
static inline bool IsMaximum(const SafeArrayVect* gradMap, int x, int y)
{
	switch (GetVect(gradMap, x, y).direction)
	{
	case none:
		return false;
	case right:
		return GetVect(gradMap, x, y).value > GetVect(gradMap, x + 1, y).value
			&& GetVect(gradMap, x, y).value > GetVect(gradMap, x - 1, y).value;
	case rightup:
		return GetVect(gradMap, x, y).value > GetVect(gradMap, x + 1, y - 1).value
			&& GetVect(gradMap, x, y).value > GetVect(gradMap, x - 1, y + 1).value;
	case up:
		return GetVect(gradMap, x, y).value > GetVect(gradMap, x, y + 1).value
			&& GetVect(gradMap, x, y).value > GetVect(gradMap, x, y - 1).value;
	case leftup:
		return GetVect(gradMap, x, y).value > GetVect(gradMap, x - 1, y - 1).value
			&& GetVect(gradMap, x, y).value > GetVect(gradMap, x + 1, y + 1).value;
	}

	return false;
}

//非极值点抑制
void NonMaximumInhibit(const SafeArrayVect* gradMap, SafeArrayUInt8* grayMap)
{
	uint16_t width = grayMap->width;
	uint16_t height = grayMap->height;

	for (uint16_t y = 0; y < height; y++)
	{
		for (uint16_t x = 0; x < width; x++)
		{
			if (!IsMaximum(gradMap, x, y))
			{
				SetUInt8(grayMap, x, y, LowGray);
			}
			else
			{
				SetUInt8(grayMap, x, y, GetVect(gradMap, x, y).value);
			}
		}
	}

	return;
}

//计算灰度梯度
void CalculateGradient(const SafeArrayUInt8* grayMap, SafeArrayVect* gradMap)
{
	uint16_t width = grayMap->width;
	uint16_t height = grayMap->height;

	int dX = 0, dY = 0;
	Vector thisGradient = { 0,none };
	for (uint16_t y = 0; y < height; y++)
	{
		for (uint16_t x = 0; x < width; x++)
		{
			dX = (GetUInt8(grayMap, x + 1, y + 1) + 2 * GetUInt8(grayMap, x + 1, y) + GetUInt8(grayMap, x + 1, y - 1))
				- (GetUInt8(grayMap, x - 1, y + 1) + 2 * GetUInt8(grayMap, x - 1, y) + GetUInt8(grayMap, x - 1, y - 1));

			dY = (GetUInt8(grayMap, x + 1, y - 1) + 2 * GetUInt8(grayMap, x, y - 1) + GetUInt8(grayMap, x - 1, y - 1))
				- (GetUInt8(grayMap, x + 1, y + 1) + 2 * GetUInt8(grayMap, x, y + 1) + GetUInt8(grayMap, x - 1, y + 1));

			thisGradient.value = (uint8_t)fminf((float)HighGray, sqrtf((float)((dX * dX) + (dY * dY))) / 4.0f);
			thisGradient.direction = GetDirection(dX, dY);

			SetVect(gradMap, x, y, thisGradient);
		}
	}

	return;
}