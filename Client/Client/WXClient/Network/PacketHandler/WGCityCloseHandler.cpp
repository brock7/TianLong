
#include "stdAfx.h"
#include "WGCityClose.h"


UINT WGCityCloseHandler::Execute( WGCityClose* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
