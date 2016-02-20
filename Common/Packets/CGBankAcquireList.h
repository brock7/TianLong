// CGBankAcquireList.h
// 
// ���߿ͻ��������Ƿ�ɹ�
// 
//////////////////////////////////////////////////////

#ifndef __CGBANKACQUIRELIST_H__
#define __CGBANKACQUIRELIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGBankAcquireList : public Packet
	{
	public:
		CGBankAcquireList( )
		{
			m_BankID = 0;
		};
		virtual ~CGBankAcquireList( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_BANKACQUIRELIST; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE);}

	public:
		BYTE					GetBankID(VOID) const {return m_BankID;};
		VOID					SetBankID(BYTE BankID) {m_BankID = BankID;};

	private:
		BYTE					m_BankID;		//�ɹ����
	};

	class CGBankAcquireListFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGBankAcquireList() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_BANKACQUIRELIST; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE);};
	};

	class CGBankAcquireListHandler 
	{
	public:
		static UINT Execute( CGBankAcquireList* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
