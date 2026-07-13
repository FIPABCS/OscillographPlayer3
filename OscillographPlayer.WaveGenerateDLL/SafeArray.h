#pragma once

#define Get(array,x,y) safe_get(&(array), x, y)
#define Set(array,x,y,value) safeSet(&(array), x, y)

typedef struct
{
	unsigned char* items;
	int width;
	int height;
} SafeArray;

inline unsigned char safeGet(SafeArray *array, int x, int y);

inline void safeSet(SafeArray *array, int x, int y, unsigned char value);


