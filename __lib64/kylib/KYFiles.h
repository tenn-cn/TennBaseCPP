// =======================================
// Unit   : 文件类单元 (KYFiles.h)
// Version: 3.0.0.0 (build 2011.03.08)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYFiles_H_
#define _KYFiles_H_

#include "KYFileUtils.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TFile - 文件类 */

class TFile
{
public:
   TFile();
   virtual ~TFile()                    { Close(); }

   // 属性
   KYString FileName() const           { return FFileName; }   // default: ""
   THandle  Handle() const             { return FHandle; }     // default: INVALID_HANDLE_VALUE
   bool     Opened() const             { return FHandle != INVALID_HANDLE_VALUE; }
   __int64  Size() const               { return FileGetSize(FHandle); }

   // 创建并打开文件, 若成功则返回 true
   bool     Create(const KYString& AFileName);

   // 打开文件, AMode 为 KYFileUtils 单元中定义的常量值
   bool     Open(const KYString& AFileName, unsigned AMode);

   // 关闭文件
   void     Close();

   // 文件指针定位
   long     Seek(long AOffset, long AOrigin) const
            { return FileSeek(FHandle, AOffset, AOrigin); }
   __int64  Seek(__int64 AOffset, long AOrigin) const
            { return FileSeek(FHandle, AOffset, AOrigin); }

   // 读取文件数据到缓冲区中
   long     Read(void* ABuffer, long ACount) const
            { return FileRead(FHandle, ABuffer, ACount); }

   // 写缓冲区数据到文件中
   long     Write(const void* ABuffer, long ACount) const
            { return FileWrite(FHandle, ABuffer, ACount); }

   // 清空文件的系统缓冲(不建议调用)
   void     Flush() const              { FileFlush(FHandle); }

   // 设置当前文件指针位置为文件结尾
   bool     SetEOF() const             { return FileSetEOF(FHandle); }

   // 文件指针的读加锁
   bool     LockRead(Longword AOffset, Longword ACount) const
            { return FileLockRead(FHandle, AOffset, ACount); }
   bool     LockRead(const __int64 AOffset, const __int64 ACount) const
            { return FileLockRead(FHandle, AOffset, ACount); }

   // 文件指针的写加锁
   bool     LockWrite(Longword AOffset, Longword ACount) const
            { return FileLockWrite(FHandle, AOffset, ACount); }
   bool     LockWrite(const __int64 AOffset, const __int64 ACount) const
            { return FileLockWrite(FHandle, AOffset, ACount); }

   // 文件指针的读/写解锁
   bool     Unlock(Longword AOffset, Longword ACount) const
            { return FileUnlock(FHandle, AOffset, ACount); }
   bool     Unlock(const __int64 AOffset, const __int64 ACount) const
            { return FileUnlock(FHandle, AOffset, ACount); }

private:
   KYString FFileName;                 // 文件名
   THandle  FHandle;                   // 文件句柄
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYDebugFile - 写Debug文件类 */

class TKYDebugFile
{
public:
   // OnDebugFull 事件, 文件行数写满时激发的事件
   typedef void (TObject::*TDoDebugFull)(void* Sender, long ALineCount);
   typedef struct
   {
      TDoDebugFull   Method;
      void*          Object;
   } TOnDebugFull;

public:
   TKYDebugFile(long AMaxLineCount = 0);
   virtual ~TKYDebugFile()             { Close(); }

   // 属性
   KYString FileName() const           { return FFile.FileName(); }  // default: ""
   bool     Opened() const             { return FFile.Opened(); }    // default: false
   long     LineCount() const          { return FLineCount; }        // default: 0
   long     MaxLineCount() const       { return FMaxLineCount; }     // default: 0

   // 设置最大行数
   void     SetMaxLineCount(long ACount);

   // 打开日志文件, ARewrite 为 true 表示清空原文件并重写
   bool     Open(const KYString& AFileName, bool ARewrite = true);

   // 关闭日志文件
   void     Close();

   // 清空文件的系统缓冲(不建议调用)
   void     Flush() const              { FFile.Flush(); }

   // 写行信息
   bool     Write(const KYString& ALine);

   // 写带时间的行信息
   bool     Write(TDateTime ATime, const KYString& ALine);

   // 写格式化行信息
   bool     WriteFmt(KYString AFormat, ...);
   bool     WriteFmt(const char* AFormat, ...);

   // 写带时间的格式化行信息
   bool     WriteFmt(TDateTime ATime, KYString AFormat, ...);
   bool     WriteFmt(TDateTime ATime, const char* AFormat, ...);

   // 事件
   TOnDebugFull OnDebugFull;

protected:
   void     DoCheckDate(TDateTime ATime);
   void     DoWrite(const char* ALine, long ALength);

private:
   TFile    FFile;                     // 文件对象
   long     FLastDate;                 // 写最后一次的日期
   long     FLineCount;                // 当前行数
   long     FMaxLineCount;             // 最大行数, 0 表示行数无限制
};

}

#endif