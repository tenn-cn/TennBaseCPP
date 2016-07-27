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

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMapIntKey - ���ͼ�ֵ map ��ģ�� */

template <class T>
class TKYMapIntKey
{
public:
   // ���캯��
   // 1. ANeedFree      ��ֵ�Ƿ���Ҫ�ͷ�, ��Ϊ false ���ͷ�
   // 2. ACanLock       �Ƿ�������, Ϊ�˶��̴߳�ȡ��ȫ, Ҳ�����ⲿʹ��������
   TKYMapIntKey(bool ANeedFree = false, bool ACanLock = false)
   {
      // ���� AVL ��
      FTree = new TKYAVLTree(ACanLock, false);

      // �ж��Ƿ���Ҫ�ͷ�
      if (ANeedFree)
      {
         FTree->OnDeletion.Object   = this;
         FTree->OnDeletion.Method   = (TKYAVLTree::TDoDeletion)
                                      &TKYMapIntKey<T>::DoDeletion;
      }
   }

   // ��������
   ~TKYMapIntKey()                     { FreeAndNil(FTree); }

   // ����
   void*          Data() const         { return FTree->Data(); }           // default: NULL
   long           Count() const        { return FTree->Count(); }          // default: 0
   long           MaxCount() const     { return FTree->MaxCount(); }       // default: 0
   bool           CanDuplicate() const { return FTree->CanDuplicate(); }   // default: false

   // ��������
   void           SetData(void* AData) { FTree->SetData(AData); }
   void           SetMaxCount(long AMaxCount)
                  { FTree->SetMaxCount(AMaxCount); }
   void           SetCanDuplicate(bool ACanDuplicate)
                  { FTree->SetCanDuplicate(ACanDuplicate); }

   // ��ֵ�����ȡֵ������ֵ, ע: ����� ANode �Ƿ�Ϸ�
   long           Key(void* ANode) const
                  { return (long)((TKYAVLTree::TNode*)ANode)->Item; }
   T*             Value(void* ANode) const
                  { return (T*)((TKYAVLTree::TNode*)ANode)->Data; }
   void           SetValue(void* ANode, T* AValue)
                  { ((TKYAVLTree::TNode*)ANode)->Data = (void*)AValue; }

   // �жϼ�ֵ�����Ƿ����
   bool           Existed(void* ANode) const
                  { return FTree->Existed((TKYAVLTree::TNode*)ANode); }

   // �жϼ�ֵ�Ƿ����
   bool           Existed(long AKey) const
                  { return FTree->Existed((void*)AKey, NULL); }

   // ���Ҽ�ֵ��
   bool           Find(long AKey, T* &AValue) const
                  { return FTree->Search((Pointer&)AKey, (Pointer&)AValue,
                                         (void*)AKey, NULL); }

   // ���Ҽ�ֵ��, ������ֵΪ NULL ��δ�ҵ���ֵ, ���򷵻��ҵ��ļ�ֵ����
   void*          Find(long AKey) const
                  { return (void*)FTree->Search((void*)AKey, NULL); }

   // �������һ����ֵ����
   // �� ANearest ���� NULL ���ʾ��ֵ�������һ������ֵ;
   // ������ֵΪ true, ���ʾ�ҵ���ֵ����, �����ֵ�� ANearest ��ֵ����֮ǰ
   bool           FindNearest(long AKey, Pointer& ANearest) const
                  { return FTree->FindNearest((void*)AKey, NULL, (TKYAVLTree::PNode&)ANearest); }

   // ��Ӽ�ֵ��, ������ֵΪ NULL �����ʧ��, ���򷵻ؼ���ļ�ֵ����
   void*          Add(long AKey, T* AValue)
                  { return (void*)FTree->Add((void*)AKey, (void*)AValue); }

   // ɾ����ֵ��
   bool           Delete(long AKey)    { return FTree->Delete((void*)AKey, NULL); }

   // ɾ����ֵ����
   bool           Remove(void* ANode)  { return FTree->Remove((TKYAVLTree::TNode*)ANode); }

   // ������м�ֵ����
   void           Clear()              { FTree->Clear(); }

   // ȡ��һ����ֵ����
   void*          First() const        { return (void*)FTree->Next(NULL); }

   // ȡ���һ����ֵ����
   void*          Last() const         { return (void*)FTree->Prior(NULL); }

   // ȡ��һ��ֵ����, ��ANode == NULL, ��ȡ��һ����ֵ����
   void*          Next(void* ANode) const
                  { return (void*)FTree->Next((TKYAVLTree::TNode*)ANode); }

   // ȡ��һ��ֵ����, ��ANode == NULL, ��ȡ���һ����ֵ����
   void*          Prior(void* ANode) const
                  { return (void*)FTree->Prior((TKYAVLTree::TNode*)ANode); }

private:
   void           DoDeletion(void* Sender, TKYAVLTree::TNode* ANode);

private:
   TKYAVLTree*    FTree;
};

// FTree �� OnDeletion �¼�����
template <class T>
void TKYMapIntKey<T>::DoDeletion(void* Sender, TKYAVLTree::TNode* ANode)
{
   delete (T*)ANode->Data;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMapStrKey - �ַ�����ֵ map ��ģ�� */

template <class T>
class TKYMapStrKey
{
public:
   // ���캯��
   // 1. ACaseSensitive ��ֵ�Ƿ����ִ�Сд, ��Ϊ false �����ִ�Сд
   // 2. ANeedFree      ��ֵ�Ƿ���Ҫ�ͷ�, ��Ϊ false ���ͷ�
   // 3. ACanLock       �Ƿ�������, Ϊ�˶��̴߳�ȡ��ȫ, Ҳ�����ⲿʹ��������
   TKYMapStrKey(bool ACaseSensitive = false, bool ANeedFree = false,
                bool ACanLock       = false)
   {
      // ��ʼ��
      FNeedFree                  = ANeedFree;
      FCaseSensitive             = ACaseSensitive;

      // ���� AVL ��
      FTree                      = new TKYAVLTree(ACanLock, false);

      // ���� FTree ���¼�����
      FTree->OnCompare.Object    = this;
      FTree->OnCompare.Method    = (TKYAVLTree::TDoCompare)
                                   &TKYMapStrKey<T>::DoCompare;
      FTree->OnDeletion.Object   = this;
      FTree->OnDeletion.Method   = (TKYAVLTree::TDoDeletion)
                                   &TKYMapStrKey<T>::DoDeletion;
   }

   // ��������
   ~TKYMapStrKey()                     { FreeAndNil(FTree); }

   // ����
   void*          Data() const         { return FTree->Data(); }           // default: NULL
   long           Count() const        { return FTree->Count(); }          // default: 0
   long           MaxCount() const     { return FTree->MaxCount(); }       // default: 0
   bool           CanDuplicate() const { return FTree->CanDuplicate(); }   // default: false
   bool           CaseSensitive() const{ return FCaseSensitive; }          // default: false

   // ��������
   void           SetData(void* AData) { FTree->SetData(AData); }
   void           SetMaxCount(long AMaxCount)
                  { FTree->SetMaxCount(AMaxCount); }
   void           SetCanDuplicate(bool ACanDuplicate)
                  { FTree->SetCanDuplicate(ACanDuplicate); }

   // ��ֵ�����ȡֵ������ֵ, ע: ����� ANode �Ƿ�Ϸ�
   KYString       Key(void* ANode) const
                  { return *(KYString*)((TKYAVLTree::TNode*)ANode)->Item; }
   T*             Value(void* ANode) const
                  { return (T*)((TKYAVLTree::TNode*)ANode)->Data; }
   void           SetValue(void* ANode, T* AValue)
                  { ((TKYAVLTree::TNode*)ANode)->Data = (void*)AValue; }

   // �жϼ�ֵ�����Ƿ����
   bool           Existed(void* ANode) const
                  { return FTree->Existed((TKYAVLTree::TNode*)ANode); }

   // �жϼ�ֵ�Ƿ����
   bool           Existed(const KYString& AKey) const
                  { return FTree->Existed((void*)&AKey, NULL); }

   // ���Ҽ�ֵ��
   bool           Find(const KYString& AKey, T* &AValue) const
                  {
                     void* pKey;
                     return FTree->Search(pKey, (Pointer&)AValue, &AKey, NULL);
                  }

   // ���Ҽ�ֵ��, ������ֵΪ NULL ��δ�ҵ���ֵ, ���򷵻��ҵ��ļ�ֵ����
   void*          Find(const KYString& AKey) const
                  { return (void*)FTree->Search(&AKey, NULL); }

   // �������һ����ֵ����
   // �� ANearest ���� NULL ���ʾ��ֵ�������һ������ֵ;
   // ������ֵΪ true, ���ʾ�ҵ���ֵ����, �����ֵ�� ANearest ��ֵ����֮ǰ
   bool           FindNearest(const KYString& AKey, Pointer& ANearest) const
                  { return FTree->FindNearest(&AKey, NULL, (TKYAVLTree::PNode&)ANearest); }

   // ��Ӽ�ֵ��, ������ֵΪ NULL �����ʧ��, ���򷵻ؼ���ļ�ֵ����
   void*          Add(const KYString& AKey, T* AValue);

   // ɾ����ֵ��
   bool           Delete(const KYString& AKey)
                  { return FTree->Delete((void*)&AKey, NULL); }

   // ɾ����ֵ����
   bool           Remove(void* ANode)  { return FTree->Remove((TKYAVLTree::TNode*)ANode); }

   // ������м�ֵ����
   void           Clear()              { FTree->Clear(); }

   // ȡ��һ����ֵ����
   void*          First() const        { return (void*)FTree->Next(NULL); }

   // ȡ���һ����ֵ����
   void*          Last() const         { return (void*)FTree->Prior(NULL); }

   // ȡ��һ��ֵ����, ��ANode == NULL, ��ȡ��һ����ֵ����
   void*          Next(void* ANode) const
                  { return (void*)FTree->Next((TKYAVLTree::TNode*)ANode); }

   // ȡ��һ��ֵ����, ��ANode == NULL, ��ȡ���һ����ֵ����
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

// FTree �� OnCompare �¼�����
template <class T>
void TKYMapStrKey<T>::DoCompare(const TKYAVLTree::TNode* ANode1,
                                const TKYAVLTree::TNode* ANode2, long& ACompare)
{
   ACompare = CompareStr(*(KYString*)ANode1->Item,
                         *(KYString*)ANode2->Item, FCaseSensitive);
}

// FTree �� OnDeletion �¼�����
template <class T>
void TKYMapStrKey<T>::DoDeletion(void* Sender, TKYAVLTree::TNode* ANode)
{
   // �ͷż�ֵ
   delete (KYString*)ANode->Item;

   // �ж��Ƿ���Ҫ�ͷ�
   if (FNeedFree)
      delete (T*)ANode->Data;
}

// ��Ӽ�ֵ��, ������ֵΪ NULL �����ʧ��, ���򷵻ؼ���ļ�ֵ����
template <class T>
void* TKYMapStrKey<T>::Add(const KYString& AKey, T* AValue)
{
   // ����
   KYString*          pKey    = new KYString(AKey);
   TKYAVLTree::TNode* result  = FTree->Add((void*)pKey, (void*)AValue);

   // ������ʧ�����ͷ�
   if (result == NULL)
      delete pKey;

   // ���ؽ��
   return (void*)result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMapObjKey - ָ���ֵ map ��ģ�� */

template <class T>
class TKYMapObjKey
{
public:
   // ���캯��
   // 1. ANeedFree      ��ֵ�Ƿ���Ҫ�ͷ�, ��Ϊ false ���ͷ�
   // 2. ACanLock       �Ƿ�������, Ϊ�˶��̴߳�ȡ��ȫ, Ҳ�����ⲿʹ��������
   TKYMapObjKey(bool ANeedFree = false, bool ACanLock = false)
   {
      // ���� AVL ��
      FTree = new TKYAVLTree(ACanLock, false);

      // �ж��Ƿ���Ҫ�ͷ�
      if (ANeedFree)
      {
         FTree->OnDeletion.Object   = this;
         FTree->OnDeletion.Method   = (TKYAVLTree::TDoDeletion)
                                      &TKYMapObjKey<T>::DoDeletion;
      }
   }

   // ��������
   ~TKYMapObjKey()                     { FreeAndNil(FTree); }

   // ����
   void*          Data() const         { return FTree->Data(); }           // default: NULL
   long           Count() const        { return FTree->Count(); }          // default: 0
   long           MaxCount() const     { return FTree->MaxCount(); }       // default: 0
   bool           CanDuplicate() const { return FTree->CanDuplicate(); }   // default: false

   // ��������
   void           SetData(void* AData) { FTree->SetData(AData); }
   void           SetMaxCount(long AMaxCount)
                  { FTree->SetMaxCount(AMaxCount); }
   void           SetCanDuplicate(bool ACanDuplicate)
                  { FTree->SetCanDuplicate(ACanDuplicate); }

   // ��ֵ�����ȡֵ������ֵ, ע: ����� ANode �Ƿ�Ϸ�
   void*          Key(void* ANode) const
                  { return ((TKYAVLTree::TNode*)ANode)->Item; }
   T*             Value(void* ANode) const
                  { return (T*)((TKYAVLTree::TNode*)ANode)->Data; }
   void           SetValue(void* ANode, T* AValue)
                  { ((TKYAVLTree::TNode*)ANode)->Data = (void*)AValue; }

   // �жϼ�ֵ�����Ƿ����
   bool           NodeExisted(void* ANode) const
                  { return FTree->Existed((TKYAVLTree::TNode*)ANode); }

   // �жϼ�ֵ�Ƿ����
   bool           Existed(void* AKey) const
                  { return FTree->Existed(AKey, NULL); }

   // ���Ҽ�ֵ��
   bool           Find(void* AKey, T* &AValue) const
                  { return FTree->Search(AKey, (Pointer&)AValue, AKey, NULL); }

   // ���Ҽ�ֵ��, ������ֵΪ NULL ��δ�ҵ���ֵ, ���򷵻��ҵ��ļ�ֵ����
   void*          Find(void* AKey) const
                  { return (void*)FTree->Search(AKey, NULL); }

   // �������һ����ֵ����
   // �� ANearest ���� NULL ���ʾ��ֵ�������һ������ֵ;
   // ������ֵΪ true, ���ʾ�ҵ���ֵ����, �����ֵ�� ANearest ��ֵ����֮ǰ
   bool           FindNearest(void* AKey, Pointer& ANearest) const
                  { return FTree->FindNearest(AKey, NULL, (TKYAVLTree::PNode&)ANearest); }

   // ��Ӽ�ֵ��, ������ֵΪ NULL �����ʧ��, ���򷵻ؼ���ļ�ֵ����
   void*          Add(void* AKey, T* AValue)
                  { return (void*)FTree->Add(AKey, (void*)AValue); }

   // ɾ����ֵ��
   bool           Delete(void* AKey)   { return FTree->Delete(AKey, NULL); }

   // ɾ����ֵ����
   bool           Remove(void* ANode)  { return FTree->Remove((TKYAVLTree::TNode*)ANode); }

   // ������м�ֵ����
   void           Clear()              { FTree->Clear(); }

   // ȡ��һ����ֵ����
   void*          First() const        { return (void*)FTree->Next(NULL); }

   // ȡ���һ����ֵ����
   void*          Last() const         { return (void*)FTree->Prior(NULL); }

   // ȡ��һ��ֵ����, ��ANode == NULL, ��ȡ��һ����ֵ����
   void*          Next(void* ANode) const
                  { return (void*)FTree->Next((TKYAVLTree::TNode*)ANode); }

   // ȡ��һ��ֵ����, ��ANode == NULL, ��ȡ���һ����ֵ����
   void*          Prior(void* ANode) const
                  { return (void*)FTree->Prior((TKYAVLTree::TNode*)ANode); }

private:
   void           DoDeletion(void* Sender, TKYAVLTree::TNode* ANode);

private:
   TKYAVLTree*    FTree;
};

// FTree �� OnDeletion �¼�����
template <class T>
void TKYMapObjKey<T>::DoDeletion(void* Sender, TKYAVLTree::TNode* ANode)
{
   delete (T*)ANode->Data;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMapIntKeyEx - ���ͼ�ֵ map ��ģ�� */

template <class T>
class TKYMapIntKeyEx
{
public:
   // ���캯��
   // 1. ANeedFree      ��ֵ�Ƿ���Ҫ�ͷ�, ��Ϊ false ���ͷ�
   // 2. ACanLock       �Ƿ�������, Ϊ�˶��̴߳�ȡ��ȫ, Ҳ�����ⲿʹ��������
   TKYMapIntKeyEx(bool ANeedFree = false, bool ACanLock = false)
   {
      // ���� AVL ��
      FTree = new TKYAVLTreeEx(ACanLock, false);

      // �ж��Ƿ���Ҫ�ͷ�
      if (ANeedFree)
      {
         FTree->OnDeletion.Object   = this;
         FTree->OnDeletion.Method   = (TKYAVLTreeEx::TDoDeletion)
                                      &TKYMapIntKeyEx<T>::DoDeletion;
      }
   }

   // ��������
   ~TKYMapIntKeyEx()                   { FreeAndNil(FTree); }

   // ����
   void*          Data() const         { return FTree->Data(); }           // default: NULL
   long           Count() const        { return FTree->Count(); }          // default: 0
   bool           Sorted() const       { return FTree->Sorted(); }         // default: true
   long           MaxCount() const     { return FTree->MaxCount(); }       // default: 0
   bool           CanDuplicate() const { return FTree->CanDuplicate(); }   // default: false

   // ��������
   void           SetData(void* AData) { FTree->SetData(AData); }
   void           SetSorted(bool ASorted)
                  { FTree->SetSorted(ASorted); }
   void           SetMaxCount(long AMaxCount)
                  { FTree->SetMaxCount(AMaxCount); }
   void           SetCanDuplicate(bool ACanDuplicate)
                  { FTree->SetCanDuplicate(ACanDuplicate); }

   // ��ֵ�����ȡֵ������ֵ, ע: ����� ANode �Ƿ�Ϸ�
   long           Key(void* ANode) const
                  { return (long)((TKYAVLTreeEx::TNode*)ANode)->Item; }
   T*             Value(void* ANode) const
                  { return (T*)((TKYAVLTreeEx::TNode*)ANode)->Data; }
   void           SetValue(void* ANode, T* AValue)
                  { ((TKYAVLTreeEx::TNode*)ANode)->Data = (void*)AValue; }

   // ����������ȡ��ֵ����, AIndex ȡֵ��Χ: [0..Count()-1], ���Կ��ٶ�λָ���������
   void*          Node(long AIndex) const
                  { return (void*)FTree->Node(AIndex); }

   // ȡ��ֵ�������ڵ�����, ���������򷵻� -1
   long           IndexOf(void* ANode) const
                  { return FTree->IndexOf((TKYAVLTreeEx::TNode*)ANode); }

   // ȡ��ֵ���ڵ�����, ���������򷵻� -1
   long           IndexOf(long AKey) const
                  { return FTree->IndexOf((void*)AKey, NULL); }

   // �жϼ�ֵ�����Ƿ����
   bool           Existed(void* ANode) const
                  { return FTree->Existed((TKYAVLTreeEx::TNode*)ANode); }

   // �жϼ�ֵ�Ƿ����
   bool           Existed(long AKey) const
                  { return FTree->Existed((void*)AKey, NULL); }

   // ��С��������
   void           Sort()               { FTree->Sort(); }

   // ���Ҽ�ֵ��
   bool           Find(long AKey, T* &AValue) const
                  { return FTree->Search((Pointer&)AKey, (Pointer&)AValue,
                                         (void*)AKey, NULL); }

   // ���Ҽ�ֵ��, ������ֵΪ NULL ��δ�ҵ���ֵ, ���򷵻��ҵ��ļ�ֵ����
   void*          Find(long AKey) const
                  { return (void*)FTree->Search((void*)AKey, NULL); }

   // �������һ����ֵ����
   // ��δ������ ANearest ���ǲ��ҽ��, Ҳ����˵, ��δ�ҵ��򷵻� NULL;
   // �� ANearest ���� NULL ���ʾ��ֵ�������һ������ֵ;
   // ������ֵΪ true, ���ʾ�ҵ���ֵ����, �����ֵ�� ANearest ��ֵ����֮ǰ
   bool           FindNearest(long AKey, Pointer& ANearest) const
                  { return FTree->FindNearest((void*)AKey, NULL, (TKYAVLTreeEx::PNode&)ANearest); }

   // ��Ӽ�ֵ��, ������ֵΪ NULL �����ʧ��, ���򷵻ؼ���ļ�ֵ����
   void*          Add(long AKey, T* AValue)
                  { return (void*)FTree->Add((void*)AKey, (void*)AValue); }

   // ɾ����ֵ��
   bool           Delete(long AKey)    { return FTree->Delete((void*)AKey, NULL); }

   // ɾ����ֵ����
   bool           Remove(void* ANode)  { return FTree->Remove((TKYAVLTreeEx::TNode*)ANode); }

   // ������м�ֵ����
   void           Clear()              { FTree->Clear(); }

   // ȡ��һ����ֵ����
   void*          First() const        { return (void*)FTree->Next(NULL); }

   // ȡ���һ����ֵ����
   void*          Last() const         { return (void*)FTree->Prior(NULL); }

   // ȡ��һ��ֵ����, ��ANode == NULL, ��ȡ��һ����ֵ����
   void*          Next(void* ANode) const
                  { return (void*)FTree->Next((TKYAVLTreeEx::TNode*)ANode); }

   // ȡ��һ��ֵ����, ��ANode == NULL, ��ȡ���һ����ֵ����
   void*          Prior(void* ANode) const
                  { return (void*)FTree->Prior((TKYAVLTreeEx::TNode*)ANode); }

private:
   void           DoDeletion(void* Sender, TKYAVLTreeEx::TNode* ANode);

private:
   TKYAVLTreeEx*  FTree;
};

// FTree �� OnDeletion �¼�����
template <class T>
void TKYMapIntKeyEx<T>::DoDeletion(void* Sender, TKYAVLTreeEx::TNode* ANode)
{
   delete (T*)ANode->Data;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMapStrKeyEx - �ַ�����ֵ map ��ģ�� */

template <class T>
class TKYMapStrKeyEx
{
public:
   // ���캯��
   // 1. ACaseSensitive ��ֵ�Ƿ����ִ�Сд, ��Ϊ false �����ִ�Сд
   // 2. ANeedFree      ��ֵ�Ƿ���Ҫ�ͷ�, ��Ϊ false ���ͷ�
   // 3. ACanLock       �Ƿ�������, Ϊ�˶��̴߳�ȡ��ȫ, Ҳ�����ⲿʹ��������
   TKYMapStrKeyEx(bool ACaseSensitive = false, bool ANeedFree = false,
                  bool ACanLock       = false)
   {
      // ��ʼ��
      FNeedFree                  = ANeedFree;
      FCaseSensitive             = ACaseSensitive;

      // ���� AVL ��
      FTree                      = new TKYAVLTreeEx(ACanLock, false);

      // ���� FTree ���¼�����
      FTree->OnCompare.Object    = this;
      FTree->OnCompare.Method    = (TKYAVLTreeEx::TDoCompare)
                                   &TKYMapStrKeyEx<T>::DoCompare;
      FTree->OnDeletion.Object   = this;
      FTree->OnDeletion.Method   = (TKYAVLTreeEx::TDoDeletion)
                                   &TKYMapStrKeyEx<T>::DoDeletion;
   }

   // ��������
   ~TKYMapStrKeyEx()                   { FreeAndNil(FTree); }

   // ����
   void*          Data() const         { return FTree->Data(); }           // default: NULL
   long           Count() const        { return FTree->Count(); }          // default: 0
   bool           Sorted() const       { return FTree->Sorted(); }         // default: true
   long           MaxCount() const     { return FTree->MaxCount(); }       // default: 0
   bool           CanDuplicate() const { return FTree->CanDuplicate(); }   // default: false
   bool           CaseSensitive() const{ return FCaseSensitive; }          // default: false

   // ��������
   void           SetData(void* AData) { FTree->SetData(AData); }
   void           SetSorted(bool ASorted)
                  { FTree->SetSorted(ASorted); }
   void           SetMaxCount(long AMaxCount)
                  { FTree->SetMaxCount(AMaxCount); }
   void           SetCanDuplicate(bool ACanDuplicate)
                  { FTree->SetCanDuplicate(ACanDuplicate); }

   // ��ֵ�����ȡֵ������ֵ, ע: ����� ANode �Ƿ�Ϸ�
   KYString       Key(void* ANode) const
                  { return *(KYString*)((TKYAVLTreeEx::TNode*)ANode)->Item; }
   T*             Value(void* ANode) const
                  { return (T*)((TKYAVLTreeEx::TNode*)ANode)->Data; }
   void           SetValue(void* ANode, T* AValue)
                  { ((TKYAVLTreeEx::TNode*)ANode)->Data = (void*)AValue; }

   // ����������ȡ��ֵ����, AIndex ȡֵ��Χ: [0..Count()-1], ���Կ��ٶ�λָ���������
   void*          Node(long AIndex) const
                  { return (void*)FTree->Node(AIndex); }

   // ȡ��ֵ�������ڵ�����, ���������򷵻� -1
   long           IndexOf(void* ANode) const
                  { return FTree->IndexOf((TKYAVLTreeEx::TNode*)ANode); }

   // ȡ��ֵ���ڵ�����, ���������򷵻� -1
   long           IndexOf(const KYString& AKey) const
                  { return FTree->IndexOf((void*)&AKey, NULL); }

   // �жϼ�ֵ�����Ƿ����
   bool           Existed(void* ANode) const
                  { return FTree->Existed((TKYAVLTreeEx::TNode*)ANode); }

   // �жϼ�ֵ�Ƿ����
   bool           Existed(const KYString& AKey) const
                  { return FTree->Existed((void*)&AKey, NULL); }

   // ��С��������
   void           Sort()               { FTree->Sort(); }

   // ���Ҽ�ֵ��
   bool           Find(const KYString& AKey, T* &AValue) const
                  {
                     void* pKey;
                     return FTree->Search(pKey, (Pointer&)AValue, &AKey, NULL);
                  }

   // ���Ҽ�ֵ��, ������ֵΪ NULL ��δ�ҵ���ֵ, ���򷵻��ҵ��ļ�ֵ����
   void*          Find(const KYString& AKey) const
                  { return (void*)FTree->Search(&AKey, NULL); }

   // �������һ����ֵ����
   // ��δ������ ANearest ���ǲ��ҽ��, Ҳ����˵, ��δ�ҵ��򷵻� NULL;
   // �� ANearest ���� NULL ���ʾ��ֵ�������һ������ֵ;
   // ������ֵΪ true, ���ʾ�ҵ���ֵ����, �����ֵ�� ANearest ��ֵ����֮ǰ
   bool           FindNearest(const KYString& AKey, Pointer& ANearest) const
                  { return FTree->FindNearest(&AKey, NULL, (TKYAVLTreeEx::PNode&)ANearest); }

   // ��Ӽ�ֵ��, ������ֵΪ NULL �����ʧ��, ���򷵻ؼ���ļ�ֵ����
   void*          Add(const KYString& AKey, T* AValue);

   // ɾ����ֵ��
   bool           Delete(const KYString& AKey)
                  { return FTree->Delete((void*)&AKey, NULL); }

   // ɾ����ֵ����
   bool           Remove(void* ANode)  { return FTree->Remove((TKYAVLTreeEx::TNode*)ANode); }

   // ������м�ֵ����
   void           Clear()              { FTree->Clear(); }

   // ȡ��һ����ֵ����
   void*          First() const        { return (void*)FTree->Next(NULL); }

   // ȡ���һ����ֵ����
   void*          Last() const         { return (void*)FTree->Prior(NULL); }

   // ȡ��һ��ֵ����, ��ANode == NULL, ��ȡ��һ����ֵ����
   void*          Next(void* ANode) const
                  { return (void*)FTree->Next((TKYAVLTreeEx::TNode*)ANode); }

   // ȡ��һ��ֵ����, ��ANode == NULL, ��ȡ���һ����ֵ����
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

// FTree �� OnCompare �¼�����
template <class T>
void TKYMapStrKeyEx<T>::DoCompare(const TKYAVLTreeEx::TNode* ANode1,
                                  const TKYAVLTreeEx::TNode* ANode2, long& ACompare)
{
   ACompare = CompareStr(*(KYString*)ANode1->Item,
                         *(KYString*)ANode2->Item, FCaseSensitive);
}

// FTree �� OnDeletion �¼�����
template <class T>
void TKYMapStrKeyEx<T>::DoDeletion(void* Sender, TKYAVLTreeEx::TNode* ANode)
{
   // �ͷż�ֵ
   delete (KYString*)ANode->Item;

   // �ж��Ƿ���Ҫ�ͷ�
   if (FNeedFree)
      delete (T*)ANode->Data;
}

// ��Ӽ�ֵ��, ������ֵΪ NULL �����ʧ��, ���򷵻ؼ���ļ�ֵ����
template <class T>
void* TKYMapStrKeyEx<T>::Add(const KYString& AKey, T* AValue)
{
   // ����
   KYString*            pKey     = new KYString(AKey);
   TKYAVLTreeEx::TNode* result   = FTree->Add((void*)pKey, (void*)AValue);

   // ������ʧ�����ͷ�
   if (result == NULL)
      delete pKey;

   // ���ؽ��
   return (void*)result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMapObjKeyEx - ָ���ֵ map ��ģ�� */

template <class T>
class TKYMapObjKeyEx
{
public:
   // ���캯��
   // 1. ANeedFree      ��ֵ�Ƿ���Ҫ�ͷ�, ��Ϊ false ���ͷ�
   // 2. ACanLock       �Ƿ�������, Ϊ�˶��̴߳�ȡ��ȫ, Ҳ�����ⲿʹ��������
   TKYMapObjKeyEx(bool ANeedFree = false, bool ACanLock = false)
   {
      // ���� AVL ��
      FTree = new TKYAVLTreeEx(ACanLock, false);

      // �ж��Ƿ���Ҫ�ͷ�
      if (ANeedFree)
      {
         FTree->OnDeletion.Object   = this;
         FTree->OnDeletion.Method   = (TKYAVLTreeEx::TDoDeletion)
                                      &TKYMapObjKeyEx<T>::DoDeletion;
      }
   }

   // ��������
   ~TKYMapObjKeyEx()                   { FreeAndNil(FTree); }

   // ����
   void*          Data() const         { return FTree->Data(); }           // default: NULL
   long           Count() const        { return FTree->Count(); }          // default: 0
   bool           Sorted() const       { return FTree->Sorted(); }         // default: true
   long           MaxCount() const     { return FTree->MaxCount(); }       // default: 0
   bool           CanDuplicate() const { return FTree->CanDuplicate(); }   // default: false

   // ��������
   void           SetData(void* AData) { FTree->SetData(AData); }
   void           SetSorted(bool ASorted)
                  { FTree->SetSorted(ASorted); }
   void           SetMaxCount(long AMaxCount)
                  { FTree->SetMaxCount(AMaxCount); }
   void           SetCanDuplicate(bool ACanDuplicate)
                  { FTree->SetCanDuplicate(ACanDuplicate); }

   // ��ֵ�����ȡֵ������ֵ, ע: ����� ANode �Ƿ�Ϸ�
   void*          Key(void* ANode) const
                  { return ((TKYAVLTreeEx::TNode*)ANode)->Item; }
   T*             Value(void* ANode) const
                  { return (T*)((TKYAVLTreeEx::TNode*)ANode)->Data; }
   void           SetValue(void* ANode, T* AValue)
                  { ((TKYAVLTreeEx::TNode*)ANode)->Data = (void*)AValue; }

   // ����������ȡ��ֵ����, AIndex ȡֵ��Χ: [0..Count()-1], ���Կ��ٶ�λָ���������
   void*          Node(long AIndex) const
                  { return (void*)FTree->Node(AIndex); }

   // ȡ��ֵ�������ڵ�����, ���������򷵻� -1
   long           NodeIndexOf(void* ANode) const
                  { return FTree->IndexOf((TKYAVLTreeEx::TNode*)ANode); }

   // �жϼ�ֵ�����Ƿ����
   bool           NodeExisted(void* ANode) const
                  { return FTree->Existed((TKYAVLTreeEx::TNode*)ANode); }

   // ȡ��ֵ���ڵ�����, ���������򷵻� -1
   long           IndexOf(void* AKey) const
                  { return FTree->IndexOf(AKey, NULL); }

   // �жϼ�ֵ�Ƿ����
   bool           Existed(void* AKey) const
                  { return FTree->Existed(AKey, NULL); }

   // ��С��������
   void           Sort()               { FTree->Sort(); }

   // ���Ҽ�ֵ��
   bool           Find(void* AKey, T* &AValue) const
                  { return FTree->Search(AKey, (Pointer&)AValue, AKey, NULL); }

   // ���Ҽ�ֵ��, ������ֵΪ NULL ��δ�ҵ���ֵ, ���򷵻��ҵ��ļ�ֵ����
   void*          Find(void* AKey) const
                  { return (void*)FTree->Search(AKey, NULL); }

   // �������һ����ֵ����
   // ��δ������ ANearest ���ǲ��ҽ��, Ҳ����˵, ��δ�ҵ��򷵻� NULL;
   // �� ANearest ���� NULL ���ʾ��ֵ�������һ������ֵ;
   // ������ֵΪ true, ���ʾ�ҵ���ֵ����, �����ֵ�� ANearest ��ֵ����֮ǰ
   bool           FindNearest(void* AKey, Pointer& ANearest) const
                  { return FTree->FindNearest(AKey, NULL, (TKYAVLTreeEx::PNode&)ANearest); }

   // ��Ӽ�ֵ��, ������ֵΪ NULL �����ʧ��, ���򷵻ؼ���ļ�ֵ����
   void*          Add(void* AKey, T* AValue)
                  { return (void*)FTree->Add(AKey, (void*)AValue); }

   // ɾ����ֵ��
   bool           Delete(void* AKey)   { return FTree->Delete(AKey, NULL); }

   // ɾ����ֵ����
   bool           Remove(void* ANode)  { return FTree->Remove((TKYAVLTreeEx::TNode*)ANode); }

   // ������м�ֵ����
   void           Clear()              { FTree->Clear(); }

   // ȡ��һ����ֵ����
   void*          First() const        { return (void*)FTree->Next(NULL); }

   // ȡ���һ����ֵ����
   void*          Last() const         { return (void*)FTree->Prior(NULL); }

   // ȡ��һ��ֵ����, ��ANode == NULL, ��ȡ��һ����ֵ����
   void*          Next(void* ANode) const
                  { return (void*)FTree->Next((TKYAVLTreeEx::TNode*)ANode); }

   // ȡ��һ��ֵ����, ��ANode == NULL, ��ȡ���һ����ֵ����
   void*          Prior(void* ANode) const
                  { return (void*)FTree->Prior((TKYAVLTreeEx::TNode*)ANode); }

private:
   void           DoDeletion(void* Sender, TKYAVLTreeEx::TNode* ANode);

private:
   TKYAVLTreeEx*  FTree;
};

// FTree �� OnDeletion �¼�����
template <class T>
void TKYMapObjKeyEx<T>::DoDeletion(void* Sender, TKYAVLTreeEx::TNode* ANode)
{
   delete (T*)ANode->Data;
}

}

#endif
