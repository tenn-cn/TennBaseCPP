// =======================================
// Unit   : ������¼����Ԫ (KYWavePlay.h)
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

// KYWin 2.0 ��ʼʹ�� KYWin �����ռ�
namespace KYWin
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �����ض��� */

typedef  WAVEFORMATEX   TWaveFormatEx;
typedef  WAVEHDR        TWaveHdr, *PWaveHdr;
typedef  HWAVEOUT       HWaveOut;
typedef  HWAVEIN        HWaveIn;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYWaveBase - ¼�������������� */

class TKYWaveBase
{
public:
   #pragma pack(push, 1)

   // Wave �ļ�ͷ���
   typedef struct
   {
      char           Flag[4];          // 'RIFF'
      Longword       Size;
      char           Wave[4];          // 'WAVE'
   } TWaveHeadTag;

   // ע:
   //    'fmt ': ��ʽ��
   //    'fact': ʵ�ʳߴ��
   //    'data': ���ݶ�
   // Wave �ļ���
   typedef struct
   {
      char           Sect[4];
      Longword       Size;
   } TWaveHeadSect;

   // Wave �ļ�ͷ, �̶���ʽ (58�ֽ�)
   typedef struct
   {
      TWaveHeadTag   Tag;              // ͷ���
      TWaveHeadSect  FmtSect;          // ��ʽ��
      TWaveFormatEx  WaveFmt;          // ������ʽ
      TWaveHeadSect  FactSect;         // ����ʵ����
      Longword       FactSize;         // ��������
      TWaveHeadSect  DataSect;         // ���ݶ�
   } TFixedWaveHead;

   #pragma pack(pop)

public:
   // ����ֵ
   static const int Default_BufferSize;         // Ĭ�ϻ������ߴ�
   static const int Default_BufferCount;        // Ĭ�ϻ���������

   static const int Max_BufferSize;             // ��󻺳����ߴ�
   static const int Max_BufferCount;            // ��󻺳�������

   // �绰�������ø�ʽ:
   //    {CCITT A-Law, CCITT U-Law, PCM}, 8000 HZ, 8 λ, ������
   static const TWaveFormatEx Null_WaveFormat;  // ������ʽ�Ŀ�ֵ
   static const TWaveFormatEx ALaw_WaveFormat;  // CCITT A-Law 8000 HZ, 8 λ, ������
   static const TWaveFormatEx ULaw_WaveFormat;  // CCITT U-Law 8000 HZ, 8 λ, ������
   static const TWaveFormatEx PCM8_WaveFormat;  // PCM 8000 HZ, 8 λ, ������

   // ������ͨ���¼�����
   typedef void (TObject::*TDoWaveCustomEvent)();
   typedef struct
   {
      TDoWaveCustomEvent      Method;
      void*                   Object;
   } TOnWaveCustomEvent;

   // ��������־�¼�����
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
   // �ж�������ʽ�Ƿ���ͬ
   static bool IsSameWaveFormat(const TWaveFormatEx& AFormat1,
                                const TWaveFormatEx& AFormat2);

   // �õ������ļ���ʽ
   static bool GetWaveFormat(int AWavHandle, TWaveFormatEx& AFormat);

   // ��ת���������ݵĿ�ʼ�����������ݳߴ�
   static bool SeekWaveDataHead(int AWavHandle, Longword& ADataSize);

   // д�����ļ�ͷ��Ϣ
   static bool WriteWaveHead(int AWavHandle, Longword ADataSize,
                                 const TWaveFormatEx& AFormat, int& AHeadSize);

   // �����������ݻ�����
   static void NewWaveHdr(PWaveHdr& AHdr, int ABufferSize);

   // �ͷ��������ݻ�����
   static void FreeWaveHdr(PWaveHdr& AHdr);

protected:
   // ���������ļ���
   static bool FindWaveHeadSect(int AWavHandle, TWaveHeadSect& AHeadSect,
                               int& AFilePos);
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYWaveCustomPlay - ��������ͨ���� */

class TKYWaveCustomPlay: public TKYWaveBase
{
   // �������Żص�����
   static void __stdcall KYWaveCustom_OutProc(HWaveOut AWaveOut,
                               Longword AMsg, Longword AInstance,
                               Longword AParam1, Longword AParam2);
public:
   TKYWaveCustomPlay();
   virtual ~TKYWaveCustomPlay();

   // ������
   void* Data() const                  { return FData; }
   bool  IsOpened() const              { return FIsOpened; }
   bool  IsPlaying() const             { return FIsPlaying; }
   bool  IsValidDev() const            { return FIsValidDev; }

   // ��������
   void  SetData(void* AData)          { FData = AData; }

   // �������ĳߴ�͸���
   int   BufferSize() const            { return FBufferSize; }
   int   BufferCount() const           { return FBufferCount; }

   // ����
   bool  Open(int ABufferSize = Default_BufferSize,
              int ABufferCount = Default_BufferCount);
   void  Close();
   bool  Play();
   void  Stop();

   // �¼�
   TOnWaveCustomEvent   OnPlayEnd;
   TOnWaveLogEvent      OnPlayLog;

protected:
   void  Lock() const                  { FLock->Enter(); }
   void  Unlock() const                { FLock->Leave(); }

   // ���� KYWaveCustom_OutProc ��Ϣ
   void           DoWaveDone();

   // �ڲ�����, ���̳���ʹ��
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
   // ͬ���¼�����ֵ
   static const int Synchro_Event_PlayEnd;
   static const int Synchro_Event_PlayLog;

private:
   // ��, �¼����߳�
   TKYCritSect*   FLock;
   TKYEvent*      FEvent;
   TKYEvent*      FNotify;
   TKYThread*     FThread;

   // ��������
   void*          FData;
   bool           FIsOpened;
   bool           FIsPlaying;
   bool           FIsValidDev;

   // ���Ż�����
   int            FBufferSize;
   int            FBufferCount;
   TKYQueue*      FQueueSpaces;
   TKYQueue*      FQueueBuffers;

   // ����������
   HWAVEOUT       FWaveOut;
   TWaveFormatEx  FWaveFormat;
   bool           FIsNextFormat;
   TWaveFormatEx  FNextWaveFormat;

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYWaveFilePlay - ���������ļ��� */

class TKYWaveFilePlay: public TKYWaveCustomPlay
{
public:
   TKYWaveFilePlay();
   virtual ~TKYWaveFilePlay();

   // ������
   int   VoiceCount() const            { return FVoices->Count(); }
   KYString Voices(int AIndex) const;

   // ��������
   void  SetVoices(int AIndex, const KYString& AItem);

   // ����
   void  Assign(const TKYStringList* AVoices);

   void  AddVoice(const KYString& AVoice);
   void  DeleteVoice(int AIndex);
   void  ClearVoices();

   // ARepeat: 0/1����һ�Σ��������� ARepeat��, ARepeat < 0��ʾѭ������
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

/* TKYWaveCustomRecord - ����¼��ͨ���� */

class TKYWaveCustomRecord: public TKYWaveBase
{
   // ����¼���Ļص�����
   static void __stdcall KYWaveCustom_InProc(HWaveIn AWaveIn,
                             Longword AMsg, Longword AInstance,
                             Longword AParam1, Longword AParam2);

public:
   TKYWaveCustomRecord();
   virtual ~TKYWaveCustomRecord();

   // ������
   void* Data() const                  { return FData; }
   bool  IsOpened() const              { return FIsOpened; }
   bool  IsRecording() const           { return FIsRecording; }
   bool  IsValidDev() const            { return FIsValidDev; }

   // ¼����������ʽ
   TWaveFormatEx  WaveFormat() const   { return FWaveFormat; }

   // �������ĳߴ�͸���
   int   BufferSize() const            { return FBufferSize; }
   int   BufferCount() const           { return FBufferCount; }

   // ��������
   void  SetData(void* AData)          { FData = AData; }
   void  SetIsSynchroEvent(bool AValue);

   // ¼����������ʽ
   void  SetWaveFormat(const TWaveFormatEx& AFmt);

   // ����
   bool  Open(int ABufferSize = Default_BufferSize,
              int ABufferCount = Default_BufferCount);
   void  Close();

   bool  Start();
   void  Stop();

   // �¼�
   TOnWaveCustomEvent   OnRecordEnd;
   TOnWaveLogEvent      OnRecordLog;

protected:
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

   // ���� KYWaveCustom_InProc ��Ϣ
   void           DoWaveData();

   // �ڲ�����, ���̳���ʹ��
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
   // ͬ���¼�����ֵ
   static const int Synchro_Event_RecordEnd;
   static const int Synchro_Event_RecordLog;

private:
   // ��, �¼����߳�
   TKYCritSect*   FLock;
   TKYEvent*      FEvent;
   TKYEvent*      FNotify;
   TKYThread*     FThread;

   // ��������
   void*          FData;
   bool           FIsOpened;
   bool           FIsRecording;
   bool           FIsValidDev;

   // ¼��������
   int            FBufferSize;
   int            FBufferCount;
   TKYQueue*      FQueueSpaces;
   TKYQueue*      FQueueBuffers;

   // ����������
   HWAVEIN        FWaveIn;
   TWaveFormatEx  FWaveFormat;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYWaveFileRecord - �����ļ�¼���� */

class TKYWaveFileRecord: public TKYWaveCustomRecord
{
public:
   TKYWaveFileRecord();
   virtual ~TKYWaveFileRecord(){};

   // ������
   KYString FileName() const           { return FFileName; }

   // ��������
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
