// =======================================
// Unit   : RCKernel objects
// Version: 5.0.0.0 (build 2014.07.07)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

//#include <stdio.h>
//#include <stdarg.h>

#include "RCKObjs.h"

// ʹ�� RCKernel �������ռ�
namespace RCKernel
{
   namespace __object__
   {

/* begin namespace */

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TRCKObjs - RCK ������ */

class TRCKObjs
{
private:
   // ӳ���б�����
   typedef TKYMapObjKey<TRCAppObj>     TAppList;
   typedef TKYMapObjKey<TRCConnObj>    TConnList;
   typedef TKYMapObjKey<TRCFuncObj>    TFuncList;

public:
   TRCKObjs();
   virtual ~TRCKObjs();

   // ���
   void           Clear();

   // ���/ɾ������ID
   bool           AddFuncID(void* AFuncID, TRCFuncObj* AFuncObj)
                  { return (FFuncIDs->Add(AFuncID, AFuncObj) != NULL); }
   bool           DeleteFuncID(void* AFuncID)
                  { return FFuncIDs->Delete(AFuncID); }

   // ����ID -> ����
   TRCFuncObj*    FuncID2Object(void* AFuncID)
                  {
                     TRCFuncObj* result = NULL;
                     FFuncIDs->Find(AFuncID, result);
                     return result;
                  }

   // ����/�ͷ�Ӧ�ö���
   TRCAppObj*     CreateAppObj(const char* AName, const char* APassword, long& ARetCode);
   void           FreeAppObj(TRCAppObj* AppObj);

   // Ӧ�ö������ü����� 1
   TRCAppObj*     IncRefAppObj(void* AppID);
   bool           IncRefAppObj(TRCAppObj* AppObj);

   // ����/�ͷ����Ӷ���(ע: AConnID �������)
   TRCConnObj*    CreateConnObj(void* AConnID, bool ANeedFree, long& ARetCode);
   void           FreeConnObj(TRCConnObj* AConnObj);

   // ���Ӷ������ü����� 1
   TRCConnObj*    IncRefConnObj(void* AConnID);
   bool           IncRefConnObj(TRCConnObj* AConnObj);

   // ����/�ͷ��������
   TRCCmdObj*     CreateCmdObj(void* AConnID = NULL);
   void           FreeCmdObj(TRCCmdObj* ACmdObj);

protected:
   // ��ǰ��
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

private:
   // ���������
   bool           DoClearAppObjs(TKYList& AList);
   bool           DoClearConnObjs(TKYList& AList);
   bool           DoClearCmdQueue(TKYList& AList);

   // �¼�����
   void           DoAppDeletion(TRCAppObj* AppObj);
   void           DoCmdDeletion(TRCCmdObj* ACmdObj);
   void           DoConnDeletion(TRCConnObj* AConnObj);

private:
   TKYCritSect*   FLock;               // ��ǰ��
   TAppList*      FAppIDs;             // Ӧ�� ID �б�
   TAppList*      FAppObjs;            // Ӧ�ö����б�
   TFuncList*     FFuncIDs;            // ���� ID �б�
   TConnList*     FConnIDs;            // ���� ID �б�
   TConnList*     FConnObjs;           // ���Ӷ����б�
   TKYCycQueue*   FCmdQueue;           // ����������

public:
   // ȫ�� RCK ���󼯶���
   static TRCKObjs*     _obj()         { return _Object; }

   // ��ʼ��/�ͷ� _Object
   static void          _InitObj();
   static void          _FreeObj(bool AForced = false);

   // Ӧ�ö������ü�����/�� 1(ע: AppObj ����������)
   static void          _IncRefAppObj(TRCAppObj* AppObj)
                        { AppObj->IncObjTimes(); }
   static void          _DecRefAppObj(TRCAppObj* AppObj)
                        { if (AppObj->DecObjTimes() == 0) delete AppObj; }

   // ���Ӷ������ü�����/�� 1(ע: AConnObj ����������)
   static void          _IncRefConnObj(TRCConnObj* AConnObj)
                        { AConnObj->IncObjTimes(); }
   static void          _DecRefConnObj(TRCConnObj* AConnObj)
                        { if (AConnObj->DecObjTimes() == 0) delete AConnObj; }

private:
   // ȫ�� RCK ���󼯶���
   static TKYCritSect*  _Lock;
   static TRCKObjs*     _Object;
   static long          _RefCount;

   // TRCKObjs �ľ�̬��Ա��ʼ����
   static class TInitialization
   {
   public:
      TInitialization();
      ~TInitialization();
   } _Initialization;

   friend class TInitialization;
};

/* TRCKObjs - RCK ������ */

// ---------------- ��̬��Ա ----------------

// ȫ��������
TKYCritSect*   TRCKObjs::_Lock      = NULL;
TRCKObjs*      TRCKObjs::_Object    = NULL;
long           TRCKObjs::_RefCount  = 0;

// TRCKObjs �ľ�̬��Ա��ʼ������
TRCKObjs::TInitialization TRCKObjs::_Initialization;

// ---------------- ��̬���� ----------------
// TRCKObjs �ľ�̬��Ա��ʼ����Ĺ��캯��
TRCKObjs::TInitialization::TInitialization()
{
   // ��ʼ��
   _RefCount   = 0;
   _Object     = NULL;

   // ��������
   _Lock       = new TKYCritSect;
}

// TRCKObjs �ľ�̬��Ա��ʼ�������������
TRCKObjs::TInitialization::~TInitialization()
{
   // �ͷŶ���
   FreeAndNil(_Lock);
   FreeAndNil(_Object);
}

// ��ʼ�� _Object
void TRCKObjs::_InitObj()
{
   _Lock->Enter();
   if (_RefCount == 0)
      try
      {
         // �����������ü�����Ϊ 1
         _Object     = new TRCKObjs;
         _RefCount   = 1;

         // ��ʼ�� RCKernel �ӿ�
         RCKInitialize();
      }
      catch (...) {}
   else
      _RefCount++;
   _Lock->Leave();
}

// �ͷ� _Object
void TRCKObjs::_FreeObj(bool AForced)
{
   _Lock->Enter();
   if (_RefCount > 0)
      try
      {
         // ���ü����� 1
         _RefCount--;

         // �ж��Ƿ���Ҫ�ͷ�
         if ((_RefCount == 0) || AForced)
         {
            // ���ü�����Ϊ 0
            _RefCount   = 0;

            // ������ͷŶ���
            _Object->Clear();
            FreeAndNil(_Object);

            // �ͷ� RCKernel �ӿ�
            RCKUninitialize();
         }
      }
      catch (...) {}
   _Lock->Leave();
}

// ---------------- ���캯������������ ----------------
// ���캯��
TRCKObjs::TRCKObjs()
{
   // ��������
   FLock       = new TKYCritSect;
   FAppIDs     = new TAppList(false, false);
   FAppObjs    = new TAppList(false, false);
   FFuncIDs    = new TFuncList(false, true);
   FConnIDs    = new TConnList(false, false);
   FConnObjs   = new TConnList(false, false);
   FCmdQueue   = new TKYCycQueue(0, TKYCycQueue::slPower10);

   // ���ö�������
   FCmdQueue->SetMaxCount(FCmdQueue->CycCount());
}

// ��������
TRCKObjs::~TRCKObjs()
{
   // ���
   Clear();

   // �ͷŶ���
   FreeAndNil(FLock);
   FreeAndNil(FAppIDs);
   FreeAndNil(FAppObjs);
   FreeAndNil(FFuncIDs);
   FreeAndNil(FConnIDs);
   FreeAndNil(FConnObjs);
   FreeAndNil(FCmdQueue);
}

// ---------------- ˽�к��� ----------------
// ���������Ӧ�ö����б�
bool TRCKObjs::DoClearAppObjs(TKYList& AList)
{
   // ��ʼ��
   bool result = false;

   // ����б��Ƿ�ǿ�
   if (FAppObjs->Count() > 0)
      try
      {
         // ��������
         AList.ChangeCapacity(FAppObjs->Count());

         // ѭ������
         void* pNode = FAppObjs->Next(NULL);
         while (pNode != NULL)
         {
            AList.Add(FAppObjs->Value(pNode));
            pNode = FAppObjs->Next(pNode);
         }

         // ���
         FAppIDs->Clear();
         FAppObjs->Clear();

         // ���ؽ��
         result = true;
      }
      catch (...) {}

   // ���ؽ��
   return result;
}

// ������������Ӷ����б�
bool TRCKObjs::DoClearConnObjs(TKYList& AList)
{
   // ��ʼ��
   bool result = false;

   // ����б��Ƿ�ǿ�
   if (FConnObjs->Count() > 0)
      try
      {
         // ��������
         AList.ChangeCapacity(FConnObjs->Count());

         // ѭ������
         void* pNode = FConnObjs->Next(NULL);
         while (pNode != NULL)
         {
            AList.Add(FConnObjs->Value(pNode));
            pNode = FConnObjs->Next(pNode);
         }

         // ���
         FConnIDs->Clear();
         FConnObjs->Clear();

         // ���ؽ��
         result = true;
      }
      catch (...) {}

   // ���ؽ��
   return result;
}

// �������������������
bool TRCKObjs::DoClearCmdQueue(TKYList& AList)
{
   // ��ʼ��
   bool result = false;

   // ����б��Ƿ�ǿ�
   if (FCmdQueue->Count() != 0)
      try
      {
         // ��ʼ��
         void* pData;

         // ����������ѭ����������
         AList.ChangeCapacity(FCmdQueue->Count());
         while (FCmdQueue->Pop(pData))
            AList.Add(pData);

         // ���ؽ��
         result = true;
      }
      catch (...) {}

   // ���ؽ��
   return result;
}

// �б�� OnDeletion �¼�����
void TRCKObjs::DoAppDeletion(TRCAppObj* AppObj)
{
   _DecRefAppObj(AppObj);
}

// �б�� OnDeletion �¼�����
void TRCKObjs::DoCmdDeletion(TRCCmdObj* ACmdObj)
{
   delete ACmdObj;
}

// �б�� OnDeletion �¼�����
void TRCKObjs::DoConnDeletion(TRCConnObj* AConnObj)
{
   _DecRefConnObj(AConnObj);
}

// ---------------- ���к��� ----------------
// ���
void TRCKObjs::Clear()
{
   // ��ʼ��
   TKYList  objApps(false);
   TKYList  objCmds(false);
   TKYList  objConns(false);
   bool     boolApp  = false;
   bool     boolCmd  = false;
   bool     boolConn = false;

   // ��������б�
   FFuncIDs->Clear();

   // ����������б�
   Lock();
   {
      boolApp  = DoClearAppObjs(objApps);
      boolCmd  = DoClearCmdQueue(objCmds);
      boolConn = DoClearConnObjs(objConns);
   }
   Unlock();

   // �ж��Ƿ���Ҫ���Ӧ��
   if (boolApp)
   {
      // ���� OnDeletion �¼�����
      objApps.OnDeletion.Object  = this;
      objApps.OnDeletion.Method  = (TKYList::TDoDeletion)&TRCKObjs::DoAppDeletion;

      // ���
      objApps.Clear();
   }

   // �ж��Ƿ���Ҫ�������
   if (boolCmd)
   {
      // ���� OnDeletion �¼�����
      objCmds.OnDeletion.Object  = this;
      objCmds.OnDeletion.Method  = (TKYList::TDoDeletion)&TRCKObjs::DoCmdDeletion;

      // ���
      objCmds.Clear();
   }

   // �ж��Ƿ���Ҫ�������
   if (boolConn)
   {
      // ���� OnDeletion �¼�����
      objConns.OnDeletion.Object = this;
      objConns.OnDeletion.Method = (TKYList::TDoDeletion)&TRCKObjs::DoConnDeletion;

      // ���
      objConns.Clear();
   }
}

// ����Ӧ�ö���
TRCAppObj* TRCKObjs::CreateAppObj(const char* AName, const char* APassword, long& ARetCode)
{
   // ��ʼ��
   TRCAppObj*  result   = NULL;
   KYString    strName  = AName;
   void*       pAppID   = RCKAppCreate((char*)strName, APassword);

   // �ж��Ƿ񴴽��ɹ�
   if (pAppID != NULL)
   {
      ARetCode = krUnknown;

      // ��������, �������б�
      try
      {
         TRCAppObj* objApp = new TRCAppObj;
         if (objApp != NULL)
         {
            Lock();
            try
            {
               // ��ӵ� ID �б�
               void* pNode = FAppIDs->Add(pAppID, objApp);
               if (pNode == NULL)
               {
                  pNode = FAppIDs->Find(pAppID);
                  if (pNode != NULL)
                  {
                     FAppObjs->Delete(FAppIDs->Value(pNode));
                     FAppIDs->SetValue(pNode, objApp);
                  }
               }

               // ��ӵ������б�
               if (pNode == NULL)
                  ;
               else if ((FAppObjs->Add(objApp, objApp) != NULL)
                     || FAppObjs->Existed(objApp))
               {
                  // ���ö�������
                  objApp->FName  = strName;
                  objApp->FAppID = pAppID;
                  result         = objApp;
                  ARetCode       = krSuccess;
               }
               else
                  FAppIDs->Remove(pNode);
            }
            catch (...) {}
            Unlock();

            // ��ʧ�����ͷŶ���
            if (result == NULL)
               delete objApp;
         }
      }
      catch (...) {}

      // ��ʧ�����ͷ�
      if (result == NULL)
         RCKAppFree(pAppID);
   }
   else
      ARetCode = RCKGetLastError();

   // ���ؽ��
   return result;
}

// �ͷ�Ӧ�ö���
void TRCKObjs::FreeAppObj(TRCAppObj* AppObj)
{
   // ��ʼ��
   TRCAppObj* objApp = NULL;

   // ���Ҳ��ͷ�
   Lock();
   try
   {
      void* pNode = FAppObjs->Find(AppObj);
      if (pNode != NULL)
      {
         objApp   = FAppObjs->Value(pNode);
         FAppObjs->Remove(pNode);

         // �ͷ� ID ��
         pNode    = FAppIDs->Find(objApp->AppID());
         if ((pNode != NULL) && (FAppIDs->Value(pNode) == objApp))
            FAppIDs->Remove(pNode);
      }
   }
   catch (...) {}
   Unlock();

   // �����������ü����� 1
   if (objApp != NULL)
      _DecRefAppObj(objApp);
}

// Ӧ�ö������ü����� 1
TRCAppObj* TRCKObjs::IncRefAppObj(void* AppID)
{
   // ��ʼ��
   TRCAppObj* result = NULL;

   // ����
   Lock();
   try
   {
      void* pNode = FAppIDs->Find(AppID);
      if (pNode != NULL)
      {
         result   = FAppIDs->Value(pNode);
         result->IncObjTimes();
      }
   }
   catch (...) {}
   Unlock();

   // ���ؽ��
   return result;
}

// Ӧ�ö������ü����� 1
bool TRCKObjs::IncRefAppObj(TRCAppObj* AppObj)
{
   // ��ʼ��
   bool result = false;

   // ����
   Lock();
   try
   {
      void* pNode = FAppObjs->Find(AppObj);
      if (pNode != NULL)
      {
         AppObj->IncObjTimes();
         result   = true;
      }
   }
   catch (...) {}
   Unlock();

   // ���ؽ��
   return result;
}

// �������Ӷ���(ע: AConnID �������)
TRCConnObj* TRCKObjs::CreateConnObj(void* AConnID, bool ANeedFree, long& ARetCode)
{
   // ��ʼ��
   TRCConnObj* result = NULL;

   // ��������Ƿ����
   RCKConnGetInt(AConnID, kcaState, &ARetCode);
   if (ARetCode == krSuccess)
   {
      ARetCode = krUnknown;

      // ��������, �������б�
      try
      {
         TRCConnObj* objConn = new TRCConnObj;
         if (objConn != NULL)
         {
            Lock();
            try
            {
               // ��ӵ� ID �б�
               void* pNode = FConnIDs->Add(AConnID, objConn);
               if (pNode == NULL)
               {
                  if (FConnIDs->Existed(AConnID))
                     ARetCode          = krIsExisted;
               }
               else if ((FConnObjs->Add(objConn, objConn) != NULL)
                     || FConnObjs->Existed(objConn))
               {
                  // ���ö�������
                  objConn->FConnID     = AConnID;
                  objConn->FCanEvent   = true;
                  objConn->FNeedFree   = ANeedFree;
                  result               = objConn;
                  ARetCode             = krSuccess;
               }
               else
                  FConnIDs->Remove(pNode);
            }
            catch (...) {}
            Unlock();

            // ��ʧ�����ͷŶ���
            if (result == NULL)
               delete objConn;
         }
      }
      catch (...) {}
   }

   // ���ؽ��
   return result;
}

// �ͷ����Ӷ���
void TRCKObjs::FreeConnObj(TRCConnObj* AConnObj)
{
   // ��ʼ��
   TRCConnObj* objConn = NULL;

   // ���Ҳ��ͷ�
   Lock();
   try
   {
      void* pNode = FConnObjs->Find(AConnObj);
      if (pNode != NULL)
      {
         objConn  = FConnObjs->Value(pNode);
         FConnObjs->Remove(pNode);

         // �ͷ� ID ��
         pNode    = FConnIDs->Find(objConn->ConnID());
         if ((pNode != NULL) && (FConnIDs->Value(pNode) == objConn))
            FConnIDs->Remove(pNode);
      }
   }
   catch (...) {}
   Unlock();

   // �����������ü����� 1
   if (objConn != NULL)
      _DecRefConnObj(objConn);
}

// ���Ӷ������ü����� 1
TRCConnObj* TRCKObjs::IncRefConnObj(void* AConnID)
{
   // ��ʼ��
   TRCConnObj* result = NULL;

   // ����
   Lock();
   try
   {
      void* pNode = FConnIDs->Find(AConnID);
      if (pNode != NULL)
      {
         result   = FConnIDs->Value(pNode);
         result->IncObjTimes();
      }
   }
   catch (...) {}
   Unlock();

   // ���ؽ��
   return result;
}

// ���Ӷ������ü����� 1
bool TRCKObjs::IncRefConnObj(TRCConnObj* AConnObj)
{
   // ��ʼ��
   bool result = false;

   // ����
   Lock();
   try
   {
      void* pNode = FConnObjs->Find(AConnObj);
      if (pNode != NULL)
      {
         AConnObj->IncObjTimes();
         result   = true;
      }
   }
   catch (...) {}
   Unlock();

   // ���ؽ��
   return result;
}

// �����������
TRCCmdObj* TRCKObjs::CreateCmdObj(void* AConnID)
{
   // ��ʼ��
   TRCCmdObj* result = NULL;

   // ��������
   Lock();
   try
   {
      result = (TRCCmdObj*)FCmdQueue->Pop();
   }
   catch (...) {}
   Unlock();

   // �ж��Ƿ�Ϊ��
   if (result == NULL)
      result = new TRCCmdObj;

   // ��������
   result->SetConnID(AConnID);

   // ���ؽ��
   return result;
}

// �ͷ��������
void TRCKObjs::FreeCmdObj(TRCCmdObj* ACmdObj)
{
   if (ACmdObj != NULL)
   {
      // ��ʼ��
      bool boolPush = false;

      // �ر�����
      ACmdObj->End();

      // ѹ�����
      Lock();
      try
      {
         boolPush = FCmdQueue->Push(ACmdObj);
      }
      catch (...) {}
      Unlock();

      // ѹ�����ʧ�����ͷŶ���
      if (!boolPush)
         delete ACmdObj;
   }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ��ʼ��/�ͷŵ�Ԫ */

// ��ʼ�� RCKObjs ��Ԫ(ע: �ڲ������ü���)
void InitRCKObjs()
{
   TRCKObjs::_InitObj();
}

// �ͷ� RCKObjs ��Ԫ(ע: �ڲ������ü���)
void FreeRCKObjs(bool AForced)
{
   TRCKObjs::_FreeObj(AForced);
}

// ����ID -> ����
TRCCmdObj* CmdID2Object(void* ACmdID)
{
   return (TRCCmdObj*)RCKCmdGetObj(ACmdID, kmaData, NULL);
}

// ����ID -> ����
TRCFuncObj* FuncID2Object(void* AFuncID)
{
   // ��ʼ��
   TRCFuncObj* result = NULL;

   // �ж��Ƿ��Ѿ���ʼ��
   if (TRCKObjs::_obj() != NULL)
      result = TRCKObjs::_obj()->FuncID2Object(AFuncID);

   // ���ؽ��
   return result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* Ӧ�ö���(TRCAppObj)����غ��� */

// ����Ӧ�ö���
TRCAppObj* CreateAppObj(const char* AName, const char* APassword, long* ARetCode)
{
   // ��ʼ��
   TRCAppObj* result  = NULL;
   long       intCode = krNotExist;

   // �ж��Ƿ��Ѿ���ʼ��
   if (TRCKObjs::_obj() != NULL)
      result = TRCKObjs::_obj()->CreateAppObj(AName, APassword, intCode);

   // ���÷�����
   if (ARetCode != NULL)
      *ARetCode = intCode;

   // ���ؽ��
   return result;
}

// �ͷ�Ӧ�ö���
void FreeAppObj(TRCAppObj* AppObj)
{
   // �ж��Ƿ��Ѿ���ʼ��
   if (TRCKObjs::_obj() != NULL)
      TRCKObjs::_obj()->FreeAppObj(AppObj);
}

// Ӧ�ö������ü����� 1
TRCAppObj* IncRefAppObj(void* AppID)
{
   // ��ʼ��
   TRCAppObj* result = NULL;

   // �ж��Ƿ��Ѿ���ʼ��
   if (TRCKObjs::_obj() != NULL)
      result = TRCKObjs::_obj()->IncRefAppObj(AppID);

   // ���ؽ��
   return result;
}

// Ӧ�ö������ü����� 1
bool IncRefAppObj_(TRCAppObj* AppObj)
{
   return (TRCKObjs::_obj() != NULL) && TRCKObjs::_obj()->IncRefAppObj(AppObj);
}

// Ӧ�ö������ü����� 1(ע: AppObj ����������)
void IncRefAppObj(TRCAppObj* AppObj)
{
   TRCKObjs::_IncRefAppObj(AppObj);
}

// Ӧ�ö������ü����� 1(ע: AppObj ����������)
void DecRefAppObj(TRCAppObj* AppObj)
{
   TRCKObjs::_DecRefAppObj(AppObj);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ���Ӷ���(TRCConnObj)����غ��� */

// �������Ӷ���(ע: AConnID �������)
TRCConnObj* CreateConnObj(void* AConnID, bool ANeedFree, long* ARetCode)
{
   // ��ʼ��
   TRCConnObj* result  = NULL;
   long        intCode = krNotExist;

   // �ж��Ƿ��Ѿ���ʼ��
   if (TRCKObjs::_obj() != NULL)
      result = TRCKObjs::_obj()->CreateConnObj(AConnID, ANeedFree, intCode);

   // ���÷�����
   if (ARetCode != NULL)
      *ARetCode = intCode;

   // ���ؽ��
   return result;
}

// �ͷ����Ӷ���
void FreeConnObj(TRCConnObj* AConnObj)
{
   // �ж��Ƿ��Ѿ���ʼ��
   if (TRCKObjs::_obj() != NULL)
      TRCKObjs::_obj()->FreeConnObj(AConnObj);
}

// ���Ӷ������ü����� 1
TRCConnObj* IncRefConnObj(void* AConnID)
{
   // ��ʼ��
   TRCConnObj* result = NULL;

   // �ж��Ƿ��Ѿ���ʼ��
   if (TRCKObjs::_obj() != NULL)
      result = TRCKObjs::_obj()->IncRefConnObj(AConnID);

   // ���ؽ��
   return result;
}

// ���Ӷ������ü����� 1
bool IncRefConnObj_(TRCConnObj* AConnObj)
{
   return (TRCKObjs::_obj() != NULL) && TRCKObjs::_obj()->IncRefConnObj(AConnObj);
}

// ���Ӷ������ü����� 1(ע: AConnObj ����������)
void IncRefConnObj(TRCConnObj* AConnObj)
{
   TRCKObjs::_IncRefConnObj(AConnObj);
}

// ���Ӷ������ü����� 1(ע: AConnObj ����������)
void DecRefConnObj(TRCConnObj* AConnObj)
{
   TRCKObjs::_DecRefConnObj(AConnObj);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �������(TRCCmdObj)����غ��� */

// �����������
TRCCmdObj* CreateCmdObj(void* AConnID)
{
   // ��ʼ��
   TRCCmdObj* result = NULL;

   // �ж��Ƿ��Ѿ���ʼ��
   if (TRCKObjs::_obj() != NULL)
      result = TRCKObjs::_obj()->CreateCmdObj(AConnID);

   // ���ؽ��
   return result;
}

// �����������(ע: AConnObj �������)
TRCCmdObj* CreateCmdObj(TRCConnObj* AConnObj)
{
   // ��ʼ��
   TRCCmdObj* result = NULL;

   // �ж��Ƿ��Ѿ���ʼ��
   if (TRCKObjs::_obj() != NULL)
      result = TRCKObjs::_obj()->CreateCmdObj(AConnObj->ConnID());

   // ���ؽ��
   return result;
}

// �ͷ��������
void FreeCmdObj(TRCCmdObj* ACmdObj)
{
   // �ж��Ƿ��Ѿ���ʼ��
   if (TRCKObjs::_obj() != NULL)
      TRCKObjs::_obj()->FreeCmdObj(ACmdObj);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TRCAppObj - RCK Ӧ���� */

// ---------------- ��̬���� ----------------
// Ӧ�õ� OnConnLogin �ص��¼�
void __stdcall TRCAppObj::_RCKOnConnLogin(void* AppID, void* AConnID, bool& AIsRefused)
{
   TRCAppObj* objApp = TRCKObjs::_obj()->IncRefAppObj(AppID);
   if (objApp != NULL)
   {
      objApp->DoConnLogin(AConnID, AIsRefused);
      TRCKObjs::_DecRefAppObj(objApp);
   }
   else
      AIsRefused  = true;
}

// Ӧ�õ� OnConnLogout �ص��¼�
void __stdcall TRCAppObj::_RCKOnConnLogout(void* AppID, void* AConnID)
{
   TRCAppObj* objApp = TRCKObjs::_obj()->IncRefAppObj(AppID);
   if (objApp != NULL)
   {
      objApp->DoConnLogout(AConnID);
      TRCKObjs::_DecRefAppObj(objApp);
   }
}

// Ӧ�õ� OnExecResp �ص��¼�
void __stdcall TRCAppObj::_RCKOnExecResp(void* AppID, void* AFuncID, void* AConnID)
{
   TRCAppObj* objApp = TRCKObjs::_obj()->IncRefAppObj(AppID);
   if (objApp != NULL)
   {
      objApp->DoExecResp(AFuncID, AConnID);
      TRCKObjs::_DecRefAppObj(objApp);
   }
}

// Ӧ�õ� OnRespCancel �ص��¼�
void __stdcall TRCAppObj::_RCKOnRespCancel(void* AppID, void* AFuncID, void* AConnID)
{
   TRCAppObj* objApp = TRCKObjs::_obj()->IncRefAppObj(AppID);
   if (objApp != NULL)
   {
      objApp->DoRespCancel(AFuncID, AConnID);
      TRCKObjs::_DecRefAppObj(objApp);
   }
}

// Ӧ�õ� OnRespConfirm �ص��¼�
void __stdcall TRCAppObj::_RCKOnRespConfirm(void* AppID, void* AFuncID, void* AConnID)
{
   TRCAppObj* objApp = TRCKObjs::_obj()->IncRefAppObj(AppID);
   if (objApp != NULL)
   {
      objApp->DoRespConfirm(AFuncID, AConnID);
      TRCKObjs::_DecRefAppObj(objApp);
   }
}

// ---------------- ���캯������������ ----------------
// ���캯��
TRCAppObj::TRCAppObj()
{
   // ��ʼ��
   FData       = NULL;
   FAppID      = NULL;
   FObjTimes   = 1;

   // ��������
   FLock       = new TKYCritSect;
   FFuncList   = new TObjList(false, false);

   // ִ�г�ʼ���¼�
   DoInitEvent();
}

// ��������
TRCAppObj::~TRCAppObj()
{
   // �ж��Ƿ�ǿ�
   if (FAppID != NULL)
   {
      // �ر�
      Close();

      // ִ����������б�
      DoClearFuncObjs();

      // �ͷ�Ӧ�ö���
      RCKAppFree(FAppID);
   }

   // �ͷŶ���
   FreeAndNil(FLock);
   FreeAndNil(FFuncList);
}

// ---------------- ˽�к��� ----------------
// ִ�г�ʼ���¼�
void TRCAppObj::DoInitEvent()
{
   OnFuncFree.Method       = NULL;
   OnFuncFree.Object       = NULL;

   FOnConnLogin.Method     = NULL;
   FOnConnLogin.Object     = NULL;
   FOnConnLogout.Method    = NULL;
   FOnConnLogout.Object    = NULL;

   FOnExecResp.Method      = NULL;
   FOnExecResp.Object      = NULL;
   FOnRespCancel.Method    = NULL;
   FOnRespCancel.Object    = NULL;
   FOnRespConfirm.Method   = NULL;
   FOnRespConfirm.Object   = NULL;
}

// ִ����������б�
void TRCAppObj::DoClearFuncObjs()
{
   // ��ʼ��
   TKYList  objIDs(false);
   TKYList  objObjs(false);
   bool     boolNext = false;

   // ���������
   Lock();
   if (FFuncList->Count() > 0)
      try
      {
         // ��������
         objIDs.ChangeCapacity(FFuncList->Count());
         objObjs.ChangeCapacity(FFuncList->Count());

         // ѭ������
         void* pNode = FFuncList->Next(NULL);
         while (pNode != NULL)
         {
            objIDs.Add(FFuncList->Value(pNode));
            objObjs.Add(FFuncList->Key(pNode));
            pNode = FFuncList->Next(pNode);
         }

         // ���
         FFuncList->Clear();
         boolNext = true;
      }
      catch (...) {}
   Unlock();

   // �ж��Ƿ����
   if (boolNext)
   {
      // ���� OnDeletion �¼�����
      objIDs.OnDeletion.Object   = this;
      objIDs.OnDeletion.Method   = (TKYList::TDoDeletion)&TRCAppObj::DoIDDeletion;
      objObjs.OnDeletion.Object  = this;
      objObjs.OnDeletion.Method  = (TKYList::TDoDeletion)&TRCAppObj::DoObjDeletion;

      // ���
      objIDs.Clear();
      objObjs.Clear();
   }
}

// ���� OnConnLogin �¼�
void TRCAppObj::DoConnLogin(void* AConnID, bool& AIsRefused)
{
   try
   {
      AIsRefused = true;
      ((TObject*)FOnConnLogin.Object->*FOnConnLogin.Method)(this, AConnID, AIsRefused);
   }
   catch (...) {}
}

// ���� OnConnLogout �¼�
void TRCAppObj::DoConnLogout(void* AConnID)
{
   try
   {
      ((TObject*)FOnConnLogout.Object->*FOnConnLogout.Method)(this, AConnID);
   }
   catch (...) {}
}

// ���� OnExecResp �¼�
void TRCAppObj::DoExecResp(void* AFuncID, void* AConnID)
{
   try
   {
      ((TObject*)FOnExecResp.Object->*FOnExecResp.Method)(this, AFuncID, AConnID);
   }
   catch (...) {}
}

// ���� OnRespCancel �¼�
void TRCAppObj::DoRespCancel(void* AFuncID, void* AConnID)
{
   try
   {
      ((TObject*)FOnRespCancel.Object->*FOnRespCancel.Method)(this, AFuncID, AConnID);
   }
   catch (...) {}
}

// ���� OnRespConfirm �¼�
void TRCAppObj::DoRespConfirm(void* AFuncID, void* AConnID)
{
   try
   {
      ((TObject*)FOnRespConfirm.Object->*FOnRespConfirm.Method)(this, AFuncID, AConnID);
   }
   catch (...) {}
}

// �б�� OnDeletion �¼�����
void TRCAppObj::DoIDDeletion(void* AFuncID)
{
   // ��ȫ���б���ɾ��
   if (TRCKObjs::_obj()->DeleteFuncID(AFuncID))
      RCKFuncFree(AFuncID);
}

// �б�� OnDeletion �¼�����
void TRCAppObj::DoObjDeletion(TRCFuncObj* AFuncObj)
{
   // ���� OnFuncFree �¼�
   if (OnFuncFree.Method != NULL)
      try
      {
         ((TObject*)OnFuncFree.Object->*OnFuncFree.Method)(this, AFuncObj);
      }
      catch (...) {}

   // �ͷ�
   InterlockedExchangePointer((Pointer*)&AFuncObj->FAppObj, NULL);
   delete AFuncObj;
}

// ---------------- ���к��� ----------------
// ���� Name ����
long TRCAppObj::SetName(const KYString& AName)
{
   // ��ʼ��
   long result = krUnknown;

   // ����
   Lock();
   if (FAppID != NULL)
   {
      result = RCKAppSetStr(FAppID, kaaName, (char*)AName, AName.Length());
      if (result == krSuccess)
         FName = AName;
   }
   Unlock();

   // ���ؽ��
   return result;
}

// �½���������
TRCFuncObj* TRCAppObj::NewFuncObj(const char* AName, long ARetType,  long  ARetSize,
                                                     long ACallMode, void* ACallback)
{
   // ��ʼ��
   TRCFuncObj* result   = NULL;
   KYString    strName  = AName;
   void*       pFuncID  = RCKFuncCreate(FAppID, ACallMode, ACallback,
                                        (char*)strName, ARetType, ARetSize);

   // �ж��Ƿ񴴽��ɹ�
   if (pFuncID != NULL)
   {
      // �������󲢼����б�
      try
      {
         TRCFuncObj* objFunc  = new TRCFuncObj;
         if (objFunc != NULL)
         {
            Lock();
            try
            {
               if (FFuncList->Add(objFunc, pFuncID) != NULL)
               {
                  objFunc->FName    = strName;
                  objFunc->FFuncID  = pFuncID;
                  objFunc->FAppObj  = this;
                  result            = objFunc;

                  // �����б�
                  TRCKObjs::_obj()->AddFuncID(pFuncID, objFunc);
               }
            }
            catch (...) {}
            Unlock();

            // ��ʧ�����ͷŶ���
            if (result == NULL)
               delete objFunc;
         }
      }
      catch (...) {}

      // ��ʧ�����ͷź���
      if (result == NULL)
         RCKFuncFree(pFuncID);
   }

   // ���ؽ��
   return result;
}

// ɾ����������
bool TRCAppObj::DeleteFuncObj(TRCFuncObj* AFuncObj)
{
   // ��ʼ��
   bool result = false;

   // ���״̬
   if ((FAppID != NULL) && (State() == kasDefining))
   {
      // ��ʼ��
      void* pFuncID  = NULL;

      // ���б���ɾ��
      Lock();
      try
      {
         void* pNode = FFuncList->Find(AFuncObj);
         if (pNode != NULL)
         {
            pFuncID  = FFuncList->Value(pFuncID);
            result   = true;
            FFuncList->Remove(pNode);
         }
      }
      catch (...) {}
      Unlock();

      // �ж��Ƿ�ɹ�
      if (result)
      {
         DoIDDeletion(pFuncID);
         DoObjDeletion(AFuncObj);
      }
   }

   // ���ؽ��
   return result;
}

// �����������
void TRCAppObj::ClearFuncObjs()
{
   // ���״̬
   if ((FAppID != NULL) && (State() == kasDefining))
      DoClearFuncObjs();
}

// ���� OnConnLogin �¼�
long TRCAppObj::SetOnConnLogin(TDoConnLogin AMethod, void* AObject)
{
   // ��ʼ��
   long result = krUnknown;

   // ����
   Lock();
   if (FAppID != NULL)
   {
      void* pFunc = (AMethod != NULL) ? &_RCKOnConnLogin : NULL;
      result = RCKAppSetObj(FAppID, kaaOnConnLogin, pFunc);
      if (result != krSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnConnLogin.Object = AObject;
         FOnConnLogin.Method = AMethod;
      }
      else
      {
         FOnConnLogin.Method = NULL;
         FOnConnLogin.Object = NULL;
      }
   }
   Unlock();

   // ���ؽ��
   return result;
}

// ���� OnConnLogout �¼�
long TRCAppObj::SetOnConnLogout(TDoConnLogout AMethod, void* AObject)
{
   // ��ʼ��
   long result = krUnknown;

   // ����
   Lock();
   if (FAppID != NULL)
   {
      void* pFunc = (AMethod != NULL) ? &_RCKOnConnLogout : NULL;
      result = RCKAppSetObj(FAppID, kaaOnConnLogout, pFunc);
      if (result != krSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnConnLogout.Object = AObject;
         FOnConnLogout.Method = AMethod;
      }
      else
      {
         FOnConnLogout.Method = NULL;
         FOnConnLogout.Object = NULL;
      }
   }
   Unlock();

   // ���ؽ��
   return result;
}

// ���� OnExecResp �¼�
long TRCAppObj::SetOnExecResp(TDoRespEvent AMethod, void* AObject)
{
   // ��ʼ��
   long result = krUnknown;

   // ����
   Lock();
   if (FAppID != NULL)
   {
      void* pFunc = (AMethod != NULL) ? &_RCKOnExecResp : NULL;
      result = RCKAppSetObj(FAppID, kaaOnExecResp, pFunc);
      if (result != krSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnExecResp.Object = AObject;
         FOnExecResp.Method = AMethod;
      }
      else
      {
         FOnExecResp.Method = NULL;
         FOnExecResp.Object = NULL;
      }
   }
   Unlock();

   // ���ؽ��
   return result;
}

// ���� OnRespCancel �¼�
long TRCAppObj::SetOnRespCancel(TDoRespEvent AMethod, void* AObject)
{
   // ��ʼ��
   long result = krUnknown;

   // ����
   Lock();
   if (FAppID != NULL)
   {
      void* pFunc = (AMethod != NULL) ? &_RCKOnRespCancel : NULL;
      result = RCKAppSetObj(FAppID, kaaOnRespCancel, pFunc);
      if (result != krSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnRespCancel.Object = AObject;
         FOnRespCancel.Method = AMethod;
      }
      else
      {
         FOnRespCancel.Method = NULL;
         FOnRespCancel.Object = NULL;
      }
   }
   Unlock();

   // ���ؽ��
   return result;
}

// ���� OnRespConfirm �¼�
long TRCAppObj::SetOnRespConfirm(TDoRespEvent AMethod, void* AObject)
{
   // ��ʼ��
   long result = krUnknown;

   // ����
   Lock();
   if (FAppID != NULL)
   {
      void* pFunc = (AMethod != NULL) ? &_RCKOnRespConfirm : NULL;
      result = RCKAppSetObj(FAppID, kaaOnRespConfirm, pFunc);
      if (result != krSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnRespConfirm.Object = AObject;
         FOnRespConfirm.Method = AMethod;
      }
      else
      {
         FOnRespConfirm.Method = NULL;
         FOnRespConfirm.Object = NULL;
      }
   }
   Unlock();

   // ���ؽ��
   return result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TRCFuncObj - RCK ������ */

// ---------------- ���캯������������ ----------------
// ���캯��
TRCFuncObj::TRCFuncObj()
{
   FData       = NULL;
   FFuncID     = NULL;
   FAppObj     = NULL;
}

// ��������
TRCFuncObj::~TRCFuncObj()
{
   TRCAppObj* objApp = (TRCAppObj*)InterlockedExchangePointer((Pointer*)&FAppObj, NULL);
   if (objApp != NULL)
      objApp->DeleteFuncObj(this);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TRCConnObj - RCK ������ */

// ---------------- ��̬���� ----------------
// ���ӵ� OnConnect �ص��¼�
void __stdcall TRCConnObj::_RCKOnConnect(void* AConnID)
{
   TRCConnObj* objConn = TRCKObjs::_obj()->IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      objConn->DoConnect();
      TRCKObjs::_DecRefConnObj(objConn);
   }
}

// ���ӵ� OnDisconnect �ص��¼�
void __stdcall TRCConnObj::_RCKOnDisconnect(void* AConnID)
{
   TRCConnObj* objConn = TRCKObjs::_obj()->IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      objConn->DoDisconnect();
      TRCKObjs::_DecRefConnObj(objConn);
   }
}

// ���ӵ� OnLogin �ص��¼�
void __stdcall TRCConnObj::_RCKOnLogin(void* AConnID)
{
   TRCConnObj* objConn = TRCKObjs::_obj()->IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      objConn->DoLogin();
      TRCKObjs::_DecRefConnObj(objConn);
   }
}

// ���ӵ� OnLogout �ص��¼�
void __stdcall TRCConnObj::_RCKOnLogout(void* AConnID)
{
   TRCConnObj* objConn = TRCKObjs::_obj()->IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      objConn->DoLogout();
      TRCKObjs::_DecRefConnObj(objConn);
   }
}

// ���ӵ� OnCmdReturn �ص��¼�
void __stdcall TRCConnObj::_RCKOnCmdReturn(void* AConnID, void* ACmdID, long AResult)
{
   TRCConnObj* objConn = TRCKObjs::_obj()->IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      objConn->DoCmdReturn(ACmdID, AResult);
      TRCKObjs::_DecRefConnObj(objConn);
   }
}

// ���ӵ� OnRecvData �ص��¼�
void __stdcall TRCConnObj::_RCKOnRecvData(void* AConnID, const void* AData, long ASize)
{
   TRCConnObj* objConn = TRCKObjs::_obj()->IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      objConn->DoRecvData(AData, ASize);
      TRCKObjs::_DecRefConnObj(objConn);
   }
}

// ---------------- ���캯������������ ----------------
// ���캯��
TRCConnObj::TRCConnObj(void* AConnID, bool ANeedFree)
{
   // ��ʼ��
   FData          = NULL;
   FConnID        = AConnID;
   FObjTimes      = 1;
   FCanEvent      = false;
   FNeedFree      = ANeedFree && (AConnID != NULL);

   // ִ�г�ʼ���¼�
   DoInitEvent();
}

// ��������
TRCConnObj::~TRCConnObj()
{
   // �ж��Ƿ���Ҫ�ͷ�����
   if (FNeedFree && (FConnID != NULL))
      RCKConnFree(FConnID);
}

// ---------------- ˽�к��� ----------------
// ִ�г�ʼ���¼�
void TRCConnObj::DoInitEvent()
{
   FOnConnect.Method    = NULL;
   FOnConnect.Object    = NULL;
   FOnDisconnect.Method = NULL;
   FOnDisconnect.Object = NULL;

   FOnLogin.Method      = NULL;
   FOnLogin.Object      = NULL;
   FOnLogout.Method     = NULL;
   FOnLogout.Object     = NULL;

   FOnCmdReturn.Method  = NULL;
   FOnCmdReturn.Object  = NULL;
   FOnRecvData.Method   = NULL;
   FOnRecvData.Object   = NULL;
}

// ���� OnConnect �¼�
void TRCConnObj::DoConnect()
{
   try
   {
      ((TObject*)FOnConnect.Object->*FOnConnect.Method)(this);
   }
   catch (...) {}
}

// ���� OnDisconnect �¼�
void TRCConnObj::DoDisconnect()
{
   try
   {
      ((TObject*)FOnDisconnect.Object->*FOnDisconnect.Method)(this);
   }
   catch (...) {}
}

// ���� OnLogin �¼�
void TRCConnObj::DoLogin()
{
   try
   {
      ((TObject*)FOnLogin.Object->*FOnLogin.Method)(this);
   }
   catch (...) {}
}

// ���� OnLogout �¼�
void TRCConnObj::DoLogout()
{
   try
   {
      ((TObject*)FOnLogout.Object->*FOnLogout.Method)(this);
   }
   catch (...) {}
}

// ���� OnCmdReturn �¼�
void TRCConnObj::DoCmdReturn(void* ACmdID, long AResult)
{
   try
   {
      ((TObject*)FOnCmdReturn.Object->*FOnCmdReturn.Method)(this, ACmdID, AResult);
   }
   catch (...) {}
}

// ���� OnRecvData �¼�
void TRCConnObj::DoRecvData(const void* AData, long ASize)
{
   try
   {
      ((TObject*)FOnRecvData.Object->*FOnRecvData.Method)(this, AData, ASize);
   }
   catch (...) {}
}

// ---------------- ���к��� ----------------
// ���� OnConnect �¼�
long TRCConnObj::SetOnConnect(TDoNotify AMethod, void* AObject)
{
   // ��ʼ��
   long result = krUnknown;

   // ������
   if (FConnID == NULL)
      ;
   else if (!FCanEvent)
      result = krFailure;
   else
   {
      void* pFunc = (AMethod != NULL) ? &_RCKOnConnect : NULL;
      result = RCKConnSetObj(FConnID, kcaOnConnect, pFunc);
      if (result != krSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnConnect.Object = AObject;
         FOnConnect.Method = AMethod;
      }
      else
      {
         FOnConnect.Method = NULL;
         FOnConnect.Object = NULL;
      }
   }

   // ���ؽ��
   return result;
}

// ���� OnDisconnect �¼�
long TRCConnObj::SetOnDisconnect(TDoNotify AMethod, void* AObject)
{
   // ��ʼ��
   long result = krUnknown;

   // ������
   if (FConnID == NULL)
      ;
   else if (!FCanEvent)
      result = krFailure;
   else
   {
      void* pFunc = (AMethod != NULL) ? &_RCKOnDisconnect : NULL;
      result = RCKConnSetObj(FConnID, kcaOnDisconnect, pFunc);
      if (result != krSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnDisconnect.Object = AObject;
         FOnDisconnect.Method = AMethod;
      }
      else
      {
         FOnDisconnect.Method = NULL;
         FOnDisconnect.Object = NULL;
      }
   }

   // ���ؽ��
   return result;
}

// ���� OnLogin �¼�
long TRCConnObj::SetOnLogin(TDoNotify AMethod, void* AObject)
{
   // ��ʼ��
   long result = krUnknown;

   // ������
   if (FConnID == NULL)
      ;
   else if (!FCanEvent)
      result = krFailure;
   else
   {
      void* pFunc = (AMethod != NULL) ? &_RCKOnLogin : NULL;
      result = RCKConnSetObj(FConnID, kcaOnLogin, pFunc);
      if (result != krSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnLogin.Object = AObject;
         FOnLogin.Method = AMethod;
      }
      else
      {
         FOnLogin.Method = NULL;
         FOnLogin.Object = NULL;
      }
   }

   // ���ؽ��
   return result;
}

// ���� OnLogout �¼�
long TRCConnObj::SetOnLogout(TDoNotify AMethod, void* AObject)
{
   // ��ʼ��
   long result = krUnknown;

   // ������
   if (FConnID == NULL)
      ;
   else if (!FCanEvent)
      result = krFailure;
   else
   {
      void* pFunc = (AMethod != NULL) ? &_RCKOnLogout : NULL;
      result = RCKConnSetObj(FConnID, kcaOnLogout, pFunc);
      if (result != krSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnLogout.Object = AObject;
         FOnLogout.Method = AMethod;
      }
      else
      {
         FOnLogout.Method = NULL;
         FOnLogout.Object = NULL;
      }
   }

   // ���ؽ��
   return result;
}

// ���� OnCmdReturn �¼�
long TRCConnObj::SetOnCmdReturn(TDoCmdReturn AMethod, void* AObject)
{
   // ��ʼ��
   long result = krUnknown;

   // ������
   if (FConnID == NULL)
      ;
   else if (!FCanEvent)
      result = krFailure;
   else
   {
      void* pFunc = (AMethod != NULL) ? &_RCKOnCmdReturn : NULL;
      result = RCKConnSetObj(FConnID, kcaOnCmdReturn, pFunc);
      if (result != krSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnCmdReturn.Object = AObject;
         FOnCmdReturn.Method = AMethod;
      }
      else
      {
         FOnCmdReturn.Method = NULL;
         FOnCmdReturn.Object = NULL;
      }
   }

   // ���ؽ��
   return result;
}

// ���� OnRecvData �¼�
long TRCConnObj::SetOnRecvData(TDoRecvData AMethod, void* AObject)
{
   // ��ʼ��
   long result = krUnknown;

   // ������
   if (FConnID == NULL)
      ;
   else if (!FCanEvent)
      result = krFailure;
   else
   {
      void* pFunc = (AMethod != NULL) ? &_RCKOnRecvData : NULL;
      result = RCKConnSetObj(FConnID, kcaOnRecvData, pFunc);
      if (result != krSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnRecvData.Object = AObject;
         FOnRecvData.Method = AMethod;
      }
      else
      {
         FOnRecvData.Method = NULL;
         FOnRecvData.Object = NULL;
      }
   }

   // ���ؽ��
   return result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TRCCmdObj - RCK ������ */

// ---------------- ���캯������������ ----------------
// ���캯��
TRCCmdObj::TRCCmdObj()
{
   FData    = NULL;
   FCmdID   = RCKCmdCreate();

   // ���ù�������
   if (FCmdID != NULL)
      RCKCmdSetObj(FCmdID, kmaData, this);
}

// ��������
TRCCmdObj::~TRCCmdObj()
{
   if (FCmdID != NULL)
   {
      // �������
      RCKCmdSetObj(FCmdID, kmaData, NULL);

      // �ͷŶ���
      RCKCmdFree(FCmdID);
   }
}

// ---------------- ���к��� ----------------
// ȡ���Ӷ���
TRCConnObj* TRCCmdObj::ConnObj() const
{
   // ��ʼ��
   TRCConnObj* result   = NULL;
   void*       pConnID  = RCKCmdGetObj(FCmdID, kmaConnObj, NULL);

   // �ж��Ƿ�ǿ�
   if (pConnID != NULL)
   {
      result = TRCKObjs::_obj()->IncRefConnObj(pConnID);
      if (result != NULL)
         TRCKObjs::_DecRefConnObj(result);
   }

   // ���ؽ��
   return result;
}

// ִ�д�����������
long TRCCmdObj::ExecByParams(Longword ATimeout, ...) const
{
   // ȡ��������
   long intCount = ParamCount();
   if (intCount > 0)
   {
      // ��ʼ��
      const void* pUnknow;

      // �������
      RCKCmdParamClear(FCmdID);

      // �ɱ������ʼ��
      va_list  arrArgs;
      va_start(arrArgs, ATimeout);

      // �������
      for (long intNo = 0; intNo < intCount; intNo++)
         switch (ParamType(intNo))
         {
         case kdtString:
            RCKCmdParamByStr(FCmdID, intNo, va_arg(arrArgs, const char*), -1);
            break;

         case kdtInteger:
            RCKCmdParamByInt(FCmdID, intNo, va_arg(arrArgs, int));
            break;

         case kdtBoolean:
            RCKCmdParamByBool(FCmdID, intNo, va_arg(arrArgs, bool));
            break;

         case kdtDouble:
            RCKCmdParamByFloat(FCmdID, intNo, va_arg(arrArgs, double));
            break;

         case kdtStruct:
            RCKCmdParamByStruct(FCmdID, intNo, va_arg(arrArgs, void*));
            break;

         case kdtByte:
            RCKCmdParamByInt(FCmdID, intNo, va_arg(arrArgs, Byte));
            break;

         case kdtWord:
            RCKCmdParamByInt(FCmdID, intNo, va_arg(arrArgs, Word));
            break;

         case kdtSmallint:
            RCKCmdParamByInt(FCmdID, intNo, va_arg(arrArgs, Smallint));
            break;

         case kdtInt64:
            RCKCmdParamByInt64(FCmdID, intNo, va_arg(arrArgs, __int64));
            break;

         case kdtDateTime:
            RCKCmdParamByDate(FCmdID, intNo, va_arg(arrArgs, double));
            break;

         case kdtCurrency:
            RCKCmdParamByFloat(FCmdID, intNo, va_arg(arrArgs, double));
            break;

         default:
            pUnknow = va_arg(arrArgs, const void*);
         }

      // �ɱ�����ͷ�
      va_end(arrArgs);
   }

   // ִ��
   return RCKCmdExecute(FCmdID, ATimeout);
}

// ȡ����ֵ���ַ���ֵ
KYString TRCCmdObj::RetAsStr() const
{
   // ��ʼ��
   KYString result;
   long     intSize;
   char*    pRet;

   // ȡ����ֵ
   pRet  = RCKCmdRetStr(FCmdID, &intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // ���ؽ��
   return result;
}

// ȡ�����ֶε��ַ���ֵ
KYString TRCCmdObj::FieldAsStr(long AIndex) const
{
   // ��ʼ��
   KYString result;
   long     intSize;
   char*    pRet;

   // ȡ�����ֶ�ֵ
   pRet  = RCKCmdFieldStr(FCmdID, AIndex, &intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // ���ؽ��
   return result;
}

// ȡ�����ֶε��ַ���ֵ
KYString TRCCmdObj::FieldAsStr(const char* AName) const
{
   // ��ʼ��
   KYString result;
   long     intSize;
   char*    pRet;

   // ȡ�����ֶ�ֵ
   pRet  = RCKCmdFieldStr_(FCmdID, AName, &intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // ���ؽ��
   return result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TRCResp - RCK Ӧ���� */

// ---------------- ��̬���� ----------------
// ȡ����ֵ���ַ���ֵ
KYString TRCResp::RetAsStr()
{
   // ��ʼ��
   KYString result;
   long     intSize;
   char*    pRet;

   // ȡ����ֵ
   pRet  = RCKRespRetStr(&intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // ���ؽ��
   return result;
}

// ȡ�����ֶε��ַ���ֵ
KYString TRCResp::FieldAsStr(long AIndex)
{
   // ��ʼ��
   KYString result;
   long     intSize;
   char*    pRet;

   // ȡ�����ֶ�ֵ
   pRet  = RCKRespFieldStr(AIndex, &intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // ���ؽ��
   return result;
}

// ȡ�����ֶε��ַ���ֵ
KYString TRCResp::FieldAsStr(const char* AName)
{
   // ��ʼ��
   KYString result;
   long     intSize;
   char*    pRet;

   // ȡ�����ֶ�ֵ
   pRet  = RCKRespFieldStr_(AName, &intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // ���ؽ��
   return result;
}

// ȡ�������ַ���ֵ
KYString TRCResp::ParamAsStr(long AIndex)
{
   // ��ʼ��
   KYString result;
   long     intSize;
   char*    pRet;

   // ȡ����ֵ
   pRet  = RCKRespParamStr(AIndex, &intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // ���ؽ��
   return result;
}

// ȡ�������ַ���ֵ
KYString TRCResp::ParamAsStr(const char* AName)
{
   // ��ʼ��
   KYString result;
   long     intSize;
   char*    pRet;

   // ȡ����ֵ
   pRet  = RCKRespParamStr_(AName, &intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // ���ؽ��
   return result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TRCFields - RCK �ֶμ��� */

// ---------------- ���캯������������ ----------------
// ���캯��
TRCFields::TRCFields()
{
   FFields  = RCKFieldsCreate();
   FIsBound = (FFields != NULL);
}

TRCFields::TRCFields(void* AFields)
{
   FFields  = AFields;
   FIsBound = false;
}

// ��������
TRCFields::~TRCFields()
{
   if (FIsBound)
      RCKFieldsFree(FFields);
}

// ---------------- ���к��� ----------------
// ȡ�ֶζ��弯�ַ���
KYString TRCFields::FieldDefs() const
{
   // ��ʼ��
   KYString result;
   long     intSize;
   char*    pRet;

   // ȡ�ֶ�ֵ
   pRet  = (char*)RCKFieldDefsOutput(FFields, &intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // ���ؽ��
   return result;
}

// ȡ�ֶμ��ַ���
KYString TRCFields::DataSet() const
{
   // ��ʼ��
   KYString result;
   long     intSize;
   char*    pRet;

   // ȡ�ֶ�ֵ
   pRet  = (char*)RCKFieldsOutput(FFields, &intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // ���ؽ��
   return result;
}

// ȡ�ֶε��ַ���ֵ
KYString TRCFields::AsStr(long AIndex) const
{
   // ��ʼ��
   KYString result;
   long     intSize;
   char*    pRet;

   // ȡ�ֶ�ֵ
   pRet  = RCKFieldAsStr(FFields, AIndex, &intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // ���ؽ��
   return result;
}

// ȡ�ֶε��ַ���ֵ
KYString TRCFields::AsStr(const char* AName) const
{
   // ��ʼ��
   KYString result;
   long     intSize;
   char*    pRet;

   // ȡ�ֶ�ֵ
   pRet  = RCKFieldAsStr_(FFields, AName, &intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // ���ؽ��
   return result;
}

/* end namespace */

   }
}
