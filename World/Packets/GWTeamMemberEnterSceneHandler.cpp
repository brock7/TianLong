#include "stdafx.h"
#include "GWTeamMemberEnterScene.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "OnlineUser.h"
#include "Log.h"
#include "Team.h"
#include "WGTeamResult.h"

UINT GWTeamMemberEnterSceneHandler::Execute( GWTeamMemberEnterScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
	GUID_t guid = pPacket->GetGUID(); // ��������Ϣ����ҵ� GUID

	USER* pUser = g_pOnlineUser->FindUser( guid );
	if( pUser == NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamMemberEnterScene...User GUID=%X not find!", 
			guid ) ;
		return PACKET_EXE_CONTINUE;
	}

	if( pUser->GetTeamID() == INVALID_ID )
	{ // �����Ժ�Ҫ���ϻ�����Ϣ��ɾ���������˴���Ķ�����Ϣ
		Assert( FALSE && "�����û�ж��飡" );
		return PACKET_EXE_CONTINUE;
	}

	Team* pTeam = g_pTeamList->GetTeam( pUser->GetTeamID() );
	if( pTeam==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_CONTINUE ;
	}

	WGTeamResult Msg;
	Msg.SetReturn( TEAM_RESULT_ENTERSCENE );
	Msg.SetTeamID( pUser->GetTeamID() );
	Msg.SetGUID( pPacket->GetGUID() );
	Msg.SetSceneID( pPacket->GetSceneID() );
	Msg.SetGUIDEx( pPacket->GetPlayerID() );
	Msg.SetIcon( pUser->GetPortrait() );

	for(int i=0; i<pTeam->MemberCount(); ++i)
	{
		TEAMMEMBER* pMember = pTeam->Member( i );
		if( pMember==NULL )
		{
			Assert(FALSE);
			continue;
		}

		USER* pUser = g_pOnlineUser->FindUser( pMember->m_Member );
		if( pUser==NULL )
		{
			Assert(FALSE);
			continue;
		}

		pServerPlayer = g_pServerManager->GetServerPlayer( pUser->GetServerID() );
		if( pServerPlayer == NULL )
		{
			Assert(FALSE);
			continue;
		}

		Msg.SetPlayerID( pUser->GetServerID() );
		pServerPlayer->SendPacket( &Msg );
	}

	Log::SaveLog( WORLD_LOGFILE, "GWTeamMemberEnterScene...Success!  GUID=%X", guid );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
