// =======================================
// Unit   : KYWin 公用单元 (KYWUtils.h)
// Version: 3.0.0.0 (build 2011.03.10)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYWUtils_H_
#define _KYWUtils_H_

#include "KYLib.h"

// KYWin 2.0 开始使用 KYWin 命名空间
namespace KYWin
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 公共函数 */

// 检查字符串中是否存在空白字符
bool     IsExistedBlankChar(const KYString& AStr);

// 字符串列表转换成'\0'结尾的字符串
KYString StrList2PChars(const TKYStringList* AList);

// 字符串列表转换成'\0'结尾的字符串数组
// 注: 使用(char**)(char*)result转换返回值, 保存AList->Item()的字符串指针)
KYString StrList2PCharArray(const TKYStringList* AList);

// 执行可执行文件
bool     WinExec32(const KYString& AFileName,
                   const KYString& AParameter   = "",
                             Word  AShowWindow  = SW_SHOWNORMAL,
                             bool  AIsWaitExit  = false);

// 执行可执行文件(返回进程和主线程句柄, 需要调用者来关闭句柄)
bool     WinExec32(const KYString& AFileName, const KYString& AParameter,
                          THandle& AProcHandle,     Longword& AProcID,
                          THandle& AThreadHandle,   Longword& AThreadID,
                             Word  AShowWindow  = SW_SHOWNORMAL);

}

#endif
