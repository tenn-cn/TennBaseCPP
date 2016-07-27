// =======================================
// Unit   : �ַ���ƥ���� (KYMatch.h)
// Version: 3.0.0.0 (build 2011.03.08)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYMatch_H_
#define _KYMatch_H_

#include "KYMemBlock.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ���ú��� */

// �ַ���ģʽƥ��
// 1. AText    ΪҪƥ����ı���
// 2. ALike    Ϊ��ͨ�����ģʽ��
// 3. AMulti   Ϊ����ͨ���
// 4. ASingle  Ϊ����ͨ���
// 5. ����������ֵΪ true ���ʾƥ��, ����ƥ��
// ����:
//    1. StrLike("aabccaaaBBBcccaaac", "*a??b*c", false) ����ֵΪ true
//    2. StrLike("aabccaaaBBBcccaaac", "*a??b*c", true)  ����ֵΪ false
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

// �ַ�������ͬ����
// 1. ACompare == 0 ��ʾ AStr1 �� AStr2 ����ͬ
// 2. ACompare < 0  ��ʾ AStr1 �� AStr2 С
// 3. ACompare > 0  ��ʾ AStr1 �� AStr2 ��
// ����: SameLen("aabbbcccdd", "aaabbcccdd") ����ֵΪ 2, �� "aa" ��ͬ
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

/* TKYMatch - �ַ���ƥ���� */

class TKYMatch
{
private:
   // ͨ����
   #pragma pack(push, 1)
   typedef struct
   {
      KYString*   Next;                // ����ݱ�
      bool        IsLeft;              // ��ߺ��ж���ͨ���
      bool        IsRight;             // �ұߺ��ж���ͨ���
      long        IncLeft;             // ��ƫ����
      long        DecRight;            // ��ƫ����
   } TItem, *PItem;
   #pragma pack(pop)

public:
   // ״̬
   enum TState   {msInvalid,           // ƥ�䴮��Ч
                  msPending,           // ƥ�䴮���ڽ��������ڹر�
                  msParsed};           // ƥ�䴮�ѽ���

public:
   TKYMatch();
   virtual ~TKYMatch();

   // ����
   Byte           State() const        { return FState; }         // default: msInvalid
   char           CharMulti() const    { return FCharMulti; }     // default: '*'
   char           CharSingle() const   { return FCharSingle; }    // default: '?'
   KYString       MatchStr() const     { return FMatchStr; }      // default: ""

   KYString       ModeItem(long AIndex) const
                  { return FModeItems->Item(AIndex); }
   long           ModeCount() const    { return FModeItems->Count(); }        // default: 0
   bool           CaseSensitive() const{ return FModeItems->CaseSensitive(); }// default: true

   // ��������
   void           SetCharMulti(char AMulti)
                  { if (FState == msInvalid) FCharMulti = AMulti; }
   void           SetCharSingle(char ASingle)
                  { if (FState == msInvalid) FCharSingle = ASingle; }
   void           SetCaseSensitive(bool ACaseSensitive)
                  { if (FState == msInvalid) FModeItems->SetCaseSensitive(ACaseSensitive); }

   // ��/�ر�
   // ע: �� AMatchStr == "", �� ModeCount() == 0
   bool           Open(const KYString& AMatchStr);
   void           Close();

   // ƥ��
   // 1. AText    Ϊƥ����ı���
   // 2. ALength  Ϊ�ı����ĳ���
   // 3. APosList Ϊƥ��λ���б�, ��Ϊ NULL �򲻷���λ���б�,
   //             ���� APosList ����������Ϊ ModeCount()
   // 4. (����ֵ) ��Ϊ 0 ���ʾδƥ��, ��Ϊ -1 ��ʾ״̬�Ƿ�,
   //             ����Ϊ��һͨ�����ƥ��λ��
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
   Byte           FState;              // ƥ��״̬
   char           FCharMulti;          // ����ͨ���
   char           FCharSingle;         // ����ͨ���
   KYString       FMatchStr;           // ƥ�䴮
   TKYStringList* FModeItems;          // ģʽƥ�����б�

private:
   // ����/�ͷ���
   static TItem*  NewItem();
   static void    DeleteItem(TItem* AItem);

   // Inner_ItemManager �� DoInitialize �� DoFinalize ����ָ��
   static void    DoInitItem(TItem* AItem, Word ASize);
   static void    DoFreeItem(TItem* AItem, Word ASize);

   // ����ƥ�䴮
   static bool    ParseStr(const KYString& AStr, TKYStringList& AItems,
                                      char AMulti,         char ASingle);

   // ƥ��ֻ��һ��
   static long    MatchOnly(const char* AText,   long  ALength,
                        const KYString& AValue, TItem* AItem,
                                  bool  ACaseSensitive);

   // ƥ�����һ��
   static long    MatchLast(const char* AText,   long  ALength, long AFrom,
                        const KYString& AValue, TItem* AItem,
                                  bool  ACaseSensitive);

   // ͨ���������, ���ͨ���� TItem
   static TKYMemBlock*  Inner_ItemManager;

   // TKYMatch �ľ�̬��Ա��ʼ����
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
