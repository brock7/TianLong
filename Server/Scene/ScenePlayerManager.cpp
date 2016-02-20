#include "stdafx.h"

#include "ScenePlayerManager.h"
#include "Scene.h"
#include "Log.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "SceneManager.h"
#include "ServerManager.h"
#include "GWNotifyUser.h"
#include "PacketFactoryManager.h"



ScenePlayerManager::ScenePlayerManager( )
{
__ENTER_FUNCTION

	FD_ZERO( &m_ReadFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_WriteFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_ExceptFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_ReadFDs[SELECT_USE] ) ;
	FD_ZERO( &m_WriteFDs[SELECT_USE] ) ;
	FD_ZERO( &m_ExceptFDs[SELECT_USE] ) ;

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 0;

	m_MinFD = m_MaxFD = INVALID_SOCKET ;

	m_nFDSize = 0 ;

	m_SceneID = INVALID_ID ;

__LEAVE_FUNCTION
}

ScenePlayerManager::~ScenePlayerManager( )
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

BOOL ScenePlayerManager::Select( )
{
__ENTER_FUNCTION

//	if( m_nPlayers == 0 )
	{
		MySleep( 50 ) ;
	}

	if( m_MaxFD==INVALID_SOCKET && m_MinFD==INVALID_SOCKET )
		return TRUE ;

	m_Timeout[SELECT_USE].tv_sec  = m_Timeout[SELECT_BAK].tv_sec;
	m_Timeout[SELECT_USE].tv_usec = m_Timeout[SELECT_BAK].tv_usec;

	m_ReadFDs[SELECT_USE]   = m_ReadFDs[SELECT_BAK];
	m_WriteFDs[SELECT_USE]  = m_WriteFDs[SELECT_BAK];
	m_ExceptFDs[SELECT_USE] = m_ExceptFDs[SELECT_BAK];

	_MY_TRY 
	{
		INT ret = SocketAPI::select_ex(	(INT)m_MaxFD+1 , 
										&m_ReadFDs[SELECT_USE] , 
										&m_WriteFDs[SELECT_USE] , 
										&m_ExceptFDs[SELECT_USE] , 
										&m_Timeout[SELECT_USE] ) ;
		if( ret == SOCKET_ERROR )
		{
			Assert(FALSE) ;
		}
	} 
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ScenePlayerManager::ProcessInputs( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	//���ݶ�ȡ
	UINT nPlayerCount = GetPlayerNumber() ;
	for( UINT i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i]==INVALID_ID )
			continue ;

		GamePlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		if (pPlayer->IsDirty())
		{
			continue ;
		}

		//�����ѶϿ�
		if( pPlayer->IsDisconnect() )
			continue;

		SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
		Assert( s!=INVALID_SOCKET ) ;

		if( FD_ISSET( s, &m_ReadFDs[SELECT_USE] ) )
		{
			if( pPlayer->GetSocket()->isSockError() )
			{//���ӳ��ִ���
				g_pLog->FastSaveLog( LOG_FILE_1, "ERROR CI0 GUID=%X NeedRemove", 
					pPlayer->m_HumanGUID ) ;
					
				RemovePlayer( pPlayer, "������ӳ��ִ���" ) ;
			}
			else
			{//��������
				_MY_TRY
				{
					ret = pPlayer->ProcessInput( ) ;
					if( !ret )
					{
						g_pLog->FastSaveLog( LOG_FILE_1, "ERROR CI1 GUID=%X NeedRemove",
							pPlayer->m_HumanGUID ) ;

						RemovePlayer( pPlayer, "ProcessInputִ�з��ؽ������" ) ;
					}
				}
				_MY_CATCH
				{
					SaveCodeLog( ) ;

					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR CI2 GUID=%X NeedRemove",
						pPlayer->m_HumanGUID ) ;

					RemovePlayer( pPlayer, "ProcessInputִ�з����쳣" ) ;
				}
			}
		}
	}


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ScenePlayerManager::ProcessOutputs( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	//���ݷ���
	UINT nPlayerCount = GetPlayerNumber() ;
	for( UINT i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i]==INVALID_ID )
			continue ;

		GamePlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		if( pPlayer->IsDisconnect( ) )
			continue ;

		SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
		Assert( s!=INVALID_SOCKET ) ;

		if( FD_ISSET( s, &m_WriteFDs[SELECT_USE] ) )
		{
			if( pPlayer->GetSocket()->isSockError() )
			{//���ӳ��ִ���
				g_pLog->FastSaveLog( LOG_FILE_1, "ERROR CO0 GUID=%X NeedRemove",
					pPlayer->m_HumanGUID ) ;

				RemovePlayer( pPlayer, "ProcessOutputsʱ������ӳ���" ) ;
			}
			else
			{//��������
				_MY_TRY
				{
					ret = pPlayer->ProcessOutput( ) ;
					if( !ret )
					{
						g_pLog->FastSaveLog( LOG_FILE_1, "ERROR CO1 GUID=%X NeedRemove",
							pPlayer->m_HumanGUID ) ;

						RemovePlayer( pPlayer, "ProcessOutputִ�г���" ) ;
					}
				}
				_MY_CATCH
				{
					SaveCodeLog( ) ;

					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR CO2 GUID=%X NeedRemove",
						pPlayer->m_HumanGUID ) ;

					RemovePlayer( pPlayer, "ProcessOutputִ�г����쳣" ) ;
				}
			}
		}
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ScenePlayerManager::ProcessExceptions( )
{
__ENTER_FUNCTION

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	UINT nPlayerCount = GetPlayerNumber() ;
	for( UINT i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i]==INVALID_ID )
			continue ;

		//ĳ����ҶϿ���������

		GamePlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		if( pPlayer->IsDisconnect() )
			continue ;

		SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
		Assert( s!=INVALID_SOCKET ) ;

		if( FD_ISSET( s, &m_ExceptFDs[SELECT_USE] ) )
		{
			RemovePlayer( pPlayer, "ĳ����ҶϿ���������" ) ;
		}
	}




	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ScenePlayerManager::ProcessCommands( )
{
__ENTER_FUNCTION

	BOOL ret ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	UINT nPlayerCount = GetPlayerNumber() ;
	for( UINT i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i]==INVALID_ID )
			continue ;

		GamePlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		if( pPlayer->IsDisconnect() )
			continue ;

		SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
		Assert( s!=INVALID_SOCKET ) ;

		if( pPlayer->GetSocket()->isSockError() )
		{//���ӳ��ִ���
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR CC1 GUID=%X NeedRemove",
				pPlayer->m_HumanGUID ) ;

			RemovePlayer( pPlayer, "ProcessCommandsʱ���ӳ���" ) ;
		}
		else
		{//��������
			_MY_TRY
			{
				ret = pPlayer->ProcessCommand( FALSE ) ;
				if( !ret )
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR CC2 GUID=%X NeedRemove",
						pPlayer->m_HumanGUID ) ;

					RemovePlayer( pPlayer, "ProcessCommandִ�н������", TRUE ) ;
				}
			}
			_MY_CATCH
			{
				SaveCodeLog( ) ;

				g_pLog->FastSaveLog( LOG_FILE_1, "ERROR CC3 GUID=%X NeedRemove",
					pPlayer->m_HumanGUID ) ;

				RemovePlayer( pPlayer,  "ProcessCommandִ�з����쳣", TRUE ) ;
			}
		}
	}




	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ScenePlayerManager::RemovePlayer( Player* pPlayer, char* szReason, BOOL bReal )
{
__ENTER_FUNCTION

	Assert( pPlayer ) ;

	BOOL ret = FALSE ;

	//��һ�������Socket��Ϣ
	SOCKET fd = pPlayer->GetSocket()->getSOCKET() ;
	if( fd != INVALID_SOCKET )
	{
		DelPlayerSocket( fd ) ;
		pPlayer->Disconnect( ) ;

		g_pLog->FastSaveLog( LOG_FILE_1, "ScenePlayerManager::RemovePlayer Disconnect(SOCKET=%d)...OK", 
			fd ) ;
	}

	if( !bReal )
	{//��������
		GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;

		if( !pGamePlayer->IsDirty( ) )
		{
			Scene* pScene = g_pSceneManager->GetScene( GetSceneID() ) ;
			pScene->OnScenePlayerDisconnect( pGamePlayer ) ;
			
			GWNotifyUser* pMsg = (GWNotifyUser*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_NOTIFYUSER)) ;
			pMsg->SetGUID( pGamePlayer->m_HumanGUID ) ;
			pMsg->SetPlayerID( pGamePlayer->PlayerID() ) ;
			pMsg->SetStatus( GWNotifyUser::NUS_DISCONNECT ) ;
			g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;
		}

		g_pLog->FastSaveLog( LOG_FILE_1, "ScenePlayerManager::RemovePlayer !Real (SOCKET=%d,GUID=%X)...OK", 
			fd, pGamePlayer->m_HumanGUID ) ;

		return TRUE ;
	}

	//�ڶ�������������е��������
	Scene* pScene;
	_MY_TRY
	{
		pScene = g_pSceneManager->GetScene( GetSceneID() ) ;
		if( pScene )
		{
			GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
			Obj_Human *pHuman = pGamePlayer->GetHuman() ;

			if( pHuman && pHuman->getScene() )
			{
				Assert( GetSceneID()==pHuman->getScene()->SceneID() ) ;
				if( pHuman->IsActiveObj() )
				{
					pScene->OnScenePlayerLeave( pGamePlayer ) ;

					pGamePlayer->GetHuman()->SetActiveFlag( FALSE );
					pScene->ObjectLeaveScene( pHuman ) ;
				}
				else
				{
					pHuman->OnLeaveScene();
					pHuman->setScene( NULL );
					pScene->GetHumanManager()->Remove( pHuman );
					pScene->GetObjManager()->RemoveObj( pHuman->GetID() );
				}
			}
			Assert( pGamePlayer->GetHuman()->HumanNode()->m_pNext == NULL ) ;
			Assert( pGamePlayer->GetHuman()->HumanNode()->m_pPrev == NULL ) ;
		}
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}

	//�����������PlayerManager�е���Ϣ
	if( pPlayer->PlayerManagerID()!=INVALID_ID )
	{
		ret = DelPlayer( pPlayer->PlayerID() ) ;
		if( !ret )
		{
			SaveCodeLog( ) ;
		}
	}

	//���Ĳ������PlayerPool�е���Ϣ��ע��˲������������
	//�����ô˲����󣬵�ǰPlayer���п��ܻᱻ���Ϸ�����½������
	GUID_t guid = ((GamePlayer*)pPlayer)->GetHuman()->GetGUID() ;
	if( guid!=INVALID_ID )
	{
		((GamePlayer*)pPlayer)->ChooseFreeOwn(pScene->GetRecyclePlayerManager(), 5) ;
	}


	g_pLog->FastSaveLog( LOG_FILE_1, "ScenePlayerManager::Real RemovePlayer(GUID=%X)...OK", 
		guid ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ScenePlayerManager::AddPlayer( Player* pPlayer )
{
__ENTER_FUNCTION

	if( m_nFDSize>=FD_SETSIZE )
	{//�Ѿ������ܹ������������������
		return FALSE ;
	}

	BOOL ret = PlayerManager::AddPlayer( pPlayer ) ;
	if( !ret )
	{
		Assert( FALSE ) ;
		return FALSE ;
	}


	SOCKET fd = pPlayer->GetSocket()->getSOCKET() ;
	AddPlayerSocket( fd ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ScenePlayerManager::AddPlayerSocket( SOCKET fd )
{
__ENTER_FUNCTION

	Assert(fd!=INVALID_SOCKET) ;
	if( FD_ISSET(fd,&m_ReadFDs[SELECT_BAK]) )
	{
		return FALSE ;
	}

	m_MinFD = ((m_MinFD==INVALID_SOCKET)?fd:min(fd , m_MinFD));
	m_MaxFD = ((m_MaxFD==INVALID_SOCKET)?fd:max(fd,m_MaxFD));

	FD_SET(fd , &m_ReadFDs[SELECT_BAK]);
	FD_SET(fd , &m_WriteFDs[SELECT_BAK]);
	FD_SET(fd , &m_ExceptFDs[SELECT_BAK]);

	m_nFDSize++ ;


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ScenePlayerManager::DelPlayer( PlayerID_t pid )
{
__ENTER_FUNCTION

	Assert( pid!=INVALID_ID) ;

//���PlayerManager�е���Ϣ
	PlayerManager::RemovePlayer( pid ) ;




	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ScenePlayerManager::DelPlayerSocket( SOCKET fd )
{
__ENTER_FUNCTION

	if( m_MinFD==INVALID_SOCKET )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR ScenePlayerManager::DelPlayerSocket m_MinFD (SOCKET=%d)", 
			fd ) ;
		return FALSE ;
	}
	if( m_MaxFD==INVALID_SOCKET )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR ScenePlayerManager::DelPlayerSocket m_MaxFD (SOCKET=%d)", 
			fd ) ;
		return FALSE ;
	}
	if( fd == INVALID_SOCKET )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR ScenePlayerManager::DelPlayerSocket fd (SOCKET=%d)", 
			fd ) ;
		return FALSE ;
	}
	if( !FD_ISSET(fd,&m_ReadFDs[SELECT_BAK]) )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR ScenePlayerManager::DelPlayerSocket(SOCKET=%d)", 
			fd ) ;
		return FALSE ;
	}


	if( fd==m_MinFD ) 
	{
		SOCKET s = m_MaxFD ;
		UINT nPlayerCount = GetPlayerNumber() ;
		for( UINT i=0; i<nPlayerCount; i++ )
		{
			if( m_pPlayers[i]==INVALID_ID )
				continue ;

			GamePlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
			Assert( pPlayer ) ;
			if( pPlayer==NULL )
				continue ;


			SOCKET temp = pPlayer->GetSocket()->getSOCKET() ;
			if( temp == fd )
				continue ;
			if( temp==INVALID_SOCKET )
				continue ;

			if( temp < s )
			{
				s = temp ;
			}
		}

		if( m_MinFD == m_MaxFD )
		{
			m_MinFD = m_MaxFD = INVALID_SOCKET ;
		}
		else
		{
			m_MinFD = s ;
		}
	} 
	else if( fd==m_MaxFD ) 
	{
		SOCKET s = m_MinFD ;
		UINT nPlayerCount = GetPlayerNumber() ;
		for( UINT i=0; i<nPlayerCount; i++ )
		{
			if( m_pPlayers[i]==INVALID_ID )
				continue ;

			GamePlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
			Assert( pPlayer ) ;
			if( pPlayer==NULL )
				continue ;


			SOCKET temp = pPlayer->GetSocket()->getSOCKET() ;
			if( temp == fd )
				continue ;
			if( temp==INVALID_SOCKET )
				continue ;

			if( temp > s )
			{
				s = temp ;
			}
		}

		if( m_MaxFD == m_MinFD )
		{
			m_MinFD = m_MaxFD = INVALID_SOCKET ;
		}
		else
		{
			m_MaxFD = s ;
		}
	}

	FD_CLR(fd , &m_ReadFDs[SELECT_BAK]);
	FD_CLR(fd , &m_ReadFDs[SELECT_USE]);
	FD_CLR(fd , &m_WriteFDs[SELECT_BAK]);
	FD_CLR(fd , &m_WriteFDs[SELECT_USE]);
	FD_CLR(fd , &m_ExceptFDs[SELECT_BAK]);
	FD_CLR(fd , &m_ExceptFDs[SELECT_USE]);

	m_nFDSize-- ;
	Assert( m_nFDSize>=0 ) ;



	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ScenePlayerManager::HeartBeat( UINT uTime )
{
__ENTER_FUNCTION

	BOOL ret ;

	UINT nPlayerCount = GetPlayerNumber() ;
	for( UINT i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i] == INVALID_ID )
			continue ;

		GamePlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		if( pPlayer==NULL )
		{
			Assert(FALSE) ;
			return FALSE ;
		}

		_MY_TRY
		{
			ret = pPlayer->HeartBeat(uTime ) ;
		}
		_MY_CATCH
		{
			SaveCodeLog( ) ;
			ret = FALSE ;
		}
		if( !ret )
		{//����߼���������ʧ�ܣ�����Ҫ�Ͽ���ǰ����
			if( pPlayer->GetPlayerStatus()==PS_SERVER_ANOTHER_GUID_ENTER 
				&& !pPlayer->IsDirty() )
			{
				GWNotifyUser* pMsg = (GWNotifyUser*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_NOTIFYUSER)) ;
				pMsg->SetGUID( pPlayer->m_HumanGUID ) ;
				pMsg->SetPlayerID( -1 ) ;
				pMsg->SetStatus( GWNotifyUser::NUS_NEED_WORLD_KICK ) ;
				g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;

				g_pLog->FastSaveLog( LOG_FILE_1, "WGNotifyUserHandler: GUID=%X pPlayer->GetPlayerStatus()==PS_SERVER_ANOTHER_GUID_ENTER",
					pPlayer->m_HumanGUID ) ;
			}

			ret = RemovePlayer( pPlayer, "HeartBeat�߼���������ʧ��", TRUE ) ;
			Assert( ret ) ;
		}
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID ScenePlayerManager::RemoveAllPlayer( )
{
__ENTER_FUNCTION

	UINT nPlayerCount = GetPlayerNumber() ;
	for( UINT i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[0] == INVALID_ID )
			break ;

		Player* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[0]) ;
		if( pPlayer==NULL )
		{
			Assert(FALSE) ;
			break ;
		}

		RemovePlayer( pPlayer, "������Ҷ��Ͽ�", TRUE ) ;
	}

__LEAVE_FUNCTION
}
