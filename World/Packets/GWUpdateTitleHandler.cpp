#include "stdafx.h"

#include "GWUpdateTitle.h"
#include "Log.h"
#include "OnlineUser.h"

using namespace Packets;

UINT GWUpdateTitleHandler::Execute(GWUpdateTitle* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	USER* pUser = g_pOnlineUser->FindUser( pPacket->GetGUID() );
	if( pUser==NULL )
	{//û�з����ڡ������û��б����Ӧ��GUID���û�
		Log::SaveLog( WORLD_LOGFILE, "GWUpdateTitleHandler...User GUID=%X not find!",
			pPacket->GetGUID() );

		Assert( FALSE && "�Ҳ�������" );
		return PACKET_EXE_CONTINUE;
	}

	pUser->SetTitle( pPacket->GetTitle() );

	Log::SaveLog( WORLD_LOGFILE, "GWUpdateTitleHandler...User GUID=%X Changed Title to Title=%s", 
		pPacket->GetGUID(), pPacket->GetTitle() );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
