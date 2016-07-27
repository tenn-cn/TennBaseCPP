// =======================================
// Unit   : cycle queue(ʹ��ѭ����ʽ�Ķ���)
// Version: 3.0.1.0 (build 2014.10.20)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYCycQueue_H_
#define _KYCycQueue_H_

#include "KYObject.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYCycQueue - ʹ��ѭ����ʽ�Ķ����� */

// ע: Ϊ�˶��̴߳�ȡ��ȫ, Push �� Pop ���������߳�ʱ����ͬʱ����������Ҫ����,
//     �����߳� Push ʱ������������, ���߳� Pop/Peek ʱ������������!

class TKYCycQueue
{
private:
   // ѭ������
   typedef struct
   {
      Longword    PushCount;           // ѹ����м���
      Longword    PopCount;            // �������м���
      void*       Next;                // ��һ��
   } TIndex, *PIndex;

public:
   // ѭ�������ĳߴ缶��
   enum TLevel   {slPower6    = 0,     // 2^6  ��: 64
                  slPower7    = 1,     // 2^7  ��: 128
                  slPower8    = 2,     // 2^8  ��: 256
                  slPower9    = 3,     // 2^9  ��: 512
                  slPower10   = 4,     // 2^10 ��: 1024
                  slPower11   = 5,     // 2^11 ��: 2048
                  slPower12   = 6,     // 2^12 ��: 4096
                  slPower13   = 7,     // 2^13 ��: 8192
                  slPower14   = 8,     // 2^14 ��: 16384
                  slPower15   = 9,     // 2^15 ��: 32768
                  slPower16   = 10};   // 2^16 ��: 65536

public:
   // ���캯��
   // 1. ACacheSize ѭ�������Ļ������
   // 2. ASizeLevel ѭ�������ĳߴ缶��, ����ÿ��ѭ����������������
   TKYCycQueue(Word ACacheSize = 64, Byte ASizeLevel = slPower10);
   virtual ~TKYCycQueue();

   // ����
   Longword       Count() const        { return FPushCount - FPopCount; }
   Longword       MaxCount() const     { return FMaxCount; }            // default: 0xFFFFFFFF
   Longword       CycCount() const     { return FCycCount; }            // default: 2 ^ (ASizeLevel + 6)
   Longword       PopCount() const     { return FPopCount; }            // default: 0
   Longword       PushCount() const    { return FPushCount; }           // default: 0
   TLevel         SizeLevel() const    { return (TLevel)FSizeLevel; }   // default: ASizeLevel
   Word           CacheSize() const    { return FCacheSize; }           // default: ACacheSize
   Word           CacheCount() const   { return (Word)(FPushCache - FPopCache); }

   // ���ö��е��������
   void           SetMaxCount(Longword AMaxCount)
                  { FMaxCount = AMaxCount; }

   // ����ѭ�������Ļ������
   void           SetCacheSize(Word ACacheSize)
                  { FCacheSize = ACacheSize; }

   // ѹ��������
   bool           Push(void* AData);

   // Ԥ��ȡ������
   bool           Peek(Pointer& AData) const;
   void*          Peek() const;

   // ����������
   bool           Pop(Pointer& AData);
   void*          Pop();

private:
   // ȡ����������/ָ����
   Longword       DoCount(TIndex* AIndex) const
                  { return AIndex->PushCount - AIndex->PopCount; }
   void*          DoItem(TIndex* AIndex, Longword ANo) const
                  { return (Byte*)AIndex + sizeof(TIndex) + sizeof(void*) * ANo; }

   // ����������ѹ��/��������
   bool           DoPush(TIndex* AIndex, void* AData) const;
   bool           DoPeek(TIndex* AIndex, Pointer& AData) const;
   bool           DoPop(TIndex* AIndex, Pointer& AData) const;

   // ����/�ͷ�����
   TIndex*        DoNew();
   void           DoDelete(TIndex* AIndex);

   // ��������б�
   void           DoClear(TIndex* AHead);

private:
   TIndex*        FIndexHead;          // ����ͷ����
   TIndex*        FIndexTail;          // ����β����
   Longword       FPushCount;          // ѹ����м���
   Longword       FPopCount;           // �������м���
   Longword       FMaxCount;           // ���е��������

   Byte           FSizeLevel;          // ѭ�������ĳߴ缶��
   Word           FCacheSize;          // ѭ�������Ļ������
   Longword       FCycCount;           // ѭ������������
   Longword       FCycMask;            // ѭ������������

   long           FCacheFlag;          // ������б�־
   TIndex*        FCacheHead;          // ����ͷ����
   TIndex*        FCacheTail;          // ����β����
   Longword       FPushCache;          // ѹ�뻺�����
   Longword       FPopCache;           // �����������
};

}

#endif
