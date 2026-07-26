#pragma once

#include "Point.h"
#include "ContinueLine.h"

#include <stdint.h>
#include <malloc.h>

typedef enum ContinueLineDirection { headToTail, tailToHead }LineDirection;

typedef struct EndpointPair
{
	Point headEndpoint, tailEndpoint;
	LineDirection direction;
	ContinueLine* line;
}EndpointPair;

typedef struct EndpointCollection
{
	uint64_t count;
	EndpointPair* items;
}EndpointCollection;

inline void InitEndpointPair(EndpointPair* endpointPair, ContinueLine* line)
{
	endpointPair->headEndpoint = (Point){ 0,0 };
	endpointPair->tailEndpoint = (Point){ 0,0 };
	endpointPair->direction = headToTail;

	endpointPair->line = line;

	return;
}

inline void InitEndpointCollection(EndpointCollection* endpoints, EndpointPair* endpointsArray)
{
	endpoints->count = 0;
	endpoints->items = endpointsArray;
}

inline void AddEndpoint(EndpointCollection* endpoints, EndpointPair endpointPair)
{
	uint64_t* count = &endpoints->count;

	endpoints->items[*count] = endpointPair;
	(*count)++;

	return;
}

inline void DeletEndpointPair(EndpointCollection* endpoints, EndpointPair* endpointPair)
{
	free(endpointPair->line->items);
	free(endpointPair->line);

	uint64_t* count = &endpoints->count;

	(*count)--;

	*endpointPair = endpoints->items[*count];

	return;
}
