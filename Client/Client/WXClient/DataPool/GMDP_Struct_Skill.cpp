// GMDP_Struct_Skill.cpp

#include "StdAfx.h"

#include "TDDBC_Struct.h"
#include "GMDataPool.h"
#include "GMDP_CharacterData.h"
#include "..\Object\ObjectManager.h"
#include "..\Object\Character\Obj_Character.h"
#include "../object/character/obj_playermyself.h"
#include "..\Procedure\GameProcedure.h"
#include "..\Interface\GMGameInterface.h"
#include "..\Event\GMEventSystem.h"
#include "..\DBC\GMDataBase.h"

#include "GMDP_Struct_Skill.h"

ORESULT SCLIENT_SKILL::IsCanUse( INT idUser, INT idLevel, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir )const
{
	ORESULT oResult;

	oResult = IsCanUse_CheckLevel(idUser, idLevel);
	if ( OR_FAILED( oResult ) )
		return oResult;

	oResult = IsCanUse_Leaned();
	if ( OR_FAILED( oResult ) )
		return oResult;

	oResult = IsCanUse_CheckCoolDown();
	if ( OR_FAILED( oResult ) )
		return oResult;

	oResult = IsCanUse_CheckDeplete( idUser );
	if ( OR_FAILED( oResult ) )
		return oResult;

	oResult = IsCanUse_CheckTarget( idUser, idTargetObj, fTargetX, fTargetZ, fDir );
	if ( OR_FAILED( oResult ) )
		return oResult;

	return OR_OK;
}

ORESULT SCLIENT_SKILL::IsCanUse_CheckLevel( INT idUser, INT idLevel )const
{
	//CObject_Character *pUser = (CObject_Character*)(CObjectManager::GetMe()->FindObject( idUser ));
	//if ( pUser == NULL )
	//{
	//	Assert( pUser != NULL && "SCLIENT_SKILL::IsCanUse_CheckDeplete" );
	//	return OR_ERROR;
	//}

	return OR_OK;
}

ORESULT SCLIENT_SKILL::IsCanUse_Leaned( VOID )const
{
	if ( !m_bLeaned )
		return OR_CHAR_DO_NOT_KNOW_THIS_SKILL;

	return OR_OK;
}

ORESULT SCLIENT_SKILL::IsCanUse_CheckCoolDown( VOID )const
{
	// ��ȴ�ж�
	INT nCoolDownID = m_pDefine->m_nCooldownID;
	if ( INVALID_ID != nCoolDownID )
	{
		const CDataPool::COOLDOWN_GROUP* pCoolDownGroup;
		pCoolDownGroup = CDataPool::GetMe()->CoolDownGroup_Get(nCoolDownID);

		if ( pCoolDownGroup == NULL || pCoolDownGroup->nTime > 0 ) 
			return OR_COOL_DOWNING;
	}
	return OR_OK;
}

ORESULT SCLIENT_SKILL::IsCanUse_CheckDeplete( INT idUser )const
{
	CObject_Character *pUser = (CObject_Character*)(CObjectManager::GetMe()->FindObject( idUser ));
	if ( pUser == NULL )
	{
		Assert( pUser != NULL && "SCLIENT_SKILL::IsCanUse_CheckDeplete" );
		return OR_ERROR;
	}

	CCharacterData *pCharData = CDataPool::GetMe()->CharacterData_Get( idUser );
	if ( pCharData == NULL )
	{
		Assert( pCharData != NULL && "SCLIENT_SKILL::IsCanUse_CheckDeplete" );
		return OR_ERROR;
	}

	// �ķ�Ҫ��
	//const SCLIENT_XINFA* pXinfa = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_XinFa(m_pDefine->m_nXinFaID);
	//if ( pXinfa && pXinfa->m_nLevel <m_pDefine->m_nXinFaLevelRequirement )
	//{
	//	return OR_NEED_HIGH_LEVEL_XINFA;
	//}

	// �����ж�
	if(m_pDefine->m_bMustUseWeapon)
	{
		if(pUser->GetCharacterData()->Get_Equip(HEQUIP_WEAPON) == INVALID_ID)
			return OR_NEED_A_WEAPON;
	}

	// �����ж�
	if ( m_pDeplete == NULL )
	{
		//Assert( m_pDeplete != NULL && "SCLIENT_SKILL::IsCanUse_CheckDeplete" );
		return OR_OK;
	}
	const CImpactEffectMap*	pImpactEffectMap = pUser->GetImpactEffectMap();
	CImpactEffectMap::const_iterator itEnd;
	itEnd = pImpactEffectMap->end();

	// �Ƿ��м������м������ĵ�BUFF
	INT nDerateAllImpactID = INVALID_ID;
	CImpactEffectMap::const_iterator itDerateAll;
	itDerateAll = pImpactEffectMap->find( nDerateAllImpactID );
	if ( itDerateAll != itEnd )
	{
		return OR_OK;
	}

	// �Ƿ��м��⵱ǰ�������ĵ�BUFF
	INT nDerateCurImpactID = INVALID_ID;
	CImpactEffectMap::const_iterator itDerateCur;
	itDerateCur = pImpactEffectMap->find( nDerateCurImpactID );
	if ( itDerateCur != itEnd )
	{
		return OR_OK;
	}

	if ( m_pDeplete->m_nHP > 0 )
	{
		// �Ƿ��м���HP���ĵ�BUFF
		INT nDerateHPImpactID = INVALID_ID;
		CImpactEffectMap::const_iterator itDerateHP;
		itDerateHP = pImpactEffectMap->find( nDerateHPImpactID );
		if ( itDerateHP == itEnd )
		{
			if ( pCharData->Get_HP() < m_pDeplete->m_nHP )
				return OR_NOT_ENOUGH_HP;
		}
	}

	if ( m_pDeplete->m_nMP > 0 )
	{
		// �Ƿ��м���MP���ĵ�BUFF
		INT nDerateMPImpactID = INVALID_ID;
		CImpactEffectMap::const_iterator itDerateMP;
		itDerateMP = pImpactEffectMap->find( nDerateMPImpactID );
		if ( itDerateMP == itEnd )
		{
			if ( pCharData->Get_MP() < m_pDeplete->m_nMP )
				return OR_LACK_MANA;
		}
	}

	if ( m_pDeplete->m_nSP > 0 )
	{
		// �Ƿ��м���SP���ĵ�BUFF
		INT nDerateSPImpactID = INVALID_ID;
		CImpactEffectMap::const_iterator itDerateSP;
		itDerateSP = pImpactEffectMap->find( nDerateSPImpactID );
		if ( itDerateSP == itEnd )
		{
			if ( pCharData->Get_Rage() < m_pDeplete->m_nSP )
				return OR_NOT_ENOUGH_RAGE;
		}
	}

	if ( m_pDeplete->m_nStrikePoint > 0 )
	{
		// �Ƿ��м���StrikePoint���ĵ�BUFF
		// �����m_pDeplete->m_nStrikePointΪ������
		INT nDerateStrikePointImpactID = INVALID_ID;
		CImpactEffectMap::const_iterator itDerateStrikePoint;
		itDerateStrikePoint = pImpactEffectMap->find( nDerateStrikePointImpactID );
		if ( itDerateStrikePoint == itEnd )
		{
			INT nStrikePoint = m_pDeplete->m_nStrikePoint * 3;
			if ( pCharData->Get_StrikePoint() < nStrikePoint )
				return OR_NOT_ENOUGH_STRIKE_POINT;
		}
	}

	if ( m_pDeplete->m_nItemID != -1 )
	{
		// �Ƿ��м���Item���ĵ�BUFF
		INT nDerateItemImpactID = INVALID_ID;
		CImpactEffectMap::const_iterator itDerateItem;
		itDerateItem = pImpactEffectMap->find( nDerateItemImpactID );
		if ( itDerateItem == itEnd )
		{
			INT nIndex =  CDataPool::GetMe()->UserBag_GetItemIndexByID( m_pDeplete->m_nItemID );
			if ( nIndex == -1 )
				return OR_NOT_ENOUGH_ITEM;
		}
	}
	return OR_OK;
}

ORESULT SCLIENT_SKILL::IsCanUse_CheckTarget( INT idUser, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir )const
{
	switch(m_pDefine->m_nSelectType)
	{
	case SELECT_TYPE_NONE:	
		break;
	case SELECT_TYPE_CHARACTER:	
		{
			if(idTargetObj==INVALID_ID)
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "û��Ŀ��");
				return OR_NO_TARGET;
			}
			// m_nTargetMustInSpecialState--> 0:��ģ�1:����; -1:û��Ҫ��
			INT nTargetMustAliveFlag = !(m_pDefine->m_nTargetMustInSpecialState);
			CObject_Character* pTargetObj = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject(idTargetObj));
			if (!pTargetObj)
			{
				Assert(NULL=="[SCLIENT_SKILL::IsCanUse_CheckTarget]...pTargetObj = NULL...");
				return OR_ERROR;
			}
			CObject_Character* pThisObj = (CObject_Character*)(CObjectManager::GetMe()->FindObject(idUser));
			if (!pThisObj)
			{
				Assert(NULL=="[SCLIENT_SKILL::IsCanUse_CheckTarget]...pThisObj = NULL...");
				return OR_ERROR;
			}

			//��Ӫ�ж�
			ENUM_RELATION eCampType = CGameProcedure::s_pGameInterface->GetCampType( 
				pThisObj, 
				pTargetObj );

			if( m_pDefine->m_nFriendness < 0 && eCampType == RELATION_ENEMY )
				;
			else if ( m_pDefine->m_nFriendness > 0 && eCampType == RELATION_FRIEND)
				;
			else if ( m_pDefine->m_nFriendness == 0 )
				;
			else
			{
				return OR_INVALID_TARGET;
			}

			if( !(pTargetObj->IsDie())
				&& nTargetMustAliveFlag )
			{// �ü���ֻ�Ի�Ŀ����Ч
			}
			else if( pTargetObj->IsDie()
				&& !nTargetMustAliveFlag )
			{// �ü���ֻ����Ŀ����Ч
			}
			else
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "��ЧĿ��");
				return OR_INVALID_TARGET;
			}
		}
		break;
	case SELECT_TYPE_POS:
		{
			if( !(fTargetX>=0.f && fTargetZ>=0.f) )
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "��ЧĿ��");
				return OR_INVALID_TARGET_POS;
			}
		}
		break;
	case SELECT_TYPE_DIR:
		break;
	case SELECT_TYPE_SELF:		
		break;
	}
	return OR_OK;
}

LPCTSTR SCLIENT_SKILL::GetSkillDesc()	const		// �õ����ܵ�����
{

	if(m_pDefine)
	{
		// �õ������ķ�id
		INT	iSkillClass    = m_pDefine->m_nSkillClass;

		INT iLevel = m_nLevel - 1;
		if( iLevel < 0 ) iLevel = 0;

		// �õ����ܶ�Ӧ���ķ�����
		const SCLIENT_SKILLCLASS* pSkillClass =  CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_SkillClass(iSkillClass);
		if(pSkillClass)
		{
			// ��������id
			int iSkillData_V1ID = m_pDefine->m_anSkillByLevel[iLevel];
			
			//�����ݱ�
			DBC_DEFINEHANDLE(s_pSkillData_V1, DBC_SKILLDATA_V1_DEPLETE);
			
			//������¼
			const _DBC_SKILLDATA_V1_DEPLETE* pSkillData_V1 = (const _DBC_SKILLDATA_V1_DEPLETE*)s_pSkillData_V1->Search_Index_EQU((UINT)iSkillData_V1ID);
		
			if(pSkillData_V1)
			{
				return pSkillData_V1->paraPair[11].szDesc;
			}
		}
		else
		{
			return m_pDefine->m_pszDesc;
		}
	}

	static TCHAR szBuf[2]= {0,0};
	return szBuf;
}

LPCTSTR SCLIENT_SKILL::GetSkillDesc_Interface()	const		// �õ����ܵ�����
{

	if(m_pDefine)
	{
		// �õ������ķ�id
		INT	iSkillClass    = m_pDefine->m_nSkillClass;

		INT iLevel = m_nLevel - 1;
		if( iLevel < 0 ) iLevel = 0;

		// �õ����ܶ�Ӧ���ķ�����
		const SCLIENT_SKILLCLASS* pSkillClass =  CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_SkillClass(iSkillClass);
		if(pSkillClass)
		{
			// ��������id
			int iSkillData_V1ID = m_pDefine->m_anSkillByLevel[iLevel];
			
			//�����ݱ�
			DBC_DEFINEHANDLE(s_pSkillData_V1, DBC_SKILLDATA_V1_DEPLETE);
			
			//������¼
			const _DBC_SKILLDATA_V1_DEPLETE* pSkillData_V1 = (const _DBC_SKILLDATA_V1_DEPLETE*)s_pSkillData_V1->Search_Index_EQU((UINT)iSkillData_V1ID);
		
			if(pSkillData_V1)
			{
				return pSkillData_V1->Skill_Desc_Interface;
			}
		}
		else
		{
			return m_pDefine->m_pszDesc;
		}
	}

	static TCHAR szBuf2[2]= {0,0};
	return szBuf2;
}

