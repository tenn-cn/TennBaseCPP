// =======================================
// Unit   : 内存块单元 (KYMemBlock.h)
// Version: 3.0.0.0 (build 2012.06.02)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYMemBlock_H_
#define _KYMemBlock_H_

#include "KYList.h"
#include "KYAVLTreeC.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYFmtMemEvent - 格式化内存项事件类 */

class TKYFmtMemEvent
{
public:
   // TOnFormat 事件类型
   typedef void (TObject::*TDoFormat)(void* AItem, Word ASize);
   typedef struct
   {
      TDoFormat      Method;
      void*          Object;
   } TOnFormat;

public:
   TKYFmtMemEvent()                    { Clear(); }
   ~TKYFmtMemEvent()                   { Clear(); }

   // 清除
   void              Clear();

   // 执行 OnInitialize 事件
   void              DoInitialize(void* AItem, Word ASize)
                     {
                        if (OnInitialize.Method != NULL)
                           ((TObject*)OnInitialize.Object->*OnInitialize.Method)(AItem, ASize);
                     }

   // 执行 OnFinalize 事件
   void              DoFinalize(void* AItem, Word ASize)
                     {
                        if (OnFinalize.Method != NULL)
                           ((TObject*)OnFinalize.Object->*OnFinalize.Method)(AItem, ASize);
                     }

   // 事件
   TOnFormat         OnInitialize;
   TOnFormat         OnFinalize;

protected:
private:
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYFormatBlock - 内存块格式化类 */

class TKYFormatBlock
{
   // 内部方法
   typedef void     (TKYFormatBlock::*TDoInitQueue)();
   typedef Longword (TKYFormatBlock::*TDoNewHandle)();
   typedef void     (TKYFormatBlock::*TDoDeleteHandle)(Longword AHandle);

public:
   // TOnFormatProc 事件
   typedef void (*TFormatProc)(void* AItem, Word ASize);

public:
   TKYFormatBlock(Word AHeadSize = 0, Word AItemSize = 0,
                  long ACapacity = Default_Capacity);
   virtual ~TKYFormatBlock();

   // 取句柄对应的项指针
   void*             operator[](Longword AHandle) const { return Item(AHandle); }
   void*             Item(Longword AHandle) const;

   // 属性
   void*             Head() const         { return FHead; }
   long              Count() const        { return FCount; }      // default: 0
   long              Capacity() const     { return FCapacity; }   // default: Default_Capacity
   Word              HeadSize() const     { return FHeadSize; }   // default: 0
   Word              ItemSize() const     { return FItemSize; }   // default: 0

   char*             MinAddress() const   { return FMinAddress; }
   char*             MaxAddress() const   { return FMinAddress + FItemSize * (FCapacity - 1); }

   // 判断项指针是否存在
   bool              IsExist(const void* AItem) const;

   // 判断句柄是否存在
   bool              IsExist(Longword AHandle) const;

   // 清空所有项
   void              Clear();

   // 新建项指针
   void*             NewItem();

   // 删除项指针
   bool              DeleteItem(void* AItem);

   // 新建句柄
   Longword          NewHandle();

   // 新建句柄并返回句柄对应的项指针
   Longword          NewHandle(Pointer& AItem);

   // 删除句柄
   bool              DeleteHandle(Longword AHandle);

   // 项指针转换成句柄
   Longword          Item2Handle(const void* AItem) const;

   // 内存格式化方法指针
   TFormatProc       DoInitialize;
   TFormatProc       DoFinalize;

   // 设置格式化事件
   TKYFmtMemEvent*   FormatEvent() const  { return FFormatEvent; }
   void              SetFormatEvent(TKYFmtMemEvent* AEvent);

protected:
   void*             _Block() const       { return FMapItems; }
   void              DoDeleteHandle(Longword AHandle);

private:
   void              SetHandleBit(Longword AHandle, bool AValue);

   void              DoInitQueueOfByte();
   Longword          DoNewHandleOfByte();
   void              DoDeleteHandleOfByte(Longword AHandle);

   void              DoInitQueueOfWord();
   Longword          DoNewHandleOfWord();
   void              DoDeleteHandleOfWord(Longword AHandle);

   void              DoInitQueueOfLongword();
   Longword          DoNewHandleOfLongword();
   void              DoDeleteHandleOfLongword(Longword AHandle);

private:
   Word              FHeadSize;
   Word              FItemSize;
   long              FCapacity;
   long              FCount;
   Longword          FBegin;
   Longword          FEnd;

   void*             FHead;
   void*             FMapItems;
   void*             FSpaceQueue;
   char*             FMinAddress;

   // 格式化事件
   TKYFmtMemEvent*   FFormatEvent;

   // 方法
   TDoInitQueue      FDoInitQueue;
   TDoNewHandle      FDoNewHandle;
   TDoDeleteHandle   FDoDeleteHandle;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMemHandles - 内存句柄集类 */

class TKYMemHandles
{
public:
   TKYMemHandles(bool ACanLock = false,
                 Word AItemSize = sizeof(void*),
                 long ACapacity = Default_Capacity,
                 long ADelta = Delta_Auto);
   virtual ~TKYMemHandles();

   // 取句柄对应的项指针
   void*             operator[](Longword AHandle) const { return Item(AHandle); }
   void*             Item(Longword AHandle) const;

   // 属性
   long              Capacity() const     { return FCapacity; }         // default: Default_Capacity
   long              Count() const        { return FCount; }            // default: 0
   long              Delta() const        { return FDelta; }            // default: Default_Capacity
   Word              ItemSize() const     { return FMain->ItemSize(); } // default: sizeof(void*)

   // 判断句柄是否存在
   bool              IsExist(Longword AHandle) const;

   // 新建句柄
   Longword          New();

   // 新建句柄并返回句柄对应的项指针
   Longword          New(Pointer& AItem);

   // 删除句柄
   void              Delete(Longword AHandle);

   // 清空所有项
   void              Clear();

   // 内存格式化方法指针
   void              SetDoInitialize(TKYFormatBlock::TFormatProc AProc);
   void              SetDoFinalize(TKYFormatBlock::TFormatProc AProc);

   // 设置格式化事件
   void              SetFormatEvent(TKYFmtMemEvent* AEvent);

protected:
   void              LockRead() const     { if (FLockRW != NULL) FLockRW->LockRead(); }
   void              UnlockRead() const   { if (FLockRW != NULL) FLockRW->UnlockRead(); }

   void              LockWrite() const    { if (FLockRW != NULL) FLockRW->LockWrite(); }
   void              UnlockWrite() const  { if (FLockRW != NULL) FLockRW->UnlockWrite(); }

private:
   void              DoDeletion(void* AItem);

   TKYFormatBlock*   Handle2Block(Longword AHandle, long& AIndex,
                                  Longword& AChild) const;

private:
   TKYLockRW*        FLockRW;
   TKYFormatBlock*   FMain;
   TKYList*          FList;
   long              FSpace;
   long              FCount;
   long              FDelta;
   long              FCapacity;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMemBlock - 内存控制块类 */

class TKYMemBlock
{
   // 内存索引项
   typedef struct
   {
      TKYFormatBlock*   Block;
      char*             MinAddr;
      char*             MaxAddr;
   } TMemIndex, *PMemIndex;

public:
   TKYMemBlock(bool ACanLock = false,
               Word AItemSize = sizeof(void*),
               long ACapacity = Default_Capacity,
               long ADelta = Delta_Auto);
   virtual ~TKYMemBlock();

   // 属性
   long              Capacity() const     { return FCapacity; }         // default: Default_Capacity
   long              Count() const        { return FCount; }            // default: 0
   long              Delta() const        { return FDelta; }            // default: Default_Capacity
   Word              ItemSize() const     { return FMain->ItemSize(); } // default: sizeof(void*)

   // 判断项指针是否存在
   bool              IsExist(const void* AItem) const;

   // 新建项指针
   void*             New();

   // 删除项指针
   void              Delete(void* AItem);

   // 清空所有项
   void              Clear();

   // 内存格式化方法指针
   void              SetDoInitialize(TKYFormatBlock::TFormatProc AProc);
   void              SetDoFinalize(TKYFormatBlock::TFormatProc AProc);

   // 设置格式化事件
   void              SetFormatEvent(TKYFmtMemEvent* AEvent);

protected:
   void              Lock() const         { if (FLock != NULL) FLock->Enter(); }
   void              Unlock() const       { if (FLock != NULL) FLock->Leave(); }

private:
   TKYFormatBlock*   Item2Block(const void* AItem, PKYAVLNode& ANode) const;

private:
   TKYCritSect*      FLock;
   TKYFormatBlock*   FMain;
   void*             FList;
   void*             FSpace;
   long              FCount;
   long              FDelta;
   long              FCapacity;

private:
   static long       DoCompare(const TKYAVLNode* ANode1, const TKYAVLNode* ANode2);
   static void       DoDeletion(void* ATree, TKYAVLNode* ANode);
};

}

#endif