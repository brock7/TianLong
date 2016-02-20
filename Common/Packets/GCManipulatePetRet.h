// GCManipulatePetRet.h
// 
// ����������ﷵ�ؽ��
//
//////////////////////////////////////////////////////

#ifndef __GC_MANIPULATEPETRET_H__
#define __GC_MANIPULATEPETRET_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCManipulatePetRet : public Packet
	{
	public:
		enum ENUM_MANIPULATEPET_RET
		{
			MANIPULATEPET_RET_INVALID = -1,
			MANIPULATEPET_RET_CAPTUREFALID,		// ��׽ʧ��
			MANIPULATEPET_RET_CAPTURESUCC,		// ��׽�ɹ�
			MANIPULATEPET_RET_CALLUPFALID,		// �ٻ�ʧ��
			MANIPULATEPET_RET_CALLUPSUCC,		// �ٻ��ɹ�
			MANIPULATEPET_RET_FREEFALID,		// ����ʧ��
			MANIPULATEPET_RET_FREESUCC,			// �����ɹ�
			MANIPULATEPET_RET_RECALLFALID,		// �ջ�ʧ��
			MANIPULATEPET_RET_RECALLSUCC,		// �ջسɹ�

		};

		GCManipulatePetRet( )
		{
			m_bFighting = FALSE;
			m_Ret = -1;
		}

		~GCManipulatePetRet( ){}

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_MANIPULATEPETRET ; }
		virtual UINT			GetPacketSize()const { return (UINT)sizeof(PET_GUID_t)+
															  (UINT)sizeof(BOOL)+
															  (UINT)sizeof(INT); }
		
	public:
		//ʹ�����ݽӿ�
		PET_GUID_t			GetGUID(VOID) const { return m_GUID; }
		VOID				SetGUID(PET_GUID_t guid) { m_GUID = guid; }
		BOOL				IsFighting(VOID) const { return m_bFighting; }
		INT					GetManipulateRet(VOID) const { return (ENUM_MANIPULATEPET_RET)m_Ret; }
		VOID				SetManipulateRet(INT Ret) { m_Ret = Ret; }
		VOID				SetFightingFlag(BOOL bFlag) { m_bFighting = bFlag; }

	private:
		PET_GUID_t		m_GUID;
		BOOL			m_bFighting;	// �Ƿ��ڲ�ս״̬
		INT				m_Ret;			// ���ؽ��
		
	};


	class GCManipulatePetRetFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCManipulatePetRet() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_MANIPULATEPETRET; }
		UINT		GetPacketMaxSize()const { return (UINT)sizeof(PET_GUID_t)+
													 (UINT)sizeof(BOOL)+
													 (UINT)sizeof(INT); }
	};

	class GCManipulatePetRetHandler 
	{
	public:
		static UINT Execute( GCManipulatePetRet* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif // __GCPLAYERDIE_H__
