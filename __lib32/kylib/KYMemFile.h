// =======================================
// Unit   : 内存文件类
// Version: 3.2.0.0 (build 2012.08.17)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYMemFile_H_
#define _KYMemFile_H_

#include "KYAVLTree.h"
#include "KYMapFile.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYBaseMemFile - 内存文件基类 */

class TKYBaseMemFile
{
public:
   TKYBaseMemFile();
   virtual ~TKYBaseMemFile();

   // 属性
   long           Size() const         { return FSize; }       // default: 0
   bool           Opened() const       { return FOpened; }     // default: false
   Byte*          MemBase() const      { return FMemBase; }    // default: NULL
   KYString       FileName() const     { return FFileName; }   // default: ""
   bool           ReadOnly() const     { return FReadOnly; }   // default: false

   // 打开内存文件
   // 1. 若 AReadOnly  == true 则 ACanCreate 自动更改为 false
   // 2. 若 ACanCreate == true 则 ASize 参数有效, 否则此参数为文件实际尺寸
   virtual bool   Open(const KYString& AFileName, bool ACanCreate = false,
                                 long  ASize = 0, bool AReadOnly  = false) = 0;

   // 关闭内存文件
   virtual void   Close() = 0;

   // 写内存到文件
   virtual bool   Write(long AOffset, long ASize) = 0;

   // 写内存文件
   virtual bool   Write(long AOffset, const void* ABuffer, long ASize) = 0;

   // 清除缓存到写入文件中
   virtual void   Flush() = 0;

   // 预加载映像文件
   // 注: 为了把文件内容确保加载到映像内存中, 加快初次读取文件操作速度, 一般是在
   // 文件打开后调用。ASize <= 0 表示整个文件加载, 也可以预加载文件从头开始到指
   // 定 ASize 部分。
   virtual bool   Preload(long ASize = 0) = 0;

protected:
   // 执行初始化
   void           DoInit();

protected:
   long           FSize;               // 内存尺寸
   bool           FOpened;             // 是否已打开
   bool           FWrited;             // 是否已写数据
   bool           FReadOnly;           // 是否只读打开
   Byte*          FMemBase;            // 内存基地址
   KYString       FFileName;           // 文件名

public:
   // 执行创建指定尺寸的文件
   static bool    _DoCreate(const KYString& AFileName, long ASize);
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMemFile - 内存文件类 */

class TKYMemFile: public TKYBaseMemFile
{
public:
   TKYMemFile();
   virtual ~TKYMemFile();

   // 打开内存文件
   // 1. 若 AReadOnly  == true 则 ACanCreate 自动更改为 false
   // 2. 若 ACanCreate == true 则 ASize 参数有效, 否则此参数为文件实际尺寸
   virtual bool   Open(const KYString& AFileName, bool ACanCreate = false,
                                 long  ASize = 0, bool AReadOnly  = false);

   // 关闭内存文件
   virtual void   Close();

   // 写内存到文件
   virtual bool   Write(long AOffset, long ASize);

   // 写内存文件
   virtual bool   Write(long AOffset, const void* ABuffer, long ASize);

   // 清除缓存到写入文件中
   virtual void   Flush();

   // 预加载映像文件
   // 注: 为了把文件内容确保加载到映像内存中, 加快初次读取文件操作速度, 一般是在
   // 文件打开后调用。ASize <= 0 表示整个文件加载, 也可以预加载文件从头开始到指
   // 定 ASize 部分。
   virtual bool   Preload(long ASize = 0);

protected:
private:
   TFile*         FFile;               // 文件对象
   KYString       FBuffer;             // 文件缓冲区
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMemFileEx - 映像内存文件类 */

class TKYMemFileEx: public TKYBaseMemFile
{
public:
   TKYMemFileEx();
   virtual ~TKYMemFileEx();

   // 打开内存文件
   // 1. 若 AReadOnly  == true 则 ACanCreate 自动更改为 false
   // 2. 若 ACanCreate == true 则 ASize 参数有效, 否则此参数为文件实际尺寸
   virtual bool   Open(const KYString& AFileName, bool ACanCreate = false,
                                 long  ASize = 0, bool AReadOnly  = false);

   // 关闭内存文件
   virtual void   Close();

   // 写内存到文件
   virtual bool   Write(long AOffset, long ASize);

   // 写内存文件
   virtual bool   Write(long AOffset, const void* ABuffer, long ASize);

   // 清除缓存到写入文件中
   virtual void   Flush();

   // 预加载映像文件
   // 注: 为了把文件内容确保加载到映像内存中, 加快初次读取文件操作速度, 一般是在
   // 文件打开后调用。ASize <= 0 表示整个文件加载, 也可以预加载文件从头开始到指
   // 定 ASize 部分。
   virtual bool   Preload(long ASize = 0);

protected:
private:
   TKYMapFile*    FFile;               // 文件对象
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYPartMemFile - 部分内存文件类 */

class TKYPartMemFile: public TKYBaseMemFile
{
public:
   TKYPartMemFile();
   virtual ~TKYPartMemFile();

   // 属性
   bool           LagSave() const      { return FLagSave; }    // default: true
   void           SetLagSave(bool AEnabled);

   // 打开内存文件
   // 1. 若 AReadOnly  == true 则 ACanCreate 自动更改为 false
   // 2. 若 ACanCreate == true 则 ASize 参数有效, 否则此参数为文件实际尺寸
   virtual bool   Open(const KYString& AFileName, bool ACanCreate = false,
                                 long  ASize = 0, bool AReadOnly  = false);

   // 关闭内存文件
   virtual void   Close();

   // 写内存到文件
   virtual bool   Write(long AOffset, long ASize);

   // 写内存文件
   virtual bool   Write(long AOffset, const void* ABuffer, long ASize);

   // 清除缓存到写入文件中
   virtual void   Flush();

   // 预加载映像文件
   // 注: 为了把文件内容确保加载到映像内存中, 加快初次读取文件操作速度, 一般是在
   // 文件打开后调用。ASize <= 0 表示整个文件加载, 也可以预加载文件从头开始到指
   // 定 ASize 部分。
   virtual bool   Preload(long ASize = 0);

   // 准备指定文件位置尺寸的数据
   // 注: 若 ASize <= 0 表示从指定位置到末尾的所有数据, 数据按 1KB 对齐
   bool           Prepare(long AOffset, long ASize);

   // 刷新指定文件位置尺寸的数据
   // 注: 若 ASize <= 0 表示从指定位置到末尾的所有数据, 数据按 1KB 对齐
   bool           Refresh(long AOffset, long ASize);

protected:
   // 操作锁
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

private:
   // 写滞后保存的相关方法
   void           DoSave();
   bool           DoWrite(long AOffset, long ASize);

private:
   TKYCritSect*   FLock;               // 锁对象
   TFile*         FFile;               // 文件对象
   TKYAVLTree*    FBlocks;             // 写块列表
   KYString       FBuffer;             // 文件缓冲区
   KYString       FMapBuff;            // 文件映射表缓冲区
   Byte*          FMapRead;            // 文件读取映射表
   bool           FLagSave;            // 写滞后保存模式
};

}

#endif
