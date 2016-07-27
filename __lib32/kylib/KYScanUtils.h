// =======================================
// Unit   : ɨ���ַ�����Ԫ (KYScanUtils.h)
// Version: 3.0.0.0 (build 2011.03.09)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYScanUtils_H_
#define _KYScanUtils_H_

#include "KYDateTime.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

/*----------------------------------------------------------------------------*
   ���ʽ�Ļ���Ԫ��
 *----------------------------------------------------------------------------*
   Blank             in [#0..#32]
   Digit             in ['0'..'9']
   Letter            in ['A'..'Z', 'a'..'z']
   HexDigit          in ['0'..'9', 'A'..'F', 'a'..'F']
   ValidChar         in [#0..#12, #14..#38, #40..#255]
   QuotedChar        =  ''''
   Underline         =  '_'

   SignOp            -> '+'
                     -> '-'

   LineComment       -> '//' ...
   BlockComment      -> '/*' ... '*' -> '/'
                     -> '(*' ... '*)'
                     -> '{'  ... '}'

   Number            -> [SignOp] -> UnsignedNum

   UnsignedNum       -> UnsignedInteger
                     -> UnsignedReal

   UnsignedInteger   -> DigitSeq
                     -> '$'  -> HexDigitSeq
                     -> 'x'  -> HexDigitSeq
                     -> 'X'  -> HexDigitSeq
                     -> '0x' -> HexDigitSeq
                     -> '0X' -> HexDigitSeq

   UnsignedReal      -> DigitSeq -> '.' -> DigitSeq
                     -> DigitSeq -> '.' -> DigitSeq -> 'E' -> ScaleFactor
                     -> DigitSeq -> 'E' -> ScaleFactor

   ScaleFactor       -> [SignOp] -> DigitSeq

   DigitSeq          -> Digit
                     -> DigitSeq -> Digit

   HexDigitSeq       -> HexDigit
                     -> HexDigitSeq -> HexDigit

   String            -> QuotedStr
                     -> ASCIIChar
                     -> String -> QuotedStr
                     -> String -> ASCIIChar

   ASCIIChar         -> '#' -> UnsiginedInteger

   QuotedStr         -> QuotedChar -> CharStr -> QuotedChar
   CharStr           -> ValidChar
                     -> QuotedChar -> QuotedChar
                     -> CharStr -> ValidChar
                     -> CharStr -> QuotedChar -> QuotedChar

   Identifier        -> Letter
                     -> Underline
                     -> Identifier -> Underline
                     -> Identifier -> Letter
                     -> Identifier -> Digit

   Designator        -> Identifier
                     -> Designator + '.' + Identifier

 *----------------------------------------------------------------------------*/

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ������Ϣ */

const KYString Error_NoEndOfComment = "\"%s\" expected in comment \"%s\" started";
const KYString Error_InvalidInteger = "\"%s\" is not a valid integer value";
const KYString Error_IllegalIdent   = "Illegal identifier: \"%s\"";
const KYString Error_GetQuotedStr   = "Unterminated string";

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ���ú��� */

// ���˵�ע��(��������), APos ��ע�ͷ���֮��ʼ
bool  FilterCommentChar1(char* &APos, const char* AEnd, char AChar);

// ���˵�ע��(˫������), APos ��ע�ͷ���֮��ʼ
bool  FilterCommentChar2(char* &APos, const char* AEnd, char AChar1, char AChar2);

// ���˵��հ��ַ���ע��
bool  FilterBlankAndComment(char* &APos, const char* AEnd, KYString& AError);

// ���˵���߿հ��ַ�(��ɨ��)
void  FilterBlankLeft(char* &APos, const char* AEnd);

// ���˵��ұ߿հ��ַ�(��ɨ��)
void  FilterBlankRight(char* &APos, const char* ABegin);

// ���˵��հ��ַ�����ע�� "//"(��ɨ��)
void  FilterBlank_Comment(char* &APos, const char* AEnd);

// ���˵��հ��ַ�����ע�� "//"(��ɨ��)
void  FilterValidEnd(char* &APos, const char* AEnd);

// ���ҵ�һ��ָ���ַ�
bool  ScanFirstChar(char* &APos, const char* AEnd, char AChar);

// �������ұߵ�ָ���ַ�
bool  ScanLastChar(char* &APos, const char* AEnd, char AChar);

// ���ҵ�һ��ָ���ַ�(���� "//" �Զ�����)
bool  ScanFirstChar_Comment(char* &APos, const char* AEnd, char AChar);

// �������ұߵ�ָ���ַ�(���� "//" �Զ�����)
bool  ScanLastChar_Comment(char* &APos, const char* AEnd, char AChar);

// ɨ�����ִ�
bool  ScanDigitSeq(char* &APos, const char* AEnd);

// ɨ��ʮ���������ִ�, APos �� '$', 'x', '0x' ֮��ʼ
bool  ScanHexDigitSeq(char* &APos, const char* AEnd);

// ɨ���޷�������
bool  ScanUnsignedInteger(char* &APos, const char* AEnd);

// ɨ�踡�����, APos �� 'E' ֮��ʼ
bool  ScanScaleFactor(char* &APos, const char* AEnd);

// ɨ���޷�����
bool  ScanUnsignedNum(char* &APos, const char* AEnd, bool& AIsFloat);

// ɨ������
bool  ScanNumber(char* &APos, const char* AEnd, bool& AIsFloat);

// ȡ ASCII ���ַ�, APos �� '#' ֮��ʼ
bool  GetASCIIChar(char* &APos, const char* AEnd, char& AChar, KYString& AError);

// ȡ��ʶ��
KYString GetIdentifier(char* &APos, const char* AEnd);

// ȡָʾ��
KYString GetDesignator(char* &APos, const char* AEnd, KYString& AError);

// ȡ�޷�����
KYString GetUnsignedNumber(char* &APos, const char* AEnd, bool& AIsFloat);

// ȡ����
KYString GetNumber(char* &APos, const char* AEnd, bool& AIsFloat);

// ȡ�������ڵ��ַ���, APos �ӵ�����֮��ʼ
KYString GetQuotedStr(char* &APos, const char* AEnd, KYString& AError);

// ȡ�ַ���
KYString GetString(char* &APos, const char* AEnd, KYString& AError);

// ���ַ����ӵ�����, ���ַ��д��ڵ��������Զ���Ӷ���������
// ��: QuotedStr("abc'123'def") ����ֵΪ: "'abc''123''def'"
KYString QuotedStr(const char* AStr, int ALength);
inline KYString QuotedStr(const KYString& AStr)
{
   return QuotedStr((char*)AStr, AStr.Length());
}

// ���ַ����ӵ�����
// 1. ���ַ��д��ڵ��������Զ���Ӷ���������;
// 2. �����ɼ��ַ�����ʾ "#" + <ASCII��>, ����ǰ��ǿմ�ʱ��ӵ�����.
// ��: SetQuotedStr("abc\x0D'123'\x0D\x0Adef") ����ֵΪ: "'abc'#13'''123'''#13#10'def'"
KYString SetQuotedStr(const char* AStr, int ALength);
inline KYString SetQuotedStr(const KYString& AStr)
{
   return SetQuotedStr((char*)AStr, AStr.Length());
}

// TDateTime -> String
void     DateTime2Str(TDateTime ADateTime, KYString& AStr);
KYString DateTime2Str(TDateTime ADateTime);

}

#endif
