#include "stdafx.h"
#include "CGDispelBuffReq.h"

namespace Packets
{
	UINT CGDispelBuffReqHandler::Execute( CGDispelBuffReq* pPacket, Player* pPlayer )
	{
		__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

		__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
	}

}
