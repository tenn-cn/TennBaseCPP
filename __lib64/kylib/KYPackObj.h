// =======================================
// Unit   : 压缩/解压缩单元 (KYPackObj.h)
// Version: 3.0.1.0 (build 2014.12.13)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYPackObj_H_
#define _KYPackObj_H_

#include "KYList.h"
#include "KYCycQueue.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYCustomPack - 压缩基类 */

class TKYCustomPack
{
public:
   TKYCustomPack()                     { FData = NULL; FSize = 0; }
   virtual ~TKYCustomPack()            {}

   // 属性
   void*          Data() const         { return FData; }       // default: NULL
   Longword       Size() const         { return FSize; }       // default: 0

   // 设置自定义数据指针
   void           SetData(void* AData) { FData = AData; }

   // 重置, 如: FSize 置 0
   virtual void   Reset() = 0;

   // 压缩缓冲区
   // 1. ABuffer 存放 ASize 字节的待压缩数据
   // 2. ADest   存放压缩后的数据, 缓冲区尺寸至少为 ASize
   // 3. 若返回值 > 0 表示压缩后数据的尺寸
   // 4. 若返回值为 0 表示压缩失败, 即待压缩数据无法再压缩
   // 5. 若返回值为 -1 表示参数不合法
   virtual long   Pack(const char* ABuffer, char* ADest, long ASize) = 0;

protected:
   Longword       FSize;               // 总压缩数据尺寸

private:
   void*          FData;               // 自定义数据指针
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYCustomUnpack - 解压缩基类 */

class TKYCustomUnpack
{
public:
   TKYCustomUnpack()                   { FData = NULL; FSize = 0; }
   virtual ~TKYCustomUnpack()          {}

   // 属性
   void*          Data() const         { return FData; }       // default: NULL
   Longword       Size() const         { return FSize; }       // default: 0

   // 设置自定义数据指针
   void           SetData(void* AData) { FData = AData; }

   // 重置, 如: FSize 置 0
   virtual void   Reset() = 0;

   // 解压缩缓冲区
   // 1. 若 AIsPacked 为 false 表示数据未压缩, 即为解压缩的上下文相关性而操作,
   //    但不会拷贝数据到 ADest, 并返回值为 0
   // 2. ABuffer 存放 ASize 字节的已压缩数据
   // 3. ADest   存放解压缩后的数据, ADestSize >= ASize
   // 4. 若返回值 > 0 表示解压缩后数据的尺寸
   // 5. 若返回值为 0 表示解压缩失败, 可能 ADestSize 太小或数据未压缩
   // 6. 若返回值为 -1 表示参数不合法
   // 7. 若返回值为 -2 表示解压缩失败, 原因是数据已损坏
   virtual long   Unpack(const char* ABuffer, long ASize,
                               char* ADest,   long ADestSize,
                               bool  AIsPacked = true) = 0;

protected:
   Longword       FSize;               // 总压缩数据尺寸

private:
   void*          FData;               // 自定义数据指针
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYPack - 压缩类(基于LZ77压缩算法) */

class TKYPack: public TKYCustomPack
{
private:
   // 链接结点
   typedef struct
   {
      Word        Hash;                // 哈希值
      Word        Next;                // 下一项
   } TNode, *PNode;

   // 压缩项
   typedef union
   {
      void*       Data;                // 数据指针
      struct
      {
         Word     Offset;              // 偏移量
         Byte     Depth;               // 深度
      } I;
   } TItem, *PItem;

   // 编码项
   typedef struct
   {
      Byte        Length;              // 编码位数
      char        Bits[4];             // 编码
   } TCode, *PCode;

   // 编码表头项
   typedef struct
   {
      Word        Index;               // 索引, 如: 深度/偏移量
      Byte        Length;              // 编码位数
   } THead, *PHead;

   // Huffman 树结点
   typedef struct THuffNode
   {
      THuffNode*  Parent;              // 父结点
      THuffNode*  Next;                // 下一个结点
      Longword    Freq;                // 频度
      Word        Index;               // 索引, 如: 深度/偏移量
      Byte        Length;              // 编码位数
   } *PHuffNode;

public:
   TKYPack();
   virtual ~TKYPack();

   // 属性
   bool           IsFine() const       { return FIsFine; }     // default: false
   bool           IsQuick() const      { return FIsQuick; }    // default: true
   bool           IsPacking() const    { return FIsPacking; }  // default: false
   Byte           NiceDepth() const    { return FNiceDepth; }  // default: 64

   // 设置属性
   void           SetIsFine(bool AIsFine)
                  { if (!FIsPacking) FIsFine = AIsFine; }
   void           SetIsQuick(bool AIsQuick)
                  { if (!FIsPacking) FIsQuick = AIsQuick; }
   void           SetNiceDepth(Byte ADepth)
                  { if (!FIsPacking) FNiceDepth = ADepth; }

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
   void           DoHash(Byte* APos, Byte* AEnd, Word ASameLen);
   bool           DoScan(Byte* APos, Byte* AEnd,  Word  AOffset,
                                 Longword  ASize, Word& ASameLen);
   bool           DoMatch(Byte* APos,  Byte* AEnd, Word& AOffset,
                          Byte& ADepth, Word ASameLen);

   // 根据属性 IsQuick 压缩缓冲区
   void           DoPack0(Byte* &APos, Byte* AEnd, Longword& ABits, Longword& AOffsets);
   void           DoPack1(Byte* &APos, Byte* AEnd, Longword& ABits, Longword& AOffsets);

   // 执行编码
   long           DoOutput(char* ADest, long ASize, Longword ABits, Longword AOffsets);
   void           DoEncode(char* &ADest,    Byte& ABitBegin,
                          TCode*  ADepths, TCode* AOffsets);

private:
   // Huffman 树项 Freq 和 Length 的索引方法
   void           DoCompare1(const THuffNode* AItem1,
                             const THuffNode* AItem2, int& ACompare);
   void           DoCompare2(const THuffNode* AItem1,
                             const THuffNode* AItem2, int& ACompare);

private:
   Word*          FHash;               // 哈希表
   TNode*         FNodes;              // 结点列表
   TKYCycQueue*   FQueue;              // 压缩队列
   char*          FContext;            // 压缩数据的上下文
   KYString       FDepthCodes;         // 深度的 Huffman 编码
   KYString       FDepthStats;         // 深度的次数统计
   Longword       FLastSize;           // 上一次 pack 结束后的尺寸
   Byte           FNiceDepth;          // 匹配合适深度, 若达到此深度就中止下一个结点匹配
   bool           FIsFine;             // 是否使用哈夫曼压缩, 若为 true 则压缩速度相对较慢
   bool           FIsQuick;            // 是否使用快速压缩, 若为 true 则压缩率相对较差
   bool           FIsPacking;          // 是否正在压缩
   bool           FIsOverflow;         // 上下文列表是否溢出

private:
   // 初始化深度和偏移量的静态编码表
   static void     _Init_D0_2_Codes(TCode* ACodes, const Byte* ABase,
                               const Byte* ASuffix,      Byte  AHigh);
   static void     _Init_D3_5_Codes(TCode* ACodes, const Byte* ABase,
                               const Byte* ASuffix, Byte AHigh, Byte APrefix);
   static void     _Init_Offset_Codes();

   // 深度模式的统计函数
   static Longword _CalcDepthStats(Longword* ADepthStats, const TCode* ACodes);
   static Longword _SelectDepthModel(Longword* ADepthStats, Byte& AModel);
   static void     _WriteDepth(char* &APos, Byte& ABitBegin, Byte ADepth);

   // Huffman 编码的相关方法
   static void     _AddStats(Longword* AStats, TKYList& AList,
                             KYString& ANodes,    Word  AHigh);
   static Longword _BuildTree(Longword* AStats, KYString& AHeads, Word AHigh);
   static Longword _BuildCodes(Longword* AStats, TCode* ACodes,
                               KYString& AHeads,  Byte& ADelta, Word AHigh);
   static Longword _CalcHeadBits(const KYString& AHeads, Byte ADelta, Word AHigh);
   static void     _WriteHeadBits(char* &APos,   Byte& ABitBegin,
                         const KYString& AHeads, Byte  ADelta, Word AHigh);

private:
   // 深度和偏移量的静态编码表
   static TCode    _Codes_D0[256];
   static TCode    _Codes_D1[256];
   static TCode    _Codes_D2[256];
   static TCode    _Codes_D3[256];
   static TCode    _Codes_D4[256];
   static TCode    _Codes_D5[256];
   static TCode    _Codes_Offset[0x8000];

   // 各深度模式的静态编码表
   static TCode*  _Codes_Depth[6];

   // TKYPack 的静态成员初始化类
   static class TInitialization
   {
   public:
      TInitialization();
      ~TInitialization();
   } _Initialization;

   friend class TInitialization;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYUnpack - 解压缩类(基于LZ77压缩算法) */

class TKYUnpack: public TKYCustomUnpack
{
private:
   // 码索引
   typedef struct
   {
      Longword    Code;                // 深度编码
      Longword    Short;               // 短一位的编码
      Byte        Index;               // 深度表的索引
      Byte        Length;              // 编码长度
   } TIndex;

public:
   TKYUnpack();
   virtual ~TKYUnpack();

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

   // 执行解码
   long           DoDecode(char* ABuff, char* ABuffEnd,
                           char* ADest, char* ADestEnd);
   long           DoDecode(char* ABuff, char* ABuffEnd,
                           char* ADest, char* ADestEnd, Byte AModel);

private:
   char*          FContext;            // 已解压的上下文
   Longword       FLastSize;           // 上一次 unpack 结束后的尺寸
   bool           FIsOverflow;         // 判断上下文列表是否溢出
   bool           FIsUnpacking;        // 判断是否正在解压缩

private:
   // Huffman 解码的相关方法
   static bool    _ReadHeadBits(char* &APos, char* AEnd, Byte& ABitBegin,
                            KYString&  ADepths,      KYString& AIndexs);
   static bool    _DecodeDepth(char* &APos,        char* AEnd,
                               Byte&  ABitBegin,   Byte& ADepth,
                               Byte*  ADepths, Longword  ADCount,
                             TIndex*  AIndexs,     Byte  AICount);
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 缓冲区位操作相关函数 */

// 缓冲区 <-> 位数据
// 注: APos, AEnd 和 ABits 非空, ABitBegin 取值 [0..7], ABitCount > 0
void        BitsToBuffer(char* &APos, Byte& ABitBegin,
                                const char* ABits, long ABitCount);
void        BufferToBits(char* &APos, Byte& ABitBegin,
                                      char* ABits, long  ABitCount);

inline bool BitsToBuffer(char* &APos, char* AEnd,  Byte& ABitBegin,
                                const char* ABits, long  ABitCount)
{
   if (APos + ((ABitCount + ABitBegin - 1) >> 3) < AEnd)
   {
      BitsToBuffer(APos, ABitBegin, ABits, ABitCount);
      return true;
   }
   else
      return false;
}

inline bool BufferToBits(char* &APos, char* AEnd,  Byte& ABitBegin,
                                      char* ABits, long  ABitCount)
{
   if (APos + ((ABitCount + ABitBegin - 1) >> 3) < AEnd)
   {
      BufferToBits(APos, ABitBegin, ABits, ABitCount);
      return true;
   }
   else
      return false;
}

// 缓冲区 <-> Boolean (1 bit)
// 注: APos 和 AEnd 非空, ABitBegin 取值 [0..7]
void        BoolToBuffer(char* &APos, Byte& ABitBegin, bool AValue);
void        BufferToBool(char* &APos, Byte& ABitBegin, bool& AValue);

inline bool BoolToBuffer(char* &APos, char* AEnd, Byte& ABitBegin, bool AValue)
{
   if (APos < AEnd)
   {
      BoolToBuffer(APos, ABitBegin, AValue);
      return true;
   }
   else
      return false;
}

inline bool BufferToBool(char* &APos, char* AEnd, Byte& ABitBegin, bool& AValue)
{
   if (APos < AEnd)
   {
      BufferToBool(APos, ABitBegin, AValue);
      return true;
   }
   else
      return false;
}

// 缓冲区 <-> char
// 注: APos 和 AEnd 非空, ABitBegin 取值 [0..7]
void        CharToBuffer(char* &APos, Byte& ABitBegin, char AValue);
void        BufferToChar(char* &APos, Byte& ABitBegin, char& AValue);

inline bool CharToBuffer(char* &APos, char* AEnd, Byte& ABitBegin, char AValue)
{
   if (APos + ((ABitBegin + 7) >> 3) < AEnd)
   {
      CharToBuffer(APos, ABitBegin, AValue);
      return true;
   }
   else
      return false;
}

inline bool BufferToChar(char* &APos, char* AEnd, Byte& ABitBegin, char& AValue)
{
   if (APos + ((ABitBegin + 7) >> 3) < AEnd)
   {
      BufferToChar(APos, ABitBegin, AValue);
      return true;
   }
   else
      return false;
}

// 缓冲区 <-> Word
// 注: APos 和 AEnd 非空, ABitBegin 取值 [0..7]
void        WordToBuffer(char* &APos, Byte& ABitBegin, Word  AValue);
void        BufferToWord(char* &APos, Byte& ABitBegin, Word& AValue);

inline bool WordToBuffer(char* &APos, char* AEnd, Byte& ABitBegin, Word  AValue)
{
   if (APos + ((ABitBegin + 15) >> 3) < AEnd)
   {
      WordToBuffer(APos, ABitBegin, AValue);
      return true;
   }
   else
      return false;
}

inline bool BufferToWord(char* &APos, char* AEnd, Byte& ABitBegin, Word& AValue)
{
   if (APos + ((ABitBegin + 15) >> 3) < AEnd)
   {
      BufferToWord(APos, ABitBegin, AValue);
      return true;
   }
   else
      return false;
}

// 缓冲区 <-> long
// 注: APos 和 AEnd 非空, ABitBegin 取值 [0..7]
void        LongToBuffer(char* &APos, Byte& ABitBegin, long  AValue);
void        BufferToLong(char* &APos, Byte& ABitBegin, long& AValue);

inline bool LongToBuffer(char* &APos, char* AEnd, Byte& ABitBegin, long  AValue)
{
   if (APos + ((ABitBegin + 31) >> 3) < AEnd)
   {
      LongToBuffer(APos, ABitBegin, AValue);
      return true;
   }
   else
      return false;
}

inline bool BufferToLong(char* &APos, char* AEnd, Byte& ABitBegin, long& AValue)
{
   if (APos + ((ABitBegin + 31) >> 3) < AEnd)
   {
      BufferToLong(APos, ABitBegin, AValue);
      return true;
   }
   else
      return false;
}

// Byte 最大位数 <-> 缓冲区
// 注: APos 和 AEnd 非空, ABitBegin 取值 [0..7], AMax 取值 [1..8]
inline void ByteToBuffer(char* &APos, Byte& ABitBegin, Byte AValue, Byte AMax)
{
   AValue <<= 8 - AMax;
   BitsToBuffer(APos, ABitBegin, (char*)&AValue, AMax);
}

inline void BufferToByte(char* &APos, Byte& ABitBegin, Byte& AValue, Byte AMax)
{
   BufferToBits(APos, ABitBegin, (char*)&AValue, AMax);
   AValue >>= 8 - AMax;
}

inline bool ByteToBuffer(char* &APos, char* AEnd,   Byte& ABitBegin,
                                      Byte  AValue, Byte  AMax)
{
   AValue <<= 8 - AMax;
   return BitsToBuffer(APos, AEnd, ABitBegin, (char*)&AValue, AMax);
}

inline bool BufferToByte(char* &APos, char* AEnd,   Byte& ABitBegin,
                                      Byte& AValue, Byte  AMax)
{
   bool result = BufferToBits(APos, AEnd, ABitBegin, (char*)&AValue, AMax);
   if (result)
      AValue >>= 8 - AMax;
   return result;
}

// Word 最大位数 <-> 缓冲区
// 注: APos 和 AEnd 非空, ABitBegin 取值 [0..7], AMax 取值 [1..16]
void        WordToBuffer(char* &APos, Byte& ABitBegin, Word  AValue, Byte AMax);
void        BufferToWord(char* &APos, Byte& ABitBegin, Word& AValue, Byte AMax);

bool        WordToBuffer(char* &APos, char* AEnd,   Byte& ABitBegin,
                                      Word  AValue, Byte  AMax);
bool        BufferToWord(char* &APos, char* AEnd,   Byte& ABitBegin,
                                      Word& AValue, Byte  AMax);

}

#endif
