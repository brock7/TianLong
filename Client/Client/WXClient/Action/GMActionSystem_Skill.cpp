#include "StdAfx.h"
#include "GMActionSystem_Skill.h"
#include "..\Procedure\GameProcedure.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "TDEventSystem.h"
#include "TDEventDefine.h"
#include "..\Interface\GMGameInterface.h"
#include "..\Object\ObjectManager.h"
#include "..\Object\Character\Obj_PlayerMySelf.h"
#include "..\DataPool\GMDataPool.h"
#include "..\Interface\GMInterface_Script_Tooltip.h"
#include "..\Event\GMEventSystem.h"

CActionItem_Skill::CActionItem_Skill(INT nID)
	: CActionItem(nID)
{
}

CActionItem_Skill::~CActionItem_Skill()
{
}

VOID CActionItem_Skill::Update_Skill(const SCLIENT_SKILL* pSkill)
{
	TDAssert(pSkill);

	//����
	m_idSkillImpl =	pSkill->m_pDefine->m_nID*100 + pSkill->m_nLevel;
	//����
	m_strName = pSkill->m_pDefine->m_lpszName;
	//ͼ��
	m_strIconName = pSkill->m_pDefine->m_lpszIconName;

	BOOL bOldEnable = IsEnabled();
	BOOL bNewEnable = FALSE;
	ORESULT oResult = pSkill->IsCanUse_CheckDeplete( CObjectManager::GetMe()->GetMySelf()->GetID());
	if ( OR_SUCCEEDED( oResult ) )
	{
		oResult = pSkill->IsCanUse_Leaned();
		if ( OR_SUCCEEDED( oResult ) )
		{
			bNewEnable = TRUE;
		}
	}

	if ( bNewEnable != bOldEnable )
	{
		(bNewEnable)?(Enable()):(Disable());
		//֪ͨUI
		UpdateToRefrence();
	}

	////����
	//m_idSkillImpl =	pSkill->m_pDefine->m_nID;
	////����
	//m_strName = pSkill->m_pDefine->m_lpszName;
	////ͼ��
	//m_strIconName = pSkill->m_pDefine->m_lpszIconName;
	//
	//const SCLIENT_XINFA* pXinfa = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_XinFa(pSkill->m_pDefine->m_nXinFaID);

	//if(pXinfa && pXinfa->m_nLevel < pSkill->m_pDefine->m_nXinFaLevelRequirement)
	//{
	//	Disable();
	//}
	////�Ƿ񼤻�
	//else if( (0==pSkill->m_nActiveTime)  || (FALSE == pSkill->m_bLeaned) )
	//{
	//	Disable();
	//}
	//else
	//{
	//	Enable();
	//}
	////֪ͨUI
	//UpdateToRefrence();
}

const SCLIENT_SKILL* CActionItem_Skill::GetSkillImpl(VOID)
{
	return CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill(m_idSkillImpl);
}

INT	CActionItem_Skill::GetDefineID(VOID)
{
	const SCLIENT_SKILL* pSkill = GetSkillImpl();	
	if(!pSkill) return -1;

	return pSkill->m_pDefine->m_nID*100 + pSkill->m_nLevel;
}

LPCTSTR CActionItem_Skill::GetDesc(VOID)
{
	SCLIENT_SKILL* pSkill = const_cast<SCLIENT_SKILL*>(GetSkillImpl());

	//if(pSkill)
	//{
	//	static CHAR szTemp[MAX_PATH];
	//	_snprintf(szTemp, MAX_PATH, "%s\n-------\n%s\n%s", 
	//		pSkill->m_pDefine->m_lpszName,
	//		pSkill->m_pDefine->m_pszDesc,
	//		pSkill->m_bLeaned ? "�Ѿ�ѧ��" : "δѧ��");
	//	return szTemp;
	//}
	//else
	//	return "ERROR";//

	if(pSkill)
	{
		
		return pSkill->GetSkillDesc();
	}//

	static TCHAR szBuf[32]= {0};
	return szBuf;
}

//�õ���ȴ״��ID
INT	CActionItem_Skill::GetCoolDownID(VOID)
{
	const SCLIENT_SKILL* pSkill = GetSkillImpl();
	if(!pSkill) return -1;

	return pSkill->m_pDefine->m_nCooldownID;
}

INT CActionItem_Skill::GetPosIndex(VOID)
{
	const SCLIENT_SKILL* pSkill = GetSkillImpl();
	if(!pSkill) return -1;

	return pSkill->m_nPosIndex;
}

BOOL CActionItem_Skill::AutoKeepOn(VOID)
{
	const SCLIENT_SKILL* pSkillImpl = GetSkillImpl();
	if(!pSkillImpl) return FALSE;

	//�Ƿ��ܹ��ܹ��Զ�����
	if(pSkillImpl->m_pDefine->m_bAutoRedo)
	{
		CGameInterface::GetMe()->Skill_SetActive(this);
		return TRUE;
	}

	return FALSE;
}

VOID CActionItem_Skill::DoAction(VOID)
{
	const SCLIENT_SKILL* pSkill = GetSkillImpl();
	if(!pSkill) return;

	ORESULT oResult;
	// 
	oResult = pSkill->IsCanUse_Leaned();
	if ( OR_FAILED( oResult ) )
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( oResult ) );
		return ;
	}
	////�����ȴ�Ƿ����
	oResult = pSkill->IsCanUse_CheckCoolDown();
	if ( OR_FAILED( oResult ) )
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( oResult ) );
		return ;
	}

	// ���ļ��
	INT idUser;
	CObject_PlayerMySelf *pMySelf = CObjectManager::GetMe()->GetMySelf( );
	idUser = (pMySelf)?(pMySelf->GetID()):(INVALID_ID);

	oResult = pSkill->IsCanUse_CheckDeplete( idUser );
	if ( OR_FAILED( oResult ) )
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( oResult ) );
		return ;
	}

	//�������������ô�ͻ�Ϊѡ��״̬��
	AutoKeepOn();
	//���ݲ�������
	switch(pSkill->m_pDefine->m_nSelectType)
	{
	case SELECT_TYPE_NONE:	
		{
			CGameProcedure::s_pGameInterface->Player_UseSkill(GetID());
			//�ָ�����Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;
	case SELECT_TYPE_CHARACTER:	
		{
			if(CObjectManager::GetMe()->GetMainTarget())
			{

				CGameProcedure::s_pGameInterface->Player_UseSkill(GetID(), 
					CObjectManager::GetMe()->GetMainTarget()->GetServerID());
			}
			else
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( OR_NO_TARGET ) );
			}
			//�ָ�����Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;
	case SELECT_TYPE_POS:
		CActionSystem::GetMe()->SetDefaultAction(this);
//		CGameProcedure::s_pGameInterface->Skill_SetActive(this);
		break;
	case SELECT_TYPE_DIR:
		CActionSystem::GetMe()->SetDefaultAction(this);
//		CGameProcedure::s_pGameInterface->Skill_SetActive(this);
		break;
	case SELECT_TYPE_SELF:		
		{
			CGameProcedure::s_pGameInterface->Player_UseSkill(GetID(), 
				CObjectManager::GetMe()->GetMySelf()->GetServerID());
			//�ָ�����Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;
	case SELECT_TYPE_HUMAN_GUID:
		{
			//��ǰ�Ƿ��Ѿ�ѡ����һ������
			GUID_t guid;
			CObjectManager::GetMe()->GetMainTarget(guid);
			if(guid == (GUID_t)-1)
			{
				//��δѡ��Ϸ��Ķ���
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "��ЧĿ��");
				return;
			}

			CGameProcedure::s_pGameInterface->Player_UseSkill(GetID(), (GUID_t)guid);
			//�ָ�����Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;
	}
}

BOOL CActionItem_Skill::CoolDownIsOver(VOID)
{
	INT nCoolDownID = GetCoolDownID();

	//����û����ȴ��Action
	if(INVALID_ID == nCoolDownID) return TRUE;
	//ȡ����ȴ������
	const CDataPool::COOLDOWN_GROUP* pCoolDownGroup =
		CDataPool::GetMe()->CoolDownGroup_Get(nCoolDownID);

	if( pCoolDownGroup && pCoolDownGroup->nTime <= 0 && CDataPool::GetMe()->CommonCoolDown_Get() <= 0 ) 
		return TRUE;
	else 
		return FALSE;
}

//�϶�����
VOID CActionItem_Skill::NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName)
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
	
	//�������������������disable״̬�������϶�
	if(!IsEnabled()) return;

	CHAR cSourceName = szSourceName[0];
	CHAR cTargetType = szTargetName[0];

	//��������ϵ������������
	if( cSourceName == 'M' && cTargetType != 'M' )
		return;

	INT nOldTargetId = -1;
	INT nIndex =-1;

	switch(cTargetType)
	{
	case 'M':		//���˵�
		{
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

// �õ����ܵȼ�
INT	CActionItem_Skill::GetSkillLevel()
{
	const SCLIENT_SKILL* pImpl = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill(m_idSkillImpl);
	if(pImpl)
	{
		return pImpl->m_pDefine->m_nLevelRequirement;
	}
	return -1;
}

// �õ����ܵȼ�
//INT	CActionItem_Skill::GetSkillXinfaLevel()
//{
//	const SCLIENT_SKILL* pImpl = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill(m_idSkillImpl);
//	if(pImpl)
//	{
//		INT id = 0;
//		
//		if(NULL == pImpl->m_pDefine)
//		{
//			return -1;
//		}
//
//
//		const SCLIENT_XINFA* pXinfa = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_XinFa(pImpl->m_pDefine->m_nXinFaID);
//		if(!pXinfa) 
//		{
//			return -1;
//		};
//
//		return pXinfa->m_nLevel;
//		
//	}
//	return -1;
//}

// �õ��Ƿ�ѧϰ�˼���
BOOL	CActionItem_Skill::GetIsLearnedSkill()
{
	const SCLIENT_SKILL* pImpl = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill(m_idSkillImpl);
	if(pImpl)
	{
		return pImpl->m_bLeaned;
	}

	return FALSE;

}
