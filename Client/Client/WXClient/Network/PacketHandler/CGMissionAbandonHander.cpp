#include "StdAfx.h"
#include "CGMissionAbandon.h"

using namespace Packets;

UINT CGMissionAbandonHandler::Execute(CGMissionAbandon* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;

}