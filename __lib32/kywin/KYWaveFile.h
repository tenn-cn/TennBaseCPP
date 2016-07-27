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

// KYWin 2.0 ��ʼʹ�� KYWin �����ռ�
namespace KYWin
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYWaveHead - �����ļ�ͷ�� */

class TKYWaveHead
{
public:
   #pragma pack(push, 1)

   // Wave �ļ�ͷ���
   typedef struct
   {
      char           Flag[4];          // 'RIFF'
      Longword       Size;
      char           Wave[4];          // 'WAVE'
   } TTag, *PTag;

   // ע:
   //    'fmt ': ��ʽ��
   //    'fact': ʵ�ʳߴ��
   //    'data': ���ݶ�
   // Wave �ļ���
   typedef struct
   {
      char           Sect[4];
      Longword       Size;
   } TSect, *PSect;

   // Wave ��ʽ�ṹ
   typedef struct
   {
      Word           FormatTag;        // ������ʽ
      Word           Channels;         // ����ͨ����
      Longword       SamplesPerSec;    // ÿ�������(Hz)
      Longword       AvgBytesPerSec;   // ÿ�����ƽ���ֽ���
      Word           BlockAlign;       // ÿ֡�ֽ���, ����ߴ�
      Word           BitsPerSample;    // ÿ��������bit��
      Word           NextSize;         // ������չ���ݳߴ�, Ĭ��Ϊ0
   } TWaveFmt, *PWaveFmt;

   // ͨ�� Wave �ļ�ͷ, �̶���ʽ (58�ֽ�)
   typedef struct
   {
      TTag           Tag;              // ͷ���
      TSect          FmtSect;          // ��ʽ��
      TWaveFmt       WaveFmt;          // ������ʽ, WaveFmt.NextSize = 0
      TSect          FactSect;         // ����ʵ����
      Longword       FactSize;         // ������
      TSect          DataSect;         // ���ݶ�
   } T_Custom;

   // ADPCM/GSM610 ��ʽ Wave �ļ�ͷ
   typedef struct
   {
      TTag           Tag;              // ͷ���
      TSect          FmtSect;          // ��ʽ��
      TWaveFmt       WaveFmt;          // ������ʽ, WaveFmt.NextSize = 2
      Byte           NextData[2];      // ��չ����
      TSect          FactSect;         // ����ʵ����
      Longword       FactSize;         // ������
      TSect          DataSect;         // ���ݶ�
   } T_ADPCM;

   // MP3 ��ʽ Wave �ļ�ͷ
   typedef struct
   {
      TTag           Tag;              // ͷ���
      TSect          FmtSect;          // ��ʽ��
      TWaveFmt       WaveFmt;          // ������ʽ, WaveFmt.NextSize = 12
      Byte           NextData[12];     // ��չ����
      TSect          FactSect;         // ����ʵ����
      Longword       FactSize;         // ������
      TSect          DataSect;         // ���ݶ�
   } T_MP3;

   #pragma pack(pop)

public:
   TKYWaveHead()                       {}
   virtual ~TKYWaveHead()              {}

public:
   // ��ʼ�������ļ�ͷ��Ϣ
   static void       InitCustom(T_Custom& AHead, const TWaveFmt& AFormat);
   static void       InitADPCM(T_ADPCM& AHead, const TWaveFmt& AFormat,
                                               const Byte*     ANextData);
   static void       InitMP3(T_MP3& AHead, const TWaveFmt& AFormat,
                                           const Byte*     ANextData);

   // ������ʽֵ�õ�������ʽ
   static TWaveFmt   Format2WaveFmt(Word AFormat);
   static KYString   Format2SectData(Word AFormat);

   // ����������ʽ�������ݲ�����
   static Longword   DataSize2FactSize(Longword ADataSize, const TWaveFmt& AFormat);

   // ����¼����ʽ, ¼��ʱ��(����)��¼�����ݳߴ�ת������
   static long       _Time2Size(long ATime, Word AFormat);
   static long       _Size2Time(long ASize, Word AFormat);

public:
   // ������ͷ��ǿ�ֵ
   static const TTag     Tag_Null;

   // Wave �ļ��ο�ֵ
   static const TSect    FmtSect_Null;
   static const TSect    FactSect_Null;
   static const TSect    DataSect_Null;

   // �绰�������ø�ʽ {8000 HZ, 8 λ, ������}:
   static const TWaveFmt WF_Null;      // ������ʽ�Ŀ�ֵ
   static const TWaveFmt WF_ALaw;      // CCITT A-Law
   static const TWaveFmt WF_ULaw;      // CCITT U-Law
   static const TWaveFmt WF_PCM8;      // PCM8
   static const TWaveFmt WF_G729A;     // G.729A
   static const TWaveFmt WF_ADPCM;     // IMA ADPCM
   static const TWaveFmt WF_GSM610;    // GSM 6.10
   static const TWaveFmt WF_MP3;       // MP3

   // ���ø�ʽ�� NextData
   static const Byte     ND_ADPCM[2];  // IMA ADPCM �� NextData
   static const Byte     ND_GSM610[2]; // GSM 6.10 �� NextData
   static const Byte     ND_MP3[12];   // MP3 �� NextData
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYWaveFile - �����ļ��� */

class TKYWaveFile
{
public:
   // �����ض���
   typedef TKYWaveHead::TTag           TWaveTag;
   typedef TKYWaveHead::TSect          TWaveSect;
   typedef TKYWaveHead::TWaveFmt       TWaveFmt;

public:
   TKYWaveFile();
   virtual ~TKYWaveFile();

   // ����
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

   // ����
   bool           Open(const KYString& AFileName);
   void           Close();

   long           Seek(long AOffset, long AOrigin);
   long           Read(void* ABuffer, long ASize);

private:
   void           DoInit();

private:
   TFile*         FFile;               // �ļ�
   TWaveTag       FWaveTag;            // �ļ�ͷ��־
   TWaveFmt*      FWaveFmt;            // ������ʽ = (TWaveFmt*)(char*)FFmtInfo
   KYString       FFmtInfo;            // ��ʽ��Ϣ(TWaveFmt + NextData)
   KYString       FFileName;           // �ļ���
   Longword       FFactSize;           // ������
   Longword       FDataSize;           // ���ݳߴ�
   long           FDataOrigin;         // ������ʼ��
   Word           FBlockAlign;         // ÿ֡�ֽ���, ����ߴ�
   long           FBlockIndex;         // ����֡����
   long           FBlockLength;        // ����֡����

private:
   // ��ȡ����Ϣ
   static bool    ReadFmtSect(char* &APos, char* AEnd, KYString& AFmtInfo);
   static bool    ReadFactSect(char* &APos, char* AEnd, Longword& AFactSize);
   static bool    ReadDataSect(char* &APos, char* AEnd, Longword& ADataSize);
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYWaveStore - �����ļ��洢�� */

class TKYWaveStore
{
public:
   // ���������ļ�ͷָ��ӿ�
   typedef void*  (*TNewHead)(long AFormat, long& AHeadSize);

   // �ͷ������ļ�ͷָ��ӿ�
   typedef void   (*TDeleteHead)(void* AHead, long AFormat);

   // ���������ļ�ͷָ��ӿ�
   typedef void   (*TUpdateHead)(void* AHead, long AFormat,
                              Longword ADataSize, Longword ASamples);

public:
   TKYWaveStore();
   virtual ~TKYWaveStore();

   // ����
   long           Format() const       { return FFormat; }
   void*          WaveHead() const     { return FWaveHead; }
   long           HeadSize() const     { return FHeadSize; }
   Longword       DataSize() const     { return FDataSize; }
   KYString       FileName() const     { return FFileName; }
   bool           Recording() const    { return (FFile != NULL); }

   // ����
   bool           Begin(const KYString& AFileName, long AFormat);
   bool           End(Longword ASamples = 0);

   long           Write(const void* ABuffer, long ASize);

   // �ص������ӿ�
   TNewHead       NewHead;             // Ĭ��Ϊ: DoNewHead
   TDeleteHead    DeleteHead;          // Ĭ��Ϊ: DoDeleteHead
   TUpdateHead    UpdateHead;          // Ĭ��Ϊ: DoUpdateHead

protected:
private:
   TFile*         FFile;               // �ļ�
   long           FFormat;             // ������ʽ
   void*          FWaveHead;           // �ļ�ͷ��Ϣ
   long           FHeadSize;           // �ļ�ͷ�ߴ�
   Longword       FDataSize;           // ���ݳߴ�
   KYString       FFileName;           // �ļ���

public:
   // Ĭ�Ϸ���/�ͷ�/���������ļ�ͷ�ӿ�
   static void*   DoNewHead(long AFormat, long& AHeadSize);
   static void    DoDeleteHead(void* AHead, long AFormat);
   static void    DoUpdateHead(void* AHead, long AFormat,
                            Longword ADataSize, Longword ASamples);
};

}

#endif
