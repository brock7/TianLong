#include "stdafx.h"
/********************************************************************************
 *	�ļ�����	WGGuildErrorHandler.cpp
 *	ȫ·����	d:\Prj\Server\Server\Packets\WGGuildErrorHandler.cpp
 *	����ʱ�䣺	2005 �� 12 �� 12 ��	17:24
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/

#include "WGGuildError.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCGuildError.h"

UINT WGGuildErrorHandler::Execute( WGGuildError* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetPlayerID();
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	if( pGamePlayer==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGGuildErrorHandler::Execute pGamePlayer==NULL" );
		return PACKET_EXE_CONTINUE;
	}

	if (pGamePlayer->m_HumanGUID != pPacket->GetGUID())
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGGuildErrorHandler::Execute pGamePlayer->m_HumanGUID[%d] != pPacket->GetGUID()[%d]",pGamePlayer->m_HumanGUID, pPacket->GetGUID());
		return PACKET_EXE_CONTINUE;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );
	Scene* pScene = pHuman->getScene();
	if( !pScene )
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGGuildErrorHandler::Execute pHuman->getScene() == NULL" );
		return PACKET_EXE_CONTINUE;
	}

	if( pPlayer->IsServerPlayer() )
	{//�������յ��������������������
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );

		pScene->SendPacket( pPacket, PlayerID );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGGuildErrorHandler: ServerPlayer (ErrorCode=%d) ",
			pPacket->GetErrorCode() );

		return PACKET_EXE_NOTREMOVE;
	}
	else if( pPlayer->IsGamePlayer() )
	{//�����յ�Cache�����Ϣ
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

		GCGuildError Msg;
		Msg.SetErrorCode( pPacket->GetErrorCode() );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGGuildErrorHandler: GamePlayer (ErrorCode=%d) ",
			pPacket->GetErrorCode() );
	}
	else
	{
		Assert(FALSE);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
