#include "StdAfx.h"


#include "CGTeamKick.h"



UINT CGTeamKickHandler::Execute( CGTeamKick* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
