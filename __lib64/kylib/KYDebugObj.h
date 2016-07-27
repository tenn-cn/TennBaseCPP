// =======================================
// Unit   : Debug client object (.h)
// Version: 3.0.0.0 (build 2011.03.13)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYDebugObj_H_
#define _KYDebugObj_H_

#include "KYThread.h"
#include "KYSeqObj.h"
#include "KYMemBlock.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYDebugClient - Debug 的客户端线程类 */

class TKYDebugClient: public TThread
{
   // Debug 头信息
   typedef struct
   {
      int         Size;                // 头尺寸
      Longword    LastTick;            // Tick
      Longword    QueueSize;           // 队列尺寸
      Longword    DebugIndex;          // 索引
   } TDebugHead, *PDebugHead;

   // Debug 项
   typedef char   TDebugItem[256];

public:
   TKYDebugClient(const KYString& AName = "");
   virtual ~TKYDebugClient();

   // 判断 Debug 服务端是否已经打开
   bool           Opened() const       { return FOpened; }

   // 关闭线程, 建议 FreeKYDebugClient 释放线程
   void           Close();

   // 写 Debug 信息
   bool           Write(const KYString& AInfo);

protected:
   virtual void   Execute();

private:
   void           ClearQueue();

   void           OpenDebug();
   void           CloseDebug();
   void           CheckDebug();
   void           WriteDebug(long ACount);

private:
   // Client 的 Debug 项
   TDebugHead*    FHead;
   TDebugItem*    FItems;
   TKYEvent*      FEvent;
   TKYMutex*      FMutex;
   THandle        FHandle;
   KYString       FMapName;
   long           FMaxCount;

   // Debug 队列
   TKYQueue*      FQueue;
   TKYEvent*      FNotify;
   bool           FOpened;

private:
   // Debug 项管理器, 存放 Debug 项 TDebugItem
   static TKYMemBlock*  Inner_ItemManager;

   // TKYDebugClient 的静态成员初始化类
   static class TInitialization
   {
   public:
      TInitialization();
      ~TInitialization();
   } Inner_Initialization;

   friend class TInitialization;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 公共函数 */

// 关闭并释放 Debug 客户端线程
void FreeKYDebugClient(TKYDebugClient* &AThread, Longword ATimeout = 5000);

}

#endif
