// =======================================
// Unit   : �߳�ͬ���� (KYSyncObj.h)
// Version: 3.0.0.0 (build 2011.03.09)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYSyncObj_H_
#define _KYSyncObj_H_

#include <windows.h>
#include "KYObject.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ���ú��� */

// �õ���ȫ����
LPSECURITY_ATTRIBUTES GetSecurityAttrib();

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYCritSect - �ٽ����� */

class TKYCritSect
{
public:
   TKYCritSect();
   virtual ~TKYCritSect();

   // �����ٽ���, ��ͬһ�߳��ڿ��Եݹ����
   void              Enter()           { EnterCriticalSection(&FCritSect); }

   // �뿪�ٽ���, ��ͬһ�߳��ڿ��Եݹ����, ���뿪������������������ͬ
   void              Leave()           { LeaveCriticalSection(&FCritSect); }

private:
   CRITICAL_SECTION  FCritSect;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYEvent - �¼��� */

class TKYEvent
{
public:
   // �¼���λ���� {�Զ�, �ֶ�, ����}
   enum TResetType  {erAuto,           // �Զ���λ
                     erManual,         // �ֶ���λ
                     erPulse};         // ���帴λ

public:
   TKYEvent(const char* AName = NULL, TResetType AResetType = erAuto,
                                            bool AInitSignaled = false);
   virtual ~TKYEvent();

   // �¼����
   THandle           Handle() const    { return FEvent; }

   // ����
   void              Pulse() const     { PulseEvent(FEvent); }

   // ��λ
   void              Reset() const     { ResetEvent(FEvent); }

   // ��λ
   void              Set() const       { SetEvent(FEvent); }

   // �ȴ��¼�
   bool              Wait(DWORD ATimeout = INFINITE) const
                     {
                        return (WaitForSingleObject(FEvent, ATimeout) == WAIT_OBJECT_0);
                     }

private:
   THandle           FEvent;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMutex - ����Ԫ�� */

class TKYMutex
{
public:
   TKYMutex(const char* AName = NULL);
   virtual ~TKYMutex();

   // ����Ԫ���
   THandle           Handle() const    { return FHandle; }

   // ���ż���, ���ɹ��򷵻� true, ���򷵻� false
   bool              TryLock() const   { return Lock(0); }

   // ����, ���ɹ��򷵻� true, ���򷵻� false
   bool              Lock(DWORD ATimeout = INFINITE) const
                     {
                        return (WaitForSingleObject(FHandle, ATimeout) == WAIT_OBJECT_0);
                     }

   // ����
   void              Unlock() const    { ReleaseMutex(FHandle); }

private:
   THandle           FHandle;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYSemaphore - �ź����� */

class TKYSemaphore
{
public:
   TKYSemaphore(const char* AName = NULL, long AInitCount = 0,
                                          long AMaxCount  = 10);
   virtual ~TKYSemaphore();

   // �ź������
   THandle           Handle() const { return FHandle; }

   // �ȴ��ź���
   bool              DecCount(DWORD ATimeout = INFINITE) const
                     {
                        return (WaitForSingleObject(FHandle, ATimeout) == WAIT_OBJECT_0);
                     }

   // �ź����� ANum
   bool              IncCount(long ANum = 1) const
                     {
                        return (ReleaseSemaphore(FHandle, ANum, NULL) != FALSE);
                     }

private:
   THandle           FHandle;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYLockRW - �����д���� */

class TKYLockRW
{
public:
   TKYLockRW();
   virtual ~TKYLockRW();

   // ������
   bool              LockRead();

   // д����
   bool              LockWrite();

   // ���Ŷ�����
   bool              TryLockRead();

   // ����д����
   bool              TryLockWrite();

   // ������
   void              UnlockRead();

   // д����
   void              UnlockWrite();

private:
   inline void       SetReadSignal();
   inline void       SetWriteSignal();

private:
   TKYCritSect       FRWLock;
   TKYEvent          FReader;
   TKYEvent          FWriter;

   long              FReadingCount;
   long              FWritingCount;
   long              FWaitingReadCount;
   long              FWaitingWriteCount;
};

}

#endif
