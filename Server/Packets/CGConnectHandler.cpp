#include "stdafx.h"


#include "CGConnect.h"
#include "GamePlayer.h"
#include "Log.h"
#include "GWAskUserData.h"
#include "IncomingPlayerManager.h"
#include "ServerManager.h"
#include "PacketFactoryManager.h"
#include "GameTable.h"
#include "SocketAPI.h"
#include "GUIDManager.h"
#include "Obj_Human.h"
#include "GCConnect.h"
#ifdef __LINUX__
#include <arpa/inet.h>			// for inet_xxx()
#endif

BOOL IsActiveIP( CHAR* szHostIP )
{
	UINT curip = inet_addr( szHostIP ) ;
	INT i ;
	for( i=0; i<g_IPRange.m_DisableCount; i++ )
	{
		UINT dip = inet_addr(g_IPRange.m_aaIPDisable[i]) ;
		UINT dmask = inet_addr(g_IPRange.m_aaMaskDisable[i]) ;
		if( (dip&dmask) == (curip&dmask) )
			return FALSE ;
	}
	for( i=0; i<g_IPRange.m_EnableCount; i++ )
	{
		UINT eip = inet_addr(g_IPRange.m_aaIPEnable[i]) ;
		UINT emask = inet_addr(g_IPRange.m_aaMaskEnable[i]) ;
		if( (eip&emask) == (curip&emask) )
			return TRUE ;
	}
	return TRUE ;
}

UINT CGConnectHandler::Execute( CGConnect* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==g_pIncomingPlayerManager->m_ThreadID ) ;

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;

	//���GamePlayer״̬�Ƿ�����
	if( pGamePlayer->GetPlayerStatus() != PS_SERVER_CONNECT )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGConnect::GetPlayerStatus! GUID=%X, Status=%d",
			pPacket->GetGUID(), pGamePlayer->GetPlayerStatus() ) ;
		return PACKET_EXE_ERROR ;
	}

	//���IP��ַ�Ƿ�Ϸ�
	if( !IsActiveIP(pGamePlayer->GetSocket()->m_Host ) )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGConnect::Not active ip address! " ) ;
		return PACKET_EXE_ERROR ;
	}

	//��֤�û�����ĺϷ��ԣ����Լ�ʱ��ɣ�
	if( !pGamePlayer->CheckKey(pPacket->GetKey()) )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGConnect::GamePlayer->CheckKey " ) ;
		return PACKET_EXE_ERROR ;
	}

	//��鲻�ϸ�
	if(	pPacket->GetCheckVer()!=CURRENT_VERSION )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGConnect::Versions FALSE " ) ;
		return PACKET_EXE_ERROR ;
	}

	//�жϵ�ǰ��Ҫ��½��GUID������Ƿ����ڱ��浵��
	Obj_Human* pOldHuman = (Obj_Human*)(g_pGUIDManager->Get(pPacket->GetGUID())) ;
	if( pOldHuman )
	{
		GamePlayer* pOldPlayer = (GamePlayer*)(pOldHuman->GetPlayer()) ;
		if( pOldPlayer && pOldPlayer->GetPlayerStatus()==PS_SERVER_ENTER_RECYCLE )
		{
			GCConnect *pMsg = (GCConnect*)(g_pPacketFactoryManager->CreatePacket(PACKET_GC_CONNECT)) ;
			pMsg->SetServerID(-1);
			pMsg->SetSceneID(-1);
			pMsg->SetEstate(1);//1��ʾ���������ڴ��̵�ǰ��ң�����ҵȴ�
			pPlayer->SendPacket(pMsg);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGConnect::PS_SERVER_ENTER_RECYCLE GUID=%X", pPacket->GetGUID() ) ;
			return PACKET_EXE_CONTINUE ;
		}
	}

	//ȡ���û���ɫ���ݣ�����ʼ���û���ɫ��Ϣ
	//  ��������ڱ�ķ������ϣ����������ݿ��ϣ���
	//  ����Ҫ�ȴ���ķ������˽����ݷ��͹���

	ID_t ServerID = pPacket->GetServerID() ;

	if( !StrSafeCheck( pPacket->GetAccount(), MAX_ACCOUNT ) )
	{
		Assert(FALSE);
	}

	GWAskUserData* AskMsg = (GWAskUserData*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_ASKUSERDATA)) ;
	AskMsg->SetPlayerID( pGamePlayer->PlayerID() ) ;
	AskMsg->SetGUID( pPacket->GetGUID() ) ;
	AskMsg->SetKey( pPacket->GetKey() ) ;
	AskMsg->SetAccount( pPacket->GetAccount() ) ;
	AskMsg->SetGender( pPacket->GetGender() ) ;//�Ա�
	g_pServerManager->SendPacket( AskMsg, ServerID ) ;

	pGamePlayer->m_HumanGUID = pPacket->GetGUID() ;
	if (pPacket->GetGUID() == INVALID_ID)
	{
		Assert(FALSE);
	}

	//��Player״̬����Ϊ���ȴ�������ȡ�����ϡ�
	pGamePlayer->SetPlayerStatus( PS_SERVER_WAITING_FOR_SERVER_DATA ) ;


	g_pLog->FastSaveLog( LOG_FILE_1, "CGConnect::Execute(GUID=%X SOCKET=%d IP=%s) ...OK ", 
		pPacket->GetGUID(), pGamePlayer->GetSocket()->getSOCKET(), pGamePlayer->GetSocket()->m_Host ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}




