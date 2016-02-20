#include "stdafx.h"

#include "CGBBSSychMessages.h"


UINT CGBBSSychMessagesHandler::Execute( CGBBSSychMessages* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
