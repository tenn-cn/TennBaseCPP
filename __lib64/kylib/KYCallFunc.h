// =======================================
// Unit   : 调用外部函数通用单元 (KYCallFunc.h)
// Version: 3.0.0.0 (build 2012.06.16)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
参数传递

前四个整型参数将被传入寄存器中。整数值将被传入 RCX、RDX、R8 和 R9 中（按从左至右
的顺序）。前四个参数及后面的参数将传递到堆栈中。在寄存器中，所有参数都是右对齐
的。因此，被调用方可以根据需要忽略寄存器中上面的位，并且只访问所需的寄存器部分。

浮点型和双精度参数传入 XMM0、XMM1、XMM2、XMM3（最大可达 4 个）中，并忽略常用于该
基槽的整型槽（RCX、RDX、R8 和 R9），反之亦然。

__m128 类型、数组和字符串从来不按即时值进行传递，但会向调用方分配的内存中传递指
针。大小为 8、16、32 或 64 位的结构/联合以及 __m64 将被视为具有相同大小的整数进
行传递。其他大小的结构/联合将作为指针传递到调用方分配的内存中。对于这些作为指针
（包括 __m128）传递的聚合类型，调用方分配的临时内存将是 16 字节对齐的。

返回值 (C++)

适合于 64 位的返回值是通过 RAX 返回的（包括 __m64 类型），但 __m128、__m128i、
__m128d、浮点值和双精度值除外，它们是在 XMM0 中返回的。如果返回值不适合于64位，
则由调用方为返回值分配指针并将其作为第一个参数传递。然后，后续参数将向右侧移动
一个参数的位置。RAX 中的被调用方必须返回相同的指针。要返回的用户定义类型的长度
必须为 8、16、32 或 64 位。

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#ifndef _KYCallFunc_H_
#define _KYCallFunc_H_

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 常量及类型定义 */

// 参数类型
enum TKYParamType {ptPointer,          // Pointer
                   ptByte,             // Byte
                   ptWord,             // Word
                   ptLongword,         // Longword
                   ptInteger,          // Integer
                   ptShortint,         // Shortint
                   ptSmallint,         // Smallint
                   ptLongint,          // Longint
                   ptInt64,            // Int64
                   ptBoolean,          // Boolean
                   ptSingle,           // Single
                   ptDouble,           // Double
                   ptCurrency,         // Currency: __int64 类型存储
                   ptStruct};          // Struct: sizeof(Struct) -> RecSize

// 参数项
#pragma pack(push, 1)
typedef struct
{
   char           Type;                // 参数类型 (TKYParamType)
   bool           IsRef;               // 是否引用传递(若为true则IsReal,IsAddr忽略)
   bool           IsReal;              // 是否为浮点数
   bool           IsAddr;              // ptStruct 尺寸不为: 1, 2, 4, 8
   int            RecSize;             // ptStruct 类型尺寸
   union
   {
      __int64     VInt64;              // __int64  类型值
      double      VDouble;             // double   类型值
      float       VSingle;             // float    类型值
      void*       VPointer;            // void*    类型值
      char        Buffer[8];           // 值的缓冲区
   };
} TKYParamItem, *PKYParamItem;
#pragma pack(pop)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 调用函数 */

// 注:
//    1. 若函数无返回值, 则 AResult 传为 NULL
//    2. AFunc 函数指针不允许为空
//    3. 若 ACount != 0 时, AParams 不允许为空

// 调用函数指针
bool CallFunc(void* AFunc, TKYParamItem* AParams,
                           TKYParamItem* AResult, unsigned ACount);

}

#endif