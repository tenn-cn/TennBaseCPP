// =======================================
// Unit   : 队列文件 (KYQueueFile.h)
// Version: 3.0.0.0 (build 2011.03.09)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYQueueFile_H_
#define _KYQueueFile_H_

#include "KYFiles.h"
#include "KYSyncObj.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYQueueFile - 队列文件类 */

class TKYQueueFile
{
public:
   // 队列状态
   enum TState      {qsInactive,       // 未打开
                     qsOpening,        // 正在打开
                     qsClosing,        // 正在关闭
                     qsOpened};        // 已经打开

   // 版本信息
   #pragma pack(push, 1)
   typedef struct
   {
      Byte           Major;            // 主版本
      Byte           Minor;            // 小版本
      Byte           Release;          // 发布小版本
      Byte           Build;            // 编译小版本
   } TVersion, *PVersion;
   #pragma pack(pop)

protected:
   #pragma pack(push, 1)

   // 头信息
   typedef struct
   {
      TVersion       Version;          // 队列版本
      Longword       ItemSize;         // 队列项尺寸
      Longword       MaxCount;         // 最大项数
      Longword       IndexOrigin;      // 索引开始位置
      Longword       QueueOrigin;      // 队列开始位置
   } THead, *PHead;

   // 索引信息
   typedef struct
   {
      Longword       PushCount;        // 压入队列计数
      Longword       PushIndex;        // 压入队列索引
      Longword       PopCount;         // 弹出队列计数
      Longword       PopIndex;         // 弹出队列索引
   } TIndex, *PIndex;

   #pragma pack(pop)

public:
   TKYQueueFile();
   virtual ~TKYQueueFile();

   // 属性
   TState            State() const           { return FState; }            // default: qsInactive
   bool              IsCycle() const         { return FIsCycle; }          // default: false
   TVersion          Version() const         { return FHead.Version; }     // default: 0,0,0,0
   Longword          ItemSize() const        { return FHead.ItemSize; }    // default: 0
   Longword          MaxCount() const        { return FHead.MaxCount; }    // default: 0
   KYString          FileName() const        { return FFile->FileName(); } // default: ""

   Longword          PopIndex();       // 取 Pop 索引
   Longword          PushIndex();      // 取 Push 索引
   Longword          IdleCount();      // 取队列空闲项数
   Longword          Count();          // 取队列项数

   // 打开队列文件
   // 1. 若 ACanCreate == true 且文件不存在时, 则创建队列文件
   // 2. AItemSize 和 AMaxCount 只有创建队列文件时有效
   // 3. AIsCycle 属性表示当前打开的文件队列是否可以循环写
   bool              Open(const KYString& AFileName,
                                 Longword AItemSize   = 0,
                                 Longword AMaxCount   = 0,
                                     bool ACanCreate  = true,
                                     bool AIsCycle    = true);

   // 关闭队列文件
   void              Close();

   // 清除队列所有项
   bool              Clear();

   // 压入队列项数据
   bool              Push(const void* AData, long ASize);

   // 预读取尾部队列项数据
   bool              PeekTail(void* AData, long& ASize);

   // 预读取队列项数据
   bool              Peek(void* AData, long& ASize);

   // 弹出队列项数据
   bool              Pop(void* AData, long& ASize);

   // 清空文件的系统缓冲(不建议调用)
   void              Flush() const           { FFile->Flush(); }

protected:
   void              Lock() const            { FLock->Enter(); }
   void              Unlock() const          { FLock->Leave(); }

   bool              LockIndexRead() const;
   void              UnlockIndexRead() const;

   bool              LockIndexWrite() const;
   void              UnlockIndexWrite() const;

   void              _ReadIndex(TIndex& AIndex) const;
   void              _WriteIndex(const TIndex& AIndex) const;

   bool              IncRefCount();
   void              DecRefCount();

private:
   bool              CheckHead(const THead& AHead);
   bool              DoCreate(const KYString& AFileName,
                                     Longword AItemSize, Longword AMaxCount);

   void              DoInit();

private:
   TKYCritSect*      FLock;                  // 锁
   TFile*            FFile;                  // 队列文件对象指针
   THead             FHead;                  // 队列头信息
   TState            FState;                 // 队列状态
   bool              FIsCycle;               // 是否循环写
   Longword          FRefCount;              // 引用计数

public:
   // 静态成员
   static const Longword Item_MaxSize;       // 队列项最大尺寸(=0x10000000)
   static const TVersion Ver_Current;        // 当前版本
};

}

#endif
