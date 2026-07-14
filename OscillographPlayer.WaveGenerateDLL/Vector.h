#pragma once

typedef enum Direction
{
	right,leftup,up,rightup,none
} Direction;

typedef struct
{
	unsigned char value;
	Direction direction;
} Vector;

inline Direction GetDirection(int dX, int dY);
