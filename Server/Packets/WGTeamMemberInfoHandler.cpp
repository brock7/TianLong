#include "stdafx.h"
// WGTeamMemberInfoHandler.cpp

#include "WGTeamMemberInfo.h"
#include "GCTeamMemberInfo.h"

#include "Log.h"
#include "ServerManager.h"
#include "PlayerPool.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

UINT WGTeamMemberInfoHandler::Execute( WGTeamMemberInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetPlayerID();
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	if( pGamePlayer==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGTeamMemberInfoHandler::Execute pGamePlayer==NULL" );
		return PACKET_EXE_CONTINUE;
	}

	if (pGamePlayer->m_HumanGUID != pPacket->GetGUID())
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGTeamMemberInfoHandler::Execute pGamePlayer->m_HumanGUID[%d] != pPacket->GetGUID()[%d]",pGamePlayer->m_HumanGUID, pPacket->GetGUID());
		return PACKET_EXE_CONTINUE;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );
	Scene* pScene = pHuman->getScene();
	if( !pScene )
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGTeamMemberInfoHandler::Execute pHuman->getScene() == NULL" );
		return PACKET_EXE_CONTINUE;
	}

	if( pPlayer->IsServerPlayer() )
	{//�������յ��������������������
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );

		pScene->SendPacket( pPacket, PlayerID );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGTeamMemberInfoHandler: ServerPlayer (GUID=%X) ",
			pPacket->GetGUID() );

		return PACKET_EXE_NOTREMOVE;
	}
	else if( pPlayer->IsGamePlayer() )
	{//�����յ�Cache�����Ϣ
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

		GCTeamMemberInfo Msg;

		Msg.setGUID( pPacket->GetGUID() );
		Msg.SetFamily( pPacket->GetFamily() );
		Msg.SetLevel( pPacket->GetLevel() );
		Msg.SetDead( FALSE ); // ����ͬһ��������Ĭ���ǻ��
		Msg.SetDeadLink( pPacket->GetDeadLinkFlag() );

		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGTeamMemberInfoHandler: GamePlayer (GUID=%X) ",
			pPacket->GetGUID() );
	}
	else
	{
		Assert(FALSE);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
