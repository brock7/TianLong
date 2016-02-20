

#ifndef __WebPlayer_H__
#define __WebPlayer_H__

#include "Type.h"
#include "Config.h"
#include "ServerSocket.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"
#include "ServerPlayer.h"


#define WORLD_PLAYER_ID			0
#define BILLING_PLAYER_ID		1

//ϵͳ�ڿ���ӵ�е����ķ������˳�������
#define MAX_SERVER 24

class WebPlayer
{
public :
	WebPlayer( ) ;
	~WebPlayer( ) ;

	VOID				CleanUp( ) ;

	//��ʼ��
	BOOL				Init( ) ;

	//���߼��ӿ�
	BOOL				Tick( ) ;

	//�����������
	BOOL				Select( ) ;
	//���ݽ��ջ��������ӽ���
	BOOL				ProcessInputs( ) ;
	//���ݷ���
	BOOL				ProcessOutputs( ) ;
	//�����쳣�Ͽ�����
	BOOL				ProcessExceptions( ) ;
	//��Ϣ����
	BOOL				ProcessCommands( ) ;

	//�߼�����
	virtual BOOL		HeartBeat( ) ;

public :
	//������ע�ᣨֻ��ע�������������ܽ������ݣ�
	BOOL				AddServer( SOCKET fd ) ;
	//���ӳ���
	BOOL				DelServer( SOCKET fd ) ;
	//�Ͽ����Ӻ��������մ���
	BOOL				RemoveServer( ) ;

	//���ӵ�Web����������
	BOOL				ConnectWebServer( );
public :
	//******************************************************************************
	//ͨ�ýӿ�
	//******************************************************************************

	//ȡ�õ�ǰ�ķ������˳���ID
	ID_t				GetWebID( ) ;
	//ȡ�õ�ǰ�ķ������˳�����Ϣ
	_SERVER_DATA*		GetCurrentServerInfo( ) ;

	//�жϷ������������Ƿ��ڻ״̬
	BOOL				IsWebServerActive( );

	BOOL				SendPacket( Packet* pPacket ) ;

private :
	//
	//��������
	enum{
		SELECT_BAK = 0,	//��ǰϵͳ��ӵ�е������������
		SELECT_USE = 1,	//����select���õľ������
		SELECT_MAX = 2, //�ṹʹ������
	};
	fd_set			m_ReadFDs[SELECT_MAX];
	fd_set			m_WriteFDs[SELECT_MAX];
	fd_set			m_ExceptFDs[SELECT_MAX];

	timeval			m_Timeout[SELECT_MAX];

	SOCKET			m_MinFD;
	SOCKET			m_MaxFD;

	INT				m_nFDSize ;
	//��������
	//

	MyLock			m_Lock ;

	//��ǰ���������������ӵ�Socket
	ServerSocket*	m_pServerSocket ;
	SOCKET			m_SocketID ;

	//��ǰ�������˳������Ϣ
	_SERVER_DATA*	m_pCurServerInfo ;

	ServerPlayer	m_WebPlayer;

	PlayerID_t		m_pServerHash ;

public :
	TID						m_ThreadID ;
};

extern WebPlayer* g_pWebPlayer ;

#endif
