// =======================================
// Unit   : ������� (KYHandles.h)
// Version: 3.0.0.0 (build 2011.03.08)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYHandles_H_
#define _KYHandles_H_

#include "KYObject.h"
#include "KYSyncObj.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYHandles - ������� */

class TKYHandles
{
private:
   // �����ṹ, ��� = 1..0x8000000, 0 ��ʾ�վ��
   #pragma pack(push, 1)
   typedef struct
   {
      long        Prior;               // ��һ���
      long        Next;                // ��һ���
      void*       Item;                // ������ָ��
      bool        IsExist;             // �Ƿ����
   } THandleItem, *PHandleItem;
   #pragma pack(pop)

public:
   // TOnDeletion �¼�
   typedef void (TObject::*TDoDeletion)(long AHandle, void* AItem);
   typedef struct
   {
      TDoDeletion Method;
      void*       Object;
   } TOnDeletion;

public:
   TKYHandles(bool ACanLock = false, long ACapacity = 256,
                                     long ADelta = Delta_Auto);
   virtual ~TKYHandles();

   // ȡ�����Ӧ����
   void* operator[](long AHandle) const{ return Item(AHandle); }

   // ����
   long           Capacity() const     { return FCapacity; }   // default: 256
   long           Count() const        { return FCount; }      // default: 0
   long           Delta() const        { return FDelta; }      // default: Delta_Auto

   // ����������ȡ���, AIndex ȡֵ��Χ: [0..Count()-1], ��Ҫѭ���������Բ�����ʹ��
   long           Handles(long AIndex) const;

   // �жϾ���Ƿ����
   bool           IsExist(long AHandle) const;

   // ȡ�����Ӧ����, ������������򷵻� NULL
   void*          Item(long AHandle) const;

   // ���þ���������Զ�����
   void           SetDelta(long ADelta);

   // ���þ����Ӧ����
   void           SetItem(long AHandle, void* AItem);

   // ��������, �����ھ�����ڸ��ĵ�����ֵ���Զ��ͷž��, ������ OnDeletion �¼�
   void           ChangeCapacity(long ACapacity);

   // �½����
   // 1. �������Ѿ���ʱ�� Delta() != Delta_None, ���Զ���չ������������
   // 2. ��ʧ���򷵻�ֵΪ 0, ���򷵻ط���ľ��
   long           New();

   // �½�ָ���ľ��
   // 1. �� AHandle <= 0 �� AHandle > Capacity() �򷵻� 0, ������ʧ��
   // 2. �� AHandle �Ѵ����򷵻� 0, ������ʧ��, ���򷵻� AHandle, �������ɹ�
   long           New(long AHandle);

   // ɾ��ָ�����, ����������򼤷� OnDeletion �¼�
   void           Delete(long AHandle);

   // ������о��, ���� OnDeletion �¼�
   void           Clear();

   // ��ǰ����ĵ�һ�����
   long           First();

   // ��ǰ�������һ�����
   long           Prior();

   // ��ǰ�������һ�����
   long           Next();

   // ��ǰ��������һ�����
   long           Last();

   // ָ���������һ�����(��AHandle == 0, ��ȡ���һ�����)
   long           Prior(long AHandle) const;

   // ָ���������һ�����(��AHandle == 0, ��ȡ��һ�����)
   long           Next(long AHandle) const;

   // �¼�
   TOnDeletion    OnDeletion;

protected:
   void           LockCurr() const     { if (FLock != NULL) { FLock->Enter(); FLockRW->LockRead(); }}
   void           UnlockCurr() const   { if (FLock != NULL) { FLockRW->UnlockRead(); FLock->Leave(); }}

   void           LockRead() const     { if (FLockRW != NULL) FLockRW->LockRead(); }
   void           UnlockRead() const   { if (FLockRW != NULL) FLockRW->UnlockRead(); }

   void           LockWrite() const    { if (FLockRW != NULL) FLockRW->LockWrite(); }
   void           UnlockWrite() const  { if (FLockRW != NULL) FLockRW->UnlockWrite(); }

   void           DoDeletion(long AHandle, void* AItem) const;

private:
   bool           Grow(long ASize);
   bool           Reduce(long ASize);

   void           ResetData();
   void           ClearHandles();
   void           FreeHandles(long AFrom);
   void           InitSpace(long AFrom);

   long           EmptyItems(long AFrom, long AMaxCount);
   long           EmptyItemsOfEvent(long AFrom, long AMaxCount);

private:
   TKYCritSect*   FLock;               // ����ǰ���
   TKYLockRW*     FLockRW;             // ��д��

   THandleItem*   FHandles;            // ����б�
   long           FDelta;              // ����������Զ�����
   long           FCount;              // �������
   long           FCapacity;           // ����б�����

   long           FHead;               // �׾��
   long           FTail;               // β���
   long           FCurr;               // ��ǰ���

   long           FSpaceHead;          // �����׾��
   long           FSpaceTail;          // ����β���
};

}

#endif
