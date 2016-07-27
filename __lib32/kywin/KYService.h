// =======================================
// Unit   : ������ (KYService.h)
// Version: 3.0.0.0 (build 2011.03.10)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYService_H_
#define _KYService_H_

#include "KYWinSvc.h"

// KYWin 2.0 ��ʼʹ�� KYWin �����ռ�
namespace KYWin
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ���ͺͳ������� */

// ������ƺ���ָ������
//typedef void (__stdcall *TKYSvcControl)(Longword ACtrlCode);
typedef LPHANDLER_FUNCTION       TKYSvcControl;

// �¼���־����
enum TEventLogType  {etEvtSuccess      = EVENTLOG_SUCCESS,
                     etEvtError        = EVENTLOG_ERROR_TYPE,
                     etEvtWarning      = EVENTLOG_WARNING_TYPE,
                     etEvtInformation  = EVENTLOG_INFORMATION_TYPE,
                     etEvtAuditSuccess = EVENTLOG_AUDIT_SUCCESS,
                     etEvtAuditFailure = EVENTLOG_AUDIT_FAILURE};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TEventLogger - Windows �¼���־�� */

class TEventLogger
{
public:
   TEventLogger(const KYString& AName = "");
   virtual ~TEventLogger();

   // ����
   KYString       Name() const         { return FName; }
   THandle        Handle() const       { return FHandle; }

   // ����
   void           LogMessage(const KYString& AMessage,
                                       Word  AType       = etEvtError,
                                       Word  ACategory   = 0,
                                   Longword  AEventID    = 0,
                             const KYString& ARawData    = "");

protected:
private:
   KYString       FName;
   THandle        FHandle;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYService - ������ */

class TKYService
{
public:
   // TOnCustom �¼�
   typedef void (TObject::*TDoCustom)(TKYService* Sender);
   typedef struct
   {
      TDoCustom   Method;
      void*       Object;
   } TOnCustom;

   // TOnMain �¼�
   typedef bool (TObject::*TDoMain)(TKYService* Sender,
                           const TKYStringList& AParams,
                                 TKYSvcControl& AControl);
   typedef struct
   {
      TDoMain     Method;
      void*       Object;
   } TOnMain;

   // TOnCan �¼�
   typedef void (TObject::*TDoCan)(TKYService* Sender, bool& ACan);
   typedef struct
   {
      TDoCan      Method;
      void*       Object;
   } TOnCan;

public:
   TKYService(const KYString& AName = "");
   virtual ~TKYService();

   // ����
   void*          Data() const         { return FData; }          // default: NULL;
   TKYEvent*      Notify() const       { return FNotify; }        // default: (object)
   Longword       Handle() const       { return FHandle; }        // default: 0
   Longword       ErrorCode() const    { return FErrorCode; }     // default: 0
   Longword       CurrState() const    { return FCurrState; }     // default: ssSvcStopped
   TKYStringList* Parameters() const   { return FParameters; }    // default: (object)
   bool           Terminated() const   { return FTerminated; }    // default: false
   bool           AllowStop() const    { return FAllowStop; }     // default: true
   bool           AllowPause() const   { return FAllowPause; }    // default: true
   bool           SharedProc() const   { return FSharedProc; }    // default: false
   bool           Interactive() const  { return FInteractive; }   // default: false

   Longword       GrpTagID() const     { return FGrpTagID; }      // default: 0
   Longword       WaitHint() const     { return FWaitHint; }      // default: 5000
   Longword       StartType() const    { return FStartType; }     // default: sstSvcAuto
   Longword       ServiceType() const  { return FServiceType; }   // default: stSvcOwnProc
   Longword       ErrorControl() const { return FErrorControl; }  // default: secSvcNormal
   Longword       Win32ErrCode() const { return FWin32ErrCode; }  // default: 0

   KYString       Name() const         { return FName; }          // default: AName
   KYString       CmdLine() const      { return FCmdLine; }       // default: CurrAppFileName
   KYString       Account() const      { return FAccount; }       // default: ""
   KYString       Password() const     { return FPassword; }      // default: ""
   KYString       LoadGroup() const    { return FLoadGroup; }     // default: ""
   KYString       DisplayName() const  { return FDisplayName; }   // default: AName
   KYString       Description() const  { return FDescription; }   // default: ""
   TKYStringList* Dependencies() const { return FDependencies; }  // default: (object)

   // ��������
   void           SetData(void* AData) { FData = AData; }
   void           SetCurrState(Longword AState);
   void           SetAllowStop(bool Allow);
   void           SetAllowPause(bool Allow);
   void           SetSharedProc(bool AShared);
   void           SetInteractive(bool AEnabled);

   void           SetGrpTagID(Longword AID);
   void           SetWaitHint(Longword AValue);
   void           SetStartType(Longword AType);
   void           SetServiceType(Longword AType);
   void           SetErrorControl(Longword AValue);
   void           SetWin32ErrCode(Longword AValue);

   void           SetName(const KYString& AName);
   void           SetCmdLine(const KYString& ACmdLine);
   void           SetAccount(const KYString& AValue);
   void           SetPassword(const KYString& AValue);
   void           SetLoadGroup(const KYString& AValue);
   void           SetDisplayName(const KYString& AValue);
   void           SetDescription(const KYString& AValue);

   // ��װ/ж�ط���
   bool           Install();
   bool           Uninstall();

   // ��ֹ����
   void           Terminate();

   // �������״̬(TServiceStatus)
   void           ReportStatus();

   // ������ƺ���(TKYSvcControl �������õķ���)
   void           Control(Longword ACtrlCode);

   // ��־��Ϣ
   void           LogMessage(const KYString& AMessage,
                                       Word  AType       = etEvtError,
                                       Word  ACategory   = 0,
                                   Longword  AEventID    = 0,
                             const KYString& ARawData    = "");

   // �¼�
   TOnMain        OnMain;              // ��������¼�(��Ϊ����ʹ��ȱʡServiceControl����,
                                       //              һ��������ֻ����һ����������Ϊ��)
   TOnCan         OnStart;             // ���������¼�
   TOnCustom      OnExecute;           // ����ִ���¼�(����ʹ��Notify()��Terminated()����ѭ������)

   TOnCan         OnStop;              // ����ֹͣ�¼�
   TOnCan         OnPause;             // ������ͣ�¼�
   TOnCan         OnContinue;          // ��������¼�
   TOnCustom      OnShutdown;          // ����ر��¼�

protected:
private:
   void           DoMain(Longword argc, char* argv[]);
   void           DoStart();
   void           DoExecute();

   bool           DoStop();
   bool           DoPause();
   bool           DoContinue();
   void           DoShutdown();

private:
   void*          FData;
   TKYEvent*      FNotify;
   Longword       FHandle;
   Longword       FErrorCode;
   Longword       FLastState;
   Longword       FCurrState;

   TKYStringList* FParameters;
   TEventLogger*  FEventLogger;

   bool           FTerminated;
   bool           FAllowStop;
   bool           FAllowPause;
   bool           FSharedProc;
   bool           FInteractive;

   Longword       FGrpTagID;
   Longword       FWaitHint;
   Longword       FStartType;
   Longword       FServiceType;
   Longword       FErrorControl;
   Longword       FWin32ErrCode;

   KYString       FName;
   KYString       FCmdLine;
   KYString       FAccount;
   KYString       FPassword;
   KYString       FLoadGroup;
   KYString       FDisplayName;
   KYString       FDescription;
   TKYStringList* FDependencies;

public:
   // ȡ��������
   static Longword         GetSvcType(Longword ASvcType, bool AInteractive, bool AShared);
   static Longword         GetCtrlAccepted(bool ACanStop, bool ACanPause);

   // �����б���: ��ӷ���, ɾ������, ������з���
   static bool             AddSvc(const KYString& AName, TKYService* AService);
   static void             DeleteSvc(const KYString& AName);
   static void             ClearSvcs();

   // ���з����б��е����з���
   static bool             Run();

private:
   // ������������Ϳ��ƺ���
   static void __stdcall   ServiceMain(Longword argc, char* argv[]);
   static void __stdcall   ServiceControl(Longword ACtrlCode);

   // �����������ֹ
   static void             ServiceTerminate(char* AName);

private:
   // �����б�
   static TKYCritSect*     Inner_Lock;
   static TKYStringList*   Inner_Services;
   static bool             Inner_IsRunning;

   // Ĭ�Ϸ������ָ��(ע: ServiceControlȱʡ����)
   static TKYService*      Inner_DefaultSvc;

   // TKYService �ľ�̬��Ա��ʼ����
   static class TInitialization
   {
   public:
      TInitialization();
      ~TInitialization();
   } Inner_Initialization;

   friend class TInitialization;
};

}

#endif
