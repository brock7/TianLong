#include "StdAfx.h"


#include "GCChannelError.h"
#include "..\..\Procedure\GameProcedure.h"



UINT GCChannelErrorHandler::Execute( GCChannelError* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	//��ǰ������������
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		SCRIPT_SANDBOX::Talk::s_Talk.HandleChannelErrorPacket(pPacket);
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
