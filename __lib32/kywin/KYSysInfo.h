// =======================================
// Unit   : ϵͳ��Ϣ��Ԫ
// Version: 3.0.0.0 (build 2011.08.24)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYSysInfo_H_
#define _KYSysInfo_H_

#include "KYLib.h"

// KYWin 2.0 ��ʼʹ�� KYWin �����ռ�
namespace KYWin
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �������� */

// CPU ��Ч����λ, Bit 0 is processor 0, bit 31 is processor 31
const DWord _CPU_Active[32]   = {0x00000001, 0x00000002, 0x00000004, 0x00000008,
                                 0x00000010, 0x00000020, 0x00000040, 0x00000080,
                                 0x00000100, 0x00000200, 0x00000400, 0x00000800,
                                 0x00001000, 0x00002000, 0x00004000, 0x00008000,
                                 0x00010000, 0x00020000, 0x00040000, 0x00080000,
                                 0x00100000, 0x00200000, 0x00400000, 0x00800000,
                                 0x01000000, 0x02000000, 0x04000000, 0x08000000,
                                 0x10000000, 0x20000000, 0x40000000, 0x80000000};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ���Ͷ��� */

// ϵͳ��Ϣ����
enum TSysInfoClass
        {sicBase              = 0,     // SYSTEM_BASIC_INFORMATION structure
         sicPerformance       = 2,     // SYSTEM_PERFORMANCE_INFORMATION structure
         sicTimeOfDay         = 3,     // SYSTEM_TIMEOFDAY_INFORMATION structure
         sicProcess           = 5,     // SYSTEM_PROCESS_INFORMATION structures
         sicProcessorPerf     = 8,     // SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION structures
         sicInterrupt         = 23,    // SYSTEM_INTERRUPT_INFORMATION structure
         sicException         = 33,    // SYSTEM_EXCEPTION_INFORMATION structure
         sicRegistryQuota     = 37,    // SYSTEM_REGISTRY_QUOTA_INFORMATION structure
         sicLookaside         = 45};   // SYSTEM_LOOKASIDE_INFORMATION structure

// �ڴ�״̬�ṹ
typedef struct
{
   DWord          dwLength;
   DWord          dwMemoryLoad;
   QWord          ullTotalPhys;
   QWord          ullAvailPhys;
   QWord          ullTotalPageFile;
   QWord          ullAvailPageFile;
   QWord          ullTotalVirtual;
   QWord          ullAvailVirtual;
   QWord          ullAvailExtendedVirtual;
} TMemoryStatus, *PMemoryStatus;

// SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION �ṹ��
typedef struct
{
   LARGE_INTEGER  IdleTime;            // ����ʱ��(100-nanosecond �������)
   LARGE_INTEGER  KernelTime;          // �ں�ģʽʱ��(100-nanosecond �������)
   LARGE_INTEGER  UserTime;            // �û�ģʽʱ��(100-nanosecond �������)
   LARGE_INTEGER  DpcTime;             // �ӳٹ��̵���ʱ��(100-nanosecond �������)
   LARGE_INTEGER  InterruptTime;       // �ж�ʱ��(100-nanosecond �������)
   ULONG          InterruptCount;      // �жϴ���
} TProcessorPerf, *PProcessorPerf;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ���ú��� */

// ��ʼ��/�ͷŵ�Ԫ
// ע: ������������֮ǰ�����Ѿ����� InitKYSysInfo ������ʼ��
void     InitKYSysInfo();
void     FreeKYSysInfo();

// ȡϵͳ�ڴ�״̬
bool     GetMemoryStatus(TMemoryStatus& AStatus);

// ��ѯϵͳ��Ϣ
bool     QuerySysInfo(DWord AClass, void* AInfo, DWord AInfoLen, DWord& ARetLen);

// CPU ����غ���
DWord    GetCPUCount();                // ȡ CPU ����
DWord    GetCPUMask();                 // ȡ CPU ��Ч����
Word     GetCPUUsage();                // ȡ CPU ʹ��(���� QueryCPUPerf ���һ��ֵ)
Word     QueryCPUPerf();               // ��ѯ CPU ִ�����, ֵ��Χ: [0..10000]
                                       // ע: ��Ҫ���̶߳�ʱ���ô˺������� CPU ʹ��

}

#endif
