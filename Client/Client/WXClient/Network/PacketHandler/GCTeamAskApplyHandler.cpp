#include "StdAfx.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\event\GMEventSystem.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\DataPool\TeamOrGroup.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"

#include "GCTeamAskApply.h"



#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\Event\GMEventSystem.h"

#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Sound\GMSoundSystem.h"

UINT GCTeamAskApplyHandler::Execute( GCTeamAskApply* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
	//	CUIDataPool::GetMe()->SetSourceGUID( pPacket->GetSourGUID() );
	//	CUIDataPool::GetMe()->SetDestinationGUID( pPacket->GetDestGUID() );
	//	CEventSystem::GetMe()->PushEvent(GE_TEAM_APPLY, pPacket->GetSourName());

		CTeamOrGroup* pTeam = CUIDataPool::GetMe()->GetTeamOrGroup();

		if ( pTeam->HasTeam() == FALSE
		 ||  pTeam->GetLeaderGUID() != CObjectManager::GetMe()->GetMySelf()->GetServerGUID()
		 )
		{ // û�ж�����߲��Ƕӳ���ʱ���յ���Ϣ��ʾ�ͻ������ݳ�������
			TDAssert( FALSE );
			return PACKET_EXE_CONTINUE;
		}

		TeamCacheInfo* pProposerInfo = new(TeamCacheInfo);

		// ����һ��ui����ģ��
		pProposerInfo->m_UIModel.CreateNewUIModel();

		
		pProposerInfo->m_GUID = pPacket->GetSourGUID();
		strncpy(pProposerInfo->m_szNick, pPacket->GetSourName(), MAX_CHARACTER_NAME-1);
		pProposerInfo->m_szNick[MAX_CHARACTER_NAME-1] = '\0';
		pProposerInfo->m_uFamily     = pPacket->GetFamily();
		pProposerInfo->m_Scene       = pPacket->GetScene();
		pProposerInfo->m_uLevel      = pPacket->GetLevel();
		pProposerInfo->m_uDataID  = pPacket->GetDataID();
		pProposerInfo->m_uDetailFlag = pPacket->GetDetailFlag();

		if ( pProposerInfo->m_uDetailFlag>0 )
		{
			pProposerInfo->m_WeaponID = pPacket->GetWeaponID();
			pProposerInfo->m_CapID    = pPacket->GetCapID();
			pProposerInfo->m_ArmourID = pPacket->GetArmourID();
			pProposerInfo->m_CuffID   = pPacket->GetCuffID();
			pProposerInfo->m_FootID   = pPacket->GetFootID();
			pProposerInfo->m_uFaceMeshID = pPacket->GetFaceModel();
			pProposerInfo->m_uHairColor = pPacket->GetHairColor();
			pProposerInfo->m_uHairMeshID = pPacket->GetHairModel();

			pProposerInfo->m_UIModel.m_pAvatar->GetCharacterData()->Set_RaceID(pProposerInfo->m_uDataID);
			// ����uiģ��
			pProposerInfo->m_UIModel.SetUIModelInfo(HEQUIP_WEAPON,	pPacket->GetWeaponID());
			pProposerInfo->m_UIModel.SetUIModelInfo(HEQUIP_CAP,		pPacket->GetCapID());
			pProposerInfo->m_UIModel.SetUIModelInfo(HEQUIP_ARMOR,	pPacket->GetArmourID());
			pProposerInfo->m_UIModel.SetUIModelInfo(HEQUIP_CUFF,	pPacket->GetCuffID());
			pProposerInfo->m_UIModel.SetUIModelInfo(HEQUIP_BOOT,    pPacket->GetFootID());

			if(pProposerInfo->m_uFaceMeshID < 255)
			{
				// ��������
				pProposerInfo->m_UIModel.SetFaceMeshId(pProposerInfo->m_uFaceMeshID);
			}
				
			if(pProposerInfo->m_uHairMeshID < 255)
			{
				// ���÷���
				pProposerInfo->m_UIModel.SetFaceHairId(pProposerInfo->m_uHairMeshID);
			}
			
			//if(pProposerInfo->m_uHairColor < 255)
			//{
			//	// ������ɫ
			//	pProposerInfo->m_UIModel.SetHairColor(pProposerInfo->m_uHairColor);
			//}
			//else
			//{
			//	// ������ɫ
			//	pProposerInfo->m_UIModel.SetHairColor(0);
			//}//

			// ������ɫ
			pProposerInfo->m_UIModel.SetHairColor(pProposerInfo->m_uHairColor);

		}


		if ( pTeam->AddProposer(pProposerInfo) )
		{
			ADDNEWDEBUGMSG(STRING("�ж�Ա�����������"));
			CSoundSystemFMod::_PlayUISoundFunc(60);
		}

		// ֪ͨ������������������, ��˸��ť.
		CGameProcedure::s_pEventSystem->PushEvent(GE_TEAM_NOTIFY_APPLY, 1);

	}



__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
