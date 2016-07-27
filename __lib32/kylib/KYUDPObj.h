// =======================================
// Unit   : UDP client/server
// Version: 3.0.1.0 (build 2013.06.23)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYUDPObj_H_
#define _KYUDPObj_H_

#include "KYSockObj.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYCustomUDP - UDP 基类 */

class TKYCustomUDP
{
   friend class TKYUDPRecvThread;

public:
   TKYCustomUDP();
   virtual ~TKYCustomUDP();

   // 属性
   void*          Data() const            { return FData; }       // default: NULL
   TSocket        Handle() const          { return FHandle; }     // default: INVALID_SOCKET
   bool           Active() const          { return FHandle != INVALID_SOCKET; }
   int            ErrorCode() const       { return FErrorCode; }  // default: 0
   bool           BroadcastEnabled() const{ return FBroadcastEnabled; }// default: false

   // 设置属性
   void           SetData(void* AData)    { FData = AData; }
   void           SetBroadcastEnabled(bool AEnabled);

   // 打开/关闭
   virtual bool   Open() = 0;
   virtual bool   Close() = 0;

   // 发送广播数据
   bool           Broadcast(const KYString& AData, long APort);

   // 发送 UDP 数据, 返回发送数据尺寸
   long           SendBuffer(const KYString& AHost, long APort,
                                 const void* ABuffer, long ACount);
   long           SendText(const KYString& AHost, long APort, const KYString& AText);

   // 接收数据长度
   long           RecvLength();

   // 接收 UDP 数据, 返回接收数据尺寸
   long           RecvBuffer(void* ABuffer, long ACount);
   long           RecvBuffer(void* ABuffer, long ACount,
                         KYString& APeerIP, long& APeerPort);

   // 接收 UDP 数据, 返回接收数据
   KYString       RecvText();
   KYString       RecvText(KYString& APeerIP, long& APeerPort);

protected:
   void           SetBroadcastFlag(bool AEnabled);

   bool           NewSocket();
   bool           CloseSocket();
   bool           TryBind(TSocket AHandle, const KYString& AHost, long APort);

   // 事件
   TNotifyEvent   OnDestroy;

protected:
   void*          FData;
   TSocket        FHandle;
   int            FErrorCode;
   bool           FBroadcastEnabled;

public:
   // 本地主机名
   static KYString   LocalHost()       { return TKYBaseSocket::LocalHost(); }

   // 根据句柄取当前 IP 和端口号
   static bool       LocalIPPort(TSocket AHandle, KYString& AIP, long& APort)
                     { return TKYBaseSocket::LocalIPPort(AHandle, AIP, APort); }

   // 查找主机名并返回地址
   static TInAddr    LookupHost(const KYString& AHost)
                     { return TKYBaseSocket::LookupHost(AHost); }

   // 解析主机名的 IP 地址
   static KYString   ResolveHost(const KYString& AHost)
                     { return TKYBaseSocket::ResolveHost(AHost); }

   // 初始化 socket
   static bool       InitSocket(TSockAddrIn& Addr, const KYString& AHost,
                                        long APort, bool AnyAddr = false)
                     { return TKYBaseSocket::InitSocket(Addr, AHost, APort, AnyAddr); }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYUDPClient - UDP 客户端类 */

class TKYUDPClient: public TKYCustomUDP
{
public:
   // 打开 UDP 客户端, 若打开成功则返回 true, 客户端只负责发送数据不能接收数据
   virtual bool   Open();

   // 关闭 UDP 客户端
   virtual bool   Close();

protected:
private:
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYUDPServer - UDP 服务端类 */

class TKYUDPServer: public TKYCustomUDP
{
public:
   TKYUDPServer();
   virtual ~TKYUDPServer();

   // 属性
   const KYString CurrIP() const       { return FCurrIP; }  // default: ""
   long           CurrPort() const     { return FCurrPort; }// default: 0

   const KYString Host() const         { return FHost; }    // default: ""
   long           Port() const         { return FPort; }    // default: 0

   // 设置接收 UDP 的主机名或 IP 地址
   bool           SetHost(const KYString& AHost);

   //  设置接收 UDP 的端口号
   bool           SetPort(long APort);

   // 打开 UDP 服务端, 若打开成功则返回 true, 则服务端能够接收和发送数据
   virtual bool   Open();

   // 关闭 UDP 服务端
   virtual bool   Close();

protected:
private:
   KYString       FCurrIP;
   long           FCurrPort;

   KYString       FHost;
   long           FPort;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYUDPRecvThread - UDP 接收线程类 */

class TKYUDPRecvThread: public TThread
{
private:
   // UDP 项
   typedef struct
   {
      void*          Sender;
      TSocket        Handle;
   } TItem, *PItem;

   // 映射列表类型
   typedef TKYMapObjKey<TItem>         TItems;
   typedef TKYMapIntKey<TKYCustomUDP>  TUDPList;

public:
   // TOnReceive 接收事件
   typedef void (TObject::*TDoReceive)(void* Sender, TKYCustomUDP* UDP);
   typedef struct
   {
      TDoReceive     Method;
      void*          Object;
   } TOnReceive;

public:
   TKYUDPRecvThread();
   virtual ~TKYUDPRecvThread();

   // 自定义数据, 默认值为: NULL
   void*          Data() const         { return FData; }

   // 加入接收线程的 UDP 项数
   long           Count() const        { return FItems->Count(); }

   // 等待接收超时(毫秒), 默认值为: 15000
   // 除第一项以外, 后面加入线程项到开始接收时长大概为 Timeout() / 2
   Longword       Timeout() const      { return FTimeout; }

   // 设置自定义数据
   void           SetData(void* AData) { FData = AData; }

   // 设置等待接收超时(毫秒)
   // 若是一个接收线程负责多路 UDP 接收, 建议不要设太长时间
   void           SetTimeout(Longword ATimeout)
                  { FTimeout = ATimeout; }

   // 通知线程开始接收, 即在调用 Add(...) 方法之后, 需要调用 Notify() 通知
   void           Notify() const       { FNotify->Set(); }

   // 关闭线程, 建议 FreeKYUDPRecvThread 释放线程
   void           Close();

   // 清除所有接收 UDP 项
   void           Clear();

   // 判断 UDP 项是否存在
   bool           IsExist(TKYCustomUDP* UDP) const;

   // 加入 UDP 项
   bool           Add(void* Sender, TKYCustomUDP* UDP);

   // 删除 UDP 项
   bool           Delete(TKYCustomUDP* UDP);

   // 事件
   TNotifyEvent   OnThreadStart;       // 线程开始时激发的事件
   TNotifyEvent   OnThreadEnd;         // 线程结束前激发的事件
   TNotifyEvent   OnTimeout;           // 接收超时时激发的事件
   TOnReceive     OnReceive;           // 接收到数据时激发的事件

protected:
   // 当前锁
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

   // 线程执行体
   virtual void   Execute();

private:
   void           DoUDPDestroy(void* Sender);
   void           DoUDPRecv(TSocket AHandle);

   long           ReadFDSet();
   void           WaitUDPRecv();

private:
   void*          FData;               // 自定义数据
   TKYCritSect*   FLock;               // 当前锁
   TItems*        FItems;              // UDP 列表项
   TUDPList*      FFDList;             // 句柄列表
   TKYEvent*      FNotify;             // 通知事件

   TFDSet*        FFDSet;
   long           FFDCount;
   Longword       FTimeout;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 公共函数 */

// 关闭并释放 UDP 接收线程
void FreeKYUDPRecvThread(TKYUDPRecvThread* &AThread, Longword ATimeout = 15000);

}

#endif
