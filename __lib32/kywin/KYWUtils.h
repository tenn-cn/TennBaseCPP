// =======================================
// Unit   : KYWin ���õ�Ԫ (KYWUtils.h)
// Version: 3.0.0.0 (build 2011.03.10)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYWUtils_H_
#define _KYWUtils_H_

#include "KYLib.h"

// KYWin 2.0 ��ʼʹ�� KYWin �����ռ�
namespace KYWin
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �������� */

// ����ַ������Ƿ���ڿհ��ַ�
bool     IsExistedBlankChar(const KYString& AStr);

// �ַ����б�ת����'\0'��β���ַ���
KYString StrList2PChars(const TKYStringList* AList);

// �ַ����б�ת����'\0'��β���ַ�������
// ע: ʹ��(char**)(char*)resultת������ֵ, ����AList->Item()���ַ���ָ��)
KYString StrList2PCharArray(const TKYStringList* AList);

// ִ�п�ִ���ļ�
bool     WinExec32(const KYString& AFileName,
                   const KYString& AParameter   = "",
                             Word  AShowWindow  = SW_SHOWNORMAL,
                             bool  AIsWaitExit  = false);

// ִ�п�ִ���ļ�(���ؽ��̺����߳̾��, ��Ҫ���������رվ��)
bool     WinExec32(const KYString& AFileName, const KYString& AParameter,
                          THandle& AProcHandle,     Longword& AProcID,
                          THandle& AThreadHandle,   Longword& AThreadID,
                             Word  AShowWindow  = SW_SHOWNORMAL);

}

#endif
