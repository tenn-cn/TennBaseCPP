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

// KYWin 2.0 ��ʼʹ�� KYWin �����ռ�
namespace KYWin
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYAviFile - avi�ļ��� */

class TKYAviFile
{
public:
   TKYAviFile();
   virtual ~TKYAviFile();

   // ����
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

   // ����
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
   PAVIFILE       FFile;               // �ļ����
   PAVISTREAM     FStream;             // �����
   BITMAPINFO     FBmpInfo;            // ͷ��Ϣ
   Longword       FHandler;            // mmioFOURCC

   bool           FOpened;             // �Ƿ��
   bool           FReadOnly;           // �Ƿ�ֻ����
   KYString       FFileName;           // �ļ���

   long           FRate;               // ��������
   long           FScale;              // ��������(ÿ��֡�� = Rate / Scale)
   long           FTimes;              // ʱ��(����)
   long           FIndex;              // ֡����
   long           FWidth;              // ���
   long           FHeight;             // �߶�
   long           FLength;             // ֡��
   long           FMSecPerFrame;       // ÿ֡������
};

}

#endif
