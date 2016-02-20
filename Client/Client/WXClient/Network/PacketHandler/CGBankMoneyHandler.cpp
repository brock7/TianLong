#include"stdafx.h"

#include "CGBankMoney.h"


UINT CGBankMoneyHandler::Execute( CGBankMoney* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
