#include "stdafx.h"


#include "GWAskUserData.h"
#include "Log.h"
#include "OnlineUser.h"
#include "WGRetUserData.h"
#include "ServerPlayer.h"
#include "GuildManager.h"
#include "WGNotifyUser.h"
#include "ServerManager.h"

UINT GWAskUserDataHandler::Execute( GWAskUserData* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer ;

	USER* pUser = g_pOnlineUser->FindUser( pPacket->GetGUID() ) ;

	PlayerID_t OldPlayerID = INVALID_ID;
	USER* pOldUser = g_pOnlineUser->FindUserByAccountName( pPacket->GetAccount() ) ;
	if( pOldUser && pOldUser != pUser )
	{
		OldPlayerID = pOldUser->GetPlayerID();
	}

	if( pUser==NULL )
	{//û�з����ڡ������û��б����Ӧ��GUID���û�
		
		Log::SaveLog( WORLD_LOGFILE, "GWAskUserDataHandler...User GUID=%X not find!", 
			pPacket->GetGUID() ) ;

		if( g_Config.m_WorldInfo.m_EnableShareMem )
		{
			WGRetUserData Msg ;
			Msg.SetPlayerID( pPacket->GetPlayerID() ) ;
			Msg.SetReturn( UDR_NOTFINDUSERDATA ) ;

			pServerPlayer->SendPacket( &Msg ) ;

			return PACKET_EXE_CONTINUE ;
		}

//
//��������
		pUser = g_pOnlineUser->LoadUser( pPacket->GetGUID() ) ;

		if( pUser )
		{
			_HUMAN_DB_LOAD* pDBHuman = &(pUser->GetFullUserData()->m_Human);

			//��ʱ��������
			strncpy( pDBHuman->m_Name, pPacket->GetAccount(), MAX_ACCOUNT ) ;
			pDBHuman->m_Sex = (BOOL)(pPacket->GetGender()) ;
			pUser->SetKey( pPacket->GetKey() );
			//��ʱ��������

			g_pOnlineUser->OnUserLogin( pUser ) ;


			WGRetUserData Msg ;
			Msg.SetPlayerID( pPacket->GetPlayerID() ) ;
			Msg.SetReturn( UDR_USERDATA ) ;
			Msg.SetUserData( pUser->GetFullUserData() ) ;
			Msg.SetPlayerAge( pUser->GetUserAge() );


			//�������ݿ�, �����û�״̬
			g_pOnlineUser->DelData( pUser->GetFullUserData() ) ;
			pUser->SetFullUserData( NULL );
			pUser->SetUserStatus( US_NORMAL );
			pUser->ResetKickTime();

			//���û����ڷ�������PlayerID��¼���û�������
			pUser->SetPlayerID( pPacket->GetPlayerID() );

			pServerPlayer->SendPacket( &Msg ) ;

			Log::SaveLog( WORLD_LOGFILE, "GWAskUserDataHandler...Load GUID=%X OK!", 
				pPacket->GetGUID() ) ;
		}
		else
		{
			Log::SaveLog( WORLD_LOGFILE, "ERROR:GWAskUserDataHandler...Load GUID=%X false!", 
				pPacket->GetGUID() ) ;
		}
//��������
//

		return PACKET_EXE_CONTINUE ;
	}

	int iUserStatus = pUser->UserStatus() ;

	if( pUser->GetKey() != pPacket->GetKey() )
	{//��֤����󣬾�����Ҳ���ܵ�����
		WGRetUserData Msg ;
		Msg.SetPlayerID( pPacket->GetPlayerID() ) ;
		Msg.SetReturn( UDR_KEYERROR ) ;

		pServerPlayer->SendPacket( &Msg ) ;

		Log::SaveLog( WORLD_LOGFILE, "GWAskUserDataHandler.Key Error..User GUID=%X Status=%d", 
			pPacket->GetGUID(), iUserStatus ) ;

		return PACKET_EXE_CONTINUE ;
	}

	//���û����ڷ�������PlayerID��¼���û�������
	pUser->SetPlayerID( pPacket->GetPlayerID() );

	switch( iUserStatus )
	{
	case US_NONE:
		{
			WGRetUserData Msg ;
			Msg.SetPlayerID( pPacket->GetPlayerID() ) ;
			Msg.SetReturn( UDR_NOTFINDUSERDATA ) ;

			pServerPlayer->SendPacket( &Msg ) ;
		}
		break ;
	case US_HOLDUSERDATA:
		{
			WGRetUserData Msg ;
			Msg.SetPlayerID( pPacket->GetPlayerID() ) ;
			Msg.SetReturn( UDR_USERDATA ) ;
			Msg.SetUserData( pUser->GetFullUserData() ) ;
			Msg.SetPlayerAge( pUser->GetUserAge() );

			g_pOnlineUser->DelData( pUser->GetFullUserData() ) ;//�������ݿ�
			pUser->SetFullUserData( NULL );
			pUser->SetUserStatus( US_NORMAL );
			pUser->ResetKickTime();

			pServerPlayer->SendPacket( &Msg ) ;

			if( OldPlayerID != INVALID_ID )
			{
				WGNotifyUser MsgNotify ;
				MsgNotify.SetGUID( pOldUser->GetGUID() ) ;
				MsgNotify.SetStatus( WGNotifyUser::NUS_REMOVE ) ;
				pServerPlayer->SendPacket( &MsgNotify ) ;
			}
		}
		break ;
	case US_CHANGESCENE:
		{
			BOOL KickPlayerFromOldServer = FALSE;
			SceneID_t nOldSceneID = pUser->GetOldSceneID();
			ID_t nOldServerID;
			if( nOldSceneID != INVALID_ID && nOldSceneID < (SceneID_t)g_Config.m_SceneInfo.m_SceneCount )
			{
				nOldServerID = g_Config.m_SceneInfo.m_pScene[nOldSceneID].m_ServerID;
				ServerPlayer* pOldServerPlayer = g_pServerManager->GetServerPlayer( nOldServerID );
				if( pOldServerPlayer )
				{
					WGNotifyUser MsgNotify ;
					MsgNotify.SetGUID( pUser->GetGUID() ) ;
					MsgNotify.SetStatus( WGNotifyUser::NUS_REMOVE ) ;
					pOldServerPlayer->SendPacket( &MsgNotify ) ;
					KickPlayerFromOldServer = TRUE;
				}
			}
			if( KickPlayerFromOldServer )
				Log::SaveLog( WORLD_LOGFILE, "GWAskUserDataHandler...Kick OldPlayer GUID=%X OldScene=%d OldServer=%d Success", pUser->GetGUID(), nOldSceneID, nOldServerID ) ;
			else
				Log::SaveLog( WORLD_LOGFILE, "GWAskUserDataHandler...Kick OldPlayer GUID=%X OldScene=%d OldServer=%d Faild!!", pUser->GetGUID(), nOldSceneID, nOldServerID ) ;

			WGRetUserData Msg ;
			Msg.SetPlayerID( pPacket->GetPlayerID() ) ;
			Msg.SetReturn( UDR_USERDATA ) ;
			Msg.SetUserData( pUser->GetFullUserData() ) ;
			Msg.SetPlayerAge( pUser->GetUserAge() );

			g_pOnlineUser->DelData( pUser->GetFullUserData() ) ;//�������ݿ�
			pUser->SetFullUserData( NULL );
			pUser->SetUserStatus( US_NORMAL );
			pUser->ResetKickTime();

			pServerPlayer->SendPacket( &Msg ) ;
		}
		break;
	case US_NORMAL:
	case US_OFFLINE:
		{
			WGRetUserData Msg ;
			Msg.SetPlayerID( pPacket->GetPlayerID() ) ;
			Msg.SetReturn( UDR_USERDATALIVING ) ;


			pUser->SetUserStatus( US_NORMAL );
			pUser->ResetKickTime();

			pServerPlayer->SendPacket( &Msg ) ;
		}
		break ;
	case US_CRASH_DOWN://ȡ���������������,��������ʽ��֤��������������
		{
			Assert(FALSE);
			/*
			WGRetUserData Msg ;
			Msg.SetPlayerID( pPacket->GetPlayerID() ) ;
			Msg.SetReturn( UDR_USERSERVERCRASH ) ;
			pServerPlayer->SendPacket( &Msg ) ;
			*/
		}
		break;
	case US_WAIT_SHUTDOWN:
		{
			WGRetUserData Msg ;
			Msg.SetPlayerID( pPacket->GetPlayerID() ) ;
			Msg.SetReturn( UDR_USERSERVERCRASH ) ;
			pServerPlayer->SendPacket( &Msg ) ;
		}
		break ;
	default :
		{
			Assert(FALSE) ;
		}
		break ;
	};


	Log::SaveLog( WORLD_LOGFILE, "GWAskUserDataHandler...User GUID=%X Status=%d", 
		pPacket->GetGUID(), iUserStatus ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
