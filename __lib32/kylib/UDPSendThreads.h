// =======================================
// Unit   : UDP �����߳��� (UDPSendThreads.h)
// Version: 3.0.0.0 (build 2011.03.09)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _UDPSendThreads_H_
#define _UDPSendThreads_H_

#include "KYUDPObj.h"
#include "KYMemBlock.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TUDPSendThread - UDP �����߳��� */

class TUDPSendThread: public TThread
{
public:
   // TOnSend �����¼�
   typedef void (TObject::*TDoSend)(void* Sender, TKYCustomUDP* UDP);
   typedef struct
   {
      TDoSend     Method;
      void*       Object;
   } TOnSend;

public:
   TUDPSendThread();
   virtual ~TUDPSendThread();

   // ������
   void*          Data() const         { return FData; }          // default: NULL
   long           Count() const        { return FItems->Count(); }// default: 0

   // ��������
   void           SetData(void* AData) { FData = AData; }

   // �ر��߳�, ���� FreeUDPSendThread �ͷ��߳�
   void           Close();

   // ������з��;��
   void           Clear();

   // ��� UDP �������߳�, �����ط��;��
   long           Add(void* Sender, TKYCustomUDP* UDP);

   // ���÷��;���Ƿ���Ҫ��������
   bool           Change(long ASendID, bool ANeedSend);

   // ɾ�����;��
   void           Delete(long ASendID);

   // �¼�
   TOnSend        OnSend;              // �� Change(.., true) ʱ�������¼�

protected:
   virtual void   Execute();

   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

private:
   bool           SendNext(bool AIsFirst);
   bool           ExecSend();

   void           DoDeletion(long AHandle, void* AItem);

private:
   void*          FData;
   TKYCritSect*   FLock;
   TKYEvent*      FEvent;
   TKYHandles*    FItems;

   Longword       FIdleTicks;
   Longword       FIdleTimes;
   Longword       FSendTimes;

private:
   // UDP ������
   #pragma pack(push, 1)
   typedef struct
   {
      void*          Sender;
      TKYCustomUDP*  UDP;
      bool           NeedSend;
   } TUDPSendItem, *PUDPSendItem;
   #pragma pack(pop)

   // �����������, ��ŷ����� TUDPSendItem
   static TKYMemBlock*  Inner_ItemManager;

   // TUDPSendThread �ľ�̬��Ա��ʼ����
   static class TInitialization
   {
   public:
      TInitialization();
      ~TInitialization();
   } Inner_Initialization;

   friend class TInitialization;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TUDPSendThreads - UDP �����̼߳��� */

class TUDPSendThreads
{
public:
   // TOnAddThread �����¼�
   typedef void (TObject::*TDoAddThread)(void* Sender, TUDPSendThread* AThread);
   typedef struct
   {
      TDoAddThread   Method;
      void*          Object;
   } TOnAddThread;

public:
   TUDPSendThreads();
   ~TUDPSendThreads();

   // ������
   long              ThreadCount() const     { return FThreads->Count(); } // default: 0
   long              MaxThreadCount() const  { return FMaxThreadCount; }   // default: 1

   // ��������
   void              SetMaxThreadCount(long ACount);

   // ��� UDP �������߳�, �����ط����̼߳����;��
   TUDPSendThread*   AddThread(void* Sender, TKYCustomUDP* UDP, long& ASendID);

   // ������з����߳�
   void              ClearThreads();

   // �¼�
   TOnAddThread      OnAddThread;            // AddThread �������߳�ʱ�������¼�

protected:
   void              Lock() const            { FLock->Enter(); }
   void              Unlock() const          { FLock->Leave(); }

private:
   TKYCritSect*      FLock;
   TKYHandles*       FThreads;
   long              FMaxThreadCount;
   bool              FFreeing;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �������� */

// �رղ��ͷ� UDP �����߳�
void FreeUDPSendThread(TUDPSendThread* &AThread, Longword ATimeout = 15000);

}

#endif
