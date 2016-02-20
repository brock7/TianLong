#include "StdAfx.h"
#include "Obj_Item_Medicine.h"
#include "TDException.h"
#include "..\character\obj_playermyself.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\ObjectManager.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Global.h"
#include "..\..\World\WorldManager.h"
#include "TDGameInterface.h"

CObject_Item_Medicine::CObject_Item_Medicine(INT id)
		: CObject_Item(id)
{
	m_theBaseDef = NULL;
}

CObject_Item_Medicine::~CObject_Item_Medicine()
{
}

VOID CObject_Item_Medicine::AsMedicine(const _DBC_ITEM_MEDIC* pMedicDefine)
{
	TDAssert(pMedicDefine);

	m_theBaseDef = pMedicDefine;
	m_nParticularID = ( ( ( ( ( m_theBaseDef->nClass * 100 ) + m_theBaseDef->nQuality ) * 100 ) + m_theBaseDef->nType ) * 1000 ) + m_theBaseDef->nIndex;

}

LPCTSTR  CObject_Item_Medicine::GetIconName(VOID) const
{
	return m_theBaseDef->szIcon;
}

//������ϸ����
VOID CObject_Item_Medicine::SetExtraInfo(const _ITEM * pItemInfo)
{
	TDAssert(pItemInfo);

	//���û��ຯ��������_ITEM�ṹ
	CObject_Item::SetExtraInfo(pItemInfo);

	const MEDIC_INFO& infoMedic = pItemInfo->m_Medic;

	SetNumber(pItemInfo->GetItemCount());
	SetManufacturer(pItemInfo);
}

LPCTSTR  CObject_Item_Medicine::GetExtraDesc(VOID)
{
	m_strExtraDesc = m_theBaseDef->szDesc;
	return m_strExtraDesc.c_str();
}

//��¡��ϸ��Ϣ
VOID	CObject_Item_Medicine::Clone(const CObject_Item * pItemSource)
{
	SetNumber(((CObject_Item_Medicine*)pItemSource)->GetNumber());
	CObject_Item::Clone(pItemSource);
}

//�õ���Ʒ�����
ITEM_CLASS	CObject_Item_Medicine::GetItemClass(VOID) const 
{ 
	return (ITEM_CLASS)m_theBaseDef->nClass; 
}

// �õ���Ʒ��Ʒ��
INT		CObject_Item_Medicine::GetQuantity()
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nQuality;
	}

	return -1;
}

	
	// �õ���Ʒ����npc�ļ۸�
INT		CObject_Item_Medicine::GetItemPrice()
{
	if(m_theBaseDef)
	{
		return (m_theBaseDef->nSalePrice*this->GetNumber());
	}
	return 0;
}

// �õ���Ʒ������ļ���ID
INT		CObject_Item_Medicine::GetItemSkillID()
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nSkillID;
	}
	return 0;
}

INT		CObject_Item_Medicine::GetItemTargetType()
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nTargetType;
	}
	return ITEM_TARGET_TYPE_INVALID;
}

INT		CObject_Item_Medicine::GetNeedLevel()
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nLevelRequire;
	}
	return 0;
}

	// �õ�����Ʒ�ڱ��е�����
INT CObject_Item_Medicine::GetMedicineItemTableType()
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nType;
	}

	return -1;
}

// �õ�quality��Ϣ
INT	CObject_Item_Medicine::GetItemTableQuality(VOID)
{
	return m_theBaseDef->nQuality;
}

// �õ�type��Ϣ
INT	CObject_Item_Medicine::GetItemTableType(VOID)
{
	return m_theBaseDef->nType;
}

bool CObject_Item_Medicine::IsValidTarget(CObject* pSelectObj, fVector2& fvPos, int& objID, PET_GUID_t& petID)
{
	if(!m_theBaseDef) return false;
	//����Լ�
	CObject_PlayerMySelf* pMySlef = CObjectManager::GetMe()->GetMySelf();

	//����Ŀ��
	if(ITEM_TARGET_TYPE_NONE==m_theBaseDef->nTargetType) 
		return true;

	//��Ҫѡ��һ������λ��
	if(ITEM_TARGET_TYPE_POS==m_theBaseDef->nTargetType) 
	{
		return CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fvPos)==TRUE;
	}

	//Ŀǰ��֧�ֵķ�ʽ
	if(	ITEM_TARGET_TYPE_DIR==m_theBaseDef->nTargetType ||	//����
		ITEM_TARGET_TYPE_ITEM==m_theBaseDef->nTargetType)	//��Ʒ->��Ʒ
		return false;

	switch(m_theBaseDef->nTargetType)
	{
	case ITEM_TARGET_TYPE_SELF:	//����
		{
			objID = pMySlef->GetServerID();
		}
		return true;

	case ITEM_TARGET_TYPE_SELF_PET:	//�Լ��ĳ���
		{
			petID = pMySlef->GetCharacterData()->Get_CurrentPetGUID();
			//������δ�ͷ�
			if(petID.IsNull())
				return false;

			objID = pMySlef->GetServerID();
			return true;
		}
		break;

	case ITEM_TARGET_TYPE_FRIEND:	//�Ѻõ�Ŀ��
	case ITEM_TARGET_TYPE_ENEMY:	//�ж�Ŀ��
		{
			//������Ҫһ��Ŀ��
			if(!pSelectObj) return false;
			//�����ǽ�ɫ
			if(!g_theKernel.IsKindOf(pSelectObj->GetClass(), GETCLASS(CObject_Character)))
				return false;

			//�����Ӫ
			ENUM_RELATION eCampType = 
				CGameProcedure::s_pGameInterface->GetCampType(pMySlef, pSelectObj);

			//�������Ѻ�Ŀ��/�ж�Ŀ��
			if( (m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_FRIEND && 
					(pSelectObj==pMySlef || eCampType==RELATION_FRIEND)) ||
				(m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_ENEMY && 
					(eCampType==RELATION_ENEMY)))
			{
				objID = pSelectObj->GetServerID();
				return true;
			}
		}
		return false;

	case ITEM_TARGET_TYPE_FRIEND_PLAYER: // �Ѻ����	
	case ITEM_TARGET_TYPE_ENEMY_PLAYER:	//	�ж����
		{
			//������Ҫһ��Ŀ��
			if(!pSelectObj) return false;
			//���������
			if(!g_theKernel.IsKindOf(pSelectObj->GetClass(), GETCLASS(CObject_PlayerOther)))
				return false;

			//�����Ӫ
			ENUM_RELATION eCampType = 
				CGameProcedure::s_pGameInterface->GetCampType(pMySlef, pSelectObj);

			//�������Ѻ����/�ж����
			if(	(m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_FRIEND_PLAYER &&
					(pSelectObj==pMySlef || eCampType==RELATION_FRIEND)) ||
				(m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_ENEMY_PLAYER &&
					(eCampType==RELATION_ENEMY)))
			{
				objID = pSelectObj->GetServerID();
				return true;
			}
		}
		return false;

	case ITEM_TARGET_TYPE_FRIEND_MONSTER:	//�Ѻù���
	case ITEM_TARGET_TYPE_ENEMY_MONSTER:	//�жԹ���
		{
			//������Ҫһ��Ŀ��
			if(!pSelectObj) return false;
			//�����ǽ�ɫ
			if(!g_theKernel.IsKindOf(pSelectObj->GetClass(), GETCLASS(CObject_Character)))
				return false;
			//������NPC
			if(((CObject_Character*)pSelectObj)->GetCharacterType() != 
					CObject_Character::CT_MONSTER) return false;

			//�����Ӫ
			ENUM_RELATION eCampType = 
				CGameProcedure::s_pGameInterface->GetCampType(pMySlef, pSelectObj);

			//�������Ѻ�NPC
			if(	(m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_FRIEND_MONSTER && 
					eCampType==RELATION_FRIEND) ||
				(m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_ENEMY_MONSTER && 
					eCampType==RELATION_ENEMY))
			{
				objID = pSelectObj->GetServerID();
				return true;
			}
		}
		return false;

	case ITEM_TARGET_TYPE_FRIEND_PET:	//�Ѻó���
	case ITEM_TARGET_TYPE_ENEMY_PET:	//�жԳ���	
		{
			//������Ҫһ��Ŀ��
			if(!pSelectObj) return false;
			//�����ǽ�ɫ
			if(!g_theKernel.IsKindOf(pSelectObj->GetClass(), GETCLASS(CObject_Character)))
				return false;
			//������NPC
			if(((CObject_Character*)pSelectObj)->GetCharacterType() != 
					CObject_Character::CT_MONSTER) return false;
			//�����ǳ���
			if(((CObject_PlayerNPC*)pSelectObj)->GetNpcType() !=
					NPC_TYPE_PET) return false;

			//�����Ӫ
			ENUM_RELATION eCampType = 
				CGameProcedure::s_pGameInterface->GetCampType(pMySlef, pSelectObj);

			//�������Ѻó���
			if(	(m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_FRIEND_MONSTER && 
					eCampType==RELATION_FRIEND) ||
				(m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_ENEMY_PET && 
					eCampType==RELATION_ENEMY))
			{
				objID = pSelectObj->GetServerID();
				return true;
			}
		}
		return false;

	case ITEM_TARGET_TYPE_ALL_CHARACTER:	//���н�ɫ
		{
			//������Ҫһ��Ŀ��
			if(!pSelectObj) return false;
			//�����ǽ�ɫ
			if(!g_theKernel.IsKindOf(pSelectObj->GetClass(), GETCLASS(CObject_Character)))
				return false;

			return true;
		}
		return false;

	default:
		break;
	}

	return false;
}

bool CObject_Item_Medicine::IsAreaTargetType(void) const
{
	if(!m_theBaseDef) return false;
	//����Լ�
	CObject_PlayerMySelf* pMySlef = CObjectManager::GetMe()->GetMySelf();

	//����Ŀ��
	return (ITEM_TARGET_TYPE_POS==m_theBaseDef->nTargetType);
}

bool CObject_Item_Medicine::IsTargetOne(void) const
{
	if(!m_theBaseDef) return false;

	return (ITEM_TARGET_TYPE_SELF==m_theBaseDef->nTargetType || 
			ITEM_TARGET_TYPE_SELF_PET==m_theBaseDef->nTargetType);
}


	// �õ���Ʒ����������2006-4-28
LPCTSTR	CObject_Item_Medicine::GetItemTableTypeDesc()
{
	return m_theBaseDef->szTypeDesc;
}

// �õ���Ʒ����������Ϣ
INT	CObject_Item_Medicine::GetItemLevelDesc()
{

	return 1;//m_theBaseDef->nLevelDesc;
}

