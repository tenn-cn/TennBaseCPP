// =======================================
// Unit   : 基类 (KYObject.h)
// Version: 3.1.2.0 (build 2013.05.31)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYObject_H_
#define _KYObject_H_

#include <windows.h>

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 常量列表 */

// 空指针值
#ifndef NULL
#define NULL               0
#endif

// 空指针值
//#define nil                NULL
//#define null               NULL

// 取结构项偏移量的宏{T: 结构类型, I: 结构项}
#define __Offset__(T, I)   ((long)&((T*)0)->I)

// 取绝对值的宏
#ifndef ABS
#define ABS(a)             (((a) >= 0)   ? (a) : -(a))
#endif

// 取最小值的宏
#ifndef MIN
#define MIN(a, b)          (((a) <= (b)) ? (a) : (b))
#endif

// 取最大值的宏
#ifndef MAX
#define MAX(a, b)          (((a) >= (b)) ? (a) : (b))
#endif

// 常量值
enum _TKYLibConst
{
   // 最大值
   Item_MaxCount           = 0x8000000,      // 最大项数
   Delta_MaxCount          = 0x2000000,      // 最大增长幅度

   // 容量增长幅度
   Delta_Auto              = 0,              // 自动增长
   Delta_None              = -1,             // 不增长，即容量限定

   // 默认值
   Default_Capacity        = 256,            // 默认容量
   Default_CacheSize       = 256,            // 默认缓冲尺寸

   // 添加项返回失败结果
   Add_IsExist             = -1,             // 索引项已存在
   Add_Error               = -2,             // 加索引项失败
   Add_IsFull              = -3,             // 容量已满

   // 搜索返回失败结果，若返回值 >= 0 则成功
   Search_None             = -1,             // 无搜索结果

   // 通用列表项的默认尺寸
   CustomItem_DefSize      = sizeof(void*),  // 默认尺寸必须大于 0
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 与 Pascal 对应的基本类型 */

// 有符号整型
typedef int                Integer,    *PInteger;
typedef __int64            Int64,      *PInt64;
typedef long               Longint,    *PLongint;
typedef short              Smallint,   *PSmallint;
typedef char               Shortint,   *PShortint;
typedef char               TFileName[260];

// 无符号整型
typedef unsigned int       Cardinal,   *PCardinal;
typedef unsigned long      Longword,   *PLongword;
typedef unsigned __int64   QWord,      *PQWord;
typedef unsigned long      DWord,      *PDWord;
typedef unsigned short     Word,       *PWord;
typedef unsigned char      Byte,       *PByte;
typedef unsigned char      Char;

// 布尔类型
typedef bool               Boolean,    *PBoolean;

// 浮点数
typedef float              Single,     *PSingle;
typedef double             Double,     *PDouble;

// 字符串指针
typedef char*              PChar;

// 无类型指针类型
typedef void*              Pointer;
typedef void**             PPointer;

// 句柄类型
typedef void*              THandle;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TObject - 基类 */

class TObject
{
};

/* 基本事件定义 */

// TOnEvent 事件
typedef void (TObject::*TDoEvent)();
typedef struct
{
   TDoEvent       Method;
   void*          Object;
} TOnEvent;

// TNotifyEvent 事件
typedef void (TObject::*TDoNotify)(void* Sender);
typedef struct
{
   TDoNotify      Method;
   void*          Object;
} TNotifyEvent;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 公用函数 */

// 取绝对值
template <class T>
inline T Abs(const T& a)
{
   return (a >= 0) ? a : -a;
}

// 取最小值
template <class T>
inline const T& Min(const T& a, const T& b)
{
   return (a <= b) ? a : b;
}

// 取最大值
template <class T>
inline const T& Max(const T& a, const T& b)
{
   return (a >= b) ? a : b;
}

// 释放并清空对象
template <class T>
inline void FreeAndNil(T* &AObject)
{
   T* obj  = AObject;
   AObject = NULL;

   delete obj;
}

// 释放并清空指针
template <class T>
inline void FreePointer(T* &APointer)
{
   void* p  = APointer;
   APointer = NULL;

   // 释放
   if (p != NULL)
      try
      {
         free(p);
      }
      catch (...) {}
}

// 取浮点数的小数部分
inline double Frac(double AValue)
{
   return AValue - (long)AValue;
}

// 指针交换函数
#if !defined(InterlockedExchangePointer) && !defined(_WIN64)
inline void* InterlockedExchangePointer(volatile Pointer* ADest, void* AValue)
{
   return (void*)InterlockedExchange((long*)ADest, (long)AValue);
}
#endif

}

#endif
