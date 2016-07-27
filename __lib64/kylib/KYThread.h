// =======================================
// Unit   : �߳��� (KYThread.h)
// Version: 3.0.0.0 (build 2011.03.09)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYThread_H_
#define _KYThread_H_

#include <process.h>
#include <windows.h>
#include "KYObject.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �������� */

class TThread;
class TKYThread;

// ǿ����ֹ�߳�(ע: ������ TThread �̳ж���)
bool  KillThread(TThread* AThread, Longword AExitCode);

// ǿ����ֹ�߳�(ע: �������߳� API ����)
bool  KillThread(THandle AHandle, Longword AExitCode);

// �رղ��ͷ��߳�
void  FreeKYThread(TKYThread* &AThread, Longword ATimeout = 5000);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TThread - �߳��� */

class TThread
{
   // ǿ����ֹ�̺߳���
   friend bool    KillThread(TThread*, Longword);

   // �̺߳���
   static unsigned __stdcall ThreadProc(void* AThread);

public:
   // �߳����ȼ�
   enum TPriority {tpIdle,   tpLowest,  tpLower, tpNormal,
                   tpHigher, tpHighest, tpTimeCritical};

public:
   TThread(bool ACreateSuspended = true);
   virtual ~TThread();

   // �߳����ȼ�, Ĭ��ֵΪ: tpNormal
   TPriority      Priority() const;

   // �߳̾��
   THandle        Handle() const          { return FHandle; }

   // �߳� ID
   unsigned       ThreadID() const        { return FThreadID; }

   // �̷߳���ֵ, Ĭ��ֵΪ: 0
   unsigned       ReturnValue() const     { return FReturnValue; }

   // �Ƿ��߳���ֹʱ�ͷ��̶߳���, Ĭ��ֵΪ: false
   bool           FreeOnTerminate() const { return FFreeOnTerminate; }

   // �Ƿ��߳�������ֹ��־, Ĭ��ֵΪ: false
   bool           Terminated() const      { return FTerminated; }

   // �Ƿ��߳�������, Ĭ��ֵΪ: ACreateSuspended
   bool           Suspended() const       { return FSuspended; }

   // �Ƿ��߳������н���, Ĭ��ֵΪ: false
   bool           Finished() const        { return FFinished; }

   // �����߳����ȼ�
   void           SetPriority(TPriority AValue) const;

   // �����Ƿ��߳���ֹʱ�ͷ��̶߳���
   void           SetFreeOnTerminate(bool AValue)
                  { FFreeOnTerminate = AValue; }

   // ���������ߵ��߳�
   void           Resume();

   // �������е��߳�
   void           Suspend();

   // �����߳���ֹ��־
   void           Terminate();

   // �ȴ��߳����н���
   bool           WaitFor(Longword ATimeout = INFINITE);

   // �¼�
   TNotifyEvent   OnTerminate;

protected:
   void           Return(unsigned AValue) { FReturnValue = AValue; }

   virtual void   Execute() = 0;
   virtual void   DoTerminate();

private:
   THandle        FHandle;
   unsigned       FThreadID;
   unsigned       FReturnValue;
   bool           FFreeOnTerminate;
   bool           FTerminated;
   bool           FSuspended;
   bool           FFinished;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYThread - �����߳��� */

class TKYThread: public TThread
{
public:
   // �߳�ִ�з���
   typedef unsigned (TObject::*TDoExecute)(TKYThread* AThread, void* AParam);

public:
   TKYThread(TDoExecute AExecute = NULL, void* AObject = NULL,
                  void* AParam = NULL,    bool ACreateSuspended = true);

protected:
   virtual void   Execute();

private:
   TDoExecute     FExecute;
   void*          FObject;
   void*          FParam;
};

}

#endif
