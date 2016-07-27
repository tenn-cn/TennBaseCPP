// =======================================
// Unit   : �б��� (KYList.h)
// Version: 3.0.0.0 (build 2012.06.28)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYList_H_
#define _KYList_H_

#include "KYString.h"
#include "KYSyncObj.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYList - �б��� {���鲻Ҫ�̳�, ��Ҫ�̳���ʹ�� TKYCustomList} */

class TKYList
{
public:
   // TOnCompare ��Ƚ��¼�
   // 1. �� AItem1 ���� AItem2 �� ACompare == 0
   // 2. �� AItem1 ���� AItem2 �� ACompare > 0
   // 3. �� AItem1 С�� AItem2 �� ACompare < 0
   typedef void (TObject::*TDoCompare)(const void* AItem1,
                                       const void* AItem2, int& ACompare);
   typedef struct
   {
      TDoCompare  Method;
      void*       Object;
   } TOnCompare;

   // TOnDeletion ��ɾ���¼�
   typedef void (TObject::*TDoDeletion)(void* AItem);
   typedef struct
   {
      TDoDeletion Method;
      void*       Object;
   } TOnDeletion;

public:
   TKYList(bool ACanLock = false);
   TKYList(const TKYList& AList);
   virtual ~TKYList();

   // �����б�, ͬʱҲ���� OnCompare �¼�����, ����� OnDeletion �¼�����
   TKYList&       operator=(const TKYList& AList);

   // ����������ȡ��, AIndex ȡֵ��Χ: [0..Count()-1]
   void*          operator[](long AIndex) const { return Item(AIndex); }
   void*          Item(long AIndex) const;

   // ����
   long           Capacity() const        { return FCapacity; }      // default: 0
   long           Count() const           { return FCount; }         // default: 0
   long           Delta() const           { return FDelta; }         // default: Delta_Auto
   bool           Sorted() const          { return FSorted; }        // default: false
   bool           CanDuplicate() const    { return FCanDuplicate; }  // default: true

   // ��������
   bool           SetItem(long AIndex, void* AItem);
   void           SetDelta(long ADelta);
   void           SetSorted(bool ASorted);
   void           SetCanDuplicate(bool ACanDuplicate);

   // �����б�, ͬʱҲ���� OnCompare �¼�����, ����� OnDeletion �¼�����
   void           Assign(const TKYList& AList);

   // ��������, �� Count() ���ڸ��ĵ�����ֵ���Զ��ͷ���, ������ OnDeletion �¼�
   void           ChangeCapacity(long ACapacity);

   // �����
   // 1. �� Sorted ����Ϊ false ����ӵ�ĩβ��, �������ֵ��С���뵽ָ��λ��
   // 2. ������ֵ >= 0, ������������, ���򷵻����ʧ�ܵĴ�����
   long           Add(void* AItem);

   // ����ָ��������
   // 1. �� Sorted ����Ϊ true ��������� Insert ����, ������� Add ���������
   // 2. �� AIndex <= 0 ������һ��, �� AIndex >= Count() �����ĩβ��
   bool           Insert(long AIndex, void* AItem);

   // ɾ��ָ��������, ���� OnDeletion �¼�
   void           Delete(long AIndex);

   // ɾ��<��������>�ĵ�һ��, ���� OnDeletion �¼�
   void           Delete(void* AItem);

   // ɾ��<��ָ����ͬ>�ĵ�һ��, ������ɾ��ǰ������, ���� OnDeletion �¼�
   long           Remove(void* AItem);

   // ���������, ���� OnDeletion �¼�
   void           Clear();

   // ���� OnCompare ��С��������
   // ��Ҫ��������, ��ֻҪ�� OnCompare �ķ�������ֵ ACompare = -ACompare ����
   void           Sort();

   // �ƶ����� AIndex � ANewIndex ����, �� Sorted ����Ϊ true ��ʧ��
   bool           MoveTo(long AIndex, long ANewIndex);

   // �������� AIndex1 �� AIndex2 ��, �� Sorted ����Ϊ true ��ʧ��
   bool           Exchange(long AIndex1, long AIndex2);

   // ����ֵΪ AItem �� Compare(...) == 0 ��һ�������
   long           IndexOf(const void* AItem) const;

   // �� AFrom ������ʼ����ֵΪ AItem �� Compare(...) == 0 ��ͬ����
   long           SameCount(const void* AItem, long AFrom = 0) const;

   // �� AFrom ������ʼ����ֵΪ AItem �� Compare(...) == 0 ��һ�������
   long           Search(const void* AItem, long AFrom = 0) const;
   long           Search(const void* AItem, Pointer& ARetItem, long AFrom = 0) const;

   // �������һ������
   // 1. �� ANearest == -1 ���ʾδ����, ����ʧ��;
   // 2. ������ֵΪ true, ���ʾ�ҵ�������, ������ֵ�� ANearest ����֮ǰ
   bool           FindNearest(const void* AItem, long& ANearest, long AFrom = 0) const;

   // �¼�
   TOnCompare     OnCompare;
   TOnDeletion    OnDeletion;

protected:
   void           LockRead() const        { if (FLockRW != NULL) FLockRW->LockRead(); }
   void           UnlockRead() const      { if (FLockRW != NULL) FLockRW->UnlockRead(); }

   void           LockWrite() const       { if (FLockRW != NULL) FLockRW->LockWrite(); }
   void           UnlockWrite() const     { if (FLockRW != NULL) FLockRW->UnlockWrite(); }

   // FindItem �������б���ֲ���, FindByOrder ������˳�����
   int            Compare(const void* AItem1, const void* AItem2) const;
   bool           FindItem(long AFrom, long ATo, const void* AItem, long& ANearest) const;
   long           FindByOrder(long AFrom, long ATo, const void* AItem) const;

   long           InsertItem(long AIndex, void* AItem);
   bool           UpdateItem(long AIndex, void* AItem);

   void           ClearItems();
   void           FreeItems(long AFrom, long ACount);
   void           QuickSort(long AFrom, long ATo);

   bool           SetCapacity(long ACapacity);

private:
   bool           Grow();

   // OnCompare.Method != NULL
   bool           DoFindItem(long AFrom, long ATo, const void* AItem, long& ANearest) const;
   inline int     DoCompare(const void* AItem1, const void* AItem2) const;

private:
   TKYLockRW*     FLockRW;             // ��д��
   Pointer*       FItems;              // ���б�

   long           FCapacity;           // �б�����
   long           FCount;              // �����
   long           FDelta;              // �������Զ�����

   bool           FSorted;             // �Ƿ�����, Ĭ��ֵΪ false
   bool           FHasSorted;          // �Ƿ�������, Ĭ��ֵΪ false
   bool           FCanDuplicate;       // �Ƿ������ظ�, Ĭ��ֵΪ true
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYStringList - �ַ����б��� {���鲻Ҫ�̳�, ��Ҫ�̳���ʹ�� TKYCustomList} */

class TKYStringList
{
protected:
   // �ַ����б���
   typedef struct
   {
      KYString*   Item;
      void*       Data;
   } TItem, *PItem;

public:
   // TOnCompare ��Ƚ��¼�
   // 1. �� AItem1 ���� AItem2 �� ACompare == 0
   // 2. �� AItem1 ���� AItem2 �� ACompare > 0
   // 3. �� AItem1 С�� AItem2 �� ACompare < 0
   typedef void (TObject::*TDoCompare)(const KYString& AItem1, const void* AData1,
                                       const KYString& AItem2, const void* AData2,
                                        bool ACaseSensitive,   int& ACompare);
   typedef struct
   {
      TDoCompare  Method;
      void*       Object;
   } TOnCompare;

   // TOnDeletion ��ɾ���¼�
   typedef void (TObject::*TDoDeletion)(const KYString& AItem, void* AData);
   typedef struct
   {
      TDoDeletion Method;
      void*       Object;
   } TOnDeletion;

public:
   TKYStringList(bool ACanLock = false);
   TKYStringList(const TKYStringList& AList);
   virtual ~TKYStringList();

   // �����ַ����б�, ͬʱҲ���� OnCompare �¼�����, ����� OnDeletion �¼�����
   TKYStringList& operator=(const TKYStringList& AList);

   // ����������ȡ�ַ�����, AIndex ȡֵ��Χ: [0..Count()-1]
   KYString       operator[](long AIndex) const { return Item(AIndex); }

   // ����������ȡ�ַ�����, AIndex ȡֵ��Χ: [0..Count()-1]
   KYString       Item(long AIndex) const;

   // ����������ȡ������, AIndex ȡֵ��Χ: [0..Count()-1]
   void*          Data(long AIndex) const;

   // ȡ�ַ����б���ı�����, �ַ�����֮��ʹ�� ASeparateStr �ָ�
   KYString       Text(const char* ASeparateStr, long ALength) const;
   KYString       Text(const char* ASeparateStr = "\r\n") const
                  { return Text(ASeparateStr, Length(ASeparateStr)); }
   KYString       Text(const KYString& ASeparateStr) const
                  { return Text((char*)ASeparateStr, ASeparateStr.Length()); }

   // ȡ�������ַ���������
   // 1. <�ַ�����> = <Name> + '=' + <Value>
   // 2. ��û�� '=' �ָ����� <Name> = <�ַ�����>
   KYString       Name(long AIndex) const;

   // ����ָ�������Ƶ�ֵ, <�ַ�����> = <Name> + '=' + <Value>
   KYString       Value(const KYString& AName) const;

   // ����
   long           Capacity() const     { return FCapacity; }      // default: 0
   long           Count() const        { return FCount; }         // default: 0
   long           Delta() const        { return FDelta; }         // default: Delta_Auto
   bool           Sorted() const       { return FSorted; }        // default: false
   bool           CanDuplicate() const { return FCanDuplicate; }  // default: true
   bool           CaseSensitive() const{ return FCaseSensitive; } // default: false

   // ��������
   void           SetDelta(long ADelta);
   void           SetSorted(bool ASorted);
   void           SetCanDuplicate(bool ACanDuplicate);
   void           SetCaseSensitive(bool ACaseSensitive);

   // ָ���������ַ����ֵ
   bool           SetItem(long AIndex, const KYString& AItem);

   // ָ�������������ֵ
   bool           SetData(long AIndex, void* AData);

   // ָ���������ַ����������ֵ
   bool           SetItemData(long AIndex, const KYString& AItem, void* AData);

   // �����ַ����б���ı�����, �� "\r\n" �� "\r" �� "\n" �ָ����ָ��ַ�����
   void           SetText(const char* AText);
   void           SetText(const char* AText, long ALength);
   void           SetText(const KYString& AText)
                  { SetText((char*)AText, AText.Length()); }

   // �����ַ����б���ı�����, �ַ�����֮��ʹ�� ASeparateStr �ָ�
   void           SetText(const char* AText, long ALength, const KYString& ASeparateStr);
   void           SetText(const char* AText, const KYString& ASeparateStr)
                  { SetText(AText, Length(AText), ASeparateStr); }
   void           SetText(const KYString& AText, const KYString& ASeparateStr)
                  { SetText((char*)AText, AText.Length(), ASeparateStr); }

   // ����ָ�������Ƶ�ֵ, <�ַ�����> = <Name> + '=' + <Value>
   // 1. �� AName �������, �����ֵ
   // 2. �� Sorted ����Ϊ false, ����ӵ�ĩβ, ��������б���
   void           SetValue(const KYString& AName, const KYString& AValue);

   // �Ƚ��б�������ַ������Ƿ���ͬ
   bool           Equals(const TKYStringList& AList) const;

   // �����ַ����б�, ͬʱҲ���� OnCompare �¼�����, ����� OnDeletion �¼�����
   void           Assign(const TKYStringList& AList);

   // ����ַ����б� AList ��������
   // �� Sorted ����Ϊ false, �������б���ӵ�ĩβ, ��������б���
   void           AddList(const TKYStringList& AList);

   // ��������, �� Count() ���ڸ��ĵ�����ֵ���Զ��ͷ���, ������ OnDeletion �¼�
   void           ChangeCapacity(long ACapacity);

   // �����
   // 1. �� Sorted ����Ϊ false ����ӵ�ĩβ��, �������ֵ��С���뵽ָ��λ��
   // 2. ������ֵ >= 0, ������������, ���򷵻����ʧ�ܵĴ�����
   long           Add(const KYString& AItem, void* AData = NULL);

   // ����ָ��������
   // 1. �� Sorted ����Ϊ true ��������� Insert ����, ������� Add ���������
   // 2. �� AIndex <= 0 ������һ��, �� AIndex >= Count() �����ĩβ��
   bool           Insert(long AIndex, const KYString& AItem, void* AData = NULL);

   // ɾ��ָ��������, ���� OnDeletion �¼�
   void           Delete(long AIndex);

   // ɾ��<��������>�ĵ�һ��, ���� OnDeletion �¼�
   void           Delete(const KYString& AItem, void* AData = NULL);

   // ���������, ���� OnDeletion �¼�
   void           Clear();

   // ���� OnCompare ��С��������
   // ��Ҫ��������, ��ֻҪ�� OnCompare �ķ�������ֵ ACompare = -ACompare ����
   void           Sort();

   // �ƶ����� AIndex � ANewIndex ����, �� Sorted ����Ϊ true ��ʧ��
   bool           MoveTo(long AIndex, long ANewIndex);

   // �������� AIndex1 �� AIndex2 ��, �� Sorted ����Ϊ true ��ʧ��
   bool           Exchange(long AIndex1, long AIndex2);

   // ����ָ�������Ƶ�����, <�ַ�����> = <Name> + '=' + <Value>
   long           IndexOfName(const KYString& AName) const;

   // ����ֵΪ AItem �� Data �� Compare(...) == 0 ��һ�������
   long           IndexOf(const KYString& AItem, const void* AData = NULL) const;

   // �� AFrom ������ʼ����ֵΪ AItem �� Data �� Compare(...) == 0 ��ͬ����
   long           SameCount(const KYString& AItem, const void* AData = NULL,
                                                          long AFrom = 0) const;

   // �� AFrom ������ʼ����ֵΪ AItem �� Data �� Compare(...) == 0 ��һ�������
   long           Search(const KYString& AItem, const void* AData = NULL,
                                                       long AFrom = 0) const;
   long           Search(const KYString& AItem, const void* AData,
                                Pointer& ARetData, long AFrom = 0) const;

   // �������һ������
   // 1. �� ANearest == -1 ���ʾδ����, ����ʧ��;
   // 2. ������ֵΪ true, ���ʾ�ҵ�������, ������ֵ�� ANearest ����֮ǰ
   bool           FindNearest(const KYString& AItem, const void* AData,
                                        long& ANearest, long AFrom = 0) const;

   // ��ȡ�ļ����ݲ���ӵ��ַ����б�, �ַ�����֮��ʹ�� ASeparateStr �ָ�,
   // �� ASeparateStr == "", �� "\r\n" �� "\r" �� "\n" �ָ����ָ��ַ�����
   bool           LoadFromFile(const KYString& AFileName,
                               const KYString& ASeparateStr = "");

   // �����ַ����б���ı����ݵ��ļ�, �ַ�����֮��ʹ�� ASeparateStr �ָ�
   bool           SaveToFile(const KYString& AFileName,
                             const KYString& ASeparateStr = "\r\n");

   // �¼�
   TOnCompare     OnCompare;
   TOnDeletion    OnDeletion;

protected:
   void           LockRead() const     { if (FLockRW != NULL) FLockRW->LockRead(); }
   void           UnlockRead() const   { if (FLockRW != NULL) FLockRW->UnlockRead(); }

   void           LockWrite() const    { if (FLockRW != NULL) FLockRW->LockWrite(); }
   void           UnlockWrite() const  { if (FLockRW != NULL) FLockRW->UnlockWrite(); }

   // FindItem �������б���ֲ���, FindByOrder ������˳�����
   int            Compare(const TItem& AItem1, const TItem& AItem2) const;
   bool           FindItem(long AFrom, long ATo, const TItem& AItem, long& ANearest) const;
   long           FindByOrder(long AFrom, long ATo, const TItem& AItem) const;
   long           FindByName(long AFrom, long ATo, const KYString& AName) const;

   long           InsertItem(long AIndex, const TItem& AItem, bool AIsNew);
   bool           UpdateItem(long AIndex, const KYString& AItem,
                            void* AData, bool AIsItem, bool AIsData);

   void           ClearItems();
   void           FreeItems(long AFrom, long ACount);
   void           QuickSort(long AFrom, long ATo);

   bool           SetCapacity(long ACapacity);

private:
   bool           Grow();

   // OnCompare.Method != NULL
   bool           DoFindItem(long AFrom, long ATo, const TItem& AItem, long& ANearest) const;
   inline int     DoCompare(const TItem& AItem1, const TItem& AItem2) const;

private:
   TKYLockRW*     FLockRW;             // ��д��
   TItem*         FItems;              // ���б�

   long           FCapacity;           // �б�����
   long           FCount;              // �����
   long           FDelta;              // �������Զ�����

   bool           FSorted;             // �Ƿ�����, Ĭ��ֵΪ false
   bool           FHasSorted;          // �Ƿ�������, Ĭ��ֵΪ false
   bool           FCanDuplicate;       // �Ƿ������ظ�, Ĭ��ֵΪ true
   bool           FCaseSensitive;      // �Ƿ����ִ�Сд, Ĭ��ֵΪ false
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYCustomList - ͨ���б��� */

class TKYCustomList
{
public:
   TKYCustomList(long AItemSize = CustomItem_DefSize, bool ACanLock = false);
   virtual ~TKYCustomList();

   // ����
   long           Capacity() const        { return FCapacity; }   // default: 0
   long           Count() const           { return FCount; }      // default: 0
   long           Delta() const           { return FDelta; }      // default: Delta_Auto

   // �����б��������Զ�����
   void           SetDelta(long ADelta);

   // ��������, �� Count() ���ڸ��ĵ�����ֵ���Զ��ͷ���, ������ DoDeletion ����
   void           ChangeCapacity(long ACapacity);

   // ɾ��ָ��������, ������ DoDeletion ����
   void           Delete(long AIndex);

   // ���������, ������ DoDeletion ����
   void           Clear();

   // �ƶ����� AIndex � ANewIndex ����, �� Sorted ����Ϊ true ��ʧ��
   bool           MoveTo(long AIndex, long ANewIndex);

   // �������� AIndex1 �� AIndex2 ��, �� Sorted ����Ϊ true ��ʧ��
   bool           Exchange(long AIndex1, long AIndex2);

protected:
   // �ȽϺ�ɾ��Ϊ���麯��, ����Ҫ����
   virtual int    Compare(const void* AItem1, const void* AItem2) const = 0;
   virtual void   DoDeletion(void* AItem) = 0;

   // ����
   void           LockRead() const        { if (FLockRW != NULL) FLockRW->LockRead(); }
   void           UnlockRead() const      { if (FLockRW != NULL) FLockRW->UnlockRead(); }

   // д��
   void           LockWrite() const       { if (FLockRW != NULL) FLockRW->LockWrite(); }
   void           UnlockWrite() const     { if (FLockRW != NULL) FLockRW->UnlockWrite(); }

   // ��ָ��
   void*          _Item(long AIndex) const{ return ((char*)FItems + FItemSize * AIndex); }
   long           _ItemMaxCount() const   { return (Item_MaxCount << 2) / FItemSize; }
   long           _ItemSize() const       { return FItemSize; }

   // _SetSorted �� _Sort ������û��д����
   bool           _Sorted() const         { return FSorted; }
   bool           _HasSorted() const      { return FSorted || FHasSorted; }
   void           _ResetHasSorted()       { FHasSorted = false; }
   void           _SetSorted(bool ASorted);
   void           _Sort();

   // FCanDuplicate �������й�, �����������������Ч
   bool           _CanDuplicate() const                { return FCanDuplicate; }
   void           _SetCanDuplicate(bool ACanDuplicate) { FCanDuplicate = ACanDuplicate; }

   // FindItem �������б���ֲ���, FindByOrder ������˳�����
   bool           FindItem(long AFrom, long ATo, const void* AItem, long& ANearest) const;
   long           FindByOrder(long AFrom, long ATo, const void* AItem) const;

   // ����/������
   long           InsertItem(long AIndex, const void* AItem);
   bool           UpdateItem(long AIndex, const void* AItem);

   // ���/�ͷ���
   void           ClearItems();
   void           FreeItems(long AFrom, long ACount);

   // ��������
   bool           SetCapacity(long ACapacity);

private:
   bool           Grow();
   void           QuickSort(long AFrom, long ATo);

private:
   TKYLockRW*     FLockRW;             // ��д��
   void*          FItems;              // ���б�
   long           FItemSize;           // ��ߴ�
   long           FCapacity;           // �б�����
   long           FCount;              // �����
   long           FDelta;              // �������Զ�����

   bool           FSorted;             // �Ƿ�����, Ĭ��ֵΪ false
   bool           FHasSorted;          // �Ƿ�������, Ĭ��ֵΪ false
   bool           FCanDuplicate;       // �Ƿ������ظ�, Ĭ��ֵΪ true
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYDataList - �����б��� */

class TKYDataList: public TKYCustomList
{
public:
   // TOnDeletion ��ɾ���¼�
   typedef void (TObject::*TDoDeletion)(void* Sender, void* AData);
   typedef struct
   {
      TDoDeletion Method;
      void*       Object;
   } TOnDeletion;

public:
   TKYDataList(long AItemSize = CustomItem_DefSize, bool ACanLock = false);
   virtual ~TKYDataList();

   // ����������ȡ��, AIndex ȡֵ��Χ: [0..Count()-1]
   void*          operator[](long AIndex) const { return Item(AIndex); }

   // ����������ȡ��, AIndex ȡֵ��Χ: [0..Count()-1]
   void*          Item(long AIndex) const;

   // ��ߴ�
   long           ItemSize() const              { return _ItemSize(); }

   // �����, ���� Sorted ����Ϊ false, ������ӵ�ĩβ��
   // ������ֵ >= 0, ������������, ���򷵻����ʧ�ܵĴ�����
   long           Add(const void* AItem);

   // ����ָ��������
   // �� AIndex <= 0 ������һ��, �� AIndex >= Count() �����ĩβ��
   bool           Insert(long AIndex, const void* AItem);

   // �¼�
   TOnDeletion    OnDeletion;

protected:
   // �ȽϺ�ɾ��Ϊ���麯��, ����Ҫ����
   virtual int    Compare(const void* AItem1, const void* AItem2) const;
   virtual void   DoDeletion(void* AItem);

private:
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYNameStrList - �����ַ����б��� */

class TKYNameStrList: public TKYCustomList
{
protected:
   // �����ַ����б���
   typedef struct
   {
      KYString*   Name;
      KYString*   Item;
      void*       Data;
   } TItem, *PItem;

public:
   // TOnDeletion ��ɾ���¼�
   typedef void (TObject::*TDoDeletion)(void* Sender, const KYString& AName,
                                                      const KYString& AItem,
                                                                void* AData);
   typedef struct
   {
      TDoDeletion Method;
      void*       Object;
   } TOnDeletion;

public:
   TKYNameStrList(bool ACanLock = false);
   virtual ~TKYNameStrList();

   // �����б�, ����� OnDeletion �¼�����
   TKYNameStrList& operator=(const TKYNameStrList& AList);

   // ����
   // 1. _SetCanDuplicate(false);
   bool           Sorted() const          { return _Sorted(); }      // default: true
   bool           CaseSensitive() const   { return FCaseSensitive; } // default: false

   // ����������ȡ����, AIndex ȡֵ��Χ: [0..Count()-1]
   KYString       Name(long AIndex) const;

   // ����������ȡ�ַ�����, AIndex ȡֵ��Χ: [0..Count()-1]
   KYString       Item(long AIndex) const;

   // ����������ȡ���ݴ���, AIndex ȡֵ��Χ: [0..Count()-1]
   void*          Data(long AIndex) const;

   // ����������ȡ�ַ�����������, AIndex ȡֵ��Χ: [0..Count()-1]
   void*          ItemData(long AIndex, KYString& AItem) const;

   // ȡ������������, ���������򷵻� -1
   long           IndexOf(const KYString& AName) const;

   // �������ֶ�ȡ�ַ�����, ���������򷵻ؿմ�
   KYString       Item(const KYString& AName) const;

   // �������ֶ�ȡ������, ���������򷵻� NULL
   void*          Data(const KYString& AName) const;

   // �������ֶ�ȡ�ַ�����������, ���������򷵻� NULL
   void*          ItemData(const KYString& AName, KYString& AItem) const;

   // �����Ƿ���������
   void           SetSorted(bool ASorted);

   // ���������Ƿ����ִ�Сд
   void           SetCaseSensitive(bool ACaseSensitive);

   // �����������ַ�����, AIndex ȡֵ��Χ: [0..Count()-1]
   bool           SetItem(long AIndex, const KYString& AItem);

   // ����������������, AIndex ȡֵ��Χ: [0..Count()-1]
   bool           SetData(long AIndex, void* AData);

   // �����������ַ�����������, AIndex ȡֵ��Χ: [0..Count()-1]
   bool           SetItemData(long AIndex, const KYString& AItem, void* AData);

   // ����
   void           Sort();

   // �����б�, ����� OnDeletion �¼�����
   void           Assign(const TKYNameStrList& AList);

   // ���������
   // 1. �� Sorted ����Ϊ false ����ӵ�ĩβ��, ����������ִ�С���뵽ָ��λ��
   // 2. ������ֵ >= 0, ������������, ���򷵻����ʧ�ܵĴ�����
   long           Add(const KYString& AName, const KYString& AItem = "",
                                                       void* AData = NULL);

   // ����������
   // 1. �� Sorted ����Ϊ true ��������� Insert ����, ������� Add ���������
   // 2. �� AIndex <= 0 ������һ��, �� AIndex >= Count() �����ĩβ��
   bool           Insert(long AIndex, const KYString& AName,
                                      const KYString& AItem = "",
                                                void* AData = NULL);

   // ɾ��������, ���� OnDeletion �¼�
   void           Delete(const KYString& AName);

   // ɾ��ָ��������, ���� OnDeletion �¼�
   using          TKYCustomList::Delete;

   // �¼�
   TOnDeletion    OnDeletion;

protected:
   // �ȽϺ�ɾ��Ϊ���麯��, ����Ҫ����
   virtual int    Compare(const void* AItem1, const void* AItem2) const;
   virtual void   DoDeletion(void* AItem);

   long           _IndexOf(const KYString& AName) const;
   PItem          _ItemOfName(const KYString& AName) const;

private:
   bool           FCaseSensitive;      // �Ƿ����ִ�Сд, Ĭ��ֵΪ false
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYCode2Name - ����ת���������� */

class TKYCode2Name: public TKYCustomList
{
protected:
   typedef struct
   {
      long        Code;
      KYString*   Name;
   } TItem, *PItem;

public:
   TKYCode2Name(bool ACanLock = false);
   virtual ~TKYCode2Name();

   // ����
   // 1. _SetSorted(true);
   // 2. _SetCanDuplicate(false);

   // ����������ȡ����, AIndex ȡֵ��Χ: [0..Count()-1]
   long           Code(long AIndex) const;

   // ����������ȡ����, AIndex ȡֵ��Χ: [0..Count()-1]
   KYString       Name(long AIndex) const;

   // ���������
   // 1. ���� Sorted ����Ϊ true, ���Ը��ݱ����С���뵽ָ��λ��
   // 2. ������ֵ >= 0, ������������, ���򷵻����ʧ�ܵĴ�����
   long           Add(long ACode, const KYString& AName);

   // ɾ��ָ��������
   void           Remove(long ACode);

   // ȡ���������
   KYString       NameOf(long ACode) const;

   // ȡ���������
   long           IndexOf(long ACode) const;

   // �жϱ����Ƿ����
   bool           Existed(long ACode) const  { return (IndexOf(ACode) >= 0); }

protected:
   // �ȽϺ�ɾ��Ϊ���麯��, ����Ҫ����
   virtual int    Compare(const void* AItem1, const void* AItem2) const;
   virtual void   DoDeletion(void* AItem);

   long           _IndexOf(long ACode) const;
   PItem          _ItemOf(long ACode) const;
};

}

#endif
