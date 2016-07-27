// =======================================
// Unit   : audio or video file (KYAviFile.h)
// Version: 3.0.0.0 (build 2011.03.10)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYAviFile_H_
#define _KYAviFile_H_

#include "KYLib.h"
#include <vfw.h>

// KYWin 2.0 开始使用 KYWin 命名空间
namespace KYWin
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYAviFile - avi文件类 */

class TKYAviFile
{
public:
   TKYAviFile();
   virtual ~TKYAviFile();

   // 属性
   BITMAPINFO*    BmpInfo()            { return &FBmpInfo; }
   Longword       Handler() const      { return FHandler; }

   bool           Opened() const       { return FOpened; }
   bool           ReadOnly() const     { return FReadOnly; }
   KYString       FileName() const     { return FFileName; }

   long           Rate() const         { return FRate; }
   long           Scale() const        { return FScale; }
   long           Times() const        { return FTimes; }
   long           Index() const        { return FIndex; }
   long           Width() const        { return FWidth; }
   long           Height() const       { return FHeight; }
   long           Length() const       { return FLength; }
   long           MSecPerFrame() const { return FMSecPerFrame; }

   // 方法
   bool           Create(const KYString& AFileName, LPBITMAPINFOHEADER ABmpHeader,
                                    long AScale, long ARate,  Longword AHandler);
   bool           Open(const KYString& AFileName);
   void           Close();

   long           Seek(long AOffset, long AOrigin);
   long           Read(void* ABuffer, long ASize, bool& AKeyFrame);
   bool           Write(void* ABuffer, long ASize, bool AKeyFrame);

private:
   void           DoInit();

private:
   PAVIFILE       FFile;               // 文件句柄
   PAVISTREAM     FStream;             // 流句柄
   BITMAPINFO     FBmpInfo;            // 头信息
   Longword       FHandler;            // mmioFOURCC

   bool           FOpened;             // 是否打开
   bool           FReadOnly;           // 是否只读打开
   KYString       FFileName;           // 文件名

   long           FRate;               // 采样速率
   long           FScale;              // 采样比率(每秒帧数 = Rate / Scale)
   long           FTimes;              // 时间(毫秒)
   long           FIndex;              // 帧索引
   long           FWidth;              // 宽度
   long           FHeight;             // 高度
   long           FLength;             // 帧数
   long           FMSecPerFrame;       // 每帧毫秒数
};

}

#endif
