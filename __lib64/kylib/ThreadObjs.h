// =======================================
// Unit   : 线程对象单元 (ThreadObjs.h)
// Version: 3.0.0.0 (build 2012.06.04)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _ThreadObjs_H_
#define _ThreadObjs_H_

#include "KYList.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TThreadStrings - 线程字符串列表类 */

class TThreadStrings
{
   // 线程字符串项
   typedef struct
   {
      Longword    ThreadID;
      KYString    Value;
   } TItem, *PItem;

public:
   TThreadStrings();
   virtual ~TThreadStrings();

   // 字符串列表个数
   long           Count() const        { return FItems->Count(); }

   // 指定索引的字符串值, AIndex 取值范围: [0..Count()-1]
   KYString       Value(long AIndex) const;

   // 指定索引的线程 ID, AIndex 取值范围: [0..Count()-1]
   Longword       ThreadID(long AIndex) const;

   // 当前线程的字符串值
   KYString       ThreadString() const;

   // 设置当前线程的字符串值
   void           SetThreadString(const KYString& AValue);

   // 清除所有项
   void           Clear();

   // 当前线程的 attch/detach, 此对象基本上用于动态库且返回值为 char* 的接口函数
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
