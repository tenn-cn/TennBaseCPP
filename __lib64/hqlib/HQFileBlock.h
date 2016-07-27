// =======================================
// Unit   : 文件块单元
// Version: 3.0.1.0 (build 2011.07.08)
// Author : Kennybee
// Email  : lzk(at)hopechart.com
// Copyright (C) Hopechart Co., Ltd.
// =======================================

#ifndef _HQFileBlock_H_
#define _HQFileBlock_H_

#include "HQUtils.h"

// HQLib 的命名空间
namespace HQLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* THQFileBlock - 文件块类 */

class THQFileBlock
{
public:
   // 文件状态
   enum TState   {bsInactive,          // 未打开
                  bsOpening,           // 正在打开
                  bsClosing,           // 正在关闭
                  bsOpened};           // 已经打开

protected:
   #pragma pack(push, 1)

   // 头信息
   typedef struct
   {
      TVersion    Version;             // 文件版本
      Word        TagSize;             // 附加尺寸
      Word        ItemSize;            // 项尺寸
      Longword    Capacity;            // 存储容量
      Longword    MapOrigin;           // 映射开始位置
      Longword    TagOrigin;           // 附加开始位置
      Longword    ItemOrigin;          // 项开始位置
      Longword    IndexOrigin;         // 索引开始位置
      Longword    QueueOrigin;         // 空闲队列开始位置
   } THead, *PHead;

   // 索引信息
   typedef struct
   {
      Longword    Count;               // 已分配项数
      Longword    Begin;               // 开始索引
      Longword    End;                 // 末尾索引
   } TIndex, *PIndex;

   #pragma pack(pop)

private:
   // 内部方法
   typedef void     (THQFileBlock::*TDoInit)();
   typedef Longword (THQFileBlock::*TDoNew)(Longword AIndex);
   typedef void     (THQFileBlock::*TDoDelete)(Longword AHandle, Longword AIndex);

public:
   THQFileBlock(bool ACanLock = false);
   virtual ~THQFileBlock();

   // 属性
   void*          Data() const         { return FData; }             // default: NULL
   TState         State() const        { return FState; }            // default: bsInactive
   TVersion       Version() const      { return FHead.Version; }     // default: 0,0,0,0
   Word           TagSize() const      { return FHead.TagSize; }     // default: 0
   Word           ItemSize() const     { return FHead.ItemSize; }    // default: 0
   Longword       Capacity() const     { return FHead.Capacity; }    // default: 0
   Longword       RefCount() const     { return FRefCount; }         // default: 0
   KYString       FileName() const     { return FFile->FileName(); } // default: ""

   Longword       Count();             // 取已分配项数
   Longword       IdleCount();         // 取空闲项数

   // 设置属性
   void           SetData(void* AData) { FData = AData; }
   void           SetMutex(TKYMutex* AMutex);

   // 增减引用计数
   bool           IncRefCount();
   void           DecRefCount();

   // 打开/关闭文件
   // 1. 若 ACanCreate == true 且文件不存在时, 则创建文件
   // 2. ACapacity, AItemSize 和 ATagSize 只有创建文件时有效
   bool           Open(const KYString& AFileName,     Longword ACapacity = 0,
                                 Word  AItemSize = 0,     Word ATagSize  = 0,
                                 bool  ACanCreate = true);
   void           Close();

   // 预加载文件
   // 注: 为了把文件内容确保加载到内存映像中, 加快初次读取文件操作速度, 一般是在
   // 文件打开后调用。ASize <= 0 表示整个文件加载, 也可以预加载文件从头开始到指
   // 定 ASize 部分。
   bool           Preload(long ASize = 0);

   // 分配/删除/清除所有句柄
   // New:        若返回值为 0 表示分配句柄失败
   // Delete:     若返回值为 false 表示句柄不存在或状态不合法
   Longword       New();
   bool           Delete(Longword AHandle);
   void           Clear();

   // 判断句柄是否存在
   bool           Existed(Longword AHandle);

   // 读取/保存项数据
   // Read:       ASize 传入要读取的尺寸, 返回实际读取的尺寸
   // Write:      ASize 传入要保存的尺寸, 返回实际保存的尺寸
   bool           Read(Longword AHandle, long AOffset, void* AData, long& ASize);
   bool           Write(Longword AHandle, long AOffset, const void* AData, long& ASize);

   // 读取/保存附加数据
   // ReadTag:    ASize 传入要读取的尺寸, 返回实际读取的尺寸
   // WriteTag:   ASize 传入要保存的尺寸, 返回实际保存的尺寸
   bool           ReadTag(long AOffset, void* AData, long& ASize);
   bool           WriteTag(long AOffset, const void* AData, long& ASize);

protected:
   void           Lock() const         { if (FLock != NULL) FLock->Enter(); }
   void           Unlock() const       { if (FLock != NULL) FLock->Leave(); }

   bool           LockFile() const     { return (FMutex == NULL) || FMutex->Lock(); }
   void           UnlockFile() const   { if (FMutex != NULL) FMutex->Unlock(); }

   // 偏移量转内存地址(AOffset > 0)
   void*          Offset2Mem(long AOffset) const
                  { return (Byte*)FFile->MapMem() + AOffset; }

private:
   void           DoInit();
   void           DoInitQueueFun(Longword ACapacity);
   bool           DoCheck(const THead& AHead);
   bool           DoExisted(Longword AHandle);
   void           DoSetBit(Longword AHandle, bool AValue);

   bool           DoOpen(const KYString& AFileName);
   bool           DoCreate(const KYString& AFileName, Longword ACapacity,
                                     Word  AItemSize,     Word ATagSize);
   void           DoClose();
   
   void           DoInitHead(Longword ACapacity, Word AItemSize, Word ATagSize);
   void           DoUpdate();

private:
   void           DoInitQueue();
   void           DoInitOfByte();
   void           DoInitOfWord();
   void           DoInitOfLongword();

   Longword       DoNewHandle();
   Longword       DoNewOfByte(Longword AIndex);
   Longword       DoNewOfWord(Longword AIndex);
   Longword       DoNewOfLongword(Longword AIndex);

   void           DoDeleteHandle(Longword AHandle);
   void           DoDeleteOfByte(Longword AHandle, Longword AIndex);
   void           DoDeleteOfWord(Longword AHandle, Longword AIndex);
   void           DoDeleteOfLongword(Longword AHandle, Longword AIndex);

private:
   void*          FData;               // 自定义数据
   TKYMapFile*    FFile;               // 文件对象
   TKYCritSect*   FLock;               // 状态锁
   TKYMutex*      FMutex;              // 互斥元

   THead          FHead;               // 头信息
   TState         FState;              // 状态
   TIndex*        FIndex;              // 索引信息
   Byte*          FMapBits;            // 项映射位值
   Longword       FRefCount;           // 引用计数

   TDoInit        FDoInit;             // 初始化空闲队列
   TDoNew         FDoNew;              // 分配句柄
   TDoDelete      FDoDelete;           // 释放句柄

public:
   // 静态成员
   static const TVersion   _Ver_Current;// 当前版本(1.0.0.0)
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* THQFileHandles - 文件句柄集类 */

class THQFileHandles
{
public:
   // 文件句柄集状态
   enum TState   {hsInactive,          // 未打开
                  hsOpening,           // 正在打开
                  hsClosing,           // 正在关闭
                  hsOpened};           // 已经打开

protected:
   #pragma pack(push, 1)

   // 头信息
   typedef struct
   {
      TVersion    Version;             // 文件版本
      Word        TagSize;             // 附加尺寸
      Word        MapSize;             // 映射尺寸
      Word        ItemSize;            // 项尺寸
      Word        MaxFiles;            // 最大文件个数
      Longword    BlockSize;           // 文件块容量
      Longword    ExtOrigin;           // 扩展开始位置
      Longword    TagOrigin;           // 附加开始位置
      Longword    MapOrigin;           // 映射开始位置
      Longword    IndexOrigin;         // 索引开始位置
      Longword    QueueOrigin;         // 空闲队列开始位置
   } THead, *PHead;

   // 扩展信息
   typedef struct
   {
      Longword    Count;               // 已分配项数
      Longword    Capacity;            // 总存储容量
      Word        FileCount;           // 文件个数
   } TExtInfo, *PExtInfo;

   // 映射项
   typedef struct
   {
      Longword    Count;               // 已分配项数
      Longword    IdleCount;           // 空闲项数
   } TMapItem, *PMapItem;

   // 索引信息
   typedef struct
   {
      Longword    Count;               // 已分配项数
      Longword    Begin;               // 开始索引
      Longword    End;                 // 末尾索引
   } TIndex, *PIndex;

   #pragma pack(pop)

public:
   THQFileHandles();
   virtual ~THQFileHandles();

   // 属性
   void*          Data() const         { return FData; }             // default: NULL
   TState         State() const        { return FState; }            // default: hsInactive
   TVersion       Version() const      { return FHead.Version; }     // default: 0,0,0,0
   Word           TagSize() const      { return FHead.TagSize; }     // default: 0
   Word           ItemSize() const     { return FHead.ItemSize; }    // default: 0
   Word           MaxFiles() const     { return FHead.MaxFiles; }    // default: 0
   Longword       BlockSize() const    { return FHead.BlockSize; }   // default: 0
   Longword       RefCount() const     { return FRefCount; }         // default: 0

   KYString       Path() const         { return FPath; }             // default: CurrAppFilePath
   KYString       Prefix() const       { return FPrefix; }           // default: ""
   KYString       Suffix() const       { return FSuffix; }           // default: ".hfb"
   KYString       FileName() const     { return FFile->FileName(); } // default: ""

   Longword       Capacity();          // 取总存储容量
   Longword       Count();             // 取已分配项数

   // 设置属性
   void           SetData(void* AData) { FData = AData; }

   void           SetTagSize(Word ASize);
   void           SetItemSize(Word ASize);
   void           SetMaxFiles(Word ACount);
   void           SetBlockSize(Longword ASize);

   void           SetPath(const KYString& APath);
   void           SetPrefix(const KYString& AName);
   void           SetSuffix(const KYString& AName);

   // 增减引用计数
   bool           IncRefCount();
   void           DecRefCount();

   // 打开/关闭文件句柄集
   bool           Open();
   void           Close();

   // 预加载文件
   // 注: 为了把文件内容确保加载到内存映像中, 加快初次读取文件操作速度, 一般是在
   // 文件打开后调用。ASize <= 0 表示整个文件加载, 也可以预加载文件从头开始到指
   // 定 ASize 部分。
   bool           Preload(long ASize = 0);

   // 分配/删除/清除所有句柄
   // New:        若返回值为 0 表示分配句柄失败
   // Delete:     若返回值为 false 表示句柄不存在或状态不合法
   Longword       New();
   bool           Delete(Longword AHandle);
   void           Clear();

   // 判断句柄是否存在
   bool           Existed(Longword AHandle);

   // 读取/保存项数据
   // Read:       ASize 传入要读取的尺寸, 返回实际读取的尺寸
   // Write:      ASize 传入要保存的尺寸, 返回实际保存的尺寸
   bool           Read(Longword AHandle, long AOffset, void* AData, long& ASize);
   bool           Write(Longword AHandle, long AOffset, const void* AData, long& ASize);

   // 读取/保存附加数据
   // ReadTag:    ASize 传入要读取的尺寸, 返回实际读取的尺寸
   // WriteTag:   ASize 传入要保存的尺寸, 返回实际保存的尺寸
   bool           ReadTag(long AOffset, void* AData, long& ASize);
   bool           WriteTag(long AOffset, const void* AData, long& ASize);

protected:
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

   bool           LockFile() const     { return FMutex->Lock(); }
   void           UnlockFile() const   { FMutex->Unlock(); }

   // 偏移量转内存地址(AOffset > 0)
   void*          Offset2Mem(long AOffset) const
                  { return (Byte*)FFile->MapMem() + AOffset; }

private:
   void           DoInit();
   bool           DoCheck(const THead& AHead);
   Word           DoCheckHandle(Longword AHandle);

   bool           DoOpen();
   bool           DoCreate();
   void           DoClose();

   void           DoDeleteBlock(long AHandle, THQFileBlock* AFileBlock);

   void           DoInitHead();
   void           DoInitHandles();
   void           DoUpdate();
   Word           DoNewBlockFile();
   void           DoOpenBlockFile(Word AFileNo);
   void           DoOpenBlockFiles();

   void           DoPushBlock(Word AFileIndex);
   Word           DoPeekBlock();
                  
   Byte*          DoMapItem(Word AFileNo) const 
                  { return FMapItems + (AFileNo - 1) * FHead.MapSize;}
   THQFileBlock*  DoHandleToBlock(Longword AHandle) const
                  { return (THQFileBlock*)FBlocks->Item(DoHandleToFileNo(AHandle)); }
   Word           DoHandleToFileNo(Longword AHandle) const
                  { return (Word)((AHandle - 1) / FHead.BlockSize + 1); }
   Longword       DoHandleToBlockHandle(Longword AHandle) const
                  { return ((AHandle - 1) % FHead.BlockSize) + 1; }

private:
   void*          FData;               // 自定义数据
   TKYMapFile*    FFile;               // 文件对象
   TKYCritSect*   FLock;               // 状态锁
   TKYMutex*      FMutex;              // 互斥元
   TKYHandles*    FBlocks;             // 文件块集

   THead          FHead;               // 头信息
   TState         FState;              // 句柄集状态
   TExtInfo*      FExtInfo;            // 扩展信息
   TIndex*        FIndex;              // 索引信息
   Word*          FQueue;              // 空闲队列
   Byte*          FMapItems;           // 映射项列表
   Longword       FRefCount;           // 引用计数

   KYString       FPath;               // 文件句柄集所在路径, 末尾带路径分隔符
   KYString       FPrefix;             // 文件名前缀
   KYString       FSuffix;             // 文件名后缀, 默认值为 ".hfb"

public:
   // 静态成员
   static const TVersion _Ver_Current; // 当前版本(1.0.0.0)
};

}

#endif
