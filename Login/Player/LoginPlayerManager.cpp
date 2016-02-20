#include "stdafx.h"


#include "LoginPlayerManager.h"
#include "Config.h"
#include "Log.h"
#include "PlayerPool.h"
#include "TimeManager.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"



#define ACCEPT_ONESTEP 50

#define MAX_LOGIN_PLAYER_AUTH_TIME	30000

LoginPlayerManager* g_pLoginPlayerManager = NULL ;

LoginPlayerManager::LoginPlayerManager( )
{
__ENTER_FUNCTION

	FD_ZERO( &m_ReadFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_WriteFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_ExceptFDs[SELECT_BAK] ) ;

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 0;

	m_MinFD = m_MaxFD = INVALID_SOCKET ;

	m_nFDSize = 0 ;

	m_PacketQue = new ASYNC_PACKET[MAX_CACHE_SIZE] ;
	Assert(m_PacketQue) ;
	m_QueSize = MAX_CACHE_SIZE ;
	m_Head = 0 ;
	m_Tail = 0 ;

__LEAVE_FUNCTION
}

LoginPlayerManager::~LoginPlayerManager( )
{
__ENTER_FUNCTION

	SAFE_DELETE( m_pServerSocket ) ;
	SAFE_DELETE_ARRAY( m_PacketQue ) ;

__LEAVE_FUNCTION
}

BOOL LoginPlayerManager::Init( )
{
__ENTER_FUNCTION

	
	INT LoginPort = (g_pServerManager->GetCurrentServerInfo())->m_Port0;
	
	m_pServerSocket = new ServerSocket( LoginPort) ;
	Assert( m_pServerSocket ) ;

	m_pServerSocket->setNonBlocking() ;

	m_SocketID = m_pServerSocket->getSOCKET() ;
	Assert( m_SocketID != INVALID_SOCKET ) ;

	FD_SET(m_SocketID , &m_ReadFDs[SELECT_BAK]);
	FD_SET(m_SocketID , &m_ExceptFDs[SELECT_BAK]);

	m_MinFD = m_MaxFD = m_SocketID;

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 0;

	Log::SaveLog(LOGIN_LOGFILE,"LoginPlayerManager Start ServerSocket At Port: %d",LoginPort);


__LEAVE_FUNCTION

	return TRUE ;
}

BOOL LoginPlayerManager::Select( )
{
__ENTER_FUNCTION

	m_Timeout[SELECT_USE].tv_sec  = m_Timeout[SELECT_BAK].tv_sec;
	m_Timeout[SELECT_USE].tv_usec = m_Timeout[SELECT_BAK].tv_usec;

	m_ReadFDs[SELECT_USE]   = m_ReadFDs[SELECT_BAK];
	m_WriteFDs[SELECT_USE]  = m_WriteFDs[SELECT_BAK];
	m_ExceptFDs[SELECT_USE] = m_ExceptFDs[SELECT_BAK];

	MySleep(100) ;

	_MY_TRY 
	{
		INT iRet = SocketAPI::select_ex(	(INT)m_MaxFD+1 , 
											&m_ReadFDs[SELECT_USE] , 
											&m_WriteFDs[SELECT_USE] , 
											&m_ExceptFDs[SELECT_USE] , 
											&m_Timeout[SELECT_USE] ) ;
		Assert( iRet!=SOCKET_ERROR ) ;
	} 
	_MY_CATCH
	{
		Log::SaveLog( LOGIN_LOGFILE, "ERROR: IncomingPlayerManager::Select( )..." ) ;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LoginPlayerManager::RemovePlayer( Player* pPlayer )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;
	
	//��һ�������PlayerManager�е���Ϣ
	ret = DelPlayer( pPlayer->PlayerID() ) ;
	Assert( ret ) ;

	//�ڶ��������PlayerPool�е���Ϣ��ע��˲������������
	//�����ô˲����󣬵�ǰPlayer���п��ܻᱻ���Ϸ�����½������
	((LoginPlayer*)pPlayer)->FreeOwn( ) ;


	Log::SaveLog( LOGIN_LOGFILE, "LoginPlayerManager::RemovePlayer(PID:%d)...OK", pPlayer->PlayerID() ) ;

	return ret ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LoginPlayerManager::ProcessInputs( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	//�����ӽ��룺
	if( FD_ISSET(m_SocketID,&m_ReadFDs[SELECT_USE]) )
	{
		for( INT i=0; i<ACCEPT_ONESTEP; i++ )
		{
			if( !AcceptNewConnection() )
				break;
		}
	}

	//���ݶ�ȡ
	UINT nPlayerCount = GetPlayerNumber() ;
	for( UINT i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i]==INVALID_ID )
			continue ;

		LoginPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
		if( s == m_SocketID )
			continue ;

		if( FD_ISSET( s, &m_ReadFDs[SELECT_USE] ) )
		{
			if( pPlayer->GetSocket()->isSockError() )
			{//���ӳ��ִ���
				RemovePlayer( pPlayer ) ;
			}
			else
			{//��������
				_MY_TRY
				{
					ret = pPlayer->ProcessInput( ) ;
					if( !ret )
					{
						RemovePlayer( pPlayer ) ;
					}
				}
				_MY_CATCH
				{
					RemovePlayer( pPlayer ) ;
				}
			}
		}
	}


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LoginPlayerManager::ProcessOutputs( )
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

		LoginPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
		if( s == m_SocketID )
			continue ;

		if( FD_ISSET( s, &m_WriteFDs[SELECT_USE] ) )
		{
			if( pPlayer->GetSocket()->isSockError() )
			{//���ӳ��ִ���
				RemovePlayer( pPlayer ) ;
			}
			else
			{//��������
				_MY_TRY
				{
					ret = pPlayer->ProcessOutput( ) ;
					if( !ret )
					{
						RemovePlayer( pPlayer ) ;
					}
				}
				_MY_CATCH
				{
					RemovePlayer( pPlayer ) ;
				}
			}
		}
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LoginPlayerManager::ProcessExceptions( )
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

		LoginPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
		if( s == m_SocketID )
		{//��������������⣬�ѡ�����
			Assert( FALSE ) ;
			continue ;
		}

		if( FD_ISSET( s, &m_ExceptFDs[SELECT_USE] ) )
		{
			RemovePlayer( pPlayer ) ;
		}
	}





	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LoginPlayerManager::ProcessCommands( )
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

		LoginPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
		if( s == m_SocketID )
			continue ;

		if( pPlayer->GetSocket()->isSockError() )
		{//���ӳ��ִ���
			RemovePlayer( pPlayer ) ;
		}
		else
		{//��������
			_MY_TRY
			{
				ret = pPlayer->ProcessCommand( FALSE ) ;
				if( !ret )
				{
					RemovePlayer( pPlayer ) ;
				}
			}
			_MY_CATCH
			{
				RemovePlayer( pPlayer ) ;
			}
		}
	}


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LoginPlayerManager::AcceptNewConnection( )
{
__ENTER_FUNCTION

	INT iStep = 0 ;
	BOOL ret = FALSE ;

	//����ҳ����ҳ�һ�����е�������ݼ�
	LoginPlayer* client = g_pPlayerPool->NewPlayer() ;
//	Assert( client ) ;
	if( client==NULL )
		return FALSE ;

	iStep = 5 ;
	client->CleanUp( ) ;

	INT fd = INVALID_SOCKET ;

	iStep = 10 ;
	_MY_TRY
	{
		//���ܿͻ��˽���Socket���
		ret = m_pServerSocket->accept( client->GetSocket() ) ;
		if( !ret )
		{
			client->CleanUp() ;
			g_pPlayerPool->DelPlayer( client->PlayerID() ) ;
			return FALSE ;
		}
	}
	_MY_CATCH
	{
		iStep += 1000 ;
		goto EXCEPTION ;
	}

	_MY_TRY
	{
		iStep = 30 ;
		fd = (INT)client->GetSocket()->getSOCKET();
		if( fd == INVALID_SOCKET )
		{
			Assert(FALSE) ;
			goto EXCEPTION ;
		}

		iStep = 40 ;
		ret = client->GetSocket()->setNonBlocking() ;
		if( !ret )
		{
			Assert(FALSE) ;
			goto EXCEPTION ;
		}

		iStep = 50 ;
		if( client->GetSocket()->getSockError() )
		{
			Assert(FALSE) ;
			goto EXCEPTION ;
		}

		iStep = 60 ;
		ret = client->GetSocket()->setLinger(0) ;
		if( !ret )
		{	
			Assert(FALSE) ;
			goto EXCEPTION ;
		}

		iStep = 70 ;
		//��ʼ�����������Ϣ
		client->Init( ) ;
		//���õ�ǰ�ͻ������ӵ�״̬
		client->SetPlayerStatus( PS_LOGIN_CONNECT ) ;
		client->m_ConnectTime = g_pTimeManager->CurrentTime();

		iStep = 80 ;
		_MY_TRY
		{
			ret = AddPlayer( client ) ;
			if( !ret )
			{
				Assert(FALSE) ;
				goto EXCEPTION ;
			}
		}
		_MY_CATCH
		{
			iStep += 10000 ;
			goto EXCEPTION ;
		}
	}
	_MY_CATCH
	{
		iStep += 100000 ;
	}

	Log::SaveLog( LOGIN_LOGFILE, "AcceptNewConnection(SOCKET=%d, IP=%s)...OK", 
		client->GetSocket()->getSOCKET(), client->GetSocket()->m_Host ) ;

	return TRUE ;


EXCEPTION:
	Log::SaveLog( LOGIN_LOGFILE, "AcceptNewConnection(SOCKET=%d,IP=%s,Step=%d)...Fails", 
		client->GetSocket()->getSOCKET(), client->GetSocket()->m_Host,iStep) ;

	client->CleanUp() ;
	g_pPlayerPool->DelPlayer( client->PlayerID() ) ;
	
	

	return FALSE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LoginPlayerManager::AddPlayer( Player* pPlayer )
{
__ENTER_FUNCTION

	if( m_nFDSize>=FD_SETSIZE )
	{//�Ѿ������ܹ������������������
		Assert(FALSE) ;
		return FALSE ;
	}

	BOOL ret = PlayerManager::AddPlayer( pPlayer ) ;
	if( !ret )
	{
		Assert(FALSE) ;
		return FALSE ;
	}

	//��IncomingPlayerManagerģ������û�г���IDֵ��
	LoginPlayer* pLoginPlayer = (LoginPlayer*)pPlayer ;
//	pLoginPlayer->SetSceneID( INVALID_ID ) ;

	SOCKET fd = pPlayer->GetSocket()->getSOCKET() ;
	Assert( fd != INVALID_SOCKET ) ;

	m_MinFD = min(fd , m_MinFD);
	m_MaxFD = max(fd , m_MaxFD);

	FD_SET(fd , &m_ReadFDs[SELECT_BAK]);
	FD_SET(fd , &m_WriteFDs[SELECT_BAK]);
	FD_SET(fd , &m_ExceptFDs[SELECT_BAK]);

	m_nFDSize++ ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LoginPlayerManager::DelPlayer( PlayerID_t pid )
{
__ENTER_FUNCTION

	Assert( pid!=INVALID_ID ) ;
	LoginPlayer* pLoginPlayer = g_pPlayerPool->GetPlayer(pid) ;
	Assert( pLoginPlayer ) ;

	SOCKET fd = pLoginPlayer->GetSocket()->getSOCKET() ;

	Assert( m_MinFD!=INVALID_SOCKET ) ;
	Assert( m_MaxFD!=INVALID_SOCKET ) ;
	if( fd==INVALID_SOCKET ) 
	{
		Assert(FALSE) ;
	}

	if( fd==m_MinFD ) 
	{
		SOCKET s = m_MaxFD ;
		UINT nPlayerCount = GetPlayerNumber() ;
		for( UINT i=0; i<nPlayerCount; i++ )
		{
			if( m_pPlayers[i]==INVALID_ID )
				continue ;

			LoginPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
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
			if( s > m_SocketID )
			{
				m_MinFD = m_SocketID ;
			}
			else
			{
				m_MinFD = s ;
			}
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

			LoginPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
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
			if( s < m_SocketID )
			{
				m_MaxFD = m_SocketID ;
			}
			else
			{
				m_MaxFD = s ;
			}
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

	PlayerManager::RemovePlayer( pid ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LoginPlayerManager::HeartBeat( )
{
__ENTER_FUNCTION

	BOOL ret ;

	UINT uTime = g_pTimeManager->CurrentTime() ;
	
	UINT nPlayerCount = GetPlayerNumber() ;
	for( UINT i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i] == INVALID_ID )
			continue ;

		Player* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		if( pPlayer==NULL )
		{
			Assert(FALSE) ;
			return FALSE ;
		}

		ret = pPlayer->HeartBeat(uTime ) ;
		if( !ret )
		{//����߼���������ʧ�ܣ�����Ҫ�Ͽ���ǰ����
			ret = RemovePlayer( pPlayer ) ;
			Assert( ret ) ;
		}
		else
		{
			LoginPlayer* pLoginPlayer = (LoginPlayer*)pPlayer;
			Assert(pLoginPlayer);

			if(pLoginPlayer->GetPlayerStatus() == PS_LOGIN_CONNECT)
			{
				if(uTime-pLoginPlayer->m_ConnectTime>MAX_LOGIN_PLAYER_AUTH_TIME)
				{
					ret = RemovePlayer(pPlayer);
					Assert( ret ) ;
				}
			}
		}
		
	}


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}


VOID LoginPlayerManager::RemoveAllPlayer( )
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
	
		RemovePlayer( pPlayer ) ;
	}

__LEAVE_FUNCTION
}


BOOL LoginPlayerManager::ProcessCacheCommands( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	for( UINT i=0; i<m_QueSize; i++ )
	{
		Packet* pPacket = NULL ;
		PlayerID_t PlayerID ;
		UINT Flag ;

		ret = RecvPacket( pPacket, PlayerID, Flag ) ;
		if( !ret )
			break ;

		Assert( pPacket ) ;

		if( Flag==PF_REMOVE )
		{
			g_pPacketFactoryManager->RemovePacket( pPacket ) ;
			continue ;
		}

		BOOL bNeedRemove = TRUE ;

		if( PlayerID==INVALID_ID )
		{
			_MY_TRY
			{
				pPacket->Execute(NULL) ;
			}
			_MY_CATCH
			{
			}
		}
		else
		{
			_MY_TRY
			{
				Player* pPlayer = g_pPlayerPool->GetPlayer( PlayerID ) ;
				Assert( pPlayer ) ;
				UINT uret = pPacket->Execute(pPlayer) ;
				if( uret == PACKET_EXE_ERROR )
				{
					RemovePlayer( pPlayer ) ;
					MovePacket( PlayerID ) ;
				}
				else if( uret == PACKET_EXE_BREAK )
				{
				}
				else if( uret == PACKET_EXE_CONTINUE )
				{
				}
				else if( uret == PACKET_EXE_NOTREMOVE )
				{
					bNeedRemove = FALSE ;
				}
				else if( uret == PACKET_EXE_NOTREMOVE_ERROR )
				{
					bNeedRemove = FALSE ;

					RemovePlayer( pPlayer ) ;
					MovePacket( PlayerID ) ;
				}
			}
			_MY_CATCH
			{
			}
		}

		//������Ϣ
		if( bNeedRemove )
			g_pPacketFactoryManager->RemovePacket( pPacket ) ;
	}


	
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LoginPlayerManager::MovePacket( PlayerID_t PlayerID )
{
__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	UINT Cur = m_Head ;

	for( UINT i=0; i<m_QueSize; i++ )
	{
		if( m_PacketQue[Cur].m_pPacket == NULL )
			break ;

		if( m_PacketQue[Cur].m_PlayerID == PlayerID )
		{
			m_PacketQue[Cur].m_Flag = PF_REMOVE ;
		}

		Cur ++ ;
		if( Cur>=m_QueSize ) Cur = 0 ;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LoginPlayerManager::ResizeCache( )
{
__ENTER_FUNCTION

	ASYNC_PACKET* pNew = new ASYNC_PACKET[m_QueSize+MAX_CACHE_SIZE] ;
	if( pNew==NULL )
		return FALSE ;

	memcpy( pNew, &(m_PacketQue[m_Head]), sizeof(ASYNC_PACKET)*(m_QueSize-m_Head) ) ;
	if( m_Head!=0 )
	{
		memcpy( &(pNew[m_QueSize-m_Head]), &(m_PacketQue[0]), sizeof(ASYNC_PACKET)*(m_Head) ) ;
	}

	memset( m_PacketQue, 0, sizeof(ASYNC_PACKET)*m_QueSize ) ;
	SAFE_DELETE_ARRAY( m_PacketQue ) ;
	m_PacketQue = pNew ;

	m_Head = 0 ;
	m_Tail = m_QueSize ;
	m_QueSize = m_QueSize+MAX_CACHE_SIZE ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LoginPlayerManager::SendPacket( Packet* pPacket, PlayerID_t PlayerID, UINT Flag )
{
__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( m_PacketQue[m_Tail].m_pPacket )
	{//��������
		BOOL ret = ResizeCache( ) ;
		Assert( ret ) ;
	}

	m_PacketQue[m_Tail].m_pPacket = pPacket ;
	m_PacketQue[m_Tail].m_PlayerID = PlayerID ;
	m_PacketQue[m_Tail].m_Flag = Flag ;

	m_Tail ++ ;
	if( m_Tail>=m_QueSize ) 
		m_Tail = 0 ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LoginPlayerManager::RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID, UINT& Flag )
{
__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( m_PacketQue[m_Head].m_pPacket==NULL )
	{//��������û����Ϣ
		return FALSE ;
	}

	pPacket = m_PacketQue[m_Head].m_pPacket ;
	PlayerID = m_PacketQue[m_Head].m_PlayerID ;
	Flag = m_PacketQue[m_Head].m_Flag ;

	m_PacketQue[m_Head].m_pPacket = NULL ;
	m_PacketQue[m_Head].m_PlayerID = INVALID_ID ;
	m_PacketQue[m_Head].m_Flag = PF_NONE ;

	m_Head ++ ;
	if( m_Head>=m_QueSize ) 
		m_Head = 0 ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}


