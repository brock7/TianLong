

#ifndef __GCCHAT_H__
#define __GCCHAT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "CGChat.h"


namespace Packets
{

#define MAX_CHAT_SIZE 256

	class GCChat : public Packet 
	{
	public:
		GCChat( ){ Reset() ; } ;
		virtual ~GCChat( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHAT ; }
		virtual UINT			GetPacketSize()const 
		{
			if( m_ChatType==CHAT_TYPE_NORMAL )
			{
				return	sizeof(BYTE)+
						sizeof(BYTE)+
						m_ContexSize*sizeof(CHAR)+
						sizeof(BYTE)+
						m_SourNameSize*sizeof(CHAR)+
						sizeof(ObjID_t)+
						sizeof(CampID_t)+
						sizeof(UINT) ; 
			}
			else
			{
				return	sizeof(BYTE)+
						sizeof(BYTE)+
						m_ContexSize*sizeof(CHAR)+
						sizeof(BYTE)+
						m_SourNameSize*sizeof(CHAR)+
						sizeof(CampID_t)+
						sizeof(UINT) ; 
			}
		}

		VOID	Reset()
		{
			m_ChatType = 0 ;
			m_ContexSize = 0 ;
			m_SourNameSize = 0 ;
			m_CampID = -1;
			memset( m_Contex, 0, MAX_CHAT_SIZE ) ;
			memset( m_SourName, 0, MAX_CHARACTER_NAME ) ;
		};

	public:
		//ʹ�����ݽӿ�

		//���á���ȡ�������� ����CGChat::CHAT_TYPE
		VOID					SetChatType( BYTE bType ){ m_ChatType = bType ; } ;
		BYTE					GetChatType( ){ return m_ChatType ; } ;

		//���á���ȡ��������
		VOID					SetContexSize( BYTE bSize ){ m_ContexSize = bSize ; } ;
		BYTE					GetContexSize( ){ return m_ContexSize ; } ;

		VOID					SetContex( CHAR* pContex ){
			strncpy( m_Contex, pContex, MAX_CHAT_SIZE-1 ) ;
			m_Contex[MAX_CHAT_SIZE-1] = 0 ;
		};
		CHAR*					GetContex(){ return (CHAR*)m_Contex ; } ;

		//���á���ȡ˵���˵�����
		VOID					SetSourNameSize( BYTE bSize ){ m_SourNameSize = bSize ; } ;
		BYTE					GetSourNameSize( ){ return m_SourNameSize ; } ;

		VOID					SetSourName( const	CHAR* pSourName ){
			strncpy( m_SourName, pSourName, MAX_CHARACTER_NAME-1 ) ;
			m_SourName[MAX_CHARACTER_NAME-1] = 0 ;
		};
		CHAR*					GetSourName(){ return (CHAR*)m_SourName ; } ;

		VOID					SetSourID( ObjID_t id ){ m_SourID = id ; }
		ObjID_t					GetSourID( ){ return m_SourID ; }

		VOID					SetWorldChatID( UINT uid ){ m_uWorldChatID = uid ; }
		UINT					GetWorldChatID( ){ return m_uWorldChatID ; }

		CampID_t				GetSourCamp() { return m_CampID; }
		VOID					SetSourCamp( CampID_t id ) { m_CampID = id; } 

	private:
		//����
		BYTE					m_ChatType ;//������Ϣ���� enum CGChat::CHAT_TYPE
		
		//������������
		BYTE					m_ContexSize ;
		CHAR					m_Contex[MAX_CHAT_SIZE] ;

		//˵��������
		BYTE					m_SourNameSize ;
		CHAR					m_SourName[MAX_CHARACTER_NAME] ;

		//˵���ߵ�ObjID, ��ͨ˵������Ч
		ObjID_t					m_SourID ;

		CampID_t				m_CampID;	//��������Ӫ

		UINT					m_uWorldChatID ; //������Ϣ�����кţ����ھ������ת�Ƴ�����Ϣ��ʧ����
	};


	class GCChatFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCChat() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CHAT ; }
		UINT		GetPacketMaxSize()const { return 	sizeof(BYTE)+
														sizeof(BYTE)+
														sizeof(CHAR)*MAX_CHAT_SIZE+
														sizeof(BYTE)+
														sizeof(CHAR)*MAX_CHARACTER_NAME+
														sizeof(ObjID_t)+
														sizeof(CampID_t)+
														sizeof(UINT) ; }
	};


	class GCChatHandler 
	{
	public:
		static UINT Execute( GCChat* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
