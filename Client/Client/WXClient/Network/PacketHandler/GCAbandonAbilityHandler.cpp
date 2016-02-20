#include "StdAfx.h"
#include "GCAbandonAbility.h"

UINT GCAbandonAbilityHandler::Execute( GCAbandonAbility* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
