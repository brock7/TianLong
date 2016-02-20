// GameStruct_Pet.h

#ifndef __GAMESTRUCT_PET_H__
#define __GAMESTRUCT_PET_H__

#include "Type.h"
#include "GameDefine2.h"
#include "DB_Struct.h"

////////////////////////////////////////////////
// ���ӳ������Զ���
////////////////////////////////////////////////
enum ENUM_PET_FOOD_TYPE
{
	PET_FOOD_TYPE_INVALID = 0,
	PET_FOOD_TYPE_MEAT,				//��ʳ����
	PET_FOOD_TYPE_GRASS,			//�������
	PET_FOOD_TYPE_WORM,				//�������
	PET_FOOD_TYPE_PADDY,			//�������

	PET_FOOD_TYPE_NUMBER,
};

struct PET_ATTR
{
	INT		m_Type;						// ������
	CHAR	m_Name[NPC_NAME_LEN];		// ����
	INT		m_nPetPhyle;				// ����
	INT		m_TakeLevel;				// ��Я���ȼ�
	INT		m_bVarPet;					// �Ƿ�Ϊ�������
	INT		m_bBabyPet;					// �Ƿ�Ϊ��������
	INT		m_AI;						// �����Ը�
	INT		m_FoodType;					// ʳ����
	INT		m_PassiveSkillCount;		// �Զ�������
	INT		m_VoluntarySkill;			// �ֶ�����
	INT		m_PassiveSkill1;			// �Զ�����1
	INT		m_PassiveSkill2;			// �Զ�����2
	INT		m_PassiveSkill3;			// �Զ�����3
	INT		m_PassiveSkill4;			// �Զ�����4
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
	INT		m_ProcreateInterval;		// ���ﷱֳʱ��
};

////////////////////////////////////////////////
// _PET_DETAIL_ATTRIB ��������
////////////////////////////////////////////////
struct _PET_DETAIL_ATTRIB
{
public:
	_PET_DETAIL_ATTRIB(VOID);

	BOOL Init(const _PET_DB *pPetDB,
		ObjID_t idObj,
		INT nAtt_Physics,
		INT nAtt_Magic,
		INT nDef_Physics,
		INT nDef_Magic,
		INT nHit,
		INT nMiss,
		INT nCritical,
		INT nModelID,
		INT nMountID);

	VOID CleanUp(VOID);

public:
	PET_GUID_t		m_GUID;							// ID

	ObjID_t			m_ObjID;						// ����Obj���͵�ObjID
	INT				m_nDataID;						// ģ��ID,��������
	CHAR			m_szName[MAX_CHARACTER_NAME];	// ����
	INT				m_nAIType;						// �Ը�
	PET_GUID_t		m_SpouseGUID;					// ��ż��GUID
	INT				m_nLevel;						// �ȼ�
	INT				m_nExp;							// ����
	INT				m_nHP;							// Ѫ��ǰֵ
	INT				m_nHPMax;						// Ѫ���ֵ

	INT				m_nLife;						// ��ǰ����
	BYTE			m_byGeneration;					// ������
	BYTE			m_byHappiness;					// ���ֶ�

	INT				m_nAtt_Physics;					// ��������
	INT				m_nAtt_Magic;					// ħ��������
	INT				m_nDef_Physics;					// ���������
	INT				m_nDef_Magic;					// ħ��������

	INT				m_nHit;							// ������
	INT				m_nMiss;						// ������
	INT				m_nCritical;					// ������

	INT				m_nModelID;						// ����
	INT				m_nMountID;						// ����ID

	INT				m_StrPerception;				// ��������
	INT				m_ConPerception;				// ��������
	INT 			m_DexPerception;				// ������
	INT				m_SprPerception;				// ��������
	INT 			m_IntPerception;				// ��������

	INT				m_Str;							// ����
	INT				m_Con;							// ����
	INT 			m_Dex;							// ��
	INT				m_Spr;							// ����
	INT 			m_Int;							// ����
	INT 			m_GenGu;						// ����

	INT				m_nRemainPoint;					// Ǳ�ܵ�

	_OWN_SKILL		m_aSkill[MAX_PET_SKILL_COUNT];	// �����б�
};


////////////////////////////////////////////////
// _PET_PLACARD_ITEM ���﹫����һ��
////////////////////////////////////////////////

// �ͻ��˳��﹫������ʾ����
#define MAX_PETPLACARD_LIST_ITEM_NUM		(2)

#define PET_PLACARD_ITEM_MESSAGE_SIZE		(64)

struct _PET_PLACARD_ITEM
{
public:
	_PET_PLACARD_ITEM(VOID);
	~_PET_PLACARD_ITEM(VOID);

	BOOL Init(GUID_t HumanGUID,
				const CHAR *pszHumanName,
				INT nHumanLevel,
				const CHAR *pszHumanGuildName,
				INT nHumanMenPai,
				const _PET_DETAIL_ATTRIB *pPetAttr,
				const CHAR *pszMessage,
				UINT uCreateTime);

	VOID CleanUp(VOID);

	BOOL IsInvalid(VOID)const
	{
		return (m_HumanGUID == INVALID_ID)?(TRUE):(FALSE);
	}

public:
	UINT GetCreateTime(VOID)const
	{
		return m_uCreateTime;
	}

	GUID_t GetHumanGUID(VOID)const
	{
		return m_HumanGUID;
	}

	const CHAR *GetHumanName(VOID)const
	{
		return m_szHumanName;
	}

	INT GetHumanLevel(VOID)const
	{
		return m_nHumanLevel;
	}

	const CHAR *GetHumanGuildName(VOID)const
	{
		return m_szHumanGuildName;
	}

	const _PET_DETAIL_ATTRIB *GetPetAttr(VOID)const
	{
		return &m_PetAttr;
	}

	const CHAR *GetMessage(VOID)const
	{
		return m_szMessage;
	}

	const INT GetHumanMenPai(VOID) const
	{
		return m_nHumanMenPai;
	}

protected:
	// ������Ϣ
	UINT				m_uCreateTime;

	// ������Ϣ
	GUID_t				m_HumanGUID;
	CHAR				m_szHumanName[MAX_CHARACTER_NAME];
	INT					m_nHumanLevel;
	CHAR				m_szHumanGuildName[MAX_GUILD_NAME_SIZE];
	INT					m_nHumanMenPai;

	// ������Ϣ
	_PET_DETAIL_ATTRIB	m_PetAttr;

	// ����
	CHAR				m_szMessage[PET_PLACARD_ITEM_MESSAGE_SIZE];
};

#endif // __GAMESTRUCT_PET_H__
