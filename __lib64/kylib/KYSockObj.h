// =======================================
// Unit   : socket ����
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

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

class TKYBaseSocket;
class TKYSockRecvThread;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �ض��� socket ��ص����� */

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
   Longword             OnOff;         // ���� {0: �����, 1: ���}
   Longword             IdleTime;      // �״� KeepAlive ���ǰ��TCP����ʱ��(����)
   Longword             Interval;      // ���� KeepAlive �����ʱ����(����)
} TTCPKeepAlive;

// TOnSocketEvent �¼�����
typedef void (TObject::*TDoSocketEvent)(void* Sender, TKYBaseSocket* ASocket);
typedef struct
{
   TDoSocketEvent       Method;
   void*                Object;
} TOnSocketEvent;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �������� */

// socket �ĳ���ֵ
enum _TKYSocket_const
{
   Sock_BufferSize      = 8192,        // socket ���ջ������ߴ�
   UDP_BufferSize       = 65515,       // UDP �Ļ������ߴ�,
                                       // ����UDPЭ����˵, ����������󳤶�Ϊ
                                       // 65535������IP��ͷ����20, ������ 65515
};

// �쳣��Ϣ
const char* const SockExcept_Startup   = "WSAStartup failure";

// socket ������(Function name)
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

// ��ַ����
const KYString    Sock_LocalHost       = "LOCALHOST";
const KYString    Sock_LocalAddr       = "127.0.0.1";
const KYString    Sock_Broadcast       = "255.255.255.255";

// ������Ϣ
const KYString    SockErr_Call         = "call <socket::%s> error: %s (%d)";
const KYString    SockErr_NonOpened    = "socket is not opened";
const KYString    SockErr_NotAddress   = "the address is empty";
const KYString    SockErr_NotAccept    = "accept thread create failure";
const KYString    SockErr_NotChange    = "can't change attrib on opened socket";
const KYString    SockErr_NotCreate    = "can't create new socket";

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �������� */

// ��鷵��ֵ
int   CheckSocketResult(int AResult, const char* AName, KYString& AError);

// �رղ��ͷ� socket �����߳�
void  FreeKYSockRecvThread(TKYSockRecvThread* &AThread, Longword ATimeout = 15000);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYBaseSocket - Socket ���� */

class TKYBaseSocket
{
   friend class TKYSockRecvThread;

public:
   // Socket ����(Type)
   enum TType     {stStream     = SOCK_STREAM,     // stream socket
                   stDatagram   = SOCK_DGRAM,      // datagram socket
                   stRaw        = SOCK_RAW,        // raw-protocol interface
                   stRDM        = SOCK_RDM,        // reliably-delivered message
                   stSeqPacket  = SOCK_SEQPACKET}; // sequenced packet stream

   // Socket Э����(Protocol family)
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

   // Socket Э��(Protocol)
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

   // Socket Э����
   typedef struct
   {
      Byte           Type;             // ����
      Byte           Domain;           // Э����
      Word           Protocol;         // Э��
   } TProto;

public:
   TKYBaseSocket();
   virtual ~TKYBaseSocket();

   // ����
   virtual bool      Active() const;                        // default: false
   void*             Data() const      { return FData; }    // default: NULL
   TProto            Proto() const     { return FProto; }   // default: {stStream, pfUnspec, spIP}
   TSocket           Handle() const    { return FHandle; }

   // ȡ��ǰ IP ��ַ
   KYString          LocalAddr() const;

   // ȡ��ǰ�˿ں�
   long              LocalPort() const;

   // ȡԶ�˵�������, ���ܻ����, ���������
   KYString          RemoteHost();

   // ȡԶ�˵� IP ��ַ
   KYString          RemoteAddr() const;

   // ȡԶ�˵Ķ˿ں�
   long              RemotePort() const;

   // ��������
   void              SetData(void* AData)    { FData = AData; }
   void              SetProto(const TProto& AProto);
   void              SetActive(bool AValue)  { if (AValue) Open(); else Close(); }

   // ����
   virtual void      Lock() const;

   // ����
   virtual void      Unlock() const;

   // ��
   virtual bool      Open();

   // �ر�
   virtual void      Close();

   // ��������
   long              SendBuffer(const void* ABuffer, long ACount) const
                     { return send(FHandle, (const char*)ABuffer, ACount, 0); }

   // ���������ӵ�����
   long              SendBuffer(const KYString& AHost, long APort,
                                    const void* ABuffer, long ACount) const;

   // ��������
   long              SendText(const KYString& AText) const
                     { return SendBuffer((char*)AText, AText.Length()); }

   // ���������ӵ�����
   long              SendText(const KYString& AHost, long APort,
                              const KYString& AText) const
                     { return SendBuffer(AHost, APort, (char*)AText, AText.Length()); }

   // �������ݳ���
   long              RecvLength() const;

   // ��������
   long              RecvBuffer(void* ABuffer, long ACount) const
                     { return recv(FHandle, (char*)ABuffer, ACount, 0); }

   // ���������ӵ�����
   long              RecvBuffer(void* ABuffer, long ACount,
                            KYString& APeerIP, long& APeerPort) const;

   // ��������
   KYString          RecvText() const;

   // ���������ӵ�����
   KYString          RecvText(KYString& APeerIP, long& APeerPort) const;

protected:
   bool              DoOpen();
   void              DoClose();

protected:
   // �¼�
   TNotifyEvent      OnDestroy;

   TProto            FProto;
   TSocket           FHandle;

private:
   void*             FData;
   KYString          FRemoteHost;

public:
   // ����������
   static KYString   LocalHost();

   // ���ݾ��ȡ��ǰ IP �Ͷ˿ں�
   static bool       LocalIPPort(TSocket AHandle, KYString& AIP, long& APort);

   // ���ݾ��ȡԶ�� IP �Ͷ˿ں�
   static bool       RemoteIPPort(TSocket AHandle, KYString& AIP, long& APort);

   // ���������������ص�ַ
   static TInAddr    LookupHost(const KYString& AHost);

   // ������������ IP ��ַ
   static KYString   ResolveHost(const KYString& AHost);

   // ��ʼ�� socket
   static bool       InitSocket(TSockAddrIn& Addr, const KYString& AHost,
                                        long APort, bool AnyAddr = false);

   // ȡ�����뼰������Ϣ
   static int        LastError(const char* AName, KYString& AError)
                     { return CheckSocketResult(-1, AName, AError); }

   // �½� socket
   static TSocket    NewSocket(const TProto& AProto)
                     { return socket(AProto.Domain, AProto.Type, AProto.Protocol); }

   // �ر� socket
   static bool       CloseSocket(TSocket AHandle)
                     {  // {SD_SEND: 0x01, SD_RECV: 0x02, SD_BOTH: 0x03}
                        //shutdown(AHandle, 0x01);
                        return (closesocket(AHandle) == 0);
                     }

   // ���� socket(ע: ����ʱ�������ڷ����� socket)
   static bool       Connect(TSocket AHandle, const KYString& AHost, long APort,
                            Longword ATimeout);
   static bool       Connect(TSocket AHandle, const KYString& AHost, long APort)
                     {
                        TSockAddrIn recAddr;
                        return InitSocket(recAddr, AHost, APort, false)
                              && (connect(AHandle, (TSockAddr*)&recAddr,
                                          sizeof(recAddr)) == 0);
                     }

   // ���ž���� IP �Ͷ˿ں�
   static bool       TryBind(TSocket AHandle, const KYString& AHost, long APort)
                     {
                        TSockAddrIn recAddr;
                        return InitSocket(recAddr, AHost, APort, true)
                              && (bind(AHandle, (TSockAddr*)&recAddr,
                                       sizeof(recAddr)) == 0);
                     }

   // ������
   static bool       Listen(TSocket AHandle, long AQueueSize = SOMAXCONN)
                     { return (listen(AHandle, AQueueSize) == 0); }

   // ��������
   static TSocket    Accept(TSocket AHandle, TSockAddrIn* Addr = NULL)
                     {
                        TSockAddrIn  recAddr;
                        TSockAddrIn* pAddr   = (Addr == NULL) ? &recAddr : Addr;
                        int          intLen  = sizeof(recAddr);
                        return accept(AHandle, (TSockAddr*)pAddr, &intLen);
                     }

   // ���� KeepAlive ����
   // 1. AHandle     Socket ���
   // 2. AIsAlive    �Ƿ� KeepAlive ���
   // 3. AIdle       �״� KeepAlive ���ǰ��TCP����ʱ��(��)
   // 4. AInterval   ���� KeepAlive �����ʱ����(��)
   // 5. ACount      ��Ⳣ�ԵĴ���, ��������յ���Ӧ��, ����漸�β��ٷ���
   static void       SetKeepAlive(TSocket AHandle, bool AIsAlive,  long AIdle,
                                                   long AInterval, long ACount);

   // ���� Linger ʱ��
   // 1. AHandle     Socket ���
   // 2. ALinger     ����ʱ��(��), �� ALinger < 0 �� SO_DONTLINGER
   static void       SetLinger(TSocket AHandle, long ALinger);

   // ���� NoDelay ����
   // 1. AHandle     Socket ���
   // 2. AEnabled    �Ƿ���� TCP ���͵� Nagle �㷨
   static void       SetNoDelay(TSocket AHandle, bool AEnabled);

   // ���� Nonblocking ����
   // 1. AHandle     Socket ���
   // 2. AEnabled    �Ƿ������ģʽ {true: ������, false: ����}
   static bool       SetNonblocking(TSocket AHandle, bool AEnabled);

private:
   // TKYBaseSocket �ľ�̬��Ա��ʼ����
   static class TInitialization
   {
   public:
      TInitialization();
      ~TInitialization();
   } Inner_Initialization;

   friend class TInitialization;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYSocketBuffer - Socket ��д�������� */

class TKYSocketBuffer
{
public:
   TKYSocketBuffer(TKYBaseSocket* ASocket  = NULL,
                         Longword ATimeout = 30000);
   virtual ~TKYSocketBuffer();

   // ����
   TKYBaseSocket*    Socket() const    { return FSocket; }
   Longword          Timeout() const   { return FTimeout; }

   // ��������
   void              SetSocket(TKYBaseSocket* ASocket){ FSocket  = ASocket; }
   void              SetTimeout(Longword ATimeout)    { FTimeout = ATimeout; }

   // �ȴ�������, ������ true ���ʾ���ջ����������ݿɶ�
   bool              WaitForRead(Longword ATimeout) const;

   // �ȴ�д����, ������ true ���ʾ���ͻ����������ݿ�д
   bool              WaitForWrite(Longword ATimeout) const;

   // ������(��ȴ� Timeout() ����)
   long              Read(void* ABuffer, long ACount) const;

   // д����(��ȴ� Timeout() ����)
   long              Write(const void* ABuffer, long ACount) const;

protected:
private:
   TKYEvent*         FEvent;
   TKYBaseSocket*    FSocket;
   Longword          FTimeout;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYSockRecvThread - Socket �����߳��� */

class TKYSockRecvThread: public TThread
{
private:
   // Socket ��
   typedef struct
   {
      void*          Sender;
      TSocket        Handle;
   } TItem, *PItem;

   // ӳ���б�����
   typedef TKYMapObjKey<TItem>         TItems;
   typedef TKYMapIntKey<TKYBaseSocket> TSockets;

public:
   TKYSockRecvThread();
   virtual ~TKYSockRecvThread();

   // �Զ�������, Ĭ��ֵΪ: NULL
   void*             Data() const         { return FData; }

   // ��������̵߳� socket ����
   long              Count() const        { return FItems->Count(); }

   // �ȴ����ճ�ʱ(����), Ĭ��ֵΪ: 5000
   // ����һ������, ��������߳����ʼ����ʱ�����Ϊ Timeout() / 2
   Longword          Timeout() const      { return FTimeout; }

   // �����Զ�������
   void              SetData(void* AData) { FData = AData; }

   // ���õȴ����ճ�ʱ(����)
   // ����һ�������̸߳����· socket ����, ���鲻Ҫ��̫��ʱ��
   void              SetTimeout(Longword ATimeout)
                     { FTimeout = ATimeout; }

   // ֪ͨ�߳̿�ʼ����, ���ڵ��� Add(...) ����֮��, ��Ҫ���� Notify() ֪ͨ
   void              Notify() const       { FNotify->Set(); }

   // �ر��߳�, ���� FreeKYSockRecvThread �ͷ��߳�
   void              Close();

   // ������н��� socket ��
   void              Clear();

   // �ж� socket ���Ƿ����
   bool              IsExist(TKYBaseSocket* ASocket) const;

   // ���� socket ��
   bool              Add(void* Sender, TKYBaseSocket* ASocket);

   // ɾ�� socket ��
   bool              Delete(TKYBaseSocket* ASocket);

   // �¼�
   TNotifyEvent      OnThreadStart;       // �߳̿�ʼʱ�������¼�
   TNotifyEvent      OnThreadEnd;         // �߳̽���ǰ�������¼�
   TNotifyEvent      OnTimeout;           // ���ճ�ʱʱ�������¼�
   TOnSocketEvent    OnReceive;           // ���յ�����ʱ�������¼�

protected:
   // ��ǰ��
   void              Lock() const         { FLock->Enter(); }
   void              Unlock() const       { FLock->Leave(); }

   // �߳�ִ����
   virtual void      Execute();

private:
   void              DoSocketDestroy(void* Sender);
   void              DoSocketRecv(TSocket AHandle);

   long              ReadFDSet();
   void              WaitSocketRecv();

private:
   void*             FData;               // �Զ�������
   TKYCritSect*      FLock;               // ��ǰ��
   TItems*           FItems;              // Socket �б���
   TSockets*         FFDList;             // ����б�
   TKYEvent*         FNotify;             // ֪ͨ�¼�

   TFDSet*           FFDSet;
   long              FFDCount;
   Longword          FTimeout;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYSockRecvThreads - Socket �����̼߳��� */

class TKYSockRecvThreads
{
private:
   // ӳ���б�����
   typedef TKYMapObjKey<TKYSockRecvThread>   TThreads;

public:
   TKYSockRecvThreads(long AMaxCount = 0, long ACacheSize = 10);
   virtual ~TKYSockRecvThreads();

   // ����
   long              Count() const     { return FThreads->Count(); } // default: 0
   Longword          Timeout() const   { return FTimeout; }          // default: 5000
   long              MaxCount() const  { return FMaxCount; }         // default: AMaxCount
   long              CacheSize() const { return FCaches->MaxCount();}// default: ACacheSize

   // ���ý����̳߳�ʱ(����)
   void              SetTimeout(Longword ATimeout);

   // ���ý����̵߳�������, 0 ��ʾ������, ��ÿ Add(...) ��������һ���߳�
   void              SetMaxCount(long AMaxCount);

   // ���ý����̻߳���سߴ�
   void              SetCacheSize(long ACacheSize);

   // ������н����߳�
   void              ClearThreads();

   // �ж� socket ���Ƿ����
   bool              IsExist(TKYBaseSocket* ASocket) const;

   // ���� socket ��
   bool              Add(void* Sender, TKYBaseSocket* ASocket);

   // ɾ�� socket ��
   bool              Delete(TKYBaseSocket* ASocket);

   // �¼�
   TNotifyEvent      OnThreadStart;    // �߳̿�ʼʱ�������¼�
   TNotifyEvent      OnThreadEnd;      // �߳̽���ǰ�������¼�
   TNotifyEvent      OnTimeout;        // ���ճ�ʱʱ�������¼�
   TOnSocketEvent    OnReceive;        // ���յ�����ʱ�������¼�

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
