#ifndef __CLASKCREATECHAR_H__
#define __CLASKCREATECHAR_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class CLAskCreateChar : public Packet 
	{
	public:
		CLAskCreateChar( ){} ;
		virtual ~CLAskCreateChar( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CL_ASKCREATECHAR ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(CHAR)*MAX_CHARACTER_NAME+
				   sizeof(BOOL)+
				   sizeof(BYTE)+
				   sizeof(BYTE)+
				   sizeof(BYTE)+
				   sizeof(BYTE)+
				   sizeof(BYTE)+
				   sizeof(GUID_t)+
				   sizeof(CampID_t)+
				   sizeof(MenPaiID_t);
				
		}

	public:
		//ʹ�����ݽӿ�
		VOID		   SetName(const CHAR* pName)
		{
			strncpy(m_Name,pName,MAX_CHARACTER_NAME-1);
		}
		const CHAR*	  GetName()	const
		{
			return m_Name;
		}

		VOID		SetSex(BOOL bSex)
		{
			m_Sex = bSex;
		}
		BOOL		GetSex()
		{
			return m_Sex;
		}
		VOID		SetHairColor(BYTE bColor)
		{
			m_HairColor	 = bColor;
		}
		BYTE		GetHairColor()
		{
			return m_HairColor;
		}
		VOID		SetFaceColor(BYTE bColor)
		{
			m_FaceColor = bColor;
		}
		BYTE		GetFaceColor()
		{
			return m_FaceColor;
		}
		VOID		SetHairModel(BYTE bModel)
		{
			m_HairModel = bModel;
		}
		BYTE		GetHairModel()
		{
			return m_HairModel;
		}
		VOID		SetFaceModel(BYTE bModel)
		{
			m_FaceModel	 = bModel;
		}
		BYTE		GetFaceModel()
		{
			return m_FaceModel;
		}
		VOID		SetHeadID(BYTE bHead)
		{
			m_HeadID = bHead;
		}
		BYTE		GetHeadID()
		{
			return m_HeadID;
		}

		PlayerID_t				GetPlayerID() const
		{
			return PlayerID;
		}
		VOID					SetPlayerID(PlayerID_t	 pID)
		{
			PlayerID = pID;
		}

		const	CHAR*			GetAccount()	const;
		VOID					SetAccount(const CHAR*	pAccount);

		GUID_t					GetCharGuid()
		{
			return m_GUID;
		}

		VOID					SetCharGuid(GUID_t guid)
		{
			m_GUID	 = guid;
		}

		CampID_t				GetCharCamp()
		{
			return m_Camp ;
		}

		VOID					SetCharCamp(CampID_t id)
		{
			m_Camp = id;
		}

		MenPaiID_t				GetMenPai()
		{
			return m_MenPai;
		}

		VOID					SetMenPai(MenPaiID_t id)
		{
			m_MenPai = id;
		}

	private:
		//����
		CHAR			m_Name[MAX_CHARACTER_NAME];		//�½�ɫ����
		BOOL			m_Sex;							//�½�ɫ�Ա�
		BYTE			m_HairColor;					//�½�ɫͷ����ɫ	
		BYTE			m_FaceColor;					//�½�ɫ������ɫ
		BYTE			m_HairModel;					//�½�ɫͷ��ģ��
		BYTE			m_FaceModel;					//�½�ɫ����ģ��
		BYTE			m_HeadID;						//�½�ɫͷ�����
		GUID_t			m_GUID;							//
		CampID_t		m_Camp;							//�½�ɫ����Ӫ
		MenPaiID_t		m_MenPai;
		
		//��ҳ�id���ͻ��˲�����д
		PlayerID_t				PlayerID;
		CHAR					szAccount[MAX_ACCOUNT+1] ;	//�û�����
	};

	class CLAskCreateCharFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CLAskCreateChar() ; }
		PacketID_t	GetPacketID()const { return PACKET_CL_ASKCREATECHAR ; }
		UINT		GetPacketMaxSize() const
		{ 
				return sizeof(CHAR)*MAX_CHARACTER_NAME+
				sizeof(BOOL)+
				sizeof(BYTE)+
				sizeof(BYTE)+
				sizeof(BYTE)+
				sizeof(BYTE)+
				sizeof(BYTE)+
				sizeof(GUID_t)+
				sizeof(CampID_t)+
				sizeof(MenPaiID_t);

		}
	};


	class CLAskCreateCharHandler 
	{
	public:
		static UINT Execute( CLAskCreateChar* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;


#endif