// =======================================
// Unit   : TCP �����߳��� (TCPSendThreads.h)
// Version: 3.0.0.0 (build 2011.03.08)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _TCPSendThreads_H_
#define _TCPSendThreads_H_

#include "KYSockObj.h"
#include "KYMemBlock.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TTCPSendThread - TCP �����߳��� */

class TTCPSendThread: public TThread
{
public:
   TTCPSendThread();
   virtual ~TTCPSendThread();

   // ������
   void*          Data() const         { return FData; }          // default: NULL
   long           Count() const        { return FItems->Count(); }// default: 0

   // ��������
   void           SetData(void* AData) { FData = AData; }

   // �ر��߳�, ���� FreeTCPSendThread �ͷ��߳�
   void           Close();

   // ������з��;��
   void           Clear();

   // ��� TCP �������߳�, �����ط��;��
   long           Add(void* Sender, TKYBaseSocket* TCP);

   // ���÷��;���Ƿ���Ҫ��������
   bool           Change(long ASendID, bool ANeedSend);

   // ɾ�����;��
   void           Delete(long ASendID);

   // �¼�
   TOnSocketEvent OnSend;              // �� Change(.., true) ʱ�������¼�

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
   // TCP ������
   #pragma pack(push, 1)
   typedef struct
   {
      void*          Sender;
      TKYBaseSocket* TCP;
      bool           NeedSend;
   } TSendItem, *PSendItem;
   #pragma pack(pop)

   // �����������, ��ŷ����� TSendItem
   static TKYMemBlock*  Inner_ItemManager;

   // TTCPSendThread �ľ�̬��Ա��ʼ����
   static class TInitialization
   {
   public:
      TInitialization();
      ~TInitialization();
   } Inner_Initialization;

   friend class TInitialization;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TTCPSendThreads - TCP �����̼߳��� */

class TTCPSendThreads
{
public:
   // TOnAddThread �����¼�
   typedef void (TObject::*TDoAddThread)(void* Sender, TTCPSendThread* AThread);
   typedef struct
   {
      TDoAddThread   Method;
      void*          Object;
   } TOnAddThread;

public:
   TTCPSendThreads();
   ~TTCPSendThreads();

   // ������
   long              ThreadCount() const     { return FThreads->Count(); } // default: 0
   long              MaxThreadCount() const  { return FMaxThreadCount; }   // default: 1

   // ��������
   void              SetMaxThreadCount(long ACount);

   // ��� TCP �������߳�, �����ط����̼߳����;��
   TTCPSendThread*   AddThread(void* Sender, TKYBaseSocket* TCP, long& ASendID);

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

// �رղ��ͷ� TCP �����߳�
void FreeTCPSendThread(TTCPSendThread* &AThread, Longword ATimeout = 15000);

}

#endif