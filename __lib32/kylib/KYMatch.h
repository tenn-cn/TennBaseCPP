// =======================================
// Unit   : 字符串匹配类 (KYMatch.h)
// Version: 3.0.0.0 (build 2011.03.08)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYMatch_H_
#define _KYMatch_H_

#include "KYMemBlock.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 公用函数 */

// 字符串模式匹配
// 1. AText    为要匹配的文本串
// 2. ALike    为带通配符的模式串
// 3. AMulti   为多项通配符
// 4. ASingle  为单项通配符
// 5. 若函数返回值为 true 则表示匹配, 否则不匹配
// 例如:
//    1. StrLike("aabccaaaBBBcccaaac", "*a??b*c", false) 返回值为 true
//    2. StrLike("aabccaaaBBBcccaaac", "*a??b*c", true)  返回值为 false
bool        StrLike(const char* AText, long ATextLen,
                const KYString& ALike, bool ACaseSensitive = true,
                           char AMulti = '*', char ASingle = '?');

inline bool StrLike(const char* AText, const KYString& ALike,
                           bool ACaseSensitive = true,
                           char AMulti = '*', char ASingle = '?')
            { return StrLike(AText, Length(AText), ALike,
                             ACaseSensitive, AMulti, ASingle); }

inline bool StrLike(const KYString& AText, const KYString& ALike,
                               bool ACaseSensitive = true,
                               char AMulti = '*', char ASingle = '?')
            { return StrLike((char*)AText, AText.Length(), ALike,
                             ACaseSensitive, AMulti, ASingle); }

// 字符串的相同长度
// 1. ACompare == 0 表示 AStr1 与 AStr2 的相同
// 2. ACompare < 0  表示 AStr1 比 AStr2 小
// 3. ACompare > 0  表示 AStr1 比 AStr2 大
// 例如: SameLen("aabbbcccdd", "aaabbcccdd") 返回值为 2, 即 "aa" 相同
long        SameLen(const char* AStr1, const char* AStr2,
                          long& ACompare,     bool ACaseSensitive = true);
long        SameLen(const char* AStr1,    long ALen1, const char* AStr2,
                          long& ACompare, bool ACaseSensitive = true);
long        SameLen(const char* AStr1,    long ALen1,
                    const char* AStr2,    long ALen2,
                          long& ACompare, bool ACaseSensitive = true);

inline long SameLen(const char* AStr1, const char* AStr2,
                           bool ACaseSensitive = true)
{
   long intCompare;
   return SameLen(AStr1, AStr2, intCompare, ACaseSensitive);
}

inline long SameLen(const char* AStr1, long ALen1,
                    const char* AStr2, bool ACaseSensitive = true)
{
   long intCompare;
   return SameLen(AStr1, ALen1, AStr2, intCompare, ACaseSensitive);
}

inline long SameLen(const char* AStr1, long ALen1,
                    const char* AStr2, long ALen2, bool ACaseSensitive = true)
{
   long intCompare;
   return SameLen(AStr1, ALen1, AStr2, ALen2, intCompare, ACaseSensitive);
}

inline long SameLen(const char* AStr1, const KYString& AStr2,
                           bool ACaseSensitive = true)
            { return SameLen((char*)AStr2, AStr2.Length(),
                             (char*)AStr1, ACaseSensitive); }

inline long SameLen(const KYString& AStr1, const char* AStr2,
                               bool ACaseSensitive = true)
            { return SameLen((char*)AStr1, AStr1.Length(),
                             (char*)AStr2, ACaseSensitive); }

inline long SameLen(const KYString& AStr1, const KYString& AStr2,
                               bool ACaseSensitive = true)
            { return SameLen((char*)AStr1, AStr1.Length(),
                             (char*)AStr2, AStr2.Length(), ACaseSensitive); }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMatch - 字符串匹配类 */

class TKYMatch
{
private:
   // 通配项
   #pragma pack(push, 1)
   typedef struct
   {
      KYString*   Next;                // 项回溯表
      bool        IsLeft;              // 左边含有多项通配符
      bool        IsRight;             // 右边含有多项通配符
      long        IncLeft;             // 左偏移量
      long        DecRight;            // 右偏移量
   } TItem, *PItem;
   #pragma pack(pop)

public:
   // 状态
   enum TState   {msInvalid,           // 匹配串无效
                  msPending,           // 匹配串正在解析或正在关闭
                  msParsed};           // 匹配串已解析

public:
   TKYMatch();
   virtual ~TKYMatch();

   // 属性
   Byte           State() const        { return FState; }         // default: msInvalid
   char           CharMulti() const    { return FCharMulti; }     // default: '*'
   char           CharSingle() const   { return FCharSingle; }    // default: '?'
   KYString       MatchStr() const     { return FMatchStr; }      // default: ""

   KYString       ModeItem(long AIndex) const
                  { return FModeItems->Item(AIndex); }
   long           ModeCount() const    { return FModeItems->Count(); }        // default: 0
   bool           CaseSensitive() const{ return FModeItems->CaseSensitive(); }// default: true

   // 设置属性
   void           SetCharMulti(char AMulti)
                  { if (FState == msInvalid) FCharMulti = AMulti; }
   void           SetCharSingle(char ASingle)
                  { if (FState == msInvalid) FCharSingle = ASingle; }
   void           SetCaseSensitive(bool ACaseSensitive)
                  { if (FState == msInvalid) FModeItems->SetCaseSensitive(ACaseSensitive); }

   // 打开/关闭
   // 注: 若 AMatchStr == "", 则 ModeCount() == 0
   bool           Open(const KYString& AMatchStr);
   void           Close();

   // 匹配
   // 1. AText    为匹配的文本串
   // 2. ALength  为文本串的长度
   // 3. APosList 为匹配位置列表, 若为 NULL 则不返回位置列表,
   //             否则 APosList 的项数至少为 ModeCount()
   // 4. (返回值) 若为 0 则表示未匹配, 若为 -1 表示状态非法,
   //             否则为第一通配项的匹配位置
   long           Match(const char* AText, long ALength, long* APosList) const;
   long           Match(const KYString& AText, long* APosList) const
                  { return Match((char*)AText, AText.Length(), APosList); }

   long           Match(const char* AText, long ALength) const
                  { return Match(AText, ALength, NULL); }
   long           Match(const KYString& AText) const
                  { return Match((char*)AText, AText.Length()); }

protected:
   long           DoMatch(const char* AText, long ALength, long* APosList) const;

private:
   long           DoMatchItem(const char* AText,  long ALength, long* APosList,
                                    bool* AFixed, long AIndex,  long  AFrom) const;

   void           DoDeletion(const KYString& AItem, TItem* AData);

private:
   Byte           FState;              // 匹配状态
   char           FCharMulti;          // 多项通配符
   char           FCharSingle;         // 单项通配符
   KYString       FMatchStr;           // 匹配串
   TKYStringList* FModeItems;          // 模式匹配项列表

private:
   // 分配/释放项
   static TItem*  NewItem();
   static void    DeleteItem(TItem* AItem);

   // Inner_ItemManager 的 DoInitialize 和 DoFinalize 函数指针
   static void    DoInitItem(TItem* AItem, Word ASize);
   static void    DoFreeItem(TItem* AItem, Word ASize);

   // 解析匹配串
   static bool    ParseStr(const KYString& AStr, TKYStringList& AItems,
                                      char AMulti,         char ASingle);

   // 匹配只有一项
   static long    MatchOnly(const char* AText,   long  ALength,
                        const KYString& AValue, TItem* AItem,
                                  bool  ACaseSensitive);

   // 匹配最后一项
   static long    MatchLast(const char* AText,   long  ALength, long AFrom,
                        const KYString& AValue, TItem* AItem,
                                  bool  ACaseSensitive);

   // 通配项管理器, 存放通配项 TItem
   static TKYMemBlock*  Inner_ItemManager;

   // TKYMatch 的静态成员初始化类
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
