#pragma once
#include "Obj_Item.h"
#include "GameDefine2.h"
#include "Obj_Item_Equip.h"
#include "TDDBC_Struct.h"
#include "../ObjectDef.h"
#include "GameStruct_Item.h"

/*
|
|	װ������Ʒ
|
*/
class CObject_Item_Equip : public CObject_Item
{
public:
	//�õ���Ʒ�����
	virtual ITEM_CLASS		GetItemClass(VOID) const;
	//��Ʒ������
	virtual LPCTSTR			GetName(VOID) const;
	//��Ʒ����
	virtual LPCTSTR			GetDesc(VOID) const;
	//ͼ����
	virtual LPCTSTR			GetIconName(VOID) const;
	//��ϸ����(������Ҫ������)
	virtual LPCTSTR			GetExtraDesc(VOID);
	//������ϸ����
	virtual VOID			SetExtraInfo(const _ITEM * pItemInfo);
	//�õ�װ����
	HUMAN_EQUIP				GetItemType(VOID) const;
	//�õ���ʾ����ID
	virtual INT				GetVisualID(VOID)	const;
	//��ѯ�߼�����,һ�����ڽű�����
	virtual STRING			GetAttributeValue(LPCTSTR szValueName);
	//��¡��ϸ��Ϣ
	virtual VOID			Clone(const CObject_Item * pItemSource);
	//�õ���װ���
	INT						GetSetID(VOID) const;
	//�õ���������(����Ƿ�����������WEAPON_TYPE_NONE)
	ENUM_WEAPON_TYPE		GetWeaponType(VOID) const;
	//����װ����ǰ�;öȶ���һ���Ѿ����ɵ�װ����ֻ�����ֵ�ᱻ�ı�
	VOID					SetDur(INT Dur);
	//��õ�ǰ�;ö�
	INT						GetDur(VOID) const;
	//�������;ö�
	INT						GetMaxDur(VOID) const;
	
	//��¡��ϸ��Ϣ
	VOID					ClonetExtraInfo(CObject_Item_Equip * pItemSource);
	//�����ϸ��Ϣ
	VOID*					GetExtraInfo() const{return m_pExtraDefine;};
	//�����Ʒ������������
	virtual	INT				GetMaxOverLay(VOID) const	{return 1;};

	// ���װ��������۸�
	virtual INT				GetRepairPrice();

	//--------------------------------------------------------------------------------------------------------------
	//-- supertooltip ʹ��
	//

	// �õ����ʹ�������Ʒ��Ҫ�ĵȼ�
	virtual INT				GetNeedLevel();

	// �õ���Ʒ�;ö�
	virtual INT				GetItemDur();

	// �õ���Ʒ����;ö�
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


	// �õ���Ʒ����������2006-4-28
	virtual LPCTSTR			GetItemTableTypeDesc();

	// ��ʽ�������ַ���
	void SetAttri(const _ITEM_ATTR& att, char* const pszBuf);

	// �õ���չ��ɫ����
	LPCTSTR			GetExtBlueAttrInfo();

	// �õ���չ��ɫ����
	LPCTSTR			GetGreenAttrInfo();

	// �õ���װ����
	LPCTSTR			GetSuitAttrInfo();

	// �õ���ʯ�ĸ���.
	INT				GetGemMaxCount(VOID) const;

	// �õ���ʯ��ͼ��
	VOID			GetGemIcon(INT iGemIndex, STRING& strIconName);

	// �õ���ʯ�ĸ��ӹ���
	VOID			GetGemExtAttr(INT iGemIndex, STRING& strGemAttr);

	//�������������
	INT		GetMaxRepair(VOID) const;

	//���ʹ��ְҵ
	INT		GetNeedJob(VOID) const;

/*
	CObject_Item*			GetAppendItem(INT nIndex) const;

	VOID					SetAppendItem(CObject_Item* pItem,INT nIndex);
*/


public:
	typedef std::vector< _ITEM_ATTR >		EQUIP_ATTRIBUTES;
	typedef std::vector< _ITEM_GEMINFO >	EQUIP_ATTACH_GEM;

public:
	CObject_Item_Equip(INT id);
	virtual ~CObject_Item_Equip();

	//����һ����ɫװ��
	VOID		AsWhiteEquip(const _DBC_ITEM_EQUIP_WHITE* pDefine);
	//����һ����ɫװ��
	VOID		AsGreenEquip(const _DBC_ITEM_EQUIP_GREEN* pDefine); 
	//����һ����ɫװ��
	VOID		AsBlueEquip(const _DBC_ITEM_EQUIP_BLUE* pDefine);

	//�õ����а�ɫ����
	VOID		GetAllWhiteAttribute(STRING& strValue);
	//�õ�������ɫ����
	VOID		GetAllBlueAttribute(STRING& strValue);
	//�õ�������ɫ����. 
	VOID		GetAllGreenAttribute(STRING& strValue);
	//�õ�������װ����. 
	VOID		GetAllSuitAttribute(STRING& strValue);

	//�õ���Ƕ�ı�ʯ����.
	INT			GetGemCount();
	// �õ���Ʒ����qualityһ��2006��3��30
	virtual INT	GetItemTableQuality(VOID);

	// �õ���Ʒ����typeһ��2006��3��30
	virtual INT	GetItemTableType(VOID);

public:
	//--------------------------------------------------------------
	enum EQUIP_QUALITY
	{
		INVALID_EQUIP = 0,

		WHITE_EQUIP,
		GREEN_EQUIP,
		BLUE_EQUIP,
		PURPLE_EQUIP,
	};

	//�������Զ���
	union BASE_DEFINE
	{
		const _DBC_ITEM_EQUIP_WHITE*	pDefineAsWhite;
		const _DBC_ITEM_EQUIP_GREEN*	pDefineAsGreen;
		const _DBC_ITEM_EQUIP_BLUE*		pDefineAsBlue;
	};

	enum EQUIP_BIND_INFO
	{
		FREE_BIND = 0,		// �ް�����
		BINDED,				// �Ѿ���
		GETUP_BIND,			// ʰȡ��
		EQUIP_BIND,			// װ����
	};
	//��չ���Զ���
	struct EXTRA_DEFINE
	{
		INT					m_CurDurPoint;				//��ǰ�;�ֵ
		INT					m_MaxDurPoint;				//����;�ֵ
		EQUIP_ATTRIBUTES	m_vEquipAttributes;			//װ������(��ɫ����ɫ����)
		EQUIP_ATTRIBUTES	m_vBlueEquipAttributes;		//װ������(��ɫװ������)
		EQUIP_ATTACH_GEM	m_vEquipAttachGem;			//���ӵı�ʯ
		INT					m_nLevelNeed;				//����ȼ�
//		INT					m_nFrogLevel;				//����ȼ�
		INT					m_nRepairFailureTimes;		//����ʧ�ܴ���
		INT					m_nEquipQulity;				//װ��Ʒ��
		INT					m_nSellPrice;				//�۳��۸�
		//BOOL				m_bLocked;					//�Ƿ�����
		EQUIP_BIND_INFO		M_nEquipBindInfo;			// װ���Ƿ��
		INT					m_EnableGemCount;			// ������Ƕ�ı�ʯ����2006��4��30
		EXTRA_DEFINE()
		{
			m_CurDurPoint = 0;				
			m_MaxDurPoint = 0;				
			m_nLevelNeed  = 0;				
			m_nRepairFailureTimes = 0;		
			m_nEquipQulity = 0;				
			m_nSellPrice   = 0;				
			M_nEquipBindInfo = FREE_BIND;	
			m_EnableGemCount = 0;
		}
													
	};
public:
	// �õ�װ����������
	BASE_DEFINE	GetEquipBaseDefine() { return m_theBaseDef;}
	// �õ�װ��������(��, ��, ��)
	EQUIP_QUALITY	GetEquipQuantity();

public:

	enum EQUIP_ATTRIB
	{
		EQUIP_ATTRIB_UNIDENTIFY,	// װ��δ������
		EQUIP_ATTRIB_IDENTIFY,		// װ���Ѽ�����
	};

	EQUIP_ATTRIB	m_EquipAttrib;
	EQUIP_ATTRIB	GetEquipAttrib();

	// �Ƿ�������
	BOOL IsWeapon();

	// �Ƿ��Ƿ���
	BOOL IsDefence();

	// �Ƿ���װ����
	BOOL IsAppend();

protected:
	//Ʒ��
	EQUIP_QUALITY		m_Quality;				

	//--------------------------------------------------------------
	//��������
	BASE_DEFINE			m_theBaseDef;			//�������Զ���(���ݱ�)

	//----------------------------------------------------------------
	//��չ����(��Ҫ����������)
	EXTRA_DEFINE*		m_pExtraDefine;
	STRING				m_strExtraDesc;

//	std::vector< CObject_Item* >	m_vAppendItem;
};