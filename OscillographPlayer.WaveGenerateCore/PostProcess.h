#pragma once

#include "Defines.h"

#include <stdbool.h>
#include <stdint.h>

//双阈值及孤立弱边缘抑制
bool DoubleThresholdAndConnect(SafeArrayUInt8* grayMap, SafeArrayUInt8* edgeMap, uint8_t highThreshold, uint8_t lowThreshold);
