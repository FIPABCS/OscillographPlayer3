#include "SafeArray.h"

inline unsigned char safeGet(SafeArray *array, int x, int y)
{
    x = x >= 0 ? x : 0;
    x = x < array->width ? x : array->width - 1;

    y = y >= 0 ? y : 0;
    y = y < array->height ? y : array->height - 1;

    return array->items[y * array->width + x];

}

inline void safeSet(SafeArray *array, int x, int y, unsigned char value)
{
    x = x >= 0 ? x : 0;
    x = x < array->width ? x : array->width - 1;

    y = y >= 0 ? y : 0;
    y = y < array->height ? y : array->height - 1;

    array->items[y * array->width + x] = value;

    return;
}
