#include "StdAfx.h"
#include "CGStallDefaultPage.h"


UINT CGStallDefaultPageHandler::Execute( CGStallDefaultPage* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
