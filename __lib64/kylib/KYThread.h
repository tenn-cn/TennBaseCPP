// =======================================
// Unit   : 线程类 (KYThread.h)
// Version: 3.0.0.0 (build 2011.03.09)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYThread_H_
#define _KYThread_H_

#include <process.h>
#include <windows.h>
#include "KYObject.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 公共函数 */

class TThread;
class TKYThread;

// 强制终止线程(注: 适用于 TThread 继承对象)
bool  KillThread(TThread* AThread, Longword AExitCode);

// 强制终止线程(注: 适用于线程 API 函数)
bool  KillThread(THandle AHandle, Longword AExitCode);

// 关闭并释放线程
void  FreeKYThread(TKYThread* &AThread, Longword ATimeout = 5000);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TThread - 线程类 */

class TThread
{
   // 强制终止线程函数
   friend bool    KillThread(TThread*, Longword);

   // 线程函数
   static unsigned __stdcall ThreadProc(void* AThread);

public:
   // 线程优先级
   enum TPriority {tpIdle,   tpLowest,  tpLower, tpNormal,
                   tpHigher, tpHighest, tpTimeCritical};

public:
   TThread(bool ACreateSuspended = true);
   virtual ~TThread();

   // 线程优先级, 默认值为: tpNormal
   TPriority      Priority() const;

   // 线程句柄
   THandle        Handle() const          { return FHandle; }

   // 线程 ID
   unsigned       ThreadID() const        { return FThreadID; }

   // 线程返回值, 默认值为: 0
   unsigned       ReturnValue() const     { return FReturnValue; }

   // 是否线程终止时释放线程对象, 默认值为: false
   bool           FreeOnTerminate() const { return FFreeOnTerminate; }

   // 是否线程已置终止标志, 默认值为: false
   bool           Terminated() const      { return FTerminated; }

   // 是否线程已休眠, 默认值为: ACreateSuspended
   bool           Suspended() const       { return FSuspended; }

   // 是否线程已运行结束, 默认值为: false
   bool           Finished() const        { return FFinished; }

   // 设置线程优先级
   void           SetPriority(TPriority AValue) const;

   // 设置是否线程终止时释放线程对象
   void           SetFreeOnTerminate(bool AValue)
                  { FFreeOnTerminate = AValue; }

   // 唤醒已休眠的线程
   void           Resume();

   // 休眠运行的线程
   void           Suspend();

   // 设置线程终止标志
   void           Terminate();

   // 等待线程运行结束
   bool           WaitFor(Longword ATimeout = INFINITE);

   // 事件
   TNotifyEvent   OnTerminate;

protected:
   void           Return(unsigned AValue) { FReturnValue = AValue; }

   virtual void   Execute() = 0;
   virtual void   DoTerminate();

private:
   THandle        FHandle;
   unsigned       FThreadID;
   unsigned       FReturnValue;
   bool           FFreeOnTerminate;
   bool           FTerminated;
   bool           FSuspended;
   bool           FFinished;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYThread - 独立线程类 */

class TKYThread: public TThread
{
public:
   // 线程执行方法
   typedef unsigned (TObject::*TDoExecute)(TKYThread* AThread, void* AParam);

public:
   TKYThread(TDoExecute AExecute = NULL, void* AObject = NULL,
                  void* AParam = NULL,    bool ACreateSuspended = true);

protected:
   virtual void   Execute();

private:
   TDoExecute     FExecute;
   void*          FObject;
   void*          FParam;
};

}

#endif
