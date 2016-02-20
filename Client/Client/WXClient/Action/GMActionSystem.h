/****************************************\
*										*
* 			  ����������				*
*										*
\****************************************/
#pragma once

#include "TDActionSystem.h"

struct SCLIENT_SKILLCLASS;
struct SCLIENT_SKILL;
struct SCLIENT_LIFEABILITY;
struct PET_SKILL;
struct TALK_ACT_STRUCT;

class tObject_Item;
namespace LuaPlus
{
	class LuaObject;
	class LuaState;
};
struct EVENT;

class CActionItem : public tActionItem
{
public:
	//----------------------------------
	//�õ�id
	virtual INT				GetID(VOID) const { return m_ID; }
	//�õ�����
	virtual LPCTSTR			GetName(VOID) const { return m_strName.c_str(); }
	//�õ�ͼ��
	virtual LPCTSTR			GetIconName(VOID) const { return m_strIconName.c_str(); }
	//����Check״̬
	virtual VOID			SetCheckState(BOOL bCheck);
	//�������
	virtual VOID			AddReference(tActionReference* pRef,BOOL bIsInMenuToolbar=FALSE);
	//�Ͽ�����
	virtual VOID			RemoveRefrence(tActionReference* pRef);
	// Is Enabled
	virtual BOOL			IsEnabled(VOID) const {return m_bEnabled;}
	//Enable
	virtual VOID			Enable(VOID) {m_bEnabled=TRUE;}
	//Disable
	virtual VOID			Disable(VOID) {m_bEnabled=FALSE;}
	//����Default״̬(ֻ��ͨ��ActionSystem����)
	virtual VOID			SetDefaultState(BOOL bDefault);
	//-----------------------------------------------------------------------------------------------------------------------------
	//-- super tool tip ʹ��
	//--
	// �õ���Ʒ�ĵȼ�
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

	// �õ���������
	virtual LPCTSTR			GetTypeDesc();


	//--------------------------
	//�õ���������
	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_EMPTY; }
	//�����ַ���
	virtual	LPCTSTR			GetType_String(VOID)	{ return ""; }
	//�õ�����ID
	/*
	|	����ս������, �Ǽ��ܱ��е�ID (DBC_SKILL_DATA)
	|	��������ܣ�������ܱ��е�ID(DBC_LIFEABILITY_DEFINE)
	|	������Ʒ������Ʒ���е�ID(DBC_ITEM_*)
	|	�����ķ������ķ����е�ID(DBC_SKILL_XINFA)
	*/
	virtual INT				GetDefineID(VOID)		 { return -1;	}
	//�õ�����
	virtual INT				GetNum(VOID)			 { return 0;	}
	//�õ��ڲ�����
	virtual VOID*			GetImpl(VOID)			 { return NULL;}
	//�õ�����
	virtual LPCTSTR			GetDesc(VOID)			 { return _T("");	}
	//�õ���ȴ״��ID
	virtual INT				GetCoolDownID(VOID)		 { return -1;	}
	//�õ���������������
	virtual INT				GetPosIndex(VOID)		 { return -1;	}
	//�����
	virtual VOID			DoAction(VOID)			 {				}
	//�����(������)
	virtual VOID			DoSubAction(VOID)		 {				}
	//�Ƿ���Ч
	virtual BOOL			IsValidate(VOID)		 {	return FALSE;}
	//�����ȴ�Ƿ����
	virtual BOOL			CoolDownIsOver(VOID)	 { return TRUE; }
	//�϶�����
	virtual VOID			NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName) { }
	//��ʾtooltips
	virtual VOID			NotifyTooltipsShow(VOID);
	//����tooltips
	virtual VOID			NotifyTooltipsHide(VOID);
	//��ѯ�߼�����
	virtual STRING			GetAttributeValue(LPCTSTR szAttributeName){ return ""; }
	// �õ��ķ��ȼ�
	//virtual INT				GetXinfaLevel(){return -1;}

	// �Ƿ���Ҫ��ToolTip����ʾ�۸�
	virtual INT				IsViewToolTopsPrice();
	// �����ToolTip����ʾ�ļ۸�
	virtual INT		 		GetViewToolTopsPrice();

	// �õ���Ʒ���εȼ����� 2006-5-18
	virtual INT GetItemLevelDesc(){	return 1;}
public:
	//�������
	VOID			ClearRefrence(VOID);
	//������ȴ
	virtual VOID	UpdateCoolDown(VOID);
	//֪ͨUI
	VOID			UpdateToRefrence(VOID);
	//���������ʾ��ť����
	VOID			DoActionPrepare(BOOL bCheck);
	//���빫����ȴ
	VOID			UpdateCommonCoolDown(VOID);

	//--------------------------------------------
	//�������ű��еĺ���
protected:
	friend class CScriptSystem;

	INT		Lua_GetID(LuaPlus::LuaState* pState);	//�õ�ID
	INT		Lua_GetType(LuaPlus::LuaState* pState);	//�õ�����
	INT		Lua_GetName(LuaPlus::LuaState* pState);	//�õ�����
	INT		Lua_GetDesc(LuaPlus::LuaState* pState);	//�õ�����
	INT		Lua_GetDefineID(LuaPlus::LuaState* pState);	//�õ�����Դ���е�ID
														/*
														|	����ս������, �Ǽ��ܱ��е�ID (DBC_SKILL_DATA)
														|	���ڳ��＼��, �Ǽ��ܱ��е�ID (DBC_SKILL_DATA)
														|	��������ܣ�������ܱ��е�ID(DBC_LIFEABILITY_DEFINE)
														|	������Ʒ������Ʒ���е�ID(DBC_ITEM_*)
														|	�����ķ������ķ����е�ID(DBC_SKILL_XINFA)
														|
														*/
	INT		Lua_GetOwnerXinfa(LuaPlus::LuaState* pState);	//!!ֻ���ս�����ܣ��õ��������ķ�
	INT		Lua_GetPetSkillOwner(LuaPlus::LuaState* pState);//!!ֻ��Գ��＼�ܣ��õ����������ڵڼ�ֻ����ġ�
	INT		Lua_GetOwnerQuest(LuaPlus::LuaState* pState);	//!!ֻ���������������Ʒ���õ���Ʒ�����ڵڼ�������ġ�
	INT		Lua_GetPrice(LuaPlus::LuaState* pState);	//�õ��۸�
	INT		Lua_GetNum(LuaPlus::LuaState* pState);		//�õ�ÿһ�������

public:
	CActionItem(INT id=0);
	virtual ~CActionItem();

	static LuaPlus::LuaObject*		s_pMetaTable;
	//�Ƿ�ActionItem
	static CActionItem s_InvalidAction;

protected:
	//ID(�ͻ����ڲ���ʾ)
	INT				m_ID;
	//����
	STRING			m_strName;
	//ͼ��
	STRING			m_strIconName;
	//UI����
	std::set< tActionReference* >	m_setReference;
	//Checked
	BOOL			m_bChecked;
	// ��Ч��Ч
	BOOL 			m_bEnabled;

	BOOL			m_bLocked;

};

class CActionSystem : public tActionSystem
{
public:
	//����idȡ��Action
	virtual tActionItem*	GetAction(INT id);
	//�õ�ȱʡ����
	virtual tActionItem*	GetDefaultAction(VOID) { return m_pDefaultAction; }
	virtual VOID			SetDefaultAction(tActionItem* pDefAction);

	//�õ����μ���ʹ����Ϻ���һ�β����ļ���
	virtual tActionItem*	GetSetoutAction(VOID) { return m_pSetoutAction; }
	virtual VOID			SetSetoutAction(tActionItem* pSetoutAction) { m_pSetoutAction = pSetoutAction; }

public:
	//�õ�ĳ��Action����Ŀ
	virtual INT				GetActionNum(LPCTSTR szType);

	//ö��Action
	virtual tActionItem*	EnumAction(INT nPackageIndex, LPCTSTR szFilter);
	//ö�ٳ���Skill
	virtual tActionItem*	EnumPetSkill(INT nPetIndex, INT nPetSkill, LPCTSTR szFilter);

	//��ǰ������ʾTootips�İ�ť
	virtual tActionItem*	GetTooltipsFocus(void);

	//����������ݲ��� 
	// nIndex	(0,1,2,3,4,5,6,7,8,9)
	// bPrepare �Ƿ���Ԥ��(����ʾCheck״̬)
	virtual VOID			MainMenuBar_DoAction(INT nIndex, BOOL bPrepare);
	//�������˵�����Ŀ
	virtual VOID			MainMenuBar_Set(INT nIndex, INT idAction);
	//�������˵�����Ŀ���ӷ�����������
	virtual VOID			MainMenuBar_SetID(INT nIndex, INT idType, INT idData);

	//ȡ�����˵�����Ŀ
	virtual INT				MainMenuBar_Get(INT nIndex);

	//���ñ���
	virtual VOID			UserBag_Set(INT nIndex, INT idAction);
	//�洢���
	virtual VOID			SaveAction(VOID);


public:
	virtual VOID	Initial(VOID*);
	virtual VOID	Release(VOID);
	virtual VOID	Tick(VOID);

public:
	VOID			UpdateToolBarForPetSkill(VOID);
	//----------------------------------------------------
	//���ܸ���
	VOID			UserSkill_Update(VOID);
	//���ܿ�ʼʹ��(����������)
	static VOID	WINAPI		_OnSkillActive(const EVENT* pEvent, UINT dwOwnerData);

	VOID			UserPetSkill_Update(INT nPetNum);
	VOID			UserPetSkillStudy_Update(VOID);
	VOID			UserTargetPetSkill_Update(VOID);

	//----------------------------------------------------
	//����ܸ���
	VOID			UserLifeAbility_Update(VOID);

	//----------------------------------------------------
	//�������µİ������ݸ���
	VOID			UserBag_Update(VOID);

	//----------------------------------------------------
	//������������
	VOID			UserBank_Update(VOID);

	//----------------------------------------------------
	//���½��׽����Լ���Ϣ
	VOID			UserExchangeSelf_Update(VOID);
	//�Է�
	VOID			UserExchangeOther_Update(VOID);

	// ������Ʒ�ύ����ĸ���
	VOID			UserMission_Update(VOID);

	// ����̵�ĸ���
	VOID			UserPS_Self_Update(VOID);
	VOID			UserPS_Other_Update(VOID);


	//----------------------------------------------------
	//�µ����Ӳ���
	VOID			ItemBox_Created(INT ObjId=-1);
	//�����е���Ŀ�����ı�
	VOID			ItemBox_Update(VOID);

	//----------------------------------------------------
	//�̵�������ݸ���
	VOID			Booth_Update(VOID);

	//----------------------------------------------------
	//�����б����ݸ���
	VOID			QuestLogItem_Update(VOID);

	//----------------------------------------------------
	//�ķ����ݸ���
	VOID			SkillClass_Update(VOID);


	//----------------------------------------------------
	//��ȴʱ�����
	VOID			UpdateCoolDown(INT nCoolDownID);
	//������ȴ����
	VOID			UpdateCommonCoolDown(INT nSkillID);

	//----------------------------------------------------
	// װ������, �����ݳ��е����ݸ���װ��action����

	// ͨ�����ݳ��е�����, ��������װ��action item
	VOID			UserEquip_Update(VOID);

	// ͨ�����ݳ��е�����, ��������װ��action item
	VOID			OtherPlayerEquip_Update(VOID);

	//----------------------------------------------------
	//��̯�����ۣ�
	VOID			UserStallSale_Update(VOID);
	//��̯������
	VOID			UserStallBuy_Update(VOID);

	//----------------------------------------------------
	//�������ﶯ��
	VOID			UserChatMood_Update(VOID);

protected:
	//ɾ��ĳ��Action
	VOID			EraseActionItem(CActionItem* pItem);

	//������Ҽ��ܸ��²���
	VOID			UpdateAction_FromSkill(const SCLIENT_SKILL* pSkill);
	//���ݳ��＼�ܸ��²���
	VOID			UpdateAction_FromPetSkill(INT nPetNum,const PET_SKILL* pSkill);
	//���ݳ������ɼ��ܸ��²���
	VOID			UpdateAction_FromPetSkillStudy(const PET_SKILL* pSk);
	//������Ʒ����
	VOID			UpdateAction_FromItem(tObject_Item* pItem);
	//���������
	VOID			UpdateAction_FromLifeAbility(const SCLIENT_LIFEABILITY* pLifeAbility);
	//�������ɴ�������ϵ
	VOID			UpdateAction_FromSkillClass(const SCLIENT_SKILLCLASS* pSkillClass);
	//�������춯��
	VOID			UpdateAction_FromChatMood(const TALK_ACT_STRUCT* pActStruct);
	
	//----------------------------------------------------
	//ˢ�¹�����
	VOID			UpdateToolBar(VOID);

protected:
	//-----------------------------------------------------
	//���в���
	std::map< INT, CActionItem* >		m_mapAllAction;
	//ȱʡ����(������)
	tActionItem*						m_pDefaultAction;
	//׼��������action
	tActionItem*						m_pSetoutAction;
	
protected:
	//-----------------------------------------------------
	//����������
	struct ACTIONBAR_ITEM
	{
		
		ACTION_OPTYPE	typeItem;	// �����õ�ActionItem������

		INT			idImpl;			// ������Ŀ�Ķ���ID
									/*
									|
									|	����		- ����ID
									|	�����	- �����ID
									|	��Ʒ		- client��idtable
									|	���＼��	- ����ID
									*/
		//---����ʱ
		INT			idActionItem;		// ���õ�ActionItem
		
		INT			idItemCount;		// ��Ʒ����

		INT			idPetIndex;			// ��������
	};

	//һ���������Ķ�������
	typedef std::vector< ACTIONBAR_ITEM >	ACTIONBAR_BARDEF;

	//�������� 10 X ITEM
	const static INT	s_MainMenuBarNum = 20;
	ACTIONBAR_BARDEF	m_barMain;

public:
	//���ݼ���id����Action
	//bAsDefID - ���ݼ��ܱ��ж���id
	tActionItem*						GetAction_SkillID(INT idSkill);
	//���ݼ��ܱ��ж���id
	tActionItem*						GetAction_PetSkillID(INT nPetNum,INT idPetSkill);
	//������Ʒid����Action
	tActionItem*						GetAction_ItemID(INT idItem);
	//������Ʒ��idtable����Action
	tActionItem*						GetAction_ItemIDTable_FromMyPacket(INT idItem);
	//���������id
	tActionItem*						GetAction_LifeAbilityID(INT idAbility);
	//�����ķ�id
	tActionItem*						GetAction_SkillClass(INT idSkillClass);
	//�������춯��id
	tActionItem*						GetAction_ChatMoodID(INT idAct);

	//�����Ч��Action
	VOID								CleanInvalidAction(BOOL bForceCleanItemBox=FALSE);

	//��ǰ����������ı��
	VOID		SetCurBankRentBoxIndex(INT nCurBankRentBoxIndex){m_nCurBankRentBoxIndex=nCurBankRentBoxIndex;}
	INT			GetCurBankRentBoxIndex(VOID){return m_nCurBankRentBoxIndex;}

public:
	CActionSystem();
	virtual ~CActionSystem();
	static CActionSystem*	GetMe(VOID) { return s_pMe; }
private:
	INT		_CreateID(VOID) const;

	//��ǰ������������ı��
	INT		m_nCurBankRentBoxIndex;
	//�Ƿ��ʼ����Ͽ����
//	BOOL			m_bIsInitalDone;

protected:
	static CActionSystem*	s_pMe;
	WX_DECLARE_DYNAMIC(CActionSystem);
};

extern const char*		NAMETYPE_SKILL;
extern const char*		NAMETYPE_LIFESKILL;
extern const char*		NAMETYPE_PACKAGEITEM;
extern const char*		NAMETYPE_BANKITEM;
extern const char*		NAMETYPE_BOOTITEM;
extern const char*		NAMETYPE_LOOTITEM;
extern const char*		NAMETYPE_XINFA;
extern const char*		NAMETYPE_PETSKILL;
extern const char*		NAMETYPE_VIRTUALITEM;
extern const char*		NAMETYPE_EQUIP;
extern const char*		NAMETYPE_EXCHANGESELF;
extern const char*		NAMETYPE_EXCHANGEOTHER;
extern const char*		NAMETYPE_MISSIONREFER;
extern const char*		NAMETYPE_APPENDITEM;
extern const char*		NAMETYPE_STALLSELF;
extern const char*		NAMETYPE_STALLOTHER;
extern const char*		NAMETYPE_OTHEREQUIP;
extern const char*		NAMETYPE_PS_SELF;
extern const char*		NAMETYPE_PS_OTHER;
extern const char*		NAMETYPE_CHATMOOD;
extern const char*		NAMETYPE_CALLBACK;
extern const char*		NAMETYPE_SPLITGEM;
extern const char*		NAMETYPE_CITY_RESEARCH;
extern const char*		NAMETYPE_CITY_SHOP;
extern const char*		NAMETYPE_TRANSFER_ITEM;
extern const char*		NAMETYPE_QUESTUI_DEMAND;
extern const char*		NAMETYPE_QUESTUI_REWARD;

