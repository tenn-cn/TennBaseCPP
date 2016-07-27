// =======================================
// Unit   : 系统信息单元
// Version: 3.0.0.0 (build 2011.08.24)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYSysInfo_H_
#define _KYSysInfo_H_

#include "KYLib.h"

// KYWin 2.0 开始使用 KYWin 命名空间
namespace KYWin
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 常量定义 */

// CPU 有效掩码位, Bit 0 is processor 0, bit 31 is processor 31
const DWord _CPU_Active[32]   = {0x00000001, 0x00000002, 0x00000004, 0x00000008,
                                 0x00000010, 0x00000020, 0x00000040, 0x00000080,
                                 0x00000100, 0x00000200, 0x00000400, 0x00000800,
                                 0x00001000, 0x00002000, 0x00004000, 0x00008000,
                                 0x00010000, 0x00020000, 0x00040000, 0x00080000,
                                 0x00100000, 0x00200000, 0x00400000, 0x00800000,
                                 0x01000000, 0x02000000, 0x04000000, 0x08000000,
                                 0x10000000, 0x20000000, 0x40000000, 0x80000000};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 类型定义 */

// 系统信息种类
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

// 内存状态结构
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

// SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION 结构项
typedef struct
{
   LARGE_INTEGER  IdleTime;            // 空闲时间(100-nanosecond 间隔个数)
   LARGE_INTEGER  KernelTime;          // 内核模式时间(100-nanosecond 间隔个数)
   LARGE_INTEGER  UserTime;            // 用户模式时间(100-nanosecond 间隔个数)
   LARGE_INTEGER  DpcTime;             // 延迟过程调用时间(100-nanosecond 间隔个数)
   LARGE_INTEGER  InterruptTime;       // 中断时间(100-nanosecond 间隔个数)
   ULONG          InterruptCount;      // 中断次数
} TProcessorPerf, *PProcessorPerf;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 公用函数 */

// 初始化/释放单元
// 注: 调用其它函数之前必须已经调用 InitKYSysInfo 函数初始化
void     InitKYSysInfo();
void     FreeKYSysInfo();

// 取系统内存状态
bool     GetMemoryStatus(TMemoryStatus& AStatus);

// 查询系统信息
bool     QuerySysInfo(DWord AClass, void* AInfo, DWord AInfoLen, DWord& ARetLen);

// CPU 的相关函数
DWord    GetCPUCount();                // 取 CPU 个数
DWord    GetCPUMask();                 // 取 CPU 有效掩码
Word     GetCPUUsage();                // 取 CPU 使用(即是 QueryCPUPerf 最后一次值)
Word     QueryCPUPerf();               // 查询 CPU 执行情况, 值范围: [0..10000]
                                       // 注: 需要单线程定时调用此函数更新 CPU 使用

}

#endif
