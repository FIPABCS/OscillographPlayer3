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
#include <stdint.h>

HEAD bool CallingConvertion EdgeDetection(uint8_t* grayImage, uint16_t width, uint16_t height,
	float stepLength, uint8_t lowThreshold, uint8_t highThreshold, uint8_t* edgeArray);