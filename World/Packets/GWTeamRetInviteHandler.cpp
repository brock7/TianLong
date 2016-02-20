#include "stdafx.h"


#include "GWTeamRetInvite.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "WGTeamError.h"
#include "Team.h"
#include "ServerManager.h"
#include "WGTeamResult.h"
#include "WGTeamLeaderAskInvite.h"




UINT GWTeamRetInviteHandler::Execute( GWTeamRetInvite* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GUID_t sGuid = pPacket->GetSourGUID();//�����˵�GUID
	GUID_t dGuid = pPacket->GetDestGUID();//�������˵�GUID

	USER* pDestUser = g_pOnlineUser->FindUser( dGuid );
	if( pDestUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...User Dest GUID=%X not find!", 
			dGuid );

		return PACKET_EXE_CONTINUE;
	}

	ServerPlayer* pDestServerPlayer = (ServerPlayer*)pPlayer;

	USER* pSourUser = g_pOnlineUser->FindUser( sGuid );
	if( pSourUser==NULL )
	{
		if( pPacket->GetReturn()==TRUE )
		{
			WGTeamError dMsg;
			dMsg.SetPlayerID( pDestUser->GetPlayerID() );
			dMsg.SetGUID( pDestUser->GetGUID() );
			dMsg.SetErrorCode( TEAM_ERROR_TARGETNOTONLINE );
			pDestServerPlayer->SendPacket( &dMsg );
		}

		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...User Sour GUID=%X not find!", 
			sGuid );
		return PACKET_EXE_CONTINUE;
	}

	if( pSourUser->IsEnemy( pDestUser->GetUserCampData() ) )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...Sour Name=%X, Dest Name=%s are enemy!", 
			pSourUser->GetName(), pDestUser->GetName() );

		return PACKET_EXE_CONTINUE;
	}

	ID_t SourServerID = pSourUser->GetServerID();
	ServerPlayer* pSourServerPlayer = g_pServerManager->GetServerPlayer( SourServerID );
	if( pSourServerPlayer==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	USER* pLeader = NULL;
	ServerPlayer* pLeaderServerPlayer;

	if ( pSourUser->GetTeamID() != INVALID_ID )
	{
		Team* pTeam = g_pTeamList->GetTeam( pSourUser->GetTeamID() );

		if ( pTeam == NULL )
		{
			Assert( pTeam );
			pSourUser->SetTeamID( INVALID_ID );
		}
		else if ( pTeam->Leader()->m_Member != sGuid )
		{ // �����˲��Ƕӳ�
			pLeader = g_pOnlineUser->FindUser( pTeam->Leader()->m_Member );

			pLeaderServerPlayer = g_pServerManager->GetServerPlayer( pLeader->GetServerID() );
			if ( pLeaderServerPlayer == NULL )
			{
				Assert( FALSE );
				pLeader = NULL;
			}
		}
	}


	if( pPacket->GetReturn()==FALSE )
	{//�������˲�ͬ��������
		WGTeamError Msg;
		Msg.SetPlayerID( pSourUser->GetPlayerID() );
		Msg.SetGUID( pDestUser->GetGUID() );
		Msg.SetErrorCode( TEAM_ERROR_INVITEREFUSE );

		pSourServerPlayer->SendPacket( &Msg );

		if ( pLeader != NULL )
		{
			Msg.SetPlayerID( pLeader->GetPlayerID() );
			pLeaderServerPlayer->SendPacket( &Msg );
		}

		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...Invite Refuse SourGUID=%X DestGUID=%X", 
			sGuid, dGuid );
	}
	else if( pDestUser->GetTeamID() != INVALID_ID )
	{//���������Ѿ��ж���
		WGTeamError Msg;
		Msg.SetPlayerID( pSourUser->GetPlayerID() );
		Msg.SetGUID( pDestUser->GetGUID() );
		Msg.SetErrorCode(TEAM_ERROR_INVITEDESTHASTEAM);

		pSourServerPlayer->SendPacket( &Msg );

		if ( pLeader != NULL )
		{
			Msg.SetPlayerID( pLeader->GetPlayerID() );
			pLeaderServerPlayer->SendPacket( &Msg );
		}

		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...DestInTeam SourGUID=%X DestGUID=%X", 
			sGuid, dGuid );
	}
	else if( pSourUser->GetTeamID()==INVALID_ID )
	{//�����˺ͱ������˶��޶���
		TeamID_t tid = g_pTeamList->CreateTeam( );
		Assert( tid!=INVALID_ID );
		Team* pTeam = g_pTeamList->GetTeam( tid );
		Assert( pTeam );

		if ( pSourUser->GetGUID() == pDestUser->GetGUID() )
		{

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
			Msg.SetIcon( pSourUser->GetPortrait() );
			Msg.SetSceneID( pSourUser->GetSceneID() );
			Msg.SetDataID( pSourUser->GetSex() );


			pSourServerPlayer->SendPacket( &Msg );

			Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...CreateTeam GUID=%X TeamID=%d", 
				sGuid, tid );
		}
		else
		{
			//����Ҽ��������
			TEAMMEMBER sMember;
			sMember.m_Member = sGuid;
			pTeam->AddMember( &sMember );
			TEAMMEMBER dMember;
			dMember.m_Member = dGuid;
			pTeam->AddMember( &dMember );

			//������Ҷ�������
			pSourUser->SetTeamID( tid );
			pDestUser->SetTeamID( tid );

			//���ؽ��
			WGTeamResult sMsg1;
			sMsg1.SetPlayerID( pSourUser->GetPlayerID() );
			sMsg1.SetReturn( TEAM_RESULT_TEAMREFRESH );
			sMsg1.SetGUID( sGuid );
			sMsg1.SetTeamID( tid );
			sMsg1.SetGUIDEx( pSourUser->GetPlayerID() ); // ����ҵ� PlayerID ����
			sMsg1.SetSceneID( pSourUser->GetSceneID() );
			pSourServerPlayer->SendPacket( &sMsg1 );

			WGTeamResult sMsg2;
			sMsg2.SetPlayerID( pSourUser->GetPlayerID() );
			sMsg2.SetReturn( TEAM_RESULT_MEMBERENTERTEAM );
			sMsg2.SetGUID( dGuid );
			sMsg2.SetTeamID( tid );
			sMsg2.SetGUIDEx( pDestUser->GetPlayerID() ); // ����ҵ� PlayerID ����
			sMsg2.SetSceneID( pDestUser->GetSceneID() );
			sMsg2.SetName( pDestUser->GetName() );
			sMsg2.SetIcon( pDestUser->GetPortrait() );
			sMsg2.SetDataID( pDestUser->GetSex() );
			pSourServerPlayer->SendPacket( &sMsg2 );

			WGTeamResult dMsg1;
			dMsg1.SetPlayerID( pDestUser->GetPlayerID() );
			dMsg1.SetReturn( TEAM_RESULT_TEAMREFRESH );
			dMsg1.SetGUID( sGuid );
			dMsg1.SetTeamID( tid );
			dMsg1.SetGUIDEx( pSourUser->GetPlayerID() ); // ����ҵ� PlayerID ����
			dMsg1.SetSceneID( pSourUser->GetSceneID() );
			dMsg1.SetName( pSourUser->GetName() );
			dMsg1.SetIcon( pSourUser->GetPortrait() );
			dMsg1.SetDataID( pSourUser->GetSex() );
			pDestServerPlayer->SendPacket( &dMsg1 );

			WGTeamResult dMsg2;
			dMsg2.SetPlayerID( pDestUser->GetPlayerID() );
			dMsg2.SetReturn( TEAM_RESULT_MEMBERENTERTEAM );
			dMsg2.SetGUID( dGuid );
			dMsg2.SetTeamID( tid );
			dMsg2.SetGUIDEx( pDestUser->GetPlayerID() ); // ����ҵ� PlayerID ����
			dMsg2.SetSceneID( pDestUser->GetSceneID() );
			pDestServerPlayer->SendPacket( &dMsg2 );

			Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...CreateTeam sGUID=%X dGUID=%X TeamID=%d", 
				sGuid, dGuid, tid );
		}

	}
	else
	{//�������ж���, ���������޶���
		TeamID_t tid = pSourUser->GetTeamID();
		Team* pTeam = g_pTeamList->GetTeam( tid );
		Assert( pTeam );
		if( pTeam->IsFull() )
		{//���������Ѿ�����
			WGTeamError sMsg;
			sMsg.SetPlayerID( pSourUser->GetPlayerID() );
			sMsg.SetGUID( pDestUser->GetGUID() );
			sMsg.SetErrorCode( TEAM_ERROR_INVITETEAMFULL );
			pSourServerPlayer->SendPacket( &sMsg );

			WGTeamError dMsg;
			dMsg.SetPlayerID( pDestUser->GetPlayerID() );
			dMsg.SetGUID( pDestUser->GetGUID() );
			dMsg.SetErrorCode( TEAM_ERROR_APPLYTEAMFULL );
			pDestServerPlayer->SendPacket( &dMsg );
	
			Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...Team Full sGUID=%X dGUID=%X TeamID=%d", 
				sGuid, dGuid, tid );
		}
		else
		{ //����Ҽ������
			TEAMMEMBER Member;
			Member.m_Member = dGuid;
			pTeam->AddMember( &Member );

			//������Ҷ�����Ϣ
			pDestUser->SetTeamID( pTeam->GetTeamID() );
			WGTeamResult Msg1; // �����¶�Ա��
			Msg1.SetPlayerID( pDestUser->GetPlayerID() );
			Msg1.SetTeamID( tid );
			WGTeamResult Msg2; // ����ÿ����Ա��
			Msg2.SetReturn( TEAM_RESULT_MEMBERENTERTEAM );
			Msg2.SetTeamID( tid );
			Msg2.SetGUID( dGuid );
			Msg2.SetGUIDEx( pDestUser->GetPlayerID() ); // ����ҵ� PlayerID ����
			Msg2.SetSceneID( pDestUser->GetSceneID() );
			Msg2.SetName( pDestUser->GetName() );
			Msg2.SetIcon( pDestUser->GetPortrait() );
			Msg2.SetDataID( pDestUser->GetSex() );

			//֪ͨ�������ڵ����
			for( int i=0; i<pTeam->MemberCount(); i++ )
			{
				TEAMMEMBER* pMember = pTeam->Member( i );
				if( pMember==NULL )
				{
					Assert(FALSE);
					continue;
				}

				USER* pUser = g_pOnlineUser->FindUser( pMember->m_Member );
				if( pUser==NULL )
				{//�����Ա����,���û������ǿ�
					continue;
				}
				
				ID_t ServerID = pUser->GetServerID();
				ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID );
				if( pServerPlayer==NULL )
				{
					Assert(FALSE);
					continue;
				}

				Msg2.SetPlayerID( pUser->GetPlayerID() );
				pServerPlayer->SendPacket( &Msg2 );

				if( pMember->m_Member != dGuid )
				{ // ���Լ��������Ҵ��������
					Msg1.SetReturn( TEAM_RESULT_TEAMREFRESH );
					Msg1.SetName( pUser->GetName() );
					Msg1.SetIcon( pUser->GetPortrait() );
					Msg1.SetDataID( pUser->GetSex() );

					Msg1.SetGUID( pMember->m_Member );
					Msg1.SetGUIDEx( pUser->GetPlayerID() ); // ����ҵ� PlayerID ����
					Msg1.SetSceneID( pUser->GetSceneID() );

					pDestServerPlayer->SendPacket( &Msg1 );
				}

			}

			Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...Success! sGUID=%X dGUID=%X TeamID=%d", 
				sGuid, dGuid, tid );
		}
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
