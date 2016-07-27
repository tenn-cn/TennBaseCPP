// =======================================
// Unit   : 共享内存队列类 (KYQueueMap.h)
// Version: 3.0.0.0 (build 2011.03.16)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYQueueMap_H_
#define _KYQueueMap_H_

#include "KYMapMem.h"
#include "KYSyncObj.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYQueueMap - 共享内存队列类 */

class TKYQueueMap
{
protected:
   // 头信息
   #pragma pack(push, 1)
   typedef struct
   {
      Word        HeadSize;            // 队列头尺寸
      Word        QueueTag;            // 队列标签值
      Longword    ItemSize;            // 队列项尺寸
      Longword    MaxCount;            // 最大项数
      Longword    LastTick;            // 压入队列的最后 Tick
      Longword    PushCount;           // 压入队列计数
      Longword    PushIndex;           // 压入队列索引
      Longword    PopCount;            // 弹出队列计数
      Longword    PopIndex;            // 弹出队列索引
   } THead, *PHead;
   #pragma pack(pop)

public:
   // 队列项最大尺寸
   static const Longword Item_MaxSize; // = 0x10000000;

public:
   TKYQueueMap(bool ACanLock = false);
   virtual ~TKYQueueMap();

   // 属性
   KYString       Name() const         { return FName; }       // default: ""
   bool           Opened() const       { return FOpened; }     // default: false
   bool           IsCycle() const      { return FIsCycle; }    // default: false
   Word           QueueTag() const     { return FOpened ? FHead->QueueTag : FQueueTag; }
   Longword       ItemSize() const     { return FOpened ? FHead->ItemSize : 0; }
   Longword       MaxCount() const     { return FOpened ? FHead->MaxCount : 0; }
   Longword       LastTick() const     { return FOpened ? FHead->LastTick : 0; }
   Longword       PopIndex() const     { return FOpened ? FHead->PopIndex  : 0; }
   Longword       PushIndex() const    { return FOpened ? FHead->PushIndex : 0; }
   Longword       IdleCount() const;   // 取队列空闲项数
   Longword       Count() const;       // 取队列项数

   // 设置标签值, 当创建共享内存队列时会使用此值做为队列标签值
   void           SetQueueTag(Word ATag);

   // 打开共享内存队列
   // 1. 若 ACanCreate == true 且共享内存不存在时, 则创建共享内存队列
   // 2. AItemSize 和 AMaxCount 只有创建共享内存队列时有效
   // 3. AIsCycle 属性表示当前打开的共享内存队列是否可以循环写
   bool           Open(const KYString& AName, Longword AItemSize  = 0,
                                              Longword AMaxCount  = 0,
                                                  bool ACanCreate = true,
                                                  bool AIsCycle   = true);

   // 关闭共享内存队列
   void           Close();

   // 清除队列所有项
   bool           Clear();

   // 压入队列项数据
   bool           Push(const void* AData, long ASize);

   // 预读取队列项数据
   bool           Peek(void* AData, long& ASize) const;

   // 弹出队列项数据
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
   TKYCritSect*   FLock;               // 锁
   bool           FOpened;             // 队列是否已打开
   bool           FCanLock;            // 队列是否带共享内存锁
   bool           FIsCycle;            // 是否循环写
   Word           FQueueTag;           // 队列标签值

   KYString       FName;               // 共享内存名 (区分大小写)
   THead*         FHead;               // 队列头信息
   Byte*          FQueue;              // 队列
   THandle        FHandle;             // 共享内存句柄
   TKYMutex*      FQueueLock;          // 共享内存锁
};

}

#endif
