// �����˴���ʯ��Ƕ�Լ���Ʒ���ԼӼ��ĺ���
#ifndef _HUMAN_ITEM_CONTAINER_H_
#define _HUMAN_ITEM_CONTAINER_H_
#include "ItemContainer.h"


class HumanDB;

struct _HUMAN_ITEMCONTAINER_INIT: 
	public _ITEMCONTAINER_INIT
{
	HumanDB*				m_pHumanDB;
	ITEM_CONTAINER_TYPE		m_ICTType;
	UINT					m_DBOffSet;

};

class Obj_Human;

class HumanItemContainer:public ItemContainer
{
	HumanDB*				m_pHumanDB;
	ITEM_CONTAINER_TYPE		m_ICTType;
	UINT					m_DBOffSet;

public:
	virtual		BOOL		Init( const _ITEMCONTAINER_INIT* pInit ) ;//��ʼ������
	//������ŵ�Bag���
	virtual		UINT		ConIndex2BagIndex(UINT uConIndex);
	//������ŵ��������
	virtual		UINT		BagIndex2ConIndex(UINT uBagIndex);
	//���Դ����һ��ֵ�ǲ����ܹ������Con�ķ�Χ֮��
	virtual		BOOL		IsInContainer(UINT uBagIndex);

protected :
	//������Ʒ������
	virtual		BOOL		SetItem( const INT nIndex, const Item* pItem );
	//������Ʒ�ص�����
	virtual		VOID		SetItemLayCount(INT nIndex, INT nCount) ;
	//������Ʒ�ص�����
	virtual		BOOL		DecItemLayCount(INT nIndex, INT nCount=1) ;
	//������Ʒ�ص�����
	virtual		BOOL		IncItemLayCount(INT nIndex, INT nCount=1);
	//ɾ����Ʒ
	virtual		BOOL		EraseItem(UINT uIndex);
	//������Ʒ�;ö�
	virtual	BOOL			SetItemDur(INT nIndex, INT nDur);
	//������Ʒ���˵�
	virtual	BOOL			SetItemDamagePoint(INT nIndex, INT nPoint);
	//������Ʒ����;ö�
	virtual	BOOL			SetItemMaxDur(INT nIndex, INT nDur);

	//������Ʒ����
	virtual	BOOL			SetItemValue(INT nIndex,_ITEM*	pItem);
	//������Ʒ�ﶨ
	virtual	BOOL			SetItemBind(INT nIndex);
	//������Ʒ������Ϣ
	virtual BOOL			SetItemIdent(INT nIndex);
	//������������Ϣ
	virtual	BOOL			SetItemCreator(INT nIndex,const CHAR* CreatorName);

	//������Ʒ����
	virtual BOOL			AddItemAttr(INT nIndex,_ITEM_ATTR iA);
	//ɾ����Ʒ����
	virtual	BOOL			DelItemAttr(INT nIndex,_ITEM_ATTR iA);
	//ɾ����ʯ��Ϣ
	virtual BOOL			DelGemInfo(INT nIndex,INT GemIndex);
	//��ӱ�ʯ��Ϣ
	virtual	BOOL			AddGemInfo(INT nIndex,INT& GemIndex,UINT GemType);
private:
	BOOL					SetDBDirty(INT OffSet);

	
};



#endif