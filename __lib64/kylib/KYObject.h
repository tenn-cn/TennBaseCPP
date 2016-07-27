// =======================================
// Unit   : ���� (KYObject.h)
// Version: 3.1.2.0 (build 2013.05.31)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYObject_H_
#define _KYObject_H_

#include <windows.h>

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �����б� */

// ��ָ��ֵ
#ifndef NULL
#define NULL               0
#endif

// ��ָ��ֵ
//#define nil                NULL
//#define null               NULL

// ȡ�ṹ��ƫ�����ĺ�{T: �ṹ����, I: �ṹ��}
#define __Offset__(T, I)   ((long)&((T*)0)->I)

// ȡ����ֵ�ĺ�
#ifndef ABS
#define ABS(a)             (((a) >= 0)   ? (a) : -(a))
#endif

// ȡ��Сֵ�ĺ�
#ifndef MIN
#define MIN(a, b)          (((a) <= (b)) ? (a) : (b))
#endif

// ȡ���ֵ�ĺ�
#ifndef MAX
#define MAX(a, b)          (((a) >= (b)) ? (a) : (b))
#endif

// ����ֵ
enum _TKYLibConst
{
   // ���ֵ
   Item_MaxCount           = 0x8000000,      // �������
   Delta_MaxCount          = 0x2000000,      // �����������

   // ������������
   Delta_Auto              = 0,              // �Զ�����
   Delta_None              = -1,             // ���������������޶�

   // Ĭ��ֵ
   Default_Capacity        = 256,            // Ĭ������
   Default_CacheSize       = 256,            // Ĭ�ϻ���ߴ�

   // ������ʧ�ܽ��
   Add_IsExist             = -1,             // �������Ѵ���
   Add_Error               = -2,             // ��������ʧ��
   Add_IsFull              = -3,             // ��������

   // ��������ʧ�ܽ����������ֵ >= 0 ��ɹ�
   Search_None             = -1,             // ���������

   // ͨ���б����Ĭ�ϳߴ�
   CustomItem_DefSize      = sizeof(void*),  // Ĭ�ϳߴ������� 0
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �� Pascal ��Ӧ�Ļ������� */

// �з�������
typedef int                Integer,    *PInteger;
typedef __int64            Int64,      *PInt64;
typedef long               Longint,    *PLongint;
typedef short              Smallint,   *PSmallint;
typedef char               Shortint,   *PShortint;
typedef char               TFileName[260];

// �޷�������
typedef unsigned int       Cardinal,   *PCardinal;
typedef unsigned long      Longword,   *PLongword;
typedef unsigned __int64   QWord,      *PQWord;
typedef unsigned long      DWord,      *PDWord;
typedef unsigned short     Word,       *PWord;
typedef unsigned char      Byte,       *PByte;
typedef unsigned char      Char;

// ��������
typedef bool               Boolean,    *PBoolean;

// ������
typedef float              Single,     *PSingle;
typedef double             Double,     *PDouble;

// �ַ���ָ��
typedef char*              PChar;

// ������ָ������
typedef void*              Pointer;
typedef void**             PPointer;

// �������
typedef void*              THandle;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TObject - ���� */

class TObject
{
};

/* �����¼����� */

// TOnEvent �¼�
typedef void (TObject::*TDoEvent)();
typedef struct
{
   TDoEvent       Method;
   void*          Object;
} TOnEvent;

// TNotifyEvent �¼�
typedef void (TObject::*TDoNotify)(void* Sender);
typedef struct
{
   TDoNotify      Method;
   void*          Object;
} TNotifyEvent;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ���ú��� */

// ȡ����ֵ
template <class T>
inline T Abs(const T& a)
{
   return (a >= 0) ? a : -a;
}

// ȡ��Сֵ
template <class T>
inline const T& Min(const T& a, const T& b)
{
   return (a <= b) ? a : b;
}

// ȡ���ֵ
template <class T>
inline const T& Max(const T& a, const T& b)
{
   return (a >= b) ? a : b;
}

// �ͷŲ���ն���
template <class T>
inline void FreeAndNil(T* &AObject)
{
   T* obj  = AObject;
   AObject = NULL;

   delete obj;
}

// �ͷŲ����ָ��
template <class T>
inline void FreePointer(T* &APointer)
{
   void* p  = APointer;
   APointer = NULL;

   // �ͷ�
   if (p != NULL)
      try
      {
         free(p);
      }
      catch (...) {}
}

// ȡ��������С������
inline double Frac(double AValue)
{
   return AValue - (long)AValue;
}

// ָ�뽻������
#if !defined(InterlockedExchangePointer) && !defined(_WIN64)
inline void* InterlockedExchangePointer(volatile Pointer* ADest, void* AValue)
{
   return (void*)InterlockedExchange((long*)ADest, (long)AValue);
}
#endif

}

#endif
