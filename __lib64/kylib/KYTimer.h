// =======================================
// Unit   : ��ʱ���� (KYTimer.h)
// Version: 3.0.0.0 (build 2012.05.28)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYTimer_H_
#define _KYTimer_H_

#include "KYThread.h"
#include "KYHandles.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYTimer - ��ʱ���� */

class TKYTimer: public TThread
{
public:
   // ��ʱ���� DoTimer ����
   typedef void (TObject::*TDoTimer)(void* Sender, void* AParam, long ATimerID);

private:
   // ��ʱ����
   typedef struct
   {
      TDoTimer    Method;              // ����ָ��
      void*       Object;              // ����ָ��
      void*       Param;               // �������
      Longword    Interval;            // ��ʱ�����
      Longword    BeginTick;           // ��ʼ Tick
   } TItem, *PItem;

public:
   TKYTimer();
   virtual ~TKYTimer();

   // ��ʱ���Ƿ��Ѵ�, Ĭ��ֵΪ: false
   bool           Enabled() const               { return FEnabled; }

   // ��ʱ���ļ��(����), Ĭ��ֵΪ: 1000
   Longword       Interval() const              { return FInterval; }

   // �ж϶�ʱ�������Ƿ����
   bool           Existed(long ATimerID) const  { return FItems->IsExist(ATimerID); }

   // ���ö�ʱ���Ƿ��
   void           SetEnabled(bool AEnabled);

   // ���ö�ʱ���ļ��(����)
   void           SetInterval(Longword AInterval);

   // �رն�ʱ���߳�, ���� FreeKYTimer �ͷ��߳�
   void           Close();

   // �½���ʱ����, ÿ����ʱ������Լ��� TDoTimer �¼�����
   long           New(TDoTimer AMethod, void* AObject, void* AParam);

   // ɾ����ʱ����
   void           Delete(long ATimerID);

   // ������ж�ʱ����
   void           Clear();

   // ��Ӷ�ʱ����ָ��������¼�, ÿ���¼����� TDoTimer ���һ��
   bool           AddEvent(long ATimerID, Longword AInterval);

   // ɾ������Ӷ�ʱ����ָ��������¼�
   void           DeleteEvent(long ATimerID);

protected:
   virtual void   Execute();

   // ��
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

   // ����
   void           LockRead() const     { FLockRW->LockRead(); }
   void           UnlockRead() const   { FLockRW->UnlockRead(); }

   // д��
   void           LockWrite() const    { FLockRW->LockWrite(); }
   void           UnlockWrite() const  { FLockRW->UnlockWrite(); }

private:
   void           UpdateTimer();
   bool           ScanNext(bool AIsFirst);

   // FItems �� OnDeletion �¼�����
   void           DoDeletion(long AHandle, void* AItem);

private:
   TKYCritSect*   FLock;               // ��
   TKYLockRW*     FLockRW;             // ��д��
   TKYEvent*      FEvent;
   TKYHandles*    FItems;
   TKYHandles*    FTimers;
   Longword       FInterval;
   long           FPerfCount;
   bool           FEnabled;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ���ú��� */

// �رղ��ͷŶ�ʱ���߳�
void FreeKYTimer(TKYTimer* &AThread, Longword ATimeout = 15000);

}

#endif
