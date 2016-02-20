#ifndef _LCRET_CREATECHAR_H_
#define _LCRET_CREATECHAR_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class LCRetCreateChar : public Packet 
	{
	public:
		LCRetCreateChar( ){} ;
		virtual ~LCRetCreateChar( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LC_RETCREATECHAR; }
		virtual UINT			GetPacketSize() const 
		{
				return	sizeof(ASKCREATECHAR_RESULT);
		}

	public:
		//ʹ�����ݽӿ�
		ASKCREATECHAR_RESULT		GetResult() const;
		VOID						SetResult(ASKCREATECHAR_RESULT result);
		const	CHAR*				GetAccount()	const;
		VOID						SetAccount(const CHAR*	pAccount);
	private:
		//����
		ASKCREATECHAR_RESULT		Result;
		CHAR						szAccount[MAX_ACCOUNT+1] ;	//�û�����
	
	};

	class LCRetCreateCharFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LCRetCreateChar() ; }
		PacketID_t	GetPacketID()const { return PACKET_LC_RETCREATECHAR; }
		UINT		GetPacketMaxSize() const
		{ 
			return 	sizeof(ASKCREATECHAR_RESULT);
		}
	};

	class LCRetCreateCharHandler 
	{
	public:
		static UINT Execute( LCRetCreateChar* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif