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

//定义降噪核，包括权重数组和直径
typedef struct
{
	float* weight;
	float diameter;
} NoiseReductionCore;

HEAD unsigned char* CallingConvertion EdgeDetection(unsigned char *grayMap, int width, int height,
	float sigma, unsigned char lowThreshold, unsigned char highThreshold,
	NoiseReductionCore* noiseReductionCore);
