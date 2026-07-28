#include "StandardOutputArray.h"

#include "Point.h"
#include "PointQueue.h"

#include <stdbool.h>
#include <stdint.h>

//钳制超限数据下由int转换为int16_t
static inline int16_t ConvertToInt16(int num)
{
	if (num < INT16_MIN) { num = INT16_MIN; }
	else if (num > INT16_MAX) { num = INT16_MAX; }

	return (int16_t)num;
}

//标准化处理输出数组
void StandardOutputArray(PointQueue* arrangedPoint, uint16_t width, uint16_t height,
	bool horizontalFlip, bool verticalFlip, int16_t* dataArray)
{
	int
		xMove = width / 2,
		yMove = height / 2,
		horiCoe = horizontalFlip ? -1 : 1,
		veriCoe = verticalFlip ? -1 : 1;

	for (int i = 0;QueueLength(arrangedPoint);)
	{
		Point curtPoint = Dequeue(arrangedPoint);

		dataArray[i] = ConvertToInt16((curtPoint.x - xMove) * horiCoe);
		i++;
		dataArray[i] = ConvertToInt16((curtPoint.y - yMove) * veriCoe);
		i++;
	}

	return;
}