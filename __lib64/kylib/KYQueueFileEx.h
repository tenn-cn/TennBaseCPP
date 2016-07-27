// =======================================
// Unit   : ӳ���ļ����� (KYQueueFileEx.h)
// Version: 3.0.0.0 (build 2011.03.26)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYQueueFileEx_H_
#define _KYQueueFileEx_H_

#include "KYMapFile.h"
#include "KYSyncObj.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYQueueFileEx - ӳ���ļ������� */

class TKYQueueFileEx
{
public:
   // ����״̬
   enum TState   {qsInactive,          // δ��
                  qsOpening,           // ���ڴ�
                  qsClosing,           // ���ڹر�
                  qsOpened};           // �Ѿ���

   // �汾��Ϣ
   #pragma pack(push, 1)
   typedef struct
   {
      Byte        Major;               // ���汾
      Byte        Minor;               // С�汾
      Byte        Release;             // ����С�汾
      Byte        Build;               // ����С�汾
   } TVersion, *PVersion;
   #pragma pack(pop)

protected:
   #pragma pack(push, 1)

   // ͷ��Ϣ
   typedef struct
   {
      TVersion    Version;             // ���а汾
      Longword    TagSize;             // ���ӳߴ�
      Longword    ItemSize;            // ������ߴ�
      Longword    MaxCount;            // �������
      Longword    TagOrigin;           // ���ӿ�ʼλ��
      Longword    IndexOrigin;         // ������ʼλ��
      Longword    QueueOrigin;         // ���п�ʼλ��
   } THead, *PHead;

   // ������Ϣ
   typedef struct
   {
      Longword    PushCount;           // ѹ����м���
      Longword    PushIndex;           // ѹ���������
      Longword    PopCount;            // �������м���
      Longword    PopIndex;            // ������������
   } TIndex, *PIndex;

   #pragma pack(pop)

public:
   TKYQueueFileEx();
   virtual ~TKYQueueFileEx();

   // ����
   void*          Data() const         { return FData; }             // default: NULL
   TState         State() const        { return (TState)FState; }    // default: qsInactive
   bool           IsCycle() const      { return FIsCycle; }          // default: false
   TVersion       Version() const      { return FHead.Version; }     // default: 0,0,0,0
   Longword       TagSize() const      { return FHead.TagSize; }     // default: 0
   Longword       ItemSize() const     { return FHead.ItemSize; }    // default: 0
   Longword       MaxCount() const     { return FHead.MaxCount; }    // default: 0
   KYString       FileName() const     { return FFile->FileName(); } // default: ""

   Longword       Count();             // ȡ��������
   Longword       IdleCount();         // ȡ���п�������
   Longword       PushIndex();         // ȡ Push ����
   Longword       PopIndex();          // ȡ Pop ����

   // ��������
   void           SetData(void* AData) { FData = AData; }

   // �򿪶����ļ�
   // 1. �� ACanCreate == true ���ļ�������ʱ, �򴴽������ļ�
   // 2. ATagSize, AItemSize �� AMaxCount ֻ�д��������ļ�ʱ��Ч
   // 3. AIsCycle ���Ա�ʾ��ǰ�򿪵��ļ������Ƿ����ѭ��д, �����������Ƿ���
   bool           Open(const KYString& AFileName,     Longword ATagSize  = 0,
                             Longword  AItemSize = 0, Longword AMaxCount = 0,
                                 bool  ACanCreate= true,  bool AIsCycle  = false);

   // �رն����ļ�
   void           Close();

   // �������������
   bool           Clear();

   // ѹ�����������: ���ݳߴ� = Min(ASize, ItemSize)
   bool           Push(const void* AData, long ASize);

   // Ԥ��ȡβ������������
   bool           PeekTail(void* AData, long& ASize);

   // Ԥ��ȡ����������
   bool           Peek(void* AData, long& ASize);

   // ��������������
   bool           Pop(void* AData, long& ASize);

   // ��ȡ��������: ASize ����Ҫ��ȡ�ĳߴ�, ����ʵ�ʶ�ȡ�ĳߴ�
   bool           ReadTag(long AOffset, void* AData, long& ASize);

   // ���渽������: ASize ����Ҫ����ĳߴ�, ����ʵ�ʱ���ĳߴ�
   bool           WriteTag(long AOffset, const void* AData, long& ASize);

protected:
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

   bool           LockFile() const     { return FMutex->Lock(); }
   void           UnlockFile() const   { FMutex->Unlock(); }

   bool           IncRefCount();
   void           DecRefCount();

private:
   void           DoInit();
   bool           DoCheck(const THead& AHead);

   bool           DoOpen(const KYString& AFileName);
   bool           DoCreate(const KYString& AFileName, Longword ATagSize,
                                 Longword  AItemSize, Longword AMaxCount);

private:
   void*          FData;               // �Զ�������
   TKYMapFile*    FFile;               // �ļ�����
   TKYCritSect*   FLock;               // ��
   TKYMutex*      FMutex;              // ����Ԫ
   TIndex*        FIndex;              // ��������
   void*          FQueue;              // ������ʼ��
   void*          FTag;                // ������ʼ����

   THead          FHead;               // ����ͷ��Ϣ
   Byte           FState;              // ����״̬
   bool           FIsCycle;            // �Ƿ�ѭ��д
   Longword       FRefCount;           // ���ü���

public:
   // ��̬��Ա
   static const Longword Item_MaxSize; // ���������ߴ�(=0x10000000)
   static const TVersion Ver_Current;  // ��ǰ�汾(3.0.0.0)
};

}

#endif
