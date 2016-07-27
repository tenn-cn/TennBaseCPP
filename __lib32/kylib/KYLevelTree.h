// =======================================
// Unit   : 层次树 (KYLevelTree.h)
// Version: 3.0.0.0 (build 2012.06.02)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYLevelTree_H_
#define _KYLevelTree_H_

#include "KYMemBlock.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYLevelTree - 层次树类 */

class TKYLevelTree
{
public:
   // 树结点的附属模式
   enum TAttachMode    {knaAdd,              // 当前结点所在层的末尾
                        knaAddChild,         // 子结点的末尾
                        knaInsert,           // 当前结点所在层的上一个
                        knaAddFirst,         // 当前结点所在层的第一个
                        knaAddChildFirst};   // 子结点的第一个

   // 树结点
   typedef struct TKYTreeNode
   {
      void*             Item;                // 结点项指针(指针空间为 ItemSize)
      void*             Data;                // 指针(用于外挂附加项)
   } *PKYTreeNode;

   // 删除项事件
   typedef void (TObject::*TDoNodeDeletion)(void* Sender, PKYTreeNode ANode);
   typedef struct
   {
      TDoNodeDeletion   Method;
      void*             Object;
   } TOnNodeDeletion;

public:
   TKYLevelTree(bool ACanLock = false, Word AItemSize = 0);
   virtual ~TKYLevelTree();

   // 结点项指针分配的尺寸
   Word              ItemSize() const     { return (FItems == NULL) ? 0 : FItems->ItemSize(); }

   // 当前结点个数
   long              Count() const        { return FCount; }

   // 自定义数据
   void*             Data() const         { return FData; }
   void              SetData(void* AData) { FData = AData; }

   // 当前结点所在的层号, 层号从 0 开始
   unsigned          Level(const PKYTreeNode ANode) const
                     { return ((const PKYNodeItem)ANode)->Level; }

   // 根结点
   PKYTreeNode       Root() const         { return (PKYTreeNode)FRoot; }

   // 与根结点同层的末尾结点
   PKYTreeNode       Last() const         { return (PKYTreeNode)FLast; }

   // 最末尾结点
   PKYTreeNode       Tail() const         { return (PKYTreeNode)FTail; }

   // 根据索引读取项, AIndex 取值范围: [0..Count()-1], 因要循环查找所以不建议使用
   PKYTreeNode       Item(long AIndex) const;
   PKYTreeNode       operator[](long AIndex) const { return Item(AIndex); }

   // 取 ANode 的父结点
   PKYTreeNode       GetParent(const PKYTreeNode ANode) const;

   // 取 ANode 的下一个结点
   PKYTreeNode       GetNext(const PKYTreeNode ANode) const;

   // 取 ANode 的前一个结点
   PKYTreeNode       GetPrior(const PKYTreeNode ANode) const;

   // 取 ANode 的第一个子结点
   PKYTreeNode       GetFirstChild(const PKYTreeNode ANode) const;

   // 取 ANode 的末尾子结点
   PKYTreeNode       GetLastChild(const PKYTreeNode ANode) const;

   // 取 ANode 的下一个兄弟结点
   PKYTreeNode       GetNextSibling(const PKYTreeNode ANode) const;

   // 取 ANode 的上一个兄弟结点
   PKYTreeNode       GetPriorSibling(const PKYTreeNode ANode) const;

   // 根据层号新增结点到最末尾结点并更新 FTail
   PKYTreeNode       Add(unsigned ALevel);

   // 新增与 ANode 同层的末尾结点
   PKYTreeNode       Add(PKYTreeNode ANode);

   // 新增 ANode 的子结点层的末尾结点
   PKYTreeNode       AddChild(PKYTreeNode ANode);

   // 插入新结点在 ANode 结点之前
   PKYTreeNode       Insert(PKYTreeNode ANode);

   // 判断结点是否存在
   bool              IsExist(const PKYTreeNode ANode) const;

   // 判断结点 AChild 是否为 ANode 的子结点
   bool              IsChild(const PKYTreeNode ANode, const PKYTreeNode AChild) const;

   // 判断 ANode 是否正在删除
   bool              IsDeleting(const PKYTreeNode ANode) const;

   // 判断 ANode 是否有子结点
   bool              HasChildren(const PKYTreeNode ANode) const;

   // 复制层次树(注: 只复制树分支和结点, 不复制 Item 和 Data), 激发 OnDeletion 事件
   void              Assign(const TKYLevelTree& ASource);

   // 清除树的所有结点, 激发 OnDeletion 事件
   void              Clear();

   // 删除 ANode 结点, 激发 OnDeletion 事件
   void              Delete(PKYTreeNode ANode);

   // 移动 ANode 结点到 ADest 结点的指定链接
   void              MoveTo(PKYTreeNode ANode, PKYTreeNode ADest, TAttachMode AMode);

   // 从 AFrom 结点拷贝到 ANode 结点的指定链接(注: 只复制树分支和结点, 不复制 Item 和 Data)
   void              CopyFrom(PKYTreeNode ANode, PKYTreeNode AFrom, TAttachMode AMode);

   // 设置内存格式化方法指针
   void              SetDoInitialize(TKYFormatBlock::TFormatProc AProc)
                     { if (FItems != NULL) FItems->SetDoInitialize(AProc); }

   // 设置内存格式化事件
   void              SetOnInitialize(const TKYFmtMemEvent::TOnFormat& AEvent);

   // 事件
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
   TKYCritSect*      FLockItems;          // 项锁
   TKYLockRW*        FLockRW;             // 锁
   TKYMemHandles*    FItems;              // 存放结点的 TKYTreeNode.Item 项指针数据
   TKYFmtMemEvent    FFormatEvent;        // 格式化事件

   long              FCount;              // 结点个数
   void*             FRoot;               // 根结点
   void*             FLast;               // 与根结点同层的末尾结点
   void*             FTail;               // 最末尾结点
   void*             FData;               // 自定义数据

private:
   // 树结点项
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

   // 树结点项管理器, 存放树结点项 TKYNodeItem
   static TKYMemBlock*  Inner_ItemManager;

   // TKYLevelTree 的静态成员初始化类
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
