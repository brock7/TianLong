// GCTeamMemberInfoHandler.cpp

#include "StdAfx.h"
#include "GCTeamMemberInfo.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\TeamOrGroup.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"

UINT GCTeamMemberInfoHandler::Execute( GCTeamMemberInfo* pPacket, Player* pPlayer )
{ // ��ʱ�������Ŷ����
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pDataPool);
		CTeamOrGroup* team = pDataPool->GetTeamOrGroup();
		TeamMemberInfo* pTMInfo = team->GetMember( pPacket->getGUID() );

		if( pTMInfo == NULL )
		{ // ����л�����ʱ�������������
			// TDAssert(FALSE);
			return PACKET_EXE_CONTINUE;
		}

		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_FAMILY) )
		{
			pTMInfo->m_uFamily = pPacket->GetFamily();
		}

		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_LEVEL) )
		{
			pTMInfo->m_uLevel = pPacket->GetLevel();
		}

		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_POSITION) )
		{
			CGameProcedure::m_bWaitNeedFreshMinimap = true;
			pTMInfo->m_WorldPos.m_fX = pPacket->GetWorldPos()->m_fX;
			pTMInfo->m_WorldPos.m_fZ = pPacket->GetWorldPos()->m_fZ;
		}

		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_HP) )
		{
			pTMInfo->m_nHP = pPacket->GetHP();
		}

		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_MAX_HP) )
		{
			pTMInfo->m_dwMaxHP = pPacket->GetMaxHP();
		}

		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_MP) )
		{
			pTMInfo->m_nMP = pPacket->GetMP();
		}

		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_MAX_MP) )
		{
			pTMInfo->m_dwMaxMP = pPacket->GetMaxMP();
		}

		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_ANGER) )
		{
			pTMInfo->m_nAnger = pPacket->GetAnger();
		}

		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_WEAPON) )
		{
			// ����id
			pTMInfo->m_WeaponID = pPacket->GetWeaponID();

			// ����uiģ��
			pTMInfo->m_UIModel.SetUIModelInfo(HEQUIP_WEAPON, pTMInfo->m_WeaponID);

		}

		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_CAP) )
		{
			// ñ��id
			pTMInfo->m_CapID = pPacket->GetCapID();

			// ����uiģ��
			pTMInfo->m_UIModel.SetUIModelInfo(HEQUIP_CAP, pTMInfo->m_CapID);
		}

		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_ARMOR) )
		{
			// ����
			pTMInfo->m_ArmourID = pPacket->GetArmourID();

			// ����uiģ��
			pTMInfo->m_UIModel.SetUIModelInfo(HEQUIP_ARMOR, pTMInfo->m_ArmourID);
		}

		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_CUFF) )
		{
			// ��
			pTMInfo->m_CuffID = pPacket->GetCuffID();

			// ����uiģ��
			pTMInfo->m_UIModel.SetUIModelInfo(HEQUIP_CUFF, pTMInfo->m_CuffID);
		}

		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_BOOT) )
		{
			// ��
			pTMInfo->m_FootID = pPacket->GetFootID();

			// ����uiģ��
			pTMInfo->m_UIModel.SetUIModelInfo(HEQUIP_BOOT, pTMInfo->m_FootID);
		}

		/*
		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_BUFF) )
		{
		}
		*/

		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_DEADLINK) )
		{
			pTMInfo->m_bDeadLink = pPacket->IsDeadLink();
		}

		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_DEAD) )
		{
			pTMInfo->m_bDead = pPacket->IsDead();
		}

		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_FACEMESH) )
		{
			pTMInfo->m_uFaceMeshID = pPacket->GetFaceModel();
			if(
			pTMInfo->m_uFaceMeshID < 255)
			{
				// ��������
				pTMInfo->m_UIModel.SetFaceMeshId(pTMInfo->m_uFaceMeshID);
			}
		}

		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_HAIRMESH) )
		{
			pTMInfo->m_uHairMeshID = pPacket->GetHairModel();

			if(pTMInfo->m_uHairMeshID < 255)
			{
				// ���÷���
				pTMInfo->m_UIModel.SetFaceHairId(pTMInfo->m_uHairMeshID);
			}
		}

		if( pPacket->IsUpdateAttrib(TEAM_MEMBER_ATT_HAIRCOLOR) )
		{
			pTMInfo->m_uHairColor = pPacket->GetHairColor();
				
			//if(pTMInfo->m_uHairColor < 255)
			//{
			//	// ������ɫ
			//	pTMInfo->m_UIModel.SetHairColor(pTMInfo->m_uHairColor);
			//}
			//else
			//{
			//	// ������ɫ
			//	pTMInfo->m_UIModel.SetHairColor(0);
			//}//

			// ������ɫ
			pTMInfo->m_UIModel.SetHairColor(pTMInfo->m_uHairColor);
		}

		INT idx = team->GetMemberUIIndex( pTMInfo->m_GUID );
		CGameProcedure::s_pEventSystem->PushEvent(GE_TEAM_UPTEDATA_MEMBER_INFO, idx);
		for( idx =0; idx<team->GetTeamMemberCount(); ++idx )
		{
			if( team->GetMemberByIndex(idx)->m_GUID == pTMInfo->m_GUID )
			{
				CGameProcedure::s_pEventSystem->PushEvent(GE_TEAM_REFRESH_MEMBER, idx);
			}
		}
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
