// =======================================
// Unit   : 快速压缩/解压缩单元 (KYQuickPack.h)
// Version: 3.0.0.0 (build 2011.03.08)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYQuickPack_H_
#define _KYQuickPack_H_

#include "KYPackObj.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYQuickPack - 快速压缩类(基于LZRW压缩算法) */

class TKYQuickPack: public TKYCustomPack
{
public:
   TKYQuickPack();
   virtual ~TKYQuickPack();

   // 属性
   bool           IsPacking() const    { return FIsPacking; }  // default: false

   // 重置
   virtual void   Reset();

   // 压缩缓冲区
   // 1. ABuffer 存放 ASize 字节的待压缩数据
   // 2. ADest   存放压缩后的数据, 缓冲区尺寸至少为 ASize
   // 3. 若返回值 > 0 表示压缩后数据的尺寸
   // 4. 若返回值为 0 表示压缩失败, 即待压缩数据无法再压缩
   // 5. 若返回值为 -1 表示参数不合法
   // 6. 若返回值为 -2 表示压缩失败, 原因是正在压缩
   virtual long   Pack(const char* ABuffer, char* ADest, long ASize);

private:
   void           DoCopy(const char* ABuffer, long ASize);
   bool           DoMatch(Byte* APos, Byte* AEnd, Word& AOffset, Byte& ADepth);

private:
   Word           FHash[4096];         // 哈希表
   char           FContext[4096];      // 压缩数据的上下文
   Longword       FLastSize;           // 上一次 pack 结束后的尺寸
   bool           FIsPacking;          // 判断是否正在压缩
   bool           FIsOverflow;         // 判断上下文列表是否溢出
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYQuickUnpack - 快速解压缩类(基于LZRW压缩算法) */

class TKYQuickUnpack: public TKYCustomUnpack
{
public:
   TKYQuickUnpack();
   virtual ~TKYQuickUnpack();

   // 属性
   bool           IsUnpacking() const  { return FIsUnpacking; }// default: false

   // 重置
   virtual void   Reset();

   // 解压缩缓冲区
   // 1. 若 AIsPacked 为 false 表示数据未压缩, 即为解压缩的上下文相关性而操作,
   //    但不会拷贝数据到 ADest, 并返回值为 0
   // 2. ABuffer 存放 ASize 字节的已压缩数据
   // 3. ADest   存放解压缩后的数据, ADestSize >= ASize
   // 4. 若返回值 > 0 表示解压缩后数据的尺寸
   // 5. 若返回值为 0 表示解压缩失败, 可能 ADestSize 太小或数据未压缩
   // 6. 若返回值为 -1 表示参数不合法
   // 7. 若返回值为 -2 表示解压缩失败, 原因是数据已损坏
   // 8. 若返回值为 -3 表示解压缩失败, 原因是正在解压缩
   virtual long   Unpack(const char* ABuffer, long ASize,
                               char* ADest,   long ADestSize,
                               bool  AIsPacked = true);

private:
   void           DoCopy(const char* ABuffer, long ASize);
   bool           DoInsert(char* &APos, Word AOffset, Byte ADepth);

private:
   char           FContext[4096];      // 已解压的上下文
   Longword       FLastSize;           // 上一次 unpack 结束后的尺寸
   bool           FIsOverflow;         // 判断上下文列表是否溢出
   bool           FIsUnpacking;        // 判断是否正在解压缩
};

}

#endif
