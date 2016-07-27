// =======================================
// Unit   : AVL tree (KYAVLTreeC.h)
// Version: 3.0.1.0 (build 2012.06.08)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYAVLTreeC_H_
#define _KYAVLTreeC_H_

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 类型定义 */

// AVL 树结点类型
typedef struct
{
   void*       Item;                // 项数据
   void*       Data;                // 自定义数据
} TKYAVLNode, *PKYAVLNode;

// OnCompare 结点比较事件
// 若返回值 result == 0 则表示 ANode1 等于 ANode2
// 若返回值 result > 0  则表示 ANode1 大于 ANode2
// 若返回值 result < 0  则表示 ANode1 小于 ANode2
typedef long (*TKYAVL_OnCompare)(const TKYAVLNode* ANode1,
                                 const TKYAVLNode* ANode2);

// OnDeletion 结点删除事件
typedef void (*TKYAVL_OnDeletion)(void* ATree, TKYAVLNode* ANode);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 公用函数, 注: 为了效率不检查 void* ATree 是否合法, 此参数需要外部来维护 */

// 创建/释放 AVL 树(注: 当 ATree == NULL 时, KYAVLTree_Free 不会产生异常)
void*       KYAVLTree_Create(TKYAVL_OnCompare  OnCompare,
                             TKYAVL_OnDeletion OnDeletion);
void        KYAVLTree_Free(void* ATree);

// 读取 AVL 树属性
void*       KYAVLTree_Data(void* ATree);           // default: NULL
long        KYAVLTree_Count(void* ATree);          // default: 0
long        KYAVLTree_MaxCount(void* ATree);       // default: 0
bool        KYAVLTree_CanDuplicate(void* ATree);   // default: false

// 设置 AVL 树属性
void        KYAVLTree_SetData(void* ATree, void* AData);
void        KYAVLTree_SetMaxCount(void* ATree, long AMaxCount);
void        KYAVLTree_SetCanDuplicate(void* ATree, bool ACanDuplicate);

// 清除 AVL 树的所有结点, 激发 OnDeletion 事件
void        KYAVLTree_Clear(void* ATree);

// 添加 AVL 结点, 同时: {Node->Item == AItem, Node->Data == Data}
TKYAVLNode* KYAVLTree_Add(void* ATree, void* AItem, void* AData);

// 删除值为 AItem 和 Data 的 Compare(...) == 0 第一个 AVL 结点, 激发 OnDeletion 事件
bool        KYAVLTree_Delete(void* ATree, const void* AItem, const void* AData);

// 移除指定 AVL 结点, 激发 OnDeletion 事件
bool        KYAVLTree_Remove(void* ATree, TKYAVLNode* ANode);

// 搜索值为 AItem 和 Data 的 Compare(...) == 0 第一个 AVL 结点
TKYAVLNode* KYAVLTree_Find(void* ATree, const void* AItem, const void* AData);
bool        KYAVLTree_Search(void* ATree, void** ARetItem,    void** ARetData,
                                    const void*  AItem, const void*  AData);

// 查找最近一个 AVL 结点
// 若 ANearest == NULL 则表示项值大于树中的最后一个结点值;
// 若返回值为 true, 则表示找到项值的结点, 否则项值在 ANearest 结点之前
bool        KYAVLTree_FindNearest(void* ATree, const void* AItem,
                            const void* AData, PKYAVLNode* ANearest);

// 判断 AVL 结点是否存在
bool        KYAVLTree_Existed(void* ATree, TKYAVLNode* ANode);
//bool      KYAVLTree_Existed(void* ATree, const void* AItem, const void* AData)
//          { return KYAVLTree_Find(ATree, AItem, AData) != NULL; }

// 取 AVL 树的结点
TKYAVLNode* KYAVLTree_Root(void* ATree);           // 树的根结点, default: NULL
TKYAVLNode* KYAVLTree_Last(void* ATree);           // 树的尾结点, default: NULL
TKYAVLNode* KYAVLTree_First(void* ATree);          // 树的首结点, default: NULL

// 取 AVL 结点的属性
long        KYAVLTree_Level(TKYAVLNode* ANode);    // 结点所在的层号, Level(NULL)  = -1
long        KYAVLTree_Height(TKYAVLNode* ANode);   // 结点的子树高度, Height(NULL) = 0
char        KYAVLTree_Balance(TKYAVLNode* ANode);  // 结点的子树平衡标志

// 取 AVL 结点的左右子结点及父结点
TKYAVLNode* KYAVLTree_Left(TKYAVLNode* ANode);     // 左子结点, Left(NULL)   = NULL
TKYAVLNode* KYAVLTree_Right(TKYAVLNode* ANode);    // 右子结点, Right(NULL)  = NULL
TKYAVLNode* KYAVLTree_Parent(TKYAVLNode* ANode);   // 父结点,   Parent(NULL) = NULL

// 取 AVL 结点的前后结点
TKYAVLNode* KYAVLTree_Prior(TKYAVLNode* ANode);    // 前一结点, Prior(NULL)  = NULL
TKYAVLNode* KYAVLTree_Next(TKYAVLNode* ANode);     // 下一结点, Next(NULL)   = NULL

}

#endif
