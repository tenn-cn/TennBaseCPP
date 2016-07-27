// =======================================
// Unit   : Debug client object (.h)
// Version: 3.0.0.0 (build 2011.03.13)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYDebugObj_H_
#define _KYDebugObj_H_

#include "KYThread.h"
#include "KYSeqObj.h"
#include "KYMemBlock.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYDebugClient - Debug �Ŀͻ����߳��� */

class TKYDebugClient: public TThread
{
   // Debug ͷ��Ϣ
   typedef struct
   {
      int         Size;                // ͷ�ߴ�
      Longword    LastTick;            // Tick
      Longword    QueueSize;           // ���гߴ�
      Longword    DebugIndex;          // ����
   } TDebugHead, *PDebugHead;

   // Debug ��
   typedef char   TDebugItem[256];

public:
   TKYDebugClient(const KYString& AName = "");
   virtual ~TKYDebugClient();

   // �ж� Debug ������Ƿ��Ѿ���
   bool           Opened() const       { return FOpened; }

   // �ر��߳�, ���� FreeKYDebugClient �ͷ��߳�
   void           Close();

   // д Debug ��Ϣ
   bool           Write(const KYString& AInfo);

protected:
   virtual void   Execute();

private:
   void           ClearQueue();

   void           OpenDebug();
   void           CloseDebug();
   void           CheckDebug();
   void           WriteDebug(long ACount);

private:
   // Client �� Debug ��
   TDebugHead*    FHead;
   TDebugItem*    FItems;
   TKYEvent*      FEvent;
   TKYMutex*      FMutex;
   THandle        FHandle;
   KYString       FMapName;
   long           FMaxCount;

   // Debug ����
   TKYQueue*      FQueue;
   TKYEvent*      FNotify;
   bool           FOpened;

private:
   // Debug �������, ��� Debug �� TDebugItem
   static TKYMemBlock*  Inner_ItemManager;

   // TKYDebugClient �ľ�̬��Ա��ʼ����
   static class TInitialization
   {
   public:
      TInitialization();
      ~TInitialization();
   } Inner_Initialization;

   friend class TInitialization;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �������� */

// �رղ��ͷ� Debug �ͻ����߳�
void FreeKYDebugClient(TKYDebugClient* &AThread, Longword ATimeout = 5000);

}

#endif
