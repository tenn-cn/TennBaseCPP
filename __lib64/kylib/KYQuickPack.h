// =======================================
// Unit   : ����ѹ��/��ѹ����Ԫ (KYQuickPack.h)
// Version: 3.0.0.0 (build 2011.03.08)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYQuickPack_H_
#define _KYQuickPack_H_

#include "KYPackObj.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYQuickPack - ����ѹ����(����LZRWѹ���㷨) */

class TKYQuickPack: public TKYCustomPack
{
public:
   TKYQuickPack();
   virtual ~TKYQuickPack();

   // ����
   bool           IsPacking() const    { return FIsPacking; }  // default: false

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
   bool           DoMatch(Byte* APos, Byte* AEnd, Word& AOffset, Byte& ADepth);

private:
   Word           FHash[4096];         // ��ϣ��
   char           FContext[4096];      // ѹ�����ݵ�������
   Longword       FLastSize;           // ��һ�� pack ������ĳߴ�
   bool           FIsPacking;          // �ж��Ƿ�����ѹ��
   bool           FIsOverflow;         // �ж��������б��Ƿ����
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYQuickUnpack - ���ٽ�ѹ����(����LZRWѹ���㷨) */

class TKYQuickUnpack: public TKYCustomUnpack
{
public:
   TKYQuickUnpack();
   virtual ~TKYQuickUnpack();

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

private:
   char           FContext[4096];      // �ѽ�ѹ��������
   Longword       FLastSize;           // ��һ�� unpack ������ĳߴ�
   bool           FIsOverflow;         // �ж��������б��Ƿ����
   bool           FIsUnpacking;        // �ж��Ƿ����ڽ�ѹ��
};

}

#endif
