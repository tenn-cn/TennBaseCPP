// =======================================
// Unit   : HQLib 公共单元
// Version: 3.0.0.1 (build 2011.07.08)
// Author : Kyee Ye
// Email  : kyee(at)hopechart.com
// Copyright (C) Hopechart Co., Ltd.
// =======================================

#ifndef _HQUtils_H_
#define _HQUtils_H_

#include "KYLib.h"

// HQLib 的命名空间
namespace HQLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 取 SEH 异常代码(只适用于 windows 下) */

// 异常忽略
#define  _ExceptIgnore     EXCEPTION_EXECUTE_HANDLER

// 异常码
#define  _ExceptCode(code) (code = _exception_code(), _ExceptIgnore)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 常量定义 */

// 位的掩码
const Byte        Mask_Bits[8]   = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
const Byte        Mask_NotBits[8]= {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};

// Boolean -> String
const char* const Bool_Str[2]    = {"false", "true"};
const KYString    Bool_String[2] = {"false", "true"};

// Boolean -> Success
const char* const Bool_Success[2]= {"failure", "success"};

// 日志头分隔线
const KYString    Line_LogHeader = "----+----+----+----+----+----+----+----+"
                                   "----+----+----+----+----+----+----+----+"
                                   "----+----+----+----+";

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 类型定义 */

// 版本信息
#pragma pack(push, 1)
typedef struct
{
   Byte           Major;            // 主版本
   Byte           Minor;            // 小版本
   Byte           Release;          // 发布小版本
   Byte           Build;            // 编译小版本
} TVersion, *PVersion;
#pragma pack(pop)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 全局变量 */

// 应用程序相关变量
extern long       App_Version;      // 版本号(如: "3.0.0.0" 值为 0x03000000), 默认值: 0
extern KYString   App_VerInfo;      // 版本信息(格式: "x.x.x.x (build yyyy.mm.dd)"), 默认值: ""
extern KYString   App_LogRoot;      // 日志根路径(末尾带路径分隔符), 默认值: ""
extern KYString   App_ProjName;     // 工程名(如: "test.dll"), 默认值: ""

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 公用函数 */

// 初始化/释放单元函数
void        InitHQUtils();
void        FreeHQUtils();

// 写全局日志: App_LogRoot + ChangeFileExt(App_ProjName, ".log");
// 注: 必须已调用过 InitHQUtils 初始化单元函数, 且使用 App_Version 变量
bool        _WriteLog(const KYString& ALine, bool AIncTime = true);

// 截断到指定长度的字符串
// 返回值: <截断后字符串> + '...'
void        TruncStr(KYString& AStr, long ALen);
void        TruncStr(const KYString& AStr, char* AValue, long& ALen);

// 取固定尺寸的字符串(注: AStr != NULL)
KYString    GetFixedStr(const char* AStr, Byte AMaxLen);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 版本相关函数 */

// 合成版本值
TVersion    Version(Byte AMajor, Byte AMinor, Byte ARelease, Byte ABuild);

// Version <-> String
KYString    VerToString(const TVersion& AVersion);
TVersion    StringToVer(const KYString& AStr);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 设备号相关函数(注: DevNo 的格式 yymmddxxxx) */

// 生成 DevID (注: AYear > 2000, AMonth in [1..12], ADay in [1..31])
long        BuildDevID(Word AYear, Word AMonth, Word ADay, Word AOrder);

// 分解 DevID
void        ParseDevID(long ADevID, Word& AYear, Word& AMonth, Word& ADay, Word& AOrder);

// DevID -> String
KYString    DevIDToString(long ADevID);

// String -> DevID (注: 若返回值为 0 表示失败)
long        StringToDevID(const KYString& ADevNo);

// DevID -> Int64
__int64     DevIDToInt64(long ADevID);

// Int64 -> DevID (注: 若返回值为 0 表示失败)
long        Int64ToDevID(__int64 ADevNo);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 日期相关函数(注: 整型值格式 yyyymmdd) */

// Date <-> Integer
long        Date2Int(TDateTime ADate);
bool        Int2Date(long AValue, TDateTime& ADate);
TDateTime   Int2Date(long AValue);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 时间(time_t)相关函数 */

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
