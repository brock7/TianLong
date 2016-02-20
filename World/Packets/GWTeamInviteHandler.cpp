#include "stdafx.h"


#include "GWTeamInvite.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"

#include "Team.h"
#include "WGTeamError.h"
#include "WGTeamResult.h"
#include "WGTeamAskInvite.h"
#include "WGTeamLeaderAskInvite.h"



UINT GWTeamInviteHandler::Execute( GWTeamInvite* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pSourServerPlayer = (ServerPlayer*)pPlayer;

	GUID_t sGuid = pPacket->GetSourGUID();

	USER* pSourUser = g_pOnlineUser->FindUser( sGuid );
	if( pSourUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...User Sour GUID=%X not find!", 
			sGuid );

		return PACKET_EXE_CONTINUE;
	}

	USER* pDestUser = g_pOnlineUser->FindUser( pPacket->GetDestName() );
	if( pDestUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...User Dest Name=%s not find!", 
			pPacket->GetDestName() );

		return PACKET_EXE_CONTINUE;
	}

	GUID_t dGuid = pDestUser->GetGUID();

	if( pSourUser->IsEnemy( pDestUser->GetUserCampData() ) )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...Sour Name=%s, Dest Name=%s are enemy!", 
			pSourUser->GetName(), pDestUser->GetName() );

		return PACKET_EXE_CONTINUE;
	}

	// �����˾ܾ�������
	if( pDestUser->IsRefuseTeamInvite() )
	{
		WGTeamError Msg;
		Msg.SetPlayerID( pSourUser->GetPlayerID() );
		Msg.SetGUID(sGuid);
		Msg.SetErrorCode(TEAM_ERROR_REFUSEINVITESETTING);
		pSourServerPlayer->SendPacket( &Msg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...DestGUID=%X refuse to be invited. ", 
			dGuid );
		return PACKET_EXE_CONTINUE;
	}

	if( pDestUser->GetTeamID() != INVALID_ID )
	{//���������Ѿ�����ĳ��������
		WGTeamError Msg;
		Msg.SetPlayerID( pSourUser->GetPlayerID() );
		Msg.SetGUID(sGuid);
		Msg.SetErrorCode(TEAM_ERROR_INVITEDESTHASTEAM);
		pSourServerPlayer->SendPacket( &Msg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...DestInTeam SourGUID=%X DestGUID=%X", 
			sGuid, dGuid );
	}
	else if( sGuid==dGuid && pSourUser->GetTeamID()==INVALID_ID )
	{//�����˺ͱ�����������ͬһ����, �Ҳ�����ĳ������
		//�Խ�����
		TeamID_t tid = g_pTeamList->CreateTeam( );
		Assert( tid!=INVALID_ID );
		Team* pTeam = g_pTeamList->GetTeam( tid );
		Assert( pTeam );

		//����Ҽ��������
		TEAMMEMBER Member;
		Member.m_Member = sGuid;
		pTeam->AddMember( &Member );

		//������Ҷ�������
		pSourUser->SetTeamID( tid );

		//���ؽ��
		WGTeamResult Msg;
		Msg.SetPlayerID( pSourUser->GetPlayerID() );
		Msg.SetReturn( TEAM_RESULT_MEMBERENTERTEAM );
		Msg.SetGUID( sGuid );
		Msg.SetTeamID( tid );
		Msg.SetGUIDEx( pSourUser->GetPlayerID() ); // ����ҵ� PlayerID ����
		Msg.SetSceneID( pSourUser->GetSceneID() );
		Msg.SetDataID( pSourUser->GetSex() );
		Msg.SetIcon( pSourUser->GetPortrait() );

		pSourServerPlayer->SendPacket( &Msg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...CreateTeam GUID=%X TeamID=%d", 
			sGuid, tid );
	}
	else
	{
		if ( pSourUser->GetTeamID() != INVALID_ID )
		{
			//�������Ѿ���һ������
			Team* pTeam = g_pTeamList->GetTeam( pSourUser->GetTeamID() );
			Assert( pTeam );

			GUID_t tlGUID = pTeam->Leader()->m_Member; // �ӳ�GUID

			//�����˲��Ƕӳ���Ҫ���ӳ���������
			if ( tlGUID != sGuid )
			{ // ���Ƕӳ������
				USER* pLeaderUser = g_pOnlineUser->FindUser( tlGUID );
				if( pLeaderUser == NULL )
				{
					Assert(FALSE);
				}

				ID_t LeaderServerID = pLeaderUser->GetServerID();
				ServerPlayer* pLeaderServerPlayer = g_pServerManager->GetServerPlayer( LeaderServerID );
				if( pLeaderServerPlayer==NULL )
				{
					Assert(FALSE);
				}

				WGTeamLeaderAskInvite Msg;
				Msg.SetPlayerID( pLeaderUser->GetPlayerID() );//�ӳ���Player_t
				Msg.SetSourGUID( sGuid );		//������
				Msg.SetDestGUID( dGuid );		//��Ҫ����
				Msg.SetSourName( pSourUser->GetName() );
				Msg.SetDestName( pDestUser->GetName() );
				Msg.SetGUID(tlGUID);

				pLeaderServerPlayer->SendPacket( &Msg );

				Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...TeamLeaderAskInvite SourGUID=%X DestGUID=%X", 
					sGuid, dGuid );
				return PACKET_EXE_CONTINUE;
			}
		}

		ID_t DestServerID = pDestUser->GetServerID();
		ServerPlayer* pDestServerPlayer = g_pServerManager->GetServerPlayer( DestServerID );
		if( pDestServerPlayer )
		{//�������˷���������Ϣ
			WGTeamAskInvite Msg;
			Msg.SetPlayerID( pDestUser->GetPlayerID() );
			Msg.SetdGUID( dGuid );
			Msg.SetsGUID( sGuid );

			if ( pSourUser->GetTeamID() != INVALID_ID )
			{
				Team* pTeam = g_pTeamList->GetTeam( pSourUser->GetTeamID() );
				Assert( pTeam );

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
			}
			else
			{
			//	Msg.SetMemberCount( 1 );
				WGTeamAskInvite::InviterInfo info;
				info.m_PlayerID = pSourUser->GetPlayerID();
				info.m_NickSize = (UCHAR)strlen(pSourUser->GetName());
				strncpy( (CHAR*)info.m_szNick, pSourUser->GetName(), info.m_NickSize );
				info.m_uFamily = pSourUser->GetMenpai();
				info.m_Scene = pSourUser->GetSceneID();
				info.m_Level = pSourUser->GetLevel();
				info.m_uDataID = pSourUser->GetSex();

				Msg.AddInviterInfo( info );
			}

			pDestServerPlayer->SendPacket( &Msg );
		}
		else
		{
			Assert(FALSE);
		}

		Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...AskInvite SourGUID=%X DestGUID=%X", 
			sGuid, dGuid );

	}


	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
