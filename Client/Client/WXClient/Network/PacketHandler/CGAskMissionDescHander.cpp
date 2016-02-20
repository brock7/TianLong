#include "StdAfx.h"
#include "CGMissionAccept.h"

using namespace Packets;

UINT CGMissionAcceptHandler::Execute(CGMissionAccept* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;

}