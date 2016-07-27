// =======================================
// Unit   : �ڴ�鵥Ԫ (KYMemBlock.h)
// Version: 3.0.0.0 (build 2012.06.02)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYMemBlock_H_
#define _KYMemBlock_H_

#include "KYList.h"
#include "KYAVLTreeC.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYFmtMemEvent - ��ʽ���ڴ����¼��� */

class TKYFmtMemEvent
{
public:
   // TOnFormat �¼�����
   typedef void (TObject::*TDoFormat)(void* AItem, Word ASize);
   typedef struct
   {
      TDoFormat      Method;
      void*          Object;
   } TOnFormat;

public:
   TKYFmtMemEvent()                    { Clear(); }
   ~TKYFmtMemEvent()                   { Clear(); }

   // ���
   void              Clear();

   // ִ�� OnInitialize �¼�
   void              DoInitialize(void* AItem, Word ASize)
                     {
                        if (OnInitialize.Method != NULL)
                           ((TObject*)OnInitialize.Object->*OnInitialize.Method)(AItem, ASize);
                     }

   // ִ�� OnFinalize �¼�
   void              DoFinalize(void* AItem, Word ASize)
                     {
                        if (OnFinalize.Method != NULL)
                           ((TObject*)OnFinalize.Object->*OnFinalize.Method)(AItem, ASize);
                     }

   // �¼�
   TOnFormat         OnInitialize;
   TOnFormat         OnFinalize;

protected:
private:
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYFormatBlock - �ڴ���ʽ���� */

class TKYFormatBlock
{
   // �ڲ�����
   typedef void     (TKYFormatBlock::*TDoInitQueue)();
   typedef Longword (TKYFormatBlock::*TDoNewHandle)();
   typedef void     (TKYFormatBlock::*TDoDeleteHandle)(Longword AHandle);

public:
   // TOnFormatProc �¼�
   typedef void (*TFormatProc)(void* AItem, Word ASize);

public:
   TKYFormatBlock(Word AHeadSize = 0, Word AItemSize = 0,
                  long ACapacity = Default_Capacity);
   virtual ~TKYFormatBlock();

   // ȡ�����Ӧ����ָ��
   void*             operator[](Longword AHandle) const { return Item(AHandle); }
   void*             Item(Longword AHandle) const;

   // ����
   void*             Head() const         { return FHead; }
   long              Count() const        { return FCount; }      // default: 0
   long              Capacity() const     { return FCapacity; }   // default: Default_Capacity
   Word              HeadSize() const     { return FHeadSize; }   // default: 0
   Word              ItemSize() const     { return FItemSize; }   // default: 0

   char*             MinAddress() const   { return FMinAddress; }
   char*             MaxAddress() const   { return FMinAddress + FItemSize * (FCapacity - 1); }

   // �ж���ָ���Ƿ����
   bool              IsExist(const void* AItem) const;

   // �жϾ���Ƿ����
   bool              IsExist(Longword AHandle) const;

   // ���������
   void              Clear();

   // �½���ָ��
   void*             NewItem();

   // ɾ����ָ��
   bool              DeleteItem(void* AItem);

   // �½����
   Longword          NewHandle();

   // �½���������ؾ����Ӧ����ָ��
   Longword          NewHandle(Pointer& AItem);

   // ɾ�����
   bool              DeleteHandle(Longword AHandle);

   // ��ָ��ת���ɾ��
   Longword          Item2Handle(const void* AItem) const;

   // �ڴ��ʽ������ָ��
   TFormatProc       DoInitialize;
   TFormatProc       DoFinalize;

   // ���ø�ʽ���¼�
   TKYFmtMemEvent*   FormatEvent() const  { return FFormatEvent; }
   void              SetFormatEvent(TKYFmtMemEvent* AEvent);

protected:
   void*             _Block() const       { return FMapItems; }
   void              DoDeleteHandle(Longword AHandle);

private:
   void              SetHandleBit(Longword AHandle, bool AValue);

   void              DoInitQueueOfByte();
   Longword          DoNewHandleOfByte();
   void              DoDeleteHandleOfByte(Longword AHandle);

   void              DoInitQueueOfWord();
   Longword          DoNewHandleOfWord();
   void              DoDeleteHandleOfWord(Longword AHandle);

   void              DoInitQueueOfLongword();
   Longword          DoNewHandleOfLongword();
   void              DoDeleteHandleOfLongword(Longword AHandle);

private:
   Word              FHeadSize;
   Word              FItemSize;
   long              FCapacity;
   long              FCount;
   Longword          FBegin;
   Longword          FEnd;

   void*             FHead;
   void*             FMapItems;
   void*             FSpaceQueue;
   char*             FMinAddress;

   // ��ʽ���¼�
   TKYFmtMemEvent*   FFormatEvent;

   // ����
   TDoInitQueue      FDoInitQueue;
   TDoNewHandle      FDoNewHandle;
   TDoDeleteHandle   FDoDeleteHandle;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMemHandles - �ڴ������� */

class TKYMemHandles
{
public:
   TKYMemHandles(bool ACanLock = false,
                 Word AItemSize = sizeof(void*),
                 long ACapacity = Default_Capacity,
                 long ADelta = Delta_Auto);
   virtual ~TKYMemHandles();

   // ȡ�����Ӧ����ָ��
   void*             operator[](Longword AHandle) const { return Item(AHandle); }
   void*             Item(Longword AHandle) const;

   // ����
   long              Capacity() const     { return FCapacity; }         // default: Default_Capacity
   long              Count() const        { return FCount; }            // default: 0
   long              Delta() const        { return FDelta; }            // default: Default_Capacity
   Word              ItemSize() const     { return FMain->ItemSize(); } // default: sizeof(void*)

   // �жϾ���Ƿ����
   bool              IsExist(Longword AHandle) const;

   // �½����
   Longword          New();

   // �½���������ؾ����Ӧ����ָ��
   Longword          New(Pointer& AItem);

   // ɾ�����
   void              Delete(Longword AHandle);

   // ���������
   void              Clear();

   // �ڴ��ʽ������ָ��
   void              SetDoInitialize(TKYFormatBlock::TFormatProc AProc);
   void              SetDoFinalize(TKYFormatBlock::TFormatProc AProc);

   // ���ø�ʽ���¼�
   void              SetFormatEvent(TKYFmtMemEvent* AEvent);

protected:
   void              LockRead() const     { if (FLockRW != NULL) FLockRW->LockRead(); }
   void              UnlockRead() const   { if (FLockRW != NULL) FLockRW->UnlockRead(); }

   void              LockWrite() const    { if (FLockRW != NULL) FLockRW->LockWrite(); }
   void              UnlockWrite() const  { if (FLockRW != NULL) FLockRW->UnlockWrite(); }

private:
   void              DoDeletion(void* AItem);

   TKYFormatBlock*   Handle2Block(Longword AHandle, long& AIndex,
                                  Longword& AChild) const;

private:
   TKYLockRW*        FLockRW;
   TKYFormatBlock*   FMain;
   TKYList*          FList;
   long              FSpace;
   long              FCount;
   long              FDelta;
   long              FCapacity;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMemBlock - �ڴ���ƿ��� */

class TKYMemBlock
{
   // �ڴ�������
   typedef struct
   {
      TKYFormatBlock*   Block;
      char*             MinAddr;
      char*             MaxAddr;
   } TMemIndex, *PMemIndex;

public:
   TKYMemBlock(bool ACanLock = false,
               Word AItemSize = sizeof(void*),
               long ACapacity = Default_Capacity,
               long ADelta = Delta_Auto);
   virtual ~TKYMemBlock();

   // ����
   long              Capacity() const     { return FCapacity; }         // default: Default_Capacity
   long              Count() const        { return FCount; }            // default: 0
   long              Delta() const        { return FDelta; }            // default: Default_Capacity
   Word              ItemSize() const     { return FMain->ItemSize(); } // default: sizeof(void*)

   // �ж���ָ���Ƿ����
   bool              IsExist(const void* AItem) const;

   // �½���ָ��
   void*             New();

   // ɾ����ָ��
   void              Delete(void* AItem);

   // ���������
   void              Clear();

   // �ڴ��ʽ������ָ��
   void              SetDoInitialize(TKYFormatBlock::TFormatProc AProc);
   void              SetDoFinalize(TKYFormatBlock::TFormatProc AProc);

   // ���ø�ʽ���¼�
   void              SetFormatEvent(TKYFmtMemEvent* AEvent);

protected:
   void              Lock() const         { if (FLock != NULL) FLock->Enter(); }
   void              Unlock() const       { if (FLock != NULL) FLock->Leave(); }

private:
   TKYFormatBlock*   Item2Block(const void* AItem, PKYAVLNode& ANode) const;

private:
   TKYCritSect*      FLock;
   TKYFormatBlock*   FMain;
   void*             FList;
   void*             FSpace;
   long              FCount;
   long              FDelta;
   long              FCapacity;

private:
   static long       DoCompare(const TKYAVLNode* ANode1, const TKYAVLNode* ANode2);
   static void       DoDeletion(void* ATree, TKYAVLNode* ANode);
};

}

#endif