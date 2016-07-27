// =======================================
// Unit   : 索引文件类 (KYIndexFile.h)
// Version: 3.2.0.0 (build 2012.08.16)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYIndexFile_H_
#define _KYIndexFile_H_

#include "KYFields.h"
#include "KYAVLTree.h"
#include "KYMemFile.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

class TKYIndexTree;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYIndexFile - 索引文件类 */

class TKYIndexFile
{
public:
   // 文件类型
   enum TType       {ftMemFile   = 0,     // 内存文件    (TKYMemFile)
                     ftMapFile   = 1,     // 映像文件    (TKYMemFileEx)
                     ftPartFile  = 2};    // 部分内存文件(TKYPartMemFile)

   // 索引文件状态
   enum TState      {fsInactive,          // 未打开
                     fsOpening,           // 正在打开
                     fsClosing,           // 正在关闭
                     fsOpened};           // 已经打开

   // 版本信息
   #pragma pack(push, 1)
   typedef struct
   {
      Byte           Major;               // 主版本
      Byte           Minor;               // 小版本
      Byte           Release;             // 发布小版本
      Byte           Build;               // 编译小版本
   } TVersion, *PVersion;
   #pragma pack(pop)

protected:
   // 文件头信息
   #pragma pack(push, 1)
   typedef struct
   {
      char           Flag[4];             // 文件标记符
      TVersion       Version;             // 索引文件版本
      long           TagSize;             // 附加数据尺寸
      long           TagOrigin;           // 附加数据开始位置
      Word           BlockItems;          // 块的项数[1..65535]
      Word           IndexCount;          // 索引个数[0..65535]
      long           FirstBlock;          // 第一个索引块的位置
      long           LastBlock;           // 最后一个索引块的位置
      long           TailOrigin;          // 文件尾开始位置
   } THead, *PHead;
   #pragma pack(pop)

private:
   friend class TKYIndexTree;

   // TDoClosed 索引文件已关闭的事件方法
   typedef void (TObject::*TDoClosed)(TKYIndexFile* AFile);

   // 方法项的联合类型
   typedef union
   {
      void*          Value;
      TDoClosed      Method;
   } TMethodItem;

public:
   TKYIndexFile(long AType = ftMapFile);
   virtual ~TKYIndexFile();

   // 属性
   void*             Data() const         { return FData; }             // default: NULL
   long              Size() const         { return FFile->Size(); }     // default: 0
   TType             Type() const         { return (TType)FType; }      // derault: ftMapFile
   TState            State() const        { return (TState)FState; }    // default: fsInactive
   TVersion          Version() const      { return FHead.Version; }     // default: 0,0,0,0
   Longword          TagSize() const      { return FHead.TagSize; }     // default: 0
   long              IdleSize();                                        // default: 0
   KYString          FileName() const     { return FFile->FileName(); } // default: ""
   bool              ReadOnly() const     { return FFile->ReadOnly(); } // default: false
   bool              IsEditing() const    { return FIsEditing; }        // default: false
   bool              IsOverflow() const   { return FIsOverflow; }       // default: false
   Word              BlockItems() const   { return FHead.BlockItems; }  // default: 0

   // 取索引树个数及编号对应的索引
   Word              IndexCount();                                      // default: 0
   long              Index(Word AIndexNo);                              // default: 0

   // 设置属性
   void              SetData(void* AData) { FData = AData; }

   // 打开索引文件
   bool              Open(const KYString& AFileName, bool AReadOnly = false,
                                                 Longword ATimeout  = INFINITE);

   // 关闭索引文件
   void              Close(bool ANeedWait = false);

   // 清除缓存到写入文件中
   void              Flush();

   // 预加载索引文件
   // 注: 为了把文件内容确保加载到内存映像中, 加快初次读取文件操作速度, 一般是在
   // 文件打开后调用。ASize <= 0 表示整个文件加载, 也可以预加载文件从头开始到指
   // 定 ASize 部分。
   bool              Preload(long ASize = 0);

   // 读取附加数据: ASize 传入要读取的尺寸, 返回实际读取的尺寸
   bool              ReadTag(long AOffset, void* AData, long& ASize);

   // 保存附加数据: ASize 传入要保存的尺寸, 返回实际保存的尺寸
   bool              WriteTag(long AOffset, const void* AData, long& ASize);

protected:
   // 状态锁
   void              Lock() const         { FLock->Enter(); }
   void              Unlock() const       { FLock->Leave(); }

   // 文件锁
   bool              LockFile(Longword ATimeout = INFINITE) const
                     { return FMutex->Lock(ATimeout); }
   void              UnlockFile() const   { FMutex->Unlock(); }

   // 准备指定文件位置尺寸的数据
   void              Prepare(long AOffset, long ASize)
                     {
                        if (FType == ftPartFile)
                           ((TKYPartMemFile*)FFile)->Prepare(AOffset, ASize);
                     }

   // 偏移量转内存地址(AOffset > 0)
   void*             Offset2Mem(long AOffset) const
                     { return FFile->MemBase() + AOffset; }

   // 写文件数据
   bool              WriteInt(long AOffset, long AValue)
                     { return FFile->Write(AOffset, &AValue, 4); }
   bool              WriteFile(long AOffset, long ASize)
                     { return FFile->Write(AOffset, ASize); }
   bool              WriteFile(long AOffset, const void* ABuffer, long ASize)
                     { return FFile->Write(AOffset, ABuffer, ASize); }

   // 引用计数增减
   bool              IncRefCount(bool ACanEdit = false);
   void              DecRefCount();

   // 开始/结束编辑
   // 1. BeginEdit 必须在创建索引, 添加索引, 修改值前调用否则操作无效, 自动调用
   //              LockFile 进行文件加锁及引用计数加 1.
   // 2. EndEdit   对编辑数据保存到文件或者取消数据编辑, 同时自动调用UnlockFile
   //              进行文件解锁及引用计数减 1.
   bool              BeginEdit(Longword ATimeout = INFINITE);
   void              EndEdit(bool ACancel = false);

   // 添加索引, 若成功则返回索引编号
   bool              AddIndex(long AIndex, Word& AIndexNo);

   // 分配文件空间, 返回偏移量
   long              Assign(long ASize);

   // 重分配偏移量的内存
   void*             Reassign(long AOffset, long ASize);

private:
   void              DoInit();
   bool              DoCheck(const THead& AHead);

   // 执行打开/关闭
   bool              DoOpen(const KYString& AFileName, bool AReadOnly,
                                  Longword  ATimeout);
   void              DoClose();

   // 添加/删除引用的关闭事件方法(注: 为了防止死锁 ADoClosed 内部不能使用锁)
   bool              DoAddRef(void* AObject, TDoClosed ADoClosed);
   void              DoDeleteRef(void* AObject);
   void              DoCloseRefs();

private:
   void*             FData;               // 自定义数据
   TKYCritSect*      FLock;               // 对象锁
   TKYBaseMemFile*   FFile;               // 文件对象
   TKYAVLTree*       FRefs;               // 引用索引的方法列表
   TKYMutex*         FMutex;              // 互斥元
   TKYStringList*    FOffsets;            // 修改内容的偏移量列表
   Longword          FRefCount;           // 引用计数
   long              FLastSize;           // 最后尺寸, 用于临时分配使用

   THead             FHead;               // 文件头信息
   long              FType;               // 文件类型
   Byte              FState;              // 索引文件状态
   bool              FIsEditing;          // 是否正在编辑
   bool              FIsOverflow;         // 是否溢出

public:
   // 创建索引文件(注: 若文件已存在则删除文件并重新创建)
   static bool       CreateFile(const KYString& AFileName, long AInitedSize,
                                          long  ATagSize,  Word ABlockItems);

   // 增长文件尺寸
   static bool       GrowFileSize(const KYString& AFileName, long ANewSize);

public:
   // 静态成员
   static const TVersion _Ver_Current; // 当前版本(1.0.0.0)
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYIndexTree - 索引树类 */
// 注: 为了提高效率内部没有锁, 也就是说, 多线程对同一对象操作不安全。

class TKYIndexTree
{
public:
   // 索引树状态
   enum TState      {tsInactive,          // 未打开
                     tsCreating,          // 正在创建索引
                     tsClosing,           // 正在关闭
                     tsOpening,           // 正在打开
                     tsOpened,            // 已经打开
                     tsEditCond,          // 编辑搜索条件
                     tsSeeking,           // 正在搜索索引
                     tsBrowse,            // 浏览当前索引
                     tsInsert};           // 插入新索引

   // 索引键项定义
   typedef struct
   {
      KYString       Name;                // 键名(Name.Length() <= 255)
      Word           Size;                // 键尺寸(ftString, ftFixedStr, ftFixedSize 才有效否则为 0)
      Byte           Type;                // 键类型(TKYFieldType 类型值)
      Byte           Dimension;           // 结点维数[2..256](Dimension != 0)
      bool           CaseSensitive;       // 区分大小写/整型值符号
                     // CaseSensitive 值针对不同键类型的含义:
                     // -----------------------------------------------------------
                     //  键类型        true              false
                     // -----------------------------------------------------------
                     //  ftString      区分大小写        不区分大小写
                     //  ftFixedStr    区分大小写        不区分大小写
                     //  ftFixedSize   区分大小写        不区分大小写
                     //  ftInteger     有符号4字节整型   无符号4字节整型
                     //  ftInt64       有符号8字节整型   无符号8字节整型
                     //  ftByte        无符号1字节整型   有符号1字节整型
                     // -----------------------------------------------------------
   } TKeyDef, *PKeyDef;

   // 键和值类型
   typedef TKYFieldType          TType;

   // 可变数据
   typedef TKYVarType::TVarData  TVarData;

protected:
   // 索引头结构: {<Head> + <KeyDefs> + <TagData>}
   #pragma pack(push, 1)
   typedef struct
   {
      long           Self;                // 索引项位置
      long           Size;                // 索引头尺寸
      long           Root;                // 根结点的位置
      Word           TagSize;             // 附加数据尺寸
      Byte           KeyCount;            // 键个数[1..256]
      Byte           ValueType;           // 值类型(TKYFieldType 类型值)
      long           ValueSize;           // 值尺寸(ftString, ftFixedStr, ftFixedSize 才有效否则为 0)
      long           TagOffset;           // 附加数据的偏移量
      Word           KeyOffset;           // 键字段定义列表的偏移量
   } THead, *PHead;

   // 结点头信息
   typedef struct
   {
      Byte           Count;               // 项数[1..维数]
      bool           IsLeaf;              // 是否为叶子结点
   } TNodeHead, *PNodeHead;

   // 允许重复的值头信息
   typedef struct
   {
      long           Prior;               // 上一个值偏移量
      long           Next;                // 下一个值偏移量
   } TValHead, *PValHead;
   #pragma pack(pop)

   // 值类型的掩码
   enum TValMask    {vmType   = 0x1F,     // 值类型(TKYFieldType 类型值)的掩码
                     vmMulti  = 0x20};    // 允许多个值即 CanDuplicate 属性为 true

private:
   // 结点分裂操作
   enum TPartOp     {poNew    = 0,        // 新建结点
                     poNext   = 1,        // 结点向后分裂
                     poPrior  = 2};       // 结点向前分裂

   // 条件项
   enum TCondOp     {coNull   = 0x00,     // (空)
                     coEqual  = 0x01,     // 条件等于
                     coFrom   = 0x02,     // 条件从
                     coTo     = 0x04};    // 条件到

   // 键位置项
   typedef struct
   {
      TKYVarType     CondEqual;           // 键值条件等于
      TKYVarType     CondFrom;            // 键值条件从
      TKYVarType     CondTo;              // 键值条件到
      TKYVarType     Key;                 // 键值
      TKeyDef        KeyDef;              // 索引键项定义
      long           ItemSize;            // 结点项尺寸
      long           NodeSize;            // 结点尺寸(<NodeHead> + [<Item> + <Link>])
      long           RefRoot;             // 保存根结点位置的偏移量
      Byte           PreBits;             // 前缀位数(ftString, ftFixedStr 才有效否则为 0)
      Byte           CondOps;             // 键值条件项位值
      bool           IsFalse;             // 条件是否不成立
      bool           Found;               // 是否未搜索到键值
      bool           Bof;                 // 是否在第一条位置
      bool           Eof;                 // 是否在末尾位置
   } TKeyPlace, *PKeyPlace;

public:
   TKYIndexTree();
   virtual ~TKYIndexTree();

   // 属性
   void*             Data() const         { return FData; }                // default: NULL
   TState            State() const        { return (TState)FState; }       // default: tsInactive
   long              Index() const        { return FHead.Self; }           // default: 0
   Word              IndexNo() const      { return FIndexNo; }             // default: 0
   Word              TagSize() const      { return FHead.TagSize; }        // default: 0
   long              KeyCount() const     { return FPlaces->Count(); }     // default: 0
   long              ValueSize() const    { return FHead.ValueSize; }      // default: 0
   TType             ValueType() const    { return (TType)FHead.ValueType;}// default: ftUnknown
   TKYIndexFile*     File() const         { return FClosed ? NULL : FFile;}// default: NULL
   bool              Closed() const       { return FClosed; }              // default: false
   bool              IsOverflow() const   { return (FState >= tsOpened)    // default: false
                                                   && FFile->IsOverflow(); }
   bool              CanDuplicate() const { return FCanDuplicate; }        // default: false

   // 值及索引键值
   TKYVarType*       Value() const        { return FValue; }
   TKYVarType*       Key(long ANo) const  { return (TKYVarType*)FKeys->Item(ANo); }
   TKYVarType*       Key(const KYString& AName) const;
   long              KeyNo(const KYString& AName) const;
   TKeyDef           KeyDef(long ANo) const;

   // 设置属性
   void              SetData(void* AData) { FData = AData; }
   bool              SetTagSize(Word ATagSize);
   bool              SetValueDef(TType AType, long ASize, bool ACanDuplicate = false);

   // 编辑索引键定义(注: AName 不区分大小写且长度超过 255 部分自动截断)
   long              AddKeyDef(Byte ADimension, const KYString& AName, TType AType,
                               Word ASize = 0,  bool ACaseSensitive = true);
   bool              DeleteKeyDef(long ANo);
   bool              DeleteKeyDef(const KYString& AName);
   void              ClearKeyDefs();

   // 创建/打开/关闭索引
   bool              Create(TKYIndexFile* AFile);
   bool              Open(TKYIndexFile* AFile, long AIndex, Word AIndexNo);
   void              Close();

   // 开始/结束插入
   // 1. 开始插入: 设置 tsInsert 状态
   // 2. 结束插入: 提交或取消键和值
   // 3. EndInsert 若 ACancel == true 表示取消编辑, 否则提交键和值
   // 4. EndInsert 若 ACanUpdate == true 表示所有键已存在时更新值
   // 5. EndInsert 若 ACanUpdate == false 表示所有键已存在时提交值失败
   bool              BeginInsert();
   bool              EndInsert(bool ACancel = false, bool ACanUpdate = false);

   // 更新当前索引所在的值
   // 1. 若 State() != tsBrowse 则更新失败
   // 2. 更新部分值时, 类型必须为 ftFixedSize 否则失败
   bool              Update();
   bool              Update(long AOffset, const void* AData, long& ASize);

   // 编辑搜索条件: ftFixedSize 类型使用 TKYVarType::vtString 值
   bool              BeginConds();
   void              CancelConds();
   void              DeleteCond(long AKeyNo);
   bool              AddCondEqual(long AKeyNo, const TKYVarType& ACondition);
   bool              AddCondFrom(long AKeyNo, const TKYVarType& ACondition);
   bool              AddCondTo(long AKeyNo, const TKYVarType& ACondition);

   void              DeleteCond(const KYString& AKey)
                     { DeleteCond(KeyNo(AKey)); }
   bool              AddCondEqual(const KYString& AKey, const TKYVarType& ACondition)
                     { return AddCondEqual(KeyNo(AKey), ACondition); }
   bool              AddCondFrom(const KYString& AKey, const TKYVarType& ACondition)
                     { return AddCondFrom(KeyNo(AKey), ACondition); }
   bool              AddCondTo(const KYString& AKey, const TKYVarType& ACondition)
                     { return AddCondTo(KeyNo(AKey), ACondition); }

   // 按条件搜索
   // 1. Search   返回 true 表示成功且返回第一项值和 State 为 tsBrowse
   // 2. SeekFirst搜索满足条件的第一项值(注: State 必须为 tsBrowse)
   // 3. SeekPrior搜索满足条件的上一项值(注: State 必须为 tsBrowse)
   // 4. SeekNext 搜索满足条件的下一项值(注: State 必须为 tsBrowse)
   // 5. SeekLast 搜索满足条件的最后一项值(注: State 必须为 tsBrowse)
   bool              Search();
   bool              SeekFirst();
   bool              SeekPrior();
   bool              SeekNext();
   bool              SeekLast();

   // 按键号条件搜索(注: State 必须为 tsBrowse)
   bool              SeekFirst(long AKeyNo);
   bool              SeekPrior(long AKeyNo, bool AIncDuplicate = true);
   bool              SeekNext(long AKeyNo, bool AIncDuplicate = true);
   bool              SeekLast(long AKeyNo);

   // 按键名条件搜索(注: State 必须为 tsBrowse)
   bool              SeekFirst(const KYString& AKey)
                     { return SeekFirst(KeyNo(AKey)); }
   bool              SeekPrior(const KYString& AKey, bool AIncDuplicate = true)
                     { return SeekPrior(KeyNo(AKey), AIncDuplicate); }
   bool              SeekNext(const KYString& AKey, bool AIncDuplicate = true)
                     { return SeekNext(KeyNo(AKey), AIncDuplicate); }
   bool              SeekLast(const KYString& AKey)
                     { return SeekLast(KeyNo(AKey)); }

   // 按条件和距离跳转记录
   // (注: State 必须为 tsBrowse, 且 ADistance > 0, 并返回实际跳转的距离)
   // 1. JumpPrior向前跳转指定距离, 若成功则返回值为 true, 且得到记录值,
   //             否则返回值为 false, 且记录值无效, 并返回实际跳转的距离
   // 2. JumpNext 向后跳转指定距离, 若成功则返回值为 true, 且得到记录值,
   //             否则返回值为 false, 且记录值无效, 并返回实际跳转的距离
   bool              JumpPrior(long& ADistance);
   bool              JumpNext(long& ADistance);

   // 读取附加数据: ASize 传入要读取的尺寸, 返回实际读取的尺寸
   bool              ReadTag(long AOffset, void* AData, long& ASize);

   // 保存附加数据: ASize 传入要保存的尺寸, 返回实际保存的尺寸
   bool              WriteTag(long AOffset, const void* AData, long& ASize);

protected:
   // 取名字对应的键号(不检查状态)
   long              KeyNoOf(const KYString& AName) const;

   // 分配键列表
   bool              AssignKeys();
   void              ClearKeyValues();

   // 添加键定义(不检查状态)
   long              DoAddKeyDef(Byte ADimension, const KYString& AName,
                                TType AType, Word ASize,    bool  ACaseSensitive);

   // 存取键定义
   bool              DoLoadKeyDefs(const char* APos, const char* AEnd, long ACount);
   KYString          DoSaveKeyDefs();

private:
   void              DoInit(bool AIncHead);
   void              DoInit(TKeyPlace& APlace, long ARefRoot, bool AIncCond);
   void              DoInitChild(long AKeyNo,   long ARefRoot,
                                 bool AIncCond, bool AIsLast);

   bool              DoCheck(const THead& AHead);
   void              DoClosed(TKYIndexFile* AFile);

   // 分析查找条件是否合法
   void              DoInitConds();
   bool              DoAnalyzeCond(TKeyPlace& APlace);
   bool              DoAnalyzeConds();

   // 查找键值及所在的结点
   bool              DoFind(TKeyPlace& APlace, const TKYVarType& AKey,
                              TKYList& ANodes,          TKYList& AOrderNos,
                                 long& ALink);
   bool              DoFind(TKeyPlace& APlace, const TKYVarType& AKey,
                                 long& ANode,              long& AOrderNo);
   bool              DoFindIn0(TKeyPlace& APlace, const TKYVarType& AFrom,
                        const TKYVarType& ATo,                bool  AIsInc,
                                    long& ANode,              long& AOrderNo);
   bool              DoFindIn1(TKeyPlace& APlace, const TKYVarType& AFrom,
                        const TKYVarType& ATo,                bool  AIsInc,
                                    long& ANode,              long& AOrderNo);
   bool              DoFindTo0(TKeyPlace& APlace,  const TKYVarType& AKey,
                                    bool  AIsInc, long& ANode, long& AOrderNo);
   bool              DoFindTo1(TKeyPlace& APlace,  const TKYVarType& AKey,
                                    bool  AIsInc, long& ANode, long& AOrderNo);
   bool              DoFindFrom0(TKeyPlace& APlace,  const TKYVarType& AKey,
                                      bool  AIsInc, long& ANode, long& AOrderNo);
   bool              DoFindFrom1(TKeyPlace& APlace,  const TKYVarType& AKey,
                                      bool  AIsInc, long& ANode, long& AOrderNo);
   bool              DoGetFirst(TKeyPlace& APlace, long& ANode, long& AOrderNo);
   bool              DoGetLast(TKeyPlace& APlace, long& ANode, long& AOrderNo);

   // 插入相关方法
   void              DoInitKey(long AKeyNo, const TKYVarType& AKey);
   bool              DoAppendItem(long  AHead, Byte ACount,
                        const KYString& AItem, long ALink);
   bool              DoInsertItem(long  AHead, long AInsNo, long ACount,
                        const KYString& AItem, long ALink);
   bool              DoChangeRoot(long ANode);
   bool              DoChangeNode(long  AHead, long AInsNo, long AIndex,
                        const KYString& AItem, long ALink);
   bool              DoAssignNode(long& ANode, PNodeHead& AHead, long ACount);

   bool              DoMinKey(const KYString& AItem, long AIndex);
   bool              DoNewLeaf(long ALink, long& ANode, KYString* AItem);

   TNodeHead*        DoGetSub(long AParent, long AOrderNo);
   TPartOp           DoGetPartOp(long AParent, long AOrderNo, long AInsNo);

   bool              DoInsertNew(long  AParent, long  AOrderNo, long  ACurrent,
                             KYString& AItem,   long& ALink,    long& AInsNo,
                                 bool& ABreak,  long  AIndex);
   bool              DoMergeNext(long  AParent, long  AOrderNo, long  ACurrent,
                             KYString& AItem,   long& ALink,    long& AInsNo,
                                 bool& ABreak,  long  AIndex);
   bool              DoMergePrior(long  AParent, long  AOrderNo, long  ACurrent,
                              KYString& AItem,   long& ALink,    long& AInsNo,
                                  bool& ABreak,  long  AIndex);
   bool              DoMergePriorEx(long  AParent, long  AOrderNo, long  ACurrent,
                                KYString& AItem,   long& ALink,    long& AInsNo,
                               TNodeHead* APrior,  long  APriorPos);

   bool              DoInsertKey(long ALink);
   bool              DoInsertRoot(long  AHead, long AInsNo,
                        const KYString& AItem, long ALink);

   // 插入键和值
   bool              DoInsert(long AKeyNo, const TKYVarType& AKey, long& ALink);
   bool              DoInsertEx(long AKeyNo, const TKYVarType& AKey, bool ACanUpdate);

   // 执行搜索
   bool              DoSeekFirst();
   bool              DoSeekPrior();
   bool              DoSeekNext();
   bool              DoSeekLast();

   // 执行搜索(注: APlace.RefRoot 必须合法)
   bool              DoSeekFirst(TKeyPlace& APlace, long& ALink);
   bool              DoSeekPrior(TKeyPlace& APlace, long& ALink);
   bool              DoSeekNext(TKeyPlace& APlace, long& ALink);
   bool              DoSeekLast(TKeyPlace& APlace, long& ALink);
   bool              DoSeekCurr(TKeyPlace& APlace, long& ARefLink);

   // 循环跳转值(注: FCanDuplicate == true)
   bool              DoJumpPriorValue(long& ANo, long ACount);
   bool              DoJumpNextValue(long& ANo, long ACount);

   // TKYList 的 OnDeletion 事件方法
   void              DoKeyDeletion(TKYVarType* AItem);
   void              DoPlaceDeletion(TKeyPlace* AItem);

private:
   // 不同数据类型的比较方法
   // 若返回值 result == 0 则表示 AItem 等于 ACond
   // 若返回值 result > 0  则表示 AItem 大于 ACond
   // 若返回值 result < 0  则表示 AItem 小于 ACond
   long              DoCompare_1(TKeyPlace& APlace, void* AItem, TVarData* ACond);
   long              DoCompare_2(TKeyPlace& APlace, void* AItem, TVarData* ACond);
   long              DoCompare_3(TKeyPlace& APlace, void* AItem, TVarData* ACond);
   long              DoCompare_4(TKeyPlace& APlace, void* AItem, TVarData* ACond);
   long              DoCompare_5(TKeyPlace& APlace, void* AItem, TVarData* ACond);
   long              DoCompare_6(TKeyPlace& APlace, void* AItem, TVarData* ACond);
   long              DoCompare_7(TKeyPlace& APlace, void* AItem, TVarData* ACond);
   long              DoCompare_8(TKeyPlace& APlace, void* AItem, TVarData* ACond);
   long              DoCompare_9(TKeyPlace& APlace, void* AItem, TVarData* ACond);
   long              DoCompare_10(TKeyPlace& APlace, void* AItem, TVarData* ACond);
   long              DoCompare_11(TKeyPlace& APlace, void* AItem, TVarData* ACond);
   long              DoCompare_12(TKeyPlace& APlace, void* AItem, TVarData* ACond);

   // 读取不同类型键值的方法
   void              DoGetKey_1(TKeyPlace& APlace, void* AItem);
   void              DoGetKey_2(TKeyPlace& APlace, void* AItem);
   void              DoGetKey_3(TKeyPlace& APlace, void* AItem);
   void              DoGetKey_12(TKeyPlace& APlace, void* AItem);

   // 设置不同类型键值的方法
   bool              DoSetKey_1(TKeyPlace& APlace, void* AItem);
   bool              DoSetKey_2(TKeyPlace& APlace, void* AItem);
   bool              DoSetKey_3(TKeyPlace& APlace, void* AItem);
   bool              DoSetKey_12(TKeyPlace& APlace, void* AItem);

   // 读取不同类型值的方法
   void              DoGetValue_1(long ALink);
   void              DoGetValue_3(long ALink);
   void              DoGetValue_5(long ALink);
   void              DoGetValue_12(long ALink);

   void              DoGetValue1_1(long ALink);
   void              DoGetValue1_3(long ALink);
   void              DoGetValue1_12(long ALink);

   // 设置不同类型值的方法
   bool              DoSetValue_1(long ARefLink);
   bool              DoSetValue_2(long ARefLink);
   bool              DoSetValue_3(long ARefLink);
   bool              DoSetValue_5(long ARefLink);
   bool              DoSetValue_12(long ARefLink);

   bool              DoSetValue1_1(long ALink);
   bool              DoSetValue1_2(long ALink);
   bool              DoSetValue1_3(long ALink);
   bool              DoSetValue1_12(long ALink);

   // 分配不同类型值的方法
   bool              DoNewValue_1(long& ALink);
   bool              DoNewValue_2(long& ALink);
   bool              DoNewValue_3(long& ALink);
   bool              DoNewValue_5(long& ALink);
   bool              DoNewValue_12(long& ALink);

   bool              DoNewValue1_1(long& ALink);
   bool              DoNewValue1_2(long& ALink);
   bool              DoNewValue1_3(long& ALink);
   bool              DoNewValue1_12(long& ALink);

private:
   void*             FData;               // 自定义数据
   TKYIndexFile*     FFile;               // 索引文件对象
   TKYList*          FKeys;               // 索引键值列表
   TKYList*          FPlaces;             // 索引键位置列表

   TKYVarType*       FVar;                // 方法 DoCompare_x 使用的变量
   TKYVarType*       FValue;              // 索引项对应值
   long              FValHead;            // 值的头连接
   long              FValLink;            // 值的当前连接

   THead             FHead;               // 索引头信息
   Byte              FState;              // 索引树状态
   bool              FClosed;             // 索引文件已关闭
   Word              FIndexNo;            // 索引树编号
   bool              FCanDuplicate;       // 是否允许重复, 默认值为 false

   // 插入键值相关数据
   Byte              FDimension;          // 当前键维度
   TKeyPlace*        FPlace;              // 当前键位置
   TKYList*          FNodes;              // 层路径结点
   TKYList*          FOrderNos;           // 结点中的序号
   long              FItemSize;           // 当前键的项尺寸

public:
   // 校正值
   static void       _VerifyValue(TKYVarType& AVar, TType AType, long ASize);

public:
   // 值比较函数指针(类型必须相同)
   typedef long (*TCompareVar)(const TKYVarType& AVar1, const TKYVarType& AVar2,
                                           bool  ACaseSensitive);

   // 不同类型的比较函数函数指针
   static const TCompareVar         _Type_CompVar[13];

   // 键和值类型对应的可变数据类型
   // 注: 类型 ftString, ftFixedStr, ftFixedSize 对应 TKYVarType::vtString 类型
   static const TKYVarType::TType   _Type_Var[13];

private:
   // 方法指针类型
   typedef long (TKYIndexTree::*TDoCompare)(TKeyPlace& APlace,
                                void* AItem, TVarData* ACond);
   typedef void (TKYIndexTree::*TDoGetKey)(TKeyPlace& APlace, void* AItem);
   typedef bool (TKYIndexTree::*TDoSetKey)(TKeyPlace& APlace, void* AItem);
   typedef void (TKYIndexTree::*TDoGetValue)(long ALink);
   typedef bool (TKYIndexTree::*TDoSetValue)(long ARefLink);
   typedef bool (TKYIndexTree::*TDoNewValue)(long& ALink);
   typedef bool (TKYIndexTree::*TDoPart)(long  AParent, long  AOrderNo, long  ACurrent,
                                     KYString& AItem,   long& ALink,    long& AInsNo,
                                         bool& ABreak,  long  AIndex);

   // 不同数据类型的方法指针
   static const TDoCompare          _Type_Compare[13];
   static const TDoGetKey           _Type_GetKey[13];
   static const TDoSetKey           _Type_SetKey[13];
   static const TDoGetValue         _Type_GetValue[2][13];
   static const TDoSetValue         _Type_SetValue[2][13];
   static const TDoNewValue         _Type_NewValue[2][13];
   static const TDoPart             _Op_DoPart[3];

   // 索引键定义集对象
   static TKYFieldDefs              _Defs_KeyInfo;
   static long                      _MinSize_KeyInfo;

   // TKYIndexTree 的静态成员初始化类
   static class TInitialization
   {
   public:
      TInitialization();
   } _Initialization;

   friend class TInitialization;
};

}

#endif
