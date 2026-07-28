#pragma once

#include "SafeArrayReturnBlank.h"
#include "Point.h"
#include "PointQueue.h"
#include "Defines.h"

#include <stdint.h>
#include <stdbool.h>

bool SortEdgePoint(SafeArrayRBUInt8* edgeMap, Point startPoint, PointQueue* edgePoint);