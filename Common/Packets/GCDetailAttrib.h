// GCDetailAttrib.h
// 
// ��ɫ����ϸ����
// 
//////////////////////////////////////////////////////

#ifndef __GCDETAILATTRIB_H__
#define __GCDETAILATTRIB_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"


namespace Packets
{
	class GCDetailAttrib :
		public Packet
	{
	public:
		GCDetailAttrib( )
		{
			m_Flags.m_uHighFlags	=0;
			m_Flags.m_uLowFlags		=0;
			//m_nCampType				=INVALID_CAMP;
		}
		virtual ~GCDetailAttrib( ){}

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_DETAILATTRIB ; }
		virtual UINT			GetPacketSize()const 
		{
			UINT uSize = sizeof(ObjID_t) + sizeof(Flag64) ;
			UINT i;
			for ( i = 0; i < DETAIL_ATTRIB_NUMBERS; i++ )
			{
				if(m_Flags.isSetBit(i))
				{
					switch ( i )
					{
					case DETAIL_ATTRIB_CAMP:
						uSize += sizeof( m_CampData );
						break;
					case DETAIL_ATTRIB_CURRENT_PET_GUID:
						uSize += sizeof( PET_GUID_t );
						break;
					default:
						uSize += sizeof( UINT );	
						break;
					}
				}
			}
			return uSize;
		}

	public:
		VOID			SetObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t			GetObjID(VOID)const { return m_ObjID; }

		Flag64			GetFlags( VOID )const{ return m_Flags; }

		BOOL			IsUpdateAttrib( ENUM_DETAIL_ATTRIB eAttrib )const{ return m_Flags.isSetBit((INT)eAttrib)?(TRUE):(FALSE); }
		VOID			SetUpdateAttrib( ENUM_DETAIL_ATTRIB eAttrib, BOOL bUpdate ){ m_Flags.UpdateBits((INT)eAttrib,bUpdate); }

		UINT			GetLevel( VOID )const{ return m_uLevel; }
		VOID			SetLevel( UINT uLevel ){ m_uLevel =uLevel; SetUpdateAttrib( DETAIL_ATTRIB_LEVEL, TRUE ); }

		INT				GetMenpai(VOID)	const	{return m_nMenPai;}
		VOID			SetMenpai(INT	Menpai){ m_nMenPai	=	Menpai; SetUpdateAttrib(DETAIL_ATTRIB_MEMPAI,TRUE); }

		INT				GetGuild(VOID)	const	{return m_nGuild;}
		VOID			SetGuild(INT	nGuild){ m_nGuild	=	nGuild; SetUpdateAttrib(DETAIL_ATTRIB_GUILD,TRUE); }

		const _CAMP_DATA		*GetCampData(VOID)const{ return &m_CampData; }
		VOID					SetCampData( const _CAMP_DATA *pCampData ) { m_CampData = *pCampData; SetUpdateAttrib(DETAIL_ATTRIB_CAMP,TRUE); }
		//INT				GetCampType(VOID)	const	{ return m_nCampType; }
		//VOID			SetCampType(INT	nCampType)		{m_nCampType = nCampType; SetUpdateAttrib(DETAIL_ATTRIB_CAMP,TRUE);}

		INT				GetDataID(VOID)	const	{return m_nDataID;}
		VOID			SetDataID(INT	nDataID)		{m_nDataID = nDataID; SetUpdateAttrib(DETAIL_ATTRIB_DATAID,TRUE);}

		INT				GetPortraitID(VOID)	const	{return m_nPortraitID;}
		VOID			SetPortraitID(INT	nPortraitID)		{m_nPortraitID = nPortraitID; SetUpdateAttrib(DETAIL_ATTRIB_PORTRAITID,TRUE);}

		INT				GetModelID(VOID)	const	{return m_nModelID;}
		VOID			SetModelID(INT	nModelID)		{m_nModelID = nModelID; SetUpdateAttrib(DETAIL_ATTRIB_MODELID,TRUE);}

		INT				GetMountID(VOID)	const	{return m_nMountID;}
		VOID			SetMountID(INT	nID)		{m_nMountID = nID; SetUpdateAttrib(DETAIL_ATTRIB_MOUNTID,TRUE);}

		PET_GUID_t		GetCurrentPetGUID( VOID )const{ return m_guidCurrentPet; }
		VOID			SetCurrentPetGUID( PET_GUID_t guid ){ m_guidCurrentPet =guid; SetUpdateAttrib( DETAIL_ATTRIB_CURRENT_PET_GUID, TRUE ); }

		INT				GetExp( VOID )const{ return m_nExp; }
		VOID			SetExp( INT nExp ){ m_nExp = nExp; SetUpdateAttrib( DETAIL_ATTRIB_EXP, TRUE ); }

		INT				GetHP(VOID) const{return m_nHP;}
		VOID			SetHP(INT nHp){m_nHP	= nHp;SetUpdateAttrib(DETAIL_ATTRIB_HP,TRUE);}

		INT				GetMP(VOID) const {return m_nMP;}
		VOID			SetMP(INT nMp){m_nMP = nMp; SetUpdateAttrib(DETAIL_ATTRIB_MP,TRUE);}

		INT				GetMAXHP(VOID) const {return m_nMAXHP;}
		VOID			SetMAXHP(INT nMaxHp) {m_nMAXHP = nMaxHp;SetUpdateAttrib(DETAIL_ATTRIB_MAXHP,TRUE);}

		INT				GetMAXMP(VOID) const {return m_nMAXMP;}
		VOID			SetMAXMP(INT nMaxMp) {m_nMAXMP = nMaxMp;SetUpdateAttrib(DETAIL_ATTRIB_MAXMP,TRUE);}

		FLOAT			GetMoveSpeed(VOID) const {return m_fMoveSpeed;}
		VOID			SetMoveSpeed(FLOAT fMoveSpeed){m_fMoveSpeed = fMoveSpeed;SetUpdateAttrib(DETAIL_ATTRIB_MOVESPEED,TRUE);}

		UINT			GetMoney( VOID )const{ return m_uMoney; }
		VOID			SetMoney( UINT uMoney ){ m_uMoney =uMoney; SetUpdateAttrib( DETAIL_ATTRIB_MONEY, TRUE ); }

		INT				GetStr() const {return m_Str;}			
		VOID			SetStr(INT str){m_Str	=	str;SetUpdateAttrib(DETAIL_ATTRIB_STR,TRUE);}

		INT				GetSpr() const{return m_Spr;}			
		VOID			SetSpr(INT spr){m_Spr	=	spr;SetUpdateAttrib(DETAIL_ATTRIB_SPR,TRUE);}


		INT				GetCon() const {return m_Con;}			
		VOID			SetCon(INT con){m_Con	=	con;SetUpdateAttrib(DETAIL_ATTRIB_CON,TRUE);}

		INT				GetInt()const {return m_Int;}			
		VOID			SetInt(INT iInt){m_Int	=	iInt;SetUpdateAttrib(DETAIL_ATTRIB_INT,TRUE);}

		INT				GetDex() const {return m_Dex;}			
		VOID			SetDex(INT dex){m_Dex	=	dex;SetUpdateAttrib(DETAIL_ATTRIB_DEX,TRUE);}


		INT				GetPoint_Remain( VOID )const{ return m_nPoint_Remain; }
		VOID			SetPoint_Remain( INT nPoint_Remain ){ m_nPoint_Remain = nPoint_Remain; SetUpdateAttrib( DETAIL_ATTRIB_POINT_REMAIN, TRUE ); }

		INT				GetHP_ReSpeed( VOID )const{ return m_nHP_ReSpeed; }
		VOID			SetHP_ReSpeed( INT nHP_ReSpeed ){ m_nHP_ReSpeed = nHP_ReSpeed; SetUpdateAttrib( DETAIL_ATTRIB_HP_RESPEED, TRUE ); }

		INT				GetMP_ReSpeed( VOID )const{ return m_nMP_ReSpeed; }
		VOID			SetMP_ReSpeed( INT nMP_ReSpeed ){ m_nMP_ReSpeed = nMP_ReSpeed; SetUpdateAttrib( DETAIL_ATTRIB_MP_RESPEED, TRUE ); }


		INT				GetAtt_Physics( VOID )const{ return m_nAtt_Physics; }
		VOID			SetAtt_Physics( INT nAtt_Physics ){ m_nAtt_Physics = nAtt_Physics; SetUpdateAttrib( DETAIL_ATTRIB_ATT_PHYSICS, TRUE ); }

		INT				GetAtt_Magic( VOID )const{ return m_nAtt_Magic; }
		VOID			SetAtt_Magic( INT nAtt_Magic ){ m_nAtt_Magic = nAtt_Magic; SetUpdateAttrib( DETAIL_ATTRIB_ATT_MAGIC, TRUE ); }

		INT				GetDef_Physics( VOID )const{ return m_nDef_Physics; }
		VOID			SetDef_Physics( INT nDef_Physics ){ m_nDef_Physics = nDef_Physics; SetUpdateAttrib( DETAIL_ATTRIB_DEF_PHYSICS, TRUE ); }

		INT				GetDef_Magic( VOID )const{ return m_nDef_Magic; }
		VOID			SetDef_Magic( INT nDef_Magic ){ m_nDef_Magic = nDef_Magic; SetUpdateAttrib( DETAIL_ATTRIB_DEF_MAGIC, TRUE ); }

		INT				GetHit( VOID )const{ return m_nHit; }
		VOID			SetHit( INT nHit ){ m_nHit = nHit; SetUpdateAttrib( DETAIL_ATTRIB_HIT, TRUE ); }

		INT				GetMiss(VOID) const {return m_nMiss;}
		VOID			SetMiss(INT nMiss){m_nMiss	=	 nMiss;SetUpdateAttrib(DETAIL_ATTRIB_MISS,TRUE);}

		INT				GetCritic( VOID )const{ return m_nCritic; }
		VOID			SetCritic( INT Critic ){ m_nCritic = Critic; SetUpdateAttrib( DETAIL_ATTRIB_CRITRATE, TRUE ); }

		INT				GetRage( VOID )const{ return m_nRage; }
		VOID			SetRage( INT Rage ){ m_nRage = Rage; SetUpdateAttrib( DETAIL_ATTRIB_RAGE, TRUE ); }

		INT				GetStrikePoint( VOID )const{ return m_nStrikePoint; }
		VOID			SetStrikePoint( INT StrikePoint ){ m_nStrikePoint = StrikePoint; SetUpdateAttrib( DETAIL_ATTRIB_STRIKE_POINT, TRUE ); }

		INT				GetAttSpeed(VOID) const {return m_nAttackSpeed;}
		VOID			SetAttSpeed(INT AttSpeed){m_nAttackSpeed = AttSpeed;SetUpdateAttrib(DETAIL_ATTRIB_ATTACKSPEED,TRUE);}

		INT				GetAttCold(VOID)	const {return m_nAttCold;}
		VOID			SetAttCold(INT AttCold){ m_nAttCold = AttCold;SetUpdateAttrib(DETAIL_ATTRIB_ATTACKCOLD,TRUE);}

		INT				GetAttFire(VOID) const {return m_nAttFire;}
		VOID			SetAttFire(INT AttFire){m_nAttFire = AttFire;SetUpdateAttrib(DETAIL_ATTRIB_ATTACKFIRE,TRUE);}

		INT				GetAttLight(VOID)	const {return m_nAttLight;}
		VOID			SetAttLight(INT AttLight){m_nAttLight	=	 AttLight; SetUpdateAttrib(DETAIL_ATTRIB_ATTACKLIGHT,TRUE);}

		INT				GetAttPoison(VOID)	const {return m_nAttPoison;}
		VOID			SetAttPoison(INT AttPoison){m_nAttPoison	= AttPoison; SetUpdateAttrib(DETAIL_ATTRIB_ATTACKPOISON,TRUE);}

		INT				GetDefCold(VOID)	const {return m_nDefCold;}
		VOID			SetDefCold(INT DefCold){ m_nDefCold = DefCold;SetUpdateAttrib(DETAIL_ATTRIB_DEFENCECOLD,TRUE);}

		INT				GetDefFire(VOID) const {return m_nDefFire;}
		VOID			SetDefFire(INT DefFire){m_nDefFire = DefFire;SetUpdateAttrib(DETAIL_ATTRIB_DEFENCEFIRE,TRUE);}

		INT				GetDefLight(VOID)	const {return m_nDefLight;}
		VOID			SetDefLight(INT DefLight){m_nDefLight	=	 DefLight; SetUpdateAttrib(DETAIL_ATTRIB_DEFENCELIGHT,TRUE);}

		INT				GetDefPoison(VOID)	const {return m_nDefPoison;}
		VOID			SetDefPoison(INT DefPoison){m_nDefPoison	= DefPoison; SetUpdateAttrib(DETAIL_ATTRIB_DEFENCEPOISON,TRUE);}

		INT				GetLimitMove(VOID)	const {return m_bLimitMove;}
		VOID			SetLimitMove(BOOL bLimitMove){m_bLimitMove	= bLimitMove; SetUpdateAttrib(DETAIL_ATTRIB_LIMIT_MOVE,TRUE);}

		INT				GetCanActionFlag1(VOID)	const {return m_bCanActionFlag1;}
		VOID			SetCanActionFlag1(BOOL bFlag){m_bCanActionFlag1	= bFlag; SetUpdateAttrib(DETAIL_ATTRIB_CAN_ACTION1,TRUE);}

		INT				GetCanActionFlag2(VOID)	const {return m_bCanActionFlag2;}
		VOID			SetCanActionFlag2(BOOL bFlag){m_bCanActionFlag2	= bFlag; SetUpdateAttrib(DETAIL_ATTRIB_CAN_ACTION2,TRUE);}

		//INT			GetLimitHandle(VOID)	const {return m_bLimitHandle;}
		//VOID			SetLimitHandle(BOOL bLimitHandle){m_bLimitHandle	= bLimitHandle; SetUpdateAttrib(DETAIL_ATTRIB_LIMIT_HANDLE,TRUE);}

		INT				GetRMB(VOID)	const {return m_RMBMoney;}
		VOID			SetRMB(INT nRMB){m_RMBMoney	= nRMB; SetUpdateAttrib(DETAIL_ATTRIB_RMB,TRUE);}

		INT				GetBankRMB(VOID)	const {return m_BankMoney;}
		VOID			SetBankRMB(INT nRMB){m_BankMoney	= nRMB; SetUpdateAttrib(DETAIL_ATTRIB_BANK_RMB,TRUE);}

		INT				GetDoubleExpTime_Num(VOID)	const {return m_DoubleExpTime_Num;}
		VOID			SetDoubleExpTime_Num(INT nExpTime_Num){m_DoubleExpTime_Num	= nExpTime_Num; SetUpdateAttrib(DETAIL_ATTRIB_ENERGY,TRUE);}

		/*INT				GetGmRight(VOID)	const {return m_GmRight;}
		VOID			SetGmRight(INT nRight){m_GmRight	= nRight; SetUpdateAttrib(DETAIL_ATTRIB_MAX_ENERGY,TRUE);}*/

	private:
		ObjID_t			m_ObjID;	// ����Obj���͵�ObjID

		Flag64			m_Flags;		// ÿ��λ��ʾһ�������Ƿ�Ҫˢ�� ENUM_DETAIL_ATTRIB


		UINT			m_uLevel;		// �ȼ�
		INT				m_nExp;			// ����ֵ
		INT				m_nHP;			//������
		INT				m_nMP;			//ħ����
		INT				m_nMAXHP;		//���������
		INT				m_nMAXMP;		//���ħ����
		FLOAT			m_fMoveSpeed;	//�ƶ��ٶ�


		UINT			m_uMoney;		// ��Ϸ����

		//һ��ս������


		INT				m_Str;					//���� ����
		INT				m_Spr;					//���� ����
		INT				m_Con;					//���� ����
		INT 			m_Int;					//���� ����
		INT 			m_Dex;					//�� ����
		INT				m_nPoint_Remain;		//ʣ����������

		//����ս������
		INT				m_nHP_ReSpeed;		//HP�ָ��ٶ�  ��/��
		INT				m_nMP_ReSpeed;		//MP�ָ��ٶ�  ��/��
		INT				m_nAtt_Physics;		//��������
		INT				m_nAtt_Magic;		//ħ��������
		INT				m_nDef_Physics;		//���������
		INT				m_nDef_Magic;		//ħ��������
		INT				m_nHit;				//������
		INT				m_nMiss;			//������
		INT				m_nCritic;			//����һ����

		INT				m_nRage;			//ŭ��
		INT				m_nStrikePoint;		//������

		INT				m_nAttackSpeed;		//�����ٶ�

		INT				m_nAttCold;			//������
		INT				m_nDefCold;			//������
		INT				m_nAttFire;			//�𹥻�
		INT				m_nDefFire;			//�����
		INT				m_nAttLight;		//�繥��
		INT				m_nDefLight;		//�����

		INT				m_nAttPoison;		//������
		INT				m_nDefPoison;		//������

		INT				m_nMenPai;			//����
		INT				m_nGuild;			//����


		_CAMP_DATA		m_CampData;			// ��Ӫ	
		INT				m_nDataID;			// DataID
		INT				m_nPortraitID;		// ͷ��ID
		INT				m_nModelID;			// ����
		INT				m_nMountID;			//����
		PET_GUID_t		m_guidCurrentPet;	//��ǰ����
		
		BOOL			m_bLimitMove;		//�Ƿ����Ʋ����ƶ�
		BOOL			m_bCanActionFlag1;		//�������ޱ��1,���ڻ��Դ���
		BOOL			m_bCanActionFlag2;		//�������ޱ��2,���ڳ�Ĭ

		INT				m_RMBMoney;			//m_Vigor ;			// ����  //Ԫ��
		INT				m_BankMoney;		//m_MaxVigor ;		// �������� //�����е�Ԫ��
		INT				m_DoubleExpTime_Num ;			// ˫������ʱ��ͱ���;
		INT				m_GmRight ;		// ��������

		//INT				m_nGoodBadValue;	// �ƶ�ֵ
	};

	class GCDetailAttribFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCDetailAttrib() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_DETAILATTRIB; }
		UINT		GetPacketMaxSize()const { return	sizeof(GCDetailAttrib) - sizeof(Packet); }
														
	};

	class GCDetailAttribHandler 
	{
	public:
		static UINT Execute( GCDetailAttrib* pPacket, Player* pPlayer ) ;
	};
}

#endif // __GCDETAILATTRIB_H__
