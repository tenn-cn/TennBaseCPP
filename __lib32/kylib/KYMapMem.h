// =======================================
// Unit   : Map Memory (KYMapMem.h)
// Version: 3.0.0.0 (build 2012.06.04)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYMapMem_H_
#define _KYMapMem_H_

#include "KYObject.h"
#include "KYString.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// 新建内存映象
void* NewMapMem(const KYString& AName,   long  ASize,
                       THandle& AHandle, bool& AIsExist);

// 打开内存映象
void* OpenMapMem(const KYString& AName, THandle& AHandle);

// 删除内存映象
void  DeleteMapMem(THandle AHandle, void* AMapMem);

}

#endif
