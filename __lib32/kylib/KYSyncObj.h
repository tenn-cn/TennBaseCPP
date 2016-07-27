// =======================================
// Unit   : 线程同步类 (KYSyncObj.h)
// Version: 3.0.0.0 (build 2011.03.09)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYSyncObj_H_
#define _KYSyncObj_H_

#include <windows.h>
#include "KYObject.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 公用函数 */

// 得到安全属性
LPSECURITY_ATTRIBUTES GetSecurityAttrib();

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYCritSect - 临界区类 */

class TKYCritSect
{
public:
   TKYCritSect();
   virtual ~TKYCritSect();

   // 进入临界区, 在同一线程内可以递归调用
   void              Enter()           { EnterCriticalSection(&FCritSect); }

   // 离开临界区, 在同一线程内可以递归调用, 但离开次数必须与进入次数相同
   void              Leave()           { LeaveCriticalSection(&FCritSect); }

private:
   CRITICAL_SECTION  FCritSect;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYEvent - 事件类 */

class TKYEvent
{
public:
   // 事件复位类型 {自动, 手动, 脉冲}
   enum TResetType  {erAuto,           // 自动复位
                     erManual,         // 手动复位
                     erPulse};         // 脉冲复位

public:
   TKYEvent(const char* AName = NULL, TResetType AResetType = erAuto,
                                            bool AInitSignaled = false);
   virtual ~TKYEvent();

   // 事件句柄
   THandle           Handle() const    { return FEvent; }

   // 脉冲
   void              Pulse() const     { PulseEvent(FEvent); }

   // 复位
   void              Reset() const     { ResetEvent(FEvent); }

   // 置位
   void              Set() const       { SetEvent(FEvent); }

   // 等待事件
   bool              Wait(DWORD ATimeout = INFINITE) const
                     {
                        return (WaitForSingleObject(FEvent, ATimeout) == WAIT_OBJECT_0);
                     }

private:
   THandle           FEvent;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMutex - 互斥元类 */

class TKYMutex
{
public:
   TKYMutex(const char* AName = NULL);
   virtual ~TKYMutex();

   // 互斥元句柄
   THandle           Handle() const    { return FHandle; }

   // 试着加锁, 若成功则返回 true, 否则返回 false
   bool              TryLock() const   { return Lock(0); }

   // 加锁, 若成功则返回 true, 否则返回 false
   bool              Lock(DWORD ATimeout = INFINITE) const
                     {
                        return (WaitForSingleObject(FHandle, ATimeout) == WAIT_OBJECT_0);
                     }

   // 解锁
   void              Unlock() const    { ReleaseMutex(FHandle); }

private:
   THandle           FHandle;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYSemaphore - 信号量类 */

class TKYSemaphore
{
public:
   TKYSemaphore(const char* AName = NULL, long AInitCount = 0,
                                          long AMaxCount  = 10);
   virtual ~TKYSemaphore();

   // 信号量句柄
   THandle           Handle() const { return FHandle; }

   // 等待信号量
   bool              DecCount(DWORD ATimeout = INFINITE) const
                     {
                        return (WaitForSingleObject(FHandle, ATimeout) == WAIT_OBJECT_0);
                     }

   // 信号量加 ANum
   bool              IncCount(long ANum = 1) const
                     {
                        return (ReleaseSemaphore(FHandle, ANum, NULL) != FALSE);
                     }

private:
   THandle           FHandle;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYLockRW - 多读单写锁类 */

class TKYLockRW
{
public:
   TKYLockRW();
   virtual ~TKYLockRW();

   // 读加锁
   bool              LockRead();

   // 写加锁
   bool              LockWrite();

   // 试着读加锁
   bool              TryLockRead();

   // 试着写加锁
   bool              TryLockWrite();

   // 读解锁
   void              UnlockRead();

   // 写解锁
   void              UnlockWrite();

private:
   inline void       SetReadSignal();
   inline void       SetWriteSignal();

private:
   TKYCritSect       FRWLock;
   TKYEvent          FReader;
   TKYEvent          FWriter;

   long              FReadingCount;
   long              FWritingCount;
   long              FWaitingReadCount;
   long              FWaitingWriteCount;
};

}

#endif
