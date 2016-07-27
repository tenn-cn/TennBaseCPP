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

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYChain - 数据链类 */

class TKYChain
{
protected:
   // 链接数据项结构
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

   // 得到空闲项数
   unsigned long  IdleCount() const;

   // 属性
   unsigned long  Capacity() const     { return FCapacity; }   // default: 64
   long           Delta() const        { return FDelta; }      // default: Delta_Auto

   // 设置容量增长幅度
   void           SetDelta(long ADelta);

   // 压入项数据
   virtual bool   Push(void* AData) = 0;

   // 预读取项数据
   virtual void*  Peek() const = 0;

   // 弹出项数据
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
   bool           FIsDisposing;        // 是否正在 Dispose 项

   TKYCritSect*   FPushLock;           // Push 锁
   TKYCritSect*   FPopLock;            // Pop 锁

   unsigned long  FCapacity;           // 容量
   long           FDelta;              // 容量增长幅度

   unsigned long  FHead;               // 头索引号
   unsigned long  FTail;               // 尾索引号

   void*          FMemHead;            // 内存块的栈指针
   PLinkItem      FLinkHead;           // 空闲队列头
   PLinkItem      FLinkTail;           // 空闲队列尾
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYQueue - 队列类 */

class TKYQueue: public TKYChain
{
public:
   TKYQueue(bool APushLock = false, bool APopLock = false,
                           unsigned long ACapacity = 64,
                                    long ADelta = Delta_Auto);

   // 返回队列的项数
   unsigned long  Count() const;

   // 压入项数据
   virtual bool   Push(void* AData);

   // 预读取项数据
   virtual void*  Peek() const;

   // 弹出项数据
   virtual void*  Pop();

private:
   bool           FIsPushing;          // 是否正在 Push 项

   unsigned long  FHead;               // 头索引号
   unsigned long  FTail;               // 尾索引号

   PLinkItem      FLinkHead;           // 队列头
   PLinkItem      FLinkTail;           // 队列尾
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYStack - 栈类 */

class TKYStack: public TKYChain
{
public:
   TKYStack(bool APushLock = false, bool APopLock = false,
                           unsigned long ACapacity = 64,
                                    long ADelta = Delta_Auto);

   // 返回栈的项数
   unsigned long  Count() const;

   // 压入项数据
   virtual bool   Push(void* AData);

   // 预读取项数据
   virtual void*  Peek() const;

   // 弹出项数据
   virtual void*  Pop();

private:
   bool           FIsPushing;          // 是否正在 Push 项

   unsigned long  FHead;               // 头索引号
   unsigned long  FTail;               // 尾索引号

   PLinkItem      FLinkTop;            // 栈顶
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYQueueEx - 队列扩展类 */

class TKYQueueEx: public TKYChain
{
public:
   TKYQueueEx(bool APushLock = false, bool APopLock = false,
                             unsigned long ACapacity = 64,
                                      long ADelta = Delta_Auto);

   // 返回队列的项数
   unsigned long  Count() const;

   // 压入项数据
   virtual bool   Push(void* AData);

   // 预读取项数据
   virtual void*  Peek() const;

   // 弹出项数据
   virtual void*  Pop();

   // 预读第一项
   void*          First();

   // 预读下一项
   void*          Next();

   // 预读指定深度的项
   void*          Seek(long ADeep) const;

   // 摘取指定项数
   long           Drop(void** AItems, long ACount);

private:
   bool           FIsPushing;          // 是否正在 Push 项

   unsigned long  FHead;               // 头索引号
   unsigned long  FTail;               // 尾索引号

   PLinkItem      FLinkHead;           // 队列头
   PLinkItem      FLinkTail;           // 队列尾
   PLinkItem      FLinkCurr;           // 当前项
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYStackEx - 栈扩展类 */

class TKYStackEx: public TKYChain
{
public:
   TKYStackEx(bool APushLock = false, bool APopLock = false,
                             unsigned long ACapacity = 64,
                                      long ADelta = Delta_Auto);

   // 返回栈的项数
   unsigned long  Count() const;

   // 压入项数据
   virtual bool   Push(void* AData);

   // 预读取项数据
   virtual void*  Peek() const;

   // 弹出项数据
   virtual void*  Pop();

   // 预读第一项
   void*          First();

   // 预读下一项
   void*          Next();

   // 预读指定深度的项
   void*          Seek(long ADeep) const;

   // 摘取指定项数
   long           Drop(void** AItems, long ACount);

private:
   bool           FIsPushing;          // 是否正在 Push 项

   unsigned long  FHead;               // 头索引号
   unsigned long  FTail;               // 尾索引号

   PLinkItem      FLinkTop;            // 栈顶
   PLinkItem      FLinkCurr;           // 当前项
};

}

#endif
