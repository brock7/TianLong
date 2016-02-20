// GCShopMerchandiseList.h
// 
// ���˷����ͻ��˵���Ʒ�б�
// 
//////////////////////////////////////////////////////

#ifndef __GCSHOPMERCHANDISELIST_H__
#define __GCSHOPMERCHANDISELIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCShopMerchandiseList : public Packet
	{
	public:
		//����������Ʒ�ṹ
		struct _MERCHANDISE_ITEM
		{
			UINT		idTable;		//��Դid
			BYTE		byNumber;		//����
			UINT		nPrice;			//�۸�
			INT			MaxNumber;		//������Ʒ������
			_MERCHANDISE_ITEM()
			{
				idTable		=	0;		//��Դid
				byNumber	=	0;		//����
				nPrice		=	0;			//�۸�
				MaxNumber	=	0;		//������Ʒ������
			}
			static UINT GetSize()
			{
				return sizeof(UINT) + sizeof(BYTE) + sizeof(UINT) + sizeof(INT);
			}
			BOOL Write( SocketOutputStream& oStream )const
			{
				__ENTER_FUNCTION
				oStream.Write( (CHAR*)(&idTable),	sizeof(UINT));
				oStream.Write( (CHAR*)(&byNumber),	sizeof(BYTE));
				oStream.Write( (CHAR*)(&nPrice),	sizeof(UINT));
				oStream.Write( (CHAR*)(&MaxNumber),	sizeof(INT));
				__LEAVE_FUNCTION
				return FALSE;
			}
			BOOL Read( SocketInputStream& iStream ) 
			{
				__ENTER_FUNCTION
				iStream.Read( (CHAR*)(&idTable),	sizeof(UINT));
				iStream.Read( (CHAR*)(&byNumber),	sizeof(BYTE));
				iStream.Read( (CHAR*)(&nPrice),		sizeof(UINT));
				iStream.Read( (CHAR*)(&MaxNumber),	sizeof(INT));
				__LEAVE_FUNCTION
				return FALSE;
			}
		};

	public:
		GCShopMerchandiseList( )
		{
			m_nObjID = INVALID_ID;
			m_nMerchadiseNum = 0;
			m_UniqueID = 0;
			m_bBuyBack = 0;
			m_nRepairLevel = 0;		
			m_nBuyLevel =0;		
			m_nRepairType =0;		
			m_nBuyType=0;			
			m_nRepairSpend =0.0f;	
			m_nRepairOkProb =0.0f;	
			m_bBuyBack = 0;			
			m_fScale = 0;			
			m_bCanBuyMulti = 0;

		}
		virtual ~GCShopMerchandiseList( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_SHOPMERCHANDISELIST ; }
		virtual UINT			GetPacketSize()const { return	
			sizeof(ObjID_t) + sizeof(BYTE)*2 + sizeof(INT)*4+sizeof(FLOAT)*2+sizeof(UINT)+sizeof(BYTE)+sizeof(FLOAT)+
			sizeof(UCHAR)*3 + m_nMerchadiseNum*_MERCHANDISE_ITEM::GetSize();}

	public:
		BYTE			GetMerchadiseNum(VOID) const {return m_nMerchadiseNum;};
		VOID			SetMerchadiseNum(BYTE nNumber) {m_nMerchadiseNum = nNumber;};

		_MERCHANDISE_ITEM*	GetMerchadiseList(VOID) { return m_MerchadiseList; }
		VOID				SetMerchadiseList(_MERCHANDISE_ITEM* pSour)
		{
			memcpy(m_MerchadiseList, pSour, m_nMerchadiseNum*sizeof(_MERCHANDISE_ITEM));
		}

		VOID			SetBuyType(INT nBuyType){m_nBuyType = nBuyType;}
		INT				GetBuyType(VOID){return m_nBuyType;}

		VOID			SetRepairType(INT nRepairType){m_nRepairType = nRepairType;}
		INT				GetRepairType(VOID){return m_nRepairType;}

		VOID			SetBuyLevel(INT nBuyLevel){m_nBuyLevel = nBuyLevel;}
		INT				GetBuyLevel(VOID){return m_nBuyLevel;}

		VOID			SetRepairLevel(INT nRepairLevel){m_nRepairLevel = nRepairLevel;}
		INT				GetRepairLevel(VOID){return m_nRepairLevel;}

		VOID			SetRepairSpend(FLOAT nRepairSpend){m_nRepairSpend = nRepairSpend;}
		FLOAT			GetRepairSpend(VOID){return m_nRepairSpend;}

		VOID			SetRepairOkProb(FLOAT nRepairOkProb){m_nRepairOkProb = nRepairOkProb;}
		FLOAT			GetRepairOkProb(VOID){return m_nRepairOkProb;}

		VOID			SetObjID(ObjID_t id) { m_nObjID = id; }
		ObjID_t			GetObjID(void) { return m_nObjID; }

		VOID			SetUniqueID(UINT id) { m_UniqueID = id; }
		UINT			GetUniqueID(void) { return m_UniqueID; }

		VOID			SetCanBuyBack(BYTE bBuyBack) { m_bBuyBack = bBuyBack; }
		BYTE			GetCanBuyBack(void) { return m_bBuyBack; }

		VOID			SetScale(FLOAT fScale) { m_fScale = fScale; }
		FLOAT			GetScale(void) { return m_fScale; }

		UCHAR			GetCurrencyUnit( ) { return m_uCurrencyUnit; }
		VOID			SetCurrencyUnit( UCHAR uUnit ) { m_uCurrencyUnit = uUnit; }

		UCHAR			GetSerialNum( ) { return m_uSerialNum; }
		VOID			SetSerialNum( UCHAR uSn ) { m_uSerialNum = uSn; }

		UCHAR			GetBuyMulti( ) { return m_bCanBuyMulti; }
		VOID			SetBuyMulti( UCHAR bCanBuyMulti ) { m_bCanBuyMulti = bCanBuyMulti; }

		UCHAR			GetShopType( ) { return m_nShopType; }
		VOID			SetShopType( UCHAR nType ) { m_nShopType = nType; }

	private:
		//���˵�id
		ObjID_t						m_nObjID;
		BYTE						m_nShopType;		// �̵�����
		//��Ʒ����
		BYTE						m_nMerchadiseNum;
		INT							m_nRepairLevel;		// ����ȼ�
		INT							m_nBuyLevel;		// �չ��ȼ�
		INT							m_nRepairType;		// ��������
		INT							m_nBuyType;			// �̵���չ�����
		FLOAT						m_nRepairSpend;		// ������
		FLOAT						m_nRepairOkProb;	// ����ɹ�����
		UINT						m_UniqueID;
		BYTE						m_bBuyBack;			//	�Ƿ�֧�ֻع�

		FLOAT						m_fScale;			// �̵�ϵ��

		UCHAR						m_uCurrencyUnit;
		UCHAR						m_uSerialNum;
		UCHAR						m_bCanBuyMulti;		//�Ƿ��ܹ�ָ����������

		//������Ʒ�б�
		_MERCHANDISE_ITEM			m_MerchadiseList[MAX_BOOTH_NUMBER];
	};


	class GCShopMerchandiseListFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCShopMerchandiseList() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_SHOPMERCHANDISELIST; };
		UINT		GetPacketMaxSize()const
		{
			return sizeof(ObjID_t)
				+sizeof(BYTE)
				+sizeof(BYTE)
				+sizeof(INT)
				+sizeof(INT)
				+sizeof(INT)
				+sizeof(INT)
				+sizeof(FLOAT)
				+sizeof(FLOAT)
				+sizeof(UINT)
				+sizeof(BYTE)
				+sizeof(FLOAT)
				+sizeof(UCHAR)
				+sizeof(UCHAR)
				+sizeof(UCHAR)
				+sizeof(GCShopMerchandiseList::_MERCHANDISE_ITEM) * MAX_BOOTH_NUMBER;
		}
	};

	class GCShopMerchandiseListHandler 
	{
	public:
		static UINT Execute( GCShopMerchandiseList* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif


