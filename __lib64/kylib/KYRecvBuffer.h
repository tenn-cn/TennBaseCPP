// =======================================
// Unit   : ���ջ�������Ԫ
// Version: 3.0.1.0 (build 2013.06.03)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYRecvBuffer_H_
#define _KYRecvBuffer_H_

#include "KYString.h"
#include "KYSyncObj.h"
#include "KYCycQueue.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYRecvBuffer - ���ջ������� */

class TKYRecvBuffer
{
public:
   // ���캯��
   // 1. ACanLock    �Ƿ��ڲ�����
   // 2. ACacheSize  ��������سߴ�, ���Լ���Ƶ�������ڴ�
   TKYRecvBuffer(bool ACanLock = true, Word ACacheSize = 64);
   virtual ~TKYRecvBuffer();

   // ��ǰ���������ݳߴ�, Ĭ��ֵΪ: 0
   long           Size() const         { return FSize; }

   // ���ݼ��뻺����
   long           Push(const void* AData, long ASize);
   long           Push(const KYString& AText)
                  { return Push((const char*)AText, AText.Length()); }

   // ���������
   void           Clear();

   // �ӻ������н�������, ���ؽ��յ����ݳߴ�
   long           Recv(void* AData, long ASize);

   // �ӻ������н���ָ���ߴ������
   KYString       Recv(long ASize);

   // ���ջ������е���������
   KYString       Recv();

protected:
   void           Lock() const         { if (FLock != NULL) FLock->Enter(); }
   void           Unlock() const       { if (FLock != NULL) FLock->Leave(); }

   // ִ�н���(ע: AData �ǿ�, �� (ASize > 0) && (ASize <= FSize))
   void           DoRecv(void* AData, long ASize);
   void           AdjustPos();

private:
   void*          NewBlock();
   void           DeleteBlock(void* ABlock);

private:
   TKYCritSect*   FLock;               // ��
   TKYCycQueue*   FCache;              // ���������
   TKYCycQueue*   FBlocks;             // ��������, ÿ��ߴ�Ϊ 1 Kb
   void*          FLastBlock;          // ���һ�黺����
   long           FBegin;              // ��ʼλ��
   long           FEnd;                // ����λ��
   long           FSize;               // ��ǰ���������ݳߴ�
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYRecvCache - ���ո��ٻ�����: ������ɾ����ǰ������ */

class TKYRecvCache
{
public:
   // ���캯��
   // 1. AMaxSize    ���ջ����������ߴ�
   // 2. ACanLock    �Ƿ��ڲ�����
   TKYRecvCache(long AMaxSize = 256, bool ACanLock = false);
   virtual ~TKYRecvCache();

   // ����
   long           MaxSize() const      { return FMaxSize; } // default: 256
   long           Size() const         { return FSize; }    // default: 0

   // ���ݼ��뻺����
   // ������������Զ��������ȼ��������
   long           Push(const void* AData, long ASize);
   long           Push(const KYString& AText)
                  { return Push((const char*)AText, AText.Length()); }

   // ���������
   void           Clear();

   // �ӻ������н�������, ���ؽ��յ����ݳߴ�
   long           Recv(void* AData, long ASize);

   // �ӻ������н���ָ���ߴ������
   KYString       Recv(long ASize);

   // ���ջ������е���������
   KYString       Recv()               { return Recv(FMaxSize); }

protected:
   void           Lock() const         { if (FLock != NULL) FLock->Enter(); }
   void           Unlock() const       { if (FLock != NULL) FLock->Leave(); }

   // ִ�н���(ע: AData �ǿ�, �� (ASize > 0) && (ASize <= FSize))
   void           DoRecv(void* AData, long ASize);

private:
   TKYCritSect*   FLock;               // ��
   char*          FBlock;              // ��������
   Longword       FMaxSize;            // �����������ߴ�
   Longword       FSize;               // ��ǰ���������ݳߴ�
   Longword       FBegin;              // ��ʼλ��
   Longword       FEnd;                // ����λ��
};

}

#endif
