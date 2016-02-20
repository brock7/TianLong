//
//�ļ����ƣ�	SocketInputStream.h
//����������	��Ϣ���ݵĽ��ջ��棬�ṩ���ݵĽ��պ͸�ʽ����ȡ����
//				
//
//


#ifndef __SOCKETINPUTSTREAM_H__
#define __SOCKETINPUTSTREAM_H__

#include "BaseType.h"
#include "Socket.h"

//��ʼ���Ľ��ջ��泤��
#define DEFAULTSOCKETINPUTBUFFERSIZE 64*1024
//����������Ļ��泤�ȣ������������ֵ����Ͽ�����
#define DISCONNECTSOCKETINPUTSIZE 96*1024

class Packet ;
class SocketInputStream
{
public :
	SocketInputStream( Socket* sock, 
					   UINT BufferSize = DEFAULTSOCKETINPUTBUFFERSIZE,
					   UINT MaxBufferSize = DISCONNECTSOCKETINPUTSIZE ) ;
	virtual ~SocketInputStream( ) ;


public :
	UINT						Read( CHAR* buf, UINT len ) ;
	BOOL						ReadPacket( Packet* p ) ;

	BOOL						Peek( CHAR* buf, UINT len ) ;

	BOOL						Find( CHAR* buf ) ;
	
	BOOL						Skip( UINT len ) ;
	
	UINT						Fill( ) ;
	UINT						Fill_RAW( ) ;

	VOID						Initsize( ) ;
	BOOL						Resize( INT size ) ;
	
	UINT						Capacity( )const { return m_BufferLen; }
	
	UINT						Length( )const ;
	UINT						Size( )const { return Length(); }

	BOOL						IsEmpty( )const { return m_Head==m_Tail; }

	VOID						CleanUp( ) ;

	UINT						GetHead(){return m_Head;}
	UINT						GetTail(){return m_Tail;}
	UINT						GetBuffLen(){return m_BufferLen;}
	CHAR*						GetBuff(){return m_Buffer;}
private :
	Socket*		m_pSocket ;
	
	CHAR*		m_Buffer ;
	
	UINT		m_BufferLen ;
	UINT		m_MaxBufferLen ;
	
	UINT		m_Head ;
	UINT		m_Tail ;



};




#endif
