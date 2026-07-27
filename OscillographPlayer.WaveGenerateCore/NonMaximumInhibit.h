#pragma once

#include "Defines.h"

//非极值点抑制
void NonMaximumInhibit(const SafeArrayVect* gradMap, SafeArrayUInt8* grayMap);

//计算灰度梯度大小及方向
void CalculateGradient(const SafeArrayUInt8* grayMap, SafeArrayVect* gradMap);
