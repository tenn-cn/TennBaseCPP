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

/* TTCPSrvObj - RC4TCP ������������ */

// ---------------- ��̬��Ա ----------------

// ---------------- ��̬���� ----------------
// ����� OnListen �ص��¼�
void __stdcall TTCPSrvObj::_SrvOnListen(void* ASrvID)
{
   TTCPSrvObj* objSrv = (TTCPSrvObj*)TCPServerGetObj(ASrvID, tsaData, NULL);
   if (objSrv != NULL)
      objSrv->DoListen();
}

// ����� OnDisconnect �ص��¼�
void __stdcall TTCPSrvObj::_SrvOnDisconnect(void* ASrvID)
{
   TTCPSrvObj* objSrv = (TTCPSrvObj*)TCPServerGetObj(ASrvID, tsaData, NULL);
   if (objSrv != NULL)
      objSrv->DoDisconnect();
}

// ����� OnAccept �ص��¼�
void __stdcall TTCPSrvObj::_SrvOnAccept(void* ASrvID, void* AConnID, bool& AIsRefused)
{
   // ��ʼ��
   AIsRefused  = true;

   // ȡ�������
   TTCPSrvObj* objSrv = (TTCPSrvObj*)TCPServerGetObj(ASrvID, tsaData, NULL);
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
void __stdcall TTCPSrvObj::_SrvOnFreeClt(void* AConnID)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // �����¼�
      try
      {
         TTCPSrvObj* objSrv = (TTCPSrvObj*)objConn->SlotExtObj(tscaData);
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
void __stdcall TTCPSrvObj::_SrvOnCltConnect(void* AConnID)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // �����¼�
      try
      {
         TTCPSrvObj* objSrv = (TTCPSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoCltConnect(objConn);
      }
      catch (...) {}

      // ���Ӷ������ü����� 1
      DecRefConnObj(objConn);
   }
}

// ���ӵ� OnCltDisconnect �ص��¼�
void __stdcall TTCPSrvObj::_SrvOnCltDisconnect(void* AConnID)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // �����¼�
      try
      {
         TTCPSrvObj* objSrv = (TTCPSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoCltDisconnect(objConn);
      }
      catch (...) {}

      // ���Ӷ������ü����� 1
      DecRefConnObj(objConn);
   }
}

// ���ӵ� OnCltLogin �ص��¼�
void __stdcall TTCPSrvObj::_SrvOnCltLogin(void* AConnID)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // �����¼�
      try
      {
         TTCPSrvObj* objSrv = (TTCPSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoCltLogin(objConn);
      }
      catch (...) {}

      // ���Ӷ������ü����� 1
      DecRefConnObj(objConn);
   }
}

// ���ӵ� OnCltLogout �ص��¼�
void __stdcall TTCPSrvObj::_SrvOnCltLogout(void* AConnID)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // �����¼�
      try
      {
         TTCPSrvObj* objSrv = (TTCPSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoCltLogout(objConn);
      }
      catch (...) {}

      // ���Ӷ������ü����� 1
      DecRefConnObj(objConn);
   }
}

// ���ӵ� OnCltCmdReturn �ص��¼�
void __stdcall TTCPSrvObj::_SrvOnCltCmdReturn(void* AConnID, void* ACmdID, long AResult)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // �����¼�
      try
      {
         TTCPSrvObj* objSrv = (TTCPSrvObj*)objConn->SlotExtObj(tscaData);
         if (objSrv != NULL)
            objSrv->DoCltCmdReturn(objConn, ACmdID, AResult);
      }
      catch (...) {}

      // ���Ӷ������ü����� 1
      DecRefConnObj(objConn);
   }
}

// ���ӵ� OnCltRecvData �ص��¼�
void __stdcall TTCPSrvObj::_SrvOnCltRecvData(void* AConnID, const void* AData, long ASize)
{
   TRCConnObj* objConn = IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      // �����¼�
      try
      {
         TTCPSrvObj* objSrv = (TTCPSrvObj*)objConn->SlotExtObj(tscaData);
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
TTCPSrvObj::TTCPSrvObj()
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

   // ���� TCP ����˶���
   FSrvID                  = TCPServerCreate();
   if (FSrvID != NULL)
   {
      // ���ù���
      TCPServerSetObj(FSrvID, tsaData, this);

      // �����¼��Ļص�����
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

// ��������
TTCPSrvObj::~TTCPSrvObj()
{
   if (FSrvID != NULL)
   {
      // �ر�
      TCPServerClose(FSrvID);

      // �������
      TCPServerSetObj(FSrvID, tsaData, NULL);

      // �ͷŶ���
      TCPServerFree(FSrvID);
   }
}

// ---------------- ˽�к��� ----------------

// ---------------- �������� ----------------
// ���� OnListen �¼�
void TTCPSrvObj::DoListen()
{
   if (OnListen.Method != NULL)
      try
      {
         ((TObject*)OnListen.Object->*OnListen.Method)(this);
      }
      catch (...) {}
}

// ���� OnDisconnect �¼�
void TTCPSrvObj::DoDisconnect()
{
   if (OnDisconnect.Method != NULL)
      try
      {
         ((TObject*)OnDisconnect.Object->*OnDisconnect.Method)(this);
      }
      catch (...) {}
}

// ���� OnAccept �¼�
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

// ���� OnFreeClt �¼�
void TTCPSrvObj::DoFreeClt(TRCConnObj* AConnObj)
{
   if (OnFreeClt.Method != NULL)
      try
      {
         ((TObject*)OnFreeClt.Object->*OnFreeClt.Method)(this, AConnObj);
      }
      catch (...) {}
}

// ���� OnCltConnect �¼�
void TTCPSrvObj::DoCltConnect(TRCConnObj* AConnObj)
{
   if (OnCltConnect.Method != NULL)
      try
      {
         ((TObject*)OnCltConnect.Object->*OnCltConnect.Method)(this, AConnObj);
      }
      catch (...) {}
}

// ���� OnCltDisconnect �¼�
void TTCPSrvObj::DoCltDisconnect(TRCConnObj* AConnObj)
{
   if (OnCltDisconnect.Method != NULL)
      try
      {
         ((TObject*)OnCltDisconnect.Object->*OnCltDisconnect.Method)(this, AConnObj);
      }
      catch (...) {}
}

// ���� OnCltLogin �¼�
void TTCPSrvObj::DoCltLogin(TRCConnObj* AConnObj)
{
   if (OnCltLogin.Method != NULL)
      try
      {
         ((TObject*)OnCltLogin.Object->*OnCltLogin.Method)(this, AConnObj);
      }
      catch (...) {}
}

// ���� OnCltLogout �¼�
void TTCPSrvObj::DoCltLogout(TRCConnObj* AConnObj)
{
   if (OnCltLogout.Method != NULL)
      try
      {
         ((TObject*)OnCltLogout.Object->*OnCltLogout.Method)(this, AConnObj);
      }
      catch (...) {}
}

// ���� OnCltCmdReturn �¼�
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

// ���� OnCltRecvData �¼�
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

// ---------------- ���к��� ----------------
// ��������ȡ�����ӵĿͻ��˶���
TRCConnObj* TTCPSrvObj::Client(long AIndex) const
{
   // ��ʼ��
   TRCConnObj* result   = NULL;
   void*       pConnID  = TCPServerGetObj(FSrvID, tsaBaseSrvClient + AIndex, NULL);

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
