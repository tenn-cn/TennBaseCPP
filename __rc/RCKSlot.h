// =======================================
// Unit   : RCK Slot (RCK ͨѶ�۽ӿ�)
// Version: 5.0.0.0 (build 2014.07.07)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _RCKSlot_H_
#define _RCKSlot_H_

namespace RCKernel
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �������� */

// ����ֵ��������
enum TRCKResult
        {krSuccess         =  1,       // �ɹ�
         krFailure         =  0,       // ʧ��
         krUnknown         = -1,       // δ֪����
         krNotExist        = -2,       // ������(��: ���Ӷ���)
         krNotConnect      = -3,       // ����δ��
         krNonsupport      = -4,       // ��֧��
         krVersion         = -5,       // �汾������
         krTimeout         = -6,       // ������ʱ
         krPassword        = -7,       // �������
         krIsExisted       = -8,       // �Ѵ���
         krIsRefused       = -9,       // ���ܾ�
         krIsIllegal       = -10,      // ���Ϸ�
         krIsNullName      = -11,      // ����Ϊ��
         krAttrInvalid     = -12,      // ������Ч
         krStateInvalid    = -13,      // ״̬��Ч
         krHandleInvalid   = -14,      // �����Ч
         krAccessIllegal   = -15};     // ��ȡ�Ƿ�

// ͨѶ�۵�������
enum TRCKSlotAttrib
        {ksaError          = 0,        // ͨѶ�۴�ʧ�ܵĴ�����Ϣ
         ksaState          = 1,        // ͨѶ�۵�״̬
         ksaConnObj        = 2,        // ͨѶ�۵İ����Ӷ���
         ksaCurrAddr       = 3,        // ͨѶ�۵ı��ص�ַ
         ksaCurrPort       = 4,        // ͨѶ�۵ı��ض˿ں�
         ksaPeerAddr       = 5,        // ͨѶ�۵ĶԶ˵�ַ
         ksaPeerPort       = 6,        // ͨѶ�۵ĶԶ˶˿ں�
         ksaSlotMaxSize    = 7,        // ͨѶ�۵����ݰ���󳤶�, Ĭ��ֵ: 8192

         // �ص��¼�����, ȡֵ��Χ: [32..63]
         ksaOnDisconnect   = 32,       // ͨѶ�۵� OnDisconnect �¼�, �Ͽ�ͨѶ(TRCKSlotOnEvent)
         ksaOnRecvEvent    = 33,       // ͨѶ�۵� OnRecvEvent  �¼�, �����¼�(TRCKSlotOnEvent)
         ksaOnRecvData     = 34,       // ͨѶ�۵� OnRecvData   �¼�, ��������(TRCKSlotOnRecvData)

         // ��չ���Ի���
         ksaBaseExtended   = 0x10000}; // ͨѶ�۵���չ���Ի���: ��չ������
                                       // (ע: ��չ���������С�� 0x10000)

// ͨѶ�۵�״̬
enum TRCKSlotState
        {kssInactive       = 0,        // δ��
         kssClosing        = 1,        // ���ڹر�
         kssOpening        = 2,        // ���ڴ�
         kssOpened         = 3};       // �Ѿ���

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ͨѶ�۶���ĺ������Ͷ��� */

// ����ͨѶ�۵ĺ�������
typedef void  (__stdcall *TRCKSlotOp)(void* ASlotObj);

// ��ͨѶ�۵ĺ�������(����ֵ����Ϊ TRCKResult)
typedef long  (__stdcall *TRCKSlotOpen)(void* ASlotObj, long ATimeout);

// �������ݵĺ�������(����ֵΪ���ճɹ������ݳߴ�, ��С�� 0 ��Ϊ������)
typedef long  (__stdcall *TRCKSlotRecv)(void* ASlotObj, void* AData, long ASize);

// �������ݵĺ�������(����ֵΪ���ͳɹ������ݳߴ�, ��С�� 0 ��Ϊ������)
typedef long  (__stdcall *TRCKSlotSend)(void* ASlotObj, const void* AData, long ASize);

// ���Է��͵ĺ�������(����ֵΪ���Է��͵����ݳߴ�, ��С�� 0 ��Ϊ������)
typedef long  (__stdcall *TRCKSlotCanSend)(void* ASlotObj, long ASize);

// ȡͨѶ���������Եĺ�������(����ֵΪ����ֵ, ARetCode Ϊʧ�ܵķ�����, ����Ϊ��)
typedef long  (__stdcall *TRCKSlotGetInt)(void* ASlotObj, long Attrib, long* ARetCode);

// ȡͨѶ��ָ�����Եĺ�������(����ֵΪ����ֵ, ARetCode Ϊʧ�ܵķ�����, ����Ϊ��)
typedef void* (__stdcall *TRCKSlotGetObj)(void* ASlotObj, long Attrib, long* ARetCode);

// ȡͨѶ���ַ������Եĺ�������(����ֵΪ����ֵ, ARetCode �ɹ�ʱΪ����ֵ�ߴ�, ����Ϊ������, ����Ϊ��)
typedef char* (__stdcall *TRCKSlotGetStr)(void* ASlotObj, long Attrib, long* ARetCode);

// ����ͨѶ���������Եĺ�������
typedef long  (__stdcall *TRCKSlotSetInt)(void* ASlotObj, long Attrib, long  AValue);

// ����ͨѶ��ָ�����Եĺ�������
typedef long  (__stdcall *TRCKSlotSetObj)(void* ASlotObj, long Attrib, void* AValue);

// ����ͨѶ���ַ������Եĺ�������
typedef long  (__stdcall *TRCKSlotSetStr)(void* ASlotObj, long Attrib,
                                    const char* AValue,   long ASize);

// ͨѶ�۶����¼��Ļص���������
typedef void  (__stdcall *TRCKSlotOnEvent)(void* ASlotObj, void* AConnObj);

// ͨѶ�۶��� OnRecvData �¼��Ļص���������
// ע: �� Recv ����Ϊ NULL ��ͨ�� OnRecvData �¼���������,
//     ����ͨ�� OnRecvEvent  �¼������� Recv ������������.
typedef void  (__stdcall *TRCKSlotOnRecvData)(void* ASlotObj, void* AConnObj,
                                        const void* AData,    long  ASize);

// ͨѶ�۶���ķ����б�
#pragma pack(push, 1)
typedef struct
{
   long              Size;             // = sizeof(TRCKSlotMethods)

   TRCKSlotOpen      Open;             // ��ͨѶ��
   TRCKSlotOp        Close;            // �ر�ͨѶ��
   TRCKSlotOp        FreeObj;          // �ͷ�ͨѶ�۶���

   TRCKSlotRecv      Recv;             // ��������
   TRCKSlotSend      Send;             // ��������
   TRCKSlotCanSend   CanSend;          // ���Է��͵����ݳߴ�

   TRCKSlotGetInt    GetInt;           // ȡͨѶ����������
   TRCKSlotGetObj    GetObj;           // ȡͨѶ��ָ������
   TRCKSlotGetStr    GetStr;           // ȡͨѶ���ַ�������

   TRCKSlotSetInt    SetInt;           // ����ͨѶ����������
   TRCKSlotSetObj    SetObj;           // ����ͨѶ��ָ������
   TRCKSlotSetStr    SetStr;           // ����ͨѶ���ַ�������
} TRCKSlotMethods, *PRCKSlotMethods;
#pragma pack(pop)

}

#endif
