#ifndef _LCRET_LOGIN_H_
#define _LCRET_LOGIN_H_
#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class LCRetLogin : public Packet 
	{
	public:
		LCRetLogin( ){} ;
		virtual ~LCRetLogin( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LC_RETLOGIN ; }
		virtual UINT			GetPacketSize() const 
		{
			return				sizeof(CHAR)*MAX_ACCOUNT+
								sizeof(LOGIN_RESULT);
		}

	public:
		//ʹ�����ݽӿ�
		const	CHAR*			GetAccount()	const;
		VOID					SetAccount(const CHAR*	pAccount);
	
		LOGIN_RESULT			GetResult() const;
		VOID					SetResult(LOGIN_RESULT result);
	private:
		//����
		//û�п�BillingSystem ǰʹ�ÿͻ����Լ���GUID
		CHAR					szAccount[MAX_ACCOUNT+1];	//�û�����
		LOGIN_RESULT			Result;
	};

	class LCRetLoginFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LCRetLogin() ; }
		PacketID_t	GetPacketID()const { return PACKET_LC_RETLOGIN ; }
		UINT		GetPacketMaxSize() const
		{ 
			return	sizeof(CHAR)*MAX_ACCOUNT+
					sizeof(LOGIN_RESULT);
		}
	};


	class LCRetLoginHandler 
	{
	public:
		static UINT Execute( LCRetLogin* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif