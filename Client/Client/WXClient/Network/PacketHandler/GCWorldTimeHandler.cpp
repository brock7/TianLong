#include "StdAfx.h"
#include "GCWorldTime.h"

UINT GCWorldTimeHandler::Execute( GCWorldTime* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
