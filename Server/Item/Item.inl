/////////////////////////////////////////////////////////////////////////////////
//�ļ�����Item.inl
//������������Ʒ���ݶ�ȡд���inline����
//�޸ļ�¼��2005-11-2����
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef __ITEM_INL__
#define __ITEM_INL__
#include "DB_Struct.h"
#include "GameTable.h"

inline _ITEM_GUID Item::GetGUID( )
{
	__ENTER_FUNCTION

	Assert( m_pItem ) ;
	return m_pItem->m_ItemGUID ;

	__LEAVE_FUNCTION

	_ITEM_GUID	ig;
	return	ig;
};

inline UINT Item::GetItemTableIndex( ) const//ȡ����Ʒ�������Ʒ�������
{
	__ENTER_FUNCTION

	Assert( m_pItem ) ;
	return m_pItem->m_ItemIndex ;

	__LEAVE_FUNCTION

	return	-1;
}

inline UCHAR Item::GetItemClass()	const
{
	__ENTER_FUNCTION

	Assert(m_pItem);
	return	m_pItem->ItemClass();

	__LEAVE_FUNCTION

	return	0;

}

inline UCHAR	Item::GetItemQual() const
{
	__ENTER_FUNCTION

	Assert(m_pItem);
	return	m_pItem->GetQual();

	__LEAVE_FUNCTION

	return	0;
}


inline UCHAR	Item::GetItemType() const
{
	__ENTER_FUNCTION
	
	Assert(m_pItem);
	return	m_pItem->ItemType();

	__LEAVE_FUNCTION

	return	0;
}

inline UCHAR	Item::GetItemIndex() const
{
	__ENTER_FUNCTION

	Assert(m_pItem);
	return	m_pItem->GetIndex();

	__LEAVE_FUNCTION

	return	0;
}

inline INT Item::GetLayedNum( ) const//ȡ�õ�����Ʒ������
{
	__ENTER_FUNCTION

	switch(m_Type)
	{
	case IDT_ITEM:
		{
			Assert( m_pItem ) ;
			return m_pItem->GetItemCount();
		}
		break;
	case IDT_PET:
		{
			Assert(m_pPet);
			return 1;
		}
		break;
	default:
		{
			Assert(FALSE);
			return 0;
		}
	}


	__LEAVE_FUNCTION

	return 0 ;
}


inline BOOL Item::IsPWLock() const
{
	__ENTER_FUNCTION
	switch(m_Type)
	{
		case IDT_ITEM:
			{
				Assert( m_pItem ) ;
				return m_pItem->GetItemPLock();
			}
			break;
		case IDT_PET:
			{
				Assert(m_pPet);
				return 1;
			}
			break;
		default:
			{
				Assert(FALSE);
				return 0;
			}
	}

	__LEAVE_FUNCTION
	return TRUE;
}

inline INT Item::GetMaxLayedNum( ) const//ȡ���ܹ�������Ʒ���������
{
	__ENTER_FUNCTION

	switch(m_Type)
	{
	case IDT_ITEM:
		{
			Assert( m_pItem ) ;
			return m_pItem->GetItemTileMax();
		}
		break;
	case IDT_PET:
		{
			Assert(m_pPet);
			return 1;
		}
		break;
	default:
		{
			Assert(FALSE);
			return 0;
		}
	}

	__LEAVE_FUNCTION

	return 0 ;
}

BOOL	Item::IsFull() const
{
	__ENTER_FUNCTION

	switch(m_Type)
	{
	case IDT_ITEM:
		{
			Assert(m_pItem);
			return m_pItem->isFullTile();
		}
		break;
	case IDT_PET:
		{
			Assert(m_pPet);
			return !m_pPet->m_GUID.IsNull();
		}
		break;
	default:
		{
			Assert(FALSE);
			return FALSE;
		}
	}

	__LEAVE_FUNCTION

	return FALSE;
}




USHORT					Item::GetSetNum()			const				//��װ���
{
	Assert(m_pItem);
	return	m_pItem->GetEquipData()->m_SetNum;
}
BYTE					Item::GetMaxSetCount()			const				//��װ����
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_MaxNum;
}
BYTE					Item::GetEquipPoint()		const				//��Ʒװ���
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_EquipPoint;
}
BYTE					Item::GetMaxDurPoint()		const			//����;�ֵ
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_MaxDurPoint;
}
BYTE					Item::GetRequireLevel()		const			//����ȼ�
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_NeedLevel;
}
BYTE					Item::GetGemSlotMax()		const			//���ʯ����
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_GemMax;
}
BYTE					Item::GetFailTimes()		const				//����ʧ�ܴ���
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_FaileTimes;

}
BYTE					Item::GetDurPoints()		const 				//��ǰ�;�ֵ
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_CurDurPoint;
}
USHORT					Item::GetDamagePoint()	const
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_CurDamagePoint;
}

BOOL					Item::GetItemBind()	const
{
	Assert(m_pItem);
	return m_pItem->GetItemBind();
}

BOOL					Item::GetItemIdent()	const
{
	Assert(m_pItem);

	if(m_pItem->ItemClass()==ICLASS_EQUIP&&m_pItem->GetQual() == EQUALITY_BLUE)
	{
		return m_pItem->GetItemIdent();
	}
	return TRUE;
	
}

BYTE					Item::GetAttrCount()		const				//���Ե�����
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_AttrCount;
}
BYTE					Item::GetEquipGemCount()	const				//��Ƕ��ʯ������
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	return m_pItem->GetEquipData()->m_StoneCount;
}
const _ITEM_ATTR				Item::GetEquipAttr(UCHAR	nIndex)		const	//���װ������
{	
	Assert(m_pItem);
	Assert(nIndex<m_pItem->GetEquipData()->m_AttrCount);
	return m_pItem->GetEquipData()->m_pAttr[nIndex];
}
const _ITEM_GEMINFO			Item::GetEquipGemInfo(UCHAR nIndex)	const		//��ñ�ʯ��Ϣ
{
	Assert(m_pItem);
	Assert(nIndex<m_pItem->GetEquipData()->m_GemMax);
	return m_pItem->GetEquipData()->m_pGemInfo[nIndex];
}

BYTE						Item::GetItemLevel()		const				//��Ʒ����,�߻�����
{
	Assert(m_pItem);
	return m_pItem->GetMedicData()->m_nLevel;
}
BYTE						Item::GetReqSkillLevel()	const				//��ü��ܼ�������
{
	Assert(m_pItem);
	return	m_pItem->GetMedicData()->m_nReqSkillLevel;
}
INT							Item::GetReqSkill()			const				//��Ҫӵ�еļ��ܺ�
{
	Assert(m_pItem);
	return m_pItem->GetMedicData()->m_nReqSkill;
}
INT							Item::GetScriptID()			const				//�����ؽű����
{
	Assert(m_pItem);
	return m_pItem->GetMedicData()->m_nScriptID;
}
INT							Item::GetSkillID()			const				//�����ؼ��ܱ��
{
	Assert(m_pItem);
	return m_pItem->GetMedicData()->m_nSkillID;
}
BOOL						Item::IsCosSelf()			const				//�Ƿ������Լ�
{
	Assert(m_pItem);
	return m_pItem->GetMedicData()->m_bCosSelf;
}
BYTE						Item::GetTargetType()		const			   //��Ʒѡ���������
{
	Assert(m_pItem);
	return	m_pItem->GetMedicData()->m_TargetType;
}


const _ITEM_ATTR				Item::GetGemAttr()		const				//��ʯ����
{
	Assert(m_pItem);
	return m_pItem->GetGemData()->m_Attr;
}

INT						Item::GetMapLevel()			const				//�ر�ͼ����
{
	Assert(m_pItem);
	return m_pItem->GetStoreMapData()->m_nLevel;
}
FLOAT					Item::GetXPos()				const				//x����
{
	Assert(m_pItem);
	return m_pItem->GetStoreMapData()->m_xPos;
}
FLOAT					Item::GetZPos()				const				//z����
{
	Assert(m_pItem);
	return m_pItem->GetStoreMapData()->m_zPos;
}
INT						Item::GetSceneID()			const				//�������
{
	Assert(m_pItem);
	return m_pItem->GetStoreMapData()->m_SceneID;
}
INT						Item::GetGrowType()			const				//��Ӧ����������
{
	Assert(m_pItem);
	return m_pItem->GetStoreMapData()->m_GrowType;
}

inline  PET_GUID_t		Item::GetPetGUID( )								//ȡ�ó���GUID
{
	Assert(m_pPet);
	return m_pPet->m_GUID;
}

inline  PET_GUID_t		Item::GetSpouseGUID( )
{
	Assert(m_pPet);
	return m_pPet->m_SpouseGUID;
}


inline	INT				Item::GetDataID()					//ȡ�ó���ģ��		
{
	Assert(m_pPet);
	return m_pPet->m_nDataID;
}
inline  const CHAR*		Item::GetName()	const				//ȡ������
{
	Assert(m_pPet);
	return m_pPet->m_szName;	
}
inline  const CHAR*		Item::GetNick() const				//ȡ���ǳ�
{
	Assert(m_pPet);
	return m_pPet->m_szNick;
}
inline  INT				Item::GetLevel() const				//ȡ�õȼ�
{
	Assert(m_pPet);
	return m_pPet->m_nLevel;
}
inline  INT				Item::GetTakeLevel() const			//ȡ��Я���ȼ�
{
	Assert(m_pPet);
	return m_pPet->m_nTakeLevel;
}
inline  INT				Item::GetAttackType() const			//ȡ�ý������ͣ���/����
{
	Assert(m_pPet);
	return m_pPet->m_nAttackType;
}
inline  INT				Item::GetAIType() const				//ȡ��AI����
{
	Assert(m_pPet);
	return m_pPet->m_AIType;
}
inline  const _CAMP_DATA *Item::GetCampData() const				//ȡ����Ӫ
{
	Assert(m_pPet);
	return &(m_pPet->m_CampData);
}
inline  INT				Item::GetHP() const					//ȡ������ֵ
{
	Assert(m_pPet);
	return m_pPet->m_nHP;
}
inline  INT				Item::GetBaseMaxHP() const				//ȡ������ֵ����
{
	Assert(m_pPet);

	INT nBaseParam = 0;
	INT nValue = 0;
	INT nAttribParam = 0;
	FLOAT fLevelRate = 0;
	FLOAT fAttribRate = 0.f;

	INT nLevel = GetLevel();
	FLOAT fGrowRate = GetGrowRate();

	nBaseParam = g_TableInit.m_PetConfig.m_BaseHP;
	fAttribRate = g_TableInit.m_PetConfig.m_Con_HP_Pram;
	fLevelRate = g_TableInit.m_PetConfig.m_Level_HP_Pram;
	nAttribParam = GetLvl1Attr(CATTR_LEVEL1_CON);
	nValue = (INT)(nBaseParam + nAttribParam*fAttribRate + nLevel*fLevelRate*fGrowRate);
	return nValue;
}
inline  INT				Item::GetLife() const				//ȡ�õ�ǰ����
{
	Assert(m_pPet);
	return m_pPet->m_nLife;
}
inline  BYTE			Item::GetPetType() const				// ���������죬Ұ��
{
	Assert(m_pPet);
	return m_pPet->m_byPetType;
}
inline  BYTE			Item::GetGeneration() const			// ������
{
	Assert(m_pPet);
	return m_pPet->m_byGeneration;
}
inline  BYTE			Item::GetHappiness() const				// ���ֶ�		
{
	Assert(m_pPet);
	return m_pPet->m_byHappiness;
}
inline  INT				Item::GetStrPer() const				// ��������	
{
	Assert(m_pPet);
	return m_pPet->m_nStrPerception;
}
inline  INT				Item::GetConPer() const				// ��������	
{
	Assert(m_pPet);
	return m_pPet->m_nConPerception;
}
inline  INT 			Item::GetDexPer() const				// ������
{
	Assert(m_pPet);
	return m_pPet->m_nDexPerception;
}
inline  INT				Item::GetSprPer() const				// ��������
{
	Assert(m_pPet);
	return m_pPet->m_nSprPerception;
}
inline  INT 			Item::GetIntPer() const				// ��������
{
	Assert(m_pPet);
	return m_pPet->m_nIntPerception;
}
inline	INT				Item::GetGenGu() const				// ����
{
	Assert(m_pPet);
	return m_pPet->m_nGenGu;
}
inline  FLOAT			Item::GetGrowRate() const			// �ɳ���
{
	Assert(m_pPet);
	return m_pPet->m_fGrowRate;
}
inline  INT				Item::GetRemainPoint() const			// һ������ʣ�����
{
	Assert(m_pPet);
	return m_pPet->m_nRemainPoint;
}
inline  INT				Item::GetExp() const					// ����ֵ
{
	Assert(m_pPet);
	return m_pPet->m_nExp;
}
inline  INT				Item::GetLvl1Attr(CHAR_ATTR_LEVEL1 type)const// ����һ��ս�����ԣ����������ܺ�װ�����ӵĲ��֣�
{
	Assert(m_pPet);
	return m_pPet->m_BaseAttrLevel1.Get(type);
}
inline  _PET_SKILL		Item::GetSkill(UINT SkillIndex)		//���＼��
{
	Assert(m_pPet);
	Assert(SkillIndex<PET_MAX_SKILL_COUNT);
	return m_pPet->m_SkillList[SkillIndex];
}
inline  VOID			Item::SetCooldown(CooldownID_t nID, Time_t nTime)
{
	Assert(m_pPet);
	m_pPet->m_CooldownList.RegisterCooldown(nID, nTime);
}
inline	BOOL			Item::IsCooldowned(CooldownID_t nID) const
{
	Assert(m_pPet);
	return m_pPet->m_CooldownList.IsSpecificSlotCooldownedByID(nID);
}
inline	VOID			Item::HeartBeat_Cooldown(Time_t nDeltaTime)
{
	Assert(m_pPet);
	m_pPet->m_CooldownList.HeartBeat(nDeltaTime);
}
inline	Time_t			Item::GetCooldownRemained(CooldownID_t nID) const
{
	Assert(m_pPet);
	return m_pPet->m_CooldownList.GetRemainTimeByID(nID);
}

/*
inline	_IMPACT_LIST&	Item::GetImpactList()				// �õ�����BUFF�б�
{
	Assert(m_pPet);
	return m_pPet->m_ImpactList;
}
*/

#endif					
						
						
						
