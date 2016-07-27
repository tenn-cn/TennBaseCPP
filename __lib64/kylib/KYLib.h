// =======================================
// Unit   : 包含所有单元的头文件
// Version: 3.1.0.1 (build 2014.12.14)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYLib_H_
#define _KYLib_H_

// KYLib 3.0 中所有的头文件
#include "KYAsynchRun.h"
#include "KYCache.h"
#include "KYCallFunc.h"
#include "KYCodeUtils.h"
#include "KYCycBuffer.h"
#include "KYDebug.h"
#include "KYDebugObj.h"
#include "KYFixedPack.h"
#include "KYIndexFile.h"
#include "KYIniFiles.h"
#include "KYLevelTree.h"
#include "KYLogObjs.h"
#include "KYMatch.h"
#include "KYQueueFile.h"
#include "KYQueueFileEx.h"
#include "KYQueueMap.h"
#include "KYQuickPack.h"
#include "KYRecvBuffer.h"
#include "KYScanUtils.h"
#include "KYSmartObjs.h"
#include "KYTable.h"
#include "KYTCPObj.h"
#include "KYTimer.h"
#include "ThreadObjs.h"
#include "TCPSendThreads.h"
#include "UDPSendThreads.h"

// KYLib 2.0 开始使用 KYLib 命名空间
using namespace KYLib;

// 静态函数中由于静态对象初始化的先后顺序问题, 而必须优先初始化的函数
inline void InitKYLib(THandle AHandle = NULL)
{
   // 初始化 TDateTime 的格式设置
   InitFormatSettings();

   // 设置模块句柄
   InitHInstance(AHandle);
}

#endif
