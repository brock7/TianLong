#include "StdAfx.h"
#include "GCNewItem.h"

UINT GCNewItemHandler :: Execute( GCNewItem* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

