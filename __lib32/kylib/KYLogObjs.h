// =======================================
// Unit   : 日志对象集 (KYLogObjs.h)
// Version: 3.0.0.0 (build 2012.06.02)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYLogObjs_H_
#define _KYLogObjs_H_

#include "KYFiles.h"
#include "KYMemBlock.h"
#include "KYRunThreads.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYLogObj - 日志对象类 */

class TKYLogObj
{
public:
   TKYLogObj(long AMaxLineCount = 10000, Byte AFileCount = 2,
                                         bool ACanLock   = false);
   virtual ~TKYLogObj();

   // 属性
   void*          Data() const         { return FData; }                   // default: NULL
   KYString       Path() const         { return FPath; }                   // default: ""
   KYString       Prefix() const       { return FPrefix; }                 // default: ""
   KYString       Suffix() const       { return FSuffix; }                 // default: ".txt"
   KYString       FileName() const     { return FFileObj->FileName(); }    // default: ""

   bool           Opened() const       { return FFileObj->Opened(); }      // default: false
   bool           IsChecked() const    { return FIsChecked; }              // default: false
   Byte           FileIndex() const    { return FFileIndex; }              // default: 1
   Byte           FileCount() const    { return FFileCount; }              // default: 2
   long           LineCount() const    { return FFileObj->LineCount(); }   // default: 0
   long           MaxLineCount() const { return FFileObj->MaxLineCount(); }// default: 10000

   // 设置属性
   void           SetData(void* AData) { FData = AData; }
   void           SetPath(const KYString& APath);
   void           SetPrefix(const KYString& APrefix);
   void           SetSuffix(const KYString& ASuffix);
   void           SetFileIndex(Byte AIndex)  { FFileIndex = AIndex; }
   void           SetIsChecked(bool AChecked){ FIsChecked = AChecked; }
   void           SetFileCount(Byte ACount, bool ANeedOpen = true);
   void           SetMaxLineCount(long ACount);

   // 打开日志
   // 1. <FileName> = <Path> + <Prefix> + <FileIndex> + <Suffix>
   // 2. 若 Path 属性为 "", 则自动设置 Path = CurrModuleFilePath + "Log\\";
   void           Open();

   // 关闭日志
   void           Close();

   // 写行信息
   void           Write(const KYString& ALine);

   // 写带时间的行信息
   void           WriteEx(TDateTime ATime, const KYString& ALine);

   // 写带时间及线程ID的行信息
   void           WriteID(TDateTime ATime, const KYString& ALine);

   // 写格式化行信息
   void           WriteFmt(KYString AFormat, ...);
   void           WriteFmt(const char* AFormat, ...);

   // 写带时间的格式化行信息
   void           WriteFmtEx(TDateTime ATime, KYString AFormat, ...);
   void           WriteFmtEx(TDateTime ATime, const char* AFormat, ...);

   // 写带时间及线程ID的格式化行信息
   void           WriteFmtID(TDateTime ATime, KYString AFormat, ...);
   void           WriteFmtID(TDateTime ATime, const char* AFormat, ...);

   // 事件, 当文件更换时激发事件, 事件非空时不自动写第一行信息
   TNotifyEvent   OnChanged;

protected:
   void           Lock() const         { if (FLock != NULL) FLock->Enter(); }
   void           Unlock() const       { if (FLock != NULL) FLock->Leave(); }

private:
   void           DoFull(void* Sender, long ALineCount);
   void           OpenFile();

private:
   void*          FData;               // 自定义数据
   TKYCritSect*   FLock;               // 锁
   KYString       FPath;               // 日志路径
   KYString       FPrefix;             // 文件名前缀
   KYString       FSuffix;             // 文件名后缀, 默认值为 ".txt"
   TKYDebugFile*  FFileObj;            // 日志文件对象
   Byte           FFileIndex;          // 日志文件索引
   Byte           FFileCount;          // 日志文件个数
   bool           FIsChecked;          // OpenFile时是否检查尺寸, 默认值为 false
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYLogObjs - 日志对象集类 */

class TKYLogObjs
{
public:
   TKYLogObjs(long ADefLineCount = 10000, Byte ADefFileCount = 2);
   virtual ~TKYLogObjs();

   // 属性
   bool           Opened() const       { return FOpened; }        // default: false
   void*          Data() const         { return FData; }          // default: false
   KYString       Root() const         { return FRoot; }          // default: ""
   KYString       DefPrefix() const    { return FDefPrefix; }     // default: ""
   KYString       DefSuffix() const    { return FDefSuffix; }     // default: ".txt"
   bool           DefIsChecked() const { return FDefIsChecked; }  // default: false
   long           DefLineCount() const { return FDefLineCount; }  // default: 10000
   Byte           DefFileCount() const { return FDefFileCount; }  // default: 2

   bool           Opened(Longword AID) const;                     // default: false
   KYString       Path(Longword AID) const;                       // default: Root()
   Byte           FileCount(Longword AID) const;                  // default: DefFileCount()
   long           MaxLineCount(Longword AID) const;               // default: DefLineCount()

   // 设置属性
   void           SetData(void* AData) { FData = AData; }
   void           SetRoot(const KYString& ARoot);
   void           SetDefPrefix(const KYString& APrefix);
   void           SetDefSuffix(const KYString& ASuffix);
   void           SetDefLineCount(long ACount);
   void           SetDefFileCount(Byte ACount);
   void           SetDefIsChecked(bool AChecked);

   bool           SetPath(Longword AID, const KYString& APath);
   bool           SetPrefix(Longword AID, const KYString& APrefix);
   bool           SetSuffix(Longword AID, const KYString& ASuffix);
   bool           SetIsChecked(Longword AID, bool AChecked);
   bool           SetFileCount(Longword AID, Byte ACount);
   bool           SetMaxLineCount(Longword AID, long ACount);

   // 打开所有已经加入的日志对象, 所有日志项都由日志集中的 FThread 线程来执行写
   void           Open();

   // 关闭所有已经加入的日志对象
   void           Close();

   // 新建日志项
   Longword       New(const KYString& APath);

   // 删除日志项
   void           Delete(Longword AID);

   // 清除所有日志项
   void           Clear();

   // 写行信息
   bool           Write(Longword AID, const KYString& ALine);

   // 写带时间的行信息
   bool           WriteEx(Longword AID, TDateTime ATime, const KYString& ALine);

   // 当文件更换时激发事件, 事件非空时不自动写第一行信息
   // 若有事件则必须在 New 日志项前设置
   TNotifyEvent   OnChanged;

protected:
   void           LockRead() const     { FLockRW->LockRead(); }
   void           UnlockRead() const   { FLockRW->UnlockRead(); }

   void           LockWrite() const    { FLockRW->LockWrite(); }
   void           UnlockWrite() const  { FLockRW->UnlockWrite(); }

private:
   void           DoFreeLogObj(TKYLogObj* AObj);
   void           DoDeleteDead(Longword ADeadID);
   void           DoClearDeads();

private:
   // FItems/FDeads 的 DoIntialize 和 DoFinalize 函数
   static void    DoInitItem(Pointer* AItem, Word ASize);
   static void    DoFreeItem(Pointer* AItem, Word ASize);

private:
   void*          FData;               // 自定义数据
   KYString       FRoot;               // 根路径
   KYString       FDefPrefix;          // 默认文件名前缀
   KYString       FDefSuffix;          // 默认文件名后缀, 默认值为 ".txt"
   long           FDefLineCount;       // 默认日志最大行数
   Byte           FDefFileCount;       // 默认日志文件个数
   bool           FDefIsChecked;       // 默认OpenFile时是否检查尺寸, 默认值为 false
   bool           FOpened;             // 是否已经打开

   TKYLockRW*     FLockRW;             // 读写锁
   TKYRunThread*  FThread;             // 写日志线程
   TKYMemHandles* FItems;              // 日志对象集
   TKYMemHandles* FDeads;              // 释放日志对象列表 - TKYLogObj 对象
};

}

#endif
