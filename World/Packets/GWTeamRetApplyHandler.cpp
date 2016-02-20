#include "stdafx.h"


#include "GWTeamRetApply.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "WGTeamError.h"
#include "Team.h"
#include "ServerManager.h"
#include "WGTeamResult.h"



UINT GWTeamRetApplyHandler::Execute( GWTeamRetApply* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GUID_t sGuid = pPacket->GetSourGUID();//�����˵�GUID
	GUID_t lGuid = pPacket->GetLeaderGUID();//�ӳ���GUID

	USER* pLeaderUser = g_pOnlineUser->FindUser( lGuid );
	if( pLeaderUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetApplyHandler...User Leader GUID=%X not find!", 
			lGuid );

		return PACKET_EXE_CONTINUE;
	}

	ServerPlayer* pLeaderServerPlayer = (ServerPlayer*)pPlayer;

	USER* pSourUser = g_pOnlineUser->FindUser( sGuid );
	if( pSourUser==NULL )
	{
		if( pPacket->GetReturn()==TRUE )
		{
			WGTeamError dMsg;
			dMsg.SetPlayerID( pLeaderUser->GetPlayerID() );
			dMsg.SetGUID( pLeaderUser->GetGUID() );
			dMsg.SetErrorCode( TEAM_ERROR_TARGETNOTONLINE );
			pLeaderServerPlayer->SendPacket( &dMsg );
		}

		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetApplyHandler...User Sour GUID=%X not find!", 
			sGuid );

		return PACKET_EXE_CONTINUE;
	}

	ID_t SourServerID = pSourUser->GetServerID();
	ServerPlayer* pSourServerPlayer = g_pServerManager->GetServerPlayer( SourServerID );
	if( pSourServerPlayer==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	if( pLeaderUser->IsEnemy( pSourUser->GetUserCampData() ) )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetApplyHandler...Lead Name=%s, Invite Name=%s are enemy!", 
			pLeaderUser->GetName(), pSourUser->GetName() );

		return PACKET_EXE_CONTINUE;
	}

	if( pPacket->GetReturn()==FALSE )
	{//�ӳ���ͬ��
		WGTeamError sMsg;
		sMsg.SetPlayerID( pSourUser->GetPlayerID() );
		sMsg.SetGUID( pLeaderUser->GetGUID() );
		sMsg.SetErrorCode( TEAM_ERROR_APPLYLEADERREFUSE );
		pSourServerPlayer->SendPacket( &sMsg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetApplyHandler...Leader lGUID=%X Refuse sGUID=%X join team!", 
			lGuid, sGuid );
	}
	else if( pSourUser->GetTeamID() != INVALID_ID )
	{//�������Ѿ�����ĳ������
		//WGTeamError sMsg;
		//sMsg.SetPlayerID( pSourUser->m_PlayerID );
		//sMsg.SetErrorCode( TEAM_ERROR_APPLYSOURHASTEAM );
		//pSourServerPlayer->SendPacket( &sMsg );

		WGTeamError lMsg;
		lMsg.SetPlayerID( pLeaderUser->GetPlayerID() );
		lMsg.SetGUID( pLeaderUser->GetGUID() );
		lMsg.SetErrorCode( TEAM_ERROR_APPLYSOURHASTEAM );
		pLeaderServerPlayer->SendPacket( &lMsg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetApplyHandler...Leader lGUID=%X, sGUID=%X has team!", 
			lGuid, sGuid );
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
			sMsg.SetErrorCode( TEAM_ERROR_APPLYTEAMFULL );
			pSourServerPlayer->SendPacket( &sMsg );

			WGTeamError lMsg;
			lMsg.SetPlayerID( pLeaderUser->GetPlayerID() );
			lMsg.SetGUID( pLeaderUser->GetGUID() );
			lMsg.SetErrorCode( TEAM_ERROR_TEAMFULL );
			pLeaderServerPlayer->SendPacket( &lMsg );
	
			Log::SaveLog( WORLD_LOGFILE, "GWTeamRetApplyHandler...Team Full  sGUID=%X lGUID=%X", 
				sGuid, lGuid );
		}
		else if( pTeam->Leader()->m_Member != lGuid )
		{//�ӳ�GUID�Ͷ����м�¼�Ķӳ�GUID����
			WGTeamError sMsg;
			sMsg.SetPlayerID( pSourUser->GetPlayerID() );
			sMsg.SetGUID( pLeaderUser->GetGUID() );
			sMsg.SetErrorCode( TEAM_ERROR_APPLYLEADERGUIDERROR );
			pSourServerPlayer->SendPacket( &sMsg );

			WGTeamError lMsg;
			lMsg.SetPlayerID( pLeaderUser->GetPlayerID() );
			lMsg.SetGUID( pLeaderUser->GetGUID() );
			lMsg.SetErrorCode( TEAM_ERROR_APPLYLEADERGUIDERROR );
			pLeaderServerPlayer->SendPacket( &lMsg );
	
			Log::SaveLog( WORLD_LOGFILE, "GWTeamRetApplyHandler...Team Full  sGUID=%X lGUID=%X", 
				sGuid, lGuid );
		}
		else 
		{//����
			//����Ҽ������
			TEAMMEMBER Member;
			Member.m_Member = sGuid;
			pTeam->AddMember( &Member );

			//������Ҷ�����Ϣ
			pSourUser->SetTeamID( pTeam->GetTeamID() );
			WGTeamResult Msg1; // �����¶�Ա��
			Msg1.SetPlayerID( pSourUser->GetPlayerID() );
			Msg1.SetTeamID( tid );
			WGTeamResult Msg2; // ����ÿ����Ա��
			Msg2.SetReturn( TEAM_RESULT_MEMBERENTERTEAM );
			Msg2.SetTeamID( tid );
			Msg2.SetGUID( sGuid );
			Msg2.SetGUIDEx( pSourUser->GetPlayerID() ); // ����ҵ� PlayerID ����
			Msg2.SetSceneID( pSourUser->GetSceneID() );
			Msg2.SetName( pSourUser->GetName() );
			Msg2.SetIcon( pSourUser->GetPortrait() );
			Msg2.SetDataID( pSourUser->GetSex() );

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
				{
					Assert(FALSE);
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

				if( pMember->m_Member != sGuid )
				{ // ���Լ��������Ҵ��������
					Msg1.SetReturn( TEAM_RESULT_TEAMREFRESH );
					Msg1.SetName( pUser->GetName() );
					Msg1.SetIcon( pUser->GetPortrait() );
					Msg1.SetDataID( pUser->GetSex() );

					Msg1.SetGUID( pMember->m_Member );
					Msg1.SetGUIDEx( pUser->GetPlayerID() ); // ����ҵ� PlayerID ����
					Msg1.SetSceneID( pUser->GetSceneID() );

					pSourServerPlayer->SendPacket( &Msg1 );
				}
			}//end for

			Log::SaveLog( WORLD_LOGFILE, "GWTeamRetApplyHandler...Success!  sGUID=%X lGUID=%X", 
				sGuid, lGuid );
		}

	}




	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
