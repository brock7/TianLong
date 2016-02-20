#include "stdafx.h"
#include "LWAskCharLogin.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "WLRetCharLogin.h"
#include "Log.h"
#include "ServerManager.h"

UINT LWAskCharLoginHandler::Execute(LWAskCharLogin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	
	Assert(pPacket);
	
	ASK_LOGINSTATUS	AskStatus  = pPacket->GetAskStatus();
	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer ;
	Assert(pServerPlayer);

	USER* pUser;

	GUID_t	CharGuid = pPacket->GetPlayerGUID();

	if(AskStatus == ALS_ASKSTATUS)
	{
		
		WLRetCharLogin Msg;
		pUser = g_pOnlineUser->FindUser( CharGuid ) ;
		if(pUser) //�û����ڣ�ֱ�ӷ��سɹ���Ϣ
		{
			UINT uUserStatus = pUser->UserStatus();

			switch (uUserStatus)
			{
			case US_CRASH_DOWN:
			case US_WAIT_SHUTDOWN://������Crash
				{
					Msg.SetResult(ASKCHARLOGIN_SERVER_STOP);
					Msg.SetAccount(pPacket->GetAccount());
					Msg.SetPlayerID(pPacket->GetPlayerID());
					Msg.SetPlayerGUID(CharGuid);
					Msg.SetHoldStatus(TRUE);
					Msg.SetPlayerServerID(pUser->GetServerID());
					pServerPlayer->SendPacket(&Msg);

					Log::SaveLog( WORLD_LOGFILE, "LWAskCharLoginHandler...User Hold,CharGUID = %X",CharGuid) ;
					return PACKET_EXE_CONTINUE;
				}
				break;
			case US_NONE:
			case US_NORMAL:
			case US_OFFLINE:
			case US_CHANGESCENE:
			case US_HOLDUSERDATA:
				{
					pUser->SetKey( pPacket->GetUserKey() );
					Msg.SetResult(ASKCHARLOGIN_SUCCESS);
					Msg.SetAccount(pPacket->GetAccount());
					Msg.SetPlayerID(pPacket->GetPlayerID());
					Msg.SetPlayerGUID(CharGuid);
					Msg.SetHoldStatus(TRUE);
					Msg.SetPlayerServerID(pUser->GetServerID());
					pServerPlayer->SendPacket(&Msg);
					Log::SaveLog( WORLD_LOGFILE, "LWAskCharLoginHandler...User Hold,CharGUID = %X",CharGuid) ;
					return PACKET_EXE_CONTINUE;
				}
				break;
			default:
				{
					Log::SaveLog( WORLD_LOGFILE, "LWAskCharLoginHandler...UNKNOW ERROR,CharGUID = %X, Status=%d",CharGuid,uUserStatus) ;
					return PACKET_EXE_ERROR;	
				}
				break;
			}
		}
		else //�û�������
		{
			Msg.SetResult(ASKCHARLOGIN_LOADDB_ERROR);
			Msg.SetAccount(pPacket->GetAccount());
			Msg.SetPlayerID(pPacket->GetPlayerID());
			Msg.SetPlayerGUID(CharGuid);
			Msg.SetHoldStatus(FALSE);
			pServerPlayer->SendPacket(&Msg);
			Log::SaveLog( WORLD_LOGFILE, "LWAskCharLoginHandler...No User,CharGUID = %X",CharGuid) ;
			return PACKET_EXE_CONTINUE;	
			

		}
	}
	else if(AskStatus == ALS_SENDDATA)
	{
		pUser = g_pOnlineUser->FindUser( CharGuid ) ;
		if( pUser )
		{//������û�����
			Log::SaveLog( WORLD_LOGFILE, "LWAskCharLoginHandler...Char Exist! CharGUID = %X",CharGuid) ;
			return PACKET_EXE_CONTINUE;
		}

		ID_t	ServerID = g_Config.SceneID2ServerID(pPacket->GetUserData()->m_Human.m_StartScene);

		ServerPlayer* pFindPlayer = g_pServerManager->GetServerPlayer( ServerID ) ;
		if( pFindPlayer==NULL ) //������ά��
		{
			WLRetCharLogin Msg;
			Msg.SetResult(ASKCHARLOGIN_SERVER_STOP);
			Msg.SetAccount(pPacket->GetAccount());
			Msg.SetPlayerID(pPacket->GetPlayerID());
			Msg.SetPlayerGUID(CharGuid);
			Msg.SetHoldStatus(FALSE);
			pServerPlayer->SendPacket(&Msg);
			Log::SaveLog( WORLD_LOGFILE, "LWAskCharLoginHandler...Server Stop CharGUID = %X",CharGuid) ;
			return PACKET_EXE_CONTINUE;
		}	


		pUser	 = g_pOnlineUser->NewUser();

		if(!pUser) //����
		{
			WLRetCharLogin Msg;
			Msg.SetResult(ASKCHARLIST_WORLD_FULL);
			Msg.SetAccount(pPacket->GetAccount());
			Msg.SetPlayerID(pPacket->GetPlayerID());
			Msg.SetPlayerGUID(CharGuid);
			Msg.SetHoldStatus(FALSE);
			pServerPlayer->SendPacket(&Msg);

			Log::SaveLog( WORLD_LOGFILE, "LWAskCharLoginHandler...World Full CharGUID = %X",CharGuid) ;
			return PACKET_EXE_CONTINUE;
		}

		pUser->CleanUp();

		FULLUSERDATA* pFullUserData = g_pOnlineUser->NewData();
		Assert(pFullUserData);
		memcpy(pFullUserData,pPacket->GetUserData(),sizeof(FULLUSERDATA));
		pUser->SetFullUserData( pFullUserData );
		pUser->SetKey( pPacket->GetUserKey() );
		pUser->SetUserStatus( US_HOLDUSERDATA );
		pUser->ResetKickTime() ;
		pUser->SetGUID( CharGuid );
		pUser->SetAccount( pPacket->GetAccount() );
		pUser->SetUserAge( pPacket->GetUserAge() );
		g_pOnlineUser->OnUserLogin( pUser ) ;

		Assert(pFullUserData->m_Human.m_StartScene!=INVALID_ID);
		Assert(pFullUserData->m_Human.m_StartScene<(INT)g_Config.m_SceneInfo.m_SceneCount);
		pUser->SetSceneID( pFullUserData->m_Human.m_StartScene );


		WLRetCharLogin Msg;
		Msg.SetResult(ASKCHARLOGIN_SUCCESS);
		Msg.SetAccount(pPacket->GetAccount());
		Msg.SetPlayerID(pPacket->GetPlayerID());
		Msg.SetPlayerGUID(CharGuid);
		Msg.SetHoldStatus(TRUE);
		Msg.SetPlayerServerID(pUser->GetServerID());
		pServerPlayer->SendPacket(&Msg);

		Log::SaveLog( WORLD_LOGFILE, "LWAskCharLoginHandler...ALSCENDDATA GUID=%X", CharGuid) ;
		return PACKET_EXE_CONTINUE;
	}

	
	Log::SaveLog( WORLD_LOGFILE, "LWAskCharLoginHandler::Execute()...GUID=%X OK",CharGuid) ;

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}