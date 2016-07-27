// =======================================
// Unit   : map file (KYMapFile.h)
// Version: 3.0.2.0 (build 2012.07.17)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYMapFile_H_
#define _KYMapFile_H_

#include "KYFiles.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMapFile - 文件映像类 */

class TKYMapFile
{
public:
   TKYMapFile()                        { DoInit(); }
   virtual ~TKYMapFile()               { Close(); }

   // 属性
   long        Size() const            { return FSize; }       // default: 0
   bool        Opened() const          { return FMapMem != NULL; }
   void*       MapMem() const          { return FMapMem; }     // default: NULL
   THandle     Handle() const          { return FHandle; }     // default: NULL
   KYString    FileName() const        { return FFileName; }   // default: ""
   bool        ReadOnly() const        { return FReadOnly; }   // default: false

   // 打开映像文件
   // 1. 若 AReadOnly  == true 则 ACanCreate 自动更改为 false
   // 2. 若 ACanCreate == true 则 ASize 参数有效, 否则此参数为文件实际尺寸
   bool        Open(const KYString& AFileName,         long ASize      = 0,
                              bool  AReadOnly = false, bool ACanCreate = true);

   // 关闭映像文件
   void        Close();

   // 清除缓存到写入文件中
   void        Flush();

   // 预加载映像文件
   // 注: 为了把文件内容确保加载到内存映像中, 加快初次读取文件操作速度, 一般是在
   // 文件打开后调用。ASize <= 0 表示整个文件加载, 也可以预加载文件从头开始到指
   // 定 ASize 部分。
   bool        Preload(long ASize = 0);

protected:
private:
   void        DoInit();
   bool        DoCreate(const KYString& AFileName, long ASize);

private:
   long        FSize;                  // 映像尺寸
   void*       FMapMem;                // 映像内存
   THandle     FHandle;                // 映像句柄
   KYString    FFileName;              // 文件名
   bool        FReadOnly;              // 是否只读打开
};

}

#endif
