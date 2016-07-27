// =======================================
// Unit   : �ļ��൥Ԫ (KYFiles.h)
// Version: 3.0.0.0 (build 2011.03.08)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYFiles_H_
#define _KYFiles_H_

#include "KYFileUtils.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TFile - �ļ��� */

class TFile
{
public:
   TFile();
   virtual ~TFile()                    { Close(); }

   // ����
   KYString FileName() const           { return FFileName; }   // default: ""
   THandle  Handle() const             { return FHandle; }     // default: INVALID_HANDLE_VALUE
   bool     Opened() const             { return FHandle != INVALID_HANDLE_VALUE; }
   __int64  Size() const               { return FileGetSize(FHandle); }

   // ���������ļ�, ���ɹ��򷵻� true
   bool     Create(const KYString& AFileName);

   // ���ļ�, AMode Ϊ KYFileUtils ��Ԫ�ж���ĳ���ֵ
   bool     Open(const KYString& AFileName, unsigned AMode);

   // �ر��ļ�
   void     Close();

   // �ļ�ָ�붨λ
   long     Seek(long AOffset, long AOrigin) const
            { return FileSeek(FHandle, AOffset, AOrigin); }
   __int64  Seek(__int64 AOffset, long AOrigin) const
            { return FileSeek(FHandle, AOffset, AOrigin); }

   // ��ȡ�ļ����ݵ���������
   long     Read(void* ABuffer, long ACount) const
            { return FileRead(FHandle, ABuffer, ACount); }

   // д���������ݵ��ļ���
   long     Write(const void* ABuffer, long ACount) const
            { return FileWrite(FHandle, ABuffer, ACount); }

   // ����ļ���ϵͳ����(���������)
   void     Flush() const              { FileFlush(FHandle); }

   // ���õ�ǰ�ļ�ָ��λ��Ϊ�ļ���β
   bool     SetEOF() const             { return FileSetEOF(FHandle); }

   // �ļ�ָ��Ķ�����
   bool     LockRead(Longword AOffset, Longword ACount) const
            { return FileLockRead(FHandle, AOffset, ACount); }
   bool     LockRead(const __int64 AOffset, const __int64 ACount) const
            { return FileLockRead(FHandle, AOffset, ACount); }

   // �ļ�ָ���д����
   bool     LockWrite(Longword AOffset, Longword ACount) const
            { return FileLockWrite(FHandle, AOffset, ACount); }
   bool     LockWrite(const __int64 AOffset, const __int64 ACount) const
            { return FileLockWrite(FHandle, AOffset, ACount); }

   // �ļ�ָ��Ķ�/д����
   bool     Unlock(Longword AOffset, Longword ACount) const
            { return FileUnlock(FHandle, AOffset, ACount); }
   bool     Unlock(const __int64 AOffset, const __int64 ACount) const
            { return FileUnlock(FHandle, AOffset, ACount); }

private:
   KYString FFileName;                 // �ļ���
   THandle  FHandle;                   // �ļ����
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYDebugFile - дDebug�ļ��� */

class TKYDebugFile
{
public:
   // OnDebugFull �¼�, �ļ�����д��ʱ�������¼�
   typedef void (TObject::*TDoDebugFull)(void* Sender, long ALineCount);
   typedef struct
   {
      TDoDebugFull   Method;
      void*          Object;
   } TOnDebugFull;

public:
   TKYDebugFile(long AMaxLineCount = 0);
   virtual ~TKYDebugFile()             { Close(); }

   // ����
   KYString FileName() const           { return FFile.FileName(); }  // default: ""
   bool     Opened() const             { return FFile.Opened(); }    // default: false
   long     LineCount() const          { return FLineCount; }        // default: 0
   long     MaxLineCount() const       { return FMaxLineCount; }     // default: 0

   // �����������
   void     SetMaxLineCount(long ACount);

   // ����־�ļ�, ARewrite Ϊ true ��ʾ���ԭ�ļ�����д
   bool     Open(const KYString& AFileName, bool ARewrite = true);

   // �ر���־�ļ�
   void     Close();

   // ����ļ���ϵͳ����(���������)
   void     Flush() const              { FFile.Flush(); }

   // д����Ϣ
   bool     Write(const KYString& ALine);

   // д��ʱ�������Ϣ
   bool     Write(TDateTime ATime, const KYString& ALine);

   // д��ʽ������Ϣ
   bool     WriteFmt(KYString AFormat, ...);
   bool     WriteFmt(const char* AFormat, ...);

   // д��ʱ��ĸ�ʽ������Ϣ
   bool     WriteFmt(TDateTime ATime, KYString AFormat, ...);
   bool     WriteFmt(TDateTime ATime, const char* AFormat, ...);

   // �¼�
   TOnDebugFull OnDebugFull;

protected:
   void     DoCheckDate(TDateTime ATime);
   void     DoWrite(const char* ALine, long ALength);

private:
   TFile    FFile;                     // �ļ�����
   long     FLastDate;                 // д���һ�ε�����
   long     FLineCount;                // ��ǰ����
   long     FMaxLineCount;             // �������, 0 ��ʾ����������
};

}

#endif