// =======================================
// Unit   : ѹ��/��ѹ����Ԫ (KYPackObj.h)
// Version: 3.0.1.0 (build 2014.12.13)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYPackObj_H_
#define _KYPackObj_H_

#include "KYList.h"
#include "KYCycQueue.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYCustomPack - ѹ������ */

class TKYCustomPack
{
public:
   TKYCustomPack()                     { FData = NULL; FSize = 0; }
   virtual ~TKYCustomPack()            {}

   // ����
   void*          Data() const         { return FData; }       // default: NULL
   Longword       Size() const         { return FSize; }       // default: 0

   // �����Զ�������ָ��
   void           SetData(void* AData) { FData = AData; }

   // ����, ��: FSize �� 0
   virtual void   Reset() = 0;

   // ѹ��������
   // 1. ABuffer ��� ASize �ֽڵĴ�ѹ������
   // 2. ADest   ���ѹ���������, �������ߴ�����Ϊ ASize
   // 3. ������ֵ > 0 ��ʾѹ�������ݵĳߴ�
   // 4. ������ֵΪ 0 ��ʾѹ��ʧ��, ����ѹ�������޷���ѹ��
   // 5. ������ֵΪ -1 ��ʾ�������Ϸ�
   virtual long   Pack(const char* ABuffer, char* ADest, long ASize) = 0;

protected:
   Longword       FSize;               // ��ѹ�����ݳߴ�

private:
   void*          FData;               // �Զ�������ָ��
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYCustomUnpack - ��ѹ������ */

class TKYCustomUnpack
{
public:
   TKYCustomUnpack()                   { FData = NULL; FSize = 0; }
   virtual ~TKYCustomUnpack()          {}

   // ����
   void*          Data() const         { return FData; }       // default: NULL
   Longword       Size() const         { return FSize; }       // default: 0

   // �����Զ�������ָ��
   void           SetData(void* AData) { FData = AData; }

   // ����, ��: FSize �� 0
   virtual void   Reset() = 0;

   // ��ѹ��������
   // 1. �� AIsPacked Ϊ false ��ʾ����δѹ��, ��Ϊ��ѹ��������������Զ�����,
   //    �����´�����ݵ� ADest, ������ֵΪ 0
   // 2. ABuffer ��� ASize �ֽڵ���ѹ������
   // 3. ADest   ��Ž�ѹ���������, ADestSize >= ASize
   // 4. ������ֵ > 0 ��ʾ��ѹ�������ݵĳߴ�
   // 5. ������ֵΪ 0 ��ʾ��ѹ��ʧ��, ���� ADestSize ̫С������δѹ��
   // 6. ������ֵΪ -1 ��ʾ�������Ϸ�
   // 7. ������ֵΪ -2 ��ʾ��ѹ��ʧ��, ԭ������������
   virtual long   Unpack(const char* ABuffer, long ASize,
                               char* ADest,   long ADestSize,
                               bool  AIsPacked = true) = 0;

protected:
   Longword       FSize;               // ��ѹ�����ݳߴ�

private:
   void*          FData;               // �Զ�������ָ��
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYPack - ѹ����(����LZ77ѹ���㷨) */

class TKYPack: public TKYCustomPack
{
private:
   // ���ӽ��
   typedef struct
   {
      Word        Hash;                // ��ϣֵ
      Word        Next;                // ��һ��
   } TNode, *PNode;

   // ѹ����
   typedef union
   {
      void*       Data;                // ����ָ��
      struct
      {
         Word     Offset;              // ƫ����
         Byte     Depth;               // ���
      } I;
   } TItem, *PItem;

   // ������
   typedef struct
   {
      Byte        Length;              // ����λ��
      char        Bits[4];             // ����
   } TCode, *PCode;

   // �����ͷ��
   typedef struct
   {
      Word        Index;               // ����, ��: ���/ƫ����
      Byte        Length;              // ����λ��
   } THead, *PHead;

   // Huffman �����
   typedef struct THuffNode
   {
      THuffNode*  Parent;              // �����
      THuffNode*  Next;                // ��һ�����
      Longword    Freq;                // Ƶ��
      Word        Index;               // ����, ��: ���/ƫ����
      Byte        Length;              // ����λ��
   } *PHuffNode;

public:
   TKYPack();
   virtual ~TKYPack();

   // ����
   bool           IsFine() const       { return FIsFine; }     // default: false
   bool           IsQuick() const      { return FIsQuick; }    // default: true
   bool           IsPacking() const    { return FIsPacking; }  // default: false
   Byte           NiceDepth() const    { return FNiceDepth; }  // default: 64

   // ��������
   void           SetIsFine(bool AIsFine)
                  { if (!FIsPacking) FIsFine = AIsFine; }
   void           SetIsQuick(bool AIsQuick)
                  { if (!FIsPacking) FIsQuick = AIsQuick; }
   void           SetNiceDepth(Byte ADepth)
                  { if (!FIsPacking) FNiceDepth = ADepth; }

   // ����
   virtual void   Reset();

   // ѹ��������
   // 1. ABuffer ��� ASize �ֽڵĴ�ѹ������
   // 2. ADest   ���ѹ���������, �������ߴ�����Ϊ ASize
   // 3. ������ֵ > 0 ��ʾѹ�������ݵĳߴ�
   // 4. ������ֵΪ 0 ��ʾѹ��ʧ��, ����ѹ�������޷���ѹ��
   // 5. ������ֵΪ -1 ��ʾ�������Ϸ�
   // 6. ������ֵΪ -2 ��ʾѹ��ʧ��, ԭ��������ѹ��
   virtual long   Pack(const char* ABuffer, char* ADest, long ASize);

private:
   void           DoCopy(const char* ABuffer, long ASize);
   void           DoHash(Byte* APos, Byte* AEnd, Word ASameLen);
   bool           DoScan(Byte* APos, Byte* AEnd,  Word  AOffset,
                                 Longword  ASize, Word& ASameLen);
   bool           DoMatch(Byte* APos,  Byte* AEnd, Word& AOffset,
                          Byte& ADepth, Word ASameLen);

   // �������� IsQuick ѹ��������
   void           DoPack0(Byte* &APos, Byte* AEnd, Longword& ABits, Longword& AOffsets);
   void           DoPack1(Byte* &APos, Byte* AEnd, Longword& ABits, Longword& AOffsets);

   // ִ�б���
   long           DoOutput(char* ADest, long ASize, Longword ABits, Longword AOffsets);
   void           DoEncode(char* &ADest,    Byte& ABitBegin,
                          TCode*  ADepths, TCode* AOffsets);

private:
   // Huffman ���� Freq �� Length ����������
   void           DoCompare1(const THuffNode* AItem1,
                             const THuffNode* AItem2, int& ACompare);
   void           DoCompare2(const THuffNode* AItem1,
                             const THuffNode* AItem2, int& ACompare);

private:
   Word*          FHash;               // ��ϣ��
   TNode*         FNodes;              // ����б�
   TKYCycQueue*   FQueue;              // ѹ������
   char*          FContext;            // ѹ�����ݵ�������
   KYString       FDepthCodes;         // ��ȵ� Huffman ����
   KYString       FDepthStats;         // ��ȵĴ���ͳ��
   Longword       FLastSize;           // ��һ�� pack ������ĳߴ�
   Byte           FNiceDepth;          // ƥ��������, ���ﵽ����Ⱦ���ֹ��һ�����ƥ��
   bool           FIsFine;             // �Ƿ�ʹ�ù�����ѹ��, ��Ϊ true ��ѹ���ٶ���Խ���
   bool           FIsQuick;            // �Ƿ�ʹ�ÿ���ѹ��, ��Ϊ true ��ѹ������Խϲ�
   bool           FIsPacking;          // �Ƿ�����ѹ��
   bool           FIsOverflow;         // �������б��Ƿ����

private:
   // ��ʼ����Ⱥ�ƫ�����ľ�̬�����
   static void     _Init_D0_2_Codes(TCode* ACodes, const Byte* ABase,
                               const Byte* ASuffix,      Byte  AHigh);
   static void     _Init_D3_5_Codes(TCode* ACodes, const Byte* ABase,
                               const Byte* ASuffix, Byte AHigh, Byte APrefix);
   static void     _Init_Offset_Codes();

   // ���ģʽ��ͳ�ƺ���
   static Longword _CalcDepthStats(Longword* ADepthStats, const TCode* ACodes);
   static Longword _SelectDepthModel(Longword* ADepthStats, Byte& AModel);
   static void     _WriteDepth(char* &APos, Byte& ABitBegin, Byte ADepth);

   // Huffman �������ط���
   static void     _AddStats(Longword* AStats, TKYList& AList,
                             KYString& ANodes,    Word  AHigh);
   static Longword _BuildTree(Longword* AStats, KYString& AHeads, Word AHigh);
   static Longword _BuildCodes(Longword* AStats, TCode* ACodes,
                               KYString& AHeads,  Byte& ADelta, Word AHigh);
   static Longword _CalcHeadBits(const KYString& AHeads, Byte ADelta, Word AHigh);
   static void     _WriteHeadBits(char* &APos,   Byte& ABitBegin,
                         const KYString& AHeads, Byte  ADelta, Word AHigh);

private:
   // ��Ⱥ�ƫ�����ľ�̬�����
   static TCode    _Codes_D0[256];
   static TCode    _Codes_D1[256];
   static TCode    _Codes_D2[256];
   static TCode    _Codes_D3[256];
   static TCode    _Codes_D4[256];
   static TCode    _Codes_D5[256];
   static TCode    _Codes_Offset[0x8000];

   // �����ģʽ�ľ�̬�����
   static TCode*  _Codes_Depth[6];

   // TKYPack �ľ�̬��Ա��ʼ����
   static class TInitialization
   {
   public:
      TInitialization();
      ~TInitialization();
   } _Initialization;

   friend class TInitialization;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYUnpack - ��ѹ����(����LZ77ѹ���㷨) */

class TKYUnpack: public TKYCustomUnpack
{
private:
   // ������
   typedef struct
   {
      Longword    Code;                // ��ȱ���
      Longword    Short;               // ��һλ�ı���
      Byte        Index;               // ��ȱ������
      Byte        Length;              // ���볤��
   } TIndex;

public:
   TKYUnpack();
   virtual ~TKYUnpack();

   // ����
   bool           IsUnpacking() const  { return FIsUnpacking; }// default: false

   // ����
   virtual void   Reset();

   // ��ѹ��������
   // 1. �� AIsPacked Ϊ false ��ʾ����δѹ��, ��Ϊ��ѹ��������������Զ�����,
   //    �����´�����ݵ� ADest, ������ֵΪ 0
   // 2. ABuffer ��� ASize �ֽڵ���ѹ������
   // 3. ADest   ��Ž�ѹ���������, ADestSize >= ASize
   // 4. ������ֵ > 0 ��ʾ��ѹ�������ݵĳߴ�
   // 5. ������ֵΪ 0 ��ʾ��ѹ��ʧ��, ���� ADestSize ̫С������δѹ��
   // 6. ������ֵΪ -1 ��ʾ�������Ϸ�
   // 7. ������ֵΪ -2 ��ʾ��ѹ��ʧ��, ԭ������������
   // 8. ������ֵΪ -3 ��ʾ��ѹ��ʧ��, ԭ�������ڽ�ѹ��
   virtual long   Unpack(const char* ABuffer, long ASize,
                               char* ADest,   long ADestSize,
                               bool  AIsPacked = true);

private:
   void           DoCopy(const char* ABuffer, long ASize);
   bool           DoInsert(char* &APos, Word AOffset, Byte ADepth);

   // ִ�н���
   long           DoDecode(char* ABuff, char* ABuffEnd,
                           char* ADest, char* ADestEnd);
   long           DoDecode(char* ABuff, char* ABuffEnd,
                           char* ADest, char* ADestEnd, Byte AModel);

private:
   char*          FContext;            // �ѽ�ѹ��������
   Longword       FLastSize;           // ��һ�� unpack ������ĳߴ�
   bool           FIsOverflow;         // �ж��������б��Ƿ����
   bool           FIsUnpacking;        // �ж��Ƿ����ڽ�ѹ��

private:
   // Huffman �������ط���
   static bool    _ReadHeadBits(char* &APos, char* AEnd, Byte& ABitBegin,
                            KYString&  ADepths,      KYString& AIndexs);
   static bool    _DecodeDepth(char* &APos,        char* AEnd,
                               Byte&  ABitBegin,   Byte& ADepth,
                               Byte*  ADepths, Longword  ADCount,
                             TIndex*  AIndexs,     Byte  AICount);
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ������λ������غ��� */

// ������ <-> λ����
// ע: APos, AEnd �� ABits �ǿ�, ABitBegin ȡֵ [0..7], ABitCount > 0
void        BitsToBuffer(char* &APos, Byte& ABitBegin,
                                const char* ABits, long ABitCount);
void        BufferToBits(char* &APos, Byte& ABitBegin,
                                      char* ABits, long  ABitCount);

inline bool BitsToBuffer(char* &APos, char* AEnd,  Byte& ABitBegin,
                                const char* ABits, long  ABitCount)
{
   if (APos + ((ABitCount + ABitBegin - 1) >> 3) < AEnd)
   {
      BitsToBuffer(APos, ABitBegin, ABits, ABitCount);
      return true;
   }
   else
      return false;
}

inline bool BufferToBits(char* &APos, char* AEnd,  Byte& ABitBegin,
                                      char* ABits, long  ABitCount)
{
   if (APos + ((ABitCount + ABitBegin - 1) >> 3) < AEnd)
   {
      BufferToBits(APos, ABitBegin, ABits, ABitCount);
      return true;
   }
   else
      return false;
}

// ������ <-> Boolean (1 bit)
// ע: APos �� AEnd �ǿ�, ABitBegin ȡֵ [0..7]
void        BoolToBuffer(char* &APos, Byte& ABitBegin, bool AValue);
void        BufferToBool(char* &APos, Byte& ABitBegin, bool& AValue);

inline bool BoolToBuffer(char* &APos, char* AEnd, Byte& ABitBegin, bool AValue)
{
   if (APos < AEnd)
   {
      BoolToBuffer(APos, ABitBegin, AValue);
      return true;
   }
   else
      return false;
}

inline bool BufferToBool(char* &APos, char* AEnd, Byte& ABitBegin, bool& AValue)
{
   if (APos < AEnd)
   {
      BufferToBool(APos, ABitBegin, AValue);
      return true;
   }
   else
      return false;
}

// ������ <-> char
// ע: APos �� AEnd �ǿ�, ABitBegin ȡֵ [0..7]
void        CharToBuffer(char* &APos, Byte& ABitBegin, char AValue);
void        BufferToChar(char* &APos, Byte& ABitBegin, char& AValue);

inline bool CharToBuffer(char* &APos, char* AEnd, Byte& ABitBegin, char AValue)
{
   if (APos + ((ABitBegin + 7) >> 3) < AEnd)
   {
      CharToBuffer(APos, ABitBegin, AValue);
      return true;
   }
   else
      return false;
}

inline bool BufferToChar(char* &APos, char* AEnd, Byte& ABitBegin, char& AValue)
{
   if (APos + ((ABitBegin + 7) >> 3) < AEnd)
   {
      BufferToChar(APos, ABitBegin, AValue);
      return true;
   }
   else
      return false;
}

// ������ <-> Word
// ע: APos �� AEnd �ǿ�, ABitBegin ȡֵ [0..7]
void        WordToBuffer(char* &APos, Byte& ABitBegin, Word  AValue);
void        BufferToWord(char* &APos, Byte& ABitBegin, Word& AValue);

inline bool WordToBuffer(char* &APos, char* AEnd, Byte& ABitBegin, Word  AValue)
{
   if (APos + ((ABitBegin + 15) >> 3) < AEnd)
   {
      WordToBuffer(APos, ABitBegin, AValue);
      return true;
   }
   else
      return false;
}

inline bool BufferToWord(char* &APos, char* AEnd, Byte& ABitBegin, Word& AValue)
{
   if (APos + ((ABitBegin + 15) >> 3) < AEnd)
   {
      BufferToWord(APos, ABitBegin, AValue);
      return true;
   }
   else
      return false;
}

// ������ <-> long
// ע: APos �� AEnd �ǿ�, ABitBegin ȡֵ [0..7]
void        LongToBuffer(char* &APos, Byte& ABitBegin, long  AValue);
void        BufferToLong(char* &APos, Byte& ABitBegin, long& AValue);

inline bool LongToBuffer(char* &APos, char* AEnd, Byte& ABitBegin, long  AValue)
{
   if (APos + ((ABitBegin + 31) >> 3) < AEnd)
   {
      LongToBuffer(APos, ABitBegin, AValue);
      return true;
   }
   else
      return false;
}

inline bool BufferToLong(char* &APos, char* AEnd, Byte& ABitBegin, long& AValue)
{
   if (APos + ((ABitBegin + 31) >> 3) < AEnd)
   {
      BufferToLong(APos, ABitBegin, AValue);
      return true;
   }
   else
      return false;
}

// Byte ���λ�� <-> ������
// ע: APos �� AEnd �ǿ�, ABitBegin ȡֵ [0..7], AMax ȡֵ [1..8]
inline void ByteToBuffer(char* &APos, Byte& ABitBegin, Byte AValue, Byte AMax)
{
   AValue <<= 8 - AMax;
   BitsToBuffer(APos, ABitBegin, (char*)&AValue, AMax);
}

inline void BufferToByte(char* &APos, Byte& ABitBegin, Byte& AValue, Byte AMax)
{
   BufferToBits(APos, ABitBegin, (char*)&AValue, AMax);
   AValue >>= 8 - AMax;
}

inline bool ByteToBuffer(char* &APos, char* AEnd,   Byte& ABitBegin,
                                      Byte  AValue, Byte  AMax)
{
   AValue <<= 8 - AMax;
   return BitsToBuffer(APos, AEnd, ABitBegin, (char*)&AValue, AMax);
}

inline bool BufferToByte(char* &APos, char* AEnd,   Byte& ABitBegin,
                                      Byte& AValue, Byte  AMax)
{
   bool result = BufferToBits(APos, AEnd, ABitBegin, (char*)&AValue, AMax);
   if (result)
      AValue >>= 8 - AMax;
   return result;
}

// Word ���λ�� <-> ������
// ע: APos �� AEnd �ǿ�, ABitBegin ȡֵ [0..7], AMax ȡֵ [1..16]
void        WordToBuffer(char* &APos, Byte& ABitBegin, Word  AValue, Byte AMax);
void        BufferToWord(char* &APos, Byte& ABitBegin, Word& AValue, Byte AMax);

bool        WordToBuffer(char* &APos, char* AEnd,   Byte& ABitBegin,
                                      Word  AValue, Byte  AMax);
bool        BufferToWord(char* &APos, char* AEnd,   Byte& ABitBegin,
                                      Word& AValue, Byte  AMax);

}

#endif
