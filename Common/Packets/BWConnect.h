

#ifndef __SSCONNECT_H__
#define __SSCONNECT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class BWConnect : public Packet 
	{
	public:
		BWConnect( ){} ;
		virtual ~BWConnect( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_BW_CONNECT ; }
		virtual UINT			GetPacketSize() const { return sizeof(ID_t)+(sizeof(CHAR)*KEY_SIZE); }

	public:
		//ʹ�����ݽӿ�
		ID_t			GetServerID( ){ return m_ServerID ; } ;
		VOID			SetServerID( ID_t id ){ m_ServerID = id ; } ;
		VOID			SetKey(CHAR* NewKey){strcpy(m_szKey, NewKey);}
		CHAR*			GetKey(){return m_szKey;}

	private:
		//����
		ID_t			m_ServerID ;	//������Ϣ�ķ������˳����ServerID
		CHAR			m_szKey[KEY_SIZE];
	};


	class BWConnectFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new BWConnect() ; }
		PacketID_t	GetPacketID() const { return PACKET_BW_CONNECT ; }
		UINT		GetPacketMaxSize() const { return sizeof(ID_t)+(sizeof(CHAR)*KEY_SIZE); }
	};


	class BWConnectHandler 
	{
	public:
		static UINT Execute( BWConnect* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
