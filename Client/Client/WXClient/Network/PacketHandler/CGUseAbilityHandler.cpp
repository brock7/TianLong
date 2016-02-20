#include "StdAfx.h"
#include "CGUseAbility.h"

UINT CGUseAbilityHandler::Execute(CGUseAbility* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}

