//TDDBC_Struct.h
//�������ݿ�ṹ


#pragma once

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								�������								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

/////////////////////////////////////////////////////////////////////
//��������							(CHAR_01)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_RACE			(101)		//��������(�У�Ů)
struct _DBC_CHAR_RACE
{
	INT		nID;
	INT		nGender;
	INT		nIsPlayer;
	INT		nModelID;
	INT		nDefHeadGeo;
	INT		nDefHairGeo;
	INT		nDefBody;
	INT		nDefArm;
	INT		nDefFoot;
	INT		nIdleInterval;
};

/////////////////////////////////////////////////////////////////////
//����ͷ��ģ��						(CHAR_02)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_HAIR_GEO		(102)		//����ͷ��ģ��
struct _DBC_CHAR_HAIR_GEO
{
	INT		nID;
	INT		nRace;
	LPCSTR	pMeshFile;
	LPCSTR	pShowName;
};

/////////////////////////////////////////////////////////////////////
//��������ģ��						(CHAR_04)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_HEAD_GEO		(104)		//��������ģ��
struct _DBC_CHAR_HEAD_GEO
{
	INT		nID;
	INT		nRace;
	LPCSTR	pMeshFile;
	LPCSTR	pShowName;
};

/////////////////////////////////////////////////////////////////////
//���Ƕ�����						(CHAR_06)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_ACTION_SET	(106)
#define MAX_WEAPON_TYPE_NUMBER		(9)
struct _DBC_CHARACTER_ACTION_SET
{
	INT		nID;
	LPCSTR	pWeapon_Set[MAX_WEAPON_TYPE_NUMBER];
	BOOL	bHideWeapon;
	INT		nAppointedWeaponID;		// ָ��������ID
	LPCSTR	pszDesc;
};

/////////////////////////////////////////////////////////////////////
//������Ч							(CHAR_07)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_EFFECT		(107)
struct _DBC_CHARACTER_EFFECT
{
	INT		nID;
	INT		nEffect1;
	INT		nEffect2;
	INT		nSoundID;
	LPCSTR	pLocator;
	LPCSTR	pWeaponLocator;
};

/////////////////////////////////////////////////////////////////////
//������������ֵ						(CHAR_08)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_EXPLEVEL		(108)
struct _DBC_CHARACTER_EXPLEVEL
{
	INT		nID;
	INT		nEffectNeed;
};

/////////////////////////////////////////////////////////////////////
//����ID��Ӧģ������					(CHAR_09)
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_MODEL			(109)
#define MAX_MOUNT_NUMBER			(20)
struct _DBC_CHARACTER_MODEL
{
	INT		m_nID;
	LPCSTR	m_pszModelName;
	FLOAT	m_fFuseTime;				// �����ۺ�ʱ��
	LPCSTR	m_pszActionSetName_None;
	LPCSTR	m_apszActionSetName_Mount[MAX_MOUNT_NUMBER];
};

/////////////////////////////////////////////////////////////////////
//����ͷ��
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_FACE		(110)		//����ͷ��
struct _DBC_CHAR_FACE
{
	INT		nID;
	INT		nRace;
	LPCSTR	pImageSetName;
};

/////////////////////////////////////////////////////////////////////
//�����
/////////////////////////////////////////////////////////////////////
#define DBC_CHARACTER_MOUNT		(111)
struct _DBC_CHARACTER_MOUNT
{
	INT		m_nID;
	INT		m_nModelID;
	FLOAT	m_fAddHeight;
	INT		m_nPassLevel;
};

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								�������								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

/////////////////////////////////////////////////////////////////////
//���ﶨ���ܱ�						(CREATURE_01)
/////////////////////////////////////////////////////////////////////
#define DBC_CREATURE_ATT			(201)
struct _DBC_CREATURE_ATT
{
	INT		nID;
	LPCSTR	pName;
	INT		nCannotBeAttack;
	INT		nLevel;
	INT		NOTCARE1[2];
	INT		nIdleInterval;
	INT		NOTCARE2[6];
	INT		nCanHandle;				// �Ƿ���Խ���
	INT		nMonsterBossFlag;
	INT		nAttackRate_P;
	INT		nDefence_P;
	INT		nAttackRate_M;
	INT		nDefence_M;
	INT		nMaxHP;
	INT		nMaxMP;
	INT		nRestoreHP;
	INT		nRestoreMP;
	INT		nHit;
	INT		nMiss;
	INT		nCritrate;
	INT		nSpeed;
	INT		nMonsterWalkSpeed;
	INT		nAttackSpeed;
	INT		nAttackCold;
	INT		nDefenceCold;
	INT		nAttackFire;
	INT		nDefenceFire;
	INT		nAttackLight;
	INT		nDefenceLight;
	INT		nAttackPoison;
	INT		nDefencePoison;
	INT		nMonsterBossSnapImmID;
	INT		nMonsterBossDurationImmID;
	INT		nModelID;				// ����ID ��ǰ��LPCSTR	szModeleFile;
//	INT		nReserve;				// �Ѿ��ϳ� LPCSTR	szActionSetFile;		// �����ļ����� 
	INT		nIsDisplayerName;		// �Ƿ���ʾͷ����Ϣ��
	INT		nIsCanSwerve;			// �Ƿ�����ҵ�ѡת��
	FLOAT	fBoardHeight;			// ͷ�����ְ�߶�
	FLOAT	fProjTexRange;			// ѡ�л���С
	FLOAT	fShadowRange;			// �Ƿ���ʾ��Ӱ����Ӱ��С
	LPCSTR	szIconName;				// ͷ��ͼ��
};

/////////////////////////////////////////////////////////////////////
//���︽������						(DBC_PET_EX_ATT)
/////////////////////////////////////////////////////////////////////
#define DBC_PET_EX_ATT				(202)
struct _DBC_PET_EX_ATT
{
	INT		m_Type;						// ������
	LPTSTR	m_Name;						// ����
	INT		m_TakeLevel;				// ��Я���ȼ�
	INT		m_Camp;						// ��Ӫ
	INT		m_Reserve1;					// ����
	INT		m_Reserve2;					// ����
	INT		m_FoodType;					// ʳ����
	INT		m_SkillLearnNum;			// ����ѧ�ļ�����
	INT		m_PositiveSkill;			// ��������
	INT		m_PassiveSkill1;			// ��������1
	INT		m_PassiveSkill2;			// ��������2
	INT		m_PassiveSkill3;			// ��������3
	INT		m_PassiveSkill4;			// ��������4
	INT		m_Life;						// ��׼����
	INT		m_StrPerception;			// ��׼��������
	INT		m_ConPerception;			// ��׼��������
	INT		m_DexPerception;			// ��׼��������
	INT		m_SprPerception;			// ��׼������
	INT		m_IntPerception;			// ��׼��������
	INT		m_GrowRate0;				// �ɳ���1
	INT		m_GrowRate1;				// �ɳ���2
	INT		m_GrowRate2;				// �ɳ���3
	INT		m_GrowRate3;				// �ɳ���4
	INT		m_GrowRate4;				// �ɳ���5
	INT		m_CowardiceRate;			// ��С����
	INT		m_WarinessRate;				// ��������	
	INT		m_LoyalismRate;				// �ҳϼ���
	INT		m_CanninessRate;			// ��������
	INT		m_ValourRate;				// ���ͼ���
};

/////////////////////////////////////////////////////////////////////
//�������������					(CREATURE_03)
/////////////////////////////////////////////////////////////////////
#define DBC_CREATURE_SOUND			(203)
struct _DBC_CREATURE_SOUND
{
	INT		nID;
	INT		nAttack;
	INT		nUnderAttack;
	INT		nDeath;
};

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								��Ʒ���								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

/////////////////////////////////////////////////////////////////////
//װ������Ʒ��ʾ����				(ITEM_01)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_VISUAL_CHAR		(301)
struct _DBC_ITEM_VISUAL_CHAR
{
	#define CHAR_RACE_NUM	(2)

	INT		nID;
	LPCSTR	pVisualEntity[2*CHAR_RACE_NUM];
	LPCSTR	pEffectName[30];//����30�и��ֱ�ʯ����Ч
};

/////////////////////////////////////////////////////////////////////
//�ҽ�����Ʒ��ʾ����				(ITEM_02)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_VISUAL_LOCATOR		(302)
struct _DBC_ITEM_VISUAL_LOCATOR
{
	INT		nID;
	LPCSTR	pObjFile_Right;
	LPCSTR	pMatName_Right;
	LPCSTR	pObjParam1_Right;
	LPCSTR	pObjParam2_Right;
	LPCSTR	pObjFile_Left;
	LPCSTR	pMatName_Left;
	LPCSTR	pObjParam1_Left;
	LPCSTR	pObjParam2_Left;
	LPCSTR	pEffectName[30];//����30�и��ֱ�ʯ����Ч

};

/////////////////////////////////////////////////////////////////////
//��Ʒ�����_��װ					(ITEM_03)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_EQUIP_WHITE		(303)
struct _DBC_ITEM_EQUIP_WHITE
{
	INT		nID;
	INT		nClass;
	INT		nQuality;
	INT		nType;
	INT		nIndex;
	INT		nEquipPoint;
	INT		nVisualID;
	INT		nRule;
	INT		nSetID;
	INT		nSetNum;
	INT		nJob;
	LPCSTR	szName;
	INT		nLevelRequire;
	LPCSTR	szDesc;
	INT		nBasePrice;
	INT		nBMaxDur;
	INT		nBMaxRepair;
	INT		nBMaxGem;
	//...NOT CARE
	INT		NOTCARE[66];
	LPCTSTR szIcon;
	LPCTSTR szTypeDesc;		// ��Ʒ����������2006��4��28
};

/////////////////////////////////////////////////////////////////////
//��Ʒ�����_��װ					(ITEM_04)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_EQUIP_GREEN		(304)
struct _DBC_ITEM_EQUIP_GREEN
{
	INT		nID;
	INT		nClass;
	INT		nQuality;
	INT		nType;
	INT		nIndex;
	INT		nEquipPoint;
	INT		nVisualID;
	INT		nRule;
	INT		nSetID;
	INT		nSetNum;
	INT		nJob;
	LPCSTR	szName;
	INT		nLevelRequire;
	LPCSTR	szDesc;
	INT		nBasePrice;
	INT		nBMaxDur;
	INT		nBMaxRepair;
	INT		nBMaxGem;
	//...NOT CARE
	INT		NOTCARE[66];
	LPCTSTR szIcon;
	LPCTSTR szTypeDesc;		// ��Ʒ����������2006��4��28
};


/////////////////////////////////////////////////////////////////////
//��Ʒ�����_��װ					(ITEM_05)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_EQUIP_BLUE		(305)
struct _DBC_ITEM_EQUIP_BLUE
{
	INT		nID;
	INT		nClass;
	INT		nQuality;
	INT		nType;
	INT		nIndex;
	INT		nEquipPoint;
	INT		nVisualID;
	INT		nRule;
	INT		nSetID;
	INT		nSetNum;
	INT		nJob;
	LPCSTR	szName;
	INT		nLevelRequire;
	LPCSTR	szDesc;
	INT		nBasePrice;
	INT		nBMaxDur;
	INT		nBMaxRepair;
	INT		nBMaxGem;
	//...NOT CARE
	INT		NOTCARE[66];
	LPCTSTR szIcon;
	LPCTSTR szTypeDesc;		// ��Ʒ����������2006��4��28
};

/////////////////////////////////////////////////////////////////////
//��Ʒ�����_ҩƿ				(ITEM_06)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_MEDIC			(306)
struct _DBC_ITEM_MEDIC
{
	INT		nID;
	INT		nClass;
	INT		nQuality;
	INT		nType;
	INT		nIndex;
	LPCSTR	szIcon;
	LPCSTR	szName;
	LPCSTR	szDesc;
	INT		nLevelRequire;
	INT		nSalePrice;	//�����۸�
	INT		nRule;		//��Ӧ����
	INT		nPileCount; //��������
	INT		nScriptID;	//�ű����
	INT		nSkillID;	//���ܱ��
	INT		NOTCARE[3];	//�����ܵȼ�
	INT		nMaxHold;	//����������
	INT		nTargetType;	// ��Զ������� ENUM_ITEM_TARGET_TYPE
	LPCTSTR szTypeDesc;		// ��Ʒ����������2006��4��28

	//... NOT CARE
};

/////////////////////////////////////////////////////////////////////
//��Ʒ�����_��ʯ				(ITEM_07)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_GEM			(307)
struct _DBC_ITEM_GEM
{
	INT		nID;
	INT		nClass;
	INT		nQuality;
	INT		nType;
	INT		nGemIndex;
	LPCTSTR	szIcon;
	INT		nRule;
	LPCTSTR	szName;
	LPCTSTR szDesc;
	INT		nPrice;		// ��ʯ�ļ۸�
	INT		nAttr[60];	// ��ʯ�ĸ�������
	LPCTSTR	szColor;	// �������Ե���ɫ
	LPCTSTR szTypeDesc; // ��������
	INT		nEffectIndex;//��Ч����
	//... NOT CARE
};

/////////////////////////////////////////////////////////////////////
//��Ʒ�����_��ͼ				(ITEM_08)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_STOREMAP			(308)
struct _DBC_ITEM_STOREMAP
{
	INT		nID;
	INT		nClass;
	INT		nQuality;
	INT		nType;
	INT		nIndex;
	LPCSTR	szIcon;
	LPCSTR	szName;
	LPCSTR	szDesc;
	INT		nLevelRequire;
	INT		nRule;
	//.......................
	//... NOT CARE
};

/////////////////////////////////////////////////////////////////////
//��װ��Ϻ󸽼�����			(ITEM_15)
/////////////////////////////////////////////////////////////////////
#define	DBC_ITEM_SETATT			(315)
struct _DBC_ITEM_SETATT
{
	INT		nID;
	INT		nAtt[60];
};


//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								�������								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

/////////////////////////////////////////////////////////////////////
//�����ļ�����						(SOUND_01)
/////////////////////////////////////////////////////////////////////
#define DBC_SOUND_INFO				(401)
struct _DBC_SOUND_INFO
{
	INT		nID;
	LPCSTR	pSoundFile;
};


//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								��Ч���								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

/////////////////////////////////////////////////////////////////////
//��Ч����							(EFFECT_01)
/////////////////////////////////////////////////////////////////////
#define DBC_EFFECT_DEFINE			(501)
struct _DBC_EFFECT_DEFINE
{
	INT		nID;
	LPCSTR	pEffectType;
	LPCSTR	pParam1;
	LPCSTR	pParam2;
	LPCSTR	pParam3;
	LPCSTR	pParam4;
	LPCSTR	pParam5;
	LPCSTR	pParam6;
};

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								�������								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
/////////////////////////////////////////////////////////////////////
//�ķ������						(SKILL_01)
/////////////////////////////////////////////////////////////////////
#define DBC_SKILL_XINFA				(601)
struct _DBC_SKILL_XINFA
{
	INT		nID;
	INT		nMenpaiID;
	LPCSTR	pszName;
	LPCSTR	pszDesc;
	LPCSTR	pszIconName;
};

/////////////////////////////////////////////////////////////////////
//����Ч��							(SKILL_02)
/////////////////////////////////////////////////////////////////////
#define DBC_SKILL_ADDIMPACT			(602)
struct _DBC_SKILL_ADDIMPACT
{
	INT		nID;
	LPCSTR	pDesc;
	INT		nParamNum;
	INT		nAddAttributeNum;
	LPCSTR	pAddAttribute1;
	LPCSTR	pAddAttribute2;
	LPCSTR	pAddAttribute3;
	LPCSTR	pAddAttribute4;
	INT		nSpliceID;
	LPCSTR	pIcon;
};
//
///////////////////////////////////////////////////////////////////////
////�ӵ�Ч������						(SKILL_03)
///////////////////////////////////////////////////////////////////////
//#define DBC_SKILL_BULLET			(603)
//struct _DBC_SKILL_BULLET
//{
//	INT		nID;							//�ӵ�����
//	INT		nType;							/*
//											|	0 - ˲�䵽��
//											|	1 - �ƶ�����
//											*/
//	INT		nEffect;						//��ЧID
//	FLOAT	fSpeed;							//�ƶ��ٶ�	(m/s)
//	INT		nHitEffect;						//������Ч
//	INT		nHitEffectLiveTime;				//������Чά��ʱ��
//};

/////////////////////////////////////////////////////////////////////
//BUFF����Ч��						(BUFF_IMPACT)
/////////////////////////////////////////////////////////////////////
#define DBC_BUFF_IMPACT					(604)
//_DBC_BUFF_IMPACT

/////////////////////////////////////////////////////////////////////
//DIRECTLY����Ч��						(DIRECTLY_IMPACT)
/////////////////////////////////////////////////////////////////////
#define DBC_DIRECTLY_IMPACT				(605)
//_DBC_DIRECTLY_IMPACT

/////////////////////////////////////////////////////////////////////
//�ӵ�						(BULLET)
/////////////////////////////////////////////////////////////////////
#define DBC_BULLET_DATA					(606)
//_DBC_BULLET_DATA

/////////////////////////////////////////////////////////////////////
//����						(SKILL)
/////////////////////////////////////////////////////////////////////
#define DBC_SKILL_DATA					(607)
//_DBC_SKILL_DATA

/////////////////////////////////////////////////////////////////////
//����						(SKILL_DEPLETE)
/////////////////////////////////////////////////////////////////////
#define DBC_SKILL_DEPLETE					(608)
struct _DBC_SKILL_DEPLETE
{
	INT			m_nID;
	INT			m_nHP;
	INT			m_nMP;
	INT			m_nSP;
	INT			m_nStrikePoint;
	INT			m_nItemID;
};

/////////////////////////////////////////////////////////////////////
//����						(MISSION)
/////////////////////////////////////////////////////////////////////
#define DBC_MISSION_LIST				(610)
struct _DBC_MISSION_LIST
{
	INT				nScriptID;
	INT				nMissionID;
	INT				nReward;
	INT				nPunish;
	INT				nDialog;
	INT				nPromulgatorScene;
	INT				nPromulgatorID;
	INT				nSubmitorScene;
	INT				nSubmitorID;
	INT				nDestScene;
	INT				nDestX;
	INT				nDestZ;
};

#define DBC_MISSION_DATA				(611)
struct _DBC_MISSION_DATA
{
	INT		nMissionID;
	INT		nMissionClass;
	INT		nMissionSubClass;
	INT		nMissionType;
	INT		nMissionLevel;
	INT		nLevel;
	INT		nMaxLeve;
	INT		nCamp;
	INT		nIdentity; //no use
	INT		nItem1ID;
	INT		nItem1Num;
	INT		nItem2ID;
	INT		nItem2Num;
	INT		nItem3ID;
	INT		nItem3Num;
	INT		nItem4ID;
	INT		nItem4Num;
	INT		nItem5ID;
	INT		nItem5Num;
	INT		nItem6ID;
	INT		nItem6Num;
	INT		nItem7ID;
	INT		nItem7Num;
	INT		nMonster1ID;
	INT		nMonster1Num;
	INT		nMonster2ID;
	INT		nMonster2Num;
	INT		nMonster3ID;
	INT		nMonster3Num;
	INT		nMonster4ID;
	INT		nMonster4Num;
	INT		nMoney1;
	INT		nMoney2;
	INT		nMenPai;
	INT		nGuildLevel; //no use
	INT		nMissionKind;
	INT		nTotalTimes;
	INT		nDayTimes;
	INT		nTimeDist;
	INT		nMaxCycle;
	INT		nPositionTag;
	INT		nPreMission;
	INT		nAftMission;
};

#define DBC_MISSION_REWARD				(612)
struct _DBC_MISSION_REWARD
{
	INT			nRewardID;
	INT			nMoney;
	INT			nItem1ID;
	INT			nItem1Num;
	INT			nItem2ID;
	INT			nItem2Num;
	INT			nItem3ID;
	INT			nItem3Num;
	INT			nItem4ID;
	INT			nItem4Num;
	INT			nItem5ID;
	INT			nItem5Num;
	INT			nItem6ID;
	INT			nItem6Num;
	INT			nExp;
	INT			nLevel;
	INT			nGuildExp1;
	INT			nGuildExp2;
	INT			nGuildExp3;
	INT			nBuff1ID;
	INT			nBuff2ID;
	INT			nBuff3ID;
};

#define DBC_MISSION_PUNISH				(613)
struct _DBC_MISSION_PUNISH
{
	INT		nPunishID;
	INT		nMoney;
	INT		nItem1ID;
	INT		nItem1Num;
	INT		nItem2ID;
	INT		nItem2Num;
	INT		nItem3ID;
	INT		nItem3Num;
	INT		nExp;
	INT		nLevel;
	INT		nMissionTag;
};

#define DBC_MISSION_DIALOG				(614)
struct _DBC_MISSION_DIALOG
{
	INT			nDialogID;
	LPCSTR		szMissionName;		//��������
	LPCSTR		szMissionDesc;	//��������
	LPCSTR		szMissionTarget;	//����Ŀ��
	LPCSTR		szMissionContinue;	//��������
	LPCSTR		szMissionAbandon;	//��������
	LPCSTR		szMissionSuccess;	//�������
};

/////////////////////////////////////////////////////////////////////
//����OBJ����						(SPECIAL_OBJ_DATA)
/////////////////////////////////////////////////////////////////////
#define DBC_SPECIAL_OBJ_DATA			(630)
struct _DBC_SPECIAL_OBJ_DATA
{
	INT			m_nID;					//ID
	LPCSTR		m_lpszName;				//����
	LPCSTR		m_lpszToolTips;			//ToolTips
	INT			m_nType;				//��𣨷�����ר�ã�
	INT			m_nLogicID;				//�߼�ID��������ר�ã�
	INT			m_nStealthLevel;		//���μ��𣨷�����ר�ã�����ר�ã�
	INT			m_nTrapFlags;			//������λ���ϣ�������ר�ã�����ר�ã�
	INT			m_nActiveTimes;			//���Լ����Ĵ�����������ר�ã�
	LPCSTR		m_lpszEffect_Normal;	//�����ڳ�����Ч
	LPCSTR		m_lpszSound_Normal;		//�����ڳ�����Ч
	LPCSTR		m_lpszEffect_Active;	//������Ч
	LPCSTR		m_lpszSound_Active;		//������Ч
	LPCSTR		m_lpszEffect_Die;		//������Ч
	LPCSTR		m_lpszSound_Die;		//������Ч
	INT			m_nReserve1;			//Ԥ��
	INT			m_nReserve2;			//Ԥ��
	INT			m_nReserve3;			//Ԥ��
	INT			m_nBulletID;			//�ӵ�ID
	INT			m_nDuration;			//����ʱ�䣨������ר�ã�
	INT			m_nInterval;			//����򴥷�ʱ������������ר�ã�
	FLOAT		m_nTriggerRadius;		//�����뾶��������ר�ã�����ר�ã�
	FLOAT		m_nEffectRadius;		//Ӱ��뾶��������ר�ã�����ר�ã�
	INT			m_nEffectUnitNum;		//Ӱ�������Ŀ��������ר�ã�����ר�ã�
	INT			m_nReserve4;			//Ԥ��
	INT			m_nReserve5;			//Ԥ��
	INT			m_nReserve6;			//Ԥ��
	INT			m_nReserve7;			//Ԥ��
	INT			m_nReserve8;			//Ԥ��
	LPCSTR		m_lpszParamDesc0;		//����˵����������ר�ã�
	INT			m_nParam0;				//����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc1;		//����˵����������ר�ã�
	INT			m_nParam1;				//����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc2;		//����˵����������ר�ã�
	INT			m_nParam2;				//����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc3;		//����˵����������ר�ã�
	INT			m_nParam3;				//����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc4;		//����˵����������ר�ã�
	INT			m_nParam4;				//����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc5;		//����˵����������ר�ã�
	INT			m_nParam5;				//����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc6;		//����˵����������ר�ã�
	INT			m_nParam6;				//����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc7;		//����˵����������ר�ã�
	INT			m_nParam7;				//����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc8;		//����˵����������ר�ã�
	INT			m_nParam8;				//����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc9;		//����˵����������ר�ã�
	INT			m_nParam9;				//����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc10;		//����˵����������ר�ã�
	INT			m_nParam10;				//����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc11;		//����˵����������ר�ã�
	INT			m_nParam11;				//����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc12;		//����˵����������ר�ã�
	INT			m_nParam12;				//����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc13;		//����˵����������ר�ã�
	INT			m_nParam13;				//����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc14;		//����˵����������ר�ã�
	INT			m_nParam14;				//����ֵ��������ר�ã�
	LPCSTR		m_lpszParamDesc15;		//����˵����������ר�ã�
	INT			m_nParam15;				//����ֵ��������ר�ã�
	LPCSTR		m_lpszInfo;				//��ϸ˵��(�ڲ�ʹ��)
};

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								����������							//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

/////////////////////////////////////////////////////////////////////
//��������							(SCENE_01)
/////////////////////////////////////////////////////////////////////
#define DBC_SCENE_DEFINE			(701)
struct _DBC_SCENE_DEFINE
{
	static const int SCENE_SERVER_ID_COLUMN = 1;

	INT		nLocalID;
	INT		nServerID;			//	 <--- ServerID is here!
	INT		nCityLevel;
	LPCSTR	szName;
	INT		nXSize;
	INT		nZSize;
	LPCSTR	szWXObjectName;
	LPCSTR	szRegionFile;
	LPCSTR  szCollisionfile;	// �������������ļ���
	LPCSTR	szMiniMap;
	INT		nBackSound;
	LPCSTR  szSceneMap;

	INT		nWroldMapPosX;
	INT		nWroldMapPosY;				// ����ͼ�����ڵ�λ��
	INT		nNameWroldMapPosX;
	INT		nNameWroldMapPosY;			// �����������ڵ�λ��
	LPCSTR	szSceneType;				// ������ͼ��ȼ�
	LPCSTR	szCityNameNormalImageSet;		// ���������������õ�ͼƬ��Դ
	LPCSTR	szCityNameNormalImage;		// ���������������õ�ͼƬ��Դ
	LPCSTR	szCityNameHoverImageSet;		// ���������������õ�ͼƬ��Դ
	LPCSTR	szCityNameHoverImage;		// ���������������õ�ͼƬ��Դ

};

/////////////////////////////////////////////////////////////////////
//���������ﶨ��					(SCENE_02)
/////////////////////////////////////////////////////////////////////
#define DBC_BUILDING_DEINFE			(702)
struct _DBC_BUILDING_DEFINE
{
	static const int MAX_LEVEL = 5;

	INT		nID;				//��Դid
	LPCSTR	szResType;			//��Դ����
	LPCSTR	szLevel[MAX_LEVEL];	//�ȼ�1-5��Դ
};

/////////////////////////////////////////////////////////////////////
//���н����ﶨ��					(SCENE_03)
/////////////////////////////////////////////////////////////////////
#define DBC_CITY_BUILDING			(703)
struct _DBC_CITY_BUILDING
{
	INT		nID;					//id
	INT		nCityID;				//����id(DBC_CITY_DEINFE)
	INT		nBuildingID;			//������id(DBC_BUILDING_DEINFE)
	INT		nInitLevel;				//��ʼ�ȼ�
	LPCSTR	szGfxPosition;			//λ��(gfx)
	LPCSTR	szGfxOrientation;		//��ת��Ԫ��(gfx)
};

/////////////////////////////////////////////////////////////////////
//�������͵㶨��							(SCENE_02)
/////////////////////////////////////////////////////////////////////
#define DBC_SCENE_POS_DEFINE			(917)
struct _DBC_SCENE_POS_DEFINE
{
	INT		nID;				// id
	LPCSTR	szSceneName;		// ����������
	INT		nSceneID;			// ������id
	INT		nXPos;				// ���͵��λ��
	INT		nZPos;				// ���͵��λ��
	LPCSTR	szAimName;			// ���͵��ĸ�����
};

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								UI���									//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
/////////////////////////////////////////////////////////////////////
//UI�鶨��						(UI_01)
/////////////////////////////////////////////////////////////////////
#define DBC_UI_LAYOUTDEFINE		(801)
struct _DBC_UI_LAYOUTDEFINE
{
	INT		nID;
	LPCSTR	szName;
	LPCSTR	szLayout;
	LPCSTR	szScript;
	INT		nDemise;
};


//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//								��������							//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
/////////////////////////////////////////////////////////////////////
//��Դ������							(LIFE_01)
/////////////////////////////////////////////////////////////////////
#define DBC_LIFEABILITY_GROWPOINT		(901)
struct _DBC_LIFEABILITY_GROWPOINT
{
	INT		nID;
	LPCSTR	szName;
	LPCSTR	szMeshFile;
	INT		nLifeAbility;
	LPCSTR	szTips;
	INT		nTimeNeed;
	LPCSTR	szNotcare[4];
	INT		nAnimOpen;
};

/////////////////////////////////////////////////////////////////////
//�����								(LIFE_02)
/////////////////////////////////////////////////////////////////////
#define DBC_LIFEABILITY_DEFINE	(902)
struct _DBC_LIFEABILITY_DEFINE
{
	INT		nID;
	LPCSTR	szName;
	INT		nLevelNeed;
	INT		nLevelMax;
	INT		nTimeOperation;
	INT		nToolNeed;
	FLOAT	fPlatformDist;
	INT		nPlatformID;
	INT		nAnimOperation;
	LPCSTR	szIconName;
	INT		nItemVisualLocator; //�����ҽ�
	LPCSTR	szExplain;			//��ϸ����
	INT		nPopup;				//�Ƿ񵯳��¼�����
	LPCSTR	szProgressbarName;	//����������
};

/////////////////////////////////////////////////////////////////////
//�����								(LIFE_03)
/////////////////////////////////////////////////////////////////////
#define DBC_LIFEABILITY_ITEMCOMPOSE		(903)
struct _DBC_LIFEABILITY_ITEMCOMPOSE
{
	INT		nID;
	LPCSTR	szName;
	INT		nResultID;
	INT		nResultNum;
	INT		nLifeAbility;
	INT		nLifeAbility_Level;
	INT		nStuff1ID;
	INT		nStuff1num;
	INT		nStuff2ID;
	INT		nStuff2num;
	INT		nStuff3ID;
	INT		nStuff3num;
	INT		nStuff4ID;
	INT		nStuff4num;
	INT		nStuff5ID;
	INT		nStuff5num;
	INT		nAttribute_ID1;
	INT		nAttribute_Level1;
	INT		nAttribute_ID2;
	INT		nAttribute_Level2;
	INT		nCoolDown_Time;
	INT		nFacility;				// ��Ҫ�Ĺ���	
	INT		nCoolDoon_Troop;		// ��ȴ��
	INT		nProficiencyRequire;	// ����������
	INT		nProficiencyIncrease;	// ����������
	INT		NOTCARE;				// �����ĵ�1��
	INT		nProficiencyTime;		// �䷽ʱ��
	INT		nScriptID;				// �ű�ID
	INT		nFailItem;				// ʧ�ܺ�����Ʒ
	LPCSTR	szExplain;				// ��ϸ����


	// ���ܷ���

	//NOT CARE...
};

/////////////////////////////////////////////////////////////////////
//�����								(LIFE_04)
/////////////////////////////////////////////////////////////////////
#define DBC_LIFEABILITY_PLATFORM		(904)
struct _DBC_LIFEABILITY_PLATFORM
{
	INT		nID;
	INT		nType;
	LPCSTR	szName;
	LPCSTR	szMeshFile;
	INT		nClickType;
};
/////////////////////////////////////////////////////////////////////
// �������ɫת���ַ�
/////////////////////////////////////////////////////////////////////
#define DBC_CODE_CONVERT			( 905 )
struct _DBC_CODE_CONVERT
{
	INT		nID;		// ID
	LPCSTR  szName;		// ����
	LPCSTR  szCode;		// �ַ�����ʽ��ʮ����������
};

/////////////////////////////////////////////////////////////////////
//�ķ�����								(XINFA_02)
/////////////////////////////////////////////////////////////////////
#define DBC_XINFA_XINFADEFINE	(902)
struct _DBC_XINFA_XINFADEFINE
{
	INT		nID;			//�ķ�ID
	INT		nIDMenpai;		//����ID
	LPCSTR	szName;			//�ķ�����
	LPCSTR	szTips;			//�ķ�����
	LPCSTR	szIconName;		//ͼ������
};
/////////////////////////////////////////////////////////////////////
//�ķ���������						(XINFA_02)
/////////////////////////////////////////////////////////////////////
struct Spend
{
	INT		dwSpendMoney;			// ��Ҫ���ѵĽ�Ǯ��
	INT		dwSpendExperience;		// ��Ҫ���ѵľ���ֵ
};
#define DBC_XINFA_STUDYSPEND	(907)
struct _DBC_XINFA_STUDYSPEND
{
	INT		nStudyLevel;			// Ҫѧϰ�ĵȼ�
	Spend	StudySpend[54];
};

/////////////////////////////////////////////////////////////////////
//�ƺ��б�						(TITLE)
/////////////////////////////////////////////////////////////////////
#define DBC_TITLE_NAME			(908)
struct _DBC_TITLE_NAME
{
	INT		nTitleIndex;			// �ƺŵ�����
	UINT	nReserve;				// ���ͱ���
	LPCSTR	szTitle;				// �ƺŵ�����
	LPCSTR	szTitleDesc;			// �ƺŵ�����
};
/////////////////////////////////////////////////////////////////////
//��Ӫ����	
/////////////////////////////////////////////////////////////////////
#define DBC_CAMP_DATA	(909)

/////////////////////////////////////////////////////////////////////
//����	skillData_v1.txt(��ȡ��)					
/////////////////////////////////////////////////////////////////////
#define DBC_SKILLDATA_V1_DEPLETE					(910)
struct _DBC_SKILLDATA_V1_DATA_PAIR
{
	INT		nData;
	LPCSTR  szDesc;
	
};
struct _DBC_SKILLDATA_V1_DEPLETE
{

	INT		nId;				//  ID	  
	INT		nCeHuaId;			//	�߻�ר��ID	
	LPCSTR	szEffectDesc;		//	Ч���ļ�Ҫ�߼�˵��
	INT		nNeedLevel;			//	
	INT		nNeedMoney;			//
	INT		nSkillLogicid;		//	�����߼�ID	
	INT		nCooldownTime;		//	��ȴʱ��	
	INT		nJuqiTime;			//	����ʱ��(ms)	
	INT		nDirectTime;		//	����ʱ��(ms)	
	LPCSTR  szCondition1Desc;	//	���������Ĳ���˵��
	INT		nCondition11;		//	���������Ĳ���ֵ	
	INT	    nCondition12;		//	���������Ĳ���ֵ	
	LPCSTR  szCondition2Desc;	//	���������Ĳ���˵��
	INT		nCondition21;		//	���������Ĳ���ֵ	
	INT		nCondition22;		//	���������Ĳ���ֵ	
	LPCSTR  szCondition3Desc;	//	���������Ĳ���˵��
	INT		nCondition31;		//	���������Ĳ���ֵ	
	INT		nCondition32;		//	���������Ĳ���ֵ	
	LPCSTR	szCondition4Desc;	//	���������Ĳ���˵��
	INT		nCondition41;		//	���������Ĳ���ֵ	
	INT		nCondition42;		//	���������Ĳ���ֵ	
	LPCSTR  szCondition5Desc;	//	���������Ĳ���˵��
	INT		nCondition51;		//	���������Ĳ���ֵ	
	INT		nCondition52;		//	���������Ĳ���ֵ	
	LPCSTR  szCondition6Desc;	//	����˵��
	    
	// ������
	_DBC_SKILLDATA_V1_DATA_PAIR	paraPair[12];

	LPCSTR Skill_Desc_Interface;

};

/////////////////////////////////////////////////////////////////////
//�ƺ��б�						(MissionKind)
/////////////////////////////////////////////////////////////////////
#define DBC_MISSION_KIND			(911)
struct _DBC_MISSION_KIND
{
	INT		nIndex;					// �������������
	LPCSTR	szKindName;				// �������������
};

/////////////////////////////////////////////////////////////////////
// �������ɫת���ַ�
/////////////////////////////////////////////////////////////////////
#define DBC_STRING_DICT			( 912 )
struct _DBC_STRING_DICT
{
	INT		nID;			// ID
	LPCSTR  szKey;			// �ؼ���
	LPCSTR  szString;		// �ַ���
};


/////////////////////////////////////////////////////////////////////
// ������Ч�б�
/////////////////////////////////////////////////////////////////////
#define DBC_ENV_SOUND			( 913 )
struct _DBC_ENV_SOUND
{
	INT		nID;			// ID
	UINT	nSoundID;		// ��ЧID
	UINT	nXPos;			// ��Դ��X����
	UINT	nZPos;			// ��Դ��Z����
	UINT	nDistance;		// ������������������
};

/////////////////////////////////////////////////////////////////////
// ��Ʒ�����
/////////////////////////////////////////////////////////////////////
#define DBC_ITEM_RULE			( 914 )
struct _DBC_ITEM_RULE
{
	INT		nID;			// ID
	INT		bCanDrop;		// �Ƿ�ɶ���
	INT		bCanOverLay;	// �Ƿ���ص�
	INT		bCanQuick;		// �Ƿ�ɷ�������
	INT		bCanSale;		// �Ƿ���Գ��۸�NPC�̵�
	INT		bCanExchange;	// �Ƿ���Խ���
	INT		bCanUse;		// �Ƿ����ʹ��
	INT		bPickBind;		// �Ƿ���ʰȡ�
	INT		bEquipBind;		// �Ƿ���װ���
	INT		bUnique;		// �Ƿ���Ψһ
};

/////////////////////////////////////////////////////////////////////
// ���˴ʴʻ��
/////////////////////////////////////////////////////////////////////
#define DBC_TALK_FILTER			( 915 )
struct _DBC_TALK_FILTER
{
	INT		nID;			// ID
	LPCSTR	szString;		// ���˴ʻ㣨��������˵�Ĵʻ㣩
};

/////////////////////////////////////////////////////////////////////
// �ȼ���Ǯ��Ӧ��
/////////////////////////////////////////////////////////////////////
#define DBC_LV_MAXMONEY			( 916 )
struct _DBC_LV_MAXMONEY
{
	INT		nID;			//ID
	INT		nLv;			//�ȼ�
	INT		nMaxMoney;		//����Ǯ
};

/////////////////////////////////////////////////////////////////////
// ���춯�������
/////////////////////////////////////////////////////////////////////
#define DBC_TALK_ACT			( 918 )
struct _DBC_TALK_ACT
{
	INT		nID;			//ID
	LPCSTR	szCmd;			//����
	LPCSTR	szNobody;		//û��Ŀ��ʱ���ִ�
	LPCSTR	szMyself;		//Ŀ���Լ�ʱ���ִ�
	LPCSTR	szOther;		//Ŀ�����������ʱ���ִ�
	LPCSTR	szAct;			//�յ�������Ϣʱ��Ҫ���Ķ������

	LPCSTR	szIconName;		//ͼ��
	LPCSTR	szToolTip;		//��ʾ��Ϣ
};

/////////////////////////////////////////////////////////////////////
// �����������ñ�
/////////////////////////////////////////////////////////////////////
#define DBC_TALK_CONFIG			( 919 )
struct _DBC_TALK_CONFIG
{
	INT		nID;			//ID ����ENUM_CHAT_TYPE
	LPCSTR	szChannelName;	//Ƶ������
	LPCSTR	szChannelHeader;	//Ƶ��ǰ���ַ�
	INT		nTimeSlice;		//ʱ��������λ���룩
	INT		nNeedType1;		//��������1 ����CHAT_NEED_TYPE
	INT		nNeedValue1;	//������ֵ1
	INT		nNeedType2;		//��������2 ����CHAT_NEED_TYPE
	INT		nNeedValue2;	//������ֵ2
	INT		nNeedType3;		//��������3 ����CHAT_NEED_TYPE
	INT		nNeedValue3;	//������ֵ3
	INT		nNeedLv;		//�ȼ����ƣ������Ƶ�����Ͷ��ټ������ڴ�Ƶ������
};

/////////////////////////////////////////////////////////////////////
// ���͸ı����ı�
/////////////////////////////////////////////////////////////////////
#define DBC_HAIR_STYLE			( 920 )
struct _DBC_HAIR_STYLE
{
	INT		nID;			//ID����
	INT		nRaceID;		//�Ա�����
	INT		nItemTableID;	//������ƷID
	INT		nItemCount;		//������Ʒ����
};

/////////////////////////////////////////////////////////////////////
// ����ͷ�����ݱ�
/////////////////////////////////////////////////////////////////////
#define DBC_MONSTER_PAOPAO		( 921 )
struct _DBC_MONSTER_PAOPAO
{
	INT		nID;			//ID����
	LPCSTR	szPaoPaoTxt;	//����Ҫ˵��ͷ������
};


#define DBC_CAMP_AND_STAND		(922)	//��Ӫ


/////////////////////////////////////////////////////////////////////
//������Ϣ��ɫ��
/////////////////////////////////////////////////////////////////////
#define DBC_SYSTEM_COLOR		(923)
struct _DBC_SYSTEM_COLOR
{
	const static INT NAME_PLAYER_MYSELF		 = 1;	//����_����
	const static INT NAME_PLAYER_OTHER		 = 2;	//����_�������
	const static INT NAME_NPC_PEACE			 = 3;	//����_��ƽNPC
	const static INT NAME_NPC_ATTACK_ACTIVE	 = 4;	//����_��������NPC
	const static INT NAME_NPC_ATTACK_PASS	 = 5;	//����_����������NPC
	const static INT NAME_NPC_CANNT_ATTACK	 = 6;	//���ɹ�����NPC
	const static INT NAME_PET				 = 7;	//��������
	const static INT NANE_SELF_PET			 = 8;	//�Լ��ĳ���

	const static INT NANE_ISACK_NOTRETORT_PLAYER	 = 10;	//���Թ���_�����������
	const static INT NANE_ISACK_ISTRETORT_PLAYER	 = 11;	//���Թ���_�ỹ�������
	const static INT NANE_NOTACK_ISTRETORT_PLAYER	 = 12;	//���ɹ���_�ỹ�������
	const static INT NANE_NOTACK_NOTRETORT_PLAYER	 = 13;	//���ɹ���_�����������
	const static INT NANE_ISACK_NOTRETORT_MONSTER	 = 14;	//�ɹ�������������������
	const static INT NANE_ISACK_ISRETORT_MONSTER	 = 15;	//�ɹ������ỹ����������
	const static INT NANE_ISACK_ISACK_MONSTER		 = 16;	//�ɹ������ỹ����������
	const static INT NANE_NOTACK_ISACK_MONSTER		 = 17;	//���ɹ������ỹ����������
	const static INT NANE_NOTACK_NOTRETORT_MONSTER	 = 18;	//���ɹ��������ỹ������������

	const static INT TITLE_NORMAL			 = 100;	//�ƺ�_��ͨ
	const static INT TITLE_BANGPAI			 = 101;	//�ƺ�_����ְ��
	const static INT TITLE_MOOD				 = 102;	//�ƺ�_����Զ�������
	const static INT TITLE_PLAYER_SHOP		 = 103;	//�ƺ�_����̵�

	const static INT UI_DEBUGLISTBOX		 = 1000; //debuglistbox����ɫ

	INT		m_nID;
	LPCSTR  strDesc;
	LPCSTR	strFontColor;
	LPCSTR	strExtenColor;
};

/////////////////////////////////////////////////////////////////////
// ��ȫƥ����˱����ڴ�����ᡢ������֮��ĵط�������ʹ��ϵͳ����
/////////////////////////////////////////////////////////////////////
#define DBC_FULLCMP_FILTER		( 924 )
struct _DBC_FULLCMP_FILTER
{
	INT		nID;			//ID����
	LPCSTR	szFilterType;	//����ʲô�ط������磺all,guild,team,.....��ʾ�������еط�������ᣬ����ӵȵȡ�
	LPCSTR	szFullTxt;		//��ȫ���δ�
};

/////////////////////////////////////////////////////////////////////
// ����������Ҫ�ľ����
/////////////////////////////////////////////////////////////////////
#define DBC_PET_LEVELUP			( 925 )
struct _DBC_PET_LEVELUP
{
	INT		nID;			//ID����
	INT		nExp;			//����Ҫ����
};

