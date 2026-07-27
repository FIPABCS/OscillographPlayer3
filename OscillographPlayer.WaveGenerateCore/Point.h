#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

typedef struct Point { int x;int y; } Point;

inline bool Coincide(Point point1, Point point2)
{
	return point1.x == point2.x && point1.y == point2.y;
}

inline uint16_t CityDistant(Point point1, Point point2)
{
	return (uint16_t)(abs((int)point1.x - (int)point2.x) + abs((int)point1.y - point2.y));
}