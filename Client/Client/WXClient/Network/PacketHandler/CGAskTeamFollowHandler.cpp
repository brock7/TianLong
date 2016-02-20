#include "StdAfx.h"
#include "CGAskTeamFollow.h"

UINT CGAskTeamFollowHandler::Execute( CGAskTeamFollow* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
