// =======================================
// Unit   : ���뵥Ԫ (KYCodeUtils.h)
// Version: 3.0.1.0 (build 2014.12.14)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYCodeUtils_H_
#define _KYCodeUtils_H_

#include "KYString.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ��������� */

// ��ʼ�������
void              Randomize();

// ���������, ע: AModule > 1
Longword          Random();
inline Longword   Random(Longword AModule)
                  { return Random() % AModule; }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ASCII/Hex ���� */

// ASCII ��ת����ʮ��������, ����: ASCIIToHex("ABC") -> "414243"
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

// ʮ��������ת���� ASCII ��, ����: HexToASCII("414243") -> "ABC"
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

// �ַ���ת�����ÿո�ָ���ʮ�����ƴ�, ����: StrToHex("ABC") -> "41 42 43"
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

// ���ָ���ʮ�����ƴ�ת�����ַ���, ����: HexToStr("41 42 43") -> "ABC"
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

/* Unicode/UTF8 ���� */

// ȡ unicode-string ����
long        WideLength(const char* AUnicode);
inline long WideLength(const char* AUnicode, long ASize)
            { return (AUnicode == NULL) || (ASize <= 1) ? 0 : (ASize >> 1); }
inline long WideLength(const KYString& AUnicode)
            { return AUnicode.Length() >> 1; }

// ȡ UTF8-string ����(ɨ�赽�Ƿ����뼴��ֹ, ��������Ч����)
long        UTF8Length(const char* AUTF8, long ASize);
inline long UTF8Length(const char* AUTF8)
            { return UTF8Length(AUTF8, Length(AUTF8)); }
inline long UTF8Length(const KYString& AUTF8)
            { return UTF8Length((char*)AUTF8, AUTF8.Length()); }

// ANSI -> Unicode �ַ���
// 1. �� ADest == NULL, �򷵻������ ADestSize �� unicode-string ����
// 2. ���� ADestSize >= ASize + ASize, �ҷ��� ADest ռ�óߴ�
// 3. ���� ADest ����ĩβ 0 �ַ�
// 4. ���� ADest �� unicode-string ����, ������Ϊ -1 ���ʾ ADestSize ̫С
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

// Unicode -> ANSI �ַ���
// 1. �� ADest == NULL, �򷵻� ANSI-string ����
// 2. ���� ADestSize >= ASize
// 3. ���� ADest ����ĩβ 0 �ַ�
// 4. ���� ADest �� ANSI-string ����, ������Ϊ -1 ���ʾ ADestSize ̫С
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

// ANSI -> UTF8 �ַ���
// 1. �� ADest == NULL, �򷵻������ ADestSize �� UTF8-string ����
// 2. ���� ADestSize >= ASize + ASize, �ҷ��� ADest ռ�óߴ�
// 3. ���� ADest ����ĩβ 0 �ַ�
// 4. ���� ADest �� UTF8-string ����, ������Ϊ -1 ���ʾ ADestSize ̫С
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

// UTF8 -> ANSI �ַ���
// *. �� AUTF8 ɨ�赽�Ƿ����뼴��ֹ, ��ֻת����Ч�ߴ�
// 1. �� ADest == NULL, �򷵻� ANSI-string ����
// 2. ���� ADestSize >= ASize
// 3. ���� ADest ����ĩβ 0 �ַ�
// 4. ���� ADest �� ANSI-string ����, ������Ϊ -1 ���ʾ ADestSize ̫С
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

// Unicode -> UTF8 �ַ���
// 1. �� ADest == NULL, �򷵻������ ADestSize �� UTF8-string ����
// 2. ���� ADestSize >= ASize + (ASize >> 1), �ҷ��� ADest ռ�óߴ�
// 3. ���� ADest ����ĩβ 0 �ַ�
// 4. ���� ADest �� UTF8-string ����, ������Ϊ -1 ���ʾ ADestSize ̫С
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

// UTF8 -> Unicode �ַ���
// *. �� AUTF8 ɨ�赽�Ƿ����뼴��ֹ, ��ֻת����Ч�ߴ�
// 1. �� ADest == NULL, �򷵻������ ADestSize �� unicode-string ����
// 2. ���� ADestSize >= ASize + ASize, �ҷ��� ADest ռ�óߴ�
// 3. ���� ADest ����ĩβ 0 �ַ�
// 4. ���� ADest �� unicode-string ����, ������Ϊ -1 ���ʾ ADestSize ̫С
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

/* CRC32 ���� */

// CRC32 ���
extern const Longword CRC32_Table[256];

// �õ� CRC32 ֵ
inline Longword GetCRC32(Byte AIndex)  { return CRC32_Table[AIndex]; }

// ���� CRC32 ֵ
inline Longword CalcCRC32(Byte AValue, Longword ACRCCode)
{
   return CRC32_Table[(Byte)((Byte)ACRCCode ^ AValue)] ^ (ACRCCode >> 8);
}

// ���㻺�������ݵ� CRC32 ֵ
Longword BufferToCRC32(const void* ABuffer, long ASize, Longword ACRCCode = 0);

// ���� String �� CRC32 ֵ
inline Longword StringToCRC32(const KYString& AStr, Longword ACRCCode = 0)
{
   return BufferToCRC32((const char*)AStr, AStr.Length(), ACRCCode);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* Base64 ���� */

// ��ʼ�� Base64 �����
void     InitBase64Map();
void     InitBase64Table(const char* ABase64Table);

// ֵ <--> ����
char     Base64ToChar(Byte AValue);
Byte     CharToBase64(char AValue);

// Base64 ����� (��λ--->��λ)
KYString StringToBase64(const KYString& ASource);
KYString Base64ToString(const KYString& ASource);

// Base64 ����� (��λ--->��λ)
KYString StringToBase64E(const KYString& ASource);
KYString Base64ToStringE(const KYString& ASource);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ����/���ܱ��� */

// �ж��Ƿ�Ϊ����
bool     IsPrimeNumber(long AValue);

// �õ���ǰֵ���������
long     GetMaxGene(long AValue);

// �����������õ���ֵ
KYString GetKeyValue(Longword ASeed, Longword AKeyLen);

// ����/���ܻ�����
void     EncryptBuffer(void* ABuffer, long ASize, const KYString& AKey);
void     DecryptBuffer(void* ABuffer, long ASize, const KYString& AKey);

// ����/�����ַ���
KYString EncryptString(const KYString& AStr, const KYString& AKey);
KYString DecryptString(const KYString& AStr, const KYString& AKey);

// ����/�����ַ���(�� EncryptString/DecryptString ����, ������Խϲ�)
KYString KYGetEncrypt(const KYString& AKey, const KYString& AStr);
KYString KYGetDecrypt(const KYString& AKey, const KYString& AStr);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* MD5 ���� */

// MD5 ժҪ����
typedef Byte   TMD5Digest[16];

// MD5 �����Ľṹ
typedef struct
{
   Longword    State[4];
   Longword    Count[2];
   Byte        Buffer[64];
} TMD5Context, *PMD5Context;


// MD5 ��ʼ��/����/����
void     MD5Init(TMD5Context& AContext);
void     MD5Update(TMD5Context& AContext, const void* ABuffer, Longword ALength);
void     MD5Final(TMD5Context& AContext, TMD5Digest& ADigest);

// �ַ����� MD5
void     MD5String(const KYString& AStr, TMD5Digest& ADigest);

// MD5 ժҪ�Ƚ��Ƿ���ͬ
bool     MD5SameDigest(const TMD5Digest& ADigest1, const TMD5Digest& ADigest2);

// MD5 ժҪת����ʮ�����Ƶ��ַ���
KYString MD5Digest2HexStr(const TMD5Digest& ADigest);

}

#endif
