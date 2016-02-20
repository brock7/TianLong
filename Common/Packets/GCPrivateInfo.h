// GCStallOpen.h
// 
// ���߿ͻ��������Ƿ�ɹ�
// 
//////////////////////////////////////////////////////

#ifndef __GCPRIVATEINFO_H__
#define __GCPRIVATEINFO_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCPrivateInfo : public Packet
	{
	public:
		GCPrivateInfo( )
		{
			m_CharGUID		= 0;							
		}

		virtual ~GCPrivateInfo( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_PRIVATEINFO; }
		virtual UINT			GetPacketSize()const { return sizeof(UINT);}

	public:
		//UINT m_CharGUID
		UINT		GetCharGUID()  {return m_CharGUID;}
		VOID		SetCharGUID(UINT CharGUID)  {m_CharGUID = CharGUID;}

	private:
		UINT		m_CharGUID;							//��ɫ���

	};

	class GCPrivateInfoFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCPrivateInfo() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_PRIVATEINFO; };
		UINT		GetPacketMaxSize()const { return sizeof(UINT);}
	};

	class GCPrivateInfoHandler 
	{
	public:
		static UINT Execute( GCPrivateInfo* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
