// =======================================
// Unit   : ini-file 类 (KYIniFiles.h)
// Version: 3.0.0.0 (build 2011.03.08)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYIniFiles_H_
#define _KYIniFiles_H_

#include "KYList.h"
#include "KYDateTime.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TCustomIniFile - ini-file 基类 */

class TCustomIniFile
{
public:
   TCustomIniFile(const KYString& AFileName = "");
   virtual ~TCustomIniFile();

   // 文件名
   KYString          FileName() const  { return FFileName; }

   // 判断 ASection 节是否存在
   bool              SectionExists(const KYString& ASection);

   // 判断 ASection 节 AIdent 键是否存在
   bool              ValueExists(const KYString& ASection,
                                 const KYString& AIdent);

   // 读取 ASection 节 AIdent 键的字符串值, 若键不存在则返回默认值
   virtual KYString  ReadString(const KYString& ASection,
                                const KYString& AIdent,
                                const KYString& ADefault) = 0;

   // 读取 ASection 节 AIdent 键的整型值, 若键不存在或值转换失败则返回默认值
   virtual long      ReadInteger(const KYString& ASection,
                                 const KYString& AIdent, long ADefault);

   // 读取 ASection 节 AIdent 键的布尔值, 若键不存在或值转换失败则返回默认值
   virtual bool      ReadBool(const KYString& ASection,
                              const KYString& AIdent, bool ADefault);

   // 读取 ASection 节 AIdent 键的浮点值, 若键不存在或值转换失败则返回默认值
   virtual double    ReadFloat(const KYString& ASection,
                               const KYString& AIdent, double ADefault);

   // 读取 ASection 节 AIdent 键的日期值, 若键不存在或值转换失败则返回默认值
   virtual TDateTime ReadDate(const KYString& ASection,
                              const KYString& AIdent, TDateTime ADefault);

   // 读取 ASection 节 AIdent 键的时间值, 若键不存在或值转换失败则返回默认值
   virtual TDateTime ReadTime(const KYString& ASection,
                              const KYString& AIdent, TDateTime ADefault);

   // 读取 ASection 节 AIdent 键的日期时间值, 若键不存在或值转换失败则返回默认值
   virtual TDateTime ReadDateTime(const KYString& ASection,
                                  const KYString& AIdent, TDateTime ADefault);

   // 保存 ASection 节 AIdent 键的字符串值, 若键不存在则创建键, 写失败则返回 false
   virtual bool      WriteString(const KYString& ASection,
                                 const KYString& AIdent,
                                 const KYString& AValue) = 0;

   // 保存 ASection 节 AIdent 键的整型值, 若键不存在则创建键, 写失败则返回 false
   virtual bool      WriteInteger(const KYString& ASection,
                                  const KYString& AIdent, long AValue);

   // 保存 ASection 节 AIdent 键的布尔值, 若键不存在则创建键, 写失败则返回 false
   virtual bool      WriteBool(const KYString& ASection,
                               const KYString& AIdent, bool AValue);

   // 保存 ASection 节 AIdent 键的浮点值, 若键不存在则创建键, 写失败则返回 false
   virtual bool      WriteFloat(const KYString& ASection,
                                const KYString& AIdent, double AValue);

   // 保存 ASection 节 AIdent 键的日期值, 若键不存在则创建键, 写失败则返回 false
   virtual bool      WriteDate(const KYString& ASection,
                               const KYString& AIdent, TDateTime AValue);

   // 保存 ASection 节 AIdent 键的时间值, 若键不存在则创建键, 写失败则返回 false
   virtual bool      WriteTime(const KYString& ASection,
                               const KYString& AIdent, TDateTime AValue);

   // 保存 ASection 节 AIdent 键的日期时间值, 若键不存在则创建键, 写失败则返回 false
   virtual bool      WriteDateTime(const KYString& ASection,
                                   const KYString& AIdent, TDateTime AValue);

   // 读取 ASection 节的键名列表
   virtual bool      ReadSection(const KYString& ASection,
                                  TKYStringList& AList) = 0;

   // 读取 ini 文件的节名列表
   virtual bool      ReadSections(TKYStringList& AList) = 0;

   // 读取 ASection 节的键项(Name=Value)列表
   virtual bool      ReadSectionValues(const KYString& ASection,
                                        TKYStringList& AList) = 0;

   // 删除 ASection 节及属于此节的所有键项
   virtual bool      EraseSection(const KYString& ASection) = 0;

   // 删除 ASection 节的 AIdent 键项
   virtual bool      DeleteKey(const KYString& ASection,
                               const KYString& AIdent) = 0;

   // 更新文件
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

   // 读取 ASection 节 AIdent 键的字符串值, 若键不存在则返回默认值
   virtual KYString  ReadString(const KYString& ASection,
                                const KYString& AIdent,
                                const KYString& ADefault);

   // 保存 ASection 节 AIdent 键的字符串值, 若键不存在则创建键, 写失败则返回 false
   virtual bool      WriteString(const KYString& ASection,
                                 const KYString& AIdent,
                                 const KYString& AValue);

   // 读取 ASection 节的键名列表
   virtual bool      ReadSection(const KYString& ASection,
                                  TKYStringList& AList);

   // 读取 ini 文件的节名列表
   virtual bool      ReadSections(TKYStringList& AList);

   // 读取 ASection 节的键项(Name=Value)列表
   virtual bool      ReadSectionValues(const KYString& ASection,
                                        TKYStringList& AList);

   // 删除 ASection 节及属于此节的所有键项
   virtual bool      EraseSection(const KYString& ASection);

   // 删除 ASection 节的 AIdent 键项
   virtual bool      DeleteKey(const KYString& ASection,
                               const KYString& AIdent);

   // 更新文件
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

   // 节名是否排序
   bool              Sorted() const    { return FSections->Sorted(); }  // default: false

   // 节名大小写敏感
   bool              CaseSensitive() const
                     { return FSections->CaseSensitive(); }             // default: false

   // 节名是否排序
   void              SetSorted(bool ASorted)
                     { FSections->SetSorted(ASorted); }

   // 设置节名大小写敏感
   void              SetCaseSensitive(bool ACaseSensitive)
                     { FSections->SetCaseSensitive(ACaseSensitive); }

   // 清除所有节及键项
   void              Clear();

   // 更改 FileName 属性, 及是否重新加载
   bool              Rename(const KYString& AFileName, bool AReload);

   // 读取所有节及键项
   bool              GetStrings(TKYStringList& AList);

   // 设置所有节及键项
   bool              SetStrings(const TKYStringList& AList);

   // 读取 ASection 节 AIdent 键的字符串值, 若键不存在则返回默认值
   virtual KYString  ReadString(const KYString& ASection,
                                const KYString& AIdent,
                                const KYString& ADefault);

   // 保存 ASection 节 AIdent 键的字符串值, 若键不存在则创建键, 写失败则返回 false
   virtual bool      WriteString(const KYString& ASection,
                                 const KYString& AIdent,
                                 const KYString& AValue);

   // 读取 ASection 节的键名列表
   virtual bool      ReadSection(const KYString& ASection,
                                  TKYStringList& AList);

   // 读取 ini 文件的节名列表
   virtual bool      ReadSections(TKYStringList& AList);

   // 读取 ASection 节的键项(Name=Value)列表
   virtual bool      ReadSectionValues(const KYString& ASection,
                                        TKYStringList& AList);

   // 删除 ASection 节及属于此节的所有键项
   virtual bool      EraseSection(const KYString& ASection);

   // 删除 ASection 节的 AIdent 键项
   virtual bool      DeleteKey(const KYString& ASection,
                               const KYString& AIdent);

   // 保存所有节及键项到 ini 文件中
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
