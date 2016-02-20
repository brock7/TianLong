// CGManipulatePet.h
// 
// ����������ﷵ�ؽ��
//
//////////////////////////////////////////////////////

#ifndef __CG_MANIPULATEPET_H__
#define __CG_MANIPULATEPET_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	enum ENUM_MANIPULATE_TYPE
	{
		MANIPULATE_INVALID = -1,	// ��Ч
		MANIPULATE_CREATEPET,		// �ٻ�����
		MANIPULATE_DELETEPET,		// �ջس���
		MANIPULATE_FREEPET,			// ��������
		MANIPULATE_ASKOTHERPETINFO, // �쿴������ҵĳ�����Ϣ(�������ѵ���...)


		MANIPULATE_NUMBERS,
	};

	class CGManipulatePet : public Packet
	{
	public:

		CGManipulatePet( )
		{
		}

		~CGManipulatePet( ){}

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_MANIPULATEPET ; }
		virtual UINT			GetPacketSize()const 
		{
				UINT uSize = sizeof(ObjID_t) + sizeof(PET_GUID_t) + sizeof(INT);
				return uSize;
		}

		
	public:
		VOID			SetObjID(const ObjID_t objID) { m_ObjID = objID; }
		ObjID_t			GetObjID(VOID) const { return m_ObjID; }

		VOID			SetGUID(PET_GUID_t guid){ m_PetGUID = guid; }
		PET_GUID_t		GetGUID(VOID)const{ return m_PetGUID; }

		// nType	:	ENUM_MANIPULATE_TYPE
		VOID			SetManipulateType(INT nType){ m_nType = nType; }
		INT				GetManipulateType(VOID)const{ return m_nType; }

	private:
		ObjID_t			m_ObjID;	// �����ObjID;
		PET_GUID_t		m_PetGUID;
		INT				m_nType;
	};


	class CGManipulatePetFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGManipulatePet() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_MANIPULATEPET; }
		UINT		GetPacketMaxSize()const 
		{

			UINT uSize = sizeof(CGManipulatePet) - sizeof(Packet);
			return uSize;
		}
	};

	class CGManipulatePetHandler 
	{
	public:
		static UINT Execute( CGManipulatePet* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif // __GCPLAYERDIE_H__
