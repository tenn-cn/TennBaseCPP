// =======================================
// Unit   : RC Kernel
// Version: 5.0.0.0 (build 2014.07.07)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _RCKernel_H_
#define _RCKernel_H_

#include "RCKSlot.h"
#ifdef _WIN64
#pragma comment(lib, "RCKernel64.lib")
#else
#pragma comment(lib, "RCKernel32.lib")
#endif

#define NULL 0

namespace RCKernel
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �������� */

// ��������
enum TRCKDataType
        {kdtNone           = 0,        // ������
         kdtString         = 1,        // �ַ�������  (�Զ���ߴ�)
         kdtInteger        = 2,        // ��������    (4 Bytes)
         kdtBoolean        = 3,        // ��������    (1 Bytes)
         kdtDouble         = 4,        // ��������    (8 Bytes)
         kdtStruct         = 5,        // �ṹ����    (�Զ���ߴ�)
         kdtByte           = 6,        // �ֽ�        (1 Bytes)
         kdtWord           = 7,        // ˫�ֽ�      (2 Bytes)
         kdtSmallint       = 8,        // 16λ����    (2 Bytes)
         kdtInt64          = 9,        // 64λ����    (8 Bytes)
         kdtDateTime       = 10,       // ����ʱ������(8 Bytes)
         kdtCurrency       = 11};      // ��������    (8 Bytes)

// �ӿڵ�������
enum TRCKAttrib
        {kaVersion         = 0,        // �ӿڰ汾��Ϣ, ��ʽ: "x.x.x.x (build yyyy.mm.dd)"
         kaAppCount        = 1,        // Ӧ�ø���
         kaCmdCount        = 2,        // �������
         kaConnCount       = 3,        // ���Ӹ���
         kaDealThreads     = 4,        // �����߳�������, Ĭ��ֵ: 16
         kaDealCacheSize   = 5,        // �����̻߳���سߴ�, Ĭ��ֵ: 16
         kaSendThreads     = 6,        // �����߳�������, Ĭ��ֵ: 8
         kaSendCacheSize   = 7,        // �����̻߳���سߴ�, Ĭ��ֵ: 8

         // ��չ���Ի���
         kaBaseAppObj      = 0x10000,  // Ӧ����������: Ӧ�ö���
         kaBaseAppName     = 0x20000,  // Ӧ����������: Ӧ����
         kaBaseConnObj     = 0x40000}; // ������������: ���Ӷ���

// Ӧ�õ�������
enum TRCKAppAttrib
        {kaaData           = 0,        // �Զ�������
         kaaName           = 1,        // Ӧ����
         kaaState          = 2,        // Ӧ�õ�״̬
         kaaPassword       = 3,        // Ӧ�õ�����
         kaaFuncCount      = 4,        // Ӧ�õĺ�������
         kaaConnCount      = 5,        // Ӧ�õ���������
         kaaMaxThreads     = 6,        // Ӧ�õĻص����������߳�������, Ĭ��ֵ: 8
         kaaMaxConnCount   = 7,        // Ӧ�õ�������Ӹ���, Ĭ��ֵ: 0 ��ʾ������

         // �ص��¼�����, ȡֵ��Χ: [32..63]
         kaaOnConnLogin    = 32,       // Ӧ�õ� OnConnLogin   �¼�, ���ӵ�¼(TRCKOnAppLogin)
         kaaOnConnLogout   = 33,       // Ӧ�õ� OnConnLogout  �¼�, ���ӵǳ�(TRCKOnAppLogout)
         kaaOnExecResp     = 34,       // Ӧ�õ� OnExecResp    �¼�, ִ��Ӧ��(TRCKOnAppResp)
         kaaOnRespCancel   = 35,       // Ӧ�õ� OnRespCancel  �¼�, Ӧ����ȡ��(TRCKOnAppResp)
         kaaOnRespConfirm  = 36,       // Ӧ�õ� OnRespConfirm �¼�, Ӧ����ȷ��(TRCKOnAppResp)

         // ��չ���Ի���
         kaaBaseFuncObj    = 0x10000,  // Ӧ�õĺ�����������: ��������
         kaaBaseFuncName   = 0x20000,  // Ӧ�õĺ�����������: ������
         kaaBaseConnObj    = 0x40000}; // Ӧ�õ�������������: ���Ӷ���

// Ӧ�õ�״̬
enum TRCKAppState
        {kasInactive       = 0,        // ����δ����
         kasDefining       = 1,        // �������ڶ���
         kasDefined        = 2,        // �����Ѷ���
         kasClosing        = 3,        // ���ڹر�
         kasOpening        = 4,        // ���ڴ�
         kasOpened         = 5};       // �Ѵ�

// ������������
enum TRCKFuncAttrib
        {kfaData           = 0,        // �Զ�������
         kfaName           = 1,        // ������
         kfaActive         = 2,        // �����Ƿ��Ѽ���
         kfaAppObj         = 3,        // ��������Ӧ�ö���
         kfaRetType        = 4,        // ��������ֵ����
         kfaRetSize        = 5,        // ��������ֵ����ߴ�
         kfaCallMode       = 6,        // �ص�����ģʽ
         kfaCallback       = 7,        // �ص�����ָ��
         kfaParamCount     = 8,        // �����Ĳ�������
         kfaFieldCount     = 9,        // �����ķ����ֶθ���
         kfaNeedConfirm    = 10,       // ����Ӧ�𷵻��Ƿ���Ҫȷ��
         kfaNeedEncrypt    = 11,       // ���������ͷ���ֵ��ͨѶ���Ƿ���Ҫ����

         // ��չ���Ի���
         kfaBaseParamName  = 0x10000,  // ����������������: ������
         kfaBaseParamType  = 0x20000,  // ����������������: ��������
         kfaBaseParamSize  = 0x30000,  // ����������������: ��������ߴ�
         kfaBaseParamIsRef = 0x40000,  // ����������������: �����Ƿ�Ϊ����ָ��
         kfaBaseFieldName  = 0x60000,  // ���������ֶ���������: �����ֶ���
         kfaBaseFieldType  = 0x70000,  // ���������ֶ���������: �����ֶ�����
         kfaBaseFieldSize  = 0x80000}; // ���������ֶ���������: �����ֶζ���ߴ�

// �ص�����ģʽ
enum TRCKFuncCallMode
        {kfcmNone          = 0,        // ��, ����ͨ�� OnExecResp �¼�����
         kfcmFixed         = 1,        // �̶��Ļص�ģʽ(TRCKDoFixed����)
         kfcmCdecl         = 2,        // C ���Ժ�������Э��
         kfcmStdcall       = 3,        // API ��׼��������Э��
         kfcmIDCdecl       = 4,        // �Զ���� AConnObj ������ C ���Ժ�������Э��
         kfcmIDStdcall     = 5};       // �Զ���� AConnObj ������ API ��׼��������Э��

// ���ӵ�������
enum TRCKConnAttrib
        {kcaError          = 0,        // ������Ϣ
         kcaData           = 1,        // �Զ�������
         kcaType           = 2,        // ���ӵ�����
         kcaState          = 3,        // ���ӵ�״̬
         kcaSlotObj        = 4,        // ���Ӱ󶨵�ͨѶ�۶���(����TRCKSlotMethods�����б�)
         kcaCanLogin       = 5,        // �����Ƿ��ܹ���¼�Զ�Ӧ��, Ĭ��ֵ: (Type == kctTCPClient)
         kcaCanBindApp     = 6,        // �����Ƿ��ܹ���Ӧ��, Ĭ��ֵ: (Type == kctTCPSrvClient)
         kcaBindAppObj     = 7,        // ���Ӱ󶨵�Ӧ�ö���, Ĭ��ֵ: NULL
         kcaBindTimeout    = 8,        // ���ӵ�Ӧ�ð󶨳�ʱ(��λ: ����), Ĭ��ֵ: 0 ��ʾ����ⳬʱ
         kcaIsSyncLink     = 9,        // ͨѶ�۶���򿪳ɹ����Ƿ�ͬ����������, Ĭ��ֵ: (Type == kctTCPClient)
         kcaSlotTimeout    = 10,       // ͨѶ�۶���Ĵ򿪳�ʱ(��λ: ����), Ĭ��ֵ: 30000
         kcaSlotMaxSize    = 11,       // ͨѶ�۶�������ݰ���󳤶�, Ĭ��ֵ: 8192
         kcaSendQueueSize  = 12,       // ���ӵ� RCKConnSendData ���Ͷ��гߴ�, Ĭ��ֵ: 256
         kcaTimeout        = 13,       // ���ӵĲ�����ʱ(��λ: ����), Ĭ��ֵ: 30000
         kcaAppName        = 14,       // ��¼�Զ�Ӧ����
         kcaAppPass        = 15,       // ��¼�Զ�Ӧ������
         kcaCurrAddr       = 16,       // ���ӵı��ص�ַ
         kcaCurrPort       = 17,       // ���ӵı��ض˿ں�
         kcaPeerAddr       = 18,       // ���ӵĶԶ˵�ַ
         kcaPeerPort       = 19,       // ���ӵĶԶ˶˿ں�
         kcaLastTick       = 20,       // ���ӵ����һ���շ�����ʱ tick
         kcaFuncCount      = 21,       // �Ѽ��ضԶ�Ӧ�õĺ�������, Ĭ��ֵ: -1 ��ʾδ����
         kcaKeepTimeout    = 22,       // ���ӵ�������ⳬʱʱ��(��λ: ����), Ĭ��ֵ: 30000
         kcaKeepInterval   = 23,       // ���ӵ�����������Լ��(��λ: ����), Ĭ��ֵ: 10000
         kcaKeepRetryTimes = 24,       // ���ӵ�����������Դ���, Ĭ��ֵ: 3

         // �ص��¼�����, ȡֵ��Χ: [32..63]
         kcaOnConnect      = 32,       // ���ӵ� OnConnect     �¼�, ���ӳɹ�(TRCKOnObjEvent)
         kcaOnDisconnect   = 33,       // ���ӵ� OnDisconnect  �¼�, �Ͽ�����(TRCKOnObjEvent)
         kcaOnLogin        = 34,       // ���ӵ� OnLogin       �¼�, ��¼�Զ�Ӧ��(TRCKOnObjEvent)
         kcaOnLogout       = 35,       // ���ӵ� OnLogout      �¼�, �ǳ��Զ�Ӧ��(TRCKOnObjEvent)
         kcaOnCmdReturn    = 36,       // ���ӵ� OnCmdReturn   �¼�, �����(TRCKOnCmdReturn)
         kcaOnRecvData     = 37,       // ���ӵ� OnRecvData    �¼�, ��������(TRCKOnRecvData)

         // ��չ���Ի���
         kcaBaseSlotExt    = 0x10000,  // ͨѶ�۶������չ���Ի���: ��չ������
         kcaBaseFuncName   = 0x20000}; // �Ѽ��ضԶ�Ӧ�õĺ�����������: ������

// ���ӵ�����
enum TRCKConnType
        {kctUnknown        = 0,        // (δ֪)
         kctTCPClient      = 1,        // TCP �ͻ���
         kctTCPSrvClient   = 2,        // TCP ����˵Ŀͻ�����
         kctSHMClient      = 3,        // SHM �ͻ���(�����ڴ�ͨѶ)
         kctSHMSrvClient   = 4,        // SHM ����˵Ŀͻ�����(�����ڴ�ͨѶ)
         // ... ...                    // ���Ը��� slot ͨѶ��ʽ�Զ�������
         };

// ���ӵ�״̬
enum TRCKConnState
        {kcsInactive       = 0,        // δ��
         kcsClosing        = 1,        // ���ڹر�
         kcsOpening        = 2,        // ���ڴ�
         kcsOpened         = 3,        // �Ѿ���
         kcsLogouting      = 4,        // ���ڵǳ��Զ�Ӧ��
         kcsLogging        = 5,        // ���ڵ�¼�Զ�Ӧ��
         kcsLogged         = 6,        // �ѵ�¼�Զ�Ӧ��
         kcsLoading        = 7,        // ���ڼ��ضԶ�Ӧ�ú����б�
         kcsLoaded         = 8};       // �Ѽ��ضԶ�Ӧ�ú����б�

// �����������
enum TRCKCmdAttrib
        {kmaData           = 0,        // �Զ�������
         kmaName           = 1,        // ����ĺ�����
         kmaState          = 2,        // �����״̬
         kmaConnObj        = 3,        // ��������Ӷ���
         kmaRetType        = 4,        // ����ķ���ֵ����
         kmaRetSize        = 5,        // ����ķ���ֵ����ߴ�
         kmaParamCount     = 6,        // ����Ĳ�������
         kmaFieldCount     = 7,        // ����ķ����ֶθ���
         kmaExecResult     = 8,        // �����ִ�з��ؽ��(ֵ�μ� TRCKCmdResult)
         kmaEncrypted      = 9,        // ����Ĳ����ͷ���ֵ��ͨѶ���Ƿ����
         kmaAppIsValid     = 10,       // �����ѿ�ʼ��Ӧ���Ƿ���Ч

         // ��չ���Ի���
         kmaBaseParamName  = 0x10000,  // ����Ĳ�����������: ������
         kmaBaseParamType  = 0x20000,  // ����Ĳ�����������: ��������
         kmaBaseParamSize  = 0x30000,  // ����Ĳ�����������: ��������ߴ�
         kmaBaseFieldName  = 0x60000,  // ����ķ����ֶ���������: �����ֶ���
         kmaBaseFieldType  = 0x70000,  // ����ķ����ֶ���������: �����ֶ�����
         kmaBaseFieldSize  = 0x80000}; // ����ķ����ֶ���������: �����ֶζ���ߴ�

// ����ؽ��
enum TRCKCmdResult
        {kmrFailure        = 0,        // ʧ��
         kmrSuccess        = 1,        // �ɹ�
         kmrException      = 2,        // �쳣
         kmrBreak          = 3};       // ��ֹ

// ����״̬
enum TRCKCmdState
        {kmsIdle           = 0,        // ����
         kmsEnding         = 1,        // ���ڽ���
         kmsBeginning      = 2,        // ���ڿ�ʼ
         kmsBegun          = 3,        // �ѿ�ʼ
         kmsExecuting      = 4,        // ����ִ��
         kmsExecuted       = 5,        // ִ�н���, �ѷ���
         kmsCanceled       = 6};       // ��ȡ��(��: ��ʱ�ȵ�)

// Ӧ��״̬
enum TRCKRespState
        {krsIdle           = 0,        // ����
         krsExecuting      = 1,        // ����ִ��
         krsWaitSend       = 2,        // Ӧ����ִ��, ���ڵȴ�����
         krsSending        = 3,        // Ӧ����ִ��, ���ڷ���
         krsSent           = 4,        // Ӧ����ִ��, Ӧ�������
         krsConfirmed      = 5,        // Ӧ����ȷ�Ͻ���
         krsCanceled       = 6};       // Ӧ����ȡ��

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �ص��������Ͷ��� */

// �̶��Ļص�ģʽ
typedef void  (__stdcall *TRCKDoFixed)(void* AFuncObj, void* AConnObj);

// �����¼��Ļص���������
typedef void  (__stdcall *TRCKOnObjEvent)(void* AObject);

// Ӧ�����ӵ�¼�¼��Ļص���������(OnConnLogin)
typedef void  (__stdcall *TRCKOnAppLogin)(void* AppObj, void* AConnObj, bool& AIsRefused);

// Ӧ�����ӵǳ��¼��Ļص���������(OnConnLogout)
typedef void  (__stdcall *TRCKOnAppLogout)(void* AppObj, void* AConnObj);

// Ӧ��Ӧ���¼��Ļص���������(OnExecResp, OnRespCancel, OnRespConfirm)
typedef void  (__stdcall *TRCKOnAppResp)(void* AppObj, void* AFuncObj, void* AConnObj);

// ����������¼��Ļص���������(AResult ֵ�μ� TRCKCmdResult)
typedef void  (__stdcall *TRCKOnCmdReturn)(void* AConnObj, void* ACmdObj, long AResult);

// �����Զ������ݵĻص���������
typedef void  (__stdcall *TRCKOnRecvData)(void* AConnObj, const void* AData, long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �ӿ���غ��� */

// �ӿڳ�ʼ��/�ͷź���
void     __stdcall RCKInitialize();
void     __stdcall RCKUninitialize();

// �ӿڰ汾��Ϣ, ��ʽ: "x.x.x.x (build yyyy.mm.dd)"
// AVersion    ���ذ汾��, ��: 0x0708090A ��ʾ 7.8.9.10
char*    __stdcall RCKGetVersion(long* AVersion);

// ȡ��ǰ��������������(ע: ���ڷ���ֵ�Ǵ�����ĺ���, �� RCKConnCreate �ȵ�)
long     __stdcall RCKGetLastError();

// ����Ӧ����ȡӦ�ö���
void*    __stdcall RCKGetAppObj(const char* AName);

// ȡ�ӿ�����
// (����ֵΪ����ֵ, ARetCode Ϊʧ�ܵķ�����, ����Ϊ��, ���ַ������ͳɹ�ʱΪֵ�ߴ�)
long     __stdcall RCKGetInt(long Attrib, long* ARetCode = NULL);
void*    __stdcall RCKGetObj(long Attrib, long* ARetCode = NULL);
char*    __stdcall RCKGetStr(long Attrib, long* ARetCode = NULL);

// ���ýӿ�����
long     __stdcall RCKSetInt(long Attrib, long  AValue);
long     __stdcall RCKSetObj(long Attrib, void* AValue);
long     __stdcall RCKSetStr(long Attrib, const char* AValue, long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* Application ����غ��� */

// ����/�ͷ�Ӧ��
void*    __stdcall RCKAppCreate(const char* AName, const char* APassword);
void     __stdcall RCKAppFree(void* AppObj);

// ��ʼ/����Ӧ�õĺ�������
long     __stdcall RCKAppFuncBegin(void* AppObj);
long     __stdcall RCKAppFuncEnd(void* AppObj);

// ���ݺ�����ȡӦ�õĺ�������
void*    __stdcall RCKAppFuncObj(void* AppObj, const char* AName);

// ��/�ر�Ӧ��/ǿ��ע��Ӧ�õ�����
long     __stdcall RCKAppOpen(void* AppObj);
long     __stdcall RCKAppClose(void* AppObj);
long     __stdcall RCKAppLogout(void* AppObj, void* AConnObj);

// ȡӦ������
// (����ֵΪ����ֵ, ARetCode Ϊʧ�ܵķ�����, ����Ϊ��, ���ַ������ͳɹ�ʱΪֵ�ߴ�)
long     __stdcall RCKAppGetInt(void* AppObj, long Attrib, long* ARetCode = NULL);
void*    __stdcall RCKAppGetObj(void* AppObj, long Attrib, long* ARetCode = NULL);
char*    __stdcall RCKAppGetStr(void* AppObj, long Attrib, long* ARetCode = NULL);

// ����Ӧ������
long     __stdcall RCKAppSetInt(void* AppObj, long Attrib, long  AValue);
long     __stdcall RCKAppSetObj(void* AppObj, long Attrib, void* AValue);
long     __stdcall RCKAppSetStr(void* AppObj, long Attrib,
                          const char* AValue, long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* Function ����غ��� */

// ����/�ͷ� Function
void*    __stdcall RCKFuncCreate(void* AppObj, long ACallMode, void* ACallback,
                           const char* AName,  long ARetType,  long  ARetSize);
void     __stdcall RCKFuncFree(void* AFuncObj);

// ��� Function ����/�����ֶ�
long     __stdcall RCKFuncAddParam(void* AFuncObj,    const char* AName,
                                   long  AType, long ASize, bool  AIsRef);
long     __stdcall RCKFuncAddField(void* AFuncObj,    const char* AName,
                                   long  AType,             long  ASize);

// ȡ Function ����������/�����ֶ�������
long     __stdcall RCKFuncParamIndex(void* AFuncObj, const char* AName);
long     __stdcall RCKFuncFieldIndex(void* AFuncObj, const char* AName);

// ȡ Function ����
// (����ֵΪ����ֵ, ARetCode Ϊʧ�ܵķ�����, ����Ϊ��, ���ַ������ͳɹ�ʱΪֵ�ߴ�)
long     __stdcall RCKFuncGetInt(void* AFuncObj, long Attrib, long* ARetCode = NULL);
void*    __stdcall RCKFuncGetObj(void* AFuncObj, long Attrib, long* ARetCode = NULL);
char*    __stdcall RCKFuncGetStr(void* AFuncObj, long Attrib, long* ARetCode = NULL);

// ���� Function ����
long     __stdcall RCKFuncSetInt(void* AFuncObj, long Attrib, long  AValue);
long     __stdcall RCKFuncSetObj(void* AFuncObj, long Attrib, void* AValue);
long     __stdcall RCKFuncSetStr(void* AFuncObj, long Attrib,
                           const char* AValue,   long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* Connection ����غ��� */

// ����/�ͷ�����(ע: ASlotObj ����ǿ���Ψһ, ASlotMethods ����ǿ�)
void*    __stdcall RCKConnCreate(long AType, void* ASlotObj,
                            const TRCKSlotMethods* ASlotMethods);
void     __stdcall RCKConnFree(void* AConnObj);

// ��/�ر�����
long     __stdcall RCKConnOpen(void* AConnObj);
long     __stdcall RCKConnClose(void* AConnObj);

// ��¼/�ǳ��Զ�Ӧ��
long     __stdcall RCKConnLogin(void* AConnObj);
long     __stdcall RCKConnLogout(void* AConnObj);

// ���ضԶ�Ӧ�õĺ����б�
long     __stdcall RCKConnLoad(void* AConnObj);

// ��������(���� ANeedPack ��ʾ�����Ƿ���Ҫѹ������)
long     __stdcall RCKConnSendData(void* AConnObj, const void* AData,
                                   long  ASize,          bool  ANeedPack);

// ȡ��������
// (����ֵΪ����ֵ, ARetCode Ϊʧ�ܵķ�����, ����Ϊ��, ���ַ������ͳɹ�ʱΪֵ�ߴ�)
long     __stdcall RCKConnGetInt(void* AConnObj, long Attrib, long* ARetCode = NULL);
void*    __stdcall RCKConnGetObj(void* AConnObj, long Attrib, long* ARetCode = NULL);
char*    __stdcall RCKConnGetStr(void* AConnObj, long Attrib, long* ARetCode = NULL);

// ������������
long     __stdcall RCKConnSetInt(void* AConnObj, long Attrib, long  AValue);
long     __stdcall RCKConnSetObj(void* AConnObj, long Attrib, void* AValue);
long     __stdcall RCKConnSetStr(void* AConnObj, long Attrib,
                           const char* AValue,   long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* Command ����غ��� */

// ����/�ͷ�����
void*    __stdcall RCKCmdCreate();
void     __stdcall RCKCmdFree(void* ACmdObj);

// ��ʼ/��������
long     __stdcall RCKCmdBegin(void* ACmdObj, const char* AName);
long     __stdcall RCKCmdEnd(void* ACmdObj);

// ���¿�ʼ/ִ������
long     __stdcall RCKCmdRestart(void* ACmdObj);
long     __stdcall RCKCmdExecute(void* ACmdObj, unsigned long ATimeout);

// ȡ��������
// (����ֵΪ����ֵ, ARetCode Ϊʧ�ܵķ�����, ����Ϊ��, ���ַ������ͳɹ�ʱΪֵ�ߴ�)
long     __stdcall RCKCmdGetInt(void* ACmdObj, long Attrib, long* ARetCode = NULL);
void*    __stdcall RCKCmdGetObj(void* ACmdObj, long Attrib, long* ARetCode = NULL);
char*    __stdcall RCKCmdGetStr(void* ACmdObj, long Attrib, long* ARetCode = NULL);

// ������������
long     __stdcall RCKCmdSetInt(void* ACmdObj, long Attrib, long  AValue);
long     __stdcall RCKCmdSetObj(void* ACmdObj, long Attrib, void* AValue);
long     __stdcall RCKCmdSetStr(void* ACmdObj, long Attrib,
                          const char* AValue,  long ASize);

// ȡ��ǰ����ĸ����ͷ���ֵ
// (ARetCode Ϊʧ�ܵķ�����, ����Ϊ��, ���ַ�����ṹ���ͳɹ�ʱΪֵ�ߴ�)
char*    __stdcall RCKCmdRetStr(void* ACmdObj, long* ARetCode = NULL);
long     __stdcall RCKCmdRetInt(void* ACmdObj, long* ARetCode = NULL);
bool     __stdcall RCKCmdRetBool(void* ACmdObj, long* ARetCode = NULL);
double   __stdcall RCKCmdRetFloat(void* ACmdObj, long* ARetCode = NULL);
void*    __stdcall RCKCmdRetStruct(void* ACmdObj, long* ARetCode = NULL);
__int64  __stdcall RCKCmdRetInt64(void* ACmdObj, long* ARetCode = NULL);
double   __stdcall RCKCmdRetDate(void* ACmdObj, long* ARetCode = NULL);
long     __stdcall RCKCmdRetCustom(void* ACmdObj, void* AValue, long ASize);

// �����ǰ�������ֵ/ȡ��ǰ����Ĳ���������
long     __stdcall RCKCmdParamClear(void* ACmdObj);
long     __stdcall RCKCmdParamIndex(void* ACmdObj, const char* AName);

// ���ݲ����������õ�ǰ����ĸ����Ͳ���ֵ
long     __stdcall RCKCmdParamByStr(void* ACmdObj, long AIndex,
                              const char* AValue,  long ASize);
long     __stdcall RCKCmdParamByInt(void* ACmdObj, long AIndex, long AValue);
long     __stdcall RCKCmdParamByBool(void* ACmdObj, long AIndex, bool AValue);
long     __stdcall RCKCmdParamByFloat(void* ACmdObj, long AIndex, double AValue);
long     __stdcall RCKCmdParamByStruct(void* ACmdObj, long AIndex, void* AValue);
long     __stdcall RCKCmdParamByInt64(void* ACmdObj, long AIndex, __int64 AValue);
long     __stdcall RCKCmdParamByDate(void* ACmdObj, long AIndex, double AValue);
long     __stdcall RCKCmdParamByCustom(void* ACmdObj, long AIndex,
                                 const void* AValue,  long ASize);

// ���ݲ��������õ�ǰ����ĸ����Ͳ���ֵ
long     __stdcall RCKCmdParamByStr_(void* ACmdObj, const char* AName,
                               const char* AValue,        long  ASize);
long     __stdcall RCKCmdParamByInt_(void* ACmdObj, const char* AName, long AValue);
long     __stdcall RCKCmdParamByBool_(void* ACmdObj, const char* AName, bool AValue);
long     __stdcall RCKCmdParamByFloat_(void* ACmdObj, const char* AName, double AValue);
long     __stdcall RCKCmdParamByStruct_(void* ACmdObj, const char* AName, void* AValue);
long     __stdcall RCKCmdParamByInt64_(void* ACmdObj, const char* AName, __int64 AValue);
long     __stdcall RCKCmdParamByDate_(void* ACmdObj, const char* AName, double AValue);
long     __stdcall RCKCmdParamByCustom_(void* ACmdObj, const char* AName,
                                  const void* AValue,        long  ASize);

// ȡ��ǰ����ķ����ֶ�������/�ֶ�ֵ�Ƿ�Ϊ��(ARetCode Ϊʧ�ܵķ�����, ����Ϊ��)
long     __stdcall RCKCmdFieldIndex(void* ACmdObj, const char* AName);
bool     __stdcall RCKCmdFieldIsNull(void* ACmdObj, long AIndex, long* ARetCode = NULL);
bool     __stdcall RCKCmdFieldIsNull_(void* ACmdObj, const char* AName, long* ARetCode = NULL);

// �����ֶ�����ȡ��ǰ����ĸ������ֶ�ֵ
// (ARetCode Ϊʧ�ܵķ�����, ����Ϊ��, ���ַ�����ṹ���ͳɹ�ʱΪֵ�ߴ�)
char*    __stdcall RCKCmdFieldStr(void* ACmdObj, long AIndex, long* ARetCode = NULL);
long     __stdcall RCKCmdFieldInt(void* ACmdObj, long AIndex, long* ARetCode = NULL);
bool     __stdcall RCKCmdFieldBool(void* ACmdObj, long AIndex, long* ARetCode = NULL);
double   __stdcall RCKCmdFieldFloat(void* ACmdObj, long AIndex, long* ARetCode = NULL);
void*    __stdcall RCKCmdFieldStruct(void* ACmdObj, long AIndex, long* ARetCode = NULL);
__int64  __stdcall RCKCmdFieldInt64(void* ACmdObj, long AIndex, long* ARetCode = NULL);
double   __stdcall RCKCmdFieldDate(void* ACmdObj, long AIndex, long* ARetCode = NULL);
long     __stdcall RCKCmdFieldCustom(void* ACmdObj, long AIndex,
                                     void* AValue,  long ASize);

// �����ֶ���ȡ��ǰ����ĸ������ֶ�ֵ
// (ARetCode Ϊʧ�ܵķ�����, ����Ϊ��, ���ַ�����ṹ���ͳɹ�ʱΪֵ�ߴ�)
char*    __stdcall RCKCmdFieldStr_(void* ACmdObj, const char* AName, long* ARetCode = NULL);
long     __stdcall RCKCmdFieldInt_(void* ACmdObj, const char* AName, long* ARetCode = NULL);
bool     __stdcall RCKCmdFieldBool_(void* ACmdObj, const char* AName, long* ARetCode = NULL);
double   __stdcall RCKCmdFieldFloat_(void* ACmdObj, const char* AName, long* ARetCode = NULL);
void*    __stdcall RCKCmdFieldStruct_(void* ACmdObj, const char* AName, long* ARetCode = NULL);
__int64  __stdcall RCKCmdFieldInt64_(void* ACmdObj, const char* AName, long* ARetCode = NULL);
double   __stdcall RCKCmdFieldDate_(void* ACmdObj, const char* AName, long* ARetCode = NULL);
long     __stdcall RCKCmdFieldCustom_(void* ACmdObj, const char* AName,
                                      void* AValue,        long  ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* Response ����غ��� */

// ȡӦ������Ӷ���/��������/״̬
void*    __stdcall RCKRespConnObj();
void*    __stdcall RCKRespFuncObj();
long     __stdcall RCKRespState();

// ȡӦ��ĸ����ͷ���ֵ
// (ARetCode Ϊʧ�ܵķ�����, ����Ϊ��, ���ַ�����ṹ���ͳɹ�ʱΪֵ�ߴ�)
char*    __stdcall RCKRespRetStr(long* ARetCode = NULL);
long     __stdcall RCKRespRetInt(long* ARetCode = NULL);
bool     __stdcall RCKRespRetBool(long* ARetCode = NULL);
double   __stdcall RCKRespRetFloat(long* ARetCode = NULL);
void*    __stdcall RCKRespRetStruct(long* ARetCode = NULL);
__int64  __stdcall RCKRespRetInt64(long* ARetCode = NULL);
double   __stdcall RCKRespRetDate(long* ARetCode = NULL);
long     __stdcall RCKRespRetCustom(void* AValue, long ASize);

// ��������ȡӦ��ĸ����Ͳ���ֵ
// (ARetCode Ϊʧ�ܵķ�����, ����Ϊ��, ���ַ�����ṹ���ͳɹ�ʱΪֵ�ߴ�)
char*    __stdcall RCKRespParamStr(long AIndex, long* ARetCode = NULL);
long     __stdcall RCKRespParamInt(long AIndex, long* ARetCode = NULL);
bool     __stdcall RCKRespParamBool(long AIndex, long* ARetCode = NULL);
double   __stdcall RCKRespParamFloat(long AIndex, long* ARetCode = NULL);
void*    __stdcall RCKRespParamStruct(long AIndex, long* ARetCode = NULL);
__int64  __stdcall RCKRespParamInt64(long AIndex, long* ARetCode = NULL);
double   __stdcall RCKRespParamDate(long AIndex, long* ARetCode = NULL);
long     __stdcall RCKRespParamCustom(long AIndex, void* AValue, long ASize);

// ���ݲ�����ȡӦ��ĸ����Ͳ���ֵ
// (ARetCode Ϊʧ�ܵķ�����, ����Ϊ��, ���ַ�����ṹ���ͳɹ�ʱΪֵ�ߴ�)
char*    __stdcall RCKRespParamStr_(const char* AName, long* ARetCode = NULL);
long     __stdcall RCKRespParamInt_(const char* AName, long* ARetCode = NULL);
bool     __stdcall RCKRespParamBool_(const char* AName, long* ARetCode = NULL);
double   __stdcall RCKRespParamFloat_(const char* AName, long* ARetCode = NULL);
void*    __stdcall RCKRespParamStruct_(const char* AName, long* ARetCode = NULL);
__int64  __stdcall RCKRespParamInt64_(const char* AName, long* ARetCode = NULL);
double   __stdcall RCKRespParamDate_(const char* AName, long* ARetCode = NULL);
long     __stdcall RCKRespParamCustom_(const char* AName, void* AValue, long ASize);

// ��������ȡӦ��ĸ������ֶη���ֵ
// (ARetCode Ϊʧ�ܵķ�����, ����Ϊ��, ���ַ�����ṹ���ͳɹ�ʱΪֵ�ߴ�)
char*    __stdcall RCKRespFieldStr(long AIndex, long* ARetCode = NULL);
long     __stdcall RCKRespFieldInt(long AIndex, long* ARetCode = NULL);
bool     __stdcall RCKRespFieldBool(long AIndex, long* ARetCode = NULL);
double   __stdcall RCKRespFieldFloat(long AIndex, long* ARetCode = NULL);
void*    __stdcall RCKRespFieldStruct(long AIndex, long* ARetCode = NULL);
__int64  __stdcall RCKRespFieldInt64(long AIndex, long* ARetCode = NULL);
double   __stdcall RCKRespFieldDate(long AIndex, long* ARetCode = NULL);
long     __stdcall RCKRespFieldCustom(long AIndex, void* AValue, long ASize);

// �����ֶ���ȡӦ��ĸ������ֶη���ֵ
// (ARetCode Ϊʧ�ܵķ�����, ����Ϊ��, ���ַ�����ṹ���ͳɹ�ʱΪֵ�ߴ�)
char*    __stdcall RCKRespFieldStr_(const char* AName, long* ARetCode = NULL);
long     __stdcall RCKRespFieldInt_(const char* AName, long* ARetCode = NULL);
bool     __stdcall RCKRespFieldBool_(const char* AName, long* ARetCode = NULL);
double   __stdcall RCKRespFieldFloat_(const char* AName, long* ARetCode = NULL);
void*    __stdcall RCKRespFieldStruct_(const char* AName, long* ARetCode = NULL);
__int64  __stdcall RCKRespFieldInt64_(const char* AName, long* ARetCode = NULL);
double   __stdcall RCKRespFieldDate_(const char* AName, long* ARetCode = NULL);
long     __stdcall RCKRespFieldCustom_(const char* AName, void* AValue, long ASize);

// ����Ӧ��ĸ����ͷ���ֵ
long     __stdcall RCKRespRetByStr(const char* AValue, long ASize);
long     __stdcall RCKRespRetByInt(long AValue);
long     __stdcall RCKRespRetByBool(bool AValue);
long     __stdcall RCKRespRetByFloat(double AValue);
long     __stdcall RCKRespRetByStruct(void* AValue);
long     __stdcall RCKRespRetByInt64(__int64 AValue);
long     __stdcall RCKRespRetByDate(double AValue);
long     __stdcall RCKRespRetByCustom(const void* AValue, long ASize);

// ������������Ӧ��ĸ������ֶη���ֵ
long     __stdcall RCKRespFieldByStr(long AIndex, const char* AValue, long ASize);
long     __stdcall RCKRespFieldByInt(long AIndex, long AValue);
long     __stdcall RCKRespFieldByBool(long AIndex, bool AValue);
long     __stdcall RCKRespFieldByFloat(long AIndex, double AValue);
long     __stdcall RCKRespFieldByStruct(long AIndex, void* AValue);
long     __stdcall RCKRespFieldByInt64(long AIndex, __int64 AValue);
long     __stdcall RCKRespFieldByDate(long AIndex, double AValue);
long     __stdcall RCKRespFieldByCustom(long AIndex, const void* AValue, long ASize);

// �����ֶ�������Ӧ��ĸ������ֶη���ֵ
long     __stdcall RCKRespFieldByStr_(const char* AName,
                                      const char* AValue, long ASize);
long     __stdcall RCKRespFieldByInt_(const char* AName, long AValue);
long     __stdcall RCKRespFieldByBool_(const char* AName, bool AValue);
long     __stdcall RCKRespFieldByFloat_(const char* AName, double AValue);
long     __stdcall RCKRespFieldByStruct_(const char* AName, void* AValue);
long     __stdcall RCKRespFieldByInt64_(const char* AName, __int64 AValue);
long     __stdcall RCKRespFieldByDate_(const char* AName, double AValue);
long     __stdcall RCKRespFieldByCustom_(const char* AName,
                                         const void* AValue, long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* Fields ����غ��� */

// ����/�ͷ��ֶμ�
void*    __stdcall RCKFieldsCreate();
void     __stdcall RCKFieldsFree(void* AFields);

// ����ֶζ���/ɾ��ָ���������ֶμ�����
long     __stdcall RCKFieldDefsAdd(void* AFields, const char* AName,
                                   long  AType,         long  ASize);
void     __stdcall RCKFieldDefsDelete(void* AFields, long AIndex);

// ���/����/����ֶζ��弯/ȡ�ֶζ��弯�����ݳߴ�
void     __stdcall RCKFieldDefsClear(void* AFields);
long     __stdcall RCKFieldDefsAssign(void* AFields, const void* ADefs, long ASize);
void*    __stdcall RCKFieldDefsOutput(void* AFields, long* ARetSize);
long     __stdcall RCKFieldDefsSize(const void* AFieldDefs);

// ���/����/����ֶμ�������/ȡ�ֶμ������ݳߴ�
void     __stdcall RCKFieldsClear(void* AFields);
long     __stdcall RCKFieldsAssign(void* AFields, const void* AData, long ASize);
void*    __stdcall RCKFieldsOutput(void* AFields, long* ARetSize);
long     __stdcall RCKFieldsSize(void* AFields);

// ȡ�ֶθ���/ȡ�ֶ�����������
long     __stdcall RCKFieldCount(void* AFields);
long     __stdcall RCKFieldIndex(void* AFields, const char* AName);

// ȡ�����ֶ���/����/����ߴ�/ֵ�ߴ�
char*    __stdcall RCKFieldName(void* AFields, long AIndex);
long     __stdcall RCKFieldType(void* AFields, long AIndex);
long     __stdcall RCKFieldDefSize(void* AFields, long AIndex);
long     __stdcall RCKFieldSize(void* AFields, long AIndex);

// �ж������ֶ��Ƿ�Ϊ��(ARetCode Ϊʧ�ܵķ�����, ����Ϊ��)
bool     __stdcall RCKFieldIsNull(void* AFields, long AIndex, long* ARetCode = NULL);
bool     __stdcall RCKFieldIsNull_(void* AFields, const char* AName, long* ARetCode = NULL);

// ȡ�����ֶεĸ�����ֵ
// (ARetCode Ϊʧ�ܵķ�����, ����Ϊ��, ���ַ�����ṹ���ͳɹ�ʱΪֵ�ߴ�)
char*    __stdcall RCKFieldAsStr(void* AFields, long AIndex, long* ARetCode = NULL);
long     __stdcall RCKFieldAsInt(void* AFields, long AIndex, long* ARetCode = NULL);
bool     __stdcall RCKFieldAsBool(void* AFields, long AIndex, long* ARetCode = NULL);
double   __stdcall RCKFieldAsFloat(void* AFields, long AIndex, long* ARetCode = NULL);
void*    __stdcall RCKFieldAsStruct(void* AFields, long AIndex, long* ARetCode = NULL);
__int64  __stdcall RCKFieldAsInt64(void* AFields, long AIndex, long* ARetCode = NULL);
double   __stdcall RCKFieldAsDate(void* AFields, long AIndex, long* ARetCode = NULL);
long     __stdcall RCKFieldGetCustom(void* AFields, long AIndex,
                                     void* AValue,  long ASize);

// ȡ�ֶ����ֶεĸ�����ֵ
// (ARetCode Ϊʧ�ܵķ�����, ����Ϊ��, ���ַ�����ṹ���ͳɹ�ʱΪֵ�ߴ�)
char*    __stdcall RCKFieldAsStr_(void* AFields, const char* AName, long* ARetCode = NULL);
long     __stdcall RCKFieldAsInt_(void* AFields, const char* AName, long* ARetCode = NULL);
bool     __stdcall RCKFieldAsBool_(void* AFields, const char* AName, long* ARetCode = NULL);
double   __stdcall RCKFieldAsFloat_(void* AFields, const char* AName, long* ARetCode = NULL);
void*    __stdcall RCKFieldAsStruct_(void* AFields, const char* AName, long* ARetCode = NULL);
__int64  __stdcall RCKFieldAsInt64_(void* AFields, const char* AName, long* ARetCode = NULL);
double   __stdcall RCKFieldAsDate_(void* AFields, const char* AName, long* ARetCode = NULL);
long     __stdcall RCKFieldGetCustom_(void* AFields, const char* AName,
                                      void* AValue,        long  ASize);

// ���������ֶε�����ֵ
long     __stdcall RCKFieldByStr(void* AFields, long AIndex,
                           const char* AValue,  long ASize);
long     __stdcall RCKFieldByInt(void* AFields, long AIndex, long AValue);
long     __stdcall RCKFieldByBool(void* AFields, long AIndex, bool AValue);
long     __stdcall RCKFieldByFloat(void* AFields, long AIndex, double AValue);
long     __stdcall RCKFieldByStruct(void* AFields, long AIndex, void* AValue);
long     __stdcall RCKFieldByInt64(void* AFields, long AIndex, __int64 AValue);
long     __stdcall RCKFieldByDate(void* AFields, long AIndex, double AValue);
long     __stdcall RCKFieldSetCustom(void* AFields, long AIndex,
                               const void* AValue,  long ASize);

// �����ֶ����ֶε�����ֵ
long     __stdcall RCKFieldByStr_(void* AFields, const char* AName,
                            const char* AValue,        long  ASize);
long     __stdcall RCKFieldByInt_(void* AFields, const char* AName, long AValue);
long     __stdcall RCKFieldByBool_(void* AFields, const char* AName, bool AValue);
long     __stdcall RCKFieldByFloat_(void* AFields, const char* AName, double AValue);
long     __stdcall RCKFieldByStruct_(void* AFields, const char* AName, const void* AValue);
long     __stdcall RCKFieldByInt64_(void* AFields, const char* AName, __int64 AValue);
long     __stdcall RCKFieldByDate_(void* AFields, const char* AName, double AValue);
long     __stdcall RCKFieldSetCustom_(void* AFields, const char* AName,
                                const void* AValue,        long  ASize);

}

#endif
