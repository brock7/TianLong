
#ifndef __GAMESTRUCT_H__
#define __GAMESTRUCT_H__

#include "Type.h"
#include "GameDefine.h"
#include "Assertx.h"

//#pragma pack(push, 1)

//��������������ĸ���λ��
struct WORLD_POS
{
	//enum
	//{
	//	POS_ERROR,
	//	POS_TOP,
	//	POS_TOP_LEFT,
	//	POS_TOP_RIGHT,
	//	POS_LEFT,
	//	POS_RIGHT,
	//	POS_BOTTOM,
	//	POS_BOTTOM_LEFT,
	//	POS_BOTTOM_RIGHT,
	//	POS_SAME,
	//};
	FLOAT	 m_fX ;
	FLOAT	 m_fZ ;

	WORLD_POS(VOID)					: m_fX(0.0f), m_fZ(0.0f)	{}
	WORLD_POS(FLOAT fX, FLOAT fZ)	: m_fX(fX)	, m_fZ(fZ)		{}
	VOID	CleanUp( ){
		m_fX = 0.0f ;
		m_fZ = 0.0f ;
	};
	//INT RelaPos(WORLD_POS const& rhs)
	//{
	//	if( (fabs(m_fX-rhs.m_fX)+fabs(m_fZ-rhs.m_fZ))<0.0001f ) return POS_SAME;
	//	if( fabs(m_fX-rhs.m_fX) < 0.0001f )
	//	{
	//		if( m_fZ > rhs.m_fZ ) return POS_BOTTOM;
	//		else return POS_TOP;
	//	}
	//	if( fabs(m_fZ-rhs.m_fZ)<0.0001f )
	//	{
	//		if( m_fX > rhs.m_fX ) return POS_LEFT;
	//		else return POS_RIGHT;
	//	}
	//	if( m_fX > rhs.m_fX && m_fZ > rhs.m_fZ )
	//		return POS_BOTTOM_LEFT;
	//	if( m_fX > rhs.m_fX && m_fZ < rhs.m_fZ )
	//		return POS_TOP_LEFT;
	//	if( m_fX < rhs.m_fX && m_fZ > rhs.m_fZ )
	//		return POS_BOTTOM_RIGHT;
	//	if( m_fX < rhs.m_fX && m_fZ < rhs.m_fZ )
	//		return POS_TOP_RIGHT;
	//	return POS_ERROR;
	//}
	WORLD_POS& operator=(WORLD_POS const& rhs)
	{
		m_fX = rhs.m_fX;
		m_fZ = rhs.m_fZ;
		return *this;
	}
	BOOL	operator==(WORLD_POS& Ref)
	{
		return (fabs(m_fX-Ref.m_fX)+fabs(m_fZ-Ref.m_fZ))<0.0001f;

	}
	BOOL	operator==(const WORLD_POS& Ref)
	{
		return (fabs(m_fX-Ref.m_fX)+fabs(m_fZ-Ref.m_fZ))<0.0001f;

	}
};

//�������������������λ��
struct MAP_POS
{
	Coord_t	 m_nX ;
	Coord_t	 m_nZ ;

	MAP_POS(VOID)					: m_nX(0)	, m_nZ(0)	{}
	MAP_POS(Coord_t nX, Coord_t nZ)	: m_nX(nX)	, m_nZ(nZ)	{}
	VOID	CleanUp( ){
		m_nX = 0 ;
		m_nX = 0 ;
	};
};




//Ч��״̬
struct _EFFECT
{
	BOOL				m_bActive ;
	INT					m_Value ;	//Ч��ֵ
	INT					m_Time ;	//Ч��ʱ��

	_EFFECT( )
	{
		CleanUp( ) ;
	}

	VOID				CleanUp( ){
		m_bActive = FALSE ;
		m_Value = 0 ;
		m_Time = 0 ;
	};

	BOOL				IsActive( ){ return m_bActive ; } ;
	VOID				SetActive( BOOL bActive ){ m_bActive = bActive ; } ;
};

//������������ʼ������
struct _MONSTERCREATER_INIT
{
	CHAR				m_FileName[_MAX_PATH] ;
	WORLD_POS			m_Position ;
};


#define			DEFAULT_ITEMBOX_RECYCLE_TIME	300000		//300�룬5����



//װ������
struct EQUIP_LIST
{
	GUID_t	m_GUID;			//װ������ID
	UINT	m_uParam1;		//װ������1
	UINT	m_uParam2;		//װ������2
};
#define EQUIP_PLAYER_FIXNUM		(8)		//����������������װ����

//��Ʒ����
struct EMBELLISH_LIST
{
	GUID_t	m_GUID;			//��Ʒ����ID
	UINT	m_uParam1;		//��Ʒ����1
	UINT	m_uParam2;		//��Ʒ����2
};
#define EMBELLISH_PLAYER_FIXNUM		(6)		//������������������Ʒ��


//��һ������л���Ϣ
struct PLAYER_OWN
{
	GUID_t    m_nGUID;						//�����ȫΨһID
	CHAR	  m_szName[MAX_CHARACTER_NAME];	//�������
	
	Coord_t   m_nX;						//���λ��X
	Coord_t   m_nZ;						//���λ��Z

	FLOAT     m_fDir;						//����泯�ķ��򣨷�Χ��0~1.0��
										//
										//       0.25 
										//    \   |   /
										// 0.5  \ | /
										//   ----   ----  0.0 (1.0f)
										//      / | \
										//    /   |   \
										//       0.75

	PLAYER_OWN( )
	{
		m_nGUID = INVALID_ID ;
		memset( m_szName, 0, MAX_CHARACTER_NAME ) ;
		m_nX = 0 ;
		m_nZ = 0 ;
		m_fDir = 0.0 ;
	};
};

struct PLAYER_S
{
	GUID_t    m_nGUID;						//�����ȫΨһID
	CHAR	  m_szName[MAX_CHARACTER_NAME];	//�������
	
	Coord_t   m_nX;						//���λ��X
	Coord_t   m_nZ;						//���λ��Z

	FLOAT     m_fDir;						//����泯�ķ��򣨷�Χ��0~1.0��
										//
										//       0.25 
										//    \   |   /
										// 0.5  \ | /
										//   ----   ----  0.0 (1.0f)
										//      / | \
										//    /   |   \
										//       0.75

	PLAYER_S( )
	{
		m_nGUID = INVALID_ID ;
		memset( m_szName, 0, MAX_CHARACTER_NAME ) ;
		m_nX = 0 ;
		m_nZ = 0 ;
		m_fDir = 0.0 ;
	};
};

struct VRECT
{
	INT nStartx ;
	INT nStartz ;
	INT nEndx ;
	INT nEndz ;

	VRECT( )
	{
		nStartx = 0 ;
		nStartz = 0 ;
		nEndx = 0 ;
		nEndz = 0 ;
	};

	BOOL IsContinue( INT x, INT z )const
	{
		if ( x < nStartx || x > nEndx || z < nStartz || z > nEndz )
			return FALSE;
		else
			return TRUE;
	}
};

//һ��ս�����Խṹ
struct _ATTR_LEVEL1
{
	INT		m_pAttr[CATTR_LEVEL1_NUMBER] ;

	_ATTR_LEVEL1( )
	{
		CleanUp( ) ;
	};
	INT	Get( INT iAttr )const{
		Assert( iAttr>=0 && iAttr<CATTR_LEVEL1_NUMBER ) ;
		return m_pAttr[iAttr] ;
	};  
	VOID Set( INT iAttr, INT iValue ){
		Assert( iAttr>=0 && iAttr<CATTR_LEVEL1_NUMBER ) ;
		m_pAttr[iAttr] = iValue ;
	} ;
	VOID CleanUp()
	{
		memset( m_pAttr, 0, sizeof(INT)*CATTR_LEVEL1_NUMBER ) ;
	} ;
};

//����ս�����Խṹ
struct _ATTR_LEVEL2
{
	INT		m_pAttr[CATTR_LEVEL2_NUMBER] ;

	_ATTR_LEVEL2( )
	{
		memset( m_pAttr, 0, sizeof(INT)*CATTR_LEVEL2_NUMBER ) ;
	}
	INT	Get( INT iAttr ){
		Assert( iAttr>=0 && iAttr<CATTR_LEVEL2_NUMBER ) ;
		return m_pAttr[iAttr] ;
	};
	VOID Set( INT iAttr, INT iValue ){
		Assert( iAttr>=0 && iAttr<CATTR_LEVEL2_NUMBER ) ;
		m_pAttr[iAttr] = iValue ;
	} ;
};

//��ɫ��ӵ�еĳƺ�
//#��ͷ���ַ���������һ���ַ�����ԴID,����ͨ��������������������ñ���������
#define IDTOSTRING(str, strid, strsize)	char str[strsize];\
										memset(str, 0, strsize);\
										sprintf(str, "#%d", strid);\

#define STRINGTOID(str, strid)			INT strid = atoi((CHAR*)(str+1));\
										

struct _TITLE
{
	enum
	{//��ʾ��ǰtitle
		NO_TITLE = 0,
		LEVEL_TITLE,
		LIFE_TITLE,
		RANK_TITLE,
		STORY_TITLE,
		KILLBOSS_TITLE,
		TREASURE_TITLE,
		DY_TITLE0,
		DY_TITLE1,
		DY_TITLE2,
		DY_TITLE3,
		DY_TITLE4,
		DY_TITLE5,
		DY_TITLE6,
		DY_TITLE7,
		DY_TITLE8,
		DY_TITLE9,
		NICK_TITLE,
		SPOUSE_TITLE,
		BANGPAI_TITLE,
		JIEBAI_TITLE,
		SHITU_TITLE,
		SHANGDIAN_TITLE,
		MOOD_TITLE,


		MAX_NUM_TITLE,
	};

	BYTE				m_LevelTitle;
	BYTE				m_LifeTitle;
	BYTE				m_RankTitle;
	BYTE				m_StoryTitle;
	BYTE				m_KillBossTitle;
	BYTE				m_TreasureTitle;
	SHORT				m_aDyTitle[MAX_DYNAMIC_TITLE_SIZE];
	CHAR				m_szNick[MAX_NICK_NAME];		//��ɫ�ǳ�
	CHAR				m_szSpouse[MAX_NICK_NAME];		//��ż����
	CHAR				m_szBangPai[MAX_NICK_NAME];		//��������
	CHAR				m_szJieBai[MAX_NICK_NAME];		//�������
	CHAR				m_szShiTu[MAX_NICK_NAME];		//ʦͽ�ƺ�
	CHAR				m_szShangDian[MAX_NICK_NAME];	//�̵�ƺ�

	BYTE				m_CurTitle;						//��ǰ�ƺ�����

	VOID	CleanUp()
	{
		memset(this,0,sizeof(_TITLE));
	}
};

struct	ITEM_PICK_CTL
{
	ObjID_t				OwnerID;			//���յ�ʰȡ��ID
	UINT				uBetTime;			//ϵͳ�Ĳ�ʱ��
	UCHAR				uMaxBetPoint;		//���Bet����
	PICK_RULER			ePickRuler;			//ϵͳ���Ʒ���


	ITEM_PICK_CTL()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		OwnerID			=	INVALID_ID;		//��������
		ePickRuler		=	IPR_FREE_PICK;	//����ʰȡ
		uBetTime		=	0;				//����ʰȡ
		uMaxBetPoint	=	0;
	}
};

typedef	ITEM_PICK_CTL	IPC;

#define MAX_PICKER_COUNT	6


//�����ܲ���ʰȡ����Ա�б�
struct TEAM_PICKER
{
	UINT		m_uCount;
	ObjID_t		m_PickerID[MAX_PICKER_COUNT];

	TEAM_PICKER()
	{
		memset(this,0,sizeof(TEAM_PICKER));
	}

	VOID	AddPicker(ObjID_t id)
	{
		for(UINT nIndex=0;nIndex<m_uCount;nIndex++)
		{
			if(m_PickerID[nIndex]==id)
				return;
		}
		m_PickerID[m_uCount] = id;
		m_uCount++;

	}


};




//����˺���¼
#define		MAX_DAMAGE_REC_COUNT	10


//�˺���¼
struct DAMAGE_RECORD
{
	ObjID_t		m_Killer;
	TeamID_t	m_TeamID;
	UINT		m_uDamage;
	
	DAMAGE_RECORD()
	{
		CleanUp();
	}
	void	CleanUp()
	{
		m_Killer	= INVALID_ID;
		m_TeamID	= INVALID_ID;
		m_uDamage	= 0;
	}
};



//�˺�����
struct DAMAGE_MEM_LIST
{
	UINT				m_uCount;
	DAMAGE_RECORD		m_DamageRec[MAX_DAMAGE_REC_COUNT];
	
	DAMAGE_MEM_LIST()
	{
		CleanUp();
	}

	void	CleanUp()
	{
		m_uCount	=	0;
		for(int i = 0;i<MAX_DAMAGE_REC_COUNT;i++)
			m_DamageRec[i].CleanUp();
	}
	
	void	AddMember(ObjID_t	KillerID,TeamID_t KillerTeam,UINT	Damage)
	{
		if(KillerTeam!=INVALID_ID)
		{
			m_DamageRec[m_uCount].m_Killer	=	INVALID_ID;
			m_DamageRec[m_uCount].m_TeamID	=	KillerTeam;
			m_DamageRec[m_uCount].m_uDamage	=	Damage;
		}
		else
		{
			m_DamageRec[m_uCount].m_Killer	=	KillerID;
			m_DamageRec[m_uCount].m_TeamID	=	INVALID_ID;
			m_DamageRec[m_uCount].m_uDamage	=	Damage;
		}

		m_uCount++;
	}

	void	AddMember(DAMAGE_RECORD&	dRec)
	{
		if(dRec.m_TeamID!=INVALID_ID)
		{
			m_DamageRec[m_uCount].m_Killer	=	INVALID_ID;
			m_DamageRec[m_uCount].m_TeamID	=	dRec.m_TeamID;
			m_DamageRec[m_uCount].m_uDamage	=	dRec.m_uDamage;
		}
		else
		{
			m_DamageRec[m_uCount].m_Killer	=	dRec.m_Killer;
			m_DamageRec[m_uCount].m_TeamID	=	INVALID_ID;
			m_DamageRec[m_uCount].m_uDamage	=	dRec.m_uDamage;
		}
		m_uCount++;
	}
	
	DAMAGE_RECORD*	FindMember(ObjID_t KillerID,TeamID_t KillerTeam)
	{
		for(UINT i =0;i<m_uCount;i++)
		{
			if((m_DamageRec[i].m_Killer == KillerID && KillerID!=INVALID_ID)||
				(m_DamageRec[i].m_TeamID == KillerTeam && KillerTeam!=INVALID_ID))
			{
					return &m_DamageRec[i];
					
			}
		}
		return NULL;
	}
	
	


};


struct _OWN_ABILITY 
{
	// AbilityID_t	m_Ability_ID; ����Ҫ ID���������� ID
	WORD		m_Level; // ���ܵȼ�
	WORD		m_Exp; // ����������
};


#define		MAX_MONSTER_DROP_TASK_ITEM		5
#define		MAX_MONSTER_KILLER_NUM			18

struct CHAR_OWNER_DROP_LIST 
{
	ObjID_t		HumanID;
	UINT		DropItemIndex[MAX_MONSTER_DROP_TASK_ITEM];
	UINT		DropCount;

	CHAR_OWNER_DROP_LIST()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		memset(this,0,sizeof(*this));
	}

	VOID	AddItem(UINT ItemIndex)
	{
		Assert(DropCount<MAX_MONSTER_DROP_TASK_ITEM);
		DropItemIndex[DropCount] = ItemIndex;
		DropCount++;
	}
};

struct MONSTER_OWNER_LIST 
{
	CHAR_OWNER_DROP_LIST	OwnerDropList[MAX_MONSTER_KILLER_NUM];
	UINT					OwnerCount;

	MONSTER_OWNER_LIST()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		memset(this,0,sizeof(*this));
	}

	VOID	AddItem(UINT uItemIndex,ObjID_t	HumanID)
	{
		for(UINT i =0;i<OwnerCount;i++)
		{
			if(OwnerDropList[i].HumanID	== HumanID)
			{
				OwnerDropList[i].AddItem(uItemIndex);
			}
		}
	}

	VOID	AddOwner(ObjID_t	HumanID)
	{
		OwnerDropList[OwnerCount].HumanID	=	HumanID;
		OwnerCount++;
	}
};



struct RELATION_MEMBER
{
	GUID_t			m_MemberGUID ;
	CHAR			m_szMemberName[MAX_CHARACTER_NAME] ;
	INT				m_nLevel;							//��ɫ�ȼ�
	INT				m_nMenPai;							//���� MENPAI_ATTRIBUTE
	INT				m_nPortrait;						// ͷ��
	GuildID_t		m_GuildID;							//���ID
	
	struct ReMember_ExtData
	{
		INT			m_nLevel;							//��ɫ�ȼ�
		INT			m_nMenPai;							//���� MENPAI_ATTRIBUTE
		INT			m_nPortrait;						//ͷ��
		GuildID_t	m_GuildID;							//���ID
	};

	RELATION_MEMBER( )
	{
		CleanUp( );
	};

	VOID CleanUp( )
	{
		m_MemberGUID = INVALID_ID;
		memset( m_szMemberName, 0, sizeof(m_szMemberName) );
		m_nLevel = 0;
		m_nMenPai = 9;
		m_nPortrait = -1;
		m_GuildID = INVALID_ID;
	};

	ReMember_ExtData	GetExtData()
	{
		ReMember_ExtData ExtData;
		
		ExtData.m_nLevel = m_nLevel;
		ExtData.m_nMenPai = m_nMenPai;
		ExtData.m_nPortrait = m_nPortrait;
		ExtData.m_GuildID = m_GuildID;
		return ExtData;
	}

	VOID			 SetExtData(ReMember_ExtData& ExtData)
	{
		m_nLevel = ExtData.m_nLevel;
		m_nMenPai = ExtData.m_nMenPai;
		m_nPortrait = ExtData.m_nPortrait;
		m_GuildID = ExtData.m_GuildID;
	}
};

struct MarriageInfo
{
	GUID_t		m_SpouseGUID;								// ��ż�� GUID
//	UINT		m_uWeddingTime;								// ����ʱ��

	MarriageInfo()
	{
		CleanUp();
	}

	VOID CleanUp()
	{
		m_SpouseGUID = INVALID_ID;
	}
};

struct PrenticeInfo
{
//	UINT		m_uRecruitingTime;							// ��ͽʱ��
	time_t		m_BetrayingTime;							// ���һ����ʦʱ��
	UINT		m_uMoralPoint;								// ʦ�µ�
	UCHAR		m_uPrenticeCount;							// ͽ������
	GUID_t		m_PrenticeGUID[MAX_PRENTICE_COUNT];			// ͽ�ܵ� GUID

	PrenticeInfo()
	{
		CleanUp();
	}

	VOID CleanUp()
	{
		m_BetrayingTime = 0;
		m_uMoralPoint = 0;
		m_uPrenticeCount = 0;

		for( INT i=0; i<MAX_PRENTICE_COUNT; ++i )
		{
			m_PrenticeGUID[i] = INVALID_ID;
		}
	}

};

struct MasterInfo
{
	GUID_t		m_MasterGUID;								// ʦ���� GUID
//	UINT		m_uApprenticingTime;						// ��ʦʱ��
//	UINT		m_uBetrayingTime;							// �ϴ���ʦʱ��
//	UINT		m_uBetrayTimes;								// ��ʦ����

	MasterInfo()
	{
		CleanUp();
	}

	VOID CleanUp()
	{
		m_MasterGUID = INVALID_ID;
	}

};

class SocketOutputStream ;
class SocketInputStream ;
//�ʼ�
struct MAIL
{



	struct MailInfo
	{
		GUID_t		m_GUID;								// ������ GUID
		BYTE		m_SourSize ;
		INT			m_nPortrait;						// ������ͷ��
		BYTE		m_DestSize ;
		WORD		m_ContexSize ;
		UINT		m_uFlag ;							//�ʼ���־ enum MAIL_TYPE
		time_t		m_uCreateTime ;						//�ʼ�����ʱ��
		CampID_t	m_Camp;
		//ִ���ʼ�Ӧ�ò���
		UINT		m_uParam0 ;
		UINT		m_uParam1 ;
		UINT		m_uParam2 ;
		UINT		m_uParam3 ;
	};

	VOID	GetMailInfo(MailInfo& mInfo)
	{
		mInfo.m_GUID		= m_GUID;
		mInfo.m_SourSize	= m_SourSize;
		mInfo.m_nPortrait	= m_nPortrait;
		mInfo.m_DestSize	= m_DestSize;
		mInfo.m_ContexSize	= m_ContexSize;
		mInfo.m_uFlag		= m_uFlag;
		mInfo.m_uCreateTime	= m_uCreateTime;
		mInfo.m_uParam0		= m_uParam0;
		mInfo.m_uParam1		= m_uParam1;
		mInfo.m_uParam2		= m_uParam2;
		mInfo.m_uParam3		= m_uParam3;
		mInfo.m_Camp		= m_Camp;
	}

	VOID	SetMailInfo(MailInfo& mInfo)
	{
		m_GUID			= 	mInfo.m_GUID;
		m_SourSize		= 	mInfo.m_SourSize;
		m_nPortrait		= 	mInfo.m_nPortrait;
		m_DestSize		= 	mInfo.m_DestSize;
		m_ContexSize	= 	mInfo.m_ContexSize;
		m_uFlag			= 	mInfo.m_uFlag;
		m_uCreateTime	= 	mInfo.m_uCreateTime;
		m_uParam0		= 	mInfo.m_uParam0;
		m_uParam1		= 	mInfo.m_uParam1;
		m_uParam2		= 	mInfo.m_uParam2;
		m_uParam3		= 	mInfo.m_uParam3;
		m_Camp			=	mInfo.m_Camp;
	}
	GUID_t		m_GUID;								// ������ GUID
	BYTE		m_SourSize ;
	CHAR		m_szSourName[MAX_CHARACTER_NAME] ;	//������

	INT			m_nPortrait;						// ������ͷ��

	BYTE		m_DestSize ;
	CHAR		m_szDestName[MAX_CHARACTER_NAME] ;	//������

	WORD		m_ContexSize ;
	CHAR		m_szContex[MAX_MAIL_CONTEX] ;		//����

	UINT		m_uFlag ;							//�ʼ���־ enum MAIL_TYPE
	time_t		m_uCreateTime ;						//�ʼ�����ʱ��

	CampID_t	m_Camp;								//��������Ӫ

	//ִ���ʼ�Ӧ�ò���
	UINT		m_uParam0 ;
	UINT		m_uParam1 ;
	UINT		m_uParam2 ;
	UINT		m_uParam3 ;

	MAIL( )
	{
		CleanUp( ) ;
	};
	VOID CleanUp( )
	{
		m_GUID = INVALID_INDEX;
		m_SourSize = 0 ;
		memset( m_szSourName, 0, sizeof(CHAR)*MAX_CHARACTER_NAME ) ;

		m_nPortrait = -1;

		m_DestSize = 0 ;
		memset( m_szDestName, 0, sizeof(CHAR)*MAX_CHARACTER_NAME ) ;

		m_ContexSize = 0 ;
		memset( m_szContex, 0, sizeof(CHAR)*MAX_MAIL_CONTEX ) ;

		m_uFlag = MAIL_TYPE_NORMAL ;
		m_uCreateTime = 0 ;

		m_uParam0 = 0 ;
		m_uParam1 = 0 ;
		m_uParam2 = 0 ;
		m_uParam3 = 0 ;
	};
	VOID Read( SocketInputStream& iStream ) ;
	VOID Write( SocketOutputStream& oStream ) const ;
};


#define MAX_MAIL_SIZE 20
struct MAIL_LIST
{
	MAIL		m_aMail[MAX_MAIL_SIZE] ;
	BYTE		m_Count ;//�ʼ�����
	BYTE		m_TotalLeft ;//�û��ʺ�����ʼ�ʣ������

	MAIL_LIST( )
	{
		CleanUp( ) ;
	};
	VOID CleanUp( )
	{
		m_Count = 0 ;
		m_TotalLeft = 0 ;
		for( INT i=0;i<MAX_MAIL_SIZE; i++ )
		{
			m_aMail[i].CleanUp() ;
		}
	};
	VOID Read( SocketInputStream& iStream ) ;
	VOID Write( SocketOutputStream& oStream ) const ;
};

// �����ʼ���ָ���͸���ͬ�˵�ͬ�����ʼ�
#define MAX_RECEIVER 100
struct BATCH_MAIL
{
	GUID_t				m_GUID;								// GUID

	BYTE				m_SourSize;
	CHAR				m_szSourName[MAX_CHARACTER_NAME];	//������

	BYTE				m_ReceiverCount;					//����������
	struct
	{
		BYTE			m_DestSize;
		CHAR			m_szDestName[MAX_CHARACTER_NAME];	//������
	}m_Receivers[MAX_RECEIVER];

	WORD				m_ContentSize;
	CHAR				m_szContent[MAX_MAIL_CONTEX];		//����

	UCHAR				m_uFlag;							//�ʼ���־ enum MAIL_TYPE
	time_t				m_uCreateTime;						//�ʼ�����ʱ��

	CampID_t			m_Camp;								//�����ߵ���Ӫ

	BATCH_MAIL()		{ CleanUp(); }

	GUID_t				GetGUID( )
	{
		return m_GUID;
	}
	VOID				SetGUID( GUID_t guid )
	{
		m_GUID = guid;
	}

	const CHAR*			GetSourName()
	{
		return m_szSourName;
	}
	VOID				SetSourName( const CHAR* szName )
	{
		strncpy(m_szSourName, szName, MAX_CHARACTER_NAME - 1);
		m_SourSize = (UCHAR)strlen(m_szSourName);
	}

	BYTE				GetReceiverCount()
	{
		return m_ReceiverCount;
	}
	const CHAR*			GetDestName(BYTE idx)
	{
		if( idx >= m_ReceiverCount )
		{
			Assert( idx );
			return NULL;
		}

		return m_Receivers[idx].m_szDestName;
	}
	VOID				AddDestName( const CHAR* szName )
	{
		strncpy(m_Receivers[m_ReceiverCount].m_szDestName, szName, MAX_CHARACTER_NAME - 1);
		m_Receivers[m_ReceiverCount].m_DestSize = (UCHAR)strlen(m_Receivers[m_ReceiverCount].m_szDestName);
		++m_ReceiverCount;
	}

	const CHAR*			GetMailContent()
	{
		return m_szContent;
	}
	VOID				SetMailContent( const CHAR* szContent )
	{
		strncpy(m_szContent, szContent, MAX_MAIL_CONTEX - 1);
		m_ContentSize = (UCHAR)strlen(m_szContent);
	}

	UCHAR				GetMailFlag()				
	{
		return m_uFlag;
	}
	VOID				SetMailFlag(UCHAR uFlag)
	{
		m_uFlag = uFlag;
	}

	time_t				GetCreateTime()
	{
		return m_uCreateTime;
	}
	VOID				SetCreateTime(time_t uCreateTime)
	{
		m_uCreateTime = uCreateTime;
	}

	CampID_t			GetSourCamp()
	{
		return m_Camp;
	}
	VOID				SetSourCamp(CampID_t id)
	{
		m_Camp = id;
	}

	VOID				CleanUp();
	UINT				GetSize() const;
	VOID				Read( SocketInputStream& iStream );
	VOID				Write( SocketOutputStream& oStream ) const;
};


struct USER_SIMPLE_DATA
{
	CHAR				m_Account[MAX_CHARACTER_NAME];	// ���û����˻�����
	GUID_t				m_AccountGUID;					// ����GUID
	CHAR				m_Name[MAX_CHARACTER_NAME];		// ���û��Ľ�ɫ����
	GUID_t				m_GUID;							// ���û���Ψһ��
	UINT				m_uMenPai;						// ����
	INT					m_nPortrait;					// ͷ��
	UCHAR				m_uFaceMeshID;					// ����ģ��
	UCHAR				m_uHairMeshID;					// ͷ��ģ��
	UINT				m_uHairColor;					// ��ɫ
	INT					m_nLevel;						// ����
	USHORT				m_uSex;							// �Ա�
	CHAR				m_szTitle[MAX_CHARACTER_TITLE];	// �ƺ�
	GuildID_t			m_GuildID;						// ��� ID

	USER_SIMPLE_DATA( )
	{
		CleanUp( ) ;
	}
	VOID CleanUp( )
	{
		memset( m_Name, 0, sizeof(m_Name) );
		memset( m_Account, 0, sizeof(m_Account) );
		m_GUID = INVALID_ID;
		m_AccountGUID = INVALID_ID;
		m_uMenPai = 9;
		m_nPortrait = -1;
		m_nLevel = 0;
		m_uSex = 0;
		memset( m_szTitle, 0, sizeof(m_szTitle) );
		m_GuildID = INVALID_ID;
	}
};

#define		MAX_SQL_LENGTH				4096
#define		MAX_LONG_SQL_LENGTH			204800

struct DB_QUERY
{
	UCHAR	m_SqlStr[MAX_SQL_LENGTH];		//ִ�е�Sql���
	
	VOID	Clear()
	{
		memset(m_SqlStr,0,MAX_SQL_LENGTH);
	}

	VOID	Parse(const CHAR* pTemplate,...);
	
};

struct LONG_DB_QUERY
{
	UCHAR	m_SqlStr[MAX_LONG_SQL_LENGTH];		//ִ�е�Sql���

	VOID	Clear()
	{
		memset(m_SqlStr,0,MAX_LONG_SQL_LENGTH);
	}

	VOID	Parse(const CHAR* pTemplate,...);
};

struct DB_CHAR_EQUIP_LIST
{
	DB_CHAR_EQUIP_LIST()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		memset(this,0,sizeof(*this));
	}
	UINT		m_Equip[HEQUIP_NUMBER];			//װ��
};

struct DB_CHAR_BASE_INFO
{

	DB_CHAR_BASE_INFO()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		memset(this,0,sizeof(*this));
	}

	GUID_t				m_GUID;							//��ɫȫ�ֱ��
	BOOL				m_Sex;							//�Ա�
	CHAR				m_Name[MAX_CHARACTER_NAME];		//��ɫ����
	INT					m_Level;						//��ɫ�ȼ�
	UINT				m_HairColor;					//ͷ����ɫ	
	BYTE				m_FaceColor;					//������ɫ
	BYTE				m_HairModel;					//ͷ��ģ��
	BYTE				m_FaceModel;					//����ģ��
	SceneID_t			m_StartScene;					//��ɫ���ڳ���
	INT					m_Menpai;						//��ɫ����
	INT					m_HeadID;						//ͷ�����
	CampID_t			m_Camp;							//��Ӫ���
	DB_CHAR_EQUIP_LIST	m_EquipList;					//װ���б�
};

// ��Ա�б��еĶ�Ա��Ϣ��
struct TEAM_LIST_ENTRY
{
	GUID_t				m_GUID;							// GUID
	SceneID_t			m_SceneID;						// ����ID
	UINT				m_ExtraID;						// ��Ա�� PlayerID(WG) �� ObjID(GC)
	UCHAR				m_NameSize;						// ��������
	CHAR				m_Name[MAX_CHARACTER_NAME];		// ��Ա������
	INT					m_nPortrait;					// ͷ��
	USHORT				m_uDataID;						// ��Ա���Ա�

	TEAM_LIST_ENTRY( )
	{
		CleanUp( );
	};

	VOID				CleanUp( )
	{
		m_GUID			= INVALID_ID;
		m_SceneID		= INVALID_ID;
		m_ExtraID		= INVALID_ID;
		m_NameSize		= 0;
		memset			(m_Name, 0, sizeof(m_Name));
		m_nPortrait		= -1;
		m_uDataID		= 0;
	};

	TEAM_LIST_ENTRY&	operator= ( const TEAM_LIST_ENTRY& entry )
	{
		m_GUID			= entry.m_GUID;
		m_SceneID		= entry.m_SceneID;
		m_ExtraID		= entry.m_ExtraID;
		m_NameSize		= entry.m_NameSize;
		strncpy			( m_Name, entry.m_Name, sizeof(m_Name) - 1 );
		m_nPortrait		= entry.m_nPortrait;
		m_uDataID		= entry.m_uDataID;
		return			*this;
	}

	VOID				SetGUID( GUID_t guid ) { m_GUID = guid; }
	GUID_t				GetGUID( ) const { return m_GUID; }

	VOID				SetSceneID( SceneID_t SceneID ) { m_SceneID = SceneID; }
	SceneID_t			GetSceneID( ) const { return m_SceneID; }

	VOID				SetExtraID( UINT id ) { m_ExtraID = id; }
	UINT				GetExtraID( ) const { return m_ExtraID; }

	VOID				SetName( const CHAR* pName )
	{
		strncpy			( m_Name, pName, MAX_CHARACTER_NAME-1 );
		m_NameSize		= (UCHAR)strlen(m_Name);
	}
	const CHAR*			GetName( ) const { return m_Name; }

	VOID				SetIcon( INT icon ) { m_nPortrait = icon; }
	INT					GetIcon( ) const { return m_nPortrait; }

	VOID				SetDataID(USHORT dataid) { m_uDataID = dataid; }
	USHORT				GetDataID() const { return m_uDataID; }

	UINT				GetSize() const;
	VOID				Read( SocketInputStream& iStream );
	VOID				Write( SocketOutputStream& oStream ) const;
};

//ID List
typedef struct _ObjID_List
{
	enum
	{
		MAX_LIST_SIZE = 512,
	};
	_ObjID_List()
	{
		CleanUp();
	}
	VOID CleanUp(VOID)
	{
		m_nCount=0;
		memset((VOID*)m_aIDs, INVALID_ID, sizeof(m_aIDs));
	}
	INT m_nCount;
	ObjID_t m_aIDs[MAX_LIST_SIZE];
} ObjID_List;

//����̵��ΨһID
struct _PLAYERSHOP_GUID
{
	ID_t	m_World ;		//�����: 
	ID_t	m_Server ;		//����˳���ţ�
	ID_t	m_Scene ;		//������
	INT		m_PoolPos ;		//���ݳ�λ��

	_PLAYERSHOP_GUID()
	{
		Reset();
	}
	_PLAYERSHOP_GUID& operator=(_PLAYERSHOP_GUID const& rhs)
	{
		m_PoolPos	= rhs.m_PoolPos;
		m_Server	= rhs.m_Server;
		m_World		= rhs.m_World;
		m_Scene		= rhs.m_Scene;
		return *this;
	}
	BOOL	operator ==(_PLAYERSHOP_GUID& Ref)	const
	{
		return (Ref.m_Scene==m_Scene)&&(Ref.m_PoolPos==m_PoolPos)&&(Ref.m_Server==m_Server)&&(Ref.m_World==m_World);
	}
	BOOL	isNull() const
	{
		return (m_Scene ==INVALID_ID)&&(m_World ==INVALID_ID)&&(m_PoolPos==-1)&&(m_Server == INVALID_ID);
	}
	VOID	Reset()
	{
		m_PoolPos = -1;
		m_Server = INVALID_ID;
		m_World	 = INVALID_ID;	
		m_Scene	 = INVALID_ID;
	}
};

enum  SM_COMMANDS
{
	CMD_UNKNOW,
	CMD_SAVE_ALL,
	CMD_CLEAR_ALL,

};


struct SM_COMMANDS_STATE
{
	SM_COMMANDS	cmdType;	
	union 
	{
		INT		iParam[6];
		FLOAT	fParam[6];
		CHAR	cParam[24];
	};
};


struct GLOBAL_CONFIG
{
	GLOBAL_CONFIG()
	{
		Commands.cmdType = CMD_UNKNOW;
	}
	SM_COMMANDS_STATE		Commands;
};

//������ļ����õ�ǰ��Ķ���
#include "GameStruct_Item.h"
#include "GameStruct_Skill.h"
#include "GameStruct_Scene.h"

#include "GameStruct_Relation.h"
#include "GameStruct_Guild.h"
#include "GameStruct_City.h"
#include "GameStruct_Script.h"

#include "GameStruct_MinorPasswd.h"
#include "GameStruct_Finger.h"

#endif
