// =======================================
// Unit   : �ַ����� (KYString.h)
// Version: 3.1.0.0 (build 2014.12.13)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYString_H_
#define _KYString_H_

#include <windows.h>
#include "KYObject.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// KYString ������
class       KYString;

// ��ʼ�� KYString ��Ԫ
void        InitKYString();

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* KYString �ַ����Ƚ���غ��� */

// �õ��Ӵ���λ��
// ����: Pos("aa", "aabc") -> result = 1
//       Pos("aa", "abc")  -> result = 0
int         Pos(const KYString& ASubstr,   const KYString& AStr,
                           bool ACaseSensitive = true, int AFrom = 1);
int         Pos(const char* ASubstr,   const KYString& AStr,
                       bool ACaseSensitive = true, int AFrom = 1);
int         Pos(const KYString& ASubstr,       const char* AStr,
                           bool ACaseSensitive = true, int AFrom = 1);

// �ַ����Ƚ�
int         CompareStr(const KYString& AStr1, const KYString& AStr2, bool ACaseSensitive = true);
int         CompareStr(const KYString& AStr1,     const char* AStr2, bool ACaseSensitive = true);
inline int  CompareStr(const char* AStr1,     const KYString& AStr2, bool ACaseSensitive = true)
            { return -CompareStr(AStr2, AStr1, ACaseSensitive); }

// �Ƿ���ͬ�ַ���
bool        SameStr(const KYString& AStr1, const KYString& AStr2, bool ACaseSensitive = true);
bool        SameStr(const KYString& AStr1,     const char* AStr2, bool ACaseSensitive = true);
inline bool SameStr(const char* AStr1,     const KYString& AStr2, bool ACaseSensitive = true)
            { return SameStr(AStr2, AStr1, ACaseSensitive); }

// �ı��Ƚ�(�����ִ�Сд)
inline int  CompareText(const KYString& AStr1,const KYString& AStr2)
            { return CompareStr(AStr1, AStr2, false); }
inline int  CompareText(const KYString& AStr1,    const char* AStr2)
            { return CompareStr(AStr1, AStr2, false); }
inline int  CompareText(const char* AStr1,    const KYString& AStr2)
            { return CompareStr(AStr1, AStr2, false); }

// �Ƿ���ͬ�ı�(�����ִ�Сд)
inline bool SameText(const KYString& AStr1,   const KYString& AStr2)
            { return SameStr(AStr1, AStr2, false); }
inline bool SameText(const KYString& AStr1,       const char* AStr2)
            { return SameStr(AStr1, AStr2, false); }
inline bool SameText(const char* AStr1,       const KYString& AStr2)
            { return SameStr(AStr1, AStr2, false); }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* KYString - �ַ����� */

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
   // KYString ��ṹ: <ReferenceCount(4)> + <Length(4)> + <content> + <end(1)>
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   #pragma pack(push, 1)
   typedef struct
   {
      long     RefCount;               // ���ü���
      int      Length;                 // �ַ�������
      char     Content[1];             // ������Ȳ����޸�, �պô�Ž�β'\0'�ַ�
   } TItem, *PItem;
   #pragma pack(pop)

public:
   KYString();                         // ������ַ���
   KYString(const KYString& AStr);     // ���쿽���ַ���
   KYString(const char* AStr);         // ���쿽��'\0'�ַ���β���ַ���
   KYString(char AChar);               // ����ָ���ַ����ַ���
   KYString(int  ALength);             // ����ָ�����ȵ��ַ���, ������δ��ʼ��
   KYString(long ALength);             // ����ָ�����ȵ��ַ���, ������δ��ʼ��
   KYString(char AChar, int ACount);   // ����ָ���ַ����������ַ���
   KYString(const char* AStr, int ALength);// ���쿽���� AStr ��ʼ�� ALength ���ַ����ַ���

   // ��������
   virtual ~KYString();

   // ��ֵ����
   KYString&   operator=(const KYString& AStr);
   KYString&   operator=(const char* AStr);
   KYString&   operator=(char AChar);

   // �ַ������Ӳ���
   KYString&   operator+=(const KYString& AStr);
   KYString&   operator+=(const char* AStr);
   KYString&   operator+=(char AChar);

   // ȡλ�õ��ַ�, APos ȡֵ��ΧΪ: [1..Length()]
   char        operator[](int APos) const { return FData->Content[APos - 1]; }

   // ����λ�õ��ַ�, APos ȡֵ��ΧΪ: [1..Length()]
   void        SetChar(int APos, char AChar);

   // ����ת��, ֧�� (const char*) �� (char*) ת��, ���߲���ǵ��ַ���
   // Ϊ�մ�ʱ, (const char*) ���� "" �ַ���ָ��, �� (char*) ���� NULL
   operator    const char* () const    { return (FData == NULL) ? Empty_C_Str : FData->Content;}
   operator    char* () const          { return (FData == NULL) ? NULL : FData->Content; }

   // ȡ�ַ�������
   int         Length() const          { return (FData == NULL) ? 0 : FData->Length; }

   // �����ַ�������, �൱�ڷ���ָ�����ȵĻ�����
   // 1. �� ALength < Length() ʱ, �Զ��ضϺ��������
   // 2. ������ Length() �ַ�������, ͬʱ���������δ��ʼ��
   void        SetLength(int ALength);

   // ����ת������, ������ (char*) ����
   // 1. �� String �����ü���Ϊ 1, ���� (char*) ��ͬ
   // 2. �������¿�¡һ���µ��ַ����������׵�ַ
   char*       c_str();

   // ����ַ����Ƿ�Ϊ���ִ�, ������ ['0'..'9'] �ַ����
   bool        IsNumber() const;

   // ����ַ����Ƿ�Ϊ��ʶ��, �����ַ�Ϊ {'_', ['a'..'z'], ['A'..'Z']} ���,
   // �����ַ����԰��� ['0'..'9'] �ַ�
   bool        IsValidIdent() const;

   // ת����Сд��ĸ, ����: "AbC" -> "abc"
   KYString&   ToLowerCase();

   // ת���ɴ�д��ĸ, ����: "AbC" -> "ABC"
   KYString&   ToUpperCase();

   // ת���ɴ�Сд�����ַ�, ����: "AbC" -> "aBc"
   KYString&   ToInverseCase();

   // ȥ�����߿հ��ַ�, ����: " \x0D\x0A AbC " -> "AbC"
   KYString&   Trim();

   // ȥ����߿հ��ַ�, ����: " \x0D\x0A AbC " -> "AbC "
   KYString&   TrimLeft();

   // ȥ���ұ߿հ��ַ�, ����: " \x0D\x0A AbC " -> " \x0D\x0A AbC"
   KYString&   TrimRight();

   // ȥ������ָ���ַ�, ����: AChar = ' ', " \x0D\x0A AbC " -> "\x0D\x0A AbC"
   KYString&   TrimChar(char AChar = ' ');

   // ȥ�����ָ���ַ�, ����: AChar = ' ', " \x0D\x0A AbC " -> "\x0D\x0A AbC "
   KYString&   TrimCharLeft(char AChar = ' ');

   // ȥ���ұ�ָ���ַ�, ����: AChar = ' ', " \x0D\x0A AbC " -> " \x0D\x0A AbC"
   KYString&   TrimCharRight(char AChar = ' ');

   // ʣ��, ����: Remain("abcd1234", 4, 4) -> "d123"
   KYString&   Remain(int AFrom, int ACount);

   // ɾ��, ����: Delete("abcd1234", 4, 4) -> "abc4"
   KYString&   Delete(int AFrom, int ACount);

   // ����ַ���, ���ַ����ÿ�
   KYString&   Clear()                 { _UpdateData(NULL); return *this; }

   // ����, ����: Insert("aa", "abcd1234", 4) -> "abcaad1234"
   KYString&   Insert(const KYString& ASubstr, int APos);
   KYString&   Insert(const char* ASubstr, int ALength, int APos);
   KYString&   Insert(const char* ASubstr, int APos);
   KYString&   Insert(char AChar, int APos);

   // ��ʼ��ָ���ַ����ַ���, ����: StringOfChar('a', 5) -> "aaaaa"
   KYString&   StringOfChar(char AChar, int ACount);

   // �����ַ���, ����: Copy("abcd1234", 4, 10) -> "d1234"
   KYString&   Copy(const KYString& AStr, int AFrom, int ACount);
   KYString&   Copy(const char* AStr, int AFrom, int ACount);  // ע: ���'\0'������
   KYString&   Copy(const char* AStr, int ACount)
               { return Copy(AStr, 1, ACount); }

   // �����ַ���, ������ AStr ��ʼ�� ALength ���ַ����ַ���(ע: �����'\0'������)
   KYString&   SetString(const char* AStr, int ALength);

   // ��ʽ����(����ֵ����Ϊ KYString ����, ����ʹ�� (const char*) ת��)
   // Format ��ʽ���� printf ��ʽ����ͬ
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
   // ָ����ַ���ָ��
   static const char* const Empty_C_Str;  // = ""

   // KYString �ľ�̬��Ա��ʼ����
   static class TInitialization
   {
   public:
      TInitialization();
      ~TInitialization();
   } _Initialization;

   friend class TInitialization;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* MBCS functions - ���ֽ��ַ������� */

enum TMbcsByteType {mbSingleByte, mbLeadByte, mbTrailByte};

// ָ���������ֽ�����, AIndex = 1..Length
TMbcsByteType ByteType(const char* AStr, long AIndex);
TMbcsByteType ByteType(const KYString& AStr, long AIndex);

// �õ����һ���ַ�
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

// ȡϵͳ������Ϣ
KYString SysErrorMessage(unsigned long AErrCode);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �ַ������� */

// ����/ʮ����������/��ĸ������
extern const char Offset_IsDigit[256];
extern const char Offset_IsHexDigit[256];
extern const char Offset_IsLetter[256];

// ��Сд ASCII ���
extern const char ASCIIMap_Lower[256];
extern const char ASCIIMap_Upper[256];
extern const char ASCIIMap_Inverse[256];

// �Ƿ�Ϊָ�������ַ���
#define     __IsDigitChar(ch)          (Offset_IsDigit[(unsigned char)(ch)] != 0)
#define     __IsHexDigitChar(ch)       (Offset_IsHexDigit[(unsigned char)(ch)] != 0)
#define     __IsLetterChar(ch)         (Offset_IsLetter[(unsigned char)(ch)] != 0)

#define     __IsNotDigitChar(ch)       (Offset_IsDigit[(unsigned char)(ch)] == 0)
#define     __IsNotHexDigitChar(ch)    (Offset_IsHexDigit[(unsigned char)(ch)] == 0)
#define     __IsNotLetterChar(ch)      (Offset_IsLetter[(unsigned char)(ch)] == 0)

// �õ���Сд�ַ�
inline char LowerCaseChar(char AChar)  { return ASCIIMap_Lower[(unsigned char)AChar]; }
inline char UpperCaseChar(char AChar)  { return ASCIIMap_Upper[(unsigned char)AChar]; }
inline char InverseCaseChar(char AChar){ return ASCIIMap_Inverse[(unsigned char)AChar]; }

// �õ�Ŀ�괮�� KMP �Ļ��ݱ�
// ע: Length(ANext) ������ڵ��� ADestLen
bool GetKMPNext(const char* ADest, int ADestLen, int* ANext,
                       bool ACaseSensitive = true);

// _FindNext, _FindNextEx �� FindNext, FindNextEx ��ͬ, ֻ�ǲ��������ĺϷ���
// ע: AText �� ADest �ǿ�, ADestLen > 0, ANextPos > 0, AFrom > 0
int  _FindNext(const char* AText, int ATextLen,
               const char* ADest, int ADestLen,
                      int* ANext, int ANextPos = 1,
                       int AFrom = 1, bool ACaseSensitive = true);
int  _FindNextEx(const char* AText, int ATextLen,
                 const char* ADest, int ADestLen,
                        int* ANext, int& ANextPos,
                         int AFrom = 1, bool ACaseSensitive = true);

// ���� KMP ���ݱ������һ��ƥ����
// ע: ANext ���ݱ�� ACaseSensitive �����뵱ǰ�� ACaseSensitive ������ͬ,
//     FindNextEx Ϊ���ܹ����� ANextPos �� FindNext Ҫ����Ƚ�, ����Ч��Ҫ
//     ��Բ�һЩ
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

// �õ�Ŀ�괮��λ��(ADest �� AText �еĵ�һ��λ��)
int  StrPos(const char* ADest, int ADestLen,
            const char* AText, int ATextLen,
                   bool ACaseSensitive = true, int AFrom = 1);

// �ַ����Ƚ�
int  StrCompare(const char* AStr1, const char* AStr2, bool ACaseSensitive = true);
int  StrCompare(const char* AStr1, int ALen1,
                const char* AStr2, int ALen2,
                      bool ACaseSensitive = true);

// �ж��ַ��������Ƿ���ͬ
bool IsSameStr(const char* AStr1, const char* AStr2, bool ACaseSensitive = true);
bool IsSameStr(const char* AStr1, int ALen1,
               const char* AStr2, int ALen2,
                      bool ACaseSensitive = true);
inline bool IsSameStr(const char* AStr1, const char* AStr2,
                              int ALen, bool ACaseSensitive = true)
            { return IsSameStr(AStr1, ALen, AStr2, ALen, ACaseSensitive); }

// ɨ����ֵ�һ���ַ���ָ��
char* StrScan(const char* AStr, char AChar);

// ɨ��������һ���ַ���ָ��
char* StrRScan(const char* AStr, char AChar);

// ת���ɴ�д�ַ���
char* StrUpper(char* AStr);

// ת����Сд�ַ���
char* StrLower(char* AStr);

// �ַ�������󳤶����ƵĿ���
// 1. ADest ����: ADest != NULL, �� AMaxLen >= 0
// 2. ADest �ѷ���Ŀռ�ߴ����: size(ADest) >= AMaxLen + 1
// 3. ADest ������ĩβ�Զ���� '\0' �ַ�
char* StrLCopy(char* ADest, const char* AStr, long AMaxLen);
char* StrLCopy(char* ADest, const KYString& AStr, long AMaxLen);

// �ַ��������ߴ�Ŀ���
// 1. ADest ����: ADest != NULL, �� AMaxSize > 0
// 2. ADest �ѷ���Ŀռ�ߴ����: size(ADest) >= AMaxSize
// 3. ADest �� Length(AStr) < AMaxSize ʱĩβ�Զ���� '\0' �ַ�
char* StrNCopy(char* ADest, const char* AStr, long AMaxSize);
char* StrNCopy(char* ADest, const KYString& AStr, long AMaxSize);

// �� AStr �в��� AOld ��, ��ʹ�� ANew �滻
// ע: SameStr(AOld, ANew, true) ����Ϊ false, �ڲ�������Ƿ���ͬ
// ��: StringReplace("aabbccddbbcc", "bc", "x", true) ����ֵΪ: "aabxcddbxc"
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

/* KYString ��غ��� */

// ɨ������ֵ, ����ɨ��������, ������Ϊ 0 ���ʾ��������ֵ
int         ScanInteger(const char* AStr, int ALength, bool& ASigned);
inline int  ScanInteger(const KYString& AStr, bool& ASigned)
            { return ScanInteger((char*)AStr, AStr.Length(), ASigned); }

// ɨ����ֵ, ����ɨ��������, ������Ϊ 0 ���ʾ������ֵ
int         ScanNumber(const char* AStr, int ALength, bool& ASigned, bool& AIsFloat);
inline int  ScanNumber(const KYString& AStr, bool& ASigned, bool& AIsFloat)
            { return ScanNumber((char*)AStr, AStr.Length(), ASigned, AIsFloat); }

// �ж��Ƿ�Ϊ���ִ�
bool        IsNumber(const char* AStr, int ALength);
inline bool IsNumber(const KYString& AStr)
            { return AStr.IsNumber(); }

// �ж��Ƿ�Ϊ��Ч��ʶ��
bool        IsValidIdent(const char* AIdent, int ALength);
inline bool IsValidIdent(const KYString& AIdent)
            { return AIdent.IsValidIdent(); }

// ȡ�ַ�������
inline int  Length(const char* AStr)
            { return (AStr == NULL) ? 0 : strlen(AStr); }
inline int  Length(const KYString& AStr)
            { return AStr.Length(); }

// �����ַ�������, �൱�ڷ���ָ�����ȵĻ�����
// 1. �� ALength < Length() ʱ, �Զ��ضϺ��������
// 2. ������ Length() �ַ�������, ͬʱ���������δ��ʼ��
inline void SetLength(KYString& AStr, int ALength)
            { AStr.SetLength(ALength); }

// ת����Сд��ĸ, ����: "AbC" -> "abc"
inline KYString LowerCase(const KYString& AStr)
{
   KYString result(AStr);
   return result.ToLowerCase();
}

// ת���ɴ�д��ĸ, ����: "AbC" -> "ABC"
inline KYString UpperCase(const KYString& AStr)
{
   KYString result(AStr);
   return result.ToUpperCase();
}

// ת���ɴ�Сд�����ַ�, ����: "AbC" -> "aBc"
inline KYString InverseCase(const KYString& AStr)
{
   KYString result(AStr);
   return result.ToInverseCase();
}

// ȥ�����߿հ��ַ�, ����: " \x0D\x0A AbC " -> "AbC"
inline KYString Trim(const KYString& AStr)
{
   KYString result(AStr);
   return result.Trim();
}

// ȥ����߿հ��ַ�, ����: " \x0D\x0A AbC " -> "AbC "
inline KYString TrimLeft(const KYString& AStr)
{
   KYString result(AStr);
   return result.TrimLeft();
}

// ȥ���ұ߿հ��ַ�, ����: " \x0D\x0A AbC " -> " \x0D\x0A AbC"
inline KYString TrimRight(const KYString& AStr)
{
   KYString result(AStr);
   return result.TrimRight();
}

// ȥ������ָ���ַ�, ����: AChar = ' ', " \x0D\x0A AbC " -> "\x0D\x0A AbC"
inline KYString TrimChar(const KYString& AStr, char AChar = ' ')
{
   KYString result(AStr);
   return result.TrimChar(AChar);
}

// ȥ�����ָ���ַ�, ����: AChar = ' ', " \x0D\x0A AbC " -> "\x0D\x0A AbC "
inline KYString TrimCharLeft(const KYString& AStr, char AChar = ' ')
{
   KYString result(AStr);
   return result.TrimCharLeft(AChar);
}

// ȥ���ұ�ָ���ַ�, ����: AChar = ' ', " \x0D\x0A AbC " -> " \x0D\x0A AbC"
inline KYString TrimCharRight(const KYString& AStr, char AChar = ' ')
{
   KYString result(AStr);
   return result.TrimCharRight(AChar);
}

// �����ַ���, ������ AStr ��ʼ�� ALength ���ַ����ַ���
inline KYString SetString(const char* AStr, int ALength)
{
   KYString result;
   return result.SetString(AStr, ALength);
}

// ��ʼ��ָ���ַ����ַ���, ����: StringOfChar('a', 5) -> "aaaaa"
inline KYString StringOfChar(char AChar, int ACount)
{
   KYString result;
   return result.StringOfChar(AChar, ACount);
}

// �����ַ���, ����: Copy("abcd1234", 4, 10) -> "d1234"
inline KYString Copy(const KYString& AStr, int AFrom, int ACount)
{
   KYString result;
   return result.Copy(AStr, AFrom, ACount);
}

// �����ַ���, ����: Copy("abcd1234", 4, 10) -> "d1234"
inline KYString Copy(const char* AStr, int AFrom, int ACount)
{
   KYString result;
   return result.Copy(AStr, AFrom, ACount);
}

// �����ַ���, ����: Copy("abcd1234", 4) -> "abcd"
inline KYString Copy(const char* AStr, int ACount)
{
   KYString result;
   return result.Copy(AStr, ACount);
}

// ʣ��, ����: Remain("abcd1234", 4, 4) -> "d123"
inline void Remain(KYString& AStr, int AFrom, int ACount)
            { AStr.Remain(AFrom, ACount); }

// ɾ��, ����: Delete("abcd1234", 4, 4) -> "abc4"
inline void Delete(KYString& AStr, int AFrom, int ACount)
            { AStr.Delete(AFrom, ACount); }

// ����, ����: Insert("aa", "abcd1234", 4) -> "abcaad1234"
inline void Insert(const KYString& ASubstr, KYString& AStr, int APos)
            { AStr.Insert(ASubstr, APos); }
inline void Insert(const char* ASubstr, int ALength, KYString& AStr, int APos)
            { AStr.Insert(ASubstr, ALength, APos); }
inline void Insert(const char* ASubstr, KYString& AStr, int APos)
            { AStr.Insert(ASubstr, APos); }
inline void Insert(char AChar, KYString& AStr, int APos)
            { AStr.Insert(AChar, APos); }

// ��ʽ����(����ֵ����Ϊ KYString ����, ����ʹ�� (const char*) ת��)
// Format ��ʽ���� printf ��ʽ����ͬ
KYString    Format(KYString AFormat, ...);
KYString    Format(const char* AFormat, ...);

// ����ת���ɶ�����/ʮ�����ƴ�
KYString&   IntToBin(int AValue,         KYString& AStr);
KYString&   IntToBin(__int64 AValue,     KYString& AStr);
KYString&   IntToHex(int AValue,         KYString& AStr);
KYString&   IntToHex(__int64 AValue,     KYString& AStr);

// ��ֵת�����ַ���
KYString&   IntToStr(int AValue,         KYString& AStr);
KYString&   IntToStr(__int64 AValue,     KYString& AStr);
KYString&   FloatToStr(double AValue,    KYString& AStr);

// ����ת���ɶ�����/ʮ�����ƴ�
KYString    IntToBin(int AValue);
KYString    IntToBin(__int64 AValue);
KYString    IntToHex(int AValue);
KYString    IntToHex(__int64 AValue);

// ��ֵת�����ַ���
KYString    IntToStr(int AValue);
KYString    IntToStr(__int64 AValue);
KYString    FloatToStr(double AValue);

// ����ת���ɴ�λ���Ķ�����/ʮ������/ʮ���ƴ�
KYString    IntToBin(int AValue,         int ADigits);
KYString    IntToBin(__int64 AValue,     int ADigits);
KYString    IntToHex(int AValue,         int ADigits);
KYString    IntToHex(__int64 AValue,     int ADigits);
KYString    IntToStr(int AValue,         int ADigits);
KYString    IntToStr(__int64 AValue,     int ADigits);

// long ת�����ַ���
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

// �ַ���ת����ֵ(�� ACode = 0 ��ɹ�, ���򷵻ش���λ��)
// ����: "123.3"        -> result = 123, ACode = 4
//       "123456"       -> result = 123456, ACode = 0
//       "12345678901"  -> result = 1234567890, ACode = 11
long        _ValLong(const char* AStr,   int& ACode);
long        _ValLong(const char* AStr,   int  ALength, int& ACode);

// �ַ���ת64λ����ֵ(�� ACode = 0 ��ɹ�, ���򷵻ش���λ��)
// ����: "123.3"        -> result = 123, ACode = 4
//       "123456"       -> result = 123456, ACode = 0
//       "12345678901"  -> result = 12345678901, ACode = 0
__int64     _ValInt64(const char* AStr,  int& ACode);
__int64     _ValInt64(const char* AStr,  int  ALength, int& ACode);

// �ַ���ת����ֵ, �� AStr ת��ʧ���򷵻�ֵΪ 0
int         StrToInt(const char* AStr);
int         StrToInt(const KYString& AStr);

// �ַ���ת64λ����ֵ, �� AStr ת��ʧ���򷵻�ֵΪ 0
__int64     StrToInt64(const char* AStr);
__int64     StrToInt64(const KYString& AStr);

// �ַ���ת����ֵ, �� AStr ת��ʧ���򷵻�ֵΪ ADefault
int         StrToIntDef(const char* AStr,     int ADefault);
int         StrToIntDef(const KYString& AStr, int ADefault);

// �ַ���ת64λ����ֵ, �� AStr ת��ʧ���򷵻�ֵΪ ADefault
__int64     StrToInt64Def(const char* AStr,     __int64 ADefault);
__int64     StrToInt64Def(const KYString& AStr, __int64 ADefault);

// ת���ɸ�����
double      StrToFloat(const char* AStr);
double      StrToFloat(const KYString& AStr);

}

#endif
