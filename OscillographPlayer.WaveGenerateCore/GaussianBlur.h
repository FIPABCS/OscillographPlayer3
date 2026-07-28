#pragma once

#include "SafeArray.h"
#include "Defines.h"

#include <stdint.h>
#include <stdbool.h>

//高斯模糊
bool GaussianBlur(SafeArrayUInt8* grayMap, float sigma);
