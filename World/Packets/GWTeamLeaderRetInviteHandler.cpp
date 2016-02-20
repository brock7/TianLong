#include "stdafx.h"


#include "GWTeamLeaderRetInvite.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "WGTeamError.h"
#include "Team.h"
#include "ServerManager.h"
#include "WGTeamResult.h"
#include "WGTeamLeaderAskInvite.h"
#include "WGTeamAskInvite.h"


UINT GWTeamLeaderRetInviteHandler::Execute( GWTeamLeaderRetInvite* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GUID_t sGuid = pPacket->GetSourGUID();//�����˵�GUID
	GUID_t dGuid = pPacket->GetDestGUID();//�������˵�GUID
	GUID_t lGuid = pPacket->GetLeaderGUID();//�ӳ���GUID


	USER* pSourUser = g_pOnlineUser->FindUser( sGuid );
	if( pSourUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderRetInviteHandler...User Sour GUID=%X not find!", 
			sGuid );

		return PACKET_EXE_CONTINUE;
	}

	USER* pDestUser = g_pOnlineUser->FindUser( dGuid );
	if( pDestUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderRetInviteHandler...User Dest GUID=%X not find!", 
			dGuid );

		return PACKET_EXE_CONTINUE;
	}

	USER* pLeaderUser = g_pOnlineUser->FindUser( lGuid );
	if( pLeaderUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderRetInviteHandler...User Leader GUID=%X not find!", 
			lGuid );

		return PACKET_EXE_CONTINUE;
	}

	ID_t SourServerID = pSourUser->GetServerID();
	ServerPlayer* pSourServerPlayer = g_pServerManager->GetServerPlayer( SourServerID );
	if( pSourServerPlayer==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	ID_t DestServerID = pDestUser->GetServerID();
	ServerPlayer* pDestServerPlayer = g_pServerManager->GetServerPlayer( DestServerID );
	if( pDestServerPlayer==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	ServerPlayer* pLeaderServerPlayer = (ServerPlayer*)pPlayer;

	if( pSourUser->GetTeamID() != pLeaderUser->GetTeamID() )
	{ // �����˺Ͷӳ��Ѿ�����һ��������
		WGTeamError lMsg;
		lMsg.SetPlayerID( pLeaderUser->GetPlayerID() );
		lMsg.SetGUID( pLeaderUser->GetGUID() );
		lMsg.SetErrorCode( TEAM_ERROR_INVITERNOTINTEAM );
		pLeaderServerPlayer->SendPacket( &lMsg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderRetInviteHandler...Inviter(sGUID=%X) invited dGUID=%X not in Leader's(lGUID=%X) team.", 
			sGuid, dGuid, lGuid );
	}
	else if( pPacket->GetReturn()==FALSE )
	{//�ӳ���ͬ��
		WGTeamError sMsg;
		sMsg.SetPlayerID( pSourUser->GetPlayerID() );
		sMsg.SetGUID( pLeaderUser->GetGUID() );
		sMsg.SetErrorCode( TEAM_ERROR_INVITELEADERREFUSE );
		pSourServerPlayer->SendPacket( &sMsg );
/*
		WGTeamError dMsg;
		dMsg.SetPlayerID( pDestUser->m_PlayerID );
		dMsg.SetErrorCode( TEAM_ERROR_INVITELEADERREFUSE );
		pDestServerPlayer->SendPacket( &dMsg );
*/
		Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderRetInviteHandler...Invite Leader Refuse sGUID=%X dGUID=%X lGUID=%X", 
			sGuid, dGuid, lGuid );
	}
	else if( pDestUser->GetTeamID() != INVALID_ID )
	{//�����������ж�����
		WGTeamError sMsg;
		sMsg.SetPlayerID( pSourUser->GetPlayerID() );
		sMsg.SetGUID( pLeaderUser->GetGUID() );
		sMsg.SetErrorCode( TEAM_ERROR_INVITEDESTHASTEAM );
		pSourServerPlayer->SendPacket( &sMsg );
/*
		WGTeamError dMsg;
		dMsg.SetPlayerID( pDestUser->m_PlayerID );
		dMsg.SetErrorCode( TEAM_ERROR_INVITEDESTHASTEAM );
		pDestServerPlayer->SendPacket( &dMsg );
*/
		WGTeamError lMsg;
		lMsg.SetPlayerID( pLeaderUser->GetPlayerID() );
		lMsg.SetGUID( pLeaderUser->GetGUID() );
		lMsg.SetErrorCode( TEAM_ERROR_INVITEDESTHASTEAM );
		pLeaderServerPlayer->SendPacket( &lMsg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderRetInviteHandler...DestInTeam  sGUID=%X dGUID=%X lGUID=%X", 
			sGuid, dGuid, lGuid );
	}
	else
	{//�������
		TeamID_t tid = pLeaderUser->GetTeamID();
		Team* pTeam = g_pTeamList->GetTeam( tid );
		Assert( pTeam );
		if( pTeam->IsFull() )
		{//���������Ѿ�����
			WGTeamError sMsg;
			sMsg.SetPlayerID( pSourUser->GetPlayerID() );
			sMsg.SetGUID( pLeaderUser->GetGUID() );
			sMsg.SetErrorCode( TEAM_ERROR_INVITETEAMFULL );
			pSourServerPlayer->SendPacket( &sMsg );
/*
			WGTeamError dMsg;
			dMsg.SetPlayerID( pDestUser->m_PlayerID );
			dMsg.SetErrorCode( TEAM_ERROR_INVITETEAMFULL );
			pDestServerPlayer->SendPacket( &dMsg );
*/
			WGTeamError lMsg;
			lMsg.SetPlayerID( pLeaderUser->GetPlayerID() );
			lMsg.SetGUID( pLeaderUser->GetGUID() );
			lMsg.SetErrorCode( TEAM_ERROR_INVITETEAMFULL );
			pLeaderServerPlayer->SendPacket( &lMsg );
	
			Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderRetInviteHandler...Team Full  sGUID=%X dGUID=%X lGUID=%X", 
				sGuid, dGuid, lGuid );
		}
		else
		{//ͬ�ⱻ�����˼������
			ID_t DestServerID = pDestUser->GetServerID();
			ServerPlayer* pDestServerPlayer = g_pServerManager->GetServerPlayer( DestServerID );
			if( pDestServerPlayer )
			{//�������˷���������Ϣ
				WGTeamAskInvite Msg;
				Msg.SetPlayerID( pDestUser->GetPlayerID() );
				Msg.SetdGUID( dGuid );
				Msg.SetsGUID( lGuid );
			//	Msg.SetMemberCount( pTeam->MemberCount() );

				for( INT i=0; i<pTeam->MemberCount(); ++i )
				{
					WGTeamAskInvite::InviterInfo info;
					TEAMMEMBER* pMember = pTeam->Member( i );
					if( pMember == NULL )
					{
						Assert(FALSE);
						continue ;
					}

					USER* pUser = g_pOnlineUser->FindUser( pMember->m_Member );
					if( pUser == NULL )
					{//�����Ա����,���û������ǿ�
						continue ;
					}

					info.m_PlayerID = pUser->GetPlayerID();
					info.m_NickSize = (UCHAR)strlen(pUser->GetName());
					strncpy( (CHAR*)info.m_szNick, pUser->GetName(), info.m_NickSize );
					info.m_uFamily = pUser->GetMenpai();
					info.m_Scene = pUser->GetSceneID();
					info.m_Level = pUser->GetLevel();
					info.m_uDataID = pUser->GetSex();

					Msg.AddInviterInfo( info );
				}

				pDestServerPlayer->SendPacket( &Msg );
			}
			else
			{
				Assert(FALSE);
			}

			Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderRetInviteHandler...AgreeAndInvite!  sGUID=%X dGUID=%X lGUID=%X", 
				sGuid, dGuid, lGuid );
		}
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
