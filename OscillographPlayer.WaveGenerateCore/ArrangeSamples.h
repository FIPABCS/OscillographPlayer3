#pragma once

#include "PointQueue.h"

typedef enum ArrangeMethod { ByPoint, ByFrame } ArrangeMethods;

void ArrangeSamples(PointQueue* edgePoint, int sampleInFrame, ArrangeMethods arrangeMethod, PointQueue* arrangedPoint);
