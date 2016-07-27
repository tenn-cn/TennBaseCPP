// =======================================
// Unit   : 字段定义集及字段集
// Version: 3.0.1.0 (build 2013.05.23)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYFields_H_
#define _KYFields_H_

#include "KYVarType.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// 字段定义集类
class TKYFieldDefs;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYFieldDef - 字段定义类 */

// 字段类型
enum TKYFieldType {ftUnknown,          // 0:  (未知)
                   ftString,           // 1:  字符串类型
                   ftFixedStr,         // 2:  字符串类型
                   ftSmallint,         // 3:  双字节整型
                   ftInteger,          // 4:  四字节整型
                   ftInt64,            // 5:  八字节整型
                   ftWord,             // 6:  双字节类型
                   ftByte,             // 7:  字节类型
                   ftBoolean,          // 8:  布尔类型
                   ftFloat,            // 9:  浮点型
                   ftCurrency,         // 10: 金额类型
                   ftDateTime,         // 11: 日期时间类型
                   ftFixedSize};       // 12: 固定尺寸类型

// 字段定义类
class TKYFieldDef
{
   friend class TKYFieldDefs;
public:
   // 静态成员
   static const TKYFieldType        Type_MaxValue;      // ftFixedSize 最大类型值
   static const Word                Type_Sizes[13];     // 类型尺寸
   static const KYString            Type_Names[13];     // 类型名
   static const TKYVarType::TType   Type_VarTypes[13];  // 字段类型对应的可变类型

   // 静态方法
   static long    CalcMapSize(long ACount);
   static long    CalcPhysSize(TKYFieldType AType, long ASize, bool AIsMinSize);

public:
   TKYFieldDef(const TKYFieldDefs* AOwner = NULL);
   virtual ~TKYFieldDef();

   // '=' 操作符重载
   TKYFieldDef&   operator=(const TKYFieldDef& AFieldDef);

   // 读属性
   KYString       Name() const      { return FName; }    // default: ""
   TKYFieldType   Type() const      { return FType; }    // default: ftUnknown
   long           Size() const      { return FSize; }    // default: 0
   long           MinSize() const   { return CalcPhysSize(FType, FSize, true); }
   long           PhysSize() const  { return CalcPhysSize(FType, FSize, false); }

   // 写属性
   bool           SetName(const KYString& AName, bool ANotCheck = false);
   bool           SetType(TKYFieldType AType);
   bool           SetSize(long ASize);

   // 拷贝字段定义, 但不复制 FOwner 成员
   bool           Assign(const TKYFieldDef& AFieldDef);

protected:
private:
   TKYFieldDefs*  FOwner;              // 所属字段定义集对象
   KYString       FName;               // 字段名
   TKYFieldType   FType;               // 字段类型
   long           FSize;               // 字段尺寸
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYFieldDefs - 字段定义集类 */

class TKYFieldDefs
{
public:
   TKYFieldDefs();
   TKYFieldDefs(const TKYFieldDefs& AFieldDefs);
   virtual ~TKYFieldDefs();

   // '=' 操作符重载
   TKYFieldDefs&  operator=(const TKYFieldDefs& AFieldDefs);

   // 读属性
   long           Count() const              { return FItems->Count(); }
   TKYFieldDef*   Items(long AIndex) const   { return (TKYFieldDef*)FItems->Item(AIndex); }
   long           RecordSize() const;        // 记录最大尺寸
   long           RecordMinSize() const;     // 记录最小尺寸

   // 拷贝字段定义集
   void           Assign(const TKYFieldDefs& AFieldDefs);

   // 清除所有字段定义
   void           Clear();

   // 删除指定索引的字段定义
   void           Delete(long AIndex);

   // 加入字段定义到末尾
   long           Add(const KYString& AName, TKYFieldType AType,
                                 long ASize,         bool ANotCheck = false);
   long           Add(const TKYFieldDef& AFieldDef);

   // 插入指定索引的字段定义
   // 若 AIndex <= 0 则插入第一项, 若 AIndex >= Count() 则插到末尾项
   long           Insert(long AIndex, const KYString& AName,
                 TKYFieldType AType, long ASize, bool ANotCheck = false);
   long           Insert(long AIndex, const TKYFieldDef& AFieldDef);

   // 查找指定字段名的字段定义, 字段名不区分大小写
   TKYFieldDef*   Find(const KYString& AName) const;

   // 指定字段名在列表中的索引, 字段名不区分大小写
   long           IndexOf(const KYString& AName) const;

protected:
   void           DoDeletion(TKYFieldDef* AItem);

private:
   TKYList*       FItems;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYField - 字段类 */

class TKYField
{
public:
   // 可变数据
   typedef TKYVarType::TVarData  TVarData;

public:
   TKYField(const KYString& AName = "",
               TKYFieldType AType = ftUnknown,
                       long ASize = 0);
   TKYField(const TKYFieldDef& AFieldDef);
   TKYField(const TKYField& AField);
   virtual ~TKYField();

   // '=' 操作符重载
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

   // 读属性
   KYString       Name() const         { return FName; }    // default: ""
   TKYFieldType   Type() const         { return FType; }    // default: ftUnknown
   long           Size() const         { return FSize; }    // default: 0
   long           MinSize() const      { return IsNull() ? 0 : FMinSize; }
   long           DataSize() const;                         // default: 0

   bool           CanNull() const      { return FCanNull; } // default: false
   bool           IsNull() const       { return FCanNull && FMaybeNull
                                                         && FVar->IsNull(); }

   // 设置属性
   void           SetCanNull(bool ACanNull);
   void           SetIsNull(bool AIsNull);

   // 读字段值属性
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

   // 写字段值属性
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

   // 清除字段值
   void           Clear();

   // 从字段中取值到缓冲区中
   long           GetData(void* AHead, Pointer& Annex) const;
   long           GetData(void* AHead, Pointer& Annex, void* AEnd, bool& AIsError) const;

   // 从缓冲区中写值到字段
   long           SetData(void* AHead, Pointer& Annex);
   long           SetData(void* AHead, Pointer& Annex, void* AEnd, bool& AIsError);

protected:
private:
   void           AssignFixedSize();
   void           DisposeFixedSize();

private:
   TKYVarType*    FVar;                // 字段变量值
   KYString       FName;               // 字段名
   TKYFieldType   FType;               // 字段类型
   long           FSize;               // 字段尺寸
   long           FMinSize;            // 字段最小尺寸(根据类型计算出来)
   bool           FCanNull;            // 字段是否允许为空
   bool           FMaybeNull;          // 字段值是否可能为空
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYFields - 字段集类 */

class TKYFields
{
public:
   TKYFields();
   TKYFields(const TKYFields& AFields);
   TKYFields(const TKYFieldDefs& AFieldDefs);
   virtual ~TKYFields();

   // '=' 操作符重载
   TKYFields&     operator=(const TKYFields& AFields);
   TKYFields&     operator=(const TKYFieldDefs& AFieldDefs);

   // 读属性
   bool           CanNull() const            { return FCanNull; }       // default: false
   long           Count() const              { return FItems->Count(); }// default: 0
   TKYField*      Fields(long AIndex) const  { return (TKYField*)FItems->Item(AIndex); }
   long           RecordSize() const;        // 记录数据尺寸
   long           RecordMinSize() const;     // 记录最小尺寸

   // 设置字段集是否允许为空
   void           SetCanNull(bool ACanNull);

   // 拷贝字段集
   void           Assign(const TKYFields& AFields);

   // 拷贝字段定义集
   void           Assign(const TKYFieldDefs& AFieldDefs);

   // 清除所有字段
   void           Clear();

   // 删除指定索引的字段
   void           Delete(long AIndex);

   // 移除指定字段
   void           Remove(TKYField* AField);

   // 添加字段
   long           Add(const TKYField& AField);
   long           Add(const TKYFieldDef& AFieldDef);
   long           Add(const KYString& AName, TKYFieldType AType, long ASize);

   // 查找指定字段名的字段, 字段名不区分大小写
   TKYField*      FindField(const KYString& AName) const;

   // 取指定字段名的索引, 字段名不区分大小写
   long           IndexOf(const KYString& AName) const;

   // 取指定字段的索引
   long           IndexOf(const TKYField* AField) const;

   // 清除记录内容
   void           ClearDataSet();

   // 写记录到缓冲区中
   long           GetDataSet(void* ABuffer) const;
   bool           GetDataSet(void* ABuffer, long& ASize) const;

   // 从缓冲区中读记录
   long           SetDataSet(const void* ABuffer);
   bool           SetDataSet(const void* ABuffer, long& ASize);

   // CanNull() 为 true 时, 写字段空映射表到缓冲区中
   long           GetNullMap(void* ABuffer) const;
   bool           GetNullMap(void* ABuffer, long& ASize) const;

   // CanNull() 为 true 时, 从缓冲区中读字段空映射表
   long           SetNullMap(const void* ABuffer);
   bool           SetNullMap(const void* ABuffer, long& ASize);

protected:
   void           DoDeletion(TKYField* AItem);

private:
   // 读取写字段空映射表到缓冲区, 注: FCanNull && (ABuffer != NULL)
   long           DoGetNullMap(void* ABuffer) const;
   long           DoSetNullMap(const void* ABuffer);

private:
   TKYList*       FItems;
   bool           FCanNull;
};

}

#endif
