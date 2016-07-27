// =======================================
// Unit   : Sequence object (KYSeqObj.h)
// Version: 3.0.0.0 (build 2011.03.09)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYSeqObj_H_
#define _KYSeqObj_H_

#include "KYSyncObj.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYChain - �������� */

class TKYChain
{
protected:
   // ����������ṹ
   typedef struct TLinkItem
   {
      void*       Data;
      TLinkItem*  Next;
   } *PLinkItem;

public:
   TKYChain(bool APushLock = false, bool APopLock = false,
                           unsigned long ACapacity = 64,
                                    long ADelta = Delta_Auto);
   virtual ~TKYChain();

   // �õ���������
   unsigned long  IdleCount() const;

   // ����
   unsigned long  Capacity() const     { return FCapacity; }   // default: 64
   long           Delta() const        { return FDelta; }      // default: Delta_Auto

   // ����������������
   void           SetDelta(long ADelta);

   // ѹ��������
   virtual bool   Push(void* AData) = 0;

   // Ԥ��ȡ������
   virtual void*  Peek() const = 0;

   // ����������
   virtual void*  Pop() = 0;

protected:
   void           PushLock() const     { if (FPushLock != NULL) FPushLock->Enter(); }
   void           PushUnlock() const   { if (FPushLock != NULL) FPushLock->Leave(); }

   void           PopLock() const      { if (FPopLock != NULL) FPopLock->Enter(); }
   void           PopUnlock() const    { if (FPopLock != NULL) FPopLock->Leave(); }

   void           InitCapacity(unsigned long ACapacity);

   PLinkItem      NewItem();
   void           DisposeItem(PLinkItem AItem);

private:
   bool           Grow(long ADelta);

private:
   bool           FIsDisposing;        // �Ƿ����� Dispose ��

   TKYCritSect*   FPushLock;           // Push ��
   TKYCritSect*   FPopLock;            // Pop ��

   unsigned long  FCapacity;           // ����
   long           FDelta;              // ������������

   unsigned long  FHead;               // ͷ������
   unsigned long  FTail;               // β������

   void*          FMemHead;            // �ڴ���ջָ��
   PLinkItem      FLinkHead;           // ���ж���ͷ
   PLinkItem      FLinkTail;           // ���ж���β
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYQueue - ������ */

class TKYQueue: public TKYChain
{
public:
   TKYQueue(bool APushLock = false, bool APopLock = false,
                           unsigned long ACapacity = 64,
                                    long ADelta = Delta_Auto);

   // ���ض��е�����
   unsigned long  Count() const;

   // ѹ��������
   virtual bool   Push(void* AData);

   // Ԥ��ȡ������
   virtual void*  Peek() const;

   // ����������
   virtual void*  Pop();

private:
   bool           FIsPushing;          // �Ƿ����� Push ��

   unsigned long  FHead;               // ͷ������
   unsigned long  FTail;               // β������

   PLinkItem      FLinkHead;           // ����ͷ
   PLinkItem      FLinkTail;           // ����β
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYStack - ջ�� */

class TKYStack: public TKYChain
{
public:
   TKYStack(bool APushLock = false, bool APopLock = false,
                           unsigned long ACapacity = 64,
                                    long ADelta = Delta_Auto);

   // ����ջ������
   unsigned long  Count() const;

   // ѹ��������
   virtual bool   Push(void* AData);

   // Ԥ��ȡ������
   virtual void*  Peek() const;

   // ����������
   virtual void*  Pop();

private:
   bool           FIsPushing;          // �Ƿ����� Push ��

   unsigned long  FHead;               // ͷ������
   unsigned long  FTail;               // β������

   PLinkItem      FLinkTop;            // ջ��
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYQueueEx - ������չ�� */

class TKYQueueEx: public TKYChain
{
public:
   TKYQueueEx(bool APushLock = false, bool APopLock = false,
                             unsigned long ACapacity = 64,
                                      long ADelta = Delta_Auto);

   // ���ض��е�����
   unsigned long  Count() const;

   // ѹ��������
   virtual bool   Push(void* AData);

   // Ԥ��ȡ������
   virtual void*  Peek() const;

   // ����������
   virtual void*  Pop();

   // Ԥ����һ��
   void*          First();

   // Ԥ����һ��
   void*          Next();

   // Ԥ��ָ����ȵ���
   void*          Seek(long ADeep) const;

   // ժȡָ������
   long           Drop(void** AItems, long ACount);

private:
   bool           FIsPushing;          // �Ƿ����� Push ��

   unsigned long  FHead;               // ͷ������
   unsigned long  FTail;               // β������

   PLinkItem      FLinkHead;           // ����ͷ
   PLinkItem      FLinkTail;           // ����β
   PLinkItem      FLinkCurr;           // ��ǰ��
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYStackEx - ջ��չ�� */

class TKYStackEx: public TKYChain
{
public:
   TKYStackEx(bool APushLock = false, bool APopLock = false,
                             unsigned long ACapacity = 64,
                                      long ADelta = Delta_Auto);

   // ����ջ������
   unsigned long  Count() const;

   // ѹ��������
   virtual bool   Push(void* AData);

   // Ԥ��ȡ������
   virtual void*  Peek() const;

   // ����������
   virtual void*  Pop();

   // Ԥ����һ��
   void*          First();

   // Ԥ����һ��
   void*          Next();

   // Ԥ��ָ����ȵ���
   void*          Seek(long ADeep) const;

   // ժȡָ������
   long           Drop(void** AItems, long ACount);

private:
   bool           FIsPushing;          // �Ƿ����� Push ��

   unsigned long  FHead;               // ͷ������
   unsigned long  FTail;               // β������

   PLinkItem      FLinkTop;            // ջ��
   PLinkItem      FLinkCurr;           // ��ǰ��
};

}

#endif
