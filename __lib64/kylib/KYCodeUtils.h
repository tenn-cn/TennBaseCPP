// =======================================
// Unit   : 编码单元 (KYCodeUtils.h)
// Version: 3.0.1.0 (build 2014.12.14)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYCodeUtils_H_
#define _KYCodeUtils_H_

#include "KYString.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 随机数函数 */

// 初始化随机数
void              Randomize();

// 生成随机数, 注: AModule > 1
Longword          Random();
inline Longword   Random(Longword AModule)
                  { return Random() % AModule; }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ASCII/Hex 编码 */

// ASCII 码转换成十六进制码, 例如: ASCIIToHex("ABC") -> "414243"
KYString          ASCIIToHex(const char* AStr, long ALength);
long              ASCIIToHex(const char* AStr, char* AHex, long AMaxSize);
long              ASCIIToHex(const char* AStr, long ALength,
                                   char* AHex, long AMaxSize);
inline KYString   ASCIIToHex(const char* AStr)
                  { return ASCIIToHex(AStr, Length(AStr)); }
inline KYString   ASCIIToHex(const KYString& AStr)
                  { return ASCIIToHex((char*)AStr, AStr.Length()); }
inline long       ASCIIToHex(const KYString& AStr, char* AHex, long AMaxSize)
                  { return ASCIIToHex((char*)AStr, AStr.Length(), AHex, AMaxSize); }

// 十六进制码转换成 ASCII 码, 例如: HexToASCII("414243") -> "ABC"
KYString          HexToASCII(const char* AHex, long ALength);
long              HexToASCII(const char* AHex, char* AStr, long AMaxSize);
long              HexToASCII(const char* AHex, long ALength,
                                   char* AStr, long AMaxSize);
inline KYString   HexToASCII(const char* AHex)
                  { return HexToASCII(AHex, Length(AHex)); }
inline KYString   HexToASCII(const KYString& AHex)
                  { return HexToASCII((char*)AHex, AHex.Length()); }
inline long       HexToASCII(const KYString& AHex, char* AStr, long AMaxSize)
                  { return HexToASCII((char*)AHex, AHex.Length(), AStr, AMaxSize); }

// 字符串转换成用空格分隔的十六进制串, 例如: StrToHex("ABC") -> "41 42 43"
KYString          StrToHex(const char* AStr, long ALength);
long              StrToHex(const char* AStr, char* AHex, long AMaxSize);
long              StrToHex(const char* AStr, long ALength,
                                 char* AHex, long AMaxSize);
inline KYString   StrToHex(const char* AStr)
                  { return StrToHex(AStr, Length(AStr)); }
inline KYString   StrToHex(const KYString& AStr)
                  { return StrToHex((char*)AStr, AStr.Length()); }
inline long       StrToHex(const KYString& AStr, char* AHex, long AMaxSize)
                  { return StrToHex((char*)AStr, AStr.Length(), AHex, AMaxSize); }

// 带分隔符十六进制串转换成字符串, 例如: HexToStr("41 42 43") -> "ABC"
KYString          HexToStr(const char* AHex, long ALength);
long              HexToStr(const char* AHex, char* AStr, long AMaxSize);
long              HexToStr(const char* AHex, long ALength,
                                 char* AStr, long AMaxSize);
inline KYString   HexToStr(const char* AHex)
                  { return HexToStr(AHex, Length(AHex)); }
inline KYString   HexToStr(const KYString& AHex)
                  { return HexToStr((char*)AHex, AHex.Length()); }
inline long       HexToStr(const KYString& AHex, char* AStr, long AMaxSize)
                  { return HexToStr((char*)AHex, AHex.Length(), AStr, AMaxSize); }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* Unicode/UTF8 编码 */

// 取 unicode-string 长度
long        WideLength(const char* AUnicode);
inline long WideLength(const char* AUnicode, long ASize)
            { return (AUnicode == NULL) || (ASize <= 1) ? 0 : (ASize >> 1); }
inline long WideLength(const KYString& AUnicode)
            { return AUnicode.Length() >> 1; }

// 取 UTF8-string 长度(扫描到非法编码即中止, 并返回有效长度)
long        UTF8Length(const char* AUTF8, long ASize);
inline long UTF8Length(const char* AUTF8)
            { return UTF8Length(AUTF8, Length(AUTF8)); }
inline long UTF8Length(const KYString& AUTF8)
            { return UTF8Length((char*)AUTF8, AUTF8.Length()); }

// ANSI -> Unicode 字符串
// 1. 若 ADest == NULL, 则返回所需的 ADestSize 和 unicode-string 长度
// 2. 必须 ADestSize >= ASize + ASize, 且返回 ADest 占用尺寸
// 3. 返回 ADest 不含末尾 0 字符
// 4. 返回 ADest 的 unicode-string 长度, 若返回为 -1 则表示 ADestSize 太小
long        AnsiToUnicode(const char* AStr,  long  ASize,
                                char* ADest, long& ADestSize);
long        AnsiToUnicode(const KYString& AStr, KYString& ADest);

inline long AnsiToUnicode(const char* AStr, char* ADest, long& ADestSize)
{
   return AnsiToUnicode(AStr, Length(AStr), ADest, ADestSize);
}

inline long AnsiToUnicode(const KYString& AStr, char* ADest, long& ADestSize)
{
   return AnsiToUnicode((char*)AStr, AStr.Length(), ADest, ADestSize);
}

inline KYString AnsiToUnicode(const KYString& AStr)
{
   KYString result;
   AnsiToUnicode(AStr, result);
   return result;
}

// Unicode -> ANSI 字符串
// 1. 若 ADest == NULL, 则返回 ANSI-string 长度
// 2. 必须 ADestSize >= ASize
// 3. 返回 ADest 不含末尾 0 字符
// 4. 返回 ADest 的 ANSI-string 长度, 若返回为 -1 则表示 ADestSize 太小
long        UnicodeToAnsi(const char* AUnicode, long ASize,
                                char* ADest,    long ADestSize);
long        UnicodeToAnsi(const KYString& AUnicode, KYString& ADest);

inline long UnicodeToAnsi(const char* AUnicode, char* ADest, long ADestSize)
{
   long intLen = WideLength(AUnicode);
   return UnicodeToAnsi(AUnicode, intLen + intLen, ADest, ADestSize);
}

inline long UnicodeToAnsi(const KYString& AUnicode, char* ADest, long ADestSize)
{
   return UnicodeToAnsi((char*)AUnicode, AUnicode.Length(), ADest, ADestSize);
}

inline KYString UnicodeToAnsi(const KYString& AUnicode)
{
   KYString result;
   UnicodeToAnsi(AUnicode, result);
   return result;
}

// ANSI -> UTF8 字符串
// 1. 若 ADest == NULL, 则返回所需的 ADestSize 和 UTF8-string 长度
// 2. 必须 ADestSize >= ASize + ASize, 且返回 ADest 占用尺寸
// 3. 返回 ADest 不含末尾 0 字符
// 4. 返回 ADest 的 UTF8-string 长度, 若返回为 -1 则表示 ADestSize 太小
long        AnsiToUTF8(const char* AStr,  long  ASize,
                             char* ADest, long& ADestSize);
long        AnsiToUTF8(const KYString& AStr, KYString& ADest);

inline long AnsiToUTF8(const char* AStr, char* ADest, long& ADestSize)
{
   return AnsiToUTF8(AStr, Length(AStr), ADest, ADestSize);
}

inline long AnsiToUTF8(const KYString& AStr, char* ADest, long& ADestSize)
{
   return AnsiToUTF8((char*)AStr, AStr.Length(), ADest, ADestSize);
}

inline KYString AnsiToUTF8(const KYString& AStr)
{
   KYString result;
   AnsiToUTF8(AStr, result);
   return result;
}

// UTF8 -> ANSI 字符串
// *. 若 AUTF8 扫描到非法编码即中止, 即只转换有效尺寸
// 1. 若 ADest == NULL, 则返回 ANSI-string 长度
// 2. 必须 ADestSize >= ASize
// 3. 返回 ADest 不含末尾 0 字符
// 4. 返回 ADest 的 ANSI-string 长度, 若返回为 -1 则表示 ADestSize 太小
long        UTF8ToAnsi(const char* AUTF8, long ASize,
                             char* ADest, long ADestSize);
long        UTF8ToAnsi(const KYString& AUTF8, KYString& ADest);

inline long UTF8ToAnsi(const char* AUTF8, char* ADest, long ADestSize)
{
   return UTF8ToAnsi(AUTF8, Length(AUTF8), ADest, ADestSize);
}

inline long UTF8ToAnsi(const KYString& AUTF8, char* ADest, long ADestSize)
{
   return UTF8ToAnsi((char*)AUTF8, AUTF8.Length(), ADest, ADestSize);
}

inline KYString UTF8ToAnsi(const KYString& AUTF8)
{
   KYString result;
   UTF8ToAnsi(AUTF8, result);
   return result;
}

// Unicode -> UTF8 字符串
// 1. 若 ADest == NULL, 则返回所需的 ADestSize 和 UTF8-string 长度
// 2. 必须 ADestSize >= ASize + (ASize >> 1), 且返回 ADest 占用尺寸
// 3. 返回 ADest 不含末尾 0 字符
// 4. 返回 ADest 的 UTF8-string 长度, 若返回为 -1 则表示 ADestSize 太小
long        UnicodeToUTF8(const char* AUnicode, long  ASize,
                                char* ADest,    long& ADestSize);
long        UnicodeToUTF8(const KYString& AUnicode, KYString& ADest);

inline long UnicodeToUTF8(const char* AUnicode, char* ADest, long& ASize)
{
   long intLen = WideLength(AUnicode);
   return UnicodeToUTF8(AUnicode, intLen + intLen, ADest, ASize);
}

inline long UnicodeToUTF8(const KYString& AUnicode, char* ADest, long& ASize)
{
   return UnicodeToUTF8((char*)AUnicode, AUnicode.Length(), ADest, ASize);
}

inline KYString UnicodeToUTF8(const KYString& AUnicode)
{
   KYString result;
   UnicodeToUTF8(AUnicode, result);
   return result;
}

// UTF8 -> Unicode 字符串
// *. 若 AUTF8 扫描到非法编码即中止, 即只转换有效尺寸
// 1. 若 ADest == NULL, 则返回所需的 ADestSize 和 unicode-string 长度
// 2. 必须 ADestSize >= ASize + ASize, 且返回 ADest 占用尺寸
// 3. 返回 ADest 不含末尾 0 字符
// 4. 返回 ADest 的 unicode-string 长度, 若返回为 -1 则表示 ADestSize 太小
long        UTF8ToUnicode(const char* AUTF8, long  ASize,
                                char* ADest, long& ADestSize);
long        UTF8ToUnicode(const KYString& AUTF8, KYString& ADest);

inline long UTF8ToUnicode(const char* AUTF8, char* ADest, long& ADestSize)
{
   return UTF8ToUnicode(AUTF8, Length(AUTF8), ADest, ADestSize);
}

inline long UTF8ToUnicode(const KYString& AUTF8, char* ADest, long& ADestSize)
{
   return UTF8ToUnicode((char*)AUTF8, AUTF8.Length(), ADest, ADestSize);
}

inline KYString UTF8ToUnicode(const KYString& AUTF8)
{
   KYString result;
   UTF8ToUnicode(AUTF8, result);
   return result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* CRC32 编码 */

// CRC32 码表
extern const Longword CRC32_Table[256];

// 得到 CRC32 值
inline Longword GetCRC32(Byte AIndex)  { return CRC32_Table[AIndex]; }

// 计算 CRC32 值
inline Longword CalcCRC32(Byte AValue, Longword ACRCCode)
{
   return CRC32_Table[(Byte)((Byte)ACRCCode ^ AValue)] ^ (ACRCCode >> 8);
}

// 计算缓冲区数据的 CRC32 值
Longword BufferToCRC32(const void* ABuffer, long ASize, Longword ACRCCode = 0);

// 计算 String 的 CRC32 值
inline Longword StringToCRC32(const KYString& AStr, Longword ACRCCode = 0)
{
   return BufferToCRC32((const char*)AStr, AStr.Length(), ACRCCode);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* Base64 编码 */

// 初始化 Base64 编码表
void     InitBase64Map();
void     InitBase64Table(const char* ABase64Table);

// 值 <--> 编码
char     Base64ToChar(Byte AValue);
Byte     CharToBase64(char AValue);

// Base64 编解码 (高位--->低位)
KYString StringToBase64(const KYString& ASource);
KYString Base64ToString(const KYString& ASource);

// Base64 编解码 (低位--->高位)
KYString StringToBase64E(const KYString& ASource);
KYString Base64ToStringE(const KYString& ASource);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 加密/解密编码 */

// 判断是否为素数
bool     IsPrimeNumber(long AValue);

// 得到当前值的最大素数
long     GetMaxGene(long AValue);

// 根据种子数得到键值
KYString GetKeyValue(Longword ASeed, Longword AKeyLen);

// 加密/解密缓冲区
void     EncryptBuffer(void* ABuffer, long ASize, const KYString& AKey);
void     DecryptBuffer(void* ABuffer, long ASize, const KYString& AKey);

// 加密/解密字符串
KYString EncryptString(const KYString& AStr, const KYString& AKey);
KYString DecryptString(const KYString& AStr, const KYString& AKey);

// 加密/解密字符串(与 EncryptString/DecryptString 相似, 性能相对较差)
KYString KYGetEncrypt(const KYString& AKey, const KYString& AStr);
KYString KYGetDecrypt(const KYString& AKey, const KYString& AStr);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* MD5 编码 */

// MD5 摘要类型
typedef Byte   TMD5Digest[16];

// MD5 上下文结构
typedef struct
{
   Longword    State[4];
   Longword    Count[2];
   Byte        Buffer[64];
} TMD5Context, *PMD5Context;


// MD5 初始化/更新/结束
void     MD5Init(TMD5Context& AContext);
void     MD5Update(TMD5Context& AContext, const void* ABuffer, Longword ALength);
void     MD5Final(TMD5Context& AContext, TMD5Digest& ADigest);

// 字符串的 MD5
void     MD5String(const KYString& AStr, TMD5Digest& ADigest);

// MD5 摘要比较是否相同
bool     MD5SameDigest(const TMD5Digest& ADigest1, const TMD5Digest& ADigest2);

// MD5 摘要转换成十六进制的字符串
KYString MD5Digest2HexStr(const TMD5Digest& ADigest);

}

#endif
