
#include "StdAfx.h"
#include "GCGuildApply.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"

#include "../../Object/ObjectManager.h"

UINT GCGuildApplyHandler::Execute( GCGuildApply* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		ObjID_t NpcId	=	pPacket->GetNpcId();		//Npc Id

		if(CObjectManager::GetMe()->FindServerObject((INT)NpcId))
		{
			//�򿪴������ɵĽ���
			INT careId = CObjectManager::GetMe()->FindServerObject(NpcId)->GetID();
			CEventSystem::GetMe()->PushEvent(GE_GUILD_CREATE,careId);
		}
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
