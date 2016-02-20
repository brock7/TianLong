
#ifndef __GAMETABLE_H__
#define __GAMETABLE_H__

#include "Type.h"
#include "DB_Struct.h"
#include "PetConfig.h"
#include "DataRecords.h"
#include "CampAndStand.h"
#include "TemplateManager.h"
#include "FileDataMgr.h"
#include "GameStruct_Pet.h"
#include <map>
#include <vector>
using namespace std;

class AIScript;

//����ϵ����
struct _BASE_VALUE_TBL
{
	//����Ӱ����������*��������
	INT		m_Table[AINFOTYPE_NUMBER][MATTRIBUTE_NUMBER] ;

	_BASE_VALUE_TBL( )
	{
		memset( m_Table, 0, sizeof(INT)*AINFOTYPE_NUMBER*MATTRIBUTE_NUMBER ) ;
	};
	INT		Get( UINT AttrInfo, UINT MenPai ){
		Assert( AttrInfo<AINFOTYPE_NUMBER && MenPai<MATTRIBUTE_NUMBER ) ;
		return m_Table[AttrInfo][MenPai] ;
	};
};
extern _BASE_VALUE_TBL g_BaseValueTbl ;


struct MONSTER_EXT_ATTR
{
	BOOL	m_Used;
	INT		m_Type;
	CHAR	m_Name[NPC_NAME_LEN];
	INT		m_CannotFight;
	INT		m_Level;
	INT		m_BaseExp;
	INT		m_OutLook;
	INT		m_BaseAI;
	INT		m_ExtAI;
	INT		m_Camp;
	FLOAT	m_DropSearchRange;
	INT		m_SearchTeamCount;
	INT		m_MinDamagePercent;
	INT		m_CanHandle;
	INT		m_FriendValue;
	INT		m_WalkSpeed;
	INT		m_BossFlag;
	INT		m_SnapSkillImmID;		//˲����������ID
	INT		m_DurationSkillImmID;	//������������ID
	INT		m_AttackAnimTime;		//��������ʱ��
	INT		m_AttackCooldownTime;	//������ȴʱ��
};

//����������Ա�
struct _MONSTER_ATTREX_TBL
{
	INT						m_Table[MAXTYPE_NUMBER][CATTR_LEVEL2_NUMBER] ;
	MONSTER_EXT_ATTR		m_TableExt[MAXTYPE_NUMBER];
	UINT						m_Count ;
	UINT						m_MaxType;

	_MONSTER_ATTREX_TBL()
	{
		memset( m_Table, 0, sizeof(INT)*MAXTYPE_NUMBER*CATTR_LEVEL2_NUMBER ) ;
		memset(m_TableExt,0,sizeof(MONSTER_EXT_ATTR)*MAXTYPE_NUMBER);
		m_Count = 0 ;
	};
	INT			Get( UINT MonsterType, UINT AttrEx )
	{
			Assert( MonsterType<=m_MaxType);
			Assert(AttrEx<CATTR_LEVEL2_NUMBER);
			Assert(m_TableExt[MonsterType].m_Used==TRUE);
			return m_Table[MonsterType][AttrEx] ;
	};

	MONSTER_EXT_ATTR	*GetExtAttr(UINT MonsterType)
	{
		Assert(MonsterType<=m_MaxType);
		Assert(m_TableExt[MonsterType].m_Used==TRUE);
		return	&m_TableExt[MonsterType];
	}
	INT		Count( ){ return m_Count ; } ;
};
extern _MONSTER_ATTREX_TBL g_MonsterAttrExTbl ;

// NPC����˵��������
struct _NPC_PAOPAO_INDEX_TBL
{
	INT m_nBaseIndex; // NPCPaoPao��ʼ����ID
	INT m_nIndexStep; // NPCPaoPao����ID��Ӧ����Ч��Χ
};
extern _NPC_PAOPAO_INDEX_TBL g_NPCPaoPaoIndexTbl[MAXTYPE_NUMBER];

//�������Ա�
struct _PET_ATTR_TBL
{
	PET_ATTR					m_TableExt[MAXTYPE_NUMBER];
	UINT						m_Count ;
	UINT						m_MaxType;

	_PET_ATTR_TBL()
	{
		memset(m_TableExt,0,sizeof(PET_ATTR)*MAXTYPE_NUMBER);
		m_Count = 0 ;
	};

	PET_ATTR	*GetAttr(UINT PetType)
	{
		if (PetType<=m_MaxType)
			return	&m_TableExt[PetType];
		return NULL;
	}
	INT		Count( ){ return m_Count ; } ;
};
extern _PET_ATTR_TBL g_PetAttrTbl ;

//�������Ե���䷽����
#define PET_ATTRPOINT_DIS_NUM (1024)
struct _PET_ATTRPOINT_DIS_TBL
{
	struct _PET_ATTRPOINT_DIS
	{
		INT			m_ID;
		INT			m_StrRate;
		INT			m_ConRate;
		INT			m_SprRate;
		INT			m_DexRate;
		INT			m_IntRate;
	};

	_PET_ATTRPOINT_DIS	m_Table[PET_ATTRPOINT_DIS_NUM];
    INT					m_Count;				//��������

	_PET_ATTRPOINT_DIS_TBL()
	{
		m_Count = 0;
	}

	// nType���������������Եȡ����� nID��������
	INT GetAttrDisRate(INT nType, INT nID)
	{
		Assert(nID < m_Count);
		Assert(nType < CATTR_LEVEL1_NUMBER);
		INT nRet;

		switch(nType)
		{
		case CATTR_LEVEL1_STR:
			nRet = m_Table[nID].m_StrRate;
			break;
		case CATTR_LEVEL1_CON:
			nRet = m_Table[nID].m_ConRate;
			break;
		case CATTR_LEVEL1_SPR:
			nRet = m_Table[nID].m_SprRate;
			break;
		case CATTR_LEVEL1_DEX:
			nRet = m_Table[nID].m_DexRate;
			break;
		case CATTR_LEVEL1_INT:
			nRet = m_Table[nID].m_IntRate;
			break;
		default:
			break;
		}
		return nRet;		
	}
};
extern _PET_ATTRPOINT_DIS_TBL g_PetAttrPointDisTbl;

// ����������
#define PET_LEVEL_NUM (105)
struct _PET_LEVELUP_TBL
{
	INT m_Table[PET_LEVEL_NUM];
	
	_PET_LEVELUP_TBL()
	{
		memset(m_Table, INVALID_ID, PET_LEVEL_NUM);
	}

	INT GetExp(INT nLevel)
	{
		Assert(nLevel <= PET_LEVEL_NUM);
		return m_Table[nLevel];
	}
};
extern _PET_LEVELUP_TBL g_PetLevelUpTbl;

// ����������
struct _PET_DOMESTICATION_MONEY_TBL
{
	FLOAT m_Table[PET_LEVEL_NUM];
	
	_PET_DOMESTICATION_MONEY_TBL()
	{
		memset(m_Table, 0, PET_LEVEL_NUM);
	}

	FLOAT GetDomesticationMoney(INT nLevel)
	{
		Assert(nLevel <= PET_LEVEL_NUM);
		return m_Table[nLevel];
	}
};
extern _PET_DOMESTICATION_MONEY_TBL g_PetDomesticationMoneyTbl;

// ���＼�����ɱ�
#define PET_MAX_NUM	(1024)
struct _PET_SKILLDISTR_TBL
{
	struct PET_SKILL_MAP
	{
		INT		m_nSkillIndex;
		INT		m_nSkillRange;

		PET_SKILL_MAP( )
		{
			m_nSkillIndex = -1;
			m_nSkillRange = 0;
		}
	};

	PET_SKILL_MAP	m_Table[PET_MAX_SKILL_COUNT - 1];
	INT				m_PetID;

};
extern _PET_SKILLDISTR_TBL g_PetSkillDistrTbl[PET_MAX_NUM];

// ���＼��������
#define PET_SKILL_NUM (512)
struct _PET_SKILLINDEX_TBL
{
	INT		m_nIndex;
	INT		m_nSkillID;
};
extern _PET_SKILLINDEX_TBL g_PetSkillIndexTbl[PET_SKILL_NUM];

// ����ѧϰ���ܼ��ʱ�
struct _PET_STUDYSKILL
{
	INT		m_nID;				// ID
	INT		m_nMaxSpaceCount;	// �ܿո�����
	INT		m_nSpaceCount;		// ��ǰ�ո�����
	INT		m_nNewPercent;		// �����¸��ϵİٷֱ�
};

#define MAKE_PET_STUDYSKILL_ID( x, y )	(x * 10 + y)
#define PET_STUDYSKILLRATE_NUM (56)
struct _PET_STUDYSKILL_TBL
{
	_PET_STUDYSKILL		m_aStudySkill[PET_STUDYSKILLRATE_NUM];

	const _PET_STUDYSKILL *Get( INT nMaxSpaceCount, INT nSpaceCount )const
	{
		INT nID = MAKE_PET_STUDYSKILL_ID( nMaxSpaceCount, nSpaceCount );
		if ( nID < PET_STUDYSKILLRATE_NUM )
		{
			return &m_aStudySkill[nID];
		}
		else
		{
			return NULL;
		}
	}

	_PET_STUDYSKILL_TBL( VOID )
	{
		memset( m_aStudySkill, 0, sizeof( m_aStudySkill ) );
	}
};
extern _PET_STUDYSKILL_TBL g_PetStudySkillTbl;

// ���������ܼ��ʱ�
#define PET_APPERCEIVESKILLRATE_NUM (24)
struct _PET_APPERCEIVESKILL_TBL
{
	INT		m_nSkillCount;					//���＼������
	INT		m_nLearnedVoluntarySkillCount;	//��������������
	INT		m_nLearnedPassiveSkillCount;	//���б���������
	INT		m_nApperceiveVoluntarySkillRate;//�����������ܸ��ʣ���λ1/10w��
	INT		m_nApperceivePassiveSkillRate;	//���򱻶����ܸ��ʣ���λ1/10w��

};
extern _PET_APPERCEIVESKILL_TBL g_PetApperceiveSkillTbl[PET_APPERCEIVESKILLRATE_NUM];

#define PET_AITYPE (5)
#define PET_SKILL_TYPE (5)
extern FLOAT g_PetAIStrategyTble[PET_SKILL_TYPE][PET_AITYPE];

///////////////////////////////////////////////////////////////////////////////////
//����AI����
struct _MONSTER_AI_TBL
{
	INT		m_Table[MAXAI_NUMBER][AIPARAM_NUMBER] ;
	INT		m_Count ;

	_MONSTER_AI_TBL( )
	{
		memset( m_Table, 0, sizeof(INT)*MAXAI_NUMBER*AIPARAM_NUMBER ) ;
		m_Count = 0 ;
	};
	INT		Get( UINT AIType, UINT AIParam )
	{
		Assert( AIType<(UINT)m_Count && AIParam<AIPARAM_NUMBER ) ;
		return m_Table[AIType][AIParam] ;
	};
	INT		Count( ){ return m_Count ; } ;
};
extern _MONSTER_AI_TBL g_MonsterAITbl ;

//��������˵���ṹ --�Ѿ�����
struct _MONSTER_SPEAK_TBL
{
	struct MonsterSpeakItem
	{
		INT nSpeakID;
		CHAR szSpeakContent[MAX_MONSTERSPEAK_CONTENT];
	};
	MonsterSpeakItem	m_Table[MAXCHAT_NUMBER] ;	//��¼���е�̸����ǣ���1001��2050������
	INT		m_NumberOfType[MAXCHATTYPE_NUMBER] ;	//ÿһ��ֻ�Ǽ�¼ÿ��̸�����͵ĸ���
    INT 	m_TypeCount ;							//��¼ʵ�ʵ�̸��������
	INT 	m_Count ;								//��¼ʵ�ʵ�̸����

	_MONSTER_SPEAK_TBL( )
	{
		memset( m_NumberOfType, 0, sizeof(INT)*MAXCHATTYPE_NUMBER ) ;
		memset( m_Table, 0, sizeof(MonsterSpeakItem)*MAXCHAT_NUMBER ) ;
		m_TypeCount = 0 ;
		m_Count = 0 ;

	};
	CHAR*	Get( UINT ChatType ,INT Index = -1 )
	{//������������ѡȡ��¼��Ҳ����ѡȡָ���ļ�¼
		if( -1 != Index && 0 < Index && m_Count > Index )
			return m_Table[Index].szSpeakContent;

		Assert( ChatType<(UINT)m_TypeCount && ChatType>-1) ;
		if ( ChatType<(UINT)m_TypeCount && ChatType>-1 )
			return "error";

		INT Offset=0 ;
		for(UINT i=0; i<ChatType; i++)
		{
			Offset += m_NumberOfType[i] ;
		}
		INT index = rand() % (m_NumberOfType[ChatType]+1) + Offset ;
		Assert(index<m_Count) ;
		
		return m_Table[index].szSpeakContent ;
	};
	INT		Count( ){ return m_Count ; } ;
	INT		TypeCount( ){ return m_TypeCount ; } ;
};
extern _MONSTER_SPEAK_TBL g_MonsterSpeakTbl ;

//һ������ֵ������
struct _ATTR_LEVELUP_TBL
{
	INT		m_Table[CATTR_LEVEL1_NUMBER][MATTRIBUTE_NUMBER][MAX_TABLE_DEFINE_LEVEL+1] ;

	_ATTR_LEVELUP_TBL( )
	{
		memset( m_Table, 0, sizeof(INT)*CATTR_LEVEL1_NUMBER*MATTRIBUTE_NUMBER*MAX_TABLE_DEFINE_LEVEL ) ;
	}
	INT		Get( UINT Attr, UINT MenPai ,UINT iLevel){
		Assert( Attr<CATTR_LEVEL1_NUMBER && MenPai<MATTRIBUTE_NUMBER && iLevel<=MAX_TABLE_DEFINE_LEVEL ) ;
		return m_Table[Attr][MenPai][iLevel] ;
	};
	VOID	Set( UINT Attr, UINT MenPai ,UINT iLevel,INT iValue){
		Assert( Attr<CATTR_LEVEL1_NUMBER && MenPai<MATTRIBUTE_NUMBER && iLevel<=MAX_TABLE_DEFINE_LEVEL ) ;
		m_Table[Attr][MenPai][iLevel] = iValue;	
	}
};
extern _ATTR_LEVELUP_TBL g_AttrLevelupTbl ;

//����ֵ��
struct _LEVEL_EXP_TBL {

	UINT		m_Table[MAX_PLAYER_EXP_LEVEL][2];

	_LEVEL_EXP_TBL()
	{
		memset(m_Table,0,sizeof(UINT)*MAX_PLAYER_EXP_LEVEL*2);
	}

	INT Get(UINT Level){
		Assert(Level<MAX_PLAYER_EXP_LEVEL);
		Assert(Level>=0);
		return m_Table[Level][1];
	}

};
extern	_LEVEL_EXP_TBL	g_LevelExpTbl;

//�ķ�ѧϰ���ı�
//struct _XINFA_UPLEVEL_SPEND_TBL {
//	INT		m_Table[MAX_ALL_XINFA_NUM][MAX_XINFA_LEVEL_NUM][3];
//
//	_XINFA_UPLEVEL_SPEND_TBL()
//	{
//		memset(m_Table,0,sizeof(UINT)*MAX_ALL_XINFA_NUM*MAX_XINFA_LEVEL_NUM*3);
//	}
//	INT GetMoney(UINT XinfaId, UINT Level)
//	{
//		Assert(XinfaId<MAX_ALL_XINFA_NUM);
//		Assert(XinfaId>=0);
//		Assert(Level<MAX_PLAYER_EXP_LEVEL);
//		Assert(Level>=0);
//		return m_Table[XinfaId][Level-1][1];
//	}
//
//	INT GetExp(UINT XinfaId, UINT Level)
//	{
//		Assert(XinfaId<MAX_ALL_XINFA_NUM);
//		Assert(XinfaId>=0);
//		Assert(Level<MAX_PLAYER_EXP_LEVEL);
//		Assert(Level>=0);
//		return m_Table[XinfaId][Level-1][2];
//	}
//};
//extern	_XINFA_UPLEVEL_SPEND_TBL	g_XinfaUpLevelSpendTbl;

struct _EXP_ATT_TBL {

	FLOAT	 m_Table[MAX_100_PLUS_DELTA_LEVEL];

	_EXP_ATT_TBL()
	{
		memset(m_Table,0,sizeof(FLOAT)*MAX_100_PLUS_DELTA_LEVEL);
		
	}

	FLOAT Get(INT DeltaLevel)
	{
		Assert(DeltaLevel<=100);
		Assert(DeltaLevel>=-100);

		return m_Table[DeltaLevel+100];
	}
};


extern _EXP_ATT_TBL	g_ExpAttTbl;


struct _GROW_POINT_INFO
{
	INT				 m_GrowPointIndex;
	INT				 m_OpenTime;
	WORD			 m_OpenAbility;
	ScriptID_t		 m_ScriptID;
	INT				 m_OpenAnim;
	
};


struct _GROW_POINT_INFO_TBL
{

	_GROW_POINT_INFO	*m_Table;

	INT					m_Count;

	_GROW_POINT_INFO_TBL()
	{
		m_Table	=	0;
		m_Count	=	0;
	}

	_GROW_POINT_INFO*	Get(INT GrowType);
	
};

extern	_GROW_POINT_INFO_TBL g_GrowPointInfoTbl;


struct _PLANTS_INFO 
{
	INT			 m_PlantIndex;	
	INT			 m_PlantType;
	INT			 m_Product;

};

struct _PLANTS_INFO_TBL
{

	_PLANTS_INFO*		m_Table;
	INT					m_Count;

	_PLANTS_INFO_TBL()
	{
		m_Table	=	0;
		m_Count	=	0;
	}

};

extern	_PLANTS_INFO_TBL g_PlantInfoTbl;

//���
#define QUES_CON_SIZE 256
#define OPT_CON_SIZE 64
struct _QUESTION_DATA
{
	INT					m_nIndex ;
	CHAR				m_szContex[QUES_CON_SIZE] ;
	CHAR				m_szOption0[OPT_CON_SIZE] ;
	CHAR				m_szOption1[OPT_CON_SIZE] ;
	CHAR				m_szOption2[OPT_CON_SIZE] ;
	CHAR				m_szOption3[OPT_CON_SIZE] ;
	CHAR				m_szOption4[OPT_CON_SIZE] ;
	CHAR				m_szOption5[OPT_CON_SIZE] ;
	BYTE				m_Key0 ;
	BYTE				m_Key1 ;
	BYTE				m_Key2 ;
	BYTE				m_Key3 ;
	BYTE				m_Key4 ;
	BYTE				m_Key5 ;

	_QUESTION_DATA( )
	{
		m_nIndex = INVALID_INDEX ;
		memset( m_szContex, 0, QUES_CON_SIZE ) ;
		memset( m_szOption0, 0, OPT_CON_SIZE ) ;
		memset( m_szOption1, 0, OPT_CON_SIZE ) ;
		memset( m_szOption2, 0, OPT_CON_SIZE ) ;
		memset( m_szOption3, 0, OPT_CON_SIZE ) ;
		memset( m_szOption4, 0, OPT_CON_SIZE ) ;
		memset( m_szOption5, 0, OPT_CON_SIZE ) ;
		m_Key0 = INVALID_INDEX ;
		m_Key1 = INVALID_INDEX ;
		m_Key2 = INVALID_INDEX ;
		m_Key3 = INVALID_INDEX ;
		m_Key4 = INVALID_INDEX ;
		m_Key5 = INVALID_INDEX ;
	};
};

#define MAX_QUESTIONS 4096
struct _QUESTION_LIB
{
	_QUESTION_DATA		m_aQuestions[MAX_QUESTIONS] ;
	INT					m_Count ;

	_QUESTION_LIB( )
	{
		m_Count = 0 ;
	};
};

extern _QUESTION_LIB g_QuestionLib ;

#define MAX_IP_RANGE 256
struct _IPRANGE
{
	INT			m_EnableCount ;
	CHAR		m_aaIPEnable[MAX_IP_RANGE][IP_SIZE] ;
	CHAR		m_aaMaskEnable[MAX_IP_RANGE][IP_SIZE] ;
	INT			m_DisableCount ;
	CHAR		m_aaIPDisable[MAX_IP_RANGE][IP_SIZE] ;
	CHAR		m_aaMaskDisable[MAX_IP_RANGE][IP_SIZE] ;

	_IPRANGE( )
	{
		m_EnableCount = 0 ;
	}
};

extern _IPRANGE g_IPRange ;

struct _LEVEL_MAX_MONEY
{
	UINT	m_Table[MAX_PLAYER_EXP_LEVEL][2];

	_LEVEL_MAX_MONEY()
	{
		memset(m_Table,0,sizeof(UINT)*MAX_PLAYER_EXP_LEVEL*2);
	}

	INT Get(UINT Level){
		Assert(Level<=MAX_PLAYER_EXP_LEVEL);
		Assert(Level>=0);
		return m_Table[Level-1][1];
	}
};
extern _LEVEL_MAX_MONEY g_LevelMoneyTbl;

struct 	CITY_SCENE_ENTRY
{
	INT			m_PortID;					//��ڳ���ID
	CHAR		m_szSceneFile[MAX_FILE_PATH];	//scene�ļ���
};

struct _CITY_SCENE
{
	CITY_SCENE_ENTRY	m_Table[MAX_SCENE];

	_CITY_SCENE()
	{
		memset(m_Table,0,sizeof(CITY_SCENE_ENTRY)*MAX_SCENE);
	}

	CHAR* GetszFileNameByPort(INT PortID)
	{
		Assert(PortID<MAX_SCENE);
		Assert(PortID>=0);
		for(INT i =0;i<MAX_SCENE;i++)
		{
			if(m_Table[i].m_PortID == PortID)
			{
				return (CHAR*)m_Table[i].m_szSceneFile;
			}
		}
		return NULL;
	}
};
extern _CITY_SCENE g_CitySceneTbl;

#define MAX_CHAT_CONSUME_TYPE 3
struct _CHAT_CONSUME_TYPE
{
	UINT			m_DelayTime ;
	INT				m_ConsumeCount ;	//������������
	INT				m_ConsumeType[MAX_CHAT_CONSUME_TYPE] ;//��������
	INT				m_ConsumeValue[MAX_CHAT_CONSUME_TYPE] ;//��������
	INT				m_NeedLevel ; //��Ҫ�ȼ�

	_CHAT_CONSUME_TYPE( )
	{
		CleanUp( ) ;
	};
	VOID CleanUp( )
	{
		m_DelayTime = 1 ;
		m_ConsumeCount = 0 ;
		for( INT i=0; i<MAX_CHAT_CONSUME_TYPE; i++ )
		{
			m_ConsumeType[i] = CHAT_NEED_NONE ;
			m_ConsumeValue[i] = 0 ;
		}
		m_NeedLevel = 0 ;
	};
};

#define MAX_CHAT_TYPE_INTABLE 16
struct _CHAT_CONSUME
{
	UINT				m_Count ;
	_CHAT_CONSUME_TYPE	m_ChatComsumeType[MAX_CHAT_TYPE_INTABLE] ;
	
	_CHAT_CONSUME( )
	{
		CleanUp( ) ;
	}
	VOID CleanUp( )
	{
		m_Count = 0 ;
	}
};

extern _CHAT_CONSUME g_ChatConsumeTbl ;

struct _HAIR_STYLE_TYPE_
{
	enum
	{
		HAIRSTYLE_NONE				= -1,
		HAIRSTYLE_INDEX,
		HAIRSTYLE_RACEID,
		HAIRSTYLE_ITEMSERIAL		= HAIRSTYLE_RACEID + 4,
		HAIRSTYLE_ITEMCOSTCOUNT,
		HAIRSTYLE_USESCOPE,
	};

	INT					m_nIndex;
	INT					m_nRaceID;
	UINT				m_uItemSerial;
	INT					m_nItemCostCount;
	INT					m_nUseScope;

	_HAIR_STYLE_TYPE_( )
	{
		CleanUp( );
	}

	VOID				CleanUp( )
	{
		m_nIndex		= -1;
		m_nRaceID		= 0;
		m_uItemSerial	= 0;
		m_nItemCostCount= 0;
		m_nUseScope		= 0;	// 0 Ϊ�����ã� 1 ֻ�д�������ʱ��ѡ�ã� 2 ֻ����Ϸ�п���ѡ�ã� 3 ��������ʱ�Լ���Ϸ�ж�����ѡ��
	}
};

#define MAX_HAIR_STYLE_INTABLE 256
struct _HAIR_STYLE
{
	UINT				m_uCount;
	_HAIR_STYLE_TYPE_	m_HairStyle[MAX_HAIR_STYLE_INTABLE];

	_HAIR_STYLE( )
	{
		CleanUp( ) ;
	}
	VOID CleanUp( )
	{
		m_uCount = 0 ;
	}
};

extern _HAIR_STYLE g_HairStyleTbl;

#define MAX_GM_SIZE 256
struct _GM_TABLE
{
	INT					m_uCount ;
	GUID_t				m_aGUID[MAX_GM_SIZE] ;

	_GM_TABLE( )
	{
		CleanUp( ) ;
	};
	VOID CleanUp( )
	{
		m_uCount = 0 ;
	}
};

extern _GM_TABLE g_GMTbl;

//�������ݱ�
struct _MISSION_DATA_t
{
	MissionID_t nMissionID;
	SHORT		nMissionClass;
	SHORT		nMissionSubClass;
	SHORT		nMissionType;
	SHORT		nMissionLevel;
	SHORT		nLevel;
	SHORT		nMaxLeve;
	CampID_t	nCamp;
	SHORT		nIdentity; //no use
	INT			nItem1ID;
	SHORT		nItem1Num;
	INT			nItem2ID;
	SHORT		nItem2Num;
	INT			nItem3ID;
	SHORT		nItem3Num;
	INT			nItem4ID;
	SHORT		nItem4Num;
	INT			nItem5ID;
	SHORT		nItem5Num;
	INT			nItem6ID;
	SHORT		nItem6Num;
	INT			nItem7ID;
	SHORT		nItem7Num;
	INT			nMonster1ID;
	SHORT		nMonster1Num;
	INT			nMonster2ID;
	SHORT		nMonster2Num;
	INT			nMonster3ID;
	SHORT		nMonster3Num;
	INT			nMonster4ID;
	SHORT		nMonster4Num;
	INT			nMoney1;
	INT			nMoney2;
	MenPaiID_t	nMenPai;
	SHORT		nGuildLevel; //no use
	INT			nMissionKind;
	INT			nTotalTimes;
	INT			nDayTimes;
	INT			nTimeDist;
	INT			nMaxCycle;
	SHORT		nPositionTag;
	MissionID_t	nPreMission;
	MissionID_t nAftMission;
};
struct _MISSION_DATA
{
	_MISSION_DATA_t*	m_MissionData;
	SHORT				m_nMissionDataCount;
	_MISSION_DATA_t*	Get(MissionID_t nMissionID);
};
extern _MISSION_DATA	g_MissionDataTable;

struct _MISSION_REWARD_t
{
	SHORT		nRewardID;
	INT			nMoney;
	INT			nItem1ID;
	SHORT		nItem1Num;
	INT			nItem2ID;
	SHORT		nItem2Num;
	INT			nItem3ID;
	SHORT		nItem3Num;
	INT			nItem4ID;
	SHORT		nItem4Num;
	INT			nItem5ID;
	SHORT		nItem5Num;
	INT			nItem6ID;
	SHORT		nItem6Num;
	INT			nExp;
	INT			nLevel;
	INT			nGuildExp1;
	INT			nGuildExp2;
	INT			nGuildExp3;
	INT			nBuff1ID;
	INT			nBuff2ID;
	INT			nBuff3ID;
};
struct _MISSION_REWARD
{
	_MISSION_REWARD_t*	m_MissionReward;
	SHORT				m_nMissionRewardCount;
	_MISSION_REWARD_t*	Get(SHORT nRewardID);
};
extern _MISSION_REWARD	g_MissionRewardTable;

struct _MISSION_PUNISH_t
{
	SHORT		nPunishID;
	INT			nMoney;
	INT			nItem1ID;
	SHORT		nItem1Num;
	INT			nItem2ID;
	SHORT		nItem2Num;
	INT			nItem3ID;
	SHORT		nItem3Num;
	INT			nExp;
	SHORT		nLevel;
	SHORT		nMissionTag;
};
struct _MISSION_PUNISH
{
	_MISSION_PUNISH_t*	m_MissionPunish;
	SHORT				m_nMissionPunishCount;
	_MISSION_PUNISH_t*	Get(SHORT nPunishID);
};
extern _MISSION_PUNISH	g_MissionPunishTable;

struct _MISSION_DIALOG_t
{
	SHORT		nDialogID;
	CHAR		szMissionName[256];		//��������
	CHAR		szMissionDesc[1024];	//��������
	CHAR		szMissionTarget[1024];	//����Ŀ��
	CHAR		szMissionContinue[512];	//��������
	CHAR		szMissionAbandon[512];	//��������
	CHAR		szMissionSuccess[512];	//�������
};
struct _MISSION_DIALOG
{
	_MISSION_DIALOG_t*	m_MissionDialog;
	SHORT				m_nMissionDialogCount;
	_MISSION_DIALOG_t*	Get(SHORT nDialogID);
};
extern _MISSION_DIALOG	g_MissionDialogTable;

struct _MISSION_LIST_t
{
	MissionID_t		nMissionID;
	ScriptID_t		nScriptID;
	SHORT			nReward;
	SHORT			nPunish;
	SHORT			nDialog;
	INT				nPromulgatorScene;
	INT				nPromulgatorID;
	INT				nSubmitorScene;
	INT				nSubmitorID;
	INT				nDestScene;
	INT				nDestX;
	INT				nDestZ;
};
extern map<INT,vector<_MISSION_LIST_t> >	g_PromulgatorIndex;
extern map<INT,vector<_MISSION_LIST_t> >	g_SubmitorIndex;
extern map<INT,vector<_MISSION_LIST_t> >	g_PromulgatorAndSubmitorIndex;
extern map<INT,_MISSION_LIST_t>				g_MissionIndexByScriptID;

//Skill and Combat
using namespace Combat_Module::Skill_Module;
typedef class TemplateManager_T<XinFaData_T> XinFaDataMgr_T;
extern XinFaDataMgr_T g_XinFaDataMgr;
typedef class TemplateManager_T<SkillTemplateData_T> SkillTemplateDataMgr_T;
extern SkillTemplateDataMgr_T g_SkillTemplateDataMgr;
typedef class TemplateManager_T<SkillInstanceData_T> SkillDataMgr_T;
extern SkillDataMgr_T g_SkillDataMgr;
typedef class TemplateManager_T<ImpactSEData_T> ImpactSEDataMgr_T;
extern ImpactSEDataMgr_T g_ImpactSEDataMgr;
//end Skill and combat
//Impact
using namespace Combat_Module::Impact_Module;
typedef class TemplateManager_T<ImpactData_T> ImpactDataMgr_T;
extern ImpactDataMgr_T g_StandardImpactMgr;
//ID collection
typedef class TemplateManager_T<IDCollection_T> IDCollectionMgr_T;
extern IDCollectionMgr_T g_IDCollectionMgr;
//AI 
#define AISCRIPT_NUM (1024)
extern CFileDataMgr g_AIFileDataMgr;
extern AIScript* g_pAIScriptList[AISCRIPT_NUM];
// camp and stand
//using namespace Combat_Module::Camp_Stand;
//typedef class TemplateManager_T<CampStandRecord_T> CampAndStandDataMgr_T;
extern CampAndStandDataMgr_T g_CampAndStandDataMgr;
// Special Obj
using namespace Combat_Module::Special_Obj_Module;
typedef class TemplateManager_T<SpecialObjData_T> SpecialObjDataMgr_T;
extern SpecialObjDataMgr_T g_SpecialObjDataMgr;
// end
class TableInit
{
public :
	TableInit() ;
	~TableInit() ;

	BOOL			Init( ) ;
	BOOL			ReloadCombatTables(VOID);
	VOID			InitGMTable( ) ;

protected :
	VOID			InitBaseValueTable( ) ;
	VOID			InitMonsterAttrExTable( ) ;
	VOID			InitMonsterAITable( ) ;
	VOID			InitNPCPaoPaoIndexTable();
	// pet
	VOID			InitPetConfigTable( ) ;
	VOID			InitPetAttrTable( );
	VOID			InitPetAttrPointDistribute( );
	VOID			InitPetLevelUpTable( );
	VOID			InitPetSkillDistributeTable( );
	VOID			InitPetSkillIndexTable( );
	VOID			InitPetStudySkillTable( );
	VOID			InitPetApperceiveSkillTable( );
	VOID			InitPetDomesticationMoney( );
	VOID			InitPetTrainCharge( );
	VOID			InitPetAIStrategyTable( );
	//skill
	BOOL 			InitXinFaDataMgr();
	BOOL 			InitSkillTemplateDataMgr();
	BOOL			InitSkillDataMgr();
	BOOL			InitImpactSEDataMgr();
	BOOL			InitStandardImpactMgr();
	//end skill
	//IdCollectionMgr
	BOOL			InitIdCollectionMgr();
	//camp and stand
	BOOL			InitCampAndStandDataMgr();
	//Special Obj
	BOOL			InitSpecialObjDataMgr();
	//end
	VOID			InitAttrLevelupTable( ) ;
	VOID			InitLevelExpTable();
	//VOID			InitXinfaUpLevelSpendTable();
	VOID			InitMonsterSpeakTable();
	VOID			InitExpAttTable();
	VOID			InitGrowPointTable();
	VOID			InitPlantTable();
	UINT			GetLevelExp(UINT iLevel);

	VOID			InitAIScriptFile();
	VOID			CleanUpAIScrpt();
	VOID			InitQuestionLib() ;
	VOID			InitIPRange( ) ;
	VOID			InitLevelMoneyTable();
	VOID			InitCitySceneTable();
	VOID			InitChatConsume( ) ;
	// data of AI
	VOID			InitAIStateList();
	VOID			CleanUpAIStateList();

	VOID			InitHairStyleTable( );

	VOID			InitMissionDataTable();
	VOID			CleanMissionData();
	VOID			InitMissionRewardTable();
	VOID			CleanMissionReward();
	VOID			InitMissionPunishTable();
	VOID			CleanMissionPunish();
	VOID			InitMissionDialogTable();
	VOID			CleanMissionDialog();
	VOID			InitMissionListTable();

public:
	BOOL			GetMissionListInfo( INT nMissionIndex, INT nScriptID, INT nIdent, _MISSION_LIST_t& one );
	BOOL			GetMissionListInfo( INT nScriptID, _MISSION_LIST_t& one );
	SPetConfig		m_PetConfig;

};
extern TableInit g_TableInit ;


#endif
