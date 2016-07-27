// =======================================
// Unit   : 可变类型类 (KYVarType.h)
// Version: 3.0.0.0 (build 2012.06.04)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYVarType_H_
#define _KYVarType_H_

#include "KYDateTime.h"
#include "KYMemBlock.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYVarType - 可变类型类 */

class TKYVarType
{
public:
   // 变量类型
   enum TType       {vtUnknown,        // 0:  (未知)
                     vtBoolean,        // 1:  布尔类型
                     vtByte,           // 2:  字节类型
                     vtWord,           // 3:  双字节类型
                     vtLongword,       // 4:  四字节类型
                     vtShortint,       // 5:  单字节整型
                     vtSmallint,       // 6:  双字节整型
                     vtLongint,        // 7:  四字节整型
                     vtInt64,          // 8:  八字节整型
                     vtInteger,        // 9:  整型
                     vtSingle,         // 10: 单精度浮点型
                     vtDouble,         // 11: 双精度浮点型
                     vtCurrency,       // 12: 金额类型
                     vtDateTime,       // 13: 日期时间类型
                     vtString,         // 14: 字符串类型
                     vtPointer};       // 15: 指针类型

   // 可变数据项
   #pragma pack(push, 1)
   typedef struct
   {
      Byte           Type;             // 类型
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
         Byte        Bytes[8];         // 值的缓冲区
      };
   } TVarData, *PVarData;
   #pragma pack(pop)

public:
   TKYVarType(TType AType = vtUnknown);
   virtual ~TKYVarType();

   // '=' 操作符重载, 若 Type() == vtUnknown, 则第一次赋值时自动设置相应的类型
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

   // 变量类型
   TType       Type() const            { return (TType)FData->Type; }

   // 可变数据值
   TVarData*   Value() const           { return FData; }

   // 判断是是否为空值
   bool        IsNull() const          { return (FData->VInt64 == 0)
                                             || ((FData->Type == vtString)
                                                && (FData->VString->Length() == 0)); }

   // 设置变量类型, 并自动转换值
   void        SetType(TType AType);

   // 设置可变数据值, 并自动转换值
   bool        SetValue(const TVarData* AValue);

   // 读变量值属性
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

   // 设置变量值属性
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

   // 清除值, 变量类型不变
   void        Clear();

   // 清空数据, 且类型置为 vtUnknown
   void        Empty();

   // 从缓冲区加载值(vtString 类型除外)
   long        LoadFrom(const void* ABuffer);

   // 保存值到缓冲区(vtString 类型除外)
   long        SaveTo(void* ABuffer);

   // 交换指针(只用于vtUnknown 和 vtPointer 类型)
   void*       ExchangePointer(const void* AValue);

protected:
private:
   TVarData*   FData;

public:
   // 判断整型值存放是否为: 低->高
   static const union TInt32_Union
   {
      long           Value;
      Byte           Bytes[4];
   } Int32_Memory;                        // Int32_Memory.Value = 0x03020100;

   // 静态成员
   static const TVarData Null_Var;        // TVarData 类型空值
   static const TType    Type_MaxValue;   // 15: vtPointer 最大类型值
   static const Word     Type_Sizes[16];  // 类型尺寸
   static const KYString Type_Names[16];  // 类型名
   static const KYString Bool_String[2];  // 布尔的字符串值

public:
   // 分配和释放可变数据
   static TVarData* VarNew()              { return (TVarData*)VarData_Manager->New(); }
   static void VarDelete(TVarData* AData) { VarData_Manager->Delete(AData); }

   // 数据转换
   static bool VarCast(TVarData& ADest, const TVarData& ASource, TType ADestType);
   static bool VarCast(TVarData& ADest, const TVarData& ASource,
                           TType ADestType,   KYString& AError);

   // 拷贝数据
   static void VarCopy(TVarData& ADest, const TVarData& ASource);

   // 清空数据
   static void VarEmpty(TVarData& AValue);

private:
   static void DoInitVarData(TVarData* AData, Word ASize);
   static void DoFreeVarData(TVarData* AData, Word ASize);

   // 可变数据项管理器, 存放可变数据项 TVarData
   static TKYMemBlock*  VarData_Manager;

   // TKYVarType 的静态成员初始化类
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
