#include"stdafx.h"

#include "CGBankRemoveItem.h"


UINT CGBankRemoveItemHandler::Execute( CGBankRemoveItem* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
