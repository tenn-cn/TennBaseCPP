// =======================================
// Unit   : smart objects(���ܶ���)
// Version: 3.0.0.0 (build 2013.06.24)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYSmartObjs_H_
#define _KYSmartObjs_H_

#include "KYList.h"
#include "KYMapObjs.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYSmartObjs - ���ܶ�����ģ�� */

template <class T>
class TKYSmartObjs
{
private:
   // ������
   typedef struct
   {
      long        Count;               // �������ô���
      T           Object;              // ����
   } TItem, *PItem;

   // ӳ���б�����
   typedef TKYMapObjKey<TItem>   TItems;

public:
   // ����/��������
   TKYSmartObjs();
   virtual ~TKYSmartObjs();

   // ����
   void*          Data() const         { return FItems->Data(); }    // default: NULL
   long           Count() const        { return FItems->Count(); }   // default: 0

   // ��������
   void           SetData(void* AData) { FItems->SetData(AData); }

   // ������ж���
   void           Clear();

   // ����/�ͷŶ���
   T*             CreateObj();
   void           FreeObj(T* AObj);

   // �������ü����� 1, ������ֵΪ true �����óɹ�
   bool           IncRefObj(T* AObj);

   // �������ü�����/�� 1, ע: ����������
   static void    _IncRefObj(T* AObj);
   static void    _DecRefObj(T* AObj);

protected:
   // ��ǰ��
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

private:
   // �б�� OnDeletion �¼�����
   void           DoDeletion(TItem* AItem);

private:
   TKYCritSect*   FLock;               // ��ǰ��
   TItems*        FItems;              // �������б�
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYAllotObjs - ���������ģ�� */

template <class T>
class TKYAllotObjs
{
private:
   // ������
   typedef struct
   {
      long        Count;               // �������ô���
      T           Object;              // ����
   } TItem, *PItem;

public:
   // ����/��������
   TKYAllotObjs(long AMaxCount = 64);
   virtual ~TKYAllotObjs();

   // ����
   void*          Data() const         { return FData; }             // default: NULL
   long           Count() const        { return FItems->Count(); }   // default: 0
   long           MaxCount() const     { return FItems->Capacity(); }// default: AMaxCount

   // ��������
   void           SetData(void* AData) { FData = AData; }
   void           SetMaxCount(long AMaxCount);

   // ������ж���
   void           Clear();

   // ����������ü����� 1
   T*             IncRefObj();

   // �������ü�����/�� 1, ע: ����������
   static void    _IncRefObj(T* AObj);
   static void    _DecRefObj(T* AObj);

protected:
   // ��ǰ��
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

private:
   // �б�� OnDeletion �¼�����
   void           DoDeletion(TItem* AItem);

private:
   void*          FData;               // �Զ�������
   TKYCritSect*   FLock;               // ��ǰ��
   TKYList*       FItems;              // �������б�
};

// ���������
typedef TKYAllotObjs<TKYCritSect>      TKYLockObjs;
typedef TKYAllotObjs<TKYMutex>         TKYMutexObjs;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYSmartObjs - ���ܶ�����ģ�� */

// ---------------- ���캯������������ ----------------
// ���캯��
template <class T>
TKYSmartObjs<T>::TKYSmartObjs()
{
   // ��������
   FLock    = new TKYCritSect;
   FItems   = new TItems(false, false);
}

// ��������
template <class T>
TKYSmartObjs<T>::~TKYSmartObjs()
{
   // ������ж���
   Clear();

   // �ͷŶ���
   FreeAndNil(FLock);
   FreeAndNil(FItems);
}

// ---------------- ˽�к��� ----------------
// �б�� OnDeletion �¼�����
template <class T>
void TKYSmartObjs<T>::DoDeletion(TItem* AItem)
{
   if (InterlockedDecrement(&AItem->Count) == 0)
      delete AItem;
}

// ---------------- ���к��� ----------------
// ������ж���
template <class T>
void TKYSmartObjs<T>::Clear()
{
   // ��ʼ��
   TKYList  objList(false);
   bool     boolNext = false;

   // ���������
   Lock();
   if (FItems->Count() > 0)
      try
      {
         // ��������
         objList.ChangeCapacity(FItems->Count());

         // ѭ������
         void* pNode = FItems->Next(NULL);
         while (pNode != NULL)
         {
            objList.Add(FItems->Value(pNode));
            pNode = FItems->Next(pNode);
         }

         // ���
         FItems->Clear();
         boolNext = true;
      }
      catch (...) {}
   Unlock();

   // �ж��Ƿ����
   if (boolNext)
   {
      // ���� OnDeletion �¼�����
      objList.OnDeletion.Object  = this;
      objList.OnDeletion.Method  = (TKYList::TDoDeletion)&TKYSmartObjs<T>::DoDeletion;

      // ���
      objList.Clear();
   }
}

// ��������
template <class T>
T* TKYSmartObjs<T>::CreateObj()
{
   // ��ʼ��
   T*     result = NULL;
   TItem* pItem  = new TItem;

   // �ж��Ƿ����ɹ�
   if (pItem != NULL)
   {
      // ��ʼ�����ü���
      pItem->Count = 1;

      // �����б�
      Lock();
      if (FItems->Add(&pItem->Object, pItem) != NULL)
         result = &pItem->Object;
      Unlock();

      // ��ʧ�����ͷ�
      if (result == NULL)
         delete pItem;
   }

   // ���ؽ��
   return result;
}

// �ͷŶ���
template <class T>
void TKYSmartObjs<T>::FreeObj(T* AObj)
{
   // ��ʼ��
   TItem* pItem = NULL;

   // ����
   Lock();
   {
      void* pNode = FItems->Find(AObj);
      if (pNode != NULL)
      {
         pItem = FItems->Value(pNode);
         FItems->Remove(pNode);
      }
   }
   Unlock();

   // �����������ü����� 1
   if ((pItem != NULL) && (InterlockedDecrement(&pItem->Count) == 0))
      delete pItem;
}

// �������ü����� 1, ������ֵΪ true �����óɹ�
template <class T>
bool TKYSmartObjs<T>::IncRefObj(T* AObj)
{
   // ��ʼ��
   bool   result = false;
   TItem* pItem  = NULL;

   // ����
   Lock();
   if (FItems->Find(AObj, pItem))
   {
      InterlockedIncrement(&pItem->Count);
      result = true;
   }
   Unlock();

   // ���ؽ��
   return result;
}

// �������ü����� 1, ע: ����������
template <class T>
void TKYSmartObjs<T>::_IncRefObj(T* AObj)
{
   TItem* pItem = (TItem*)((Byte*)AObj - __Offset__(TItem, Object));
   InterlockedIncrement(&pItem->Count);
}

// �������ü����� 1, ע: ����������
template <class T>
void TKYSmartObjs<T>::_DecRefObj(T* AObj)
{
   TItem* pItem = (TItem*)((Byte*)AObj - __Offset__(TItem, Object));
   if (InterlockedDecrement(&pItem->Count) == 0)
      delete pItem;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYAllotObjs - ���������ģ�� */

// ---------------- ���캯������������ ----------------
// ���캯��
template <class T>
TKYAllotObjs<T>::TKYAllotObjs(long AMaxCount)
{
   // ��ʼ��
   FData    = NULL;

   // ��������
   FLock    = new TKYCritSect;
   FItems   = new TKYList(false);

   // ���ö�������
   FItems->SetDelta(Delta_None);
   FItems->ChangeCapacity(AMaxCount);
}

// ��������
template <class T>
TKYAllotObjs<T>::~TKYAllotObjs()
{
   // ������ж���
   Clear();

   // �ͷŶ���
   FreeAndNil(FLock);
   FreeAndNil(FItems);
}

// ---------------- ˽�к��� ----------------
// �б�� OnDeletion �¼�����
template <class T>
void TKYAllotObjs<T>::DoDeletion(TItem* AItem)
{
   if (InterlockedDecrement(&AItem->Count) == 0)
      delete AItem;
}

// ---------------- ���к��� ----------------
// ���ö���������
template <class T>
void TKYAllotObjs<T>::SetMaxCount(long AMaxCount)
{
   // ��ʼ��
   TKYList  objList(false);
   bool     boolNext = false;

   // У��ֵ
   if (AMaxCount < 0)
      AMaxCount = 0;

   // ���ò�����ɾ��
   Lock();
   if (FItems->Capacity() != AMaxCount)
   {
      // �жϸ����Ƿ񳬳�����
      if (FItems->Count() > AMaxCount)
         try
         {
            // ��������
            objList.ChangeCapacity(FItems->Count() - AMaxCount);
            boolNext = true;

            // ѭ������
            for (long intNo = AMaxCount; intNo < FItems->Count(); intNo++)
               objList.Add(FItems->Item(intNo));
         }
         catch (...) {}

      // ��������, �Զ�ɾ��������������
      FItems->ChangeCapacity(AMaxCount);
   }
   Unlock();

   // �ж��Ƿ����
   if (boolNext)
   {
      // ���� OnDeletion �¼�����
      objList.OnDeletion.Object  = this;
      objList.OnDeletion.Method  = (TKYList::TDoDeletion)&TKYAllotObjs<T>::DoDeletion;

      // ���
      objList.Clear();
   }
}

// ������ж���
template <class T>
void TKYAllotObjs<T>::Clear()
{
   // ��ʼ��
   TKYList  objList(false);
   bool     boolNext = false;

   // ���������
   Lock();
   if (FItems->Count() > 0)
      try
      {
         // ȡ���ֵ
         long intMax = FItems->Capacity();

         // ���������
         objList.Assign(*FItems);
         FItems->Clear();
         FItems->ChangeCapacity(intMax);
         boolNext = true;
      }
      catch (...) {}
   Unlock();

   // �ж��Ƿ����
   if (boolNext)
   {
      // ���� OnDeletion �¼�����
      objList.OnDeletion.Object  = this;
      objList.OnDeletion.Method  = (TKYList::TDoDeletion)&TKYAllotObjs<T>::DoDeletion;

      // ���
      objList.Clear();
   }
}

// ����������ü����� 1
template <class T>
T* TKYAllotObjs<T>::IncRefObj()
{
   // ��ʼ��
   T* result = NULL;

   // �������
   Lock();
   try
   {
      // ������δ���������
      if (FItems->Count() < FItems->Capacity())
      {
         TItem* pItem = new TItem;
         if (pItem != NULL)
         {
            pItem->Count = 1;
            if (FItems->Add(pItem) >= 0)
            {
               pItem->Count = 2;
               result       = &pItem->Object;
            }
            else
               delete pItem;
         }
      }
      else if (FItems->Count() != 0)
      {
         // ��ʼ��
         TItem* pItem;
         TItem* pMin    = (TItem*)FItems->Item(0);
         long   intMin  = pMin->Count;

         // ������С������
         if (intMin > 1)
            for (long intNo = 1; intNo < FItems->Count(); intNo++)
            {
               pItem = (TItem*)FItems->Item(intNo);
               if (pItem->Count < intMin)
               {
                  pMin   = pItem;
                  intMin = pItem->Count;
                  if (intMin == 1)
                     break;
               }
            }

         // ���ؽ��
         InterlockedIncrement(&pMin->Count);
         result = &pMin->Object;
      }
   }
   catch (...) {}
   Unlock();

   // ���ؽ��
   return result;
}

// �������ü����� 1, ע: ����������
template <class T>
void TKYAllotObjs<T>::_IncRefObj(T* AObj)
{
   TItem* pItem = (TItem*)((Byte*)AObj - __Offset__(TItem, Object));
   InterlockedIncrement(&pItem->Count);
}

// �������ü����� 1, ע: ����������
template <class T>
void TKYAllotObjs<T>::_DecRefObj(T* AObj)
{
   TItem* pItem = (TItem*)((Byte*)AObj - __Offset__(TItem, Object));
   if (InterlockedDecrement(&pItem->Count) == 0)
      delete pItem;
}

}

#endif
