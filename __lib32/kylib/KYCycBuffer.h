// =======================================
// Unit   : cycle buffer(ʹ��ѭ����ʽ�Ļ�����)
// Version: 3.0.1.0 (build 2014.10.20)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYCycBuffer_H_
#define _KYCycBuffer_H_

#include "KYString.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYCycBuffer - ʹ��ѭ����ʽ�Ļ������� */

// ע: Ϊ�˶��̴߳�ȡ��ȫ, Push �� Pop ���������߳�ʱ����ͬʱ����������Ҫ����,
//     �����߳� Push ʱ������������, ���߳� Pop/Peek/Lose ʱ������������!

class TKYCycBuffer
{
private:
   // ���ݿ������
   typedef struct
   {
      Longword    PushSize;            // ѹ�����ݳߴ�
      Longword    PopSize;             // �������ݳߴ�
      void*       Next;                // ��һ��
   } TIndex, *PIndex;

public:
   // ���ݿ�ĳߴ�����
   enum TType    {stSize2_8   = 0,     // 2^8  �ֽ���: 256
                  stSize2_9   = 1,     // 2^9  �ֽ���: 512
                  stSize2_10  = 2,     // 2^10 �ֽ���: 1024
                  stSize2_11  = 3,     // 2^11 �ֽ���: 2048
                  stSize2_12  = 4,     // 2^12 �ֽ���: 4096
                  stSize2_13  = 5,     // 2^13 �ֽ���: 8192
                  stSize2_14  = 6,     // 2^14 �ֽ���: 16384
                  stSize2_15  = 7,     // 2^15 �ֽ���: 32768
                  stSize2_16  = 8};    // 2^16 �ֽ���: 65536

   // ��д���ݵķ���, �������ݳߴ�
   typedef long (TObject::*TDoRead)(void* AData, long ASize);
   typedef long (TObject::*TDoWrite)(const void* AData, long ASize);

   // ��д���ݵ� Push �� Pop ��������
   // ע: ��Ϊ Push �� Pop �������ط���, �������޷�ָ���ĸ�����, ������ȷ���巽
   // ������, �����ñ�����ʶ�𵽾����ĸ�������
   typedef Longword (TKYCycBuffer::*TDoPop)(void* AData, Longword ASize);
   typedef Longword (TKYCycBuffer::*TDoPush)(const void* AData, Longword ASize);

public:
   // ���캯��
   // 1. AMaxCache   ���ݿ����󻺳����
   // 2. ASizeType   ���ݿ�ĳߴ�����, ����ÿ�����ݿ�ĳߴ綨��
   TKYCycBuffer(Word AMaxCache = 64, Byte ASizeType = stSize2_10);
   virtual ~TKYCycBuffer();

   // ����
   Longword       Size() const         { return FPushSize - FPopSize; }
   Longword       MaxSize() const      { return FMaxSize; }             // default: 0xFFFFFFFF
   Longword       PopSize() const      { return FPopSize; }             // default: 0
   Longword       PushSize() const     { return FPushSize; }            // default: 0
   Longword       BlockSize() const    { return FBlockSize; }           // default: 2 ^ (ASizeType + 8)
   TType          SizeType() const     { return (TType)FSizeType; }     // default: ASizeType
   Word           MaxCache() const     { return FMaxCache; }            // default: AMaxCache
   Word           CacheCount() const   { return (Word)(FPushCache - FPopCache); }

   // �����������ߴ�
   void           SetMaxSize(Longword AMaxSize)
                  { FMaxSize = AMaxSize; }

   // �������ݿ����󻺳����
   void           SetMaxCache(Word AMaxCache)
                  { FMaxCache = AMaxCache; }

   // ���ݼ��뻺����
   Longword       Push(TDoRead ADoRead, void* AObject, Longword ASize);
   Longword       Push(const void* AData, Longword ASize);

   Longword       Push(TKYCycBuffer& ABuffer, Longword ASize)
                  { return Push((TDoRead)(TDoPop)&TKYCycBuffer::Pop, &ABuffer, ASize); }
   Longword       Push(TKYCycBuffer& ABuffer)
                  { return Push(ABuffer, 0xFFFFFFFF); }
   Longword       Push(const KYString& AText)
                  { return Push((const char*)AText, AText.Length()); }

   // �ӻ������ж�ȡ����, ���ض�ȡ�����ݳߴ�
   Longword       Pop(TDoWrite ADoWrite, void* AObject, Longword ASize);
   Longword       Pop(void* AData, Longword ASize);

   Longword       Pop(TKYCycBuffer& ABuffer, Longword ASize)
                  { return Pop((TDoWrite)(TDoPush)&TKYCycBuffer::Push, &ABuffer, ASize); }
   Longword       Pop(TKYCycBuffer& ABuffer)
                  { return Pop(ABuffer, 0xFFFFFFFF); }

   // �ӻ������ж�ȡָ���ߴ������(ע: ASize > 0)
   KYString       Pop(long ASize);
   KYString       Pop()                { return Pop(0x7FFFFFFF); }

   // �ӻ�������Ԥ��ȡ����, ����Ԥ��ȡ�����ݳߴ�
   Longword       Peek(TDoWrite ADoWrite, void* AObject, Longword ASize) const;
   Longword       Peek(void* AData, Longword ASize) const;

   Longword       Peek(TKYCycBuffer& ABuffer, Longword ASize) const
                  { return Peek((TDoWrite)(TDoPush)&TKYCycBuffer::Push, &ABuffer, ASize); }
   Longword       Peek(TKYCycBuffer& ABuffer) const
                  { return Peek(ABuffer, 0xFFFFFFFF); }

   // �ӻ�������Ԥ��ȡָ���ߴ������(ע: ASize > 0)
   KYString       Peek(long ASize) const;
   KYString       Peek() const         { return Peek(0x7FFFFFFF); }

   // �ӻ������ж���ָ���ߴ������, ���ض��������ݳߴ�
   Longword       Lose(Longword ASize);
   Longword       Lose()               { return Lose(0xFFFFFFFF); }

private:
   // ����/�ͷ�����
   TIndex*        DoNew();
   void           DoDelete(TIndex* AIndex);

   // ��������б�
   void           DoClear(TIndex* AHead);

private:
   // ȡ���ݿ�ĳߴ�/ָ����
   Longword       DoSize(TIndex* AIndex) const
                  { return AIndex->PushSize - AIndex->PopSize; }
   void*          DoItem(TIndex* AIndex, Longword ANo) const
                  { return (Byte*)AIndex + sizeof(TIndex) + ANo; }

   // �����ݿ���ѹ������
   Longword       DoPush(TIndex* AIndex, TDoRead ADoRead, void* AObject, Longword ASize) const;
   Longword       DoPush(TIndex* AIndex, const void* AData, Longword ASize) const;

   // �����ݿ���Ԥ��ȡ����
   Longword       DoPeek(TIndex* AIndex, TDoWrite ADoWrite, void* AObject, Longword ASize) const;
   Longword       DoPeek(TIndex* AIndex, void* AData, Longword ASize) const;

   // �����ݿ��е�������
   Longword       DoPop(TIndex* AIndex, TDoWrite ADoWrite, void* AObject, Longword ASize) const;
   Longword       DoPop(TIndex* AIndex, void* AData, Longword ASize) const;

private:
   TIndex*        FBlockHead;          // ���ݿ��ͷ����
   TIndex*        FBlockTail;          // ���ݿ��β����
   Longword       FBlockSize;          // ���ݿ�ĳߴ�
   Longword       FBlockMask;          // ���ݿ������
   Longword       FPushSize;           // ѹ�����ݳߴ�
   Longword       FPopSize;            // �������ݳߴ�
   Longword       FMaxSize;            // �������ߴ�

   Word           FMaxCache;           // ���ݿ�Ļ������
   Byte           FSizeType;           // ���ݿ�ĳߴ缶��

   long           FCacheFlag;          // ������б�־
   TIndex*        FCacheHead;          // �����ͷ����
   TIndex*        FCacheTail;          // �����β����
   Longword       FPushCache;          // ѹ�뻺�����
   Longword       FPopCache;           // �����������
};

}

#endif
