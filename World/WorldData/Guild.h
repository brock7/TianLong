/////////////////////////////////////////////////////////////////////////////////
//�ļ�����Guild.h
//����������������ݺ��߼�����
/////////////////////////////////////////////////////////////////////////////////


#ifndef __GUILD_H__
#define __GUILD_H__

#include "Type.h"
#include "GameDefine_Guild.h"
#include "DB_Struct.h"

#define MAX_USER_IN_GUILD			160
#define USER_ARRAY_SIZE				MAX_USER_IN_GUILD + MAX_PROPOSER_IN_GUILD

//����г�Ա����Ϣ
struct GUILDUSER
{
	GUID_t				m_UserGUID;							//���GUID
	CHAR				m_szUserName[MAX_CHARACTER_NAME];	//�������
	GUILD_POSITION		m_Position;							//ְ��
	UINT				m_uAccess;							//Ȩ��
	BOOL				m_bOnlineFlag;						//�Ƿ����ߣ���������Ҫ�洢
	UINT				m_uLastLoginTime;					//�������ʱ��
	UINT				m_uLevel;							//���Ｖ��
	CHAR				m_bMenPaiID;						//����
	INT					m_iCurContribute;					//����
	INT					m_iMaxContribute;					//�����
	INT					m_iJoinTime;						//����ʱ��

	GUILDUSER( )
	{
		CleanUp( );
	};
	VOID CleanUp( )
	{	
		m_UserGUID		= INVALID_ID;
		memset( m_szUserName, 0, sizeof(CHAR)*MAX_CHARACTER_NAME );
		m_Position		= GUILD_POSITION_TRAINEE;
		m_uAccess		= 0;
		m_bOnlineFlag	= FALSE;
		m_uLastLoginTime= 0;
		m_uLevel		=	0;
		m_bMenPaiID		=	-1;
		m_iCurContribute=	0;
		m_iMaxContribute=	0;
		m_iJoinTime		=	0;
	}
};

struct _GUILD_INIT
{
	GuildID_t			m_GuildID;							//���ID
	CHAR				m_GuildName[MAX_GUILD_NAME_SIZE];	//�������
	GUILD_STATUS		m_Status;							//����״̬
	GUID_t				m_ChieftainGUID;					//���� GUID
	INT					m_nProposerCount;					//��������
	INT					m_UserCount;						//����û�����
	INT					m_MaxUserSize;						//����˿�����
	INT					m_GuildPoint;						//��ṱ�׵�
	INT					m_GuildMoney;						//����ʽ�
	INT					m_nTime;							//����ʱ��򴴽�ʣ��ʱ��
	GUILDUSER			m_aGuildUser[USER_ARRAY_SIZE];		//����Ա

	_GUILD_INIT( )
	{
		m_GuildID = INVALID_ID;
		memset( m_GuildName, 0, sizeof(CHAR)*MAX_GUILD_NAME_SIZE );
		m_Status = GUILD_STATUS_INVALID;
		m_ChieftainGUID = INVALID_ID;
		m_nProposerCount = 0; // ��������
		m_UserCount = 0;
		m_MaxUserSize = 0;
		m_GuildPoint = 0;
		m_GuildMoney = 0;
		m_nTime = 0;
		for( INT i=0; i<USER_ARRAY_SIZE; i++ )
		{
			m_aGuildUser[i].CleanUp();
		}
	};
	~_GUILD_INIT( )
	{
	};
};

class Packet;
class City;
class USER;

class Guild
{
public :
	Guild( );
	~Guild( );
	//-------------------------------------------------------------------------------------------------
	//enum
	//-------------------------------------------------------------------------------------------------
	enum PacketType
	{
		PACKET_RETURN	= 1, // ���ز���
	};

	//��������
	enum OPT_TYPE
	{
		OPT_NONE	= 0,	// ���ز���
		OPT_ASK_MEMBER_LIST,			//����ǰ���а��ɳ�Ա�б�
		OPT_ASK_DETAIL_INFO,			//������ɵ���ϸ��Ϣ
		OPT_ASK_APPOINT_POS_INFO,		//������ɵĿ�������Ϣ
		OPT_ASK_SELF_INFO,				//�����˰�����Ϣ
	};

	//-------------------------------------------------------------------------------------------------
	//��ʼ��
	//-------------------------------------------------------------------------------------------------
	BOOL				Init( _GUILD_INIT* pInit );
	BOOL				InitFromShareMem();
	VOID				ValidateShareMem();
	VOID				CleanUp( );

	//-------------------------------------------------------------------------------------------------
	//��������
	//-------------------------------------------------------------------------------------------------
	BOOL				HeartBeat( UINT uTime=0 );		//�߼������ӿ�
	BOOL				IsMemberFull( ) { return (m_UserCount >= m_MaxUserSize); }// �Ƿ����Ѿ���Ա
	BOOL				IsProposerFull( ) { return (m_nProposerCount >= MAX_PROPOSER_IN_GUILD); }// �Ƿ��Ѿ���̫���������
	BOOL				IsAuthorized( GUID_t userGUID, GUILD_AUTHORITY authority );	// �ж��Ƿ���ĳ��Ȩ��
	BOOL				IsSameCamp(CampID_t nCamp) { return GetAmbi() == (INT)nCamp; } //�Ƿ�ͬ��Ӫ

	//-------------------------------------------------------------------------------------------------
	// �����¼��ӿ�
	//-------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------
	// ������ᣬ��������ʽ��ᣬFALSE ��ʾ���������������������
	BOOL				OnGuildCreate( const USER* pCreater, const CHAR* szName, CampID_t nCamp );
	VOID				OnGuildCreateFailed();
	VOID				OnGuildFound();
	BOOL				OnGuildDestroy( GUID_t createrGUID );

	//-------------------------------------------------------------------------------------------------
	// ����ְ��Ȩ�ޱ����Ȩ���ж�
	GUILD_ADMIN_t::ORESULT	OnUserPositionChange( GUID_t userGUID, GUILD_POSITION position );//����ְ���ԭ��,һ��������ְΪ��ͨ����,����ְΪĳ����Ա
	
	VOID				OnUserAuthorityChange( GUID_t userGUID, GUILD_AUTHORITY authority );
	VOID				OnPositionAuthorityChange( GUILD_POSITION position, GUILD_AUTHORITY authority );
	VOID				OnUserAccessChange( GUID_t userGUID, INT newAccess ); //���Ȩ�ޱ仯
	VOID				OnGuildPointChange( GUID_t userGUID, INT nMoney ); //��ṱ�׵㷢���仯
	BOOL				CanDoIt(OPT_TYPE Opt, GUILD_POSITION position, GUILD_AUTHORITY authority);//����ְλ��Ȩ�޲����ж�����
	BOOL				CanAppointedBy(GUILD_POSITION position, const GUILDUSER* pUser);//�Ƿ��ܱ���������
	CHAR*				GetPosName(INT iPosition);//��ô˰���е�ǰְλ������

	//-------------------------------------------------------------------------------------------------
	// ������������Ӧһ�����
	GUILD_ERROR_TYPE	OnUserEnter( const USER* pUser, CampID_t nCamp );
	GUILD_ERROR_TYPE	OnRecruitUser( GUID_t userGUID );// ���հ���
	GUILD_ERROR_TYPE	OnUserLeave( GUID_t userGUID );// ���������뿪���
	GUILD_ERROR_TYPE	OnUserLeaveByIndex( INT iIndex );// ���������뿪���
	GUILD_ERROR_TYPE	OnDemise();// ���ã�������ְ��ͬ���ǣ�������Ҫ������ְ����˼���ȥ
	VOID				OnGuildMoneyChange( GUID_t userGUID, INT nMoney ); //����ʽ����仯// ֧ȡ��������

	//-------------------------------------------------------------------------------------------------
	// ������������
	VOID				OnUserLogin( GUID_t userGUID );
	VOID				OnUserLogoff( GUID_t userGUID );

	//-------------------------------------------------------------------------------------------------
	// �㲥���� position ����ְ��İ��ڹ㲥����������Ϣ
	VOID				BroadCast_Return( Packet* pPacket, GUILD_POSITION positon = GUILD_POSITION_MEMBER );

	//-------------------------------------------------------------------------------------------------
	//������
	BOOL				IsLock(){return m_IsLock;}
	VOID				SetLock(BOOL bIsLock){m_IsLock	=	bIsLock;}

	//-------------------------------------------------------------------------------------------------
	//���ݽӿ�
	//-------------------------------------------------------------------------------------------------

	//�˰���ID
	GuildID_t			GetGuildID( ){ return m_GuildID; }

	// �������
	const CHAR*			GetGuildName( ) { return m_GuildName; }

	// �����ּ
	const CHAR*			GetGuildDesc( ) { return m_GuildDesc; }

	// �����ּ
	VOID				SetGuildDesc(CHAR*	pszGuildDesc)
	{
		memset(m_GuildDesc, 0, MAX_GUILD_DESC_SIZE);
		memcpy(m_GuildDesc, pszGuildDesc, MAX_GUILD_DESC_SIZE); 
	}

	BYTE				GetGuildLevel(){ return m_GuildLevel;}
	VOID				SetGuildLevel(BYTE GuildLevel){m_GuildLevel = GuildLevel;}

	//������
	CHAR*				GetGuildChiefName(){return m_GuildChiefName;}

	//��������
	CHAR*				GetCreatorName( ) { return m_GuildCreatorName; }

	//���ɵ����
	City*				GetCity(){ return m_pCity;}

	//����ʱ��
	INT					GetFoundedTime(){return m_nTime;}

	// ȡ�ð���
	const GUILDUSER*	GetGuildUser( GUID_t userGUID );

	// ͨ��������ð���
	GUILDUSER*	GetGuildUserByIndex( INT nIndex);

	// ȡ�ð�������
	INT					GetGuildUserCount( ) { return m_UserCount; }

	// ȡ��Ԥ����������
	INT					GetGuildProposerCount( ) { return m_nProposerCount; }

	// ȡ�ð�������
	INT					GetGuildUserMax( ) { return m_MaxUserSize; }

	// ���״̬
	GUILD_STATUS		GetGuildStatus( ) { return m_Status; }

	// ���� GUID
	GUID_t				GetChieftainGUID( ) { return m_ChieftainGUID; }

	//-------------------------------------------------------------------------------------------------
	//�������Խӿ�
	
	//����
	INT					GetLongevity(){ return m_Longevity;}
	VOID				SetLongevity(INT Longevity){ m_Longevity = Longevity;}

	//���׶�
	INT					GetContribute(){ return m_Contribute;}
	VOID				SetContribute(INT Contribute){ m_Contribute = Contribute;}

	//����
	INT					GetHonor(){ return m_Honor;}
	VOID				SetHonor(INT Honor){ m_Honor = Honor;}

	//��ҵ��
	INT					GetIndustry(){ return m_nIndustryLevel;}
	VOID				SetIndustry(INT nIndustryLevel){ m_nIndustryLevel = nIndustryLevel;}

	//ũҵ��
	INT					GetAgr(){ return m_nAgrLevel;}
	VOID				SetAgr(INT nAgrLevel){ m_nAgrLevel = nAgrLevel;}

	//��ҵ��
	INT					GetCom(){ return m_nComLevel;}
	VOID				SetCom(INT nComLevel){ m_nComLevel = nComLevel;}

	//������
	INT					GetDef(){ return m_nDefLevel;}
	VOID				SetDef(INT nDefLevel){ m_nDefLevel = nDefLevel;}

	//�Ƽ���
	INT					GetTech(){ return m_nTechLevel;}
	VOID				SetTech(INT nTechLevel){ m_nTechLevel = nTechLevel;}

	//���Ŷ�
	INT					GetAmbi(){ return m_nAmbiLevel;}
	VOID				SetAmbi(INT nAmbiLevel){ m_nAmbiLevel = nAmbiLevel;}

	//����ʽ�
	INT					GetMoney(){ return m_GuildMoney;}
	VOID				SetMoney(INT GuildMoney){ m_GuildMoney = GuildMoney;}


protected :
	//-------------------------------------------------------------------------------------------------
	//����ڲ��߼������ܽӿ�
	VOID				AddGuildUser( GUILDUSER& GuildUser );

	
protected :
	inline GUILDUSER*	ILGetGuildUser( GUID_t userGUID );
	inline INT			ILGetGuildUserIndex( GUID_t userGUID );

	GuildID_t			m_GuildID; //���ID
	CHAR				m_GuildName[MAX_GUILD_NAME_SIZE];		//�������
	CHAR				m_GuildDesc[MAX_GUILD_DESC_SIZE];		//�������
	CHAR				m_GuildChiefName[MAX_CHARACTER_NAME];	//������
	CHAR				m_GuildCreatorName[MAX_CHARACTER_NAME];	//��������
	BYTE				m_GuildLevel;
	GUILD_STATUS		m_Status;								//����״̬
	GUID_t				m_ChieftainGUID;						//������ GUID
	INT					m_nProposerCount;						//��������
	INT					m_UserCount;							//����û�����
	INT					m_MaxUserSize;							//����˿�����
	INT					m_GuildPoint;							//��ṱ�׵�
	GUILDUSER			m_aGuildUser[USER_ARRAY_SIZE];			//����Ա
	BOOL				m_IsLock;								//��ǰ�����Ƿ���ס��ĳЩ������ֻ��ͬʱ����һ�����紴������
	CityID_t			m_CityID;								//�󶨵ĳ���ID
	City*				m_pCity;								//�󶨵ĳ���ָ��

	// GUILD_STATUS_UNCOMMITTED ʱ����ʾʣ�������
	// GUILD_STATUS_NORMAL ʱ����ʾ������ʱ��
	INT					m_nTime;
	GuildSMU*			m_pGuildSmu;
protected:
	//-------------------------------------------------------------------------------------------------
	//��������
	INT					m_Longevity;				//����
	INT					m_Contribute;				//���׶�
	INT					m_Honor;					//����
	INT					m_nIndustryLevel;			//��ҵ��
	INT					m_nAgrLevel;				//ũҵ��
	INT					m_nComLevel;				//��ҵ��
	INT					m_nDefLevel;				//������
	INT					m_nTechLevel;				//�Ƽ���
	INT					m_nAmbiLevel;				//���Ŷ�
	INT					m_GuildMoney;				//����ʽ�

	//-------------------------------------------------------------------------------------------------
	//����ṹ
	GUILD_ADMIN_t		m_AdminLayOut;							


private :

	// ���ڴ洢����������ʱ���
	UINT				m_uTimeStamp;

	// �������
	INT					m_nUserMallocIndex;
};


#endif
