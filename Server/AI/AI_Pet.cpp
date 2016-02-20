#include "stdafx.h"
// AI_Pet.cpp

#include "Obj_Character.h"
#include "TimeManager.h"
#include "Obj_Pet.h"
#include "Scene.h"
#include "GameTable.h"
#include "AI_Pet.h"
#include "GCPlayerRelive.h"
#include "GCManipulatePetRet.h"
#include "GCOperateResult.h"
#include "SinCos.h"

#define DELAY_TIME (5000)
#define PET_MOVE_RANGE (5)
#define MAX_TRY_TIMES (5)

AI_Pet::AI_Pet( VOID )
{
	m_RandMoveTimer.CleanUp();
	m_ScanImpactOfOwnerTimer.CleanUp();
}

AI_Pet::~AI_Pet( VOID )
{
}

BOOL AI_Pet::Init( Obj_Character *pCharacter )
{
__ENTER_FUNCTION

	BOOL bResult = AI_Character::Init( pCharacter );
	if ( !bResult )
	{
		return FALSE;
	}

	m_RandMoveTimer.CleanUp();
	m_ScanImpactOfOwnerTimer.CleanUp();

	Obj_Pet* pPet = (Obj_Pet*)m_pCharacter;
	if(NULL==m_pCharacter)
	{
		AssertEx(FALSE,"[AI_Pet::Init]: NULL m_pCharacter Found!! check it now.");
		return FALSE;
	}

	m_nDelayTime = DELAY_TIME;

	return TRUE;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID AI_Pet::Term( VOID )
{
	AI_Character::Term( );
}

VOID AI_Pet::AI_Logic_Idle( UINT uTime )
{
__ENTER_FUNCTION

	Obj_Pet* pPet = (Obj_Pet*)GetCharacter();
	if (!pPet->GetCreator()) 
	{
		if( !m_RandMoveTimer.IsSetTimer() )
		{
			m_RandMoveTimer.BeginTimer(DELAY_TIME+rand()%DELAY_TIME, uTime ) ;
		}
		if( m_RandMoveTimer.CountingTimer( uTime ) )
		{
			ToRandMove( ) ;
		}
	}
	else 
	{
		if (m_nDelayTime > 0)
		{
			m_nDelayTime -= m_pCharacter->GetLogicTime();
		}
		else
		{
			if (m_pCharacter && !m_pCharacter->IsMoving() )
			{// �鿴��������û��Ҫ�����ļ��ܣ������ʱ����
				m_pCharacter->SetMoveMode(Obj_Pet::MOVE_MODE_WALK);
				ProcessSkillInCache(FALSE);
			}
		}
		ChangeMoveMode();
	}

__LEAVE_FUNCTION	
}

VOID AI_Pet::AI_Logic_Combat( UINT uTime )
{
__ENTER_FUNCTION

	Obj_Pet* pPet = (Obj_Pet*)m_pCharacter;
	if(NULL==m_pCharacter)
	{
		AssertEx(FALSE,"[AI_Pet::Logic_Attack]: NULL m_pCharacter Found!! check it now.");
		return ;
	}
	// �Ƿ����ս��
	if (IsCombatBeOver())
	{
		ChangeState(ESTATE_IDLE);
		pPet->SetTargetID(INVALID_ID);
		return ;
	}
	// �����ǰ������û�����򲻽����κβ���
	if ( FALSE == m_pCharacter->CanUseSkillNow())
	{
		return ;
	}

	if ( m_pCharacter->IsMoving() ) 
	{// �������Moving�򲻽��й���
		return ;
	}


	// ִ�л����е��﹦�ͻ򷨹��͵ļ���
	if (FALSE == ProcessSkillInCache() )
	{// Ĭ��ʹ��0����ͨ����
		// �ж�Ŀ�����Ч��
		Obj* pTarObj = m_pCharacter->getScene()->GetObjManager()->GetObj( pPet->GetTargetID() );
		if ( pTarObj == NULL || !((Obj_Character*)pTarObj)->IsAlive() )
		{// Ŀ����Ч
			pPet->SetTargetID( INVALID_ID );

			ChangeState(ESTATE_IDLE);
			return ;
		}

		UseSkill(0, 1, pPet->GetTargetID(), pTarObj->getWorldPos()->m_fX, pTarObj->getWorldPos()->m_fZ); 
	}

__LEAVE_FUNCTION

}

VOID AI_Pet::AI_Logic_Dead( UINT uTime )
{
__ENTER_FUNCTION

	if ( m_BodyTimer.CountingTimer( uTime ) )
	{
		m_BodyTimer.CleanUp();
		Obj_Pet *pPet = GetCharacter();
		if (pPet)
		{
			const Obj_Character::_RELIVE_INFO *pReliveInfo = pPet->GetReliveInfo(TRUE);
			Obj_Human* pCreator = pPet->GetCreator();
			if (pCreator != NULL)
			{
				if (pReliveInfo != NULL )
				{
					pCreator->SetGUIDOfCallUpPet(pPet->GetPetGUID());
					pCreator->CallUpPet();
					Obj_Pet* pNewPet = pCreator->GetPet();
					if (pNewPet)
					{
						pNewPet->SetReliveInfo(TRUE, pPet->GetReliveInfo(TRUE));
					}
					AI_Pet* pAI = (AI_Pet*)(pNewPet->GetAIObj());
					if (pAI)
					{
						pAI->Relive(TRUE);
					}
				}
				pCreator->SetPetHP(pPet->GetPetGUID(), pCreator->GetPetMaxHP(pPet->GetPetGUID()));
				// ���³����һЩ��Ϣ
				pCreator->RefeshPetDetailAttrib(pPet->GetPetGUID());
			}				
			pPet->SetActiveFlag( FALSE );
		}
	}

__LEAVE_FUNCTION
}

VOID	AI_Pet::ChangeMoveMode()
{
__ENTER_FUNCTION

	Obj_Pet* pPet = GetCharacter();
	if (!pPet)
	{
		return ;
	}
	Obj_Character* pOwner = pPet->GetOwner();
	if (!pOwner)
	{
		return ;
	}
	FLOAT fDistSqr = CalcDistSqrOfToOwner();
	if ( fDistSqr > REFOLLOW_DISTSQR_C ) 
	{// �������15m��˲��
		if (pPet->CanMove()) 
		{// ��ǿ�����²���˲��:�綨��, ѣ��...
			FLOAT fDir = pOwner->getDir()-__PI/2;

			FLOAT fX = pOwner->getWorldPos()->m_fX - (-(2*sin(fDir)) );
			FLOAT fZ = pOwner->getWorldPos()->m_fZ - (2*cos(fDir));
			WORLD_POS pos;
			pos.m_fX = fX;
			pos.m_fZ = fZ;

			pPet->getScene()->GetMap()->VerifyPos(&pos);
			pPet->Teleport( &pos );
			pPet->SetMoveMode(Obj_Pet::MOVE_MODE_WALK);
			return ;
		}
	}
	else if (fDistSqr > REFOLLOW_DISTSQR_B) 
	{// �������8m�������
		pPet->SetMoveMode(Obj_Pet::MOVE_MODE_SPRINT);
	}
	else if (fDistSqr > REFOLLOW_DISTSQR_A)
	{// �������3m��С��
		if (pPet->GetMoveMode() != Obj_Pet::MOVE_MODE_SPRINT)
			pPet->SetMoveMode(Obj_Pet::MOVE_MODE_RUN);
	}
	else
	{// ������3m���ڣ���������
		if (pPet->GetMoveMode() != Obj_Pet::MOVE_MODE_WALK)
			pPet->SetMoveMode(Obj_Pet::MOVE_MODE_HOBBLE);
	}

	if (!m_pCharacter->IsMoving() && fDistSqr > 4.1f)
		Baby_Go(pOwner->getWorldPos());

__LEAVE_FUNCTION
}

FLOAT	AI_Pet::CalcDistSqrOfToOwner(VOID)
{
__ENTER_FUNCTION
	Obj_Pet* pPet = GetCharacter();
	if (pPet)
	{
		Obj_Character* pOwner = GetCharacter()->GetOwner();
		if (pOwner)
		{
			FLOAT fDeltaX = pOwner->getWorldPos()->m_fX - m_pCharacter->getWorldPos()->m_fX;
			FLOAT fDeltaZ = pOwner->getWorldPos()->m_fZ - m_pCharacter->getWorldPos()->m_fZ;

			return (fDeltaX*fDeltaX + fDeltaZ*fDeltaZ);
		}
	}

	return 0.f;
__LEAVE_FUNCTION
	return 0.f;
}

VOID AI_Pet::Event_OnDie( Obj *pKiller/* = NULL*/ )
{
__ENTER_FUNCTION

	ObjID_t idKiller = INVALID_ID;
	if ( pKiller != NULL )
	{
		idKiller = pKiller->GetID() ;
	}
	
	After_Die();

	//////////////////////////////////////////////////////////////////
	// ������︴�����
	UINT nTermTime = 0;
	Obj_Pet* pPet = GetCharacter();
	if (pPet)
	{
		const Obj_Character::_RELIVE_INFO *pReliveInfo = pPet->GetReliveInfo(TRUE);
		if (pReliveInfo != NULL)
		{
			Obj_Human* pCreator = pPet->GetCreator();
			if (pCreator)
			{
				nTermTime = BODYSTAYTIME / 3;
			}
		}
	}
	if (nTermTime)
		m_BodyTimer.BeginTimer(nTermTime, g_pTimeManager->CurrentTime());
	else
		m_BodyTimer.BeginTimer(BODYSTAYTIME, g_pTimeManager->CurrentTime());

    ChangeState(ESTATE_DEAD);	


__LEAVE_FUNCTION    
}

VOID AI_Pet::Baby_Go(const WORLD_POS *paTargetPos)
{
__ENTER_FUNCTION
	if (!paTargetPos)
	{
		Assert(NULL && "AI_Pet::Baby_Go...paTargetPos=NULL...");
		return ;
	}
	if (!m_pCharacter)
	{
		return ;
	}

	Obj_Character* pOwner = GetCharacter()->GetOwner();
	if (!pOwner)
	{
		return ;
	}
	
	FLOAT fDir = pOwner->getDir()-__HALF_PI;

	FLOAT fX = paTargetPos->m_fX + (2*SinCos::Sin(fDir));
	FLOAT fZ = paTargetPos->m_fZ + (2*SinCos::Cos(fDir));
	WORLD_POS pos;
	pos.m_fX = fX;
	pos.m_fZ = fZ;
	// ȷ��λ�õĺϷ���
	m_pCharacter->getScene()->GetMap()->VerifyPos(&pos);
	if (m_pCharacter->getScene()->GetMap()->IsCanGo(pos,m_pCharacter->GetDriverLevel())) {
		Move( &pos );
	}
	else {
		Move(paTargetPos);
	}

__LEAVE_FUNCTION
}

VOID AI_Pet::BabyToAttack(VOID)
{
__ENTER_FUNCTION
	
	ChangeState(ESTATE_COMBAT);

__LEAVE_FUNCTION
}

VOID AI_Pet::Relive( BOOL bSkillRelive )
{
__ENTER_FUNCTION

	Obj_Pet* pPet = GetCharacter();
	if (!pPet)
	{
		Assert(NULL && "AI_Pet::Relive...pPet=NULL...");
		return ;
	}
	Obj_Human *pCreator = pPet->GetCreator();
	if (!pCreator || !pCreator->IsAliveInDeed())
	{
		return ;
	}
	const Obj_Character::_RELIVE_INFO *pReliveInfo = pPet->GetReliveInfo(bSkillRelive);
	if ( pReliveInfo != NULL )
	{
		INT nHP;
		if ( pReliveInfo->m_bHPUsePercent )
		{
			nHP = (pReliveInfo->m_nHPPercent * pPet->GetMaxHP() ) / 100;
			pPet->SetHP( nHP );
		}
		else
		{
			if ( pReliveInfo->m_nHP != -1 )
			{
				nHP = pReliveInfo->m_nHP;
				pPet->SetHP( nHP );
			}
		}

		// ��֤һ���Ḵ��
		if(pPet->GetHP() <= 0)
		{
			pPet->SetHP(1);
		}

		INT nMP;
		if ( pReliveInfo->m_bMPUsePercent )
		{
			nMP = (pReliveInfo->m_nMPPercent * pPet->GetMaxMP() ) / 100;
			pPet->SetMP( nMP );
		}
		else
		{
			if ( pReliveInfo->m_nMP != -1 )
			{
				nMP = pReliveInfo->m_nMP;
				pPet->SetMP( nMP );
			}
		}

		INT nSP;
		if ( pReliveInfo->m_bSPUsePercent )
		{
			nSP = (pReliveInfo->m_nSPPercent * pPet->GetMaxRage() ) / 100;
			pPet->SetRage( nSP );
		}
		else
		{
			if ( pReliveInfo->m_nSP != -1 )
			{
				nSP = pReliveInfo->m_nSP;
				pPet->SetRage( nSP );
			}
		}
		
		// ����Ҫ���ó�����߶�
		pPet->SetHappiness(g_Config.m_ConfigInfo.m_PetCallUpHappiness+1);

		SceneID_t idCurScene;
		if ( pPet->getScene() != NULL )
		{
			idCurScene = pPet->getScene()->SceneID();
		}
		else
		{
			idCurScene = INVALID_ID;
		}

		pPet->Teleport( &(pReliveInfo->m_Pos) );
	}
	pPet->ResetReliveInfo();

	// ����к�����Ч�򽫸ó������еļ��ܻ�������
	Item* pPetItem = pCreator->GetPetItem(pPet->GetPetGUID());
	if (pPetItem)
	{
		for (INT i = 0; i < PET_MAX_SKILL_COUNT; ++i)
		{
			pPet->PushSkillToCache(pPetItem->GetSkill(i).m_nSkillID);
		}
	}

	ChangeState(ESTATE_IDLE);

__LEAVE_FUNCTION
}

VOID AI_Pet::After_Die()
{
__ENTER_FUNCTION

	Obj_Pet* pPet = GetCharacter();
	if (pPet)
	{
		// �����ڲ�׽������ҷ���ʧ����Ϣ����ϵ�ǰʦ��
		pPet->SendCaptureFailedToOthers(INVALID_ID);
        Obj_Character* pOwner = pPet->GetOwner();
		if (pOwner && pOwner->GetObjType() == Obj::OBJ_TYPE_HUMAN)
		{
			Obj_Human* pHuman = (Obj_Human*)pOwner;
			if (!pHuman) {
				return;
			}
			PET_GUID_t guidPet = pPet->GetPetGUID();
			// ���ջس���ʱ�������������г����ֵ�BUFFͳͳ���
			pHuman->Impact_CleanAllImpactWhenPetDead(pPet->GetID());
			pPet->Impact_CleanAllImpactWhenPetDead(pPet->GetID());
			// ǿ��ˢ�³�����Ϣ
			pPet->SendMsg_RefeshAttrib();
			
			INT nIndex = pHuman->GetPetIndexByGUID(guidPet);
			Item *pPetItem = pHuman->GetPetItem(guidPet);
			if(pPetItem != NULL)
			{
				//pHuman->SetPetHP(guidPet, pHuman->GetPetMaxHP(guidPet));
				if(pHuman->GetPetHappiness(guidPet) > 50)
				{
					pHuman->SetPetHappiness(guidPet, 50);
				}
				// ���³����һЩ��Ϣ���������ֶ�
				pHuman->RefeshPetDetailAttrib(guidPet);
			}
			// �������˵�ǰ�ĳ���guid
			pHuman->CleanupPet();
			guidPet.Reset();
			pHuman->SetCurrentPetGUID(guidPet);

			pHuman->SendOperateResultMsg(OR_PET_DIE);

		}
	}
__LEAVE_FUNCTION    
}

ORESULT AI_Pet::PushCommand_UseSkill(ObjID_t idSkill, ObjID_t idTarget, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir, GUID_t guidTarget)
{
__ENTER_FUNCTION
	Obj_Pet* pPet = GetCharacter();
	if (!pPet)
	{
		Assert(NULL && "AI_Pet::PushCommand_UseSkill...pPet=NULL...");
		return OR_ERROR;
	}
	
	SLogicSkill_Param SkillParam;
	SkillParam.idSkill = idSkill;
	SkillParam.idTarget = idTarget;
	SkillParam.TargetPos = WORLD_POS(fTargetX, fTargetZ);
	SkillParam.fDir = fDir;
	SkillParam.guidTarget = guidTarget;

	SetSkillParamCache(SkillParam);

	return OR_OK;
__LEAVE_FUNCTION
	return OR_ERROR;
}

BOOL AI_Pet::ProcessSkillInCache(BOOL IsAttackState/*= TRUE*/)
{
__ENTER_FUNCTION
	
	Obj_Pet* pPet = GetCharacter();
	if (!pPet)
	{
		Assert(NULL && "AI_Pet::ProcessSkillInCache...pPet=NULL...");
		return FALSE;
	}
	if (FALSE == pPet->IsActiveObj() )
	{
		return FALSE;
	}
    Obj_Human* pCreator = pPet->GetCreator();
    if (!pCreator)
	{// ������Ч��ֱ�ӷ���
		return FALSE;
	}
	if (FALSE == pPet->CanUseSkillNow() )
	{
		return FALSE;
	}
	// ��ִ�л����еļ���--m_SkillID
	const SLogicSkill_Param& SkillParam = GetSkillParamCache();
	if (SkillParam.IsValid() == TRUE)
	{
		INT nRet = UseSkill(SkillParam.idSkill, 1, SkillParam.idTarget, 
			SkillParam.TargetPos.m_fX, SkillParam.TargetPos.m_fZ, SkillParam.fDir, SkillParam.guidTarget);
		const_cast<SLogicSkill_Param&>(SkillParam).Cleanup();
		if(OR_FAILED(nRet))
		{
			pCreator->SendOperateResultMsg(nRet);
		}
		return TRUE;
	}

	INT i;
	for ( i = 0; i < HUMAN_PET_MAX_COUNT; ++i)
	{
		if (INVALID_ID != pPet->m_aCacheOfSkill[i])
		{
			BOOL bCooldowned = pPet->Skill_IsSkillCooldowned(pPet->m_aCacheOfSkill[i]);
			if (!bCooldowned)
			{// �ü��ܵ�CD��û�е�
				continue;
			}
			const SkillTemplateData_T* pSkillData_T = g_SkillTemplateDataMgr.GetInstanceByID(pPet->m_aCacheOfSkill[i]);
			if (!pSkillData_T)
			{
				Assert(NULL && "AI_Pet::ProcessSkillInCache...pSkillData_T=NULL...");
				return FALSE;
			}

			ID_t OperateMode = pSkillData_T->GetOperateModeForPetSkill();
			ID_t TypeOfSkill = pSkillData_T->GetTypeOfPetSkill();

			// ����ǽ���״̬��ֻ�ͷ��﹦�ͻ򷨹��͵ļ���
			if (TRUE == IsAttackState)
			{
				ID_t RateOfSkill = pSkillData_T->GetPetRateOfSkill();
				RateOfSkill == -1 ? RateOfSkill = 100 : NULL;
				FLOAT fLaunchRate = g_PetAIStrategyTble[TypeOfSkill][pPet->GetAIType()];
				INT nUseSkillRate = (INT)(RateOfSkill * fLaunchRate);

				if (rand() % 100 > nUseSkillRate)
				{// ��������ü���ʧ�����ж���һ��
					continue;
				}

				if (PET_SKILL_OPERATE_AISTRATEGY == OperateMode && 
					(PET_TYPE_AISKILL_PHYSICATTACK == TypeOfSkill || PET_TYPE_AISKILL_MAGICATTACK == TypeOfSkill) )
				{
					// �ж�Ŀ�����Ч��
					Obj* pTarObj = m_pCharacter->getScene()->GetObjManager()->GetObj( pPet->GetTargetID() );
					if ( pTarObj == NULL || !((Obj_Character*)pTarObj)->IsAlive() )
					{// Ŀ����Ч���л��ɸ���״̬
						pPet->SetTargetID( INVALID_ID );
//						SetAIState( AI_PET_FOLLOW );
						return FALSE;
					}

					ORESULT oResult = UseSkill(pPet->m_aCacheOfSkill[i], 1, pTarObj->GetID(), 
						pTarObj->getWorldPos()->m_fX, pTarObj->getWorldPos()->m_fZ);

					if (OR_OK == oResult)
					{
						break;
					}
					continue;
				}
			}
			if (PET_SKILL_OPERATE_INCEACEATTR == OperateMode || PET_SKILL_OPERATE_AISTRATEGY == OperateMode)
			{// �������ǿ�������Ե���ν�����Եı������ܣ�ִ��һ�ξͽ��ӻ�����ɾ��
				ORESULT oResult =UseSkill(pPet->m_aCacheOfSkill[i], 1, pPet->GetID(), 
					pPet->getWorldPos()->m_fX, pPet->getWorldPos()->m_fZ);

				if (OR_OK == oResult)
				{
					pPet->m_aCacheOfSkill[i] = INVALID_ID;
					break;
				}
				continue;
			}

			if (PET_SKILL_OPERATE_AISTRATEGY == OperateMode)
			{
				if (PET_TYPE_AISKILL_PROTECTEOWNER == TypeOfSkill)
				{// ����ǻ����͵ļ��ܣ���Ҫ�������������û�иü��ܲ�����ImpactID
					UINT uTime = g_pTimeManager->CurrentTime();
					if (!m_ScanImpactOfOwnerTimer.IsSetTimer() )
					{
						m_ScanImpactOfOwnerTimer.BeginTimer(SCAN_IMPACTOFOWNER_INTERVAL, uTime);
					}
					else if (!m_ScanImpactOfOwnerTimer.CountingTimer(uTime) )
					{
						continue;
					}

					BOOL bHaveImpact = pPet->Impact_HaveImpactOfSpecificImpactID(pSkillData_T->GetImpactIDOfSkill() );
					if (!bHaveImpact)
					{// û�и�Ч����ʩ���ü���
						ORESULT oResult = UseSkill(pPet->m_aCacheOfSkill[i], 1, pCreator->GetID(), 
							pCreator->getWorldPos()->m_fX, pCreator->getWorldPos()->m_fZ);

						if (OR_OK == oResult)
						{
							break;
						}
						continue;
					}
				}
			}
			continue;
		}
	}
	if (HUMAN_PET_MAX_COUNT == i)
	{
		return FALSE;
	}

	return TRUE;	

__LEAVE_FUNCTION
	return FALSE;
}

VOID AI_Pet::ToRandMove()
{
__ENTER_FUNCTION
	
	WORLD_POS Tar;
	BOOL FindPos=FALSE;
	for (INT i=0; i<MAX_TRY_TIMES; i++)
	{
		Tar.m_fX = GetCharacter()->GetRespawnPos()->m_fX+rand()%PET_MOVE_RANGE - rand()%PET_MOVE_RANGE;
		Tar.m_fZ = GetCharacter()->GetRespawnPos()->m_fZ+rand()%PET_MOVE_RANGE - rand()%PET_MOVE_RANGE;
		GetCharacter()->getScene()->GetMap()->VerifyPos( &Tar );
		if (GetCharacter()->getScene()->GetMap()->IsCanGo(Tar,GetCharacter()->GetDriverLevel()) )
		{
			FindPos = TRUE ;
			break ;
		}
	}

	if( FindPos==FALSE )
		return ;
	if(Move(&Tar) == FALSE)
		return ;

__LEAVE_FUNCTION
}

BOOL	AI_Pet::IsCombatBeOver()
{
__ENTER_FUNCTION
	Obj_Pet* pPet = (Obj_Pet*)m_pCharacter;
	if (!pPet) 
	{
		Assert(NULL && "AI_Pet::IsCombatBeOver...pPet=NULL...");
		return TRUE;
	}
	Obj_Human* pCreator = pPet->GetCreator();
	if (!pCreator)
	{
		return TRUE;
	}
	if (pCreator->GetStealthLevel() > 0)
	{
		return TRUE;
	}
	if (INVALID_ID != pPet->GetTargetID())
	{
		Obj* pObj = pPet->getScene()->GetObjManager()->GetObj(pPet->GetTargetID());
		if (pObj && IsCharacterObj(pObj->GetObjType())==TRUE)
		{
			if ( ((Obj_Character*)pObj)->IsCanViewMe(pCreator) == FALSE )
			{// �����������˿�����Ŀ����ս������
				return TRUE;
			}
			FLOAT fDeltaX = pObj->getWorldPos()->m_fX - pPet->getWorldPos()->m_fX;
			FLOAT fDeltaZ = pObj->getWorldPos()->m_fZ - pPet->getWorldPos()->m_fZ;
			FLOAT fDistSqr = fDeltaX*fDeltaX + fDeltaZ*fDeltaZ;

			if ( fDistSqr > REFOLLOW_DISTSQR_C ) 
			{// �������15m���Զ�����ս��
				return TRUE;			
			}
		}
	}
	
	return FALSE;
__LEAVE_FUNCTION
	return FALSE;
}
