
#include "stdafx.h"
#include "CGStallClose.h"


UINT CGStallCloseHandler::Execute( CGStallClose* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
