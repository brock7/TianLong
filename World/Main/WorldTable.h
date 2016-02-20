
/////////////////////////////////////////////////////////////////////////////////
//�ļ�����WorldTable.h
//�����������������ݷ��������õ�����Դ���
//�޸ļ�¼��
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __WROLDTABLE_H__
#define __WROLDTABLE_H__

#include "Type.h"
#include "../Common/Combat/CampAndStand.h"

//���ɣ����� ��ְλ������Ȩ�ޡ����ձ�
#define MAX_MEMBER_TYPES 100
struct _GUILD_MEMBER_ACCESS
{
	INT		m_MemberTypeCount ;
	INT		m_Table[MAX_MEMBER_TYPES][GUILD_AUTHORITY_NUMBER] ;

	_GUILD_MEMBER_ACCESS( )
	{
		m_MemberTypeCount = 0 ;
		memset( m_Table, 0, sizeof(INT)*MAX_MEMBER_TYPES*GUILD_AUTHORITY_NUMBER ) ;
	};
	INT		Get( UINT membertype, UINT accesstype ){
		Assert( membertype<MAX_MEMBER_TYPES && accesstype<GUILD_AUTHORITY_NUMBER ) ;
		return m_Table[membertype][accesstype] ;
	};
};
extern _GUILD_MEMBER_ACCESS g_GuildMemberAccessTbl ;

struct _DBC_CITY_BUILDING
{
	INT		nID;					//id
	INT		nCityID;				//����id(DBC_CITY_DEINFE)
	INT		nBuildingType;			//��������
	INT		nBuildingID;			//������id(DBC_BUILDING_DEINFE)
	INT		nInitialLevel;			//������ĳ�ʼ����
	INT		nLevelRequire1;			//����1����������
	INT		nLevelRequire2;			//����2����������
	INT		nLevelRequire3;			//����3����������
	INT		nLevelRequire4;			//����4����������
	INT		nLevelRequire5;			//����4����������
	INT		nResearchItem0;			//0�������о����䷽
	INT		nResearchItem1;			//1�������о����䷽
	INT		nResearchItem2;			//2�������о����䷽
	INT		nResearchItem3;			//3�������о����䷽
	INT		nResearchItem4;			//4�������о����䷽
};

struct _DBC_CITY_DEFINE
{
	INT					m_Num;
	_DBC_CITY_BUILDING	m_Table[MAX_CITY_BUILDING];

	_DBC_CITY_DEFINE()
	{
		m_Num = 0;
		memset(m_Table,0,sizeof(_DBC_CITY_BUILDING)*MAX_CITY_BUILDING);
	}
};

extern _DBC_CITY_DEFINE g_CityBuildingTbl;

struct SKILL_DATA
{
	INT nSkillID;
	INT nMenPai;
};
extern SKILL_DATA*		g_SkillData; //��������
extern INT				g_SkillData_Count;
extern CampAndStandDataMgr_T g_CampAndStandDataMgr;

class TableInit
{
public :
	TableInit() ;
	~TableInit() ;

	BOOL			Init( ) ;

protected :
	VOID			Init_GuildMemberAccess() ;
	VOID			Init_CityBuilding() ;
	VOID			Init_SkillData();
	VOID			Init_CampAndStandDataMgr();

};
extern TableInit g_TableInit ;


#endif
