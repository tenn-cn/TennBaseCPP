// =======================================
// Unit   : ���ļ��� (KYTable.h)
// Version: 3.0.0.0 (build 2011.03.09)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYTable_H_
#define _KYTable_H_

#include "KYFiles.h"
#include "KYFields.h"

// KYLib 2.0 ��ʼʹ�� KYLib �����ռ�
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TKYTable - ���ļ��� */

class TKYTable
{
public:
   // ���Ҽ�¼��ѡ��(λ����)
   enum TLocateOption  {loCaseInsensitive = 0x01,  // �����ִ�Сд
                        loPartialKey      = 0x02}; // ����ƥ��

   // ���ݼ���״̬
   enum TState         {kdsInactive,               // δ��״̬
                        kdsOpening,                // ���ڴ򿪱�
                        kdsCreating,               // ���ڴ�����
                        kdsBrowse,                 // �����¼
                        kdsEdit,                   // �༭��ǰ��¼
                        kdsInsert};                // �����¼�¼

   // �汾��Ϣ
   #pragma pack(push, 1)
   typedef struct
   {
      Byte              Major;                     // ���汾
      Byte              Minor;                     // С�汾
      Byte              Release;                   // �����汾
      Byte              Build;                     // ����С�汾
   } TVersion, *PVersion;
   #pragma pack(pop)

protected:
   // �� TableOptions ����ѡ��
   enum TTableOption   {toMoveOnlyNext = 0x0001,   // �ƶ�ָ��ֻ�� Next
                        toCannotDelete = 0x0002,   // ��¼������ɾ��
                        toFieldCanNull = 0x0004,   // �ֶ�����Ϊ��
                        toNoFieldDefs  = 0x0008};  // ���ֶζ��弯

   // ��չͷ��Ϣ��дģʽ
   enum TInfoExMode    {iwmUpdate,
                        iwmInsert,
                        iwmDelete,
                        iwmRecall};

   // ��¼ͷ��Ϣ����
   enum TRecHeadKind   {rhkVer1,
                        rhkFixed,
                        rhkByte,
                        rhkByte0,
                        rhkWord,
                        rhkWord0,
                        rhkInt,
                        rhkInt0};

   // ��ṹ   = <HeadInfo> + <HeadInfoEx> + <OwnerInfo> + <FieldInfo> + <Record>
   // ��¼�ṹ = [RecordHead] + <RecordValue>
   #pragma pack(push, 1)

   // ��ͷ������Ϣ
   typedef struct
   {
      long              Size;
      TVersion          Version;
      long              FieldSize;
      long              FieldCount;
      long              RecordHead;
      long              RecordSize;
      long              HeadOrigin;
      long              OwnerOrigin;
      long              FieldOrigin;
      long              RecordOrigin;
      // ^^^^^^ Add on 2006.02.18 by Kyee ^^^^^^
      // Add: Table version 2.0.0.0 ���Ժ�汾
      Longword          TableOptions;
      Byte              NullMapOffset;
   } THeadInfo, *PHeadInfo;

   // ��¼ͷ��Ϣ(���� rhkVer1, rhkInt ����)
   typedef struct
   {
      long              PriorSize;
      long              FullSize;
      bool              Deleted;
   } TRecordHead, *PRecordHead;

   #pragma pack(pop)

public:
   TKYTable();
   virtual ~TKYTable();

   // ----- ����������
   // ��Ĵ�������ʱ��
   TDateTime            CreateDate() const   { return FHeadInfoEx->Fields(1)->AsDateTime(); }

   // �������޸�����ʱ��
   TDateTime            ModifyDate() const   { return FHeadInfoEx->Fields(2)->AsDateTime(); }

   // �Զ�������, ��󳤶� 32, Ĭ��ֵΪ: ""
   KYString             OwnerName() const    { return FOwnerInfo->Fields(2)->AsString(); }

   // �Զ������ݳߴ�, ���ݷ��ڱ�� <HeadInfoEx> �� <FieldInfo> ֮��, Ĭ��ֵΪ: 0
   long                 OwnerSize() const    { return FOwnerInfo->Fields(3)->AsInteger(); }

   // �Զ���汾��Ϣ, Ĭ��ֵΪ: {0, 0, 0, 0}
   TVersion             OwnerVersion() const;

   // ��İ汾��Ϣ, Ĭ��ֵΪ: {2, 1, 0, 0}
   TVersion             TableVersion() const { return FHeadInfo.Version; }

   // ���¼֮���������Ƿ�ֻ����������, Ĭ��ֵΪ: false
   bool                 MoveOnlyNext() const { return IsOption(FHeadInfo.TableOptions, toMoveOnlyNext); }

   // ���¼�Ƿ�����ɾ��, Ĭ��ֵΪ: false
   bool                 CannotDelete() const { return IsOption(FHeadInfo.TableOptions, toCannotDelete); }

   // ���¼���ֶ�ֵ�Ƿ�����մ��, Ĭ��ֵΪ: false
   // ��������Ϊ true, ��ĳ�ֶ�δ��ֵ������ʱ��ռ��¼�ռ�, ��ֻռһ���յı�־(1 bit)
   bool                 FieldCanNull() const { return IsOption(FHeadInfo.TableOptions, toFieldCanNull); }

   // ���ļ��Ƿ񲻴���ֶζ�����Ϣ, Ĭ��ֵΪ: false
   // ��������Ϊ true, ���ڴ򿪱�֮ǰ����ҪԤ�ȶ��� FieldDefs ����, ��ż�¼���ֶμ���Ϣ
   bool                 NoFieldDefs() const  { return IsOption(FHeadInfo.TableOptions, toNoFieldDefs); }

   // ----- ������
   // ������ݿ���, �����ļ����ڵ�·�����ϲ�·��, Ĭ��ֵΪ: ""
   KYString             DatabaseName() const { return FDatabaseName; }

   // ����, ������ļ������ȫ·�����ļ���, Ĭ��ֵΪ: ""
   // �������� DatabaseName, ����ļ���Ϊ: DatabaseName() + TableName()
   KYString             TableName() const    { return FTableName; }

   // ���Ƿ��ռ��, Ĭ��ֵΪ: false
   bool                 Exclusive() const    { return FExclusive; }

   // ���Ƿ�ֻ����, Ĭ��ֵΪ: false
   bool                 ReadOnly() const     { return FReadOnly; }

   // �жϱ��ļ��Ƿ����
   bool                 Existed() const      { return FileExists(GetFileName(FDatabaseName, FTableName)); }

   // ����������¼ʱ�Ƿ���䵽��ɾ����¼�Ŀռ�, Ĭ��ֵΪ: false
   // �����ô�����Ϊ true, ���½���¼ʱ��Ҫ������ɾ����¼��λ��, ���ܿ��ܻ��úܵ�
   bool                 FillDeleted() const  { return FFillDeleted; }

   // �Ƿ�������ɾ���ļ�¼, Ĭ��ֵΪ: false
   // �����ô�����Ϊ true, ���ƶ���¼ָ��ʱ����������ɾ����¼, Ҳ����˵, �ƶ���
   // ¼ָ��ʱ���ж��Ƿ���ɾ����¼, �� Deleted() ���Ա�ʾ��¼�Ƿ���ɾ��
   bool                 ShowDeleted() const  { return FShowDeleted; }

   // ----- �ֶ�����
   // ���¼���ֶθ���
   long                 FieldCount() const   { return FHeadInfo.FieldCount; }

   // ���¼���ֶζ��弯��Ϣ
   TKYFieldDefs*        FieldDefs() const    { return FFieldDefs; }

   // �����ֶκ�ȡ���¼���ֶ�ֵ, �ֶκ�ȡֵ��Χ: [0..FieldCount()-1]
   TKYField*            Fields(long AFieldNo) const
                        { return Active() ? FFields->Fields(AFieldNo) : NULL; }

   // �����ֶ���ȡ��¼���ֶοɱ�����ֵ
   TKYVarType::TVarData* FieldValues(const KYString& AFieldName) const;

   // ----- ��¼����
   // ���Ƿ��Ѵ�, Ĭ��ֵΪ: false
   bool                 Active() const       { return (FState >= kdsBrowse); }

   // ���, ����, ���, �༭������¼ʱ�Ĵ�����Ϣ, Ĭ��ֵΪ: ""
   KYString             Error() const        { return FError; }

   // ��ǰ״̬, Ĭ��ֵΪ: kdsInactive
   TState               State() const        { return FState; }

   // �жϼ�¼ָ���Ƿ��ڿ�ʼλ��, Ĭ��ֵΪ: true
   bool                 Bof() const          { return FBOF; }

   // �жϼ�¼ָ���Ƿ���ĩβ, Ĭ��ֵΪ: true
   // ��������Ϊ true ���ʾ�޷��������ƶ���¼ָ��, ���δ��
   bool                 Eof() const          { return FEOF; }

   // �ж� Locate �����Ƿ���ҵ���¼, Ĭ��ֵΪ: false
   bool                 Found() const        { return FFound; }

   // ��¼��ʶ��, Ϊ���¼��������, �ڱ��ļ���Ψһ, ��ʶ�ŷ�Χ: [1..HaveRecCount()]
   long                 RecID() const        { return FRecID; }

   // ��¼��, Ϊ���¼���߼����, �� ShowDeleted() �����й�, ��Χ: [1..ShowRecCount()]
   // �� ShowDeleted() ����Ϊ true, ���� RecNo() == RecID(), ����Ͳ�һ����ͬ
   long                 RecNo() const        { return FRecNo; }

   // ��ǰ��¼�Ƿ���ɾ��
   bool                 Deleted() const      { return Active() && FRecordHead.Deleted; }

   // ���ļ��д��ڵļ�¼��, ������ɾ����¼
   long                 HaveRecCount() const { return FHeadInfoEx->Fields(6)->AsInteger(); }

   // ���ļ�������ļ�¼��,
   // �� ShowDeleted() ����Ϊ true, ���� ShowDeleted() == HaveRecCount(),
   // ���� ShowRecCount() == RecordCount()
   long                 ShowRecCount() const;

   // ��ǰ��¼�����ݳߴ�
   long                 RecCurrSize() const  { return Active() ? FFields->RecordSize() : 0; }

   // ��ǰ��¼���ڿռ����������¼�ߴ�
   long                 RecFullSize() const;

   // δɾ���ļ�¼��
   long                 RecordCount() const  { return FHeadInfoEx->Fields(5)->AsInteger(); }

   // ��¼�����ߴ�
   long                 RecordSize() const   { return FHeadInfo.RecordSize; }

   // �����Զ�������
   bool                 SetOwnerName(const KYString& AName);

   // �����Զ������ݳߴ�, ������ CreateTable ֮ǰ����, ������Ч
   bool                 SetOwnerSize(long ASize);

   // �����Զ���汾��Ϣ
   bool                 SetOwnerVersion(const TVersion& AVersion);

   // ���ñ��¼֮���������Ƿ�ֻ����������, ������ CreateTable ֮ǰ����, ������Ч
   bool                 SetMoveOnlyNext(bool AEnabled);

   // ���ñ��¼�Ƿ�����ɾ��, ������ CreateTable ֮ǰ����, ������Ч
   bool                 SetCannotDelete(bool AEnabled);

   // ���ñ��¼���ֶ�ֵ�Ƿ�����մ��, ������ CreateTable ֮ǰ����, ������Ч
   bool                 SetFieldCanNull(bool AEnabled);

   // ���ñ��ļ��Ƿ񲻴���ֶζ�����Ϣ, ������ CreateTable ֮ǰ����, ������Ч
   bool                 SetNoFieldDefs(bool AEnabled);

   // ���ñ��ļ����ڵ�·�����ϲ�·��, ������ Open ֮ǰ����, ������Ч
   bool                 SetDatabaseName(const KYString& AName);

   // ���ñ���ļ������ȫ·�����ļ���, ������ Open ֮ǰ����, ������Ч
   bool                 SetTableName(const KYString& AName);

   // ���ñ��Ƿ��ռ��, ������ Open ֮ǰ����, ������Ч
   bool                 SetExclusive(bool AEnabled);

   // ���ñ��Ƿ�ֻ����, ������ Open ֮ǰ����, ������Ч
   bool                 SetReadOnly(bool AEnabled);

   // ���ñ���������¼ʱ�Ƿ���䵽��ɾ����¼�Ŀռ�
   bool                 SetFillDeleted(bool AEnabled);

   // �����Ƿ�������ɾ���ļ�¼, ������ Open ֮ǰ����, ������Ч
   bool                 SetShowDeleted(bool AEnabled);

   // �����ֶ�ֵ, ������ Active() Ϊ true ֮������, ������Ч
   bool                 SetFieldValues(const KYString& AFieldName,
                           const TKYVarType::TVarData* AValue);

   // ���ô�/�رձ�
   bool                 SetActive(bool AValue);

   // ���ü�¼��ʶ��, ������ Active() Ϊ true ֮������, ������Ч
   // ����ʶ��ָ�������ɾ����¼�� ShowDeleted() ����Ϊ false, ���¼����Ч
   bool                 SetRecID(long ARecID);

   // ���ü�¼��, ������ Active() Ϊ true ֮������, ������Ч
   bool                 SetRecNo(long ARecNo);

   // �������ļ�, ������ʧ�ܿɴ� Error() ����ȡ������Ϣ
   bool                 CreateTable(bool AIsVer1 = false);

   // ɾ�����ļ�, ��ʧ�ܿɴ� Error() ����ȡ������Ϣ
   bool                 DeleteTable();

   // ��ձ�����м�¼, ��ʧ�ܿɴ� Error() ����ȡ������Ϣ
   bool                 EmptyTable();

   // ��ձ��������ɾ����¼, ��ʧ�ܿɴ� Error() ����ȡ������Ϣ
   bool                 PackTable();

   // ���ļ�������, ��ʧ�ܿɴ� Error() ����ȡ������Ϣ
   bool                 RenameTable(const KYString& ANewName);

   // ��ȡ���ļ�������, ��ʧ�ܿɴ� Error() ����ȡ������Ϣ
   bool                 ReadTableAttrib();

   // �򿪱�, ��ʧ�ܿɴ� Error() ����ȡ������Ϣ
   bool                 Open();

   // �رձ�
   void                 Close();

   // �жϱ��Ƿ�Ϊ��, ����¼���Ƿ�Ϊ 0
   bool                 IsEmpty();

   // ˢ�±�
   // �� AFixedCursor Ϊ false ʱ, ��ˢ�º��¼�Զ���λ����һ����¼
   // ���� AFixedCursor Ϊ true ʱ, ���ı� RecID �� RecNo ֵ, �� RecNo ��һ��׼ȷ
   void                 Refresh(bool AFixedCursor = false);

   // ���µ�ǰ��¼
   void                 UpdateRecord();

   // ɾ����ǰ��¼
   bool                 Delete();

   // �ָ���ǰ��ɾ����¼
   bool                 RecallRecord();

   // ׷�Ӽ�¼
   bool                 Append()             { return Insert(); }

   // ȡ���༭�ɲ����¼
   bool                 Cancel();

   // �༭��ǰ��¼
   bool                 Edit();

   // �����¼
   bool                 Insert();

   // �ύ�ѱ༭�����ļ�¼
   bool                 Post();

   // �ƶ���¼ָ��
   long                 MoveBy(long ADistance);

   // �Ƶ���һ����¼
   bool                 First();

   // �Ƶ����һ����¼
   bool                 Last();

   // �Ƶ���һ����¼
   bool                 Next();

   // �Ƶ���һ����¼
   bool                 Prior();

   // �����ֶ���ȡ�ֶ�ֵ
   TKYField*            FindField(const KYString& AFieldName) const
                        { return Active() ? FFields->FindField(AFieldName) : NULL; }
   TKYField*            FieldByName(const KYString& AFieldName) const
                        { return Active() ? FFields->FindField(AFieldName) : NULL; }

   // �����������Ҽ�¼, ��ָ�����ҵ��ĵ�һ����¼, ��δ�ҵ��򲻸ı��¼ָ��
   // ��Ҫƥ�����ֶ�ֵʱ, AFields ���ֶ���֮��ʹ�� ';' �ָ�, ���ֶ�ֵ��˳���
   // ���� AFields �ֶ�����˳����ͬ, ����������Ҳ������ͬ, ������ܻ�����쳣
   bool                 Locate(const KYString& AFields, Byte AOptions, ...);

   // �������Ҽ�¼�ķ���, �൱�� Locate �����ķֲ�����
   // 1. BeginKeys      ��ʼ�������ֶ�����, ���� FIsAdding Ϊ true
   // 2. AddKey         �����Ҫ���ҵ��ֶμ��ֶ�ֵ, ���Զ�ε���
   // 3. FindKeys       ִ����������, ���ɹ���ָ�����ҵ��ĵ�һ����¼,
   //                   ��δ�ҵ��򲻸ı��¼ָ��, �������ӵĲ����ֶ�ֵ
   bool                 BeginKeys();
   bool                 AddKey(const KYString& AFieldName, const TKYVarType& AValue);
   bool                 FindKeys(Byte AOptions);

   // ������������ĵ�һ����¼
   bool                 FindFirst();

   // ����������������һ����¼
   bool                 FindLast();

   // ���������������һ����¼
   bool                 FindNext();

   // ���������������һ����¼
   bool                 FindPrior();

   // ��ȡ�Զ�������ݲ�����ʵ�ʶ�ȡ�ߴ�, ������ Active() Ϊ true ֮�����, ����ʧ�ܲ����� 0
   long                 GetOwnerData(void* AData, long AOffset, long ACount) const;

   // д�Զ�������ݲ�����ʵ��д��ߴ�, ������ Active() Ϊ true ֮�����, ����ʧ�ܲ����� 0
   long                 SetOwnerData(const void* AData, long AOffset, long ACount);

protected:
   void                 Lock() const         { FLock->Enter(); }
   void                 Unlock() const       { FLock->Leave(); }

   bool                 LockRead(Longword AOffset, Longword ACount) const;
   bool                 UnlockRead(Longword AOffset, Longword ACount) const;

   bool                 LockWrite(Longword AOffset, Longword ACount) const;
   bool                 UnlockWrite(Longword AOffset, Longword ACount) const;

   // ��¼ָ�����
   bool                 RecOfFirst();
   bool                 RecOfLast();
   long                 RecOfGoto(long APos, bool AIsID);
   long                 RecOfNext(long ADistance);
   long                 RecOfPrior(long ADistance);
   long                 RecOfNextID(long ADistance);
   long                 RecOfPriorID(long ADistance);
   long                 RecOfFixedID(long ADistance, bool AIsNext);

   // ��/д��¼ͷ��Ϣ
   bool                 ReadRecHead(TRecordHead& ARecHead, long AOffset,
                                            bool AMoveOnlyNext,
                                            bool ACannotDelete,
                                            bool AHasLocked = false);
   bool                 WriteRecHead(const TRecordHead& ARecHead, long AOffset,
                                                   bool AHasLocked = false);

   // ��/д��չͷ��Ϣ
   bool                 ReadHeadInfoEx();
   bool                 WriteHeadInfoEx(TInfoExMode AMode, long ALastRecSize = 0);

   // ��/д�Զ�����Ϣ
   bool                 ReadOwnerInfo();
   bool                 WriteOwnerInfo();

   // ��¼�༭���ڲ�����
   bool                 CheckBrowseMode(const char* AErrOp);
   bool                 InternalCancel();
   bool                 InternalEdit();
   bool                 InternalInsert();

   bool                 PostOfDeleteFlag(long AOffset);
   bool                 PostOfRecallFlag(long AOffset);
   bool                 PostOfFindDeleted(long& AOffset, long& ARecID,
                                          long& ARecNo,  long  ARecSize,
                                          bool  AOverflow, TRecordHead& AHead);
   bool                 PostOfUpdateRec(const char* ABuffer,   long AOffset,
                                              long  AFullSize, long ARecSize);
   bool                 PostOfInsertRec(const char* ABuffer, long ARecSize);

   bool                 InternalPostEdit(const char* ABuffer, long ARecSize);
   bool                 InternalPostInsert(const char* ABuffer, long ARecSize,
                                                  bool AOverflow);
   bool                 InternalPost();

   bool                 InternalDelete();
   bool                 InternalRecallRecord();

   bool                 InternalRefresh(bool AFixedCursor);
   bool                 InternalUpdateRecord();

   // ���Ҽ�¼�ĺ���
   long                 AnalyzeFields(const KYString& AFields);
   bool                 CompareRecord();
   bool                 FindRecord(bool ARestart, bool AGoForward);

private:
   void                 InitialInfo();
   void                 ReleaseInfo();

   void                 NullRecord();
   bool                 ReadRecord(const TRecordHead& AHead, long AOffset, long ARecNo);
   bool                 ReadDataOfFile(long AOffset, long ACount, bool ANeedSeek);

   void                 DoValueDeletion(TKYField* AItem);

private:
   TKYCritSect*         FLock;
   KYString             FError;

   // ͷ��Ϣ
   THeadInfo            FHeadInfo;
   TKYFields*           FHeadInfoEx;
   TKYFields*           FOwnerInfo;
   TKYFields*           FFields;
   TKYFieldDefs*        FFieldDefs;

   // ��¼ͷ��Ϣ
   TKYFields*           FRecHeadFields;
   KYString             FRecHeadBuffer;
   long                 FRecMinSize;
   TRecordHead          FRecordHead;
   TRecHeadKind         FRecHeadKind;

   // ��򿪷�ʽ
   KYString             FDatabaseName;
   KYString             FTableName;
   bool                 FExclusive;
   bool                 FReadOnly;
   bool                 FFillDeleted;
   bool                 FShowDeleted;

   // ���ļ���������
   TFile*               FFile;
   long                 FCursor;
   KYString             FBuffer;

   // ״̬
   TState               FState;
   bool                 FBOF;
   bool                 FEOF;
   long                 FRecID;
   long                 FRecNo;

   // ����
   bool                 FFound;
   bool                 FIsAdding;
   Byte                 FKeyOptions;
   TKYList*             FKeyFields;
   TKYList*             FKeyValues;

public:
   // ѡ��ֵ������ѡ��
   static bool          IsOption(Longword AValue, Longword AOption)
                        { return (AValue & AOption) == AOption; }
   static void          SetOption(Longword& AValue, Longword AOption, bool AEnabled)
                        { AEnabled ? (AValue |= AOption) : (AValue &= ~AOption); }

   // �Ƿ���ͬ�ֶ�ֵ
   static bool          SameFieldValue(const TKYField& AField,
                                       const TKYField& AValue, Byte AOptions);

   // �ϳɰ汾ֵ
   static TVersion      Version(Byte AMajor, Byte AMinor, Byte ARelease, Byte ABuild);

   // Version <-> String
   static KYString      VerToString(const TVersion& AVersion);
   static TVersion      StringToVer(const KYString& AStr);

   // �õ�ƫ����
   static Byte          GetNullMapOffset(TRecHeadKind AKind, bool ACannotDelete,
                                                             bool ACanNull);

   // �õ���¼ͷ�ֶμ�
   static void          GetRecordHead(TKYFields* AHead, TRecHeadKind AKind,
                                            bool ACannotDelete);

   // �õ���¼ͷ�ߴ�
   static long          GetRecHeadSize(TRecHeadKind AKind, long AFieldCount,
                                               bool ACannotDelete, bool ACanNull);

   // �õ���¼ͷ����
   static TRecHeadKind  GetRecHeadKind(const TVersion& AVersion, bool AFixed,
                                                  bool AMoveOnlyNext, long ARecordSize);

   // ����¼ͷ�ߴ��Ƿ�Ϸ�
   static bool          CheckRecHeadSize(const THeadInfo& AHeadInfo, TRecHeadKind& AKind);

   // ���ͷ��Ϣ�Ƿ�Ϸ�
   static bool          CheckHeadInfo(const THeadInfo& AHeadInfo, long AFileSize);

   // �����չͷ��Ϣ�Ƿ�Ϸ�
   static bool          CheckHeadInfoEx(const THeadInfo& AHeadInfo,
                                        const TKYFields* AHeadInfoEx, long AFileSize);

   // ����Զ�����Ϣ�Ƿ�Ϸ�
   static bool          CheckOwnerInfo(const THeadInfo& AHeadInfo,
                                       const TKYFields* AOwnerInfo);

   // ����ͷ�����Ϣ
   static bool          ReadTableHead(const TFile* AFile, TRecHeadKind& AKind,
                                        THeadInfo& AHeadInfo,
                                        TKYFields* AHeadInfoEx,
                                        TKYFields* AOwnerInfo);

   // ���ֶζ��弯
   static bool          ReadFieldDefs(const TFile* AFile,
                                  const THeadInfo& AHeadInfo,
                                     TKYFieldDefs* AFieldDefs);

   // У����ͷ��Ϣ
   static void          VerifyTableHead(THeadInfo& AHeadInfo,
                                        TKYFields* AHeadInfoEx,
                                        TKYFields* AOwnerInfo,
                                     TKYFieldDefs* AFieldDefs, bool AIsVer1);

   // д��ͷ��Ϣ
   static void          WriteTableHead(const TFile* AFile,
                                   const THeadInfo& AHeadInfo,
                                   const TKYFields* AHeadInfoEx,
                                   const TKYFields* AOwnerInfo,
                                const TKYFieldDefs* AFieldDefs);

   // �Ƶ��Ѿ�ɾ���ļ�¼
   static void          RemoveRecordOfDeleted(const TFile* AFile,
                                              TRecHeadKind AKind,
                                          const THeadInfo& AHeadInfo,
                                                TKYFields* AHeadInfoEx);

protected:
   // ������<->TVersion
   static bool          Buffer2Version(const void* ABuffer, TVersion& AVersion);
   static long          Version2Buffer(const TVersion& AVersion, void* ABuffer);

   // ������<->THeadInfo
   static bool          Buffer2HeadInfo(const void* ABuffer, THeadInfo& AHeadInfo);
   static long          HeadInfo2Buffer(const THeadInfo& AHeadInfo, void* ABuffer);

private:
   // �ֶαȽϺ���ָ��
   typedef bool          (*TSameValue)(const TKYField& AField,
                                       const TKYField& AValue, Byte AOptions);

   // ��̬�ֶζ��弯����
   static TKYFieldDefs  Defs_Version;        // TVersion
   static TKYFieldDefs  Defs_HeadInfo;       // THeadInfo
   static TKYFieldDefs  Defs_HeadInfoEx;     // THeadInfoEx
   static TKYFieldDefs  Defs_OwnerInfo;      // TOwnerInfo
   static TKYFieldDefs  Defs_FieldInfo;      // TFieldInfo
   static TKYFieldDefs  Defs_RecordHead;     // TRecordHead
   static TKYFieldDefs  Defs_RH_Fixed;       // rhkFixed
   static TKYFieldDefs  Defs_RH_Byte;        // rhkByte
   static TKYFieldDefs  Defs_RH_Word;        // rhkWord
   static TKYFieldDefs  Defs_RH_Byte0;       // rhkByte0
   static TKYFieldDefs  Defs_RH_Word0;       // rhkWord0
   static TKYFieldDefs  Defs_RH_Int0;        // rhkInt0

   // ���ּ�¼ͷ
   static TKYFieldDefs* HeadKind_Defs[8];
   static const Byte    HeadKind_Size[2][8];

   // ���汾��Ӧ�Ľṹ�ߴ�
   static const Word    StructOfVer_Size[2][4];

   // �ֶ���
   static const KYString Field_Name;         // "Name"
   static const KYString Field_Type;         // "Type"
   static const KYString Field_Size;         // "Size"
   static const KYString Field_PriorSize;    // "PriorSize"
   static const KYString Field_FullSize;     // "FullSize"
   static const KYString Field_Deleted;      // "Deleted"

   // ���汾ֵ
   static const TVersion Version_Current;    // ��ǰ�汾
   static const TVersion Version_Ver1;       // �汾 1.0
   static const TVersion Version_NULL;       // �հ汾

   // �򿪸�ʽ
   static const Longword OpenMode_ReadOnly[2];
   static const Longword OpenMode_Exclusive[2];

   // �Ƚ���ֵͬ�ĺ���ָ������
   static const TSameValue SameValues[13];

   // TKYTable �ľ�̬��Ա��ʼ����
   static class TInitialization
   {
   public:
      TInitialization();
      ~TInitialization();
   } Inner_Initialization;

   friend class TInitialization;
};

}

#endif