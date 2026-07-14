#pragma once

typedef enum Direction
{
	right,rightdown,down,leftdown,none
} Direction;

typedef struct
{
	unsigned char value;
	Direction direction;
} Vector;
