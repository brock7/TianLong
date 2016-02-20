#include "StdAfx.h"
#include "GCPackUpPacket.h"

UINT GCPackUpPacketHandler::Execute(GCPackUpPacket* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}