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

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMapFile - �ļ�ӳ���� */

class TKYMapFile
{
public:
   TKYMapFile()                        { DoInit(); }
   virtual ~TKYMapFile()               { Close(); }

   // ����
   long        Size() const            { return FSize; }       // default: 0
   bool        Opened() const          { return FMapMem != NULL; }
   void*       MapMem() const          { return FMapMem; }     // default: NULL
   THandle     Handle() const          { return FHandle; }     // default: NULL
   KYString    FileName() const        { return FFileName; }   // default: ""
   bool        ReadOnly() const        { return FReadOnly; }   // default: false

   // ��ӳ���ļ�
   // 1. �� AReadOnly  == true �� ACanCreate �Զ�����Ϊ false
   // 2. �� ACanCreate == true �� ASize ������Ч, ����˲���Ϊ�ļ�ʵ�ʳߴ�
   bool        Open(const KYString& AFileName,         long ASize      = 0,
                              bool  AReadOnly = false, bool ACanCreate = true);

   // �ر�ӳ���ļ�
   void        Close();

   // ������浽д���ļ���
   void        Flush();

   // Ԥ����ӳ���ļ�
   // ע: Ϊ�˰��ļ�����ȷ�����ص��ڴ�ӳ����, �ӿ���ζ�ȡ�ļ������ٶ�, һ������
   // �ļ��򿪺���á�ASize <= 0 ��ʾ�����ļ�����, Ҳ����Ԥ�����ļ���ͷ��ʼ��ָ
   // �� ASize ���֡�
   bool        Preload(long ASize = 0);

protected:
private:
   void        DoInit();
   bool        DoCreate(const KYString& AFileName, long ASize);

private:
   long        FSize;                  // ӳ��ߴ�
   void*       FMapMem;                // ӳ���ڴ�
   THandle     FHandle;                // ӳ����
   KYString    FFileName;              // �ļ���
   bool        FReadOnly;              // �Ƿ�ֻ����
};

}

#endif
