#include "StdAfx.h"
#include "GMActionSystem_PetSkill.h"
#include "..\Procedure\GameProcedure.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "TDEventSystem.h"
#include "TDEventDefine.h"
#include "TDInputSystem.h"
#include "..\Interface\GMGameInterface.h"
#include "..\Object\ObjectManager.h"
#include "..\Object\Character\Obj_PlayerMySelf.h"
#include "..\DataPool\GMDataPool.h"
#include "..\Event\GMEventSystem.h"
#include "..\DBC\GMDataBase.h"

CActionItem_PetSkill::CActionItem_PetSkill(INT nID)
	: CActionItem(nID)
{
}

CActionItem_PetSkill::~CActionItem_PetSkill()
{
}

VOID CActionItem_PetSkill::Update_PetSkill(const PET_SKILL* pPetSkill)
{
	TDAssert(pPetSkill);

	//����

	m_idPetSkillImpl =	pPetSkill->m_nPosIndex;
	//�˼��������ڵڼ�ֻ����ġ�
	m_nPetNum = pPetSkill->m_nPetNum;

//	m_idPetSkillImpl =	pPetSkill->m_pDefine->m_nID;
	//����
	m_strName = pPetSkill->m_pDefine->m_lpszName;
	//ͼ��
	m_strIconName = pPetSkill->m_pDefine->m_lpszIconName;

//	SetCheckState(pPetSkill->m_bCanUse);//�����Ϊ2005��������������ĵ�
	
	SetCheckState(FALSE);//����BUG 4396�������������

	//֪ͨUI
	UpdateToRefrence();
}

PET_SKILL* CActionItem_PetSkill::GetPetSkillImpl(VOID)
{
//	return CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill(m_idPetSkillImpl);
	if(m_nPetNum == MENPAI_PETSKILLSTUDY_PETNUM)
		return CDataPool::GetMe()->PetSkillStudy_MenPaiList_Get(m_idPetSkillImpl);
	else if(m_nPetNum == TARGETPET_INDEX)
		return CDataPool::GetMe()->TargetPet_GetSkill(m_idPetSkillImpl);
	else
		return CDataPool::GetMe()->Pet_GetSkill(m_nPetNum,m_idPetSkillImpl);
}

INT CActionItem_PetSkill::GetMyPetLevel(VOID)
{
	if(m_nPetNum == TARGETPET_INDEX)
	{
		SDATA_PET* MyPet = CDataPool::GetMe()->TargetPet_GetPet();

		return MyPet->m_nLevel;
	}
	else if(m_nPetNum == MENPAI_PETSKILLSTUDY_PETNUM)
		return -1;
	else
	{
		SDATA_PET* MyPet = CDataPool::GetMe()->Pet_GetPet(m_nPetNum);
		return MyPet->m_nLevel;
	}
	
}
INT	CActionItem_PetSkill::GetDefineID(VOID)
{
	const PET_SKILL* pSkill = GetPetSkillImpl();	
	if(!pSkill) return -1;

	return pSkill->m_pDefine->m_nID;
}

LPCTSTR CActionItem_PetSkill::GetDesc(VOID)
{
	const PET_SKILL* pSkill = GetPetSkillImpl();

	if(pSkill)
	{
		if(pSkill->m_pDefine)
		{
			// �õ����ܶ�Ӧ���ķ�����
			INT iPetLevel = GetMyPetLevel();
			if(iPetLevel>=0)
			{
				// ��������id
				int iSkillData_V1ID = pSkill->m_pDefine->m_anSkillByLevel[0];
				
				//�����ݱ�
				DBC_DEFINEHANDLE(s_pSkillData_V1, DBC_SKILLDATA_V1_DEPLETE);
				
				//������¼
				const _DBC_SKILLDATA_V1_DEPLETE* pSkillData_V1 = (const _DBC_SKILLDATA_V1_DEPLETE*)s_pSkillData_V1->Search_Index_EQU((UINT)iSkillData_V1ID);
			
				if(pSkillData_V1)
				{
					return pSkillData_V1->paraPair[11].szDesc;
				}
			
			}
		}

//		static TCHAR szBuf[32]= {0};
		return "ERROR";
	}
	else
		return "ERROR";//
}

//�õ���ȴ״��ID
INT	CActionItem_PetSkill::GetCoolDownID(VOID)
{
	const PET_SKILL* pSkill = GetPetSkillImpl();
	if(!pSkill) return -1;

	return pSkill->m_pDefine->m_nCooldownID;
}

INT CActionItem_PetSkill::GetPosIndex(VOID)
{
	const PET_SKILL* pSkill = GetPetSkillImpl();
	if(!pSkill || (pSkill == NULL)) return -1;

	return pSkill->m_nPosIndex;
}

VOID CActionItem_PetSkill::DoAction(VOID)
{
	//�����ȴ�Ƿ����
	if(!CoolDownIsOver()) return;

	PET_SKILL* pSkill = GetPetSkillImpl();
	if(!pSkill) return;

	// test code{
//	if(pSkill->m_nPosIndex != PET_SKILL_INDEX_CONTROL_BY_PLAYER)
//		return;

	SDATA_PET *pPetData = CDataPool::GetMe()->Pet_GetPet( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID() );

	SDATA_PET *pThisOwner = CDataPool::GetMe()->Pet_GetPet(m_nPetNum);

	if( pThisOwner->m_GUID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID() )
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "�㲻��ʹ��δ�ų�����ļ���");	
		return;
	}

	if(GetPetSkillImpl() && GetPetSkillImpl()->m_pDefine)
	{
		//POINT pt = CGameProcedure::s_pInputSystem->MouseGetPos();
		//fVector3 fvMouseHitPlan;
		//CObjectManager::GetMe()->GetMouseOverObject(pt.x, pt.y, fvMouseHitPlan);

		CObject *pTargetObj = (CObject*)(CObjectManager::GetMe()->GetMainTarget());

		//���ݲ�������
		switch(GetPetSkillImpl()->m_pDefine->m_nSelectType)
		{
			case SELECT_TYPE_NONE:	
			case SELECT_TYPE_CHARACTER:	
				{
					CDataPool::GetMe()->Pet_SendUseSkillMessage(GetPetSkillImpl()->m_pDefine->m_nID, 
						((pTargetObj)?(pTargetObj->GetServerID()):(INVALID_ID)),
						-1, -1);
					//�ָ�����Action
					CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
				}
				break;

			case SELECT_TYPE_POS:
				{
					CActionSystem::GetMe()->SetDefaultAction(this);
				}
				break;

			case SELECT_TYPE_SELF:
			default:
				{
					TDThrow("Invalid pet skill select type:%d", GetPetSkillImpl()->m_pDefine->m_nSelectType);
				}
				break;
		}
	}
}

BOOL CActionItem_PetSkill::CoolDownIsOver(VOID)
{
	INT nCoolDownID = GetCoolDownID();

	//����û����ȴ��Action
	if(INVALID_ID == nCoolDownID) return TRUE;
	//ȡ����ȴ������
	const CDataPool::COOLDOWN_GROUP* pCoolDownGroup =
		CDataPool::GetMe()->PetSkillCoolDownGroup_Get(nCoolDownID,m_nPetNum);

	if(pCoolDownGroup && pCoolDownGroup->nTime <= 0) 
		return TRUE;
	else 
		return FALSE;
}

//�϶�����
VOID CActionItem_PetSkill::NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName)
{
	if( bDestory )
	{
		CHAR cSourceType = szSourceName[0];
		switch(cSourceType)
		{
		case 'M':		//���˵�
			{
				INT nIndex = szSourceName[1]-'0';
				nIndex = szSourceName[2]-'0' + nIndex*10 ;
				CActionSystem::GetMe()->MainMenuBar_Set(nIndex, -1 );
			}
			break;
		default:
			break;
		}
	}

	//�϶����հ׵ط�
	if(!szTargetName || szTargetName[0]=='\0') return;
	
	CHAR cSourceName = szSourceName[0];
	CHAR cTargetType = szTargetName[0];

	//��������ϵ������������
	if( cSourceName == 'M' && cTargetType != 'M' )
		return;

	INT nOldTargetId = -1;
	INT nIndex =-1;
	INT nPet_Num = -1;

	const PET_SKILL* pPetSkill = GetPetSkillImpl();

	

	switch(cTargetType)
	{
	case 'M':		//���˵�
		{
			//��������ֶ��ļ��ܣ����ܷ�������
			if(pPetSkill->m_pDefine->m_nOperateModeForPetSkill != PET_SKILL_OPERATE_NEEDOWNER)
				break;
///////////////////////////////////////////////////
			//Ҫ���Ժ�߻��������ˣ��Ͱ����ע�͵���
			nPet_Num = GetNum();
			if(nPet_Num >= 0 && nPet_Num < HUMAN_PET_MAX_COUNT )
			{
				SDATA_PET* My_Pet = CDataPool::GetMe()->Pet_GetPet(nPet_Num);
				//������Ƿų��ĳ���Ͳ�Ҫ���ڿ��
				if(My_Pet->m_GUID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID() )
					break;
			}
///////////////////////////////////////////////////
			INT nIndex = szTargetName[1]-'0';
			nIndex = szTargetName[2]-'0' + nIndex*10 ;
			//��ѯĿ��λ��ԭ��������
			nOldTargetId = CActionSystem::GetMe()->MainMenuBar_Get(nIndex);
			CActionSystem::GetMe()->MainMenuBar_Set(nIndex, GetID() );
		}
		break;

	default:
		break;
	}

	switch(cSourceName)
	{
	case 'M':
		{
			INT nIndex = szSourceName[1]-'0';
			nIndex = szSourceName[2]-'0' + nIndex*10 ;
			CActionSystem::GetMe()->MainMenuBar_Set(nIndex, nOldTargetId);
		}
		break;
	default:
		break;
	}
//	CEventSystem::GetMe()->PushEvent(GE_UPDATE_SUPERTOOLTIP);
	CEventSystem::GetMe()->PushEvent(GE_SUPERTOOLTIP, "0");
}

VOID CActionItem_PetSkill::UpdateCoolDown(VOID)
{
	std::set< tActionReference* >::iterator it;
	INT nCoolDownID = GetCoolDownID();

	//����û����ȴ��Action
	if(-1 == nCoolDownID)
	{
		for(it=m_setReference.begin(); it!=m_setReference.end(); it++)
		{
			(*it)->EnterCoolDown(-1, 0.0f);
		}
		return;
	}

	//ȡ����ȴ������
	const CDataPool::COOLDOWN_GROUP* pCoolDownGroup =
			CDataPool::GetMe()->PetSkillCoolDownGroup_Get(nCoolDownID,m_nPetNum);

	INT nTimeNow   = pCoolDownGroup->nTime;
	INT nTimeTotal = pCoolDownGroup->nTotalTime;

	for(it=m_setReference.begin(); it!=m_setReference.end(); it++)
	{
		if(nTimeNow <= 0 || nTimeTotal <= 0)
		{
			(*it)->EnterCoolDown(-1, 0.0f);
		}
		else
		{
			(*it)->EnterCoolDown(nTimeNow, 1.0f - (FLOAT)nTimeNow/(FLOAT)nTimeTotal);
		}
	}
}
