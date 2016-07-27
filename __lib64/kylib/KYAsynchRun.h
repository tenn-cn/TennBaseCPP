// =======================================
// Unit   : Asynchronous Run Methods
// Version: 3.0.1.0 (build 2013.05.31)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYAsynchRun_H_
#define _KYAsynchRun_H_

#include "KYThread.h"
#include "KYSyncObj.h"
#include "KYCycQueue.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYAsynchRun - �첽ִ���� */

class TKYAsynchRun: public TThread
{
public:
   // ��������
   // ע: void* ����Ϊָ������, ���Դ���ָ��, ��: �ṹָ��, ����ָ��ȵ�
   //           Ҳ���Դ��ݳߴ粻���� sizeof(void*) ������
   typedef void (TObject::*TDoCustom)(void* Sender);
   typedef void (TObject::*TDoParam1)(void* Sender, void* P1);
   typedef void (TObject::*TDoParam2)(void* Sender, void* P1, void* P2);
   typedef void (TObject::*TDoParam3)(void* Sender, void* P1, void* P2, void* P3);
   typedef void (TObject::*TDoParam4)(void* Sender, void* P1, void* P2, void* P3, void* P4);

private:
   // ��������
   enum TMethodType {mtCustom, mtParam1, mtParam2, mtParam3, mtParam4};

   // ������
   typedef struct
   {
      TMethodType Type;                // ��������
      TDoCustom   Method;              // ����ָ��
      void*       Object;              // �����Ķ���ָ��
      void*       Sender;              // �����߶���ָ��
      void*       Param1;              // ����1
      void*       Param2;              // ����2
      void*       Param3;              // ����3
      void*       Param4;              // ����4
   } TItem, *PItem;

public:
   TKYAsynchRun(long ACacheSize = Default_CacheSize);
   virtual ~TKYAsynchRun();

   // ִ���̶߳����еķ�������
   long           Count() const        { return FQueue->Count(); }   // default: 0

   // �ر��߳�, ���� FreeKYAsynchRun �ͷ��߳�
   void           Close();

   // ��������е�����ִ�з���
   void           Clear();

   // ��� TDoCustom ���͵ķ�����ִ���߳�
   bool           AddCustom(TDoCustom AMethod, void* AObject, void* Sender)
                  { return Add(mtCustom, AMethod, AObject,
                               Sender, NULL, NULL, NULL, NULL); }

   // ��� TDoParam1 ���͵ķ�����ִ���߳�
   bool           AddParam1(TDoParam1 AMethod, void* AObject, void* Sender,
                                               void* P1)
                  { return Add(mtParam1, (TDoCustom)AMethod, AObject,
                               Sender, P1, NULL, NULL, NULL); }

   // ��� TDoParam2 ���͵ķ�����ִ���߳�
   bool           AddParam2(TDoParam2 AMethod, void* AObject, void* Sender,
                                               void* P1, void* P2)
                  { return Add(mtParam2, (TDoCustom)AMethod, AObject,
                               Sender, P1, P2, NULL, NULL); }

   // ��� TDoParam3 ���͵ķ�����ִ���߳�
   bool           AddParam3(TDoParam3 AMethod, void* AObject, void* Sender,
                                               void* P1, void* P2, void* P3)
                  { return Add(mtParam3, (TDoCustom)AMethod, AObject,
                               Sender, P1, P2, P3, NULL); }

   // ��� TDoParam4 ���͵ķ�����ִ���߳�
   bool           AddParam4(TDoParam4 AMethod, void* AObject, void* Sender,
                                               void* P1, void* P2, void* P3, void* P4)
                  { return Add(mtParam4, (TDoCustom)AMethod, AObject,
                               Sender, P1, P2, P3, P4); }

protected:
   // ѹ����
   void           LockPush() const     { FLockPush->Enter(); }
   void           UnlockPush() const   { FLockPush->Leave(); }

   // ������
   void           LockPop() const      { FLockPop->Enter(); }
   void           UnlockPop() const    { FLockPop->Leave(); }

   // �߳�ִ����
   virtual void   Execute();

private:
   bool           Add(TMethodType AType, TDoCustom AMethod, void* AObject,
                            void* Sender, void* P1, void* P2, void* P3, void* P4);

   TItem*         New();
   void           Dispose(TItem* AItem);

private:
   TKYCritSect*   FLockPush;           // ѹ����
   TKYCritSect*   FLockPop;            // ������
   TKYEvent*      FNotify;             // ֪ͨ
   TKYCycQueue*   FQueue;              // ��Ϣ����
   TKYCycQueue*   FCache;              // ���ж���
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �������� */

// �رղ��ͷ��߳�
void FreeKYAsynchRun(TKYAsynchRun* &AThread, Longword ATimeout = 5000);

}

#endif
