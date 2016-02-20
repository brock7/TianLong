#include "StdAfx.h"
#include "CGTest.h"



UINT CGTestHandler::Execute( CGTest* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
