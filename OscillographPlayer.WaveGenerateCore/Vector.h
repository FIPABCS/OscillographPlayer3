#pragma once
#include <stdint.h>

typedef enum Direction
{
	right = 0, leftup, up, rightup, none
} Direction;

typedef struct Vector
{
	uint8_t value;
	Direction direction;
} Vector;

Direction GetDirection(int dX, int dY);
