// =======================================
// Unit   : wave file (KYWaveFile.h)
// Version: 3.0.0.0 (build 2011.03.10)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYWaveFile_H_
#define _KYWaveFile_H_

#include "KYLib.h"

// KYWin 2.0 开始使用 KYWin 命名空间
namespace KYWin
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYWaveHead - 语音文件头类 */

class TKYWaveHead
{
public:
   #pragma pack(push, 1)

   // Wave 文件头标记
   typedef struct
   {
      char           Flag[4];          // 'RIFF'
      Longword       Size;
      char           Wave[4];          // 'WAVE'
   } TTag, *PTag;

   // 注:
   //    'fmt ': 格式段
   //    'fact': 实际尺寸段
   //    'data': 数据段
   // Wave 文件段
   typedef struct
   {
      char           Sect[4];
      Longword       Size;
   } TSect, *PSect;

   // Wave 格式结构
   typedef struct
   {
      Word           FormatTag;        // 语音格式
      Word           Channels;         // 语音通道数
      Longword       SamplesPerSec;    // 每秒采样率(Hz)
      Longword       AvgBytesPerSec;   // 每秒采样平均字节数
      Word           BlockAlign;       // 每帧字节数, 对齐尺寸
      Word           BitsPerSample;    // 每个采样的bit数
      Word           NextSize;         // 紧接扩展数据尺寸, 默认为0
   } TWaveFmt, *PWaveFmt;

   // 通用 Wave 文件头, 固定格式 (58字节)
   typedef struct
   {
      TTag           Tag;              // 头标记
      TSect          FmtSect;          // 格式段
      TWaveFmt       WaveFmt;          // 语音格式, WaveFmt.NextSize = 0
      TSect          FactSect;         // 采样实况段
      Longword       FactSize;         // 采样数
      TSect          DataSect;         // 数据段
   } T_Custom;

   // ADPCM/GSM610 格式 Wave 文件头
   typedef struct
   {
      TTag           Tag;              // 头标记
      TSect          FmtSect;          // 格式段
      TWaveFmt       WaveFmt;          // 语音格式, WaveFmt.NextSize = 2
      Byte           NextData[2];      // 扩展数据
      TSect          FactSect;         // 采样实况段
      Longword       FactSize;         // 采样数
      TSect          DataSect;         // 数据段
   } T_ADPCM;

   // MP3 格式 Wave 文件头
   typedef struct
   {
      TTag           Tag;              // 头标记
      TSect          FmtSect;          // 格式段
      TWaveFmt       WaveFmt;          // 语音格式, WaveFmt.NextSize = 12
      Byte           NextData[12];     // 扩展数据
      TSect          FactSect;         // 采样实况段
      Longword       FactSize;         // 采样数
      TSect          DataSect;         // 数据段
   } T_MP3;

   #pragma pack(pop)

public:
   TKYWaveHead()                       {}
   virtual ~TKYWaveHead()              {}

public:
   // 初始化语音文件头信息
   static void       InitCustom(T_Custom& AHead, const TWaveFmt& AFormat);
   static void       InitADPCM(T_ADPCM& AHead, const TWaveFmt& AFormat,
                                               const Byte*     ANextData);
   static void       InitMP3(T_MP3& AHead, const TWaveFmt& AFormat,
                                           const Byte*     ANextData);

   // 语音格式值得到语音格式
   static TWaveFmt   Format2WaveFmt(Word AFormat);
   static KYString   Format2SectData(Word AFormat);

   // 根据语音格式计算数据采样数
   static Longword   DataSize2FactSize(Longword ADataSize, const TWaveFmt& AFormat);

   // 根据录音格式, 录音时长(毫秒)与录音数据尺寸转换函数
   static long       _Time2Size(long ATime, Word AFormat);
   static long       _Size2Time(long ASize, Word AFormat);

public:
   // 语音的头标记空值
   static const TTag     Tag_Null;

   // Wave 文件段空值
   static const TSect    FmtSect_Null;
   static const TSect    FactSect_Null;
   static const TSect    DataSect_Null;

   // 电话语音常用格式 {8000 HZ, 8 位, 单声道}:
   static const TWaveFmt WF_Null;      // 语音格式的空值
   static const TWaveFmt WF_ALaw;      // CCITT A-Law
   static const TWaveFmt WF_ULaw;      // CCITT U-Law
   static const TWaveFmt WF_PCM8;      // PCM8
   static const TWaveFmt WF_G729A;     // G.729A
   static const TWaveFmt WF_ADPCM;     // IMA ADPCM
   static const TWaveFmt WF_GSM610;    // GSM 6.10
   static const TWaveFmt WF_MP3;       // MP3

   // 常用格式的 NextData
   static const Byte     ND_ADPCM[2];  // IMA ADPCM 的 NextData
   static const Byte     ND_GSM610[2]; // GSM 6.10 的 NextData
   static const Byte     ND_MP3[12];   // MP3 的 NextData
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYWaveFile - 语音文件类 */

class TKYWaveFile
{
public:
   // 类型重定义
   typedef TKYWaveHead::TTag           TWaveTag;
   typedef TKYWaveHead::TSect          TWaveSect;
   typedef TKYWaveHead::TWaveFmt       TWaveFmt;

public:
   TKYWaveFile();
   virtual ~TKYWaveFile();

   // 属性
   __int64        Size() const         { return (FFile != NULL) ? FFile->Size() : 0; }
   bool           Opened() const       { return (FFile != NULL); }
   TWaveTag       WaveTag() const      { return FWaveTag; }
   TWaveFmt*      WaveFmt() const      { return FWaveFmt; }
   KYString       FmtInfo() const      { return FFmtInfo; }
   KYString       FileName() const     { return FFileName; }
   Longword       FactSize() const     { return FFactSize; }
   Longword       DataSize() const     { return FDataSize; }
   Word           BlockAlign() const   { return FBlockAlign; }
   long           BlockIndex() const   { return FBlockIndex; }
   long           BlockLength() const  { return FBlockLength; }

   // 方法
   bool           Open(const KYString& AFileName);
   void           Close();

   long           Seek(long AOffset, long AOrigin);
   long           Read(void* ABuffer, long ASize);

private:
   void           DoInit();

private:
   TFile*         FFile;               // 文件
   TWaveTag       FWaveTag;            // 文件头标志
   TWaveFmt*      FWaveFmt;            // 语音格式 = (TWaveFmt*)(char*)FFmtInfo
   KYString       FFmtInfo;            // 格式信息(TWaveFmt + NextData)
   KYString       FFileName;           // 文件名
   Longword       FFactSize;           // 采样数
   Longword       FDataSize;           // 数据尺寸
   long           FDataOrigin;         // 数据起始点
   Word           FBlockAlign;         // 每帧字节数, 对齐尺寸
   long           FBlockIndex;         // 语音帧索引
   long           FBlockLength;        // 语音帧长度

private:
   // 读取节信息
   static bool    ReadFmtSect(char* &APos, char* AEnd, KYString& AFmtInfo);
   static bool    ReadFactSect(char* &APos, char* AEnd, Longword& AFactSize);
   static bool    ReadDataSect(char* &APos, char* AEnd, Longword& ADataSize);
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYWaveStore - 语音文件存储类 */

class TKYWaveStore
{
public:
   // 分配语音文件头指针接口
   typedef void*  (*TNewHead)(long AFormat, long& AHeadSize);

   // 释放语音文件头指针接口
   typedef void   (*TDeleteHead)(void* AHead, long AFormat);

   // 更新语音文件头指针接口
   typedef void   (*TUpdateHead)(void* AHead, long AFormat,
                              Longword ADataSize, Longword ASamples);

public:
   TKYWaveStore();
   virtual ~TKYWaveStore();

   // 属性
   long           Format() const       { return FFormat; }
   void*          WaveHead() const     { return FWaveHead; }
   long           HeadSize() const     { return FHeadSize; }
   Longword       DataSize() const     { return FDataSize; }
   KYString       FileName() const     { return FFileName; }
   bool           Recording() const    { return (FFile != NULL); }

   // 方法
   bool           Begin(const KYString& AFileName, long AFormat);
   bool           End(Longword ASamples = 0);

   long           Write(const void* ABuffer, long ASize);

   // 回调函数接口
   TNewHead       NewHead;             // 默认为: DoNewHead
   TDeleteHead    DeleteHead;          // 默认为: DoDeleteHead
   TUpdateHead    UpdateHead;          // 默认为: DoUpdateHead

protected:
private:
   TFile*         FFile;               // 文件
   long           FFormat;             // 语音格式
   void*          FWaveHead;           // 文件头信息
   long           FHeadSize;           // 文件头尺寸
   Longword       FDataSize;           // 数据尺寸
   KYString       FFileName;           // 文件名

public:
   // 默认分配/释放/更新语音文件头接口
   static void*   DoNewHead(long AFormat, long& AHeadSize);
   static void    DoDeleteHead(void* AHead, long AFormat);
   static void    DoUpdateHead(void* AHead, long AFormat,
                            Longword ADataSize, Longword ASamples);
};

}

#endif
