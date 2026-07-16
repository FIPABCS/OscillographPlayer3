#pragma once

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

#ifdef DLL_IMPORT
#define HEAD EXTERNC __declspec(dllimport)
#else
#define HEAD EXTERNC __declspec(dllexport)
#endif

#define CallingConvertion _stdcall

#include <stdbool.h>

HEAD bool* CallingConvertion EdgeDetection(unsigned char* grayImage, int width, int height,
	float stepLength, unsigned char lowThreshold, unsigned char highThreshold);