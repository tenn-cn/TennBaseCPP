// =======================================
// Unit   : 句柄集类 (KYHandles.h)
// Version: 3.0.0.0 (build 2011.03.08)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYHandles_H_
#define _KYHandles_H_

#include "KYObject.h"
#include "KYSyncObj.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYHandles - 句柄集类 */

class TKYHandles
{
private:
   // 句柄项结构, 句柄 = 1..0x8000000, 0 表示空句柄
   #pragma pack(push, 1)
   typedef struct
   {
      long        Prior;               // 上一句柄
      long        Next;                // 下一句柄
      void*       Item;                // 项数据指针
      bool        IsExist;             // 是否存在
   } THandleItem, *PHandleItem;
   #pragma pack(pop)

public:
   // TOnDeletion 事件
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

   // 取句柄对应的项
   void* operator[](long AHandle) const{ return Item(AHandle); }

   // 属性
   long           Capacity() const     { return FCapacity; }   // default: 256
   long           Count() const        { return FCount; }      // default: 0
   long           Delta() const        { return FDelta; }      // default: Delta_Auto

   // 根据索引读取句柄, AIndex 取值范围: [0..Count()-1], 因要循环查找所以不建议使用
   long           Handles(long AIndex) const;

   // 判断句柄是否存在
   bool           IsExist(long AHandle) const;

   // 取句柄对应的项, 若句柄不存在则返回 NULL
   void*          Item(long AHandle) const;

   // 设置句柄容量的自动增量
   void           SetDelta(long ADelta);

   // 设置句柄对应的项
   void           SetItem(long AHandle, void* AItem);

   // 更改容量, 若存在句柄大于更改的容量值则自动释放句柄, 并激发 OnDeletion 事件
   void           ChangeCapacity(long ACapacity);

   // 新建句柄
   // 1. 若容量已经满时且 Delta() != Delta_None, 则自动扩展容量并分配句柄
   // 2. 若失败则返回值为 0, 否则返回分配的句柄
   long           New();

   // 新建指定的句柄
   // 1. 若 AHandle <= 0 或 AHandle > Capacity() 则返回 0, 分配句柄失败
   // 2. 若 AHandle 已存在则返回 0, 分配句柄失败, 否则返回 AHandle, 分配句柄成功
   long           New(long AHandle);

   // 删除指定句柄, 若句柄存在则激发 OnDeletion 事件
   void           Delete(long AHandle);

   // 清除所有句柄, 激发 OnDeletion 事件
   void           Clear();

   // 当前句柄的第一个句柄
   long           First();

   // 当前句柄的上一个句柄
   long           Prior();

   // 当前句柄的下一个句柄
   long           Next();

   // 当前句柄的最后一个句柄
   long           Last();

   // 指定句柄的上一个句柄(若AHandle == 0, 则取最后一个句柄)
   long           Prior(long AHandle) const;

   // 指定句柄的下一个句柄(若AHandle == 0, 则取第一个句柄)
   long           Next(long AHandle) const;

   // 事件
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
   TKYCritSect*   FLock;               // 锁当前句柄
   TKYLockRW*     FLockRW;             // 读写锁

   THandleItem*   FHandles;            // 句柄列表
   long           FDelta;              // 句柄容量的自动增量
   long           FCount;              // 句柄个数
   long           FCapacity;           // 句柄列表容量

   long           FHead;               // 首句柄
   long           FTail;               // 尾句柄
   long           FCurr;               // 当前句柄

   long           FSpaceHead;          // 空闲首句柄
   long           FSpaceTail;          // 空闲尾句柄
};

}

#endif
