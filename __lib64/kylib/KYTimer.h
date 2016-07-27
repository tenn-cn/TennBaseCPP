// =======================================
// Unit   : 定时器类 (KYTimer.h)
// Version: 3.0.0.0 (build 2012.05.28)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYTimer_H_
#define _KYTimer_H_

#include "KYThread.h"
#include "KYHandles.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYTimer - 定时器类 */

class TKYTimer: public TThread
{
public:
   // 定时器的 DoTimer 方法
   typedef void (TObject::*TDoTimer)(void* Sender, void* AParam, long ATimerID);

private:
   // 定时器项
   typedef struct
   {
      TDoTimer    Method;              // 方法指针
      void*       Object;              // 对象指针
      void*       Param;               // 传入参数
      Longword    Interval;            // 定时器间隔
      Longword    BeginTick;           // 开始 Tick
   } TItem, *PItem;

public:
   TKYTimer();
   virtual ~TKYTimer();

   // 定时器是否已打开, 默认值为: false
   bool           Enabled() const               { return FEnabled; }

   // 定时器的间隔(毫秒), 默认值为: 1000
   Longword       Interval() const              { return FInterval; }

   // 判断定时器项句柄是否存在
   bool           Existed(long ATimerID) const  { return FItems->IsExist(ATimerID); }

   // 设置定时器是否打开
   void           SetEnabled(bool AEnabled);

   // 设置定时器的间隔(毫秒)
   void           SetInterval(Longword AInterval);

   // 关闭定时器线程, 建议 FreeKYTimer 释放线程
   void           Close();

   // 新建定时器项, 每个定时器项都有自己的 TDoTimer 事件方法
   long           New(TDoTimer AMethod, void* AObject, void* AParam);

   // 删除定时器项
   void           Delete(long ATimerID);

   // 清除所有定时器项
   void           Clear();

   // 添加定时器项指定间隔的事件, 每个事件激发 TDoTimer 最多一次
   bool           AddEvent(long ATimerID, Longword AInterval);

   // 删除已添加定时器项指定间隔的事件
   void           DeleteEvent(long ATimerID);

protected:
   virtual void   Execute();

   // 锁
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

   // 读锁
   void           LockRead() const     { FLockRW->LockRead(); }
   void           UnlockRead() const   { FLockRW->UnlockRead(); }

   // 写锁
   void           LockWrite() const    { FLockRW->LockWrite(); }
   void           UnlockWrite() const  { FLockRW->UnlockWrite(); }

private:
   void           UpdateTimer();
   bool           ScanNext(bool AIsFirst);

   // FItems 的 OnDeletion 事件方法
   void           DoDeletion(long AHandle, void* AItem);

private:
   TKYCritSect*   FLock;               // 锁
   TKYLockRW*     FLockRW;             // 读写锁
   TKYEvent*      FEvent;
   TKYHandles*    FItems;
   TKYHandles*    FTimers;
   Longword       FInterval;
   long           FPerfCount;
   bool           FEnabled;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 公用函数 */

// 关闭并释放定时器线程
void FreeKYTimer(TKYTimer* &AThread, Longword ATimeout = 15000);

}

#endif
