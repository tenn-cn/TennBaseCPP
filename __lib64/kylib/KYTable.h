// =======================================
// Unit   : 表文件类 (KYTable.h)
// Version: 3.0.0.0 (build 2011.03.09)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYTable_H_
#define _KYTable_H_

#include "KYFiles.h"
#include "KYFields.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYTable - 表文件类 */

class TKYTable
{
public:
   // 查找记录的选项(位操作)
   enum TLocateOption  {loCaseInsensitive = 0x01,  // 不区分大小写
                        loPartialKey      = 0x02}; // 部分匹配

   // 数据集的状态
   enum TState         {kdsInactive,               // 未打开状态
                        kdsOpening,                // 正在打开表
                        kdsCreating,               // 正在创建表
                        kdsBrowse,                 // 浏览记录
                        kdsEdit,                   // 编辑当前记录
                        kdsInsert};                // 插入新记录

   // 版本信息
   #pragma pack(push, 1)
   typedef struct
   {
      Byte              Major;                     // 主版本
      Byte              Minor;                     // 小版本
      Byte              Release;                   // 发布版本
      Byte              Build;                     // 发布小版本
   } TVersion, *PVersion;
   #pragma pack(pop)

protected:
   // 表 TableOptions 功能选项
   enum TTableOption   {toMoveOnlyNext = 0x0001,   // 移动指针只能 Next
                        toCannotDelete = 0x0002,   // 记录不允许删除
                        toFieldCanNull = 0x0004,   // 字段允许为空
                        toNoFieldDefs  = 0x0008};  // 无字段定义集

   // 扩展头信息的写模式
   enum TInfoExMode    {iwmUpdate,
                        iwmInsert,
                        iwmDelete,
                        iwmRecall};

   // 记录头信息种类
   enum TRecHeadKind   {rhkVer1,
                        rhkFixed,
                        rhkByte,
                        rhkByte0,
                        rhkWord,
                        rhkWord0,
                        rhkInt,
                        rhkInt0};

   // 表结构   = <HeadInfo> + <HeadInfoEx> + <OwnerInfo> + <FieldInfo> + <Record>
   // 记录结构 = [RecordHead] + <RecordValue>
   #pragma pack(push, 1)

   // 表头基本信息
   typedef struct
   {
      long              Size;
      TVersion          Version;
      long              FieldSize;
      long              FieldCount;
      long              RecordHead;
      long              RecordSize;
      long              HeadOrigin;
      long              OwnerOrigin;
      long              FieldOrigin;
      long              RecordOrigin;
      // ^^^^^^ Add on 2006.02.18 by Kyee ^^^^^^
      // Add: Table version 2.0.0.0 及以后版本
      Longword          TableOptions;
      Byte              NullMapOffset;
   } THeadInfo, *PHeadInfo;

   // 记录头信息(包含 rhkVer1, rhkInt 种类)
   typedef struct
   {
      long              PriorSize;
      long              FullSize;
      bool              Deleted;
   } TRecordHead, *PRecordHead;

   #pragma pack(pop)

public:
   TKYTable();
   virtual ~TKYTable();

   // ----- 表特征属性
   // 表的创建日期时间
   TDateTime            CreateDate() const   { return FHeadInfoEx->Fields(1)->AsDateTime(); }

   // 表的最后修改日期时间
   TDateTime            ModifyDate() const   { return FHeadInfoEx->Fields(2)->AsDateTime(); }

   // 自定义名字, 最大长度 32, 默认值为: ""
   KYString             OwnerName() const    { return FOwnerInfo->Fields(2)->AsString(); }

   // 自定义数据尺寸, 数据放在表的 <HeadInfoEx> 和 <FieldInfo> 之间, 默认值为: 0
   long                 OwnerSize() const    { return FOwnerInfo->Fields(3)->AsInteger(); }

   // 自定义版本信息, 默认值为: {0, 0, 0, 0}
   TVersion             OwnerVersion() const;

   // 表的版本信息, 默认值为: {2, 1, 0, 0}
   TVersion             TableVersion() const { return FHeadInfo.Version; }

   // 表记录之间上下文是否只有向下链接, 默认值为: false
   bool                 MoveOnlyNext() const { return IsOption(FHeadInfo.TableOptions, toMoveOnlyNext); }

   // 表记录是否不允许删除, 默认值为: false
   bool                 CannotDelete() const { return IsOption(FHeadInfo.TableOptions, toCannotDelete); }

   // 表记录的字段值是否允许空存放, 默认值为: false
   // 若此属性为 true, 则当某字段未赋值而保存时不占记录空间, 而只占一个空的标志(1 bit)
   bool                 FieldCanNull() const { return IsOption(FHeadInfo.TableOptions, toFieldCanNull); }

   // 表文件是否不存放字段定义信息, 默认值为: false
   // 若此属性为 true, 则在打开表之前必须要预先定义 FieldDefs 属性, 存放记录的字段集信息
   bool                 NoFieldDefs() const  { return IsOption(FHeadInfo.TableOptions, toNoFieldDefs); }

   // ----- 表属性
   // 表的数据库名, 即表文件所在的路径或上层路径, 默认值为: ""
   KYString             DatabaseName() const { return FDatabaseName; }

   // 表名, 即表的文件名或带全路径的文件名, 默认值为: ""
   // 若设置了 DatabaseName, 则表文件名为: DatabaseName() + TableName()
   KYString             TableName() const    { return FTableName; }

   // 表是否独占打开, 默认值为: false
   bool                 Exclusive() const    { return FExclusive; }

   // 表是否只读打开, 默认值为: false
   bool                 ReadOnly() const     { return FReadOnly; }

   // 判断表文件是否存在
   bool                 Existed() const      { return FileExists(GetFileName(FDatabaseName, FTableName)); }

   // 保存新增记录时是否填充到已删除记录的空间, 默认值为: false
   // 若设置此属性为 true, 则新建记录时需要查找已删除记录的位置, 性能可能会变得很低
   bool                 FillDeleted() const  { return FFillDeleted; }

   // 是否可浏览已删除的记录, 默认值为: false
   // 若设置此属性为 true, 则移动记录指针时不会跳过已删除记录, 也就是说, 移动记
   // 录指针时不判断是否已删除记录, 由 Deleted() 属性表示记录是否已删除
   bool                 ShowDeleted() const  { return FShowDeleted; }

   // ----- 字段属性
   // 表记录的字段个数
   long                 FieldCount() const   { return FHeadInfo.FieldCount; }

   // 表记录的字段定义集信息
   TKYFieldDefs*        FieldDefs() const    { return FFieldDefs; }

   // 根据字段号取表记录的字段值, 字段号取值范围: [0..FieldCount()-1]
   TKYField*            Fields(long AFieldNo) const
                        { return Active() ? FFields->Fields(AFieldNo) : NULL; }

   // 根据字段名取记录的字段可变数据值
   TKYVarType::TVarData* FieldValues(const KYString& AFieldName) const;

   // ----- 记录属性
   // 表是否已打开, 默认值为: false
   bool                 Active() const       { return (FState >= kdsBrowse); }

   // 表打开, 创建, 浏览, 编辑或插入记录时的错误信息, 默认值为: ""
   KYString             Error() const        { return FError; }

   // 表当前状态, 默认值为: kdsInactive
   TState               State() const        { return FState; }

   // 判断记录指针是否在开始位置, 默认值为: true
   bool                 Bof() const          { return FBOF; }

   // 判断记录指针是否在末尾, 默认值为: true
   // 若此属性为 true 则表示无法再向下移动记录指针, 或表未打开
   bool                 Eof() const          { return FEOF; }

   // 判断 Locate 方法是否查找到记录, 默认值为: false
   bool                 Found() const        { return FFound; }

   // 记录标识号, 为表记录的物理编号, 在表文件中唯一, 标识号范围: [1..HaveRecCount()]
   long                 RecID() const        { return FRecID; }

   // 记录号, 为表记录的逻辑编号, 与 ShowDeleted() 属性有关, 范围: [1..ShowRecCount()]
   // 若 ShowDeleted() 属性为 true, 则有 RecNo() == RecID(), 否则就不一定相同
   long                 RecNo() const        { return FRecNo; }

   // 当前记录是否已删除
   bool                 Deleted() const      { return Active() && FRecordHead.Deleted; }

   // 表文件中存在的记录数, 包括已删除记录
   long                 HaveRecCount() const { return FHeadInfoEx->Fields(6)->AsInteger(); }

   // 表文件可浏览的记录数,
   // 若 ShowDeleted() 属性为 true, 则有 ShowDeleted() == HaveRecCount(),
   // 否则 ShowRecCount() == RecordCount()
   long                 ShowRecCount() const;

   // 当前记录的数据尺寸
   long                 RecCurrSize() const  { return Active() ? FFields->RecordSize() : 0; }

   // 当前记录所在空间的最大允许记录尺寸
   long                 RecFullSize() const;

   // 未删除的记录数
   long                 RecordCount() const  { return FHeadInfoEx->Fields(5)->AsInteger(); }

   // 记录的最大尺寸
   long                 RecordSize() const   { return FHeadInfo.RecordSize; }

   // 设置自定义名字
   bool                 SetOwnerName(const KYString& AName);

   // 设置自定义数据尺寸, 必须在 CreateTable 之前设置, 否则无效
   bool                 SetOwnerSize(long ASize);

   // 设置自定义版本信息
   bool                 SetOwnerVersion(const TVersion& AVersion);

   // 设置表记录之间上下文是否只有向下链接, 必须在 CreateTable 之前设置, 否则无效
   bool                 SetMoveOnlyNext(bool AEnabled);

   // 设置表记录是否不允许删除, 必须在 CreateTable 之前设置, 否则无效
   bool                 SetCannotDelete(bool AEnabled);

   // 设置表记录的字段值是否允许空存放, 必须在 CreateTable 之前设置, 否则无效
   bool                 SetFieldCanNull(bool AEnabled);

   // 设置表文件是否不存放字段定义信息, 必须在 CreateTable 之前设置, 否则无效
   bool                 SetNoFieldDefs(bool AEnabled);

   // 设置表文件所在的路径或上层路径, 必须在 Open 之前设置, 否则无效
   bool                 SetDatabaseName(const KYString& AName);

   // 设置表的文件名或带全路径的文件名, 必须在 Open 之前设置, 否则无效
   bool                 SetTableName(const KYString& AName);

   // 设置表是否独占打开, 必须在 Open 之前设置, 否则无效
   bool                 SetExclusive(bool AEnabled);

   // 设置表是否只读打开, 必须在 Open 之前设置, 否则无效
   bool                 SetReadOnly(bool AEnabled);

   // 设置保存新增记录时是否填充到已删除记录的空间
   bool                 SetFillDeleted(bool AEnabled);

   // 设置是否可浏览已删除的记录, 必须在 Open 之前设置, 否则无效
   bool                 SetShowDeleted(bool AEnabled);

   // 设置字段值, 必须在 Active() 为 true 之后设置, 否则无效
   bool                 SetFieldValues(const KYString& AFieldName,
                           const TKYVarType::TVarData* AValue);

   // 设置打开/关闭表
   bool                 SetActive(bool AValue);

   // 设置记录标识号, 必须在 Active() 为 true 之后设置, 否则无效
   // 若标识号指向的是已删除记录且 ShowDeleted() 属性为 false, 则记录号无效
   bool                 SetRecID(long ARecID);

   // 设置记录号, 必须在 Active() 为 true 之后设置, 否则无效
   bool                 SetRecNo(long ARecNo);

   // 创建表文件, 若创建失败可从 Error() 属性取错误信息
   bool                 CreateTable(bool AIsVer1 = false);

   // 删除表文件, 若失败可从 Error() 属性取错误信息
   bool                 DeleteTable();

   // 清空表的所有记录, 若失败可从 Error() 属性取错误信息
   bool                 EmptyTable();

   // 清空表的所有已删除记录, 若失败可从 Error() 属性取错误信息
   bool                 PackTable();

   // 表文件重命名, 若失败可从 Error() 属性取错误信息
   bool                 RenameTable(const KYString& ANewName);

   // 读取表文件的属性, 若失败可从 Error() 属性取错误信息
   bool                 ReadTableAttrib();

   // 打开表, 若失败可从 Error() 属性取错误信息
   bool                 Open();

   // 关闭表
   void                 Close();

   // 判断表是否为空, 即记录数是否为 0
   bool                 IsEmpty();

   // 刷新表
   // 若 AFixedCursor 为 false 时, 则刷新后记录自动定位到第一条记录
   // 否则 AFixedCursor 为 true 时, 不改变 RecID 和 RecNo 值, 但 RecNo 不一定准确
   void                 Refresh(bool AFixedCursor = false);

   // 更新当前记录
   void                 UpdateRecord();

   // 删除当前记录
   bool                 Delete();

   // 恢复当前已删除记录
   bool                 RecallRecord();

   // 追加记录
   bool                 Append()             { return Insert(); }

   // 取消编辑可插入记录
   bool                 Cancel();

   // 编辑当前记录
   bool                 Edit();

   // 插入记录
   bool                 Insert();

   // 提交已编辑或插入的记录
   bool                 Post();

   // 移动记录指针
   long                 MoveBy(long ADistance);

   // 移到第一条记录
   bool                 First();

   // 移到最后一条记录
   bool                 Last();

   // 移到下一条记录
   bool                 Next();

   // 移到上一条记录
   bool                 Prior();

   // 根据字段名取字段值
   TKYField*            FindField(const KYString& AFieldName) const
                        { return Active() ? FFields->FindField(AFieldName) : NULL; }
   TKYField*            FieldByName(const KYString& AFieldName) const
                        { return Active() ? FFields->FindField(AFieldName) : NULL; }

   // 根据条件查找记录, 并指向已找到的第一条记录, 若未找到则不改变记录指针
   // 若要匹配多个字段值时, AFields 的字段名之间使用 ';' 分隔, 且字段值的顺序必
   // 须与 AFields 字段名的顺序相同, 且数据类型也必须相同, 否则可能会产生异常
   bool                 Locate(const KYString& AFields, Byte AOptions, ...);

   // 条件查找记录的方法, 相当于 Locate 方法的分步操作
   // 1. BeginKeys      初始化查找字段条件, 并置 FIsAdding 为 true
   // 2. AddKey         添加需要查找的字段及字段值, 可以多次调用
   // 3. FindKeys       执行条件查找, 若成功则指向已找到的第一条记录,
   //                   若未找到则不改变记录指针, 并清除添加的查找字段值
   bool                 BeginKeys();
   bool                 AddKey(const KYString& AFieldName, const TKYVarType& AValue);
   bool                 FindKeys(Byte AOptions);

   // 满足查找条件的第一条记录
   bool                 FindFirst();

   // 满足查找条件的最后一条记录
   bool                 FindLast();

   // 满足查找条件的下一条记录
   bool                 FindNext();

   // 满足查找条件的上一条记录
   bool                 FindPrior();

   // 读取自定义的数据并返回实际读取尺寸, 必须在 Active() 为 true 之后调用, 否则失败并返回 0
   long                 GetOwnerData(void* AData, long AOffset, long ACount) const;

   // 写自定义的数据并返回实际写入尺寸, 必须在 Active() 为 true 之后调用, 否则失败并返回 0
   long                 SetOwnerData(const void* AData, long AOffset, long ACount);

protected:
   void                 Lock() const         { FLock->Enter(); }
   void                 Unlock() const       { FLock->Leave(); }

   bool                 LockRead(Longword AOffset, Longword ACount) const;
   bool                 UnlockRead(Longword AOffset, Longword ACount) const;

   bool                 LockWrite(Longword AOffset, Longword ACount) const;
   bool                 UnlockWrite(Longword AOffset, Longword ACount) const;

   // 记录指针操作
   bool                 RecOfFirst();
   bool                 RecOfLast();
   long                 RecOfGoto(long APos, bool AIsID);
   long                 RecOfNext(long ADistance);
   long                 RecOfPrior(long ADistance);
   long                 RecOfNextID(long ADistance);
   long                 RecOfPriorID(long ADistance);
   long                 RecOfFixedID(long ADistance, bool AIsNext);

   // 读/写记录头信息
   bool                 ReadRecHead(TRecordHead& ARecHead, long AOffset,
                                            bool AMoveOnlyNext,
                                            bool ACannotDelete,
                                            bool AHasLocked = false);
   bool                 WriteRecHead(const TRecordHead& ARecHead, long AOffset,
                                                   bool AHasLocked = false);

   // 读/写扩展头信息
   bool                 ReadHeadInfoEx();
   bool                 WriteHeadInfoEx(TInfoExMode AMode, long ALastRecSize = 0);

   // 读/写自定义信息
   bool                 ReadOwnerInfo();
   bool                 WriteOwnerInfo();

   // 记录编辑的内部函数
   bool                 CheckBrowseMode(const char* AErrOp);
   bool                 InternalCancel();
   bool                 InternalEdit();
   bool                 InternalInsert();

   bool                 PostOfDeleteFlag(long AOffset);
   bool                 PostOfRecallFlag(long AOffset);
   bool                 PostOfFindDeleted(long& AOffset, long& ARecID,
                                          long& ARecNo,  long  ARecSize,
                                          bool  AOverflow, TRecordHead& AHead);
   bool                 PostOfUpdateRec(const char* ABuffer,   long AOffset,
                                              long  AFullSize, long ARecSize);
   bool                 PostOfInsertRec(const char* ABuffer, long ARecSize);

   bool                 InternalPostEdit(const char* ABuffer, long ARecSize);
   bool                 InternalPostInsert(const char* ABuffer, long ARecSize,
                                                  bool AOverflow);
   bool                 InternalPost();

   bool                 InternalDelete();
   bool                 InternalRecallRecord();

   bool                 InternalRefresh(bool AFixedCursor);
   bool                 InternalUpdateRecord();

   // 查找记录的函数
   long                 AnalyzeFields(const KYString& AFields);
   bool                 CompareRecord();
   bool                 FindRecord(bool ARestart, bool AGoForward);

private:
   void                 InitialInfo();
   void                 ReleaseInfo();

   void                 NullRecord();
   bool                 ReadRecord(const TRecordHead& AHead, long AOffset, long ARecNo);
   bool                 ReadDataOfFile(long AOffset, long ACount, bool ANeedSeek);

   void                 DoValueDeletion(TKYField* AItem);

private:
   TKYCritSect*         FLock;
   KYString             FError;

   // 头信息
   THeadInfo            FHeadInfo;
   TKYFields*           FHeadInfoEx;
   TKYFields*           FOwnerInfo;
   TKYFields*           FFields;
   TKYFieldDefs*        FFieldDefs;

   // 记录头信息
   TKYFields*           FRecHeadFields;
   KYString             FRecHeadBuffer;
   long                 FRecMinSize;
   TRecordHead          FRecordHead;
   TRecHeadKind         FRecHeadKind;

   // 表打开方式
   KYString             FDatabaseName;
   KYString             FTableName;
   bool                 FExclusive;
   bool                 FReadOnly;
   bool                 FFillDeleted;
   bool                 FShowDeleted;

   // 表文件及缓冲区
   TFile*               FFile;
   long                 FCursor;
   KYString             FBuffer;

   // 状态
   TState               FState;
   bool                 FBOF;
   bool                 FEOF;
   long                 FRecID;
   long                 FRecNo;

   // 查找
   bool                 FFound;
   bool                 FIsAdding;
   Byte                 FKeyOptions;
   TKYList*             FKeyFields;
   TKYList*             FKeyValues;

public:
   // 选项值和设置选项
   static bool          IsOption(Longword AValue, Longword AOption)
                        { return (AValue & AOption) == AOption; }
   static void          SetOption(Longword& AValue, Longword AOption, bool AEnabled)
                        { AEnabled ? (AValue |= AOption) : (AValue &= ~AOption); }

   // 是否相同字段值
   static bool          SameFieldValue(const TKYField& AField,
                                       const TKYField& AValue, Byte AOptions);

   // 合成版本值
   static TVersion      Version(Byte AMajor, Byte AMinor, Byte ARelease, Byte ABuild);

   // Version <-> String
   static KYString      VerToString(const TVersion& AVersion);
   static TVersion      StringToVer(const KYString& AStr);

   // 得到偏移量
   static Byte          GetNullMapOffset(TRecHeadKind AKind, bool ACannotDelete,
                                                             bool ACanNull);

   // 得到记录头字段集
   static void          GetRecordHead(TKYFields* AHead, TRecHeadKind AKind,
                                            bool ACannotDelete);

   // 得到记录头尺寸
   static long          GetRecHeadSize(TRecHeadKind AKind, long AFieldCount,
                                               bool ACannotDelete, bool ACanNull);

   // 得到记录头种类
   static TRecHeadKind  GetRecHeadKind(const TVersion& AVersion, bool AFixed,
                                                  bool AMoveOnlyNext, long ARecordSize);

   // 检查记录头尺寸是否合法
   static bool          CheckRecHeadSize(const THeadInfo& AHeadInfo, TRecHeadKind& AKind);

   // 检查头信息是否合法
   static bool          CheckHeadInfo(const THeadInfo& AHeadInfo, long AFileSize);

   // 检查扩展头信息是否合法
   static bool          CheckHeadInfoEx(const THeadInfo& AHeadInfo,
                                        const TKYFields* AHeadInfoEx, long AFileSize);

   // 检查自定义信息是否合法
   static bool          CheckOwnerInfo(const THeadInfo& AHeadInfo,
                                       const TKYFields* AOwnerInfo);

   // 读表头相关信息
   static bool          ReadTableHead(const TFile* AFile, TRecHeadKind& AKind,
                                        THeadInfo& AHeadInfo,
                                        TKYFields* AHeadInfoEx,
                                        TKYFields* AOwnerInfo);

   // 读字段定义集
   static bool          ReadFieldDefs(const TFile* AFile,
                                  const THeadInfo& AHeadInfo,
                                     TKYFieldDefs* AFieldDefs);

   // 校正表头信息
   static void          VerifyTableHead(THeadInfo& AHeadInfo,
                                        TKYFields* AHeadInfoEx,
                                        TKYFields* AOwnerInfo,
                                     TKYFieldDefs* AFieldDefs, bool AIsVer1);

   // 写表头信息
   static void          WriteTableHead(const TFile* AFile,
                                   const THeadInfo& AHeadInfo,
                                   const TKYFields* AHeadInfoEx,
                                   const TKYFields* AOwnerInfo,
                                const TKYFieldDefs* AFieldDefs);

   // 移掉已经删除的记录
   static void          RemoveRecordOfDeleted(const TFile* AFile,
                                              TRecHeadKind AKind,
                                          const THeadInfo& AHeadInfo,
                                                TKYFields* AHeadInfoEx);

protected:
   // 缓冲区<->TVersion
   static bool          Buffer2Version(const void* ABuffer, TVersion& AVersion);
   static long          Version2Buffer(const TVersion& AVersion, void* ABuffer);

   // 缓冲区<->THeadInfo
   static bool          Buffer2HeadInfo(const void* ABuffer, THeadInfo& AHeadInfo);
   static long          HeadInfo2Buffer(const THeadInfo& AHeadInfo, void* ABuffer);

private:
   // 字段比较函数指针
   typedef bool          (*TSameValue)(const TKYField& AField,
                                       const TKYField& AValue, Byte AOptions);

   // 静态字段定义集对象
   static TKYFieldDefs  Defs_Version;        // TVersion
   static TKYFieldDefs  Defs_HeadInfo;       // THeadInfo
   static TKYFieldDefs  Defs_HeadInfoEx;     // THeadInfoEx
   static TKYFieldDefs  Defs_OwnerInfo;      // TOwnerInfo
   static TKYFieldDefs  Defs_FieldInfo;      // TFieldInfo
   static TKYFieldDefs  Defs_RecordHead;     // TRecordHead
   static TKYFieldDefs  Defs_RH_Fixed;       // rhkFixed
   static TKYFieldDefs  Defs_RH_Byte;        // rhkByte
   static TKYFieldDefs  Defs_RH_Word;        // rhkWord
   static TKYFieldDefs  Defs_RH_Byte0;       // rhkByte0
   static TKYFieldDefs  Defs_RH_Word0;       // rhkWord0
   static TKYFieldDefs  Defs_RH_Int0;        // rhkInt0

   // 各种记录头
   static TKYFieldDefs* HeadKind_Defs[8];
   static const Byte    HeadKind_Size[2][8];

   // 各版本对应的结构尺寸
   static const Word    StructOfVer_Size[2][4];

   // 字段名
   static const KYString Field_Name;         // "Name"
   static const KYString Field_Type;         // "Type"
   static const KYString Field_Size;         // "Size"
   static const KYString Field_PriorSize;    // "PriorSize"
   static const KYString Field_FullSize;     // "FullSize"
   static const KYString Field_Deleted;      // "Deleted"

   // 各版本值
   static const TVersion Version_Current;    // 当前版本
   static const TVersion Version_Ver1;       // 版本 1.0
   static const TVersion Version_NULL;       // 空版本

   // 打开格式
   static const Longword OpenMode_ReadOnly[2];
   static const Longword OpenMode_Exclusive[2];

   // 比较相同值的函数指针数组
   static const TSameValue SameValues[13];

   // TKYTable 的静态成员初始化类
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