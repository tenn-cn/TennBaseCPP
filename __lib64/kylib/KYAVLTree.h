// =======================================
// Unit   : AVL tree (KYAVLTree.h)
// Version: 3.0.1.0 (build 2014.12.13)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYAVLTree_H_
#define _KYAVLTree_H_

#include "KYMemBlock.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYAVLTree - ƽ��������� */

class TKYAVLTree
{
public:
   // �����
   typedef struct
   {
      void*       Item;                // ������
      void*       Data;                // �Զ�������
   } TNode, *PNode;

   // TOnCopy ��㿽���¼�
   typedef void (TObject::*TDoCopy)(TNode* ADest, const TNode* ASource);
   typedef struct
   {
      TDoCopy     Method;
      void*       Object;
   } TOnCopy;

   // TOnCompare ���Ƚ��¼�
   // �� ANode1 ���� ANode2 �� ACompare == 0
   // �� ANode1 ���� ANode2 �� ACompare > 0
   // �� ANode1 С�� ANode2 �� ACompare < 0
   typedef void (TObject::*TDoCompare)(const TNode* ANode1,
                                       const TNode* ANode2, long& ACompare);
   typedef struct
   {
      TDoCompare  Method;
      void*       Object;
   } TOnCompare;

   // TOnDeletion ���ɾ���¼�
   typedef void (TObject::*TDoDeletion)(void* Sender, TNode* ANode);
   typedef struct
   {
      TDoDeletion Method;
      void*       Object;
   } TOnDeletion;

protected:
   // �������
   #pragma pack(push, 1)
   typedef struct TNodeItem
   {
      TNode       Node;                // ���
      TNodeItem*  Parent;              // �������
      TNodeItem*  Left;                // ���ӽ����
      TNodeItem*  Right;               // ���ӽ����
      TNodeItem*  Prior;               // ǰһ�����
      TNodeItem*  Next;                // ��һ�����
      char        Balance;             // ƽ���־: Height(Left) - Height(Right)
      bool        Deleting;            // ����ɾ��
   } *PNodeItem;
   #pragma pack(pop)

   // ������
   typedef struct
   {
      TNodeItem*  Dest;                // Ŀ������
      TNodeItem*  Source;              // Դ�����
   } TCopyItem, *PCopyItem;

public:
   // ���캯��
   // 1. ACanLock �Ƿ�������, Ϊ�˶��̴߳�ȡ��ȫ, Ҳ�����ⲿʹ��������
   // 2. AIsHuge  ���������Ƿ񳬹� 64K ��
   TKYAVLTree(bool ACanLock = false, bool AIsHuge = false);
   virtual ~TKYAVLTree();

   // ���ز�����
   TKYAVLTree&    operator=(const TKYAVLTree& ATree)  { Assign(ATree); return *this; }
   TNode*         operator[](long AIndex) const       { return Node(AIndex); }

   // ����
   void*          Data() const         { return FData; }          // default: NULL
   long           Count() const        { return FCount; }         // default: 0
   long           MaxCount() const     { return FMaxCount; }      // default: 0
   bool           CanDuplicate() const { return FCanDuplicate; }  // default: false

   TNode*         Root() const         { return (TNode*)FRoot; }  // default: NULL

   // ����������ȡ��, AIndex ȡֵ��Χ: [0..Count()-1], ��Ҫѭ���������Բ�����ʹ��
   TNode*         Node(long AIndex) const;

   // ��������
   void           SetData(void* AData) { FData = AData; }
   void           SetMaxCount(long AMaxCount);
   void           SetCanDuplicate(bool ACanDuplicate);

   // ���������, ͬʱҲ���� OnCompare �¼�����, ���� OnCopy �� OnDeletion �¼�
   void           Assign(const TKYAVLTree& ATree);

   // ����������н��, ���� OnDeletion �¼�
   void           Clear();

   // ��ӽ��, ͬʱ: {Node->Item == AItem, Node->Data == Data}
   TNode*         Add(void* AItem, void* AData = NULL);

   // ɾ��ֵΪ AItem �� Data �� Compare(...) == 0 ��һ�����, ���� OnDeletion �¼�
   bool           Delete(const void* AItem, const void* AData = NULL);

   // ɾ��ָ�����, ���� OnDeletion �¼�
   bool           Remove(TNode* ANode);

   // ����ֵΪ AItem �� Data �� Compare(...) == 0 ��һ�����
   TNode*         Search(const void* AItem, const void* AData = NULL) const;
   bool           Search(Pointer& ARetItem, Pointer& ARetData,
                      const void* AItem, const void* AData = NULL) const;

   // �жϽ���Ƿ����
   bool           Existed(TNode* ANode) const;

   // �ж�ֵΪ AItem �� Data �� Compare(...) == 0 ����Ƿ����
   bool           Existed(const void* AItem, const void* AData = NULL) const
                  { return (Search(AItem, AData) != NULL); }

   // �������һ�����
   // �� ANearest == NULL ���ʾ��ֵ�������е����һ�����ֵ;
   // ������ֵΪ true, ���ʾ�ҵ���ֵ�Ľ��, ������ֵ�� ANearest ���֮ǰ
   bool           FindNearest(const void* AItem, const void* AData,
                                   PNode& ANearest) const;

   // ��������
   long           Level(TNode* ANode) const;    // ������ڵĲ��, Level(NULL) = -1
   long           Height(TNode* ANode) const;   // ���������߶�, Height(NULL) = 0
   char           Balance(TNode* ANode) const;  // ��������ƽ���־

   // ȡ�����ӽ�㼰�����
   TNode*         Left(TNode* ANode) const;
   TNode*         Right(TNode* ANode) const;
   TNode*         Parent(TNode* ANode) const;

   // ȡǰ����(��ANode == NULL, �� Next ȡ�׽��, Prior ȡβ���)
   TNode*         Prior(TNode* ANode) const;
   TNode*         Next(TNode* ANode) const;

   // ȡ��ǰ����ǰ����
   TNode*         First();
   TNode*         Prior();
   TNode*         Next();
   TNode*         Last();

   // �¼�
   TOnCopy        OnCopy;                       // ��㿽���¼�, ���� Assign ����
   TOnCompare     OnCompare;                    // ���Ƚ��¼�
   TOnDeletion    OnDeletion;                   // ���ɾ���¼�

protected:
   // FCurr ��
   void           LockCurr() const     { if (FLock != NULL) { FLock->Enter(); FLockRW->LockRead(); }}
   void           UnlockCurr() const   { if (FLock != NULL) { FLockRW->UnlockRead(); FLock->Leave(); }}

   // ����
   void           LockRead() const     { if (FLockRW != NULL) FLockRW->LockRead(); }
   void           UnlockRead() const   { if (FLockRW != NULL) FLockRW->UnlockRead(); }

   // д��
   void           LockWrite() const    { if (FLockRW != NULL) FLockRW->LockWrite(); }
   void           UnlockWrite() const  { if (FLockRW != NULL) FLockRW->UnlockWrite(); }

   // �����/�߶�/�Ƿ����
   long           DoLevel(TNodeItem* ANode) const;
   long           DoHeight(TNodeItem* ANode) const;
   bool           DoExisted(TNodeItem* ANode) const;

   // ����(�� ANearest ����Ϊ NULL, ���ʾ�˽��������н��ֵ, ���ʱҪ����ĩβ)
   long           Compare(const TNode* ANode1, const TNode* ANode2) const;
   bool           FindNode(const TNode* ANode, PNodeItem& ANearest) const;

   // ������, ���ؿ���������
   long           CopyTree(const TKYAVLTree& ATree, PNodeItem& ARoot,
                                  PNodeItem& AHead, PNodeItem& ATail,
                                  PNodeItem& ACurr);
   long           CopyNodes(TNodeItem* ADest, const TNodeItem* ASource);

   // ����/ɾ�����
   TNodeItem*     InsertNode(const TNode* ANode, TNodeItem* ATo);
   void           DeleteNode(TNodeItem* ANode);

   // ���������ط���
   void           ClearNodes(TNodeItem* AHead);
   void           SetAllDeleting(TNodeItem* AHead);

private:
   // OnDeletion.Method != NULL
   inline void    DoDeletion(TNode* ANode);

   // OnCompare.Method != NULL
   inline long    DoCompare(const TNode* ANode1, const TNode* ANode2) const;
   bool           DoFindNode(const TNode* ANode, PNodeItem& ANearest) const;

   // ������㲢�޸�����
   void           DoAdd(TNodeItem* ANode, TNodeItem* ATo);
   void           DoInsert(TNodeItem* ANode, TNodeItem* ATo);
   TNodeItem*     DoNewNode(const TNode* ANode, PNodeItem& ATo,
                                   bool& AIsAdd,     bool& AIsBreak);

   // ��/��ƽ��ֵ
   void           DecBalance(bool AIsRight, TNodeItem* AParent);
   void           IncBalance(bool AIsRight, TNodeItem* AParent,
                       TNodeItem* ATo,      TNodeItem* ANode);

   // ������㲢���ص���������������
   TNodeItem*     Adjust(bool AIsRight1, bool AIsRight2, TNodeItem* AParent,
                                   TNodeItem* ATo,       TNodeItem* ANode)
                  { return (this->*_DoAdjust[AIsRight1][AIsRight2])
                                             (AParent, ATo, ANode); }

   // ������㲢���ص���������������
   TNodeItem*     AdjustLL(TNodeItem* AParent, TNodeItem* ATo, TNodeItem* ANode);
   TNodeItem*     AdjustLR(TNodeItem* AParent, TNodeItem* ATo, TNodeItem* ANode);
   TNodeItem*     AdjustRL(TNodeItem* AParent, TNodeItem* ATo, TNodeItem* ANode);
   TNodeItem*     AdjustRR(TNodeItem* AParent, TNodeItem* ATo, TNodeItem* ANode);

private:
   void*          FData;               // �Զ�������
   TKYCritSect*   FLock;               // ����ǰ���
   TKYLockRW*     FLockRW;             // ��д��
   TKYMemBlock*   FNodes;              // ��㼯
   TNodeItem*     FRoot;               // �����
   TNodeItem*     FHead;               // �׽��
   TNodeItem*     FTail;               // β���
   TNodeItem*     FCurr;               // ��ǰ���
   long           FCount;              // ������
   long           FMaxCount;           // ���������, Ĭ��ֵΪ 0 ��ʾ������
   bool           FCanDuplicate;       // �Ƿ������ظ�, Ĭ��ֵΪ false

private:
   // �������ķ���
   typedef TNodeItem* (TKYAVLTree::*TDoAdjust)(TNodeItem* AParent, TNodeItem* ATo,
                                               TNodeItem* ANode);

   // ������㷽���б�
   static const TDoAdjust  _DoAdjust[2][2];

   // ���ҽ�������ƽ������
   static const char       _Delta_Balance[2];
};

}

#endif
