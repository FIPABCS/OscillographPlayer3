#pragma once

#define DefineSafeArray(T,typeName)                                                 \
typedef struct SafeArray##typeName                                                  \
{                                                                                   \
    T* items;                                                                       \
    int width;                                                                      \
    int height;                                                                     \
} SafeArray##typeName;                                                              \
                                                                                    \
static inline T Get##typeName(const SafeArray##typeName *array, int x, int y)       \
{                                                                                   \
    if (x < 0) { x = 0; }                                                           \
    if (x >= array->width) { x = array->width - 1; }                                \
                                                                                    \
    if (y < 0) { y = 0; }                                                           \
    if (y >= array->width) { y = array->width - 1; }                                \
                                                                                    \
    return array->items[y * array->width + x];                                      \
                                                                                    \
}                                                                                   \
                                                                                    \
static inline void Set##typeName(SafeArray##typeName *array, int x, int y, T value) \
{                                                                                   \
    if (x < 0) { x = 0; }                                                           \
    if (x >= array->width) { x = array->width - 1; }                                \
                                                                                    \
    if (y < 0) { y = 0; }                                                           \
    if (y >= array->width) { y = array->width - 1; }                                \
                                                                                    \
    array->items[y * array->width + x] = value;                                     \
                                                                                    \
    return;                                                                         \
}