// =======================================
// Unit   : extended AVL tree (KYAVLTreeEx.h)
// Version: 3.0.1.0 (build 2014.12.13)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYAVLTreeEx_H_
#define _KYAVLTreeEx_H_

#include "KYMemBlock.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYAVLTreeEx - 扩展平衡二叉树类 */

class TKYAVLTreeEx
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
      long        Delta;               // 左右差值: Count(Left) - Count(Right)
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
   TKYAVLTreeEx(bool ACanLock = false, bool AIsHuge = false);
   virtual ~TKYAVLTreeEx();

   // 重载操作符
   TKYAVLTreeEx&  operator=(const TKYAVLTreeEx& ATree){ Assign(ATree); return *this; }
   TNode*         operator[](long AIndex) const       { return Node(AIndex); }

   // 属性
   void*          Data() const         { return FData; }          // default: NULL
   long           Count() const        { return FCount; }         // default: 0
   bool           Sorted() const       { return FSorted; }        // default: true
   long           MaxCount() const     { return FMaxCount; }      // default: 0
   bool           HasSorted() const    { return FSorted || FHasSorted; }
   bool           CanDuplicate() const { return FCanDuplicate; }  // default: false

   TNode*         Root() const         { return (TNode*)FRoot; }  // default: NULL

   // 根据索引读取项, AIndex 取值范围: [0..Count()-1], 可以快速定位指定索引结点
   TNode*         Node(long AIndex) const;

   // 设置属性
   void           SetData(void* AData) { FData = AData; }
   void           SetSorted(bool ASorted);
   void           SetMaxCount(long AMaxCount);
   void           SetCanDuplicate(bool ACanDuplicate);

   // 复制树结点, 同时也复制 OnCompare 事件方法, 激发 OnCopy 和 OnDeletion 事件
   void           Assign(const TKYAVLTreeEx& ATree);

   // 清除树的所有结点, 激发 OnDeletion 事件
   void           Clear();

   // 根据 OnCompare 从小到大排序
   // 若要反向排序, 则只要在 OnCompare 的方法返回值 ACompare = -ACompare 即可
   void           Sort();

   // 添加结点, 同时: {Node->Item == AItem, Node->Data == Data}
   // 若 Sorted 属性为 false 则添加到末尾项, 否则根据值大小插入到指定位置
   TNode*         Add(void* AItem, void* AData = NULL);

   // 插入指定索引的结点, 同时: {Node->Item == AItem, Node->Data == Data}
   // 若 Sorted 属性为 true 则不允许调用 Insert 方法, 必须调用 Add 方法添加结点
   // 若 AIndex <= 0 则插入第一项, 若 AIndex >= Count() 则插入末尾项
   TNode*         Insert(long AIndex, void* AItem, void* AData = NULL);

   // 插入指定结点之前的结点, 同时: {Node->Item == AItem, Node->Data == Data}
   // 若 Sorted 属性为 true 则不允许调用 Insert 方法, 必须调用 Add 方法添加结点
   // 若 ANode == NULL 则插入末尾项
   TNode*         Insert(TNode* ANode, void* AItem, void* AData = NULL);

   // 删除值为 AItem 和 Data 的 Compare(...) == 0 第一个结点, 激发 OnDeletion 事件
   bool           Delete(const void* AItem, const void* AData = NULL);

   // 删除指定索引的结点, 激发 OnDeletion 事件
   bool           Delete(long AIndex);

   // 删除指定结点, 激发 OnDeletion 事件
   bool           Remove(TNode* ANode);

   // 搜索值为 AItem 和 Data 的 Compare(...) == 0 第一个结点
   TNode*         Search(const void* AItem, const void* AData = NULL) const;
   bool           Search(Pointer& ARetItem, Pointer& ARetData,
                      const void* AItem, const void* AData = NULL) const;

   // 取结点所在的索引
   long           IndexOf(TNode* ANode) const;

   // 搜索值为 AItem 和 Data 的 Compare(...) == 0 第一个结点的索引
   long           IndexOf(const void* AItem, const void* AData = NULL) const;

   // 判断结点是否存在
   bool           Existed(TNode* ANode) const;

   // 判断值为 AItem 和 Data 的 Compare(...) == 0 结点是否存在
   bool           Existed(const void* AItem, const void* AData = NULL) const
                  { return (Search(AItem, AData) != NULL); }

   // 查找最近一个结点
   // 若未排序则ANearest就是查找结果, 也就是说, 若未找到则返回 NULL;
   // 若 ANearest == NULL 则表示项值大于树中的最后一个结点值;
   // 若返回值为 true, 则表示找到项值的结点, 否则项值在 ANearest 结点之前
   bool           FindNearest(const void* AItem, const void* AData,
                                   PNode& ANearest) const;

   // 结点的属性
   long           Level(TNode* ANode) const;    // 结点所在的层号, Level(NULL) = -1
   long           Count(TNode* ANode) const;    // 子树的结点个数, Count(NULL) = 0
   long           Delta(TNode* ANode) const;    // 左右子树的结点个数差值, Delta(NULL) = 0
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

   // 结点层号/结点个数/高度/是否存在
   TNodeItem*     DoNode(long AIndex) const;
   long           DoLevel(TNodeItem* ANode) const;
   long           DoCount(TNodeItem* ANode) const;
   long           DoHeight(TNodeItem* ANode) const;
   long           DoIndexOf(TNodeItem* ANode) const;
   bool           DoExisted(TNodeItem* ANode) const;

   // 查找(若 ANearest 返回为 NULL, 则表示此结点大于所有结点值, 添加时要加入末尾)
   long           Compare(const TNode* ANode1, const TNode* ANode2) const;
   bool           FindNode(const TNode* ANode, PNodeItem& ANearest) const;
   TNodeItem*     FindByOrder(const TNode* ANode) const;

   // 排序树
   void           DoSort();
   void           ResetHasSorted()     { FHasSorted = false; }

   // 拷贝树, 返回拷贝结点个数
   long           CopyTree(const TKYAVLTreeEx& ATree, PNodeItem& ARoot,
                                    PNodeItem& AHead, PNodeItem& ATail,
                                    PNodeItem& ACurr);
   long           CopyNodes(TNodeItem* ADest, const TNodeItem* ASource);

   // 插入/删除结点
   TNodeItem*     InsertNode(TNode* ANode, TNodeItem* ATo, bool ANeedNew = true);
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
   TNodeItem*     DoNewNode(TNode* ANode, PNodeItem& ATo, bool& AIsAdd,
                             bool& AIsBreak,   bool  ANeedNew);

   // 减/增平衡值
   void           DecBalance(bool AIsRight, TNodeItem* AParent);
   void           IncBalance(bool AIsRight, TNodeItem* AParent,
                       TNodeItem* ATo,      TNodeItem* ANode);

   // 调整祖先结点差值
   void           AdjustParentDelta(TNodeItem* ANode, TNodeItem* ALast, bool AIsAdd);

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
   bool           FSorted;             // 是否排序, 默认值为 false
   bool           FHasSorted;          // 是否已排序, 默认值为 false
   bool           FCanDuplicate;       // 是否允许重复, 默认值为 true

private:
   // 调整结点的方法
   typedef TNodeItem* (TKYAVLTreeEx::*TDoAdjust)(TNodeItem* AParent, TNodeItem* ATo,
                                                 TNodeItem* ANode);

   // 调整结点方法列表
   static const TDoAdjust  _DoAdjust[2][2];

   // 左右结点调整的平衡增量
   static const char       _Delta_Balance[2];
};

}

#endif
