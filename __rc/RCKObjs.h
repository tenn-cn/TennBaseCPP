// =======================================
// Unit   : RCKernel objects
// Version: 5.0.0.0 (build 2014.07.07)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _RCKObjs_H_
#define _RCKObjs_H_

#include "KYLib.h"
#include "RCKernel.h"

// ʹ�� RCKernel �������ռ�
namespace RCKernel
{
   namespace __object__
   {

/* begin namespace */

class TRCAppObj;
class TRCFuncObj;
class TRCConnObj;
class TRCCmdObj;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ��ʼ��/�ͷŵ�Ԫ */

// ��ʼ��/�ͷ� RCKObjs ��Ԫ(ע: �ڲ������ü���)
void        InitRCKObjs();
void        FreeRCKObjs(bool AForced = false);

// ID -> ����
TRCCmdObj*  CmdID2Object(void* ACmdID);   // ����ID -> ����
TRCFuncObj* FuncID2Object(void* AFuncID); // ����ID -> ����

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* Ӧ�ö���(TRCAppObj)����غ��� */

// ����/�ͷ�Ӧ�ö���
TRCAppObj*  CreateAppObj(const char* AName, const char* APassword, long* ARetCode = NULL);
void        FreeAppObj(TRCAppObj* AppObj);

// Ӧ�ö������ü����� 1
TRCAppObj*  IncRefAppObj(void* AppID);
bool        IncRefAppObj_(TRCAppObj* AppObj);

// Ӧ�ö������ü�����/�� 1(ע: AppObj ����������)
void        IncRefAppObj(TRCAppObj* AppObj);
void        DecRefAppObj(TRCAppObj* AppObj);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ���Ӷ���(TRCConnObj)����غ��� */

// ����/�ͷ����Ӷ���(ע: AConnID �������)
TRCConnObj* CreateConnObj(void* AConnID, bool ANeedFree, long* ARetCode = NULL);
void        FreeConnObj(TRCConnObj* AConnObj);

// ���Ӷ������ü����� 1
TRCConnObj* IncRefConnObj(void* AConnID);
bool        IncRefConnObj_(TRCConnObj* AConnObj);

// ���Ӷ������ü�����/�� 1(ע: AConnObj ����������)
void        IncRefConnObj(TRCConnObj* AConnObj);
void        DecRefConnObj(TRCConnObj* AConnObj);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �������(TRCCmdObj)����غ��� */

// ����/�ͷ��������(ע: AConnObj �������)
TRCCmdObj*  CreateCmdObj(void* AConnID = NULL);
TRCCmdObj*  CreateCmdObj(TRCConnObj* AConnObj);
void        FreeCmdObj(TRCCmdObj* ACmdObj);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TRCAppObj - RCK Ӧ���� */

class TRCAppObj
{
private:
   // ӳ���б�����
   typedef TKYMapObjKey<void>    TObjList;

public:
   // ���ӵ�¼�¼�
   typedef void (TObject::*TDoConnLogin)(TRCAppObj* AppObj, void* AConnID, bool& AIsRefused);
   typedef struct
   {
      TDoConnLogin   Method;
      void*          Object;
   } TOnConnLogin;

   // ���ӵǳ��¼�
   typedef void (TObject::*TDoConnLogout)(TRCAppObj* AppObj, void* AConnID);
   typedef struct
   {
      TDoConnLogout  Method;
      void*          Object;
   } TOnConnLogout;

   // Ӧ���¼�
   typedef void (TObject::*TDoRespEvent)(TRCAppObj* AppObj, void* AFuncID, void* AConnID);
   typedef struct
   {
      TDoRespEvent   Method;
      void*          Object;
   } TOnRespEvent;

   // �����ͷ��¼�
   typedef void (TObject::*TDoFuncFree)(TRCAppObj* AppObj, TRCFuncObj* AFuncObj);
   typedef struct
   {
      TDoFuncFree    Method;
      void*          Object;
   } TOnFuncFree;

public:
   TRCAppObj();
   virtual ~TRCAppObj();

   // ����
   void*          Data() const         { return FData; }
   KYString       Name() const         { return FName; }
   void*          AppID() const        { return FAppID; }
   long           State() const        { return RCKAppGetInt(FAppID, kaaState, NULL); }
   long           FuncCount() const    { return RCKAppGetInt(FAppID, kaaFuncCount, NULL); }
   long           ConnCount() const    { return RCKAppGetInt(FAppID, kaaConnCount, NULL); }
   long           MaxThreads() const   { return RCKAppGetInt(FAppID, kaaMaxThreads, NULL); }
   long           MaxConnCount() const { return RCKAppGetInt(FAppID, kaaMaxConnCount, NULL); }

   // ��������
   void           SetData(void* AData) { FData = AData; }
   long           SetName(const KYString& AName);
   long           SetPassword(const char* APassword, long ALength)
                  { return RCKAppSetStr(FAppID, kaaPassword, APassword, ALength); }
   long           SetPassword(const KYString& APassword)
                  { return SetPassword((char*)APassword, APassword.Length()); }
   long           SetMaxThreads(long ACount)
                  { return RCKAppSetInt(FAppID, kaaMaxThreads, ACount); }
   long           SetMaxConnCount(long ACount)
                  { return RCKAppSetInt(FAppID, kaaMaxConnCount, ACount); }

   // ��ȡӦ�õ�����/����
   void*          ConnID(long AIndex) const
                  { return RCKAppGetObj(FAppID, kaaBaseConnObj + AIndex, NULL); }
   void*          FuncID(long AIndex) const
                  { return RCKAppGetObj(FAppID, kaaBaseFuncObj + AIndex, NULL); }
   void*          FuncID(const char* AName) const
                  { return RCKAppFuncObj(FAppID, AName); }
   char*          FuncName(long AIndex) const
                  { return RCKAppGetStr(FAppID, kaaBaseFuncName + AIndex, NULL); }

   // ��ʼ/������������
   long           BeginDefs()          { return RCKAppFuncBegin(FAppID); }
   long           EndDefs()            { return RCKAppFuncEnd(FAppID); }

   // �½�/ɾ��/�����������
   TRCFuncObj*    NewFuncObj(const char* AName, long ARetType,  long  ARetSize,
                                                long ACallMode, void* ACallback);
   bool           DeleteFuncObj(TRCFuncObj* AFuncObj);
   void           ClearFuncObjs();

   // ��/�ر�Ӧ��/ǿ��ע��Ӧ�õ�����
   long           Open()               { return RCKAppOpen(FAppID); }
   long           Close()              { return RCKAppClose(FAppID); }
   long           Logout(void* AConnID){ return RCKAppLogout(FAppID, AConnID); }

   // �����¼�
   long           SetOnConnLogin(TDoConnLogin AMethod, void* AObject);
   long           SetOnConnLogout(TDoConnLogout AMethod, void* AObject);
   long           SetOnExecResp(TDoRespEvent AMethod, void* AObject);
   long           SetOnRespCancel(TDoRespEvent AMethod, void* AObject);
   long           SetOnRespConfirm(TDoRespEvent AMethod, void* AObject);

   // �¼�
   TOnFuncFree    OnFuncFree;

protected:
   // ��ǰ��
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

   // ���������/�� 1
   long           IncObjTimes()        { return InterlockedIncrement(&FObjTimes); }
   long           DecObjTimes()        { return InterlockedDecrement(&FObjTimes); }

private:
   // ִ�г�ʼ���¼�/��������б�
   void           DoInitEvent();
   void           DoClearFuncObjs();

   // ִ���¼�����
   void           DoConnLogin(void* AConnID, bool& AIsRefused);
   void           DoConnLogout(void* AConnID);
   void           DoExecResp(void* AFuncID, void* AConnID);
   void           DoRespCancel(void* AFuncID, void* AConnID);
   void           DoRespConfirm(void* AFuncID, void* AConnID);

   // �¼�����
   void           DoIDDeletion(void* AFuncID);
   void           DoObjDeletion(TRCFuncObj* AFuncObj);

private:
   void*          FData;               // �Զ�������
   TKYCritSect*   FLock;               // ��ǰ��
   KYString       FName;               // Ӧ����
   void*          FAppID;              // Ӧ�� ID
   long           FObjTimes;           // �������ô���
   TObjList*      FFuncList;           // �����б�

   // �¼�
   TOnConnLogin   FOnConnLogin;        // ���ӵ�¼
   TOnConnLogout  FOnConnLogout;       // ���ӵǳ�
   TOnRespEvent   FOnExecResp;         // ִ��Ӧ��
   TOnRespEvent   FOnRespCancel;       // Ӧ����ȡ��
   TOnRespEvent   FOnRespConfirm;      // Ӧ����ȷ��

private:
   // Ӧ���¼��Ļص�����
   static void __stdcall _RCKOnConnLogin(void* AppID, void* AConnID, bool& AIsRefused);
   static void __stdcall _RCKOnConnLogout(void* AppID, void* AConnID);
   static void __stdcall _RCKOnExecResp(void* AppID, void* AFuncID, void* AConnID);
   static void __stdcall _RCKOnRespCancel(void* AppID, void* AFuncID, void* AConnID);
   static void __stdcall _RCKOnRespConfirm(void* AppID, void* AFuncID, void* AConnID);

private:
   friend class TRCKObjs;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TRCFuncObj - RCK ������ */

class TRCFuncObj
{
public:
   TRCFuncObj();
   virtual ~TRCFuncObj();

   // ����
   void*          Data() const         { return FData; }
   KYString       Name() const         { return FName; }
   void*          FuncID() const       { return FFuncID; }
   TRCAppObj*     AppObj() const       { return FAppObj; }
   bool           Active() const       { return RCKFuncGetInt(FFuncID, kfaActive, NULL) == 1; }

   long           RetType() const      { return RCKFuncGetInt(FFuncID, kfaRetType, NULL); }
   long           RetSize() const      { return RCKFuncGetInt(FFuncID, kfaRetSize, NULL); }
   long           CallMode() const     { return RCKFuncGetInt(FFuncID, kfaCallMode, NULL); }
   void*          Callback() const     { return RCKFuncGetObj(FFuncID, kfaCallback, NULL); }
   bool           NeedConfirm() const  { return RCKFuncGetInt(FFuncID, kfaNeedConfirm, NULL) == 1; }
   bool           NeedEncrypt() const  { return RCKFuncGetInt(FFuncID, kfaNeedEncrypt, NULL) == 1; }

   // ��������
   void           SetData(void* AData) { FData = AData; }
   long           SetNeedConfirm(bool ANeed)
                  { return RCKFuncSetInt(FFuncID, kfaNeedConfirm, ANeed); }
   long           SetNeedEncrypt(bool ANeed)
                  { return RCKFuncSetInt(FFuncID, kfaNeedEncrypt, ANeed); }

   // ��ȡ��������
   long           ParamCount() const   { return RCKFuncGetInt(FFuncID, kfaParamCount, NULL); }
   long           ParamIndex(const char* AName) const
                  { return RCKFuncParamIndex(FFuncID, AName); }
   long           ParamIndex(const KYString& AName) const
                  { return RCKFuncParamIndex(FFuncID, (char*)AName); }
   KYString       ParamName(long AIndex) const
                  { return RCKFuncGetStr(FFuncID, kfaBaseParamName + AIndex, NULL); }
   long           ParamType(long AIndex) const
                  { return RCKFuncGetInt(FFuncID, kfaBaseParamType + AIndex, NULL); }
   long           ParamSize(long AIndex) const
                  { return RCKFuncGetInt(FFuncID, kfaBaseParamSize + AIndex, NULL); }
   bool           ParamIsRef(long AIndex) const
                  { return RCKFuncGetInt(FFuncID, kfaBaseParamIsRef + AIndex, NULL) == 1; }

   // ��ȡ�����ֶζ���
   long           FieldCount() const   { return RCKFuncGetInt(FFuncID, kfaFieldCount, NULL); }
   long           FieldIndex(const char* AName) const
                  { return RCKFuncFieldIndex(FFuncID, AName); }
   long           FieldIndex(const KYString& AName) const
                  { return RCKFuncFieldIndex(FFuncID, (char*)AName); }
   KYString       FieldName(long AIndex) const
                  { return RCKFuncGetStr(FFuncID, kfaBaseFieldName + AIndex, NULL); }
   long           FieldType(long AIndex) const
                  { return RCKFuncGetInt(FFuncID, kfaBaseFieldType + AIndex, NULL); }
   long           FieldSize(long AIndex) const
                  { return RCKFuncGetInt(FFuncID, kfaBaseFieldSize + AIndex, NULL); }

   // ��Ӳ���
   long           AddParam(const char* AName, long AType, long ASize = 0, bool AIsRef = false)
                  { return RCKFuncAddParam(FFuncID, AName, AType, ASize, AIsRef); }
   long           AddParam(const KYString& AName, long AType, long ASize = 0, bool AIsRef = false)
                  { return RCKFuncAddParam(FFuncID, (char*)AName, AType, ASize, AIsRef); }

   // ��ӷ����ֶ�
   long           AddField(const char* AName, long AType, long ASize = 0)
                  { return RCKFuncAddField(FFuncID, AName, AType, ASize); }
   long           AddField(const KYString& AName, long AType, long ASize = 0)
                  { return RCKFuncAddField(FFuncID, (char*)AName, AType, ASize); }

private:
   void*          FData;               // �Զ�������
   KYString       FName;               // ������
   void*          FFuncID;             // ���� ID
   TRCAppObj*     FAppObj;             // Ӧ�ö���

private:
   friend class TRCKObjs;
   friend class TRCAppObj;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TRCConnObj - RCK ������ */

class TRCConnObj
{
public:
   // ����֪ͨ�¼�
   typedef void (TObject::*TDoNotify)(TRCConnObj* AConnObj);
   typedef struct
   {
      TDoNotify      Method;
      void*          Object;
   } TOnNotify;

   // ����������¼�
   typedef void (TObject::*TDoCmdReturn)(TRCConnObj* AConnObj, void* ACmdID, long AResult);
   typedef struct
   {
      TDoCmdReturn   Method;
      void*          Object;
   } TOnCmdReturn;

   // �����Զ��������¼�
   typedef void (TObject::*TDoRecvData)(TRCConnObj* AConnObj, const void* AData, long ASize);
   typedef struct
   {
      TDoRecvData    Method;
      void*          Object;
   } TOnRecvData;

public:
   TRCConnObj(void* AConnID = NULL, bool ANeedFree = false);
   virtual ~TRCConnObj();

   // ����
   void*          Data() const         { return FData; }
   void*          ConnID() const       { return FConnID; }
   bool           CanEvent() const     { return FCanEvent; }
   bool           NeedFree() const     { return FNeedFree; }

   KYString       Error() const        { return RCKConnGetStr(FConnID, kcaError, NULL); }
   long           Type() const         { return RCKConnGetInt(FConnID, kcaType, NULL); }
   long           State() const        { return RCKConnGetInt(FConnID, kcaState, NULL); }
   long           Timeout() const      { return RCKConnGetInt(FConnID, kcaTimeout, NULL); }
   void*          SlotObj() const      { return RCKConnGetObj(FConnID, kcaSlotObj, NULL); }
   bool           CanLogin() const     { return RCKConnGetInt(FConnID, kcaCanLogin, NULL) == 1; }
   bool           CanBindApp() const   { return RCKConnGetInt(FConnID, kcaCanBindApp, NULL) == 1; }
   bool           IsSyncLink() const   { return RCKConnGetInt(FConnID, kcaIsSyncLink, NULL) == 1; }
   void*          BindAppObj() const   { return RCKConnGetObj(FConnID, kcaBindAppObj, NULL); }
   long           BindTimeout() const  { return RCKConnGetInt(FConnID, kcaBindTimeout, NULL); }
   long           SlotTimeout() const  { return RCKConnGetInt(FConnID, kcaSlotTimeout, NULL); }
   long           SlotMaxSize() const  { return RCKConnGetInt(FConnID, kcaSlotMaxSize, NULL); }
   long           SendQueueSize() const{ return RCKConnGetInt(FConnID, kcaSendQueueSize, NULL); }

   KYString       AppName() const      { return RCKConnGetStr(FConnID, kcaAppName, NULL); }
   KYString       CurrAddr() const     { return RCKConnGetStr(FConnID, kcaCurrAddr, NULL); }
   long           CurrPort() const     { return RCKConnGetInt(FConnID, kcaCurrPort, NULL); }
   KYString       PeerAddr() const     { return RCKConnGetStr(FConnID, kcaPeerAddr, NULL); }
   long           PeerPort() const     { return RCKConnGetInt(FConnID, kcaPeerPort, NULL); }
   long           LastTick() const     { return RCKConnGetInt(FConnID, kcaLastTick, NULL); }

   long           KeepTimeout() const  { return RCKConnGetInt(FConnID, kcaKeepTimeout, NULL); }
   long           KeepInterval() const { return RCKConnGetInt(FConnID, kcaKeepInterval, NULL); }
   long           KeepRetryTimes()const{ return RCKConnGetInt(FConnID, kcaKeepRetryTimes, NULL); }

   long           FuncCount() const    { return RCKConnGetInt(FConnID, kcaFuncCount, NULL); }
   KYString       FuncName(long AIndex) const
                  { return RCKConnGetStr(FConnID, kcaBaseFuncName + AIndex, NULL); }

   // ��������
   void           SetData(void* AData) { FData = AData; }
   long           SetTimeout(long ATimeout)
                  { return RCKConnSetInt(FConnID, kcaTimeout, ATimeout); }
   long           SetCanLogin(bool ACanLogin)
                  { return RCKConnSetInt(FConnID, kcaCanLogin, ACanLogin); }
   long           SetCanBindApp(bool ACanBindApp)
                  { return RCKConnSetInt(FConnID, kcaCanBindApp, ACanBindApp); }
   long           SetBindTimeout(long ATimeout)
                  { return RCKConnSetInt(FConnID, kcaBindTimeout, ATimeout); }
   long           SetIsSyncLink(bool AIsSyncLink)
                  { return RCKConnSetInt(FConnID, kcaIsSyncLink, AIsSyncLink); }
   long           SetSlotTimeout(long ATimeout)
                  { return RCKConnSetInt(FConnID, kcaSlotTimeout, ATimeout); }
   long           SetSlotMaxSize(long AMaxSize)
                  { return RCKConnSetInt(FConnID, kcaSlotMaxSize, AMaxSize); }
   long           SetSendQueueSize(long AQueueSize)
                  { return RCKConnSetInt(FConnID, kcaSendQueueSize, AQueueSize); }

   long           SetAppName(const char* AName, long ALength)
                  { return RCKConnSetStr(FConnID, kcaAppName, AName, ALength); }
   long           SetAppName(const KYString& AName)
                  { return SetAppName((char*)AName, AName.Length()); }

   long           SetAppPass(const char* APassword, long ALength)
                  { return RCKConnSetStr(FConnID, kcaAppPass, APassword, ALength); }
   long           SetAppPass(const KYString& APassword)
                  { return SetAppPass((char*)APassword, APassword.Length()); }

   long           SetCurrAddr(const char* Addr, long ALength)
                  { return RCKConnSetStr(FConnID, kcaCurrAddr, Addr, ALength); }
   long           SetCurrAddr(const KYString& Addr)
                  { return SetCurrAddr((char*)Addr, Addr.Length()); }
   long           SetCurrPort(long APort)
                  { return RCKConnSetInt(FConnID, kcaCurrPort, APort); }

   long           SetPeerAddr(const char* Addr, long ALength)
                  { return RCKConnSetStr(FConnID, kcaPeerAddr, Addr, ALength); }
   long           SetPeerAddr(const KYString& Addr)
                  { return SetPeerAddr((char*)Addr, Addr.Length()); }
   long           SetPeerPort(long APort)
                  { return RCKConnSetInt(FConnID, kcaPeerPort, APort); }

   long           SetKeepTimeout(long ATimeout)
                  { return RCKConnSetInt(FConnID, kcaKeepTimeout, ATimeout); }
   long           SetKeepInterval(long AInterval)
                  { return RCKConnSetInt(FConnID, kcaKeepInterval, AInterval); }
   long           SetKeepRetryTimes(long ARetryTimes)
                  { return RCKConnSetInt(FConnID, kcaKeepRetryTimes, ARetryTimes); }

   // slot ��չ����
   bool           SlotExtBool(long AExt) const
                  { return RCKConnGetInt(FConnID, kcaBaseSlotExt + AExt, NULL) == 1; }
   long           SlotExtInt(long AExt) const
                  { return RCKConnGetInt(FConnID, kcaBaseSlotExt + AExt, NULL); }
   void*          SlotExtObj(long AExt) const
                  { return RCKConnGetObj(FConnID, kcaBaseSlotExt + AExt, NULL); }
   char*          SlotExtStr(long AExt, long* ARetSize = NULL) const
                  { return RCKConnGetStr(FConnID, kcaBaseSlotExt + AExt, ARetSize); }

   // ���� slot ��չ����
   long           SetSlotExtInt(long AExt, long AValue)
                  { return RCKConnSetInt(FConnID, kcaBaseSlotExt + AExt, AValue); }
   long           SetSlotExtObj(long AExt, void* AValue)
                  { return RCKConnSetObj(FConnID, kcaBaseSlotExt + AExt, AValue); }
   long           SetSlotExtStr(long AExt, const char* AValue, long ASize)
                  { return RCKConnSetStr(FConnID, kcaBaseSlotExt + AExt, AValue, ASize); }

   // ��/�ر�����
   long           Open()               { return RCKConnOpen(FConnID); }
   long           Close()              { return RCKConnClose(FConnID); }

   // ��¼/�ǳ��Զ�Ӧ��
   long           Login()              { return RCKConnLogin(FConnID); }
   long           Logout()             { return RCKConnLogout(FConnID); }

   // ���ضԶ�Ӧ�õĺ����б�
   long           LoadDefs()           { return RCKConnLoad(FConnID); }

   // ��������(���� ANeedPack ��ʾ�����Ƿ���Ҫѹ������)
   long           SendData(const void* AData, long ASize, bool ANeedPack)
                  { return RCKConnSendData(FConnID, AData, ASize, ANeedPack); }

   // �����¼�
   long           SetOnConnect(TDoNotify AMethod, void* AObject);
   long           SetOnDisconnect(TDoNotify AMethod, void* AObject);
   long           SetOnLogin(TDoNotify AMethod, void* AObject);
   long           SetOnLogout(TDoNotify AMethod, void* AObject);
   long           SetOnCmdReturn(TDoCmdReturn AMethod, void* AObject);
   long           SetOnRecvData(TDoRecvData AMethod, void* AObject);

protected:
   // ���������/�� 1
   long           IncObjTimes()        { return InterlockedIncrement(&FObjTimes); }
   long           DecObjTimes()        { return InterlockedDecrement(&FObjTimes); }

private:
   // ִ�г�ʼ���¼�
   void           DoInitEvent();

   // ִ���¼�����
   void           DoConnect();
   void           DoDisconnect();
   void           DoLogin();
   void           DoLogout();
   void           DoCmdReturn(void* ACmdID, long AResult);
   void           DoRecvData(const void* AData, long ASize);

private:
   void*          FData;               // �Զ�������
   void*          FConnID;             // ���� ID
   long           FObjTimes;           // �������ô���
   bool           FCanEvent;           // �Ƿ��ܹ��¼�(ע: CreateConnObj �����ſ���)
   bool           FNeedFree;           // �Ƿ���Ҫ�ͷ�

   // �¼�
   TOnNotify      FOnConnect;          // ���ӳɹ�
   TOnNotify      FOnDisconnect;       // �Ͽ�����
   TOnNotify      FOnLogin;            // ��¼�Զ�Ӧ��
   TOnNotify      FOnLogout;           // �ǳ��Զ�Ӧ��
   TOnCmdReturn   FOnCmdReturn;        // �����
   TOnRecvData    FOnRecvData;         // ��������

private:
   // �����¼��Ļص�����
   static void __stdcall _RCKOnConnect(void* AConnID);
   static void __stdcall _RCKOnDisconnect(void* AConnID);
   static void __stdcall _RCKOnLogin(void* AConnID);
   static void __stdcall _RCKOnLogout(void* AConnID);
   static void __stdcall _RCKOnCmdReturn(void* AConnID, void* ACmdID, long AResult);
   static void __stdcall _RCKOnRecvData(void* AConnID, const void* AData, long ASize);

private:
   friend class TRCKObjs;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TRCCmdObj - RCK ������ */

class TRCCmdObj
{
public:
   TRCCmdObj();
   virtual ~TRCCmdObj();

   // ����
   void*          Data() const         { return FData; }
   KYString       Name() const         { return RCKCmdGetStr(FCmdID, kmaName, NULL); }
   long           State() const        { return RCKCmdGetInt(FCmdID, kmaState, NULL); }
   void*          CmdID() const        { return FCmdID; }
   void*          ConnID() const       { return RCKCmdGetObj(FCmdID, kmaConnObj, NULL); }
   TRCConnObj*    ConnObj() const;
   long           RetType() const      { return RCKCmdGetInt(FCmdID, kmaRetType, NULL); }
   long           RetSize() const      { return RCKCmdGetInt(FCmdID, kmaRetSize, NULL); }
   bool           Encrypted() const    { return RCKCmdGetInt(FCmdID, kmaEncrypted, NULL) == 1; }
   bool           AppIsValid() const   { return RCKCmdGetInt(FCmdID, kmaAppIsValid, NULL) == 1; }
   long           ExecResult() const   { return RCKCmdGetInt(FCmdID, kmaExecResult, NULL); }

   // ��ȡ��������
   long           ParamCount() const   { return RCKCmdGetInt(FCmdID, kmaParamCount, NULL); }
   long           ParamIndex(const char* AName) const
                  { return RCKCmdParamIndex(FCmdID, AName); }
   long           ParamIndex(const KYString& AName) const
                  { return RCKCmdParamIndex(FCmdID, (char*)AName); }
   KYString       ParamName(long AIndex) const
                  { return RCKCmdGetStr(FCmdID, kmaBaseParamName + AIndex, NULL); }
   long           ParamType(long AIndex) const
                  { return RCKCmdGetInt(FCmdID, kmaBaseParamType + AIndex, NULL); }
   long           ParamSize(long AIndex) const
                  { return RCKCmdGetInt(FCmdID, kmaBaseParamSize + AIndex, NULL); }

   // ��ȡ�����ֶζ���
   long           FieldCount() const   { return RCKCmdGetInt(FCmdID, kmaFieldCount, NULL); }
   long           FieldIndex(const char* AName) const
                  { return RCKCmdFieldIndex(FCmdID, AName); }
   long           FieldIndex(const KYString& AName) const
                  { return RCKCmdFieldIndex(FCmdID, (char*)AName); }
   KYString       FieldName(long AIndex) const
                  { return RCKCmdGetStr(FCmdID, kmaBaseFieldName + AIndex, NULL); }
   long           FieldType(long AIndex) const
                  { return RCKCmdGetInt(FCmdID, kmaBaseFieldType + AIndex, NULL); }
   long           FieldSize(long AIndex) const
                  { return RCKCmdGetInt(FCmdID, kmaBaseFieldSize + AIndex, NULL); }
   bool           FieldIsNull(long AIndex) const
                  { return RCKCmdFieldIsNull(FCmdID, AIndex, NULL); }
   bool           FieldIsNull(const char* AName) const
                  { return RCKCmdFieldIsNull_(FCmdID, AName, NULL); }
   bool           FieldIsNull(const KYString& AName) const
                  { return RCKCmdFieldIsNull_(FCmdID, (char*)AName, NULL); }

   // ��������
   void           SetData(void* AData) { FData = AData; }
   long           SetConnID(void* AConnID) const
                  { return RCKCmdSetObj(FCmdID, kmaConnObj, AConnID); }
   long           SetConnObj(TRCConnObj* AConnObj) const
                  { return RCKCmdSetObj(FCmdID, kmaConnObj, (AConnObj != NULL)
                                                   ? AConnObj->ConnID() : NULL); }

   // ��ʼ����
   long           Begin(const char* AName) const
                  { return RCKCmdBegin(FCmdID, AName); }
   long           Begin(const KYString& AName) const
                  { return RCKCmdBegin(FCmdID, (char*)AName); }

   // ��������/���¿�ʼ����
   long           End() const          { return RCKCmdEnd(FCmdID); }
   long           Restart() const      { return RCKCmdRestart(FCmdID); }

   // ִ������
   long           Execute(Longword ATimeout) const
                  { return RCKCmdExecute(FCmdID, ATimeout); }
   long           ExecByParams(Longword ATimeout, ...) const;

   // �����ǰ�������ֵ
   long           ParamClear() const   { return RCKCmdParamClear(FCmdID); }

   // ���ø����Ͳ���ֵ����ط���
   long           ParamByStr(long AIndex, const char* AValue, long ASize = -1) const
                  { return RCKCmdParamByStr(FCmdID, AIndex, AValue, ASize); }
   long           ParamByStr(long AIndex, const KYString& AValue) const
                  { return RCKCmdParamByStr(FCmdID, AIndex, (char*)AValue, AValue.Length()); }
   long           ParamByStr(const char* AName, const KYString& AValue) const
                  { return RCKCmdParamByStr_(FCmdID, AName, (char*)AValue, AValue.Length()); }
   long           ParamByStr(const KYString& AName, const KYString& AValue) const
                  { return RCKCmdParamByStr_(FCmdID, (char*)AName,
                                             (char*)AValue, AValue.Length()); }

   long           ParamByInt(long AIndex, long AValue) const
                  { return RCKCmdParamByInt(FCmdID, AIndex, AValue); }
   long           ParamByInt(const char* AName, long AValue) const
                  { return RCKCmdParamByInt_(FCmdID, AName, AValue); }
   long           ParamByInt(const KYString& AName, long AValue) const
                  { return RCKCmdParamByInt_(FCmdID, (char*)AName, AValue); }

   long           ParamByBool(long AIndex, bool AValue) const
                  { return RCKCmdParamByBool(FCmdID, AIndex, AValue); }
   long           ParamByBool(const char* AName, bool AValue) const
                  { return RCKCmdParamByBool_(FCmdID, AName, AValue); }
   long           ParamByBool(const KYString& AName, bool AValue) const
                  { return RCKCmdParamByBool_(FCmdID, (char*)AName, AValue); }

   long           ParamByFloat(long AIndex, double AValue) const
                  { return RCKCmdParamByFloat(FCmdID, AIndex, AValue); }
   long           ParamByFloat(const char* AName, double AValue) const
                  { return RCKCmdParamByFloat_(FCmdID, AName, AValue); }
   long           ParamByFloat(const KYString& AName, double AValue) const
                  { return RCKCmdParamByFloat_(FCmdID, (char*)AName, AValue); }

   long           ParamByStruct(long AIndex, void* AValue) const
                  { return RCKCmdParamByStruct(FCmdID, AIndex, AValue); }
   long           ParamByStruct(const char* AName, void* AValue) const
                  { return RCKCmdParamByStruct_(FCmdID, AName, AValue); }
   long           ParamByStruct(const KYString& AName, void* AValue) const
                  { return RCKCmdParamByStruct_(FCmdID, (char*)AName, AValue); }

   long           ParamByInt64(long AIndex, __int64 AValue) const
                  { return RCKCmdParamByInt64(FCmdID, AIndex, AValue); }
   long           ParamByInt64(const char* AName, __int64 AValue) const
                  { return RCKCmdParamByInt64_(FCmdID, AName, AValue); }
   long           ParamByInt64(const KYString& AName, __int64 AValue) const
                  { return RCKCmdParamByInt64_(FCmdID, (char*)AName, AValue); }

   long           ParamByDate(long AIndex, TDateTime AValue) const
                  { return RCKCmdParamByDate(FCmdID, AIndex, AValue); }
   long           ParamByDate(const char* AName, TDateTime AValue) const
                  { return RCKCmdParamByDate_(FCmdID, AName, AValue); }
   long           ParamByDate(const KYString& AName, TDateTime AValue) const
                  { return RCKCmdParamByDate_(FCmdID, (char*)AName, AValue); }

   long           ParamByCustom(long AIndex, const void* AValue, long ASize) const
                  { return RCKCmdParamByCustom(FCmdID, AIndex, AValue, ASize); }
   long           ParamByCustom(const char* AName, const void* AValue, long ASize) const
                  { return RCKCmdParamByCustom_(FCmdID, AName, AValue, ASize); }
   long           ParamByCustom(const KYString& AName, const void* AValue, long ASize) const
                  { return RCKCmdParamByCustom_(FCmdID, (char*)AName, AValue, ASize); }

   // ȡ�����ͷ���ֵ����ط���
   KYString       RetAsStr() const;
   char*          RetAsStr(long& ASize) const
                  { return RCKCmdRetStr(FCmdID, &ASize); }
   long           RetAsInt() const     { return RCKCmdRetInt(FCmdID, NULL); }
   bool           RetAsBool() const    { return RCKCmdRetBool(FCmdID, NULL); }
   double         RetAsFloat() const   { return RCKCmdRetFloat(FCmdID, NULL); }
   void*          RetAsStruct() const  { return RCKCmdRetStruct(FCmdID, NULL); }
   __int64        RetAsInt64() const   { return RCKCmdRetInt64(FCmdID, NULL); }
   TDateTime      RetAsDate() const    { return RCKCmdRetDate(FCmdID, NULL); }
   long           RetAsCustom(void* AValue, long ASize) const
                  { return RCKCmdRetCustom(FCmdID, AValue, ASize); }

   // ȡ�����ͷ����ֶ�ֵ����ط���
   KYString       FieldAsStr(long AIndex) const;
   KYString       FieldAsStr(const char* AName) const;
   KYString       FieldAsStr(const KYString& AName) const
                  { return FieldAsStr((char*)AName); }
   char*          FieldAsStr(long AIndex, long& ASize) const
                  { return RCKCmdFieldStr(FCmdID, AIndex, &ASize); }
   char*          FieldAsStr(const char* AName, long& ASize) const
                  { return RCKCmdFieldStr_(FCmdID, AName, &ASize); }

   long           FieldAsInt(long AIndex) const
                  { return RCKCmdFieldInt(FCmdID, AIndex, NULL); }
   long           FieldAsInt(const char* AName) const
                  { return RCKCmdFieldInt_(FCmdID, AName, NULL); }
   long           FieldAsInt(const KYString& AName) const
                  { return RCKCmdFieldInt_(FCmdID, (char*)AName, NULL); }

   bool           FieldAsBool(long AIndex) const
                  { return RCKCmdFieldBool(FCmdID, AIndex, NULL); }
   bool           FieldAsBool(const char* AName) const
                  { return RCKCmdFieldBool_(FCmdID, AName, NULL); }
   bool           FieldAsBool(const KYString& AName) const
                  { return RCKCmdFieldBool_(FCmdID, (char*)AName, NULL); }

   double         FieldAsFloat(long AIndex) const
                  { return RCKCmdFieldFloat(FCmdID, AIndex, NULL); }
   double         FieldAsFloat(const char* AName) const
                  { return RCKCmdFieldFloat_(FCmdID, AName, NULL); }
   double         FieldAsFloat(const KYString& AName) const
                  { return RCKCmdFieldFloat_(FCmdID, (char*)AName, NULL); }

   void*          FieldAsStruct(long AIndex) const
                  { return RCKCmdFieldStruct(FCmdID, AIndex, NULL); }
   void*          FieldAsStruct(const char* AName) const
                  { return RCKCmdFieldStruct_(FCmdID, AName, NULL); }
   void*          FieldAsStruct(const KYString& AName) const
                  { return RCKCmdFieldStruct_(FCmdID, (char*)AName, NULL); }

   __int64        FieldAsInt64(long AIndex) const
                  { return RCKCmdFieldInt64(FCmdID, AIndex, NULL); }
   __int64        FieldAsInt64(const char* AName) const
                  { return RCKCmdFieldInt64_(FCmdID, AName, NULL); }
   __int64        FieldAsInt64(const KYString& AName) const
                  { return RCKCmdFieldInt64_(FCmdID, (char*)AName, NULL); }

   TDateTime      FieldAsDate(long AIndex) const
                  { return RCKCmdFieldDate(FCmdID, AIndex, NULL); }
   TDateTime      FieldAsDate(const char* AName) const
                  { return RCKCmdFieldDate_(FCmdID, AName, NULL); }
   TDateTime      FieldAsDate(const KYString& AName) const
                  { return RCKCmdFieldDate_(FCmdID, (char*)AName, NULL); }

   long           FieldAsCustom(long AIndex, void* AValue, long ASize) const
                  { return RCKCmdFieldCustom(FCmdID, AIndex, AValue, ASize); }
   long           FieldAsCustom(const char* AName, void* AValue, long ASize) const
                  { return RCKCmdFieldCustom_(FCmdID, AName, AValue, ASize); }
   long           FieldAsCustom(const KYString& AName, void* AValue, long ASize) const
                  { return RCKCmdFieldCustom_(FCmdID, (char*)AName, AValue, ASize); }

private:
   void*          FData;               // �Զ�������
   void*          FCmdID;              // ���� ID
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TRCResp - RCK Ӧ���� */

class TRCResp
{
public:
   // ����
   static long       State()              { return RCKRespState(); }
   static void*      FuncID()             { return RCKRespFuncObj(); }
   static void*      ConnID()             { return RCKRespConnObj(); }

   // ȡ�����ͷ���ֵ����ط���
   static KYString   RetAsStr();
   static char*      RetAsStr(long& ASize){ return RCKRespRetStr(&ASize); }
   static long       RetAsInt()           { return RCKRespRetInt(NULL); }
   static bool       RetAsBool()          { return RCKRespRetBool(NULL); }
   static double     RetAsFloat()         { return RCKRespRetFloat(NULL); }
   static void*      RetAsStruct()        { return RCKRespRetStruct(NULL); }
   static __int64    RetAsInt64()         { return RCKRespRetInt64(NULL); }
   static TDateTime  RetAsDate()          { return RCKRespRetDate(NULL); }
   static long       RetAsCustom(void* AValue, long ASize)
                     { return RCKRespRetCustom(AValue, ASize); }

   // ȡ�����ͷ����ֶ�ֵ����ط���
   static KYString   FieldAsStr(long AIndex);
   static KYString   FieldAsStr(const char* AName);
   static KYString   FieldAsStr(const KYString& AName)
                     { return FieldAsStr((char*)AName); }
   static char*      FieldAsStr(long AIndex, long& ASize)
                     { return RCKRespFieldStr(AIndex, &ASize); }
   static char*      FieldAsStr(const char* AName, long& ASize)
                     { return RCKRespFieldStr_(AName, &ASize); }

   static long       FieldAsInt(long AIndex)
                     { return RCKRespFieldInt(AIndex, NULL); }
   static long       FieldAsInt(const char* AName)
                     { return RCKRespFieldInt_(AName, NULL); }
   static long       FieldAsInt(const KYString& AName)
                     { return RCKRespFieldInt_((char*)AName, NULL); }

   static bool       FieldAsBool(long AIndex)
                     { return RCKRespFieldBool(AIndex, NULL); }
   static bool       FieldAsBool(const char* AName)
                     { return RCKRespFieldBool_(AName, NULL); }
   static bool       FieldAsBool(const KYString& AName)
                     { return RCKRespFieldBool_((char*)AName, NULL); }

   static double     FieldAsFloat(long AIndex)
                     { return RCKRespFieldFloat(AIndex, NULL); }
   static double     FieldAsFloat(const char* AName)
                     { return RCKRespFieldFloat_(AName, NULL); }
   static double     FieldAsFloat(const KYString& AName)
                     { return RCKRespFieldFloat_((char*)AName, NULL); }

   static void*      FieldAsStruct(long AIndex)
                     { return RCKRespFieldStruct(AIndex, NULL); }
   static void*      FieldAsStruct(const char* AName)
                     { return RCKRespFieldStruct_(AName, NULL); }
   static void*      FieldAsStruct(const KYString& AName)
                     { return RCKRespFieldStruct_((char*)AName, NULL); }

   static __int64    FieldAsInt64(long AIndex)
                     { return RCKRespFieldInt64(AIndex, NULL); }
   static __int64    FieldAsInt64(const char* AName)
                     { return RCKRespFieldInt64_(AName, NULL); }
   static __int64    FieldAsInt64(const KYString& AName)
                     { return RCKRespFieldInt64_((char*)AName, NULL); }

   static TDateTime  FieldAsDate(long AIndex)
                     { return RCKRespFieldDate(AIndex, NULL); }
   static TDateTime  FieldAsDate(const char* AName)
                     { return RCKRespFieldDate_(AName, NULL); }
   static TDateTime  FieldAsDate(const KYString& AName)
                     { return RCKRespFieldDate_((char*)AName, NULL); }

   static long       FieldAsCustom(long AIndex, void* AValue, long ASize)
                     { return RCKRespFieldCustom(AIndex, AValue, ASize); }
   static long       FieldAsCustom(const char* AName, void* AValue, long ASize)
                     { return RCKRespFieldCustom_(AName, AValue, ASize); }
   static long       FieldAsCustom(const KYString& AName, void* AValue, long ASize)
                     { return RCKRespFieldCustom_((char*)AName, AValue, ASize); }

   // ȡ�����Ͳ���ֵ����ط���
   static KYString   ParamAsStr(long AIndex);
   static KYString   ParamAsStr(const char* AName);
   static KYString   ParamAsStr(const KYString& AName)
                     { return ParamAsStr((char*)AName); }
   static char*      ParamAsStr(long AIndex, long& ASize)
                     { return RCKRespParamStr(AIndex, &ASize); }
   static char*      ParamAsStr(const char* AName, long& ASize)
                     { return RCKRespParamStr_(AName, &ASize); }

   static long       ParamAsInt(long AIndex)
                     { return RCKRespParamInt(AIndex, NULL); }
   static long       ParamAsInt(const char* AName)
                     { return RCKRespParamInt_(AName, NULL); }
   static long       ParamAsInt(const KYString& AName)
                     { return RCKRespParamInt_((char*)AName, NULL); }

   static bool       ParamAsBool(long AIndex)
                     { return RCKRespParamBool(AIndex, NULL); }
   static bool       ParamAsBool(const char* AName)
                     { return RCKRespParamBool_(AName, NULL); }
   static bool       ParamAsBool(const KYString& AName)
                     { return RCKRespParamBool_((char*)AName, NULL); }

   static double     ParamAsFloat(long AIndex)
                     { return RCKRespParamFloat(AIndex, NULL); }
   static double     ParamAsFloat(const char* AName)
                     { return RCKRespParamFloat_(AName, NULL); }
   static double     ParamAsFloat(const KYString& AName)
                     { return RCKRespParamFloat_((char*)AName, NULL); }

   static void*      ParamAsStruct(long AIndex)
                     { return RCKRespParamStruct(AIndex, NULL); }
   static void*      ParamAsStruct(const char* AName)
                     { return RCKRespParamStruct_(AName, NULL); }
   static void*      ParamAsStruct(const KYString& AName)
                     { return RCKRespParamStruct_((char*)AName, NULL); }

   static __int64    ParamAsInt64(long AIndex)
                     { return RCKRespParamInt64(AIndex, NULL); }
   static __int64    ParamAsInt64(const char* AName)
                     { return RCKRespParamInt64_(AName, NULL); }
   static __int64    ParamAsInt64(const KYString& AName)
                     { return RCKRespParamInt64_((char*)AName, NULL); }

   static TDateTime  ParamAsDate(long AIndex)
                     { return RCKRespParamDate(AIndex, NULL); }
   static TDateTime  ParamAsDate(const char* AName)
                     { return RCKRespParamDate_(AName, NULL); }
   static TDateTime  ParamAsDate(const KYString& AName)
                     { return RCKRespParamDate_((char*)AName, NULL); }

   static long       ParamAsCustom(long AIndex, void* AValue, long ASize)
                     { return RCKRespParamCustom(AIndex, AValue, ASize); }
   static long       ParamAsCustom(const char* AName, void* AValue, long ASize)
                     { return RCKRespParamCustom_(AName, AValue, ASize); }
   static long       ParamAsCustom(const KYString& AName, void* AValue, long ASize)
                     { return RCKRespParamCustom_((char*)AName, AValue, ASize); }

   // ���ø����ͷ���ֵ����ط���
   static long       RetByStr(const char* AValue, long ASize = -1)
                     { return RCKRespRetByStr(AValue, ASize); }
   static long       RetByStr(const KYString& AValue)
                     { return RCKRespRetByStr((char*)AValue, AValue.Length()); }
   static long       RetByInt(long AValue)
                     { return RCKRespRetByInt(AValue); }
   static long       RetByBool(bool AValue)
                     { return RCKRespRetByBool(AValue); }
   static long       RetByFloat(double AValue)
                     { return RCKRespRetByFloat(AValue); }
   static long       RetByStruct(void* AValue)
                     { return RCKRespRetByStruct(AValue); }
   static long       RetByInt64(__int64 AValue)
                     { return RCKRespRetByInt64(AValue); }
   static long       RetByDate(TDateTime AValue)
                     { return RCKRespRetByDate(AValue); }
   static long       RetByCustom(const void* AValue, long ASize)
                     { return RCKRespRetByCustom(AValue, ASize); }

   // ���ø����ͷ����ֶ�ֵ����ط���
   static long       FieldByStr(long AIndex, const char* AValue, long ASize = -1)
                     { return RCKRespFieldByStr(AIndex, AValue, ASize); }
   static long       FieldByStr(long AIndex, const KYString& AValue)
                     { return RCKRespFieldByStr(AIndex, (char*)AValue, AValue.Length()); }
   static long       FieldByStr(const char* AName, const KYString& AValue)
                     { return RCKRespFieldByStr_(AName, (char*)AValue, AValue.Length()); }
   static long       FieldByStr(const KYString& AName, const KYString& AValue)
                     { return RCKRespFieldByStr_((char*)AName,
                                                (char*)AValue, AValue.Length()); }

   static long       FieldByInt(long AIndex, long AValue)
                     { return RCKRespFieldByInt(AIndex, AValue); }
   static long       FieldByInt(const char* AName, long AValue)
                     { return RCKRespFieldByInt_(AName, AValue); }
   static long       FieldByInt(const KYString& AName, long AValue)
                     { return RCKRespFieldByInt_((char*)AName, AValue); }

   static long       FieldByBool(long AIndex, bool AValue)
                     { return RCKRespFieldByBool(AIndex, AValue); }
   static long       FieldByBool(const char* AName, bool AValue)
                     { return RCKRespFieldByBool_(AName, AValue); }
   static long       FieldByBool(const KYString& AName, bool AValue)
                     { return RCKRespFieldByBool_((char*)AName, AValue); }

   static long       FieldByFloat(long AIndex, double AValue)
                     { return RCKRespFieldByFloat(AIndex, AValue); }
   static long       FieldByFloat(const char* AName, double AValue)
                     { return RCKRespFieldByFloat_(AName, AValue); }
   static long       FieldByFloat(const KYString& AName, double AValue)
                     { return RCKRespFieldByFloat_((char*)AName, AValue); }

   static long       FieldByStruct(long AIndex, void* AValue)
                     { return RCKRespFieldByStruct(AIndex, AValue); }
   static long       FieldByStruct(const char* AName, void* AValue)
                     { return RCKRespFieldByStruct_(AName, AValue); }
   static long       FieldByStruct(const KYString& AName, void* AValue)
                     { return RCKRespFieldByStruct_((char*)AName, AValue); }

   static long       FieldByInt64(long AIndex, __int64 AValue)
                     { return RCKRespFieldByInt64(AIndex, AValue); }
   static long       FieldByInt64(const char* AName, __int64 AValue)
                     { return RCKRespFieldByInt64_(AName, AValue); }
   static long       FieldByInt64(const KYString& AName, __int64 AValue)
                     { return RCKRespFieldByInt64_((char*)AName, AValue); }

   static long       FieldByDate(long AIndex, TDateTime AValue)
                     { return RCKRespFieldByDate(AIndex, AValue); }
   static long       FieldByDate(const char* AName, TDateTime AValue)
                     { return RCKRespFieldByDate_(AName, AValue); }
   static long       FieldByDate(const KYString& AName, TDateTime AValue)
                     { return RCKRespFieldByDate_((char*)AName, AValue); }

   static long       FieldByCustom(long AIndex, const void* AValue, long ASize)
                     { return RCKRespFieldByCustom(AIndex, AValue, ASize); }
   static long       FieldByCustom(const char* AName, const void* AValue, long ASize)
                     { return RCKRespFieldByCustom_(AName, AValue, ASize); }
   static long       FieldByCustom(const KYString& AName, const void* AValue, long ASize)
                     { return RCKRespFieldByCustom_((char*)AName, AValue, ASize); }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TRCFields - RCK �ֶμ��� */

class TRCFields
{
public:
   TRCFields();
   TRCFields(void* AFields);
   virtual ~TRCFields();

   // ����
   void*          Fields() const       { return FFields; }
   bool           IsBound() const      { return FIsBound; }

   // �ֶ���Ϣ
   long           Count() const        { return RCKFieldCount(FFields); }
   KYString       Name(long AIndex) const
                  { return RCKFieldName(FFields, AIndex); }
   long           Size(long AIndex) const
                  { return RCKFieldSize(FFields, AIndex); }
   long           DefSize(long AIndex) const
                  { return RCKFieldDefSize(FFields, AIndex); }
   long           FieldType(long AIndex) const
                  { return RCKFieldType(FFields, AIndex); }
   bool           IsNull(long AIndex) const
                  { return RCKFieldIsNull(FFields, AIndex, NULL); }
   bool           IsNull_(const KYString& AName) const
                  { return RCKFieldIsNull_(FFields, (char*)AName, NULL); }

   // �ֶ�������
   long           IndexOf(const char* AName) const
                  { return RCKFieldIndex(FFields, AName); }
   long           IndexOf(const KYString& AName) const
                  { return RCKFieldIndex(FFields, (char*)AName); }

   // �ֶζ��巽��
   long           AddDef(const char* AName, long AType, long ASize) const
                  { return RCKFieldDefsAdd(FFields, AName, AType, ASize); }
   long           AddDef(const KYString& AName, long AType, long ASize) const
                  { return RCKFieldDefsAdd(FFields, (char*)AName, AType, ASize); }
   void           DeleteDef(long AIndex) const
                  { RCKFieldDefsDelete(FFields, AIndex); }
   void           ClearDefs() const    { RCKFieldDefsClear(FFields); }

   long           AssignDefs(const void* AFieldDefs, long ASize) const
                  { return RCKFieldDefsAssign(FFields, AFieldDefs, ASize); }
   KYString       FieldDefs() const;
   void*          FieldDefs(long& ASize) const
                  { return RCKFieldDefsOutput(FFields, &ASize); }
   long           Defs2Size(const void* AFieldDefs) const
                  { return RCKFieldDefsSize(AFieldDefs); }

   // �ֶ����ݷ���
   void           Clear() const        { RCKFieldsClear(FFields); }
   long           Assign(const void* AData, long ASize) const
                  { return RCKFieldsAssign(FFields, AData, ASize); }
   KYString       DataSet() const;
   void*          DataSet(long& ASize) const
                  { return RCKFieldsOutput(FFields, &ASize); }
   long           DataSize() const     { return RCKFieldsSize(FFields); }

   // ȡ�������ֶ�ֵ����ط���
   KYString       AsStr(long AIndex) const;
   KYString       AsStr(const char* AName) const;
   KYString       AsStr(const KYString& AName) const
                  { return AsStr((char*)AName); }
   char*          AsStr(long AIndex, long& ASize) const
                  { return RCKFieldAsStr(FFields, AIndex, &ASize); }
   char*          AsStr(const char* AName, long& ASize) const
                  { return RCKFieldAsStr_(FFields, AName, &ASize); }

   long           AsInt(long AIndex) const
                  { return RCKFieldAsInt(FFields, AIndex, NULL); }
   long           AsInt(const char* AName) const
                  { return RCKFieldAsInt_(FFields, AName, NULL); }
   long           AsInt(const KYString& AName) const
                  { return RCKFieldAsInt_(FFields, (char*)AName, NULL); }

   bool           AsBool(long AIndex) const
                  { return RCKFieldAsBool(FFields, AIndex, NULL); }
   bool           AsBool(const char* AName) const
                  { return RCKFieldAsBool_(FFields, AName, NULL); }
   bool           AsBool(const KYString& AName) const
                  { return RCKFieldAsBool_(FFields, (char*)AName, NULL); }

   double         AsFloat(long AIndex) const
                  { return RCKFieldAsFloat(FFields, AIndex, NULL); }
   double         AsFloat(const char* AName) const
                  { return RCKFieldAsFloat_(FFields, AName, NULL); }
   double         AsFloat(const KYString& AName) const
                  { return RCKFieldAsFloat_(FFields, (char*)AName, NULL); }

   void*          AsStruct(long AIndex) const
                  { return RCKFieldAsStruct(FFields, AIndex, NULL); }
   void*          AsStruct(const char* AName) const
                  { return RCKFieldAsStruct_(FFields, AName, NULL); }
   void*          AsStruct(const KYString& AName) const
                  { return RCKFieldAsStruct_(FFields, (char*)AName, NULL); }

   __int64        AsInt64(long AIndex) const
                  { return RCKFieldAsInt64(FFields, AIndex, NULL); }
   __int64        AsInt64(const char* AName) const
                  { return RCKFieldAsInt64_(FFields, AName, NULL); }
   __int64        AsInt64(const KYString& AName) const
                  { return RCKFieldAsInt64_(FFields, (char*)AName, NULL); }

   TDateTime      AsDate(long AIndex) const
                  { return RCKFieldAsDate(FFields, AIndex, NULL); }
   TDateTime      AsDate(const char* AName) const
                  { return RCKFieldAsDate_(FFields, AName, NULL); }
   TDateTime      AsDate(const KYString& AName) const
                  { return RCKFieldAsDate_(FFields, (char*)AName, NULL); }

   long           AsCustom(long AIndex, void* AValue, long ASize) const
                  { return RCKFieldGetCustom(FFields, AIndex, AValue, ASize); }
   long           AsCustom(const char* AName, void* AValue, long ASize) const
                  { return RCKFieldGetCustom_(FFields, AName, AValue, ASize); }
   long           AsCustom(const KYString& AName, void* AValue, long ASize) const
                  { return RCKFieldGetCustom_(FFields, (char*)AName, AValue, ASize); }

   // ���ø������ֶ�ֵ����ط���
   long           ByStr(long AIndex, const char* AValue, long ASize = -1) const
                  { return RCKFieldByStr(FFields, AIndex, AValue, ASize); }
   long           ByStr(long AIndex, const KYString& AValue) const
                  { return RCKFieldByStr(FFields, AIndex, (char*)AValue, AValue.Length()); }
   long           ByStr(const char* AName, const KYString& AValue) const
                  { return RCKFieldByStr_(FFields, AName, (char*)AValue, AValue.Length()); }
   long           ByStr(const KYString& AName, const KYString& AValue) const
                  { return RCKFieldByStr_(FFields, (char*)AName,
                                             (char*)AValue, AValue.Length()); }

   long           ByInt(long AIndex, long AValue) const
                  { return RCKFieldByInt(FFields, AIndex, AValue); }
   long           ByInt(const char* AName, long AValue) const
                  { return RCKFieldByInt_(FFields, AName, AValue); }
   long           ByInt(const KYString& AName, long AValue) const
                  { return RCKFieldByInt_(FFields, (char*)AName, AValue); }

   long           ByBool(long AIndex, bool AValue) const
                  { return RCKFieldByBool(FFields, AIndex, AValue); }
   long           ByBool(const char* AName, bool AValue) const
                  { return RCKFieldByBool_(FFields, AName, AValue); }
   long           ByBool(const KYString& AName, bool AValue) const
                  { return RCKFieldByBool_(FFields, (char*)AName, AValue); }

   long           ByFloat(long AIndex, double AValue) const
                  { return RCKFieldByFloat(FFields, AIndex, AValue); }
   long           ByFloat(const char* AName, double AValue) const
                  { return RCKFieldByFloat_(FFields, AName, AValue); }
   long           ByFloat(const KYString& AName, double AValue) const
                  { return RCKFieldByFloat_(FFields, (char*)AName, AValue); }

   long           ByStruct(long AIndex, void* AValue) const
                  { return RCKFieldByStruct(FFields, AIndex, AValue); }
   long           ByStruct(const char* AName, void* AValue) const
                  { return RCKFieldByStruct_(FFields, AName, AValue); }
   long           ByStruct(const KYString& AName, void* AValue) const
                  { return RCKFieldByStruct_(FFields, (char*)AName, AValue); }

   long           ByInt64(long AIndex, __int64 AValue) const
                  { return RCKFieldByInt64(FFields, AIndex, AValue); }
   long           ByInt64(const char* AName, __int64 AValue) const
                  { return RCKFieldByInt64_(FFields, AName, AValue); }
   long           ByInt64(const KYString& AName, __int64 AValue) const
                  { return RCKFieldByInt64_(FFields, (char*)AName, AValue); }

   long           ByDate(long AIndex, TDateTime AValue) const
                  { return RCKFieldByDate(FFields, AIndex, AValue); }
   long           ByDate(const char* AName, TDateTime AValue) const
                  { return RCKFieldByDate_(FFields, AName, AValue); }
   long           ByDate(const KYString& AName, TDateTime AValue) const
                  { return RCKFieldByDate_(FFields, (char*)AName, AValue); }

   long           ByCustom(long AIndex, const void* AValue, long ASize) const
                  { return RCKFieldSetCustom(FFields, AIndex, AValue, ASize); }
   long           ByCustom(const char* AName, const void* AValue, long ASize) const
                  { return RCKFieldSetCustom_(FFields, AName, AValue, ASize); }
   long           ByCustom(const KYString& AName, const void* AValue, long ASize) const
                  { return RCKFieldSetCustom_(FFields, (char*)AName, AValue, ASize); }

private:
   void*          FFields;             // �ֶμ����
   bool           FIsBound;            // �Ƿ�󶨶���
};

/* end namespace */

   }
}

// �����ռ�ı���
namespace RCKObjs = RCKernel::__object__;

#endif
