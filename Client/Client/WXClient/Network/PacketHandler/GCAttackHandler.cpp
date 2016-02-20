#include "StdAfx.h"
#include "GCAttack.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\WxRender\RenderSystem.h"

UINT GCAttackHandler::Execute( GCAttack* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	//CGameProcedure::s_pGfxSystem->PushDebugString("GCAttackHandler");
	//COpe_PhysicalAttack::DoFromServer(pPacket);

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
