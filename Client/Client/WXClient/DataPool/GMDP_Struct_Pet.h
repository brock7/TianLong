#include "Type.h"
#include "DB_Struct.h"
#pragma once

//�������ɼ���ѧϰʱ��ֻ�г��＼�ܺţ�û�г����ţ�������-4444��ʾ������
#define MENPAI_PETSKILLSTUDY_PETNUM	-4444

//-------------------------------------------
//	1) ���＼�����ݽṹ
//-------------------------------------------
struct PET_SKILL
{
	const _DBC_SKILL_DATA		*m_pDefine;		//���ܱ���
	INT							m_nPetNum;		//�ڼ�ֻ����ļ���
	INT							m_nPosIndex;	//�ڼ������� (UIͬ��)
	BOOL						m_bCanUse;		//�Ƿ���ã����ر�������

	VOID CleanUp( VOID )
	{
		m_pDefine		= NULL;
		m_nPetNum		= -1;
		m_nPosIndex		= -1;
		m_bCanUse		= FALSE;
	}
};

//-------------------------------------------
//	�������ݽṹ
//------------------------------------------
struct SDATA_PET
{
public:
	SDATA_PET()
	{
		m_nIsPresent	= -1;
		m_GUID.Reset();
		m_idServer		= INVALID_ID;
		m_nDataID		= INVALID_ID;
		m_nAIType		= -1;
		m_szName		= "";
		m_nLevel		= -1;
		m_nExp			= -1;
		m_nHP			= -1;
		m_nHPMax		= -1;
		m_nAge			= -1;
		m_nEraCount		= -1;
		m_nHappiness	= -1;
		m_SpouseGUID.Reset();
		m_nModelID		= INVALID_ID;
		m_nMountID		= INVALID_ID;
		m_nAttPhysics	= -1;
		m_nAttMagic		= -1;
		m_nDefPhysics	= -1;
		m_nDefMagic		= -1;
		m_nHit			= -1;
		m_nMiss			= -1;
		m_nCritical		= -1;
		m_nAttrStrApt	= -1;
		m_nAttrConApt	= -1;
		m_nAttrDexApt	= -1;
		m_nAttrSprApt	= -1;
		m_nAttrIntApt	= -1;
		m_nAttrStr		= -1;
		m_nAttrCon		= -1;
		m_nAttrDex		= -1;
		m_nAttrSpr		= -1;
		m_nAttrInt		= -1;
		m_nBasic		= -1;
		m_nPot			= -1;
		m_aSkill.resize(PET_MAX_SKILL_COUNT, NULL);
	}

	~SDATA_PET()
	{
		for(INT i = 0; i<(INT)m_aSkill.size(); i++ )
		{
			if(m_aSkill[i] != NULL)
				SAFE_DELETE(m_aSkill[i]);
		}
	}

public:
	INT			m_nIsPresent;	// �����Ƿ����
								// -1:������
								// 1:����	��ֻ֪�����֣�
								// 2:������֪��֪����ϸ��Ϣ��

	PET_GUID_t	m_GUID;			// �����GUID
	ObjID_t		m_idServer;		// server�˵�objID

	INT			m_nDataID;		// ���ݱ��е�ID
	INT			m_nAIType;		// �Ը�

	STRING		m_szName;		// ����
	INT			m_nLevel;		// �ȼ�
	INT 		m_nExp;			// ����
	INT 		m_nHP;			// Ѫ��ǰֵ
	INT 		m_nHPMax;		// Ѫ���ֵ

	INT 		m_nAge;			// ��ǰ����
	INT 		m_nEraCount;	// ������
	INT 		m_nHappiness;	// ���ֶ�

	PET_GUID_t	m_SpouseGUID;	// ��ż

	INT			m_nModelID;		// ����
	INT			m_nMountID;		// ����ID

	INT 		m_nAttPhysics;	// ��������
	INT 		m_nAttMagic;	// ħ��������
	INT 		m_nDefPhysics;	// ���������
	INT 		m_nDefMagic;	// ħ��������

	INT 		m_nHit;			// ������
	INT 		m_nMiss;		// ������
	INT			m_nCritical;	// ������

	INT			m_nAttrStrApt;	// ��������
	INT			m_nAttrConApt;	// ��������
	INT			m_nAttrDexApt;	// ������
	INT			m_nAttrSprApt;	// ��������
	INT			m_nAttrIntApt;	// ��������

	INT			m_nAttrStr;		// ����
	INT			m_nAttrCon;		// ����
	INT			m_nAttrDex;		// ��
	INT			m_nAttrSpr;		// ����
	INT			m_nAttrInt;		// ����
	INT			m_nBasic;		// ����

	INT			m_nPot;			// Ǳ�ܵ�

	std::vector< PET_SKILL* >			m_aSkill;	// �����б�

public:
	VOID CleanUp( VOID )
	{
		m_nIsPresent	= -1;
		m_GUID.Reset();
		m_idServer		= INVALID_ID;
		m_nDataID		= INVALID_ID;
		m_nAIType		= -1;
		m_szName		= "";
		m_nLevel		= -1;
		m_nExp			= -1;
		m_nHP			= -1;
		m_nHPMax		= -1;
		m_nAge			= -1;
		m_nEraCount		= -1;
		m_nHappiness	= -1;
		m_SpouseGUID.Reset();
		m_nModelID		= INVALID_ID;
		m_nMountID		= INVALID_ID;
		m_nAttPhysics	= -1;
		m_nAttMagic		= -1;
		m_nDefPhysics	= -1;
		m_nDefMagic		= -1;
		m_nHit			= -1;
		m_nMiss			= -1;
		m_nCritical		= -1;
		m_nAttrStrApt	= -1;
		m_nAttrConApt	= -1;
		m_nAttrDexApt	= -1;
		m_nAttrSprApt	= -1;
		m_nAttrIntApt	= -1;
		m_nAttrStr		= -1;
		m_nAttrCon		= -1;
		m_nAttrDex		= -1;
		m_nAttrSpr		= -1;
		m_nAttrInt		= -1;
		m_nBasic		= -1;
		m_nPot			= -1;

		for(INT i = 0; i<(INT)m_aSkill.size(); i++ )
		{
			if(m_aSkill[i] != NULL)
				m_aSkill[i]->CleanUp();
		}
		//m_aSkill.clear();
		//m_aSkill.resize(PET_MAX_SKILL_COUNT, NULL);
	}

};