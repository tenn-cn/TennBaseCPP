// =======================================
// Unit   : Windows 服务操作类 (KYWinSvc.h)
// Version: 3.0.0.0 (build 2011.03.10)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYWinSvc_H_
#define _KYWinSvc_H_

//#include <winsvc.h>
#include "KYWUtils.h"

// KYWin 2.0 开始使用 KYWin 命名空间
namespace KYWin
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 类型和常量定义 */

// 类型重定义
typedef SERVICE_STATUS           TServiceStatus;
typedef QUERY_SERVICE_CONFIGA    TQuerySvcConfig;

// 未改变服务配置的值
enum TSvcConfigValue{cfgSvcNoChange    = SERVICE_NO_CHANGE};

// 服务类型(Service Type (Bit Mask))
enum TServiceType   {stSvcKernelDrv    = SERVICE_KERNEL_DRIVER,
                     stSvcFileSysDrv   = SERVICE_FILE_SYSTEM_DRIVER,
                     stSvcAdapter      = SERVICE_ADAPTER,
                     stSvcRcgDrv       = SERVICE_RECOGNIZER_DRIVER,
                     stSvcDriver       = SERVICE_DRIVER,
                     stSvcOwnProc      = SERVICE_WIN32_OWN_PROCESS,
                     stSvcShareProc    = SERVICE_WIN32_SHARE_PROCESS,
                     stSvcWin32        = SERVICE_WIN32,
                     stSvcInteractProc = SERVICE_INTERACTIVE_PROCESS,
                     stSvcAll          = SERVICE_TYPE_ALL};

// 服务启动类型(Service Start Type)
enum TSvcStartType  {sstSvcBoot        = SERVICE_BOOT_START,
                     sstSvcSystem      = SERVICE_SYSTEM_START,
                     sstSvcAuto        = SERVICE_AUTO_START,
                     sstSvcDemand      = SERVICE_DEMAND_START,
                     sstSvcDisabled    = SERVICE_DISABLED};


// 当前服务状态(Service State)
enum TServiceState  {ssSvcStopped      = SERVICE_STOPPED,
                     ssSvcStarting     = SERVICE_START_PENDING,
                     ssSvcStopping     = SERVICE_STOP_PENDING,
                     ssSvcRunning      = SERVICE_RUNNING,
                     ssSvcContinuing   = SERVICE_CONTINUE_PENDING,
                     ssSvcPausing      = SERVICE_PAUSE_PENDING,
                     ssSvcPaused       = SERVICE_PAUSED};

// 服务控制(Service Controls)
enum TServiceControl{scSvcStop         = SERVICE_CONTROL_STOP,
                     scSvcPause        = SERVICE_CONTROL_PAUSE,
                     scSvcContinue     = SERVICE_CONTROL_CONTINUE,
                     scSvcInterrogate  = SERVICE_CONTROL_INTERROGATE,
                     scSvcShutdown     = SERVICE_CONTROL_SHUTDOWN};

// 服务控制权限(Service Controls Accepted (Bit Mask))
enum TServiceAccept {scaSvcStop        = SERVICE_ACCEPT_STOP,
                     scaSvcPauseContinue=SERVICE_ACCEPT_PAUSE_CONTINUE,
                     scaSvcShutdown    = SERVICE_ACCEPT_SHUTDOWN};

// 服务错误控制类型(Service Error Control)
enum TServiceError  {secSvcIgnore      = SERVICE_ERROR_IGNORE,
                     secSvcNormal      = SERVICE_ERROR_NORMAL,
                     secSvcSevere      = SERVICE_ERROR_SEVERE,
                     secSvcCritical    = SERVICE_ERROR_CRITICAL};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYWinSvc - 服务操作类 */

class TKYWinSvc
{
public:
   TKYWinSvc();
   virtual ~TKYWinSvc();

   // 属性
   bool              Opened() const    { return FOpened; }
   THandle           Service() const   { return FService; }
   KYString          SvcName() const   { return FSvcName; }
   Longword          SvcType();
   Longword          StartType();
   Longword          CurrState();
   Longword          LastError() const { return FLastError; }
   KYString          DisplayName();
   KYString          Description();

   // 创建服务(安装服务)
   bool              Create(const KYString& ASvcName,
                            const KYString& ACmdLine,
                            const KYString& ADisplayName,
                                  Longword  ASvcType        = stSvcOwnProc,
                                  Longword  AStartType      = sstSvcAuto,
                                  Longword  AErrorControl   = secSvcNormal,
                                  Longword* AGrpTagID       = NULL,
                            const KYString& ALoadGroup      = "",
                            const KYString& Account         = "",
                            const KYString& APassword       = "",
                       const TKYStringList* ADependencies   = NULL);

   // 打开/关闭(打开服务管理器)
   bool              Open(const KYString& ASvcName, bool ACanWrite = false);
   void              Close();

   // 服务控制方法
   bool              Start(const TKYStringList* AParameters = NULL);
   bool              Stop();
   bool              Pause();
   bool              Continue();

   // 删除服务(必须能写模式打开)
   bool              Delete();

   // 服务的配置
   bool              ChangeDescription(const KYString& ADescription);
   bool              ChangeConfig(const KYString& ADisplayName,
                                  const KYString& Account      = "",
                                  const KYString& APassword    = "",
                                        Longword  AStartType   = cfgSvcNoChange);
   bool              QueryConfig();

protected:
   bool              OpenManager(bool ACanWrite);
   void              CloseManager();

   bool              QueryStatus();

private:
   void              NewConfig();
   void              DeleteConfig();

private:
   bool              FOpened;
   bool              FQueried;
   THandle           FManager;
   THandle           FService;
   KYString          FSvcName;
   Longword          FLastError;
   TServiceStatus    FSvcStatus;
   TQuerySvcConfig*  FSvcConfig;
};

}

#endif
