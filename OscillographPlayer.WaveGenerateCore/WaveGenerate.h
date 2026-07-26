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

#include "ArrangePoints.h"

HEAD bool CallingConvertion WaveGenerate(uint8_t* edgeArray, uint16_t width, uint16_t height, 
	ArrangeMethods arrangeMethod, int sampleInFrame, bool horizontalFlip, bool verticalFlip,
	int16_t* waveArray);

HEAD void CallingConvertion AmplitudeMaximization(int16_t* waveArray, int length);