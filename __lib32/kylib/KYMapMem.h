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

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// �½��ڴ�ӳ��
void* NewMapMem(const KYString& AName,   long  ASize,
                       THandle& AHandle, bool& AIsExist);

// ���ڴ�ӳ��
void* OpenMapMem(const KYString& AName, THandle& AHandle);

// ɾ���ڴ�ӳ��
void  DeleteMapMem(THandle AHandle, void* AMapMem);

}

#endif
