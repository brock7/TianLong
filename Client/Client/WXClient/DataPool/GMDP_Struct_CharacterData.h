/****************************************\
*										*
* 			   ���ݳ����ݽṹ			*
*					-��ɫ����			*
*										*
\****************************************/
#pragma once

//========================================================
//
//			��ɫ��������ȫ�ɷ�������������������
//
//========================================================
#include "GameDefine.h"
#include "GameDefine2.h"
#include "TDDBC_Struct.h"
#include "DB_Struct.h"
#include "GameStruct_Skill.h"

#include "GMDP_Struct_Skill.h"
#include "GMDP_Struct_Title.h"
#include "GMDP_Struct_Pet.h"

//=================================================================
//-->0. Character ��������
//=================================================================
struct SDATA_CHARACTER
{
	INT				m_nRaceID;					// ���ݱ��е�ID						

	INT				m_nPortraitID;				// ͷ��ID		
	STRING			m_strName;					// ��ɫ�����֣��ӷ���������			
	STRING			m_strTitle;					//��ǰ�ƺ�
	BYTE			m_TitleType;				//��ǰ�ƺ�����
	FLOAT			m_fHPPercent;				// ����ֵ�ٷֱ�						
	FLOAT			m_fMPPercent;				// ħ��ֵ�ٷֱ�						
	INT				m_nRage;					// ŭ��
	FLOAT			m_fMoveSpeed;				// �ƶ����ٶ�						
	_CAMP_DATA		m_CampData;					// ��ӪID	
	INT				m_nOwnerID;					// ������ID
	GUID_t			m_OccupantGUID;				// ռ����(��˭��ģ�������˭��)
	INT				m_nRelative;				// ��Թ�ϵ��������Ƿ�ɱ��������Ƿ���������
	INT				m_nModelID;					// ģ��ID
	INT				m_nMountID;					// ����ID
	INT				m_nLevel;					// �ȼ�								
	BOOL			m_bFightState;				// ս��״̬							
	INT				m_nStealthLevel;			// ������
	BOOL			m_bSit;						// ����״̬
	INT				m_nAIType;					// AI����
};

//=================================================================
//-->1. Character_NPC ����NPC
//=================================================================
struct SDATA_NPC : public SDATA_CHARACTER
{

};

//=================================================================
//-->2. Character_PlayerOther �����������
//=================================================================
struct SDATA_PLAYER_OTHER : public SDATA_NPC
{
	INT				m_nMenPai;					// ����ID									
												/*
												|	0������
												|	1������
												|	2��ؤ��
												|	3���䵱
												|	4������
												|	5������
												|	6������
												|	7����ɽ
												|	8����ң
												|	9��������
												*/
	//ͷ������
	INT			m_nHairMeshID;					//-> DBC_CHARACTER_HAIR_GEO					
	//��������
	INT			m_nFaceMeshID;					//-> DBC_CHARACTER_HEAD_GEO					
	UINT		m_uHairColor;					//ͷ����ɫ
	INT			m_nEquipVer;					//��ɫ��װ��״̬�汾�ţ����ںͷ�����ͬ�����ݵ�����		
	INT			m_nEquipmentID[HEQUIP_NUMBER];	//װ����;����������ʾ				
	INT			m_nEquipmentGemID[HEQUIP_NUMBER];	//װ����ʯ��;����������ʾ				

	BOOL		m_bTeamFlag;					// �Ƿ��ж���
	BOOL		m_bTeamLeaderFlag;				// �Ƿ��Ƕӳ�
	BOOL		m_bTeamFullFlag;				// �Ƿ�������
	BOOL		m_bTeamFollowFlag;				// �Ƿ���Ӹ���
	INT			m_nTeamFollowListCount;			// �����б��������
	ObjID_t		m_aTeamFollowList[MAX_TEAM_MEMBER];	// �����б�
	
	INT			m_nTitleNum;
	INT			m_nCurTitleIndex;
	_TITLE_		m_nTitleList[MAX_TITLE_SIZE];	//�ƺű�
	BOOL		m_bIsInStall;					//�Ƿ��̯��
	STRING		m_strStallName;					//̯λ��			
};


//=================================================================
//-->3. Character_PlayerMySelf ��������Լ�
//=================================================================

class tObject_Item;
struct SDATA_PLAYER_MYSELF : public SDATA_PLAYER_OTHER
{
	//-----------------------------------------------------
	INT				m_nHP;				// ������						
	INT				m_nMP;				// ħ����						
	INT				m_nExp;				// ����ֵ						
	//INT			m_nMaxExp;			// ����ֵ����(���ݵȼ������)					
	INT				m_nMoney;			// ��Ϸ����	
	INT				m_nStrikePoint;		// ������
	INT				m_nRMB;				//m_nVigor;			// ����ֵ
	//INT				m_nMaxVigor;		// ����ֵ����
	INT				m_nDoubleExpTime_Num;			// ˫������ʱ��ͱ���
	//INT				m_nMaxEnergy;		// ����ֵ����
	//INT				m_nGoodBadValue;	// �ƶ�ֵ
	PET_GUID_t		m_guidCurrentPet;	// ��ǰ����GUID

	//-----------------------------------------------------
	INT				m_nGuild;			// ����ID 

	//-----------------------------------------------------
	//һ��ս������
	INT				m_nSTR;				// �⹦							
	INT				m_nSPR;				// �ڹ�							
	INT				m_nCON;				// ��							
	INT				m_nINT;				// ����							
	INT				m_nDEX;				// �ǻ�							
	INT				m_nPoint_Remain;	// ʣ����������				

	//-----------------------------------------------------
	//����ս������
	INT				m_nAtt_Physics;			// ��������					
	INT				m_nAtt_Magic;			// ħ��������					
	INT				m_nDef_Physics;			// ���������					
	INT				m_nDef_Magic;			// ħ��������					
	INT				m_nMaxHP;				// ���������					
	INT				m_nMaxMP;				// ���ħ����					
	INT				m_nHP_ReSpeed;			// HP�ָ��ٶ�  ��/��			
	INT				m_nMP_ReSpeed;			// MP�ָ��ٶ�  ��/��			
	INT				m_nHit;					// ������						
	INT				m_nMiss;				// ������						
	INT				m_nCritRate;			// ������						
	//FLOAT			m_fMoveSpeed;			// �ƶ��ٶ�(�ڻ���������)					
	INT				m_nAttackSpeed;			// �����ٶ�						
	INT				m_nAtt_Cold;			// ������						
	INT				m_nDef_Cold;			// ������						
	INT				m_nAtt_Fire;			// �𹥻�
	INT				m_nDef_Fire;			// �����
	INT				m_nAtt_Light;			// �繥��
	INT				m_nDef_Light;			// �����
	INT				m_nAtt_Posion;			// ������
	INT				m_nDef_Posion;			// ������

	BOOL			m_bLimitMove;			//�Ƿ����Ʋ����ƶ�
	BOOL			m_bCanActionFlag1;		//�������ޱ��1,���ڻ��Դ���
	BOOL			m_bCanActionFlag2;		//�������ޱ��2,���ڳ�Ĭ
	//BOOL			m_bLimitUseSkill;		//�Ƿ����Ʋ���ʩ��
	//BOOL			m_bLimitHandle;			//�Ƿ����Ʋ��ܽ���һ�в���

	BOOL			m_bIsMinorPwdSetup;		// �Ƿ������ö�������
	BOOL			m_bIsMinorPwdUnlocked;	// �Ƿ��Ѿ�������������

	//-----------------------------------------------------
	//��������
	SKILLCLASS_MAP		m_theSkillClass;	//����ϵ
	SSKILL_MAP			m_theSkill;			//��������
	SLIFEABILITY_MAP	m_theLifeAbility;	//���������
	SPRESCR_MAP			m_theSprescr;		//�䷽����

	//��������
//	SDATA_PET			m_thePet;			//����
};
