// =======================================
// Unit   : �̶߳���Ԫ (ThreadObjs.h)
// Version: 3.0.0.0 (build 2012.06.04)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _ThreadObjs_H_
#define _ThreadObjs_H_

#include "KYList.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TThreadStrings - �߳��ַ����б��� */

class TThreadStrings
{
   // �߳��ַ�����
   typedef struct
   {
      Longword    ThreadID;
      KYString    Value;
   } TItem, *PItem;

public:
   TThreadStrings();
   virtual ~TThreadStrings();

   // �ַ����б����
   long           Count() const        { return FItems->Count(); }

   // ָ���������ַ���ֵ, AIndex ȡֵ��Χ: [0..Count()-1]
   KYString       Value(long AIndex) const;

   // ָ���������߳� ID, AIndex ȡֵ��Χ: [0..Count()-1]
   Longword       ThreadID(long AIndex) const;

   // ��ǰ�̵߳��ַ���ֵ
   KYString       ThreadString() const;

   // ���õ�ǰ�̵߳��ַ���ֵ
   void           SetThreadString(const KYString& AValue);

   // ���������
   void           Clear();

   // ��ǰ�̵߳� attch/detach, �˶�����������ڶ�̬���ҷ���ֵΪ char* �Ľӿں���
   void           ThreadAttach();
   void           ThreadDetach();

protected:
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

private:
   void*          AddItem(Longword AThreadID);
   void           DeleteItem(Longword AThreadID);

   void           DoCompare(const void* AItem1, const void* AItem2, int& ACompare);
   void           DoDeletion(void* AItem);

private:
   TKYCritSect*   FLock;
   TKYList*       FItems;
};

}

#endif
