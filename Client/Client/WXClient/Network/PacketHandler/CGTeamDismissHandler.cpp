#include "StdAfx.h"


#include "CGTeamDismiss.h"



UINT CGTeamDismissHandler::Execute( CGTeamDismiss* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
