// =======================================
// Unit   : RC4TCP server object
// Version: 5.0.0.0 (build 2014.07.07)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#include "RC4TSrvObj.h"

namespace RC4TCP
{
   namespace __server__
   {

/* begin namespace */

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TTCPSrvObj - RC4TCP 服务器对象类 */

// ---------------- 静态成员 ----------------

// ---------------- 静态函数 ----------------
// 服务的 OnListen 回调事件
void __stdcall TTCPSrvObj::_SrvOnListen(void* ASrvID)
{
   TTCPSrvObj* objSrv = (TTCPSrvObj*)TCPServerGetObj(ASrvID, tsaData, NULL);
   if (objSrv != NULL)
      objSrv->DoListen();
}

// 服务的 OnDisconnect 回调事件
void __stdcall TTCPSrvObj::_SrvOnDisconnect(void* ASrvID)
{
   TTCPSrvObj* objSrv = (TTCPSrvObj*)TCPServerGetObj(ASrvID, tsaData, NULL);
   if (objSrv != NULL)
      objSrv->DoDisconnect();
}

// 服务的 OnAccept 回调事件
void __stdcall TTCPSrvObj::_SrvOnAccept(void* ASrvID, void* AConnID, bool& AIsRefused)
{
   // 初始化
   AIsRefused  = true;

   // 取服务对象
   TTCPSrvObj* objSrv = (TTCPSrvObj*)TCPServerGetObj(ASrvID, tsaData, NULL);
   if (objSrv != NULL)
   {
      TRCConnObj* objConn  = CreateConnObj(AConnID, false, NULL);
      if (objConn != NULL)
      {
         // 激发 OnAccept 事件
         try
         {
            if (objConn->SetSlotExtObj(tscaData, objSrv) == krSuccess)
               objSrv->DoAccept(objConn, AIsRefused);
         }
         catch (...) {}

         // 判断是否拒绝
         if (AIsRefused)
            FreeConnObj(objConn);
      }
   }
}

// 服务的 OnFreeClt 回调事件
void __stdcall TTCPSrvObj::_SrvOnFreeClt(void* AConnID)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // 激发事件
      try
      {
         TTCPSrvObj* objSrv = (TTCPSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoFreeClt(objConn);
      }
      catch (...) {}

      // 释放对象
      FreeConnObj(objConn);
      DecRefConnObj(objConn);
   }
}

// 连接的 OnCltConnect 回调事件
void __stdcall TTCPSrvObj::_SrvOnCltConnect(void* AConnID)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // 激发事件
      try
      {
         TTCPSrvObj* objSrv = (TTCPSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoCltConnect(objConn);
      }
      catch (...) {}

      // 连接对象引用计数减 1
      DecRefConnObj(objConn);
   }
}

// 连接的 OnCltDisconnect 回调事件
void __stdcall TTCPSrvObj::_SrvOnCltDisconnect(void* AConnID)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // 激发事件
      try
      {
         TTCPSrvObj* objSrv = (TTCPSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoCltDisconnect(objConn);
      }
      catch (...) {}

      // 连接对象引用计数减 1
      DecRefConnObj(objConn);
   }
}

// 连接的 OnCltLogin 回调事件
void __stdcall TTCPSrvObj::_SrvOnCltLogin(void* AConnID)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // 激发事件
      try
      {
         TTCPSrvObj* objSrv = (TTCPSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoCltLogin(objConn);
      }
      catch (...) {}

      // 连接对象引用计数减 1
      DecRefConnObj(objConn);
   }
}

// 连接的 OnCltLogout 回调事件
void __stdcall TTCPSrvObj::_SrvOnCltLogout(void* AConnID)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // 激发事件
      try
      {
         TTCPSrvObj* objSrv = (TTCPSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoCltLogout(objConn);
      }
      catch (...) {}

      // 连接对象引用计数减 1
      DecRefConnObj(objConn);
   }
}

// 连接的 OnCltCmdReturn 回调事件
void __stdcall TTCPSrvObj::_SrvOnCltCmdReturn(void* AConnID, void* ACmdID, long AResult)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // 激发事件
      try
      {
         TTCPSrvObj* objSrv = (TTCPSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoCltCmdReturn(objConn, ACmdID, AResult);
      }
      catch (...) {}

      // 连接对象引用计数减 1
      DecRefConnObj(objConn);
   }
}

// 连接的 OnCltRecvData 回调事件
void __stdcall TTCPSrvObj::_SrvOnCltRecvData(void* AConnID, const void* AData, long ASize)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // 激发事件
      try
      {
         TTCPSrvObj* objSrv = (TTCPSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoCltRecvData(objConn, AData, ASize);
      }
      catch (...) {}

      // 连接对象引用计数减 1
      DecRefConnObj(objConn);
   }
}

// ---------------- 构造函数和析构函数 ----------------
// 构造函数
TTCPSrvObj::TTCPSrvObj()
{
   // 初始化
   FData                   = NULL;
   OnListen.Method         = NULL;
   OnListen.Object         = NULL;
   OnDisconnect.Method     = NULL;
   OnDisconnect.Object     = NULL;
   OnAccept.Method         = NULL;
   OnAccept.Object         = NULL;
   OnFreeClt.Method        = NULL;
   OnFreeClt.Object        = NULL;
   OnCltConnect.Method     = NULL;
   OnCltConnect.Object     = NULL;
   OnCltDisconnect.Method  = NULL;
   OnCltDisconnect.Object  = NULL;
   OnCltLogin.Method       = NULL;
   OnCltLogin.Object       = NULL;
   OnCltLogout.Method      = NULL;
   OnCltLogout.Object      = NULL;
   OnCltCmdReturn.Method   = NULL;
   OnCltCmdReturn.Object   = NULL;
   OnCltRecvData.Method    = NULL;
   OnCltRecvData.Object    = NULL;

   // 创建 TCP 服务端对象
   FSrvID                  = TCPServerCreate();
   if (FSrvID != NULL)
   {
      // 设置关联
      TCPServerSetObj(FSrvID, tsaData, this);

      // 设置事件的回调函数
      TCPServerSetObj(FSrvID, tsaOnListen,         &TTCPSrvObj::_SrvOnListen);
      TCPServerSetObj(FSrvID, tsaOnDisconnect,     &TTCPSrvObj::_SrvOnDisconnect);
      TCPServerSetObj(FSrvID, tsaOnAccept,         &TTCPSrvObj::_SrvOnAccept);
      TCPServerSetObj(FSrvID, tsaOnFreeClt,        &TTCPSrvObj::_SrvOnFreeClt);
      TCPServerSetObj(FSrvID, tsaOnCltConnect,     &TTCPSrvObj::_SrvOnCltConnect);
      TCPServerSetObj(FSrvID, tsaOnCltDisconnect,  &TTCPSrvObj::_SrvOnCltDisconnect);
      TCPServerSetObj(FSrvID, tsaOnCltLogin,       &TTCPSrvObj::_SrvOnCltLogin);
      TCPServerSetObj(FSrvID, tsaOnCltLogout,      &TTCPSrvObj::_SrvOnCltLogout);
      TCPServerSetObj(FSrvID, tsaOnCltCmdReturn,   &TTCPSrvObj::_SrvOnCltCmdReturn);
      TCPServerSetObj(FSrvID, tsaOnCltRecvData,    &TTCPSrvObj::_SrvOnCltRecvData);
   }
}

// 析构函数
TTCPSrvObj::~TTCPSrvObj()
{
   if (FSrvID != NULL)
   {
      // 关闭
      TCPServerClose(FSrvID);

      // 清除关联
      TCPServerSetObj(FSrvID, tsaData, NULL);

      // 释放对象
      TCPServerFree(FSrvID);
   }
}

// ---------------- 私有函数 ----------------

// ---------------- 保护函数 ----------------
// 激发 OnListen 事件
void TTCPSrvObj::DoListen()
{
   if (OnListen.Method != NULL)
      try
      {
         ((TObject*)OnListen.Object->*OnListen.Method)(this);
      }
      catch (...) {}
}

// 激发 OnDisconnect 事件
void TTCPSrvObj::DoDisconnect()
{
   if (OnDisconnect.Method != NULL)
      try
      {
         ((TObject*)OnDisconnect.Object->*OnDisconnect.Method)(this);
      }
      catch (...) {}
}

// 激发 OnAccept 事件
void TTCPSrvObj::DoAccept(TRCConnObj* AConnObj, bool& AIsRefused)
{
   if (OnAccept.Method != NULL)
      try
      {
         ((TObject*)OnAccept.Object->*OnAccept.Method)(this, AConnObj, AIsRefused);
      }
      catch (...) {}
   else
      AIsRefused  = false;
}

// 激发 OnFreeClt 事件
void TTCPSrvObj::DoFreeClt(TRCConnObj* AConnObj)
{
   if (OnFreeClt.Method != NULL)
      try
      {
         ((TObject*)OnFreeClt.Object->*OnFreeClt.Method)(this, AConnObj);
      }
      catch (...) {}
}

// 激发 OnCltConnect 事件
void TTCPSrvObj::DoCltConnect(TRCConnObj* AConnObj)
{
   if (OnCltConnect.Method != NULL)
      try
      {
         ((TObject*)OnCltConnect.Object->*OnCltConnect.Method)(this, AConnObj);
      }
      catch (...) {}
}

// 激发 OnCltDisconnect 事件
void TTCPSrvObj::DoCltDisconnect(TRCConnObj* AConnObj)
{
   if (OnCltDisconnect.Method != NULL)
      try
      {
         ((TObject*)OnCltDisconnect.Object->*OnCltDisconnect.Method)(this, AConnObj);
      }
      catch (...) {}
}

// 激发 OnCltLogin 事件
void TTCPSrvObj::DoCltLogin(TRCConnObj* AConnObj)
{
   if (OnCltLogin.Method != NULL)
      try
      {
         ((TObject*)OnCltLogin.Object->*OnCltLogin.Method)(this, AConnObj);
      }
      catch (...) {}
}

// 激发 OnCltLogout 事件
void TTCPSrvObj::DoCltLogout(TRCConnObj* AConnObj)
{
   if (OnCltLogout.Method != NULL)
      try
      {
         ((TObject*)OnCltLogout.Object->*OnCltLogout.Method)(this, AConnObj);
      }
      catch (...) {}
}

// 激发 OnCltCmdReturn 事件
void TTCPSrvObj::DoCltCmdReturn(TRCConnObj* AConnObj, void* ACmdID, long AResult)
{
   if (OnCltCmdReturn.Method != NULL)
      try
      {
         ((TObject*)OnCltCmdReturn.Object->*OnCltCmdReturn.Method)(this,
                                             AConnObj, ACmdID, AResult);
      }
      catch (...) {}
}

// 激发 OnCltRecvData 事件
void TTCPSrvObj::DoCltRecvData(TRCConnObj* AConnObj, const void* AData, long ASize)
{
   if (OnCltRecvData.Method != NULL)
      try
      {
         ((TObject*)OnCltRecvData.Object->*OnCltRecvData.Method)(this,
                                             AConnObj, AData, ASize);
      }
      catch (...) {}
}

// ---------------- 公有函数 ----------------
// 根据索引取已连接的客户端对象
TRCConnObj* TTCPSrvObj::Client(long AIndex) const
{
   // 初始化
   TRCConnObj* result   = NULL;
   void*       pConnID  = TCPServerGetObj(FSrvID, tsaBaseSrvClient + AIndex, NULL);

   // 判断是否非空
   if (pConnID != NULL)
   {
      result = IncRefConnObj(pConnID);
      if (result != NULL)
         DecRefConnObj(result);
   }

   // 返回结果
   return result;
}

/* end namespace */

   }
}
