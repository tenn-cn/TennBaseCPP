// =======================================
// Unit   : 列表类 (KYList.h)
// Version: 3.0.0.0 (build 2012.06.28)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYList_H_
#define _KYList_H_

#include "KYString.h"
#include "KYSyncObj.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYList - 列表类 {建议不要继承, 若要继承请使用 TKYCustomList} */

class TKYList
{
public:
   // TOnCompare 项比较事件
   // 1. 若 AItem1 等于 AItem2 则 ACompare == 0
   // 2. 若 AItem1 大于 AItem2 则 ACompare > 0
   // 3. 若 AItem1 小于 AItem2 则 ACompare < 0
   typedef void (TObject::*TDoCompare)(const void* AItem1,
                                       const void* AItem2, int& ACompare);
   typedef struct
   {
      TDoCompare  Method;
      void*       Object;
   } TOnCompare;

   // TOnDeletion 项删除事件
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

   // 拷贝列表, 同时也复制 OnCompare 事件方法, 并清空 OnDeletion 事件方法
   TKYList&       operator=(const TKYList& AList);

   // 根据索引读取项, AIndex 取值范围: [0..Count()-1]
   void*          operator[](long AIndex) const { return Item(AIndex); }
   void*          Item(long AIndex) const;

   // 属性
   long           Capacity() const        { return FCapacity; }      // default: 0
   long           Count() const           { return FCount; }         // default: 0
   long           Delta() const           { return FDelta; }         // default: Delta_Auto
   bool           Sorted() const          { return FSorted; }        // default: false
   bool           CanDuplicate() const    { return FCanDuplicate; }  // default: true

   // 设置属性
   bool           SetItem(long AIndex, void* AItem);
   void           SetDelta(long ADelta);
   void           SetSorted(bool ASorted);
   void           SetCanDuplicate(bool ACanDuplicate);

   // 拷贝列表, 同时也复制 OnCompare 事件方法, 并清空 OnDeletion 事件方法
   void           Assign(const TKYList& AList);

   // 更改容量, 若 Count() 大于更改的容量值则自动释放项, 并激发 OnDeletion 事件
   void           ChangeCapacity(long ACapacity);

   // 添加项
   // 1. 若 Sorted 属性为 false 则添加到末尾项, 否则根据值大小插入到指定位置
   // 2. 若返回值 >= 0, 则添加项的索引, 否则返回添加失败的错误码
   long           Add(void* AItem);

   // 插入指定索引项
   // 1. 若 Sorted 属性为 true 则不允许调用 Insert 方法, 必须调用 Add 方法添加项
   // 2. 若 AIndex <= 0 则插入第一项, 若 AIndex >= Count() 则插入末尾项
   bool           Insert(long AIndex, void* AItem);

   // 删除指定索引项, 激发 OnDeletion 事件
   void           Delete(long AIndex);

   // 删除<满足条件>的第一项, 激发 OnDeletion 事件
   void           Delete(void* AItem);

   // 删除<项指针相同>的第一项, 并返回删除前的索引, 激发 OnDeletion 事件
   long           Remove(void* AItem);

   // 清除所有项, 激发 OnDeletion 事件
   void           Clear();

   // 根据 OnCompare 从小到大排序
   // 若要反向排序, 则只要在 OnCompare 的方法返回值 ACompare = -ACompare 即可
   void           Sort();

   // 移动索引 AIndex 项到 ANewIndex 索引, 若 Sorted 属性为 true 则失败
   bool           MoveTo(long AIndex, long ANewIndex);

   // 交换索引 AIndex1 和 AIndex2 项, 若 Sorted 属性为 true 则失败
   bool           Exchange(long AIndex1, long AIndex2);

   // 搜索值为 AItem 的 Compare(...) == 0 第一项的索引
   long           IndexOf(const void* AItem) const;

   // 从 AFrom 索引开始搜索值为 AItem 的 Compare(...) == 0 相同项数
   long           SameCount(const void* AItem, long AFrom = 0) const;

   // 从 AFrom 索引开始搜索值为 AItem 的 Compare(...) == 0 第一项的索引
   long           Search(const void* AItem, long AFrom = 0) const;
   long           Search(const void* AItem, Pointer& ARetItem, long AFrom = 0) const;

   // 查找最近一个索引
   // 1. 若 ANearest == -1 则表示未排序, 查找失败;
   // 2. 若返回值为 true, 则表示找到项索引, 否则项值在 ANearest 索引之前
   bool           FindNearest(const void* AItem, long& ANearest, long AFrom = 0) const;

   // 事件
   TOnCompare     OnCompare;
   TOnDeletion    OnDeletion;

protected:
   void           LockRead() const        { if (FLockRW != NULL) FLockRW->LockRead(); }
   void           UnlockRead() const      { if (FLockRW != NULL) FLockRW->UnlockRead(); }

   void           LockWrite() const       { if (FLockRW != NULL) FLockRW->LockWrite(); }
   void           UnlockWrite() const     { if (FLockRW != NULL) FLockRW->UnlockWrite(); }

   // FindItem 已排序列表二分查找, FindByOrder 是逐项顺序查找
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
   TKYLockRW*     FLockRW;             // 读写锁
   Pointer*       FItems;              // 项列表

   long           FCapacity;           // 列表容量
   long           FCount;              // 项个数
   long           FDelta;              // 容量的自动增量

   bool           FSorted;             // 是否排序, 默认值为 false
   bool           FHasSorted;          // 是否已排序, 默认值为 false
   bool           FCanDuplicate;       // 是否允许重复, 默认值为 true
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYStringList - 字符串列表类 {建议不要继承, 若要继承请使用 TKYCustomList} */

class TKYStringList
{
protected:
   // 字符串列表项
   typedef struct
   {
      KYString*   Item;
      void*       Data;
   } TItem, *PItem;

public:
   // TOnCompare 项比较事件
   // 1. 若 AItem1 等于 AItem2 则 ACompare == 0
   // 2. 若 AItem1 大于 AItem2 则 ACompare > 0
   // 3. 若 AItem1 小于 AItem2 则 ACompare < 0
   typedef void (TObject::*TDoCompare)(const KYString& AItem1, const void* AData1,
                                       const KYString& AItem2, const void* AData2,
                                        bool ACaseSensitive,   int& ACompare);
   typedef struct
   {
      TDoCompare  Method;
      void*       Object;
   } TOnCompare;

   // TOnDeletion 项删除事件
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

   // 拷贝字符串列表, 同时也复制 OnCompare 事件方法, 并清空 OnDeletion 事件方法
   TKYStringList& operator=(const TKYStringList& AList);

   // 根据索引读取字符串项, AIndex 取值范围: [0..Count()-1]
   KYString       operator[](long AIndex) const { return Item(AIndex); }

   // 根据索引读取字符串项, AIndex 取值范围: [0..Count()-1]
   KYString       Item(long AIndex) const;

   // 根据索引读取数据项, AIndex 取值范围: [0..Count()-1]
   void*          Data(long AIndex) const;

   // 取字符串列表的文本内容, 字符串项之间使用 ASeparateStr 分隔
   KYString       Text(const char* ASeparateStr, long ALength) const;
   KYString       Text(const char* ASeparateStr = "\r\n") const
                  { return Text(ASeparateStr, Length(ASeparateStr)); }
   KYString       Text(const KYString& ASeparateStr) const
                  { return Text((char*)ASeparateStr, ASeparateStr.Length()); }

   // 取索引的字符串项名称
   // 1. <字符串项> = <Name> + '=' + <Value>
   // 2. 若没有 '=' 分隔符则 <Name> = <字符串项>
   KYString       Name(long AIndex) const;

   // 查找指定项名称的值, <字符串项> = <Name> + '=' + <Value>
   KYString       Value(const KYString& AName) const;

   // 属性
   long           Capacity() const     { return FCapacity; }      // default: 0
   long           Count() const        { return FCount; }         // default: 0
   long           Delta() const        { return FDelta; }         // default: Delta_Auto
   bool           Sorted() const       { return FSorted; }        // default: false
   bool           CanDuplicate() const { return FCanDuplicate; }  // default: true
   bool           CaseSensitive() const{ return FCaseSensitive; } // default: false

   // 设置属性
   void           SetDelta(long ADelta);
   void           SetSorted(bool ASorted);
   void           SetCanDuplicate(bool ACanDuplicate);
   void           SetCaseSensitive(bool ACaseSensitive);

   // 指定索引的字符串项赋值
   bool           SetItem(long AIndex, const KYString& AItem);

   // 指定索引的数据项赋值
   bool           SetData(long AIndex, void* AData);

   // 指定索引的字符串和数据项赋值
   bool           SetItemData(long AIndex, const KYString& AItem, void* AData);

   // 设置字符串列表的文本内容, 按 "\r\n" 或 "\r" 或 "\n" 分隔符分割字符串项
   void           SetText(const char* AText);
   void           SetText(const char* AText, long ALength);
   void           SetText(const KYString& AText)
                  { SetText((char*)AText, AText.Length()); }

   // 设置字符串列表的文本内容, 字符串项之间使用 ASeparateStr 分隔
   void           SetText(const char* AText, long ALength, const KYString& ASeparateStr);
   void           SetText(const char* AText, const KYString& ASeparateStr)
                  { SetText(AText, Length(AText), ASeparateStr); }
   void           SetText(const KYString& AText, const KYString& ASeparateStr)
                  { SetText((char*)AText, AText.Length(), ASeparateStr); }

   // 设置指定项名称的值, <字符串项> = <Name> + '=' + <Value>
   // 1. 若 AName 的项存在, 则更新值
   // 2. 若 Sorted 属性为 false, 则添加到末尾, 否则插入列表中
   void           SetValue(const KYString& AName, const KYString& AValue);

   // 比较列表的所有字符串项是否相同
   bool           Equals(const TKYStringList& AList) const;

   // 拷贝字符串列表, 同时也复制 OnCompare 事件方法, 并清空 OnDeletion 事件方法
   void           Assign(const TKYStringList& AList);

   // 添加字符串列表 AList 的所有项
   // 若 Sorted 属性为 false, 则整个列表添加到末尾, 否则插入列表中
   void           AddList(const TKYStringList& AList);

   // 更改容量, 若 Count() 大于更改的容量值则自动释放项, 并激发 OnDeletion 事件
   void           ChangeCapacity(long ACapacity);

   // 添加项
   // 1. 若 Sorted 属性为 false 则添加到末尾项, 否则根据值大小插入到指定位置
   // 2. 若返回值 >= 0, 则添加项的索引, 否则返回添加失败的错误码
   long           Add(const KYString& AItem, void* AData = NULL);

   // 插入指定索引项
   // 1. 若 Sorted 属性为 true 则不允许调用 Insert 方法, 必须调用 Add 方法添加项
   // 2. 若 AIndex <= 0 则插入第一项, 若 AIndex >= Count() 则插入末尾项
   bool           Insert(long AIndex, const KYString& AItem, void* AData = NULL);

   // 删除指定索引项, 激发 OnDeletion 事件
   void           Delete(long AIndex);

   // 删除<满足条件>的第一项, 激发 OnDeletion 事件
   void           Delete(const KYString& AItem, void* AData = NULL);

   // 清除所有项, 激发 OnDeletion 事件
   void           Clear();

   // 根据 OnCompare 从小到大排序
   // 若要反向排序, 则只要在 OnCompare 的方法返回值 ACompare = -ACompare 即可
   void           Sort();

   // 移动索引 AIndex 项到 ANewIndex 索引, 若 Sorted 属性为 true 则失败
   bool           MoveTo(long AIndex, long ANewIndex);

   // 交换索引 AIndex1 和 AIndex2 项, 若 Sorted 属性为 true 则失败
   bool           Exchange(long AIndex1, long AIndex2);

   // 查找指定项名称的索引, <字符串项> = <Name> + '=' + <Value>
   long           IndexOfName(const KYString& AName) const;

   // 搜索值为 AItem 和 Data 的 Compare(...) == 0 第一项的索引
   long           IndexOf(const KYString& AItem, const void* AData = NULL) const;

   // 从 AFrom 索引开始搜索值为 AItem 和 Data 的 Compare(...) == 0 相同项数
   long           SameCount(const KYString& AItem, const void* AData = NULL,
                                                          long AFrom = 0) const;

   // 从 AFrom 索引开始搜索值为 AItem 和 Data 的 Compare(...) == 0 第一项的索引
   long           Search(const KYString& AItem, const void* AData = NULL,
                                                       long AFrom = 0) const;
   long           Search(const KYString& AItem, const void* AData,
                                Pointer& ARetData, long AFrom = 0) const;

   // 查找最近一个索引
   // 1. 若 ANearest == -1 则表示未排序, 查找失败;
   // 2. 若返回值为 true, 则表示找到项索引, 否则项值在 ANearest 索引之前
   bool           FindNearest(const KYString& AItem, const void* AData,
                                        long& ANearest, long AFrom = 0) const;

   // 读取文件内容并添加到字符串列表, 字符串项之间使用 ASeparateStr 分隔,
   // 若 ASeparateStr == "", 则按 "\r\n" 或 "\r" 或 "\n" 分隔符分割字符串项
   bool           LoadFromFile(const KYString& AFileName,
                               const KYString& ASeparateStr = "");

   // 保存字符串列表的文本内容到文件, 字符串项之间使用 ASeparateStr 分隔
   bool           SaveToFile(const KYString& AFileName,
                             const KYString& ASeparateStr = "\r\n");

   // 事件
   TOnCompare     OnCompare;
   TOnDeletion    OnDeletion;

protected:
   void           LockRead() const     { if (FLockRW != NULL) FLockRW->LockRead(); }
   void           UnlockRead() const   { if (FLockRW != NULL) FLockRW->UnlockRead(); }

   void           LockWrite() const    { if (FLockRW != NULL) FLockRW->LockWrite(); }
   void           UnlockWrite() const  { if (FLockRW != NULL) FLockRW->UnlockWrite(); }

   // FindItem 已排序列表二分查找, FindByOrder 是逐项顺序查找
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
   TKYLockRW*     FLockRW;             // 读写锁
   TItem*         FItems;              // 项列表

   long           FCapacity;           // 列表容量
   long           FCount;              // 项个数
   long           FDelta;              // 容量的自动增量

   bool           FSorted;             // 是否排序, 默认值为 false
   bool           FHasSorted;          // 是否已排序, 默认值为 false
   bool           FCanDuplicate;       // 是否允许重复, 默认值为 true
   bool           FCaseSensitive;      // 是否区分大小写, 默认值为 false
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYCustomList - 通用列表类 */

class TKYCustomList
{
public:
   TKYCustomList(long AItemSize = CustomItem_DefSize, bool ACanLock = false);
   virtual ~TKYCustomList();

   // 属性
   long           Capacity() const        { return FCapacity; }   // default: 0
   long           Count() const           { return FCount; }      // default: 0
   long           Delta() const           { return FDelta; }      // default: Delta_Auto

   // 设置列表容量的自动增量
   void           SetDelta(long ADelta);

   // 更改容量, 若 Count() 大于更改的容量值则自动释放项, 并调用 DoDeletion 方法
   void           ChangeCapacity(long ACapacity);

   // 删除指定索引项, 并调用 DoDeletion 方法
   void           Delete(long AIndex);

   // 清除所有项, 并调用 DoDeletion 方法
   void           Clear();

   // 移动索引 AIndex 项到 ANewIndex 索引, 若 Sorted 属性为 true 则失败
   bool           MoveTo(long AIndex, long ANewIndex);

   // 交换索引 AIndex1 和 AIndex2 项, 若 Sorted 属性为 true 则失败
   bool           Exchange(long AIndex1, long AIndex2);

protected:
   // 比较和删除为纯虚函数, 必须要重载
   virtual int    Compare(const void* AItem1, const void* AItem2) const = 0;
   virtual void   DoDeletion(void* AItem) = 0;

   // 读锁
   void           LockRead() const        { if (FLockRW != NULL) FLockRW->LockRead(); }
   void           UnlockRead() const      { if (FLockRW != NULL) FLockRW->UnlockRead(); }

   // 写锁
   void           LockWrite() const       { if (FLockRW != NULL) FLockRW->LockWrite(); }
   void           UnlockWrite() const     { if (FLockRW != NULL) FLockRW->UnlockWrite(); }

   // 项指针
   void*          _Item(long AIndex) const{ return ((char*)FItems + FItemSize * AIndex); }
   long           _ItemMaxCount() const   { return (Item_MaxCount << 2) / FItemSize; }
   long           _ItemSize() const       { return FItemSize; }

   // _SetSorted 和 _Sort 函数都没有写加锁
   bool           _Sorted() const         { return FSorted; }
   bool           _HasSorted() const      { return FSorted || FHasSorted; }
   void           _ResetHasSorted()       { FHasSorted = false; }
   void           _SetSorted(bool ASorted);
   void           _Sort();

   // FCanDuplicate 与排序有关, 若不排序则此属性无效
   bool           _CanDuplicate() const                { return FCanDuplicate; }
   void           _SetCanDuplicate(bool ACanDuplicate) { FCanDuplicate = ACanDuplicate; }

   // FindItem 已排序列表二分查找, FindByOrder 是逐项顺序查找
   bool           FindItem(long AFrom, long ATo, const void* AItem, long& ANearest) const;
   long           FindByOrder(long AFrom, long ATo, const void* AItem) const;

   // 插入/更新项
   long           InsertItem(long AIndex, const void* AItem);
   bool           UpdateItem(long AIndex, const void* AItem);

   // 清除/释放项
   void           ClearItems();
   void           FreeItems(long AFrom, long ACount);

   // 设置容量
   bool           SetCapacity(long ACapacity);

private:
   bool           Grow();
   void           QuickSort(long AFrom, long ATo);

private:
   TKYLockRW*     FLockRW;             // 读写锁
   void*          FItems;              // 项列表
   long           FItemSize;           // 项尺寸
   long           FCapacity;           // 列表容量
   long           FCount;              // 项个数
   long           FDelta;              // 容量的自动增量

   bool           FSorted;             // 是否排序, 默认值为 false
   bool           FHasSorted;          // 是否已排序, 默认值为 false
   bool           FCanDuplicate;       // 是否允许重复, 默认值为 true
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYDataList - 数据列表类 */

class TKYDataList: public TKYCustomList
{
public:
   // TOnDeletion 项删除事件
   typedef void (TObject::*TDoDeletion)(void* Sender, void* AData);
   typedef struct
   {
      TDoDeletion Method;
      void*       Object;
   } TOnDeletion;

public:
   TKYDataList(long AItemSize = CustomItem_DefSize, bool ACanLock = false);
   virtual ~TKYDataList();

   // 根据索引读取项, AIndex 取值范围: [0..Count()-1]
   void*          operator[](long AIndex) const { return Item(AIndex); }

   // 根据索引读取项, AIndex 取值范围: [0..Count()-1]
   void*          Item(long AIndex) const;

   // 项尺寸
   long           ItemSize() const              { return _ItemSize(); }

   // 添加项, 由于 Sorted 属性为 false, 所以添加到末尾项
   // 若返回值 >= 0, 则添加项的索引, 否则返回添加失败的错误码
   long           Add(const void* AItem);

   // 插入指定索引项
   // 若 AIndex <= 0 则插入第一项, 若 AIndex >= Count() 则插入末尾项
   bool           Insert(long AIndex, const void* AItem);

   // 事件
   TOnDeletion    OnDeletion;

protected:
   // 比较和删除为纯虚函数, 必须要重载
   virtual int    Compare(const void* AItem1, const void* AItem2) const;
   virtual void   DoDeletion(void* AItem);

private:
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYNameStrList - 名字字符串列表类 */

class TKYNameStrList: public TKYCustomList
{
protected:
   // 名字字符串列表项
   typedef struct
   {
      KYString*   Name;
      KYString*   Item;
      void*       Data;
   } TItem, *PItem;

public:
   // TOnDeletion 项删除事件
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

   // 拷贝列表, 并清空 OnDeletion 事件方法
   TKYNameStrList& operator=(const TKYNameStrList& AList);

   // 属性
   // 1. _SetCanDuplicate(false);
   bool           Sorted() const          { return _Sorted(); }      // default: true
   bool           CaseSensitive() const   { return FCaseSensitive; } // default: false

   // 根据索引读取名字, AIndex 取值范围: [0..Count()-1]
   KYString       Name(long AIndex) const;

   // 根据索引读取字符串项, AIndex 取值范围: [0..Count()-1]
   KYString       Item(long AIndex) const;

   // 根据索引读取数据串项, AIndex 取值范围: [0..Count()-1]
   void*          Data(long AIndex) const;

   // 根据索引读取字符串和数据项, AIndex 取值范围: [0..Count()-1]
   void*          ItemData(long AIndex, KYString& AItem) const;

   // 取名字所在索引, 若不存在则返回 -1
   long           IndexOf(const KYString& AName) const;

   // 根据名字读取字符串项, 若不存在则返回空串
   KYString       Item(const KYString& AName) const;

   // 根据名字读取数据项, 若不存在则返回 NULL
   void*          Data(const KYString& AName) const;

   // 根据名字读取字符串和数据项, 若不存在则返回 NULL
   void*          ItemData(const KYString& AName, KYString& AItem) const;

   // 设置是否按名字排序
   void           SetSorted(bool ASorted);

   // 设置名字是否区分大小写
   void           SetCaseSensitive(bool ACaseSensitive);

   // 设置索引的字符串项, AIndex 取值范围: [0..Count()-1]
   bool           SetItem(long AIndex, const KYString& AItem);

   // 设置索引的数据项, AIndex 取值范围: [0..Count()-1]
   bool           SetData(long AIndex, void* AData);

   // 设置索引的字符串和数据项, AIndex 取值范围: [0..Count()-1]
   bool           SetItemData(long AIndex, const KYString& AItem, void* AData);

   // 排序
   void           Sort();

   // 拷贝列表, 并清空 OnDeletion 事件方法
   void           Assign(const TKYNameStrList& AList);

   // 添加名字项
   // 1. 若 Sorted 属性为 false 则添加到末尾项, 否则根据名字大小插入到指定位置
   // 2. 若返回值 >= 0, 则添加项的索引, 否则返回添加失败的错误码
   long           Add(const KYString& AName, const KYString& AItem = "",
                                                       void* AData = NULL);

   // 插入名字项
   // 1. 若 Sorted 属性为 true 则不允许调用 Insert 方法, 必须调用 Add 方法添加项
   // 2. 若 AIndex <= 0 则插入第一项, 若 AIndex >= Count() 则插入末尾项
   bool           Insert(long AIndex, const KYString& AName,
                                      const KYString& AItem = "",
                                                void* AData = NULL);

   // 删除名字项, 激发 OnDeletion 事件
   void           Delete(const KYString& AName);

   // 删除指定索引项, 激发 OnDeletion 事件
   using          TKYCustomList::Delete;

   // 事件
   TOnDeletion    OnDeletion;

protected:
   // 比较和删除为纯虚函数, 必须要重载
   virtual int    Compare(const void* AItem1, const void* AItem2) const;
   virtual void   DoDeletion(void* AItem);

   long           _IndexOf(const KYString& AName) const;
   PItem          _ItemOfName(const KYString& AName) const;

private:
   bool           FCaseSensitive;      // 是否区分大小写, 默认值为 false
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYCode2Name - 编码转换成名字类 */

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

   // 属性
   // 1. _SetSorted(true);
   // 2. _SetCanDuplicate(false);

   // 根据索引读取编码, AIndex 取值范围: [0..Count()-1]
   long           Code(long AIndex) const;

   // 根据索引读取名字, AIndex 取值范围: [0..Count()-1]
   KYString       Name(long AIndex) const;

   // 添加名字项
   // 1. 由于 Sorted 属性为 true, 所以根据编码大小插入到指定位置
   // 2. 若返回值 >= 0, 则添加项的索引, 否则返回添加失败的错误码
   long           Add(long ACode, const KYString& AName);

   // 删除指定编码项
   void           Remove(long ACode);

   // 取编码的名称
   KYString       NameOf(long ACode) const;

   // 取编码的索引
   long           IndexOf(long ACode) const;

   // 判断编码是否存在
   bool           Existed(long ACode) const  { return (IndexOf(ACode) >= 0); }

protected:
   // 比较和删除为纯虚函数, 必须要重载
   virtual int    Compare(const void* AItem1, const void* AItem2) const;
   virtual void   DoDeletion(void* AItem);

   long           _IndexOf(long ACode) const;
   PItem          _ItemOf(long ACode) const;
};

}

#endif
