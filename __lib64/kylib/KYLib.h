// =======================================
// Unit   : �������е�Ԫ��ͷ�ļ�
// Version: 3.1.0.1 (build 2014.12.14)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYLib_H_
#define _KYLib_H_

// KYLib 3.0 �����е�ͷ�ļ�
#include "KYAsynchRun.h"
#include "KYCache.h"
#include "KYCallFunc.h"
#include "KYCodeUtils.h"
#include "KYCycBuffer.h"
#include "KYDebug.h"
#include "KYDebugObj.h"
#include "KYFixedPack.h"
#include "KYIndexFile.h"
#include "KYIniFiles.h"
#include "KYLevelTree.h"
#include "KYLogObjs.h"
#include "KYMatch.h"
#include "KYQueueFile.h"
#include "KYQueueFileEx.h"
#include "KYQueueMap.h"
#include "KYQuickPack.h"
#include "KYRecvBuffer.h"
#include "KYScanUtils.h"
#include "KYSmartObjs.h"
#include "KYTable.h"
#include "KYTCPObj.h"
#include "KYTimer.h"
#include "ThreadObjs.h"
#include "TCPSendThreads.h"
#include "UDPSendThreads.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
using namespace KYLib;

// ��̬���������ھ�̬�����ʼ�����Ⱥ�˳������, ���������ȳ�ʼ���ĺ���
inline void InitKYLib(THandle AHandle = NULL)
{
   // ��ʼ�� TDateTime �ĸ�ʽ����
   InitFormatSettings();

   // ����ģ����
   InitHInstance(AHandle);
}

#endif
