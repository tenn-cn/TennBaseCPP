// =======================================
// Unit   : RCKernel objects
// Version: 5.0.0.0 (build 2014.07.07)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

//#include <stdio.h>
//#include <stdarg.h>

#include "RCKObjs.h"

// 使用 RCKernel 的命名空间
namespace RCKernel
{
   namespace __object__
   {

/* begin namespace */

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TRCKObjs - RCK 对象集类 */

class TRCKObjs
{
private:
   // 映射列表类型
   typedef TKYMapObjKey<TRCAppObj>     TAppList;
   typedef TKYMapObjKey<TRCConnObj>    TConnList;
   typedef TKYMapObjKey<TRCFuncObj>    TFuncList;

public:
   TRCKObjs();
   virtual ~TRCKObjs();

   // 清除
   void           Clear();

   // 添加/删除函数ID
   bool           AddFuncID(void* AFuncID, TRCFuncObj* AFuncObj)
                  { return (FFuncIDs->Add(AFuncID, AFuncObj) != NULL); }
   bool           DeleteFuncID(void* AFuncID)
                  { return FFuncIDs->Delete(AFuncID); }

   // 函数ID -> 对象
   TRCFuncObj*    FuncID2Object(void* AFuncID)
                  {
                     TRCFuncObj* result = NULL;
                     FFuncIDs->Find(AFuncID, result);
                     return result;
                  }

   // 创建/释放应用对象
   TRCAppObj*     CreateAppObj(const char* AName, const char* APassword, long& ARetCode);
   void           FreeAppObj(TRCAppObj* AppObj);

   // 应用对象引用计数增 1
   TRCAppObj*     IncRefAppObj(void* AppID);
   bool           IncRefAppObj(TRCAppObj* AppObj);

   // 创建/释放连接对象(注: AConnID 必须存在)
   TRCConnObj*    CreateConnObj(void* AConnID, bool ANeedFree, long& ARetCode);
   void           FreeConnObj(TRCConnObj* AConnObj);

   // 连接对象引用计数增 1
   TRCConnObj*    IncRefConnObj(void* AConnID);
   bool           IncRefConnObj(TRCConnObj* AConnObj);

   // 创建/释放命令对象
   TRCCmdObj*     CreateCmdObj(void* AConnID = NULL);
   void           FreeCmdObj(TRCCmdObj* ACmdObj);

protected:
   // 当前锁
   void           Lock() const         { FLock->Enter(); }
   void           Unlock() const       { FLock->Leave(); }

private:
   // 拷贝并清除
   bool           DoClearAppObjs(TKYList& AList);
   bool           DoClearConnObjs(TKYList& AList);
   bool           DoClearCmdQueue(TKYList& AList);

   // 事件方法
   void           DoAppDeletion(TRCAppObj* AppObj);
   void           DoCmdDeletion(TRCCmdObj* ACmdObj);
   void           DoConnDeletion(TRCConnObj* AConnObj);

private:
   TKYCritSect*   FLock;               // 当前锁
   TAppList*      FAppIDs;             // 应用 ID 列表
   TAppList*      FAppObjs;            // 应用对象列表
   TFuncList*     FFuncIDs;            // 函数 ID 列表
   TConnList*     FConnIDs;            // 连接 ID 列表
   TConnList*     FConnObjs;           // 连接对象列表
   TKYCycQueue*   FCmdQueue;           // 命令对象队列

public:
   // 全局 RCK 对象集对象
   static TRCKObjs*     _obj()         { return _Object; }

   // 初始化/释放 _Object
   static void          _InitObj();
   static void          _FreeObj(bool AForced = false);

   // 应用对象引用计数增/减 1(注: AppObj 对象必须存在)
   static void          _IncRefAppObj(TRCAppObj* AppObj)
                        { AppObj->IncObjTimes(); }
   static void          _DecRefAppObj(TRCAppObj* AppObj)
                        { if (AppObj->DecObjTimes() == 0) delete AppObj; }

   // 连接对象引用计数增/减 1(注: AConnObj 对象必须存在)
   static void          _IncRefConnObj(TRCConnObj* AConnObj)
                        { AConnObj->IncObjTimes(); }
   static void          _DecRefConnObj(TRCConnObj* AConnObj)
                        { if (AConnObj->DecObjTimes() == 0) delete AConnObj; }

private:
   // 全局 RCK 对象集对象
   static TKYCritSect*  _Lock;
   static TRCKObjs*     _Object;
   static long          _RefCount;

   // TRCKObjs 的静态成员初始化类
   static class TInitialization
   {
   public:
      TInitialization();
      ~TInitialization();
   } _Initialization;

   friend class TInitialization;
};

/* TRCKObjs - RCK 对象集类 */

// ---------------- 静态成员 ----------------

// 全局锁对象
TKYCritSect*   TRCKObjs::_Lock      = NULL;
TRCKObjs*      TRCKObjs::_Object    = NULL;
long           TRCKObjs::_RefCount  = 0;

// TRCKObjs 的静态成员初始化对象
TRCKObjs::TInitialization TRCKObjs::_Initialization;

// ---------------- 静态函数 ----------------
// TRCKObjs 的静态成员初始化类的构造函数
TRCKObjs::TInitialization::TInitialization()
{
   // 初始化
   _RefCount   = 0;
   _Object     = NULL;

   // 创建对象
   _Lock       = new TKYCritSect;
}

// TRCKObjs 的静态成员初始化类的析构函数
TRCKObjs::TInitialization::~TInitialization()
{
   // 释放对象
   FreeAndNil(_Lock);
   FreeAndNil(_Object);
}

// 初始化 _Object
void TRCKObjs::_InitObj()
{
   _Lock->Enter();
   if (_RefCount == 0)
      try
      {
         // 创建对象并引用计数设为 1
         _Object     = new TRCKObjs;
         _RefCount   = 1;

         // 初始化 RCKernel 接口
         RCKInitialize();
      }
      catch (...) {}
   else
      _RefCount++;
   _Lock->Leave();
}

// 释放 _Object
void TRCKObjs::_FreeObj(bool AForced)
{
   _Lock->Enter();
   if (_RefCount > 0)
      try
      {
         // 引用计数减 1
         _RefCount--;

         // 判断是否需要释放
         if ((_RefCount == 0) || AForced)
         {
            // 引用计数设为 0
            _RefCount   = 0;

            // 清除并释放对象
            _Object->Clear();
            FreeAndNil(_Object);

            // 释放 RCKernel 接口
            RCKUninitialize();
         }
      }
      catch (...) {}
   _Lock->Leave();
}

// ---------------- 构造函数和析构函数 ----------------
// 构造函数
TRCKObjs::TRCKObjs()
{
   // 创建对象
   FLock       = new TKYCritSect;
   FAppIDs     = new TAppList(false, false);
   FAppObjs    = new TAppList(false, false);
   FFuncIDs    = new TFuncList(false, true);
   FConnIDs    = new TConnList(false, false);
   FConnObjs   = new TConnList(false, false);
   FCmdQueue   = new TKYCycQueue(0, TKYCycQueue::slPower10);

   // 设置对象属性
   FCmdQueue->SetMaxCount(FCmdQueue->CycCount());
}

// 析构函数
TRCKObjs::~TRCKObjs()
{
   // 清除
   Clear();

   // 释放对象
   FreeAndNil(FLock);
   FreeAndNil(FAppIDs);
   FreeAndNil(FAppObjs);
   FreeAndNil(FFuncIDs);
   FreeAndNil(FConnIDs);
   FreeAndNil(FConnObjs);
   FreeAndNil(FCmdQueue);
}

// ---------------- 私有函数 ----------------
// 拷贝并清除应用对象列表
bool TRCKObjs::DoClearAppObjs(TKYList& AList)
{
   // 初始化
   bool result = false;

   // 检查列表是否非空
   if (FAppObjs->Count() > 0)
      try
      {
         // 更改容量
         AList.ChangeCapacity(FAppObjs->Count());

         // 循环拷贝
         void* pNode = FAppObjs->Next(NULL);
         while (pNode != NULL)
         {
            AList.Add(FAppObjs->Value(pNode));
            pNode = FAppObjs->Next(pNode);
         }

         // 清除
         FAppIDs->Clear();
         FAppObjs->Clear();

         // 返回结果
         result = true;
      }
      catch (...) {}

   // 返回结果
   return result;
}

// 拷贝并清除连接对象列表
bool TRCKObjs::DoClearConnObjs(TKYList& AList)
{
   // 初始化
   bool result = false;

   // 检查列表是否非空
   if (FConnObjs->Count() > 0)
      try
      {
         // 更改容量
         AList.ChangeCapacity(FConnObjs->Count());

         // 循环拷贝
         void* pNode = FConnObjs->Next(NULL);
         while (pNode != NULL)
         {
            AList.Add(FConnObjs->Value(pNode));
            pNode = FConnObjs->Next(pNode);
         }

         // 清除
         FConnIDs->Clear();
         FConnObjs->Clear();

         // 返回结果
         result = true;
      }
      catch (...) {}

   // 返回结果
   return result;
}

// 拷贝并清除命令对象队列
bool TRCKObjs::DoClearCmdQueue(TKYList& AList)
{
   // 初始化
   bool result = false;

   // 检查列表是否非空
   if (FCmdQueue->Count() != 0)
      try
      {
         // 初始化
         void* pData;

         // 更改容量并循环弹出队列
         AList.ChangeCapacity(FCmdQueue->Count());
         while (FCmdQueue->Pop(pData))
            AList.Add(pData);

         // 返回结果
         result = true;
      }
      catch (...) {}

   // 返回结果
   return result;
}

// 列表的 OnDeletion 事件过程
void TRCKObjs::DoAppDeletion(TRCAppObj* AppObj)
{
   _DecRefAppObj(AppObj);
}

// 列表的 OnDeletion 事件过程
void TRCKObjs::DoCmdDeletion(TRCCmdObj* ACmdObj)
{
   delete ACmdObj;
}

// 列表的 OnDeletion 事件过程
void TRCKObjs::DoConnDeletion(TRCConnObj* AConnObj)
{
   _DecRefConnObj(AConnObj);
}

// ---------------- 公有函数 ----------------
// 清除
void TRCKObjs::Clear()
{
   // 初始化
   TKYList  objApps(false);
   TKYList  objCmds(false);
   TKYList  objConns(false);
   bool     boolApp  = false;
   bool     boolCmd  = false;
   bool     boolConn = false;

   // 清除函数列表
   FFuncIDs->Clear();

   // 拷贝并清除列表
   Lock();
   {
      boolApp  = DoClearAppObjs(objApps);
      boolCmd  = DoClearCmdQueue(objCmds);
      boolConn = DoClearConnObjs(objConns);
   }
   Unlock();

   // 判断是否需要清除应用
   if (boolApp)
   {
      // 设置 OnDeletion 事件方法
      objApps.OnDeletion.Object  = this;
      objApps.OnDeletion.Method  = (TKYList::TDoDeletion)&TRCKObjs::DoAppDeletion;

      // 清除
      objApps.Clear();
   }

   // 判断是否需要清除命令
   if (boolCmd)
   {
      // 设置 OnDeletion 事件方法
      objCmds.OnDeletion.Object  = this;
      objCmds.OnDeletion.Method  = (TKYList::TDoDeletion)&TRCKObjs::DoCmdDeletion;

      // 清除
      objCmds.Clear();
   }

   // 判断是否需要清除连接
   if (boolConn)
   {
      // 设置 OnDeletion 事件方法
      objConns.OnDeletion.Object = this;
      objConns.OnDeletion.Method = (TKYList::TDoDeletion)&TRCKObjs::DoConnDeletion;

      // 清除
      objConns.Clear();
   }
}

// 创建应用对象
TRCAppObj* TRCKObjs::CreateAppObj(const char* AName, const char* APassword, long& ARetCode)
{
   // 初始化
   TRCAppObj*  result   = NULL;
   KYString    strName  = AName;
   void*       pAppID   = RCKAppCreate((char*)strName, APassword);

   // 判断是否创建成功
   if (pAppID != NULL)
   {
      ARetCode = krUnknown;

      // 创建对象, 并加入列表
      try
      {
         TRCAppObj* objApp = new TRCAppObj;
         if (objApp != NULL)
         {
            Lock();
            try
            {
               // 添加到 ID 列表
               void* pNode = FAppIDs->Add(pAppID, objApp);
               if (pNode == NULL)
               {
                  pNode = FAppIDs->Find(pAppID);
                  if (pNode != NULL)
                  {
                     FAppObjs->Delete(FAppIDs->Value(pNode));
                     FAppIDs->SetValue(pNode, objApp);
                  }
               }

               // 添加到对象列表
               if (pNode == NULL)
                  ;
               else if ((FAppObjs->Add(objApp, objApp) != NULL)
                     || FAppObjs->Existed(objApp))
               {
                  // 设置对象属性
                  objApp->FName  = strName;
                  objApp->FAppID = pAppID;
                  result         = objApp;
                  ARetCode       = krSuccess;
               }
               else
                  FAppIDs->Remove(pNode);
            }
            catch (...) {}
            Unlock();

            // 若失败则释放对象
            if (result == NULL)
               delete objApp;
         }
      }
      catch (...) {}

      // 若失败则释放
      if (result == NULL)
         RCKAppFree(pAppID);
   }
   else
      ARetCode = RCKGetLastError();

   // 返回结果
   return result;
}

// 释放应用对象
void TRCKObjs::FreeAppObj(TRCAppObj* AppObj)
{
   // 初始化
   TRCAppObj* objApp = NULL;

   // 查找并释放
   Lock();
   try
   {
      void* pNode = FAppObjs->Find(AppObj);
      if (pNode != NULL)
      {
         objApp   = FAppObjs->Value(pNode);
         FAppObjs->Remove(pNode);

         // 释放 ID 项
         pNode    = FAppIDs->Find(objApp->AppID());
         if ((pNode != NULL) && (FAppIDs->Value(pNode) == objApp))
            FAppIDs->Remove(pNode);
      }
   }
   catch (...) {}
   Unlock();

   // 若存在则引用计数减 1
   if (objApp != NULL)
      _DecRefAppObj(objApp);
}

// 应用对象引用计数增 1
TRCAppObj* TRCKObjs::IncRefAppObj(void* AppID)
{
   // 初始化
   TRCAppObj* result = NULL;

   // 查找
   Lock();
   try
   {
      void* pNode = FAppIDs->Find(AppID);
      if (pNode != NULL)
      {
         result   = FAppIDs->Value(pNode);
         result->IncObjTimes();
      }
   }
   catch (...) {}
   Unlock();

   // 返回结果
   return result;
}

// 应用对象引用计数增 1
bool TRCKObjs::IncRefAppObj(TRCAppObj* AppObj)
{
   // 初始化
   bool result = false;

   // 查找
   Lock();
   try
   {
      void* pNode = FAppObjs->Find(AppObj);
      if (pNode != NULL)
      {
         AppObj->IncObjTimes();
         result   = true;
      }
   }
   catch (...) {}
   Unlock();

   // 返回结果
   return result;
}

// 创建连接对象(注: AConnID 必须存在)
TRCConnObj* TRCKObjs::CreateConnObj(void* AConnID, bool ANeedFree, long& ARetCode)
{
   // 初始化
   TRCConnObj* result = NULL;

   // 检查连接是否存在
   RCKConnGetInt(AConnID, kcaState, &ARetCode);
   if (ARetCode == krSuccess)
   {
      ARetCode = krUnknown;

      // 创建对象, 并加入列表
      try
      {
         TRCConnObj* objConn = new TRCConnObj;
         if (objConn != NULL)
         {
            Lock();
            try
            {
               // 添加到 ID 列表
               void* pNode = FConnIDs->Add(AConnID, objConn);
               if (pNode == NULL)
               {
                  if (FConnIDs->Existed(AConnID))
                     ARetCode          = krIsExisted;
               }
               else if ((FConnObjs->Add(objConn, objConn) != NULL)
                     || FConnObjs->Existed(objConn))
               {
                  // 设置对象属性
                  objConn->FConnID     = AConnID;
                  objConn->FCanEvent   = true;
                  objConn->FNeedFree   = ANeedFree;
                  result               = objConn;
                  ARetCode             = krSuccess;
               }
               else
                  FConnIDs->Remove(pNode);
            }
            catch (...) {}
            Unlock();

            // 若失败则释放对象
            if (result == NULL)
               delete objConn;
         }
      }
      catch (...) {}
   }

   // 返回结果
   return result;
}

// 释放连接对象
void TRCKObjs::FreeConnObj(TRCConnObj* AConnObj)
{
   // 初始化
   TRCConnObj* objConn = NULL;

   // 查找并释放
   Lock();
   try
   {
      void* pNode = FConnObjs->Find(AConnObj);
      if (pNode != NULL)
      {
         objConn  = FConnObjs->Value(pNode);
         FConnObjs->Remove(pNode);

         // 释放 ID 项
         pNode    = FConnIDs->Find(objConn->ConnID());
         if ((pNode != NULL) && (FConnIDs->Value(pNode) == objConn))
            FConnIDs->Remove(pNode);
      }
   }
   catch (...) {}
   Unlock();

   // 若存在则引用计数减 1
   if (objConn != NULL)
      _DecRefConnObj(objConn);
}

// 连接对象引用计数增 1
TRCConnObj* TRCKObjs::IncRefConnObj(void* AConnID)
{
   // 初始化
   TRCConnObj* result = NULL;

   // 查找
   Lock();
   try
   {
      void* pNode = FConnIDs->Find(AConnID);
      if (pNode != NULL)
      {
         result   = FConnIDs->Value(pNode);
         result->IncObjTimes();
      }
   }
   catch (...) {}
   Unlock();

   // 返回结果
   return result;
}

// 连接对象引用计数增 1
bool TRCKObjs::IncRefConnObj(TRCConnObj* AConnObj)
{
   // 初始化
   bool result = false;

   // 查找
   Lock();
   try
   {
      void* pNode = FConnObjs->Find(AConnObj);
      if (pNode != NULL)
      {
         AConnObj->IncObjTimes();
         result   = true;
      }
   }
   catch (...) {}
   Unlock();

   // 返回结果
   return result;
}

// 创建命令对象
TRCCmdObj* TRCKObjs::CreateCmdObj(void* AConnID)
{
   // 初始化
   TRCCmdObj* result = NULL;

   // 弹出队列
   Lock();
   try
   {
      result = (TRCCmdObj*)FCmdQueue->Pop();
   }
   catch (...) {}
   Unlock();

   // 判断是否为空
   if (result == NULL)
      result = new TRCCmdObj;

   // 设置连接
   result->SetConnID(AConnID);

   // 返回结果
   return result;
}

// 释放命令对象
void TRCKObjs::FreeCmdObj(TRCCmdObj* ACmdObj)
{
   if (ACmdObj != NULL)
   {
      // 初始化
      bool boolPush = false;

      // 关闭命令
      ACmdObj->End();

      // 压入队列
      Lock();
      try
      {
         boolPush = FCmdQueue->Push(ACmdObj);
      }
      catch (...) {}
      Unlock();

      // 压入队列失败则释放对象
      if (!boolPush)
         delete ACmdObj;
   }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 初始化/释放单元 */

// 初始化 RCKObjs 单元(注: 内部有引用计数)
void InitRCKObjs()
{
   TRCKObjs::_InitObj();
}

// 释放 RCKObjs 单元(注: 内部有引用计数)
void FreeRCKObjs(bool AForced)
{
   TRCKObjs::_FreeObj(AForced);
}

// 命令ID -> 对象
TRCCmdObj* CmdID2Object(void* ACmdID)
{
   return (TRCCmdObj*)RCKCmdGetObj(ACmdID, kmaData, NULL);
}

// 函数ID -> 对象
TRCFuncObj* FuncID2Object(void* AFuncID)
{
   // 初始化
   TRCFuncObj* result = NULL;

   // 判断是否已经初始化
   if (TRCKObjs::_obj() != NULL)
      result = TRCKObjs::_obj()->FuncID2Object(AFuncID);

   // 返回结果
   return result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 应用对象(TRCAppObj)的相关函数 */

// 创建应用对象
TRCAppObj* CreateAppObj(const char* AName, const char* APassword, long* ARetCode)
{
   // 初始化
   TRCAppObj* result  = NULL;
   long       intCode = krNotExist;

   // 判断是否已经初始化
   if (TRCKObjs::_obj() != NULL)
      result = TRCKObjs::_obj()->CreateAppObj(AName, APassword, intCode);

   // 设置返回码
   if (ARetCode != NULL)
      *ARetCode = intCode;

   // 返回结果
   return result;
}

// 释放应用对象
void FreeAppObj(TRCAppObj* AppObj)
{
   // 判断是否已经初始化
   if (TRCKObjs::_obj() != NULL)
      TRCKObjs::_obj()->FreeAppObj(AppObj);
}

// 应用对象引用计数增 1
TRCAppObj* IncRefAppObj(void* AppID)
{
   // 初始化
   TRCAppObj* result = NULL;

   // 判断是否已经初始化
   if (TRCKObjs::_obj() != NULL)
      result = TRCKObjs::_obj()->IncRefAppObj(AppID);

   // 返回结果
   return result;
}

// 应用对象引用计数增 1
bool IncRefAppObj_(TRCAppObj* AppObj)
{
   return (TRCKObjs::_obj() != NULL) && TRCKObjs::_obj()->IncRefAppObj(AppObj);
}

// 应用对象引用计数增 1(注: AppObj 对象必须存在)
void IncRefAppObj(TRCAppObj* AppObj)
{
   TRCKObjs::_IncRefAppObj(AppObj);
}

// 应用对象引用计数减 1(注: AppObj 对象必须存在)
void DecRefAppObj(TRCAppObj* AppObj)
{
   TRCKObjs::_DecRefAppObj(AppObj);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 连接对象(TRCConnObj)的相关函数 */

// 创建连接对象(注: AConnID 必须存在)
TRCConnObj* CreateConnObj(void* AConnID, bool ANeedFree, long* ARetCode)
{
   // 初始化
   TRCConnObj* result  = NULL;
   long        intCode = krNotExist;

   // 判断是否已经初始化
   if (TRCKObjs::_obj() != NULL)
      result = TRCKObjs::_obj()->CreateConnObj(AConnID, ANeedFree, intCode);

   // 设置返回码
   if (ARetCode != NULL)
      *ARetCode = intCode;

   // 返回结果
   return result;
}

// 释放连接对象
void FreeConnObj(TRCConnObj* AConnObj)
{
   // 判断是否已经初始化
   if (TRCKObjs::_obj() != NULL)
      TRCKObjs::_obj()->FreeConnObj(AConnObj);
}

// 连接对象引用计数增 1
TRCConnObj* IncRefConnObj(void* AConnID)
{
   // 初始化
   TRCConnObj* result = NULL;

   // 判断是否已经初始化
   if (TRCKObjs::_obj() != NULL)
      result = TRCKObjs::_obj()->IncRefConnObj(AConnID);

   // 返回结果
   return result;
}

// 连接对象引用计数增 1
bool IncRefConnObj_(TRCConnObj* AConnObj)
{
   return (TRCKObjs::_obj() != NULL) && TRCKObjs::_obj()->IncRefConnObj(AConnObj);
}

// 连接对象引用计数增 1(注: AConnObj 对象必须存在)
void IncRefConnObj(TRCConnObj* AConnObj)
{
   TRCKObjs::_IncRefConnObj(AConnObj);
}

// 连接对象引用计数减 1(注: AConnObj 对象必须存在)
void DecRefConnObj(TRCConnObj* AConnObj)
{
   TRCKObjs::_DecRefConnObj(AConnObj);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 命令对象(TRCCmdObj)的相关函数 */

// 创建命令对象
TRCCmdObj* CreateCmdObj(void* AConnID)
{
   // 初始化
   TRCCmdObj* result = NULL;

   // 判断是否已经初始化
   if (TRCKObjs::_obj() != NULL)
      result = TRCKObjs::_obj()->CreateCmdObj(AConnID);

   // 返回结果
   return result;
}

// 创建命令对象(注: AConnObj 必须存在)
TRCCmdObj* CreateCmdObj(TRCConnObj* AConnObj)
{
   // 初始化
   TRCCmdObj* result = NULL;

   // 判断是否已经初始化
   if (TRCKObjs::_obj() != NULL)
      result = TRCKObjs::_obj()->CreateCmdObj(AConnObj->ConnID());

   // 返回结果
   return result;
}

// 释放命令对象
void FreeCmdObj(TRCCmdObj* ACmdObj)
{
   // 判断是否已经初始化
   if (TRCKObjs::_obj() != NULL)
      TRCKObjs::_obj()->FreeCmdObj(ACmdObj);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TRCAppObj - RCK 应用类 */

// ---------------- 静态函数 ----------------
// 应用的 OnConnLogin 回调事件
void __stdcall TRCAppObj::_RCKOnConnLogin(void* AppID, void* AConnID, bool& AIsRefused)
{
   TRCAppObj* objApp = TRCKObjs::_obj()->IncRefAppObj(AppID);
   if (objApp != NULL)
   {
      objApp->DoConnLogin(AConnID, AIsRefused);
      TRCKObjs::_DecRefAppObj(objApp);
   }
   else
      AIsRefused  = true;
}

// 应用的 OnConnLogout 回调事件
void __stdcall TRCAppObj::_RCKOnConnLogout(void* AppID, void* AConnID)
{
   TRCAppObj* objApp = TRCKObjs::_obj()->IncRefAppObj(AppID);
   if (objApp != NULL)
   {
      objApp->DoConnLogout(AConnID);
      TRCKObjs::_DecRefAppObj(objApp);
   }
}

// 应用的 OnExecResp 回调事件
void __stdcall TRCAppObj::_RCKOnExecResp(void* AppID, void* AFuncID, void* AConnID)
{
   TRCAppObj* objApp = TRCKObjs::_obj()->IncRefAppObj(AppID);
   if (objApp != NULL)
   {
      objApp->DoExecResp(AFuncID, AConnID);
      TRCKObjs::_DecRefAppObj(objApp);
   }
}

// 应用的 OnRespCancel 回调事件
void __stdcall TRCAppObj::_RCKOnRespCancel(void* AppID, void* AFuncID, void* AConnID)
{
   TRCAppObj* objApp = TRCKObjs::_obj()->IncRefAppObj(AppID);
   if (objApp != NULL)
   {
      objApp->DoRespCancel(AFuncID, AConnID);
      TRCKObjs::_DecRefAppObj(objApp);
   }
}

// 应用的 OnRespConfirm 回调事件
void __stdcall TRCAppObj::_RCKOnRespConfirm(void* AppID, void* AFuncID, void* AConnID)
{
   TRCAppObj* objApp = TRCKObjs::_obj()->IncRefAppObj(AppID);
   if (objApp != NULL)
   {
      objApp->DoRespConfirm(AFuncID, AConnID);
      TRCKObjs::_DecRefAppObj(objApp);
   }
}

// ---------------- 构造函数和析构函数 ----------------
// 构造函数
TRCAppObj::TRCAppObj()
{
   // 初始化
   FData       = NULL;
   FAppID      = NULL;
   FObjTimes   = 1;

   // 创建对象
   FLock       = new TKYCritSect;
   FFuncList   = new TObjList(false, false);

   // 执行初始化事件
   DoInitEvent();
}

// 析构函数
TRCAppObj::~TRCAppObj()
{
   // 判断是否非空
   if (FAppID != NULL)
   {
      // 关闭
      Close();

      // 执行清除函数列表
      DoClearFuncObjs();

      // 释放应用对象
      RCKAppFree(FAppID);
   }

   // 释放对象
   FreeAndNil(FLock);
   FreeAndNil(FFuncList);
}

// ---------------- 私有函数 ----------------
// 执行初始化事件
void TRCAppObj::DoInitEvent()
{
   OnFuncFree.Method       = NULL;
   OnFuncFree.Object       = NULL;

   FOnConnLogin.Method     = NULL;
   FOnConnLogin.Object     = NULL;
   FOnConnLogout.Method    = NULL;
   FOnConnLogout.Object    = NULL;

   FOnExecResp.Method      = NULL;
   FOnExecResp.Object      = NULL;
   FOnRespCancel.Method    = NULL;
   FOnRespCancel.Object    = NULL;
   FOnRespConfirm.Method   = NULL;
   FOnRespConfirm.Object   = NULL;
}

// 执行清除函数列表
void TRCAppObj::DoClearFuncObjs()
{
   // 初始化
   TKYList  objIDs(false);
   TKYList  objObjs(false);
   bool     boolNext = false;

   // 拷贝并清除
   Lock();
   if (FFuncList->Count() > 0)
      try
      {
         // 设置容量
         objIDs.ChangeCapacity(FFuncList->Count());
         objObjs.ChangeCapacity(FFuncList->Count());

         // 循环拷贝
         void* pNode = FFuncList->Next(NULL);
         while (pNode != NULL)
         {
            objIDs.Add(FFuncList->Value(pNode));
            objObjs.Add(FFuncList->Key(pNode));
            pNode = FFuncList->Next(pNode);
         }

         // 清除
         FFuncList->Clear();
         boolNext = true;
      }
      catch (...) {}
   Unlock();

   // 判断是否继续
   if (boolNext)
   {
      // 设置 OnDeletion 事件方法
      objIDs.OnDeletion.Object   = this;
      objIDs.OnDeletion.Method   = (TKYList::TDoDeletion)&TRCAppObj::DoIDDeletion;
      objObjs.OnDeletion.Object  = this;
      objObjs.OnDeletion.Method  = (TKYList::TDoDeletion)&TRCAppObj::DoObjDeletion;

      // 清除
      objIDs.Clear();
      objObjs.Clear();
   }
}

// 激发 OnConnLogin 事件
void TRCAppObj::DoConnLogin(void* AConnID, bool& AIsRefused)
{
   try
   {
      AIsRefused = true;
      ((TObject*)FOnConnLogin.Object->*FOnConnLogin.Method)(this, AConnID, AIsRefused);
   }
   catch (...) {}
}

// 激发 OnConnLogout 事件
void TRCAppObj::DoConnLogout(void* AConnID)
{
   try
   {
      ((TObject*)FOnConnLogout.Object->*FOnConnLogout.Method)(this, AConnID);
   }
   catch (...) {}
}

// 激发 OnExecResp 事件
void TRCAppObj::DoExecResp(void* AFuncID, void* AConnID)
{
   try
   {
      ((TObject*)FOnExecResp.Object->*FOnExecResp.Method)(this, AFuncID, AConnID);
   }
   catch (...) {}
}

// 激发 OnRespCancel 事件
void TRCAppObj::DoRespCancel(void* AFuncID, void* AConnID)
{
   try
   {
      ((TObject*)FOnRespCancel.Object->*FOnRespCancel.Method)(this, AFuncID, AConnID);
   }
   catch (...) {}
}

// 激发 OnRespConfirm 事件
void TRCAppObj::DoRespConfirm(void* AFuncID, void* AConnID)
{
   try
   {
      ((TObject*)FOnRespConfirm.Object->*FOnRespConfirm.Method)(this, AFuncID, AConnID);
   }
   catch (...) {}
}

// 列表的 OnDeletion 事件方法
void TRCAppObj::DoIDDeletion(void* AFuncID)
{
   // 从全局列表中删除
   if (TRCKObjs::_obj()->DeleteFuncID(AFuncID))
      RCKFuncFree(AFuncID);
}

// 列表的 OnDeletion 事件方法
void TRCAppObj::DoObjDeletion(TRCFuncObj* AFuncObj)
{
   // 激发 OnFuncFree 事件
   if (OnFuncFree.Method != NULL)
      try
      {
         ((TObject*)OnFuncFree.Object->*OnFuncFree.Method)(this, AFuncObj);
      }
      catch (...) {}

   // 释放
   InterlockedExchangePointer((Pointer*)&AFuncObj->FAppObj, NULL);
   delete AFuncObj;
}

// ---------------- 公有函数 ----------------
// 设置 Name 属性
long TRCAppObj::SetName(const KYString& AName)
{
   // 初始化
   long result = krUnknown;

   // 操作
   Lock();
   if (FAppID != NULL)
   {
      result = RCKAppSetStr(FAppID, kaaName, (char*)AName, AName.Length());
      if (result == krSuccess)
         FName = AName;
   }
   Unlock();

   // 返回结果
   return result;
}

// 新建函数对象
TRCFuncObj* TRCAppObj::NewFuncObj(const char* AName, long ARetType,  long  ARetSize,
                                                     long ACallMode, void* ACallback)
{
   // 初始化
   TRCFuncObj* result   = NULL;
   KYString    strName  = AName;
   void*       pFuncID  = RCKFuncCreate(FAppID, ACallMode, ACallback,
                                        (char*)strName, ARetType, ARetSize);

   // 判断是否创建成功
   if (pFuncID != NULL)
   {
      // 创建对象并加入列表
      try
      {
         TRCFuncObj* objFunc  = new TRCFuncObj;
         if (objFunc != NULL)
         {
            Lock();
            try
            {
               if (FFuncList->Add(objFunc, pFuncID) != NULL)
               {
                  objFunc->FName    = strName;
                  objFunc->FFuncID  = pFuncID;
                  objFunc->FAppObj  = this;
                  result            = objFunc;

                  // 加入列表
                  TRCKObjs::_obj()->AddFuncID(pFuncID, objFunc);
               }
            }
            catch (...) {}
            Unlock();

            // 若失败则释放对象
            if (result == NULL)
               delete objFunc;
         }
      }
      catch (...) {}

      // 若失败则释放函数
      if (result == NULL)
         RCKFuncFree(pFuncID);
   }

   // 返回结果
   return result;
}

// 删除函数对象
bool TRCAppObj::DeleteFuncObj(TRCFuncObj* AFuncObj)
{
   // 初始化
   bool result = false;

   // 检查状态
   if ((FAppID != NULL) && (State() == kasDefining))
   {
      // 初始化
      void* pFuncID  = NULL;

      // 从列表中删除
      Lock();
      try
      {
         void* pNode = FFuncList->Find(AFuncObj);
         if (pNode != NULL)
         {
            pFuncID  = FFuncList->Value(pFuncID);
            result   = true;
            FFuncList->Remove(pNode);
         }
      }
      catch (...) {}
      Unlock();

      // 判断是否成功
      if (result)
      {
         DoIDDeletion(pFuncID);
         DoObjDeletion(AFuncObj);
      }
   }

   // 返回结果
   return result;
}

// 清除函数定义
void TRCAppObj::ClearFuncObjs()
{
   // 检查状态
   if ((FAppID != NULL) && (State() == kasDefining))
      DoClearFuncObjs();
}

// 设置 OnConnLogin 事件
long TRCAppObj::SetOnConnLogin(TDoConnLogin AMethod, void* AObject)
{
   // 初始化
   long result = krUnknown;

   // 操作
   Lock();
   if (FAppID != NULL)
   {
      void* pFunc = (AMethod != NULL) ? &_RCKOnConnLogin : NULL;
      result = RCKAppSetObj(FAppID, kaaOnConnLogin, pFunc);
      if (result != krSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnConnLogin.Object = AObject;
         FOnConnLogin.Method = AMethod;
      }
      else
      {
         FOnConnLogin.Method = NULL;
         FOnConnLogin.Object = NULL;
      }
   }
   Unlock();

   // 返回结果
   return result;
}

// 设置 OnConnLogout 事件
long TRCAppObj::SetOnConnLogout(TDoConnLogout AMethod, void* AObject)
{
   // 初始化
   long result = krUnknown;

   // 操作
   Lock();
   if (FAppID != NULL)
   {
      void* pFunc = (AMethod != NULL) ? &_RCKOnConnLogout : NULL;
      result = RCKAppSetObj(FAppID, kaaOnConnLogout, pFunc);
      if (result != krSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnConnLogout.Object = AObject;
         FOnConnLogout.Method = AMethod;
      }
      else
      {
         FOnConnLogout.Method = NULL;
         FOnConnLogout.Object = NULL;
      }
   }
   Unlock();

   // 返回结果
   return result;
}

// 设置 OnExecResp 事件
long TRCAppObj::SetOnExecResp(TDoRespEvent AMethod, void* AObject)
{
   // 初始化
   long result = krUnknown;

   // 操作
   Lock();
   if (FAppID != NULL)
   {
      void* pFunc = (AMethod != NULL) ? &_RCKOnExecResp : NULL;
      result = RCKAppSetObj(FAppID, kaaOnExecResp, pFunc);
      if (result != krSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnExecResp.Object = AObject;
         FOnExecResp.Method = AMethod;
      }
      else
      {
         FOnExecResp.Method = NULL;
         FOnExecResp.Object = NULL;
      }
   }
   Unlock();

   // 返回结果
   return result;
}

// 设置 OnRespCancel 事件
long TRCAppObj::SetOnRespCancel(TDoRespEvent AMethod, void* AObject)
{
   // 初始化
   long result = krUnknown;

   // 操作
   Lock();
   if (FAppID != NULL)
   {
      void* pFunc = (AMethod != NULL) ? &_RCKOnRespCancel : NULL;
      result = RCKAppSetObj(FAppID, kaaOnRespCancel, pFunc);
      if (result != krSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnRespCancel.Object = AObject;
         FOnRespCancel.Method = AMethod;
      }
      else
      {
         FOnRespCancel.Method = NULL;
         FOnRespCancel.Object = NULL;
      }
   }
   Unlock();

   // 返回结果
   return result;
}

// 设置 OnRespConfirm 事件
long TRCAppObj::SetOnRespConfirm(TDoRespEvent AMethod, void* AObject)
{
   // 初始化
   long result = krUnknown;

   // 操作
   Lock();
   if (FAppID != NULL)
   {
      void* pFunc = (AMethod != NULL) ? &_RCKOnRespConfirm : NULL;
      result = RCKAppSetObj(FAppID, kaaOnRespConfirm, pFunc);
      if (result != krSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnRespConfirm.Object = AObject;
         FOnRespConfirm.Method = AMethod;
      }
      else
      {
         FOnRespConfirm.Method = NULL;
         FOnRespConfirm.Object = NULL;
      }
   }
   Unlock();

   // 返回结果
   return result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TRCFuncObj - RCK 函数类 */

// ---------------- 构造函数和析构函数 ----------------
// 构造函数
TRCFuncObj::TRCFuncObj()
{
   FData       = NULL;
   FFuncID     = NULL;
   FAppObj     = NULL;
}

// 析构函数
TRCFuncObj::~TRCFuncObj()
{
   TRCAppObj* objApp = (TRCAppObj*)InterlockedExchangePointer((Pointer*)&FAppObj, NULL);
   if (objApp != NULL)
      objApp->DeleteFuncObj(this);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TRCConnObj - RCK 连接类 */

// ---------------- 静态函数 ----------------
// 连接的 OnConnect 回调事件
void __stdcall TRCConnObj::_RCKOnConnect(void* AConnID)
{
   TRCConnObj* objConn = TRCKObjs::_obj()->IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      objConn->DoConnect();
      TRCKObjs::_DecRefConnObj(objConn);
   }
}

// 连接的 OnDisconnect 回调事件
void __stdcall TRCConnObj::_RCKOnDisconnect(void* AConnID)
{
   TRCConnObj* objConn = TRCKObjs::_obj()->IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      objConn->DoDisconnect();
      TRCKObjs::_DecRefConnObj(objConn);
   }
}

// 连接的 OnLogin 回调事件
void __stdcall TRCConnObj::_RCKOnLogin(void* AConnID)
{
   TRCConnObj* objConn = TRCKObjs::_obj()->IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      objConn->DoLogin();
      TRCKObjs::_DecRefConnObj(objConn);
   }
}

// 连接的 OnLogout 回调事件
void __stdcall TRCConnObj::_RCKOnLogout(void* AConnID)
{
   TRCConnObj* objConn = TRCKObjs::_obj()->IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      objConn->DoLogout();
      TRCKObjs::_DecRefConnObj(objConn);
   }
}

// 连接的 OnCmdReturn 回调事件
void __stdcall TRCConnObj::_RCKOnCmdReturn(void* AConnID, void* ACmdID, long AResult)
{
   TRCConnObj* objConn = TRCKObjs::_obj()->IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      objConn->DoCmdReturn(ACmdID, AResult);
      TRCKObjs::_DecRefConnObj(objConn);
   }
}

// 连接的 OnRecvData 回调事件
void __stdcall TRCConnObj::_RCKOnRecvData(void* AConnID, const void* AData, long ASize)
{
   TRCConnObj* objConn = TRCKObjs::_obj()->IncRefConnObj(AConnID);
   if (objConn != NULL)
   {
      objConn->DoRecvData(AData, ASize);
      TRCKObjs::_DecRefConnObj(objConn);
   }
}

// ---------------- 构造函数和析构函数 ----------------
// 构造函数
TRCConnObj::TRCConnObj(void* AConnID, bool ANeedFree)
{
   // 初始化
   FData          = NULL;
   FConnID        = AConnID;
   FObjTimes      = 1;
   FCanEvent      = false;
   FNeedFree      = ANeedFree && (AConnID != NULL);

   // 执行初始化事件
   DoInitEvent();
}

// 析构函数
TRCConnObj::~TRCConnObj()
{
   // 判断是否需要释放连接
   if (FNeedFree && (FConnID != NULL))
      RCKConnFree(FConnID);
}

// ---------------- 私有函数 ----------------
// 执行初始化事件
void TRCConnObj::DoInitEvent()
{
   FOnConnect.Method    = NULL;
   FOnConnect.Object    = NULL;
   FOnDisconnect.Method = NULL;
   FOnDisconnect.Object = NULL;

   FOnLogin.Method      = NULL;
   FOnLogin.Object      = NULL;
   FOnLogout.Method     = NULL;
   FOnLogout.Object     = NULL;

   FOnCmdReturn.Method  = NULL;
   FOnCmdReturn.Object  = NULL;
   FOnRecvData.Method   = NULL;
   FOnRecvData.Object   = NULL;
}

// 激发 OnConnect 事件
void TRCConnObj::DoConnect()
{
   try
   {
      ((TObject*)FOnConnect.Object->*FOnConnect.Method)(this);
   }
   catch (...) {}
}

// 激发 OnDisconnect 事件
void TRCConnObj::DoDisconnect()
{
   try
   {
      ((TObject*)FOnDisconnect.Object->*FOnDisconnect.Method)(this);
   }
   catch (...) {}
}

// 激发 OnLogin 事件
void TRCConnObj::DoLogin()
{
   try
   {
      ((TObject*)FOnLogin.Object->*FOnLogin.Method)(this);
   }
   catch (...) {}
}

// 激发 OnLogout 事件
void TRCConnObj::DoLogout()
{
   try
   {
      ((TObject*)FOnLogout.Object->*FOnLogout.Method)(this);
   }
   catch (...) {}
}

// 激发 OnCmdReturn 事件
void TRCConnObj::DoCmdReturn(void* ACmdID, long AResult)
{
   try
   {
      ((TObject*)FOnCmdReturn.Object->*FOnCmdReturn.Method)(this, ACmdID, AResult);
   }
   catch (...) {}
}

// 激发 OnRecvData 事件
void TRCConnObj::DoRecvData(const void* AData, long ASize)
{
   try
   {
      ((TObject*)FOnRecvData.Object->*FOnRecvData.Method)(this, AData, ASize);
   }
   catch (...) {}
}

// ---------------- 公有函数 ----------------
// 设置 OnConnect 事件
long TRCConnObj::SetOnConnect(TDoNotify AMethod, void* AObject)
{
   // 初始化
   long result = krUnknown;

   // 检查参数
   if (FConnID == NULL)
      ;
   else if (!FCanEvent)
      result = krFailure;
   else
   {
      void* pFunc = (AMethod != NULL) ? &_RCKOnConnect : NULL;
      result = RCKConnSetObj(FConnID, kcaOnConnect, pFunc);
      if (result != krSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnConnect.Object = AObject;
         FOnConnect.Method = AMethod;
      }
      else
      {
         FOnConnect.Method = NULL;
         FOnConnect.Object = NULL;
      }
   }

   // 返回结果
   return result;
}

// 设置 OnDisconnect 事件
long TRCConnObj::SetOnDisconnect(TDoNotify AMethod, void* AObject)
{
   // 初始化
   long result = krUnknown;

   // 检查参数
   if (FConnID == NULL)
      ;
   else if (!FCanEvent)
      result = krFailure;
   else
   {
      void* pFunc = (AMethod != NULL) ? &_RCKOnDisconnect : NULL;
      result = RCKConnSetObj(FConnID, kcaOnDisconnect, pFunc);
      if (result != krSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnDisconnect.Object = AObject;
         FOnDisconnect.Method = AMethod;
      }
      else
      {
         FOnDisconnect.Method = NULL;
         FOnDisconnect.Object = NULL;
      }
   }

   // 返回结果
   return result;
}

// 设置 OnLogin 事件
long TRCConnObj::SetOnLogin(TDoNotify AMethod, void* AObject)
{
   // 初始化
   long result = krUnknown;

   // 检查参数
   if (FConnID == NULL)
      ;
   else if (!FCanEvent)
      result = krFailure;
   else
   {
      void* pFunc = (AMethod != NULL) ? &_RCKOnLogin : NULL;
      result = RCKConnSetObj(FConnID, kcaOnLogin, pFunc);
      if (result != krSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnLogin.Object = AObject;
         FOnLogin.Method = AMethod;
      }
      else
      {
         FOnLogin.Method = NULL;
         FOnLogin.Object = NULL;
      }
   }

   // 返回结果
   return result;
}

// 设置 OnLogout 事件
long TRCConnObj::SetOnLogout(TDoNotify AMethod, void* AObject)
{
   // 初始化
   long result = krUnknown;

   // 检查参数
   if (FConnID == NULL)
      ;
   else if (!FCanEvent)
      result = krFailure;
   else
   {
      void* pFunc = (AMethod != NULL) ? &_RCKOnLogout : NULL;
      result = RCKConnSetObj(FConnID, kcaOnLogout, pFunc);
      if (result != krSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnLogout.Object = AObject;
         FOnLogout.Method = AMethod;
      }
      else
      {
         FOnLogout.Method = NULL;
         FOnLogout.Object = NULL;
      }
   }

   // 返回结果
   return result;
}

// 设置 OnCmdReturn 事件
long TRCConnObj::SetOnCmdReturn(TDoCmdReturn AMethod, void* AObject)
{
   // 初始化
   long result = krUnknown;

   // 检查参数
   if (FConnID == NULL)
      ;
   else if (!FCanEvent)
      result = krFailure;
   else
   {
      void* pFunc = (AMethod != NULL) ? &_RCKOnCmdReturn : NULL;
      result = RCKConnSetObj(FConnID, kcaOnCmdReturn, pFunc);
      if (result != krSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnCmdReturn.Object = AObject;
         FOnCmdReturn.Method = AMethod;
      }
      else
      {
         FOnCmdReturn.Method = NULL;
         FOnCmdReturn.Object = NULL;
      }
   }

   // 返回结果
   return result;
}

// 设置 OnRecvData 事件
long TRCConnObj::SetOnRecvData(TDoRecvData AMethod, void* AObject)
{
   // 初始化
   long result = krUnknown;

   // 检查参数
   if (FConnID == NULL)
      ;
   else if (!FCanEvent)
      result = krFailure;
   else
   {
      void* pFunc = (AMethod != NULL) ? &_RCKOnRecvData : NULL;
      result = RCKConnSetObj(FConnID, kcaOnRecvData, pFunc);
      if (result != krSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnRecvData.Object = AObject;
         FOnRecvData.Method = AMethod;
      }
      else
      {
         FOnRecvData.Method = NULL;
         FOnRecvData.Object = NULL;
      }
   }

   // 返回结果
   return result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TRCCmdObj - RCK 命令类 */

// ---------------- 构造函数和析构函数 ----------------
// 构造函数
TRCCmdObj::TRCCmdObj()
{
   FData    = NULL;
   FCmdID   = RCKCmdCreate();

   // 设置关联对象
   if (FCmdID != NULL)
      RCKCmdSetObj(FCmdID, kmaData, this);
}

// 析构函数
TRCCmdObj::~TRCCmdObj()
{
   if (FCmdID != NULL)
   {
      // 清除关联
      RCKCmdSetObj(FCmdID, kmaData, NULL);

      // 释放对象
      RCKCmdFree(FCmdID);
   }
}

// ---------------- 公有函数 ----------------
// 取连接对象
TRCConnObj* TRCCmdObj::ConnObj() const
{
   // 初始化
   TRCConnObj* result   = NULL;
   void*       pConnID  = RCKCmdGetObj(FCmdID, kmaConnObj, NULL);

   // 判断是否非空
   if (pConnID != NULL)
   {
      result = TRCKObjs::_obj()->IncRefConnObj(pConnID);
      if (result != NULL)
         TRCKObjs::_DecRefConnObj(result);
   }

   // 返回结果
   return result;
}

// 执行带参数的命令
long TRCCmdObj::ExecByParams(Longword ATimeout, ...) const
{
   // 取参数个数
   long intCount = ParamCount();
   if (intCount > 0)
   {
      // 初始化
      const void* pUnknow;

      // 清除参数
      RCKCmdParamClear(FCmdID);

      // 可变参数初始化
      va_list  arrArgs;
      va_start(arrArgs, ATimeout);

      // 加入参数
      for (long intNo = 0; intNo < intCount; intNo++)
         switch (ParamType(intNo))
         {
         case kdtString:
            RCKCmdParamByStr(FCmdID, intNo, va_arg(arrArgs, const char*), -1);
            break;

         case kdtInteger:
            RCKCmdParamByInt(FCmdID, intNo, va_arg(arrArgs, int));
            break;

         case kdtBoolean:
            RCKCmdParamByBool(FCmdID, intNo, va_arg(arrArgs, bool));
            break;

         case kdtDouble:
            RCKCmdParamByFloat(FCmdID, intNo, va_arg(arrArgs, double));
            break;

         case kdtStruct:
            RCKCmdParamByStruct(FCmdID, intNo, va_arg(arrArgs, void*));
            break;

         case kdtByte:
            RCKCmdParamByInt(FCmdID, intNo, va_arg(arrArgs, Byte));
            break;

         case kdtWord:
            RCKCmdParamByInt(FCmdID, intNo, va_arg(arrArgs, Word));
            break;

         case kdtSmallint:
            RCKCmdParamByInt(FCmdID, intNo, va_arg(arrArgs, Smallint));
            break;

         case kdtInt64:
            RCKCmdParamByInt64(FCmdID, intNo, va_arg(arrArgs, __int64));
            break;

         case kdtDateTime:
            RCKCmdParamByDate(FCmdID, intNo, va_arg(arrArgs, double));
            break;

         case kdtCurrency:
            RCKCmdParamByFloat(FCmdID, intNo, va_arg(arrArgs, double));
            break;

         default:
            pUnknow = va_arg(arrArgs, const void*);
         }

      // 可变参数释放
      va_end(arrArgs);
   }

   // 执行
   return RCKCmdExecute(FCmdID, ATimeout);
}

// 取返回值的字符串值
KYString TRCCmdObj::RetAsStr() const
{
   // 初始化
   KYString result;
   long     intSize;
   char*    pRet;

   // 取返回值
   pRet  = RCKCmdRetStr(FCmdID, &intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // 返回结果
   return result;
}

// 取返回字段的字符串值
KYString TRCCmdObj::FieldAsStr(long AIndex) const
{
   // 初始化
   KYString result;
   long     intSize;
   char*    pRet;

   // 取返回字段值
   pRet  = RCKCmdFieldStr(FCmdID, AIndex, &intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // 返回结果
   return result;
}

// 取返回字段的字符串值
KYString TRCCmdObj::FieldAsStr(const char* AName) const
{
   // 初始化
   KYString result;
   long     intSize;
   char*    pRet;

   // 取返回字段值
   pRet  = RCKCmdFieldStr_(FCmdID, AName, &intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // 返回结果
   return result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TRCResp - RCK 应答类 */

// ---------------- 静态函数 ----------------
// 取返回值的字符串值
KYString TRCResp::RetAsStr()
{
   // 初始化
   KYString result;
   long     intSize;
   char*    pRet;

   // 取返回值
   pRet  = RCKRespRetStr(&intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // 返回结果
   return result;
}

// 取返回字段的字符串值
KYString TRCResp::FieldAsStr(long AIndex)
{
   // 初始化
   KYString result;
   long     intSize;
   char*    pRet;

   // 取返回字段值
   pRet  = RCKRespFieldStr(AIndex, &intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // 返回结果
   return result;
}

// 取返回字段的字符串值
KYString TRCResp::FieldAsStr(const char* AName)
{
   // 初始化
   KYString result;
   long     intSize;
   char*    pRet;

   // 取返回字段值
   pRet  = RCKRespFieldStr_(AName, &intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // 返回结果
   return result;
}

// 取参数的字符串值
KYString TRCResp::ParamAsStr(long AIndex)
{
   // 初始化
   KYString result;
   long     intSize;
   char*    pRet;

   // 取参数值
   pRet  = RCKRespParamStr(AIndex, &intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // 返回结果
   return result;
}

// 取参数的字符串值
KYString TRCResp::ParamAsStr(const char* AName)
{
   // 初始化
   KYString result;
   long     intSize;
   char*    pRet;

   // 取参数值
   pRet  = RCKRespParamStr_(AName, &intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // 返回结果
   return result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TRCFields - RCK 字段集类 */

// ---------------- 构造函数和析构函数 ----------------
// 构造函数
TRCFields::TRCFields()
{
   FFields  = RCKFieldsCreate();
   FIsBound = (FFields != NULL);
}

TRCFields::TRCFields(void* AFields)
{
   FFields  = AFields;
   FIsBound = false;
}

// 析构函数
TRCFields::~TRCFields()
{
   if (FIsBound)
      RCKFieldsFree(FFields);
}

// ---------------- 公有函数 ----------------
// 取字段定义集字符串
KYString TRCFields::FieldDefs() const
{
   // 初始化
   KYString result;
   long     intSize;
   char*    pRet;

   // 取字段值
   pRet  = (char*)RCKFieldDefsOutput(FFields, &intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // 返回结果
   return result;
}

// 取字段集字符串
KYString TRCFields::DataSet() const
{
   // 初始化
   KYString result;
   long     intSize;
   char*    pRet;

   // 取字段值
   pRet  = (char*)RCKFieldsOutput(FFields, &intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // 返回结果
   return result;
}

// 取字段的字符串值
KYString TRCFields::AsStr(long AIndex) const
{
   // 初始化
   KYString result;
   long     intSize;
   char*    pRet;

   // 取字段值
   pRet  = RCKFieldAsStr(FFields, AIndex, &intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // 返回结果
   return result;
}

// 取字段的字符串值
KYString TRCFields::AsStr(const char* AName) const
{
   // 初始化
   KYString result;
   long     intSize;
   char*    pRet;

   // 取字段值
   pRet  = RCKFieldAsStr_(FFields, AName, &intSize);
   if (pRet != NULL)
      result.SetString(pRet, intSize);

   // 返回结果
   return result;
}

/* end namespace */

   }
}
