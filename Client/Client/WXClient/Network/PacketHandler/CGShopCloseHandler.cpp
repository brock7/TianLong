#include "StdAfx.h"
#include "CGShopClose.h"

UINT CGShopCloseHandler::Execute( CGShopClose* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
