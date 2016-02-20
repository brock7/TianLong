#pragma once
#include "Obj_Item.h"
#include "GameDefine2.h"
#include "Obj_Item_Equip.h"
#include "TDDBC_Struct.h"
#include "../ObjectDef.h"
#include "GameStruct_Item.h"

/*
|
|	��ͼ����Ʒ
|
*/
class CObject_Item_StoreMap : public CObject_Item
{
public:
	//�õ���Ʒ�����
	virtual ITEM_CLASS		GetItemClass(VOID) const { return (ITEM_CLASS)m_theBaseDef->nClass; }

	// 2006-3-30;
	// �õ�quality��Ϣ
	virtual INT				GetItemTableQuality(VOID);
	
	// �õ�type��Ϣ
	virtual INT				GetItemTableType(VOID);


	//��Ʒ������
	virtual LPCTSTR			GetName(VOID) const			{ return m_theBaseDef->szName; }
	//��Ʒ����
	virtual LPCTSTR			GetDesc(VOID) const			{ return m_theBaseDef->szDesc; }
	//��Ʒ����ϸ��ţ�����client�õ�index
	virtual INT				GetParticularID(VOID) const	{ return m_nParticularID; }
	//ͼ����
	virtual LPCTSTR			GetIconName(VOID) const;
	//��ϸ����(������Ҫ������)
	virtual LPCTSTR			GetExtraDesc(VOID);
	//������ϸ����
	virtual VOID			SetExtraInfo(const _ITEM * pItemInfo);
	//��¡��ϸ��Ϣ
	virtual VOID			Clone(const CObject_Item * pItemSource);
	//�����Ʒ������������
	virtual	INT				GetMaxOverLay(VOID) const	{return 1;};


public:
	CObject_Item_StoreMap(INT id);
	virtual ~CObject_Item_StoreMap();
	//����һ����ͼ
	VOID					AsStoreMap(const _DBC_ITEM_STOREMAP* pStoreMapDefine);

protected:
	//--------------------------------------------------------------
	//��������
	const _DBC_ITEM_STOREMAP*	m_theBaseDef;			//�������Զ���(���ݱ�)

	STRING						m_strExtraDesc;
	//��Ʒ����ϸ���
	INT							m_nParticularID;
};
