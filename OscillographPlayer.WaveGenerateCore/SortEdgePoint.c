#include "SortEdgePoint.h"
#include "Point.h"
#include "PointQueue.h"
#include "EndpointList.h"
#include "Defines.h"
#include "ContinueLine.h"

#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>

//Debug
#include <stdio.h>
//Debug/

//沿连续线条移动、擦除并记录端点
static bool MoveOnContinueLineAndRecord(SafeArrayUInt8* edgeMap, Point startPoint, EndpointCollection* endpoints)
{
	if (GetUInt8(edgeMap, startPoint.x, startPoint.y) != HighGray) { return false; }

	int
		width = edgeMap->width,
		height = edgeMap->height;
	uint64_t
		mapArea = (uint64_t)width * height;


	Point* lineArray = (Point*)malloc(mapArea * sizeof(Point));
	if (!lineArray) { return false; }

	ContinueLine* line = (ContinueLine*)malloc(sizeof(ContinueLine));
	if (!line) { return false; }
	InitLine(line, lineArray, mapArea);

	EndpointPair curtEndpointPair;
	InitEndpointPair(&curtEndpointPair, line);

	int
		xTP[8] = { -1,0,1,1,1,0,-1,-1 },
		yTP[8] = { 1,1,1,0,-1,-1,-1,0 },
		checkX, checkY;
	Point
		curtPoint = startPoint;
	bool isEndpoint = false;

	//startPoint => headPoint
	while (!isEndpoint)
	{
		isEndpoint = true;

		//加入
		AddLineHead(line, curtPoint);
		SetUInt8(edgeMap, curtPoint.x, curtPoint.y, LowGray);

		//搜索和匹配
		for (int i = 0;i < 8;i++)
		{

			checkX = curtPoint.x + xTP[i];
			checkY = curtPoint.y + yTP[i];

			if (GetUInt8(edgeMap, checkX, checkY) != HighGray)
			{
				continue;
			}

			curtPoint = (Point){ checkX,checkY };

			isEndpoint = false;
			break;
		}
	}
	curtEndpointPair.headEndpoint = curtPoint;

	//startPoint => tailPoint
	curtPoint = startPoint;
	isEndpoint = false;
	while (!isEndpoint)
	{
		isEndpoint = true;

		//搜索和匹配
		for (int i = 7;i >= 0;i--)
		{

			checkX = curtPoint.x + xTP[i];
			checkY = curtPoint.y + yTP[i];

			if (GetUInt8(edgeMap, checkX, checkY) != HighGray)
			{
				continue;
			}

			curtPoint = (Point){ checkX,checkY };

			isEndpoint = false;
			break;
		}

		//加入
		AddLineTail(line, curtPoint);
		SetUInt8(edgeMap, curtPoint.x, curtPoint.y, LowGray);
	}
	curtEndpointPair.tailEndpoint = curtPoint;

	if (!Coincide(curtEndpointPair.headEndpoint, curtEndpointPair.tailEndpoint))
	{
		AddEndpoint(endpoints, curtEndpointPair);
	}

	return true;
}

//搜索最近的端点对并返回指针
static EndpointPair* SearchNearlistEndpointPair(EndpointCollection* endpoints, Point curtPoint)
{
	EndpointPair* nearlistEndpoint = NULL;
	int nearlistLength = INT32_MAX;

	EndpointPair* endpointPairs = endpoints->items;

	for (int i = 0;i < endpoints->count;i++)
	{
		Point checkPoint;
		int checkLength;

		//检查头端点
		checkPoint = endpointPairs[i].headEndpoint;
		checkLength = CityDistant(curtPoint, checkPoint);
		if (nearlistLength > checkLength)
		{
			nearlistLength = checkLength;
			nearlistEndpoint = &endpointPairs[i];
			nearlistEndpoint->direction = headToTail;
		}

		//检查尾端点
		checkPoint = endpointPairs[i].tailEndpoint;
		checkLength = CityDistant(curtPoint, checkPoint);
		if (nearlistLength > checkLength)
		{
			nearlistLength = checkLength;
			nearlistEndpoint = &endpointPairs[i];
			nearlistEndpoint->direction = tailToHead;
		}
	}

	return nearlistEndpoint;
}

bool SortEdgePoint(SafeArrayUInt8* edgeMap, Point startPoint, PointQueue* edgePoint)
{
	int
		width = edgeMap->width,
		height = edgeMap->height;
	uint64_t mapArea = (uint64_t)width * height;

	EndpointPair* endpointArray = (EndpointPair*)malloc(mapArea * sizeof(EndpointPair));
	if (!endpointArray) { return false; }

	EndpointCollection endpoints;
	InitEndpointCollection(&endpoints, endpointArray);

	//分段排序
	for (int y = 0;y < height;y++)
	{
		for (int x = 0;x < width;x++)
		{
			if (GetUInt8(edgeMap, x, y) != HighGray)
			{
				continue;
			}

			if (!MoveOnContinueLineAndRecord(edgeMap, (Point) { x, y }, & endpoints))
			{
				return false;
			}
		}
	}

	//Debug
	/*for (int i = 0;i < endpoints.count;i++)
	{
		printf("%d %d\n", endpoints.items[i].headEndpoint.x, endpoints.items[i].headEndpoint.y);
		printf("%d %d\n", endpoints.items[i].tailEndpoint.x, endpoints.items[i].tailEndpoint.y);
	}*/
	//Debug/

	//整合分段
	Point prevLineLastPoint = startPoint;
	while (endpoints.count)
	{
		EndpointPair* curtEndpointPairPtr = SearchNearlistEndpointPair(&endpoints, prevLineLastPoint);
		EndpointPair curtEndpointPair = *curtEndpointPairPtr;
		ContinueLine curtLine = *curtEndpointPair.line;

		switch (curtEndpointPair.direction)
		{
		case headToTail:
			for (uint64_t i = curtLine.headIdx + 1;i < curtLine.tailIdx;i++)
			{
				Enqueue(edgePoint, curtLine.items[i]);
			}

			prevLineLastPoint = curtEndpointPair.tailEndpoint;

			break;

		case tailToHead:
			for (uint64_t i = curtLine.tailIdx - 1;i > curtLine.headIdx;i--)
			{
				Enqueue(edgePoint, curtLine.items[i]);
			}

			prevLineLastPoint = curtEndpointPair.tailEndpoint;

			break;
		}

		DeletEndpointPair(&endpoints, curtEndpointPairPtr);
	}

	return true;
}