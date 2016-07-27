// =======================================
// Unit   : cycle queue(使用循环方式的队列)
// Version: 3.0.1.0 (build 2014.10.20)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYCycQueue_H_
#define _KYCycQueue_H_

#include "KYObject.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYCycQueue - 使用循环方式的队列类 */

// 注: 为了多线程存取安全, Push 和 Pop 分属二个线程时可以同时操作而不需要加锁,
//     但多线程 Push 时必须用锁控制, 多线程 Pop/Peek 时必须用锁控制!

class TKYCycQueue
{
private:
   // 循环索引
   typedef struct
   {
      Longword    PushCount;           // 压入队列计数
      Longword    PopCount;            // 弹出队列计数
      void*       Next;                // 下一项
   } TIndex, *PIndex;

public:
   // 循环索引的尺寸级别
   enum TLevel   {slPower6    = 0,     // 2^6  项: 64
                  slPower7    = 1,     // 2^7  项: 128
                  slPower8    = 2,     // 2^8  项: 256
                  slPower9    = 3,     // 2^9  项: 512
                  slPower10   = 4,     // 2^10 项: 1024
                  slPower11   = 5,     // 2^11 项: 2048
                  slPower12   = 6,     // 2^12 项: 4096
                  slPower13   = 7,     // 2^13 项: 8192
                  slPower14   = 8,     // 2^14 项: 16384
                  slPower15   = 9,     // 2^15 项: 32768
                  slPower16   = 10};   // 2^16 项: 65536

public:
   // 构造函数
   // 1. ACacheSize 循环索引的缓冲个数
   // 2. ASizeLevel 循环索引的尺寸级别, 既是每个循环索引的项数定义
   TKYCycQueue(Word ACacheSize = 64, Byte ASizeLevel = slPower10);
   virtual ~TKYCycQueue();

   // 属性
   Longword       Count() const        { return FPushCount - FPopCount; }
   Longword       MaxCount() const     { return FMaxCount; }            // default: 0xFFFFFFFF
   Longword       CycCount() const     { return FCycCount; }            // default: 2 ^ (ASizeLevel + 6)
   Longword       PopCount() const     { return FPopCount; }            // default: 0
   Longword       PushCount() const    { return FPushCount; }           // default: 0
   TLevel         SizeLevel() const    { return (TLevel)FSizeLevel; }   // default: ASizeLevel
   Word           CacheSize() const    { return FCacheSize; }           // default: ACacheSize
   Word           CacheCount() const   { return (Word)(FPushCache - FPopCache); }

   // 设置队列的最大项数
   void           SetMaxCount(Longword AMaxCount)
                  { FMaxCount = AMaxCount; }

   // 设置循环索引的缓冲个数
   void           SetCacheSize(Word ACacheSize)
                  { FCacheSize = ACacheSize; }

   // 压入项数据
   bool           Push(void* AData);

   // 预读取项数据
   bool           Peek(Pointer& AData) const;
   void*          Peek() const;

   // 弹出项数据
   bool           Pop(Pointer& AData);
   void*          Pop();

private:
   // 取索引的项数/指定项
   Longword       DoCount(TIndex* AIndex) const
                  { return AIndex->PushCount - AIndex->PopCount; }
   void*          DoItem(TIndex* AIndex, Longword ANo) const
                  { return (Byte*)AIndex + sizeof(TIndex) + sizeof(void*) * ANo; }

   // 从索引项中压入/弹出数据
   bool           DoPush(TIndex* AIndex, void* AData) const;
   bool           DoPeek(TIndex* AIndex, Pointer& AData) const;
   bool           DoPop(TIndex* AIndex, Pointer& AData) const;

   // 分配/释放索引
   TIndex*        DoNew();
   void           DoDelete(TIndex* AIndex);

   // 清除索引列表
   void           DoClear(TIndex* AHead);

private:
   TIndex*        FIndexHead;          // 索引头索引
   TIndex*        FIndexTail;          // 索引尾索引
   Longword       FPushCount;          // 压入队列计数
   Longword       FPopCount;           // 弹出队列计数
   Longword       FMaxCount;           // 队列的最大项数

   Byte           FSizeLevel;          // 循环索引的尺寸级别
   Word           FCacheSize;          // 循环索引的缓冲个数
   Longword       FCycCount;           // 循环索引的项数
   Longword       FCycMask;            // 循环索引的掩码

   long           FCacheFlag;          // 缓冲队列标志
   TIndex*        FCacheHead;          // 缓冲头索引
   TIndex*        FCacheTail;          // 缓冲尾索引
   Longword       FPushCache;          // 压入缓冲计数
   Longword       FPopCache;           // 弹出缓冲计数
};

}

#endif
