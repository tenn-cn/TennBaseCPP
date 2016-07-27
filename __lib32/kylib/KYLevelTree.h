// =======================================
// Unit   : ����� (KYLevelTree.h)
// Version: 3.0.0.0 (build 2012.06.02)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYLevelTree_H_
#define _KYLevelTree_H_

#include "KYMemBlock.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYLevelTree - ������� */

class TKYLevelTree
{
public:
   // �����ĸ���ģʽ
   enum TAttachMode    {knaAdd,              // ��ǰ������ڲ��ĩβ
                        knaAddChild,         // �ӽ���ĩβ
                        knaInsert,           // ��ǰ������ڲ����һ��
                        knaAddFirst,         // ��ǰ������ڲ�ĵ�һ��
                        knaAddChildFirst};   // �ӽ��ĵ�һ��

   // �����
   typedef struct TKYTreeNode
   {
      void*             Item;                // �����ָ��(ָ��ռ�Ϊ ItemSize)
      void*             Data;                // ָ��(������Ҹ�����)
   } *PKYTreeNode;

   // ɾ�����¼�
   typedef void (TObject::*TDoNodeDeletion)(void* Sender, PKYTreeNode ANode);
   typedef struct
   {
      TDoNodeDeletion   Method;
      void*             Object;
   } TOnNodeDeletion;

public:
   TKYLevelTree(bool ACanLock = false, Word AItemSize = 0);
   virtual ~TKYLevelTree();

   // �����ָ�����ĳߴ�
   Word              ItemSize() const     { return (FItems == NULL) ? 0 : FItems->ItemSize(); }

   // ��ǰ������
   long              Count() const        { return FCount; }

   // �Զ�������
   void*             Data() const         { return FData; }
   void              SetData(void* AData) { FData = AData; }

   // ��ǰ������ڵĲ��, ��Ŵ� 0 ��ʼ
   unsigned          Level(const PKYTreeNode ANode) const
                     { return ((const PKYNodeItem)ANode)->Level; }

   // �����
   PKYTreeNode       Root() const         { return (PKYTreeNode)FRoot; }

   // ������ͬ���ĩβ���
   PKYTreeNode       Last() const         { return (PKYTreeNode)FLast; }

   // ��ĩβ���
   PKYTreeNode       Tail() const         { return (PKYTreeNode)FTail; }

   // ����������ȡ��, AIndex ȡֵ��Χ: [0..Count()-1], ��Ҫѭ���������Բ�����ʹ��
   PKYTreeNode       Item(long AIndex) const;
   PKYTreeNode       operator[](long AIndex) const { return Item(AIndex); }

   // ȡ ANode �ĸ����
   PKYTreeNode       GetParent(const PKYTreeNode ANode) const;

   // ȡ ANode ����һ�����
   PKYTreeNode       GetNext(const PKYTreeNode ANode) const;

   // ȡ ANode ��ǰһ�����
   PKYTreeNode       GetPrior(const PKYTreeNode ANode) const;

   // ȡ ANode �ĵ�һ���ӽ��
   PKYTreeNode       GetFirstChild(const PKYTreeNode ANode) const;

   // ȡ ANode ��ĩβ�ӽ��
   PKYTreeNode       GetLastChild(const PKYTreeNode ANode) const;

   // ȡ ANode ����һ���ֵܽ��
   PKYTreeNode       GetNextSibling(const PKYTreeNode ANode) const;

   // ȡ ANode ����һ���ֵܽ��
   PKYTreeNode       GetPriorSibling(const PKYTreeNode ANode) const;

   // ���ݲ��������㵽��ĩβ��㲢���� FTail
   PKYTreeNode       Add(unsigned ALevel);

   // ������ ANode ͬ���ĩβ���
   PKYTreeNode       Add(PKYTreeNode ANode);

   // ���� ANode ���ӽ����ĩβ���
   PKYTreeNode       AddChild(PKYTreeNode ANode);

   // �����½���� ANode ���֮ǰ
   PKYTreeNode       Insert(PKYTreeNode ANode);

   // �жϽ���Ƿ����
   bool              IsExist(const PKYTreeNode ANode) const;

   // �жϽ�� AChild �Ƿ�Ϊ ANode ���ӽ��
   bool              IsChild(const PKYTreeNode ANode, const PKYTreeNode AChild) const;

   // �ж� ANode �Ƿ�����ɾ��
   bool              IsDeleting(const PKYTreeNode ANode) const;

   // �ж� ANode �Ƿ����ӽ��
   bool              HasChildren(const PKYTreeNode ANode) const;

   // ���Ʋ����(ע: ֻ��������֧�ͽ��, ������ Item �� Data), ���� OnDeletion �¼�
   void              Assign(const TKYLevelTree& ASource);

   // ����������н��, ���� OnDeletion �¼�
   void              Clear();

   // ɾ�� ANode ���, ���� OnDeletion �¼�
   void              Delete(PKYTreeNode ANode);

   // �ƶ� ANode ��㵽 ADest ����ָ������
   void              MoveTo(PKYTreeNode ANode, PKYTreeNode ADest, TAttachMode AMode);

   // �� AFrom ��㿽���� ANode ����ָ������(ע: ֻ��������֧�ͽ��, ������ Item �� Data)
   void              CopyFrom(PKYTreeNode ANode, PKYTreeNode AFrom, TAttachMode AMode);

   // �����ڴ��ʽ������ָ��
   void              SetDoInitialize(TKYFormatBlock::TFormatProc AProc)
                     { if (FItems != NULL) FItems->SetDoInitialize(AProc); }

   // �����ڴ��ʽ���¼�
   void              SetOnInitialize(const TKYFmtMemEvent::TOnFormat& AEvent);

   // �¼�
   TOnNodeDeletion   OnNodeDeletion;

protected:
   void              LockRead() const     { if (FLockRW != NULL) FLockRW->LockRead(); }
   void              UnlockRead() const   { if (FLockRW != NULL) FLockRW->UnlockRead(); }

   void              LockWrite() const    { if (FLockRW != NULL) FLockRW->LockWrite(); }
   void              UnlockWrite() const  { if (FLockRW != NULL) FLockRW->UnlockWrite(); }

   bool              DoIsExist(const void* ANode) const;
   bool              DoIsChild(const void* ANode, const void* AChild) const;

   void*             DoGetTail(const void* ANode) const;
   void*             DoGetNext(const void* ANode) const;
   void*             DoGetPrior(const void* ANode) const;

   void*             DoNew(unsigned ALevel);
   void*             DoCopy(void* ANode, long& ACount);
   void              DoCopyChildrenFrom(void* ANode, void* AFrom, long& ACount);

   void*             DoAddLevel0();
   void*             DoAddLevel(unsigned ALevel);

   void              DoChangeLevel(void* ANode, unsigned ALevel);

   void              DoAttachToAsAdd(void* ANode, void* ADest);
   void              DoAttachToAsAddChild(void* ANode, void* ADest);
   void              DoAttachToAsInsert(void* ANode, void* ADest);
   void              DoMoveTo(void* ANode, void* ADest, TAttachMode AMode);

   void              DoClear();
   void              DoRemove(void* ANode);
   void              DoDeleting(void* ANode);
   void              DoDeleted(void* ANode);

private:
   void              LockItems() const    { if (FLockItems != NULL) FLockItems->Enter(); }
   void              UnlockItems() const  { if (FLockItems != NULL) FLockItems->Leave(); }

private:
   TKYCritSect*      FLockItems;          // ����
   TKYLockRW*        FLockRW;             // ��
   TKYMemHandles*    FItems;              // ��Ž��� TKYTreeNode.Item ��ָ������
   TKYFmtMemEvent    FFormatEvent;        // ��ʽ���¼�

   long              FCount;              // ������
   void*             FRoot;               // �����
   void*             FLast;               // ������ͬ���ĩβ���
   void*             FTail;               // ��ĩβ���
   void*             FData;               // �Զ�������

private:
   // �������
   #pragma pack(push, 1)
   typedef struct TKYNodeItem
   {
      TKYTreeNode    Node;
      TKYLevelTree*  Owner;
      unsigned       Level;
      Longword       Handle;
      TKYNodeItem*   Parent;
      TKYNodeItem*   Prior;
      TKYNodeItem*   Next;
      TKYNodeItem*   FirstChild;
      TKYNodeItem*   LastChild;
      bool           Deleting;
   } *PKYNodeItem;
   #pragma pack(pop)

   // ������������, ���������� TKYNodeItem
   static TKYMemBlock*  Inner_ItemManager;

   // TKYLevelTree �ľ�̬��Ա��ʼ����
   static class TInitialization
   {
   public:
      TInitialization();
      ~TInitialization();
   } Inner_Initialization;

   friend class TInitialization;
};

}

#endif
