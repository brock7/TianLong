#ifndef _LWASKDELETECHAR_H_
#define _LWASKDELETECHAR_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

#include "DB_Struct.h"

namespace Packets
{
	class LWAskDeleteChar : public Packet 
	{
	public:
		LWAskDeleteChar( ){} ;
		virtual ~LWAskDeleteChar( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LW_ASKDELETECHAR ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(CHAR)*MAX_ACCOUNT + sizeof(PlayerID_t) + sizeof(GUID_t);
		}

	public:
		//ʹ�����ݽӿ�
		const	CHAR*			GetAccount()	const;
		VOID					SetAccount(const CHAR*	pAccount);

		PlayerID_t				GetPlayerID() const
		{
			return PlayerID;
		}

		VOID					SetPlayerID(PlayerID_t	 pID)
		{
			PlayerID = pID;
		}

		GUID_t					GetPlayerGUID()
		{
			return PlayerGUID;
		}

		VOID					SetPlayerGUID(GUID_t guid)
		{
			PlayerGUID	= guid;
		}

	private:
		//����
		CHAR					szAccount[MAX_ACCOUNT+1];	//�˻�����
		//��ҳ�id
		PlayerID_t				PlayerID;
		//���GUID_t
		GUID_t					PlayerGUID;
	};

	class LWAskDeleteCharFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LWAskDeleteChar() ; }
		PacketID_t	GetPacketID()const { return PACKET_LW_ASKDELETECHAR ; }
		UINT		GetPacketMaxSize() const
		{ 
			return sizeof(CHAR)*MAX_ACCOUNT + sizeof(PlayerID_t) + sizeof(GUID_t);
		}
	};


	class LWAskDeleteCharHandler 
	{
	public:
		static UINT Execute( LWAskDeleteChar* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;


#endif