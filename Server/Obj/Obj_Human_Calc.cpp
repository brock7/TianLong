#include "stdafx.h"

#include "Player.h"
#include "GameTable.h"
#include "ItemTable.h"
#include "TimeManager.h"
#include "GCDetailSkillList.h"
#include "GCOperateResult.h"
#include "Obj_Human.h"
#include "Config.h"
#include "LogicManager.h"
#include "HumanItemLogic.h"
#include "Scene.h"


using namespace Combat_Module::Skill_Module;
using namespace MenPai_Module;
using namespace Packets;

VOID Obj_Human::Levelup(VOID)
{
	__ENTER_FUNCTION
	SHORT nMenPai = GetMenPai();
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPai);
	if(NULL==pMenPaiLogic)
	{
		Assert("[Obj_Human::AttrLevel1Levelup]:Can't find MenPai Logic!");
		return;
	}
	if(100<=GetLevel())
	{
		return;
	}
	SetLevel(GetLevel()+1);
	getScene()->OnSceneHumanLevelUp( this ) ;
	SHORT nValue = 0;
	if(DEFAULT_WASHPOINT_LEVEL>GetLevel())
	{
		//�Զ��ӵ�
		//STR
		nValue = pMenPaiLogic->GetStrLevelupRefix(GetLevel());
		SetBaseStr(GetBaseStr()+nValue);
		//SPR
		nValue = pMenPaiLogic->GetSprLevelupRefix(GetLevel());
		SetBaseSpr(GetBaseSpr()+nValue);
		//CON
		nValue = pMenPaiLogic->GetConLevelupRefix(GetLevel());
		SetBaseCon(GetBaseCon()+nValue);
		//INT
		nValue = pMenPaiLogic->GetIntLevelupRefix(GetLevel());
		SetBaseInt(GetBaseInt()+nValue);
		//DEX
		nValue = pMenPaiLogic->GetDexLevelupRefix(GetLevel());
		SetBaseDex(GetBaseDex()+nValue);
	}
	else
	{
		//����ʣ�����
		nValue += Get_RemainPoints(); //ԭ��ûʹ�õ�ʣ�����
		//STR
		nValue += pMenPaiLogic->GetStrLevelupRefix(GetLevel());
		//SPR
		nValue += pMenPaiLogic->GetSprLevelupRefix(GetLevel());
		//CON
		nValue += pMenPaiLogic->GetConLevelupRefix(GetLevel());
		//INT
		nValue += pMenPaiLogic->GetIntLevelupRefix(GetLevel());
		//DEX
		nValue += pMenPaiLogic->GetDexLevelupRefix(GetLevel());
		Set_RemainPoints(nValue);
	}

	MarkAllIntAttrDirtyFlags();

	//SetMaxVigor(GetMaxVigor() + 5);

	//SetMaxEnergy(GetMaxEnergy() + 5);

	SetHP(GetMaxHP());
	SetMP(GetMaxMP());

	__LEAVE_FUNCTION;
}

//ת�����ɵ�ʱ����Ҫ���еĵ������¼���
VOID Obj_Human::ChangeMenpaiPoints(VOID)
{
	if(GetLevel() >= DEFAULT_WASHPOINT_LEVEL)
	{
		WashPoints();
		SetMP(GetMaxMP());
		SetHP(GetMaxHP());
		return;
	}

	INT nMenPai		=  GetMenPai();

	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPai);
	if(NULL==pMenPaiLogic)
	{
		Assert("[Obj_Human::ChangeMenpaiPoints]:Can't find MenPai Logic!");
		return;
	}
	SHORT nValue = 0;
	SHORT nStr = 0;
	SHORT nSpr = 0;
	SHORT nCon = 0;
	SHORT nInt = 0;
	SHORT nDex = 0;

	for(INT nLevel =0;nLevel <= GetLevel();nLevel++)
	{
		//Str
		nStr += pMenPaiLogic->GetStrLevelupRefix(nLevel);
		//Spr
		nSpr += pMenPaiLogic->GetSprLevelupRefix(nLevel);
		//Con
		nCon += pMenPaiLogic->GetConLevelupRefix(nLevel);
		//Int
		nInt += pMenPaiLogic->GetIntLevelupRefix(nLevel);
		//Dex
		nDex += pMenPaiLogic->GetDexLevelupRefix(nLevel);
	}
	//set attr1
	SetBaseStr(nStr);
	SetBaseSpr(nSpr);
	SetBaseCon(nCon);
	SetBaseInt(nInt);
	SetBaseDex(nDex);

	// ����Ҽ���HP��MP
	SetMP(GetMaxMP());
	SetHP(GetMaxHP());
}

VOID Obj_Human::WashPoints(VOID)
{
	INT nWashLevels = DEFAULT_WASHPOINT_LEVEL;
	INT nMenPai		=  GetMenPai();
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPai);
	if(NULL==pMenPaiLogic)
	{
		Assert("[Obj_Human::WashPoints]:Can't find MenPai Logic!");
		return;
	}
	SHORT nValue = 0;
	SHORT nStr = 0;
	SHORT nSpr = 0;
	SHORT nCon = 0;
	SHORT nInt = 0;
	SHORT nDex = 0;
	SHORT nRemainPoint = 0;
	SHORT nTotalPoint = 0;
	/// \block calculate total points
	//@{
	for(INT nLevel =0;nLevel<=GetLevel();nLevel++)
	{
		//Str
		nTotalPoint += pMenPaiLogic->GetStrLevelupRefix(nLevel);
		//Spr
		nTotalPoint += pMenPaiLogic->GetSprLevelupRefix(nLevel);
		//Con
		nTotalPoint += pMenPaiLogic->GetConLevelupRefix(nLevel);
		//Int
		nTotalPoint += pMenPaiLogic->GetIntLevelupRefix(nLevel);
		//Dex
		nTotalPoint += pMenPaiLogic->GetDexLevelupRefix(nLevel);
	}
	//@}
	
	for(INT nLevel =0;nLevel<DEFAULT_WASHPOINT_LEVEL;nLevel++)
	{
		//Str
		nStr += pMenPaiLogic->GetStrLevelupRefix(nLevel);
		//Spr
		nSpr += pMenPaiLogic->GetSprLevelupRefix(nLevel);
		//Con
		nCon += pMenPaiLogic->GetConLevelupRefix(nLevel);
		//Int
		nInt += pMenPaiLogic->GetIntLevelupRefix(nLevel);
		//Dex
		nDex += pMenPaiLogic->GetDexLevelupRefix(nLevel);
	}
	//set attr1
	SetBaseStr(nStr);
	SetBaseSpr(nSpr);
	SetBaseCon(nCon);
	SetBaseInt(nInt);
	SetBaseDex(nDex);

	//Remain Points
	nRemainPoint = nTotalPoint-nStr-nSpr-nCon-nInt-nDex;
	Set_RemainPoints(nRemainPoint);	
}

BOOL Obj_Human::ManualAttrPoint(CHAR_ATTR_LEVEL1 attrType)
{
	if(0>Get_RemainPoints())
	{
		AssertEx(FALSE,"[Obj_Human::ManualAttrPoint]: Assert at not enough remain point!");
		return FALSE;
	}
	switch(attrType)
	{
		case CATTR_LEVEL1_STR:
			SetBaseStr(GetBaseStr()+1);
			break;
		case CATTR_LEVEL1_SPR:
			SetBaseSpr(GetBaseSpr()+1);
			break;
		case CATTR_LEVEL1_CON:
			SetBaseCon(GetBaseCon()+1);
			break;
		case CATTR_LEVEL1_INT:
			SetBaseInt(GetBaseInt()+1);
			break;
		case CATTR_LEVEL1_DEX:
			SetBaseDex(GetBaseDex()+1);
			break;
		default:
			return FALSE;
			break;
	}
	Set_RemainPoints(Get_RemainPoints()-1);
	return TRUE;
}

BOOL Obj_Human::ManualAttrPoint(INT nStr, INT nSpr, INT nCon, INT nInt, INT nDex)
{
	if(0>nStr||0>nSpr||0>nCon||0>nInt||0>nDex)
	{
		return FALSE;
	}
	if((nStr+nSpr+nCon+nInt+nDex)>Get_RemainPoints())
	{
		AssertEx(FALSE,"[Obj_Human::ManualAttrPoint]: Assert at not enough remain point!");
		return FALSE;
	}
	//set attrs
	SetBaseStr(GetBaseStr()+nStr);
	SetBaseSpr(GetBaseSpr()+nSpr);
	SetBaseCon(GetBaseCon()+nCon);
	SetBaseInt(GetBaseInt()+nInt);
	SetBaseDex(GetBaseDex()+nDex);
	//update remain point
	Set_RemainPoints(Get_RemainPoints()-(nStr+nSpr+nCon+nInt+nDex));
	SendMsg_RefeshAttrib();
	return TRUE;
}

INT Obj_Human::Get_RemainPoints()
{
	return m_DB.GetRemainPoints();
}

VOID Obj_Human::ItemEffectFlush( )
{
__ENTER_FUNCTION

	//����Ʒ��Ӱ������ֵ���㣻
	memset( m_pItemEffect, 0, sizeof(_ITEM_EFFECT)*IATTRIBUTE_NUMBER );
	memset(m_EquipUseSkillEffect,0,sizeof(_ITEM_SKILL)*(HEQUIP_NUMBER+MAX_ITEM_SET_ATTR));	
	memset(m_EquipRandSkillEffect,0,sizeof(_ITEM_SKILL)*(HEQUIP_NUMBER+MAX_ITEM_SET_ATTR));	

	

	for( INT i=0; i<HEQUIP_NUMBER; i++ )
	{
		
		/*
		if( !m_DB.GetEquipDB()->IsSet( (HUMAN_EQUIP)i ) )//û��װ������Ʒ
			continue ;
		*/
		ItemContainer* pEquipContainer = GetEquipContain();
		if(NULL==pEquipContainer)
		{
			AssertEx(FALSE,"[Obj_Human::ItemEffectFlush]: NULL EquipContainer pointer found!");
			break;
		}
		Item*	pEquipItem = pEquipContainer->GetItem((UINT)i);
		if(NULL==pEquipItem)
		{
			AssertEx(FALSE,"[Obj_Human::ItemEffectFlush]: NULL EquipItem pointer found!");
			continue;
		}
		if(TRUE==pEquipItem->IsEmpty())
		{
			continue;//���λ���ǿյ�
		}
		if(0==pEquipItem->GetDurPoints())
		{
			continue;
		}
		//���λ��װ������Ʒ
		const	_ITEM* pCurItem = m_DB.GetEquipItem((HUMAN_EQUIP)i);
		// ��������
		INT nItemClass = pCurItem->ItemClass();
		// ��������
		INT nItemType = pCurItem->ItemType();
 		Assert(ICLASS_EQUIP==nItemClass) ; //ֻ��Ϊװ��������Ϊ�����Ʒ
		//ȡ��ÿ����Ʒ�ĸ�������
		for( INT j=0; j<pCurItem->GetEquipData()->m_AttrCount; j++ )
		{
			
			CaculateEffect(pCurItem->GetEquipData()->m_pAttr[j],(HUMAN_EQUIP)i, nItemType);
				
		}	
		
	}

	CaculateEquipSetEffect();
	MarkAllIntAttrDirtyFlags();//mark the int attributes dirty
__LEAVE_FUNCTION
}

VOID		Obj_Human::CaculateEffect(_ITEM_ATTR& Attr,INT EquipOffset, INT nItemType)
{
	__ENTER_FUNCTION

	SHORT				value;
	ITEM_ATTRIBUTE		type;

	value	=	Attr.m_Value.m_Value;
	type	=	(ITEM_ATTRIBUTE)Attr.m_AttrType;
	// Passive Skill refix
	if(INVALID_ID != nItemType)
	{
		switch(type) 
		{

		case IATTRIBUTE_BASE_ATTACK_P:			//����������
		case IATTRIBUTE_BASE_ATTACK_M:			//����ħ������
		case IATTRIBUTE_BASE_DEFENCE_P:			//�����������
		case IATTRIBUTE_BASE_DEFENCE_M:			//����ħ������
			{
				//passive skill refixs value here
				Skill_RefixItemAttr(EquipOffset, nItemType, type, value);
			}
			break;
		default:
			break;

		}
	}
	switch(type) 
	{
	
		case IATTRIBUTE_POINT_MAXHP:			//����������HP������
		case IATTRIBUTE_RATE_MAXHP:				//���ٷֱ�����HP����
		case IATTRIBUTE_RESTORE_HP:				//�ӿ�HP�Ļظ��ٶ�
		case IATTRIBUTE_POINT_MAXMP:			//����������MP������
		case IATTRIBUTE_RATE_MAXMP:				//���ٷֱ�����MP����
		case IATTRIBUTE_RESTORE_MP:				//�ӿ�MP�Ļظ��ٶ�
		case IATTRIBUTE_COLD_ATTACK:			//������
		case IATTRIBUTE_COLD_RESIST:			//���ֿ�
		case IATTRIBUTE_COLD_TIME:				//���ٱ����ٻ�ʱ��
		case IATTRIBUTE_FIRE_ATTACK:			//�𹥻�
		case IATTRIBUTE_FIRE_RESIST:			//��ֿ�
		case IATTRIBUTE_FIRE_TIME:				//���ٻ��ճ���ʱ��
		case IATTRIBUTE_LIGHT_ATTACK:			//�繥��
		case IATTRIBUTE_LIGHT_RESIST:			//��ֿ�
		case IATTRIBUTE_LIGHT_TIME:				//���ٵ��ѣ��ʱ��
		case IATTRIBUTE_POISON_ATTACK:			//������
		case IATTRIBUTE_POISON_RESIST:			//���ֿ�    
		case IATTRIBUTE_POISON_TIME:			//�����ж�ʱ��
		case IATTRIBUTE_ATTACK_P:				//������
		case IATTRIBUTE_RATE_ATTACK_P:			//���ٷֱ�����������
		case IATTRIBUTE_RATE_ATTACK_EP:			//��װ�������������ٷֱȼӳ�
		case IATTRIBUTE_DEFENCE_P:				//�������
		case IATTRIBUTE_RATE_DEFENCE_P:			//���ٷֱ������������
		case IATTRIBUTE_RATE_DEFENCE_EP:		//��װ��������������ٷֱȼӳ�
		case IATTRIBUTE_IMMUNITY_P:				//���ٷֱȵ��������˺�
		case IATTRIBUTE_ATTACK_M:				//ħ������
		case IATTRIBUTE_RATE_ATTACK_M:			//���ٷֱ�����ħ������
		case IATTRIBUTE_RATE_ATTACK_EM:			//��װ������ħ�������ٷֱȼӳ�
		case IATTRIBUTE_DEFENCE_M:				//ħ������
		case IATTRIBUTE_RATE_DEFENCE_M:			//���ٷֱ�����ħ������
		case IATTRIBUTE_RATE_DEFENCE_EM:		//��װ������ħ�������ٷֱȼӳ�
		case IATTRIBUTE_IMMUNITY_M:				//���ٷֱȵ���ħ���˺�
		case IATTRIBUTE_ATTACK_SPEED:			//�����ٶ�
		case IATTRIBUTE_SKILL_TIME:				//ħ����ȴ�ٶ�
		case IATTRIBUTE_HIT:					//����
		case IATTRIBUTE_MISS:					//����
		case IATTRIBUTE_2ATTACK_RATE:			//����һ����˫���������İٷֱ�
		case IATTRIBUTE_NO_DEFENCE_RATE:		//���ӶԷ���������
		case IATTRIBUTE_SPEED_RATE:				//�ƶ��ٶȰٷֱ�
		case IATTRIBUTE_DAMAGE_RET:				//�˺�����
		case IATTRIBUTE_DAMAGE2MANA:			//�˺�����������
		case IATTRIBUTE_STR:					//��������
		case IATTRIBUTE_SPR:					//��������
		case IATTRIBUTE_CON:					//��������
		case IATTRIBUTE_INT:					//���Ӷ���
		case IATTRIBUTE_DEX:					//������
		case IATTRIBUTE_LUK:					//��������
		case IATTRIBUTE_HP_THIEVE:				//����͵ȡ(���˺���)
		case IATTRIBUTE_MP_THIEVE:				//����͵ȡ(���˺���)
		case IATTRIBUTE_BASE_ATTACK_P:			//����������
		case IATTRIBUTE_BASE_ATTACK_M:			//����ħ������
		case IATTRIBUTE_BASE_ATTACK_TIME:		//���������ٶȣ�ֻ����ͨ������
		case IATTRIBUTE_BASE_DEFENCE_P:			//�����������
		case IATTRIBUTE_BASE_DEFENCE_M:			//����ħ������
		case IATTRIBUTE_BASE_MISS:				//��������
			{

				_ITEM_EFFECT* pEffect = &(m_pItemEffect[type]);

				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}
				break;
			}
		//��������
		case IATTRIBUTE_RESIST_ALL:				//���ٷֱȵ����������Թ���
			{

				_ITEM_EFFECT* pEffect = &(m_pItemEffect[IATTRIBUTE_COLD_RESIST]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}
				pEffect = &(m_pItemEffect[IATTRIBUTE_FIRE_RESIST]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}			
				pEffect = &(m_pItemEffect[IATTRIBUTE_LIGHT_RESIST]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}
				pEffect = &(m_pItemEffect[IATTRIBUTE_POISON_RESIST]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}			

				break;
			}
		case IATTRIBUTE_ALL:					//�������е�����һ������
			{
				_ITEM_EFFECT* pEffect = &(m_pItemEffect[IATTRIBUTE_STR]);

				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}

				pEffect = &(m_pItemEffect[IATTRIBUTE_SPR]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}			
				pEffect = &(m_pItemEffect[IATTRIBUTE_CON]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}			
				pEffect = &(m_pItemEffect[IATTRIBUTE_INT]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}			
				pEffect = &(m_pItemEffect[IATTRIBUTE_DEX]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}			
				pEffect = &(m_pItemEffect[IATTRIBUTE_LUK]);
				if(pEffect->IsActive())
					pEffect->m_Attr.m_Value += value;
				else
				{
					pEffect->m_Attr.m_Value = value;
					pEffect->SetActive(TRUE);
				}			
				break;
			}
		case IATTRIBUTE_USESKILL:				//����ĳ��ʹ�ü���
			{
				
				_ITEM_SKILL*	pSkill = &(m_EquipUseSkillEffect[EquipOffset]);
				
				if(pSkill->IsActive()) 
				{
					Assert(FALSE);				//һ��װ����ͬʱ����2������
				}
				else
				{
					pSkill->m_SkillID		=	value;
					pSkill->m_SkillRate		=	100;
					pSkill->SetActive(TRUE);
				}
				break;
			}
		case IATTRIBUTE_RAND_SKILL:				//����ĳ���������
			{
				_ITEM_SKILL*	pSkill = &(m_EquipRandSkillEffect[EquipOffset]);

				if(pSkill->IsActive()) 
				{
					Assert(FALSE);			//һ��װ����ͬʱ����2������
				}
				else
				{
					pSkill->m_SkillID		=	value;
					pSkill->m_SkillRate		=	100;
					pSkill->SetActive(TRUE);
				}
				break;
			}
		case IATTRIBUTE_SKILL_RATE:
			{
				_ITEM_SKILL*	pSkill = &(m_EquipRandSkillEffect[EquipOffset]);
				pSkill->m_SkillRate		=	value;
				break;
			}
		default:
			{
				Assert(FALSE);
				break;
			}

		}

	__LEAVE_FUNCTION
}

VOID	Obj_Human::CaculateEquipSetEffect()
{
	__ENTER_FUNCTION
		
			
		INT iCapSetNum		=	HumanItemLogic::GetEquip(this,HEQUIP_CAP)->GetSetNum();
		INT	iArmorSetNum	=	HumanItemLogic::GetEquip(this,HEQUIP_ARMOR)->GetSetNum();
		INT	iCuffSetNum		=	HumanItemLogic::GetEquip(this,HEQUIP_CUFF)->GetSetNum();
		INT	iBootSetNum		=	HumanItemLogic::GetEquip(this,HEQUIP_BOOT)->GetSetNum();
		
		if( (iArmorSetNum==iCapSetNum)&&
			(iCapSetNum== iCuffSetNum)&&
			(iCapSetNum== iBootSetNum)
			)
		{
			if((iCapSetNum!=-1)&&(iCapSetNum!=0))
			{
					EQUIP_SET_TB* pGET = g_ItemTable.GetEquipSetTB(iCapSetNum);
					if(pGET)
					{
						for(INT i = 0;i<pGET->m_nAttrCount;i++)
						{
							CaculateEffect(pGET->m_ItemAttr[i],HEQUIP_NUMBER+i);
						}
					}

			}
		}

	__LEAVE_FUNCTION
}

