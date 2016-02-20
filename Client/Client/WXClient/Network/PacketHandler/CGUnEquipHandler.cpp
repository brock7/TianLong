#include "StdAfx.h"
#include "CGUnEquip.h"

UINT CGUnEquipHandler::Execute( CGUnEquip* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}