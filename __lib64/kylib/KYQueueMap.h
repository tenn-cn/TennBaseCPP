// =======================================
// Unit   : �����ڴ������ (KYQueueMap.h)
// Version: 3.0.0.0 (build 2011.03.16)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYQueueMap_H_
#define _KYQueueMap_H_

#include "KYMapMem.h"
#include "KYSyncObj.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYQueueMap - �����ڴ������ */

class TKYQueueMap
{
protected:
   // ͷ��Ϣ
   #pragma pack(push, 1)
   typedef struct
   {
      Word        HeadSize;            // ����ͷ�ߴ�
      Word        QueueTag;            // ���б�ǩֵ
      Longword    ItemSize;            // ������ߴ�
      Longword    MaxCount;            // �������
      Longword    LastTick;            // ѹ����е���� Tick
      Longword    PushCount;           // ѹ����м���
      Longword    PushIndex;           // ѹ���������
      Longword    PopCount;            // �������м���
      Longword    PopIndex;            // ������������
   } THead, *PHead;
   #pragma pack(pop)

public:
   // ���������ߴ�
   static const Longword Item_MaxSize; // = 0x10000000;

public:
   TKYQueueMap(bool ACanLock = false);
   virtual ~TKYQueueMap();

   // ����
   KYString       Name() const         { return FName; }       // default: ""
   bool           Opened() const       { return FOpened; }     // default: false
   bool           IsCycle() const      { return FIsCycle; }    // default: false
   Word           QueueTag() const     { return FOpened ? FHead->QueueTag : FQueueTag; }
   Longword       ItemSize() const     { return FOpened ? FHead->ItemSize : 0; }
   Longword       MaxCount() const     { return FOpened ? FHead->MaxCount : 0; }
   Longword       LastTick() const     { return FOpened ? FHead->LastTick : 0; }
   Longword       PopIndex() const     { return FOpened ? FHead->PopIndex  : 0; }
   Longword       PushIndex() const    { return FOpened ? FHead->PushIndex : 0; }
   Longword       IdleCount() const;   // ȡ���п�������
   Longword       Count() const;       // ȡ��������

   // ���ñ�ǩֵ, �����������ڴ����ʱ��ʹ�ô�ֵ��Ϊ���б�ǩֵ
   void           SetQueueTag(Word ATag);

   // �򿪹����ڴ����
   // 1. �� ACanCreate == true �ҹ����ڴ治����ʱ, �򴴽������ڴ����
   // 2. AItemSize �� AMaxCount ֻ�д��������ڴ����ʱ��Ч
   // 3. AIsCycle ���Ա�ʾ��ǰ�򿪵Ĺ����ڴ�����Ƿ����ѭ��д
   bool           Open(const KYString& AName, Longword AItemSize  = 0,
                                              Longword AMaxCount  = 0,
                                                  bool ACanCreate = true,
                                                  bool AIsCycle   = true);

   // �رչ����ڴ����
   void           Close();

   // �������������
   bool           Clear();

   // ѹ�����������
   bool           Push(const void* AData, long ASize);

   // Ԥ��ȡ����������
   bool           Peek(void* AData, long& ASize) const;

   // ��������������
   bool           Pop(void* AData, long& ASize);

protected:
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

   void           QueueLock() const    { if (FQueueLock != NULL) FQueueLock->Lock(); }
   void           QueueUnlock() const  { if (FQueueLock != NULL) FQueueLock->Unlock(); }

private:
   void           DoInit();
   THead*         DoNewMem(const KYString& AName, Longword AItemSize,
                                  Longword AMaxCount, THandle& AHandle);

private:
   TKYCritSect*   FLock;               // ��
   bool           FOpened;             // �����Ƿ��Ѵ�
   bool           FCanLock;            // �����Ƿ�������ڴ���
   bool           FIsCycle;            // �Ƿ�ѭ��д
   Word           FQueueTag;           // ���б�ǩֵ

   KYString       FName;               // �����ڴ��� (���ִ�Сд)
   THead*         FHead;               // ����ͷ��Ϣ
   Byte*          FQueue;              // ����
   THandle        FHandle;             // �����ڴ���
   TKYMutex*      FQueueLock;          // �����ڴ���
};

}

#endif
