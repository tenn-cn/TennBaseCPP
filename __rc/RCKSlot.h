// =======================================
// Unit   : RCK Slot (RCK 通讯槽接口)
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

/* 常量定义 */

// 返回值及错误码
enum TRCKResult
        {krSuccess         =  1,       // 成功
         krFailure         =  0,       // 失败
         krUnknown         = -1,       // 未知错误
         krNotExist        = -2,       // 不存在(如: 连接对象)
         krNotConnect      = -3,       // 连接未打开
         krNonsupport      = -4,       // 不支持
         krVersion         = -5,       // 版本不兼容
         krTimeout         = -6,       // 操作超时
         krPassword        = -7,       // 密码错误
         krIsExisted       = -8,       // 已存在
         krIsRefused       = -9,       // 被拒绝
         krIsIllegal       = -10,      // 不合法
         krIsNullName      = -11,      // 名字为空
         krAttrInvalid     = -12,      // 属性无效
         krStateInvalid    = -13,      // 状态无效
         krHandleInvalid   = -14,      // 句柄无效
         krAccessIllegal   = -15};     // 存取非法

// 通讯槽的属性项
enum TRCKSlotAttrib
        {ksaError          = 0,        // 通讯槽打开失败的错误信息
         ksaState          = 1,        // 通讯槽的状态
         ksaConnObj        = 2,        // 通讯槽的绑定连接对象
         ksaCurrAddr       = 3,        // 通讯槽的本地地址
         ksaCurrPort       = 4,        // 通讯槽的本地端口号
         ksaPeerAddr       = 5,        // 通讯槽的对端地址
         ksaPeerPort       = 6,        // 通讯槽的对端端口号
         ksaSlotMaxSize    = 7,        // 通讯槽的数据包最大长度, 默认值: 8192

         // 回调事件属性, 取值范围: [32..63]
         ksaOnDisconnect   = 32,       // 通讯槽的 OnDisconnect 事件, 断开通讯(TRCKSlotOnEvent)
         ksaOnRecvEvent    = 33,       // 通讯槽的 OnRecvEvent  事件, 接收事件(TRCKSlotOnEvent)
         ksaOnRecvData     = 34,       // 通讯槽的 OnRecvData   事件, 接收数据(TRCKSlotOnRecvData)

         // 扩展属性基数
         ksaBaseExtended   = 0x10000}; // 通讯槽的扩展属性基数: 扩展属性项
                                       // (注: 扩展属性项必须小于 0x10000)

// 通讯槽的状态
enum TRCKSlotState
        {kssInactive       = 0,        // 未打开
         kssClosing        = 1,        // 正在关闭
         kssOpening        = 2,        // 正在打开
         kssOpened         = 3};       // 已经打开

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 通讯槽对象的函数类型定义 */

// 操作通讯槽的函数类型
typedef void  (__stdcall *TRCKSlotOp)(void* ASlotObj);

// 打开通讯槽的函数类型(返回值定义为 TRCKResult)
typedef long  (__stdcall *TRCKSlotOpen)(void* ASlotObj, long ATimeout);

// 接收数据的函数类型(返回值为接收成功的数据尺寸, 若小于 0 则为错误码)
typedef long  (__stdcall *TRCKSlotRecv)(void* ASlotObj, void* AData, long ASize);

// 发送数据的函数类型(返回值为发送成功的数据尺寸, 若小于 0 则为错误码)
typedef long  (__stdcall *TRCKSlotSend)(void* ASlotObj, const void* AData, long ASize);

// 可以发送的函数类型(返回值为可以发送的数据尺寸, 若小于 0 则为错误码)
typedef long  (__stdcall *TRCKSlotCanSend)(void* ASlotObj, long ASize);

// 取通讯槽整型属性的函数类型(返回值为属性值, ARetCode 为失败的返回码, 允许为空)
typedef long  (__stdcall *TRCKSlotGetInt)(void* ASlotObj, long Attrib, long* ARetCode);

// 取通讯槽指针属性的函数类型(返回值为属性值, ARetCode 为失败的返回码, 允许为空)
typedef void* (__stdcall *TRCKSlotGetObj)(void* ASlotObj, long Attrib, long* ARetCode);

// 取通讯槽字符串属性的函数类型(返回值为属性值, ARetCode 成功时为属性值尺寸, 否则为返回码, 允许为空)
typedef char* (__stdcall *TRCKSlotGetStr)(void* ASlotObj, long Attrib, long* ARetCode);

// 设置通讯槽整型属性的函数类型
typedef long  (__stdcall *TRCKSlotSetInt)(void* ASlotObj, long Attrib, long  AValue);

// 设置通讯槽指针属性的函数类型
typedef long  (__stdcall *TRCKSlotSetObj)(void* ASlotObj, long Attrib, void* AValue);

// 设置通讯槽字符串属性的函数类型
typedef long  (__stdcall *TRCKSlotSetStr)(void* ASlotObj, long Attrib,
                                    const char* AValue,   long ASize);

// 通讯槽对象事件的回调函数类型
typedef void  (__stdcall *TRCKSlotOnEvent)(void* ASlotObj, void* AConnObj);

// 通讯槽对象 OnRecvData 事件的回调函数类型
// 注: 若 Recv 方法为 NULL 则通过 OnRecvData 事件接收数据,
//     否则通过 OnRecvEvent  事件来调用 Recv 方法接收数据.
typedef void  (__stdcall *TRCKSlotOnRecvData)(void* ASlotObj, void* AConnObj,
                                        const void* AData,    long  ASize);

// 通讯槽对象的方法列表
#pragma pack(push, 1)
typedef struct
{
   long              Size;             // = sizeof(TRCKSlotMethods)

   TRCKSlotOpen      Open;             // 打开通讯槽
   TRCKSlotOp        Close;            // 关闭通讯槽
   TRCKSlotOp        FreeObj;          // 释放通讯槽对象

   TRCKSlotRecv      Recv;             // 接收数据
   TRCKSlotSend      Send;             // 发送数据
   TRCKSlotCanSend   CanSend;          // 可以发送的数据尺寸

   TRCKSlotGetInt    GetInt;           // 取通讯槽整型属性
   TRCKSlotGetObj    GetObj;           // 取通讯槽指针属性
   TRCKSlotGetStr    GetStr;           // 取通讯槽字符串属性

   TRCKSlotSetInt    SetInt;           // 设置通讯槽整型属性
   TRCKSlotSetObj    SetObj;           // 设置通讯槽指针属性
   TRCKSlotSetStr    SetStr;           // 设置通讯槽字符串属性
} TRCKSlotMethods, *PRCKSlotMethods;
#pragma pack(pop)

}

#endif
