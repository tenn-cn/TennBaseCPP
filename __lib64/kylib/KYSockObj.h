// =======================================
// Unit   : socket 对象
// Version: 3.0.4.0 (build 2013.07.16)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYSockObj_H_
#define _KYSockObj_H_

#include "KYThread.h"
#include "KYHandles.h"
#include "KYMapObjs.h"
#include "KYMemBlock.h"
#include "KYCycQueue.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

class TKYBaseSocket;
class TKYSockRecvThread;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 重定义 socket 相关的类型 */

typedef SOCKET          TSocket;
typedef linger          TLinger;

typedef fd_set          TFDSet;
typedef timeval         TTimeVal;

typedef in_addr         TInAddr;
typedef sockaddr        TSockAddr;
typedef sockaddr_in     TSockAddrIn;
typedef sockproto       TSockProto;

typedef netent          TNetEnt;
typedef hostent         THostEnt;
typedef servent         TServEnt;
typedef protoent        TProtoEnt;

typedef OVERLAPPED      TOverlapped;

// TCP keep-alive option
typedef struct
{
   Longword             OnOff;         // 开关 {0: 不检测, 1: 检测}
   Longword             IdleTime;      // 首次 KeepAlive 检测前的TCP空闲时间(毫秒)
   Longword             Interval;      // 两次 KeepAlive 检测间的时间间隔(毫秒)
} TTCPKeepAlive;

// TOnSocketEvent 事件类型
typedef void (TObject::*TDoSocketEvent)(void* Sender, TKYBaseSocket* ASocket);
typedef struct
{
   TDoSocketEvent       Method;
   void*                Object;
} TOnSocketEvent;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 常量定义 */

// socket 的常量值
enum _TKYSocket_const
{
   Sock_BufferSize      = 8192,        // socket 接收缓冲区尺寸
   UDP_BufferSize       = 65515,       // UDP 的缓冲区尺寸,
                                       // 对于UDP协议来说, 整个包的最大长度为
                                       // 65535，其中IP包头长度20, 所以是 65515
};

// 异常信息
const char* const SockExcept_Startup   = "WSAStartup failure";

// socket 函数名(Function name)
const char* const SockFN_Socket        = "socket";
const char* const SockFN_Shutdown      = "shutdown";
const char* const SockFN_CloseSocket   = "closesocket";
const char* const SockFN_Connect       = "connect";
const char* const SockFN_Bind          = "bind";
const char* const SockFN_Listen        = "listen";
const char* const SockFN_Accept        = "accept";
const char* const SockFN_Select        = "select";
const char* const SockFN_Recv          = "recv";
const char* const SockFN_Send          = "send";
const char* const SockFN_RecvFrom      = "recvfrom";
const char* const SockFN_SendTo        = "sendto";
const char* const SockFN_GetSockOpt    = "getsockopt";
const char* const SockFN_SetSockOpt    = "setsockopt";
const char* const SockFN_IOCtlSocket   = "ioctlsocket";
const char* const SockFN_GetSockName   = "getsockname";
const char* const SockFN_GetPeerName   = "getpeername";
const char* const SockFN_GetHostName   = "gethostname";
const char* const SockFN_GetHostByAddr = "gethostbyaddr";
const char* const SockFN_GetHostByName = "gethostbyname";

// 地址常量
const KYString    Sock_LocalHost       = "LOCALHOST";
const KYString    Sock_LocalAddr       = "127.0.0.1";
const KYString    Sock_Broadcast       = "255.255.255.255";

// 错误信息
const KYString    SockErr_Call         = "call <socket::%s> error: %s (%d)";
const KYString    SockErr_NonOpened    = "socket is not opened";
const KYString    SockErr_NotAddress   = "the address is empty";
const KYString    SockErr_NotAccept    = "accept thread create failure";
const KYString    SockErr_NotChange    = "can't change attrib on opened socket";
const KYString    SockErr_NotCreate    = "can't create new socket";

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 公共函数 */

// 检查返回值
int   CheckSocketResult(int AResult, const char* AName, KYString& AError);

// 关闭并释放 socket 接收线程
void  FreeKYSockRecvThread(TKYSockRecvThread* &AThread, Longword ATimeout = 15000);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYBaseSocket - Socket 基类 */

class TKYBaseSocket
{
   friend class TKYSockRecvThread;

public:
   // Socket 类型(Type)
   enum TType     {stStream     = SOCK_STREAM,     // stream socket
                   stDatagram   = SOCK_DGRAM,      // datagram socket
                   stRaw        = SOCK_RAW,        // raw-protocol interface
                   stRDM        = SOCK_RDM,        // reliably-delivered message
                   stSeqPacket  = SOCK_SEQPACKET}; // sequenced packet stream

   // Socket 协议族(Protocol family)
   enum TDomain   {pfUnspec     = PF_UNSPEC,       // unspecified
                   pfUnix       = PF_UNIX,         // local to host (pipes, portals)
                   pfINet       = PF_INET,         // internetwork: UDP, TCP, etc.
                   pfImpLink    = PF_IMPLINK,      // arpanet imp addresses
                   pfPup        = PF_PUP,          // pup protocols: e.g. BSP
                   pfCHAOS      = PF_CHAOS,        // mit CHAOS protocols
                   pfNS         = PF_NS,           // XEROX NS protocols
                   pfIPX        = PF_IPX,          // IPX and SPX
                   pfISO        = PF_ISO,          // ISO protocols
                   pfOSI        = PF_OSI,          // OSI is ISO
                   pfECMA       = PF_ECMA,         // european computer manufacturers
                   pfDataKit    = PF_DATAKIT,      // datakit protocols
                   pfCCITT      = PF_CCITT,        // CCITT protocols, X.25 etc
                   pfSNA        = PF_SNA,          // IBM SNA
                   pfDECNet     = PF_DECnet,       // DEC net
                   pfDLI        = PF_DLI,          // Direct data link interface
                   pfLAT        = PF_LAT,          // LAT
                   pfHyLink     = PF_HYLINK,       // NSC Hyperchannel
                   pfAppleTalk  = PF_APPLETALK,    // AppleTalk
                   pfVoiceView  = PF_VOICEVIEW,    // VoiceView
                   pfFireFox    = PF_FIREFOX,      // FireFox
                   pfUnknown1   = PF_UNKNOWN1,     // Somebody is using this!
                   pfBan        = PF_BAN,          // Banyan
                   pfMax        = PF_MAX};         // protocol familiy maximum value

   // Socket 协议(Protocol)
   enum TProtocol {spIP         = IPPROTO_IP,      // dummy for IP
                   spICMP       = IPPROTO_ICMP,    // control message protocol
                   spIGMP       = IPPROTO_IGMP,    // group management protocol
                   spGGP        = IPPROTO_GGP,     // Gateway-to-Gateway protocol
                   spTCP        = IPPROTO_TCP,     // TCP
                   spPUP        = IPPROTO_PUP,     // PUP
                   spUDP        = IPPROTO_UDP,     // user datagram protocol
                   spIDP        = IPPROTO_IDP,     // xns IDP
                   spND         = IPPROTO_ND,      // UNOFFICIAL net disk proto
                   spRaw        = IPPROTO_RAW,     // raw IP packet
                   spMax        = IPPROTO_MAX};    // protocol maximum value

   // Socket 协议项
   typedef struct
   {
      Byte           Type;             // 类型
      Byte           Domain;           // 协议族
      Word           Protocol;         // 协议
   } TProto;

public:
   TKYBaseSocket();
   virtual ~TKYBaseSocket();

   // 属性
   virtual bool      Active() const;                        // default: false
   void*             Data() const      { return FData; }    // default: NULL
   TProto            Proto() const     { return FProto; }   // default: {stStream, pfUnspec, spIP}
   TSocket           Handle() const    { return FHandle; }

   // 取当前 IP 地址
   KYString          LocalAddr() const;

   // 取当前端口号
   long              LocalPort() const;

   // 取远端的主机名, 可能会很慢, 不建议调用
   KYString          RemoteHost();

   // 取远端的 IP 地址
   KYString          RemoteAddr() const;

   // 取远端的端口号
   long              RemotePort() const;

   // 设置属性
   void              SetData(void* AData)    { FData = AData; }
   void              SetProto(const TProto& AProto);
   void              SetActive(bool AValue)  { if (AValue) Open(); else Close(); }

   // 加锁
   virtual void      Lock() const;

   // 解锁
   virtual void      Unlock() const;

   // 打开
   virtual bool      Open();

   // 关闭
   virtual void      Close();

   // 发送数据
   long              SendBuffer(const void* ABuffer, long ACount) const
                     { return send(FHandle, (const char*)ABuffer, ACount, 0); }

   // 发送无连接的数据
   long              SendBuffer(const KYString& AHost, long APort,
                                    const void* ABuffer, long ACount) const;

   // 发送数据
   long              SendText(const KYString& AText) const
                     { return SendBuffer((char*)AText, AText.Length()); }

   // 发送无连接的数据
   long              SendText(const KYString& AHost, long APort,
                              const KYString& AText) const
                     { return SendBuffer(AHost, APort, (char*)AText, AText.Length()); }

   // 接收数据长度
   long              RecvLength() const;

   // 接收数据
   long              RecvBuffer(void* ABuffer, long ACount) const
                     { return recv(FHandle, (char*)ABuffer, ACount, 0); }

   // 接收无连接的数据
   long              RecvBuffer(void* ABuffer, long ACount,
                            KYString& APeerIP, long& APeerPort) const;

   // 接收数据
   KYString          RecvText() const;

   // 接收无连接的数据
   KYString          RecvText(KYString& APeerIP, long& APeerPort) const;

protected:
   bool              DoOpen();
   void              DoClose();

protected:
   // 事件
   TNotifyEvent      OnDestroy;

   TProto            FProto;
   TSocket           FHandle;

private:
   void*             FData;
   KYString          FRemoteHost;

public:
   // 本地主机名
   static KYString   LocalHost();

   // 根据句柄取当前 IP 和端口号
   static bool       LocalIPPort(TSocket AHandle, KYString& AIP, long& APort);

   // 根据句柄取远端 IP 和端口号
   static bool       RemoteIPPort(TSocket AHandle, KYString& AIP, long& APort);

   // 查找主机名并返回地址
   static TInAddr    LookupHost(const KYString& AHost);

   // 解析主机名的 IP 地址
   static KYString   ResolveHost(const KYString& AHost);

   // 初始化 socket
   static bool       InitSocket(TSockAddrIn& Addr, const KYString& AHost,
                                        long APort, bool AnyAddr = false);

   // 取错误码及错误信息
   static int        LastError(const char* AName, KYString& AError)
                     { return CheckSocketResult(-1, AName, AError); }

   // 新建 socket
   static TSocket    NewSocket(const TProto& AProto)
                     { return socket(AProto.Domain, AProto.Type, AProto.Protocol); }

   // 关闭 socket
   static bool       CloseSocket(TSocket AHandle)
                     {  // {SD_SEND: 0x01, SD_RECV: 0x02, SD_BOTH: 0x03}
                        //shutdown(AHandle, 0x01);
                        return (closesocket(AHandle) == 0);
                     }

   // 连接 socket(注: 带超时函数用于非阻塞 socket)
   static bool       Connect(TSocket AHandle, const KYString& AHost, long APort,
                            Longword ATimeout);
   static bool       Connect(TSocket AHandle, const KYString& AHost, long APort)
                     {
                        TSockAddrIn recAddr;
                        return InitSocket(recAddr, AHost, APort, false)
                              && (connect(AHandle, (TSockAddr*)&recAddr,
                                          sizeof(recAddr)) == 0);
                     }

   // 试着句柄绑定 IP 和端口号
   static bool       TryBind(TSocket AHandle, const KYString& AHost, long APort)
                     {
                        TSockAddrIn recAddr;
                        return InitSocket(recAddr, AHost, APort, true)
                              && (bind(AHandle, (TSockAddr*)&recAddr,
                                       sizeof(recAddr)) == 0);
                     }

   // 打开侦听
   static bool       Listen(TSocket AHandle, long AQueueSize = SOMAXCONN)
                     { return (listen(AHandle, AQueueSize) == 0); }

   // 接受连接
   static TSocket    Accept(TSocket AHandle, TSockAddrIn* Addr = NULL)
                     {
                        TSockAddrIn  recAddr;
                        TSockAddrIn* pAddr   = (Addr == NULL) ? &recAddr : Addr;
                        int          intLen  = sizeof(recAddr);
                        return accept(AHandle, (TSockAddr*)pAddr, &intLen);
                     }

   // 设置 KeepAlive 参数
   // 1. AHandle     Socket 句柄
   // 2. AIsAlive    是否 KeepAlive 检测
   // 3. AIdle       首次 KeepAlive 检测前的TCP空闲时间(秒)
   // 4. AInterval   两次 KeepAlive 检测间的时间间隔(秒)
   // 5. ACount      检测尝试的次数, 如果检测包收到响应了, 则后面几次不再发送
   static void       SetKeepAlive(TSocket AHandle, bool AIsAlive,  long AIdle,
                                                   long AInterval, long ACount);

   // 设置 Linger 时长
   // 1. AHandle     Socket 句柄
   // 2. ALinger     拖延时长(秒), 若 ALinger < 0 则 SO_DONTLINGER
   static void       SetLinger(TSocket AHandle, long ALinger);

   // 设置 NoDelay 属性
   // 1. AHandle     Socket 句柄
   // 2. AEnabled    是否禁用 TCP 发送的 Nagle 算法
   static void       SetNoDelay(TSocket AHandle, bool AEnabled);

   // 设置 Nonblocking 属性
   // 1. AHandle     Socket 句柄
   // 2. AEnabled    是否非阻塞模式 {true: 非阻塞, false: 阻塞}
   static bool       SetNonblocking(TSocket AHandle, bool AEnabled);

private:
   // TKYBaseSocket 的静态成员初始化类
   static class TInitialization
   {
   public:
      TInitialization();
      ~TInitialization();
   } Inner_Initialization;

   friend class TInitialization;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYSocketBuffer - Socket 读写缓冲区类 */

class TKYSocketBuffer
{
public:
   TKYSocketBuffer(TKYBaseSocket* ASocket  = NULL,
                         Longword ATimeout = 30000);
   virtual ~TKYSocketBuffer();

   // 属性
   TKYBaseSocket*    Socket() const    { return FSocket; }
   Longword          Timeout() const   { return FTimeout; }

   // 设置属性
   void              SetSocket(TKYBaseSocket* ASocket){ FSocket  = ASocket; }
   void              SetTimeout(Longword ATimeout)    { FTimeout = ATimeout; }

   // 等待读数据, 若返回 true 则表示接收缓冲区有数据可读
   bool              WaitForRead(Longword ATimeout) const;

   // 等待写数据, 若返回 true 则表示发送缓冲区有数据可写
   bool              WaitForWrite(Longword ATimeout) const;

   // 读数据(最长等待 Timeout() 毫秒)
   long              Read(void* ABuffer, long ACount) const;

   // 写数据(最长等待 Timeout() 毫秒)
   long              Write(const void* ABuffer, long ACount) const;

protected:
private:
   TKYEvent*         FEvent;
   TKYBaseSocket*    FSocket;
   Longword          FTimeout;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYSockRecvThread - Socket 接收线程类 */

class TKYSockRecvThread: public TThread
{
private:
   // Socket 项
   typedef struct
   {
      void*          Sender;
      TSocket        Handle;
   } TItem, *PItem;

   // 映射列表类型
   typedef TKYMapObjKey<TItem>         TItems;
   typedef TKYMapIntKey<TKYBaseSocket> TSockets;

public:
   TKYSockRecvThread();
   virtual ~TKYSockRecvThread();

   // 自定义数据, 默认值为: NULL
   void*             Data() const         { return FData; }

   // 加入接收线程的 socket 项数
   long              Count() const        { return FItems->Count(); }

   // 等待接收超时(毫秒), 默认值为: 5000
   // 除第一项以外, 后面加入线程项到开始接收时长大概为 Timeout() / 2
   Longword          Timeout() const      { return FTimeout; }

   // 设置自定义数据
   void              SetData(void* AData) { FData = AData; }

   // 设置等待接收超时(毫秒)
   // 若是一个接收线程负责多路 socket 接收, 建议不要设太长时间
   void              SetTimeout(Longword ATimeout)
                     { FTimeout = ATimeout; }

   // 通知线程开始接收, 即在调用 Add(...) 方法之后, 需要调用 Notify() 通知
   void              Notify() const       { FNotify->Set(); }

   // 关闭线程, 建议 FreeKYSockRecvThread 释放线程
   void              Close();

   // 清除所有接收 socket 项
   void              Clear();

   // 判断 socket 项是否存在
   bool              IsExist(TKYBaseSocket* ASocket) const;

   // 加入 socket 项
   bool              Add(void* Sender, TKYBaseSocket* ASocket);

   // 删除 socket 项
   bool              Delete(TKYBaseSocket* ASocket);

   // 事件
   TNotifyEvent      OnThreadStart;       // 线程开始时激发的事件
   TNotifyEvent      OnThreadEnd;         // 线程结束前激发的事件
   TNotifyEvent      OnTimeout;           // 接收超时时激发的事件
   TOnSocketEvent    OnReceive;           // 接收到数据时激发的事件

protected:
   // 当前锁
   void              Lock() const         { FLock->Enter(); }
   void              Unlock() const       { FLock->Leave(); }

   // 线程执行体
   virtual void      Execute();

private:
   void              DoSocketDestroy(void* Sender);
   void              DoSocketRecv(TSocket AHandle);

   long              ReadFDSet();
   void              WaitSocketRecv();

private:
   void*             FData;               // 自定义数据
   TKYCritSect*      FLock;               // 当前锁
   TItems*           FItems;              // Socket 列表项
   TSockets*         FFDList;             // 句柄列表
   TKYEvent*         FNotify;             // 通知事件

   TFDSet*           FFDSet;
   long              FFDCount;
   Longword          FTimeout;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYSockRecvThreads - Socket 接收线程集类 */

class TKYSockRecvThreads
{
private:
   // 映射列表类型
   typedef TKYMapObjKey<TKYSockRecvThread>   TThreads;

public:
   TKYSockRecvThreads(long AMaxCount = 0, long ACacheSize = 10);
   virtual ~TKYSockRecvThreads();

   // 属性
   long              Count() const     { return FThreads->Count(); } // default: 0
   Longword          Timeout() const   { return FTimeout; }          // default: 5000
   long              MaxCount() const  { return FMaxCount; }         // default: AMaxCount
   long              CacheSize() const { return FCaches->MaxCount();}// default: ACacheSize

   // 设置接收线程超时(毫秒)
   void              SetTimeout(Longword ATimeout);

   // 设置接收线程的最大个数, 0 表示无限制, 即每 Add(...) 单独分配一个线程
   void              SetMaxCount(long AMaxCount);

   // 设置接收线程缓冲池尺寸
   void              SetCacheSize(long ACacheSize);

   // 清除所有接收线程
   void              ClearThreads();

   // 判断 socket 项是否存在
   bool              IsExist(TKYBaseSocket* ASocket) const;

   // 加入 socket 项
   bool              Add(void* Sender, TKYBaseSocket* ASocket);

   // 删除 socket 项
   bool              Delete(TKYBaseSocket* ASocket);

   // 事件
   TNotifyEvent      OnThreadStart;    // 线程开始时激发的事件
   TNotifyEvent      OnThreadEnd;      // 线程结束前激发的事件
   TNotifyEvent      OnTimeout;        // 接收超时时激发的事件
   TOnSocketEvent    OnReceive;        // 接收到数据时激发的事件

protected:
   void              Lock() const      { FLock->Enter(); }
   void              Unlock() const    { FLock->Leave(); }

private:
   void              DoThreadStart(void* Sender);
   void              DoThreadEnd(void* Sender);
   void              DoTimeout(void* Sender);
   void              DoReceive(void* Sender, TKYBaseSocket* ASocket);

   bool              AssignThread(TKYSockRecvThread* &AThread);
   void              DessignThread(TKYSockRecvThread* AThread);

private:
   TKYCritSect*      FLock;
   TKYCycQueue*      FCaches;
   TThreads*         FThreads;
   TThreads*         FSockets;
   Longword          FTimeout;
   long              FMaxCount;
};

}

#endif
