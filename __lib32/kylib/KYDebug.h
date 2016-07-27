// =======================================
// Unit   : Debug client 的封装(.h)
// Version: 3.0.0.0 (build 2011.03.13)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYDebug_H_
#define _KYDebug_H_

#include "KYString.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 写 Debug 相关函数 */

// 初始化和释放 Debug, 注: AName 为标识符
void InitDebugObj(const KYString& AName);
void FreeDebugObj();

// 判断 Debug 是否已经打开
bool DebugOpened();

// 写 Debug 行
bool DebugWrite(const char* AInfo, bool AIncThreadID = true);
bool DebugWrite(const KYString& AInfo, bool AIncThreadID = true);

// 写带 ID 的 Debug 行
bool DebugWriteByID(const char* AInfo, const char* AID, bool AIncThreadID = true);
bool DebugWriteByID(const KYString& AInfo, const char* AID, bool AIncThreadID = true);

// 格式化写 Debug
bool DebugWriteFmt(const char* AFormat, bool AIncThreadID, ...);
bool DebugWriteFmt(const KYString& AFormat, bool AIncThreadID, ...);

}

#endif
