#include "StdAfx.h"
#include "CGCharJump.h"


UINT CGCharJumpHandler::Execute( CGCharJump* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
