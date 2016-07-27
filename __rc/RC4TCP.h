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

/* 常量定义 */

// 返回值及错误码
// 见 RCKSlot.h 中的 TRCKResult 定义

// 接口的属性项
enum TTCPAttrib
        {taVersion         = 0,        // 接口版本信息, 格式: "x.x.x.x (build yyyy.mm.dd)"
         taServerCount     = 1,        // TCP 服务端个数
         taRecvThreads     = 2,        // 所有客户端的接收线程最大个数, 默认值: 8
         taCacheThreads    = 3,        // 所有客户端的接收线程缓冲池尺寸, 默认值: 8

         // 扩展属性基数
         taBaseServer      = 0x10000}; // 服务端索引基数: 服务端对象

// 服务端的属性项
enum TTCPServerAttrib
        {tsaError          = 0,        // 打开服务失败返回的错误信息
         tsaData           = 1,        // 自定义数据
         tsaAddr           = 2,        // 服务端的绑定IP地址, 默认值: ""
         tsaPort           = 3,        // 服务端的侦听端口号, 默认值: 0
         tsaState          = 4,        // 服务端的状态
         tsaLinger         = 5,        // TCP 句柄关闭后的拖延时长(秒), 默认值: 0
         tsaListenQueue    = 6,        // 服务端的侦听连接队列尺寸, 默认值: 5
         tsaRecvThreads    = 7,        // 接收线程最大个数, 默认值: 8
         tsaCacheThreads   = 8,        // 接收线程缓冲池尺寸, 默认值: 8
         tsaMaxClientCount = 9,        // 服务端连接的最大客户端个数, 默认值: 0 表示无限制
         tsaSrvClientCount = 10,       // 当前服务已连接的客户端个数
         tsaCanLogin       = 11,       // 连接是否能够登录对端应用, 默认值: false
         tsaCanBindApp     = 12,       // 连接是否能够绑定应用, 默认值: true
         tsaBindTimeout    = 13,       // 连接的应用绑定超时(单位: 毫秒), 默认值: 60000, 0 表示不检测超时
         tsaSendMaxSize    = 14,       // 通讯槽对象的发送整个包最大长度, 默认值: 8192
         tsaSendQueueSize  = 15,       // 连接的 RCKConnSendData 发送队列尺寸, 默认值: 256
         tsaTimeout        = 16,       // 连接的操作超时(单位: 毫秒), 默认值: 30000
         tsaKeepTimeout    = 17,       // 连接的心跳检测超时时间(单位: 毫秒), 默认值: 30000
         tsaKeepInterval   = 18,       // 连接的心跳检测重试间隔(单位: 毫秒), 默认值: 10000
         tsaKeepRetryTimes = 19,       // 连接的心跳检测重试次数, 默认值: 3

         // 回调事件属性, 取值范围: [32..63]
         tsaOnListen       = 32,       // 服务的 OnListen        事件, 打开侦听(TRCKOnObjEvent)
         tsaOnDisconnect   = 33,       // 服务的 OnDisconnect    事件, 断开侦听(TRCKOnObjEvent)
         tsaOnAccept       = 34,       // 服务的 OnAccept        事件, 接受连接(TTCPOnAccept), 可以设置连接属性
         tsaOnFreeClt      = 35,       // 服务的 OnFreeClt       事件, 释放连接(TRCKOnObjEvent)
         tsaOnCltConnect   = 36,       // 连接的 OnCltConnect    事件, 连接成功(TRCKOnObjEvent)
         tsaOnCltDisconnect= 37,       // 连接的 OnCltDisconnect 事件, 断开连接(TRCKOnObjEvent)
         tsaOnCltLogin     = 38,       // 连接的 OnCltLogin      事件, 登录对端应用(TRCKOnObjEvent)
         tsaOnCltLogout    = 39,       // 连接的 OnCltLogout     事件, 登出对端应用(TRCKOnObjEvent)
         tsaOnCltCmdReturn = 40,       // 连接的 OnCltCmdReturn  事件, 命令返回(TRCKOnCmdReturn)
         tsaOnCltRecvData  = 41,       // 连接的 OnCltRecvData   事件, 接收数据(TRCKOnRecvData)

         // 扩展属性基数
         tsaBaseSrvClient  = 0x10000}; // 已连接的客户端索引基数: 客户端对象

// 服务端的状态
enum TTCPServerState
        {tssInactive       = 0,        // 未打开
         tssClosing        = 1,        // 正在关闭
         tssOpening        = 2,        // 正在打开
         tssOpened         = 3};       // 已经打开

// 服务端客户连接的属性项(即是连接通讯槽对象的扩展属性项, kcaBaseSlotExt + Attrib)
enum TTCPSrvCltAttrib
        {tscaData          = 0,        // 自定义数据
         tscaSrvObj        = 1,        // 连接的服务端对象
         tscaIsValid       = 2};       // 连接是否有效

// 客户端的属性项(即是连接通讯槽对象的扩展属性项, kcaBaseSlotExt + Attrib)
enum TTCPClientAttrib
        {tcaError          = 0,        // 打开客户端失败返回的错误信息
         tcaData           = 1,        // 自定义数据
         tcaLinger         = 2};       // TCP 句柄关闭后的拖延时长(秒), 默认值: 0

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 接口回调函数类型定义 */

// 服务端接受连接事件的回调函数类型(OnAccept)
typedef void (__stdcall *TTCPOnAccept)(void* ASrvObj, void* AConnObj, bool& AIsRefused);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 接口相关函数 */

// 接口初始化/释放函数
void  __stdcall TCPInitialize();
void  __stdcall TCPUninitialize();

// 接口版本信息, 格式: "x.x.x.x (build yyyy.mm.dd)"
// AVersion    返回版本号, 如: 0x0708090A 表示 7.8.9.10
char* __stdcall TCPGetVersion(long* AVersion);

// 取当前操作的最后错误码(注: 用于返回值非错误码的函数)
long  __stdcall TCPGetLastError();

// 取接口属性
// (返回值为属性值, ARetCode 为失败的返回码, 允许为空, 当字符串类型成功时为值尺寸)
long  __stdcall TCPGetInt(long Attrib, long* ARetCode = NULL);
void* __stdcall TCPGetObj(long Attrib, long* ARetCode = NULL);
char* __stdcall TCPGetStr(long Attrib, long* ARetCode = NULL);

// 设置接口属性
long  __stdcall TCPSetInt(long Attrib, long  AValue);
long  __stdcall TCPSetObj(long Attrib, void* AValue);
long  __stdcall TCPSetStr(long Attrib, const char* AValue, long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TCP 服务端的相关函数 */

// 创建/释放 TCP 服务端
void* __stdcall TCPServerCreate();
void  __stdcall TCPServerFree(void* ASrvObj);

// 打开/关闭 TCP 服务端
long  __stdcall TCPServerOpen(void* ASrvObj);
long  __stdcall TCPServerClose(void* ASrvObj);

// 取 TCP 服务端属性
// (返回值为属性值, ARetCode 为失败的返回码, 允许为空, 当字符串类型成功时为值尺寸)
long  __stdcall TCPServerGetInt(void* ASrvObj, long Attrib, long* ARetCode = NULL);
void* __stdcall TCPServerGetObj(void* ASrvObj, long Attrib, long* ARetCode = NULL);
char* __stdcall TCPServerGetStr(void* ASrvObj, long Attrib, long* ARetCode = NULL);

// 设置 TCP 服务端属性
long  __stdcall TCPServerSetInt(void* ASrvObj, long Attrib, long  AValue);
long  __stdcall TCPServerSetObj(void* ASrvObj, long Attrib, void* AValue);
long  __stdcall TCPServerSetStr(void* ASrvObj, long Attrib,
                          const char* AValue,  long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TCP 客户端的相关函数 */

// 创建 TCP 客户端, 返回 RCKConnCreate 创建的对象
// 注:  TCP 客户端的释放调用 RCKConnFree 即可
//      TCP 客户端的属性是连接通讯槽对象的扩展属性项, kcaBaseSlotExt + Attrib
void* __stdcall TCPClientCreate();

}

#endif
