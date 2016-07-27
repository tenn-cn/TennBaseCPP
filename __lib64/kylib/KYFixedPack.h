// =======================================
// Unit   : �̶���������ѹ��/��ѹ����Ԫ
// Version: 3.0.0.0 (build 2012.04.19)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYFixedPack_H_
#define _KYFixedPack_H_

#include "KYPackObj.h"

namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYFixedPack - �̶���������ѹ����(����LZѹ���㷨) */

class TKYFixedPack
{
public:
   TKYFixedPack();
   virtual ~TKYFixedPack();

   // ����
   char*          Dest() const         { return FDest; }       // default: NULL
   Word           Size() const         { return FSize; }       // default: 0
   Word           MaxSize() const      { return FMaxSize; }    // default: 0
   Longword       DataSize() const     { return FDataSize; }   // default: 0
   bool           IsPacking() const    { return FIsPacking; }  // default: false

   // ����ѹ����������������ߴ�
   bool           SetMaxSize(Word ASize);

   // ����
   void           Reset();

   // ѹ��������
   bool           Pack(const char* ABuffer, long ASize, Word AMax = 0xFFFF);

private:
   void           DoCopy(const char* ABuffer, long ASize);
   bool           DoMatch(Byte* APos, Byte* AEnd, Word& AOffset, Byte& ADepth);

private:
   char           FContext[1024];      // ѹ�����ݵ�������
   Word           FHash[4096];         // ��ϣ��
   char*          FDest;               // ѹ����Ļ�����
   Word           FSize;               // ѹ�������ݳߴ�
   Word           FMaxSize;            // ���������ߴ�
   Longword       FDataSize;           // ��ѹ�����ݳߴ�
   Longword       FLastSize;           // ��һ�� pack ������ĳߴ�
   Byte           FBitBegin;           // ��һ���ֽ����ݿ�ʼλ
   bool           FIsPacking;          // �ж��Ƿ�����ѹ��

private:
   // ������
   typedef struct
   {
      Byte        Length;              // ����λ��
      char        Bits[3];             // ����
   } TCode, *PCode;

private:
   // ��ʼ����Ⱥ�ƫ�����ľ�̬�����
   static void    _Init_Depth_Codes(TCode* ACodes, const Byte* ABase,
                               const Byte* ASuffix,      Byte  AHigh);
   static void    _Init_Offset_Codes();

private:
   // ��Ⱥ�ƫ�����ľ�̬�����
   static TCode   _Codes_Depth[256];
   static TCode   _Codes_Offset[1024];

   // TKYFixedPack �ľ�̬��Ա��ʼ����
   static class TInitialization
   {
   public:
      TInitialization();
      ~TInitialization();
   } _Initialization;

   friend class TInitialization;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYFixedUnpack - �̶��������Ľ�ѹ����(����LZѹ���㷨) */

class TKYFixedUnpack
{
public:
   // ��ѹ��������
   // 1. ABuffer ��� ASize �ֽڵ���ѹ������
   // 2. ADest   ��Ž�ѹ���������, ADestSize >= ASize
   // 3. ������ֵ > 0 ��ʾ��ѹ�������ݵĳߴ�
   // 4. ������ֵΪ 0 ��ʾ��ѹ��ʧ��, ���� ADestSize ̫С������δѹ��
   // 5. ������ֵΪ -1 ��ʾ�������Ϸ�
   // 6. ������ֵΪ -2 ��ʾ��ѹ��ʧ��, ԭ������������
   static long    Unpack(const char* ABuffer, long ASize,
                               char* ADest,   long ADestSize);

};

}

#endif
