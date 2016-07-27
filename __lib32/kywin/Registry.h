// =======================================
// Unit   : 注册表类 (Registry.h)
// Version: 3.0.0.0 (build 2011.03.10)
// Author : Renhy
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _Registry_H_
#define _Registry_H_

#include "KYLib.h"

// KYWin 2.0 开始使用 KYWin 命名空间
namespace KYWin
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TRegistry - 注册表类 */

class TRegistry
{
public:
   // 数据类型
   enum TType {rdUnknown, rdString, rdExpandString, rdInteger, rdBinary};

   // 键盘信息
   typedef struct
   {
      int      NumSubKeys;
      int      MaxSubKeyLen;
      int      NumValues;
      int      MaxValueLen;
      int      MaxDataLen;
      FILETIME FileTime;
   } TKeyInfo;

   // 数据信息
   typedef struct
   {
      TType    RegData;
      int      DataSize;
   } TDataInfo;

public:
   TRegistry();
   TRegistry(DWORD AAccess);
   virtual ~TRegistry();

   HKEY        CurrentKey() const         { return FCurrentKey; }
   KYString    CurrentPath() const        { return FCurrentPath; }
   bool        LazyWrite() const          { return FLazyWrite; }
   HKEY        RootKey() const            { return FRootKey; }
   DWORD       Access() const             { return FAccess; }

   void        SetLazyWrite(bool AValue)  { FLazyWrite = AValue; }
   void        SetRootKey(HKEY AValue);
   void        SetAccess(DWORD AAccess)   { FAccess = AAccess; }

   void        CloseKey();
   bool        CreateKey(const KYString& AKey);
   bool        DeleteKey(const KYString& AKey);
   bool        DeleteValue(const KYString& AName);
   bool        GetDataInfo(const KYString& AValueName, TDataInfo& AValue);
   int         GetDataSize(const KYString& AValueName);
   TType       GetDataType(const KYString& AValueName);
   bool        GetKeyInfo(TKeyInfo& AValue);
   void        GetKeyNames(TKYStringList* AStrings);
   void        GetValueNames(TKYStringList* AStrings);
   bool        HasSubKeys();
   bool        KeyExists(const KYString& AKey);
   bool        LoadKey(const KYString& AKey, const KYString& AFileName);
   void        MoveKey(const KYString& AOldName,
                       const KYString& ANewName, bool ADelete);
   bool        OpenKey(const KYString& AKey, bool ACanCreate);
   bool        OpenKeyReadOnly(const KYString& AKey);
   __int64     ReadCurrency(const KYString& AName);
   int         ReadBinaryData(const KYString& AName, void* ABuffer, int ABufSize);
   bool        ReadBool(const KYString& AName);
   TDateTime   ReadDate(const KYString& AName);
   TDateTime   ReadDateTime(const KYString& AName);
   double      ReadFloat(const KYString& AName);
   int         ReadInteger(const KYString& AName);
   KYString    ReadString(const KYString& AName);
   TDateTime   ReadTime(const KYString& AName);
   bool        RegistryConnect(const KYString& AUNCName);
   void        RenameValue(const KYString& AOldName, const KYString& ANewName);
   bool        ReplaceKey(const KYString& AKey, const KYString& AFileName,
                          const KYString& ABackUpFileName);
   bool        RestoreKey(const KYString& AKey, const KYString& AFileName);
   bool        SaveKey(const KYString& AKey, const KYString& AFileName);
   bool        UnLoadKey(const KYString& AKey);
   bool        ValueExists(const KYString& AName);
   void        WriteCurrency(const KYString& AName, __int64 AValue);
   void        WriteBinaryData(const KYString& AName, const void* ABuffer,
                                                             int  ABufSize);
   void        WriteBool(const KYString& AName, bool AValue);
   void        WriteDate(const KYString& AName, TDateTime AValue);
   void        WriteDateTime(const KYString& AName, TDateTime AValue);
   void        WriteFloat(const KYString& AName, double AValue);
   void        WriteInteger(const KYString& AName, int AValue);
   void        WriteString(const KYString& AName, const KYString& AValue);
   void        WriteExpandString(const KYString& AName, const KYString& AValue);
   void        WriteTime(const KYString& AName, TDateTime AValue);

protected:
   void        ChangeKey(HKEY AValue, const KYString& APath);
   HKEY        GetBaseKey(bool ARelative);
   int         GetData(const KYString& AName, void* ABuffer, int ABufSize,
                                TType& ARegData);
   HKEY        GetKey(const KYString&  AKey);
   void        PutData(const KYString& AName, void* ABuffer, int ABufSize,
                                TType  ARegData);
   void        SetCurrentKey(HKEY AValue);

private:
   void        MoveValue(HKEY ASrcKey, HKEY ADestKey, const KYString AName);
   void        CopyValues(HKEY ASrcKey, HKEY ADestKey);
   void        CopyKeys(HKEY ASrcKey, HKEY ADestKey);

private:
   KYString    FCurrentPath;
   HKEY        FCurrentKey;
   HKEY        FRootKey;
   bool        FLazyWrite;
   bool        FCloseRootKey;
   DWORD       FAccess;
};

}

#endif
