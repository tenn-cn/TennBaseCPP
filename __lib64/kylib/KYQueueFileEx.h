// =======================================
// Unit   : 映像文件队列 (KYQueueFileEx.h)
// Version: 3.0.0.0 (build 2011.03.26)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYQueueFileEx_H_
#define _KYQueueFileEx_H_

#include "KYMapFile.h"
#include "KYSyncObj.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYQueueFileEx - 映像文件队列类 */

class TKYQueueFileEx
{
public:
   // 队列状态
   enum TState   {qsInactive,          // 未打开
                  qsOpening,           // 正在打开
                  qsClosing,           // 正在关闭
                  qsOpened};           // 已经打开

   // 版本信息
   #pragma pack(push, 1)
   typedef struct
   {
      Byte        Major;               // 主版本
      Byte        Minor;               // 小版本
      Byte        Release;             // 发布小版本
      Byte        Build;               // 编译小版本
   } TVersion, *PVersion;
   #pragma pack(pop)

protected:
   #pragma pack(push, 1)

   // 头信息
   typedef struct
   {
      TVersion    Version;             // 队列版本
      Longword    TagSize;             // 附加尺寸
      Longword    ItemSize;            // 队列项尺寸
      Longword    MaxCount;            // 最大项数
      Longword    TagOrigin;           // 附加开始位置
      Longword    IndexOrigin;         // 索引开始位置
      Longword    QueueOrigin;         // 队列开始位置
   } THead, *PHead;

   // 索引信息
   typedef struct
   {
      Longword    PushCount;           // 压入队列计数
      Longword    PushIndex;           // 压入队列索引
      Longword    PopCount;            // 弹出队列计数
      Longword    PopIndex;            // 弹出队列索引
   } TIndex, *PIndex;

   #pragma pack(pop)

public:
   TKYQueueFileEx();
   virtual ~TKYQueueFileEx();

   // 属性
   void*          Data() const         { return FData; }             // default: NULL
   TState         State() const        { return (TState)FState; }    // default: qsInactive
   bool           IsCycle() const      { return FIsCycle; }          // default: false
   TVersion       Version() const      { return FHead.Version; }     // default: 0,0,0,0
   Longword       TagSize() const      { return FHead.TagSize; }     // default: 0
   Longword       ItemSize() const     { return FHead.ItemSize; }    // default: 0
   Longword       MaxCount() const     { return FHead.MaxCount; }    // default: 0
   KYString       FileName() const     { return FFile->FileName(); } // default: ""

   Longword       Count();             // 取队列项数
   Longword       IdleCount();         // 取队列空闲项数
   Longword       PushIndex();         // 取 Push 索引
   Longword       PopIndex();          // 取 Pop 索引

   // 设置属性
   void           SetData(void* AData) { FData = AData; }

   // 打开队列文件
   // 1. 若 ACanCreate == true 且文件不存在时, 则创建队列文件
   // 2. ATagSize, AItemSize 和 AMaxCount 只有创建队列文件时有效
   // 3. AIsCycle 属性表示当前打开的文件队列是否可以循环写, 即不检查队列是否满
   bool           Open(const KYString& AFileName,     Longword ATagSize  = 0,
                             Longword  AItemSize = 0, Longword AMaxCount = 0,
                                 bool  ACanCreate= true,  bool AIsCycle  = false);

   // 关闭队列文件
   void           Close();

   // 清除队列所有项
   bool           Clear();

   // 压入队列项数据: 数据尺寸 = Min(ASize, ItemSize)
   bool           Push(const void* AData, long ASize);

   // 预读取尾部队列项数据
   bool           PeekTail(void* AData, long& ASize);

   // 预读取队列项数据
   bool           Peek(void* AData, long& ASize);

   // 弹出队列项数据
   bool           Pop(void* AData, long& ASize);

   // 读取附加数据: ASize 传入要读取的尺寸, 返回实际读取的尺寸
   bool           ReadTag(long AOffset, void* AData, long& ASize);

   // 保存附加数据: ASize 传入要保存的尺寸, 返回实际保存的尺寸
   bool           WriteTag(long AOffset, const void* AData, long& ASize);

protected:
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

   bool           LockFile() const     { return FMutex->Lock(); }
   void           UnlockFile() const   { FMutex->Unlock(); }

   bool           IncRefCount();
   void           DecRefCount();

private:
   void           DoInit();
   bool           DoCheck(const THead& AHead);

   bool           DoOpen(const KYString& AFileName);
   bool           DoCreate(const KYString& AFileName, Longword ATagSize,
                                 Longword  AItemSize, Longword AMaxCount);

private:
   void*          FData;               // 自定义数据
   TKYMapFile*    FFile;               // 文件对象
   TKYCritSect*   FLock;               // 锁
   TKYMutex*      FMutex;              // 互斥元
   TIndex*        FIndex;              // 队列索引
   void*          FQueue;              // 队列起始项
   void*          FTag;                // 附加起始数据

   THead          FHead;               // 队列头信息
   Byte           FState;              // 队列状态
   bool           FIsCycle;            // 是否循环写
   Longword       FRefCount;           // 引用计数

public:
   // 静态成员
   static const Longword Item_MaxSize; // 队列项最大尺寸(=0x10000000)
   static const TVersion Ver_Current;  // 当前版本(3.0.0.0)
};

}

#endif
