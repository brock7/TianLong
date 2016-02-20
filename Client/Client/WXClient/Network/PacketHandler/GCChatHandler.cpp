#include "StdAfx.h"
#include "GCChat.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Character\Obj_Character.h"
#include "..\..\Object\ObjectManager.h"
#include "../../DBC/GMDataBase.h"
#include "TDException.h"
#include "../../Global.h"


UINT GCChatHandler::Execute( GCChat* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	//��ǰ������������
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		SCRIPT_SANDBOX::Talk::s_Talk.HandleRecvTalkPacket(pPacket);
	}
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
