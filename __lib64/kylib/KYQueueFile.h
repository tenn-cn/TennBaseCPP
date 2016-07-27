// =======================================
// Unit   : �����ļ� (KYQueueFile.h)
// Version: 3.0.0.0 (build 2011.03.09)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYQueueFile_H_
#define _KYQueueFile_H_

#include "KYFiles.h"
#include "KYSyncObj.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYQueueFile - �����ļ��� */

class TKYQueueFile
{
public:
   // ����״̬
   enum TState      {qsInactive,       // δ��
                     qsOpening,        // ���ڴ�
                     qsClosing,        // ���ڹر�
                     qsOpened};        // �Ѿ���

   // �汾��Ϣ
   #pragma pack(push, 1)
   typedef struct
   {
      Byte           Major;            // ���汾
      Byte           Minor;            // С�汾
      Byte           Release;          // ����С�汾
      Byte           Build;            // ����С�汾
   } TVersion, *PVersion;
   #pragma pack(pop)

protected:
   #pragma pack(push, 1)

   // ͷ��Ϣ
   typedef struct
   {
      TVersion       Version;          // ���а汾
      Longword       ItemSize;         // ������ߴ�
      Longword       MaxCount;         // �������
      Longword       IndexOrigin;      // ������ʼλ��
      Longword       QueueOrigin;      // ���п�ʼλ��
   } THead, *PHead;

   // ������Ϣ
   typedef struct
   {
      Longword       PushCount;        // ѹ����м���
      Longword       PushIndex;        // ѹ���������
      Longword       PopCount;         // �������м���
      Longword       PopIndex;         // ������������
   } TIndex, *PIndex;

   #pragma pack(pop)

public:
   TKYQueueFile();
   virtual ~TKYQueueFile();

   // ����
   TState            State() const           { return FState; }            // default: qsInactive
   bool              IsCycle() const         { return FIsCycle; }          // default: false
   TVersion          Version() const         { return FHead.Version; }     // default: 0,0,0,0
   Longword          ItemSize() const        { return FHead.ItemSize; }    // default: 0
   Longword          MaxCount() const        { return FHead.MaxCount; }    // default: 0
   KYString          FileName() const        { return FFile->FileName(); } // default: ""

   Longword          PopIndex();       // ȡ Pop ����
   Longword          PushIndex();      // ȡ Push ����
   Longword          IdleCount();      // ȡ���п�������
   Longword          Count();          // ȡ��������

   // �򿪶����ļ�
   // 1. �� ACanCreate == true ���ļ�������ʱ, �򴴽������ļ�
   // 2. AItemSize �� AMaxCount ֻ�д��������ļ�ʱ��Ч
   // 3. AIsCycle ���Ա�ʾ��ǰ�򿪵��ļ������Ƿ����ѭ��д
   bool              Open(const KYString& AFileName,
                                 Longword AItemSize   = 0,
                                 Longword AMaxCount   = 0,
                                     bool ACanCreate  = true,
                                     bool AIsCycle    = true);

   // �رն����ļ�
   void              Close();

   // �������������
   bool              Clear();

   // ѹ�����������
   bool              Push(const void* AData, long ASize);

   // Ԥ��ȡβ������������
   bool              PeekTail(void* AData, long& ASize);

   // Ԥ��ȡ����������
   bool              Peek(void* AData, long& ASize);

   // ��������������
   bool              Pop(void* AData, long& ASize);

   // ����ļ���ϵͳ����(���������)
   void              Flush() const           { FFile->Flush(); }

protected:
   void              Lock() const            { FLock->Enter(); }
   void              Unlock() const          { FLock->Leave(); }

   bool              LockIndexRead() const;
   void              UnlockIndexRead() const;

   bool              LockIndexWrite() const;
   void              UnlockIndexWrite() const;

   void              _ReadIndex(TIndex& AIndex) const;
   void              _WriteIndex(const TIndex& AIndex) const;

   bool              IncRefCount();
   void              DecRefCount();

private:
   bool              CheckHead(const THead& AHead);
   bool              DoCreate(const KYString& AFileName,
                                     Longword AItemSize, Longword AMaxCount);

   void              DoInit();

private:
   TKYCritSect*      FLock;                  // ��
   TFile*            FFile;                  // �����ļ�����ָ��
   THead             FHead;                  // ����ͷ��Ϣ
   TState            FState;                 // ����״̬
   bool              FIsCycle;               // �Ƿ�ѭ��д
   Longword          FRefCount;              // ���ü���

public:
   // ��̬��Ա
   static const Longword Item_MaxSize;       // ���������ߴ�(=0x10000000)
   static const TVersion Ver_Current;        // ��ǰ�汾
};

}

#endif
