// =======================================
// Unit   : AVL tree (KYAVLTreeC.h)
// Version: 3.0.1.0 (build 2012.06.08)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYAVLTreeC_H_
#define _KYAVLTreeC_H_

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ���Ͷ��� */

// AVL ���������
typedef struct
{
   void*       Item;                // ������
   void*       Data;                // �Զ�������
} TKYAVLNode, *PKYAVLNode;

// OnCompare ���Ƚ��¼�
// ������ֵ result == 0 ���ʾ ANode1 ���� ANode2
// ������ֵ result > 0  ���ʾ ANode1 ���� ANode2
// ������ֵ result < 0  ���ʾ ANode1 С�� ANode2
typedef long (*TKYAVL_OnCompare)(const TKYAVLNode* ANode1,
                                 const TKYAVLNode* ANode2);

// OnDeletion ���ɾ���¼�
typedef void (*TKYAVL_OnDeletion)(void* ATree, TKYAVLNode* ANode);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ���ú���, ע: Ϊ��Ч�ʲ���� void* ATree �Ƿ�Ϸ�, �˲�����Ҫ�ⲿ��ά�� */

// ����/�ͷ� AVL ��(ע: �� ATree == NULL ʱ, KYAVLTree_Free ��������쳣)
void*       KYAVLTree_Create(TKYAVL_OnCompare  OnCompare,
                             TKYAVL_OnDeletion OnDeletion);
void        KYAVLTree_Free(void* ATree);

// ��ȡ AVL ������
void*       KYAVLTree_Data(void* ATree);           // default: NULL
long        KYAVLTree_Count(void* ATree);          // default: 0
long        KYAVLTree_MaxCount(void* ATree);       // default: 0
bool        KYAVLTree_CanDuplicate(void* ATree);   // default: false

// ���� AVL ������
void        KYAVLTree_SetData(void* ATree, void* AData);
void        KYAVLTree_SetMaxCount(void* ATree, long AMaxCount);
void        KYAVLTree_SetCanDuplicate(void* ATree, bool ACanDuplicate);

// ��� AVL �������н��, ���� OnDeletion �¼�
void        KYAVLTree_Clear(void* ATree);

// ��� AVL ���, ͬʱ: {Node->Item == AItem, Node->Data == Data}
TKYAVLNode* KYAVLTree_Add(void* ATree, void* AItem, void* AData);

// ɾ��ֵΪ AItem �� Data �� Compare(...) == 0 ��һ�� AVL ���, ���� OnDeletion �¼�
bool        KYAVLTree_Delete(void* ATree, const void* AItem, const void* AData);

// �Ƴ�ָ�� AVL ���, ���� OnDeletion �¼�
bool        KYAVLTree_Remove(void* ATree, TKYAVLNode* ANode);

// ����ֵΪ AItem �� Data �� Compare(...) == 0 ��һ�� AVL ���
TKYAVLNode* KYAVLTree_Find(void* ATree, const void* AItem, const void* AData);
bool        KYAVLTree_Search(void* ATree, void** ARetItem,    void** ARetData,
                                    const void*  AItem, const void*  AData);

// �������һ�� AVL ���
// �� ANearest == NULL ���ʾ��ֵ�������е����һ�����ֵ;
// ������ֵΪ true, ���ʾ�ҵ���ֵ�Ľ��, ������ֵ�� ANearest ���֮ǰ
bool        KYAVLTree_FindNearest(void* ATree, const void* AItem,
                            const void* AData, PKYAVLNode* ANearest);

// �ж� AVL ����Ƿ����
bool        KYAVLTree_Existed(void* ATree, TKYAVLNode* ANode);
//bool      KYAVLTree_Existed(void* ATree, const void* AItem, const void* AData)
//          { return KYAVLTree_Find(ATree, AItem, AData) != NULL; }

// ȡ AVL ���Ľ��
TKYAVLNode* KYAVLTree_Root(void* ATree);           // ���ĸ����, default: NULL
TKYAVLNode* KYAVLTree_Last(void* ATree);           // ����β���, default: NULL
TKYAVLNode* KYAVLTree_First(void* ATree);          // �����׽��, default: NULL

// ȡ AVL ��������
long        KYAVLTree_Level(TKYAVLNode* ANode);    // ������ڵĲ��, Level(NULL)  = -1
long        KYAVLTree_Height(TKYAVLNode* ANode);   // ���������߶�, Height(NULL) = 0
char        KYAVLTree_Balance(TKYAVLNode* ANode);  // ��������ƽ���־

// ȡ AVL ���������ӽ�㼰�����
TKYAVLNode* KYAVLTree_Left(TKYAVLNode* ANode);     // ���ӽ��, Left(NULL)   = NULL
TKYAVLNode* KYAVLTree_Right(TKYAVLNode* ANode);    // ���ӽ��, Right(NULL)  = NULL
TKYAVLNode* KYAVLTree_Parent(TKYAVLNode* ANode);   // �����,   Parent(NULL) = NULL

// ȡ AVL ����ǰ����
TKYAVLNode* KYAVLTree_Prior(TKYAVLNode* ANode);    // ǰһ���, Prior(NULL)  = NULL
TKYAVLNode* KYAVLTree_Next(TKYAVLNode* ANode);     // ��һ���, Next(NULL)   = NULL

}

#endif
