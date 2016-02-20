#include "StdAfx.h"

#include "CGExecuteScript.h"


UINT CGExecuteScriptHandler::Execute( CGExecuteScript* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
