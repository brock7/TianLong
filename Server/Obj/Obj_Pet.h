// Obj_Pet.h
//
// ���ܣ�������SERVER��ʵ��
// 
///////////////////////////////////////////////////////

#ifndef __PET_H__
#define __PET_H__

#include "Obj_Character.h"

//////////////////////////////////////////////////////
// ��ʼ��Obj_Pet�Ľṹ
//////////////////////////////////////////////////////
struct _OBJ_PET_INIT :
	public _OBJ_CHARACTER_INIT
{
	GUID_t			m_GUID;			// ��ɫ�ڳ����е�GUID����PetGUID��������
	_PET_DB			m_PetDB;
	ObjID_t			m_CreatorID;	// �����ߵ�ID�������������
	INT				m_nIndex;		// �����˳�����е�����
	INT				m_RespawnTime;
	ScriptID_t		m_ScriptID ;	// �󶨵Ľű�

	_OBJ_PET_INIT( VOID )
	{
		m_GUID			= INVALID_GUID;
		m_PetDB.CleanUp();
		m_CreatorID		= INVALID_ID;
		m_nIndex		= -1;
		m_RespawnTime	= -1;
		m_ScriptID		= INVALID_ID;
	}

	virtual VOID CleanUp( VOID )
	{
		m_GUID			= INVALID_GUID;
		m_PetDB.CleanUp();
		m_CreatorID		= INVALID_ID;
		m_nIndex		= -1;
		m_RespawnTime	= -1;
		m_ScriptID		= INVALID_ID;

		_OBJ_CHARACTER_INIT::CleanUp();
	}
};

//////////////////////////////////////////////////////
// ����ˢ�µ���һ�����Եı���ṹ
//////////////////////////////////////////////////////
struct _PET_ATTR_BACKUP
{
public:
	BOOL		m_bNameModified;
	BOOL		m_bTitleModified;
	INT			m_HP;
	INT			m_MP;
	INT			m_MaxHp;
	INT			m_MaxMp;
	FLOAT       m_MoveSpeed;
	_CAMP_DATA	m_CampData;
	ObjID_t		m_nOwnerID;
	PET_GUID_t	m_SpouseGUID;					// ��żGUID
	INT			m_nAIType;						// �Ը�
	INT			m_Level;
	INT			m_nStealthLevel;				// ������

	INT			m_nDataID;						// ����ID
	INT			m_nModelID;						// ����
	INT			m_nMountID;						// ����ID

	PET_GUID_t	m_GUID;							// ID
	INT			m_nLife;						// ��ǰ����
	BYTE		m_byGeneration;					// ������
	BYTE		m_byHappiness;					// ���ֶ�

	INT			m_nAtt_Physics;					// ��������
	INT			m_nAtt_Magic;					// ħ��������
	INT			m_nDef_Physics;					// ���������
	INT			m_nDef_Magic;					// ħ��������
	INT			m_nHit;							// ������
	INT			m_nMiss;						// ������
	INT			m_nCritical;					// ������

	INT			m_nExp;							// ����

	INT			m_StrPerception;				// ��������
	INT			m_ConPerception;				// ��������
	INT 		m_DexPerception;				// ������
	INT			m_SprPerception;				// ��������
	INT 		m_IntPerception;				// ��������
	INT			m_GenGu;						// ����9

	INT			m_Str;							// ����
	INT			m_Con;							// ����
	INT 		m_Dex;							// ��
	INT			m_Spr;							// ����
	INT 		m_Int;							// ����

	INT			m_nRemainPoint;					// Ǳ�ܵ�
	INT			m_nCampType;

public:
	_PET_ATTR_BACKUP( VOID );

	VOID CleanUp( VOID );
};



//////////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////////

#define MAX_CAPTURER_NUM (6)
#define LEVELDELTA_GETEXP (5)
#define SEND_LIFE_INTERVAL (30)

class AI_Pet;
class Obj_Pet :
	public Obj_Character
{
public:
	friend class AI_Pet;

//===========================================
// Obj���нӿڼ̳�
//===========================================
public:
	Obj_Pet( VOID );
	virtual ~Obj_Pet( VOID );

	virtual ObjType		GetObjType( VOID )const{ return OBJ_TYPE_PET; }
	virtual VOID 		CleanUp( );
	virtual BOOL 		Init( const _OBJ_INIT *pInit );
	virtual BOOL 		HeartBeat( UINT uTime = 0 );

	virtual VOID		InitAIObj( );
	virtual VOID		OnExecuteScriptTimer(UINT uTime) ;

	const _PET_DB*		GetPetDB(VOID) const { return &m_PetDB; }

//===========================================
// �����������
//===========================================
public:
	ScriptID_t			m_ScriptID ;//����󶨵Ľű�

//===========================================
// ��Ӫ���
//===========================================
public:
	virtual BOOL	IsEnemy(Obj_Character* pCharacter);
	virtual BOOL	IsFriend(Obj_Character* pCharacter);
	virtual BOOL	IsPartner(Obj_Character* pCharacter);

//===========================================
// ��Ϣ/����ˢ��
//===========================================
public:
	virtual Packet		*CreateNewObjPacket( VOID );
	virtual VOID		DestroyNewObjPacket( Packet *pPacket );

	virtual VOID 		InitBackupAttr( );
	virtual VOID 		SendMsg_RefeshAttrib( VOID );
	virtual VOID 		AskMsg_BaseAttrib( Obj_Human *pTargetHuman );
	virtual VOID 		AskMsg_DetailAttrib( Obj_Human *pTargetHuman );

protected:
	_PET_ATTR_BACKUP	m_AttrBackUp;

//===========================================
// ���ֶȱ仯����
//===========================================
public:
	virtual BOOL		HeartBeat_Happiness( UINT uTime = 0 );

protected:
	CMyTimer			m_HappinessTimer;		// ���ֶ�ÿ������ʱ���1

//===========================================
// �����仯����
//===========================================
public:
	virtual BOOL		HeartBeat_Life( UINT uTime = 0 );

protected:
	CMyTimer			m_LifeTimer;		// ���ֶ�ÿ������ʱ���1

//===========================================
// ��������
//===========================================
public:
	VOID				LevelUp(VOID);
	virtual VOID		SetReliveInfo( BOOL bSkillRelive, const _RELIVE_INFO *pReliveInfo );

//===========================================
// ����/�ķ�
//===========================================
public:
	virtual BOOL		Skill_HaveSkill( SkillID_t const nID, BYTE const nLevel ) const;

	// nIndex	:	ENUM_PET_SKILL_INDEX
	const _PET_SKILL	*Skill_GetSkill( INT nIndex ) const;

	INT 				Skill_GetCount_CortrolByPlayer(VOID);
	INT 				Skill_GetMaxCount_CortrolByPlayer(VOID);
	INT 				Skill_GetCount_CortrolByAI(VOID);
	INT 				Skill_GetMaxCount_CortrolByAI(VOID);

	BOOL 				Skill_RegisterSkill(SkillID_t const nID, INT nIndex);
	BOOL 				Skill_UnregisterSkill(SkillID_t const nID, INT nIndex);
	BOOL 				Skill_ModifySkill_Study(SkillID_t nID);
	BOOL 				Skill_ModifySkill_Realize(SkillID_t nID);
	BOOL 				Skill_ModifySkill_MenPai(SkillID_t nID);
	BOOL 				Skill_Apperceive(VOID);

	virtual INT			Skill_GetXinFaLevel(SkillID_t const nID) const;
	virtual BOOL 		Skill_HaveXinFa( SkillID_t const nID ) const;
	virtual BOOL 		Skill_SetXinFaLevel(SkillID_t const nID, INT const nLevel);
	virtual VOID 		Skill_OnEquipItem(VOID);
//===========================================
// Ч��
//===========================================
protected:
	virtual _IMPACT_LIST&	Impact_GetImpactList(VOID);

private:
	virtual VOID 			Impact_InitList(VOID);

private:
	_IMPACT_LIST			m_ImpactList;

//===========================================
// CoolDown
//===========================================
public:
	virtual Time_t		GetCooldownRemained(CooldownID_t const nID) const;
	virtual BOOL		IsCooldowned(CooldownID_t const nID) const;
	virtual VOID 		SetCooldown(CooldownID_t const nID, Time_t nCooldown);
	virtual BOOL 		HeartBeat_Cooldown(UINT uTime = 0);
	virtual BOOL 		HeartBeat_Recover(UINT uTime = 0);

protected:

//===========================================
// Capturer
//===========================================
public:
	INT					GetCaptureCount(VOID)const;

	BOOL 				AddCapturer(ObjID_t idObj); 
	BOOL 				DelCapturer(ObjID_t idObj);
	VOID 				CleanAllCapturer(VOID);
	VOID 				SendCaptureFailedToOthers(ObjID_t idObj);

protected:
	INT					m_nCapturerCount;	// ��ǰ���ڲ����Լ��������
	ObjID_t				m_aCapturer[MAX_CAPTURER_NUM];

//===========================================
// AI
//===========================================
public:
	virtual ObjID_t			GetOwnerID(VOID)const;
	virtual Obj_Character	*GetOwner( VOID );

	AI_Pet*					GetPetAI( VOID )const;
	VOID					SetTargetID( ObjID_t TargetID );
	ObjID_t					GetTargetID( VOID )const;
	const WORLD_POS*		GetRespawnPos( VOID )const;

	VOID					ToAttack( ObjID_t TargetID );
	BOOL					PushSkillToCache(INT idSkill);
	VOID					GetRateOfBuffTakeEffect(INT idSkill, INT& rateOfBuffTakeEffect);

protected:
	ObjID_t					m_OwnerID;			// ��ǰ�����ߵ�ObjID
	ObjID_t					m_TargetID;			// Ŀ��ID
	WORLD_POS				m_RespawnPos;
	INT						m_RespawnTime;
	// ֻ�������汻�����ܺͻ����ͼ���(���������Ч���ļ���)
	INT						m_aCacheOfSkill[HUMAN_PET_MAX_COUNT]; 
	INT						m_AIType;


//===========================================
// һ������
//===========================================
public:
	virtual GUID_t			GetGUID( VOID )const;

	Obj_Human				*GetCreator( VOID )const;
	const Obj_Human			*GetConstCreator( VOID )const;

	VOID					SetPetGUID(PET_GUID_t guid);
	PET_GUID_t				GetPetGUID(VOID)const;

	VOID					SetSpouseGUID(PET_GUID_t guid);
	PET_GUID_t				GetSpouseGUID(VOID)const;

	VOID					SetDataID(const UINT dataID);
	virtual UINT			GetDataID( VOID )const; 

	virtual	VOID			SetName( const CHAR *pszName );
	virtual const CHAR		*GetName( VOID )const;

	virtual	VOID			SetTitle( const CHAR *pszTitle );
	virtual const CHAR		*GetTitle( VOID )const;

	VOID					IncrementExp(INT nIncrement);
	virtual INT				GetExp( VOID )const;
	virtual VOID			SetExp( INT nExp );

	virtual VOID			SetHP( INT nHP );
	virtual INT				GetHP( VOID );

    virtual VOID			SetMP( INT nMP );
	virtual INT				GetMP( VOID );

	virtual INT				GetAIType( VOID )const;
	virtual VOID			SetAIType( INT nAIType );

	virtual INT				GetLevel( VOID )const;
	virtual VOID			SetLevel( INT nLevel );

	virtual INT				GetLife( VOID )const;
	virtual VOID			SetLife( INT nLife );

	virtual BYTE			GetGeneration( VOID )const;
	virtual VOID			SetGeneration( BYTE byGeneration );

	VOID 					IncrementHappyness(INT nIncrement);
	BYTE 					GetHappiness( VOID )const;
	VOID 					SetHappiness( BYTE byHappiness );

	INT						GetGenGu( VOID )const;
	VOID					SetGenGu( INT nGenGu );

	virtual INT 			GetStrPerception( VOID )const;
	virtual INT 			GetConPerception( VOID )const;
	virtual INT 			GetDexPerception( VOID )const;
	virtual INT 			GetSprPerception( VOID )const;
	virtual INT 			GetIntPerception( VOID )const;

	virtual INT				GetRemainPoint( VOID )const;
	virtual VOID			SetRemainPoint( INT nRemainPoint );

protected:
	GUID_t					m_GUID;				// ��ɫ�ڳ����е�GUID����PetGUID��������
	ObjID_t					m_CreatorID;		// �����ߵ�ObjID
	Obj_Human*				m_pCreator;			// ����Ĵ�����
	INT						m_nIndex;			// ��ʶ�ó�������ҳ�����еĵڼ���
	_PET_DB					m_PetDB;


//===========================================
// ��������
//===========================================
public:
	//StrikePoint
	virtual INT 				GetMaxStrikePoint(VOID);
	//Rage
	virtual INT   				GetMaxRage(VOID);
	//Attr1: Str
	virtual INT					GetStr(VOID);
	virtual INT					GetBaseStr(VOID);
	virtual VOID 				SetBaseStr(INT const nValue);
	//Attr1: Spr
	virtual INT					GetSpr(VOID);
	virtual INT					GetBaseSpr(VOID);
	virtual VOID				SetBaseSpr(INT const nValue);
	//Attr1: Con
	virtual INT					GetCon(VOID);
	virtual INT					GetBaseCon(VOID);
	virtual VOID				SetBaseCon(INT const nValue);
	//Attr1: Int
	virtual INT					GetInt(VOID);
	virtual INT					GetBaseInt(VOID);
	virtual VOID				SetBaseInt(INT const nValue);
	//Attr1: Dex
	virtual INT					GetDex(VOID);
	virtual INT					GetBaseDex(VOID);
	virtual VOID				SetBaseDex(INT const nValue);
	//HP
	virtual INT					GetMaxHP(VOID) ; 
	virtual INT 				GetBaseMaxHP(VOID);
	virtual VOID 				SetBaseMaxHP(INT const nHp);
	virtual INT					GetHPRegenerate(VOID) ; 
	virtual INT 				GetBaseHPRegenerate(VOID);
	virtual VOID 				SetBaseHPRegenerate(INT const nValue);

	//MP
	virtual INT 				GetMaxMP(VOID);
	virtual INT 				GetBaseMaxMP(VOID);
	virtual VOID 				SetBaseMaxMp(INT const nMp);
	virtual INT					GetMPRegenerate(VOID); 
	virtual INT 				GetBaseMPRegenerate(VOID);
	virtual VOID 				SetBaseMPRegenerate(INT const nValue);
	//CampID
	virtual const _CAMP_DATA	*GetCampData(VOID);
	virtual const _CAMP_DATA	*GetBaseCampData(VOID)const;
	virtual VOID				SetBaseCampData(const _CAMP_DATA *pCampData);
	virtual INT 				GetCampID(VOID);
	virtual INT 				GetBaseCampID(VOID) const;
	virtual VOID				SetBaseCampID(INT const nID);
	//modelID
	virtual INT 				GetModelID(VOID);
	virtual INT 				GetBaseModelID(VOID) const;
	virtual VOID				SetBaseModelID(INT const nID);
	//Alive flag
	//In Combat flag
	// Can move flag
	// Can Action flag
	// Unbreakable flag
	// Attr2: Move Speed
	virtual FLOAT				GetMoveSpeed(VOID);
	virtual INT					GetBaseMoveSpeed(VOID);
	virtual VOID				SetBaseMoveSpeed(INT const nValue);
	// Attr2: Attack Speed
	virtual INT 				GetAttackSpeed(VOID);
	virtual INT 				GetBaseAttackSpeed(VOID);
	virtual VOID				SetBaseAttackSpeed(INT const nValue);
	// Attr2: Miss
	virtual INT 				GetMiss(VOID);
	virtual INT 				GetBaseMiss(VOID);
	virtual VOID				SetBaseMiss(INT const nValue);
	// Attr2 Hit
	virtual INT 				GetHit(VOID);
	virtual INT 				GetBaseHit(VOID);
	virtual VOID				SetBaseHit(INT const nValue);
	// Attr2 Critical
	virtual INT 				GetCritical(VOID);
	virtual INT 				GetBaseCritical(VOID);
	virtual VOID				SetBaseCritical(INT const nValue);
	// Attr2: Attack and Defence power physics
	virtual INT 				GetAttackPhysics(VOID);
	virtual INT 				GetBaseAttackPhysics(VOID);
	virtual VOID				SetBaseAttackPhysics(INT const nValue);
	virtual INT 				GetDefencePhysics(VOID);
	virtual INT 				GetBaseDefencePhysics(VOID);
	virtual VOID				SetBaseDefencePhysics(INT const nValue);
	// Attr2: Attack and Defence power of Magic
	virtual INT 				GetAttackMagic(VOID);
	virtual INT 				GetBaseAttackMagic(VOID);
	virtual VOID				SetBaseAttackMagic(INT const nValue);
	virtual INT 				GetDefenceMagic(VOID);
	virtual INT 				GetBaseDefenceMagic(VOID);
	virtual VOID				SetBaseDefenceMagic(INT const nValue);
	// Attr2: Attack and Defence power of Cold
	virtual INT 				GetAttackCold(VOID);
	virtual INT 				GetBaseAttackCold(VOID);
	virtual VOID				SetBaseAttackCold(INT const nValue);
	virtual INT 				GetDefenceCold(VOID);
	virtual INT 				GetBaseDefenceCold(VOID);
	virtual VOID				SetBaseDefenceCold(INT const nValue);
	// Attr2: Attack and Defence power of Fire
	virtual INT 				GetAttackFire(VOID);
	virtual INT 				GetBaseAttackFire(VOID);
	virtual VOID				SetBaseAttackFire(INT const nValue);
	virtual INT 				GetDefenceFire(VOID);
	virtual INT 				GetBaseDefenceFire(VOID);
	virtual VOID				SetBaseDefenceFire(INT const nValue);
	// Attr2: Attack and Defence power of Light
	virtual INT 				GetAttackLight(VOID);
	virtual INT 				GetBaseAttackLight(VOID);
	virtual VOID				SetBaseAttackLight(INT const nValue);
	virtual INT 				GetDefenceLight(VOID);
	virtual INT 				GetBaseDefenceLight(VOID);
	virtual VOID				SetBaseDefenceLight(INT const nValue);
	// Attr2: Attack and Defence power of Poison
	virtual INT 				GetAttackPoison(VOID);
	virtual INT 				GetBaseAttackPoison(VOID);
	virtual VOID				SetBaseAttackPoison(INT const nValue);
	virtual INT 				GetDefencePoison(VOID);
	virtual INT 				GetBaseDefencePoison(VOID);
	virtual VOID				SetBaseDefencePoison(INT const nValue);
	// Attr2 Reduce Slower Duration
	virtual INT 				GetReduceSlowerDuration(VOID);
	// Attr2 Reduce Weaken Duration
	virtual INT 				GetReduceWeakenDuration(VOID);
	// Attr2 Reduce Faint Duration
	virtual INT 				GetReduceFaintDuration(VOID);
	// Attr2 Reduce Poisoned Duration
	virtual INT 				GetReducePoisonedDuration(VOID);
	// Attr2 VisionRange
	virtual INT 				GetVisionRange(VOID);
	virtual INT 				GetBaseVisionRange(VOID);
	virtual VOID				SetBaseVisionRange(INT const nValue);
};

#endif	// __PET_H__
