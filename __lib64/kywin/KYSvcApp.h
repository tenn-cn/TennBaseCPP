// =======================================
// Unit   : ��һ����Ӧ���� (KYSvcApp.h)
// Version: 3.0.0.0 (build 2011.03.10)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYSvcApp_H_
#define _KYSvcApp_H_

#include "KYService.h"

// KYWin 2.0 ��ʼʹ�� KYWin �����ռ�
namespace KYWin
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYSvcApp - ��һ����Ӧ���� */

class TKYSvcApp
{
public:
   // ��������
   enum TSwitch  {asHelp,              // display help information
                  asConsole,           // run console application
                  asService,           // run service application
                  asInstall,           // install service
                  asUninstall,         // uninstall service
                  asStart,             // start service
                  asStop,              // stop service
                  asPause,             // pause service
                  asContinue};         // continue service

   // ����/ֹͣ����ָ������
   typedef void (*TDoExecute)(const TKYStringList& AParams);
   typedef bool (*TDoStart)(const TKYStringList& AParams, bool AIsSvc);
   typedef void (*TDoStop)(bool AIsSvc);

   // �û��Զ��忪�غ���ָ������, ��������ֵ����: ASwitch > asContinue
   typedef void (*TDoUser)(long ASwitch, int argc, char* argv[]);

public:
   TKYSvcApp(const KYString& AName = "",  const KYString& ADisplayName = "",
                                          const KYString& ADescription = "");
   virtual ~TKYSvcApp();

   // ����
   KYString       Param() const        { return FParam; }
   TKYService*    SvcObj() const       { return FSvcObj; }
   bool           CaseSensitive() const{ return FSwitchs->CaseSensitive(); }

   // ��������
   void           SetParam(const KYString& AParam)
                  { FParam = AParam; }
   void           SetCaseSensitive(bool ACaseSensitive)
                  { FSwitchs->SetCaseSensitive(ACaseSensitive); }

   // ����
   void           Run(int argc, char* argv[]);

   // �������õ���ط���
   bool           AddSwitch(long ASwitch, const KYString& AName,
                                          const KYString& AHintInfo = "");
   void           DeleteSwitch(long ASwitch);
   void           ClearSwitchs();

   // ����ָ��
   TDoStart       OnStart;
   TDoStop        OnStop;

   TDoExecute     OnExecute;           // �� console ��ʽ�� OnStart �ɹ�֮��ִ��
   TDoUser        OnUser;              // �û��Զ��忪��ִ��

protected:
   // ���س�ʼ��
   void           InitSwitchs();
   KYString       ReadSwitchs();

   KYString       GetDescription();
   KYString       GetServiceSwitch();

private:
   // ������¼�����
   bool           DoMain(TKYService* Sender, const TKYStringList& AParams,
                                                   TKYSvcControl& AControl);
   void           DoStart(TKYService* Sender, bool& AStarted);
   void           DoStop(TKYService* Sender, bool& AStopped);
   void           DoShutdown(TKYService* Sender);

   // ���п��ص���ط���
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
   // ����Ŀ��ƺ���
   static void __stdcall   ServiceControl(Longword ACtrlCode);

private:
   // Ĭ�Ϸ������ָ��(ע: ServiceControlȱʡ����)
   static TKYService*      Inner_DefaultSvc;
};

}

#endif
