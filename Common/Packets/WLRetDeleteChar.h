#ifndef _WLRETDELETECHAR_H_
#define _WLRETDELETECHAR_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class WLRetDeleteChar : public Packet 
	{
	public:
		WLRetDeleteChar( ){} ;
		virtual ~WLRetDeleteChar( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_WL_RETDELETECHAR ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(ASKDELETECHAR_RESULT)+sizeof(CHAR)*MAX_ACCOUNT+sizeof(PlayerID_t)+sizeof(GUID_t);
		}

	public:
		//ʹ�����ݽӿ�
		VOID					SetResult(ASKDELETECHAR_RESULT Res)
		{
			Result = Res;
		}

		ASKDELETECHAR_RESULT		GetResult()
		{
			return Result;
		}

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

		VOID					SetPlayerGUID(GUID_t guid)
		{
			PlayerGUID	=  guid;
		}
		GUID_t					GetPlayerGUID()
		{
			return PlayerGUID;
		}
	private:
		//����
		ASKDELETECHAR_RESULT	Result;
		//��ɫguid
		CHAR					szAccount[MAX_ACCOUNT+1];	//�û�����;
		//��ҳ�id
		PlayerID_t				PlayerID;
		//��ɫGUID
		GUID_t					PlayerGUID;

	};

	class WLRetDeleteCharFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WLRetDeleteChar() ; }
		PacketID_t	GetPacketID()const { return PACKET_WL_RETDELETECHAR ; }
		UINT		GetPacketMaxSize() const
		{ 
			return sizeof(ASKDELETECHAR_RESULT)+sizeof(CHAR)*MAX_ACCOUNT+sizeof(PlayerID_t)+sizeof(GUID_t);
		}
	};


	class WLRetDeleteCharHandler 
	{
	public:
		static UINT Execute( WLRetDeleteChar* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets;
#endif
