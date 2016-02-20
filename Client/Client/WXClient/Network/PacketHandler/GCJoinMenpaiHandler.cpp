// GCJoinMenpaiHandler.cpp

#include "StdAfx.h"

#include "..\..\Procedure\Gameprocedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Sound\GMSoundSystem.h"
#include "GCJoinMenpai.h"

UINT GCJoinMenpaiHandler::Execute( GCJoinMenpai* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		short shortSucceedJoin = pPacket->GetIsSucceedJoin();
		if( JOINMENPAI_SUCCEED == shortSucceedJoin )
		{
			CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"�������ɳɹ�!");
			CSoundSystemFMod::_PlayUISoundFunc(62);
		}
		else
		{
			CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"��������ʧ��!");
		}
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
