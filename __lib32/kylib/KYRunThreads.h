// =======================================
// Unit   : 运行线程集类
// Version: 3.0.4.0 (build 2013.07.16)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYRunThreads_H_
#define _KYRunThreads_H_

#include "KYString.h"
#include "KYThread.h"
#include "KYHandles.h"
#include "KYCycQueue.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYRunThread - 运行线程类 */

class TKYRunThread: public TThread
{
public:
   // 方法类型的定义
   // 注: 方法指针就象函数指针一样可以传递并调用, 只是方法指针比函数指针多需要
   //     一个方法的对象指针而已, 不同参数类型使用不同类型的方法指针
   typedef void (TObject::*TDoCustom)();
   typedef void (TObject::*TDoStr1)(const KYString& AStr);
   typedef void (TObject::*TDoStr2)(const KYString& AStr1, const KYString& AStr2);
   typedef void (TObject::*TDoStr3)(const KYString& AStr1, const KYString& AStr2,
                                    const KYString& AStr3);

   typedef void (TObject::*TDoInt1)(long AInt);
   typedef void (TObject::*TDoInt1Str1)(long AInt1, const KYString& AStr);
   typedef void (TObject::*TDoInt1Str2)(long AInt1, const KYString& AStr1,
                                                    const KYString& AStr2);
   typedef void (TObject::*TDoInt1Str3)(long AInt1, const KYString& AStr1,
                                                    const KYString& AStr2,
                                                    const KYString& AStr3);

   typedef void (TObject::*TDoInt2)(long AInt1, long AInt2);
   typedef void (TObject::*TDoInt2Str1)(long AInt1, long  AInt2,
                                          const KYString& AStr);
   typedef void (TObject::*TDoInt2Str2)(long AInt1, long  AInt2,
                                          const KYString& AStr1,
                                          const KYString& AStr2);
   typedef void (TObject::*TDoInt2Str3)(long AInt1, long  AInt2,
                                          const KYString& AStr1,
                                          const KYString& AStr2,
                                          const KYString& AStr3);

   typedef void (TObject::*TDoFloat)(double AFloat);
   typedef void (TObject::*TDoF1Str1)(double AFloat, const KYString& AStr);
   typedef void (TObject::*TDoF1Str2)(double AFloat, const KYString& AStr1,
                                                     const KYString& AStr2);
   typedef void (TObject::*TDoF1Str3)(double AFloat, const KYString& AStr1,
                                                     const KYString& AStr2,
                                                     const KYString& AStr3);

   typedef void (TObject::*TDoInt64)(__int64 AInt64);
   typedef void (TObject::*TDoI64Str1)(__int64 AInt64, const KYString& AStr);
   typedef void (TObject::*TDoI64Str2)(__int64 AInt64, const KYString& AStr1,
                                                       const KYString& AStr2);
   typedef void (TObject::*TDoI64Str3)(__int64 AInt64, const KYString& AStr1,
                                                       const KYString& AStr2,
                                                       const KYString& AStr3);

   typedef void (TObject::*TDoP1)(void* AValue);
   typedef void (TObject::*TDoP1Str1)(void* AValue1, const KYString& AStr);
   typedef void (TObject::*TDoP1Str2)(void* AValue1, const KYString& AStr1,
                                                     const KYString& AStr2);
   typedef void (TObject::*TDoP1Str3)(void* AValue1, const KYString& AStr1,
                                                     const KYString& AStr2,
                                                     const KYString& AStr3);

   typedef void (TObject::*TDoP2)(void* AValue1, void* AValue2);
   typedef void (TObject::*TDoP2Str1)(void* AValue1, void* AValue2,
                                           const KYString& AStr);
   typedef void (TObject::*TDoP2Str2)(void* AValue1, void* AValue2,
                                           const KYString& AStr1,
                                           const KYString& AStr2);
   typedef void (TObject::*TDoP2Str3)(void* AValue1, void* AValue2,
                                           const KYString& AStr1,
                                           const KYString& AStr2,
                                           const KYString& AStr3);

private:
   // 方法类型
   enum TMethodType {mtCustom,   mtStr1,     mtStr2,     mtStr3,
                     mtInt1,     mtInt1Str1, mtInt1Str2, mtInt1Str3,
                     mtInt2,     mtInt2Str1, mtInt2Str2, mtInt2Str3,
                     mtFloat,    mtF1Str1,   mtF1Str2,   mtF1Str3,
                     mtInt64,    mtI64Str1,  mtI64Str2,  mtI64Str3,
                     mtP1,       mtP1Str1,   mtP1Str2,   mtP1Str3,
                     mtP2,       mtP2Str1,   mtP2Str2,   mtP2Str3};

   // 值项
   typedef struct
   {
      void*       P1;                  // 值1
      void*       P2;                  // 值2
   } TValue, *PValue;

   // 联合值项
   typedef union
   {
      TValue      V;                   // 值项
      __int64     VInt64;              // 64位整型值
      double      Float;               // 浮点值
   } TUnionValue, *PUnionValue;

   // 方法项
   typedef struct
   {
      TMethodType Type;                // 方法类型
      TDoCustom   Method;              // 方法指针
      void*       Object;              // 方法的对象指针

      TUnionValue Value;               // 值项
      KYString    Str1;                // 字符串1
      KYString    Str2;                // 字符串2
      KYString    Str3;                // 字符串3
   } TItem, *PItem;

public:
   TKYRunThread(long AItemNo = 0, long ACacheSize = Default_CacheSize);
   virtual ~TKYRunThread();

   // 属性
   void*          Data() const         { return FData; }             // default: NULL
   long           Count() const        { return FQueue->Count(); }   // default: 0
   long           ItemNo() const       { return FItemNo; }           // default: 0
   long           RefCount() const     { return FRefCount; }         // default: 0
   long           PerfCount() const    { return FPerfCount; }        // default: 0

   // 设置属性
   void           SetData(void* AData) { FData = AData; }

   // 增加执行点计数
   void           ResetPerfCount()     { FPerfCount = 0; }
   long           IncPerfCount(long ACount = 1)
                  { FPerfCount += ACount; return FPerfCount; }

   // 关闭线程, 建议 FreeKYRunThread 释放线程
   void           Close();

   // 清除队列中的所有执行方法
   void           Clear();

   // 添加 TDoCustom 类型的方法到执行线程
   bool           AddOfCustom(TDoCustom AMethod, void* AObject)
                  { return Add(mtCustom, AMethod, AObject, NULL, NULL, "", "", ""); }

   // 添加 TDoStr1 类型的方法到执行线程
   bool           AddOfStr1(TDoStr1 AMethod, void* AObject, const KYString& AStr)
                  { return Add(mtStr1, (TDoCustom)AMethod, AObject,
                                       NULL, NULL, AStr, "", ""); }

   // 添加 TDoStr2 类型的方法到执行线程
   bool           AddOfStr2(TDoStr2 AMethod, void* AObject, const KYString& AStr1,
                                                            const KYString& AStr2)
                  { return Add(mtStr2, (TDoCustom)AMethod, AObject,
                                       NULL, NULL, AStr1, AStr2, ""); }

   // 添加 TDoStr3 类型的方法到执行线程
   bool           AddOfStr3(TDoStr3 AMethod, void* AObject, const KYString& AStr1,
                                                            const KYString& AStr2,
                                                            const KYString& AStr3)
                  { return Add(mtStr3, (TDoCustom)AMethod, AObject,
                                       NULL, NULL, AStr1, AStr2, AStr3); }

   // 添加 TDoInt1 类型的方法到执行线程
   bool           AddOfInt1(TDoInt1 AMethod, void* AObject, long AInt)
                  { return Add(mtInt1, (TDoCustom)AMethod, AObject, (void*)AInt,
                                       NULL, "", "", ""); }

   // 添加 mtInt1Str1 类型的方法到执行线程
   bool           AddOfInt1Str1(TDoInt1Str1 AMethod, void* AObject, long AInt1,
                                                         const KYString& AStr)
                  { return Add(mtInt1Str1, (TDoCustom)AMethod, AObject,
                                           (void*)AInt1, NULL, AStr, "", ""); }

   // 添加 TDoInt1Str2 类型的方法到执行线程
   bool           AddOfInt1Str2(TDoInt1Str2 AMethod, void* AObject, long AInt1,
                                                         const KYString& AStr1,
                                                         const KYString& AStr2)
                  { return Add(mtInt1Str2, (TDoCustom)AMethod, AObject,
                                           (void*)AInt1, NULL, AStr1, AStr2, ""); }

   // 添加 TDoInt1Str3 类型的方法到执行线程
   bool           AddOfInt1Str3(TDoInt1Str3 AMethod, void* AObject, long AInt1,
                                                         const KYString& AStr1,
                                                         const KYString& AStr2,
                                                         const KYString& AStr3)
                  { return Add(mtInt1Str3, (TDoCustom)AMethod, AObject,
                                           (void*)AInt1, NULL, AStr1, AStr2, AStr3); }

   // 添加 TDoInt2 类型的方法到执行线程
   bool           AddOfInt2(TDoInt2 AMethod, void* AObject, long AInt1, long AInt2)
                  { return Add(mtInt2, (TDoCustom)AMethod, AObject, (void*)AInt1,
                                       (void*)AInt2, "", "", ""); }

   // 添加 TDoInt2Str1 类型的方法到执行线程
   bool           AddOfInt2Str1(TDoInt2Str1 AMethod, void* AObject, long AInt1, long AInt2,
                                                         const KYString& AStr)
                  { return Add(mtInt2Str1, (TDoCustom)AMethod, AObject,
                                           (void*)AInt1, (void*)AInt2, AStr, "", ""); }

   // 添加 TDoInt2Str2 类型的方法到执行线程
   bool           AddOfInt2Str2(TDoInt2Str2 AMethod, void* AObject, long AInt1, long AInt2,
                                                         const KYString& AStr1,
                                                         const KYString& AStr2)
                  { return Add(mtInt2Str2, (TDoCustom)AMethod, AObject,
                                           (void*)AInt1, (void*)AInt2, AStr1, AStr2, ""); }

   // 添加 TDoInt2Str3 类型的方法到执行线程
   bool           AddOfInt2Str3(TDoInt2Str3 AMethod, void* AObject, long AInt1, long AInt2,
                                                         const KYString& AStr1,
                                                         const KYString& AStr2,
                                                         const KYString& AStr3)
                  { return Add(mtInt2Str3, (TDoCustom)AMethod, AObject,
                                           (void*)AInt1, (void*)AInt2, AStr1, AStr2, AStr3); }

   // 添加 TDoFloat 类型的方法到执行线程
   bool           AddOfFloat(TDoFloat AMethod, void* AObject, double AFloat)
                  { return Add(mtFloat, (TDoCustom)AMethod, AObject,
                                        AFloat, "", "", ""); }

   // 添加 TDoF1Str1 类型的方法到执行线程
   bool           AddOfF1Str1(TDoF1Str1 AMethod, void* AObject, double AFloat,
                                                         const KYString& AStr)
                  { return Add(mtF1Str1, (TDoCustom)AMethod, AObject,
                                         AFloat, AStr, "", ""); }

   // 添加 TDoF1Str2 类型的方法到执行线程
   bool           AddOfF1Str2(TDoF1Str2 AMethod, void* AObject, double AFloat,
                                                         const KYString& AStr1,
                                                         const KYString& AStr2)
                  { return Add(mtF1Str2, (TDoCustom)AMethod, AObject,
                                         AFloat, AStr1, AStr2, ""); }

   // 添加 TDoF1Str3 类型的方法到执行线程
   bool           AddOfF1Str3(TDoF1Str3 AMethod, void* AObject, double AFloat,
                                                         const KYString& AStr1,
                                                         const KYString& AStr2,
                                                         const KYString& AStr3)
                  { return Add(mtF1Str3, (TDoCustom)AMethod, AObject,
                                         AFloat, AStr1, AStr2, AStr3); }

   // 添加 TDoInt64 类型的方法到执行线程
   bool           AddOfInt64(TDoInt64 AMethod, void* AObject, __int64 AInt64)
                  { return Add(mtInt64, (TDoCustom)AMethod, AObject,
                                        AInt64, "", "", ""); }

   // 添加 TDoI64Str1 类型的方法到执行线程
   bool           AddOfI64Str1(TDoI64Str1 AMethod, void* AObject, __int64 AInt64,
                                                          const KYString& AStr)
                  { return Add(mtI64Str1, (TDoCustom)AMethod, AObject,
                                          AInt64, AStr, "", ""); }

   // 添加 TDoI64Str2 类型的方法到执行线程
   bool           AddOfI64Str2(TDoI64Str2 AMethod, void* AObject, __int64 AInt64,
                                                          const KYString& AStr1,
                                                          const KYString& AStr2)
                  { return Add(mtI64Str2, (TDoCustom)AMethod, AObject,
                                          AInt64, AStr1, AStr2, ""); }

   // 添加 TDoI64Str3 类型的方法到执行线程
   bool           AddOfI64Str3(TDoI64Str3 AMethod, void* AObject, __int64 AInt64,
                                                          const KYString& AStr1,
                                                          const KYString& AStr2,
                                                          const KYString& AStr3)
                  { return Add(mtI64Str3, (TDoCustom)AMethod, AObject,
                                          AInt64, AStr1, AStr2, AStr3); }

   // 添加 TDoP1 类型的方法到执行线程
   bool           AddOfP1(TDoP1 AMethod, void* AObject, void* AValue)
                  { return Add(mtP1, (TDoCustom)AMethod, AObject, AValue,
                                     NULL, "", "", ""); }

   // 添加 mtP1Str1 类型的方法到执行线程
   bool           AddOfP1Str1(TDoP1Str1 AMethod, void* AObject, void* AValue1,
                                                      const KYString& AStr)
                  { return Add(mtP1Str1, (TDoCustom)AMethod, AObject,
                                         AValue1, NULL, AStr, "", ""); }

   // 添加 TDoP1Str2 类型的方法到执行线程
   bool           AddOfP1Str2(TDoP1Str2 AMethod, void* AObject, void* AValue1,
                                                      const KYString& AStr1,
                                                      const KYString& AStr2)
                  { return Add(mtP1Str2, (TDoCustom)AMethod, AObject,
                                         AValue1, NULL, AStr1, AStr2, ""); }

   // 添加 TDoP1Str3 类型的方法到执行线程
   bool           AddOfP1Str3(TDoP1Str3 AMethod, void* AObject, void* AValue1,
                                                      const KYString& AStr1,
                                                      const KYString& AStr2,
                                                      const KYString& AStr3)
                  { return Add(mtP1Str3, (TDoCustom)AMethod, AObject,
                                         AValue1, NULL, AStr1, AStr2, AStr3); }

   // 添加 TDoP2 类型的方法到执行线程
   bool           AddOfP2(TDoP2 AMethod, void* AObject, void* AValue1, void* AValue2)
                  { return Add(mtP2, (TDoCustom)AMethod, AObject, AValue1,
                                     AValue2, "", "", ""); }

   // 添加 TDoP2Str1 类型的方法到执行线程
   bool           AddOfP2Str1(TDoP2Str1 AMethod, void* AObject, void* AValue1,
                                  void* AValue2,      const KYString& AStr)
                  { return Add(mtP2Str1, (TDoCustom)AMethod, AObject,
                                         AValue1, AValue2, AStr, "", ""); }

   // 添加 TDoP2Str2 类型的方法到执行线程
   bool           AddOfP2Str2(TDoP2Str2 AMethod, void* AObject, void* AValue1,
                                  void* AValue2,      const KYString& AStr1,
                                                      const KYString& AStr2)
                  { return Add(mtP2Str2, (TDoCustom)AMethod, AObject,
                                         AValue1, AValue2, AStr1, AStr2, ""); }

   // 添加 TDoP2Str3 类型的方法到执行线程
   bool           AddOfP2Str3(TDoP2Str3 AMethod, void* AObject, void* AValue1,
                                  void* AValue2,      const KYString& AStr1,
                        const KYString& AStr2,        const KYString& AStr3)
                  { return Add(mtP2Str3, (TDoCustom)AMethod, AObject,
                                         AValue1, AValue2, AStr1, AStr2, AStr3); }

protected:
   // 压入锁
   void           LockPush() const     { FLockPush->Enter(); }
   void           UnlockPush() const   { FLockPush->Leave(); }

   // 弹出锁
   void           LockPop() const      { FLockPop->Enter(); }
   void           UnlockPop() const    { FLockPop->Leave(); }

   // 引用计数(在 TKYRunThreads 中使用, 一般不需要调用)
   long           IncRefCount()        { return InterlockedIncrement(&FRefCount); }
   long           DecRefCount()        { return InterlockedDecrement(&FRefCount); }

   // 线程执行体
   virtual void   Execute();

private:
   // 添加方法项
   bool           Add(TMethodType AType, TDoCustom AMethod, void* AObject,
                            void* AP1, void* AP2, const KYString& AStr1,
                  const KYString& AStr2,          const KYString& AStr3);

   bool           Add(TMethodType AType, TDoCustom AMethod, void* AObject,
                          __int64 AInt64,         const KYString& AStr1,
                  const KYString& AStr2,          const KYString& AStr3);

   bool           Add(TMethodType AType, TDoCustom AMethod, void* AObject,
                           double AFloat,         const KYString& AStr1,
                  const KYString& AStr2,          const KYString& AStr3);

   // 执行方法的方法
   void           DoMethodOfCustom(const TItem& AItem);
   void           DoMethodOfStr1(const TItem& AItem);
   void           DoMethodOfStr2(const TItem& AItem);
   void           DoMethodOfStr3(const TItem& AItem);

   void           DoMethodOfInt1(const TItem& AItem);
   void           DoMethodOfInt1Str1(const TItem& AItem);
   void           DoMethodOfInt1Str2(const TItem& AItem);
   void           DoMethodOfInt1Str3(const TItem& AItem);

   void           DoMethodOfInt2(const TItem& AItem);
   void           DoMethodOfInt2Str1(const TItem& AItem);
   void           DoMethodOfInt2Str2(const TItem& AItem);
   void           DoMethodOfInt2Str3(const TItem& AItem);

   void           DoMethodOfFloat(const TItem& AItem);
   void           DoMethodOfF1Str1(const TItem& AItem);
   void           DoMethodOfF1Str2(const TItem& AItem);
   void           DoMethodOfF1Str3(const TItem& AItem);

   void           DoMethodOfInt64(const TItem& AItem);
   void           DoMethodOfI64Str1(const TItem& AItem);
   void           DoMethodOfI64Str2(const TItem& AItem);
   void           DoMethodOfI64Str3(const TItem& AItem);

   void           DoMethodOfP1(const TItem& AItem);
   void           DoMethodOfP1Str1(const TItem& AItem);
   void           DoMethodOfP1Str2(const TItem& AItem);
   void           DoMethodOfP1Str3(const TItem& AItem);

   void           DoMethodOfP2(const TItem& AItem);
   void           DoMethodOfP2Str1(const TItem& AItem);
   void           DoMethodOfP2Str2(const TItem& AItem);
   void           DoMethodOfP2Str3(const TItem& AItem);

private:
   // 新建和删除项
   TItem*         NewItem();
   void           DeleteItem(TItem* AItem);

private:
   void*          FData;               // 自定义数据
   long           FItemNo;             // 项号
   long           FRefCount;           // 引用计数
   long           FPerfCount;          // 执行点计数
   TKYCritSect*   FLockPush;           // 压入锁
   TKYCritSect*   FLockPop;            // 弹出锁
   TKYEvent*      FNotify;             // 通知
   TKYCycQueue*   FQueue;              // 消息队列
   TKYCycQueue*   FCache;              // 空闲队列

private:
   // 执行方法的方法类型
   typedef void (TKYRunThread::*TDoMethod)(const TItem& AItem);

   // 执行方法的方法列表
   static const TDoMethod  _Type_Methods[28];
   friend class TKYRunThreads;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYRunThreads - 运行线程集类 */

class TKYRunThreads
{
public:
   TKYRunThreads(long AMaxCount = 1, long ACacheSize = 10);
   virtual ~TKYRunThreads();

   // 属性
   void*          Data() const         { return FData; }             // default: NULL
   long           Count() const        { return FThreads->Count(); } // default: 0
   long           MaxCount() const     { return FMaxCount; }         // default: AMaxCount
   long           CacheSize() const    { return FCaches->MaxCount();}// default: ACacheSize

   // 设置属性
   void           SetData(void* AData) { FData = AData; }

   // 设置线程最大个数, 使用 NewThread 时根据线程的引用计数分配线程
   void           SetMaxCount(long AMaxCount);

   // 设置线程缓冲池尺寸
   void           SetCacheSize(long ACacheSize);

   // 分配线程
   // 1. 若 AThreadNo 为 0 表示动态分配, 否则指定线程号分配线程
   // 2. 若指定线程号 AThreadNo, 则取值范围: [1..MaxCount()]
   // 3. NewThread 的线程释放必须调用 DeleteThread 方法, 否则会使线程对象指针为
   //              野指针而导致系统崩溃
   TKYRunThread*  NewThread(long AThreadNo = 0);

   // 删除线程, 线程对象指针必须是 NewThread 分配的线程
   void           DeleteThread(TKYRunThread* AThread);

   // 清除所有线程
   void           ClearThreads();

protected:
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

private:
   // 分配/释放线程
   TKYRunThread*  DoNew(long AThreadNo);
   void           DoDelete(TKYRunThread* AThread);

private:
   void*          FData;
   TKYCritSect*   FLock;
   TKYCycQueue*   FCaches;
   TKYHandles*    FThreads;
   long           FMaxCount;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 公共函数 */

// 关闭并释放运行线程
void FreeKYRunThread(TKYRunThread* &AThread, Longword ATimeout = 15000);

}

#endif
