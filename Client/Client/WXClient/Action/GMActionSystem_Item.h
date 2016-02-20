/****************************************\
*										*
* 			  ����������-��Ʒ			*
*										*
\****************************************/

#pragma once
#include "GMActionSystem.h"

class CActionItem_Item : public CActionItem
{
public:
	enum TOOLTOPS_VIEW_PRICE_TYPE
	{
		TOOLTOPS_VIEW_PRICE_NO = 0,
		TOOLTOPS_VIEW_PRICE_SELL,
		TOOLTOPS_VIEW_PRICE_REPAIR,
		TOOLTOPS_VIEW_PRICE_CALLBACK,
		TOOLTOPS_VIEW_LOOT_SILVER,				//��Ʊ���͵ļ۸񣬳����̵��õ���Ʊ�ļ۸���ʾ
	};

	//�õ���������
	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_ITEM;	}
	//�����ַ���
	virtual	LPCTSTR			GetType_String(VOID);
	//�õ�����ID
	virtual INT				GetDefineID(VOID);
	//�õ�����
	virtual INT				GetNum(VOID);
	//�õ��ڲ�����
	virtual VOID*			GetImpl(VOID)			{ return (VOID*)GetItemImpl();	}
	//�õ�����
	virtual LPCTSTR			GetDesc(VOID);
	//�õ���ȴ״��ID
	virtual INT				GetCoolDownID(VOID);


	//----------------------------------------------------------------------------------------------------------
	//-- super tool tip ʹ��
	//--

	// �õ�ʹ����Ʒ�ĵȼ�
	virtual INT				GetItemLevel();
	// �õ���Ʒ���;�
	virtual INT				GetItemDur();
	// �õ���Ʒ������;�
	virtual INT				GetItemMaxDur();
	// �õ���Ʒ���������
	virtual INT				GetItemRepairCount();
	// �õ���Ʒ�İ���Ϣ 
	virtual INT				GetItemBindInfo();
	// �õ���Ʒ����npc�ļ۸�
	virtual INT				GetItemPrice();
	// �õ���Ʒ��������
	virtual LPCTSTR			GetManufacturer();
	// �õ���ɫ����
	virtual LPCTSTR			GetBaseWhiteAttrInfo();
	// �õ���չ��ɫ����
	virtual LPCTSTR			GetExtBlueAttrInfo();
	// �õ�action�����õ�item ������
	virtual INT				GetItemType();
	// �õ���ɫ����
	LPCTSTR	GetGreenAttrInfo();
	// �õ���������
	virtual LPCTSTR			GetTypeDesc();
	// �Ƿ���Ҫ��ToolTip����ʾ�۸�
	virtual INT				IsViewToolTopsPrice();
	// �����ToolTip����ʾ�ļ۸�
	virtual INT		 		GetViewToolTopsPrice();
	
	


	//�õ���������������
	//	�����е���Ʒ	- �������е�λ��
	//	�����ϵ���Ʒ	- ������λ��
	//
	virtual INT				GetPosIndex(VOID);
	//�Ƿ��ܹ��Զ���������
	virtual BOOL			AutoKeepOn(VOID)		{ return FALSE; }
	//�����
	virtual VOID			DoAction(VOID);
	//�����(������)
	virtual VOID			DoSubAction(VOID);
	//�Ƿ���Ч
	virtual BOOL			IsValidate(VOID);
	//ˢ��
	virtual VOID			Update(tActionReference* pRef);
	//�����ȴ�Ƿ����
	virtual BOOL			CoolDownIsOver(VOID);
	//�϶�����
	virtual VOID			NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName);
	//��ѯ�߼�����
	virtual STRING			GetAttributeValue(LPCTSTR szAttributeName);

	//�������������Ŀ�ʼ����ֵ
	virtual INT				GetCurBankRentBegin(VOID);


public:

	// �õ�װ��������(��, ��, ��)
	INT GetEquipQuantity();

	// �õ�װ���ϵı�ʯ����
	INT GetGemInEquipCount();

	// �õ�װ��������;�
	INT GetItemMaxRepair();

	// �õ�װ���ϵı�ʯ��ͼ��
	VOID GetGemIcon(INT iGemIndex, STRING& strIconName);

	// �õ�װ���ϵı�ʯ����
	VOID GetGemInEquipDesc(STRING& strIconName);

	// �õ����ڵڼ�������
	INT	GetQuestIndex(VOID)	{return m_Quest_Number;}

	// �õ�װ���ļ�����Ϣ
	INT GetEquipIdentify();



public:

	//-----------------------------------------------------------------------------------------------------------------------
	//
	// ��ʯ��Ϣ
	//

	// �õ���ʯ�ȼ�
	INT GetGemLevel();

	// �õ���ʯ������Ϣ
	LPCTSTR	GetGemArribInfo();


	//-----------------------------------------------------------------------------------------------------------------------
	//
	// ԭ�Ϻ�ҩƷ
	//

	// �õ�ԭ�ϻ�ҩƷ������
	INT GetItemQuality();
	// �õ�����Ʒ�ڱ��е�����
	INT GetMedicineItemTableType();
	// �õ��䷽��Ҫ�ļ������ơ�
	LPCTSTR GetPrescrNeedSkillName();
	// �õ��䷽��Ҫ�ļ��ܵȼ���
	int GetPrescrNeedSkillLevel();
	// �õ��䷽��Ҫ���ܵĵ�ǰ�ȼ���
	int GetPrescrNeedSkillCurLevel();

	// ��������Ƿ��Ѿ�ѧ��
	bool IsPrescrStudy();
	// �õ���Ʒ���εȼ����� 2006-5-18
	virtual INT GetItemLevelDesc();

	// ʹ�ü�������2006��3��30
	VOID UseIdentify();

	// ׼��ʹ�ü�������2006-3-30
	VOID PrepareUseIdentify();

	// ȡ������״̬2006��4��15
	VOID CancelIdentify();

public:
	CActionItem_Item(INT nID);
	virtual ~CActionItem_Item();

	// 
	VOID	Update_Item(tObject_Item* pItem);

	// ����������ϸ������Ϣ
	void SendAskItemInfoMsg();

	//�õ���Ʒ
	tObject_Item*			GetItemImpl(VOID);
	

protected:
	//������Ʒ����ʱ����ƷID(ָ��tObject_Item)
	INT		m_idItemImpl;

	//���ڵڼ�������
	INT		m_Quest_Number;

private:

	// ��װ��������ж��
	VOID	UnUseEquip();
	//DoAction_Packet
	VOID	DoAction_Packet(VOID);
	//������Ʒ�Ĳ���
	VOID	DestoryItem(LPCSTR szSourceName);

};
