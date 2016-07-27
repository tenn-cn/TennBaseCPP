// =======================================
// Unit   : TCP 发送线程组 (TCPSendThreads.h)
// Version: 3.0.0.0 (build 2011.03.08)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _TCPSendThreads_H_
#define _TCPSendThreads_H_

#include "KYSockObj.h"
#include "KYMemBlock.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TTCPSendThread - TCP 发送线程类 */

class TTCPSendThread: public TThread
{
public:
   TTCPSendThread();
   virtual ~TTCPSendThread();

   // 读属性
   void*          Data() const         { return FData; }          // default: NULL
   long           Count() const        { return FItems->Count(); }// default: 0

   // 设置属性
   void           SetData(void* AData) { FData = AData; }

   // 关闭线程, 建议 FreeTCPSendThread 释放线程
   void           Close();

   // 清除所有发送句柄
   void           Clear();

   // 添加 TCP 到发送线程, 并返回发送句柄
   long           Add(void* Sender, TKYBaseSocket* TCP);

   // 设置发送句柄是否需要发送数据
   bool           Change(long ASendID, bool ANeedSend);

   // 删除发送句柄
   void           Delete(long ASendID);

   // 事件
   TOnSocketEvent OnSend;              // 当 Change(.., true) 时激发的事件

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
   // TCP 发送项
   #pragma pack(push, 1)
   typedef struct
   {
      void*          Sender;
      TKYBaseSocket* TCP;
      bool           NeedSend;
   } TSendItem, *PSendItem;
   #pragma pack(pop)

   // 发送项管理器, 存放发送项 TSendItem
   static TKYMemBlock*  Inner_ItemManager;

   // TTCPSendThread 的静态成员初始化类
   static class TInitialization
   {
   public:
      TInitialization();
      ~TInitialization();
   } Inner_Initialization;

   friend class TInitialization;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TTCPSendThreads - TCP 发送线程集类 */

class TTCPSendThreads
{
public:
   // TOnAddThread 接收事件
   typedef void (TObject::*TDoAddThread)(void* Sender, TTCPSendThread* AThread);
   typedef struct
   {
      TDoAddThread   Method;
      void*          Object;
   } TOnAddThread;

public:
   TTCPSendThreads();
   ~TTCPSendThreads();

   // 读属性
   long              ThreadCount() const     { return FThreads->Count(); } // default: 0
   long              MaxThreadCount() const  { return FMaxThreadCount; }   // default: 1

   // 设置属性
   void              SetMaxThreadCount(long ACount);

   // 添加 TCP 到发送线程, 并返回发送线程及发送句柄
   TTCPSendThread*   AddThread(void* Sender, TKYBaseSocket* TCP, long& ASendID);

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

// 关闭并释放 TCP 发送线程
void FreeTCPSendThread(TTCPSendThread* &AThread, Longword ATimeout = 15000);

}

#endif