// =======================================
// Unit   : 包含所有单元的头文件
// Version: 3.0.0.1 (build 2011.07.08)
// Author : Kyee Ye
// Email  : kyee(at)hopechart.com
// Copyright (C) Hopechart Co., Ltd.
// =======================================

#ifndef _HQLib_H_
#define _HQLib_H_

// HQLib 3.0 中所有的头文件
#include "HQFileBlock.h"

// HQLib 的命名空间
using namespace HQLib;

// 静态函数中由于静态对象初始化的先后顺序问题, 而必须优先初始化的函数
inline void InitHQLib(THandle AHandle = NULL){ InitKYLib(AHandle); }

#endif
