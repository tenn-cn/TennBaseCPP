// =======================================
// Unit   : RC for share-memory
// Version: 5.0.0.0 (build 2014.07.07)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _RC4SHM_H_
#define _RC4SHM_H_

#include "RCKernel.h"
using namespace RCKernel;

#ifdef _WIN64
#pragma comment(lib, "RC4SHM64.lib")
#else
#pragma comment(lib, "RC4SHM32.lib")
#endif

namespace RC4SHM
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �������� */

// ����ֵ��������
// �� RCKSlot.h �е� TRCKResult ����

// �ӿڵ�������
enum TSHMAttrib
        {maVersion         = 0,        // �ӿڰ汾��Ϣ, ��ʽ: "x.x.x.x (build yyyy.mm.dd)"
         maServerCount     = 1,        // SHM ����˸���
         maRecvThreads     = 2,        // ���пͻ��˵Ľ����߳�������, Ĭ��ֵ: 8
         maCacheThreads    = 3,        // ���пͻ��˵Ľ����̻߳���سߴ�, Ĭ��ֵ: 8

         // ��չ���Ի���
         maBaseServer      = 0x10000}; // �������������: ����˶���

// ����˵�������
enum TSHMServerAttrib
        {msaError          = 0,        // �򿪷���ʧ�ܷ��صĴ�����Ϣ
         msaData           = 1,        // �Զ�������
         msaName           = 2,        // ����˵İ�������, Ĭ��ֵ: ""
         msaPort           = 3,        // ����˵������˿ں�, Ĭ��ֵ: 0
         msaState          = 4,        // ����˵�״̬
         msaReqTimeout     = 5,        // ����˵���������ʱ(��λ: ����), Ĭ��ֵ: 30000
         msaListenQueue    = 6,        // ����˵��������Ӷ��гߴ�, Ĭ��ֵ: 16
         msaRecvThreads    = 7,        // �����߳�������, Ĭ��ֵ: 8
         msaCacheThreads   = 8,        // �����̻߳���سߴ�, Ĭ��ֵ: 8
         msaMaxClientCount = 9,        // ��������ӵ����ͻ��˸���, Ĭ��ֵ: 0 ��ʾ������
         msaSrvClientCount = 10,       // ��ǰ���������ӵĿͻ��˸���
         msaCanLogin       = 11,       // �����Ƿ��ܹ���¼�Զ�Ӧ��, Ĭ��ֵ: false
         msaCanBindApp     = 12,       // �����Ƿ��ܹ���Ӧ��, Ĭ��ֵ: true
         msaBindTimeout    = 13,       // ���ӵ�Ӧ�ð󶨳�ʱ(��λ: ����), Ĭ��ֵ: 60000, 0 ��ʾ����ⳬʱ
         msaSendMaxSize    = 14,       // ͨѶ�۶���ķ�����������󳤶�, Ĭ��ֵ: 8192
         msaSendQueueSize  = 15,       // ���ӵ� RCKConnSendData ���Ͷ��гߴ�, Ĭ��ֵ: 256
         msaTimeout        = 16,       // ���ӵĲ�����ʱ(��λ: ����), Ĭ��ֵ: 30000
         msaKeepTimeout    = 17,       // ���ӵ�������ⳬʱʱ��(��λ: ����), Ĭ��ֵ: 30000
         msaKeepInterval   = 18,       // ���ӵ�����������Լ��(��λ: ����), Ĭ��ֵ: 10000
         msaKeepRetryTimes = 19,       // ���ӵ�����������Դ���, Ĭ��ֵ: 3

         // �ص��¼�����, ȡֵ��Χ: [32..63]
         msaOnListen       = 32,       // ����� OnListen        �¼�, ������(TRCKOnObjEvent)
         msaOnDisconnect   = 33,       // ����� OnDisconnect    �¼�, �Ͽ�����(TRCKOnObjEvent)
         msaOnAccept       = 34,       // ����� OnAccept        �¼�, ��������(TSHMOnAccept), ����������������
         msaOnFreeClt      = 35,       // ����� OnFreeClt       �¼�, �ͷ�����(TRCKOnObjEvent)
         msaOnCltConnect   = 36,       // ���ӵ� OnCltConnect    �¼�, ���ӳɹ�(TRCKOnObjEvent)
         msaOnCltDisconnect= 37,       // ���ӵ� OnCltDisconnect �¼�, �Ͽ�����(TRCKOnObjEvent)
         msaOnCltLogin     = 38,       // ���ӵ� OnCltLogin      �¼�, ��¼�Զ�Ӧ��(TRCKOnObjEvent)
         msaOnCltLogout    = 39,       // ���ӵ� OnCltLogout     �¼�, �ǳ��Զ�Ӧ��(TRCKOnObjEvent)
         msaOnCltCmdReturn = 40,       // ���ӵ� OnCltCmdReturn  �¼�, �����(TRCKOnCmdReturn)
         msaOnCltRecvData  = 41,       // ���ӵ� OnCltRecvData   �¼�, ��������(TRCKOnRecvData)

         // ��չ���Ի���
         msaBaseSrvClient  = 0x10000}; // �����ӵ�������������: �������

// ����˵�״̬
enum TSHMServerState
        {mssInactive       = 0,        // δ��
         mssClosing        = 1,        // ���ڹر�
         mssOpening        = 2,        // ���ڴ�
         mssOpened         = 3};       // �Ѿ���

// ����˿ͻ����ӵ�������(��������ͨѶ�۶������չ������, kcaBaseSlotExt + Attrib)
enum TSHMSrvCltAttrib
        {mscaData          = 0,        // �Զ�������
         mscaSrvObj        = 1,        // ���ӵķ���˶���
         mscaIsValid       = 2};       // �����Ƿ���Ч

// �ͻ��˵�������(��������ͨѶ�۶������չ������, kcaBaseSlotExt + Attrib)
enum TSHMClientAttrib
        {mcaError          = 0,        // �򿪿ͻ���ʧ�ܷ��صĴ�����Ϣ
         mcaData           = 1};       // �Զ�������

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �ӿڻص��������Ͷ��� */

// ����˽��������¼��Ļص���������(OnAccept)
typedef void (__stdcall *TSHMOnAccept)(void* ASrvObj, void* AConnObj, bool& AIsRefused);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �ӿ���غ��� */

// �ӿڳ�ʼ��/�ͷź���
void  __stdcall SHMInitialize();
void  __stdcall SHMUninitialize();

// �ӿڰ汾��Ϣ, ��ʽ: "x.x.x.x (build yyyy.mm.dd)"
// AVersion    ���ذ汾��, ��: 0x0708090A ��ʾ 7.8.9.10
char* __stdcall SHMGetVersion(long* AVersion);

// ȡ��ǰ��������������(ע: ���ڷ���ֵ�Ǵ�����ĺ���)
long  __stdcall SHMGetLastError();

// ȡ�ӿ�����
// (����ֵΪ����ֵ, ARetCode Ϊʧ�ܵķ�����, ����Ϊ��, ���ַ������ͳɹ�ʱΪֵ�ߴ�)
long  __stdcall SHMGetInt(long Attrib, long* ARetCode = NULL);
void* __stdcall SHMGetObj(long Attrib, long* ARetCode = NULL);
char* __stdcall SHMGetStr(long Attrib, long* ARetCode = NULL);

// ���ýӿ�����
long  __stdcall SHMSetInt(long Attrib, long  AValue);
long  __stdcall SHMSetObj(long Attrib, void* AValue);
long  __stdcall SHMSetStr(long Attrib, const char* AValue, long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* SHM ����˵���غ��� */

// ����/�ͷ� SHM �����
void* __stdcall SHMServerCreate();
void  __stdcall SHMServerFree(void* ASrvObj);

// ��/�ر� SHM �����
long  __stdcall SHMServerOpen(void* ASrvObj);
long  __stdcall SHMServerClose(void* ASrvObj);

// ȡ SHM ���������
// (����ֵΪ����ֵ, ARetCode Ϊʧ�ܵķ�����, ����Ϊ��, ���ַ������ͳɹ�ʱΪֵ�ߴ�)
long  __stdcall SHMServerGetInt(void* ASrvObj, long Attrib, long* ARetCode = NULL);
void* __stdcall SHMServerGetObj(void* ASrvObj, long Attrib, long* ARetCode = NULL);
char* __stdcall SHMServerGetStr(void* ASrvObj, long Attrib, long* ARetCode = NULL);

// ���� SHM ���������
long  __stdcall SHMServerSetInt(void* ASrvObj, long Attrib, long  AValue);
long  __stdcall SHMServerSetObj(void* ASrvObj, long Attrib, void* AValue);
long  __stdcall SHMServerSetStr(void* ASrvObj, long Attrib,
                          const char* AValue,  long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* SHM �ͻ��˵���غ��� */

// ���� SHM �ͻ���, ���� RCKConnCreate �����Ķ���
// ע:  SHM �ͻ��˵��ͷŵ��� RCKConnFree ����
//      SHM �ͻ��˵�����������ͨѶ�۶������չ������, kcaBaseSlotExt + Attrib
void* __stdcall SHMClientCreate();

}

#endif
