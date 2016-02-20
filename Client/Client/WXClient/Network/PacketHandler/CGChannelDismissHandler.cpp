#include "StdAfx.h"


#include "CGChannelDismiss.h"




UINT CGChannelDismissHandler::Execute( CGChannelDismiss* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
