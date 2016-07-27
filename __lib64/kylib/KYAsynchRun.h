// =======================================
// Unit   : Asynchronous Run Methods
// Version: 3.0.1.0 (build 2013.05.31)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYAsynchRun_H_
#define _KYAsynchRun_H_

#include "KYThread.h"
#include "KYSyncObj.h"
#include "KYCycQueue.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYAsynchRun - 异步执行类 */

class TKYAsynchRun: public TThread
{
public:
   // 方法类型
   // 注: void* 类型为指针类型, 可以传递指针, 如: 结构指针, 对象指针等等
   //           也可以传递尺寸不超过 sizeof(void*) 的整型
   typedef void (TObject::*TDoCustom)(void* Sender);
   typedef void (TObject::*TDoParam1)(void* Sender, void* P1);
   typedef void (TObject::*TDoParam2)(void* Sender, void* P1, void* P2);
   typedef void (TObject::*TDoParam3)(void* Sender, void* P1, void* P2, void* P3);
   typedef void (TObject::*TDoParam4)(void* Sender, void* P1, void* P2, void* P3, void* P4);

private:
   // 方法类型
   enum TMethodType {mtCustom, mtParam1, mtParam2, mtParam3, mtParam4};

   // 方法项
   typedef struct
   {
      TMethodType Type;                // 方法类型
      TDoCustom   Method;              // 方法指针
      void*       Object;              // 方法的对象指针
      void*       Sender;              // 调用者对象指针
      void*       Param1;              // 参数1
      void*       Param2;              // 参数2
      void*       Param3;              // 参数3
      void*       Param4;              // 参数4
   } TItem, *PItem;

public:
   TKYAsynchRun(long ACacheSize = Default_CacheSize);
   virtual ~TKYAsynchRun();

   // 执行线程队列中的方法项数
   long           Count() const        { return FQueue->Count(); }   // default: 0

   // 关闭线程, 建议 FreeKYAsynchRun 释放线程
   void           Close();

   // 清除队列中的所有执行方法
   void           Clear();

   // 添加 TDoCustom 类型的方法到执行线程
   bool           AddCustom(TDoCustom AMethod, void* AObject, void* Sender)
                  { return Add(mtCustom, AMethod, AObject,
                               Sender, NULL, NULL, NULL, NULL); }

   // 添加 TDoParam1 类型的方法到执行线程
   bool           AddParam1(TDoParam1 AMethod, void* AObject, void* Sender,
                                               void* P1)
                  { return Add(mtParam1, (TDoCustom)AMethod, AObject,
                               Sender, P1, NULL, NULL, NULL); }

   // 添加 TDoParam2 类型的方法到执行线程
   bool           AddParam2(TDoParam2 AMethod, void* AObject, void* Sender,
                                               void* P1, void* P2)
                  { return Add(mtParam2, (TDoCustom)AMethod, AObject,
                               Sender, P1, P2, NULL, NULL); }

   // 添加 TDoParam3 类型的方法到执行线程
   bool           AddParam3(TDoParam3 AMethod, void* AObject, void* Sender,
                                               void* P1, void* P2, void* P3)
                  { return Add(mtParam3, (TDoCustom)AMethod, AObject,
                               Sender, P1, P2, P3, NULL); }

   // 添加 TDoParam4 类型的方法到执行线程
   bool           AddParam4(TDoParam4 AMethod, void* AObject, void* Sender,
                                               void* P1, void* P2, void* P3, void* P4)
                  { return Add(mtParam4, (TDoCustom)AMethod, AObject,
                               Sender, P1, P2, P3, P4); }

protected:
   // 压入锁
   void           LockPush() const     { FLockPush->Enter(); }
   void           UnlockPush() const   { FLockPush->Leave(); }

   // 弹出锁
   void           LockPop() const      { FLockPop->Enter(); }
   void           UnlockPop() const    { FLockPop->Leave(); }

   // 线程执行体
   virtual void   Execute();

private:
   bool           Add(TMethodType AType, TDoCustom AMethod, void* AObject,
                            void* Sender, void* P1, void* P2, void* P3, void* P4);

   TItem*         New();
   void           Dispose(TItem* AItem);

private:
   TKYCritSect*   FLockPush;           // 压入锁
   TKYCritSect*   FLockPop;            // 弹出锁
   TKYEvent*      FNotify;             // 通知
   TKYCycQueue*   FQueue;              // 消息队列
   TKYCycQueue*   FCache;              // 空闲队列
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 公共函数 */

// 关闭并释放线程
void FreeKYAsynchRun(TKYAsynchRun* &AThread, Longword ATimeout = 5000);

}

#endif
