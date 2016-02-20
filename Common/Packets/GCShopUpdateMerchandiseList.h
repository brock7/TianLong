// GCShopUpdateMerchandiseList.h
// 
// ���˷����ͻ��˵���Ʒ�б�
// 
//////////////////////////////////////////////////////

#ifndef __GCSHOPUPDATEMERCHANDISELIST_H__
#define __GCSHOPUPDATEMERCHANDISELIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCShopUpdateMerchandiseList: public Packet
	{
	public:
		//����������Ʒ�ṹ
		struct _MERCHANDISE_ITEM
		{
			UINT		idTable;		//��Դid
			BYTE		byNumber;		//����
			_MERCHANDISE_ITEM()
			{
				idTable = 0;
				byNumber = 0;
			}
		};

	public:
		GCShopUpdateMerchandiseList( )
		{
			m_nMerchadiseNum = 0;
		}
		virtual ~GCShopUpdateMerchandiseList( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_SHOPUPDATEMERCHANDISELIST ; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+
			m_nMerchadiseNum*sizeof(_MERCHANDISE_ITEM);}

	public:
		BYTE			GetMerchadiseNum(VOID) const {return m_nMerchadiseNum;};
		VOID			SetMerchadiseNum(BYTE nNumber) {m_nMerchadiseNum = nNumber;};

		_MERCHANDISE_ITEM*	GetMerchadiseList(VOID) { return m_MerchadiseList; }
		VOID				SetMerchadiseList(_MERCHANDISE_ITEM* pSour)
		{
			memcpy(m_MerchadiseList, pSour, m_nMerchadiseNum*sizeof(_MERCHANDISE_ITEM));
		}


	private:
		//��Ʒ����
		BYTE						m_nMerchadiseNum;
		//������Ʒ�б�
		_MERCHANDISE_ITEM			m_MerchadiseList[MAX_BOOTH_NUMBER];
	};


	class GCShopUpdateMerchandiseListFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCShopUpdateMerchandiseList() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_SHOPUPDATEMERCHANDISELIST; };
		UINT		GetPacketMaxSize()const { return 
			sizeof(GCShopUpdateMerchandiseList::_MERCHANDISE_ITEM)*MAX_BOOTH_NUMBER + sizeof(BYTE);};
	};

	class GCShopUpdateMerchandiseListHandler 
	{
	public:
		static UINT Execute( GCShopUpdateMerchandiseList* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif


