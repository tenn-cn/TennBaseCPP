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

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYTCPClient - TCP�ͻ����� */

class TKYTCPClient: public TKYBaseSocket
{
public:
   TKYTCPClient();
   virtual ~TKYTCPClient();

   // ����
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

   // ��������
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

   // ����
   virtual void      Lock() const;

   // ����
   virtual void      Unlock() const;

   // ������, �����ӳɹ��򼤷� OnConnect �¼�
   virtual bool      Open();

   // �ر�����, ���� OnDisconnect �¼�
   virtual void      Close();

   // �¼�
   TOnSocketEvent    OnConnect;        // ���ӳɹ�ʱ�������¼�
   TOnSocketEvent    OnDisconnect;     // �Ͽ�����ʱ�������¼�

protected:
   void              DoConnect();
   void              DoDisconnect();

private:
   TKYCritSect*      FLock;            // ��
   KYString          FHost;            // ���� Server �����������ַ
   long              FPort;            // ���� Server �Ķ˿ں�
   long              FLinger;          // Close ������ʱ��(��), Ĭ��ֵΪ: -1
   long              FTimeout;         // Open  ��ʱ(����), Ĭ��ֵΪ: -1
   KYString          FBindHost;        // �󶨵� IP ��ַ��������
   long              FBindPort;        // �󶨵Ķ˿ں�

   bool              FActive;          // �Ƿ��
   bool              FNoDelay;         // �Ƿ� NoDelay
   bool              FNonblocking;     // �Ƿ� Nonblocking
   bool              FKeepIsAlive;     // �Ƿ� KeepAlive ���
   long              FKeepIdle;        // �״� KeepAlive ���ǰ��TCP����ʱ��(��)
   long              FKeepInterval;    // ���� KeepAlive �����ʱ����(��)
   long              FKeepCount;       // ��Ⳣ�ԵĴ���, ��������յ���Ӧ��, ����漸�β��ٷ���
   int               FErrorCode;       // ������
   KYString          FErrorInfo;       // ������Ϣ
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYTCPSrvClient - TCP����˵Ŀͻ������� */

class TKYTCPSrvClient: public TKYBaseSocket
{
   friend class TKYTCPServer;

public:
   TKYTCPSrvClient(TKYTCPServer* AServer = NULL, TSocket AHandle = INVALID_SOCKET);
   virtual ~TKYTCPSrvClient();

   // FServer �еĿͻ������� ID
   long              ClientID() const  { return FClientID; }

   // ����
   virtual void      Lock() const;

   // ����
   virtual void      Unlock() const;

   // ���� Active()
   virtual bool      Open();

   // �ر�����
   virtual void      Close();

protected:
   void              DoDisconnect();

private:
   TKYCritSect*      FLock;
   TKYTCPServer*     FServer;
   long              FClientID;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYTCPServer - TCP������� */

class TKYTCPServer: public TKYBaseSocket
{
   friend class TKYTCPSrvClient;

public:
   TKYTCPServer();
   virtual ~TKYTCPServer();

   // ����
   virtual bool         Active() const;                                 // default: false
   KYString             Host() const         { return FHost; }          // default: ""
   long                 Port() const         { return FPort; }          // default: 0
   long                 Linger() const       { return FLinger; }        // default: -1
   long                 QueueSize() const    { return FQueueSize; }     // default: SOMAXCONN
   int                  ErrorCode() const    { return FErrorCode; }     // default: 0
   KYString             ErrorInfo() const    { return FErrorInfo; }     // default: ""
   TKYSockRecvThreads*  RecvThreads() const  { return FRecvThreads; }   // ��Ҫ�����¼�

   bool                 NoDelay() const      { return FNoDelay; }       // default: false
   bool                 Nonblocking() const  { return FNonblocking; }   // default: false

   bool                 KeepIsAlive() const  { return FKeepIsAlive; }   // defalut: false
   long                 KeepIdle() const     { return FKeepIdle; }      // default: 30
   long                 KeepInterval() const { return FKeepInterval; }  // default: 10
   long                 KeepCount() const    { return FKeepCount; }     // default: 3

   // �ͻ������Ӹ���
   long                 ClientCount() const  { return FClients->Count(); }

   // ȡ�����Ӧ�Ŀͻ�������, ������������򷵻� NULL
   TKYTCPSrvClient*     Client(long AClientID) const;

   // ����������ȡ�ͻ�������, AIndex ȡֵ��Χ: [0..ClientCount()-1],
   // ��Ҫѭ���������Բ�����ʹ��
   TKYTCPSrvClient*     Connection(long AIndex) const;

   // ��������
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

   // ����
   virtual void         Lock() const;

   // ����
   virtual void         Unlock() const;

   // ������, ���ɹ��򼤷� OnListen �¼�
   virtual bool         Open();

   // �ر�������ͬʱ�ر���������, ���� OnClientDisconnect �� OnDisconnect �¼�
   virtual void         Close();

   // �ر����пͻ�������
   void                 CloseClients();

   // ��һ���ͻ������Ӿ��
   long                 FirstID() const;

   // ��һ���ͻ������Ӿ��
   long                 PriorID() const;

   // ��һ���ͻ������Ӿ��
   long                 NextID() const;

   // ���һ���ͻ������Ӿ��
   long                 LastID() const;

   // ָ���������һ���ͻ������Ӿ��
   long                 PriorID(long AClientID) const;

   // ָ���������һ���ͻ������Ӿ��
   long                 NextID(long AClientID) const;

   // �¼�
   TOnSocketEvent       OnListen;            // �������ɹ�ʱ�������¼�
   TOnSocketEvent       OnDisconnect;        // �ر�����ʱ�������¼�

   // OnAccept �� OnClientConnect �� OnReceive ֮ǰ����,
   // �� OnClientConnect ��һ���� OnReceive ֮ǰ����
   TOnSocketEvent       OnAccept;            // �ͻ��˿�ʼ����ʱ�������¼�
   TOnSocketEvent       OnClientConnect;     // �ͻ���������ʱ�������¼�
   TOnSocketEvent       OnClientDisconnect;  // �ͻ��˶Ͽ�����ʱ�������¼�

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
   TKYCritSect*         FLock;               // ��
   TKYHandles*          FClients;            // �ͻ��������б�
   TKYSockRecvThreads*  FRecvThreads;        // �����̼߳�
   TKYThread*           FAcceptThread;       // �������ӵ��߳�

   KYString             FHost;               // �󶨵� IP ��ַ��������
   long                 FPort;               // �����Ķ˿ں�
   long                 FLinger;             // Close ������ʱ��(��), Ĭ��ֵΪ: -1
   long                 FQueueSize;          // ���ܿͻ������ӵĶ��гߴ�

   bool                 FActive;             // �Ƿ��
   bool                 FNoDelay;            // �Ƿ� NoDelay
   bool                 FNonblocking;        // �Ƿ� Nonblocking
   bool                 FKeepIsAlive;        // �Ƿ� KeepAlive ���
   long                 FKeepIdle;           // �״� KeepAlive ���ǰ��TCP����ʱ��(��)
   long                 FKeepInterval;       // ���� KeepAlive �����ʱ����(��)
   long                 FKeepCount;          // ��Ⳣ�ԵĴ���, ��������յ���Ӧ��, ����漸�β��ٷ���
   int                  FErrorCode;          // ������
   KYString             FErrorInfo;          // ������Ϣ
};

}

#endif
