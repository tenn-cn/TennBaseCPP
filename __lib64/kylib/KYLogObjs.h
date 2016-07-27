// =======================================
// Unit   : ��־���� (KYLogObjs.h)
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

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYLogObj - ��־������ */

class TKYLogObj
{
public:
   TKYLogObj(long AMaxLineCount = 10000, Byte AFileCount = 2,
                                         bool ACanLock   = false);
   virtual ~TKYLogObj();

   // ����
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

   // ��������
   void           SetData(void* AData) { FData = AData; }
   void           SetPath(const KYString& APath);
   void           SetPrefix(const KYString& APrefix);
   void           SetSuffix(const KYString& ASuffix);
   void           SetFileIndex(Byte AIndex)  { FFileIndex = AIndex; }
   void           SetIsChecked(bool AChecked){ FIsChecked = AChecked; }
   void           SetFileCount(Byte ACount, bool ANeedOpen = true);
   void           SetMaxLineCount(long ACount);

   // ����־
   // 1. <FileName> = <Path> + <Prefix> + <FileIndex> + <Suffix>
   // 2. �� Path ����Ϊ "", ���Զ����� Path = CurrModuleFilePath + "Log\\";
   void           Open();

   // �ر���־
   void           Close();

   // д����Ϣ
   void           Write(const KYString& ALine);

   // д��ʱ�������Ϣ
   void           WriteEx(TDateTime ATime, const KYString& ALine);

   // д��ʱ�估�߳�ID������Ϣ
   void           WriteID(TDateTime ATime, const KYString& ALine);

   // д��ʽ������Ϣ
   void           WriteFmt(KYString AFormat, ...);
   void           WriteFmt(const char* AFormat, ...);

   // д��ʱ��ĸ�ʽ������Ϣ
   void           WriteFmtEx(TDateTime ATime, KYString AFormat, ...);
   void           WriteFmtEx(TDateTime ATime, const char* AFormat, ...);

   // д��ʱ�估�߳�ID�ĸ�ʽ������Ϣ
   void           WriteFmtID(TDateTime ATime, KYString AFormat, ...);
   void           WriteFmtID(TDateTime ATime, const char* AFormat, ...);

   // �¼�, ���ļ�����ʱ�����¼�, �¼��ǿ�ʱ���Զ�д��һ����Ϣ
   TNotifyEvent   OnChanged;

protected:
   void           Lock() const         { if (FLock != NULL) FLock->Enter(); }
   void           Unlock() const       { if (FLock != NULL) FLock->Leave(); }

private:
   void           DoFull(void* Sender, long ALineCount);
   void           OpenFile();

private:
   void*          FData;               // �Զ�������
   TKYCritSect*   FLock;               // ��
   KYString       FPath;               // ��־·��
   KYString       FPrefix;             // �ļ���ǰ׺
   KYString       FSuffix;             // �ļ�����׺, Ĭ��ֵΪ ".txt"
   TKYDebugFile*  FFileObj;            // ��־�ļ�����
   Byte           FFileIndex;          // ��־�ļ�����
   Byte           FFileCount;          // ��־�ļ�����
   bool           FIsChecked;          // OpenFileʱ�Ƿ���ߴ�, Ĭ��ֵΪ false
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYLogObjs - ��־������ */

class TKYLogObjs
{
public:
   TKYLogObjs(long ADefLineCount = 10000, Byte ADefFileCount = 2);
   virtual ~TKYLogObjs();

   // ����
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

   // ��������
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

   // �������Ѿ��������־����, ������־�����־���е� FThread �߳���ִ��д
   void           Open();

   // �ر������Ѿ��������־����
   void           Close();

   // �½���־��
   Longword       New(const KYString& APath);

   // ɾ����־��
   void           Delete(Longword AID);

   // ���������־��
   void           Clear();

   // д����Ϣ
   bool           Write(Longword AID, const KYString& ALine);

   // д��ʱ�������Ϣ
   bool           WriteEx(Longword AID, TDateTime ATime, const KYString& ALine);

   // ���ļ�����ʱ�����¼�, �¼��ǿ�ʱ���Զ�д��һ����Ϣ
   // �����¼�������� New ��־��ǰ����
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
   // FItems/FDeads �� DoIntialize �� DoFinalize ����
   static void    DoInitItem(Pointer* AItem, Word ASize);
   static void    DoFreeItem(Pointer* AItem, Word ASize);

private:
   void*          FData;               // �Զ�������
   KYString       FRoot;               // ��·��
   KYString       FDefPrefix;          // Ĭ���ļ���ǰ׺
   KYString       FDefSuffix;          // Ĭ���ļ�����׺, Ĭ��ֵΪ ".txt"
   long           FDefLineCount;       // Ĭ����־�������
   Byte           FDefFileCount;       // Ĭ����־�ļ�����
   bool           FDefIsChecked;       // Ĭ��OpenFileʱ�Ƿ���ߴ�, Ĭ��ֵΪ false
   bool           FOpened;             // �Ƿ��Ѿ���

   TKYLockRW*     FLockRW;             // ��д��
   TKYRunThread*  FThread;             // д��־�߳�
   TKYMemHandles* FItems;              // ��־����
   TKYMemHandles* FDeads;              // �ͷ���־�����б� - TKYLogObj ����
};

}

#endif
