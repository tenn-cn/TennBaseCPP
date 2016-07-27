// =======================================
// Unit   : �ɱ������� (KYVarType.h)
// Version: 3.0.0.0 (build 2012.06.04)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYVarType_H_
#define _KYVarType_H_

#include "KYDateTime.h"
#include "KYMemBlock.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYVarType - �ɱ������� */

class TKYVarType
{
public:
   // ��������
   enum TType       {vtUnknown,        // 0:  (δ֪)
                     vtBoolean,        // 1:  ��������
                     vtByte,           // 2:  �ֽ�����
                     vtWord,           // 3:  ˫�ֽ�����
                     vtLongword,       // 4:  ���ֽ�����
                     vtShortint,       // 5:  ���ֽ�����
                     vtSmallint,       // 6:  ˫�ֽ�����
                     vtLongint,        // 7:  ���ֽ�����
                     vtInt64,          // 8:  ���ֽ�����
                     vtInteger,        // 9:  ����
                     vtSingle,         // 10: �����ȸ�����
                     vtDouble,         // 11: ˫���ȸ�����
                     vtCurrency,       // 12: �������
                     vtDateTime,       // 13: ����ʱ������
                     vtString,         // 14: �ַ�������
                     vtPointer};       // 15: ָ������

   // �ɱ�������
   #pragma pack(push, 1)
   typedef struct
   {
      Byte           Type;             // ����
      union
      {
         __int64     VInt64;           // vtInt64
         void*       VUnknown;         // vtUnknown
         bool        VBoolean;         // vtBoolean
         Byte        VByte;            // vtByte
         Word        VWord;            // vtWord
         Longword    VLongword;        // vtLongword
         char        VShortint;        // vtShortint
         short       VSmallint;        // vtSmallint
         long        VLongint;         // vtLongint
         int         VInteger;         // vtInteger
         float       VSingle;          // vtSingle
         double      VDouble;          // vtDouble
         __int64     VCurrency;        // vtCurrency
         TDateTime   VDateTime;        // vtDateTime
         KYString*   VString;          // vtString
         void*       VPointer;         // vtPointer
         Byte        Bytes[8];         // ֵ�Ļ�����
      };
   } TVarData, *PVarData;
   #pragma pack(pop)

public:
   TKYVarType(TType AType = vtUnknown);
   virtual ~TKYVarType();

   // '=' ����������, �� Type() == vtUnknown, ���һ�θ�ֵʱ�Զ�������Ӧ������
   TKYVarType& operator=(const bool AValue);
   TKYVarType& operator=(const Byte AValue);
   TKYVarType& operator=(const Word AValue);
   TKYVarType& operator=(const Longword AValue);
   TKYVarType& operator=(const char AValue);
   TKYVarType& operator=(const short AValue);
   TKYVarType& operator=(const long AValue);
   TKYVarType& operator=(const __int64 AValue);
   TKYVarType& operator=(const int AValue);
   TKYVarType& operator=(const float AValue);
   TKYVarType& operator=(const double AValue);
   TKYVarType& operator=(const KYString& AValue);
   TKYVarType& operator=(const char* AValue);
   TKYVarType& operator=(const void* AValue);
   TKYVarType& operator=(const TVarData* AValue);
   TKYVarType& operator=(const TKYVarType& AValue);

   // ��������
   TType       Type() const            { return (TType)FData->Type; }

   // �ɱ�����ֵ
   TVarData*   Value() const           { return FData; }

   // �ж����Ƿ�Ϊ��ֵ
   bool        IsNull() const          { return (FData->VInt64 == 0)
                                             || ((FData->Type == vtString)
                                                && (FData->VString->Length() == 0)); }

   // ���ñ�������, ���Զ�ת��ֵ
   void        SetType(TType AType);

   // ���ÿɱ�����ֵ, ���Զ�ת��ֵ
   bool        SetValue(const TVarData* AValue);

   // ������ֵ����
   bool        AsBoolean() const;
   Byte        AsByte() const;
   Word        AsWord() const;
   Longword    AsLongword() const;
   char        AsShortint() const;
   short       AsSmallint() const;
   long        AsLongint() const;
   __int64     AsInt64() const;
   int         AsInteger() const;
   float       AsSingle() const;
   double      AsDouble() const;
   __int64     AsCurrency() const;
   TDateTime   AsDateTime() const;
   KYString    AsString() const;
   void*       AsPointer() const;

   // ���ñ���ֵ����
   bool        ByBoolean(const bool AValue);
   bool        ByByte(const Byte AValue);
   bool        ByWord(const Word AValue);
   bool        ByLongword(const Longword AValue);
   bool        ByShortint(const char AValue);
   bool        BySmallint(const short AValue);
   bool        ByLongint(const long AValue);
   bool        ByInt64(const __int64 AValue);
   bool        ByInteger(const int AValue);
   bool        BySingle(const float AValue);
   bool        ByDouble(const double AValue);
   bool        ByCurrency(const __int64 AValue);
   bool        ByDateTime(const TDateTime AValue);
   bool        ByString(const KYString& AValue);
   bool        ByPointer(const void* AValue);

   // ���ֵ, �������Ͳ���
   void        Clear();

   // �������, ��������Ϊ vtUnknown
   void        Empty();

   // �ӻ���������ֵ(vtString ���ͳ���)
   long        LoadFrom(const void* ABuffer);

   // ����ֵ��������(vtString ���ͳ���)
   long        SaveTo(void* ABuffer);

   // ����ָ��(ֻ����vtUnknown �� vtPointer ����)
   void*       ExchangePointer(const void* AValue);

protected:
private:
   TVarData*   FData;

public:
   // �ж�����ֵ����Ƿ�Ϊ: ��->��
   static const union TInt32_Union
   {
      long           Value;
      Byte           Bytes[4];
   } Int32_Memory;                        // Int32_Memory.Value = 0x03020100;

   // ��̬��Ա
   static const TVarData Null_Var;        // TVarData ���Ϳ�ֵ
   static const TType    Type_MaxValue;   // 15: vtPointer �������ֵ
   static const Word     Type_Sizes[16];  // ���ͳߴ�
   static const KYString Type_Names[16];  // ������
   static const KYString Bool_String[2];  // �������ַ���ֵ

public:
   // ������ͷſɱ�����
   static TVarData* VarNew()              { return (TVarData*)VarData_Manager->New(); }
   static void VarDelete(TVarData* AData) { VarData_Manager->Delete(AData); }

   // ����ת��
   static bool VarCast(TVarData& ADest, const TVarData& ASource, TType ADestType);
   static bool VarCast(TVarData& ADest, const TVarData& ASource,
                           TType ADestType,   KYString& AError);

   // ��������
   static void VarCopy(TVarData& ADest, const TVarData& ASource);

   // �������
   static void VarEmpty(TVarData& AValue);

private:
   static void DoInitVarData(TVarData* AData, Word ASize);
   static void DoFreeVarData(TVarData* AData, Word ASize);

   // �ɱ������������, ��ſɱ������� TVarData
   static TKYMemBlock*  VarData_Manager;

   // TKYVarType �ľ�̬��Ա��ʼ����
   static class TInitialization
   {
   public:
      TInitialization();
      ~TInitialization();
   } Inner_Initialization;

   friend class TInitialization;
};

}

#endif
