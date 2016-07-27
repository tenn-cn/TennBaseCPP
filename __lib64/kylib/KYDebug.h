// =======================================
// Unit   : Debug client �ķ�װ(.h)
// Version: 3.0.0.0 (build 2011.03.13)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYDebug_H_
#define _KYDebug_H_

#include "KYString.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* д Debug ��غ��� */

// ��ʼ�����ͷ� Debug, ע: AName Ϊ��ʶ��
void InitDebugObj(const KYString& AName);
void FreeDebugObj();

// �ж� Debug �Ƿ��Ѿ���
bool DebugOpened();

// д Debug ��
bool DebugWrite(const char* AInfo, bool AIncThreadID = true);
bool DebugWrite(const KYString& AInfo, bool AIncThreadID = true);

// д�� ID �� Debug ��
bool DebugWriteByID(const char* AInfo, const char* AID, bool AIncThreadID = true);
bool DebugWriteByID(const KYString& AInfo, const char* AID, bool AIncThreadID = true);

// ��ʽ��д Debug
bool DebugWriteFmt(const char* AFormat, bool AIncThreadID, ...);
bool DebugWriteFmt(const KYString& AFormat, bool AIncThreadID, ...);

}

#endif
