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

/* 常量定义 */

// 返回值及错误码
// 见 RCKSlot.h 中的 TRCKResult 定义

// 接口的属性项
enum TSHMAttrib
        {maVersion         = 0,        // 接口版本信息, 格式: "x.x.x.x (build yyyy.mm.dd)"
         maServerCount     = 1,        // SHM 服务端个数
         maRecvThreads     = 2,        // 所有客户端的接收线程最大个数, 默认值: 8
         maCacheThreads    = 3,        // 所有客户端的接收线程缓冲池尺寸, 默认值: 8

         // 扩展属性基数
         maBaseServer      = 0x10000}; // 服务端索引基数: 服务端对象

// 服务端的属性项
enum TSHMServerAttrib
        {msaError          = 0,        // 打开服务失败返回的错误信息
         msaData           = 1,        // 自定义数据
         msaName           = 2,        // 服务端的绑定连接名, 默认值: ""
         msaPort           = 3,        // 服务端的侦听端口号, 默认值: 0
         msaState          = 4,        // 服务端的状态
         msaReqTimeout     = 5,        // 服务端的连接请求超时(单位: 毫秒), 默认值: 30000
         msaListenQueue    = 6,        // 服务端的侦听连接队列尺寸, 默认值: 16
         msaRecvThreads    = 7,        // 接收线程最大个数, 默认值: 8
         msaCacheThreads   = 8,        // 接收线程缓冲池尺寸, 默认值: 8
         msaMaxClientCount = 9,        // 服务端连接的最大客户端个数, 默认值: 0 表示无限制
         msaSrvClientCount = 10,       // 当前服务已连接的客户端个数
         msaCanLogin       = 11,       // 连接是否能够登录对端应用, 默认值: false
         msaCanBindApp     = 12,       // 连接是否能够绑定应用, 默认值: true
         msaBindTimeout    = 13,       // 连接的应用绑定超时(单位: 毫秒), 默认值: 60000, 0 表示不检测超时
         msaSendMaxSize    = 14,       // 通讯槽对象的发送整个包最大长度, 默认值: 8192
         msaSendQueueSize  = 15,       // 连接的 RCKConnSendData 发送队列尺寸, 默认值: 256
         msaTimeout        = 16,       // 连接的操作超时(单位: 毫秒), 默认值: 30000
         msaKeepTimeout    = 17,       // 连接的心跳检测超时时间(单位: 毫秒), 默认值: 30000
         msaKeepInterval   = 18,       // 连接的心跳检测重试间隔(单位: 毫秒), 默认值: 10000
         msaKeepRetryTimes = 19,       // 连接的心跳检测重试次数, 默认值: 3

         // 回调事件属性, 取值范围: [32..63]
         msaOnListen       = 32,       // 服务的 OnListen        事件, 打开侦听(TRCKOnObjEvent)
         msaOnDisconnect   = 33,       // 服务的 OnDisconnect    事件, 断开侦听(TRCKOnObjEvent)
         msaOnAccept       = 34,       // 服务的 OnAccept        事件, 接受连接(TSHMOnAccept), 可以设置连接属性
         msaOnFreeClt      = 35,       // 服务的 OnFreeClt       事件, 释放连接(TRCKOnObjEvent)
         msaOnCltConnect   = 36,       // 连接的 OnCltConnect    事件, 连接成功(TRCKOnObjEvent)
         msaOnCltDisconnect= 37,       // 连接的 OnCltDisconnect 事件, 断开连接(TRCKOnObjEvent)
         msaOnCltLogin     = 38,       // 连接的 OnCltLogin      事件, 登录对端应用(TRCKOnObjEvent)
         msaOnCltLogout    = 39,       // 连接的 OnCltLogout     事件, 登出对端应用(TRCKOnObjEvent)
         msaOnCltCmdReturn = 40,       // 连接的 OnCltCmdReturn  事件, 命令返回(TRCKOnCmdReturn)
         msaOnCltRecvData  = 41,       // 连接的 OnCltRecvData   事件, 接收数据(TRCKOnRecvData)

         // 扩展属性基数
         msaBaseSrvClient  = 0x10000}; // 已连接的子项索引基数: 子项对象

// 服务端的状态
enum TSHMServerState
        {mssInactive       = 0,        // 未打开
         mssClosing        = 1,        // 正在关闭
         mssOpening        = 2,        // 正在打开
         mssOpened         = 3};       // 已经打开

// 服务端客户连接的属性项(即是连接通讯槽对象的扩展属性项, kcaBaseSlotExt + Attrib)
enum TSHMSrvCltAttrib
        {mscaData          = 0,        // 自定义数据
         mscaSrvObj        = 1,        // 连接的服务端对象
         mscaIsValid       = 2};       // 连接是否有效

// 客户端的属性项(即是连接通讯槽对象的扩展属性项, kcaBaseSlotExt + Attrib)
enum TSHMClientAttrib
        {mcaError          = 0,        // 打开客户端失败返回的错误信息
         mcaData           = 1};       // 自定义数据

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 接口回调函数类型定义 */

// 服务端接受连接事件的回调函数类型(OnAccept)
typedef void (__stdcall *TSHMOnAccept)(void* ASrvObj, void* AConnObj, bool& AIsRefused);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 接口相关函数 */

// 接口初始化/释放函数
void  __stdcall SHMInitialize();
void  __stdcall SHMUninitialize();

// 接口版本信息, 格式: "x.x.x.x (build yyyy.mm.dd)"
// AVersion    返回版本号, 如: 0x0708090A 表示 7.8.9.10
char* __stdcall SHMGetVersion(long* AVersion);

// 取当前操作的最后错误码(注: 用于返回值非错误码的函数)
long  __stdcall SHMGetLastError();

// 取接口属性
// (返回值为属性值, ARetCode 为失败的返回码, 允许为空, 当字符串类型成功时为值尺寸)
long  __stdcall SHMGetInt(long Attrib, long* ARetCode = NULL);
void* __stdcall SHMGetObj(long Attrib, long* ARetCode = NULL);
char* __stdcall SHMGetStr(long Attrib, long* ARetCode = NULL);

// 设置接口属性
long  __stdcall SHMSetInt(long Attrib, long  AValue);
long  __stdcall SHMSetObj(long Attrib, void* AValue);
long  __stdcall SHMSetStr(long Attrib, const char* AValue, long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* SHM 服务端的相关函数 */

// 创建/释放 SHM 服务端
void* __stdcall SHMServerCreate();
void  __stdcall SHMServerFree(void* ASrvObj);

// 打开/关闭 SHM 服务端
long  __stdcall SHMServerOpen(void* ASrvObj);
long  __stdcall SHMServerClose(void* ASrvObj);

// 取 SHM 服务端属性
// (返回值为属性值, ARetCode 为失败的返回码, 允许为空, 当字符串类型成功时为值尺寸)
long  __stdcall SHMServerGetInt(void* ASrvObj, long Attrib, long* ARetCode = NULL);
void* __stdcall SHMServerGetObj(void* ASrvObj, long Attrib, long* ARetCode = NULL);
char* __stdcall SHMServerGetStr(void* ASrvObj, long Attrib, long* ARetCode = NULL);

// 设置 SHM 服务端属性
long  __stdcall SHMServerSetInt(void* ASrvObj, long Attrib, long  AValue);
long  __stdcall SHMServerSetObj(void* ASrvObj, long Attrib, void* AValue);
long  __stdcall SHMServerSetStr(void* ASrvObj, long Attrib,
                          const char* AValue,  long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* SHM 客户端的相关函数 */

// 创建 SHM 客户端, 返回 RCKConnCreate 创建的对象
// 注:  SHM 客户端的释放调用 RCKConnFree 即可
//      SHM 客户端的属性是连接通讯槽对象的扩展属性项, kcaBaseSlotExt + Attrib
void* __stdcall SHMClientCreate();

}

#endif
