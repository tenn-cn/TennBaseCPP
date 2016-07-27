// =======================================
// Unit   : ini-file �� (KYIniFiles.h)
// Version: 3.0.0.0 (build 2011.03.08)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYIniFiles_H_
#define _KYIniFiles_H_

#include "KYList.h"
#include "KYDateTime.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TCustomIniFile - ini-file ���� */

class TCustomIniFile
{
public:
   TCustomIniFile(const KYString& AFileName = "");
   virtual ~TCustomIniFile();

   // �ļ���
   KYString          FileName() const  { return FFileName; }

   // �ж� ASection ���Ƿ����
   bool              SectionExists(const KYString& ASection);

   // �ж� ASection �� AIdent ���Ƿ����
   bool              ValueExists(const KYString& ASection,
                                 const KYString& AIdent);

   // ��ȡ ASection �� AIdent �����ַ���ֵ, �����������򷵻�Ĭ��ֵ
   virtual KYString  ReadString(const KYString& ASection,
                                const KYString& AIdent,
                                const KYString& ADefault) = 0;

   // ��ȡ ASection �� AIdent ��������ֵ, ���������ڻ�ֵת��ʧ���򷵻�Ĭ��ֵ
   virtual long      ReadInteger(const KYString& ASection,
                                 const KYString& AIdent, long ADefault);

   // ��ȡ ASection �� AIdent ���Ĳ���ֵ, ���������ڻ�ֵת��ʧ���򷵻�Ĭ��ֵ
   virtual bool      ReadBool(const KYString& ASection,
                              const KYString& AIdent, bool ADefault);

   // ��ȡ ASection �� AIdent ���ĸ���ֵ, ���������ڻ�ֵת��ʧ���򷵻�Ĭ��ֵ
   virtual double    ReadFloat(const KYString& ASection,
                               const KYString& AIdent, double ADefault);

   // ��ȡ ASection �� AIdent ��������ֵ, ���������ڻ�ֵת��ʧ���򷵻�Ĭ��ֵ
   virtual TDateTime ReadDate(const KYString& ASection,
                              const KYString& AIdent, TDateTime ADefault);

   // ��ȡ ASection �� AIdent ����ʱ��ֵ, ���������ڻ�ֵת��ʧ���򷵻�Ĭ��ֵ
   virtual TDateTime ReadTime(const KYString& ASection,
                              const KYString& AIdent, TDateTime ADefault);

   // ��ȡ ASection �� AIdent ��������ʱ��ֵ, ���������ڻ�ֵת��ʧ���򷵻�Ĭ��ֵ
   virtual TDateTime ReadDateTime(const KYString& ASection,
                                  const KYString& AIdent, TDateTime ADefault);

   // ���� ASection �� AIdent �����ַ���ֵ, �����������򴴽���, дʧ���򷵻� false
   virtual bool      WriteString(const KYString& ASection,
                                 const KYString& AIdent,
                                 const KYString& AValue) = 0;

   // ���� ASection �� AIdent ��������ֵ, �����������򴴽���, дʧ���򷵻� false
   virtual bool      WriteInteger(const KYString& ASection,
                                  const KYString& AIdent, long AValue);

   // ���� ASection �� AIdent ���Ĳ���ֵ, �����������򴴽���, дʧ���򷵻� false
   virtual bool      WriteBool(const KYString& ASection,
                               const KYString& AIdent, bool AValue);

   // ���� ASection �� AIdent ���ĸ���ֵ, �����������򴴽���, дʧ���򷵻� false
   virtual bool      WriteFloat(const KYString& ASection,
                                const KYString& AIdent, double AValue);

   // ���� ASection �� AIdent ��������ֵ, �����������򴴽���, дʧ���򷵻� false
   virtual bool      WriteDate(const KYString& ASection,
                               const KYString& AIdent, TDateTime AValue);

   // ���� ASection �� AIdent ����ʱ��ֵ, �����������򴴽���, дʧ���򷵻� false
   virtual bool      WriteTime(const KYString& ASection,
                               const KYString& AIdent, TDateTime AValue);

   // ���� ASection �� AIdent ��������ʱ��ֵ, �����������򴴽���, дʧ���򷵻� false
   virtual bool      WriteDateTime(const KYString& ASection,
                                   const KYString& AIdent, TDateTime AValue);

   // ��ȡ ASection �ڵļ����б�
   virtual bool      ReadSection(const KYString& ASection,
                                  TKYStringList& AList) = 0;

   // ��ȡ ini �ļ��Ľ����б�
   virtual bool      ReadSections(TKYStringList& AList) = 0;

   // ��ȡ ASection �ڵļ���(Name=Value)�б�
   virtual bool      ReadSectionValues(const KYString& ASection,
                                        TKYStringList& AList) = 0;

   // ɾ�� ASection �ڼ����ڴ˽ڵ����м���
   virtual bool      EraseSection(const KYString& ASection) = 0;

   // ɾ�� ASection �ڵ� AIdent ����
   virtual bool      DeleteKey(const KYString& ASection,
                               const KYString& AIdent) = 0;

   // �����ļ�
   virtual bool      UpdateFile() = 0;

protected:
   void              _SetFileName(const KYString& AFileName)
                     { FFileName = AFileName; }

private:
   KYString          FFileName;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TIniFile - Encapsulates the Windows INI file interface
             (Get/SetPrivateProfileXXX functions) */

class TIniFile: public TCustomIniFile
{
public:
   TIniFile(const KYString& AFileName = ""): TCustomIniFile(AFileName) {}

   // ��ȡ ASection �� AIdent �����ַ���ֵ, �����������򷵻�Ĭ��ֵ
   virtual KYString  ReadString(const KYString& ASection,
                                const KYString& AIdent,
                                const KYString& ADefault);

   // ���� ASection �� AIdent �����ַ���ֵ, �����������򴴽���, дʧ���򷵻� false
   virtual bool      WriteString(const KYString& ASection,
                                 const KYString& AIdent,
                                 const KYString& AValue);

   // ��ȡ ASection �ڵļ����б�
   virtual bool      ReadSection(const KYString& ASection,
                                  TKYStringList& AList);

   // ��ȡ ini �ļ��Ľ����б�
   virtual bool      ReadSections(TKYStringList& AList);

   // ��ȡ ASection �ڵļ���(Name=Value)�б�
   virtual bool      ReadSectionValues(const KYString& ASection,
                                        TKYStringList& AList);

   // ɾ�� ASection �ڼ����ڴ˽ڵ����м���
   virtual bool      EraseSection(const KYString& ASection);

   // ɾ�� ASection �ڵ� AIdent ����
   virtual bool      DeleteKey(const KYString& ASection,
                               const KYString& AIdent);

   // �����ļ�
   virtual bool      UpdateFile();

protected:
private:
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TMemIniFile - loads and entire ini file into memory and allows all operations
                 to be performed on the memory image.  The image can then be
                 written out to the disk file */

class TMemIniFile: public TCustomIniFile
{
public:
   TMemIniFile(const KYString& AFileName = "");
   virtual ~TMemIniFile();

   // �����Ƿ�����
   bool              Sorted() const    { return FSections->Sorted(); }  // default: false

   // ������Сд����
   bool              CaseSensitive() const
                     { return FSections->CaseSensitive(); }             // default: false

   // �����Ƿ�����
   void              SetSorted(bool ASorted)
                     { FSections->SetSorted(ASorted); }

   // ���ý�����Сд����
   void              SetCaseSensitive(bool ACaseSensitive)
                     { FSections->SetCaseSensitive(ACaseSensitive); }

   // ������нڼ�����
   void              Clear();

   // ���� FileName ����, ���Ƿ����¼���
   bool              Rename(const KYString& AFileName, bool AReload);

   // ��ȡ���нڼ�����
   bool              GetStrings(TKYStringList& AList);

   // �������нڼ�����
   bool              SetStrings(const TKYStringList& AList);

   // ��ȡ ASection �� AIdent �����ַ���ֵ, �����������򷵻�Ĭ��ֵ
   virtual KYString  ReadString(const KYString& ASection,
                                const KYString& AIdent,
                                const KYString& ADefault);

   // ���� ASection �� AIdent �����ַ���ֵ, �����������򴴽���, дʧ���򷵻� false
   virtual bool      WriteString(const KYString& ASection,
                                 const KYString& AIdent,
                                 const KYString& AValue);

   // ��ȡ ASection �ڵļ����б�
   virtual bool      ReadSection(const KYString& ASection,
                                  TKYStringList& AList);

   // ��ȡ ini �ļ��Ľ����б�
   virtual bool      ReadSections(TKYStringList& AList);

   // ��ȡ ASection �ڵļ���(Name=Value)�б�
   virtual bool      ReadSectionValues(const KYString& ASection,
                                        TKYStringList& AList);

   // ɾ�� ASection �ڼ����ڴ˽ڵ����м���
   virtual bool      EraseSection(const KYString& ASection);

   // ɾ�� ASection �ڵ� AIdent ����
   virtual bool      DeleteKey(const KYString& ASection,
                               const KYString& AIdent);

   // �������нڼ���� ini �ļ���
   virtual bool      UpdateFile();

protected:
private:
   TKYStringList*    AddSection(const KYString& ASection);
   bool              LoadValues();

   void              DoDeletion(const KYString& AItem, void* AData);

private:
   TKYStringList*    FSections;
};

}

#endif
