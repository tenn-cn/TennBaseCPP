// =======================================
// Unit   : RC for TCP
// Version: 5.0.0.0 (build 2014.07.07)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _RC4TCP_H_
#define _RC4TCP_H_

#include "RCKernel.h"
using namespace RCKernel;

#ifdef _WIN64
#pragma comment(lib, "RC4TCP64.lib")
#else
#pragma comment(lib, "RC4TCP32.lib")
#endif

namespace RC4TCP
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �������� */

// ����ֵ��������
// �� RCKSlot.h �е� TRCKResult ����

// �ӿڵ�������
enum TTCPAttrib
        {taVersion         = 0,        // �ӿڰ汾��Ϣ, ��ʽ: "x.x.x.x (build yyyy.mm.dd)"
         taServerCount     = 1,        // TCP ����˸���
         taRecvThreads     = 2,        // ���пͻ��˵Ľ����߳�������, Ĭ��ֵ: 8
         taCacheThreads    = 3,        // ���пͻ��˵Ľ����̻߳���سߴ�, Ĭ��ֵ: 8

         // ��չ���Ի���
         taBaseServer      = 0x10000}; // �������������: ����˶���

// ����˵�������
enum TTCPServerAttrib
        {tsaError          = 0,        // �򿪷���ʧ�ܷ��صĴ�����Ϣ
         tsaData           = 1,        // �Զ�������
         tsaAddr           = 2,        // ����˵İ�IP��ַ, Ĭ��ֵ: ""
         tsaPort           = 3,        // ����˵������˿ں�, Ĭ��ֵ: 0
         tsaState          = 4,        // ����˵�״̬
         tsaLinger         = 5,        // TCP ����رպ������ʱ��(��), Ĭ��ֵ: 0
         tsaListenQueue    = 6,        // ����˵��������Ӷ��гߴ�, Ĭ��ֵ: 5
         tsaRecvThreads    = 7,        // �����߳�������, Ĭ��ֵ: 8
         tsaCacheThreads   = 8,        // �����̻߳���سߴ�, Ĭ��ֵ: 8
         tsaMaxClientCount = 9,        // ��������ӵ����ͻ��˸���, Ĭ��ֵ: 0 ��ʾ������
         tsaSrvClientCount = 10,       // ��ǰ���������ӵĿͻ��˸���
         tsaCanLogin       = 11,       // �����Ƿ��ܹ���¼�Զ�Ӧ��, Ĭ��ֵ: false
         tsaCanBindApp     = 12,       // �����Ƿ��ܹ���Ӧ��, Ĭ��ֵ: true
         tsaBindTimeout    = 13,       // ���ӵ�Ӧ�ð󶨳�ʱ(��λ: ����), Ĭ��ֵ: 60000, 0 ��ʾ����ⳬʱ
         tsaSendMaxSize    = 14,       // ͨѶ�۶���ķ�����������󳤶�, Ĭ��ֵ: 8192
         tsaSendQueueSize  = 15,       // ���ӵ� RCKConnSendData ���Ͷ��гߴ�, Ĭ��ֵ: 256
         tsaTimeout        = 16,       // ���ӵĲ�����ʱ(��λ: ����), Ĭ��ֵ: 30000
         tsaKeepTimeout    = 17,       // ���ӵ�������ⳬʱʱ��(��λ: ����), Ĭ��ֵ: 30000
         tsaKeepInterval   = 18,       // ���ӵ�����������Լ��(��λ: ����), Ĭ��ֵ: 10000
         tsaKeepRetryTimes = 19,       // ���ӵ�����������Դ���, Ĭ��ֵ: 3

         // �ص��¼�����, ȡֵ��Χ: [32..63]
         tsaOnListen       = 32,       // ����� OnListen        �¼�, ������(TRCKOnObjEvent)
         tsaOnDisconnect   = 33,       // ����� OnDisconnect    �¼�, �Ͽ�����(TRCKOnObjEvent)
         tsaOnAccept       = 34,       // ����� OnAccept        �¼�, ��������(TTCPOnAccept), ����������������
         tsaOnFreeClt      = 35,       // ����� OnFreeClt       �¼�, �ͷ�����(TRCKOnObjEvent)
         tsaOnCltConnect   = 36,       // ���ӵ� OnCltConnect    �¼�, ���ӳɹ�(TRCKOnObjEvent)
         tsaOnCltDisconnect= 37,       // ���ӵ� OnCltDisconnect �¼�, �Ͽ�����(TRCKOnObjEvent)
         tsaOnCltLogin     = 38,       // ���ӵ� OnCltLogin      �¼�, ��¼�Զ�Ӧ��(TRCKOnObjEvent)
         tsaOnCltLogout    = 39,       // ���ӵ� OnCltLogout     �¼�, �ǳ��Զ�Ӧ��(TRCKOnObjEvent)
         tsaOnCltCmdReturn = 40,       // ���ӵ� OnCltCmdReturn  �¼�, �����(TRCKOnCmdReturn)
         tsaOnCltRecvData  = 41,       // ���ӵ� OnCltRecvData   �¼�, ��������(TRCKOnRecvData)

         // ��չ���Ի���
         tsaBaseSrvClient  = 0x10000}; // �����ӵĿͻ�����������: �ͻ��˶���

// ����˵�״̬
enum TTCPServerState
        {tssInactive       = 0,        // δ��
         tssClosing        = 1,        // ���ڹر�
         tssOpening        = 2,        // ���ڴ�
         tssOpened         = 3};       // �Ѿ���

// ����˿ͻ����ӵ�������(��������ͨѶ�۶������չ������, kcaBaseSlotExt + Attrib)
enum TTCPSrvCltAttrib
        {tscaData          = 0,        // �Զ�������
         tscaSrvObj        = 1,        // ���ӵķ���˶���
         tscaIsValid       = 2};       // �����Ƿ���Ч

// �ͻ��˵�������(��������ͨѶ�۶������չ������, kcaBaseSlotExt + Attrib)
enum TTCPClientAttrib
        {tcaError          = 0,        // �򿪿ͻ���ʧ�ܷ��صĴ�����Ϣ
         tcaData           = 1,        // �Զ�������
         tcaLinger         = 2};       // TCP ����رպ������ʱ��(��), Ĭ��ֵ: 0

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �ӿڻص��������Ͷ��� */

// ����˽��������¼��Ļص���������(OnAccept)
typedef void (__stdcall *TTCPOnAccept)(void* ASrvObj, void* AConnObj, bool& AIsRefused);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �ӿ���غ��� */

// �ӿڳ�ʼ��/�ͷź���
void  __stdcall TCPInitialize();
void  __stdcall TCPUninitialize();

// �ӿڰ汾��Ϣ, ��ʽ: "x.x.x.x (build yyyy.mm.dd)"
// AVersion    ���ذ汾��, ��: 0x0708090A ��ʾ 7.8.9.10
char* __stdcall TCPGetVersion(long* AVersion);

// ȡ��ǰ��������������(ע: ���ڷ���ֵ�Ǵ�����ĺ���)
long  __stdcall TCPGetLastError();

// ȡ�ӿ�����
// (����ֵΪ����ֵ, ARetCode Ϊʧ�ܵķ�����, ����Ϊ��, ���ַ������ͳɹ�ʱΪֵ�ߴ�)
long  __stdcall TCPGetInt(long Attrib, long* ARetCode = NULL);
void* __stdcall TCPGetObj(long Attrib, long* ARetCode = NULL);
char* __stdcall TCPGetStr(long Attrib, long* ARetCode = NULL);

// ���ýӿ�����
long  __stdcall TCPSetInt(long Attrib, long  AValue);
long  __stdcall TCPSetObj(long Attrib, void* AValue);
long  __stdcall TCPSetStr(long Attrib, const char* AValue, long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TCP ����˵���غ��� */

// ����/�ͷ� TCP �����
void* __stdcall TCPServerCreate();
void  __stdcall TCPServerFree(void* ASrvObj);

// ��/�ر� TCP �����
long  __stdcall TCPServerOpen(void* ASrvObj);
long  __stdcall TCPServerClose(void* ASrvObj);

// ȡ TCP ���������
// (����ֵΪ����ֵ, ARetCode Ϊʧ�ܵķ�����, ����Ϊ��, ���ַ������ͳɹ�ʱΪֵ�ߴ�)
long  __stdcall TCPServerGetInt(void* ASrvObj, long Attrib, long* ARetCode = NULL);
void* __stdcall TCPServerGetObj(void* ASrvObj, long Attrib, long* ARetCode = NULL);
char* __stdcall TCPServerGetStr(void* ASrvObj, long Attrib, long* ARetCode = NULL);

// ���� TCP ���������
long  __stdcall TCPServerSetInt(void* ASrvObj, long Attrib, long  AValue);
long  __stdcall TCPServerSetObj(void* ASrvObj, long Attrib, void* AValue);
long  __stdcall TCPServerSetStr(void* ASrvObj, long Attrib,
                          const char* AValue,  long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TCP �ͻ��˵���غ��� */

// ���� TCP �ͻ���, ���� RCKConnCreate �����Ķ���
// ע:  TCP �ͻ��˵��ͷŵ��� RCKConnFree ����
//      TCP �ͻ��˵�����������ͨѶ�۶������չ������, kcaBaseSlotExt + Attrib
void* __stdcall TCPClientCreate();

}

#endif
