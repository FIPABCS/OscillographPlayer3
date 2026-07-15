#pragma once

typedef enum Direction
{
	right = 0, leftup, up, rightup, none
} Direction;

typedef struct
{
	unsigned char value;
	Direction direction;
} Vector;

Direction GetDirection(int dX, int dY);
