// =======================================
// Unit   : HQLib ������Ԫ
// Version: 3.0.0.1 (build 2011.07.08)
// Author : Kyee Ye
// Email  : kyee(at)hopechart.com
// Copyright (C) Hopechart Co., Ltd.
// =======================================

#ifndef _HQUtils_H_
#define _HQUtils_H_

#include "KYLib.h"

// HQLib �������ռ�
namespace HQLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ȡ SEH �쳣����(ֻ������ windows ��) */

// �쳣����
#define  _ExceptIgnore     EXCEPTION_EXECUTE_HANDLER

// �쳣��
#define  _ExceptCode(code) (code = _exception_code(), _ExceptIgnore)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �������� */

// λ������
const Byte        Mask_Bits[8]   = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
const Byte        Mask_NotBits[8]= {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};

// Boolean -> String
const char* const Bool_Str[2]    = {"false", "true"};
const KYString    Bool_String[2] = {"false", "true"};

// Boolean -> Success
const char* const Bool_Success[2]= {"failure", "success"};

// ��־ͷ�ָ���
const KYString    Line_LogHeader = "----+----+----+----+----+----+----+----+"
                                   "----+----+----+----+----+----+----+----+"
                                   "----+----+----+----+";

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ���Ͷ��� */

// �汾��Ϣ
#pragma pack(push, 1)
typedef struct
{
   Byte           Major;            // ���汾
   Byte           Minor;            // С�汾
   Byte           Release;          // ����С�汾
   Byte           Build;            // ����С�汾
} TVersion, *PVersion;
#pragma pack(pop)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ȫ�ֱ��� */

// Ӧ�ó�����ر���
extern long       App_Version;      // �汾��(��: "3.0.0.0" ֵΪ 0x03000000), Ĭ��ֵ: 0
extern KYString   App_VerInfo;      // �汾��Ϣ(��ʽ: "x.x.x.x (build yyyy.mm.dd)"), Ĭ��ֵ: ""
extern KYString   App_LogRoot;      // ��־��·��(ĩβ��·���ָ���), Ĭ��ֵ: ""
extern KYString   App_ProjName;     // ������(��: "test.dll"), Ĭ��ֵ: ""

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ���ú��� */

// ��ʼ��/�ͷŵ�Ԫ����
void        InitHQUtils();
void        FreeHQUtils();

// дȫ����־: App_LogRoot + ChangeFileExt(App_ProjName, ".log");
// ע: �����ѵ��ù� InitHQUtils ��ʼ����Ԫ����, ��ʹ�� App_Version ����
bool        _WriteLog(const KYString& ALine, bool AIncTime = true);

// �ضϵ�ָ�����ȵ��ַ���
// ����ֵ: <�ضϺ��ַ���> + '...'
void        TruncStr(KYString& AStr, long ALen);
void        TruncStr(const KYString& AStr, char* AValue, long& ALen);

// ȡ�̶��ߴ���ַ���(ע: AStr != NULL)
KYString    GetFixedStr(const char* AStr, Byte AMaxLen);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �汾��غ��� */

// �ϳɰ汾ֵ
TVersion    Version(Byte AMajor, Byte AMinor, Byte ARelease, Byte ABuild);

// Version <-> String
KYString    VerToString(const TVersion& AVersion);
TVersion    StringToVer(const KYString& AStr);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �豸����غ���(ע: DevNo �ĸ�ʽ yymmddxxxx) */

// ���� DevID (ע: AYear > 2000, AMonth in [1..12], ADay in [1..31])
long        BuildDevID(Word AYear, Word AMonth, Word ADay, Word AOrder);

// �ֽ� DevID
void        ParseDevID(long ADevID, Word& AYear, Word& AMonth, Word& ADay, Word& AOrder);

// DevID -> String
KYString    DevIDToString(long ADevID);

// String -> DevID (ע: ������ֵΪ 0 ��ʾʧ��)
long        StringToDevID(const KYString& ADevNo);

// DevID -> Int64
__int64     DevIDToInt64(long ADevID);

// Int64 -> DevID (ע: ������ֵΪ 0 ��ʾʧ��)
long        Int64ToDevID(__int64 ADevNo);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ������غ���(ע: ����ֵ��ʽ yyyymmdd) */

// Date <-> Integer
long        Date2Int(TDateTime ADate);
bool        Int2Date(long AValue, TDateTime& ADate);
TDateTime   Int2Date(long AValue);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ʱ��(time_t)��غ��� */

// TDateTime -> Tick(time_t)
inline Longword   TimeToTick(TDateTime ATime)
                  { return (Longword)((ATime - DT_1970_1_1) * 86400.0); }

// Tick(time_t) -> TDateTime
inline TDateTime  TickToTime(Longword ATick)
                  { return DT_1970_1_1 + ATick / 86400.0; }

// Tick(GetTickCount()) -> TDateTime
inline TDateTime  Tick2Time(Longword ATick)
{
   Longword dwCount = GetTickCount() - ATick;
   return Now() - dwCount / 86400000.0;
}

}

#endif
