#include "stdafx.h"
#include "GWLevelUp.h"
#include "Log.h"
#include "OnlineUser.h"

using namespace Packets;

UINT GWLevelUpHandler::Execute(GWLevelUp* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	USER* pUser = g_pOnlineUser->FindUser( pPacket->GetGUID() );
	if( pUser==NULL )
	{//û�з����ڡ������û��б����Ӧ��GUID���û�
		Log::SaveLog( WORLD_LOGFILE, "GWLevelUpHandler...User GUID=%X not find!",
			pPacket->GetGUID() );

		Assert( FALSE && "�Ҳ�������" );
		return PACKET_EXE_CONTINUE;
	}

	pUser->SetLevel( pPacket->GetLevel() );

	Log::SaveLog( WORLD_LOGFILE, "GWLevelUpHandler...User GUID=%X Level=%d", 
		pPacket->GetGUID(), pPacket->GetLevel() ) ;

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
