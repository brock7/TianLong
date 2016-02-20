/****************************************\
*										*
* 			   ���ݳ�					*
*										*
\****************************************/
#pragma once
#include "TDDataPool.h"
#include "GMDP_Struct_Pet.h"
#include "GameStruct_Script.h"
#include "GameStruct_Pet.h"
#include "GMDP_Struct_Guild.h"
#include "GMDP_Struct_RecordList.h"
#include "GMDP_Struct_PlayerShop.h"

class CObject;
class CObject_Character;
class CCharacterData;
class CObject_PlayerNPC;
enum HUMAN_EQUIP;
class tObject_Item;
struct _ITEM;
struct _BUFF_IMPACT_INFO;
struct X_PARAM;
struct X_SCRIPT;
struct EVENT;
class Relation;
class CMailPool;

class CDataPool : public tDataPool_
{
public:
	static void WINAPI	_Skill_OnUpdatePlayerAttrib(const EVENT* pEvent, UINT dwOwnerData);
	//---------------------------------------------
	//�����ӿ�
	//---------------------------------------------
public:
	enum TYPE_PLAYER_SHOP
	{
		TYPE_PLAYER_SHOP_INVALID = 0,		//��Ч��𣬻�û�з����ȥ
		TYPE_PLAYER_SHOP_ITEM,				//��Ʒ��
		TYPE_PLAYER_SHOP_GEM,				//��ʯ��
		TYPE_PLAYER_SHOP_WEAPON,			//������
		TYPE_PLAYER_SHOP_ARMOR,				//���׵�
		TYPE_PLAYER_SHOP_MATERIAL,			//���ϵ�
		TYPE_PLAYER_SHOP_PET,				//�����
	};

	//------------
	//��ɫ���Դ���/����
	//------------
	CCharacterData*			CharacterData_Create(CObject_Character* pCharacterData);
	void					CharacterData_Destroy(CObject_Character* pCharacterData);
	CCharacterData*			CharacterData_Get( INT nID );
	void					SetCharFirstLogin(BYTE bFirst);
	BYTE					GetCharFirstLogin() { return m_bCharFirstLogin; }


	//------------
	//��ȴ��
	//------------
	struct COOLDOWN_GROUP
	{
		INT		nTime;
		INT		nTotalTime;
		INT 	nTimeElapsed;
	};
	//ȡ����ȴ��
	virtual const COOLDOWN_GROUP*	CoolDownGroup_Get(INT nCoolDownID) const;
	//������ȴ��
	virtual VOID					CoolDownGroup_UpdateList(const Cooldown_T *pCoolDownList, INT nUpdateNum);
	//ȡ�ó�����ȴ��
	virtual const COOLDOWN_GROUP*	PetSkillCoolDownGroup_Get(INT nCoolDownID,INT nPetNum) const;
	//���³�����ȴ��
	virtual VOID					PetSkillCoolDownGroup_UpdateList(const Cooldown_T *pCoolDownList, INT nUpdateNum,const PET_GUID_t nPet_GUID);


	//------------
	//������ȴ��
	//------------
	virtual const INT				CommonCoolDown_Get(VOID) const { return m_nCommonCoolDown; }
	virtual VOID					CommonCoolDown_Update(VOID) { m_nCommonCoolDown = COMMONCOOLDOWN_TIME;}
	
	//------------
	//����ʱ��
	//------------
	//ȡ������ʱ��
	virtual const INT				QuestTimeGroup_Get(INT nQuestTimeID) const;
	//��������ʱ��
	virtual VOID					QuestTimeGroup_UpdateList(const INT nQuestTime, INT nUpdateNum);
	
	//------------
	//���װ��ϵ��
	//------------
	//���
	virtual VOID			UserEquip_Clear(VOID);
	//����װ��
	virtual VOID			UserEquip_SetItem(HUMAN_EQUIP ptEquip, tObject_Item* pItem, BOOL bClearOld=TRUE);
	//ȡ��װ��
	virtual tObject_Item*	UserEquip_GetItem(HUMAN_EQUIP ptEquip);
	//��װ�Ƿ���ϳɹ�,������װ��ţ����ɹ�����-1
	virtual INT				UserEquip_IsUnion(VOID);

	//------------
	//����װ��ϵ��
	//------------
	//���
	virtual VOID			OtherPlayerEquip_Clear(VOID);
	//����װ��
	virtual VOID			OtherPlayerEquip_SetItem(HUMAN_EQUIP ptEquip, tObject_Item* pItem, BOOL bClearOld=TRUE);
	//ȡ��װ��
	virtual tObject_Item*	OtherPlayerEquip_GetItem(HUMAN_EQUIP ptEquip);
	//��װ�Ƿ���ϳɹ�,������װ��ţ����ɹ�����-1
	virtual INT				OtherPlayerEquip_IsUnion(VOID);

	//------------
	// ������Ʒ
	//------------
	//���
	virtual VOID			UserBag_Clear(VOID);
	//������Ʒ
	virtual VOID			UserBag_SetItem(INT nBagIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	//����������Ʒ����
	virtual VOID			UserBag_SetItemInfo(INT nBagIndex, BOOL bEmpty, const _ITEM* pItem);
	//ȡ��װ��
	virtual tObject_Item*	UserBag_GetItem(INT nBagIndex);
	//ͨ��GUIDȡ��װ��λ��
	virtual INT				UserBag_GetItemIndexByGUID(__int64	 ItemGUID);
	//ͨ��IDȡ����Ʒλ��
	virtual INT				UserBag_GetItemIndexByID( INT nID );
	//����װ����ϸ��Ϣ
	virtual VOID			UserBag_AskExtraInfo(INT nBagIndex);
	//ͨ��GUIDȡ��װ��������
	virtual INT				UserBag_CountItemByIDTable(INT	 ItemGUID);

	//------------
	// �򿪵�����
	//------------
	//���
	virtual VOID			ItemBox_Clear(VOID);
	//��������ID
	virtual VOID			ItemBox_SetID(INT id)		{ m_idItemBox = id; }
	//ȡ������ID
	virtual INT				ItemBox_GetID(VOID) const	{ return m_idItemBox; }
	//����������Ʒ
	virtual VOID			ItemBox_SetItem(INT nBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	//ȡ��������Ʒ
	virtual tObject_Item*	ItemBox_GetItem(INT nBoxIndex);
	//����IDȡ����Ʒ
	virtual tObject_Item*	ItemBox_GetItem(WORD idWorld, WORD idServer, INT idSerial, INT& nIndex);
	//����Ŀǰ��������Ʒ����
	virtual INT				ItemBox_GetNumber(VOID);

	//���һ����Ʒ�ڵ�ǰ�̵��ʵ���չ��۸�
	virtual INT				ItemBox_GetCurNpcBuyPrice(INT nBagIndex);
	//------------
	// �̵�
	//------------
	virtual VOID				Booth_Clear( VOID );
	virtual VOID				Booth_Sold_Clear( VOID );
	virtual VOID				Booth_SetItem(INT nBoothIndex, tObject_Item* pItem);
	virtual VOID				Booth_SetItemPrice(INT nBoothIndex, UINT nPrice);
	virtual VOID				Booth_SetSoldItem(INT nBoothIndex, tObject_Item* pItem);
	virtual VOID				Booth_SetSoldPrice(INT nSoldIndex, UINT uPrice);
	virtual VOID				Booth_SetNumber(INT nBoothNumber) { m_nBoothNumber = nBoothNumber; }
	virtual VOID				Booth_SetSoldNumber(INT nBoothNumber) { m_nBoothSoldNumber = nBoothNumber; }
	virtual tObject_Item*		Booth_GetItem(INT nBoothIndex);
	virtual UINT				Booth_GetItemPrice(INT nBoothIndex);
	virtual UINT				Booth_GetSoldPrice(INT nSoldIndex);
	virtual tObject_Item*		Booth_GetSoldItem(INT nBoothIndex);
	virtual	tObject_Item*		Booth_GetItemByID(INT IDtable);
	virtual INT					Booth_GetNumber(VOID) const { return m_nBoothNumber; }
	virtual INT					Booth_GetSoldNumber(VOID) const { return m_nBoothSoldNumber ; }
//	virtual	BOOL				Booth_IsCursorRepairing(VOID)	const {return	m_bIsRepairing;}
//	virtual	VOID				Booth_SetCursorRepairing(BOOL flag)	{m_bIsRepairing = flag;}
	virtual	BOOL				Booth_IsClose(VOID)	const {return	m_bBoothWindowIsDirty;}
	virtual VOID				Booth_Open(VOID){m_bBoothWindowIsDirty = FALSE;}
	virtual VOID				Booth_Close(VOID){m_bBoothWindowIsDirty = TRUE;}
	virtual VOID				Booth_SetBuyType(INT nBuyType){m_nBuyType = nBuyType;}
	virtual INT					Booth_GetBuyType(VOID){return m_nBuyType;}
	virtual VOID				Booth_SetRepairType(INT nRepairType){m_nRepairType = nRepairType;}
	virtual INT					Booth_GetRepairType(VOID){return m_nRepairType;}

	virtual VOID				Booth_SetRepairLevel(INT nRepairLevel){m_nRepairLevel = nRepairLevel;}
	virtual INT					Booth_GetRepairLevel(VOID){return m_nRepairLevel;}
	virtual VOID				Booth_SetBuyLevel(INT nBuyLevel){m_nBuyLevel = nBuyLevel;}
	virtual INT					Booth_GetBuyLevel(VOID){return m_nBuyLevel;}
	virtual VOID				Booth_SetRepairSpend(FLOAT nRepairSpend){m_nRepairSpend = nRepairSpend;}
	virtual FLOAT				Booth_GetRepairSpend(VOID){return m_nRepairSpend;}
	virtual VOID				Booth_SetRepairOkProb(FLOAT nRepairOkProb){m_nRepairOkProb = nRepairOkProb;}
	virtual FLOAT				Booth_GetRepairOkProb(VOID){return m_nRepairOkProb;}
	virtual VOID				Booth_SetScale(FLOAT fScale){m_fScale = fScale;}
	virtual FLOAT				Booth_GetScale(VOID){return m_fScale;}

	virtual VOID				Booth_SetShopNpcId(INT nShopNpcId){m_nShopNpcId = nShopNpcId;}
	virtual INT					Booth_GetShopNpcId(VOID){return m_nShopNpcId;}

	virtual VOID				Booth_SetShopUniqueId(UINT nShopUniqueId){m_nShopUniqueId = nShopUniqueId;}
	virtual UINT				Booth_GetShopUniqueId(VOID){return m_nShopUniqueId;}

	virtual BOOL				Booth_IsCanRepair(tObject_Item* pItem);
	virtual BOOL				Booth_IsCanBuy(tObject_Item* pItem);

	virtual VOID				Booth_SetCurrencyUnit(INT nCurrencyUnit){m_nCurrencyUnit = nCurrencyUnit;}
	virtual INT					Booth_GetCurrencyUnit(VOID){return m_nCurrencyUnit;}

	virtual VOID				Booth_SetSerialNum(INT nSerialNum){m_nSerialNum = nSerialNum;}
	virtual INT					Booth_GetSerialNum(VOID){return m_nSerialNum;}

	virtual VOID				Booth_SetbCanBuyMult(INT bBuyMulti){m_nBuyMulti = bBuyMulti;}
	virtual INT					Booth_GetbCanBuyMult(VOID){return m_nBuyMulti;}

	virtual VOID				Booth_SetCallBack(INT bCallBack){m_bCallBack = bCallBack;}
	virtual INT					Booth_GetCallBack(VOID){return m_bCallBack;}

	virtual VOID				Booth_SetShopType(INT nType){m_nShopType = nType;}
	virtual INT					Booth_GetShopType(VOID){return m_nShopType;}

	virtual INT					Booth_GetShopNpcCamp(VOID){return 1;}

	//------------------------------------------------------
	// �������ݵķ���
	//------------------------------------------------------
	virtual VOID				UserBank_Clear(VOID);
	virtual VOID				UserBank_SetItem(INT nBankIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	virtual tObject_Item*		UserBank_GetItem(INT nBankIndex);
	virtual	VOID				UserBank_SetBankEndIndex(INT endindex)	{m_CurBankEndIndex = endindex;}
	virtual	INT					UserBank_GetBankEndIndex(){	return m_CurBankEndIndex;}
	virtual	VOID				UserBank_SetBankMoney(INT Money){m_CurBankMoney = Money; }
	virtual	INT					UserBank_GetBankMoney(){return m_CurBankMoney;}
	virtual	VOID				UserBank_SetBankRMB(INT Money){m_CurBankRMB = Money; }
	virtual	INT					UserBank_GetBankRMB(){return m_CurBankRMB;}
	virtual	VOID				UserBank_SetItemExtraInfo(INT nBankIndex, BOOL bEmpty, const _ITEM* pItem);
	// ��ѯ����nIndex��ŵ����������ǲ����пո�
	virtual BOOL				UserBank_IsEmpty(INT nIndex);
	virtual VOID				UserBank_SetNpcId(INT nNpcId){m_nBankNpcID = nNpcId;}
	virtual INT					UserBank_GetNpcId(VOID){return m_nBankNpcID;}


	//------------
	// ��ϵϵͳ����
	//------------
	virtual Relation*			GetRelation() { return m_pRelation; }
	virtual CMailPool*			GetMailPool() { return m_pMailPool; }

	//------------------------------------------------------
	// ���׺����ݷ���
	//------------------------------------------------------
	virtual VOID				MyExBox_Clear(VOID);
	virtual VOID				MyExBox_SetItem(INT nExBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	virtual tObject_Item*		MyExBox_GetItem(INT nExBoxIndex);
	virtual	VOID				MyExBox_SetItemExtraInfo(INT nExBoxIndex, BOOL bEmpty, const _ITEM* pItem);
	virtual	VOID				MyExBox_SetMoney(INT Money){m_MyExchangeBox.m_Money	= Money; }
	virtual	INT					MyExBox_GetMoney(){return m_MyExchangeBox.m_Money;}
	virtual	VOID				MyExBox_SetLock(BOOL Setlock){m_MyExchangeBox.m_IsLocked	=	Setlock; }
	virtual	BOOL				MyExBox_GetLock(){return m_MyExchangeBox.m_IsLocked; }
	virtual	VOID				MyExBox_SetConfirm(BOOL SetConfirm){m_MyExchangeBox.m_CanConform	=	SetConfirm; }
	virtual	BOOL				MyExBox_GetConfirm(){return m_MyExchangeBox.m_CanConform; }
	virtual	INT					MyExBox_GetItemNum(){return (INT)m_MyExchangeBox.m_ItemList.size(); }

	INT							MyExBox_GetPetByGuid(PET_GUID_t PetGuid);
	VOID						MyExBox_SetPet(INT nIndex, SDATA_PET* pPetData, BOOL bClearOld = TRUE);
	SDATA_PET*					MyExBox_GetPet(INT nIndex);
	INT							MyExBox_GetCount(VOID);
	VOID						MyExBox_SetSkill(INT nIndex,INT nSkillIndex,const _OWN_SKILL *Skill);
	PET_SKILL*					MyExBox_GetSkill(INT nIndex,INT nSkillIndex);


	// ��ѯ�Լ��Ľ��׿���ͷ�Ƿ��пո�
	virtual	BOOL				MyExBox_IsEmpty(VOID);


	virtual VOID				OtExBox_Clear(BYTE bFlagClearPet = TRUE);
	virtual VOID				OtExBox_SetItem(INT nExBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	virtual tObject_Item*		OtExBox_GetItem(INT nExBoxIndex);
	virtual	VOID				OtExBox_SetItemExtraInfo(INT nExBoxIndex, BOOL bEmpty, const _ITEM* pItem);
	virtual	VOID				OtExBox_SetMoney(INT Money){m_OtherExchangeBox.m_Money	= Money; }
	virtual	INT					OtExBox_GetMoney(){return m_OtherExchangeBox.m_Money;}
	virtual	VOID				OtExBox_SetLock(BOOL Setlock){m_OtherExchangeBox.m_IsLocked	=	Setlock; }
	virtual	BOOL				OtExBox_GetLock(){return m_OtherExchangeBox.m_IsLocked; }
	virtual	VOID				OtExBox_SetConfirm(BOOL SetConfirm){m_OtherExchangeBox.m_CanConform	=	SetConfirm; }
	virtual	BOOL				OtExBox_GetConfirm(){return m_OtherExchangeBox.m_CanConform; }
	virtual	INT					OtExBox_GetItemNum(){return (INT)m_OtherExchangeBox.m_ItemList.size(); }

	INT							OtExBox_GetPetByGuid(PET_GUID_t PetGuid);
	VOID						OtExBox_SetPet(INT nIndex, SDATA_PET* pPetData, BOOL bClearOld = TRUE);
	SDATA_PET*					OtExBox_GetPet(INT nIndex);
	INT							OtExBox_GetCount(VOID);
	VOID						OtExBox_SetSkill(INT nIndex,INT nSkillIndex,const _OWN_SKILL *Skill);
	VOID						OtExBox_SetSkill(INT nIndex, INT nSkillIndex, PET_SKILL* pPetSkillData, BOOL bClearOld);
	PET_SKILL*					OtExBox_GetSkill(INT nIndex,INT nSkillIndex);

	virtual	VOID				SetExchangObjID(INT ObjID){m_CurExID = ObjID;}				//��ǰ���׶Է�ID
	virtual	INT					GetExchangObjID()	{ return m_CurExID;}				//��ǰ���׶Է�ID

	virtual	VOID				AddToAppList(INT appid);				//���뵽�������б���
	virtual	INT					GetFromAppList();						//���������б���ȡ��һ��
	virtual	BOOL				IsStillAnyAppInList();					//�������б��Ƿ��

/*
	//------------------------------------------------------
	// ��ʯ�ϳ�/��Ƕ���������
	//------------------------------------------------------
	virtual VOID				EnchasePage_Clear(VOID);
	virtual VOID				EnchasePage_SetItem(INT nEnchaseIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	virtual tObject_Item*		EnchasePage_GetItem(INT nEnchaseIndex);
*/
/*�Ժ��ټ�
	virtual VOID				AppendItem_Clear(VOID);					//��ո�����Ʒ���б�
	virtual VOID				AppendItem_SetItem(INT nIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);//���ø�����Ʒ���б�
	virtual tObject_Item*		AppendItem_GetItem(INT nIndex);			//ȡ�ø�����Ʒ���б�ĵ�i����Ʒ��
*/

	virtual VOID				MissionBox_Clear(VOID);					//��յݽ�������Ʒ���б�
	virtual VOID				MissionBox_SetItem(INT nMissionBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);//���õݽ�������Ʒ���б�
	virtual tObject_Item*		MissionBox_GetItem(INT nMissionBoxIndex);//ȡ�õݽ�������Ʒ���б�

	virtual VOID				QuestLogBox_Clear(VOID);							//���������Ʒ���б�
	virtual VOID				QuestLogBox_SetItem(INT nQuestIndex, INT nItemIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);//����������Ʒ���б�
	virtual tObject_Item*		QuestLogBox_GetItem(INT nQuestIndex, INT nItemIndex);	//ȡ��������Ʒ���б�

	VOID						Copy_To_TargetEquip(CObject* pObj){ m_pTargetEquip = pObj; }
	CObject*					GetTargetEquip(VOID) { return m_pTargetEquip;}
	//------------------------------------------------------
	// ̯λ�����ݷ���
	//------------------------------------------------------
	virtual VOID				MyStallBox_SetDefaultPage(INT nPage){m_MyStallBox.m_nDefaultPage = nPage;}
	virtual INT					MyStallBox_GetDefaultPage(){return m_MyStallBox.m_nDefaultPage;}
	virtual VOID				MyStallBox_SetPosTax(UINT unPosTax){m_MyStallBox.m_PosTax = unPosTax;}//����̯λ��
	virtual UINT				MyStallBox_GetPosTax(VOID){return m_MyStallBox.m_PosTax;}//���̯λ��
	virtual VOID				MyStallBox_SetTradeTax(UINT unTradeTax){m_MyStallBox.m_TradeTax = unTradeTax;}//���ý���˰
	virtual UINT				MyStallBox_GetTradeTax(VOID){return m_MyStallBox.m_TradeTax;}//��ý���˰

	virtual VOID				MyStallBox_Clear(VOID);					//����Լ�̯λ��
	virtual VOID				MyStallBox_SetIsOpen(BOOL IsOpen){ m_MyStallBox.m_bIsOpen = IsOpen; }//����̯��ID
	virtual BOOL				MyStallBox_GetIsOpen(){ return m_MyStallBox.m_bIsOpen; }//���̯��ID
	virtual VOID				MyStallBox_SetObjID(UINT nObjID){ m_MyStallBox.m_ObjID = nObjID; }//����̯��ID
	virtual UINT				MyStallBox_GetObjID(){ return m_MyStallBox.m_ObjID; }//���̯��ID
	virtual VOID				MyStallBox_SetStallName(LPCTSTR szName) {m_MyStallBox.m_szStallName = szName;}//����̯λ��
	virtual LPCTSTR				MyStallBox_GetStallName(){ return m_MyStallBox.m_szStallName.c_str(); }//���̯λ��
	virtual VOID				MyStallBox_SetOwnerName(LPCTSTR szName) {m_MyStallBox.m_szOwnerName = szName;}//����̯����
	virtual LPCTSTR				MyStallBox_GetOwnerName(){ return m_MyStallBox.m_szOwnerName.c_str(); }//���̯����
	virtual VOID				MyStallBox_SetFirstPage(UINT nFirstPage) {m_MyStallBox.m_nFirstPage = nFirstPage;}//������ҳ
	virtual UINT				MyStallBox_GetFirstPage(){ return m_MyStallBox.m_nFirstPage; }//�����ҳ
	virtual VOID				MyStallBox_SetItem(INT nStallBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);//����̯λ�е���Ʒ
	virtual VOID				MyStallBox_SetItemPrice(INT nStallBoxIndex, UINT nPrice);//����̯λ��Ʒ�۸�
	virtual VOID				MyStallBox_SetItemSerial(INT nStallBoxIndex, UINT nSerial);//����̯λ��Ʒ���к�
	virtual tObject_Item*		MyStallBox_GetItem(INT nStallBoxIndex);//���̯λ��ָ��λ�õ���Ʒ
	virtual UINT				MyStallBox_GetItemPrice(INT nStallBoxIndex);//�����Ʒ�۸�
	virtual UINT				MyStallBox_GetItemSerial(INT nStallBoxIndex);//�����Ʒ���к�
	virtual	VOID				MyStallBox_SetItemExtraInfo(INT nStallBoxIndex, BOOL bEmpty, const _ITEM* pItem);//������Ʒ��ϸ��Ϣ
	virtual	INT					MyStallBox_GetItemNum(){return (INT)m_MyStallBox.m_ItemList.size(); }//�����Ʒ����
	virtual	BOOL				MyStallBox_IsOpen(){return (INT)m_MyStallBox.m_bIsOpen; }//��ѯ�Լ��Ƿ��ڰ�̯״̬
	virtual VOID				MyStallBox_SetCurSelectItem(tObject_Item* pItem){m_CurSelectItem.pItem = pItem;}//��ǰѡ�е���Ʒ
	virtual tObject_Item*		MyStallBox_GetCurSelectItem(){return m_CurSelectItem.pItem;}//��ǰѡ�е���Ʒ
	virtual VOID				MyStallBox_SetSelectItemInPos(INT nSelectItemInPos){m_nSelectItemInPos = nSelectItemInPos;}//��ǰѡ�е���Ʒ���ڵ�����
	virtual INT					MyStallBox_GetSelectItemInPos(){return m_nSelectItemInPos;}//��ǰѡ�е���Ʒ���ڵ�����
	virtual	INT					MyStallBox_GetItemIndexByGUID(__int64 ItemGUID);//����GUID������Ʒλ��
	virtual	UINT				MyStallBox_GetOwnerGUID() {return m_MyStallBox.m_OtGUID;}
	virtual	VOID				MyStallBox_SetOwnerGUID(UINT GUID){m_MyStallBox.m_OtGUID = GUID;}

	virtual INT					MyStallBox_GetnSelectPetOnStall(){return m_nSelectPetOnStall;}//��ǰѡ�е���Ʒ���ڵ�����
	virtual	VOID				MyStallBox_SetnSelectPetOnStall(INT nSelectPetOnStall){m_nSelectPetOnStall = nSelectPetOnStall;};//����GUID������Ʒλ��
	virtual	BOOL				MyStallBox_IsHaveEmpty(INT nType);

	//��̯����
	INT							MyStallBox_GetPetByGuid(PET_GUID_t PetGuid);
	VOID						MyStallBox_SetPet(INT nIndex, SDATA_PET* pPetData, BOOL bClearOld = TRUE);
	VOID						MyStallBox_SetPetPrice(INT nIndex, INT nPetPrice);
	VOID						MyStallBox_SetPetSerial(INT nIndex, INT nSerial);
	INT							MyStallBox_GetPetPrice(INT nIndex);
	INT							MyStallBox_GetPetSerial(INT nIndex);
	SDATA_PET*					MyStallBox_GetPet(INT nIndex);
	INT							MyStallBox_GetCount(VOID);
	VOID						MyStallBox_SetSkill(INT nIndex,INT nSkillIndex,const _OWN_SKILL *Skill);
	PET_SKILL*					MyStallBox_GetSkill(INT nIndex,INT nSkillIndex);
	VOID						MyStallBox_GetSelectpetGuid(PET_GUID_t &CurSelectpetGuid)
	{
								CurSelectpetGuid.m_uHighSection = m_CurSelectpetGuid.m_uHighSection;
								CurSelectpetGuid.m_uLowSection  = m_CurSelectpetGuid.m_uLowSection;
	};           
	VOID						MyStallBox_SetSelectpetGuid(PET_GUID_t CurSelectpetGuid)
	{
								m_CurSelectpetGuid.m_uHighSection = CurSelectpetGuid.m_uHighSection;
								m_CurSelectpetGuid.m_uLowSection  = CurSelectpetGuid.m_uLowSection;
	}
	virtual	BOOL				MyStallBox_IsPetOnStall(PET_GUID_t PetGuid);

	virtual	VOID				MyStallMsg_ClearUp(VOID);
	virtual	VOID				MyStallMsg_SetSerial( UINT nSerial) {m_MyStallBox.m_BBS.m_nSerial = nSerial;}
	virtual	INT					MyStallMsg_SetIDByIndex(UINT nIndex, UINT nID);
	virtual	INT					MyStallMsg_SetTimeByIndex(UINT	nIndex, BYTE nHour, BYTE nMin);
	virtual	INT					MyStallMsg_SetHasReplyByIndex(UINT	nIndex, BYTE bhasReply);
	virtual	INT					MyStallMsg_SetReTimeByIndex(UINT nIndex, BYTE nHour, BYTE nMin);
	virtual	VOID				MyStallMsg_SetTitle(LPCTSTR szMsg);
	virtual	INT					MyStallMsg_SetAuthorNameByIndex(UINT	nIndex, LPCTSTR szMsg);
	virtual	INT					MyStallMsg_SetProposedMessageByIndex(UINT	nIndex, LPCTSTR szMsg);
	virtual	INT					MyStallMsg_SetReplyMessageByIndex(UINT nIndex, LPCTSTR szMsg);
	virtual	UINT				MyStallMsg_GetSerial() {return m_MyStallBox.m_BBS.m_nSerial;}
	virtual	INT					MyStallMsg_GetIDByIndex(UINT nIndex);
	virtual	BYTE				MyStallMsg_GetHasReplyByIndex(UINT nIndex);
	virtual	LPCTSTR				MyStallMsg_GetTitle();
	virtual	LPCTSTR				MyStallMsg_GetAuthorNameByIndex(UINT nIndex);
	virtual	LPCTSTR				MyStallMsg_GetProposedMessageByIndex(UINT nIndex );
	virtual	LPCTSTR				MyStallMsg_GetReplyMessageByIndex(UINT nIndex);
	virtual	INT					MyStallMsg_GetMessageNum();
	virtual	VOID				MyStallMsg_SetMessageNum(INT nNum);
	virtual	INT					MyStallMsg_GetHourByIndex(UINT	nIndex);
	virtual	INT					MyStallMsg_GetMinByIndex(UINT	nIndex);
	virtual	INT					MyStallMsg_GetReHourByIndex(UINT	nIndex);
	virtual	INT					MyStallMsg_GetReMinByIndex(UINT	nIndex);


	virtual VOID				OtStallBox_SetDefaultPage(INT nPage){m_OtStallBox.m_nDefaultPage = nPage;}
	virtual INT					OtStallBox_GetDefaultPage(){return m_OtStallBox.m_nDefaultPage;}
	virtual VOID				OtStallBox_SetTradeTax(UINT unTradeTax){m_OtStallBox.m_TradeTax = unTradeTax;}//���ý���˰
	virtual UINT				OtStallBox_GetTradeTax(VOID){return m_OtStallBox.m_TradeTax;}//��ý���˰

	virtual VOID				OtStallBox_Clear(BYTE bFlagClearPet = TRUE);//��նԷ�̯λ��
	virtual VOID				OtStallBox_SetIsOpen(BOOL IsOpen){ m_OtStallBox.m_bIsOpen = IsOpen; }//����̯��ID
	virtual BOOL				OtStallBox_GetIsOpen(){ return m_OtStallBox.m_bIsOpen; }//���̯��ID
	virtual VOID				OtStallBox_SetObjID(UINT nObjID){ m_OtStallBox.m_ObjID = nObjID; }//����̯��ID
	virtual UINT				OtStallBox_GetObjID(){ return m_OtStallBox.m_ObjID; }//���̯��ID
	virtual VOID				OtStallBox_SetStallName(LPCTSTR szName) {m_OtStallBox.m_szStallName = szName;}//����̯λ��
	virtual LPCTSTR				OtStallBox_GetStallName(){ return m_OtStallBox.m_szStallName.c_str(); }//���̯λ��
	virtual VOID				OtStallBox_SetOwnerName(LPCTSTR szName) {m_OtStallBox.m_szOwnerName = szName;}//����̯����
	virtual LPCTSTR				OtStallBox_GetOwnerName(){ return m_OtStallBox.m_szOwnerName.c_str(); }//���̯����
	virtual VOID				OtStallBox_SetFirstPage(UINT nFirstPage) {m_OtStallBox.m_nFirstPage = nFirstPage;}//����̯λ��ҳ
	virtual UINT				OtStallBox_GetFirstPage(){ return m_OtStallBox.m_nFirstPage; }//���̯λ��ҳ
	virtual VOID				OtStallBox_SetItem(INT nExBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);//����̯λ��Ʒ
	virtual VOID				OtStallBox_SetItemPrice(INT nStallBoxIndex, UINT nPrice);//������Ʒ�۸�
	virtual VOID				OtStallBox_SetItemSerial(INT nStallBoxIndex, UINT nSerial);//������Ʒ���к�
	virtual tObject_Item*		OtStallBox_GetItem(INT nExBoxIndex);//����ƶ�λ�õ���Ʒ
	virtual UINT				OtStallBox_GetItemPrice(INT nStallBoxIndex);//�����Ʒ�۸�
	virtual UINT				OtStallBox_GetItemSerial(INT nStallBoxIndex);//�����Ʒ���к�
	virtual	VOID				OtStallBox_SetItemExtraInfo(INT nExBoxIndex, BOOL bEmpty, const _ITEM* pItem);//������Ʒ��ϸ��Ϣ
	virtual	INT					OtStallBox_GetItemNum(){return (INT)m_MyStallBox.m_ItemList.size(); }//���̯λ�е���Ʒ����
	virtual	INT					OtStallBox_GetItemIndexByGUID(__int64 ItemGUID);//����GUID������Ʒλ��

	virtual	UINT				OtStallBox_GetOwnerGUID() {return m_OtStallBox.m_OtGUID;}
	virtual	VOID				OtStallBox_SetOwnerGUID(UINT GUID){m_OtStallBox.m_OtGUID = GUID;}

	//��̯����
	INT							OtStallBox_GetPetByGuid(PET_GUID_t PetGuid);
	VOID						OtStallBox_SetPet(INT nIndex, SDATA_PET* pPetData, BOOL bClearOld = TRUE);
	VOID						OtStallBox_SetPetPrice(INT nIndex, INT nPetPrice);
	VOID						OtStallBox_SetPetSerial(INT nIndex, INT nSerial);
	INT							OtStallBox_GetPetPrice(INT nIndex);
	INT							OtStallBox_GetPetSerial(INT nIndex);
	SDATA_PET*					OtStallBox_GetPet(INT nIndex);
	INT							OtStallBox_GetCount(VOID);
	VOID						OtStallBox_SetSkill(INT nIndex,INT nSkillIndex,const _OWN_SKILL *Skill);
	PET_SKILL*					OtStallBox_GetSkill(INT nIndex,INT nSkillIndex);

	virtual	VOID				OtStallMsg_ClearUp(VOID);
	virtual	VOID				OtStallMsg_SetSerial( UINT nSerial) {m_OtStallBox.m_BBS.m_nSerial = nSerial;}
	virtual	INT					OtStallMsg_SetIDByIndex(UINT nIndex, UINT nID);
	virtual	INT					OtStallMsg_SetTimeByIndex(UINT	nIndex, BYTE nHour, BYTE nMin);
	virtual	INT					OtStallMsg_SetHasReplyByIndex(UINT	nIndex, BYTE bhasReply);
	virtual	INT					OtStallMsg_SetReTimeByIndex(UINT nIndex, BYTE nHour, BYTE nMin);
	virtual	VOID				OtStallMsg_SetTitle(LPCTSTR szMsg);
	virtual	INT					OtStallMsg_SetAuthorNameByIndex(UINT	nIndex, LPCTSTR szMsg);
	virtual	INT					OtStallMsg_SetProposedMessageByIndex(UINT	nIndex, LPCTSTR szMsg);
	virtual	INT					OtStallMsg_SetReplyMessageByIndex(UINT nIndex, LPCTSTR szMsg);
	virtual	UINT				OtStallMsg_GetSerial() {return m_OtStallBox.m_BBS.m_nSerial;}
	virtual	INT					OtStallMsg_GetIDByIndex(UINT nIndex);
	virtual	BYTE				OtStallMsg_GetHasReplyByIndex(UINT nIndex);
	virtual	LPCTSTR				OtStallMsg_GetTitle();
	virtual	LPCTSTR				OtStallMsg_GetAuthorNameByIndex(UINT nIndex);
	virtual	LPCTSTR				OtStallMsg_GetProposedMessageByIndex(UINT nIndex );
	virtual	LPCTSTR				OtStallMsg_GetReplyMessageByIndex(UINT nIndex);
	virtual	INT					OtStallMsg_GetHourByIndex(UINT	nIndex);
	virtual	INT					OtStallMsg_GetMinByIndex(UINT	nIndex);
	virtual	INT					OtStallMsg_GetReHourByIndex(UINT	nIndex);
	virtual	INT					OtStallMsg_GetReMinByIndex(UINT	nIndex);
	virtual	INT					OtStallMsg_GetMessageNum();
	virtual	VOID				OtStallMsg_SetMessageNum(INT nNum);									   
	//------------					   
	//ѧϰ�������������ķ����
	//------------
	//�����ķ���ʦ��ID
	//VOID						StudyXinfa_SetMenpaiInfo(INT idXinfaTeacher){m_idXinfaTeacher = idXinfaTeacher;}
	//INT							StudyXinfa_GetMenpaiInfo(VOID){return m_idXinfaTeacher;}
	//VOID						StudyXinfa_SetXinfaTeacherId(INT nXinfaTeacherId){m_nXinfaTeacherId = nXinfaTeacherId;}
	//INT							StudyXinfa_GetXinfaTeacherId(VOID){return m_nXinfaTeacherId;}
	//����������ͼ������ɵ��¼�
	VOID					SendStudySkillEvent(INT nSkillIDandLEVEL,INT NPCID,INT HaveUPLevelSkill);
	INT						MStudySkill(INT nSkillID,INT nSkillLEVEL);

	//------------
	//����ܽ�ʦ
	//------------
	INT							StudyAbility_GetID(VOID) { return m_nStudyAbility_ID; }
	VOID						StudyAbility_SetID(INT nAbilityID) { m_nStudyAbility_ID = nAbilityID; }

	INT							StudyAbility_GetNeedMoney(VOID) { return m_uStudyAbility_NeedMoney; }
	VOID						StudyAbility_SetNeedMoney(INT uNeedMoney) { m_uStudyAbility_NeedMoney = uNeedMoney; }

	INT							StudyAbility_GetNeedExp(VOID) { return m_uStudyAbility_NeedExp; }
	VOID						StudyAbility_SetNeedExp(INT uNeedExp) { m_uStudyAbility_NeedExp = uNeedExp; }

	INT							StudyAbility_GetSkillExp(VOID) { return m_uStudyAbility_SkillExp; }
	VOID						StudyAbility_SetSkillExp(INT uSkillExp) { m_uStudyAbility_SkillExp = uSkillExp; }

	INT							StudyAbility_GetLevelLimit(VOID) { return m_nStudyAbility_LevelLimit;}
	VOID						StudyAbility_SetLevelLimit(INT nStudyAbility_LevelLimit) { m_nStudyAbility_LevelLimit = nStudyAbility_LevelLimit;}

	INT							StudyAbility_GetScriptId(VOID) { return m_uStudyAbility_ScriptId;}
	VOID						StudyAbility_SetScriptId(INT uStudyAbility_ScriptId) { m_uStudyAbility_ScriptId = uStudyAbility_ScriptId;}

	INT							StudyAbility_GetNpcId(VOID) { return m_StudyAbility_NpcId;}
	VOID						StudyAbility_SetNpcId(INT StudyAbility_NpcId) { m_StudyAbility_NpcId = StudyAbility_NpcId;}

	INT							Pet_GetPetByGuid(PET_GUID_t PetGuid);
	SDATA_PET*					Pet_GetPet(INT nIndex);
	SDATA_PET*					Pet_GetPet(PET_GUID_t guidPet);
	VOID						Pet_ClearPet(INT nIndex);
	INT							Pet_GetCount(VOID);
	INT							Pet_GetPetFoodType(PET_GUID_t guidPet);											//��ó����ʳ������
	VOID						Pet_GetPetFoodTypeMinMax(INT nFoodType, INT* idMin = NULL, INT* idMax = NULL);	//��ó�����Ӧʳ�����͵�idTable����
	tObject_Item*				Pet_GetLowestLevel_Food_From_Package(PET_GUID_t pg, INT *idxPackage = NULL);	//�ӱ������ι��������ͼ���ʳ��
	tObject_Item*				Pet_GetLowestLevel_Dome_From_Package(PET_GUID_t pg, INT *idxPackage = NULL);	//�ӱ������ѱ��������ͼ���ʳ��

	VOID						Pet_CopyToTarget(SDATA_PET* pSourcePet);

	SDATA_PET*					TargetPet_GetPet(VOID) { return &m_TargetPet; }
	VOID						TargetPet_Clear(VOID) { m_TargetPet.m_nIsPresent = -1; m_TargetPet.CleanUp();}
	VOID						TargetPet_SetPetModel(INT nRaceID);
	PET_SKILL*					TargetPet_GetSkill(INT nSkillIndex);
	VOID						TargetPet_SetSkill(INT nSkillIndex, const _OWN_SKILL *Skill);
	VOID						TargetPet_SetSkill(INT nSkillIndex, PET_SKILL* pPetSkillData, BOOL bClearOld = TRUE);

	VOID						PetStudySkill_SetPetModel(INT nRaceID);

	VOID						Pet_SetSkill(INT nIndex, INT nSkillIndex, const _OWN_SKILL *Skill);
	VOID						Pet_SetSkill(INT nIndex, INT nSkillIndex, PET_SKILL* pPetSkillData, BOOL bClearOld = TRUE);
	PET_SKILL*					Pet_GetSkill(INT nIndex,INT nSkillIndex);
	VOID						Pet_SetPetModel(INT nIndex);
	BOOL						Pet_SendUseSkillMessage( SkillID_t idSkill, ObjID_t idTargetObj, FLOAT fTargetX, FLOAT fTargetZ );

	//------------
	// ���ܵ�BUFF����
	//------------
	INT							BuffImpact_GetCount( VOID )const{ return (INT)(m_mapBuffImpact.size()); }
	const _BUFF_IMPACT_INFO*	BuffImpact_Get( INT nSN )const;
	const _BUFF_IMPACT_INFO*	BuffImpact_GetByIndex( INT nIndex )const;
	const _BUFF_IMPACT_INFO*	BuffImpact_GetByID( INT nID )const;
	BOOL						BuffImpact_Add( const _BUFF_IMPACT_INFO *pBuffImpact );
	VOID						BuffImpact_Remove( INT nSN );
	VOID						BuffImpact_RemoveByIndex( INT nIndex );
	VOID						BuffImpact_RemoveAll( VOID );
	const CHAR*					BuffImpact_GetToolTips( INT nSN )const;
	const CHAR*					BuffImpact_GetToolTipsByIndex( INT nIndex )const;
	BOOL						BuffImpact_Dispel( INT nSN );
	BOOL						BuffImpact_DispelByIndex( INT nIndex );
	INT							BuffImpact_GetTime( INT nIndex );
	INT							BuffImpact_GetTimeByIndex( INT nIndex );


	//��ǰ��Ҫ������Ʒ
	INT							DisCard_GetItemConta(){return m_CurDisCardItem.m_nContainer;}
	INT							DisCard_GetItemPos(){return m_CurDisCardItem.m_nPosition;}
	VOID						DisCard_SetItemConta(INT nContainer){m_CurDisCardItem.m_nContainer = nContainer;}
	VOID						DisCard_SetItemPos(INT nPosition){m_CurDisCardItem.m_nPosition = nPosition;}


	X_PARAM						m_X_PARAM;
	VOID						X_PARAM_Set(X_PARAM *pBuf,INT nUIIndex);
	X_PARAM*					X_PARAM_Get( VOID ){return &m_X_PARAM;};

	X_SCRIPT					m_X_SCRIPT;
	X_SCRIPT*					X_XCRIPT_Get( VOID ){return &m_X_SCRIPT;};


	//��ǰ���ڲ�ֵ���Ʒ
	INT							Split_GetItemConta(){return m_CurSplitItem.m_nContainer;}
	INT							Split_GetItemPos(){return m_CurSplitItem.m_nPosition;}
	INT							Split_GetSum(){return m_CurSplitItem.m_nSum;}
	INT							Split_GetNumber(){return m_CurSplitItem.m_nNumber;}
	VOID						Split_SetItemConta(INT nContainer){m_CurSplitItem.m_nContainer = nContainer;}
	VOID						Split_SetItemPos(INT nPosition){m_CurSplitItem.m_nPosition = nPosition;}
	VOID						Split_SetSum(INT nSum){m_CurSplitItem.m_nSum = nSum;}
	VOID						Split_SetNumber(INT nNumber){m_CurSplitItem.m_nNumber = nNumber;}

	//���������ͺ�����
	VOID						Relation_ClearFriend();	//��պ����б�
	VOID						Relation_ClearBlack();	//��պ�����

	UINT						Relation_GetFriendNum()	{return m_uFriendCount;}
	UINT						Relation_GetBlackNum()	{return m_uBlackCount;}
	VOID						Relation_SetFriendNum(UINT uFriendCount){m_uFriendCount = uFriendCount;}
	VOID						Relation_SetBlackNum(UINT uBlackCount)	{m_uBlackCount = uBlackCount;}

	VOID						Relation_AddFriend(INT index, _OWN_RELATION* pRelation);	//���һ������
	VOID						Relation_AddBlack(INT index, _OWN_RELATION* pRelation);		//���һ��������

	VOID						Relation_DelFriend();	//ɾ��һ������
	VOID						Relation_DelBlack();	//ɾ��һ��������

	BOOL						Relation_GetFriend(INT index, _OWN_RELATION* pRelation);
	BOOL						Relation_GetBlack(INT index, _OWN_RELATION* pRelation);
	//------------------------------------------------------
	// ����̵����ݷ���
	//------------------------------------------------------
	BOOL						PlayerShop_GetMeIsOwner(){return m_bMeIsOwner;}
	VOID						PlayerShop_SetMeIsOwner(BOOL bOwner){m_bMeIsOwner = bOwner;}

	INT							PlayerShop_GetNpcID(){return m_nPSNpcID;}
	VOID						PlayerShop_SetNpcID(INT nPSNpcID){m_nPSNpcID = nPSNpcID;}

	INT							PlayerShop_GetShopNum(){return m_nShopNum;}
	VOID						PlayerShop_SetShopNum(INT nShopNum){m_nShopNum = nShopNum;}

	FLOAT						PlayerShop_GetCommercialFactor(){return m_CommerFactor;}
	VOID						PlayerShop_SetCommercialFactor(FLOAT fComFactor){ m_CommerFactor = fComFactor;}

	UINT						PlayerShop_GetCost(){return m_PlayerShopApplyInfo.m_Cost;}
	VOID						PlayerShop_SetCost(UINT uCost){m_PlayerShopApplyInfo.m_Cost = uCost;}

	BYTE						PlayerShop_GetType(){return m_PlayerShopApplyInfo.m_Type;}
	VOID						PlayerShop_SetType(BYTE nType){m_PlayerShopApplyInfo.m_Type = nType;}

	LPCTSTR						PlayerShop_GetShopNameByIndex(UINT uIndex);
	VOID						PlayerShop_SetShopNameByIndex(UINT uIndex, LPCTSTR pszShopName);

	LPCTSTR						PlayerShop_GetShopDescByIndex(UINT uIndex);
	VOID						PlayerShop_SetShopDescByIndex(UINT uIndex, LPCTSTR pszShopDesc);

	LPCTSTR						PlayerShop_GetShopFoundedTimeByIndex(UINT uIndex);
	VOID						PlayerShop_SetShopFoundedTimeByIndex(UINT uIndex, LPCTSTR pszTime);

	BYTE						PlayerShop_GetStallNumOpened(UINT uIndex);
	VOID						PlayerShop_SetStallNumOpened(UINT uIndex, BYTE nStallNumOpened);

	BYTE						PlayerShop_GetStallNumOnSale(UINT uIndex);
	VOID						PlayerShop_SetStallNumOnSale(UINT uIndex, BYTE nStallNumOnSale);

	_PLAYERSHOP_GUID			PlayerShop_GetIDByIndex(UINT uIndex);
	VOID						PlayerShop_SetIDByIndex(UINT uIndex, _PLAYERSHOP_GUID nId);

	BYTE						PlayerShop_GetIsFavorByIndex(UINT uIndex);
	VOID						PlayerShop_SetIsFavorByIndex(UINT uIndex, BYTE bIsFavor);

	INT							PlayerShop_GetStallEnableByIndex(BOOL bIsMine, UINT nStallIndex);
	VOID						PlayerShop_SetStallEnableByIndex(BOOL bIsMine, UINT nStallIndex, BYTE bIsEnable);

	BYTE						PlayerShop_GetTypeByIndex(UINT uIndex);
	VOID						PlayerShop_SetTypeByIndex(UINT uIndex, BYTE nType);

	LPCTSTR						PlayerShop_GetOwnerNameByIndex(UINT uIndex);
	VOID						PlayerShop_SetOwnerNameByIndex(UINT uIndex, LPCTSTR pszOwnerName);

	UINT						PlayerShop_GetOwnerGuidByIndex(UINT uIndex);
	VOID						PlayerShop_SetOwnerGuidByIndex(UINT uIndex, UINT uOwnerGuid);

	VOID						PlayerShop_CleanUp(BOOL bIsMine);//�������̵�

	VOID						PlayerShop_SetShopID(BOOL bIsMine, _PLAYERSHOP_GUID nShopID);//�����̵�ID
	_PLAYERSHOP_GUID			PlayerShop_GetShopID(BOOL bIsMine);//����̵�ID

	VOID						PlayerShop_SetShopSerial(BOOL bIsMine, BYTE nShopSerial);//�����̵����к�
	BYTE						PlayerShop_GetShopSerial(BOOL bIsMine);//����̵����к�

	VOID						PlayerShop_SetItem(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);//����̯λ��Ʒ
	tObject_Item*				PlayerShop_GetItem(BOOL bIsMine, INT nStallIndex, INT nBoxIndex);//����ƶ�λ�õ���Ʒ

	VOID						PlayerShop_SetItemPrice(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, UINT nPrice);//������Ʒ�۸�
	UINT						PlayerShop_GetItemPrice(BOOL bIsMine, INT nStallIndex, INT nBoxIndex);//�����Ʒ�۸�

	VOID						PlayerShop_SetItemSerial(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, UINT nSerial);//������Ʒ���к�
	UINT						PlayerShop_GetItemSerial(BOOL bIsMine, INT nStallIndex, INT nBoxIndex);//�����Ʒ���к�

	VOID						PlayerShop_SetItemOnSale(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, BYTE bIsEnable);//�������Ƿ��ϼ�
	BYTE						PlayerShop_GetItemOnSale(BOOL bIsMine, INT nStallIndex, INT nBoxIndex);//�������Ƿ��ϼ�

	VOID						PlayerShop_SetBaseMoney(BOOL bIsMine, UINT uBaseMoney);//�����̵걾��
	UINT						PlayerShop_GetBaseMoney(BOOL bIsMine);//�����̵걾��

	VOID						PlayerShop_SetProfitMoney(BOOL bIsMine, UINT uProfitPrice);//�����̵�ӯ����
	UINT						PlayerShop_GetProfitMoney(BOOL bIsMine);//�����̵�ӯ����

	//ֻ���Լ��ĵ���Կ���
	INT							PlayerShop_GetPartnerNum(){return m_nPartnerNum;}
	VOID						PlayerShop_SetPartnerNum(INT nPartnerNum){m_nPartnerNum = nPartnerNum;}

	VOID						PlayerShop_SetPartnerByIndex(UINT uIndex, GUID_t Guid, LPCTSTR szName);//����ָ��λ�õĺ������
	PlayerShopPartners_t* 		PlayerShop_GetPartnerByIndex(UINT uIndex);//���ָ��λ�õĺ������

	VOID						PlayerShop_SetShopType(BOOL bIsMine, INT nShopType);//�����̵�����
	UINT						PlayerShop_GetShopType(BOOL bIsMine);//�����̵�����

	VOID						PlayerShop_SetIsSaleOut(BOOL bIsMine, BOOL bIsSaleOut);//�����̵���̵��Ƿ��ǳ���״̬
	BOOL						PlayerShop_GetIsSaleOut(BOOL bIsMine);//�����̵���̵��Ƿ��ǳ���״̬

	VOID						PlayerShop_SetSaleOutPrice(BOOL bIsMine, UINT uSaleOutPrice);//�����̵���̵��Ƿ��ǳ���״̬
	UINT						PlayerShop_GetSaleOutPrice(BOOL bIsMine);//����̵���̳���

	VOID						PlayerShop_SetOwnerGuid(BOOL bIsMine, UINT uOwnerGuid);//���õ���guid
	UINT						PlayerShop_GetOwnerGuid(BOOL bIsMine);//��õ���guid

	VOID						PlayerShop_SetExRecNum(BOOL bIsMine, UINT uExRecNum);//���ý��׼�¼����
	UINT						PlayerShop_GetExRecNum(BOOL bIsMine);//��ý��׼�¼����

	VOID						PlayerShop_SetMaRecNum(BOOL bIsMine, UINT uMaRecNum);//���ù����¼����
	UINT						PlayerShop_GetMaRecNum(BOOL bIsMine);//��ù����¼����

	VOID						PlayerShop_SetOpenStallNum(BOOL bIsMine, UINT uOpenStallNum);//�����Ѿ�ӵ�еĹ�̨��
	UINT						PlayerShop_GetOpenStalldNum(BOOL bIsMine);//����Ѿ�ӵ�еĹ�̨��

	VOID						PlayerShop_SetShopSubType(BOOL bIsMine, UINT uShopSubType);//�����̵������
	UINT						PlayerShop_GetShopSubType(BOOL bIsMine);//����̵������

	VOID						PlayerShop_SetOwnerName(BOOL bIsMine, LPCTSTR pszOwnerName);//���õ�����
	LPCTSTR						PlayerShop_GetOwnerName(BOOL bIsMine);//��õ�����

	VOID						PlayerShop_SetShopName(BOOL bIsMine, LPCTSTR pszShopName);//�����̵���
	LPCTSTR						PlayerShop_GetShopName(BOOL bIsMine);//����̵���

	VOID						PlayerShop_SetShopDesc(BOOL bIsMine, LPCTSTR pszShopDesc);//�����̵�����
	LPCTSTR						PlayerShop_GetShopDesc(BOOL bIsMine);//����̵���

	VOID						PlayerShop_SetItemExtraInfo(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, BOOL bEmpty, const _ITEM* pItem);//������Ʒ��ϸ��Ϣ
	INT							PlayerShop_GetItemNum(BOOL bIsMine, INT nStallIndex);//���̯λ�е���Ʒ����
	INT							PlayerShop_GetItemIndexByGUID(BOOL bIsMine, INT nStallIndex, __int64 ItemGUID);//����GUID������Ʒλ��

	SDATA_PET*					PlayerShop_GetPet(BOOL bIsMine, INT nStallIndex, INT nIndex);

	INT							PlayerShop_GetPetByGuid(BOOL bIsMine, INT nStallIndex, PET_GUID_t PetGuid);//����GUID������̵�ָ����̨�в�����Ʒ
	VOID						PlayerShop_SetPet(BOOL bIsMine, INT nStallIndex, INT nIndex, SDATA_PET* pPetData, BOOL bClearOld = TRUE);//��ָ����̨�����ó���
	VOID						PlayerShop_SetPetSerial(BOOL bIsMine, INT nStallIndex, INT nIndex, INT nSerial);//����ָ����̨�ĸ������к�
	INT							PlayerShop_GetPetSerial(BOOL bIsMine, INT nStallIndex, INT nIndex);//���ָ����̨�ĸ������к�
	VOID						PlayerShop_SetPetPrice(BOOL bIsMine, INT nStallIndex, INT nIndex, INT nPetPrice);
	INT							PlayerShop_GetPetPrice(BOOL bIsMine, INT nStallIndex, INT nIndex);
	VOID						PlayerShop_SetSkill(BOOL bIsMine, INT nStallIndex, INT nIndex, INT nSkillIndex, PET_SKILL* pPetSkillData, BOOL bClearOld = TRUE);	
	VOID						PlayerShop_SetSkill(BOOL bIsMine, INT nStallIndex, INT nIndex,INT nSkillIndex, const _OWN_SKILL *Skill);
	PET_SKILL*					PlayerShop_GetSkill(BOOL bIsMine, INT nStallIndex, INT nIndex,INT nSkillIndex);

	//���
	VOID						PlayerShop_CleanObjItem(BOOL bIsMine);

	//����̵���ѡ�е���Ʒ
	VOID						PlayerShop_MyClearSelect(){m_MyPsSelectItem.m_nConIndex = -1;m_MyPsSelectItem.m_nPosition = -1;}
	VOID						PlayerShop_OtClearSelect(){m_OtPsSelectItem.m_nConIndex = -1;m_OtPsSelectItem.m_nPosition = -1;}

	INT							PlayerShop_GetMySelectConTa(){return m_MyPsSelectItem.m_nConIndex;}
	INT							PlayerShop_GetMySelectPos(){return m_MyPsSelectItem.m_nPosition;}
	INT							PlayerShop_GetOtSelectConTa(){return m_OtPsSelectItem.m_nConIndex;}
	INT							PlayerShop_GetOtSelectPos(){return m_OtPsSelectItem.m_nPosition;}

	VOID						PlayerShop_SetMySelectConTa(INT nConIndex) {m_MyPsSelectItem.m_nConIndex = nConIndex;}
	VOID						PlayerShop_SetMySelectPos(INT nPosition){m_MyPsSelectItem.m_nPosition = nPosition;}
	VOID						PlayerShop_SetOtSelectConTa(INT nConIndex) {m_OtPsSelectItem.m_nConIndex = nConIndex;}
	VOID						PlayerShop_SetOtSelectPos(INT nPosition){m_OtPsSelectItem.m_nPosition = nPosition;}

	//---------------------------------------------
	//�����б�
	//---------------------------------------------
	VOID						Guild_ClearAllInfo();//������й�����Ϣ
	GuildInfo_t*				Guild_GetInfoByIndex(INT nIndex);//ͨ��������ù�����Ϣ
	VOID						Guild_SetInfoByIndex(INT nIndex, GuildInfo_t* pGuildInfo);//ͨ���������ù�����Ϣ

	INT							Guild_GetInfoNum(){return m_GuildNum;}//��ù�������
	VOID						Guild_SetInfoNum(INT iGuildNum){m_GuildNum = iGuildNum;}//���ù�������
	
	//---------------------------------------------
	//���ɳ�Ա�б�
	//---------------------------------------------
	VOID						Guild_ClearMemInfo();//������а�����Ϣ
	GuildMemberInfo_t*			Guild_GetMemInfoByIndex(INT nIndex);//ͨ��������ð�����Ϣ
	VOID						Guild_SetMemInfoByIndex(INT nIndex, GuildMemberInfo_t* pMemberInfo);//ͨ���������ð�����Ϣ

	INT							Guild_GetMemInfoNum(){return m_GuildMemNum;}//��ð�����Ϣ
	VOID						Guild_SetMemInfoNum(INT iMemNum){m_GuildMemNum = iMemNum;}//���ð�����Ϣ

	INT							Guild_GetMaxMemNum(){return m_GuildMaxMemNum;}//��ð��ڵ��������
	VOID						Guild_SetMaxMemNum(INT iMaxMemNum){m_GuildMaxMemNum = iMaxMemNum;}//���ð��ڵ��������

	LPCTSTR						Guild_GetDesc(){return m_GuildDesc.c_str();}//��ð�����ּ
	VOID						Guild_SetDesc(LPCTSTR pGuildDesc){m_GuildDesc = pGuildDesc;}//���ð�����ּ

	LPCTSTR						Guild_GetName(){return m_GuildName.c_str();}//��ð�����ּ
	VOID						Guild_SetName(LPCTSTR pGuildName){m_GuildName = pGuildName;}//���ð�����ּ

	BYTE						Guild_GetCurAccess(){return m_uAccess;}//��õ�ǰ����Ȩ��
	VOID						Guild_SetCurAccess(BYTE uAccess){m_uAccess = uAccess;}//���õ�ǰ����Ȩ��

	BYTE						Guild_GetCurPosition(){return m_uPosition;}//��õ�ǰ����ְλ
	VOID						Guild_SetCurPosition(BYTE uPosition){m_uPosition = uPosition;}//���õ�ǰ����ְλ


	//---------------------------------------------
	//���ɵ���ϸ��Ϣ
	//---------------------------------------------
	VOID						Guild_ClearDetailInfo();//��հ��ɵ���ϸ��Ϣ
	GuildDetailInfo_t*			Guild_GetDetailInfo();//��ð��ɵ���ϸ��Ϣ
	VOID						Guild_SetDetailInfo(GuildDetailInfo_t* pDetailInfo);//���ð��ɵ���ϸ��Ϣ


	//---------------------------------------------
	//���ɿ�����ְλ
	//---------------------------------------------
	VOID						Guild_ClearAppointInfo();//��հ��ɿ�����ְλ
	GuildAppointPos_t*			Guild_GetAppointInfoByIndex(INT nIndex);//��ð��ɿ�����ְλ
	VOID						Guild_SetAppointInfoByIndex( INT nIndex, GuildAppointPos_t* pDetailInfo);//���ð��ɿ�����ְλ


	//�������ɼ���ѧϰ���ݷ���
	VOID						PetSkillStudy_MenPaiList_AddSkill(INT skillId);
	VOID						PetSkillStudy_MenPaiList_Clear();
	PET_SKILL*					PetSkillStudy_MenPaiList_Get(INT idx);	//idx �� m_petSkillStudyList ������
	PET_SKILL*					PetSkillStudy_MenPaiList_Get_BySkillId(INT sdx); //sdx �� PET_SKILL�е� m_pDefine->m_nID


	//---------------------------------------------
	//���﹫���
	//---------------------------------------------
	INT							PetPlacard_GetItemCount(VOID)const;
	const _PET_PLACARD_ITEM		*PetPlacard_GetItem(INT nIndex)const;
	VOID						PetPlacard_AddItem(const _PET_PLACARD_ITEM *pItem);
	VOID						PetPlacard_CleanUp();

	//---------------------------------------------
	//�������
	//---------------------------------------------
	//virtual INT					PI_GetMyInfoType()	{return m_MyPrivateInfo.m_nInfoType;}
	//virtual INT					PI_GetMyAge()		{return m_MyPrivateInfo.m_nAge;}
	//virtual INT					PI_GetMySex()		{return m_MyPrivateInfo.m_nSex;}
	//virtual INT					PI_GetMyBloodType()	{return m_MyPrivateInfo.m_nBloodType;}
	//virtual INT					PI_GetMyYearAnimal(){return m_MyPrivateInfo.m_nYearAnimal;}
	//virtual INT					PI_GetMyConsella()	{return m_MyPrivateInfo.m_nConsella;}
	//virtual INT					PI_GetMyProvince()	{return m_MyPrivateInfo.m_nProvince;}

	//virtual LPCTSTR				PI_GetMyJobInfo()	{return m_MyPrivateInfo.m_szJobInfo.c_str();}
	//virtual LPCTSTR				PI_GetMySchoolInfo(){return m_MyPrivateInfo.m_szSchoolInfo.c_str();}
	//virtual LPCTSTR				PI_GetMyCity()		{return m_MyPrivateInfo.m_szCity.c_str();}
	//virtual LPCTSTR				PI_GetMyEmailInfo()	{return m_MyPrivateInfo.m_szEmailInfo.c_str();}
	//virtual LPCTSTR				PI_GetMyLuckWord()	{return m_MyPrivateInfo.m_szLuckWord.c_str();}

	//virtual INT					PI_GetOtInfoType()	{return m_OtPrivateInfo.m_nInfoType;}
	//virtual INT					PI_GetOtAge()		{return m_OtPrivateInfo.m_nAge;}
	//virtual INT					PI_GetOtSex()		{return m_OtPrivateInfo.m_nSex;}
	//virtual INT					PI_GetOtBloodType()	{return m_OtPrivateInfo.m_nBloodType;}
	//virtual INT					PI_GetOtYearAnimal(){return m_OtPrivateInfo.m_nYearAnimal;}
	//virtual INT					PI_GetOtConsella()	{return m_OtPrivateInfo.m_nConsella;}
	//virtual INT					PI_GetOtProvince()	{return m_OtPrivateInfo.m_nProvince;}

	//virtual LPCTSTR				PI_GetOtName()		{return m_OtPrivateInfo.m_szName.c_str();}
	//virtual LPCTSTR				PI_GetOtJobInfo()	{return m_OtPrivateInfo.m_szJobInfo.c_str();}
	//virtual LPCTSTR				PI_GetOtSchoolInfo(){return m_OtPrivateInfo.m_szSchoolInfo.c_str();}
	//virtual LPCTSTR				PI_GetOtCity()		{return m_OtPrivateInfo.m_szCity.c_str();}
	//virtual LPCTSTR				PI_GetOtEmailInfo()	{return m_OtPrivateInfo.m_szEmailInfo.c_str();}
	//virtual LPCTSTR				PI_GetOtLuckWord()	{return m_OtPrivateInfo.m_szLuckWord.c_str();}

	//virtual VOID				PI_SetMyInfoType(int nInfoType)		{m_MyPrivateInfo.m_nInfoType = nInfoType;}
	//virtual VOID				PI_SetMyAge(int nAge)				{m_MyPrivateInfo.m_nAge = nAge;}
	//virtual VOID				PI_SetMySex(int nSex)				{m_MyPrivateInfo.m_nSex = nSex;}
	//virtual VOID				PI_SetMyBloodType(int nBloodType)	{m_MyPrivateInfo.m_nBloodType = nBloodType;}
	//virtual VOID				PI_SetMyYearAnimal(int nYearAnimal)	{m_MyPrivateInfo.m_nYearAnimal = nYearAnimal;}
	//virtual VOID				PI_SetMyConsella(int nConsella)		{m_MyPrivateInfo.m_nConsella = nConsella;}
	//virtual VOID				PI_SetMyProvince(int nProvince)		{m_MyPrivateInfo.m_nProvince = nProvince;}

	//virtual VOID				PI_SetMyJobInfo(LPCTSTR szJobInfo)		{m_MyPrivateInfo.m_szJobInfo = szJobInfo;}
	//virtual VOID				PI_SetMySchoolInfo(LPCTSTR szSchoolInfo){m_MyPrivateInfo.m_szSchoolInfo = szSchoolInfo;}
	//virtual VOID				PI_SetMyCity(LPCTSTR szCity)			{m_MyPrivateInfo.m_szCity = szCity;}
	//virtual VOID				PI_SetMyEmailInfo(LPCTSTR szEmailInfo)	{m_MyPrivateInfo.m_szEmailInfo = szEmailInfo;}
	//virtual VOID				PI_SetMyLuckWord(LPCTSTR szLuckWord)	{m_MyPrivateInfo.m_szLuckWord = szLuckWord;}

	//virtual VOID				PI_SetOtInfoType(int nInfoType)		{m_OtPrivateInfo.m_nInfoType = nInfoType;}
	//virtual VOID				PI_SetOtAge(int nAge)				{m_OtPrivateInfo.m_nAge = nAge;}
	//virtual VOID				PI_SetOtSex(int nSex)				{m_OtPrivateInfo.m_nSex = nSex;}
	//virtual VOID				PI_SetOtBloodType(int nBloodType)	{m_OtPrivateInfo.m_nBloodType = nBloodType;}
	//virtual VOID				PI_SetOtYearAnimal(int nYearAnimal)	{m_OtPrivateInfo.m_nYearAnimal = nYearAnimal;}
	//virtual VOID				PI_SetOtConsella(int nConsella)		{m_OtPrivateInfo.m_nConsella = nConsella;}
	//virtual VOID				PI_SetOtProvince(int nProvince)		{m_OtPrivateInfo.m_nProvince = nProvince;}

	//virtual VOID				PI_SetOtName(LPCTSTR szName)			{m_OtPrivateInfo.m_szName = szName;}
	//virtual VOID				PI_SetOtJobInfo(LPCTSTR szJobInfo)		{m_OtPrivateInfo.m_szJobInfo = szJobInfo;}
	//virtual VOID				PI_SetOtSchoolInfo(LPCTSTR szSchoolInfo){m_OtPrivateInfo.m_szSchoolInfo = szSchoolInfo;}
	//virtual VOID				PI_SetOtCity(LPCTSTR szCity)			{m_OtPrivateInfo.m_szCity = szCity;}
	//virtual VOID				PI_SetOtEmailInfo(LPCTSTR szEmailInfo)	{m_OtPrivateInfo.m_szEmailInfo = szEmailInfo;}
	//virtual VOID				PI_SetOtLuckWord(LPCTSTR szLuckWord)	{m_OtPrivateInfo.m_szLuckWord = szLuckWord;}

	//virtual UINT				PI_GetMyCharGUID()	{return m_MyPrivateInfo.m_unCharGUID;}
	//virtual UINT				PI_GetOtCharGUID()	{return m_OtPrivateInfo.m_unCharGUID;}

	//virtual VOID				PI_SetMyCharGUID(UINT unCharGUID) {m_MyPrivateInfo.m_unCharGUID = unCharGUID;}
	//virtual VOID				PI_SetOtCharGUID(UINT unCharGUID) {m_OtPrivateInfo.m_unCharGUID = unCharGUID;}

	//virtual VOID				PI_SetObjId(INT nObjId){m_nObjId = nObjId;}
	//virtual INT					PI_GetObjId(){return m_nObjId;}
	
	RecordList_t*				GetRecordList(){return &m_RecordList;}


	//------------------	---------------------------
	//���ݴ洢			
	//------------------	---------------------------
protected:					
	//------------		
	//��ɫ���Գ�
	//------------
	//���н�ɫ���߼�����
	std::map< INT, CCharacterData* >	m_mapAllCharacterData;

	//------------
	//��ȴʱ����
	//------------
	std::vector< COOLDOWN_GROUP >		m_vCoolDownGroup;

	std::vector< COOLDOWN_GROUP >		m_vPetSkillCoolDownGroup;
	//------------
	//������ȴ
	//------------
	INT									m_nCommonCoolDown;
	//------------
	//����ʣ��ʱ����
	//------------
	std::vector< INT >					m_vQuestTimeGroup;
	//------------
	//����Լ�װ��
	//------------
	//���װ��ʵ������
	std::vector< tObject_Item* >		m_vUserEquip;

	//------------
	//����װ��
	//------------
	//����װ��ʵ������
	std::vector< tObject_Item* >		m_vOtherPlayerEquip;

	//------------
	//�����б����
	//------------
	//��������������Ʒ
	struct  QUESTLOG_BOX
	{
		std::vector<tObject_Item*>		m_ItemList;						//��ʾ���������Լ�����Ʒ
		VOID CleanUp();
	};
	std::vector< QUESTLOG_BOX >			m_vQuestLogItem;

	//------------
	//�鿴�Է���ɫ���Խ���
	//------------
	//�Է���Obj

	CObject*							m_pTargetEquip;

	//------------
	//���Ǳ����б�
	//------------
	//��ұ���װ��ʵ������
	std::vector< tObject_Item* >		m_vUserBag;
	//��ϸ�������
	std::set< INT >						m_setAskBagExtra;

	//------------
	//��ǰ�򿪵�����
	//------------
	//����ID
	INT									m_idItemBox;
	//������Ʒʵ������
	std::vector< tObject_Item* >		m_vItemBox;


	//------------
	//��ǰ�򿪵�����
	//------------
	INT									m_nBoothNumber;
	INT									m_nBoothSoldNumber;
	std::vector< tObject_Item* >		m_listBooth;
	std::vector< tObject_Item* >		m_listBoothSold;
	std::vector< UINT >					m_listSoldPrice;
	BOOL								m_bBoothWindowIsDirty;
//	BOOL								m_bIsRepairing;
	std::vector< UINT >					m_nlistPrice;
	INT									m_nRepairLevel;		// ����ȼ�
	INT									m_nBuyLevel;		// �չ��ȼ�
	INT									m_nRepairType;		// ��������
	INT									m_nBuyType;			// �̵���չ�����
	FLOAT								m_nRepairSpend;		// ������
	FLOAT								m_nRepairOkProb;	// ����ɹ�����
	FLOAT								m_fScale;			// �۸�ϵ��
	INT									m_nShopType;		// �̵�����

	INT									m_nCurrencyUnit;	// 
	INT									m_nSerialNum;		
	INT									m_nBuyMulti;		
	BOOL								m_bCallBack;

	// ��ǰShop��NpcId
	INT									m_nShopNpcId;		// 

	// ��ǰShop��NpcId
	UINT								m_nShopUniqueId;	// 

	//------------
	//��ǰ�򿪵�����
	//------------
	//����������Ʒ�б�
	std::vector< tObject_Item* >		m_listUserBank;
	//��ǰ�������Ѿ��򿪵�λ��
	INT									m_CurBankEndIndex;
	//��ǰ������Ǯ��
	INT									m_CurBankMoney;
	//��ǰ����Ԫ����
	INT									m_CurBankRMB;
	//��ǰ����NPC��ID
	INT									m_nBankNpcID;

	//------------
	// ��ϵϵͳ����
	//------------
	Relation*							m_pRelation;
	CMailPool*							m_pMailPool;

	//------------
	//���׺�
	//------------
	struct  CLIENT_EXCHANGE_BOX
	{
		BOOL							m_IsLocked;						//��ʾ������lockѡ���Ƿ���
		BOOL							m_CanConform;					//��ʾ�Ƿ���ʾȷ����ť
		UINT							m_Money;						//��ʾ���׵Ľ�Ǯ
		std::vector<tObject_Item*>		m_ItemList;						//��ʾ���������Լ�����Ʒ
		std::vector< SDATA_PET* >		m_Petlist;						//��ʾ���������Լ��ĳ���

		VOID CleanUp();
	};
	CLIENT_EXCHANGE_BOX					m_MyExchangeBox;				//�Լ��Ľ��׺�
	CLIENT_EXCHANGE_BOX					m_OtherExchangeBox;				//�Է��Ľ��׺�
	INT									m_CurExID;
	std::vector< INT >					m_listApplicantsID;				//���뽻������
	SHORT								m_pOut;							//�Ӷ�����ȡ����λ��ָ��
	SHORT								m_pIn;							//������е�λ��ָ��


	//��ʯ�ϳ�/��Ƕ��������
//	std::vector< tObject_Item* >		m_vPageEnchase;

	// װ���ϸ�����Ʒ�����飬�Ժ��ټ�
//	std::vector< tObject_Item* >		m_vAppendItem;					// ���ӵ���Ʒ��

	//------------
	//�����
	//------------
	std::vector<tObject_Item*>			m_MissionItemList;				//�ݽ�������Ʒ����

	//------------
	// BBS�ϵ�����
	//------------
	struct MessageEntry_t
	{
		STRING	AuthorName; 
		UINT	nID;
		BYTE	nHour;
		BYTE	nMin;
		BOOL	bHasReply;
		STRING	szMessage;
		BYTE	nReHour;
		BYTE	nReMin;
		STRING	szReplyMessage;

		MessageEntry_t()
		{
			AuthorName		=	"";		
			nID				=	0;
			nHour			=	0;
			nMin			=	0;
			bHasReply		=	0;
			szMessage		=	"";
			nReHour			=	0;
			nReMin			=	0;
			szReplyMessage	=	"";
		}
	};

	struct BBS_t
	{
		BBS_t()
		{
			CleanUp();
		}
		VOID	CleanUp();

		STRING							m_szTitle;
		UINT							m_nSerial;
		INT								m_nMessageNum;
		std::vector< MessageEntry_t >	m_MessageList;
		
	};

	//------------
	//̯λ��
	//------------
	enum STALL_DEFAULT_PAGE
	{
		ITEM_PAGE = 0,
		PET_PAGE,
	};

	struct STALL_ITEM
	{
		STALL_ITEM()
		{
			pItem	= NULL;
			nPrice	= 0;
			nSerial = 0;
			nbIsEnable	=	0;
		}
		tObject_Item*					pItem;							//��Ʒָ��
		UINT							nPrice;							//�۸�
        UINT							nSerial;						//���к�
		BYTE							nbIsEnable;						//�����Ʒ�Ƿ��ϼ�
	};

	struct STALL_PET
	{
		STALL_PET()
		{
			pPet	= NULL;
			nPrice	= 0;
			nSerial = 0;
			nbIsEnable	=	TRUE;
		}
		SDATA_PET*						pPet;							//����ָ��
		UINT							nPrice;							//�۸�
		UINT							nSerial;						//���к�
		BYTE							nbIsEnable;						//�����Ʒ�Ƿ��ϼ�
	};

	struct CLIENT_STALL_BOX
	{
		INT								m_nDefaultPage;					//ȱʡ��ҳ��
		UINT							m_ObjID;
		BOOL							m_bIsOpen;						//�Լ�̯λ�Ƿ��Ѿ��򿪣��Ƿ��ڽ���״̬��
		UINT							m_PosTax;						//̯λ��
		UINT							m_TradeTax;						//ÿ�ν���˰��
		STRING							m_szStallName;					//��ʾ̯λ��
		STRING							m_szOwnerName;					//��ʾ̯����
		UINT							m_nFirstPage;					//��ʼҳ
		std::vector< STALL_ITEM >	 	m_ItemList;						//��ʾ̯λ�е�������Ʒ
		std::vector< STALL_PET >	 	m_PetList;						//��ʾ̯λ�е����г���
		BBS_t							m_BBS;							//̯λ���԰�
		UINT							m_OtGUID;
		VOID							CleanUp();		
	};

	CLIENT_STALL_BOX					m_MyStallBox;					//�Լ���̯λ��
	CLIENT_STALL_BOX					m_OtStallBox;					//�Է���̯λ��
	STALL_ITEM							m_CurSelectItem;				//���ѡ�е���Ʒ
	INT									m_nSelectItemInPos;				//ѡ�е������Ʒ���ڵ�����
	PET_GUID_t							m_CurSelectpetGuid;				//ѡ�еȴ��ϼܵĳ����GUID
	INT									m_nSelectPetOnStall;			//��̯λ�ϱ�ѡ�еĳ�����


	//------------
	//����̵����
	//------------
	PLAYERSHOP_APPLY_INFO				m_PlayerShopApplyInfo;			//�����̵���Ϣ

	std::vector< PLAYERSHOP_INFO >	 	m_PlayerShopList;				//�̵��б�
	INT									m_nShopNum;

	struct PLAYERSHOP_STALL_BOX											//��ʾ̯λ�е�������Ʒ
	{
		INT								m_bIsEnable;
		std::vector< STALL_ITEM >	 	m_ItemList;
		std::vector< STALL_PET >	 	m_PetList;
		PLAYERSHOP_STALL_BOX()
		{
			m_bIsEnable = FALSE;
			STALL_ITEM	ItemInit;
			m_ItemList.resize(STALL_BOX_SIZE, ItemInit);
			STALL_PET	PetInit;
			m_PetList.resize(STALL_BOX_SIZE, PetInit);
		}
		VOID CleanUp()
		{
			m_bIsEnable = FALSE;
			for(INT i =0; i<STALL_BOX_SIZE; i++)
			{
				STALL_ITEM	ItemInit;
				m_ItemList[i] = ItemInit;

				STALL_PET	PetInit;
				m_PetList[i] = PetInit;
			}
		}
	};

	enum SHOP_TYPE
	{
		NO_SHOP = 0,
		ITEM_SHOP,
		PET_SHOP,
	};

	struct PLAYERSHOP_BOX												//��ʾ�̵��е����й�̨
	{
		std::vector< PLAYERSHOP_STALL_BOX >	m_StallList;
		UINT								m_BaseMoney;
		UINT								m_ProfitMoney;
		_PLAYERSHOP_GUID					m_ShopID;
		STRING								m_szShopName;				//��ʾ�̵���
		STRING								m_szShopDesc;				//��ʾ�̵�����
		STRING								m_szOwnerName;				//��ʾ������
		UINT								m_OwnerGuid;				//����GUID
		BYTE								m_bIsSaleOut;				//���̵��ѱ��̳�
		UINT								m_uSaleOutPrice;			//�̵���̳���
		BYTE								m_Serial;					//�̵�����к�
		INT									m_nShopType;				//�̵������
		BYTE								m_ExRecListNum;				//���׼�¼����
		BYTE								m_MaRecListNum;				//�����¼����
		BYTE								m_StallOpenNum;				//�Ѿ�ӵ�еĹ�̨��
		BYTE								m_ShopSubType;				//�̵�������
		std::vector< PlayerShopPartners_t > m_PartnerList;				//�ϻ����б�
		
		PLAYERSHOP_BOX()
		{
			m_BaseMoney		= 0;
			m_ProfitMoney	= 0;
			m_szShopName	=	"";
			m_szOwnerName	=	"";
			m_szShopDesc	=	"";
			m_OwnerGuid		=	0;
			m_bIsSaleOut	=	FALSE;
			m_uSaleOutPrice	=	0;
			m_Serial		=	0;
			m_ExRecListNum	=	0;
			m_MaRecListNum	=	0;
			m_StallOpenNum	=	0;
			PLAYERSHOP_STALL_BOX StallInit;
			m_StallList.resize(MAX_STALL_NUM_PER_SHOP, StallInit);
			PlayerShopPartners_t PartnerInit;
			m_PartnerList.resize(MAX_PARTNER_PER_SHOP, PartnerInit);
			m_nShopType		= NO_SHOP;
			m_ShopSubType	=	0;
		}
		VOID CleanUp()
		{
			m_BaseMoney		= 0;
			m_ProfitMoney	= 0;
			m_szShopName	=	"";
			m_szOwnerName	=	"";
			m_OwnerGuid		=	0;
			m_bIsSaleOut	=	FALSE;
			m_uSaleOutPrice	=	0;
			m_Serial		=	0;
			m_ExRecListNum	=	0;
			m_MaRecListNum	=	0;
			m_StallOpenNum	=	0;
			m_ShopID.Reset();
			m_nShopType		=  NO_SHOP;
			m_ShopSubType	=	0;
			for(INT i =0; i<MAX_STALL_NUM_PER_SHOP; i++)
			{
				m_StallList[i].CleanUp();
			}
			for(INT i =0; i<MAX_PARTNER_PER_SHOP; i++)
			{
				m_PartnerList[i].CleanUp();
			}
		}
	};

	PLAYERSHOP_BOX						m_MyPlayerShop;					//�Լ�������̵�
	PLAYERSHOP_BOX						m_OtPlayerShop;					//�����˵��̵�
	FLOAT								m_CommerFactor;					//��ҵָ��
	BOOL								m_bMeIsOwner;					//��־�Լ��ǲ��ǵ���
	INT									m_nPartnerNum;					//�����������

	struct SELECT_ITEM
	{
		INT								m_nConIndex;
		INT								m_nPosition;
		VOID							CleanUp()
		{
			m_nConIndex = -1;
			m_nPosition = -1;
		}
	};
	SELECT_ITEM							m_MyPsSelectItem;				//�Լ����̵������ѡ�е���Ʒ
	SELECT_ITEM							m_OtPsSelectItem;				//���˵��̵������ѡ�е���Ʒ

	//��ǰ����̵�NPC��ID
	INT									m_nPSNpcID;


	//�ͻ�����ʾ�İ����б�
	GuildInfo_t							m_GuildList[MAX_GUILD_SIZE];
	INT									m_GuildNum;

	//�����б�
	GuildMemberInfo_t					m_GuildMemList[USER_ARRAY_SIZE];
	INT									m_GuildMaxMemNum;
	INT									m_GuildMemNum;
	STRING								m_GuildDesc;
	STRING								m_GuildName;
	BYTE								m_uPosition;
	BYTE								m_uAccess;

	//���ɵ���ϸ��Ϣ
	GuildDetailInfo_t					m_GuildDetailInfo;

	//���������ɵ�ְλ��Ϣ
	GuildAppointPos_t					m_GuildPosAvail[GUILD_POSITION_SIZE];

	// ��Ҫ���ٵ���Ʒ��һ����ʱ������
	struct DISCARD_ITEM
	{
		INT								m_nContainer;
		INT								m_nPosition;
		VOID							CleanUp()
		{
			m_nContainer = -1;
			m_nPosition = -1;
		}
	};
	DISCARD_ITEM						m_CurDisCardItem;

	// �����Ϣ
	struct SPLIT_ITEM
	{
		INT								m_nContainer;
		INT								m_nPosition;
		INT								m_nSum;			//�ܸ���
		INT								m_nNumber;		//�ֳ����ĸ���
		VOID							CleanUp()
		{
			m_nSum		 = 0;
			m_nNumber	 = 0;
			m_nContainer = -1;
			m_nPosition	 = -1;
		}
	};
	SPLIT_ITEM							m_CurSplitItem;

	//�������
	//struct PRIVATE_INFO
	//{
	//	STRING		m_szName;				//��CleanUp�в�Ҫ���
	//	INT			m_nInfoType;			//���̶ܳ�
	//	UINT		m_unCharGUID;			//��ɫ���
	//	INT			m_nAge;					//����
	//	INT			m_nSex;					//�Ա�
	//	STRING		m_szJobInfo;			//ְҵ��Ϣ		[MAX_JOBINFO_LENGTH]
	//	STRING		m_szSchoolInfo;			//��ҵԺУ		[MAX_SCHOOLINFO_LENGTH]
	//	INT			m_nBloodType;			//Ѫ��
	//	INT			m_nYearAnimal;			//��Ф
	//	INT			m_nConsella;			//����
	//	INT			m_nProvince;			//ʡ��
	//	STRING		m_szCity;				//����			[MAX_CITY_LENGTH]
	//	STRING		m_szEmailInfo;			//�ʼ�			[MAX_EMAIL_LENGTH]
	//	STRING		m_szLuckWord;			//�������		[MAX_LUCKWORD_LENGTH]

	//	VOID		CleanUp()
	//	{
	//		m_nInfoType		= 0;
	//		m_unCharGUID	= 0;
	//		m_nAge			= 0;
	//		m_nSex			= 0;
	//		m_szJobInfo		= "";
	//		m_szSchoolInfo	= "";
	//		m_nBloodType	= 0;
	//		m_nYearAnimal	= 0;
	//		m_nConsella		= 0;
	//		m_nProvince		= 0;
	//		m_szCity		= "";
	//		m_szEmailInfo	= "";
	//		m_szLuckWord	= "";
	//	}
	//};
	//PRIVATE_INFO	m_MyPrivateInfo;
	//PRIVATE_INFO	m_OtPrivateInfo;
	INT				m_nObjId;

	//------------
	// ���׼�¼�����߲�����¼
	//------------
	RecordList_t						m_RecordList;

	//------------
	// �����ķ���ʦ
	//------------
	//�ķ���ʦ���ķ�ID
	INT									m_idXinfaTeacher;
	INT									m_nXinfaTeacherId;

	//------------
	// �������ʦ���������
	//------------
	//���̵�����ܵ�ID
	INT									m_nStudyAbility_ID;
	//��Ҫ���ѵĽ�Ǯ
	UINT								m_uStudyAbility_NeedMoney;	
	//��Ҫ���ĵľ���ֵ
	UINT								m_uStudyAbility_NeedExp;	
	//������Ҫ�ﵽ�ĵ�����ѧϰ�в������ģ�
	UINT								m_uStudyAbility_SkillExp;
	//ѧϰ����������Ҫ�ĵȼ�
	INT									m_nStudyAbility_LevelLimit;
	//ѧϰ������ܵķ������ű���
	UINT								m_uStudyAbility_ScriptId;
	//ѧϰ�����NpcServer ID
	UINT								m_StudyAbility_NpcId;

	std::vector< SDATA_PET >			m_listPet;

	SDATA_PET							m_TargetPet;

	//����Pet UI��ʾ�ĳ������
	CObject_PlayerNPC*					m_pPetAvatar;

	//����TargetPet UI��ʾ�ĳ������
	CObject_PlayerNPC*					m_pTargetPetAvatar;

	//����PetStudySkill UI��ʾ�ĳ������
	CObject_PlayerNPC*					m_pPetStudySkillAvatar;


	//------------
	// ���ܵ�BUFF����
	//------------
	typedef std::map< INT, _BUFF_IMPACT_INFO* >	CBuffImpactMap;
	CBuffImpactMap						m_mapBuffImpact;
	INT									m_nBuffImpactTime;



	UINT								m_uFriendCount;		// ������
	UINT								m_uBlackCount;		// ��������
	_FRIEND_INFO						m_FriendInfo[RELATION_BLACKNAME_OFFSET - RELATION_FRIEND_OFFSET];
	_BLACKNAME_INFO						m_BlackNameInfo[MAX_RELATION_SIZE - RELATION_BLACKNAME_OFFSET];

	//------------
	// �������ɼ���ѧϰ
	//------------
	std::vector<PET_SKILL>				m_petSkillStudyList;				//�������������ļ����б�ֻ��ѧϰ�������ɼ��ܵ�ʱ��ʹ��


	//------------
	// ���﹫���
	//------------
	INT									m_nPetPlacardItemCount;								// ��ǰ����
	_PET_PLACARD_ITEM					m_aPetPlacardItems[MAX_PETPLACARD_LIST_ITEM_NUM];	// ���﹫�������

	BYTE								m_bCharFirstLogin;
public:

	//---------------------------------------------------------------------------------------------------------------------------------
	// �������
	//----------------------------------------------------------------------------------------------------------------------------------
	// ����������2006-4-15
	void Identify_Lock(INT iBagPos);

	// ȡ������������2006��4��15
	void Identify_UnLock();

	// ��ǰ���������λ��
	INT	m_nIdentifyPosition;
	// ʹ�þ���ı�������λ��
	int m_iIdentifyInBagIndex;
	//��������ȼ���
	int m_iIdentifyLevel;

	//---------------------------------------------
	//���ļ̳�
	//---------------------------------------------
public:
	CDataPool();
	virtual ~CDataPool();
	static CDataPool*	GetMe(VOID) { return s_pMe; }

public:
	virtual VOID		Initial(VOID*);
	virtual VOID		Release(VOID);
	virtual VOID		Tick(VOID);

protected:
	static CDataPool*	s_pMe;

	WX_DECLARE_DYNAMIC( CDataPool );
};