

#ifndef __HUMANDB_H__
#define __HUMANDB_H__

#include "Type.h"
#include "DB_Struct.h"

#define			GetDBStr(db)			(db)->GetDBAttrLvl1(CATTR_LEVEL1_STR)
#define			GetDBSpr(db)			(db)->GetDBAttrLvl1(CATTR_LEVEL1_SPR)
#define			GetDBCon(db)			(db)->GetDBAttrLvl1(CATTR_LEVEL1_CON)
#define			GetDBInt(db)			(db)->GetDBAttrLvl1(CATTR_LEVEL1_INT)
#define			GetDBDex(db)			(db)->GetDBAttrLvl1(CATTR_LEVEL1_DEX)

#define			SetDBStr(db,v)			(db)->SetDBAttrLvl1(CATTR_LEVEL1_STR,v)
#define			SetDBSpr(db,v)			(db)->SetDBAttrLvl1(CATTR_LEVEL1_SPR,v)
#define			SetDBCon(db,v)			(db)->SetDBAttrLvl1(CATTR_LEVEL1_CON,v)
#define			SetDBInt(db,v)			(db)->SetDBAttrLvl1(CATTR_LEVEL1_INT,v)
#define			SetDBDex(db,v)			(db)->SetDBAttrLvl1(CATTR_LEVEL1_DEX,v)



class HumanDB
{

		
#define			N_SM_LOCK(type)						if(m_HumanSMU){SM_LOCK(m_HumanSMU->m_SMUHead.flag,type)};
#define			N_SM_UNLOCK(type)					if(m_HumanSMU){SM_UNLOCK(m_HumanSMU->m_SMUHead.flag,type)};

public :
	HumanDB( ) ;
	~HumanDB( ) ;

	VOID					CleanUp( ) ;
	VOID					ValidateShareMem(BOOL bForceAll,BOOL& bUpdateAttr);
	BOOL					IsNoDirectAttr(CHAR_ATTR_DB attrType);
	VOID					ValidatePlayerID(PlayerID_t	PID);
	inline VOID				Lock();
	inline VOID				UnLock();

	/*=========================================
	 *	�������Է��ʷ���
	 ===========================================*/
	inline	GUID_t				GetGUID()	const;
	inline	VOID				SetGUID( GUID_t guid );
	inline	INT					GetPortraitID() const;
	inline	VOID				SetPortraitID( INT nID );
	inline	const	CHAR*		GetName()const;
	inline	BOOL				GetSex();
	inline	INT					GetHP();
	inline	VOID				SetHP( INT hp );
	inline	INT					GetMP();
	inline	VOID				SetMP( INT mp );
	inline	INT					GetStrikePoint(VOID);
	inline	VOID				SetStrikePoint(INT nStrikePoint);
	inline	INT					GetRMB(VOID);
	inline	VOID				SetRMB(INT nMoney);
	//inline	INT					GetVigorRegeneRate(VOID);
	//inline	VOID				SetVigorRegeneRate(INT nRate);
	inline	INT					GetDoubleExpTime_Num(VOID);
	inline	VOID				SetDoubleExpTime_Num(INT nExpTime_Num);
	inline	INT					GetGmRight(VOID);
	inline	VOID				SetGmRight(INT nRight);
	//inline	INT					GetEnergyRegeneRate(VOID);
	//inline	VOID				SetEnergyRegeneRate(INT nRate);
	inline	INT 				GetRage( VOID );
	inline	VOID				SetRage( INT nRage );
	inline	BYTE				GetLevel();
	inline	VOID				SetLevel( BYTE level );
	inline	INT					GetPKValue( VOID )const;
	inline	VOID				SetPKValue( INT nValue );
	inline	PET_GUID_t			GetCurrentPetGUID( VOID )const;
	inline	VOID				SetCurrentPetGUID( PET_GUID_t guid );
	inline	UINT				GetExp();
	inline	VOID				SetExp( UINT exp );
	inline	INT					GetMenPai( );
	inline	VOID				SetMenPai( INT MenPai );
	inline	UINT				GetMoney();
	inline	VOID				SetMoney(UINT money);
	inline	_PLAYERSHOP_GUID	GetShopGuid(INT nIndex);									
	inline	VOID				SetShopGuid( INT nIndex, _PLAYERSHOP_GUID pGuid );
	inline	_PLAYERSHOP_GUID	GetFavoriteShopGuid(INT nIndex);
	inline	VOID				SetFavoriteShopGuid( INT nIndex, _PLAYERSHOP_GUID pGuid );
	inline	const CHAR*			GetPasswd();
	inline	VOID				SetPasswd( const CHAR* szPasswd );
	inline	VOID				DelPasswd();
	inline	UINT				GetPasswdDeleteTime();
	inline	VOID				SetPasswdDeleteTime( UINT uTime );
	//inline	BYTE				GetXinfaLevel(INT nXinfaID);
	//inline	VOID				SetXinfaUplevel(INT nXinfaID, BYTE UplevelNum);
	inline	UINT				GetHairColor(VOID)const;
	inline	VOID				SetHairColor(UINT uColor);
	inline	BYTE				GetFaceColor(VOID)const;
	inline	VOID				SetFaceColor(BYTE byColor);
	inline	BYTE				GetHairModel(VOID)const;
	inline	VOID				SetHairModel(BYTE byModel);
	inline	BYTE				GetFaceModel(VOID)const;
	inline	VOID				SetFaceModel(BYTE byModel);

	inline	UINT				GetEquipID(HUMAN_EQUIP	equipPoint);
	inline	INT					GetDBAttrLvl1(CHAR_ATTR_LEVEL1 enAttrType);					//һ������GET
	inline	VOID				SetDBAttrLvl1(CHAR_ATTR_LEVEL1 enAttrType,INT iValue);		//һ������SET
	inline	INT					GetRemainPoints();											//ʣ��һ�����Ե�
	inline	INT					GetNeiShang();												//����
	inline	VOID				SetDBRemainPoints(INT Point);
	inline 	const _CAMP_DATA	*GetDBCampData(VOID)const;	//ȡ��Ӫ
	inline	VOID				SetDBCampData(const _CAMP_DATA *pCampData);//����Ӫ
	inline	SceneID_t			GetDBStartScene() ;
	inline	VOID				SetDBStartScene(SceneID_t	sceneID);
	inline	SceneID_t			GetDBBakScene();
	inline	VOID				SetDBBakScene(SceneID_t	sceneID);
	inline	VOID				SetDBPosition(const WORLD_POS& pos);						//position ��ʵʱд���ݿ�
	inline  const WORLD_POS*	GetDBPosition();
	inline	VOID				SetDBBakPosition(const WORLD_POS& pos);
	inline  const WORLD_POS*	GetDBBakPosition();
	inline	VOID				WriteDBPosition(WORLD_POS& pos);							//���logout ʱ���õĺ���
	
	inline	VOID				SetOnlineTime(UINT uTime);	//����������ʱ��
	inline	UINT				GetOnlineTime();			//ȡ��������ʱ��	
	inline  VOID				SetLoginTime(UINT uTime);	//��������¼ʱ��
	inline  UINT				GetLoginTime();				//ȡ������¼ʱ��
	inline  VOID				SetLogoutTime(UINT uTime);	//�����������ʱ��
	inline  UINT				GetLogoutTime();			//ȡ���������ʱ��
	inline  VOID				SetDBVersion(UINT Ver);		//�������ݿ�汾
	inline  UINT				GetDBVersion();				//ȡ�����ݿ�汾



	inline	VOID				SetDBGuid(GUID_t	guid);
	inline	GuildID_t			GetDBGuildID( ) ;
	inline	VOID				SetDBGuildID( GuildID_t guildID ) ;
	inline	TeamID_t			GetDBTeamID( ) ;
	inline	VOID				SetDBTeamID( TeamID_t teamID ) ;
	
	//////////////////////////////////////////////////////////////////////////
	//���гƺ����ݷ��ʷ���		
	//////////////////////////////////////////////////////////////////////////
	inline	const CHAR*			GetCurTitle() const;
	inline	VOID				SetCurTitle( const CHAR *pszTitle, BYTE nSize );
	inline	BYTE				GetCurTitleType() const;
	inline	VOID				SetCurTitleType( BYTE nType );
	inline	BYTE				GetLevelTitle();
	inline	VOID				SetLevelTitle(BYTE title);
	inline	BYTE				GetLifeTitle();
	inline	VOID				SetLifeTitle(BYTE title);
	inline	BYTE				GetRankTitle();
	inline	VOID				SetRankTitle(BYTE title);
	inline	BYTE				GetStoryTitle();
	inline	VOID				SetStoryTitle(BYTE title);
	inline	BYTE				GetKillBossTitle();
	inline	VOID				SetKillBossTitle(BYTE title);
	inline	BYTE				GetTreasureTitle();
	inline	VOID				SetTreasureTitle(BYTE title);
	inline	SHORT				GetDyTitle(BYTE	iTitleIndex);
	inline	VOID				SetDyTitle(BYTE	iTitleIndex, SHORT TitleData);
	inline  const CHAR*			GetNickName() const;
	inline  const CHAR*			GetSpouseName() const;
	inline  const CHAR*			GetBangPaiName() const;
	inline  const CHAR*			GetJieBaiName() const;
	inline  const CHAR*			GetShiTuName() const;
	inline  const CHAR*			GetShangDianName() const;
	inline  VOID				SetNickName(const CHAR *pszTitle, BYTE nSize ); 
	inline  VOID				SetSpouseName(const CHAR *pszTitle, BYTE nSize ); 
	inline  VOID				SetBangPaiName(const CHAR *pszTitle, BYTE nSize ); 
	inline  VOID				SetJieBaiName(const CHAR *pszTitle, BYTE nSize ); 
	inline  VOID				SetShiTuName(const CHAR *pszTitle, BYTE nSize ); 
	inline  VOID				SetShangDianName(const CHAR *pszTitle, BYTE nSize ); 

	//////////////////////////////////////////////////////////////////////////
	//�������ݷ��ʷ���		
	//////////////////////////////////////////////////////////////////////////
	inline	const INT			GetBankItemCount( ) const ;
	inline	VOID				SetBankItemCount( const INT Bankitemcount ) ;
	inline	const _ITEM*		GetBankItem( const INT bankindex ) const ;
	inline	VOID				SetBankItem( const INT bankindex, const _ITEM* item ) ;
	inline	const INT			GetBankEndIndex( ) const ;
	inline	VOID				SetBankEndIndex( const INT bankindex ) ;
	inline	const INT			GetBankMoney( ) const ;
	inline	VOID				SetBankMoney( const INT money ) ;
	inline  VOID				EraseBankItem(UINT uBankIndex);
	inline	INT					GetBankRMB(VOID);
	inline	VOID				SetBankRMB(INT nMoney);

	inline	VOID				SwapBankWithBank(UINT iBankIndexFr, UINT iBankIndexTo );
	inline	VOID				SwapBankWithBag(UINT iBagIndex, UINT iBankIndex );
	inline	VOID				SwapBankWithEquip( HUMAN_EQUIP	equipPoint, UINT iBankIndex ) ;
	inline	VOID				OverWriteBag2Bank(UINT iBagIndex, UINT iBankIndex);
	inline	VOID				OverWriteEquip2Bank(HUMAN_EQUIP	equipPoint, UINT iBankIndex);
	inline	VOID				OverWriteBank2Bag(UINT iBagIndex, UINT iBankIndex);
	inline	VOID				OverWriteBank2Equip(HUMAN_EQUIP	equipPoint, UINT iBankIndex);

	VOID						SetFreeOwnFlag(INT Flag);
	INT							GetFreeOwnFlag();

	
	//////////////////////////////////////////////////////////////////////////
	//�������ݷ��ʷ���		
	//////////////////////////////////////////////////////////////////////////
	inline	VOID				EraseBagItem(UINT BagIndex);
	inline	VOID				SwapEquipBag(HUMAN_EQUIP	equipPoint,UINT iBagIndex);
	inline	VOID				OverWriteBag2Equip(UINT iBagIndex,HUMAN_EQUIP	equipPoint);
	inline	VOID				OverWriteEquip2Bag(HUMAN_EQUIP	equipPoint,UINT iBagIndex);
	inline	VOID				PutBagDB(const _ITEM* pItem,UINT iBagIndex);
	inline	VOID				SwapBagDB(UINT iAIndex,UINT iBIndex);
	inline	BOOL				IncBagItemCount(UINT iBagIndex);
	inline	BOOL				DecBagItemCount(UINT iBagIndex);
	inline	VOID				SetItemBind(UINT iBagIndex,BOOL	bBind);
	inline	const _ITEM*		GetBagItem(UINT	iBagIndex)	const ;
	inline	const _ITEM*		GetEquipItem(HUMAN_EQUIP equipPoint);
	inline	VOID				EraseEquipItem(HUMAN_EQUIP equipPoint);
	inline	VOID				SetEquipMark(HUMAN_EQUIP eEquipPoint, BYTE bMark);
	inline	VOID				SetEquipDur(HUMAN_EQUIP eEquipPoint, INT Dur);
	inline	VOID				SetItemDur(UINT BagIndex, INT Dur);
	inline	VOID				RandSet( ) ;										//�����ã���������û������Ϣ
	inline	UINT				GetBaseBagSize();
	//inline	UINT				GetTaskBagSize();
	//inline	UINT				GetMatBagSize();

	//////////////////////////////////////////////////////////////////////////
	//��ȴʱ�����
	//////////////////////////////////////////////////////////////////////////
	
	inline  VOID				SetCooldown(CooldownID_t nID,Time_t nTime);
	inline	BOOL				IsCooldowned(CooldownID_t nID) const;
	inline	VOID				HeartBeat_Cooldown(Time_t nDeltaTime);
	inline	Time_t				GetCooldownRemain(CooldownID_t nID) const;
	//////////////////////////////////////////////////////////////////////////
	//�������ݷ���
	//////////////////////////////////////////////////////////////////////////
	inline BYTE					GetSkillCount();
	inline VOID					SetSkillCount(BYTE Count);
	inline SkillID_t			GetSkillID(BYTE Index);
	inline VOID					SetSkillID(SkillID_t nID,BYTE Index);
	inline BYTE					GetSkillLevel(BYTE Index);
	inline VOID					SetSkillLevel(BYTE Level,BYTE Index);
	//////////////////////////////////////////////////////////////////////////
	//�ķ����ݷ���
	//////////////////////////////////////////////////////////////////////////
	//inline BYTE					GetXinFaCount();
	//inline VOID					SetXinFaCount(BYTE Count);
	//inline SkillID_t			GetXinfaID(BYTE Index);
	//inline VOID					SetXinfaID(SkillID_t nID,BYTE Index);
	//inline BYTE					GetXinfaLevel(BYTE Index);
	//inline VOID					SetXinfaLevel(BYTE Level,BYTE Index);
	//////////////////////////////////////////////////////////////////////////
	//Impact���ݷ���
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	//�������ݷ���
	//////////////////////////////////////////////////////////////////////////
	inline VOID					SetMissionDone(MissionID_t idMission,MissionID_t idIndex);
	inline VOID					SetMissionNotDone(MissionID_t idMission,MissionID_t idIndex);
	inline VOID					SetMissionParam( UINT uIndexMission, UINT uIndexParam, INT nValue );
	inline VOID					SetMissionData( INT nIndex, INT nData );//���õ�ǰ������������
	inline VOID					SetMission(UINT uIndex,MissionID_t idMission,ScriptID_t idScript);
	inline VOID					SetMissionCount(UINT uCount);
	inline VOID					SetFlags_KillObject(UINT uIndex, BOOL bSet );
	inline VOID					SetFlags_EnterArea(UINT uIndex, BOOL bSet );
	inline VOID					SetFlags_ItemChanged(UINT uIndex, BOOL bSet );
	inline VOID					SetFlags_PetChanged(UINT uIndex, BOOL bSet );
	inline VOID					ClearMission(UINT uIndex);
	//////////////////////////////////////////////////////////////////////////
	//��ϵ���ݷ��ʷ���
	//////////////////////////////////////////////////////////////////////////

	inline  VOID				SetRelation(INT Index, const _OWN_RELATION* pRelation);
	inline	VOID				ModifyFriendType( INT nIndex, RELATION_TYPE RelationType );
	inline	VOID				UpdateRelationInfo( INT Index, const RELATION_MEMBER* pMemberInfo );
	inline	VOID				SetMood(const CHAR* szMood);
	inline	VOID				ModifyFriendPoint(INT Index, INT nFriendPoint);
	inline	VOID				AdjustRelationType(INT Index, UCHAR uType);
	inline	VOID				AdjustRelationGroup(INT Index, UCHAR uGroup);
	inline	VOID				DeleteRelation(INT Index);
	inline	VOID				IncRelationCount(INT Index);
	inline	VOID				DecRelationCount(INT Index);
	inline	VOID				SetSpouse(const MarriageInfo* pMarriageInfo);
	inline	VOID				AddPrentice(GUID_t guid);
	inline	VOID				DelPrentice(GUID_t guid);
	inline	VOID				SetMoralPoint(UINT uPoint);
	inline	VOID				SetBetrayTime(time_t BetrayTime);
	inline	VOID				SetMaster(const MasterInfo* pMasterInfo);


	inline	VOID				SetAbilityLevel(AbilityID_t abilityid,INT lvl);
	inline	VOID				SetAbilityExp(AbilityID_t abilityid,INT exp);
	inline  VOID				SetPrescrLearnedFlag( const PrescriptionID_t prescrid, BOOL flag );

	//////////////////////////////////////////////////////////////////////////
	//�������ݷ��ʷ���
	//////////////////////////////////////////////////////////////////////////
	inline const	_PET_DB_LOAD*	GetPet(UINT	iIndex)	const ;


	//////////////////////////////////////////////////////////////////////////
	//�û��������ݷ��ʷ���
	//////////////////////////////////////////////////////////////////////////
	inline	VOID				SetSettingData( INT nType, _OWN_SETTING* Value ) ;
	inline const	_OWN_SETTING*	GetSetting( INT nType ) const ;
	inline BOOL						isSetBit( INT nType, INT bit );
	//////////////////////////////////////////////////////////////////////////
	//���˽����Ϣ���ʷ���
	//////////////////////////////////////////////////////////////////////////
	inline  VOID				SetPrivateInfo(_PRIVATE_INFO_DB_LOAD* pPrivateInfo);

	inline	INT					GetReserve(INT index);
	inline VOID					SetReserve(INT index,INT value);

public :
	/*========================================
	 *	��ʼ����ɫ�������ݿ���Ϣ
	 ========================================*/
	BOOL						InitHumanDB( ) ;
	BOOL						InitEquipDB( ) ;
	BOOL						InitBagDB( ) ;
	BOOL						InitBankDB( ) ;
	BOOL						InitEffectDB( ) ;
	BOOL						InitSkillDB( ) ;
	BOOL						InitCooldownDB();
	



private:
	inline _ITEM*				_GetBagItem(UINT uBagIndex);
	inline _ITEM*				_GetBankItem(UINT uBankIndex);
	inline VOID					_RegDBAttr(CHAR_ATTR_DB eOffSet,VOID* pArg,INT iSize);//ע�ᵥ������
	inline VOID					_RegSMAttr(CHAR_ATTR_DB eOffSet,VOID* pArg,INT iSize);//ע��ShareMem����
	VOID						_RegisterDBAttributes();  //ע����������
	VOID						_RegisterSMAttributes();  //ע������ShareMem����
	
public:
	
	_PET_DB_LIST_LOAD*				m_dbPetList;	//����

	//�������Ƿ��ʷ���
	VOID						SetDBEnumAttrDirty(CHAR_ATTR_DB eOffSet,BOOL bFlag = TRUE);
	BOOL						GetDBEnumAttrDirty(CHAR_ATTR_DB eOffSet);
private:
	
	
	_HUMAN_DB_LOAD*					m_dbHuman ;		//��ɫ������Ϣ
	_BAG_DB_LOAD*					m_dbBag ;		//��ɫ������Ʒ��Ϣ
	_EQUIP_DB_LOAD*					m_dbEquip ;		//��ɫװ����Ϣ
	_BANK_DB_LOAD*					m_dbBank ;		//��ɫ������Ʒ��Ϣ
	_SKILL_DB_LOAD*					m_dbSkill ;		//��ɫ����ӵ�еļ�����Ϣ
	_COOLDOWN_DB_LOAD_FOR_HUMAN*	m_dbCooldown ;	//��ɫ���ϵ���ȴ��Ϣ
	_RELATION_DB_LOAD*				m_dbRelation;	//��ɫ��ϵ�ˣ����ѡ���������
	_ABILITY_DB_LOAD*				m_dbAbility;	//��ɫѧ����������Ϣ�Լ��䷽��
	_XINFA_DB_LOAD*					m_dbXinFa ;		//��ɫѧ����ķ���Ϣ
	_IMPACT_DB_LOAD*				m_dbImpact ;	//��ɫ������ʩ�ӵĸ���Ч����Ϣ
	_MISSION_DB_LOAD*				m_dbMission;	//�����б�
	_SETTING_DB_LOAD*				m_dbSetting ;	//��������
	_PRIVATE_INFO_DB_LOAD*			m_dbPrivateInfo;//˽����Ϣ


private:
	//�����ڴ��������
	HumanSMU*						m_HumanSMU;		//�����ڴ�����
	HUMAN_DB_ATTR_FLAG*				m_AttrFlag;		//��ɫ����ˢ�¿�������
	HUMAN_DB_ATTR_REG*				m_AttrReg;		//��ɫ����ע������
	HUMAN_DB_ATTR_REG*				m_AttrRegSM;	//��ɫ������ShareMem��ע������

public:
	inline const	_BAG_DB_LOAD*					GetBag();
	inline const	_HUMAN_DB_LOAD*					GetHumanDB()	const;
	inline const	_EQUIP_DB_LOAD*					GetEquipDB();
	inline const	_BANK_DB_LOAD*					GetBankDB();
	inline const	_SKILL_DB_LOAD*					GetSkillDB();
	inline const	_COOLDOWN_DB_LOAD_FOR_HUMAN* 	GetCooldownDB() const;
	inline const	_RELATION_DB_LOAD*				GetRelationDB()	const;
	inline const	_ABILITY_DB_LOAD*				GetAbilityDB()	const;
	inline const	_XINFA_DB_LOAD*					GetXinFaDB()	const;
	inline const	_IMPACT_DB_LOAD*				GetImpactDB()	const;
	inline const	_MISSION_DB_LOAD*				GetMissionDB()  const;
	inline const	_SETTING_DB_LOAD*				GetSettingDB()  const;
	inline const	_PRIVATE_INFO_DB_LOAD*			GetPrivateInfoDB() const;

	//�벻Ҫ��human���߼������е������к���
	inline VOID								WriteHumanDB(const	_HUMAN_DB_LOAD* pHumanDB);
	inline VOID								WriteBagDB(const	_BAG_DB_LOAD* pBagDB);
	inline VOID								WriteEquipDB(const	_EQUIP_DB_LOAD*	pEquipDB);
	inline VOID								WriteBankDB(const	_BANK_DB_LOAD*	pBankDB);
	inline VOID								WriteSkillDB(const _SKILL_DB_LOAD* pSkillDB);
	inline VOID								WriteCooldownDB(const _COOLDOWN_DB_LOAD_FOR_HUMAN*	pCooldownDB);
	inline VOID								WriteRelationDB(const _RELATION_DB_LOAD* pRelationDB);
	inline VOID								WriteAbilityDB(const _ABILITY_DB_LOAD* pAbilityDB);
	inline VOID								WriteXinFaDB(const _XINFA_DB_LOAD* pXinFaDB);
	inline VOID								WriteImpactDB(const _IMPACT_DB_LOAD* pImpactDB);
	inline VOID								WriteMissionDB(const _MISSION_DB_LOAD* pMissionDB);
	inline VOID								WriteSettingDB(const _SETTING_DB_LOAD* pSettingDB);
	inline VOID								WritePrivateInfo(const _PRIVATE_INFO_DB_LOAD* pPrivateDB);
private:
	//////////////////////////////////////////////////////////////////////////
	// �ӿںͷ���  2005.11.25 ��
	//////////////////////////////////////////////////////////////////////////
	//�����ڴ桢DB����ע������
	VOID*									GetDBEnumAttr(CHAR_ATTR_DB eOffSet);
	VOID*									GetSMEnumAttr(CHAR_ATTR_DB eOffSet);
	INT										GetDBEnumAttrSize(CHAR_ATTR_DB eOffSet);
	
};

#include "HumanDB.inl"

#endif
