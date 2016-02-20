
#include "stdafx.h"
#include "CGExchangeSynchLock.h"


UINT CGExchangeSynchLockHandler::Execute( CGExchangeSynchLock* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
