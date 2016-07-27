// =======================================
// Unit   : 固定尺寸的内存块缓冲
// Version: 3.0.0.0 (build 2014.10.21)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYCache_H_
#define _KYCache_H_

#include "KYObject.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYCache - 固定尺寸的内存块缓冲类 */

// 注:
// 1. 为了多线程存取安全, New 和 Delete 分属二个线程时可以同时操作而不需要加锁,
//    但多线程 New 时必须用锁控制, 多线程 Delete 时必须用锁控制!
// 2. 此缓冲类一般应用于线性操作的类中, 以减少频繁分配和释放内存的缓冲使用.

class TKYCache
{
private:
   // 内存块的链接
   typedef struct
   {
      void*       Self;                // 内存块所属对象
      void*       Next;                // 下一块
   } TLink, *PLink;

public:
   // 构造函数
   // 1. ABlockSize  内存块的固定尺寸, 取值范围: [0x40..0x40000000]
   // 2. AMaxCount   内存块缓冲的最大个数
   TKYCache(long ABlockSize = 1024, long AMaxCount = 256);
   virtual ~TKYCache();

   // 属性
   long           Count() const        { return FPushCount - FPopCount; }
   long           MaxCount() const     { return FMaxCount; }   // default: AMaxCount
   long           BlockSize() const    { return FBlockSize; }  // default: ABlockSize

   // 设置内存块缓冲的最大个数
   void           SetMaxCount(long AMaxCount)
                  { FMaxCount = (AMaxCount >= 0) ? AMaxCount : 0; }

   // 分配固定尺寸的内存块
   void*          New()
                  {
                     TLink* pItem = DoNew();
                     return (pItem != NULL) ? (char*)pItem + sizeof(TLink) : NULL;
                  }

   // 释放固定尺寸的内存块
   void           Delete(void* ABlock)
                  {
                     if (ABlock != NULL)
                     {
                        TLink* pItem = (TLink*)((char*)ABlock - sizeof(TLink));
                        if (pItem->Self == this)
                           DoDelete(pItem);
                     }
                  }

private:
   // 执行分配/释放带链接的内存块
   TLink*         DoNew();
   void           DoDelete(TLink* ALink);

   // 执行清除缓冲队列
   void           DoClear(TLink* AHead);

private:
   TLink*         FHead;               // 缓冲的头链接
   TLink*         FTail;               // 缓冲的尾链接
   long           FFlag;               // 缓冲队列标志
   long           FMaxCount;           // 缓冲最大个数
   long           FBlockSize;          // 内存块的尺寸
   Longword       FPushCount;          // 压入缓冲计数
   Longword       FPopCount;           // 弹出缓冲计数
};

}

#endif
