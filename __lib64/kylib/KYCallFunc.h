// =======================================
// Unit   : �����ⲿ����ͨ�õ�Ԫ (KYCallFunc.h)
// Version: 3.0.0.0 (build 2012.06.16)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
��������

ǰ�ĸ����Ͳ�����������Ĵ����С�����ֵ�������� RCX��RDX��R8 �� R9 �У�����������
��˳�򣩡�ǰ�ĸ�����������Ĳ��������ݵ���ջ�С��ڼĴ����У����в��������Ҷ���
�ġ���ˣ������÷����Ը�����Ҫ���ԼĴ����������λ������ֻ��������ļĴ������֡�

�����ͺ�˫���Ȳ������� XMM0��XMM1��XMM2��XMM3�����ɴ� 4 �����У������Գ����ڸ�
���۵����Ͳۣ�RCX��RDX��R8 �� R9������֮��Ȼ��

__m128 ���͡�������ַ�������������ʱֵ���д��ݣ���������÷�������ڴ��д���ָ
�롣��СΪ 8��16��32 �� 64 λ�Ľṹ/�����Լ� __m64 ������Ϊ������ͬ��С��������
�д��ݡ�������С�Ľṹ/���Ͻ���Ϊָ�봫�ݵ����÷�������ڴ��С�������Щ��Ϊָ��
������ __m128�����ݵľۺ����ͣ����÷��������ʱ�ڴ潫�� 16 �ֽڶ���ġ�

����ֵ (C++)

�ʺ��� 64 λ�ķ���ֵ��ͨ�� RAX ���صģ����� __m64 ���ͣ����� __m128��__m128i��
__m128d������ֵ��˫����ֵ���⣬�������� XMM0 �з��صġ��������ֵ���ʺ���64λ��
���ɵ��÷�Ϊ����ֵ����ָ�벢������Ϊ��һ���������ݡ�Ȼ�󣬺������������Ҳ��ƶ�
һ��������λ�á�RAX �еı����÷����뷵����ͬ��ָ�롣Ҫ���ص��û��������͵ĳ���
����Ϊ 8��16��32 �� 64 λ��

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#ifndef _KYCallFunc_H_
#define _KYCallFunc_H_

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ���������Ͷ��� */

// ��������
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
                   ptCurrency,         // Currency: __int64 ���ʹ洢
                   ptStruct};          // Struct: sizeof(Struct) -> RecSize

// ������
#pragma pack(push, 1)
typedef struct
{
   char           Type;                // �������� (TKYParamType)
   bool           IsRef;               // �Ƿ����ô���(��Ϊtrue��IsReal,IsAddr����)
   bool           IsReal;              // �Ƿ�Ϊ������
   bool           IsAddr;              // ptStruct �ߴ粻Ϊ: 1, 2, 4, 8
   int            RecSize;             // ptStruct ���ͳߴ�
   union
   {
      __int64     VInt64;              // __int64  ����ֵ
      double      VDouble;             // double   ����ֵ
      float       VSingle;             // float    ����ֵ
      void*       VPointer;            // void*    ����ֵ
      char        Buffer[8];           // ֵ�Ļ�����
   };
} TKYParamItem, *PKYParamItem;
#pragma pack(pop)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ���ú��� */

// ע:
//    1. �������޷���ֵ, �� AResult ��Ϊ NULL
//    2. AFunc ����ָ�벻����Ϊ��
//    3. �� ACount != 0 ʱ, AParams ������Ϊ��

// ���ú���ָ��
bool CallFunc(void* AFunc, TKYParamItem* AParams,
                           TKYParamItem* AResult, unsigned ACount);

}

#endif