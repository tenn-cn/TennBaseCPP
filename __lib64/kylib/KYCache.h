// =======================================
// Unit   : �̶��ߴ���ڴ�黺��
// Version: 3.0.0.0 (build 2014.10.21)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYCache_H_
#define _KYCache_H_

#include "KYObject.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYCache - �̶��ߴ���ڴ�黺���� */

// ע:
// 1. Ϊ�˶��̴߳�ȡ��ȫ, New �� Delete ���������߳�ʱ����ͬʱ����������Ҫ����,
//    �����߳� New ʱ������������, ���߳� Delete ʱ������������!
// 2. �˻�����һ��Ӧ�������Բ���������, �Լ���Ƶ��������ͷ��ڴ�Ļ���ʹ��.

class TKYCache
{
private:
   // �ڴ�������
   typedef struct
   {
      void*       Self;                // �ڴ����������
      void*       Next;                // ��һ��
   } TLink, *PLink;

public:
   // ���캯��
   // 1. ABlockSize  �ڴ��Ĺ̶��ߴ�, ȡֵ��Χ: [0x40..0x40000000]
   // 2. AMaxCount   �ڴ�黺���������
   TKYCache(long ABlockSize = 1024, long AMaxCount = 256);
   virtual ~TKYCache();

   // ����
   long           Count() const        { return FPushCount - FPopCount; }
   long           MaxCount() const     { return FMaxCount; }   // default: AMaxCount
   long           BlockSize() const    { return FBlockSize; }  // default: ABlockSize

   // �����ڴ�黺���������
   void           SetMaxCount(long AMaxCount)
                  { FMaxCount = (AMaxCount >= 0) ? AMaxCount : 0; }

   // ����̶��ߴ���ڴ��
   void*          New()
                  {
                     TLink* pItem = DoNew();
                     return (pItem != NULL) ? (char*)pItem + sizeof(TLink) : NULL;
                  }

   // �ͷŹ̶��ߴ���ڴ��
   void           Delete(void* ABlock)
                  {
                     if (ABlock != NULL)
                     {
                        TLink* pItem = (TLink*)((char*)ABlock - sizeof(TLink));
                        if (pItem->Self == this)
                           DoDelete(pItem);
                     }
                  }

private:
   // ִ�з���/�ͷŴ����ӵ��ڴ��
   TLink*         DoNew();
   void           DoDelete(TLink* ALink);

   // ִ������������
   void           DoClear(TLink* AHead);

private:
   TLink*         FHead;               // �����ͷ����
   TLink*         FTail;               // �����β����
   long           FFlag;               // ������б�־
   long           FMaxCount;           // ����������
   long           FBlockSize;          // �ڴ��ĳߴ�
   Longword       FPushCount;          // ѹ�뻺�����
   Longword       FPopCount;           // �����������
};

}

#endif
