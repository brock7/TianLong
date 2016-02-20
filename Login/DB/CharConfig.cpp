#include "stdafx.h"

#include "CharConfig.h"
#include "Ini.h"
#include "DB_Struct.h"
#include "TimeManager.h"
#include "DataBase/TLBB_DBC.h"
#include "FileDef.h"

using namespace DBC;

CharConfig g_CharConfig;

CharConfig::CharConfig()
{
}

CharConfig::~CharConfig()
{
	SAFE_DELETE(m_pUserData);
	SAFE_DELETE(m_StartScene);
	SAFE_DELETE(m_SkillData);
}

BOOL CharConfig::Init()
{
	//��ʼ��Ĭ�Ͻ�ɫ����
	InitDefaultCharData();
	InitSkillData();
	InitLevel1Attr();
	InitBaseValueTable();

	return TRUE;
}

VOID CharConfig::InitDefaultCharData()
{
	UINT thisTime = 0;
#if defined(__WINDOWS__)
	thisTime = GetTickCount() ;
#elif defined(__LINUX__)
	struct timeval _tstart, _tend;
	struct timezone tz;

	gettimeofday(&_tend,&tz);
	double t1, t2;
	t1 =  (double)_tstart.tv_sec*1000 + (double)_tstart.tv_usec/1000;
	t2 =  (double)_tend.tv_sec*1000 + (double)_tend.tv_usec/1000;
	thisTime = (UINT)(t2-t1);
#endif

	srand( thisTime );

	m_pUserData	= new FULLUSERDATA;

	m_StartScene = new START_SCENE[MAX_CAMP_NUM];


	Ini f("./Config/DefaultChar.ini");
	char szTmp[32];

	//��ʼ����Ӫ����
	for( INT i=0; i < MAX_CAMP_NUM; i ++)
	{
		char szTemp0[32];
		sprintf( szTemp0, "camp_%d", i );
		m_StartScene[i].nSceneNum = f.ReadInt( szTemp0, "scene" );
		f.ReadText( szTemp0, "pos_x", szTmp, 32 );
		m_StartScene[i].fStartX = (FLOAT)(atof(szTmp));
		f.ReadText( szTemp0, "pos_z", szTmp, 32 );
		m_StartScene[i].fStartZ = (FLOAT)(atof(szTmp));
	}

	//���һ����Ӫ
	INT nRand = rand() % MAX_CAMP_NUM;
	if( nRand == 0 )
		m_pUserData->m_Human.m_CampData.m_nCampID = CAMP1_PLAYER;
	else
		m_pUserData->m_Human.m_CampData.m_nCampID = CAMP2_PLAYER;

	//�����������Ӫ����ʼ�������ص��λ��
	m_pUserData->m_Human.m_StartScene = m_StartScene[nRand].nSceneNum;
	m_pUserData->m_Human.m_Position.m_fX = m_StartScene[nRand].fStartX;
	m_pUserData->m_Human.m_Position.m_fZ = m_StartScene[nRand].fStartZ;

	//��ȡ�����ĳ�ʼ������
	m_pUserData->m_Human.m_HP = f.ReadInt( "baseinfo", "hp" );
	m_pUserData->m_Human.m_MP = f.ReadInt( "baseinfo", "mp" );
	m_pUserData->m_Human.m_Level = f.ReadInt( "baseinfo", "level" );
	m_pUserData->m_Human.m_HairColor = 0;
	m_pUserData->m_Human.m_RMBMoney  = 0;//				= f.ReadInt( "baseinfo", "vigor" );				//����
	m_pUserData->m_Human.m_BankRMB   = 0;//			= f.ReadInt( "baseinfo", "max_vigor" );			//��������
	//m_pUserData->m_Human.m_VigorRegeneRate	= f.ReadInt( "baseinfo", "vigor_regenerate" );	//�����ָ��ٶ�
	//m_pUserData->m_Human.m_Energy				= f.ReadInt( "baseinfo", "energy" );			//����
	//m_pUserData->m_Human.m_MaxEnergy			= f.ReadInt( "baseinfo", "max_energy" );		//��������
	//m_pUserData->m_Human.m_EnergyRegeneRate	= f.ReadInt( "baseinfo", "energy_regenerate" );	//�����ָ��ٶ�
	m_pUserData->m_Human.m_Exp = f.ReadInt( "baseinfo", "exp" );
	m_pUserData->m_Human.m_MenPai = f.ReadInt( "baseinfo", "menpai" );
	m_pUserData->m_Human.m_Money = f.ReadInt( "baseinfo", "money" );

	int iTmp;

	//���뼼������
	iTmp = f.ReadInt( "skill", "number" );
	m_pUserData->m_Skill.m_Count = iTmp;
	iTmp>MAX_CHAR_SKILL_NUM?iTmp=MAX_CHAR_SKILL_NUM:NULL;

	for( int i=0; i<m_pUserData->m_Skill.m_Count; i++ )
	{
		char szKeyID[32] = {0}; //szKeyActiveTime[32];

		sprintf( szKeyID, "id%d", i );
		m_pUserData->m_Skill.m_aSkill[i].m_nSkillID = f.ReadInt( "skill", szKeyID );

	}

	//������������
	strncpy( m_pUserData->m_Relation.m_szMood, "�������磬��Ʒ�������Σ�", MOOD_DATA_SIZE-1 );

	//�������������
	INT iAbliCount = f.ReadInt( "ability", "count" );
	for( INT i=0; i<iAbliCount; i++ )
	{
		char szAbility[64] ={0};
		sprintf( szAbility, "abilityskill%d", i ) ;
		INT iSkillAbility = f.ReadInt("ability", szAbility ) ;
		char szLevel[64]={0}, szExp[64]={0} ;
		sprintf( szLevel, "level%d", i ) ;
		sprintf( szExp, "exp%d", i ) ;
		m_pUserData->m_Ability.m_aABility[iSkillAbility].m_Level=f.ReadInt("ability", szLevel);
		m_pUserData->m_Ability.m_aABility[iSkillAbility].m_Exp = f.ReadInt("ability", szExp);
	}

	//�����䷽����
	iTmp = f.ReadInt( "table", "number" );
	for(INT i=0; i<iTmp; i++ )
	{
		char szTmp[32];
		memset(szTmp,0,32);
		sprintf( szTmp, "peifang%d", i );
		int iSet = f.ReadInt( "table", szTmp );

		int iByte = iSet/8;
		iSet = iSet%8;

		m_pUserData->m_Ability.m_aPrescr[iByte] |= (1<<iSet);
	}

	//������������
	iTmp = f.ReadInt( "setting", "count" );
	for(INT i=0; i<iTmp; i++ )
	{
		char szTmp0[32],szTmp1[32],szTmp2[32];
		sprintf( szTmp0, "settingindex%d", i );
		sprintf( szTmp1, "settingtype%d", i );
		sprintf( szTmp2, "settingdata%d", i );

		INT iIndex=f.ReadInt( "setting", szTmp0 );
		INT iType =f.ReadInt( "setting", szTmp1 ); ;
		INT iData =f.ReadInt( "setting", szTmp2 ); ;

		m_pUserData->m_Setting.m_aSetting[iIndex].m_SettingType=iType ;
		m_pUserData->m_Setting.m_aSetting[iIndex].m_SettingData=iData ;
	}

	m_pUserData->m_bIsPasswdUnlock = FALSE;
}

VOID CharConfig::InitSkillData()
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_SKILL_TEMPLATE );

	Assert(ret);

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	INT iCount = 0;
	for( INT i = 0; i < iTableCount; i ++ )
	{
		INT iMenPai = ThirdFile.Search_Posistion(i, 2)->iValue;
		INT iLevel  = ThirdFile.Search_Posistion(i, 5)->iValue;
		INT iClass  = ThirdFile.Search_Posistion(i, 21)->iValue;
		if( iClass == 0 )
		{
			if( iMenPai == MATTRIBUTE_SHAOLIN ||
				iMenPai == MATTRIBUTE_WUDANG ||
				iMenPai == MATTRIBUTE_EMEI ||
				iMenPai == INVALID_ID )
			{
				if( iLevel < 0 )
				{
					iCount++;
				}
			}
		}
	}
	Assert( iCount > 0 );

	m_SkillData = new SKILL_DATA[iCount];
	m_SkillData_Count = iCount;

	iCount = 0;
	for( INT i = 0; i < iTableCount; i ++ )
	{
		INT iMenPai = ThirdFile.Search_Posistion(i, 2)->iValue;
		INT iLevel  = ThirdFile.Search_Posistion(i, 5)->iValue;
		INT iClass  = ThirdFile.Search_Posistion(i, 21)->iValue;
		if( iClass == 0 )
		{
			if( iMenPai == MATTRIBUTE_SHAOLIN ||
				iMenPai == MATTRIBUTE_WUDANG ||
				iMenPai == MATTRIBUTE_EMEI ||
				iMenPai == INVALID_ID )
			{
				if( iLevel < 0 )
				{
					m_SkillData[iCount].nSkillID = ThirdFile.Search_Posistion(i, 0)->iValue;
					m_SkillData[iCount].nMenPai  = ThirdFile.Search_Posistion(i, 2)->iValue;
					iCount ++;
				}
			}
		}
	}

	__LEAVE_FUNCTION
}

VOID CharConfig::InitLevel1Attr()
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_ATTR_LEVEL_UP );

	Assert(ret);

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();
	INT iTemp = 1;

	Assert(iColumnCount==MATTRIBUTE_NUMBER*CATTR_LEVEL1_NUMBER+1);

	memset( &m_Level1Attr, 0, sizeof(ATTR_DATA)*MATTRIBUTE_NUMBER );

	for( INT i = 0; i < MATTRIBUTE_NUMBER; i ++ )
	{
		m_Level1Attr[i].str1 = ThirdFile.Search_Posistion(0, iTemp)->iValue + ThirdFile.Search_Posistion(1, iTemp++)->iValue;
		m_Level1Attr[i].spr1 = ThirdFile.Search_Posistion(0, iTemp)->iValue + ThirdFile.Search_Posistion(1, iTemp++)->iValue;
		m_Level1Attr[i].con1 = ThirdFile.Search_Posistion(0, iTemp)->iValue + ThirdFile.Search_Posistion(1, iTemp++)->iValue;
		m_Level1Attr[i].int1 = ThirdFile.Search_Posistion(0, iTemp)->iValue + ThirdFile.Search_Posistion(1, iTemp++)->iValue;
		m_Level1Attr[i].dex1 = ThirdFile.Search_Posistion(0, iTemp)->iValue + ThirdFile.Search_Posistion(1, iTemp++)->iValue;
	}

	__LEAVE_FUNCTION
}

VOID CharConfig::InitBaseValueTable()
{
	__ENTER_FUNCTION

	Ini ini( FILE_BASE_VALUE ) ;
	for( INT i=0; i<MATTRIBUTE_NUMBER; i++ )
	{
		for( INT j=0; j<AINFOTYPE_NUMBER; j++ )
		{
			CHAR szSection[256], szName[256] ;
			sprintf( szSection, "MENPAI%d", i ) ;
			sprintf( szName, "AINFOTYPE%d", j ) ;
			m_BaseValueTbl.m_Table[j][i] = ini.ReadInt( szSection, szName ) ;
		}
	}
	__LEAVE_FUNCTION
}

BOOL	CharConfig::InitCharAttr(FULLUSERDATA* pData)	
{
	__ENTER_FUNCTION	
	
	Assert(pData);
	Assert(m_pUserData);

	INT				PortraitID = pData->m_Human.m_PortraitID;					//��ɫͷ��
	CHAR			Name[MAX_CHARACTER_NAME];									//��ɫ����
	strncpy(Name,pData->m_Human.m_Name,MAX_CHARACTER_NAME);
	CHAR			Title[MAX_CHARACTER_TITLE];									//��ɫ����
	strncpy(Title,pData->m_Human.m_Title,MAX_CHARACTER_TITLE);
	GUID_t			GUID = pData->m_Human.m_GUID;								//��ɫ��ȫΨһ��
	BOOL			Sex	= pData->m_Human.m_Sex;									//��ɫ�Ա�
	UINT			CreateDate	= pData->m_Human.m_CreateDate;					//��ɫ��������
	UINT			HairColor	= pData->m_Human.m_HairColor;					//ͷ����ɫ	
	BYTE			FaceColor	= pData->m_Human.m_FaceColor;					//������ɫ
	BYTE			HairModel	= pData->m_Human.m_HairModel;					//ͷ��ģ��
	BYTE			FaceModel	= pData->m_Human.m_FaceModel;					//����ģ��
	CampID_t		CampID		= pData->m_Human.m_CampData.m_nCampID;			//��Ӫ
	MenPaiID_t		MenPaiID	= pData->m_Human.m_MenPai;
	
	pData->CleanUp();

	memcpy( pData, m_pUserData, sizeof(FULLUSERDATA) );

	pData->m_Human.m_PortraitID = PortraitID; 					//��ɫͷ��
	strncpy(pData->m_Human.m_Name,Name,MAX_CHARACTER_NAME);
	strncpy(Title,pData->m_Human.m_Title,MAX_CHARACTER_TITLE);
	pData->m_Human.m_GUID = GUID ;								//��ɫ��ȫΨһ��
	pData->m_Human.m_Sex = Sex;									//��ɫ�Ա�
	pData->m_Human.m_CreateDate = CreateDate;					//��ɫ��������
	pData->m_Human.m_HairColor = HairColor;					//ͷ����ɫ	
	pData->m_Human.m_FaceColor = FaceColor;					//������ɫ
	pData->m_Human.m_HairModel = HairModel;					//ͷ��ģ��
	pData->m_Human.m_FaceModel = FaceModel;					//����ģ��
	pData->m_Human.m_CampData.m_nCampID = CampID;			//��Ӫ
	pData->m_Human.m_MenPai = MenPaiID;

	//��ʼ����Ӫ����
	if( pData->m_Human.m_CampData.m_nCampID == CAMP1_PLAYER )
	{
		pData->m_Human.m_StartScene = m_StartScene[0].nSceneNum;
		pData->m_Human.m_Position.m_fX = m_StartScene[0].fStartX;
		pData->m_Human.m_Position.m_fZ = m_StartScene[0].fStartZ;
	}
	else
	{
		pData->m_Human.m_StartScene = m_StartScene[1].nSceneNum;
		pData->m_Human.m_Position.m_fX = m_StartScene[1].fStartX;
		pData->m_Human.m_Position.m_fZ = m_StartScene[1].fStartZ;
	}

	//��ʼ����������
	INT iTemp = 0;
	for( INT i = 0; i < m_SkillData_Count; i ++ )
	{
		if( MenPaiID == m_SkillData[i].nMenPai || m_SkillData[i].nMenPai == INVALID_ID )
		{
			pData->m_Skill.m_aSkill[iTemp].m_nSkillID = m_SkillData[i].nSkillID;
			pData->m_Skill.m_aSkill[iTemp].SetSkillLevel(1);
			iTemp ++;
		}
	}
	pData->m_Skill.m_Count = pData->m_XinFa.m_Count = iTemp;

	//��ʼ��������������
	pData->m_Human.m_BaseAttrLevel1.Set( CATTR_LEVEL1_STR, m_Level1Attr[MenPaiID].str1 );
	pData->m_Human.m_BaseAttrLevel1.Set( CATTR_LEVEL1_SPR, m_Level1Attr[MenPaiID].spr1 );
	pData->m_Human.m_BaseAttrLevel1.Set( CATTR_LEVEL1_CON, m_Level1Attr[MenPaiID].con1 );
	pData->m_Human.m_BaseAttrLevel1.Set( CATTR_LEVEL1_INT, m_Level1Attr[MenPaiID].int1 );
	pData->m_Human.m_BaseAttrLevel1.Set( CATTR_LEVEL1_DEX, m_Level1Attr[MenPaiID].dex1 );

	//����HP
	INT nInitAttr = m_BaseValueTbl.Get(AINFOTYPE_BASE_HP, MenPaiID);
	INT nAttrLevel1Refix = m_BaseValueTbl.Get(AINFOTYPE_CON_HP, MenPaiID);
	INT nLevelRefix = m_BaseValueTbl.Get(AINFOTYPE_LEVEL_HP, MenPaiID);
	INT nMaxValue = nInitAttr+m_Level1Attr[MenPaiID].con1*nAttrLevel1Refix+nLevelRefix;
	nMaxValue=Float2Int((nMaxValue)/100.0f);
	pData->m_Human.m_HP = nMaxValue;

	//����MP
	nInitAttr = m_BaseValueTbl.Get(AINFOTYPE_BASE_MP, MenPaiID);
	nAttrLevel1Refix = m_BaseValueTbl.Get(AINFOTYPE_INT_MP, MenPaiID);
	nLevelRefix = m_BaseValueTbl.Get(AINFOTYPE_LEVEL_MP, MenPaiID);
	nMaxValue = nInitAttr+m_Level1Attr[MenPaiID].con1*nAttrLevel1Refix+nLevelRefix;
	nMaxValue=Float2Int((nMaxValue)/100.0f);
	pData->m_Human.m_MP = nMaxValue;

	//pData->m_PrivateInfo.m_aPrivateData.m_bFirstLogined = 1;

	//
	//�����ʼ�����....
	//

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}







