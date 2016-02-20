#pragma once
#include "Obj_Item.h"
#include "GameDefine2.h"
#include "GameDefine_Item.h"
#include "Obj_Item_Equip.h"
#include "TDDBC_Struct.h"
#include "../ObjectDef.h"
#include "GameStruct_Item.h"

/*
|
|	ҩƿ����Ʒ
|
*/
class CObject;
struct PET_GUID_t;
class CObject_Item_Medicine : public CObject_Item
{

public:

	enum IDENTIFY_TYPE		// ����������
	{
		IDEN_EQUIP = 6,		// ��������
		IDEN_DEFINCE,		// ��������
		IDEN_APPEND,		// ��������װ��Ʒ��
	};
public:
	//�õ���Ʒ�����
	virtual ITEM_CLASS		GetItemClass(VOID) const ;
	// �õ�quality��Ϣ
	virtual INT				GetItemTableQuality(VOID);
	// �õ�type��Ϣ
	virtual INT				GetItemTableType(VOID);
	//�õ����еĶ���
	const _DBC_ITEM_MEDIC*	GetBaseDefine(void) const { return m_theBaseDef; }
	//��Ʒ������
	virtual LPCTSTR			GetName(VOID) const			{ return m_theBaseDef->szName; }
	//��Ʒ����
	virtual LPCTSTR			GetDesc(VOID) const			{ return m_theBaseDef->szDesc; }
	//ͼ����
	virtual LPCTSTR			GetIconName(VOID) const;
	//��ϸ����(������Ҫ������)
	virtual LPCTSTR			GetExtraDesc(VOID);
	//������ϸ����
	virtual VOID			SetExtraInfo(const _ITEM * pItemInfo);
	//��¡��ϸ��Ϣ
	virtual VOID			Clone(const CObject_Item * pItemSource);
	// �õ���Ʒ��Ʒ��
	INT						GetQuantity();
	// �õ���Ʒ����npc�ļ۸�
	virtual INT				GetItemPrice();
	// �õ���Ʒ������ļ���ID
	INT						GetItemSkillID();
	//�õ���Ʒ��ѡ���������
	virtual INT				GetItemTargetType();
	// �õ����ʹ�������Ʒ��Ҫ�ĵȼ�
	virtual INT				GetNeedLevel();
	//����Ʒ�Ƿ��ܹ���ĳ������ʹ��
	bool 					IsValidTarget(CObject* pSelectObj, fVector2& fvPos, int& objID, PET_GUID_t& petID);
	//�Ƿ�������Χʹ����Ʒ
	bool					IsAreaTargetType(void) const;
	//�Ƿ�Ŀ������Ǿ���Ψһ��(�Լ����Լ��ĳ���)
	bool					IsTargetOne(void) const;
	//�����Ʒ������������
	virtual	INT				GetMaxOverLay(VOID) const	{ return m_theBaseDef->nPileCount; };


	// �õ���Ʒ����������2006-4-28
	LPCTSTR	GetItemTableTypeDesc();

	// �õ���Ʒ����������Ϣ
	virtual INT		GetItemLevelDesc();

public:

	// �õ�����Ʒ�ڱ��е�����
	INT GetMedicineItemTableType();

public:
	CObject_Item_Medicine(INT id);
	virtual ~CObject_Item_Medicine();
	//����һ��ҩƿ
	VOID					AsMedicine(const _DBC_ITEM_MEDIC* pMedicDefine);

protected:
	//--------------------------------------------------------------
	//��������
	const _DBC_ITEM_MEDIC*	m_theBaseDef;			//�������Զ���(���ݱ�)

	STRING					m_strExtraDesc;
};
