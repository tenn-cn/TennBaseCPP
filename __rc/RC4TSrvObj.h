// =======================================
// Unit   : RC4TCP server object
// Version: 5.0.0.0 (build 2014.07.07)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _RC4TSrvObj_H_
#define _RC4TSrvObj_H_

#include "RC4TCP.h"
#include "RCKObjs.h"
using namespace RCKObjs;

namespace RC4TCP
{
   namespace __server__
   {

/* begin namespace */

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TTCPSrvObj - RC4TCP 服务器对象类 */

class TTCPSrvObj
{
public:
   // 通知事件
   typedef void (TObject::*TDoNotify)(TTCPSrvObj* ASrvObj);
   typedef struct
   {
      TDoNotify      Method;
      void*          Object;
   } TOnNotify;

   // 接受连接事件
   typedef void (TObject::*TDoAccept)(TTCPSrvObj* ASrvObj, TRCConnObj* AConnObj,
                                            bool& AIsRefused);
   typedef struct
   {
      TDoAccept      Method;
      void*          Object;
   } TOnAccept;

   // 客户连接事件
   typedef void (TObject::*TDoCltEvent)(TTCPSrvObj* ASrvObj, TRCConnObj* AConnObj);
   typedef struct
   {
      TDoCltEvent    Method;
      void*          Object;
   } TOnCltEvent;

   // 连接命令返回事件
   typedef void (TObject::*TDoCmdReturn)(TTCPSrvObj* ASrvObj, TRCConnObj* AConnObj,
                                               void* ACmdID,        long  AResult);
   typedef struct
   {
      TDoCmdReturn   Method;
      void*          Object;
   } TOnCmdReturn;

   // 接收自定义数据事件
   typedef void (TObject::*TDoRecvData)(TTCPSrvObj* ASrvObj, TRCConnObj* AConnObj,
                                        const void* AData,         long  ASize);
   typedef struct
   {
      TDoRecvData    Method;
      void*          Object;
   } TOnRecvData;

public:
   TTCPSrvObj();
   virtual ~TTCPSrvObj();

   // 属性
   void*          Data() const         { return FData; }
   void*          SrvID() const        { return FSrvID; }
   long           State() const        { return TCPServerGetInt(FSrvID, tsaState, NULL); }
   KYString       Error() const        { return TCPServerGetStr(FSrvID, tsaError, NULL); }

   KYString       Addr() const         { return TCPServerGetStr(FSrvID, tsaAddr, NULL); }
   long           Port() const         { return TCPServerGetInt(FSrvID, tsaPort, NULL); }
   long           Linger() const       { return TCPServerGetInt(FSrvID, tsaLinger, NULL); }
   long           ListenQueue() const  { return TCPServerGetInt(FSrvID, tsaListenQueue, NULL); }
   long           RecvThreads() const  { return TCPServerGetInt(FSrvID, tsaRecvThreads, NULL); }
   long           CacheThreads() const { return TCPServerGetInt(FSrvID, tsaCacheThreads, NULL); }

   TRCConnObj*    Client(long AIndex) const;
   bool           CanLogin() const     { return TCPServerGetInt(FSrvID, tsaCanLogin, NULL) == 1; }
   bool           CanBindApp() const   { return TCPServerGetInt(FSrvID, tsaCanBindApp, NULL) == 1; }
   long           MaxClients() const   { return TCPServerGetInt(FSrvID, tsaMaxClientCount, NULL); }
   long           ClientCount() const  { return TCPServerGetInt(FSrvID, tsaSrvClientCount, NULL); }
   long           BindTimeout() const  { return TCPServerGetInt(FSrvID, tsaBindTimeout, NULL); }
   long           SendMaxSize() const  { return TCPServerGetInt(FSrvID, tsaSendMaxSize, NULL); }
   long           SendQueueSize() const{ return TCPServerGetInt(FSrvID, tsaSendQueueSize, NULL); }

   long           Timeout() const      { return TCPServerGetInt(FSrvID, tsaTimeout, NULL); }
   long           KeepTimeout() const  { return TCPServerGetInt(FSrvID, tsaKeepTimeout, NULL); }
   long           KeepInterval() const { return TCPServerGetInt(FSrvID, tsaKeepInterval, NULL); }
   long           KeepRetryTimes()const{ return TCPServerGetInt(FSrvID, tsaKeepRetryTimes, NULL); }

   // 设置属性
   void           SetData(void* AData) { FData = AData; }
   long           SetAddr(const KYString& AHost)
                  { return TCPServerSetStr(FSrvID, tsaAddr, (char*)AHost, AHost.Length()); }
   long           SetPort(long APort)
                  { return TCPServerSetInt(FSrvID, tsaPort, APort); }
   long           SetLinger(long ALinger)
                  { return TCPServerSetInt(FSrvID, tsaLinger, ALinger); }
   long           SetListenQueue(long AListenQueue)
                  { return TCPServerSetInt(FSrvID, tsaListenQueue, AListenQueue); }
   long           SetRecvThreads(long ACount)
                  { return TCPServerSetInt(FSrvID, tsaRecvThreads, ACount); }
   long           SetCacheThreads(long ACount)
                  { return TCPServerSetInt(FSrvID, tsaCacheThreads, ACount); }

   long           SetCanLogin(bool ACanLogin)
                  { return TCPServerSetInt(FSrvID, tsaCanLogin, ACanLogin); }
   long           SetCanBindApp(bool ACanBind)
                  { return TCPServerSetInt(FSrvID, tsaCanBindApp, ACanBind); }
   long           SetMaxClients(long AMaxCount)
                  { return TCPServerSetInt(FSrvID, tsaMaxClientCount, AMaxCount); }
   long           SetBindTimeout(long ATimeout)
                  { return TCPServerSetInt(FSrvID, tsaBindTimeout, ATimeout); }
   long           SetSendMaxSize(long AMaxSize)
                  { return TCPServerSetInt(FSrvID, tsaSendMaxSize, AMaxSize); }
   long           SetSendQueueSize(long AQueueSize)
                  { return TCPServerSetInt(FSrvID, tsaSendQueueSize, AQueueSize); }

   long           SetTimeout(long ATimeout)
                  { return TCPServerSetInt(FSrvID, tsaTimeout, ATimeout); }
   long           SetKeepTimeout(long ATimeout)
                  { return TCPServerSetInt(FSrvID, tsaKeepTimeout, ATimeout); }
   long           SetKeepInterval(long AInterval)
                  { return TCPServerSetInt(FSrvID, tsaKeepInterval, AInterval); }
   long           SetKeepRetryTimes(long ARetryTimes)
                  { return TCPServerSetInt(FSrvID, tsaKeepRetryTimes, ARetryTimes); }

   // 打开/关闭 TCP 服务端
   long           Open()               { return TCPServerOpen(FSrvID); }
   long           Close()              { return TCPServerClose(FSrvID); }

   // 事件
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
   // 执行事件方法
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
   void*          FData;               // 自定义数据
   void*          FSrvID;              // 服务器 ID

private:
   // 服务器事件的回调函数
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

// 命名空间的别名
namespace RC4TSrvObj = RC4TCP::__server__;

#endif
