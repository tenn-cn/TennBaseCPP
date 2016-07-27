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

/* 常量定义 */

// 数据类型
enum TRCKDataType
        {kdtNone           = 0,        // 无类型
         kdtString         = 1,        // 字符串类型  (自定义尺寸)
         kdtInteger        = 2,        // 整型类型    (4 Bytes)
         kdtBoolean        = 3,        // 布尔类型    (1 Bytes)
         kdtDouble         = 4,        // 浮点类型    (8 Bytes)
         kdtStruct         = 5,        // 结构类型    (自定义尺寸)
         kdtByte           = 6,        // 字节        (1 Bytes)
         kdtWord           = 7,        // 双字节      (2 Bytes)
         kdtSmallint       = 8,        // 16位整型    (2 Bytes)
         kdtInt64          = 9,        // 64位整型    (8 Bytes)
         kdtDateTime       = 10,       // 日期时间类型(8 Bytes)
         kdtCurrency       = 11};      // 货币类型    (8 Bytes)

// 接口的属性项
enum TRCKAttrib
        {kaVersion         = 0,        // 接口版本信息, 格式: "x.x.x.x (build yyyy.mm.dd)"
         kaAppCount        = 1,        // 应用个数
         kaCmdCount        = 2,        // 命令个数
         kaConnCount       = 3,        // 连接个数
         kaDealThreads     = 4,        // 处理线程最大个数, 默认值: 16
         kaDealCacheSize   = 5,        // 处理线程缓冲池尺寸, 默认值: 16
         kaSendThreads     = 6,        // 发送线程最大个数, 默认值: 8
         kaSendCacheSize   = 7,        // 发送线程缓冲池尺寸, 默认值: 8

         // 扩展属性基数
         kaBaseAppObj      = 0x10000,  // 应用索引基数: 应用对象
         kaBaseAppName     = 0x20000,  // 应用索引基数: 应用名
         kaBaseConnObj     = 0x40000}; // 连接索引基数: 连接对象

// 应用的属性项
enum TRCKAppAttrib
        {kaaData           = 0,        // 自定义数据
         kaaName           = 1,        // 应用名
         kaaState          = 2,        // 应用的状态
         kaaPassword       = 3,        // 应用的密码
         kaaFuncCount      = 4,        // 应用的函数个数
         kaaConnCount      = 5,        // 应用的已连接数
         kaaMaxThreads     = 6,        // 应用的回调函数处理线程最大个数, 默认值: 8
         kaaMaxConnCount   = 7,        // 应用的最大连接个数, 默认值: 0 表示无限制

         // 回调事件属性, 取值范围: [32..63]
         kaaOnConnLogin    = 32,       // 应用的 OnConnLogin   事件, 连接登录(TRCKOnAppLogin)
         kaaOnConnLogout   = 33,       // 应用的 OnConnLogout  事件, 连接登出(TRCKOnAppLogout)
         kaaOnExecResp     = 34,       // 应用的 OnExecResp    事件, 执行应答(TRCKOnAppResp)
         kaaOnRespCancel   = 35,       // 应用的 OnRespCancel  事件, 应答已取消(TRCKOnAppResp)
         kaaOnRespConfirm  = 36,       // 应用的 OnRespConfirm 事件, 应答已确认(TRCKOnAppResp)

         // 扩展属性基数
         kaaBaseFuncObj    = 0x10000,  // 应用的函数索引基数: 函数对象
         kaaBaseFuncName   = 0x20000,  // 应用的函数索引基数: 函数名
         kaaBaseConnObj    = 0x40000}; // 应用的连接索引基数: 连接对象

// 应用的状态
enum TRCKAppState
        {kasInactive       = 0,        // 函数未定义
         kasDefining       = 1,        // 函数正在定义
         kasDefined        = 2,        // 函数已定义
         kasClosing        = 3,        // 正在关闭
         kasOpening        = 4,        // 正在打开
         kasOpened         = 5};       // 已打开

// 函数的属性项
enum TRCKFuncAttrib
        {kfaData           = 0,        // 自定义数据
         kfaName           = 1,        // 函数名
         kfaActive         = 2,        // 函数是否已激活
         kfaAppObj         = 3,        // 函数所属应用对象
         kfaRetType        = 4,        // 函数返回值类型
         kfaRetSize        = 5,        // 函数返回值定义尺寸
         kfaCallMode       = 6,        // 回调函数模式
         kfaCallback       = 7,        // 回调函数指针
         kfaParamCount     = 8,        // 函数的参数个数
         kfaFieldCount     = 9,        // 函数的返回字段个数
         kfaNeedConfirm    = 10,       // 函数应答返回是否需要确认
         kfaNeedEncrypt    = 11,       // 函数参数和返回值在通讯中是否需要加密

         // 扩展属性基数
         kfaBaseParamName  = 0x10000,  // 函数参数索引基数: 参数名
         kfaBaseParamType  = 0x20000,  // 函数参数索引基数: 参数类型
         kfaBaseParamSize  = 0x30000,  // 函数参数索引基数: 参数定义尺寸
         kfaBaseParamIsRef = 0x40000,  // 函数参数索引基数: 参数是否为引用指针
         kfaBaseFieldName  = 0x60000,  // 函数返回字段索引基数: 返回字段名
         kfaBaseFieldType  = 0x70000,  // 函数返回字段索引基数: 返回字段类型
         kfaBaseFieldSize  = 0x80000}; // 函数返回字段索引基数: 返回字段定义尺寸

// 回调函数模式
enum TRCKFuncCallMode
        {kfcmNone          = 0,        // 无, 请求通过 OnExecResp 事件返回
         kfcmFixed         = 1,        // 固定的回调模式(TRCKDoFixed类型)
         kfcmCdecl         = 2,        // C 语言函数调用协议
         kfcmStdcall       = 3,        // API 标准函数调用协议
         kfcmIDCdecl       = 4,        // 自动添加 AConnObj 参数的 C 语言函数调用协议
         kfcmIDStdcall     = 5};       // 自动添加 AConnObj 参数的 API 标准函数调用协议

// 连接的属性项
enum TRCKConnAttrib
        {kcaError          = 0,        // 错误信息
         kcaData           = 1,        // 自定义数据
         kcaType           = 2,        // 连接的类型
         kcaState          = 3,        // 连接的状态
         kcaSlotObj        = 4,        // 连接绑定的通讯槽对象(用于TRCKSlotMethods方法列表)
         kcaCanLogin       = 5,        // 连接是否能够登录对端应用, 默认值: (Type == kctTCPClient)
         kcaCanBindApp     = 6,        // 连接是否能够绑定应用, 默认值: (Type == kctTCPSrvClient)
         kcaBindAppObj     = 7,        // 连接绑定的应用对象, 默认值: NULL
         kcaBindTimeout    = 8,        // 连接的应用绑定超时(单位: 毫秒), 默认值: 0 表示不检测超时
         kcaIsSyncLink     = 9,        // 通讯槽对象打开成功后是否同步建立连接, 默认值: (Type == kctTCPClient)
         kcaSlotTimeout    = 10,       // 通讯槽对象的打开超时(单位: 毫秒), 默认值: 30000
         kcaSlotMaxSize    = 11,       // 通讯槽对象的数据包最大长度, 默认值: 8192
         kcaSendQueueSize  = 12,       // 连接的 RCKConnSendData 发送队列尺寸, 默认值: 256
         kcaTimeout        = 13,       // 连接的操作超时(单位: 毫秒), 默认值: 30000
         kcaAppName        = 14,       // 登录对端应用名
         kcaAppPass        = 15,       // 登录对端应用密码
         kcaCurrAddr       = 16,       // 连接的本地地址
         kcaCurrPort       = 17,       // 连接的本地端口号
         kcaPeerAddr       = 18,       // 连接的对端地址
         kcaPeerPort       = 19,       // 连接的对端端口号
         kcaLastTick       = 20,       // 连接的最后一次收发数据时 tick
         kcaFuncCount      = 21,       // 已加载对端应用的函数个数, 默认值: -1 表示未加载
         kcaKeepTimeout    = 22,       // 连接的心跳检测超时时间(单位: 毫秒), 默认值: 30000
         kcaKeepInterval   = 23,       // 连接的心跳检测重试间隔(单位: 毫秒), 默认值: 10000
         kcaKeepRetryTimes = 24,       // 连接的心跳检测重试次数, 默认值: 3

         // 回调事件属性, 取值范围: [32..63]
         kcaOnConnect      = 32,       // 连接的 OnConnect     事件, 连接成功(TRCKOnObjEvent)
         kcaOnDisconnect   = 33,       // 连接的 OnDisconnect  事件, 断开连接(TRCKOnObjEvent)
         kcaOnLogin        = 34,       // 连接的 OnLogin       事件, 登录对端应用(TRCKOnObjEvent)
         kcaOnLogout       = 35,       // 连接的 OnLogout      事件, 登出对端应用(TRCKOnObjEvent)
         kcaOnCmdReturn    = 36,       // 连接的 OnCmdReturn   事件, 命令返回(TRCKOnCmdReturn)
         kcaOnRecvData     = 37,       // 连接的 OnRecvData    事件, 接收数据(TRCKOnRecvData)

         // 扩展属性基数
         kcaBaseSlotExt    = 0x10000,  // 通讯槽对象的扩展属性基数: 扩展属性项
         kcaBaseFuncName   = 0x20000}; // 已加载对端应用的函数索引基数: 函数名

// 连接的类型
enum TRCKConnType
        {kctUnknown        = 0,        // (未知)
         kctTCPClient      = 1,        // TCP 客户端
         kctTCPSrvClient   = 2,        // TCP 服务端的客户连接
         kctSHMClient      = 3,        // SHM 客户端(共享内存通讯)
         kctSHMSrvClient   = 4,        // SHM 服务端的客户连接(共享内存通讯)
         // ... ...                    // 可以根据 slot 通讯方式自定义扩充
         };

// 连接的状态
enum TRCKConnState
        {kcsInactive       = 0,        // 未打开
         kcsClosing        = 1,        // 正在关闭
         kcsOpening        = 2,        // 正在打开
         kcsOpened         = 3,        // 已经打开
         kcsLogouting      = 4,        // 正在登出对端应用
         kcsLogging        = 5,        // 正在登录对端应用
         kcsLogged         = 6,        // 已登录对端应用
         kcsLoading        = 7,        // 正在加载对端应用函数列表
         kcsLoaded         = 8};       // 已加载对端应用函数列表

// 命令的属性项
enum TRCKCmdAttrib
        {kmaData           = 0,        // 自定义数据
         kmaName           = 1,        // 命令的函数名
         kmaState          = 2,        // 命令的状态
         kmaConnObj        = 3,        // 命令的连接对象
         kmaRetType        = 4,        // 命令的返回值类型
         kmaRetSize        = 5,        // 命令的返回值定义尺寸
         kmaParamCount     = 6,        // 命令的参数个数
         kmaFieldCount     = 7,        // 命令的返回字段个数
         kmaExecResult     = 8,        // 命令的执行返回结果(值参见 TRCKCmdResult)
         kmaEncrypted      = 9,        // 命令的参数和返回值在通讯中是否加密
         kmaAppIsValid     = 10,       // 命令已开始的应用是否有效

         // 扩展属性基数
         kmaBaseParamName  = 0x10000,  // 命令的参数索引基数: 参数名
         kmaBaseParamType  = 0x20000,  // 命令的参数索引基数: 参数类型
         kmaBaseParamSize  = 0x30000,  // 命令的参数索引基数: 参数定义尺寸
         kmaBaseFieldName  = 0x60000,  // 命令的返回字段索引基数: 返回字段名
         kmaBaseFieldType  = 0x70000,  // 命令的返回字段索引基数: 返回字段类型
         kmaBaseFieldSize  = 0x80000}; // 命令的返回字段索引基数: 返回字段定义尺寸

// 命令返回结果
enum TRCKCmdResult
        {kmrFailure        = 0,        // 失败
         kmrSuccess        = 1,        // 成功
         kmrException      = 2,        // 异常
         kmrBreak          = 3};       // 中止

// 命令状态
enum TRCKCmdState
        {kmsIdle           = 0,        // 空闲
         kmsEnding         = 1,        // 正在结束
         kmsBeginning      = 2,        // 正在开始
         kmsBegun          = 3,        // 已开始
         kmsExecuting      = 4,        // 正在执行
         kmsExecuted       = 5,        // 执行结束, 已返回
         kmsCanceled       = 6};       // 已取消(如: 超时等等)

// 应答状态
enum TRCKRespState
        {krsIdle           = 0,        // 空闲
         krsExecuting      = 1,        // 正在执行
         krsWaitSend       = 2,        // 应答已执行, 正在等待发送
         krsSending        = 3,        // 应答已执行, 正在发送
         krsSent           = 4,        // 应答已执行, 应答发送完成
         krsConfirmed      = 5,        // 应答已确认接收
         krsCanceled       = 6};       // 应答已取消

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 回调函数类型定义 */

// 固定的回调模式
typedef void  (__stdcall *TRCKDoFixed)(void* AFuncObj, void* AConnObj);

// 对象事件的回调函数类型
typedef void  (__stdcall *TRCKOnObjEvent)(void* AObject);

// 应用连接登录事件的回调函数类型(OnConnLogin)
typedef void  (__stdcall *TRCKOnAppLogin)(void* AppObj, void* AConnObj, bool& AIsRefused);

// 应用连接登出事件的回调函数类型(OnConnLogout)
typedef void  (__stdcall *TRCKOnAppLogout)(void* AppObj, void* AConnObj);

// 应用应答事件的回调函数类型(OnExecResp, OnRespCancel, OnRespConfirm)
typedef void  (__stdcall *TRCKOnAppResp)(void* AppObj, void* AFuncObj, void* AConnObj);

// 连接命令返回事件的回调函数类型(AResult 值参见 TRCKCmdResult)
typedef void  (__stdcall *TRCKOnCmdReturn)(void* AConnObj, void* ACmdObj, long AResult);

// 接收自定义数据的回调函数类型
typedef void  (__stdcall *TRCKOnRecvData)(void* AConnObj, const void* AData, long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 接口相关函数 */

// 接口初始化/释放函数
void     __stdcall RCKInitialize();
void     __stdcall RCKUninitialize();

// 接口版本信息, 格式: "x.x.x.x (build yyyy.mm.dd)"
// AVersion    返回版本号, 如: 0x0708090A 表示 7.8.9.10
char*    __stdcall RCKGetVersion(long* AVersion);

// 取当前操作的最后错误码(注: 用于返回值非错误码的函数, 如 RCKConnCreate 等等)
long     __stdcall RCKGetLastError();

// 根据应用名取应用对象
void*    __stdcall RCKGetAppObj(const char* AName);

// 取接口属性
// (返回值为属性值, ARetCode 为失败的返回码, 允许为空, 当字符串类型成功时为值尺寸)
long     __stdcall RCKGetInt(long Attrib, long* ARetCode = NULL);
void*    __stdcall RCKGetObj(long Attrib, long* ARetCode = NULL);
char*    __stdcall RCKGetStr(long Attrib, long* ARetCode = NULL);

// 设置接口属性
long     __stdcall RCKSetInt(long Attrib, long  AValue);
long     __stdcall RCKSetObj(long Attrib, void* AValue);
long     __stdcall RCKSetStr(long Attrib, const char* AValue, long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* Application 的相关函数 */

// 创建/释放应用
void*    __stdcall RCKAppCreate(const char* AName, const char* APassword);
void     __stdcall RCKAppFree(void* AppObj);

// 开始/结束应用的函数定义
long     __stdcall RCKAppFuncBegin(void* AppObj);
long     __stdcall RCKAppFuncEnd(void* AppObj);

// 根据函数名取应用的函数对象
void*    __stdcall RCKAppFuncObj(void* AppObj, const char* AName);

// 打开/关闭应用/强制注销应用的连接
long     __stdcall RCKAppOpen(void* AppObj);
long     __stdcall RCKAppClose(void* AppObj);
long     __stdcall RCKAppLogout(void* AppObj, void* AConnObj);

// 取应用属性
// (返回值为属性值, ARetCode 为失败的返回码, 允许为空, 当字符串类型成功时为值尺寸)
long     __stdcall RCKAppGetInt(void* AppObj, long Attrib, long* ARetCode = NULL);
void*    __stdcall RCKAppGetObj(void* AppObj, long Attrib, long* ARetCode = NULL);
char*    __stdcall RCKAppGetStr(void* AppObj, long Attrib, long* ARetCode = NULL);

// 设置应用属性
long     __stdcall RCKAppSetInt(void* AppObj, long Attrib, long  AValue);
long     __stdcall RCKAppSetObj(void* AppObj, long Attrib, void* AValue);
long     __stdcall RCKAppSetStr(void* AppObj, long Attrib,
                          const char* AValue, long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* Function 的相关函数 */

// 创建/释放 Function
void*    __stdcall RCKFuncCreate(void* AppObj, long ACallMode, void* ACallback,
                           const char* AName,  long ARetType,  long  ARetSize);
void     __stdcall RCKFuncFree(void* AFuncObj);

// 添加 Function 参数/返回字段
long     __stdcall RCKFuncAddParam(void* AFuncObj,    const char* AName,
                                   long  AType, long ASize, bool  AIsRef);
long     __stdcall RCKFuncAddField(void* AFuncObj,    const char* AName,
                                   long  AType,             long  ASize);

// 取 Function 参数名索引/返回字段名索引
long     __stdcall RCKFuncParamIndex(void* AFuncObj, const char* AName);
long     __stdcall RCKFuncFieldIndex(void* AFuncObj, const char* AName);

// 取 Function 属性
// (返回值为属性值, ARetCode 为失败的返回码, 允许为空, 当字符串类型成功时为值尺寸)
long     __stdcall RCKFuncGetInt(void* AFuncObj, long Attrib, long* ARetCode = NULL);
void*    __stdcall RCKFuncGetObj(void* AFuncObj, long Attrib, long* ARetCode = NULL);
char*    __stdcall RCKFuncGetStr(void* AFuncObj, long Attrib, long* ARetCode = NULL);

// 设置 Function 属性
long     __stdcall RCKFuncSetInt(void* AFuncObj, long Attrib, long  AValue);
long     __stdcall RCKFuncSetObj(void* AFuncObj, long Attrib, void* AValue);
long     __stdcall RCKFuncSetStr(void* AFuncObj, long Attrib,
                           const char* AValue,   long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* Connection 的相关函数 */

// 创建/释放连接(注: ASlotObj 必须非空且唯一, ASlotMethods 必须非空)
void*    __stdcall RCKConnCreate(long AType, void* ASlotObj,
                            const TRCKSlotMethods* ASlotMethods);
void     __stdcall RCKConnFree(void* AConnObj);

// 打开/关闭连接
long     __stdcall RCKConnOpen(void* AConnObj);
long     __stdcall RCKConnClose(void* AConnObj);

// 登录/登出对端应用
long     __stdcall RCKConnLogin(void* AConnObj);
long     __stdcall RCKConnLogout(void* AConnObj);

// 加载对端应用的函数列表
long     __stdcall RCKConnLoad(void* AConnObj);

// 发送数据(参数 ANeedPack 表示数据是否需要压缩发送)
long     __stdcall RCKConnSendData(void* AConnObj, const void* AData,
                                   long  ASize,          bool  ANeedPack);

// 取连接属性
// (返回值为属性值, ARetCode 为失败的返回码, 允许为空, 当字符串类型成功时为值尺寸)
long     __stdcall RCKConnGetInt(void* AConnObj, long Attrib, long* ARetCode = NULL);
void*    __stdcall RCKConnGetObj(void* AConnObj, long Attrib, long* ARetCode = NULL);
char*    __stdcall RCKConnGetStr(void* AConnObj, long Attrib, long* ARetCode = NULL);

// 设置连接属性
long     __stdcall RCKConnSetInt(void* AConnObj, long Attrib, long  AValue);
long     __stdcall RCKConnSetObj(void* AConnObj, long Attrib, void* AValue);
long     __stdcall RCKConnSetStr(void* AConnObj, long Attrib,
                           const char* AValue,   long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* Command 的相关函数 */

// 创建/释放命令
void*    __stdcall RCKCmdCreate();
void     __stdcall RCKCmdFree(void* ACmdObj);

// 开始/结束命令
long     __stdcall RCKCmdBegin(void* ACmdObj, const char* AName);
long     __stdcall RCKCmdEnd(void* ACmdObj);

// 重新开始/执行命令
long     __stdcall RCKCmdRestart(void* ACmdObj);
long     __stdcall RCKCmdExecute(void* ACmdObj, unsigned long ATimeout);

// 取命令属性
// (返回值为属性值, ARetCode 为失败的返回码, 允许为空, 当字符串类型成功时为值尺寸)
long     __stdcall RCKCmdGetInt(void* ACmdObj, long Attrib, long* ARetCode = NULL);
void*    __stdcall RCKCmdGetObj(void* ACmdObj, long Attrib, long* ARetCode = NULL);
char*    __stdcall RCKCmdGetStr(void* ACmdObj, long Attrib, long* ARetCode = NULL);

// 设置命令属性
long     __stdcall RCKCmdSetInt(void* ACmdObj, long Attrib, long  AValue);
long     __stdcall RCKCmdSetObj(void* ACmdObj, long Attrib, void* AValue);
long     __stdcall RCKCmdSetStr(void* ACmdObj, long Attrib,
                          const char* AValue,  long ASize);

// 取当前命令的各类型返回值
// (ARetCode 为失败的返回码, 允许为空, 当字符串或结构类型成功时为值尺寸)
char*    __stdcall RCKCmdRetStr(void* ACmdObj, long* ARetCode = NULL);
long     __stdcall RCKCmdRetInt(void* ACmdObj, long* ARetCode = NULL);
bool     __stdcall RCKCmdRetBool(void* ACmdObj, long* ARetCode = NULL);
double   __stdcall RCKCmdRetFloat(void* ACmdObj, long* ARetCode = NULL);
void*    __stdcall RCKCmdRetStruct(void* ACmdObj, long* ARetCode = NULL);
__int64  __stdcall RCKCmdRetInt64(void* ACmdObj, long* ARetCode = NULL);
double   __stdcall RCKCmdRetDate(void* ACmdObj, long* ARetCode = NULL);
long     __stdcall RCKCmdRetCustom(void* ACmdObj, void* AValue, long ASize);

// 清除当前命令参数值/取当前命令的参数名索引
long     __stdcall RCKCmdParamClear(void* ACmdObj);
long     __stdcall RCKCmdParamIndex(void* ACmdObj, const char* AName);

// 根据参数索引设置当前命令的各类型参数值
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

// 根据参数名设置当前命令的各类型参数值
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

// 取当前命令的返回字段名索引/字段值是否为空(ARetCode 为失败的返回码, 允许为空)
long     __stdcall RCKCmdFieldIndex(void* ACmdObj, const char* AName);
bool     __stdcall RCKCmdFieldIsNull(void* ACmdObj, long AIndex, long* ARetCode = NULL);
bool     __stdcall RCKCmdFieldIsNull_(void* ACmdObj, const char* AName, long* ARetCode = NULL);

// 根据字段索引取当前命令的各类型字段值
// (ARetCode 为失败的返回码, 允许为空, 当字符串或结构类型成功时为值尺寸)
char*    __stdcall RCKCmdFieldStr(void* ACmdObj, long AIndex, long* ARetCode = NULL);
long     __stdcall RCKCmdFieldInt(void* ACmdObj, long AIndex, long* ARetCode = NULL);
bool     __stdcall RCKCmdFieldBool(void* ACmdObj, long AIndex, long* ARetCode = NULL);
double   __stdcall RCKCmdFieldFloat(void* ACmdObj, long AIndex, long* ARetCode = NULL);
void*    __stdcall RCKCmdFieldStruct(void* ACmdObj, long AIndex, long* ARetCode = NULL);
__int64  __stdcall RCKCmdFieldInt64(void* ACmdObj, long AIndex, long* ARetCode = NULL);
double   __stdcall RCKCmdFieldDate(void* ACmdObj, long AIndex, long* ARetCode = NULL);
long     __stdcall RCKCmdFieldCustom(void* ACmdObj, long AIndex,
                                     void* AValue,  long ASize);

// 根据字段名取当前命令的各类型字段值
// (ARetCode 为失败的返回码, 允许为空, 当字符串或结构类型成功时为值尺寸)
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

/* Response 的相关函数 */

// 取应答的连接对象/函数对象/状态
void*    __stdcall RCKRespConnObj();
void*    __stdcall RCKRespFuncObj();
long     __stdcall RCKRespState();

// 取应答的各类型返回值
// (ARetCode 为失败的返回码, 允许为空, 当字符串或结构类型成功时为值尺寸)
char*    __stdcall RCKRespRetStr(long* ARetCode = NULL);
long     __stdcall RCKRespRetInt(long* ARetCode = NULL);
bool     __stdcall RCKRespRetBool(long* ARetCode = NULL);
double   __stdcall RCKRespRetFloat(long* ARetCode = NULL);
void*    __stdcall RCKRespRetStruct(long* ARetCode = NULL);
__int64  __stdcall RCKRespRetInt64(long* ARetCode = NULL);
double   __stdcall RCKRespRetDate(long* ARetCode = NULL);
long     __stdcall RCKRespRetCustom(void* AValue, long ASize);

// 根据索引取应答的各类型参数值
// (ARetCode 为失败的返回码, 允许为空, 当字符串或结构类型成功时为值尺寸)
char*    __stdcall RCKRespParamStr(long AIndex, long* ARetCode = NULL);
long     __stdcall RCKRespParamInt(long AIndex, long* ARetCode = NULL);
bool     __stdcall RCKRespParamBool(long AIndex, long* ARetCode = NULL);
double   __stdcall RCKRespParamFloat(long AIndex, long* ARetCode = NULL);
void*    __stdcall RCKRespParamStruct(long AIndex, long* ARetCode = NULL);
__int64  __stdcall RCKRespParamInt64(long AIndex, long* ARetCode = NULL);
double   __stdcall RCKRespParamDate(long AIndex, long* ARetCode = NULL);
long     __stdcall RCKRespParamCustom(long AIndex, void* AValue, long ASize);

// 根据参数名取应答的各类型参数值
// (ARetCode 为失败的返回码, 允许为空, 当字符串或结构类型成功时为值尺寸)
char*    __stdcall RCKRespParamStr_(const char* AName, long* ARetCode = NULL);
long     __stdcall RCKRespParamInt_(const char* AName, long* ARetCode = NULL);
bool     __stdcall RCKRespParamBool_(const char* AName, long* ARetCode = NULL);
double   __stdcall RCKRespParamFloat_(const char* AName, long* ARetCode = NULL);
void*    __stdcall RCKRespParamStruct_(const char* AName, long* ARetCode = NULL);
__int64  __stdcall RCKRespParamInt64_(const char* AName, long* ARetCode = NULL);
double   __stdcall RCKRespParamDate_(const char* AName, long* ARetCode = NULL);
long     __stdcall RCKRespParamCustom_(const char* AName, void* AValue, long ASize);

// 根据索引取应答的各类型字段返回值
// (ARetCode 为失败的返回码, 允许为空, 当字符串或结构类型成功时为值尺寸)
char*    __stdcall RCKRespFieldStr(long AIndex, long* ARetCode = NULL);
long     __stdcall RCKRespFieldInt(long AIndex, long* ARetCode = NULL);
bool     __stdcall RCKRespFieldBool(long AIndex, long* ARetCode = NULL);
double   __stdcall RCKRespFieldFloat(long AIndex, long* ARetCode = NULL);
void*    __stdcall RCKRespFieldStruct(long AIndex, long* ARetCode = NULL);
__int64  __stdcall RCKRespFieldInt64(long AIndex, long* ARetCode = NULL);
double   __stdcall RCKRespFieldDate(long AIndex, long* ARetCode = NULL);
long     __stdcall RCKRespFieldCustom(long AIndex, void* AValue, long ASize);

// 根据字段名取应答的各类型字段返回值
// (ARetCode 为失败的返回码, 允许为空, 当字符串或结构类型成功时为值尺寸)
char*    __stdcall RCKRespFieldStr_(const char* AName, long* ARetCode = NULL);
long     __stdcall RCKRespFieldInt_(const char* AName, long* ARetCode = NULL);
bool     __stdcall RCKRespFieldBool_(const char* AName, long* ARetCode = NULL);
double   __stdcall RCKRespFieldFloat_(const char* AName, long* ARetCode = NULL);
void*    __stdcall RCKRespFieldStruct_(const char* AName, long* ARetCode = NULL);
__int64  __stdcall RCKRespFieldInt64_(const char* AName, long* ARetCode = NULL);
double   __stdcall RCKRespFieldDate_(const char* AName, long* ARetCode = NULL);
long     __stdcall RCKRespFieldCustom_(const char* AName, void* AValue, long ASize);

// 设置应答的各类型返回值
long     __stdcall RCKRespRetByStr(const char* AValue, long ASize);
long     __stdcall RCKRespRetByInt(long AValue);
long     __stdcall RCKRespRetByBool(bool AValue);
long     __stdcall RCKRespRetByFloat(double AValue);
long     __stdcall RCKRespRetByStruct(void* AValue);
long     __stdcall RCKRespRetByInt64(__int64 AValue);
long     __stdcall RCKRespRetByDate(double AValue);
long     __stdcall RCKRespRetByCustom(const void* AValue, long ASize);

// 根据索引设置应答的各类型字段返回值
long     __stdcall RCKRespFieldByStr(long AIndex, const char* AValue, long ASize);
long     __stdcall RCKRespFieldByInt(long AIndex, long AValue);
long     __stdcall RCKRespFieldByBool(long AIndex, bool AValue);
long     __stdcall RCKRespFieldByFloat(long AIndex, double AValue);
long     __stdcall RCKRespFieldByStruct(long AIndex, void* AValue);
long     __stdcall RCKRespFieldByInt64(long AIndex, __int64 AValue);
long     __stdcall RCKRespFieldByDate(long AIndex, double AValue);
long     __stdcall RCKRespFieldByCustom(long AIndex, const void* AValue, long ASize);

// 根据字段名设置应答的各类型字段返回值
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

/* Fields 的相关函数 */

// 创建/释放字段集
void*    __stdcall RCKFieldsCreate();
void     __stdcall RCKFieldsFree(void* AFields);

// 添加字段定义/删除指定索引的字段及定义
long     __stdcall RCKFieldDefsAdd(void* AFields, const char* AName,
                                   long  AType,         long  ASize);
void     __stdcall RCKFieldDefsDelete(void* AFields, long AIndex);

// 清除/拷贝/输出字段定义集/取字段定义集的数据尺寸
void     __stdcall RCKFieldDefsClear(void* AFields);
long     __stdcall RCKFieldDefsAssign(void* AFields, const void* ADefs, long ASize);
void*    __stdcall RCKFieldDefsOutput(void* AFields, long* ARetSize);
long     __stdcall RCKFieldDefsSize(const void* AFieldDefs);

// 清除/拷贝/输出字段集的数据/取字段集的数据尺寸
void     __stdcall RCKFieldsClear(void* AFields);
long     __stdcall RCKFieldsAssign(void* AFields, const void* AData, long ASize);
void*    __stdcall RCKFieldsOutput(void* AFields, long* ARetSize);
long     __stdcall RCKFieldsSize(void* AFields);

// 取字段个数/取字段名所在索引
long     __stdcall RCKFieldCount(void* AFields);
long     __stdcall RCKFieldIndex(void* AFields, const char* AName);

// 取索引字段名/类型/定义尺寸/值尺寸
char*    __stdcall RCKFieldName(void* AFields, long AIndex);
long     __stdcall RCKFieldType(void* AFields, long AIndex);
long     __stdcall RCKFieldDefSize(void* AFields, long AIndex);
long     __stdcall RCKFieldSize(void* AFields, long AIndex);

// 判断索引字段是否为空(ARetCode 为失败的返回码, 允许为空)
bool     __stdcall RCKFieldIsNull(void* AFields, long AIndex, long* ARetCode = NULL);
bool     __stdcall RCKFieldIsNull_(void* AFields, const char* AName, long* ARetCode = NULL);

// 取索引字段的各类型值
// (ARetCode 为失败的返回码, 允许为空, 当字符串或结构类型成功时为值尺寸)
char*    __stdcall RCKFieldAsStr(void* AFields, long AIndex, long* ARetCode = NULL);
long     __stdcall RCKFieldAsInt(void* AFields, long AIndex, long* ARetCode = NULL);
bool     __stdcall RCKFieldAsBool(void* AFields, long AIndex, long* ARetCode = NULL);
double   __stdcall RCKFieldAsFloat(void* AFields, long AIndex, long* ARetCode = NULL);
void*    __stdcall RCKFieldAsStruct(void* AFields, long AIndex, long* ARetCode = NULL);
__int64  __stdcall RCKFieldAsInt64(void* AFields, long AIndex, long* ARetCode = NULL);
double   __stdcall RCKFieldAsDate(void* AFields, long AIndex, long* ARetCode = NULL);
long     __stdcall RCKFieldGetCustom(void* AFields, long AIndex,
                                     void* AValue,  long ASize);

// 取字段名字段的各类型值
// (ARetCode 为失败的返回码, 允许为空, 当字符串或结构类型成功时为值尺寸)
char*    __stdcall RCKFieldAsStr_(void* AFields, const char* AName, long* ARetCode = NULL);
long     __stdcall RCKFieldAsInt_(void* AFields, const char* AName, long* ARetCode = NULL);
bool     __stdcall RCKFieldAsBool_(void* AFields, const char* AName, long* ARetCode = NULL);
double   __stdcall RCKFieldAsFloat_(void* AFields, const char* AName, long* ARetCode = NULL);
void*    __stdcall RCKFieldAsStruct_(void* AFields, const char* AName, long* ARetCode = NULL);
__int64  __stdcall RCKFieldAsInt64_(void* AFields, const char* AName, long* ARetCode = NULL);
double   __stdcall RCKFieldAsDate_(void* AFields, const char* AName, long* ARetCode = NULL);
long     __stdcall RCKFieldGetCustom_(void* AFields, const char* AName,
                                      void* AValue,        long  ASize);

// 设置索引字段的类型值
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

// 设置字段名字段的类型值
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
