// =======================================
// Unit   : 字符串类 (KYString.h)
// Version: 3.1.0.0 (build 2014.12.13)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYString_H_
#define _KYString_H_

#include <windows.h>
#include "KYObject.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// KYString 类声明
class       KYString;

// 初始化 KYString 单元
void        InitKYString();

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* KYString 字符串比较相关函数 */

// 得到子串的位置
// 例如: Pos("aa", "aabc") -> result = 1
//       Pos("aa", "abc")  -> result = 0
int         Pos(const KYString& ASubstr,   const KYString& AStr,
                           bool ACaseSensitive = true, int AFrom = 1);
int         Pos(const char* ASubstr,   const KYString& AStr,
                       bool ACaseSensitive = true, int AFrom = 1);
int         Pos(const KYString& ASubstr,       const char* AStr,
                           bool ACaseSensitive = true, int AFrom = 1);

// 字符串比较
int         CompareStr(const KYString& AStr1, const KYString& AStr2, bool ACaseSensitive = true);
int         CompareStr(const KYString& AStr1,     const char* AStr2, bool ACaseSensitive = true);
inline int  CompareStr(const char* AStr1,     const KYString& AStr2, bool ACaseSensitive = true)
            { return -CompareStr(AStr2, AStr1, ACaseSensitive); }

// 是否相同字符串
bool        SameStr(const KYString& AStr1, const KYString& AStr2, bool ACaseSensitive = true);
bool        SameStr(const KYString& AStr1,     const char* AStr2, bool ACaseSensitive = true);
inline bool SameStr(const char* AStr1,     const KYString& AStr2, bool ACaseSensitive = true)
            { return SameStr(AStr2, AStr1, ACaseSensitive); }

// 文本比较(不区分大小写)
inline int  CompareText(const KYString& AStr1,const KYString& AStr2)
            { return CompareStr(AStr1, AStr2, false); }
inline int  CompareText(const KYString& AStr1,    const char* AStr2)
            { return CompareStr(AStr1, AStr2, false); }
inline int  CompareText(const char* AStr1,    const KYString& AStr2)
            { return CompareStr(AStr1, AStr2, false); }

// 是否相同文本(不区分大小写)
inline bool SameText(const KYString& AStr1,   const KYString& AStr2)
            { return SameStr(AStr1, AStr2, false); }
inline bool SameText(const KYString& AStr1,       const char* AStr2)
            { return SameStr(AStr1, AStr2, false); }
inline bool SameText(const char* AStr1,       const KYString& AStr2)
            { return SameStr(AStr1, AStr2, false); }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* KYString - 字符串类 */

class KYString
{
   friend inline bool operator==(const KYString& AStr1, const KYString& AStr2)
                        { return SameStr(AStr1, AStr2, true); }
   friend inline bool operator==(const KYString& AStr1,     const char* AStr2)
                        { return SameStr(AStr1, AStr2, true); }
   friend inline bool operator==(const char* AStr1,     const KYString& AStr2)
                        { return SameStr(AStr1, AStr2, true); }

   friend inline bool operator!=(const KYString& AStr1, const KYString& AStr2)
                        { return !SameStr(AStr1, AStr2, true); }
   friend inline bool operator!=(const KYString& AStr1,     const char* AStr2)
                        { return !SameStr(AStr1, AStr2, true); }
   friend inline bool operator!=(const char* AStr1,     const KYString& AStr2)
                        { return !SameStr(AStr1, AStr2, true); }

   friend inline bool operator<=(const KYString& AStr1, const KYString& AStr2)
                        { return (CompareStr(AStr1, AStr2, true) <= 0); }
   friend inline bool operator<=(const KYString& AStr1,     const char* AStr2)
                        { return (CompareStr(AStr1, AStr2, true) <= 0); }
   friend inline bool operator<=(const char* AStr1,     const KYString& AStr2)
                        { return (CompareStr(AStr1, AStr2, true) <= 0); }

   friend inline bool operator>=(const KYString& AStr1, const KYString& AStr2)
                        { return (CompareStr(AStr1, AStr2, true) >= 0); }
   friend inline bool operator>=(const KYString& AStr1,     const char* AStr2)
                        { return (CompareStr(AStr1, AStr2, true) >= 0); }
   friend inline bool operator>=(const char* AStr1,     const KYString& AStr2)
                        { return (CompareStr(AStr1, AStr2, true) >= 0); }

   friend inline bool operator<(const KYString& AStr1, const KYString& AStr2)
                        { return (CompareStr(AStr1, AStr2, true) < 0); }
   friend inline bool operator<(const KYString& AStr1,     const char* AStr2)
                        { return (CompareStr(AStr1, AStr2, true) < 0); }
   friend inline bool operator<(const char* AStr1,     const KYString& AStr2)
                        { return (CompareStr(AStr1, AStr2, true) < 0); }

   friend inline bool operator>(const KYString& AStr1, const KYString& AStr2)
                        { return (CompareStr(AStr1, AStr2, true) > 0); }
   friend inline bool operator>(const KYString& AStr1,     const char* AStr2)
                        { return (CompareStr(AStr1, AStr2, true) > 0); }
   friend inline bool operator>(const char* AStr1,     const KYString& AStr2)
                        { return (CompareStr(AStr1, AStr2, true) > 0); }

   friend KYString operator+(const KYString& AStr1, const KYString& AStr2);
   friend KYString operator+(const KYString& AStr1,     const char* AStr2);
   friend KYString operator+(const KYString& AStr1,           char  AChar);
   friend KYString operator+(char AChar,            const KYString& AStr2);
   friend KYString operator+(const char* AStr1,     const KYString& AStr2);

   friend int  Pos(const KYString&, const KYString&, bool, int);
   friend int  Pos(const char*,     const KYString&, bool, int);
   friend int  Pos(const KYString&,     const char*, bool, int);

   friend int  CompareStr(const KYString&, const KYString&, bool);
   friend int  CompareStr(const KYString&,     const char*, bool);

   friend bool SameStr(const KYString&, const KYString&, bool);
   friend bool SameStr(const KYString&,     const char*, bool);

public:
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // KYString 项结构: <ReferenceCount(4)> + <Length(4)> + <content> + <end(1)>
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   #pragma pack(push, 1)
   typedef struct
   {
      long     RefCount;               // 引用计数
      int      Length;                 // 字符串长度
      char     Content[1];             // 这个长度不得修改, 刚好存放结尾'\0'字符
   } TItem, *PItem;
   #pragma pack(pop)

public:
   KYString();                         // 构造空字符串
   KYString(const KYString& AStr);     // 构造拷贝字符串
   KYString(const char* AStr);         // 构造拷贝'\0'字符结尾的字符串
   KYString(char AChar);               // 构造指定字符的字符串
   KYString(int  ALength);             // 构造指定长度的字符串, 但内容未初始化
   KYString(long ALength);             // 构造指定长度的字符串, 但内容未初始化
   KYString(char AChar, int ACount);   // 构造指定字符及个数的字符串
   KYString(const char* AStr, int ALength);// 构造拷贝从 AStr 开始的 ALength 个字符的字符串

   // 析构函数
   virtual ~KYString();

   // 赋值操作
   KYString&   operator=(const KYString& AStr);
   KYString&   operator=(const char* AStr);
   KYString&   operator=(char AChar);

   // 字符串连接操作
   KYString&   operator+=(const KYString& AStr);
   KYString&   operator+=(const char* AStr);
   KYString&   operator+=(char AChar);

   // 取位置的字符, APos 取值范围为: [1..Length()]
   char        operator[](int APos) const { return FData->Content[APos - 1]; }

   // 设置位置的字符, APos 取值范围为: [1..Length()]
   void        SetChar(int APos, char AChar);

   // 类型转换, 支持 (const char*) 和 (char*) 转换, 两者差别是当字符串
   // 为空串时, (const char*) 返回 "" 字符串指针, 而 (char*) 返回 NULL
   operator    const char* () const    { return (FData == NULL) ? Empty_C_Str : FData->Content;}
   operator    char* () const          { return (FData == NULL) ? NULL : FData->Content; }

   // 取字符串长度
   int         Length() const          { return (FData == NULL) ? 0 : FData->Length; }

   // 设置字符串长度, 相当于分配指定长度的缓冲区
   // 1. 若 ALength < Length() 时, 自动截断后面的内容
   // 2. 否则保留 Length() 字符串内容, 同时后面的内容未初始化
   void        SetLength(int ALength);

   // 类型转换函数, 功能与 (char*) 相似
   // 1. 若 String 的引用计数为 1, 则与 (char*) 相同
   // 2. 否则重新克隆一个新的字符串并返回首地址
   char*       c_str();

   // 检查字符串是否为数字串, 即都由 ['0'..'9'] 字符组成
   bool        IsNumber() const;

   // 检查字符串是否为标识符, 即首字符为 {'_', ['a'..'z'], ['A'..'Z']} 组成,
   // 后面字符可以包含 ['0'..'9'] 字符
   bool        IsValidIdent() const;

   // 转换成小写字母, 例如: "AbC" -> "abc"
   KYString&   ToLowerCase();

   // 转换成大写字母, 例如: "AbC" -> "ABC"
   KYString&   ToUpperCase();

   // 转换成大小写反向字符, 例如: "AbC" -> "aBc"
   KYString&   ToInverseCase();

   // 去除两边空白字符, 例如: " \x0D\x0A AbC " -> "AbC"
   KYString&   Trim();

   // 去除左边空白字符, 例如: " \x0D\x0A AbC " -> "AbC "
   KYString&   TrimLeft();

   // 去除右边空白字符, 例如: " \x0D\x0A AbC " -> " \x0D\x0A AbC"
   KYString&   TrimRight();

   // 去除两边指定字符, 例如: AChar = ' ', " \x0D\x0A AbC " -> "\x0D\x0A AbC"
   KYString&   TrimChar(char AChar = ' ');

   // 去除左边指定字符, 例如: AChar = ' ', " \x0D\x0A AbC " -> "\x0D\x0A AbC "
   KYString&   TrimCharLeft(char AChar = ' ');

   // 去除右边指定字符, 例如: AChar = ' ', " \x0D\x0A AbC " -> " \x0D\x0A AbC"
   KYString&   TrimCharRight(char AChar = ' ');

   // 剩下, 例如: Remain("abcd1234", 4, 4) -> "d123"
   KYString&   Remain(int AFrom, int ACount);

   // 删除, 例如: Delete("abcd1234", 4, 4) -> "abc4"
   KYString&   Delete(int AFrom, int ACount);

   // 清除字符串, 即字符串置空
   KYString&   Clear()                 { _UpdateData(NULL); return *this; }

   // 插入, 例如: Insert("aa", "abcd1234", 4) -> "abcaad1234"
   KYString&   Insert(const KYString& ASubstr, int APos);
   KYString&   Insert(const char* ASubstr, int ALength, int APos);
   KYString&   Insert(const char* ASubstr, int APos);
   KYString&   Insert(char AChar, int APos);

   // 初始化指定字符的字符串, 例如: StringOfChar('a', 5) -> "aaaaa"
   KYString&   StringOfChar(char AChar, int ACount);

   // 拷贝字符串, 例如: Copy("abcd1234", 4, 10) -> "d1234"
   KYString&   Copy(const KYString& AStr, int AFrom, int ACount);
   KYString&   Copy(const char* AStr, int AFrom, int ACount);  // 注: 检查'\0'结束符
   KYString&   Copy(const char* AStr, int ACount)
               { return Copy(AStr, 1, ACount); }

   // 设置字符串, 拷贝从 AStr 开始的 ALength 个字符的字符串(注: 不检查'\0'结束符)
   KYString&   SetString(const char* AStr, int ALength);

   // 格式化串(参数值不能为 KYString 类型, 必须使用 (const char*) 转换)
   // Format 格式串与 printf 格式串相同
   KYString&   Format(KYString AFormat, ...);
   KYString&   Format(const char* AFormat, ...);

protected:
   void        _UpdateData(PItem AData);

   PItem       _AddRef() const;
   PItem       _AddRef(long& ARefCount) const;

   static long _ReleaseRef(PItem AData);
   static long _ReleaseRefEx(PItem& AData);

private:
   PItem       FData;

private:
   // 指向空字符串指针
   static const char* const Empty_C_Str;  // = ""

   // KYString 的静态成员初始化类
   static class TInitialization
   {
   public:
      TInitialization();
      ~TInitialization();
   } _Initialization;

   friend class TInitialization;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* MBCS functions - 多字节字符集函数 */

enum TMbcsByteType {mbSingleByte, mbLeadByte, mbTrailByte};

// 指定索引的字节类型, AIndex = 1..Length
TMbcsByteType ByteType(const char* AStr, long AIndex);
TMbcsByteType ByteType(const KYString& AStr, long AIndex);

// 得到最后一个字符
char* GetLastChar(const KYString& AStr);

/* StrCharLength returns the number of bytes required by the first character
   in Str.  In Windows, multibyte characters can be up to two bytes in length.
   In Linux, multibyte characters can be up to six bytes in length (UTF-8). */

int   StrCharLength(const char* AStr);

/* ByteToCharLen returns the character length of a MBCS string, scanning the
   string for up to MaxLen bytes.  In multibyte character sets, the number of
   characters in a string may be less than the number of bytes. */

long  ByteToCharLen(const KYString& AStr, long AMaxLen);

/* CharToByteLen returns the byte length of a MBCS string, scanning the string
   for up to MaxLen characters. */

long  CharToByteLen(const KYString& AStr, long AMaxLen);

/* ByteToCharIndex returns the 1-based character index of the Index'th byte in
   a MBCS string.  Returns zero if Index is out of range:
   (Index <= 0) or (Index > Length(S)) */

long  ByteToCharIndex(const KYString& AStr, long AIndex);

/* CharToByteIndex returns the 1-based byte index of the Index'th character
   in a MBCS string.  Returns zero if Index or Result are out of range:
   (Index <= 0) or (Index > Length(S)) or (Result would be > Length(S)) */

long  CharToByteIndex(const KYString& AStr, long AIndex);

// 取系统错误信息
KYString SysErrorMessage(unsigned long AErrCode);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 字符串函数 */

// 数字/十六进制数字/字母序数表
extern const char Offset_IsDigit[256];
extern const char Offset_IsHexDigit[256];
extern const char Offset_IsLetter[256];

// 大小写 ASCII 码表
extern const char ASCIIMap_Lower[256];
extern const char ASCIIMap_Upper[256];
extern const char ASCIIMap_Inverse[256];

// 是否为指定类型字符宏
#define     __IsDigitChar(ch)          (Offset_IsDigit[(unsigned char)(ch)] != 0)
#define     __IsHexDigitChar(ch)       (Offset_IsHexDigit[(unsigned char)(ch)] != 0)
#define     __IsLetterChar(ch)         (Offset_IsLetter[(unsigned char)(ch)] != 0)

#define     __IsNotDigitChar(ch)       (Offset_IsDigit[(unsigned char)(ch)] == 0)
#define     __IsNotHexDigitChar(ch)    (Offset_IsHexDigit[(unsigned char)(ch)] == 0)
#define     __IsNotLetterChar(ch)      (Offset_IsLetter[(unsigned char)(ch)] == 0)

// 得到大小写字符
inline char LowerCaseChar(char AChar)  { return ASCIIMap_Lower[(unsigned char)AChar]; }
inline char UpperCaseChar(char AChar)  { return ASCIIMap_Upper[(unsigned char)AChar]; }
inline char InverseCaseChar(char AChar){ return ASCIIMap_Inverse[(unsigned char)AChar]; }

// 得到目标串的 KMP 的回溯表
// 注: Length(ANext) 必须大于等于 ADestLen
bool GetKMPNext(const char* ADest, int ADestLen, int* ANext,
                       bool ACaseSensitive = true);

// _FindNext, _FindNextEx 与 FindNext, FindNextEx 相同, 只是不检查参数的合法性
// 注: AText 和 ADest 非空, ADestLen > 0, ANextPos > 0, AFrom > 0
int  _FindNext(const char* AText, int ATextLen,
               const char* ADest, int ADestLen,
                      int* ANext, int ANextPos = 1,
                       int AFrom = 1, bool ACaseSensitive = true);
int  _FindNextEx(const char* AText, int ATextLen,
                 const char* ADest, int ADestLen,
                        int* ANext, int& ANextPos,
                         int AFrom = 1, bool ACaseSensitive = true);

// 根据 KMP 回溯表查找下一个匹配项
// 注: ANext 回溯表的 ACaseSensitive 必须与当前的 ACaseSensitive 参数相同,
//     FindNextEx 为了能够返回 ANextPos 比 FindNext 要更多比较, 所以效率要
//     相对差一些
int  FindNext(const char* AText, int ATextLen,
              const char* ADest, int ADestLen,
                     int* ANext, int ANextPos = 1,
                     int  AFrom = 1, int ATo = -1,
                     bool ACaseSensitive = true);
int  FindNextEx(const char* AText, int ATextLen,
                const char* ADest, int ADestLen,
                       int* ANext, int& ANextPos,
                       int  AFrom = 1, int ATo = -1,
                       bool ACaseSensitive = true);

// 得到目标串的位置(ADest 在 AText 中的第一个位置)
int  StrPos(const char* ADest, int ADestLen,
            const char* AText, int ATextLen,
                   bool ACaseSensitive = true, int AFrom = 1);

// 字符串比较
int  StrCompare(const char* AStr1, const char* AStr2, bool ACaseSensitive = true);
int  StrCompare(const char* AStr1, int ALen1,
                const char* AStr2, int ALen2,
                      bool ACaseSensitive = true);

// 判断字符串内容是否相同
bool IsSameStr(const char* AStr1, const char* AStr2, bool ACaseSensitive = true);
bool IsSameStr(const char* AStr1, int ALen1,
               const char* AStr2, int ALen2,
                      bool ACaseSensitive = true);
inline bool IsSameStr(const char* AStr1, const char* AStr2,
                              int ALen, bool ACaseSensitive = true)
            { return IsSameStr(AStr1, ALen, AStr2, ALen, ACaseSensitive); }

// 扫描出现第一个字符的指针
char* StrScan(const char* AStr, char AChar);

// 扫描出现最后一个字符的指针
char* StrRScan(const char* AStr, char AChar);

// 转换成大写字符串
char* StrUpper(char* AStr);

// 转换成小写字符串
char* StrLower(char* AStr);

// 字符串带最大长度限制的拷贝
// 1. ADest 必须: ADest != NULL, 且 AMaxLen >= 0
// 2. ADest 已分配的空间尺寸必须: size(ADest) >= AMaxLen + 1
// 3. ADest 拷贝后末尾自动添加 '\0' 字符
char* StrLCopy(char* ADest, const char* AStr, long AMaxLen);
char* StrLCopy(char* ADest, const KYString& AStr, long AMaxLen);

// 字符串带最大尺寸的拷贝
// 1. ADest 必须: ADest != NULL, 且 AMaxSize > 0
// 2. ADest 已分配的空间尺寸必须: size(ADest) >= AMaxSize
// 3. ADest 当 Length(AStr) < AMaxSize 时末尾自动添加 '\0' 字符
char* StrNCopy(char* ADest, const char* AStr, long AMaxSize);
char* StrNCopy(char* ADest, const KYString& AStr, long AMaxSize);

// 在 AStr 中查找 AOld 串, 并使用 ANew 替换
// 注: SameStr(AOld, ANew, true) 必须为 false, 内部不检查是否相同
// 如: StringReplace("aabbccddbbcc", "bc", "x", true) 返回值为: "aabxcddbxc"
KYString StringReplace(const char* AStr, long ALength,
                       const char* AOld, long AOldLen,
                       const char* ANew, long ANewLen,
                             bool  AReplaceAll     = false,
                             bool  ACaseSensitive  = true);
KYString StringReplace(const KYString& AStr, const char* AOld, long AOldLen,
                                             const char* ANew, long ANewLen,
                                                   bool  AReplaceAll     = false,
                                                   bool  ACaseSensitive  = true);
inline KYString StringReplace(const KYString& AStr, const KYString& AOld,
                              const KYString& ANew, bool  AReplaceAll    = false,
                                                    bool  ACaseSensitive = true)
{
   return StringReplace(AStr, (char*)AOld, AOld.Length(),
                              (char*)ANew, ANew.Length(),
                              AReplaceAll, ACaseSensitive);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* KYString 相关函数 */

// 扫描整数值, 返回扫描结果长度, 若返回为 0 则表示不是整数值
int         ScanInteger(const char* AStr, int ALength, bool& ASigned);
inline int  ScanInteger(const KYString& AStr, bool& ASigned)
            { return ScanInteger((char*)AStr, AStr.Length(), ASigned); }

// 扫描数值, 返回扫描结果长度, 若返回为 0 则表示不是数值
int         ScanNumber(const char* AStr, int ALength, bool& ASigned, bool& AIsFloat);
inline int  ScanNumber(const KYString& AStr, bool& ASigned, bool& AIsFloat)
            { return ScanNumber((char*)AStr, AStr.Length(), ASigned, AIsFloat); }

// 判断是否为数字串
bool        IsNumber(const char* AStr, int ALength);
inline bool IsNumber(const KYString& AStr)
            { return AStr.IsNumber(); }

// 判断是否为有效标识符
bool        IsValidIdent(const char* AIdent, int ALength);
inline bool IsValidIdent(const KYString& AIdent)
            { return AIdent.IsValidIdent(); }

// 取字符串长度
inline int  Length(const char* AStr)
            { return (AStr == NULL) ? 0 : strlen(AStr); }
inline int  Length(const KYString& AStr)
            { return AStr.Length(); }

// 设置字符串长度, 相当于分配指定长度的缓冲区
// 1. 若 ALength < Length() 时, 自动截断后面的内容
// 2. 否则保留 Length() 字符串内容, 同时后面的内容未初始化
inline void SetLength(KYString& AStr, int ALength)
            { AStr.SetLength(ALength); }

// 转换成小写字母, 例如: "AbC" -> "abc"
inline KYString LowerCase(const KYString& AStr)
{
   KYString result(AStr);
   return result.ToLowerCase();
}

// 转换成大写字母, 例如: "AbC" -> "ABC"
inline KYString UpperCase(const KYString& AStr)
{
   KYString result(AStr);
   return result.ToUpperCase();
}

// 转换成大小写反向字符, 例如: "AbC" -> "aBc"
inline KYString InverseCase(const KYString& AStr)
{
   KYString result(AStr);
   return result.ToInverseCase();
}

// 去除两边空白字符, 例如: " \x0D\x0A AbC " -> "AbC"
inline KYString Trim(const KYString& AStr)
{
   KYString result(AStr);
   return result.Trim();
}

// 去除左边空白字符, 例如: " \x0D\x0A AbC " -> "AbC "
inline KYString TrimLeft(const KYString& AStr)
{
   KYString result(AStr);
   return result.TrimLeft();
}

// 去除右边空白字符, 例如: " \x0D\x0A AbC " -> " \x0D\x0A AbC"
inline KYString TrimRight(const KYString& AStr)
{
   KYString result(AStr);
   return result.TrimRight();
}

// 去除两边指定字符, 例如: AChar = ' ', " \x0D\x0A AbC " -> "\x0D\x0A AbC"
inline KYString TrimChar(const KYString& AStr, char AChar = ' ')
{
   KYString result(AStr);
   return result.TrimChar(AChar);
}

// 去除左边指定字符, 例如: AChar = ' ', " \x0D\x0A AbC " -> "\x0D\x0A AbC "
inline KYString TrimCharLeft(const KYString& AStr, char AChar = ' ')
{
   KYString result(AStr);
   return result.TrimCharLeft(AChar);
}

// 去除右边指定字符, 例如: AChar = ' ', " \x0D\x0A AbC " -> " \x0D\x0A AbC"
inline KYString TrimCharRight(const KYString& AStr, char AChar = ' ')
{
   KYString result(AStr);
   return result.TrimCharRight(AChar);
}

// 设置字符串, 拷贝从 AStr 开始的 ALength 个字符的字符串
inline KYString SetString(const char* AStr, int ALength)
{
   KYString result;
   return result.SetString(AStr, ALength);
}

// 初始化指定字符的字符串, 例如: StringOfChar('a', 5) -> "aaaaa"
inline KYString StringOfChar(char AChar, int ACount)
{
   KYString result;
   return result.StringOfChar(AChar, ACount);
}

// 拷贝字符串, 例如: Copy("abcd1234", 4, 10) -> "d1234"
inline KYString Copy(const KYString& AStr, int AFrom, int ACount)
{
   KYString result;
   return result.Copy(AStr, AFrom, ACount);
}

// 拷贝字符串, 例如: Copy("abcd1234", 4, 10) -> "d1234"
inline KYString Copy(const char* AStr, int AFrom, int ACount)
{
   KYString result;
   return result.Copy(AStr, AFrom, ACount);
}

// 拷贝字符串, 例如: Copy("abcd1234", 4) -> "abcd"
inline KYString Copy(const char* AStr, int ACount)
{
   KYString result;
   return result.Copy(AStr, ACount);
}

// 剩下, 例如: Remain("abcd1234", 4, 4) -> "d123"
inline void Remain(KYString& AStr, int AFrom, int ACount)
            { AStr.Remain(AFrom, ACount); }

// 删除, 例如: Delete("abcd1234", 4, 4) -> "abc4"
inline void Delete(KYString& AStr, int AFrom, int ACount)
            { AStr.Delete(AFrom, ACount); }

// 插入, 例如: Insert("aa", "abcd1234", 4) -> "abcaad1234"
inline void Insert(const KYString& ASubstr, KYString& AStr, int APos)
            { AStr.Insert(ASubstr, APos); }
inline void Insert(const char* ASubstr, int ALength, KYString& AStr, int APos)
            { AStr.Insert(ASubstr, ALength, APos); }
inline void Insert(const char* ASubstr, KYString& AStr, int APos)
            { AStr.Insert(ASubstr, APos); }
inline void Insert(char AChar, KYString& AStr, int APos)
            { AStr.Insert(AChar, APos); }

// 格式化串(参数值不能为 KYString 类型, 必须使用 (const char*) 转换)
// Format 格式串与 printf 格式串相同
KYString    Format(KYString AFormat, ...);
KYString    Format(const char* AFormat, ...);

// 整型转换成二进制/十六进制串
KYString&   IntToBin(int AValue,         KYString& AStr);
KYString&   IntToBin(__int64 AValue,     KYString& AStr);
KYString&   IntToHex(int AValue,         KYString& AStr);
KYString&   IntToHex(__int64 AValue,     KYString& AStr);

// 数值转换成字符串
KYString&   IntToStr(int AValue,         KYString& AStr);
KYString&   IntToStr(__int64 AValue,     KYString& AStr);
KYString&   FloatToStr(double AValue,    KYString& AStr);

// 整型转换成二进制/十六进制串
KYString    IntToBin(int AValue);
KYString    IntToBin(__int64 AValue);
KYString    IntToHex(int AValue);
KYString    IntToHex(__int64 AValue);

// 数值转换成字符串
KYString    IntToStr(int AValue);
KYString    IntToStr(__int64 AValue);
KYString    FloatToStr(double AValue);

// 整型转换成带位数的二进制/十六进制/十进制串
KYString    IntToBin(int AValue,         int ADigits);
KYString    IntToBin(__int64 AValue,     int ADigits);
KYString    IntToHex(int AValue,         int ADigits);
KYString    IntToHex(__int64 AValue,     int ADigits);
KYString    IntToStr(int AValue,         int ADigits);
KYString    IntToStr(__int64 AValue,     int ADigits);

// long 转换成字符串
inline KYString& IntToBin(long AValue,   KYString& AStr)
                 { return IntToBin((int)AValue, AStr); }
inline KYString& IntToHex(long AValue,   KYString& AStr)
                 { return IntToHex((int)AValue, AStr); }
inline KYString& IntToStr(long AValue,   KYString& AStr)
                 { return IntToStr((int)AValue, AStr); }
inline KYString  IntToBin(long AValue,   int ADigits)
                 { return IntToBin((int)AValue, ADigits); }
inline KYString  IntToHex(long AValue,   int ADigits)
                 { return IntToHex((int)AValue, ADigits); }
inline KYString  IntToStr(long AValue,   int ADigits)
                 { return IntToStr((int)AValue, ADigits); }
inline KYString  IntToStr(long AValue)
                 { return IntToStr((int)AValue); }

// 字符串转整型值(若 ACode = 0 则成功, 否则返回错误位置)
// 例如: "123.3"        -> result = 123, ACode = 4
//       "123456"       -> result = 123456, ACode = 0
//       "12345678901"  -> result = 1234567890, ACode = 11
long        _ValLong(const char* AStr,   int& ACode);
long        _ValLong(const char* AStr,   int  ALength, int& ACode);

// 字符串转64位整型值(若 ACode = 0 则成功, 否则返回错误位置)
// 例如: "123.3"        -> result = 123, ACode = 4
//       "123456"       -> result = 123456, ACode = 0
//       "12345678901"  -> result = 12345678901, ACode = 0
__int64     _ValInt64(const char* AStr,  int& ACode);
__int64     _ValInt64(const char* AStr,  int  ALength, int& ACode);

// 字符串转整型值, 若 AStr 转换失败则返回值为 0
int         StrToInt(const char* AStr);
int         StrToInt(const KYString& AStr);

// 字符串转64位整型值, 若 AStr 转换失败则返回值为 0
__int64     StrToInt64(const char* AStr);
__int64     StrToInt64(const KYString& AStr);

// 字符串转整型值, 若 AStr 转换失败则返回值为 ADefault
int         StrToIntDef(const char* AStr,     int ADefault);
int         StrToIntDef(const KYString& AStr, int ADefault);

// 字符串转64位整型值, 若 AStr 转换失败则返回值为 ADefault
__int64     StrToInt64Def(const char* AStr,     __int64 ADefault);
__int64     StrToInt64Def(const KYString& AStr, __int64 ADefault);

// 转换成浮点数
double      StrToFloat(const char* AStr);
double      StrToFloat(const KYString& AStr);

}

#endif
