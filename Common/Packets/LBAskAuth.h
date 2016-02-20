#ifndef __LBASKAUTH_H__
#define __LBASKAUTH_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class LBAskAuth : public Packet 
	{
	public:
		LBAskAuth( ){} ;
		virtual ~LBAskAuth( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LB_ASKAUTH ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(CHAR)*MAX_ACCOUNT+
					sizeof(CHAR)*MAX_PASSWORD;

		}

	public:
		//ʹ�����ݽӿ�
		const	CHAR*			GetAccount()	const;
		VOID					SetAccount(const CHAR*	pAccount);
		const	CHAR*			GetPassWord()	const;
		VOID					SetPassWord(const CHAR*	pPassWord);

		const	PlayerID_t		GetPlayerID() const;
		VOID					SetPlayerID(PlayerID_t	pID);
	private:
		//����
		CHAR					szAccount[MAX_ACCOUNT+1] ;	//�û�����
		CHAR					szPassWord[MAX_PASSWORD+1];	//�û�����
		PlayerID_t				PlayerID;
		
	};

	class LBAskAuthFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LBAskAuth() ; }
		PacketID_t	GetPacketID()const { return PACKET_LB_ASKAUTH ; }
		UINT		GetPacketMaxSize() const
		{ 
			return	sizeof(CHAR)*MAX_ACCOUNT+
				sizeof(CHAR)*MAX_PASSWORD;
		}
	};


	class LBAskAuthHandler 
	{
	public:
		static UINT Execute( LBAskAuth* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;


#endif