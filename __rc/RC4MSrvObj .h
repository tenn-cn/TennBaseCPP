// =======================================
// Unit   : RC4SHM server object
// Version: 5.0.0.0 (build 2014.07.07)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _RC4MSrvObj_H_
#define _RC4MSrvObj_H_

#include "RC4SHM.h"
#include "RCKObjs.h"
using namespace RCKObjs;

namespace RC4SHM
{
   namespace __server__
   {

/* begin namespace */

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TSHMSrvObj - RC4SHM ������������ */

class TSHMSrvObj
{
public:
   // ֪ͨ�¼�
   typedef void (TObject::*TDoNotify)(TSHMSrvObj* ASrvObj);
   typedef struct
   {
      TDoNotify      Method;
      void*          Object;
   } TOnNotify;

   // ���������¼�
   typedef void (TObject::*TDoAccept)(TSHMSrvObj* ASrvObj, TRCConnObj* AConnObj,
                                            bool& AIsRefused);
   typedef struct
   {
      TDoAccept      Method;
      void*          Object;
   } TOnAccept;

   // �ͻ������¼�
   typedef void (TObject::*TDoCltEvent)(TSHMSrvObj* ASrvObj, TRCConnObj* AConnObj);
   typedef struct
   {
      TDoCltEvent    Method;
      void*          Object;
   } TOnCltEvent;

   // ����������¼�
   typedef void (TObject::*TDoCmdReturn)(TSHMSrvObj* ASrvObj, TRCConnObj* AConnObj,
                                               void* ACmdID,        long  AResult);
   typedef struct
   {
      TDoCmdReturn   Method;
      void*          Object;
   } TOnCmdReturn;

   // �����Զ��������¼�
   typedef void (TObject::*TDoRecvData)(TSHMSrvObj* ASrvObj, TRCConnObj* AConnObj,
                                        const void* AData,         long  ASize);
   typedef struct
   {
      TDoRecvData    Method;
      void*          Object;
   } TOnRecvData;

public:
   TSHMSrvObj();
   virtual ~TSHMSrvObj();

   // ����
   void*          Data() const         { return FData; }
   void*          SrvID() const        { return FSrvID; }
   long           State() const        { return SHMServerGetInt(FSrvID, msaState, NULL); }
   KYString       Error() const        { return SHMServerGetStr(FSrvID, msaError, NULL); }

   KYString       Host() const         { return SHMServerGetStr(FSrvID, msaName, NULL); }
   long           Port() const         { return SHMServerGetInt(FSrvID, msaPort, NULL); }
   long           ReqTimeout() const   { return SHMServerGetInt(FSrvID, msaReqTimeout, NULL); }
   long           ListenQueue() const  { return SHMServerGetInt(FSrvID, msaListenQueue, NULL); }
   long           RecvThreads() const  { return SHMServerGetInt(FSrvID, msaRecvThreads, NULL); }
   long           CacheThreads() const { return SHMServerGetInt(FSrvID, msaCacheThreads, NULL); }

   TRCConnObj*    Client(long AIndex) const;
   bool           CanLogin() const     { return SHMServerGetInt(FSrvID, msaCanLogin, NULL) == 1; }
   bool           CanBindApp() const   { return SHMServerGetInt(FSrvID, msaCanBindApp, NULL) == 1; }
   long           MaxClients() const   { return SHMServerGetInt(FSrvID, msaMaxClientCount, NULL); }
   long           ClientCount() const  { return SHMServerGetInt(FSrvID, msaSrvClientCount, NULL); }
   long           BindTimeout() const  { return SHMServerGetInt(FSrvID, msaBindTimeout, NULL); }
   long           SendMaxSize() const  { return SHMServerGetInt(FSrvID, msaSendMaxSize, NULL); }
   long           SendQueueSize() const{ return SHMServerGetInt(FSrvID, msaSendQueueSize, NULL); }

   long           Timeout() const      { return SHMServerGetInt(FSrvID, msaTimeout, NULL); }
   long           KeepTimeout() const  { return SHMServerGetInt(FSrvID, msaKeepTimeout, NULL); }
   long           KeepInterval() const { return SHMServerGetInt(FSrvID, msaKeepInterval, NULL); }
   long           KeepRetryTimes()const{ return SHMServerGetInt(FSrvID, msaKeepRetryTimes, NULL); }

   // ��������
   void           SetData(void* AData) { FData = AData; }
   long           SetHost(const KYString& AHost)
                  { return SHMServerSetStr(FSrvID, msaName, (char*)AHost, AHost.Length()); }
   long           SetPort(long APort)
                  { return SHMServerSetInt(FSrvID, msaPort, APort); }
   long           SetReqTimeout(long ATimeout)
                  { return SHMServerSetInt(FSrvID, msaReqTimeout, ATimeout); }
   long           SetListenQueue(long AListenQueue)
                  { return SHMServerSetInt(FSrvID, msaListenQueue, AListenQueue); }
   long           SetRecvThreads(long ACount)
                  { return SHMServerSetInt(FSrvID, msaRecvThreads, ACount); }
   long           SetCacheThreads(long ACount)
                  { return SHMServerSetInt(FSrvID, msaCacheThreads, ACount); }

   long           SetCanLogin(bool ACanLogin)
                  { return SHMServerSetInt(FSrvID, msaCanLogin, ACanLogin); }
   long           SetCanBindApp(bool ACanBind)
                  { return SHMServerSetInt(FSrvID, msaCanBindApp, ACanBind); }
   long           SetMaxClients(long AMaxCount)
                  { return SHMServerSetInt(FSrvID, msaMaxClientCount, AMaxCount); }
   long           SetBindTimeout(long ATimeout)
                  { return SHMServerSetInt(FSrvID, msaBindTimeout, ATimeout); }
   long           SetSendMaxSize(long AMaxSize)
                  { return SHMServerSetInt(FSrvID, msaSendMaxSize, AMaxSize); }
   long           SetSendQueueSize(long AQueueSize)
                  { return SHMServerSetInt(FSrvID, msaSendQueueSize, AQueueSize); }

   long           SetTimeout(long ATimeout)
                  { return SHMServerSetInt(FSrvID, msaTimeout, ATimeout); }
   long           SetKeepTimeout(long ATimeout)
                  { return SHMServerSetInt(FSrvID, msaKeepTimeout, ATimeout); }
   long           SetKeepInterval(long AInterval)
                  { return SHMServerSetInt(FSrvID, msaKeepInterval, AInterval); }
   long           SetKeepRetryTimes(long ARetryTimes)
                  { return SHMServerSetInt(FSrvID, msaKeepRetryTimes, ARetryTimes); }

   // ��/�ر� SHM �����
   long           Open()               { return SHMServerOpen(FSrvID); }
   long           Close()              { return SHMServerClose(FSrvID); }

   // �¼�
   TOnNotify      OnListen;
   TOnNotify      OnDisconnect;
   TOnAccept      OnAccept;
   TOnCltEvent    OnFreeClt;

   TOnCltEvent    OnCltConnect;
   TOnCltEvent    OnCltDisconnect;
   TOnCltEvent    OnCltLogin;
   TOnCltEvent    OnCltLogout;
   TOnCmdReturn   OnCltCmdReturn;
   TOnRecvData    OnCltRecvData;

protected:
private:
   // ִ���¼�����
   void           DoListen();
   void           DoDisconnect();
   void           DoAccept(TRCConnObj* AConnObj, bool& AIsRefused);
   void           DoFreeClt(TRCConnObj* AConnObj);
   void           DoCltConnect(TRCConnObj* AConnObj);
   void           DoCltDisconnect(TRCConnObj* AConnObj);
   void           DoCltLogin(TRCConnObj* AConnObj);
   void           DoCltLogout(TRCConnObj* AConnObj);
   void           DoCltCmdReturn(TRCConnObj* AConnObj, void* ACmdID, long AResult);
   void           DoCltRecvData(TRCConnObj* AConnObj, const void* AData, long ASize);

private:
   void*          FData;               // �Զ�������
   void*          FSrvID;              // ������ ID

private:
   // �������¼��Ļص�����
   static void __stdcall _SrvOnListen(void* ASrvID);
   static void __stdcall _SrvOnDisconnect(void* ASrvID);
   static void __stdcall _SrvOnAccept(void* ASrvID, void* AConnID, bool& AIsRefused);
   static void __stdcall _SrvOnFreeClt(void* AConnID);
   static void __stdcall _SrvOnCltConnect(void* AConnID);
   static void __stdcall _SrvOnCltDisconnect(void* AConnID);
   static void __stdcall _SrvOnCltLogin(void* AConnID);
   static void __stdcall _SrvOnCltLogout(void* AConnID);
   static void __stdcall _SrvOnCltCmdReturn(void* AConnID, void* ACmdID, long AResult);
   static void __stdcall _SrvOnCltRecvData(void* AConnID, const void* AData, long ASize);
};

/* end namespace */

   }
}

// �����ռ�ı���
namespace RC4MSrvObj = RC4SHM::__server__;

#endif
