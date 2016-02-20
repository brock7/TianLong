#include "stdafx.h"
/********************************************************************************
 *	�ļ�����	WGGuildReturnHandler.cpp
 *	ȫ·����	d:\Prj\Server\Server\Packets\WGGuildReturnHandler.cpp
 *	����ʱ�䣺	2005 �� 12 �� 12 ��	17:12
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/

#include "WGGuildReturn.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCGuildReturn.h"

UINT WGGuildReturnHandler::Execute( WGGuildReturn* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetPlayerID();
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	if( pGamePlayer==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGGuildReturnHandler::Execute pGamePlayer==NULL" );
		return PACKET_EXE_CONTINUE;
	}

	if (pGamePlayer->m_HumanGUID != pPacket->GetGUID())
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGGuildReturnHandler::Execute pGamePlayer->m_HumanGUID[%d] != pPacket->GetGUID()[%d]",pGamePlayer->m_HumanGUID, pPacket->GetGUID());
		return PACKET_EXE_CONTINUE;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );
	Scene* pScene = pHuman->getScene();
	if( !pScene )
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGGuildReturnHandler::Execute pHuman->getScene() == NULL" );
		return PACKET_EXE_CONTINUE;
	}

	if( pPlayer->IsServerPlayer() )
	{//�������յ��������������������
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );

		pScene->SendPacket( pPacket, PlayerID );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGGuildReturnHandler: ServerPlayer (ReturnType=%d) ",
			pPacket->GetGuildReturn()->m_ReturnType );

		return PACKET_EXE_NOTREMOVE;
	}
	else if( pPlayer->IsGamePlayer() )
	{//�����յ�Cache�����Ϣ
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

		GUILD_RETURN_TYPE ReturnType = (GUILD_RETURN_TYPE)pPacket->GetGuildReturn()->m_ReturnType;

		switch( ReturnType )
		{
		case GUILD_RETURN_RESPONSE:
			{
				if( pPacket->GetGuildReturn()->m_GUID != pHuman->GetGUID() )
				{
					break;
				}
			}
			/* go through */
		case GUILD_RETURN_CREATE:
			{
				pHuman->SetMoney(pHuman->GetMoney()-500000);
				pHuman->SetGuildID( pPacket->GetGuildReturn()->m_GuildID );
				CHAR zsMsgBangPaiTitle[MAX_NICK_NAME] = {0};
				sprintf(zsMsgBangPaiTitle, "%s����",pPacket->GetGuildReturn()->m_GuildName);
				pHuman->SetBangPaiName(zsMsgBangPaiTitle, (BYTE)strlen(zsMsgBangPaiTitle));
				pHuman->UpdateTitlesToClient();
			}
			break;
		case GUILD_RETURN_JOIN:
			{ // ��������� GuildID
				pHuman->SetGuildID( INVALID_ID );
			}
			break;
		case GUILD_RETURN_EXPEL:
		case GUILD_RETURN_LEAVE:
			{
				if( pPacket->GetGuildReturn()->m_GUID == pHuman->GetGUID() )
				{
					pHuman->SetGuildID( INVALID_ID );
					pHuman->SetBangPaiName("", 0);
					pHuman->UpdateTitlesToClient();
				}
			}
			break;
		case GUILD_RETURN_RECRUIT:
			{//����ƺ�
				if( pPacket->GetGuildReturn()->m_GUID == pHuman->GetGUID() )
				{
					CHAR zsMsgBangPaiTitle[MAX_NICK_NAME] = {0};
					sprintf(zsMsgBangPaiTitle, "%s����",pPacket->GetGuildReturn()->m_GuildName);
					pHuman->SetBangPaiName(zsMsgBangPaiTitle, (BYTE)strlen(zsMsgBangPaiTitle));
					pHuman->UpdateTitlesToClient();
					pHuman->SetGuildID( pPacket->GetGuildReturn()->m_GuildID );
				}
			}
			break;
		case GUILD_RETURN_PROMOTE:
		case GUILD_RETURN_DEMOTE:
			{
				if( pPacket->GetGuildReturn()->m_GUID == pHuman->GetGUID() )
				{
					CHAR zsMsgBangPaiTitle[MAX_NICK_NAME] = {0};
					sprintf(zsMsgBangPaiTitle, "%s%s", pPacket->GetGuildReturn()->m_GuildName, pPacket->GetGuildReturn()->m_PositionName);
					pHuman->SetBangPaiName(zsMsgBangPaiTitle, (BYTE)strlen(zsMsgBangPaiTitle));
					pHuman->UpdateTitlesToClient();
				}
			}
			break;
		case GUILD_RETURN_DEMISE:
			{
				if( pPacket->GetGuildReturn()->m_GUID == pHuman->GetGUID() )
				{
					CHAR zsMsgBangPaiTitle[MAX_NICK_NAME] = {0};
					sprintf(zsMsgBangPaiTitle, "%s%s", pPacket->GetGuildReturn()->m_GuildName, pPacket->GetGuildReturn()->m_PositionName);
					pHuman->SetBangPaiName(zsMsgBangPaiTitle, (BYTE)strlen(zsMsgBangPaiTitle));
					pHuman->UpdateTitlesToClient();
				}
				else if(pPacket->GetGuildReturn()->m_GUIDChanged == pHuman->GetGUID() )
				{
					CHAR zsMsgBangPaiTitle[MAX_NICK_NAME] = {0};
					sprintf(zsMsgBangPaiTitle, "%s%s", pPacket->GetGuildReturn()->m_GuildName, pPacket->GetGuildReturn()->m_ChangedPositionName);
					pHuman->SetBangPaiName(zsMsgBangPaiTitle, (BYTE)strlen(zsMsgBangPaiTitle));
					pHuman->UpdateTitlesToClient();
				}
			}
			break;
		case GUILD_RETURN_AUTHORIZE:
		case GUILD_RETURN_DEPRIVE_AUTHORITY:
		case GUILD_RETURN_WITHDRAW:
		case GUILD_RETURN_DEPOSIT:
		case GUILD_RETURN_FOUND:
		case GUILD_RETURN_REJECT:
			break;
		case GUILD_RETURN_DISMISS:
			{
				pHuman->SetGuildID( INVALID_ID );
				pHuman->SetBangPaiName("", 0);
				pHuman->UpdateTitlesToClient();
			}
			break;
		default:
			Assert(FALSE);
			return 0;
		}

		GCGuildReturn Msg;

		Msg.SetGuildReturn( pPacket->GetGuildReturn() );

		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGGuildReturnHandler: GamePlayer (ReturnType=%d) ",
			pPacket->GetGuildReturn()->m_ReturnType );
	}
	else
	{
		Assert(FALSE);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
