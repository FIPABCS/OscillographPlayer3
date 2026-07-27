#pragma once

#include <stdint.h>

#define DefineSafeArrayReturnBlank(T,typeName,blank)                                    \
typedef struct SafeArray##typeName                                                      \
{                                                                                       \
    T* items;                                                                           \
    uint16_t width;                                                                     \
    uint16_t height;                                                                    \
} SafeArray##typeName;                                                                  \
                                                                                        \
static inline T Get##typeName(const SafeArray##typeName *array, int x, int y)           \
{                                                                                       \
    if(x < 0 || x >= array->width || y < 0 || y >= array->height)                       \
    {                                                                                   \
        return blank;                                                                   \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        return array->items[y * array->width + x];                                      \
    }                                                                                   \
}                                                                                       \
                                                                                        \
static inline void Set##typeName(SafeArray##typeName *array, int x, int y, T value)     \
{                                                                                       \
    if(x < 0 || x >= array->width || y < 0 || y >= array->height)                       \
    {                                                                                   \
        return;                                                                         \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        array->items[y * array->width + x] = value;                                     \
        return;                                                                         \
    }                                                                                   \
}