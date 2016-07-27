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

/* TSHMSrvObj - RC4SHM ������������ */

// ---------------- ��̬��Ա ----------------

// ---------------- ��̬���� ----------------
// ����� OnListen �ص��¼�
void __stdcall TSHMSrvObj::_SrvOnListen(void* ASrvID)
{
   TSHMSrvObj* objSrv = (TSHMSrvObj*)SHMServerGetObj(ASrvID, msaData, NULL);
   if (objSrv != NULL)
      objSrv->DoListen();
}

// ����� OnDisconnect �ص��¼�
void __stdcall TSHMSrvObj::_SrvOnDisconnect(void* ASrvID)
{
   TSHMSrvObj* objSrv = (TSHMSrvObj*)SHMServerGetObj(ASrvID, msaData, NULL);
   if (objSrv != NULL)
      objSrv->DoDisconnect();
}

// ����� OnAccept �ص��¼�
void __stdcall TSHMSrvObj::_SrvOnAccept(void* ASrvID, void* AConnID, bool& AIsRefused)
{
   // ��ʼ��
   AIsRefused  = true;

   // ȡ�������
   TSHMSrvObj* objSrv = (TSHMSrvObj*)SHMServerGetObj(ASrvID, msaData, NULL);
   if (objSrv != NULL)
   {
      TRCConnObj* objConn  = CreateConnObj(AConnID, false, NULL);
      if (objConn != NULL)
      {
         // ���� OnAccept �¼�
         try
         {
            if (objConn->SetSlotExtObj(tscaData, objSrv) == krSuccess)
               objSrv->DoAccept(objConn, AIsRefused);
         }
         catch (...) {}

         // �ж��Ƿ�ܾ�
         if (AIsRefused)
            FreeConnObj(objConn);
      }
   }
}

// ����� OnFreeClt �ص��¼�
void __stdcall TSHMSrvObj::_SrvOnFreeClt(void* AConnID)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // �����¼�
      try
      {
         TSHMSrvObj* objSrv = (TSHMSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoFreeClt(objConn);
      }
      catch (...) {}

      // �ͷŶ���
      FreeConnObj(objConn);
      DecRefConnObj(objConn);
   }
}

// ���ӵ� OnCltConnect �ص��¼�
void __stdcall TSHMSrvObj::_SrvOnCltConnect(void* AConnID)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // �����¼�
      try
      {
         TSHMSrvObj* objSrv = (TSHMSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoCltConnect(objConn);
      }
      catch (...) {}

      // ���Ӷ������ü����� 1
      DecRefConnObj(objConn);
   }
}

// ���ӵ� OnCltDisconnect �ص��¼�
void __stdcall TSHMSrvObj::_SrvOnCltDisconnect(void* AConnID)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // �����¼�
      try
      {
         TSHMSrvObj* objSrv = (TSHMSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoCltDisconnect(objConn);
      }
      catch (...) {}

      // ���Ӷ������ü����� 1
      DecRefConnObj(objConn);
   }
}

// ���ӵ� OnCltLogin �ص��¼�
void __stdcall TSHMSrvObj::_SrvOnCltLogin(void* AConnID)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // �����¼�
      try
      {
         TSHMSrvObj* objSrv = (TSHMSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoCltLogin(objConn);
      }
      catch (...) {}

      // ���Ӷ������ü����� 1
      DecRefConnObj(objConn);
   }
}

// ���ӵ� OnCltLogout �ص��¼�
void __stdcall TSHMSrvObj::_SrvOnCltLogout(void* AConnID)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // �����¼�
      try
      {
         TSHMSrvObj* objSrv = (TSHMSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoCltLogout(objConn);
      }
      catch (...) {}

      // ���Ӷ������ü����� 1
      DecRefConnObj(objConn);
   }
}

// ���ӵ� OnCltCmdReturn �ص��¼�
void __stdcall TSHMSrvObj::_SrvOnCltCmdReturn(void* AConnID, void* ACmdID, long AResult)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // �����¼�
      try
      {
         TSHMSrvObj* objSrv = (TSHMSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoCltCmdReturn(objConn, ACmdID, AResult);
      }
      catch (...) {}

      // ���Ӷ������ü����� 1
      DecRefConnObj(objConn);
   }
}

// ���ӵ� OnCltRecvData �ص��¼�
void __stdcall TSHMSrvObj::_SrvOnCltRecvData(void* AConnID, const void* AData, long ASize)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // �����¼�
      try
      {
         TSHMSrvObj* objSrv = (TSHMSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoCltRecvData(objConn, AData, ASize);
      }
      catch (...) {}

      // ���Ӷ������ü����� 1
      DecRefConnObj(objConn);
   }
}

// ---------------- ���캯������������ ----------------
// ���캯��
TSHMSrvObj::TSHMSrvObj()
{
   // ��ʼ��
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

   // ���� SHM ����˶���
   FSrvID                  = SHMServerCreate();
   if (FSrvID != NULL)
   {
      // ���ù���
      SHMServerSetObj(FSrvID, msaData, this);

      // �����¼��Ļص�����
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

// ��������
TSHMSrvObj::~TSHMSrvObj()
{
   if (FSrvID != NULL)
   {
      // �ر�
      SHMServerClose(FSrvID);

      // �������
      SHMServerSetObj(FSrvID, msaData, NULL);

      // �ͷŶ���
      SHMServerFree(FSrvID);
   }
}

// ---------------- ˽�к��� ----------------

// ---------------- �������� ----------------
// ���� OnListen �¼�
void TSHMSrvObj::DoListen()
{
   if (OnListen.Method != NULL)
      try
      {
         ((TObject*)OnListen.Object->*OnListen.Method)(this);
      }
      catch (...) {}
}

// ���� OnDisconnect �¼�
void TSHMSrvObj::DoDisconnect()
{
   if (OnDisconnect.Method != NULL)
      try
      {
         ((TObject*)OnDisconnect.Object->*OnDisconnect.Method)(this);
      }
      catch (...) {}
}

// ���� OnAccept �¼�
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

// ���� OnFreeClt �¼�
void TSHMSrvObj::DoFreeClt(TRCConnObj* AConnObj)
{
   if (OnFreeClt.Method != NULL)
      try
      {
         ((TObject*)OnFreeClt.Object->*OnFreeClt.Method)(this, AConnObj);
      }
      catch (...) {}
}

// ���� OnCltConnect �¼�
void TSHMSrvObj::DoCltConnect(TRCConnObj* AConnObj)
{
   if (OnCltConnect.Method != NULL)
      try
      {
         ((TObject*)OnCltConnect.Object->*OnCltConnect.Method)(this, AConnObj);
      }
      catch (...) {}
}

// ���� OnCltDisconnect �¼�
void TSHMSrvObj::DoCltDisconnect(TRCConnObj* AConnObj)
{
   if (OnCltDisconnect.Method != NULL)
      try
      {
         ((TObject*)OnCltDisconnect.Object->*OnCltDisconnect.Method)(this, AConnObj);
      }
      catch (...) {}
}

// ���� OnCltLogin �¼�
void TSHMSrvObj::DoCltLogin(TRCConnObj* AConnObj)
{
   if (OnCltLogin.Method != NULL)
      try
      {
         ((TObject*)OnCltLogin.Object->*OnCltLogin.Method)(this, AConnObj);
      }
      catch (...) {}
}

// ���� OnCltLogout �¼�
void TSHMSrvObj::DoCltLogout(TRCConnObj* AConnObj)
{
   if (OnCltLogout.Method != NULL)
      try
      {
         ((TObject*)OnCltLogout.Object->*OnCltLogout.Method)(this, AConnObj);
      }
      catch (...) {}
}

// ���� OnCltCmdReturn �¼�
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

// ���� OnCltRecvData �¼�
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

// ---------------- ���к��� ----------------
// ��������ȡ�����ӵĿͻ��˶���
TRCConnObj* TSHMSrvObj::Client(long AIndex) const
{
   // ��ʼ��
   TRCConnObj* result   = NULL;
   void*       pConnID  = SHMServerGetObj(FSrvID, msaBaseSrvClient + AIndex, NULL);

   // �ж��Ƿ�ǿ�
   if (pConnID != NULL)
   {
      result = IncRefConnObj(pConnID);
      if (result != NULL)
         DecRefConnObj(result);
   }

   // ���ؽ��
   return result;
}

/* end namespace */

   }
}
