#include "StdAfx.h"
#include "CGCharIdle.h"


UINT CGCharIdleHandler::Execute( CGCharIdle* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
