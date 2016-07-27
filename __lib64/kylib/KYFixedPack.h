// =======================================
// Unit   : 固定缓冲区的压缩/解压缩单元
// Version: 3.0.0.0 (build 2012.04.19)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYFixedPack_H_
#define _KYFixedPack_H_

#include "KYPackObj.h"

namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYFixedPack - 固定缓冲区的压缩类(基于LZ压缩算法) */

class TKYFixedPack
{
public:
   TKYFixedPack();
   virtual ~TKYFixedPack();

   // 属性
   char*          Dest() const         { return FDest; }       // default: NULL
   Word           Size() const         { return FSize; }       // default: 0
   Word           MaxSize() const      { return FMaxSize; }    // default: 0
   Longword       DataSize() const     { return FDataSize; }   // default: 0
   bool           IsPacking() const    { return FIsPacking; }  // default: false

   // 设置压缩结果缓冲区的最大尺寸
   bool           SetMaxSize(Word ASize);

   // 重置
   void           Reset();

   // 压缩缓冲区
   bool           Pack(const char* ABuffer, long ASize, Word AMax = 0xFFFF);

private:
   void           DoCopy(const char* ABuffer, long ASize);
   bool           DoMatch(Byte* APos, Byte* AEnd, Word& AOffset, Byte& ADepth);

private:
   char           FContext[1024];      // 压缩数据的上下文
   Word           FHash[4096];         // 哈希表
   char*          FDest;               // 压缩后的缓冲区
   Word           FSize;               // 压缩后数据尺寸
   Word           FMaxSize;            // 缓冲区最大尺寸
   Longword       FDataSize;           // 总压缩数据尺寸
   Longword       FLastSize;           // 上一次 pack 结束后的尺寸
   Byte           FBitBegin;           // 第一个字节数据开始位
   bool           FIsPacking;          // 判断是否正在压缩

private:
   // 编码项
   typedef struct
   {
      Byte        Length;              // 编码位数
      char        Bits[3];             // 编码
   } TCode, *PCode;

private:
   // 初始化深度和偏移量的静态编码表
   static void    _Init_Depth_Codes(TCode* ACodes, const Byte* ABase,
                               const Byte* ASuffix,      Byte  AHigh);
   static void    _Init_Offset_Codes();

private:
   // 深度和偏移量的静态编码表
   static TCode   _Codes_Depth[256];
   static TCode   _Codes_Offset[1024];

   // TKYFixedPack 的静态成员初始化类
   static class TInitialization
   {
   public:
      TInitialization();
      ~TInitialization();
   } _Initialization;

   friend class TInitialization;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYFixedUnpack - 固定缓冲区的解压缩类(基于LZ压缩算法) */

class TKYFixedUnpack
{
public:
   // 解压缩缓冲区
   // 1. ABuffer 存放 ASize 字节的已压缩数据
   // 2. ADest   存放解压缩后的数据, ADestSize >= ASize
   // 3. 若返回值 > 0 表示解压缩后数据的尺寸
   // 4. 若返回值为 0 表示解压缩失败, 可能 ADestSize 太小或数据未压缩
   // 5. 若返回值为 -1 表示参数不合法
   // 6. 若返回值为 -2 表示解压缩失败, 原因是数据已损坏
   static long    Unpack(const char* ABuffer, long ASize,
                               char* ADest,   long ADestSize);

};

}

#endif
