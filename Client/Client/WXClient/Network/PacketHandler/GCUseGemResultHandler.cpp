#include "StdAfx.h"
#include "GCUseGemResult.h"


UINT GCUseGemResultHandler::Execute(GCUseGemResult* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}