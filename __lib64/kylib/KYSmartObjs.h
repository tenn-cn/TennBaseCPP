// =======================================
// Unit   : smart objects(智能对象集)
// Version: 3.0.0.0 (build 2013.06.24)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYSmartObjs_H_
#define _KYSmartObjs_H_

#include "KYList.h"
#include "KYMapObjs.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYSmartObjs - 智能对象集类模板 */

template <class T>
class TKYSmartObjs
{
private:
   // 对象项
   typedef struct
   {
      long        Count;               // 对象引用次数
      T           Object;              // 对象
   } TItem, *PItem;

   // 映射列表类型
   typedef TKYMapObjKey<TItem>   TItems;

public:
   // 构造/析构函数
   TKYSmartObjs();
   virtual ~TKYSmartObjs();

   // 属性
   void*          Data() const         { return FItems->Data(); }    // default: NULL
   long           Count() const        { return FItems->Count(); }   // default: 0

   // 设置属性
   void           SetData(void* AData) { FItems->SetData(AData); }

   // 清除所有对象
   void           Clear();

   // 创建/释放对象
   T*             CreateObj();
   void           FreeObj(T* AObj);

   // 对象引用计数增 1, 若返回值为 true 则引用成功
   bool           IncRefObj(T* AObj);

   // 对象引用计数增/减 1, 注: 对象必须存在
   static void    _IncRefObj(T* AObj);
   static void    _DecRefObj(T* AObj);

protected:
   // 当前锁
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

private:
   // 列表的 OnDeletion 事件方法
   void           DoDeletion(TItem* AItem);

private:
   TKYCritSect*   FLock;               // 当前锁
   TItems*        FItems;              // 对象项列表
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYAllotObjs - 分配对象集类模板 */

template <class T>
class TKYAllotObjs
{
private:
   // 对象项
   typedef struct
   {
      long        Count;               // 对象引用次数
      T           Object;              // 对象
   } TItem, *PItem;

public:
   // 构造/析构函数
   TKYAllotObjs(long AMaxCount = 64);
   virtual ~TKYAllotObjs();

   // 属性
   void*          Data() const         { return FData; }             // default: NULL
   long           Count() const        { return FItems->Count(); }   // default: 0
   long           MaxCount() const     { return FItems->Capacity(); }// default: AMaxCount

   // 设置属性
   void           SetData(void* AData) { FData = AData; }
   void           SetMaxCount(long AMaxCount);

   // 清除所有对象
   void           Clear();

   // 分配对象并引用计数增 1
   T*             IncRefObj();

   // 对象引用计数增/减 1, 注: 对象必须存在
   static void    _IncRefObj(T* AObj);
   static void    _DecRefObj(T* AObj);

protected:
   // 当前锁
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

private:
   // 列表的 OnDeletion 事件方法
   void           DoDeletion(TItem* AItem);

private:
   void*          FData;               // 自定义数据
   TKYCritSect*   FLock;               // 当前锁
   TKYList*       FItems;              // 对象项列表
};

// 分配对象集类
typedef TKYAllotObjs<TKYCritSect>      TKYLockObjs;
typedef TKYAllotObjs<TKYMutex>         TKYMutexObjs;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYSmartObjs - 智能对象集类模板 */

// ---------------- 构造函数和析构函数 ----------------
// 构造函数
template <class T>
TKYSmartObjs<T>::TKYSmartObjs()
{
   // 创建对象
   FLock    = new TKYCritSect;
   FItems   = new TItems(false, false);
}

// 析构函数
template <class T>
TKYSmartObjs<T>::~TKYSmartObjs()
{
   // 清除所有对象
   Clear();

   // 释放对象
   FreeAndNil(FLock);
   FreeAndNil(FItems);
}

// ---------------- 私有函数 ----------------
// 列表的 OnDeletion 事件方法
template <class T>
void TKYSmartObjs<T>::DoDeletion(TItem* AItem)
{
   if (InterlockedDecrement(&AItem->Count) == 0)
      delete AItem;
}

// ---------------- 公有函数 ----------------
// 清除所有对象
template <class T>
void TKYSmartObjs<T>::Clear()
{
   // 初始化
   TKYList  objList(false);
   bool     boolNext = false;

   // 拷贝并清除
   Lock();
   if (FItems->Count() > 0)
      try
      {
         // 分配容量
         objList.ChangeCapacity(FItems->Count());

         // 循环拷贝
         void* pNode = FItems->Next(NULL);
         while (pNode != NULL)
         {
            objList.Add(FItems->Value(pNode));
            pNode = FItems->Next(pNode);
         }

         // 清除
         FItems->Clear();
         boolNext = true;
      }
      catch (...) {}
   Unlock();

   // 判断是否继续
   if (boolNext)
   {
      // 设置 OnDeletion 事件方法
      objList.OnDeletion.Object  = this;
      objList.OnDeletion.Method  = (TKYList::TDoDeletion)&TKYSmartObjs<T>::DoDeletion;

      // 清除
      objList.Clear();
   }
}

// 创建对象
template <class T>
T* TKYSmartObjs<T>::CreateObj()
{
   // 初始化
   T*     result = NULL;
   TItem* pItem  = new TItem;

   // 判断是否分配成功
   if (pItem != NULL)
   {
      // 初始化引用计数
      pItem->Count = 1;

      // 加入列表
      Lock();
      if (FItems->Add(&pItem->Object, pItem) != NULL)
         result = &pItem->Object;
      Unlock();

      // 若失败则释放
      if (result == NULL)
         delete pItem;
   }

   // 返回结果
   return result;
}

// 释放对象
template <class T>
void TKYSmartObjs<T>::FreeObj(T* AObj)
{
   // 初始化
   TItem* pItem = NULL;

   // 查找
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

   // 若存在则引用计数减 1
   if ((pItem != NULL) && (InterlockedDecrement(&pItem->Count) == 0))
      delete pItem;
}

// 对象引用计数增 1, 若返回值为 true 则引用成功
template <class T>
bool TKYSmartObjs<T>::IncRefObj(T* AObj)
{
   // 初始化
   bool   result = false;
   TItem* pItem  = NULL;

   // 查找
   Lock();
   if (FItems->Find(AObj, pItem))
   {
      InterlockedIncrement(&pItem->Count);
      result = true;
   }
   Unlock();

   // 返回结果
   return result;
}

// 对象引用计数增 1, 注: 对象必须存在
template <class T>
void TKYSmartObjs<T>::_IncRefObj(T* AObj)
{
   TItem* pItem = (TItem*)((Byte*)AObj - __Offset__(TItem, Object));
   InterlockedIncrement(&pItem->Count);
}

// 对象引用计数减 1, 注: 对象必须存在
template <class T>
void TKYSmartObjs<T>::_DecRefObj(T* AObj)
{
   TItem* pItem = (TItem*)((Byte*)AObj - __Offset__(TItem, Object));
   if (InterlockedDecrement(&pItem->Count) == 0)
      delete pItem;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYAllotObjs - 分配对象集类模板 */

// ---------------- 构造函数和析构函数 ----------------
// 构造函数
template <class T>
TKYAllotObjs<T>::TKYAllotObjs(long AMaxCount)
{
   // 初始化
   FData    = NULL;

   // 创建对象
   FLock    = new TKYCritSect;
   FItems   = new TKYList(false);

   // 设置对象属性
   FItems->SetDelta(Delta_None);
   FItems->ChangeCapacity(AMaxCount);
}

// 析构函数
template <class T>
TKYAllotObjs<T>::~TKYAllotObjs()
{
   // 清除所有对象
   Clear();

   // 释放对象
   FreeAndNil(FLock);
   FreeAndNil(FItems);
}

// ---------------- 私有函数 ----------------
// 列表的 OnDeletion 事件方法
template <class T>
void TKYAllotObjs<T>::DoDeletion(TItem* AItem)
{
   if (InterlockedDecrement(&AItem->Count) == 0)
      delete AItem;
}

// ---------------- 公有函数 ----------------
// 设置对象最多个数
template <class T>
void TKYAllotObjs<T>::SetMaxCount(long AMaxCount)
{
   // 初始化
   TKYList  objList(false);
   bool     boolNext = false;

   // 校正值
   if (AMaxCount < 0)
      AMaxCount = 0;

   // 设置并拷贝删除
   Lock();
   if (FItems->Capacity() != AMaxCount)
   {
      // 判断个数是否超出容量
      if (FItems->Count() > AMaxCount)
         try
         {
            // 分配容量
            objList.ChangeCapacity(FItems->Count() - AMaxCount);
            boolNext = true;

            // 循环拷贝
            for (long intNo = AMaxCount; intNo < FItems->Count(); intNo++)
               objList.Add(FItems->Item(intNo));
         }
         catch (...) {}

      // 更改容量, 自动删除超出容量的项
      FItems->ChangeCapacity(AMaxCount);
   }
   Unlock();

   // 判断是否继续
   if (boolNext)
   {
      // 设置 OnDeletion 事件方法
      objList.OnDeletion.Object  = this;
      objList.OnDeletion.Method  = (TKYList::TDoDeletion)&TKYAllotObjs<T>::DoDeletion;

      // 清除
      objList.Clear();
   }
}

// 清除所有对象
template <class T>
void TKYAllotObjs<T>::Clear()
{
   // 初始化
   TKYList  objList(false);
   bool     boolNext = false;

   // 拷贝并清除
   Lock();
   if (FItems->Count() > 0)
      try
      {
         // 取最大值
         long intMax = FItems->Capacity();

         // 拷贝并清除
         objList.Assign(*FItems);
         FItems->Clear();
         FItems->ChangeCapacity(intMax);
         boolNext = true;
      }
      catch (...) {}
   Unlock();

   // 判断是否继续
   if (boolNext)
   {
      // 设置 OnDeletion 事件方法
      objList.OnDeletion.Object  = this;
      objList.OnDeletion.Method  = (TKYList::TDoDeletion)&TKYAllotObjs<T>::DoDeletion;

      // 清除
      objList.Clear();
   }
}

// 分配对象并引用计数增 1
template <class T>
T* TKYAllotObjs<T>::IncRefObj()
{
   // 初始化
   T* result = NULL;

   // 分配对象
   Lock();
   try
   {
      // 若容量未满则分配项
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
         // 初始化
         TItem* pItem;
         TItem* pMin    = (TItem*)FItems->Item(0);
         long   intMin  = pMin->Count;

         // 查找最小引用项
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

         // 返回结果
         InterlockedIncrement(&pMin->Count);
         result = &pMin->Object;
      }
   }
   catch (...) {}
   Unlock();

   // 返回结果
   return result;
}

// 对象引用计数增 1, 注: 对象必须存在
template <class T>
void TKYAllotObjs<T>::_IncRefObj(T* AObj)
{
   TItem* pItem = (TItem*)((Byte*)AObj - __Offset__(TItem, Object));
   InterlockedIncrement(&pItem->Count);
}

// 对象引用计数减 1, 注: 对象必须存在
template <class T>
void TKYAllotObjs<T>::_DecRefObj(T* AObj)
{
   TItem* pItem = (TItem*)((Byte*)AObj - __Offset__(TItem, Object));
   if (InterlockedDecrement(&pItem->Count) == 0)
      delete pItem;
}

}

#endif
