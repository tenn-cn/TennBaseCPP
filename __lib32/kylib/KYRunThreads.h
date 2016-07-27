// =======================================
// Unit   : �����̼߳���
// Version: 3.0.4.0 (build 2013.07.16)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYRunThreads_H_
#define _KYRunThreads_H_

#include "KYString.h"
#include "KYThread.h"
#include "KYHandles.h"
#include "KYCycQueue.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYRunThread - �����߳��� */

class TKYRunThread: public TThread
{
public:
   // �������͵Ķ���
   // ע: ����ָ�������ָ��һ�����Դ��ݲ�����, ֻ�Ƿ���ָ��Ⱥ���ָ�����Ҫ
   //     һ�������Ķ���ָ�����, ��ͬ��������ʹ�ò�ͬ���͵ķ���ָ��
   typedef void (TObject::*TDoCustom)();
   typedef void (TObject::*TDoStr1)(const KYString& AStr);
   typedef void (TObject::*TDoStr2)(const KYString& AStr1, const KYString& AStr2);
   typedef void (TObject::*TDoStr3)(const KYString& AStr1, const KYString& AStr2,
                                    const KYString& AStr3);

   typedef void (TObject::*TDoInt1)(long AInt);
   typedef void (TObject::*TDoInt1Str1)(long AInt1, const KYString& AStr);
   typedef void (TObject::*TDoInt1Str2)(long AInt1, const KYString& AStr1,
                                                    const KYString& AStr2);
   typedef void (TObject::*TDoInt1Str3)(long AInt1, const KYString& AStr1,
                                                    const KYString& AStr2,
                                                    const KYString& AStr3);

   typedef void (TObject::*TDoInt2)(long AInt1, long AInt2);
   typedef void (TObject::*TDoInt2Str1)(long AInt1, long  AInt2,
                                          const KYString& AStr);
   typedef void (TObject::*TDoInt2Str2)(long AInt1, long  AInt2,
                                          const KYString& AStr1,
                                          const KYString& AStr2);
   typedef void (TObject::*TDoInt2Str3)(long AInt1, long  AInt2,
                                          const KYString& AStr1,
                                          const KYString& AStr2,
                                          const KYString& AStr3);

   typedef void (TObject::*TDoFloat)(double AFloat);
   typedef void (TObject::*TDoF1Str1)(double AFloat, const KYString& AStr);
   typedef void (TObject::*TDoF1Str2)(double AFloat, const KYString& AStr1,
                                                     const KYString& AStr2);
   typedef void (TObject::*TDoF1Str3)(double AFloat, const KYString& AStr1,
                                                     const KYString& AStr2,
                                                     const KYString& AStr3);

   typedef void (TObject::*TDoInt64)(__int64 AInt64);
   typedef void (TObject::*TDoI64Str1)(__int64 AInt64, const KYString& AStr);
   typedef void (TObject::*TDoI64Str2)(__int64 AInt64, const KYString& AStr1,
                                                       const KYString& AStr2);
   typedef void (TObject::*TDoI64Str3)(__int64 AInt64, const KYString& AStr1,
                                                       const KYString& AStr2,
                                                       const KYString& AStr3);

   typedef void (TObject::*TDoP1)(void* AValue);
   typedef void (TObject::*TDoP1Str1)(void* AValue1, const KYString& AStr);
   typedef void (TObject::*TDoP1Str2)(void* AValue1, const KYString& AStr1,
                                                     const KYString& AStr2);
   typedef void (TObject::*TDoP1Str3)(void* AValue1, const KYString& AStr1,
                                                     const KYString& AStr2,
                                                     const KYString& AStr3);

   typedef void (TObject::*TDoP2)(void* AValue1, void* AValue2);
   typedef void (TObject::*TDoP2Str1)(void* AValue1, void* AValue2,
                                           const KYString& AStr);
   typedef void (TObject::*TDoP2Str2)(void* AValue1, void* AValue2,
                                           const KYString& AStr1,
                                           const KYString& AStr2);
   typedef void (TObject::*TDoP2Str3)(void* AValue1, void* AValue2,
                                           const KYString& AStr1,
                                           const KYString& AStr2,
                                           const KYString& AStr3);

private:
   // ��������
   enum TMethodType {mtCustom,   mtStr1,     mtStr2,     mtStr3,
                     mtInt1,     mtInt1Str1, mtInt1Str2, mtInt1Str3,
                     mtInt2,     mtInt2Str1, mtInt2Str2, mtInt2Str3,
                     mtFloat,    mtF1Str1,   mtF1Str2,   mtF1Str3,
                     mtInt64,    mtI64Str1,  mtI64Str2,  mtI64Str3,
                     mtP1,       mtP1Str1,   mtP1Str2,   mtP1Str3,
                     mtP2,       mtP2Str1,   mtP2Str2,   mtP2Str3};

   // ֵ��
   typedef struct
   {
      void*       P1;                  // ֵ1
      void*       P2;                  // ֵ2
   } TValue, *PValue;

   // ����ֵ��
   typedef union
   {
      TValue      V;                   // ֵ��
      __int64     VInt64;              // 64λ����ֵ
      double      Float;               // ����ֵ
   } TUnionValue, *PUnionValue;

   // ������
   typedef struct
   {
      TMethodType Type;                // ��������
      TDoCustom   Method;              // ����ָ��
      void*       Object;              // �����Ķ���ָ��

      TUnionValue Value;               // ֵ��
      KYString    Str1;                // �ַ���1
      KYString    Str2;                // �ַ���2
      KYString    Str3;                // �ַ���3
   } TItem, *PItem;

public:
   TKYRunThread(long AItemNo = 0, long ACacheSize = Default_CacheSize);
   virtual ~TKYRunThread();

   // ����
   void*          Data() const         { return FData; }             // default: NULL
   long           Count() const        { return FQueue->Count(); }   // default: 0
   long           ItemNo() const       { return FItemNo; }           // default: 0
   long           RefCount() const     { return FRefCount; }         // default: 0
   long           PerfCount() const    { return FPerfCount; }        // default: 0

   // ��������
   void           SetData(void* AData) { FData = AData; }

   // ����ִ�е����
   void           ResetPerfCount()     { FPerfCount = 0; }
   long           IncPerfCount(long ACount = 1)
                  { FPerfCount += ACount; return FPerfCount; }

   // �ر��߳�, ���� FreeKYRunThread �ͷ��߳�
   void           Close();

   // ��������е�����ִ�з���
   void           Clear();

   // ��� TDoCustom ���͵ķ�����ִ���߳�
   bool           AddOfCustom(TDoCustom AMethod, void* AObject)
                  { return Add(mtCustom, AMethod, AObject, NULL, NULL, "", "", ""); }

   // ��� TDoStr1 ���͵ķ�����ִ���߳�
   bool           AddOfStr1(TDoStr1 AMethod, void* AObject, const KYString& AStr)
                  { return Add(mtStr1, (TDoCustom)AMethod, AObject,
                                       NULL, NULL, AStr, "", ""); }

   // ��� TDoStr2 ���͵ķ�����ִ���߳�
   bool           AddOfStr2(TDoStr2 AMethod, void* AObject, const KYString& AStr1,
                                                            const KYString& AStr2)
                  { return Add(mtStr2, (TDoCustom)AMethod, AObject,
                                       NULL, NULL, AStr1, AStr2, ""); }

   // ��� TDoStr3 ���͵ķ�����ִ���߳�
   bool           AddOfStr3(TDoStr3 AMethod, void* AObject, const KYString& AStr1,
                                                            const KYString& AStr2,
                                                            const KYString& AStr3)
                  { return Add(mtStr3, (TDoCustom)AMethod, AObject,
                                       NULL, NULL, AStr1, AStr2, AStr3); }

   // ��� TDoInt1 ���͵ķ�����ִ���߳�
   bool           AddOfInt1(TDoInt1 AMethod, void* AObject, long AInt)
                  { return Add(mtInt1, (TDoCustom)AMethod, AObject, (void*)AInt,
                                       NULL, "", "", ""); }

   // ��� mtInt1Str1 ���͵ķ�����ִ���߳�
   bool           AddOfInt1Str1(TDoInt1Str1 AMethod, void* AObject, long AInt1,
                                                         const KYString& AStr)
                  { return Add(mtInt1Str1, (TDoCustom)AMethod, AObject,
                                           (void*)AInt1, NULL, AStr, "", ""); }

   // ��� TDoInt1Str2 ���͵ķ�����ִ���߳�
   bool           AddOfInt1Str2(TDoInt1Str2 AMethod, void* AObject, long AInt1,
                                                         const KYString& AStr1,
                                                         const KYString& AStr2)
                  { return Add(mtInt1Str2, (TDoCustom)AMethod, AObject,
                                           (void*)AInt1, NULL, AStr1, AStr2, ""); }

   // ��� TDoInt1Str3 ���͵ķ�����ִ���߳�
   bool           AddOfInt1Str3(TDoInt1Str3 AMethod, void* AObject, long AInt1,
                                                         const KYString& AStr1,
                                                         const KYString& AStr2,
                                                         const KYString& AStr3)
                  { return Add(mtInt1Str3, (TDoCustom)AMethod, AObject,
                                           (void*)AInt1, NULL, AStr1, AStr2, AStr3); }

   // ��� TDoInt2 ���͵ķ�����ִ���߳�
   bool           AddOfInt2(TDoInt2 AMethod, void* AObject, long AInt1, long AInt2)
                  { return Add(mtInt2, (TDoCustom)AMethod, AObject, (void*)AInt1,
                                       (void*)AInt2, "", "", ""); }

   // ��� TDoInt2Str1 ���͵ķ�����ִ���߳�
   bool           AddOfInt2Str1(TDoInt2Str1 AMethod, void* AObject, long AInt1, long AInt2,
                                                         const KYString& AStr)
                  { return Add(mtInt2Str1, (TDoCustom)AMethod, AObject,
                                           (void*)AInt1, (void*)AInt2, AStr, "", ""); }

   // ��� TDoInt2Str2 ���͵ķ�����ִ���߳�
   bool           AddOfInt2Str2(TDoInt2Str2 AMethod, void* AObject, long AInt1, long AInt2,
                                                         const KYString& AStr1,
                                                         const KYString& AStr2)
                  { return Add(mtInt2Str2, (TDoCustom)AMethod, AObject,
                                           (void*)AInt1, (void*)AInt2, AStr1, AStr2, ""); }

   // ��� TDoInt2Str3 ���͵ķ�����ִ���߳�
   bool           AddOfInt2Str3(TDoInt2Str3 AMethod, void* AObject, long AInt1, long AInt2,
                                                         const KYString& AStr1,
                                                         const KYString& AStr2,
                                                         const KYString& AStr3)
                  { return Add(mtInt2Str3, (TDoCustom)AMethod, AObject,
                                           (void*)AInt1, (void*)AInt2, AStr1, AStr2, AStr3); }

   // ��� TDoFloat ���͵ķ�����ִ���߳�
   bool           AddOfFloat(TDoFloat AMethod, void* AObject, double AFloat)
                  { return Add(mtFloat, (TDoCustom)AMethod, AObject,
                                        AFloat, "", "", ""); }

   // ��� TDoF1Str1 ���͵ķ�����ִ���߳�
   bool           AddOfF1Str1(TDoF1Str1 AMethod, void* AObject, double AFloat,
                                                         const KYString& AStr)
                  { return Add(mtF1Str1, (TDoCustom)AMethod, AObject,
                                         AFloat, AStr, "", ""); }

   // ��� TDoF1Str2 ���͵ķ�����ִ���߳�
   bool           AddOfF1Str2(TDoF1Str2 AMethod, void* AObject, double AFloat,
                                                         const KYString& AStr1,
                                                         const KYString& AStr2)
                  { return Add(mtF1Str2, (TDoCustom)AMethod, AObject,
                                         AFloat, AStr1, AStr2, ""); }

   // ��� TDoF1Str3 ���͵ķ�����ִ���߳�
   bool           AddOfF1Str3(TDoF1Str3 AMethod, void* AObject, double AFloat,
                                                         const KYString& AStr1,
                                                         const KYString& AStr2,
                                                         const KYString& AStr3)
                  { return Add(mtF1Str3, (TDoCustom)AMethod, AObject,
                                         AFloat, AStr1, AStr2, AStr3); }

   // ��� TDoInt64 ���͵ķ�����ִ���߳�
   bool           AddOfInt64(TDoInt64 AMethod, void* AObject, __int64 AInt64)
                  { return Add(mtInt64, (TDoCustom)AMethod, AObject,
                                        AInt64, "", "", ""); }

   // ��� TDoI64Str1 ���͵ķ�����ִ���߳�
   bool           AddOfI64Str1(TDoI64Str1 AMethod, void* AObject, __int64 AInt64,
                                                          const KYString& AStr)
                  { return Add(mtI64Str1, (TDoCustom)AMethod, AObject,
                                          AInt64, AStr, "", ""); }

   // ��� TDoI64Str2 ���͵ķ�����ִ���߳�
   bool           AddOfI64Str2(TDoI64Str2 AMethod, void* AObject, __int64 AInt64,
                                                          const KYString& AStr1,
                                                          const KYString& AStr2)
                  { return Add(mtI64Str2, (TDoCustom)AMethod, AObject,
                                          AInt64, AStr1, AStr2, ""); }

   // ��� TDoI64Str3 ���͵ķ�����ִ���߳�
   bool           AddOfI64Str3(TDoI64Str3 AMethod, void* AObject, __int64 AInt64,
                                                          const KYString& AStr1,
                                                          const KYString& AStr2,
                                                          const KYString& AStr3)
                  { return Add(mtI64Str3, (TDoCustom)AMethod, AObject,
                                          AInt64, AStr1, AStr2, AStr3); }

   // ��� TDoP1 ���͵ķ�����ִ���߳�
   bool           AddOfP1(TDoP1 AMethod, void* AObject, void* AValue)
                  { return Add(mtP1, (TDoCustom)AMethod, AObject, AValue,
                                     NULL, "", "", ""); }

   // ��� mtP1Str1 ���͵ķ�����ִ���߳�
   bool           AddOfP1Str1(TDoP1Str1 AMethod, void* AObject, void* AValue1,
                                                      const KYString& AStr)
                  { return Add(mtP1Str1, (TDoCustom)AMethod, AObject,
                                         AValue1, NULL, AStr, "", ""); }

   // ��� TDoP1Str2 ���͵ķ�����ִ���߳�
   bool           AddOfP1Str2(TDoP1Str2 AMethod, void* AObject, void* AValue1,
                                                      const KYString& AStr1,
                                                      const KYString& AStr2)
                  { return Add(mtP1Str2, (TDoCustom)AMethod, AObject,
                                         AValue1, NULL, AStr1, AStr2, ""); }

   // ��� TDoP1Str3 ���͵ķ�����ִ���߳�
   bool           AddOfP1Str3(TDoP1Str3 AMethod, void* AObject, void* AValue1,
                                                      const KYString& AStr1,
                                                      const KYString& AStr2,
                                                      const KYString& AStr3)
                  { return Add(mtP1Str3, (TDoCustom)AMethod, AObject,
                                         AValue1, NULL, AStr1, AStr2, AStr3); }

   // ��� TDoP2 ���͵ķ�����ִ���߳�
   bool           AddOfP2(TDoP2 AMethod, void* AObject, void* AValue1, void* AValue2)
                  { return Add(mtP2, (TDoCustom)AMethod, AObject, AValue1,
                                     AValue2, "", "", ""); }

   // ��� TDoP2Str1 ���͵ķ�����ִ���߳�
   bool           AddOfP2Str1(TDoP2Str1 AMethod, void* AObject, void* AValue1,
                                  void* AValue2,      const KYString& AStr)
                  { return Add(mtP2Str1, (TDoCustom)AMethod, AObject,
                                         AValue1, AValue2, AStr, "", ""); }

   // ��� TDoP2Str2 ���͵ķ�����ִ���߳�
   bool           AddOfP2Str2(TDoP2Str2 AMethod, void* AObject, void* AValue1,
                                  void* AValue2,      const KYString& AStr1,
                                                      const KYString& AStr2)
                  { return Add(mtP2Str2, (TDoCustom)AMethod, AObject,
                                         AValue1, AValue2, AStr1, AStr2, ""); }

   // ��� TDoP2Str3 ���͵ķ�����ִ���߳�
   bool           AddOfP2Str3(TDoP2Str3 AMethod, void* AObject, void* AValue1,
                                  void* AValue2,      const KYString& AStr1,
                        const KYString& AStr2,        const KYString& AStr3)
                  { return Add(mtP2Str3, (TDoCustom)AMethod, AObject,
                                         AValue1, AValue2, AStr1, AStr2, AStr3); }

protected:
   // ѹ����
   void           LockPush() const     { FLockPush->Enter(); }
   void           UnlockPush() const   { FLockPush->Leave(); }

   // ������
   void           LockPop() const      { FLockPop->Enter(); }
   void           UnlockPop() const    { FLockPop->Leave(); }

   // ���ü���(�� TKYRunThreads ��ʹ��, һ�㲻��Ҫ����)
   long           IncRefCount()        { return InterlockedIncrement(&FRefCount); }
   long           DecRefCount()        { return InterlockedDecrement(&FRefCount); }

   // �߳�ִ����
   virtual void   Execute();

private:
   // ��ӷ�����
   bool           Add(TMethodType AType, TDoCustom AMethod, void* AObject,
                            void* AP1, void* AP2, const KYString& AStr1,
                  const KYString& AStr2,          const KYString& AStr3);

   bool           Add(TMethodType AType, TDoCustom AMethod, void* AObject,
                          __int64 AInt64,         const KYString& AStr1,
                  const KYString& AStr2,          const KYString& AStr3);

   bool           Add(TMethodType AType, TDoCustom AMethod, void* AObject,
                           double AFloat,         const KYString& AStr1,
                  const KYString& AStr2,          const KYString& AStr3);

   // ִ�з����ķ���
   void           DoMethodOfCustom(const TItem& AItem);
   void           DoMethodOfStr1(const TItem& AItem);
   void           DoMethodOfStr2(const TItem& AItem);
   void           DoMethodOfStr3(const TItem& AItem);

   void           DoMethodOfInt1(const TItem& AItem);
   void           DoMethodOfInt1Str1(const TItem& AItem);
   void           DoMethodOfInt1Str2(const TItem& AItem);
   void           DoMethodOfInt1Str3(const TItem& AItem);

   void           DoMethodOfInt2(const TItem& AItem);
   void           DoMethodOfInt2Str1(const TItem& AItem);
   void           DoMethodOfInt2Str2(const TItem& AItem);
   void           DoMethodOfInt2Str3(const TItem& AItem);

   void           DoMethodOfFloat(const TItem& AItem);
   void           DoMethodOfF1Str1(const TItem& AItem);
   void           DoMethodOfF1Str2(const TItem& AItem);
   void           DoMethodOfF1Str3(const TItem& AItem);

   void           DoMethodOfInt64(const TItem& AItem);
   void           DoMethodOfI64Str1(const TItem& AItem);
   void           DoMethodOfI64Str2(const TItem& AItem);
   void           DoMethodOfI64Str3(const TItem& AItem);

   void           DoMethodOfP1(const TItem& AItem);
   void           DoMethodOfP1Str1(const TItem& AItem);
   void           DoMethodOfP1Str2(const TItem& AItem);
   void           DoMethodOfP1Str3(const TItem& AItem);

   void           DoMethodOfP2(const TItem& AItem);
   void           DoMethodOfP2Str1(const TItem& AItem);
   void           DoMethodOfP2Str2(const TItem& AItem);
   void           DoMethodOfP2Str3(const TItem& AItem);

private:
   // �½���ɾ����
   TItem*         NewItem();
   void           DeleteItem(TItem* AItem);

private:
   void*          FData;               // �Զ�������
   long           FItemNo;             // ���
   long           FRefCount;           // ���ü���
   long           FPerfCount;          // ִ�е����
   TKYCritSect*   FLockPush;           // ѹ����
   TKYCritSect*   FLockPop;            // ������
   TKYEvent*      FNotify;             // ֪ͨ
   TKYCycQueue*   FQueue;              // ��Ϣ����
   TKYCycQueue*   FCache;              // ���ж���

private:
   // ִ�з����ķ�������
   typedef void (TKYRunThread::*TDoMethod)(const TItem& AItem);

   // ִ�з����ķ����б�
   static const TDoMethod  _Type_Methods[28];
   friend class TKYRunThreads;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYRunThreads - �����̼߳��� */

class TKYRunThreads
{
public:
   TKYRunThreads(long AMaxCount = 1, long ACacheSize = 10);
   virtual ~TKYRunThreads();

   // ����
   void*          Data() const         { return FData; }             // default: NULL
   long           Count() const        { return FThreads->Count(); } // default: 0
   long           MaxCount() const     { return FMaxCount; }         // default: AMaxCount
   long           CacheSize() const    { return FCaches->MaxCount();}// default: ACacheSize

   // ��������
   void           SetData(void* AData) { FData = AData; }

   // �����߳�������, ʹ�� NewThread ʱ�����̵߳����ü��������߳�
   void           SetMaxCount(long AMaxCount);

   // �����̻߳���سߴ�
   void           SetCacheSize(long ACacheSize);

   // �����߳�
   // 1. �� AThreadNo Ϊ 0 ��ʾ��̬����, ����ָ���̺߳ŷ����߳�
   // 2. ��ָ���̺߳� AThreadNo, ��ȡֵ��Χ: [1..MaxCount()]
   // 3. NewThread ���߳��ͷű������ DeleteThread ����, �����ʹ�̶߳���ָ��Ϊ
   //              Ұָ�������ϵͳ����
   TKYRunThread*  NewThread(long AThreadNo = 0);

   // ɾ���߳�, �̶߳���ָ������� NewThread ������߳�
   void           DeleteThread(TKYRunThread* AThread);

   // ��������߳�
   void           ClearThreads();

protected:
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

private:
   // ����/�ͷ��߳�
   TKYRunThread*  DoNew(long AThreadNo);
   void           DoDelete(TKYRunThread* AThread);

private:
   void*          FData;
   TKYCritSect*   FLock;
   TKYCycQueue*   FCaches;
   TKYHandles*    FThreads;
   long           FMaxCount;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �������� */

// �رղ��ͷ������߳�
void FreeKYRunThread(TKYRunThread* &AThread, Longword ATimeout = 15000);

}

#endif
