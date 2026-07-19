#pragma once

#include "WaveGenerate.h"
#include "EdgeDetection.h"
#include "SafeArray.h"

#include <stdbool.h>
#include <stdint.h>

DefineSafeArray(uint8_t, UInt8)

HEAD bool CallingConvertion WaveGenerate(uint8_t* edgeArray,int width,int height, int frameRate, int sampleRate, uint16_t* waveArray)
{
	SafeArrayUInt8 edgeMap = { edgeArray,width,height };
}