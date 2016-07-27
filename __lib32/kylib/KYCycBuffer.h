// =======================================
// Unit   : cycle buffer(使用循环方式的缓冲区)
// Version: 3.0.1.0 (build 2014.10.20)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYCycBuffer_H_
#define _KYCycBuffer_H_

#include "KYString.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYCycBuffer - 使用循环方式的缓冲区类 */

// 注: 为了多线程存取安全, Push 和 Pop 分属二个线程时可以同时操作而不需要加锁,
//     但多线程 Push 时必须用锁控制, 多线程 Pop/Peek/Lose 时必须用锁控制!

class TKYCycBuffer
{
private:
   // 数据块的索引
   typedef struct
   {
      Longword    PushSize;            // 压入数据尺寸
      Longword    PopSize;             // 弹出数据尺寸
      void*       Next;                // 下一项
   } TIndex, *PIndex;

public:
   // 数据块的尺寸类型
   enum TType    {stSize2_8   = 0,     // 2^8  字节数: 256
                  stSize2_9   = 1,     // 2^9  字节数: 512
                  stSize2_10  = 2,     // 2^10 字节数: 1024
                  stSize2_11  = 3,     // 2^11 字节数: 2048
                  stSize2_12  = 4,     // 2^12 字节数: 4096
                  stSize2_13  = 5,     // 2^13 字节数: 8192
                  stSize2_14  = 6,     // 2^14 字节数: 16384
                  stSize2_15  = 7,     // 2^15 字节数: 32768
                  stSize2_16  = 8};    // 2^16 字节数: 65536

   // 读写数据的方法, 返回数据尺寸
   typedef long (TObject::*TDoRead)(void* AData, long ASize);
   typedef long (TObject::*TDoWrite)(const void* AData, long ASize);

   // 读写数据的 Push 和 Pop 方法类型
   // 注: 因为 Push 和 Pop 存在重载方法, 编译器无法指定哪个方法, 所以明确定义方
   // 法类型, 可以让编译器识别到具体哪个方法。
   typedef Longword (TKYCycBuffer::*TDoPop)(void* AData, Longword ASize);
   typedef Longword (TKYCycBuffer::*TDoPush)(const void* AData, Longword ASize);

public:
   // 构造函数
   // 1. AMaxCache   数据块的最大缓冲个数
   // 2. ASizeType   数据块的尺寸类型, 既是每个数据块的尺寸定义
   TKYCycBuffer(Word AMaxCache = 64, Byte ASizeType = stSize2_10);
   virtual ~TKYCycBuffer();

   // 属性
   Longword       Size() const         { return FPushSize - FPopSize; }
   Longword       MaxSize() const      { return FMaxSize; }             // default: 0xFFFFFFFF
   Longword       PopSize() const      { return FPopSize; }             // default: 0
   Longword       PushSize() const     { return FPushSize; }            // default: 0
   Longword       BlockSize() const    { return FBlockSize; }           // default: 2 ^ (ASizeType + 8)
   TType          SizeType() const     { return (TType)FSizeType; }     // default: ASizeType
   Word           MaxCache() const     { return FMaxCache; }            // default: AMaxCache
   Word           CacheCount() const   { return (Word)(FPushCache - FPopCache); }

   // 设置数据最大尺寸
   void           SetMaxSize(Longword AMaxSize)
                  { FMaxSize = AMaxSize; }

   // 设置数据块的最大缓冲个数
   void           SetMaxCache(Word AMaxCache)
                  { FMaxCache = AMaxCache; }

   // 数据加入缓冲区
   Longword       Push(TDoRead ADoRead, void* AObject, Longword ASize);
   Longword       Push(const void* AData, Longword ASize);

   Longword       Push(TKYCycBuffer& ABuffer, Longword ASize)
                  { return Push((TDoRead)(TDoPop)&TKYCycBuffer::Pop, &ABuffer, ASize); }
   Longword       Push(TKYCycBuffer& ABuffer)
                  { return Push(ABuffer, 0xFFFFFFFF); }
   Longword       Push(const KYString& AText)
                  { return Push((const char*)AText, AText.Length()); }

   // 从缓冲区中读取数据, 返回读取的数据尺寸
   Longword       Pop(TDoWrite ADoWrite, void* AObject, Longword ASize);
   Longword       Pop(void* AData, Longword ASize);

   Longword       Pop(TKYCycBuffer& ABuffer, Longword ASize)
                  { return Pop((TDoWrite)(TDoPush)&TKYCycBuffer::Push, &ABuffer, ASize); }
   Longword       Pop(TKYCycBuffer& ABuffer)
                  { return Pop(ABuffer, 0xFFFFFFFF); }

   // 从缓冲区中读取指定尺寸的数据(注: ASize > 0)
   KYString       Pop(long ASize);
   KYString       Pop()                { return Pop(0x7FFFFFFF); }

   // 从缓冲区中预读取数据, 返回预读取的数据尺寸
   Longword       Peek(TDoWrite ADoWrite, void* AObject, Longword ASize) const;
   Longword       Peek(void* AData, Longword ASize) const;

   Longword       Peek(TKYCycBuffer& ABuffer, Longword ASize) const
                  { return Peek((TDoWrite)(TDoPush)&TKYCycBuffer::Push, &ABuffer, ASize); }
   Longword       Peek(TKYCycBuffer& ABuffer) const
                  { return Peek(ABuffer, 0xFFFFFFFF); }

   // 从缓冲区中预读取指定尺寸的数据(注: ASize > 0)
   KYString       Peek(long ASize) const;
   KYString       Peek() const         { return Peek(0x7FFFFFFF); }

   // 从缓冲区中丢弃指定尺寸的数据, 返回丢弃的数据尺寸
   Longword       Lose(Longword ASize);
   Longword       Lose()               { return Lose(0xFFFFFFFF); }

private:
   // 分配/释放索引
   TIndex*        DoNew();
   void           DoDelete(TIndex* AIndex);

   // 清除索引列表
   void           DoClear(TIndex* AHead);

private:
   // 取数据块的尺寸/指定项
   Longword       DoSize(TIndex* AIndex) const
                  { return AIndex->PushSize - AIndex->PopSize; }
   void*          DoItem(TIndex* AIndex, Longword ANo) const
                  { return (Byte*)AIndex + sizeof(TIndex) + ANo; }

   // 从数据块中压入数据
   Longword       DoPush(TIndex* AIndex, TDoRead ADoRead, void* AObject, Longword ASize) const;
   Longword       DoPush(TIndex* AIndex, const void* AData, Longword ASize) const;

   // 从数据块中预读取数据
   Longword       DoPeek(TIndex* AIndex, TDoWrite ADoWrite, void* AObject, Longword ASize) const;
   Longword       DoPeek(TIndex* AIndex, void* AData, Longword ASize) const;

   // 从数据块中弹出数据
   Longword       DoPop(TIndex* AIndex, TDoWrite ADoWrite, void* AObject, Longword ASize) const;
   Longword       DoPop(TIndex* AIndex, void* AData, Longword ASize) const;

private:
   TIndex*        FBlockHead;          // 数据块的头索引
   TIndex*        FBlockTail;          // 数据块的尾索引
   Longword       FBlockSize;          // 数据块的尺寸
   Longword       FBlockMask;          // 数据块的掩码
   Longword       FPushSize;           // 压入数据尺寸
   Longword       FPopSize;            // 弹出数据尺寸
   Longword       FMaxSize;            // 数据最大尺寸

   Word           FMaxCache;           // 数据块的缓冲个数
   Byte           FSizeType;           // 数据块的尺寸级别

   long           FCacheFlag;          // 缓冲队列标志
   TIndex*        FCacheHead;          // 缓冲的头索引
   TIndex*        FCacheTail;          // 缓冲的尾索引
   Longword       FPushCache;          // 压入缓冲计数
   Longword       FPopCache;           // 弹出缓冲计数
};

}

#endif
