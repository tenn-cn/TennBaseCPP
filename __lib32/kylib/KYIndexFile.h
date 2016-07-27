// =======================================
// Unit   : �����ļ��� (KYIndexFile.h)
// Version: 3.2.0.0 (build 2012.08.16)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYIndexFile_H_
#define _KYIndexFile_H_

#include "KYFields.h"
#include "KYAVLTree.h"
#include "KYMemFile.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

class TKYIndexTree;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYIndexFile - �����ļ��� */

class TKYIndexFile
{
public:
   // �ļ�����
   enum TType       {ftMemFile   = 0,     // �ڴ��ļ�    (TKYMemFile)
                     ftMapFile   = 1,     // ӳ���ļ�    (TKYMemFileEx)
                     ftPartFile  = 2};    // �����ڴ��ļ�(TKYPartMemFile)

   // �����ļ�״̬
   enum TState      {fsInactive,          // δ��
                     fsOpening,           // ���ڴ�
                     fsClosing,           // ���ڹر�
                     fsOpened};           // �Ѿ���

   // �汾��Ϣ
   #pragma pack(push, 1)
   typedef struct
   {
      Byte           Major;               // ���汾
      Byte           Minor;               // С�汾
      Byte           Release;             // ����С�汾
      Byte           Build;               // ����С�汾
   } TVersion, *PVersion;
   #pragma pack(pop)

protected:
   // �ļ�ͷ��Ϣ
   #pragma pack(push, 1)
   typedef struct
   {
      char           Flag[4];             // �ļ���Ƿ�
      TVersion       Version;             // �����ļ��汾
      long           TagSize;             // �������ݳߴ�
      long           TagOrigin;           // �������ݿ�ʼλ��
      Word           BlockItems;          // �������[1..65535]
      Word           IndexCount;          // ��������[0..65535]
      long           FirstBlock;          // ��һ���������λ��
      long           LastBlock;           // ���һ���������λ��
      long           TailOrigin;          // �ļ�β��ʼλ��
   } THead, *PHead;
   #pragma pack(pop)

private:
   friend class TKYIndexTree;

   // TDoClosed �����ļ��ѹرյ��¼�����
   typedef void (TObject::*TDoClosed)(TKYIndexFile* AFile);

   // ���������������
   typedef union
   {
      void*          Value;
      TDoClosed      Method;
   } TMethodItem;

public:
   TKYIndexFile(long AType = ftMapFile);
   virtual ~TKYIndexFile();

   // ����
   void*             Data() const         { return FData; }             // default: NULL
   long              Size() const         { return FFile->Size(); }     // default: 0
   TType             Type() const         { return (TType)FType; }      // derault: ftMapFile
   TState            State() const        { return (TState)FState; }    // default: fsInactive
   TVersion          Version() const      { return FHead.Version; }     // default: 0,0,0,0
   Longword          TagSize() const      { return FHead.TagSize; }     // default: 0
   long              IdleSize();                                        // default: 0
   KYString          FileName() const     { return FFile->FileName(); } // default: ""
   bool              ReadOnly() const     { return FFile->ReadOnly(); } // default: false
   bool              IsEditing() const    { return FIsEditing; }        // default: false
   bool              IsOverflow() const   { return FIsOverflow; }       // default: false
   Word              BlockItems() const   { return FHead.BlockItems; }  // default: 0

   // ȡ��������������Ŷ�Ӧ������
   Word              IndexCount();                                      // default: 0
   long              Index(Word AIndexNo);                              // default: 0

   // ��������
   void              SetData(void* AData) { FData = AData; }

   // �������ļ�
   bool              Open(const KYString& AFileName, bool AReadOnly = false,
                                                 Longword ATimeout  = INFINITE);

   // �ر������ļ�
   void              Close(bool ANeedWait = false);

   // ������浽д���ļ���
   void              Flush();

   // Ԥ���������ļ�
   // ע: Ϊ�˰��ļ�����ȷ�����ص��ڴ�ӳ����, �ӿ���ζ�ȡ�ļ������ٶ�, һ������
   // �ļ��򿪺���á�ASize <= 0 ��ʾ�����ļ�����, Ҳ����Ԥ�����ļ���ͷ��ʼ��ָ
   // �� ASize ���֡�
   bool              Preload(long ASize = 0);

   // ��ȡ��������: ASize ����Ҫ��ȡ�ĳߴ�, ����ʵ�ʶ�ȡ�ĳߴ�
   bool              ReadTag(long AOffset, void* AData, long& ASize);

   // ���渽������: ASize ����Ҫ����ĳߴ�, ����ʵ�ʱ���ĳߴ�
   bool              WriteTag(long AOffset, const void* AData, long& ASize);

protected:
   // ״̬��
   void              Lock() const         { FLock->Enter(); }
   void              Unlock() const       { FLock->Leave(); }

   // �ļ���
   bool              LockFile(Longword ATimeout = INFINITE) const
                     { return FMutex->Lock(ATimeout); }
   void              UnlockFile() const   { FMutex->Unlock(); }

   // ׼��ָ���ļ�λ�óߴ������
   void              Prepare(long AOffset, long ASize)
                     {
                        if (FType == ftPartFile)
                           ((TKYPartMemFile*)FFile)->Prepare(AOffset, ASize);
                     }

   // ƫ����ת�ڴ��ַ(AOffset > 0)
   void*             Offset2Mem(long AOffset) const
                     { return FFile->MemBase() + AOffset; }

   // д�ļ�����
   bool              WriteInt(long AOffset, long AValue)
                     { return FFile->Write(AOffset, &AValue, 4); }
   bool              WriteFile(long AOffset, long ASize)
                     { return FFile->Write(AOffset, ASize); }
   bool              WriteFile(long AOffset, const void* ABuffer, long ASize)
                     { return FFile->Write(AOffset, ABuffer, ASize); }

   // ���ü�������
   bool              IncRefCount(bool ACanEdit = false);
   void              DecRefCount();

   // ��ʼ/�����༭
   // 1. BeginEdit �����ڴ�������, �������, �޸�ֵǰ���÷��������Ч, �Զ�����
   //              LockFile �����ļ����������ü����� 1.
   // 2. EndEdit   �Ա༭���ݱ��浽�ļ�����ȡ�����ݱ༭, ͬʱ�Զ�����UnlockFile
   //              �����ļ����������ü����� 1.
   bool              BeginEdit(Longword ATimeout = INFINITE);
   void              EndEdit(bool ACancel = false);

   // �������, ���ɹ��򷵻��������
   bool              AddIndex(long AIndex, Word& AIndexNo);

   // �����ļ��ռ�, ����ƫ����
   long              Assign(long ASize);

   // �ط���ƫ�������ڴ�
   void*             Reassign(long AOffset, long ASize);

private:
   void              DoInit();
   bool              DoCheck(const THead& AHead);

   // ִ�д�/�ر�
   bool              DoOpen(const KYString& AFileName, bool AReadOnly,
                                  Longword  ATimeout);
   void              DoClose();

   // ���/ɾ�����õĹر��¼�����(ע: Ϊ�˷�ֹ���� ADoClosed �ڲ�����ʹ����)
   bool              DoAddRef(void* AObject, TDoClosed ADoClosed);
   void              DoDeleteRef(void* AObject);
   void              DoCloseRefs();

private:
   void*             FData;               // �Զ�������
   TKYCritSect*      FLock;               // ������
   TKYBaseMemFile*   FFile;               // �ļ�����
   TKYAVLTree*       FRefs;               // ���������ķ����б�
   TKYMutex*         FMutex;              // ����Ԫ
   TKYStringList*    FOffsets;            // �޸����ݵ�ƫ�����б�
   Longword          FRefCount;           // ���ü���
   long              FLastSize;           // ���ߴ�, ������ʱ����ʹ��

   THead             FHead;               // �ļ�ͷ��Ϣ
   long              FType;               // �ļ�����
   Byte              FState;              // �����ļ�״̬
   bool              FIsEditing;          // �Ƿ����ڱ༭
   bool              FIsOverflow;         // �Ƿ����

public:
   // ���������ļ�(ע: ���ļ��Ѵ�����ɾ���ļ������´���)
   static bool       CreateFile(const KYString& AFileName, long AInitedSize,
                                          long  ATagSize,  Word ABlockItems);

   // �����ļ��ߴ�
   static bool       GrowFileSize(const KYString& AFileName, long ANewSize);

public:
   // ��̬��Ա
   static const TVersion _Ver_Current; // ��ǰ�汾(1.0.0.0)
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYIndexTree - �������� */
// ע: Ϊ�����Ч���ڲ�û����, Ҳ����˵, ���̶߳�ͬһ�����������ȫ��

class TKYIndexTree
{
public:
   // ������״̬
   enum TState      {tsInactive,          // δ��
                     tsCreating,          // ���ڴ�������
                     tsClosing,           // ���ڹر�
                     tsOpening,           // ���ڴ�
                     tsOpened,            // �Ѿ���
                     tsEditCond,          // �༭��������
                     tsSeeking,           // ������������
                     tsBrowse,            // �����ǰ����
                     tsInsert};           // ����������

   // ���������
   typedef struct
   {
      KYString       Name;                // ����(Name.Length() <= 255)
      Word           Size;                // ���ߴ�(ftString, ftFixedStr, ftFixedSize ����Ч����Ϊ 0)
      Byte           Type;                // ������(TKYFieldType ����ֵ)
      Byte           Dimension;           // ���ά��[2..256](Dimension != 0)
      bool           CaseSensitive;       // ���ִ�Сд/����ֵ����
                     // CaseSensitive ֵ��Բ�ͬ�����͵ĺ���:
                     // -----------------------------------------------------------
                     //  ������        true              false
                     // -----------------------------------------------------------
                     //  ftString      ���ִ�Сд        �����ִ�Сд
                     //  ftFixedStr    ���ִ�Сд        �����ִ�Сд
                     //  ftFixedSize   ���ִ�Сд        �����ִ�Сд
                     //  ftInteger     �з���4�ֽ�����   �޷���4�ֽ�����
                     //  ftInt64       �з���8�ֽ�����   �޷���8�ֽ�����
                     //  ftByte        �޷���1�ֽ�����   �з���1�ֽ�����
                     // -----------------------------------------------------------
   } TKeyDef, *PKeyDef;

   // ����ֵ����
   typedef TKYFieldType          TType;

   // �ɱ�����
   typedef TKYVarType::TVarData  TVarData;

protected:
   // ����ͷ�ṹ: {<Head> + <KeyDefs> + <TagData>}
   #pragma pack(push, 1)
   typedef struct
   {
      long           Self;                // ������λ��
      long           Size;                // ����ͷ�ߴ�
      long           Root;                // ������λ��
      Word           TagSize;             // �������ݳߴ�
      Byte           KeyCount;            // ������[1..256]
      Byte           ValueType;           // ֵ����(TKYFieldType ����ֵ)
      long           ValueSize;           // ֵ�ߴ�(ftString, ftFixedStr, ftFixedSize ����Ч����Ϊ 0)
      long           TagOffset;           // �������ݵ�ƫ����
      Word           KeyOffset;           // ���ֶζ����б��ƫ����
   } THead, *PHead;

   // ���ͷ��Ϣ
   typedef struct
   {
      Byte           Count;               // ����[1..ά��]
      bool           IsLeaf;              // �Ƿ�ΪҶ�ӽ��
   } TNodeHead, *PNodeHead;

   // �����ظ���ֵͷ��Ϣ
   typedef struct
   {
      long           Prior;               // ��һ��ֵƫ����
      long           Next;                // ��һ��ֵƫ����
   } TValHead, *PValHead;
   #pragma pack(pop)

   // ֵ���͵�����
   enum TValMask    {vmType   = 0x1F,     // ֵ����(TKYFieldType ����ֵ)������
                     vmMulti  = 0x20};    // ������ֵ�� CanDuplicate ����Ϊ true

private:
   // �����Ѳ���
   enum TPartOp     {poNew    = 0,        // �½����
                     poNext   = 1,        // ���������
                     poPrior  = 2};       // �����ǰ����

   // ������
   enum TCondOp     {coNull   = 0x00,     // (��)
                     coEqual  = 0x01,     // ��������
                     coFrom   = 0x02,     // ������
                     coTo     = 0x04};    // ������

   // ��λ����
   typedef struct
   {
      TKYVarType     CondEqual;           // ��ֵ��������
      TKYVarType     CondFrom;            // ��ֵ������
      TKYVarType     CondTo;              // ��ֵ������
      TKYVarType     Key;                 // ��ֵ
      TKeyDef        KeyDef;              // ���������
      long           ItemSize;            // �����ߴ�
      long           NodeSize;            // ���ߴ�(<NodeHead> + [<Item> + <Link>])
      long           RefRoot;             // ��������λ�õ�ƫ����
      Byte           PreBits;             // ǰ׺λ��(ftString, ftFixedStr ����Ч����Ϊ 0)
      Byte           CondOps;             // ��ֵ������λֵ
      bool           IsFalse;             // �����Ƿ񲻳���
      bool           Found;               // �Ƿ�δ��������ֵ
      bool           Bof;                 // �Ƿ��ڵ�һ��λ��
      bool           Eof;                 // �Ƿ���ĩβλ��
   } TKeyPlace, *PKeyPlace;

public:
   TKYIndexTree();
   virtual ~TKYIndexTree();

   // ����
   void*             Data() const         { return FData; }                // default: NULL
   TState            State() const        { return (TState)FState; }       // default: tsInactive
   long              Index() const        { return FHead.Self; }           // default: 0
   Word              IndexNo() const      { return FIndexNo; }             // default: 0
   Word              TagSize() const      { return FHead.TagSize; }        // default: 0
   long              KeyCount() const     { return FPlaces->Count(); }     // default: 0
   long              ValueSize() const    { return FHead.ValueSize; }      // default: 0
   TType             ValueType() const    { return (TType)FHead.ValueType;}// default: ftUnknown
   TKYIndexFile*     File() const         { return FClosed ? NULL : FFile;}// default: NULL
   bool              Closed() const       { return FClosed; }              // default: false
   bool              IsOverflow() const   { return (FState >= tsOpened)    // default: false
                                                   && FFile->IsOverflow(); }
   bool              CanDuplicate() const { return FCanDuplicate; }        // default: false

   // ֵ��������ֵ
   TKYVarType*       Value() const        { return FValue; }
   TKYVarType*       Key(long ANo) const  { return (TKYVarType*)FKeys->Item(ANo); }
   TKYVarType*       Key(const KYString& AName) const;
   long              KeyNo(const KYString& AName) const;
   TKeyDef           KeyDef(long ANo) const;

   // ��������
   void              SetData(void* AData) { FData = AData; }
   bool              SetTagSize(Word ATagSize);
   bool              SetValueDef(TType AType, long ASize, bool ACanDuplicate = false);

   // �༭����������(ע: AName �����ִ�Сд�ҳ��ȳ��� 255 �����Զ��ض�)
   long              AddKeyDef(Byte ADimension, const KYString& AName, TType AType,
                               Word ASize = 0,  bool ACaseSensitive = true);
   bool              DeleteKeyDef(long ANo);
   bool              DeleteKeyDef(const KYString& AName);
   void              ClearKeyDefs();

   // ����/��/�ر�����
   bool              Create(TKYIndexFile* AFile);
   bool              Open(TKYIndexFile* AFile, long AIndex, Word AIndexNo);
   void              Close();

   // ��ʼ/��������
   // 1. ��ʼ����: ���� tsInsert ״̬
   // 2. ��������: �ύ��ȡ������ֵ
   // 3. EndInsert �� ACancel == true ��ʾȡ���༭, �����ύ����ֵ
   // 4. EndInsert �� ACanUpdate == true ��ʾ���м��Ѵ���ʱ����ֵ
   // 5. EndInsert �� ACanUpdate == false ��ʾ���м��Ѵ���ʱ�ύֵʧ��
   bool              BeginInsert();
   bool              EndInsert(bool ACancel = false, bool ACanUpdate = false);

   // ���µ�ǰ�������ڵ�ֵ
   // 1. �� State() != tsBrowse �����ʧ��
   // 2. ���²���ֵʱ, ���ͱ���Ϊ ftFixedSize ����ʧ��
   bool              Update();
   bool              Update(long AOffset, const void* AData, long& ASize);

   // �༭��������: ftFixedSize ����ʹ�� TKYVarType::vtString ֵ
   bool              BeginConds();
   void              CancelConds();
   void              DeleteCond(long AKeyNo);
   bool              AddCondEqual(long AKeyNo, const TKYVarType& ACondition);
   bool              AddCondFrom(long AKeyNo, const TKYVarType& ACondition);
   bool              AddCondTo(long AKeyNo, const TKYVarType& ACondition);

   void              DeleteCond(const KYString& AKey)
                     { DeleteCond(KeyNo(AKey)); }
   bool              AddCondEqual(const KYString& AKey, const TKYVarType& ACondition)
                     { return AddCondEqual(KeyNo(AKey), ACondition); }
   bool              AddCondFrom(const KYString& AKey, const TKYVarType& ACondition)
                     { return AddCondFrom(KeyNo(AKey), ACondition); }
   bool              AddCondTo(const KYString& AKey, const TKYVarType& ACondition)
                     { return AddCondTo(KeyNo(AKey), ACondition); }

   // ����������
   // 1. Search   ���� true ��ʾ�ɹ��ҷ��ص�һ��ֵ�� State Ϊ tsBrowse
   // 2. SeekFirst�������������ĵ�һ��ֵ(ע: State ����Ϊ tsBrowse)
   // 3. SeekPrior����������������һ��ֵ(ע: State ����Ϊ tsBrowse)
   // 4. SeekNext ����������������һ��ֵ(ע: State ����Ϊ tsBrowse)
   // 5. SeekLast �����������������һ��ֵ(ע: State ����Ϊ tsBrowse)
   bool              Search();
   bool              SeekFirst();
   bool              SeekPrior();
   bool              SeekNext();
   bool              SeekLast();

   // ��������������(ע: State ����Ϊ tsBrowse)
   bool              SeekFirst(long AKeyNo);
   bool              SeekPrior(long AKeyNo, bool AIncDuplicate = true);
   bool              SeekNext(long AKeyNo, bool AIncDuplicate = true);
   bool              SeekLast(long AKeyNo);

   // ��������������(ע: State ����Ϊ tsBrowse)
   bool              SeekFirst(const KYString& AKey)
                     { return SeekFirst(KeyNo(AKey)); }
   bool              SeekPrior(const KYString& AKey, bool AIncDuplicate = true)
                     { return SeekPrior(KeyNo(AKey), AIncDuplicate); }
   bool              SeekNext(const KYString& AKey, bool AIncDuplicate = true)
                     { return SeekNext(KeyNo(AKey), AIncDuplicate); }
   bool              SeekLast(const KYString& AKey)
                     { return SeekLast(KeyNo(AKey)); }

   // �������;�����ת��¼
   // (ע: State ����Ϊ tsBrowse, �� ADistance > 0, ������ʵ����ת�ľ���)
   // 1. JumpPrior��ǰ��תָ������, ���ɹ��򷵻�ֵΪ true, �ҵõ���¼ֵ,
   //             ���򷵻�ֵΪ false, �Ҽ�¼ֵ��Ч, ������ʵ����ת�ľ���
   // 2. JumpNext �����תָ������, ���ɹ��򷵻�ֵΪ true, �ҵõ���¼ֵ,
   //             ���򷵻�ֵΪ false, �Ҽ�¼ֵ��Ч, ������ʵ����ת�ľ���
   bool              JumpPrior(long& ADistance);
   bool              JumpNext(long& ADistance);

   // ��ȡ��������: ASize ����Ҫ��ȡ�ĳߴ�, ����ʵ�ʶ�ȡ�ĳߴ�
   bool              ReadTag(long AOffset, void* AData, long& ASize);

   // ���渽������: ASize ����Ҫ����ĳߴ�, ����ʵ�ʱ���ĳߴ�
   bool              WriteTag(long AOffset, const void* AData, long& ASize);

protected:
   // ȡ���ֶ�Ӧ�ļ���(�����״̬)
   long              KeyNoOf(const KYString& AName) const;

   // ������б�
   bool              AssignKeys();
   void              ClearKeyValues();

   // ��Ӽ�����(�����״̬)
   long              DoAddKeyDef(Byte ADimension, const KYString& AName,
                                TType AType, Word ASize,    bool  ACaseSensitive);

   // ��ȡ������
   bool              DoLoadKeyDefs(const char* APos, const char* AEnd, long ACount);
   KYString          DoSaveKeyDefs();

private:
   void              DoInit(bool AIncHead);
   void              DoInit(TKeyPlace& APlace, long ARefRoot, bool AIncCond);
   void              DoInitChild(long AKeyNo,   long ARefRoot,
                                 bool AIncCond, bool AIsLast);

   bool              DoCheck(const THead& AHead);
   void              DoClosed(TKYIndexFile* AFile);

   // �������������Ƿ�Ϸ�
   void              DoInitConds();
   bool              DoAnalyzeCond(TKeyPlace& APlace);
   bool              DoAnalyzeConds();

   // ���Ҽ�ֵ�����ڵĽ��
   bool              DoFind(TKeyPlace& APlace, const TKYVarType& AKey,
                              TKYList& ANodes,          TKYList& AOrderNos,
                                 long& ALink);
   bool              DoFind(TKeyPlace& APlace, const TKYVarType& AKey,
                                 long& ANode,              long& AOrderNo);
   bool              DoFindIn0(TKeyPlace& APlace, const TKYVarType& AFrom,
                        const TKYVarType& ATo,                bool  AIsInc,
                                    long& ANode,              long& AOrderNo);
   bool              DoFindIn1(TKeyPlace& APlace, const TKYVarType& AFrom,
                        const TKYVarType& ATo,                bool  AIsInc,
                                    long& ANode,              long& AOrderNo);
   bool              DoFindTo0(TKeyPlace& APlace,  const TKYVarType& AKey,
                                    bool  AIsInc, long& ANode, long& AOrderNo);
   bool              DoFindTo1(TKeyPlace& APlace,  const TKYVarType& AKey,
                                    bool  AIsInc, long& ANode, long& AOrderNo);
   bool              DoFindFrom0(TKeyPlace& APlace,  const TKYVarType& AKey,
                                      bool  AIsInc, long& ANode, long& AOrderNo);
   bool              DoFindFrom1(TKeyPlace& APlace,  const TKYVarType& AKey,
                                      bool  AIsInc, long& ANode, long& AOrderNo);
   bool              DoGetFirst(TKeyPlace& APlace, long& ANode, long& AOrderNo);
   bool              DoGetLast(TKeyPlace& APlace, long& ANode, long& AOrderNo);

   // ������ط���
   void              DoInitKey(long AKeyNo, const TKYVarType& AKey);
   bool              DoAppendItem(long  AHead, Byte ACount,
                        const KYString& AItem, long ALink);
   bool              DoInsertItem(long  AHead, long AInsNo, long ACount,
                        const KYString& AItem, long ALink);
   bool              DoChangeRoot(long ANode);
   bool              DoChangeNode(long  AHead, long AInsNo, long AIndex,
                        const KYString& AItem, long ALink);
   bool              DoAssignNode(long& ANode, PNodeHead& AHead, long ACount);

   bool              DoMinKey(const KYString& AItem, long AIndex);
   bool              DoNewLeaf(long ALink, long& ANode, KYString* AItem);

   TNodeHead*        DoGetSub(long AParent, long AOrderNo);
   TPartOp           DoGetPartOp(long AParent, long AOrderNo, long AInsNo);

   bool              DoInsertNew(long  AParent, long  AOrderNo, long  ACurrent,
                             KYString& AItem,   long& ALink,    long& AInsNo,
                                 bool& ABreak,  long  AIndex);
   bool              DoMergeNext(long  AParent, long  AOrderNo, long  ACurrent,
                             KYString& AItem,   long& ALink,    long& AInsNo,
                                 bool& ABreak,  long  AIndex);
   bool              DoMergePrior(long  AParent, long  AOrderNo, long  ACurrent,
                              KYString& AItem,   long& ALink,    long& AInsNo,
                                  bool& ABreak,  long  AIndex);
   bool              DoMergePriorEx(long  AParent, long  AOrderNo, long  ACurrent,
                                KYString& AItem,   long& ALink,    long& AInsNo,
                               TNodeHead* APrior,  long  APriorPos);

   bool              DoInsertKey(long ALink);
   bool              DoInsertRoot(long  AHead, long AInsNo,
                        const KYString& AItem, long ALink);

   // �������ֵ
   bool              DoInsert(long AKeyNo, const TKYVarType& AKey, long& ALink);
   bool              DoInsertEx(long AKeyNo, const TKYVarType& AKey, bool ACanUpdate);

   // ִ������
   bool              DoSeekFirst();
   bool              DoSeekPrior();
   bool              DoSeekNext();
   bool              DoSeekLast();

   // ִ������(ע: APlace.RefRoot ����Ϸ�)
   bool              DoSeekFirst(TKeyPlace& APlace, long& ALink);
   bool              DoSeekPrior(TKeyPlace& APlace, long& ALink);
   bool              DoSeekNext(TKeyPlace& APlace, long& ALink);
   bool              DoSeekLast(TKeyPlace& APlace, long& ALink);
   bool              DoSeekCurr(TKeyPlace& APlace, long& ARefLink);

   // ѭ����תֵ(ע: FCanDuplicate == true)
   bool              DoJumpPriorValue(long& ANo, long ACount);
   bool              DoJumpNextValue(long& ANo, long ACount);

   // TKYList �� OnDeletion �¼�����
   void              DoKeyDeletion(TKYVarType* AItem);
   void              DoPlaceDeletion(TKeyPlace* AItem);

private:
   // ��ͬ�������͵ıȽϷ���
   // ������ֵ result == 0 ���ʾ AItem ���� ACond
   // ������ֵ result > 0  ���ʾ AItem ���� ACond
   // ������ֵ result < 0  ���ʾ AItem С�� ACond
   long              DoCompare_1(TKeyPlace& APlace, void* AItem, TVarData* ACond);
   long              DoCompare_2(TKeyPlace& APlace, void* AItem, TVarData* ACond);
   long              DoCompare_3(TKeyPlace& APlace, void* AItem, TVarData* ACond);
   long              DoCompare_4(TKeyPlace& APlace, void* AItem, TVarData* ACond);
   long              DoCompare_5(TKeyPlace& APlace, void* AItem, TVarData* ACond);
   long              DoCompare_6(TKeyPlace& APlace, void* AItem, TVarData* ACond);
   long              DoCompare_7(TKeyPlace& APlace, void* AItem, TVarData* ACond);
   long              DoCompare_8(TKeyPlace& APlace, void* AItem, TVarData* ACond);
   long              DoCompare_9(TKeyPlace& APlace, void* AItem, TVarData* ACond);
   long              DoCompare_10(TKeyPlace& APlace, void* AItem, TVarData* ACond);
   long              DoCompare_11(TKeyPlace& APlace, void* AItem, TVarData* ACond);
   long              DoCompare_12(TKeyPlace& APlace, void* AItem, TVarData* ACond);

   // ��ȡ��ͬ���ͼ�ֵ�ķ���
   void              DoGetKey_1(TKeyPlace& APlace, void* AItem);
   void              DoGetKey_2(TKeyPlace& APlace, void* AItem);
   void              DoGetKey_3(TKeyPlace& APlace, void* AItem);
   void              DoGetKey_12(TKeyPlace& APlace, void* AItem);

   // ���ò�ͬ���ͼ�ֵ�ķ���
   bool              DoSetKey_1(TKeyPlace& APlace, void* AItem);
   bool              DoSetKey_2(TKeyPlace& APlace, void* AItem);
   bool              DoSetKey_3(TKeyPlace& APlace, void* AItem);
   bool              DoSetKey_12(TKeyPlace& APlace, void* AItem);

   // ��ȡ��ͬ����ֵ�ķ���
   void              DoGetValue_1(long ALink);
   void              DoGetValue_3(long ALink);
   void              DoGetValue_5(long ALink);
   void              DoGetValue_12(long ALink);

   void              DoGetValue1_1(long ALink);
   void              DoGetValue1_3(long ALink);
   void              DoGetValue1_12(long ALink);

   // ���ò�ͬ����ֵ�ķ���
   bool              DoSetValue_1(long ARefLink);
   bool              DoSetValue_2(long ARefLink);
   bool              DoSetValue_3(long ARefLink);
   bool              DoSetValue_5(long ARefLink);
   bool              DoSetValue_12(long ARefLink);

   bool              DoSetValue1_1(long ALink);
   bool              DoSetValue1_2(long ALink);
   bool              DoSetValue1_3(long ALink);
   bool              DoSetValue1_12(long ALink);

   // ���䲻ͬ����ֵ�ķ���
   bool              DoNewValue_1(long& ALink);
   bool              DoNewValue_2(long& ALink);
   bool              DoNewValue_3(long& ALink);
   bool              DoNewValue_5(long& ALink);
   bool              DoNewValue_12(long& ALink);

   bool              DoNewValue1_1(long& ALink);
   bool              DoNewValue1_2(long& ALink);
   bool              DoNewValue1_3(long& ALink);
   bool              DoNewValue1_12(long& ALink);

private:
   void*             FData;               // �Զ�������
   TKYIndexFile*     FFile;               // �����ļ�����
   TKYList*          FKeys;               // ������ֵ�б�
   TKYList*          FPlaces;             // ������λ���б�

   TKYVarType*       FVar;                // ���� DoCompare_x ʹ�õı���
   TKYVarType*       FValue;              // �������Ӧֵ
   long              FValHead;            // ֵ��ͷ����
   long              FValLink;            // ֵ�ĵ�ǰ����

   THead             FHead;               // ����ͷ��Ϣ
   Byte              FState;              // ������״̬
   bool              FClosed;             // �����ļ��ѹر�
   Word              FIndexNo;            // ���������
   bool              FCanDuplicate;       // �Ƿ������ظ�, Ĭ��ֵΪ false

   // �����ֵ�������
   Byte              FDimension;          // ��ǰ��ά��
   TKeyPlace*        FPlace;              // ��ǰ��λ��
   TKYList*          FNodes;              // ��·�����
   TKYList*          FOrderNos;           // ����е����
   long              FItemSize;           // ��ǰ������ߴ�

public:
   // У��ֵ
   static void       _VerifyValue(TKYVarType& AVar, TType AType, long ASize);

public:
   // ֵ�ȽϺ���ָ��(���ͱ�����ͬ)
   typedef long (*TCompareVar)(const TKYVarType& AVar1, const TKYVarType& AVar2,
                                           bool  ACaseSensitive);

   // ��ͬ���͵ıȽϺ�������ָ��
   static const TCompareVar         _Type_CompVar[13];

   // ����ֵ���Ͷ�Ӧ�Ŀɱ���������
   // ע: ���� ftString, ftFixedStr, ftFixedSize ��Ӧ TKYVarType::vtString ����
   static const TKYVarType::TType   _Type_Var[13];

private:
   // ����ָ������
   typedef long (TKYIndexTree::*TDoCompare)(TKeyPlace& APlace,
                                void* AItem, TVarData* ACond);
   typedef void (TKYIndexTree::*TDoGetKey)(TKeyPlace& APlace, void* AItem);
   typedef bool (TKYIndexTree::*TDoSetKey)(TKeyPlace& APlace, void* AItem);
   typedef void (TKYIndexTree::*TDoGetValue)(long ALink);
   typedef bool (TKYIndexTree::*TDoSetValue)(long ARefLink);
   typedef bool (TKYIndexTree::*TDoNewValue)(long& ALink);
   typedef bool (TKYIndexTree::*TDoPart)(long  AParent, long  AOrderNo, long  ACurrent,
                                     KYString& AItem,   long& ALink,    long& AInsNo,
                                         bool& ABreak,  long  AIndex);

   // ��ͬ�������͵ķ���ָ��
   static const TDoCompare          _Type_Compare[13];
   static const TDoGetKey           _Type_GetKey[13];
   static const TDoSetKey           _Type_SetKey[13];
   static const TDoGetValue         _Type_GetValue[2][13];
   static const TDoSetValue         _Type_SetValue[2][13];
   static const TDoNewValue         _Type_NewValue[2][13];
   static const TDoPart             _Op_DoPart[3];

   // ���������弯����
   static TKYFieldDefs              _Defs_KeyInfo;
   static long                      _MinSize_KeyInfo;

   // TKYIndexTree �ľ�̬��Ա��ʼ����
   static class TInitialization
   {
   public:
      TInitialization();
   } _Initialization;

   friend class TInitialization;
};

}

#endif
