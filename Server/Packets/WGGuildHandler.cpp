#include "stdafx.h"
/********************************************************************************
 *	�ļ�����	WGGuildHandler.cpp
 *	ȫ·����	d:\Prj\Server\Server\Packets\WGGuildHandler.cpp
 *	����ʱ�䣺	2005 �� 12 �� 14 ��	12:09
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/

#include "WGGuild.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCGuild.h"

UINT WGGuildHandler::Execute( WGGuild* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetPlayerID();
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	if( pGamePlayer==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGGuildHandler::Execute pGamePlayer==NULL" );
		return PACKET_EXE_CONTINUE;
	}

	if (pGamePlayer->m_HumanGUID != pPacket->GetGUID())
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGGuildHandler::Execute pGamePlayer->m_HumanGUID[%d] != pPacket->GetGUID()[%d]",pGamePlayer->m_HumanGUID, pPacket->GetGUID());
		return PACKET_EXE_CONTINUE;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );
	Scene* pScene = pHuman->getScene();
	if( !pScene )
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGGuildHandler::Execute pHuman->getScene() == NULL" );
		return PACKET_EXE_CONTINUE;
	}

	if( pPlayer->IsServerPlayer() )
	{//�������յ��������������������
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );

		pScene->SendPacket( pPacket, PlayerID );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGGuildHandler: ServerPlayer (Type=%d) ",
			pPacket->GetGuildPacket()->m_uPacketType );

		return PACKET_EXE_NOTREMOVE;
	}
	else if( pPlayer->IsGamePlayer() )
	{//�����յ�Cache�����Ϣ
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

		GCGuild Msg;
		Msg.SetGuildPacket( pPacket->GetGuildPacket() );
		Msg.GetGuildPacket()->m_uPacketType += GUILD_PACKET_WG_GC_SEPARATOR - GUILD_PACKET_GW_WG_SEPARATOR;
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGGuildHandler: GamePlayer (Type=%d) ",
			pPacket->GetGuildPacket()->m_uPacketType );
	}
	else
	{
		Assert(FALSE);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
