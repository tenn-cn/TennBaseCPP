// =======================================
// Unit   : 放音和录音单元 (KYWavePlay.h)
// Version: 3.0.0.0 (build 2011.03.10)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYWavePlay_H_
#define _KYWavePlay_H_

#include <windows.h>
#include <mmsystem.h>
#include "KYLib.h"

// KYWin 2.0 开始使用 KYWin 命名空间
namespace KYWin
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 类型重定义 */

typedef  WAVEFORMATEX   TWaveFormatEx;
typedef  WAVEHDR        TWaveHdr, *PWaveHdr;
typedef  HWAVEOUT       HWaveOut;
typedef  HWAVEIN        HWaveIn;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYWaveBase - 录音放音公共基类 */

class TKYWaveBase
{
public:
   #pragma pack(push, 1)

   // Wave 文件头标记
   typedef struct
   {
      char           Flag[4];          // 'RIFF'
      Longword       Size;
      char           Wave[4];          // 'WAVE'
   } TWaveHeadTag;

   // 注:
   //    'fmt ': 格式段
   //    'fact': 实际尺寸段
   //    'data': 数据段
   // Wave 文件段
   typedef struct
   {
      char           Sect[4];
      Longword       Size;
   } TWaveHeadSect;

   // Wave 文件头, 固定格式 (58字节)
   typedef struct
   {
      TWaveHeadTag   Tag;              // 头标记
      TWaveHeadSect  FmtSect;          // 格式段
      TWaveFormatEx  WaveFmt;          // 语音格式
      TWaveHeadSect  FactSect;         // 采样实况段
      Longword       FactSize;         // 采样点数
      TWaveHeadSect  DataSect;         // 数据段
   } TFixedWaveHead;

   #pragma pack(pop)

public:
   // 常量值
   static const int Default_BufferSize;         // 默认缓冲区尺寸
   static const int Default_BufferCount;        // 默认缓冲区个数

   static const int Max_BufferSize;             // 最大缓冲区尺寸
   static const int Max_BufferCount;            // 最大缓冲区个数

   // 电话语音常用格式:
   //    {CCITT A-Law, CCITT U-Law, PCM}, 8000 HZ, 8 位, 单声道
   static const TWaveFormatEx Null_WaveFormat;  // 语音格式的空值
   static const TWaveFormatEx ALaw_WaveFormat;  // CCITT A-Law 8000 HZ, 8 位, 单声道
   static const TWaveFormatEx ULaw_WaveFormat;  // CCITT U-Law 8000 HZ, 8 位, 单声道
   static const TWaveFormatEx PCM8_WaveFormat;  // PCM 8000 HZ, 8 位, 单声道

   // 语音的通用事件类型
   typedef void (TObject::*TDoWaveCustomEvent)();
   typedef struct
   {
      TDoWaveCustomEvent      Method;
      void*                   Object;
   } TOnWaveCustomEvent;

   // 语音的日志事件类型
   typedef void (TObject::*TDoWaveLogEvent)(const KYString& AInfo);
   typedef struct
   {
      TDoWaveLogEvent         Method;
      void*                   Object;
   } TOnWaveLogEvent;

public:
   TKYWaveBase() {};
   virtual ~TKYWaveBase() {};

public:
   // 判断语音格式是否相同
   static bool IsSameWaveFormat(const TWaveFormatEx& AFormat1,
                                const TWaveFormatEx& AFormat2);

   // 得到语音文件格式
   static bool GetWaveFormat(int AWavHandle, TWaveFormatEx& AFormat);

   // 跳转到语音数据的开始处并返回数据尺寸
   static bool SeekWaveDataHead(int AWavHandle, Longword& ADataSize);

   // 写语音文件头信息
   static bool WriteWaveHead(int AWavHandle, Longword ADataSize,
                                 const TWaveFormatEx& AFormat, int& AHeadSize);

   // 分配语音数据缓冲区
   static void NewWaveHdr(PWaveHdr& AHdr, int ABufferSize);

   // 释放语音数据缓冲区
   static void FreeWaveHdr(PWaveHdr& AHdr);

protected:
   // 查找语音文件段
   static bool FindWaveHeadSect(int AWavHandle, TWaveHeadSect& AHeadSect,
                               int& AFilePos);
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYWaveCustomPlay - 语音播放通用类 */

class TKYWaveCustomPlay: public TKYWaveBase
{
   // 语音播放回调函数
   static void __stdcall KYWaveCustom_OutProc(HWaveOut AWaveOut,
                               Longword AMsg, Longword AInstance,
                               Longword AParam1, Longword AParam2);
public:
   TKYWaveCustomPlay();
   virtual ~TKYWaveCustomPlay();

   // 读属性
   void* Data() const                  { return FData; }
   bool  IsOpened() const              { return FIsOpened; }
   bool  IsPlaying() const             { return FIsPlaying; }
   bool  IsValidDev() const            { return FIsValidDev; }

   // 设置属性
   void  SetData(void* AData)          { FData = AData; }

   // 缓冲区的尺寸和个数
   int   BufferSize() const            { return FBufferSize; }
   int   BufferCount() const           { return FBufferCount; }

   // 方法
   bool  Open(int ABufferSize = Default_BufferSize,
              int ABufferCount = Default_BufferCount);
   void  Close();
   bool  Play();
   void  Stop();

   // 事件
   TOnWaveCustomEvent   OnPlayEnd;
   TOnWaveLogEvent      OnPlayLog;

protected:
   void  Lock() const                  { FLock->Enter(); }
   void  Unlock() const                { FLock->Leave(); }

   // 处理 KYWaveCustom_OutProc 消息
   void           DoWaveDone();

   // 内部属性, 供继承者使用
   HWAVEOUT       WaveOut() const      { return FWaveOut; }
   TWaveFormatEx  WaveFormat() const   { return FWaveFormat; }

   void           WritePlayLog(const KYString& AInfo);

   virtual bool   DoOpen(int ABufferSize, int ABufferCount);
   virtual void   DoClose();

   virtual bool   DoPlay() = 0;
   virtual void   DoStop() = 0;

   virtual bool   DoIsPlayEnd() = 0;
   virtual bool   DoGetNextFormat(TWaveFormatEx& AWaveFormat) = 0;
   virtual bool   DoGetNextData(void* AData, int& ASize) = 0;
private:
   void           SetIsSynchroEvent(const bool Value);

   void           CreateBuffer(int ASize, int ACount);
   void           FreeBuffer();

   bool           ReadNextFormat(TWaveFormatEx& AWaveFormat);
   bool           OpenWaveOut(TWaveFormatEx& AWaveFormat);
   bool           PlayWaveOut();
   void           CloseWaveOut();

   bool           BeginWaveOut();
   bool           ScanWaveOut();
   void           EndWaveOut();

   unsigned       DoThreadExecute(TKYThread* AThread, void* AParam);
   void           DoThreadSynch(TObject& Sender, void* AParam);
   void           DoPlayEnd();
private:
   // 同步事件常量值
   static const int Synchro_Event_PlayEnd;
   static const int Synchro_Event_PlayLog;

private:
   // 锁, 事件和线程
   TKYCritSect*   FLock;
   TKYEvent*      FEvent;
   TKYEvent*      FNotify;
   TKYThread*     FThread;

   // 基本数据
   void*          FData;
   bool           FIsOpened;
   bool           FIsPlaying;
   bool           FIsValidDev;

   // 播放缓冲区
   int            FBufferSize;
   int            FBufferCount;
   TKYQueue*      FQueueSpaces;
   TKYQueue*      FQueueBuffers;

   // 语音输出句柄
   HWAVEOUT       FWaveOut;
   TWaveFormatEx  FWaveFormat;
   bool           FIsNextFormat;
   TWaveFormatEx  FNextWaveFormat;

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYWaveFilePlay - 播放语音文件类 */

class TKYWaveFilePlay: public TKYWaveCustomPlay
{
public:
   TKYWaveFilePlay();
   virtual ~TKYWaveFilePlay();

   // 读属性
   int   VoiceCount() const            { return FVoices->Count(); }
   KYString Voices(int AIndex) const;

   // 设置属性
   void  SetVoices(int AIndex, const KYString& AItem);

   // 方法
   void  Assign(const TKYStringList* AVoices);

   void  AddVoice(const KYString& AVoice);
   void  DeleteVoice(int AIndex);
   void  ClearVoices();

   // ARepeat: 0/1播放一次，其他播放 ARepeat次, ARepeat < 0表示循环播放
   bool  Play(int ARepeat = 1);

protected:
   bool  DoOpen(int ABufferSize, int ABufferCount);
   void  DoClose();

   bool  DoPlay();
   void  DoStop();

   bool  DoIsPlayEnd();
   virtual bool  DoGetNextFormat(TWaveFormatEx& AWaveFormat);
   virtual bool  DoGetNextData(void* AData, int& ASize);

private:
   void  InitialVoices();

private:
   TKYStringList* FVoices;
   int            FVoiceIndex;
   int            FVoiceHandle;
   int            FRepeat;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYWaveCustomRecord - 语音录音通用类 */

class TKYWaveCustomRecord: public TKYWaveBase
{
   // 语音录音的回调函数
   static void __stdcall KYWaveCustom_InProc(HWaveIn AWaveIn,
                             Longword AMsg, Longword AInstance,
                             Longword AParam1, Longword AParam2);

public:
   TKYWaveCustomRecord();
   virtual ~TKYWaveCustomRecord();

   // 读属性
   void* Data() const                  { return FData; }
   bool  IsOpened() const              { return FIsOpened; }
   bool  IsRecording() const           { return FIsRecording; }
   bool  IsValidDev() const            { return FIsValidDev; }

   // 录音的语音格式
   TWaveFormatEx  WaveFormat() const   { return FWaveFormat; }

   // 缓冲区的尺寸和个数
   int   BufferSize() const            { return FBufferSize; }
   int   BufferCount() const           { return FBufferCount; }

   // 设置属性
   void  SetData(void* AData)          { FData = AData; }
   void  SetIsSynchroEvent(bool AValue);

   // 录音的语音格式
   void  SetWaveFormat(const TWaveFormatEx& AFmt);

   // 方法
   bool  Open(int ABufferSize = Default_BufferSize,
              int ABufferCount = Default_BufferCount);
   void  Close();

   bool  Start();
   void  Stop();

   // 事件
   TOnWaveCustomEvent   OnRecordEnd;
   TOnWaveLogEvent      OnRecordLog;

protected:
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

   // 处理 KYWaveCustom_InProc 消息
   void           DoWaveData();

   // 内部属性, 供继承者使用
   HWAVEIN        WaveIn() const       { return FWaveIn; }
   void           WriteRecordLog(KYString AInfo);

   virtual bool   DoOpen(int ABufferSize, int ABufferCount);
   virtual void   DoClose();

   virtual bool   DoStart() = 0;
   virtual void   DoStop() = 0;
   virtual bool   DoWriteData(void* AData, int ASize) = 0;

private:
   void           CreateBuffer(int ASize, int ACount);
   void           FreeBuffer();

   bool           OpenWaveIn();
   void           CloseWaveIn();

   bool           BeginWaveIn();
   bool           ScanWaveIn();
   void           EndWaveIn();

   unsigned       DoThreadExecute(TKYThread* AThread, void* AParam);
   void           DoThreadSynch(TObject* Sender, void* AParam);
   void           DoRecordEnd();

private:
   // 同步事件常量值
   static const int Synchro_Event_RecordEnd;
   static const int Synchro_Event_RecordLog;

private:
   // 锁, 事件和线程
   TKYCritSect*   FLock;
   TKYEvent*      FEvent;
   TKYEvent*      FNotify;
   TKYThread*     FThread;

   // 基本数据
   void*          FData;
   bool           FIsOpened;
   bool           FIsRecording;
   bool           FIsValidDev;

   // 录音缓冲区
   int            FBufferSize;
   int            FBufferCount;
   TKYQueue*      FQueueSpaces;
   TKYQueue*      FQueueBuffers;

   // 语音输入句柄
   HWAVEIN        FWaveIn;
   TWaveFormatEx  FWaveFormat;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYWaveFileRecord - 语音文件录音类 */

class TKYWaveFileRecord: public TKYWaveCustomRecord
{
public:
   TKYWaveFileRecord();
   virtual ~TKYWaveFileRecord(){};

   // 读属性
   KYString FileName() const           { return FFileName; }

   // 设置属性
   void     SetFileName(const KYString& Value);

protected:
   bool     DoOpen(int ABufferSize, int ABufferCount);
   void     DoClose();

   bool     DoStart();
   void     DoStop();

   bool     DoWriteData(void* AData, int ASize);

private:
   void     CloseRecord();

private:
   KYString FFileName;
   int      FDataSize;
   int      FFileHandle;
};

}

#endif
