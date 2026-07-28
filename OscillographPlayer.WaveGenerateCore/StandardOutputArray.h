#pragma once

#include "PointQueue.h"

#include <stdint.h>
#include <stdbool.h>

//标准化处理输出数组
void StandardOutputArray(PointQueue* arrangedPoint, uint16_t width, uint16_t height,
	bool horizontalFlip, bool verticalFlip, int16_t* dataArray);
