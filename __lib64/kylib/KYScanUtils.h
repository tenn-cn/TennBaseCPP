// =======================================
// Unit   : 扫描字符串单元 (KYScanUtils.h)
// Version: 3.0.0.0 (build 2011.03.09)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYScanUtils_H_
#define _KYScanUtils_H_

#include "KYDateTime.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

/*----------------------------------------------------------------------------*
   表达式的基本元素
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

/* 错误信息 */

const KYString Error_NoEndOfComment = "\"%s\" expected in comment \"%s\" started";
const KYString Error_InvalidInteger = "\"%s\" is not a valid integer value";
const KYString Error_IllegalIdent   = "Illegal identifier: \"%s\"";
const KYString Error_GetQuotedStr   = "Unterminated string";

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 公用函数 */

// 过滤掉注释(单结束符), APos 从注释符号之后开始
bool  FilterCommentChar1(char* &APos, const char* AEnd, char AChar);

// 过滤掉注释(双结束符), APos 从注释符号之后开始
bool  FilterCommentChar2(char* &APos, const char* AEnd, char AChar1, char AChar2);

// 过滤掉空白字符和注释
bool  FilterBlankAndComment(char* &APos, const char* AEnd, KYString& AError);

// 过滤掉左边空白字符(行扫描)
void  FilterBlankLeft(char* &APos, const char* AEnd);

// 过滤掉右边空白字符(行扫描)
void  FilterBlankRight(char* &APos, const char* ABegin);

// 过滤掉空白字符和行注释 "//"(行扫描)
void  FilterBlank_Comment(char* &APos, const char* AEnd);

// 过滤掉空白字符和行注释 "//"(行扫描)
void  FilterValidEnd(char* &APos, const char* AEnd);

// 查找第一个指定字符
bool  ScanFirstChar(char* &APos, const char* AEnd, char AChar);

// 查找最右边的指定字符
bool  ScanLastChar(char* &APos, const char* AEnd, char AChar);

// 查找第一个指定字符(遇到 "//" 自动结束)
bool  ScanFirstChar_Comment(char* &APos, const char* AEnd, char AChar);

// 查找最右边的指定字符(遇到 "//" 自动结束)
bool  ScanLastChar_Comment(char* &APos, const char* AEnd, char AChar);

// 扫描数字串
bool  ScanDigitSeq(char* &APos, const char* AEnd);

// 扫描十六进制数字串, APos 从 '$', 'x', '0x' 之后开始
bool  ScanHexDigitSeq(char* &APos, const char* AEnd);

// 扫描无符号整型
bool  ScanUnsignedInteger(char* &APos, const char* AEnd);

// 扫描浮点标量, APos 从 'E' 之后开始
bool  ScanScaleFactor(char* &APos, const char* AEnd);

// 扫描无符号数
bool  ScanUnsignedNum(char* &APos, const char* AEnd, bool& AIsFloat);

// 扫描数字
bool  ScanNumber(char* &APos, const char* AEnd, bool& AIsFloat);

// 取 ASCII 码字符, APos 从 '#' 之后开始
bool  GetASCIIChar(char* &APos, const char* AEnd, char& AChar, KYString& AError);

// 取标识符
KYString GetIdentifier(char* &APos, const char* AEnd);

// 取指示符
KYString GetDesignator(char* &APos, const char* AEnd, KYString& AError);

// 取无符号数
KYString GetUnsignedNumber(char* &APos, const char* AEnd, bool& AIsFloat);

// 取数字
KYString GetNumber(char* &APos, const char* AEnd, bool& AIsFloat);

// 取单引号内的字符串, APos 从单引号之后开始
KYString GetQuotedStr(char* &APos, const char* AEnd, KYString& AError);

// 取字符串
KYString GetString(char* &APos, const char* AEnd, KYString& AError);

// 给字符串加单引号, 若字符中存在单引号则自动添加二个单引号
// 如: QuotedStr("abc'123'def") 返回值为: "'abc''123''def'"
KYString QuotedStr(const char* AStr, int ALength);
inline KYString QuotedStr(const KYString& AStr)
{
   return QuotedStr((char*)AStr, AStr.Length());
}

// 给字符串加单引号
// 1. 若字符中存在单引号则自动添加二个单引号;
// 2. 若不可见字符则显示 "#" + <ASCII码>, 并且前后非空串时添加单引号.
// 如: SetQuotedStr("abc\x0D'123'\x0D\x0Adef") 返回值为: "'abc'#13'''123'''#13#10'def'"
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
