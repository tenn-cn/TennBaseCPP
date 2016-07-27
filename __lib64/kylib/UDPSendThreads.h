// =======================================
// Unit   : UDP 发送线程组 (UDPSendThreads.h)
// Version: 3.0.0.0 (build 2011.03.09)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _UDPSendThreads_H_
#define _UDPSendThreads_H_

#include "KYUDPObj.h"
#include "KYMemBlock.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TUDPSendThread - UDP 发送线程类 */

class TUDPSendThread: public TThread
{
public:
   // TOnSend 接收事件
   typedef void (TObject::*TDoSend)(void* Sender, TKYCustomUDP* UDP);
   typedef struct
   {
      TDoSend     Method;
      void*       Object;
   } TOnSend;

public:
   TUDPSendThread();
   virtual ~TUDPSendThread();

   // 读属性
   void*          Data() const         { return FData; }          // default: NULL
   long           Count() const        { return FItems->Count(); }// default: 0

   // 设置属性
   void           SetData(void* AData) { FData = AData; }

   // 关闭线程, 建议 FreeUDPSendThread 释放线程
   void           Close();

   // 清除所有发送句柄
   void           Clear();

   // 添加 UDP 到发送线程, 并返回发送句柄
   long           Add(void* Sender, TKYCustomUDP* UDP);

   // 设置发送句柄是否需要发送数据
   bool           Change(long ASendID, bool ANeedSend);

   // 删除发送句柄
   void           Delete(long ASendID);

   // 事件
   TOnSend        OnSend;              // 当 Change(.., true) 时激发的事件

protected:
   virtual void   Execute();

   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

private:
   bool           SendNext(bool AIsFirst);
   bool           ExecSend();

   void           DoDeletion(long AHandle, void* AItem);

private:
   void*          FData;
   TKYCritSect*   FLock;
   TKYEvent*      FEvent;
   TKYHandles*    FItems;

   Longword       FIdleTicks;
   Longword       FIdleTimes;
   Longword       FSendTimes;

private:
   // UDP 发送项
   #pragma pack(push, 1)
   typedef struct
   {
      void*          Sender;
      TKYCustomUDP*  UDP;
      bool           NeedSend;
   } TUDPSendItem, *PUDPSendItem;
   #pragma pack(pop)

   // 发送项管理器, 存放发送项 TUDPSendItem
   static TKYMemBlock*  Inner_ItemManager;

   // TUDPSendThread 的静态成员初始化类
   static class TInitialization
   {
   public:
      TInitialization();
      ~TInitialization();
   } Inner_Initialization;

   friend class TInitialization;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TUDPSendThreads - UDP 发送线程集类 */

class TUDPSendThreads
{
public:
   // TOnAddThread 接收事件
   typedef void (TObject::*TDoAddThread)(void* Sender, TUDPSendThread* AThread);
   typedef struct
   {
      TDoAddThread   Method;
      void*          Object;
   } TOnAddThread;

public:
   TUDPSendThreads();
   ~TUDPSendThreads();

   // 读属性
   long              ThreadCount() const     { return FThreads->Count(); } // default: 0
   long              MaxThreadCount() const  { return FMaxThreadCount; }   // default: 1

   // 设置属性
   void              SetMaxThreadCount(long ACount);

   // 添加 UDP 到发送线程, 并返回发送线程及发送句柄
   TUDPSendThread*   AddThread(void* Sender, TKYCustomUDP* UDP, long& ASendID);

   // 清除所有发送线程
   void              ClearThreads();

   // 事件
   TOnAddThread      OnAddThread;            // AddThread 分配新线程时激发的事件

protected:
   void              Lock() const            { FLock->Enter(); }
   void              Unlock() const          { FLock->Leave(); }

private:
   TKYCritSect*      FLock;
   TKYHandles*       FThreads;
   long              FMaxThreadCount;
   bool              FFreeing;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 公共函数 */

// 关闭并释放 UDP 发送线程
void FreeUDPSendThread(TUDPSendThread* &AThread, Longword ATimeout = 15000);

}

#endif
