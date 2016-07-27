// =======================================
// Unit   : 单一服务应用类 (KYSvcApp.h)
// Version: 3.0.0.0 (build 2011.03.10)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYSvcApp_H_
#define _KYSvcApp_H_

#include "KYService.h"

// KYWin 2.0 开始使用 KYWin 命名空间
namespace KYWin
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYSvcApp - 单一服务应用类 */

class TKYSvcApp
{
public:
   // 开关类型
   enum TSwitch  {asHelp,              // display help information
                  asConsole,           // run console application
                  asService,           // run service application
                  asInstall,           // install service
                  asUninstall,         // uninstall service
                  asStart,             // start service
                  asStop,              // stop service
                  asPause,             // pause service
                  asContinue};         // continue service

   // 启动/停止函数指针类型
   typedef void (*TDoExecute)(const TKYStringList& AParams);
   typedef bool (*TDoStart)(const TKYStringList& AParams, bool AIsSvc);
   typedef void (*TDoStop)(bool AIsSvc);

   // 用户自定义开关函数指针类型, 开关类型值必须: ASwitch > asContinue
   typedef void (*TDoUser)(long ASwitch, int argc, char* argv[]);

public:
   TKYSvcApp(const KYString& AName = "",  const KYString& ADisplayName = "",
                                          const KYString& ADescription = "");
   virtual ~TKYSvcApp();

   // 属性
   KYString       Param() const        { return FParam; }
   TKYService*    SvcObj() const       { return FSvcObj; }
   bool           CaseSensitive() const{ return FSwitchs->CaseSensitive(); }

   // 设置属性
   void           SetParam(const KYString& AParam)
                  { FParam = AParam; }
   void           SetCaseSensitive(bool ACaseSensitive)
                  { FSwitchs->SetCaseSensitive(ACaseSensitive); }

   // 运行
   void           Run(int argc, char* argv[]);

   // 开关配置的相关方法
   bool           AddSwitch(long ASwitch, const KYString& AName,
                                          const KYString& AHintInfo = "");
   void           DeleteSwitch(long ASwitch);
   void           ClearSwitchs();

   // 函数指针
   TDoStart       OnStart;
   TDoStop        OnStop;

   TDoExecute     OnExecute;           // 在 console 方式下 OnStart 成功之后执行
   TDoUser        OnUser;              // 用户自定义开关执行

protected:
   // 开关初始化
   void           InitSwitchs();
   KYString       ReadSwitchs();

   KYString       GetDescription();
   KYString       GetServiceSwitch();

private:
   // 服务的事件方法
   bool           DoMain(TKYService* Sender, const TKYStringList& AParams,
                                                   TKYSvcControl& AControl);
   void           DoStart(TKYService* Sender, bool& AStarted);
   void           DoStop(TKYService* Sender, bool& AStopped);
   void           DoShutdown(TKYService* Sender);

   // 运行开关的相关方法
   void           DoSwitch_Help(const KYString& AName, bool ANotExist);
   void           DoSwitch_Console(int argc, char* argv[]);
   void           DoSwitch_Service();
   void           DoSwitch_Install();
   void           DoSwitch_Uninstall();
   void           DoSwitch_Start(int argc, char* argv[]);
   void           DoSwitch_Stop();
   void           DoSwitch_Pause();
   void           DoSwitch_Continue();
   void           DoSwitch_User(long ASwitch, int argc, char* argv[]);

private:
   bool           FChanged;
   KYString       FParam;
   TKYService*    FSvcObj;
   TKYStringList* FParams;
   TKYStringList* FSwitchs;
   TKYCode2Name*  FHintInfos;

private:
   // 服务的控制函数
   static void __stdcall   ServiceControl(Longword ACtrlCode);

private:
   // 默认服务对象指针(注: ServiceControl缺省对象)
   static TKYService*      Inner_DefaultSvc;
};

}

#endif
