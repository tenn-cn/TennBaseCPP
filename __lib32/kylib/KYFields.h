// =======================================
// Unit   : �ֶζ��弯���ֶμ�
// Version: 3.0.1.0 (build 2013.05.23)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYFields_H_
#define _KYFields_H_

#include "KYVarType.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// �ֶζ��弯��
class TKYFieldDefs;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYFieldDef - �ֶζ����� */

// �ֶ�����
enum TKYFieldType {ftUnknown,          // 0:  (δ֪)
                   ftString,           // 1:  �ַ�������
                   ftFixedStr,         // 2:  �ַ�������
                   ftSmallint,         // 3:  ˫�ֽ�����
                   ftInteger,          // 4:  ���ֽ�����
                   ftInt64,            // 5:  ���ֽ�����
                   ftWord,             // 6:  ˫�ֽ�����
                   ftByte,             // 7:  �ֽ�����
                   ftBoolean,          // 8:  ��������
                   ftFloat,            // 9:  ������
                   ftCurrency,         // 10: �������
                   ftDateTime,         // 11: ����ʱ������
                   ftFixedSize};       // 12: �̶��ߴ�����

// �ֶζ�����
class TKYFieldDef
{
   friend class TKYFieldDefs;
public:
   // ��̬��Ա
   static const TKYFieldType        Type_MaxValue;      // ftFixedSize �������ֵ
   static const Word                Type_Sizes[13];     // ���ͳߴ�
   static const KYString            Type_Names[13];     // ������
   static const TKYVarType::TType   Type_VarTypes[13];  // �ֶ����Ͷ�Ӧ�Ŀɱ�����

   // ��̬����
   static long    CalcMapSize(long ACount);
   static long    CalcPhysSize(TKYFieldType AType, long ASize, bool AIsMinSize);

public:
   TKYFieldDef(const TKYFieldDefs* AOwner = NULL);
   virtual ~TKYFieldDef();

   // '=' ����������
   TKYFieldDef&   operator=(const TKYFieldDef& AFieldDef);

   // ������
   KYString       Name() const      { return FName; }    // default: ""
   TKYFieldType   Type() const      { return FType; }    // default: ftUnknown
   long           Size() const      { return FSize; }    // default: 0
   long           MinSize() const   { return CalcPhysSize(FType, FSize, true); }
   long           PhysSize() const  { return CalcPhysSize(FType, FSize, false); }

   // д����
   bool           SetName(const KYString& AName, bool ANotCheck = false);
   bool           SetType(TKYFieldType AType);
   bool           SetSize(long ASize);

   // �����ֶζ���, �������� FOwner ��Ա
   bool           Assign(const TKYFieldDef& AFieldDef);

protected:
private:
   TKYFieldDefs*  FOwner;              // �����ֶζ��弯����
   KYString       FName;               // �ֶ���
   TKYFieldType   FType;               // �ֶ�����
   long           FSize;               // �ֶγߴ�
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYFieldDefs - �ֶζ��弯�� */

class TKYFieldDefs
{
public:
   TKYFieldDefs();
   TKYFieldDefs(const TKYFieldDefs& AFieldDefs);
   virtual ~TKYFieldDefs();

   // '=' ����������
   TKYFieldDefs&  operator=(const TKYFieldDefs& AFieldDefs);

   // ������
   long           Count() const              { return FItems->Count(); }
   TKYFieldDef*   Items(long AIndex) const   { return (TKYFieldDef*)FItems->Item(AIndex); }
   long           RecordSize() const;        // ��¼���ߴ�
   long           RecordMinSize() const;     // ��¼��С�ߴ�

   // �����ֶζ��弯
   void           Assign(const TKYFieldDefs& AFieldDefs);

   // ��������ֶζ���
   void           Clear();

   // ɾ��ָ���������ֶζ���
   void           Delete(long AIndex);

   // �����ֶζ��嵽ĩβ
   long           Add(const KYString& AName, TKYFieldType AType,
                                 long ASize,         bool ANotCheck = false);
   long           Add(const TKYFieldDef& AFieldDef);

   // ����ָ���������ֶζ���
   // �� AIndex <= 0 ������һ��, �� AIndex >= Count() ��嵽ĩβ��
   long           Insert(long AIndex, const KYString& AName,
                 TKYFieldType AType, long ASize, bool ANotCheck = false);
   long           Insert(long AIndex, const TKYFieldDef& AFieldDef);

   // ����ָ���ֶ������ֶζ���, �ֶ��������ִ�Сд
   TKYFieldDef*   Find(const KYString& AName) const;

   // ָ���ֶ������б��е�����, �ֶ��������ִ�Сд
   long           IndexOf(const KYString& AName) const;

protected:
   void           DoDeletion(TKYFieldDef* AItem);

private:
   TKYList*       FItems;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYField - �ֶ��� */

class TKYField
{
public:
   // �ɱ�����
   typedef TKYVarType::TVarData  TVarData;

public:
   TKYField(const KYString& AName = "",
               TKYFieldType AType = ftUnknown,
                       long ASize = 0);
   TKYField(const TKYFieldDef& AFieldDef);
   TKYField(const TKYField& AField);
   virtual ~TKYField();

   // '=' ����������
   TKYField&      operator=(const KYString& AValue);
   TKYField&      operator=(const char* AValue);
   TKYField&      operator=(const short AValue);
   TKYField&      operator=(const int AValue);
   TKYField&      operator=(const long AValue);
   TKYField&      operator=(const Longword AValue);
   TKYField&      operator=(const __int64 AValue);
   TKYField&      operator=(const Word AValue);
   TKYField&      operator=(const Byte AValue);
   TKYField&      operator=(const bool AValue);
   TKYField&      operator=(const float AValue);
   TKYField&      operator=(const double AValue);
   TKYField&      operator=(const TKYField& AValue);
   TKYField&      operator=(const TKYVarType& AValue);
   TKYField&      operator=(const TVarData* AValue);

   // ������
   KYString       Name() const         { return FName; }    // default: ""
   TKYFieldType   Type() const         { return FType; }    // default: ftUnknown
   long           Size() const         { return FSize; }    // default: 0
   long           MinSize() const      { return IsNull() ? 0 : FMinSize; }
   long           DataSize() const;                         // default: 0

   bool           CanNull() const      { return FCanNull; } // default: false
   bool           IsNull() const       { return FCanNull && FMaybeNull
                                                         && FVar->IsNull(); }

   // ��������
   void           SetCanNull(bool ACanNull);
   void           SetIsNull(bool AIsNull);

   // ���ֶ�ֵ����
   KYString       AsString() const     { return FVar->AsString(); }
   short          AsSmallint() const   { return FVar->AsSmallint(); }
   long           AsInteger() const    { return FVar->AsLongint(); }
   __int64        AsInt64() const      { return FVar->AsInt64(); }
   Word           AsWord() const       { return FVar->AsWord(); }
   Byte           AsByte() const       { return FVar->AsByte(); }
   bool           AsBoolean() const    { return FVar->AsBoolean(); }
   double         AsFloat() const      { return FVar->AsDouble(); }
   __int64        AsCurrency() const   { return FVar->AsCurrency(); }
   TDateTime      AsDateTime() const   { return FVar->AsDateTime(); }
   TVarData*      Value() const        { return FVar->Value(); }

   // д�ֶ�ֵ����
   bool           ByString(const KYString& AValue)
                  { return FVar->ByString(AValue); }
   bool           BySmallint(const short AValue)
                  { return FVar->BySmallint(AValue); }
   bool           ByInteger(const long AValue)
                  { return FVar->ByLongint(AValue); }
   bool           ByInt64(const __int64 AValue)
                  { return FVar->ByInt64(AValue); }
   bool           ByWord(const Word AValue)
                  { return FVar->ByWord(AValue); }
   bool           ByByte(const Byte AValue)
                  { return FVar->ByByte(AValue); }
   bool           ByBoolean(const bool AValue)
                  { return FVar->ByBoolean(AValue); }
   bool           ByFloat(const double AValue)
                  { return FVar->ByDouble(AValue); }
   bool           ByCurrency(const __int64 AValue)
                  { return FVar->ByCurrency(AValue); }
   bool           ByDateTime(const TDateTime AValue)
                  { return FVar->ByDateTime(AValue); }
   bool           SetValue(const TKYVarType& AValue)
                  { return SetValue(AValue.Value()); }
   bool           SetValue(const TVarData* AValue);

   // ����ֶ�ֵ
   void           Clear();

   // ���ֶ���ȡֵ����������
   long           GetData(void* AHead, Pointer& Annex) const;
   long           GetData(void* AHead, Pointer& Annex, void* AEnd, bool& AIsError) const;

   // �ӻ�������дֵ���ֶ�
   long           SetData(void* AHead, Pointer& Annex);
   long           SetData(void* AHead, Pointer& Annex, void* AEnd, bool& AIsError);

protected:
private:
   void           AssignFixedSize();
   void           DisposeFixedSize();

private:
   TKYVarType*    FVar;                // �ֶα���ֵ
   KYString       FName;               // �ֶ���
   TKYFieldType   FType;               // �ֶ�����
   long           FSize;               // �ֶγߴ�
   long           FMinSize;            // �ֶ���С�ߴ�(�������ͼ������)
   bool           FCanNull;            // �ֶ��Ƿ�����Ϊ��
   bool           FMaybeNull;          // �ֶ�ֵ�Ƿ����Ϊ��
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYFields - �ֶμ��� */

class TKYFields
{
public:
   TKYFields();
   TKYFields(const TKYFields& AFields);
   TKYFields(const TKYFieldDefs& AFieldDefs);
   virtual ~TKYFields();

   // '=' ����������
   TKYFields&     operator=(const TKYFields& AFields);
   TKYFields&     operator=(const TKYFieldDefs& AFieldDefs);

   // ������
   bool           CanNull() const            { return FCanNull; }       // default: false
   long           Count() const              { return FItems->Count(); }// default: 0
   TKYField*      Fields(long AIndex) const  { return (TKYField*)FItems->Item(AIndex); }
   long           RecordSize() const;        // ��¼���ݳߴ�
   long           RecordMinSize() const;     // ��¼��С�ߴ�

   // �����ֶμ��Ƿ�����Ϊ��
   void           SetCanNull(bool ACanNull);

   // �����ֶμ�
   void           Assign(const TKYFields& AFields);

   // �����ֶζ��弯
   void           Assign(const TKYFieldDefs& AFieldDefs);

   // ��������ֶ�
   void           Clear();

   // ɾ��ָ���������ֶ�
   void           Delete(long AIndex);

   // �Ƴ�ָ���ֶ�
   void           Remove(TKYField* AField);

   // ����ֶ�
   long           Add(const TKYField& AField);
   long           Add(const TKYFieldDef& AFieldDef);
   long           Add(const KYString& AName, TKYFieldType AType, long ASize);

   // ����ָ���ֶ������ֶ�, �ֶ��������ִ�Сд
   TKYField*      FindField(const KYString& AName) const;

   // ȡָ���ֶ���������, �ֶ��������ִ�Сд
   long           IndexOf(const KYString& AName) const;

   // ȡָ���ֶε�����
   long           IndexOf(const TKYField* AField) const;

   // �����¼����
   void           ClearDataSet();

   // д��¼����������
   long           GetDataSet(void* ABuffer) const;
   bool           GetDataSet(void* ABuffer, long& ASize) const;

   // �ӻ������ж���¼
   long           SetDataSet(const void* ABuffer);
   bool           SetDataSet(const void* ABuffer, long& ASize);

   // CanNull() Ϊ true ʱ, д�ֶο�ӳ�����������
   long           GetNullMap(void* ABuffer) const;
   bool           GetNullMap(void* ABuffer, long& ASize) const;

   // CanNull() Ϊ true ʱ, �ӻ������ж��ֶο�ӳ���
   long           SetNullMap(const void* ABuffer);
   bool           SetNullMap(const void* ABuffer, long& ASize);

protected:
   void           DoDeletion(TKYField* AItem);

private:
   // ��ȡд�ֶο�ӳ���������, ע: FCanNull && (ABuffer != NULL)
   long           DoGetNullMap(void* ABuffer) const;
   long           DoSetNullMap(const void* ABuffer);

private:
   TKYList*       FItems;
   bool           FCanNull;
};

}

#endif
