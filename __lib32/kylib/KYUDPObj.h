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

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYCustomUDP - UDP ���� */

class TKYCustomUDP
{
   friend class TKYUDPRecvThread;

public:
   TKYCustomUDP();
   virtual ~TKYCustomUDP();

   // ����
   void*          Data() const            { return FData; }       // default: NULL
   TSocket        Handle() const          { return FHandle; }     // default: INVALID_SOCKET
   bool           Active() const          { return FHandle != INVALID_SOCKET; }
   int            ErrorCode() const       { return FErrorCode; }  // default: 0
   bool           BroadcastEnabled() const{ return FBroadcastEnabled; }// default: false

   // ��������
   void           SetData(void* AData)    { FData = AData; }
   void           SetBroadcastEnabled(bool AEnabled);

   // ��/�ر�
   virtual bool   Open() = 0;
   virtual bool   Close() = 0;

   // ���͹㲥����
   bool           Broadcast(const KYString& AData, long APort);

   // ���� UDP ����, ���ط������ݳߴ�
   long           SendBuffer(const KYString& AHost, long APort,
                                 const void* ABuffer, long ACount);
   long           SendText(const KYString& AHost, long APort, const KYString& AText);

   // �������ݳ���
   long           RecvLength();

   // ���� UDP ����, ���ؽ������ݳߴ�
   long           RecvBuffer(void* ABuffer, long ACount);
   long           RecvBuffer(void* ABuffer, long ACount,
                         KYString& APeerIP, long& APeerPort);

   // ���� UDP ����, ���ؽ�������
   KYString       RecvText();
   KYString       RecvText(KYString& APeerIP, long& APeerPort);

protected:
   void           SetBroadcastFlag(bool AEnabled);

   bool           NewSocket();
   bool           CloseSocket();
   bool           TryBind(TSocket AHandle, const KYString& AHost, long APort);

   // �¼�
   TNotifyEvent   OnDestroy;

protected:
   void*          FData;
   TSocket        FHandle;
   int            FErrorCode;
   bool           FBroadcastEnabled;

public:
   // ����������
   static KYString   LocalHost()       { return TKYBaseSocket::LocalHost(); }

   // ���ݾ��ȡ��ǰ IP �Ͷ˿ں�
   static bool       LocalIPPort(TSocket AHandle, KYString& AIP, long& APort)
                     { return TKYBaseSocket::LocalIPPort(AHandle, AIP, APort); }

   // ���������������ص�ַ
   static TInAddr    LookupHost(const KYString& AHost)
                     { return TKYBaseSocket::LookupHost(AHost); }

   // ������������ IP ��ַ
   static KYString   ResolveHost(const KYString& AHost)
                     { return TKYBaseSocket::ResolveHost(AHost); }

   // ��ʼ�� socket
   static bool       InitSocket(TSockAddrIn& Addr, const KYString& AHost,
                                        long APort, bool AnyAddr = false)
                     { return TKYBaseSocket::InitSocket(Addr, AHost, APort, AnyAddr); }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYUDPClient - UDP �ͻ����� */

class TKYUDPClient: public TKYCustomUDP
{
public:
   // �� UDP �ͻ���, ���򿪳ɹ��򷵻� true, �ͻ���ֻ���������ݲ��ܽ�������
   virtual bool   Open();

   // �ر� UDP �ͻ���
   virtual bool   Close();

protected:
private:
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYUDPServer - UDP ������� */

class TKYUDPServer: public TKYCustomUDP
{
public:
   TKYUDPServer();
   virtual ~TKYUDPServer();

   // ����
   const KYString CurrIP() const       { return FCurrIP; }  // default: ""
   long           CurrPort() const     { return FCurrPort; }// default: 0

   const KYString Host() const         { return FHost; }    // default: ""
   long           Port() const         { return FPort; }    // default: 0

   // ���ý��� UDP ���������� IP ��ַ
   bool           SetHost(const KYString& AHost);

   //  ���ý��� UDP �Ķ˿ں�
   bool           SetPort(long APort);

   // �� UDP �����, ���򿪳ɹ��򷵻� true, �������ܹ����պͷ�������
   virtual bool   Open();

   // �ر� UDP �����
   virtual bool   Close();

protected:
private:
   KYString       FCurrIP;
   long           FCurrPort;

   KYString       FHost;
   long           FPort;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYUDPRecvThread - UDP �����߳��� */

class TKYUDPRecvThread: public TThread
{
private:
   // UDP ��
   typedef struct
   {
      void*          Sender;
      TSocket        Handle;
   } TItem, *PItem;

   // ӳ���б�����
   typedef TKYMapObjKey<TItem>         TItems;
   typedef TKYMapIntKey<TKYCustomUDP>  TUDPList;

public:
   // TOnReceive �����¼�
   typedef void (TObject::*TDoReceive)(void* Sender, TKYCustomUDP* UDP);
   typedef struct
   {
      TDoReceive     Method;
      void*          Object;
   } TOnReceive;

public:
   TKYUDPRecvThread();
   virtual ~TKYUDPRecvThread();

   // �Զ�������, Ĭ��ֵΪ: NULL
   void*          Data() const         { return FData; }

   // ��������̵߳� UDP ����
   long           Count() const        { return FItems->Count(); }

   // �ȴ����ճ�ʱ(����), Ĭ��ֵΪ: 15000
   // ����һ������, ��������߳����ʼ����ʱ�����Ϊ Timeout() / 2
   Longword       Timeout() const      { return FTimeout; }

   // �����Զ�������
   void           SetData(void* AData) { FData = AData; }

   // ���õȴ����ճ�ʱ(����)
   // ����һ�������̸߳����· UDP ����, ���鲻Ҫ��̫��ʱ��
   void           SetTimeout(Longword ATimeout)
                  { FTimeout = ATimeout; }

   // ֪ͨ�߳̿�ʼ����, ���ڵ��� Add(...) ����֮��, ��Ҫ���� Notify() ֪ͨ
   void           Notify() const       { FNotify->Set(); }

   // �ر��߳�, ���� FreeKYUDPRecvThread �ͷ��߳�
   void           Close();

   // ������н��� UDP ��
   void           Clear();

   // �ж� UDP ���Ƿ����
   bool           IsExist(TKYCustomUDP* UDP) const;

   // ���� UDP ��
   bool           Add(void* Sender, TKYCustomUDP* UDP);

   // ɾ�� UDP ��
   bool           Delete(TKYCustomUDP* UDP);

   // �¼�
   TNotifyEvent   OnThreadStart;       // �߳̿�ʼʱ�������¼�
   TNotifyEvent   OnThreadEnd;         // �߳̽���ǰ�������¼�
   TNotifyEvent   OnTimeout;           // ���ճ�ʱʱ�������¼�
   TOnReceive     OnReceive;           // ���յ�����ʱ�������¼�

protected:
   // ��ǰ��
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

   // �߳�ִ����
   virtual void   Execute();

private:
   void           DoUDPDestroy(void* Sender);
   void           DoUDPRecv(TSocket AHandle);

   long           ReadFDSet();
   void           WaitUDPRecv();

private:
   void*          FData;               // �Զ�������
   TKYCritSect*   FLock;               // ��ǰ��
   TItems*        FItems;              // UDP �б���
   TUDPList*      FFDList;             // ����б�
   TKYEvent*      FNotify;             // ֪ͨ�¼�

   TFDSet*        FFDSet;
   long           FFDCount;
   Longword       FTimeout;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �������� */

// �رղ��ͷ� UDP �����߳�
void FreeKYUDPRecvThread(TKYUDPRecvThread* &AThread, Longword ATimeout = 15000);

}

#endif
