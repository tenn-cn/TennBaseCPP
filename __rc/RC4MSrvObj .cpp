// =======================================
// Unit   : RC4SHM server object
// Version: 5.0.0.0 (build 2014.07.07)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#include "RC4MSrvObj.h"

namespace RC4SHM
{
   namespace __server__
   {

/* begin namespace */

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TSHMSrvObj - RC4SHM 服务器对象类 */

// ---------------- 静态成员 ----------------

// ---------------- 静态函数 ----------------
// 服务的 OnListen 回调事件
void __stdcall TSHMSrvObj::_SrvOnListen(void* ASrvID)
{
   TSHMSrvObj* objSrv = (TSHMSrvObj*)SHMServerGetObj(ASrvID, msaData, NULL);
   if (objSrv != NULL)
      objSrv->DoListen();
}

// 服务的 OnDisconnect 回调事件
void __stdcall TSHMSrvObj::_SrvOnDisconnect(void* ASrvID)
{
   TSHMSrvObj* objSrv = (TSHMSrvObj*)SHMServerGetObj(ASrvID, msaData, NULL);
   if (objSrv != NULL)
      objSrv->DoDisconnect();
}

// 服务的 OnAccept 回调事件
void __stdcall TSHMSrvObj::_SrvOnAccept(void* ASrvID, void* AConnID, bool& AIsRefused)
{
   // 初始化
   AIsRefused  = true;

   // 取服务对象
   TSHMSrvObj* objSrv = (TSHMSrvObj*)SHMServerGetObj(ASrvID, msaData, NULL);
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
void __stdcall TSHMSrvObj::_SrvOnFreeClt(void* AConnID)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // 激发事件
      try
      {
         TSHMSrvObj* objSrv = (TSHMSrvObj*)objConn->SlotExtObj(tscaData);
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
void __stdcall TSHMSrvObj::_SrvOnCltConnect(void* AConnID)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // 激发事件
      try
      {
         TSHMSrvObj* objSrv = (TSHMSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoCltConnect(objConn);
      }
      catch (...) {}

      // 连接对象引用计数减 1
      DecRefConnObj(objConn);
   }
}

// 连接的 OnCltDisconnect 回调事件
void __stdcall TSHMSrvObj::_SrvOnCltDisconnect(void* AConnID)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // 激发事件
      try
      {
         TSHMSrvObj* objSrv = (TSHMSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoCltDisconnect(objConn);
      }
      catch (...) {}

      // 连接对象引用计数减 1
      DecRefConnObj(objConn);
   }
}

// 连接的 OnCltLogin 回调事件
void __stdcall TSHMSrvObj::_SrvOnCltLogin(void* AConnID)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // 激发事件
      try
      {
         TSHMSrvObj* objSrv = (TSHMSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoCltLogin(objConn);
      }
      catch (...) {}

      // 连接对象引用计数减 1
      DecRefConnObj(objConn);
   }
}

// 连接的 OnCltLogout 回调事件
void __stdcall TSHMSrvObj::_SrvOnCltLogout(void* AConnID)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // 激发事件
      try
      {
         TSHMSrvObj* objSrv = (TSHMSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoCltLogout(objConn);
      }
      catch (...) {}

      // 连接对象引用计数减 1
      DecRefConnObj(objConn);
   }
}

// 连接的 OnCltCmdReturn 回调事件
void __stdcall TSHMSrvObj::_SrvOnCltCmdReturn(void* AConnID, void* ACmdID, long AResult)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // 激发事件
      try
      {
         TSHMSrvObj* objSrv = (TSHMSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoCltCmdReturn(objConn, ACmdID, AResult);
      }
      catch (...) {}

      // 连接对象引用计数减 1
      DecRefConnObj(objConn);
   }
}

// 连接的 OnCltRecvData 回调事件
void __stdcall TSHMSrvObj::_SrvOnCltRecvData(void* AConnID, const void* AData, long ASize)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // 激发事件
      try
      {
         TSHMSrvObj* objSrv = (TSHMSrvObj*)objConn->SlotExtObj(tscaData);
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
TSHMSrvObj::TSHMSrvObj()
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

   // 创建 SHM 服务端对象
   FSrvID                  = SHMServerCreate();
   if (FSrvID != NULL)
   {
      // 设置关联
      SHMServerSetObj(FSrvID, msaData, this);

      // 设置事件的回调函数
      SHMServerSetObj(FSrvID, msaOnListen,         &TSHMSrvObj::_SrvOnListen);
      SHMServerSetObj(FSrvID, msaOnDisconnect,     &TSHMSrvObj::_SrvOnDisconnect);
      SHMServerSetObj(FSrvID, msaOnAccept,         &TSHMSrvObj::_SrvOnAccept);
      SHMServerSetObj(FSrvID, msaOnFreeClt,        &TSHMSrvObj::_SrvOnFreeClt);
      SHMServerSetObj(FSrvID, msaOnCltConnect,     &TSHMSrvObj::_SrvOnCltConnect);
      SHMServerSetObj(FSrvID, msaOnCltDisconnect,  &TSHMSrvObj::_SrvOnCltDisconnect);
      SHMServerSetObj(FSrvID, msaOnCltLogin,       &TSHMSrvObj::_SrvOnCltLogin);
      SHMServerSetObj(FSrvID, msaOnCltLogout,      &TSHMSrvObj::_SrvOnCltLogout);
      SHMServerSetObj(FSrvID, msaOnCltCmdReturn,   &TSHMSrvObj::_SrvOnCltCmdReturn);
      SHMServerSetObj(FSrvID, msaOnCltRecvData,    &TSHMSrvObj::_SrvOnCltRecvData);
   }
}

// 析构函数
TSHMSrvObj::~TSHMSrvObj()
{
   if (FSrvID != NULL)
   {
      // 关闭
      SHMServerClose(FSrvID);

      // 清除关联
      SHMServerSetObj(FSrvID, msaData, NULL);

      // 释放对象
      SHMServerFree(FSrvID);
   }
}

// ---------------- 私有函数 ----------------

// ---------------- 保护函数 ----------------
// 激发 OnListen 事件
void TSHMSrvObj::DoListen()
{
   if (OnListen.Method != NULL)
      try
      {
         ((TObject*)OnListen.Object->*OnListen.Method)(this);
      }
      catch (...) {}
}

// 激发 OnDisconnect 事件
void TSHMSrvObj::DoDisconnect()
{
   if (OnDisconnect.Method != NULL)
      try
      {
         ((TObject*)OnDisconnect.Object->*OnDisconnect.Method)(this);
      }
      catch (...) {}
}

// 激发 OnAccept 事件
void TSHMSrvObj::DoAccept(TRCConnObj* AConnObj, bool& AIsRefused)
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
void TSHMSrvObj::DoFreeClt(TRCConnObj* AConnObj)
{
   if (OnFreeClt.Method != NULL)
      try
      {
         ((TObject*)OnFreeClt.Object->*OnFreeClt.Method)(this, AConnObj);
      }
      catch (...) {}
}

// 激发 OnCltConnect 事件
void TSHMSrvObj::DoCltConnect(TRCConnObj* AConnObj)
{
   if (OnCltConnect.Method != NULL)
      try
      {
         ((TObject*)OnCltConnect.Object->*OnCltConnect.Method)(this, AConnObj);
      }
      catch (...) {}
}

// 激发 OnCltDisconnect 事件
void TSHMSrvObj::DoCltDisconnect(TRCConnObj* AConnObj)
{
   if (OnCltDisconnect.Method != NULL)
      try
      {
         ((TObject*)OnCltDisconnect.Object->*OnCltDisconnect.Method)(this, AConnObj);
      }
      catch (...) {}
}

// 激发 OnCltLogin 事件
void TSHMSrvObj::DoCltLogin(TRCConnObj* AConnObj)
{
   if (OnCltLogin.Method != NULL)
      try
      {
         ((TObject*)OnCltLogin.Object->*OnCltLogin.Method)(this, AConnObj);
      }
      catch (...) {}
}

// 激发 OnCltLogout 事件
void TSHMSrvObj::DoCltLogout(TRCConnObj* AConnObj)
{
   if (OnCltLogout.Method != NULL)
      try
      {
         ((TObject*)OnCltLogout.Object->*OnCltLogout.Method)(this, AConnObj);
      }
      catch (...) {}
}

// 激发 OnCltCmdReturn 事件
void TSHMSrvObj::DoCltCmdReturn(TRCConnObj* AConnObj, void* ACmdID, long AResult)
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
void TSHMSrvObj::DoCltRecvData(TRCConnObj* AConnObj, const void* AData, long ASize)
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
TRCConnObj* TSHMSrvObj::Client(long AIndex) const
{
   // 初始化
   TRCConnObj* result   = NULL;
   void*       pConnID  = SHMServerGetObj(FSrvID, msaBaseSrvClient + AIndex, NULL);

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
