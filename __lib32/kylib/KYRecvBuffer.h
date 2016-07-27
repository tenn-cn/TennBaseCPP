// =======================================
// Unit   : 接收缓冲区单元
// Version: 3.0.1.0 (build 2013.06.03)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYRecvBuffer_H_
#define _KYRecvBuffer_H_

#include "KYString.h"
#include "KYSyncObj.h"
#include "KYCycQueue.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYRecvBuffer - 接收缓冲区类 */

class TKYRecvBuffer
{
public:
   // 构造函数
   // 1. ACanLock    是否内部加锁
   // 2. ACacheSize  缓冲区块池尺寸, 可以减少频繁分配内存
   TKYRecvBuffer(bool ACanLock = true, Word ACacheSize = 64);
   virtual ~TKYRecvBuffer();

   // 当前缓冲区数据尺寸, 默认值为: 0
   long           Size() const         { return FSize; }

   // 数据加入缓冲区
   long           Push(const void* AData, long ASize);
   long           Push(const KYString& AText)
                  { return Push((const char*)AText, AText.Length()); }

   // 清除缓冲区
   void           Clear();

   // 从缓冲区中接收数据, 返回接收的数据尺寸
   long           Recv(void* AData, long ASize);

   // 从缓冲区中接收指定尺寸的数据
   KYString       Recv(long ASize);

   // 接收缓冲区中的所有数据
   KYString       Recv();

protected:
   void           Lock() const         { if (FLock != NULL) FLock->Enter(); }
   void           Unlock() const       { if (FLock != NULL) FLock->Leave(); }

   // 执行接收(注: AData 非空, 且 (ASize > 0) && (ASize <= FSize))
   void           DoRecv(void* AData, long ASize);
   void           AdjustPos();

private:
   void*          NewBlock();
   void           DeleteBlock(void* ABlock);

private:
   TKYCritSect*   FLock;               // 锁
   TKYCycQueue*   FCache;              // 缓冲区块池
   TKYCycQueue*   FBlocks;             // 缓冲区块, 每块尺寸为 1 Kb
   void*          FLastBlock;          // 最后一块缓冲区
   long           FBegin;              // 开始位置
   long           FEnd;                // 结束位置
   long           FSize;               // 当前缓冲区数据尺寸
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYRecvCache - 接收高速缓存类: 若满则删除最前面数据 */

class TKYRecvCache
{
public:
   // 构造函数
   // 1. AMaxSize    接收缓冲区的最大尺寸
   // 2. ACanLock    是否内部加锁
   TKYRecvCache(long AMaxSize = 256, bool ACanLock = false);
   virtual ~TKYRecvCache();

   // 属性
   long           MaxSize() const      { return FMaxSize; } // default: 256
   long           Size() const         { return FSize; }    // default: 0

   // 数据加入缓冲区
   // 若数据溢出则自动丢弃最先加入的数据
   long           Push(const void* AData, long ASize);
   long           Push(const KYString& AText)
                  { return Push((const char*)AText, AText.Length()); }

   // 清除缓冲区
   void           Clear();

   // 从缓冲区中接收数据, 返回接收的数据尺寸
   long           Recv(void* AData, long ASize);

   // 从缓冲区中接收指定尺寸的数据
   KYString       Recv(long ASize);

   // 接收缓冲区中的所有数据
   KYString       Recv()               { return Recv(FMaxSize); }

protected:
   void           Lock() const         { if (FLock != NULL) FLock->Enter(); }
   void           Unlock() const       { if (FLock != NULL) FLock->Leave(); }

   // 执行接收(注: AData 非空, 且 (ASize > 0) && (ASize <= FSize))
   void           DoRecv(void* AData, long ASize);

private:
   TKYCritSect*   FLock;               // 锁
   char*          FBlock;              // 缓冲区块
   Longword       FMaxSize;            // 缓冲区的最大尺寸
   Longword       FSize;               // 当前缓冲区数据尺寸
   Longword       FBegin;              // 开始位置
   Longword       FEnd;                // 结束位置
};

}

#endif
