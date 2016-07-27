// =======================================
// Unit   : map template (KYMapObjs.h)
// Version: 3.0.2.0 (build 2014.12.14)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYMapObjs_H_
#define _KYMapObjs_H_

#include "KYAVLTree.h"
#include "KYAVLTreeEx.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMapIntKey - 整型键值 map 类模板 */

template <class T>
class TKYMapIntKey
{
public:
   // 构造函数
   // 1. ANeedFree      键值是否需要释放, 若为 false 则不释放
   // 2. ACanLock       是否内置锁, 为了多线程存取安全, 也可以外部使用锁控制
   TKYMapIntKey(bool ANeedFree = false, bool ACanLock = false)
   {
      // 创建 AVL 树
      FTree = new TKYAVLTree(ACanLock, false);

      // 判断是否需要释放
      if (ANeedFree)
      {
         FTree->OnDeletion.Object   = this;
         FTree->OnDeletion.Method   = (TKYAVLTree::TDoDeletion)
                                      &TKYMapIntKey<T>::DoDeletion;
      }
   }

   // 析构函数
   ~TKYMapIntKey()                     { FreeAndNil(FTree); }

   // 属性
   void*          Data() const         { return FTree->Data(); }           // default: NULL
   long           Count() const        { return FTree->Count(); }          // default: 0
   long           MaxCount() const     { return FTree->MaxCount(); }       // default: 0
   bool           CanDuplicate() const { return FTree->CanDuplicate(); }   // default: false

   // 设置属性
   void           SetData(void* AData) { FTree->SetData(AData); }
   void           SetMaxCount(long AMaxCount)
                  { FTree->SetMaxCount(AMaxCount); }
   void           SetCanDuplicate(bool ACanDuplicate)
                  { FTree->SetCanDuplicate(ACanDuplicate); }

   // 键值项结点的取值和设置值, 注: 不检查 ANode 是否合法
   long           Key(void* ANode) const
                  { return (long)((TKYAVLTree::TNode*)ANode)->Item; }
   T*             Value(void* ANode) const
                  { return (T*)((TKYAVLTree::TNode*)ANode)->Data; }
   void           SetValue(void* ANode, T* AValue)
                  { ((TKYAVLTree::TNode*)ANode)->Data = (void*)AValue; }

   // 判断键值项结点是否存在
   bool           Existed(void* ANode) const
                  { return FTree->Existed((TKYAVLTree::TNode*)ANode); }

   // 判断键值是否存在
   bool           Existed(long AKey) const
                  { return FTree->Existed((void*)AKey, NULL); }

   // 查找键值项
   bool           Find(long AKey, T* &AValue) const
                  { return FTree->Search((Pointer&)AKey, (Pointer&)AValue,
                                         (void*)AKey, NULL); }

   // 查找键值项, 若返回值为 NULL 则未找到键值, 否则返回找到的键值项结点
   void*          Find(long AKey) const
                  { return (void*)FTree->Search((void*)AKey, NULL); }

   // 查找最近一个键值项结点
   // 若 ANearest 返回 NULL 则表示键值大于最后一个结点键值;
   // 若返回值为 true, 则表示找到键值项结点, 否则键值在 ANearest 键值项结点之前
   bool           FindNearest(long AKey, Pointer& ANearest) const
                  { return FTree->FindNearest((void*)AKey, NULL, (TKYAVLTree::PNode&)ANearest); }

   // 添加键值项, 若返回值为 NULL 则加入失败, 否则返回加入的键值项结点
   void*          Add(long AKey, T* AValue)
                  { return (void*)FTree->Add((void*)AKey, (void*)AValue); }

   // 删除键值项
   bool           Delete(long AKey)    { return FTree->Delete((void*)AKey, NULL); }

   // 删除键值项结点
   bool           Remove(void* ANode)  { return FTree->Remove((TKYAVLTree::TNode*)ANode); }

   // 清除所有键值项结点
   void           Clear()              { FTree->Clear(); }

   // 取第一个键值项结点
   void*          First() const        { return (void*)FTree->Next(NULL); }

   // 取最后一个键值项结点
   void*          Last() const         { return (void*)FTree->Prior(NULL); }

   // 取下一键值项结点, 若ANode == NULL, 则取第一个键值项结点
   void*          Next(void* ANode) const
                  { return (void*)FTree->Next((TKYAVLTree::TNode*)ANode); }

   // 取上一键值项结点, 若ANode == NULL, 则取最后一个键值项结点
   void*          Prior(void* ANode) const
                  { return (void*)FTree->Prior((TKYAVLTree::TNode*)ANode); }

private:
   void           DoDeletion(void* Sender, TKYAVLTree::TNode* ANode);

private:
   TKYAVLTree*    FTree;
};

// FTree 的 OnDeletion 事件方法
template <class T>
void TKYMapIntKey<T>::DoDeletion(void* Sender, TKYAVLTree::TNode* ANode)
{
   delete (T*)ANode->Data;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMapStrKey - 字符串键值 map 类模板 */

template <class T>
class TKYMapStrKey
{
public:
   // 构造函数
   // 1. ACaseSensitive 键值是否区分大小写, 若为 false 则不区分大小写
   // 2. ANeedFree      键值是否需要释放, 若为 false 则不释放
   // 3. ACanLock       是否内置锁, 为了多线程存取安全, 也可以外部使用锁控制
   TKYMapStrKey(bool ACaseSensitive = false, bool ANeedFree = false,
                bool ACanLock       = false)
   {
      // 初始化
      FNeedFree                  = ANeedFree;
      FCaseSensitive             = ACaseSensitive;

      // 创建 AVL 树
      FTree                      = new TKYAVLTree(ACanLock, false);

      // 设置 FTree 的事件方法
      FTree->OnCompare.Object    = this;
      FTree->OnCompare.Method    = (TKYAVLTree::TDoCompare)
                                   &TKYMapStrKey<T>::DoCompare;
      FTree->OnDeletion.Object   = this;
      FTree->OnDeletion.Method   = (TKYAVLTree::TDoDeletion)
                                   &TKYMapStrKey<T>::DoDeletion;
   }

   // 析构函数
   ~TKYMapStrKey()                     { FreeAndNil(FTree); }

   // 属性
   void*          Data() const         { return FTree->Data(); }           // default: NULL
   long           Count() const        { return FTree->Count(); }          // default: 0
   long           MaxCount() const     { return FTree->MaxCount(); }       // default: 0
   bool           CanDuplicate() const { return FTree->CanDuplicate(); }   // default: false
   bool           CaseSensitive() const{ return FCaseSensitive; }          // default: false

   // 设置属性
   void           SetData(void* AData) { FTree->SetData(AData); }
   void           SetMaxCount(long AMaxCount)
                  { FTree->SetMaxCount(AMaxCount); }
   void           SetCanDuplicate(bool ACanDuplicate)
                  { FTree->SetCanDuplicate(ACanDuplicate); }

   // 键值项结点的取值和设置值, 注: 不检查 ANode 是否合法
   KYString       Key(void* ANode) const
                  { return *(KYString*)((TKYAVLTree::TNode*)ANode)->Item; }
   T*             Value(void* ANode) const
                  { return (T*)((TKYAVLTree::TNode*)ANode)->Data; }
   void           SetValue(void* ANode, T* AValue)
                  { ((TKYAVLTree::TNode*)ANode)->Data = (void*)AValue; }

   // 判断键值项结点是否存在
   bool           Existed(void* ANode) const
                  { return FTree->Existed((TKYAVLTree::TNode*)ANode); }

   // 判断键值是否存在
   bool           Existed(const KYString& AKey) const
                  { return FTree->Existed((void*)&AKey, NULL); }

   // 查找键值项
   bool           Find(const KYString& AKey, T* &AValue) const
                  {
                     void* pKey;
                     return FTree->Search(pKey, (Pointer&)AValue, &AKey, NULL);
                  }

   // 查找键值项, 若返回值为 NULL 则未找到键值, 否则返回找到的键值项结点
   void*          Find(const KYString& AKey) const
                  { return (void*)FTree->Search(&AKey, NULL); }

   // 查找最近一个键值项结点
   // 若 ANearest 返回 NULL 则表示键值大于最后一个结点键值;
   // 若返回值为 true, 则表示找到键值项结点, 否则键值在 ANearest 键值项结点之前
   bool           FindNearest(const KYString& AKey, Pointer& ANearest) const
                  { return FTree->FindNearest(&AKey, NULL, (TKYAVLTree::PNode&)ANearest); }

   // 添加键值项, 若返回值为 NULL 则加入失败, 否则返回加入的键值项结点
   void*          Add(const KYString& AKey, T* AValue);

   // 删除键值项
   bool           Delete(const KYString& AKey)
                  { return FTree->Delete((void*)&AKey, NULL); }

   // 删除键值项结点
   bool           Remove(void* ANode)  { return FTree->Remove((TKYAVLTree::TNode*)ANode); }

   // 清除所有键值项结点
   void           Clear()              { FTree->Clear(); }

   // 取第一个键值项结点
   void*          First() const        { return (void*)FTree->Next(NULL); }

   // 取最后一个键值项结点
   void*          Last() const         { return (void*)FTree->Prior(NULL); }

   // 取下一键值项结点, 若ANode == NULL, 则取第一个键值项结点
   void*          Next(void* ANode) const
                  { return (void*)FTree->Next((TKYAVLTree::TNode*)ANode); }

   // 取上一键值项结点, 若ANode == NULL, 则取最后一个键值项结点
   void*          Prior(void* ANode) const
                  { return (void*)FTree->Prior((TKYAVLTree::TNode*)ANode); }

private:
   void           DoCompare(const TKYAVLTree::TNode* ANode1,
                            const TKYAVLTree::TNode* ANode2, long& ACompare);
   void           DoDeletion(void* Sender, TKYAVLTree::TNode* ANode);

private:
   TKYAVLTree*    FTree;
   bool           FNeedFree;
   bool           FCaseSensitive;
};

// FTree 的 OnCompare 事件方法
template <class T>
void TKYMapStrKey<T>::DoCompare(const TKYAVLTree::TNode* ANode1,
                                const TKYAVLTree::TNode* ANode2, long& ACompare)
{
   ACompare = CompareStr(*(KYString*)ANode1->Item,
                         *(KYString*)ANode2->Item, FCaseSensitive);
}

// FTree 的 OnDeletion 事件方法
template <class T>
void TKYMapStrKey<T>::DoDeletion(void* Sender, TKYAVLTree::TNode* ANode)
{
   // 释放键值
   delete (KYString*)ANode->Item;

   // 判断是否需要释放
   if (FNeedFree)
      delete (T*)ANode->Data;
}

// 添加键值项, 若返回值为 NULL 则加入失败, 否则返回加入的键值项结点
template <class T>
void* TKYMapStrKey<T>::Add(const KYString& AKey, T* AValue)
{
   // 加入
   KYString*          pKey    = new KYString(AKey);
   TKYAVLTree::TNode* result  = FTree->Add((void*)pKey, (void*)AValue);

   // 若加入失败则释放
   if (result == NULL)
      delete pKey;

   // 返回结果
   return (void*)result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMapObjKey - 指针键值 map 类模板 */

template <class T>
class TKYMapObjKey
{
public:
   // 构造函数
   // 1. ANeedFree      键值是否需要释放, 若为 false 则不释放
   // 2. ACanLock       是否内置锁, 为了多线程存取安全, 也可以外部使用锁控制
   TKYMapObjKey(bool ANeedFree = false, bool ACanLock = false)
   {
      // 创建 AVL 树
      FTree = new TKYAVLTree(ACanLock, false);

      // 判断是否需要释放
      if (ANeedFree)
      {
         FTree->OnDeletion.Object   = this;
         FTree->OnDeletion.Method   = (TKYAVLTree::TDoDeletion)
                                      &TKYMapObjKey<T>::DoDeletion;
      }
   }

   // 析构函数
   ~TKYMapObjKey()                     { FreeAndNil(FTree); }

   // 属性
   void*          Data() const         { return FTree->Data(); }           // default: NULL
   long           Count() const        { return FTree->Count(); }          // default: 0
   long           MaxCount() const     { return FTree->MaxCount(); }       // default: 0
   bool           CanDuplicate() const { return FTree->CanDuplicate(); }   // default: false

   // 设置属性
   void           SetData(void* AData) { FTree->SetData(AData); }
   void           SetMaxCount(long AMaxCount)
                  { FTree->SetMaxCount(AMaxCount); }
   void           SetCanDuplicate(bool ACanDuplicate)
                  { FTree->SetCanDuplicate(ACanDuplicate); }

   // 键值项结点的取值和设置值, 注: 不检查 ANode 是否合法
   void*          Key(void* ANode) const
                  { return ((TKYAVLTree::TNode*)ANode)->Item; }
   T*             Value(void* ANode) const
                  { return (T*)((TKYAVLTree::TNode*)ANode)->Data; }
   void           SetValue(void* ANode, T* AValue)
                  { ((TKYAVLTree::TNode*)ANode)->Data = (void*)AValue; }

   // 判断键值项结点是否存在
   bool           NodeExisted(void* ANode) const
                  { return FTree->Existed((TKYAVLTree::TNode*)ANode); }

   // 判断键值是否存在
   bool           Existed(void* AKey) const
                  { return FTree->Existed(AKey, NULL); }

   // 查找键值项
   bool           Find(void* AKey, T* &AValue) const
                  { return FTree->Search(AKey, (Pointer&)AValue, AKey, NULL); }

   // 查找键值项, 若返回值为 NULL 则未找到键值, 否则返回找到的键值项结点
   void*          Find(void* AKey) const
                  { return (void*)FTree->Search(AKey, NULL); }

   // 查找最近一个键值项结点
   // 若 ANearest 返回 NULL 则表示键值大于最后一个结点键值;
   // 若返回值为 true, 则表示找到键值项结点, 否则键值在 ANearest 键值项结点之前
   bool           FindNearest(void* AKey, Pointer& ANearest) const
                  { return FTree->FindNearest(AKey, NULL, (TKYAVLTree::PNode&)ANearest); }

   // 添加键值项, 若返回值为 NULL 则加入失败, 否则返回加入的键值项结点
   void*          Add(void* AKey, T* AValue)
                  { return (void*)FTree->Add(AKey, (void*)AValue); }

   // 删除键值项
   bool           Delete(void* AKey)   { return FTree->Delete(AKey, NULL); }

   // 删除键值项结点
   bool           Remove(void* ANode)  { return FTree->Remove((TKYAVLTree::TNode*)ANode); }

   // 清除所有键值项结点
   void           Clear()              { FTree->Clear(); }

   // 取第一个键值项结点
   void*          First() const        { return (void*)FTree->Next(NULL); }

   // 取最后一个键值项结点
   void*          Last() const         { return (void*)FTree->Prior(NULL); }

   // 取下一键值项结点, 若ANode == NULL, 则取第一个键值项结点
   void*          Next(void* ANode) const
                  { return (void*)FTree->Next((TKYAVLTree::TNode*)ANode); }

   // 取上一键值项结点, 若ANode == NULL, 则取最后一个键值项结点
   void*          Prior(void* ANode) const
                  { return (void*)FTree->Prior((TKYAVLTree::TNode*)ANode); }

private:
   void           DoDeletion(void* Sender, TKYAVLTree::TNode* ANode);

private:
   TKYAVLTree*    FTree;
};

// FTree 的 OnDeletion 事件方法
template <class T>
void TKYMapObjKey<T>::DoDeletion(void* Sender, TKYAVLTree::TNode* ANode)
{
   delete (T*)ANode->Data;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMapIntKeyEx - 整型键值 map 类模板 */

template <class T>
class TKYMapIntKeyEx
{
public:
   // 构造函数
   // 1. ANeedFree      键值是否需要释放, 若为 false 则不释放
   // 2. ACanLock       是否内置锁, 为了多线程存取安全, 也可以外部使用锁控制
   TKYMapIntKeyEx(bool ANeedFree = false, bool ACanLock = false)
   {
      // 创建 AVL 树
      FTree = new TKYAVLTreeEx(ACanLock, false);

      // 判断是否需要释放
      if (ANeedFree)
      {
         FTree->OnDeletion.Object   = this;
         FTree->OnDeletion.Method   = (TKYAVLTreeEx::TDoDeletion)
                                      &TKYMapIntKeyEx<T>::DoDeletion;
      }
   }

   // 析构函数
   ~TKYMapIntKeyEx()                   { FreeAndNil(FTree); }

   // 属性
   void*          Data() const         { return FTree->Data(); }           // default: NULL
   long           Count() const        { return FTree->Count(); }          // default: 0
   bool           Sorted() const       { return FTree->Sorted(); }         // default: true
   long           MaxCount() const     { return FTree->MaxCount(); }       // default: 0
   bool           CanDuplicate() const { return FTree->CanDuplicate(); }   // default: false

   // 设置属性
   void           SetData(void* AData) { FTree->SetData(AData); }
   void           SetSorted(bool ASorted)
                  { FTree->SetSorted(ASorted); }
   void           SetMaxCount(long AMaxCount)
                  { FTree->SetMaxCount(AMaxCount); }
   void           SetCanDuplicate(bool ACanDuplicate)
                  { FTree->SetCanDuplicate(ACanDuplicate); }

   // 键值项结点的取值和设置值, 注: 不检查 ANode 是否合法
   long           Key(void* ANode) const
                  { return (long)((TKYAVLTreeEx::TNode*)ANode)->Item; }
   T*             Value(void* ANode) const
                  { return (T*)((TKYAVLTreeEx::TNode*)ANode)->Data; }
   void           SetValue(void* ANode, T* AValue)
                  { ((TKYAVLTreeEx::TNode*)ANode)->Data = (void*)AValue; }

   // 根据索引读取键值项结点, AIndex 取值范围: [0..Count()-1], 可以快速定位指定索引结点
   void*          Node(long AIndex) const
                  { return (void*)FTree->Node(AIndex); }

   // 取键值项结点所在的索引, 若不存在则返回 -1
   long           IndexOf(void* ANode) const
                  { return FTree->IndexOf((TKYAVLTreeEx::TNode*)ANode); }

   // 取键值所在的索引, 若不存在则返回 -1
   long           IndexOf(long AKey) const
                  { return FTree->IndexOf((void*)AKey, NULL); }

   // 判断键值项结点是否存在
   bool           Existed(void* ANode) const
                  { return FTree->Existed((TKYAVLTreeEx::TNode*)ANode); }

   // 判断键值是否存在
   bool           Existed(long AKey) const
                  { return FTree->Existed((void*)AKey, NULL); }

   // 从小到大排序
   void           Sort()               { FTree->Sort(); }

   // 查找键值项
   bool           Find(long AKey, T* &AValue) const
                  { return FTree->Search((Pointer&)AKey, (Pointer&)AValue,
                                         (void*)AKey, NULL); }

   // 查找键值项, 若返回值为 NULL 则未找到键值, 否则返回找到的键值项结点
   void*          Find(long AKey) const
                  { return (void*)FTree->Search((void*)AKey, NULL); }

   // 查找最近一个键值项结点
   // 若未排序则 ANearest 就是查找结果, 也就是说, 若未找到则返回 NULL;
   // 若 ANearest 返回 NULL 则表示键值大于最后一个结点键值;
   // 若返回值为 true, 则表示找到键值项结点, 否则键值在 ANearest 键值项结点之前
   bool           FindNearest(long AKey, Pointer& ANearest) const
                  { return FTree->FindNearest((void*)AKey, NULL, (TKYAVLTreeEx::PNode&)ANearest); }

   // 添加键值项, 若返回值为 NULL 则加入失败, 否则返回加入的键值项结点
   void*          Add(long AKey, T* AValue)
                  { return (void*)FTree->Add((void*)AKey, (void*)AValue); }

   // 删除键值项
   bool           Delete(long AKey)    { return FTree->Delete((void*)AKey, NULL); }

   // 删除键值项结点
   bool           Remove(void* ANode)  { return FTree->Remove((TKYAVLTreeEx::TNode*)ANode); }

   // 清除所有键值项结点
   void           Clear()              { FTree->Clear(); }

   // 取第一个键值项结点
   void*          First() const        { return (void*)FTree->Next(NULL); }

   // 取最后一个键值项结点
   void*          Last() const         { return (void*)FTree->Prior(NULL); }

   // 取下一键值项结点, 若ANode == NULL, 则取第一个键值项结点
   void*          Next(void* ANode) const
                  { return (void*)FTree->Next((TKYAVLTreeEx::TNode*)ANode); }

   // 取上一键值项结点, 若ANode == NULL, 则取最后一个键值项结点
   void*          Prior(void* ANode) const
                  { return (void*)FTree->Prior((TKYAVLTreeEx::TNode*)ANode); }

private:
   void           DoDeletion(void* Sender, TKYAVLTreeEx::TNode* ANode);

private:
   TKYAVLTreeEx*  FTree;
};

// FTree 的 OnDeletion 事件方法
template <class T>
void TKYMapIntKeyEx<T>::DoDeletion(void* Sender, TKYAVLTreeEx::TNode* ANode)
{
   delete (T*)ANode->Data;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMapStrKeyEx - 字符串键值 map 类模板 */

template <class T>
class TKYMapStrKeyEx
{
public:
   // 构造函数
   // 1. ACaseSensitive 键值是否区分大小写, 若为 false 则不区分大小写
   // 2. ANeedFree      键值是否需要释放, 若为 false 则不释放
   // 3. ACanLock       是否内置锁, 为了多线程存取安全, 也可以外部使用锁控制
   TKYMapStrKeyEx(bool ACaseSensitive = false, bool ANeedFree = false,
                  bool ACanLock       = false)
   {
      // 初始化
      FNeedFree                  = ANeedFree;
      FCaseSensitive             = ACaseSensitive;

      // 创建 AVL 树
      FTree                      = new TKYAVLTreeEx(ACanLock, false);

      // 设置 FTree 的事件方法
      FTree->OnCompare.Object    = this;
      FTree->OnCompare.Method    = (TKYAVLTreeEx::TDoCompare)
                                   &TKYMapStrKeyEx<T>::DoCompare;
      FTree->OnDeletion.Object   = this;
      FTree->OnDeletion.Method   = (TKYAVLTreeEx::TDoDeletion)
                                   &TKYMapStrKeyEx<T>::DoDeletion;
   }

   // 析构函数
   ~TKYMapStrKeyEx()                   { FreeAndNil(FTree); }

   // 属性
   void*          Data() const         { return FTree->Data(); }           // default: NULL
   long           Count() const        { return FTree->Count(); }          // default: 0
   bool           Sorted() const       { return FTree->Sorted(); }         // default: true
   long           MaxCount() const     { return FTree->MaxCount(); }       // default: 0
   bool           CanDuplicate() const { return FTree->CanDuplicate(); }   // default: false
   bool           CaseSensitive() const{ return FCaseSensitive; }          // default: false

   // 设置属性
   void           SetData(void* AData) { FTree->SetData(AData); }
   void           SetSorted(bool ASorted)
                  { FTree->SetSorted(ASorted); }
   void           SetMaxCount(long AMaxCount)
                  { FTree->SetMaxCount(AMaxCount); }
   void           SetCanDuplicate(bool ACanDuplicate)
                  { FTree->SetCanDuplicate(ACanDuplicate); }

   // 键值项结点的取值和设置值, 注: 不检查 ANode 是否合法
   KYString       Key(void* ANode) const
                  { return *(KYString*)((TKYAVLTreeEx::TNode*)ANode)->Item; }
   T*             Value(void* ANode) const
                  { return (T*)((TKYAVLTreeEx::TNode*)ANode)->Data; }
   void           SetValue(void* ANode, T* AValue)
                  { ((TKYAVLTreeEx::TNode*)ANode)->Data = (void*)AValue; }

   // 根据索引读取键值项结点, AIndex 取值范围: [0..Count()-1], 可以快速定位指定索引结点
   void*          Node(long AIndex) const
                  { return (void*)FTree->Node(AIndex); }

   // 取键值项结点所在的索引, 若不存在则返回 -1
   long           IndexOf(void* ANode) const
                  { return FTree->IndexOf((TKYAVLTreeEx::TNode*)ANode); }

   // 取键值所在的索引, 若不存在则返回 -1
   long           IndexOf(const KYString& AKey) const
                  { return FTree->IndexOf((void*)&AKey, NULL); }

   // 判断键值项结点是否存在
   bool           Existed(void* ANode) const
                  { return FTree->Existed((TKYAVLTreeEx::TNode*)ANode); }

   // 判断键值是否存在
   bool           Existed(const KYString& AKey) const
                  { return FTree->Existed((void*)&AKey, NULL); }

   // 从小到大排序
   void           Sort()               { FTree->Sort(); }

   // 查找键值项
   bool           Find(const KYString& AKey, T* &AValue) const
                  {
                     void* pKey;
                     return FTree->Search(pKey, (Pointer&)AValue, &AKey, NULL);
                  }

   // 查找键值项, 若返回值为 NULL 则未找到键值, 否则返回找到的键值项结点
   void*          Find(const KYString& AKey) const
                  { return (void*)FTree->Search(&AKey, NULL); }

   // 查找最近一个键值项结点
   // 若未排序则 ANearest 就是查找结果, 也就是说, 若未找到则返回 NULL;
   // 若 ANearest 返回 NULL 则表示键值大于最后一个结点键值;
   // 若返回值为 true, 则表示找到键值项结点, 否则键值在 ANearest 键值项结点之前
   bool           FindNearest(const KYString& AKey, Pointer& ANearest) const
                  { return FTree->FindNearest(&AKey, NULL, (TKYAVLTreeEx::PNode&)ANearest); }

   // 添加键值项, 若返回值为 NULL 则加入失败, 否则返回加入的键值项结点
   void*          Add(const KYString& AKey, T* AValue);

   // 删除键值项
   bool           Delete(const KYString& AKey)
                  { return FTree->Delete((void*)&AKey, NULL); }

   // 删除键值项结点
   bool           Remove(void* ANode)  { return FTree->Remove((TKYAVLTreeEx::TNode*)ANode); }

   // 清除所有键值项结点
   void           Clear()              { FTree->Clear(); }

   // 取第一个键值项结点
   void*          First() const        { return (void*)FTree->Next(NULL); }

   // 取最后一个键值项结点
   void*          Last() const         { return (void*)FTree->Prior(NULL); }

   // 取下一键值项结点, 若ANode == NULL, 则取第一个键值项结点
   void*          Next(void* ANode) const
                  { return (void*)FTree->Next((TKYAVLTreeEx::TNode*)ANode); }

   // 取上一键值项结点, 若ANode == NULL, 则取最后一个键值项结点
   void*          Prior(void* ANode) const
                  { return (void*)FTree->Prior((TKYAVLTreeEx::TNode*)ANode); }

private:
   void           DoCompare(const TKYAVLTreeEx::TNode* ANode1,
                            const TKYAVLTreeEx::TNode* ANode2, long& ACompare);
   void           DoDeletion(void* Sender, TKYAVLTreeEx::TNode* ANode);

private:
   TKYAVLTreeEx*  FTree;
   bool           FNeedFree;
   bool           FCaseSensitive;
};

// FTree 的 OnCompare 事件方法
template <class T>
void TKYMapStrKeyEx<T>::DoCompare(const TKYAVLTreeEx::TNode* ANode1,
                                  const TKYAVLTreeEx::TNode* ANode2, long& ACompare)
{
   ACompare = CompareStr(*(KYString*)ANode1->Item,
                         *(KYString*)ANode2->Item, FCaseSensitive);
}

// FTree 的 OnDeletion 事件方法
template <class T>
void TKYMapStrKeyEx<T>::DoDeletion(void* Sender, TKYAVLTreeEx::TNode* ANode)
{
   // 释放键值
   delete (KYString*)ANode->Item;

   // 判断是否需要释放
   if (FNeedFree)
      delete (T*)ANode->Data;
}

// 添加键值项, 若返回值为 NULL 则加入失败, 否则返回加入的键值项结点
template <class T>
void* TKYMapStrKeyEx<T>::Add(const KYString& AKey, T* AValue)
{
   // 加入
   KYString*            pKey     = new KYString(AKey);
   TKYAVLTreeEx::TNode* result   = FTree->Add((void*)pKey, (void*)AValue);

   // 若加入失败则释放
   if (result == NULL)
      delete pKey;

   // 返回结果
   return (void*)result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMapObjKeyEx - 指针键值 map 类模板 */

template <class T>
class TKYMapObjKeyEx
{
public:
   // 构造函数
   // 1. ANeedFree      键值是否需要释放, 若为 false 则不释放
   // 2. ACanLock       是否内置锁, 为了多线程存取安全, 也可以外部使用锁控制
   TKYMapObjKeyEx(bool ANeedFree = false, bool ACanLock = false)
   {
      // 创建 AVL 树
      FTree = new TKYAVLTreeEx(ACanLock, false);

      // 判断是否需要释放
      if (ANeedFree)
      {
         FTree->OnDeletion.Object   = this;
         FTree->OnDeletion.Method   = (TKYAVLTreeEx::TDoDeletion)
                                      &TKYMapObjKeyEx<T>::DoDeletion;
      }
   }

   // 析构函数
   ~TKYMapObjKeyEx()                   { FreeAndNil(FTree); }

   // 属性
   void*          Data() const         { return FTree->Data(); }           // default: NULL
   long           Count() const        { return FTree->Count(); }          // default: 0
   bool           Sorted() const       { return FTree->Sorted(); }         // default: true
   long           MaxCount() const     { return FTree->MaxCount(); }       // default: 0
   bool           CanDuplicate() const { return FTree->CanDuplicate(); }   // default: false

   // 设置属性
   void           SetData(void* AData) { FTree->SetData(AData); }
   void           SetSorted(bool ASorted)
                  { FTree->SetSorted(ASorted); }
   void           SetMaxCount(long AMaxCount)
                  { FTree->SetMaxCount(AMaxCount); }
   void           SetCanDuplicate(bool ACanDuplicate)
                  { FTree->SetCanDuplicate(ACanDuplicate); }

   // 键值项结点的取值和设置值, 注: 不检查 ANode 是否合法
   void*          Key(void* ANode) const
                  { return ((TKYAVLTreeEx::TNode*)ANode)->Item; }
   T*             Value(void* ANode) const
                  { return (T*)((TKYAVLTreeEx::TNode*)ANode)->Data; }
   void           SetValue(void* ANode, T* AValue)
                  { ((TKYAVLTreeEx::TNode*)ANode)->Data = (void*)AValue; }

   // 根据索引读取键值项结点, AIndex 取值范围: [0..Count()-1], 可以快速定位指定索引结点
   void*          Node(long AIndex) const
                  { return (void*)FTree->Node(AIndex); }

   // 取键值项结点所在的索引, 若不存在则返回 -1
   long           NodeIndexOf(void* ANode) const
                  { return FTree->IndexOf((TKYAVLTreeEx::TNode*)ANode); }

   // 判断键值项结点是否存在
   bool           NodeExisted(void* ANode) const
                  { return FTree->Existed((TKYAVLTreeEx::TNode*)ANode); }

   // 取键值所在的索引, 若不存在则返回 -1
   long           IndexOf(void* AKey) const
                  { return FTree->IndexOf(AKey, NULL); }

   // 判断键值是否存在
   bool           Existed(void* AKey) const
                  { return FTree->Existed(AKey, NULL); }

   // 从小到大排序
   void           Sort()               { FTree->Sort(); }

   // 查找键值项
   bool           Find(void* AKey, T* &AValue) const
                  { return FTree->Search(AKey, (Pointer&)AValue, AKey, NULL); }

   // 查找键值项, 若返回值为 NULL 则未找到键值, 否则返回找到的键值项结点
   void*          Find(void* AKey) const
                  { return (void*)FTree->Search(AKey, NULL); }

   // 查找最近一个键值项结点
   // 若未排序则 ANearest 就是查找结果, 也就是说, 若未找到则返回 NULL;
   // 若 ANearest 返回 NULL 则表示键值大于最后一个结点键值;
   // 若返回值为 true, 则表示找到键值项结点, 否则键值在 ANearest 键值项结点之前
   bool           FindNearest(void* AKey, Pointer& ANearest) const
                  { return FTree->FindNearest(AKey, NULL, (TKYAVLTreeEx::PNode&)ANearest); }

   // 添加键值项, 若返回值为 NULL 则加入失败, 否则返回加入的键值项结点
   void*          Add(void* AKey, T* AValue)
                  { return (void*)FTree->Add(AKey, (void*)AValue); }

   // 删除键值项
   bool           Delete(void* AKey)   { return FTree->Delete(AKey, NULL); }

   // 删除键值项结点
   bool           Remove(void* ANode)  { return FTree->Remove((TKYAVLTreeEx::TNode*)ANode); }

   // 清除所有键值项结点
   void           Clear()              { FTree->Clear(); }

   // 取第一个键值项结点
   void*          First() const        { return (void*)FTree->Next(NULL); }

   // 取最后一个键值项结点
   void*          Last() const         { return (void*)FTree->Prior(NULL); }

   // 取下一键值项结点, 若ANode == NULL, 则取第一个键值项结点
   void*          Next(void* ANode) const
                  { return (void*)FTree->Next((TKYAVLTreeEx::TNode*)ANode); }

   // 取上一键值项结点, 若ANode == NULL, 则取最后一个键值项结点
   void*          Prior(void* ANode) const
                  { return (void*)FTree->Prior((TKYAVLTreeEx::TNode*)ANode); }

private:
   void           DoDeletion(void* Sender, TKYAVLTreeEx::TNode* ANode);

private:
   TKYAVLTreeEx*  FTree;
};

// FTree 的 OnDeletion 事件方法
template <class T>
void TKYMapObjKeyEx<T>::DoDeletion(void* Sender, TKYAVLTreeEx::TNode* ANode)
{
   delete (T*)ANode->Data;
}

}

#endif
