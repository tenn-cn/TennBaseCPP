// =======================================
// Unit   : 调用外部函数通用单元 (KYCallFunc.h)
// Version: 3.0.0.0 (build 2011.03.08)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ~~~~~~~~~~~~~~~~~ Parameter passing(参数传递) ~~~~~~~~~~~~~~~~~
1. 参数是通过栈传递给函数，这取决于例程的调用约定。

2. 变量参数(var)总是通过引用传递(作为32 位指针)，它指向(变量参数的)实际
   存储位置。

3. 值传递和常量参数(const)可通过值传递，也可能通过引用传递，这取决于参数的
   类型和大小：
   a. 一个有序类型的参数通过8 位、16 位、32 位或64 位值进行传递，它们的格式和
      相同类型的变量一致。

   b. 一个实数参数总是被传递到栈中。一个float类型的参数占用4 字节；double或
      Currency类型占用8 字节。

   c. 一个指针、类、类引用或函数(指针)类型的参数作为32 位指针传递。

   d. 一个方法指针作为两个32 位指针被传递到栈中。实例(对象)指针在方法指针
      之前被压入栈，所以方法指针占据较低的地址。

   e. 在pascal调用约定下，一个Variant类型参数被作为32位指针传递。

   f. 1、2 或4 字节的集合、记录和静态数组被当作8 位、16 位和32 位值传递。
      较大的集合、记录和静态数组被作为32 位指针。一个例外是，当使用cdecl、
      stdcall 或safecall 调用约定时，一个记录类型总是直接传给栈，此时，记录
      的大小被圆整位双字(4 字节的倍数)

4. 当两个参数被传递到栈时，每个参数占用4 字节的倍数。对8 位或16 位参数来说，
   即使它们只占用1个字节或1 个字，它也作为双字传递。双字中未用的部分没有定义。

5. 在pascal、cdecl、stdcall 和safecall 约定下，所有的参数被传递给栈。在pascal
   约定下，参数按它们声明的顺序(从左到右)被压入栈，所以，第一个参数在最高地址，
   最后一个参数在最低地址。在cdecl、stdcall 和safecall 约定下，参数以声明的相反
   顺序(从右到左)被压入栈，所以，第一个参数在最低位置，而最后一个在最高位置。

6. 寄存器保存约定
   函数必须保留EBX、ESI、EDI 和EBP 寄存器，但可以修改EAX、EDX 和ECX。当在汇编语
   言中实现构造和销毁时，保证预留DL 寄存器。函数被调用时，是假定CPU的direction
   标志是清除的(对应于CLD 指令)，并且返回时，direction 标志也必须是清除的。

 ~~~~~~~~~~~~~~~~~ Function results(函数结果) ~~~~~~~~~~~~~~~~~
以下约定适用于函数的返回值：
1. 可能的话，有序类型通过寄存器返回值：字节通过AL 返回，字通过AX 返回，双字
   通过EAX 返回。

2. 实数类型的返回值在浮点协处理器的栈顶寄存器(top-of-stack register，ST(0))。
   对于Currency 类型的返回值，ST(0)中的值被乘以10000。比如，Currency值1.234
   在ST(0)中的值为12340。

3. 对字符串、动态数组、方法指针、Variant、或int64 类型的返回值，就像函数在其
   它参数的后面额外声明了一个var 参数。换句话说，是函数调用者传递一个额外的
   32 位指针，它指向的变量用来返回结果。

4. 指针、类、类引用和函数指针类型，结果通过EAX 返回。

5. 对静态数组、记录和集合类型，若结果占用1 个字节，它通过AL 返回；若结果占用
   2 个字节，它通过AX 返回；若结果占用4 个字节，它通过EAX 返回。否则(结果超
   过4 个字节)，结果通过一个额外的var 参数返回，它在所有声明的参数的后边。

 ~~~~~~~~~~~~~~~~~ Method calls(方法调用) ~~~~~~~~~~~~~~~~~
1. 方法和其它普通函数使用相同的调用约定，除了每个方法有一个隐含的参数this，这是
   一个实例或类的引用。this 参数作为32 位指针传递。

2. 在pascal 调用约定下，this 就像在所有其它参数的后面声明(有时还要包括返回函数
   值的额外的var 参数)，所以，它最后被压入栈，所在的地址比其它参数要低。

3. 在cdecl、stdcall 和safecall 调用约定下，this 就像在所有其它参数的前面声明，
   但却在作为函数返回值的额外的var 参数之后(如果有的话)，所以，除了额外的var
   参数，它最后一个被压入栈。

 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef _KYCallFunc_H_
#define _KYCallFunc_H_

#include "KYObject.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 常量及类型定义 */

// 参数传递模式
enum TKYParamMode {pmValue,            // 传值
                   pmVar};             // 变量, 即引用地址方式

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
                   ptCurrency,         // Currency
                   ptStruct};          // Struct: sizeof(Struct) -> Param[1]

// 参数项
#pragma pack(push, 1)
typedef struct
{
   Byte           Mode;                // 传递方式 (TKYParamMode)
   Byte           Type;                // 参数类型 (TKYParamType)
   union
   {
      Longword    Param[2];            // 参数值
      double      VDouble;             // double   类型值
      float       VSingle;             // float    类型值
      __int64     VInt64;              // __int64  类型值
   };
} TKYParamItem, *PKYParamItem;
#pragma pack(pop)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 调用函数 */

// 注:
//    1. 若函数无返回值, 则 AResult 传为 NULL
//    2. AFunc 函数指针不允许为空
//    3. 若 ACount != 0 时, AParams 不允许为空

// 调用 cdecl 参数约定的函数
bool CallByCdecl(void* AFunc, TKYParamItem* AParams,
                              TKYParamItem* AResult, Word ACount);

// 调用 stdcall 参数约定的函数
bool CallByStdcall(void* AFunc, TKYParamItem* AParams,
                                TKYParamItem* AResult, Word ACount);

// 调用 pascal 参数约定的函数
bool CallByPascal(void* AFunc, TKYParamItem* AParams,
                               TKYParamItem* AResult, Word ACount);

}

#endif