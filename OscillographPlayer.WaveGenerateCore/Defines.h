#pragma once

#include "SafeArray.h"
#include "SafeArrayReturnBlank.h"
#include "Vector.h"

#include <stdint.h>

#define HighGray 255
#define LowGray 0

DefineSafeArray(uint8_t, UInt8)
DefineSafeArray(Vector, Vect)
DefineSafeArrayReturnBlank(uint8_t, UInt8, LowGray)
