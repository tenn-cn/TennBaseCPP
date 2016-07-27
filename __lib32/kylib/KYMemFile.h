// =======================================
// Unit   : �ڴ��ļ���
// Version: 3.2.0.0 (build 2012.08.17)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYMemFile_H_
#define _KYMemFile_H_

#include "KYAVLTree.h"
#include "KYMapFile.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYBaseMemFile - �ڴ��ļ����� */

class TKYBaseMemFile
{
public:
   TKYBaseMemFile();
   virtual ~TKYBaseMemFile();

   // ����
   long           Size() const         { return FSize; }       // default: 0
   bool           Opened() const       { return FOpened; }     // default: false
   Byte*          MemBase() const      { return FMemBase; }    // default: NULL
   KYString       FileName() const     { return FFileName; }   // default: ""
   bool           ReadOnly() const     { return FReadOnly; }   // default: false

   // ���ڴ��ļ�
   // 1. �� AReadOnly  == true �� ACanCreate �Զ�����Ϊ false
   // 2. �� ACanCreate == true �� ASize ������Ч, ����˲���Ϊ�ļ�ʵ�ʳߴ�
   virtual bool   Open(const KYString& AFileName, bool ACanCreate = false,
                                 long  ASize = 0, bool AReadOnly  = false) = 0;

   // �ر��ڴ��ļ�
   virtual void   Close() = 0;

   // д�ڴ浽�ļ�
   virtual bool   Write(long AOffset, long ASize) = 0;

   // д�ڴ��ļ�
   virtual bool   Write(long AOffset, const void* ABuffer, long ASize) = 0;

   // ������浽д���ļ���
   virtual void   Flush() = 0;

   // Ԥ����ӳ���ļ�
   // ע: Ϊ�˰��ļ�����ȷ�����ص�ӳ���ڴ���, �ӿ���ζ�ȡ�ļ������ٶ�, һ������
   // �ļ��򿪺���á�ASize <= 0 ��ʾ�����ļ�����, Ҳ����Ԥ�����ļ���ͷ��ʼ��ָ
   // �� ASize ���֡�
   virtual bool   Preload(long ASize = 0) = 0;

protected:
   // ִ�г�ʼ��
   void           DoInit();

protected:
   long           FSize;               // �ڴ�ߴ�
   bool           FOpened;             // �Ƿ��Ѵ�
   bool           FWrited;             // �Ƿ���д����
   bool           FReadOnly;           // �Ƿ�ֻ����
   Byte*          FMemBase;            // �ڴ����ַ
   KYString       FFileName;           // �ļ���

public:
   // ִ�д���ָ���ߴ���ļ�
   static bool    _DoCreate(const KYString& AFileName, long ASize);
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMemFile - �ڴ��ļ��� */

class TKYMemFile: public TKYBaseMemFile
{
public:
   TKYMemFile();
   virtual ~TKYMemFile();

   // ���ڴ��ļ�
   // 1. �� AReadOnly  == true �� ACanCreate �Զ�����Ϊ false
   // 2. �� ACanCreate == true �� ASize ������Ч, ����˲���Ϊ�ļ�ʵ�ʳߴ�
   virtual bool   Open(const KYString& AFileName, bool ACanCreate = false,
                                 long  ASize = 0, bool AReadOnly  = false);

   // �ر��ڴ��ļ�
   virtual void   Close();

   // д�ڴ浽�ļ�
   virtual bool   Write(long AOffset, long ASize);

   // д�ڴ��ļ�
   virtual bool   Write(long AOffset, const void* ABuffer, long ASize);

   // ������浽д���ļ���
   virtual void   Flush();

   // Ԥ����ӳ���ļ�
   // ע: Ϊ�˰��ļ�����ȷ�����ص�ӳ���ڴ���, �ӿ���ζ�ȡ�ļ������ٶ�, һ������
   // �ļ��򿪺���á�ASize <= 0 ��ʾ�����ļ�����, Ҳ����Ԥ�����ļ���ͷ��ʼ��ָ
   // �� ASize ���֡�
   virtual bool   Preload(long ASize = 0);

protected:
private:
   TFile*         FFile;               // �ļ�����
   KYString       FBuffer;             // �ļ�������
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYMemFileEx - ӳ���ڴ��ļ��� */

class TKYMemFileEx: public TKYBaseMemFile
{
public:
   TKYMemFileEx();
   virtual ~TKYMemFileEx();

   // ���ڴ��ļ�
   // 1. �� AReadOnly  == true �� ACanCreate �Զ�����Ϊ false
   // 2. �� ACanCreate == true �� ASize ������Ч, ����˲���Ϊ�ļ�ʵ�ʳߴ�
   virtual bool   Open(const KYString& AFileName, bool ACanCreate = false,
                                 long  ASize = 0, bool AReadOnly  = false);

   // �ر��ڴ��ļ�
   virtual void   Close();

   // д�ڴ浽�ļ�
   virtual bool   Write(long AOffset, long ASize);

   // д�ڴ��ļ�
   virtual bool   Write(long AOffset, const void* ABuffer, long ASize);

   // ������浽д���ļ���
   virtual void   Flush();

   // Ԥ����ӳ���ļ�
   // ע: Ϊ�˰��ļ�����ȷ�����ص�ӳ���ڴ���, �ӿ���ζ�ȡ�ļ������ٶ�, һ������
   // �ļ��򿪺���á�ASize <= 0 ��ʾ�����ļ�����, Ҳ����Ԥ�����ļ���ͷ��ʼ��ָ
   // �� ASize ���֡�
   virtual bool   Preload(long ASize = 0);

protected:
private:
   TKYMapFile*    FFile;               // �ļ�����
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYPartMemFile - �����ڴ��ļ��� */

class TKYPartMemFile: public TKYBaseMemFile
{
public:
   TKYPartMemFile();
   virtual ~TKYPartMemFile();

   // ����
   bool           LagSave() const      { return FLagSave; }    // default: true
   void           SetLagSave(bool AEnabled);

   // ���ڴ��ļ�
   // 1. �� AReadOnly  == true �� ACanCreate �Զ�����Ϊ false
   // 2. �� ACanCreate == true �� ASize ������Ч, ����˲���Ϊ�ļ�ʵ�ʳߴ�
   virtual bool   Open(const KYString& AFileName, bool ACanCreate = false,
                                 long  ASize = 0, bool AReadOnly  = false);

   // �ر��ڴ��ļ�
   virtual void   Close();

   // д�ڴ浽�ļ�
   virtual bool   Write(long AOffset, long ASize);

   // д�ڴ��ļ�
   virtual bool   Write(long AOffset, const void* ABuffer, long ASize);

   // ������浽д���ļ���
   virtual void   Flush();

   // Ԥ����ӳ���ļ�
   // ע: Ϊ�˰��ļ�����ȷ�����ص�ӳ���ڴ���, �ӿ���ζ�ȡ�ļ������ٶ�, һ������
   // �ļ��򿪺���á�ASize <= 0 ��ʾ�����ļ�����, Ҳ����Ԥ�����ļ���ͷ��ʼ��ָ
   // �� ASize ���֡�
   virtual bool   Preload(long ASize = 0);

   // ׼��ָ���ļ�λ�óߴ������
   // ע: �� ASize <= 0 ��ʾ��ָ��λ�õ�ĩβ����������, ���ݰ� 1KB ����
   bool           Prepare(long AOffset, long ASize);

   // ˢ��ָ���ļ�λ�óߴ������
   // ע: �� ASize <= 0 ��ʾ��ָ��λ�õ�ĩβ����������, ���ݰ� 1KB ����
   bool           Refresh(long AOffset, long ASize);

protected:
   // ������
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

private:
   // д�ͺ󱣴����ط���
   void           DoSave();
   bool           DoWrite(long AOffset, long ASize);

private:
   TKYCritSect*   FLock;               // ������
   TFile*         FFile;               // �ļ�����
   TKYAVLTree*    FBlocks;             // д���б�
   KYString       FBuffer;             // �ļ�������
   KYString       FMapBuff;            // �ļ�ӳ�������
   Byte*          FMapRead;            // �ļ���ȡӳ���
   bool           FLagSave;            // д�ͺ󱣴�ģʽ
};

}

#endif
