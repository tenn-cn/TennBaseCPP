// =======================================
// Unit   : TCP client/server
// Version: 3.0.4.0 (build 2013.11.02)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYTCPObj_H_
#define _KYTCPObj_H_

#include "KYSockObj.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYTCPClient - TCP客户端类 */

class TKYTCPClient: public TKYBaseSocket
{
public:
   TKYTCPClient();
   virtual ~TKYTCPClient();

   // 属性
   virtual bool      Active() const;                                 // default: false
   KYString          Host() const         { return FHost; }          // default: ""
   long              Port() const         { return FPort; }          // default: 0
   long              Linger() const       { return FLinger; }        // default: -1
   long              Timeout() const      { return FTimeout; }       // default: -1
   KYString          BindHost() const     { return FBindHost; }      // default: ""
   long              BindPort() const     { return FBindPort; }      // default: 0
   int               ErrorCode() const    { return FErrorCode; }     // default: 0
   KYString          ErrorInfo() const    { return FErrorInfo; }     // default: ""

   bool              NoDelay() const      { return FNoDelay; }       // default: false
   bool              Nonblocking() const  { return FNonblocking; }   // default: false

   bool              KeepIsAlive() const  { return FKeepIsAlive; }   // defalut: false
   long              KeepIdle() const     { return FKeepIdle; }      // default: 30
   long              KeepInterval() const { return FKeepInterval; }  // default: 10
   long              KeepCount() const    { return FKeepCount; }     // default: 3

   // 设置属性
   void              SetHost(const KYString& AHost);
   void              SetPort(long APort);
   void              SetLinger(long ALinger);
   void              SetTimeout(long ATimeout);
   void              SetBindHost(const KYString& AHost);
   void              SetBindPort(long APort);

   void              SetNoDelay(bool AEnabled);
   void              SetNonblocking(bool AEnabled);

   void              SetKeepIsAlive(bool AValue)
                     { if (!FActive) FKeepIsAlive = AValue; }
   void              SetKeepIdle(long AValue)
                     { if (!FActive) FKeepIdle = AValue; }
   void              SetKeepInterval(long AValue)
                     { if (!FActive) FKeepInterval = AValue; }
   void              SetKeepCount(long AValue)
                     { if (!FActive) FKeepCount = AValue; }

   // 加锁
   virtual void      Lock() const;

   // 解锁
   virtual void      Unlock() const;

   // 打开连接, 若连接成功则激发 OnConnect 事件
   virtual bool      Open();

   // 关闭连接, 激发 OnDisconnect 事件
   virtual void      Close();

   // 事件
   TOnSocketEvent    OnConnect;        // 连接成功时激发的事件
   TOnSocketEvent    OnDisconnect;     // 断开连接时激发的事件

protected:
   void              DoConnect();
   void              DoDisconnect();

private:
   TKYCritSect*      FLock;            // 锁
   KYString          FHost;            // 连接 Server 的主机名或地址
   long              FPort;            // 连接 Server 的端口号
   long              FLinger;          // Close 后拖延时长(秒), 默认值为: -1
   long              FTimeout;         // Open  超时(毫秒), 默认值为: -1
   KYString          FBindHost;        // 绑定的 IP 地址或主机名
   long              FBindPort;        // 绑定的端口号

   bool              FActive;          // 是否打开
   bool              FNoDelay;         // 是否 NoDelay
   bool              FNonblocking;     // 是否 Nonblocking
   bool              FKeepIsAlive;     // 是否 KeepAlive 检测
   long              FKeepIdle;        // 首次 KeepAlive 检测前的TCP空闲时间(秒)
   long              FKeepInterval;    // 两次 KeepAlive 检测间的时间间隔(秒)
   long              FKeepCount;       // 检测尝试的次数, 如果检测包收到响应了, 则后面几次不再发送
   int               FErrorCode;       // 错误码
   KYString          FErrorInfo;       // 错误信息
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYTCPSrvClient - TCP服务端的客户连接类 */

class TKYTCPSrvClient: public TKYBaseSocket
{
   friend class TKYTCPServer;

public:
   TKYTCPSrvClient(TKYTCPServer* AServer = NULL, TSocket AHandle = INVALID_SOCKET);
   virtual ~TKYTCPSrvClient();

   // FServer 中的客户端连接 ID
   long              ClientID() const  { return FClientID; }

   // 加锁
   virtual void      Lock() const;

   // 解锁
   virtual void      Unlock() const;

   // 返回 Active()
   virtual bool      Open();

   // 关闭连接
   virtual void      Close();

protected:
   void              DoDisconnect();

private:
   TKYCritSect*      FLock;
   TKYTCPServer*     FServer;
   long              FClientID;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYTCPServer - TCP服务端类 */

class TKYTCPServer: public TKYBaseSocket
{
   friend class TKYTCPSrvClient;

public:
   TKYTCPServer();
   virtual ~TKYTCPServer();

   // 属性
   virtual bool         Active() const;                                 // default: false
   KYString             Host() const         { return FHost; }          // default: ""
   long                 Port() const         { return FPort; }          // default: 0
   long                 Linger() const       { return FLinger; }        // default: -1
   long                 QueueSize() const    { return FQueueSize; }     // default: SOMAXCONN
   int                  ErrorCode() const    { return FErrorCode; }     // default: 0
   KYString             ErrorInfo() const    { return FErrorInfo; }     // default: ""
   TKYSockRecvThreads*  RecvThreads() const  { return FRecvThreads; }   // 需要设置事件

   bool                 NoDelay() const      { return FNoDelay; }       // default: false
   bool                 Nonblocking() const  { return FNonblocking; }   // default: false

   bool                 KeepIsAlive() const  { return FKeepIsAlive; }   // defalut: false
   long                 KeepIdle() const     { return FKeepIdle; }      // default: 30
   long                 KeepInterval() const { return FKeepInterval; }  // default: 10
   long                 KeepCount() const    { return FKeepCount; }     // default: 3

   // 客户端连接个数
   long                 ClientCount() const  { return FClients->Count(); }

   // 取句柄对应的客户端连接, 若句柄不存在则返回 NULL
   TKYTCPSrvClient*     Client(long AClientID) const;

   // 根据索引读取客户端连接, AIndex 取值范围: [0..ClientCount()-1],
   // 因要循环查找所以不建议使用
   TKYTCPSrvClient*     Connection(long AIndex) const;

   // 设置属性
   void                 SetHost(const KYString& AHost);
   void                 SetPort(long APort);
   void                 SetLinger(long ALinger);
   void                 SetQueueSize(long AQueueSize);

   void                 SetNoDelay(bool AEnabled);
   void                 SetNonblocking(bool AEnabled);

   void                 SetKeepIsAlive(bool AValue)
                        { if (!FActive) FKeepIsAlive = AValue; }
   void                 SetKeepIdle(long AValue)
                        { if (!FActive) FKeepIdle = AValue; }
   void                 SetKeepInterval(long AValue)
                        { if (!FActive) FKeepInterval = AValue; }
   void                 SetKeepCount(long AValue)
                        { if (!FActive) FKeepCount = AValue; }

   // 加锁
   virtual void         Lock() const;

   // 解锁
   virtual void         Unlock() const;

   // 打开侦听, 若成功则激发 OnListen 事件
   virtual bool         Open();

   // 关闭侦听并同时关闭所有连接, 激发 OnClientDisconnect 和 OnDisconnect 事件
   virtual void         Close();

   // 关闭所有客户端连接
   void                 CloseClients();

   // 第一个客户端连接句柄
   long                 FirstID() const;

   // 上一个客户端连接句柄
   long                 PriorID() const;

   // 下一个客户端连接句柄
   long                 NextID() const;

   // 最后一个客户端连接句柄
   long                 LastID() const;

   // 指定句柄的上一个客户端连接句柄
   long                 PriorID(long AClientID) const;

   // 指定句柄的下一个客户端连接句柄
   long                 NextID(long AClientID) const;

   // 事件
   TOnSocketEvent       OnListen;            // 打开侦听成功时激发的事件
   TOnSocketEvent       OnDisconnect;        // 关闭侦听时激发的事件

   // OnAccept 在 OnClientConnect 及 OnReceive 之前激发,
   // 而 OnClientConnect 不一定在 OnReceive 之前激发
   TOnSocketEvent       OnAccept;            // 客户端开始连接时激发的事件
   TOnSocketEvent       OnClientConnect;     // 客户端已连接时激发的事件
   TOnSocketEvent       OnClientDisconnect;  // 客户端断开连接时激发的事件

protected:
   void                 DoListen();
   void                 DoDisconnect();

   void                 DoAccept(TKYTCPSrvClient* AClient);
   void                 DoClientConnect(TKYTCPSrvClient* AClient);
   void                 DoClientDisconnect(TKYTCPSrvClient* AClient);

private:
   unsigned             ExecuteAccept(TKYThread* AThread, void* AParam);

   bool                 DoCopyAndClear(TKYList& AList);
   void                 DoClientDeletion(TKYTCPSrvClient* AClient);

private:
   TKYCritSect*         FLock;               // 锁
   TKYHandles*          FClients;            // 客户端连接列表
   TKYSockRecvThreads*  FRecvThreads;        // 接收线程集
   TKYThread*           FAcceptThread;       // 接受连接的线程

   KYString             FHost;               // 绑定的 IP 地址或主机名
   long                 FPort;               // 侦听的端口号
   long                 FLinger;             // Close 后拖延时长(秒), 默认值为: -1
   long                 FQueueSize;          // 接受客户端连接的队列尺寸

   bool                 FActive;             // 是否打开
   bool                 FNoDelay;            // 是否 NoDelay
   bool                 FNonblocking;        // 是否 Nonblocking
   bool                 FKeepIsAlive;        // 是否 KeepAlive 检测
   long                 FKeepIdle;           // 首次 KeepAlive 检测前的TCP空闲时间(秒)
   long                 FKeepInterval;       // 两次 KeepAlive 检测间的时间间隔(秒)
   long                 FKeepCount;          // 检测尝试的次数, 如果检测包收到响应了, 则后面几次不再发送
   int                  FErrorCode;          // 错误码
   KYString             FErrorInfo;          // 错误信息
};

}

#endif
