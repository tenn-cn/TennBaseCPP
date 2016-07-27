// =======================================
// Unit   : 服务类 (KYService.h)
// Version: 3.0.0.0 (build 2011.03.10)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYService_H_
#define _KYService_H_

#include "KYWinSvc.h"

// KYWin 2.0 开始使用 KYWin 命名空间
namespace KYWin
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 类型和常量定义 */

// 服务控制函数指针类型
//typedef void (__stdcall *TKYSvcControl)(Longword ACtrlCode);
typedef LPHANDLER_FUNCTION       TKYSvcControl;

// 事件日志类型
enum TEventLogType  {etEvtSuccess      = EVENTLOG_SUCCESS,
                     etEvtError        = EVENTLOG_ERROR_TYPE,
                     etEvtWarning      = EVENTLOG_WARNING_TYPE,
                     etEvtInformation  = EVENTLOG_INFORMATION_TYPE,
                     etEvtAuditSuccess = EVENTLOG_AUDIT_SUCCESS,
                     etEvtAuditFailure = EVENTLOG_AUDIT_FAILURE};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TEventLogger - Windows 事件日志类 */

class TEventLogger
{
public:
   TEventLogger(const KYString& AName = "");
   virtual ~TEventLogger();

   // 属性
   KYString       Name() const         { return FName; }
   THandle        Handle() const       { return FHandle; }

   // 方法
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

/* TKYService - 服务类 */

class TKYService
{
public:
   // TOnCustom 事件
   typedef void (TObject::*TDoCustom)(TKYService* Sender);
   typedef struct
   {
      TDoCustom   Method;
      void*       Object;
   } TOnCustom;

   // TOnMain 事件
   typedef bool (TObject::*TDoMain)(TKYService* Sender,
                           const TKYStringList& AParams,
                                 TKYSvcControl& AControl);
   typedef struct
   {
      TDoMain     Method;
      void*       Object;
   } TOnMain;

   // TOnCan 事件
   typedef void (TObject::*TDoCan)(TKYService* Sender, bool& ACan);
   typedef struct
   {
      TDoCan      Method;
      void*       Object;
   } TOnCan;

public:
   TKYService(const KYString& AName = "");
   virtual ~TKYService();

   // 属性
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

   // 设置属性
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

   // 安装/卸载服务
   bool           Install();
   bool           Uninstall();

   // 终止服务
   void           Terminate();

   // 报告服务状态(TServiceStatus)
   void           ReportStatus();

   // 服务控制函数(TKYSvcControl 函数调用的方法)
   void           Control(Longword ACtrlCode);

   // 日志消息
   void           LogMessage(const KYString& AMessage,
                                       Word  AType       = etEvtError,
                                       Word  ACategory   = 0,
                                   Longword  AEventID    = 0,
                             const KYString& ARawData    = "");

   // 事件
   TOnMain        OnMain;              // 服务入口事件(若为空则使用缺省ServiceControl处理,
                                       //              一个进程中只能有一个可以设置为空)
   TOnCan         OnStart;             // 服务启动事件
   TOnCustom      OnExecute;           // 服务执行事件(必须使用Notify()和Terminated()属性循环处理)

   TOnCan         OnStop;              // 服务停止事件
   TOnCan         OnPause;             // 服务暂停事件
   TOnCan         OnContinue;          // 服务继续事件
   TOnCustom      OnShutdown;          // 服务关闭事件

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
   // 取服务类型
   static Longword         GetSvcType(Longword ASvcType, bool AInteractive, bool AShared);
   static Longword         GetCtrlAccepted(bool ACanStop, bool ACanPause);

   // 运行列表中: 添加服务, 删除服务, 清除所有服务
   static bool             AddSvc(const KYString& AName, TKYService* AService);
   static void             DeleteSvc(const KYString& AName);
   static void             ClearSvcs();

   // 运行服务列表中的所有服务
   static bool             Run();

private:
   // 服务的主函数和控制函数
   static void __stdcall   ServiceMain(Longword argc, char* argv[]);
   static void __stdcall   ServiceControl(Longword ACtrlCode);

   // 服务非正常终止
   static void             ServiceTerminate(char* AName);

private:
   // 服务列表
   static TKYCritSect*     Inner_Lock;
   static TKYStringList*   Inner_Services;
   static bool             Inner_IsRunning;

   // 默认服务对象指针(注: ServiceControl缺省对象)
   static TKYService*      Inner_DefaultSvc;

   // TKYService 的静态成员初始化类
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
