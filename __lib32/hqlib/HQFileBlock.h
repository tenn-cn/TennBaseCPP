// =======================================
// Unit   : �ļ��鵥Ԫ
// Version: 3.0.1.0 (build 2011.07.08)
// Author : Kennybee
// Email  : lzk(at)hopechart.com
// Copyright (C) Hopechart Co., Ltd.
// =======================================

#ifndef _HQFileBlock_H_
#define _HQFileBlock_H_

#include "HQUtils.h"

// HQLib �������ռ�
namespace HQLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* THQFileBlock - �ļ����� */

class THQFileBlock
{
public:
   // �ļ�״̬
   enum TState   {bsInactive,          // δ��
                  bsOpening,           // ���ڴ�
                  bsClosing,           // ���ڹر�
                  bsOpened};           // �Ѿ���

protected:
   #pragma pack(push, 1)

   // ͷ��Ϣ
   typedef struct
   {
      TVersion    Version;             // �ļ��汾
      Word        TagSize;             // ���ӳߴ�
      Word        ItemSize;            // ��ߴ�
      Longword    Capacity;            // �洢����
      Longword    MapOrigin;           // ӳ�俪ʼλ��
      Longword    TagOrigin;           // ���ӿ�ʼλ��
      Longword    ItemOrigin;          // �ʼλ��
      Longword    IndexOrigin;         // ������ʼλ��
      Longword    QueueOrigin;         // ���ж��п�ʼλ��
   } THead, *PHead;

   // ������Ϣ
   typedef struct
   {
      Longword    Count;               // �ѷ�������
      Longword    Begin;               // ��ʼ����
      Longword    End;                 // ĩβ����
   } TIndex, *PIndex;

   #pragma pack(pop)

private:
   // �ڲ�����
   typedef void     (THQFileBlock::*TDoInit)();
   typedef Longword (THQFileBlock::*TDoNew)(Longword AIndex);
   typedef void     (THQFileBlock::*TDoDelete)(Longword AHandle, Longword AIndex);

public:
   THQFileBlock(bool ACanLock = false);
   virtual ~THQFileBlock();

   // ����
   void*          Data() const         { return FData; }             // default: NULL
   TState         State() const        { return FState; }            // default: bsInactive
   TVersion       Version() const      { return FHead.Version; }     // default: 0,0,0,0
   Word           TagSize() const      { return FHead.TagSize; }     // default: 0
   Word           ItemSize() const     { return FHead.ItemSize; }    // default: 0
   Longword       Capacity() const     { return FHead.Capacity; }    // default: 0
   Longword       RefCount() const     { return FRefCount; }         // default: 0
   KYString       FileName() const     { return FFile->FileName(); } // default: ""

   Longword       Count();             // ȡ�ѷ�������
   Longword       IdleCount();         // ȡ��������

   // ��������
   void           SetData(void* AData) { FData = AData; }
   void           SetMutex(TKYMutex* AMutex);

   // �������ü���
   bool           IncRefCount();
   void           DecRefCount();

   // ��/�ر��ļ�
   // 1. �� ACanCreate == true ���ļ�������ʱ, �򴴽��ļ�
   // 2. ACapacity, AItemSize �� ATagSize ֻ�д����ļ�ʱ��Ч
   bool           Open(const KYString& AFileName,     Longword ACapacity = 0,
                                 Word  AItemSize = 0,     Word ATagSize  = 0,
                                 bool  ACanCreate = true);
   void           Close();

   // Ԥ�����ļ�
   // ע: Ϊ�˰��ļ�����ȷ�����ص��ڴ�ӳ����, �ӿ���ζ�ȡ�ļ������ٶ�, һ������
   // �ļ��򿪺���á�ASize <= 0 ��ʾ�����ļ�����, Ҳ����Ԥ�����ļ���ͷ��ʼ��ָ
   // �� ASize ���֡�
   bool           Preload(long ASize = 0);

   // ����/ɾ��/������о��
   // New:        ������ֵΪ 0 ��ʾ������ʧ��
   // Delete:     ������ֵΪ false ��ʾ��������ڻ�״̬���Ϸ�
   Longword       New();
   bool           Delete(Longword AHandle);
   void           Clear();

   // �жϾ���Ƿ����
   bool           Existed(Longword AHandle);

   // ��ȡ/����������
   // Read:       ASize ����Ҫ��ȡ�ĳߴ�, ����ʵ�ʶ�ȡ�ĳߴ�
   // Write:      ASize ����Ҫ����ĳߴ�, ����ʵ�ʱ���ĳߴ�
   bool           Read(Longword AHandle, long AOffset, void* AData, long& ASize);
   bool           Write(Longword AHandle, long AOffset, const void* AData, long& ASize);

   // ��ȡ/���渽������
   // ReadTag:    ASize ����Ҫ��ȡ�ĳߴ�, ����ʵ�ʶ�ȡ�ĳߴ�
   // WriteTag:   ASize ����Ҫ����ĳߴ�, ����ʵ�ʱ���ĳߴ�
   bool           ReadTag(long AOffset, void* AData, long& ASize);
   bool           WriteTag(long AOffset, const void* AData, long& ASize);

protected:
   void           Lock() const         { if (FLock != NULL) FLock->Enter(); }
   void           Unlock() const       { if (FLock != NULL) FLock->Leave(); }

   bool           LockFile() const     { return (FMutex == NULL) || FMutex->Lock(); }
   void           UnlockFile() const   { if (FMutex != NULL) FMutex->Unlock(); }

   // ƫ����ת�ڴ��ַ(AOffset > 0)
   void*          Offset2Mem(long AOffset) const
                  { return (Byte*)FFile->MapMem() + AOffset; }

private:
   void           DoInit();
   void           DoInitQueueFun(Longword ACapacity);
   bool           DoCheck(const THead& AHead);
   bool           DoExisted(Longword AHandle);
   void           DoSetBit(Longword AHandle, bool AValue);

   bool           DoOpen(const KYString& AFileName);
   bool           DoCreate(const KYString& AFileName, Longword ACapacity,
                                     Word  AItemSize,     Word ATagSize);
   void           DoClose();
   
   void           DoInitHead(Longword ACapacity, Word AItemSize, Word ATagSize);
   void           DoUpdate();

private:
   void           DoInitQueue();
   void           DoInitOfByte();
   void           DoInitOfWord();
   void           DoInitOfLongword();

   Longword       DoNewHandle();
   Longword       DoNewOfByte(Longword AIndex);
   Longword       DoNewOfWord(Longword AIndex);
   Longword       DoNewOfLongword(Longword AIndex);

   void           DoDeleteHandle(Longword AHandle);
   void           DoDeleteOfByte(Longword AHandle, Longword AIndex);
   void           DoDeleteOfWord(Longword AHandle, Longword AIndex);
   void           DoDeleteOfLongword(Longword AHandle, Longword AIndex);

private:
   void*          FData;               // �Զ�������
   TKYMapFile*    FFile;               // �ļ�����
   TKYCritSect*   FLock;               // ״̬��
   TKYMutex*      FMutex;              // ����Ԫ

   THead          FHead;               // ͷ��Ϣ
   TState         FState;              // ״̬
   TIndex*        FIndex;              // ������Ϣ
   Byte*          FMapBits;            // ��ӳ��λֵ
   Longword       FRefCount;           // ���ü���

   TDoInit        FDoInit;             // ��ʼ�����ж���
   TDoNew         FDoNew;              // ������
   TDoDelete      FDoDelete;           // �ͷž��

public:
   // ��̬��Ա
   static const TVersion   _Ver_Current;// ��ǰ�汾(1.0.0.0)
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* THQFileHandles - �ļ�������� */

class THQFileHandles
{
public:
   // �ļ������״̬
   enum TState   {hsInactive,          // δ��
                  hsOpening,           // ���ڴ�
                  hsClosing,           // ���ڹر�
                  hsOpened};           // �Ѿ���

protected:
   #pragma pack(push, 1)

   // ͷ��Ϣ
   typedef struct
   {
      TVersion    Version;             // �ļ��汾
      Word        TagSize;             // ���ӳߴ�
      Word        MapSize;             // ӳ��ߴ�
      Word        ItemSize;            // ��ߴ�
      Word        MaxFiles;            // ����ļ�����
      Longword    BlockSize;           // �ļ�������
      Longword    ExtOrigin;           // ��չ��ʼλ��
      Longword    TagOrigin;           // ���ӿ�ʼλ��
      Longword    MapOrigin;           // ӳ�俪ʼλ��
      Longword    IndexOrigin;         // ������ʼλ��
      Longword    QueueOrigin;         // ���ж��п�ʼλ��
   } THead, *PHead;

   // ��չ��Ϣ
   typedef struct
   {
      Longword    Count;               // �ѷ�������
      Longword    Capacity;            // �ܴ洢����
      Word        FileCount;           // �ļ�����
   } TExtInfo, *PExtInfo;

   // ӳ����
   typedef struct
   {
      Longword    Count;               // �ѷ�������
      Longword    IdleCount;           // ��������
   } TMapItem, *PMapItem;

   // ������Ϣ
   typedef struct
   {
      Longword    Count;               // �ѷ�������
      Longword    Begin;               // ��ʼ����
      Longword    End;                 // ĩβ����
   } TIndex, *PIndex;

   #pragma pack(pop)

public:
   THQFileHandles();
   virtual ~THQFileHandles();

   // ����
   void*          Data() const         { return FData; }             // default: NULL
   TState         State() const        { return FState; }            // default: hsInactive
   TVersion       Version() const      { return FHead.Version; }     // default: 0,0,0,0
   Word           TagSize() const      { return FHead.TagSize; }     // default: 0
   Word           ItemSize() const     { return FHead.ItemSize; }    // default: 0
   Word           MaxFiles() const     { return FHead.MaxFiles; }    // default: 0
   Longword       BlockSize() const    { return FHead.BlockSize; }   // default: 0
   Longword       RefCount() const     { return FRefCount; }         // default: 0

   KYString       Path() const         { return FPath; }             // default: CurrAppFilePath
   KYString       Prefix() const       { return FPrefix; }           // default: ""
   KYString       Suffix() const       { return FSuffix; }           // default: ".hfb"
   KYString       FileName() const     { return FFile->FileName(); } // default: ""

   Longword       Capacity();          // ȡ�ܴ洢����
   Longword       Count();             // ȡ�ѷ�������

   // ��������
   void           SetData(void* AData) { FData = AData; }

   void           SetTagSize(Word ASize);
   void           SetItemSize(Word ASize);
   void           SetMaxFiles(Word ACount);
   void           SetBlockSize(Longword ASize);

   void           SetPath(const KYString& APath);
   void           SetPrefix(const KYString& AName);
   void           SetSuffix(const KYString& AName);

   // �������ü���
   bool           IncRefCount();
   void           DecRefCount();

   // ��/�ر��ļ������
   bool           Open();
   void           Close();

   // Ԥ�����ļ�
   // ע: Ϊ�˰��ļ�����ȷ�����ص��ڴ�ӳ����, �ӿ���ζ�ȡ�ļ������ٶ�, һ������
   // �ļ��򿪺���á�ASize <= 0 ��ʾ�����ļ�����, Ҳ����Ԥ�����ļ���ͷ��ʼ��ָ
   // �� ASize ���֡�
   bool           Preload(long ASize = 0);

   // ����/ɾ��/������о��
   // New:        ������ֵΪ 0 ��ʾ������ʧ��
   // Delete:     ������ֵΪ false ��ʾ��������ڻ�״̬���Ϸ�
   Longword       New();
   bool           Delete(Longword AHandle);
   void           Clear();

   // �жϾ���Ƿ����
   bool           Existed(Longword AHandle);

   // ��ȡ/����������
   // Read:       ASize ����Ҫ��ȡ�ĳߴ�, ����ʵ�ʶ�ȡ�ĳߴ�
   // Write:      ASize ����Ҫ����ĳߴ�, ����ʵ�ʱ���ĳߴ�
   bool           Read(Longword AHandle, long AOffset, void* AData, long& ASize);
   bool           Write(Longword AHandle, long AOffset, const void* AData, long& ASize);

   // ��ȡ/���渽������
   // ReadTag:    ASize ����Ҫ��ȡ�ĳߴ�, ����ʵ�ʶ�ȡ�ĳߴ�
   // WriteTag:   ASize ����Ҫ����ĳߴ�, ����ʵ�ʱ���ĳߴ�
   bool           ReadTag(long AOffset, void* AData, long& ASize);
   bool           WriteTag(long AOffset, const void* AData, long& ASize);

protected:
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

   bool           LockFile() const     { return FMutex->Lock(); }
   void           UnlockFile() const   { FMutex->Unlock(); }

   // ƫ����ת�ڴ��ַ(AOffset > 0)
   void*          Offset2Mem(long AOffset) const
                  { return (Byte*)FFile->MapMem() + AOffset; }

private:
   void           DoInit();
   bool           DoCheck(const THead& AHead);
   Word           DoCheckHandle(Longword AHandle);

   bool           DoOpen();
   bool           DoCreate();
   void           DoClose();

   void           DoDeleteBlock(long AHandle, THQFileBlock* AFileBlock);

   void           DoInitHead();
   void           DoInitHandles();
   void           DoUpdate();
   Word           DoNewBlockFile();
   void           DoOpenBlockFile(Word AFileNo);
   void           DoOpenBlockFiles();

   void           DoPushBlock(Word AFileIndex);
   Word           DoPeekBlock();
                  
   Byte*          DoMapItem(Word AFileNo) const 
                  { return FMapItems + (AFileNo - 1) * FHead.MapSize;}
   THQFileBlock*  DoHandleToBlock(Longword AHandle) const
                  { return (THQFileBlock*)FBlocks->Item(DoHandleToFileNo(AHandle)); }
   Word           DoHandleToFileNo(Longword AHandle) const
                  { return (Word)((AHandle - 1) / FHead.BlockSize + 1); }
   Longword       DoHandleToBlockHandle(Longword AHandle) const
                  { return ((AHandle - 1) % FHead.BlockSize) + 1; }

private:
   void*          FData;               // �Զ�������
   TKYMapFile*    FFile;               // �ļ�����
   TKYCritSect*   FLock;               // ״̬��
   TKYMutex*      FMutex;              // ����Ԫ
   TKYHandles*    FBlocks;             // �ļ��鼯

   THead          FHead;               // ͷ��Ϣ
   TState         FState;              // �����״̬
   TExtInfo*      FExtInfo;            // ��չ��Ϣ
   TIndex*        FIndex;              // ������Ϣ
   Word*          FQueue;              // ���ж���
   Byte*          FMapItems;           // ӳ�����б�
   Longword       FRefCount;           // ���ü���

   KYString       FPath;               // �ļ����������·��, ĩβ��·���ָ���
   KYString       FPrefix;             // �ļ���ǰ׺
   KYString       FSuffix;             // �ļ�����׺, Ĭ��ֵΪ ".hfb"

public:
   // ��̬��Ա
   static const TVersion _Ver_Current; // ��ǰ�汾(1.0.0.0)
};

}

#endif
