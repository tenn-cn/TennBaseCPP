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

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYAVLTree - 平衡二叉树类 */

class TKYAVLTree
{
public:
   // 树结点
   typedef struct
   {
      void*       Item;                // 项数据
      void*       Data;                // 自定义数据
   } TNode, *PNode;

   // TOnCopy 结点拷贝事件
   typedef void (TObject::*TDoCopy)(TNode* ADest, const TNode* ASource);
   typedef struct
   {
      TDoCopy     Method;
      void*       Object;
   } TOnCopy;

   // TOnCompare 结点比较事件
   // 若 ANode1 等于 ANode2 则 ACompare == 0
   // 若 ANode1 大于 ANode2 则 ACompare > 0
   // 若 ANode1 小于 ANode2 则 ACompare < 0
   typedef void (TObject::*TDoCompare)(const TNode* ANode1,
                                       const TNode* ANode2, long& ACompare);
   typedef struct
   {
      TDoCompare  Method;
      void*       Object;
   } TOnCompare;

   // TOnDeletion 结点删除事件
   typedef void (TObject::*TDoDeletion)(void* Sender, TNode* ANode);
   typedef struct
   {
      TDoDeletion Method;
      void*       Object;
   } TOnDeletion;

protected:
   // 树结点项
   #pragma pack(push, 1)
   typedef struct TNodeItem
   {
      TNode       Node;                // 结点
      TNodeItem*  Parent;              // 父结点项
      TNodeItem*  Left;                // 左子结点项
      TNodeItem*  Right;               // 右子结点项
      TNodeItem*  Prior;               // 前一结点项
      TNodeItem*  Next;                // 下一结点项
      char        Balance;             // 平衡标志: Height(Left) - Height(Right)
      bool        Deleting;            // 正在删除
   } *PNodeItem;
   #pragma pack(pop)

   // 拷贝项
   typedef struct
   {
      TNodeItem*  Dest;                // 目标结点项
      TNodeItem*  Source;              // 源结点项
   } TCopyItem, *PCopyItem;

public:
   // 构造函数
   // 1. ACanLock 是否内置锁, 为了多线程存取安全, 也可以外部使用锁控制
   // 2. AIsHuge  树结点个数是否超过 64K 个
   TKYAVLTree(bool ACanLock = false, bool AIsHuge = false);
   virtual ~TKYAVLTree();

   // 重载操作符
   TKYAVLTree&    operator=(const TKYAVLTree& ATree)  { Assign(ATree); return *this; }
   TNode*         operator[](long AIndex) const       { return Node(AIndex); }

   // 属性
   void*          Data() const         { return FData; }          // default: NULL
   long           Count() const        { return FCount; }         // default: 0
   long           MaxCount() const     { return FMaxCount; }      // default: 0
   bool           CanDuplicate() const { return FCanDuplicate; }  // default: false

   TNode*         Root() const         { return (TNode*)FRoot; }  // default: NULL

   // 根据索引读取项, AIndex 取值范围: [0..Count()-1], 因要循环查找所以不建议使用
   TNode*         Node(long AIndex) const;

   // 设置属性
   void           SetData(void* AData) { FData = AData; }
   void           SetMaxCount(long AMaxCount);
   void           SetCanDuplicate(bool ACanDuplicate);

   // 复制树结点, 同时也复制 OnCompare 事件方法, 激发 OnCopy 和 OnDeletion 事件
   void           Assign(const TKYAVLTree& ATree);

   // 清除树的所有结点, 激发 OnDeletion 事件
   void           Clear();

   // 添加结点, 同时: {Node->Item == AItem, Node->Data == Data}
   TNode*         Add(void* AItem, void* AData = NULL);

   // 删除值为 AItem 和 Data 的 Compare(...) == 0 第一个结点, 激发 OnDeletion 事件
   bool           Delete(const void* AItem, const void* AData = NULL);

   // 删除指定结点, 激发 OnDeletion 事件
   bool           Remove(TNode* ANode);

   // 搜索值为 AItem 和 Data 的 Compare(...) == 0 第一个结点
   TNode*         Search(const void* AItem, const void* AData = NULL) const;
   bool           Search(Pointer& ARetItem, Pointer& ARetData,
                      const void* AItem, const void* AData = NULL) const;

   // 判断结点是否存在
   bool           Existed(TNode* ANode) const;

   // 判断值为 AItem 和 Data 的 Compare(...) == 0 结点是否存在
   bool           Existed(const void* AItem, const void* AData = NULL) const
                  { return (Search(AItem, AData) != NULL); }

   // 查找最近一个结点
   // 若 ANearest == NULL 则表示项值大于树中的最后一个结点值;
   // 若返回值为 true, 则表示找到项值的结点, 否则项值在 ANearest 结点之前
   bool           FindNearest(const void* AItem, const void* AData,
                                   PNode& ANearest) const;

   // 结点的属性
   long           Level(TNode* ANode) const;    // 结点所在的层号, Level(NULL) = -1
   long           Height(TNode* ANode) const;   // 结点的子树高度, Height(NULL) = 0
   char           Balance(TNode* ANode) const;  // 结点的子树平衡标志

   // 取左右子结点及父结点
   TNode*         Left(TNode* ANode) const;
   TNode*         Right(TNode* ANode) const;
   TNode*         Parent(TNode* ANode) const;

   // 取前后结点(若ANode == NULL, 则 Next 取首结点, Prior 取尾结点)
   TNode*         Prior(TNode* ANode) const;
   TNode*         Next(TNode* ANode) const;

   // 取当前结点的前后结点
   TNode*         First();
   TNode*         Prior();
   TNode*         Next();
   TNode*         Last();

   // 事件
   TOnCopy        OnCopy;                       // 结点拷贝事件, 用于 Assign 方法
   TOnCompare     OnCompare;                    // 结点比较事件
   TOnDeletion    OnDeletion;                   // 结点删除事件

protected:
   // FCurr 锁
   void           LockCurr() const     { if (FLock != NULL) { FLock->Enter(); FLockRW->LockRead(); }}
   void           UnlockCurr() const   { if (FLock != NULL) { FLockRW->UnlockRead(); FLock->Leave(); }}

   // 读锁
   void           LockRead() const     { if (FLockRW != NULL) FLockRW->LockRead(); }
   void           UnlockRead() const   { if (FLockRW != NULL) FLockRW->UnlockRead(); }

   // 写锁
   void           LockWrite() const    { if (FLockRW != NULL) FLockRW->LockWrite(); }
   void           UnlockWrite() const  { if (FLockRW != NULL) FLockRW->UnlockWrite(); }

   // 结点层号/高度/是否存在
   long           DoLevel(TNodeItem* ANode) const;
   long           DoHeight(TNodeItem* ANode) const;
   bool           DoExisted(TNodeItem* ANode) const;

   // 查找(若 ANearest 返回为 NULL, 则表示此结点大于所有结点值, 添加时要加入末尾)
   long           Compare(const TNode* ANode1, const TNode* ANode2) const;
   bool           FindNode(const TNode* ANode, PNodeItem& ANearest) const;

   // 拷贝树, 返回拷贝结点个数
   long           CopyTree(const TKYAVLTree& ATree, PNodeItem& ARoot,
                                  PNodeItem& AHead, PNodeItem& ATail,
                                  PNodeItem& ACurr);
   long           CopyNodes(TNodeItem* ADest, const TNodeItem* ASource);

   // 插入/删除结点
   TNodeItem*     InsertNode(const TNode* ANode, TNodeItem* ATo);
   void           DeleteNode(TNodeItem* ANode);

   // 清除结点的相关方法
   void           ClearNodes(TNodeItem* AHead);
   void           SetAllDeleting(TNodeItem* AHead);

private:
   // OnDeletion.Method != NULL
   inline void    DoDeletion(TNode* ANode);

   // OnCompare.Method != NULL
   inline long    DoCompare(const TNode* ANode1, const TNode* ANode2) const;
   bool           DoFindNode(const TNode* ANode, PNodeItem& ANearest) const;

   // 新增结点并修改链接
   void           DoAdd(TNodeItem* ANode, TNodeItem* ATo);
   void           DoInsert(TNodeItem* ANode, TNodeItem* ATo);
   TNodeItem*     DoNewNode(const TNode* ANode, PNodeItem& ATo,
                                   bool& AIsAdd,     bool& AIsBreak);

   // 减/增平衡值
   void           DecBalance(bool AIsRight, TNodeItem* AParent);
   void           IncBalance(bool AIsRight, TNodeItem* AParent,
                       TNodeItem* ATo,      TNodeItem* ANode);

   // 调整结点并返回调整后的子树根结点
   TNodeItem*     Adjust(bool AIsRight1, bool AIsRight2, TNodeItem* AParent,
                                   TNodeItem* ATo,       TNodeItem* ANode)
                  { return (this->*_DoAdjust[AIsRight1][AIsRight2])
                                             (AParent, ATo, ANode); }

   // 调整结点并返回调整后的子树根结点
   TNodeItem*     AdjustLL(TNodeItem* AParent, TNodeItem* ATo, TNodeItem* ANode);
   TNodeItem*     AdjustLR(TNodeItem* AParent, TNodeItem* ATo, TNodeItem* ANode);
   TNodeItem*     AdjustRL(TNodeItem* AParent, TNodeItem* ATo, TNodeItem* ANode);
   TNodeItem*     AdjustRR(TNodeItem* AParent, TNodeItem* ATo, TNodeItem* ANode);

private:
   void*          FData;               // 自定义数据
   TKYCritSect*   FLock;               // 锁当前结点
   TKYLockRW*     FLockRW;             // 读写锁
   TKYMemBlock*   FNodes;              // 结点集
   TNodeItem*     FRoot;               // 根结点
   TNodeItem*     FHead;               // 首结点
   TNodeItem*     FTail;               // 尾结点
   TNodeItem*     FCurr;               // 当前结点
   long           FCount;              // 结点个数
   long           FMaxCount;           // 结点最大个数, 默认值为 0 表示无限制
   bool           FCanDuplicate;       // 是否允许重复, 默认值为 false

private:
   // 调整结点的方法
   typedef TNodeItem* (TKYAVLTree::*TDoAdjust)(TNodeItem* AParent, TNodeItem* ATo,
                                               TNodeItem* ANode);

   // 调整结点方法列表
   static const TDoAdjust  _DoAdjust[2][2];

   // 左右结点调整的平衡增量
   static const char       _Delta_Balance[2];
};

}

#endif
