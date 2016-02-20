///////////////////////////////////////////////////////////////////////////////
// �ļ�����DataRecords.h
// ����˵�������ܺ�Ч���ı��¼�ṹ
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef DATARECORDS_H
#define DATARECORDS_H
#include "Type.h"

namespace DBC
{
	class DBCFile;
}
class Descriptor_T
{
	public:
		Descriptor_T():m_szDesc(NULL),m_nValue(0) {};
		Descriptor_T(Descriptor_T const& rhs): m_szDesc(rhs.m_szDesc),m_nValue(rhs.m_nValue){};
		~Descriptor_T() {};
		Descriptor_T& operator=(Descriptor_T const& rhs) 
		{
			m_szDesc = rhs.m_szDesc;
			m_nValue = rhs.m_nValue;
			return *this;
		};
		BOOL InitFromDB(DBC::DBCFile* pDB, INT const nRow, INT& nColumn);
		CHAR const* GetDescription(VOID) const {return m_szDesc;};
		INT GetValue(VOID) const {return m_nValue;};
		VOID SetValue(INT const nValue) {m_nValue=nValue;};
	protected:
	private:
		CHAR const* m_szDesc;
		INT	m_nValue;
};
class ConDepTerm_T
{
	public:
		ConDepTerm_T(): m_szDesc(NULL),m_nType(INVALID_ID),m_nParam0(0),m_nParam1(0){};
		ConDepTerm_T(ConDepTerm_T const& rhs) : m_szDesc(rhs.m_szDesc), m_nType(rhs.m_nType),m_nParam0(rhs.m_nParam0), m_nParam1(rhs.m_nParam1){};
		~ConDepTerm_T(){};
		ConDepTerm_T& operator=(ConDepTerm_T const& rhs) 
		{
			m_szDesc = rhs.m_szDesc;
			m_nType = rhs.m_nType;
			m_nParam0 = rhs.m_nParam0;
			m_nParam1 = rhs.m_nParam1; 
			return *this;
		};
		BOOL InitFromDB(DBC::DBCFile* pDB, INT const nRow, INT& nColumn);
		CHAR const* GetDescription(VOID) const {return m_szDesc;};
		INT GetType(VOID) const {return m_nType;};
		//VOID SetType(INT nValue) {m_nType=nValue;};
		INT GetParam0(VOID) const {return m_nParam0;};
		//VOID SetParam0(INT nValue) {m_nParam0=nValue;};
		INT GetParam1(VOID) const {return m_nParam1;};
		//VOID SetParam1(INT nValue) {m_nParam1=nValue;};
	protected:
	private:
		CHAR const* m_szDesc;
		INT m_nType;
		INT m_nParam0;
		INT m_nParam1;
};

namespace Combat_Module
{
	//IDCollection_T
	class IDCollection_T
	{
		public:
			enum
			{
				MAX_ID = 1024,
			};
			enum
			{
				COLLECTION_SIZE = 30,
			};
			typedef enum
			{
				TYPE_BUFF_ID = 0,
				TYPE_IMPACT_MUTEX_ID,
				TYPE_IMPACT_LOGIC_ID,
				TYPE_SKILL_ID,
				TYPE_SKILL_LOGIC_ID,
				TYPE_DIRECT_IMPACT_ID,
			} CollectionIdType_T;
			IDCollection_T() :m_bInited(FALSE), m_nID(INVALID_ID),m_szDescription(NULL), m_nSize(0)
			{
				memset((VOID*)m_aID, INVALID_ID, sizeof(m_aID));
			};
			~IDCollection_T() {};
			BOOL IsInited(VOID) const {return m_bInited;};
			BOOL InitFromDB(DBC::DBCFile* pDB, INT const nRow);
			ID_t GetID(VOID) const{return m_nID;};
			CHAR const* GetDescription(VOID) const {return m_szDescription;};
			INT GetType(VOID) const {return m_nType;}
			INT GetCollectionSize(VOID) const 
			{
				INT& rSize = (INT&)m_nSize;
				COLLECTION_SIZE<rSize?rSize=COLLECTION_SIZE:NULL;
				return m_nSize;
			};
			INT GetIDByIndex(INT nIndex) const;
			BOOL IsThisIDInCollection(ID_t nID) const;
			CHAR const* GetEndDesc(VOID) const {return m_szEndDesc;};
		protected:
		private:
			BOOL m_bInited;
			ID_t m_nID;
			CHAR const* m_szDescription;
			INT m_nType;
			INT m_nSize;
			INT m_aID[COLLECTION_SIZE];
			CHAR const* m_szEndDesc;
	};
	namespace Skill_Module
	{
		typedef enum
		{
			INVALID_TYPE = SKILL_TYPE_INVALID,
			SKILL_NEED_CHARGING = SKILL_TYPE_GATHER,
			SKILL_NEED_CHANNELING = SKILL_TYPE_LEAD,
			SKILL_INSTANT_LAUNCHING = SKILL_TYPE_LAUNCH,
			SKILL_PASSIVE = SKILL_TYPE_PASSIVE,
			NUMBER_OF_SKILL_TYPE
		} SkillType_T;

		//XinFaData
		class XinFaData_T
		{
			public:
				enum
				{
					MAX_ID = 255,
				};
				XinFaData_T(): m_nMenPai(-1),m_bInited(FALSE),m_szDescription(NULL),m_szName(NULL),m_szIcon(NULL){};
				~XinFaData_T(){};
				BOOL InitFromDB(DBC::DBCFile* pDB, INT const nRow);
				inline MenPaiID_t GetMenPai() const {return m_nMenPai;}
				inline CHAR const* GetName(VOID) {return m_szName;}
				inline CHAR const* GetDescription(VOID) {return m_szDescription;}
				inline CHAR const* GetIcon(VOID){return m_szIcon;}
				inline BOOL IsInited(VOID) const {return m_bInited;}
			protected:
			private:
				MenPaiID_t m_nMenPai;
				CHAR const* m_szName;
				CHAR const* m_szDescription;
				CHAR const* m_szIcon;
				BOOL m_bInited;
		};
		//SkillTemplate
		class SkillTemplateData_T
		{
			public:
				enum
				{
					MAX_ID = 2048,
				};
				SkillTemplateData_T();
				~SkillTemplateData_T(){};
				BOOL IsInited(VOID) const {return m_bInited;};
				BOOL InitFromDB(DBC::DBCFile* pDB, INT const nRow);
				SkillID_t GetSkillID(VOID) const {return m_nSkillID;};
				MenPaiID_t GetMenPai(VOID) const {return m_nMenPai;};
				CHAR const* GetName(VOID) const {return m_szName;};
				INT GetSkillMaxLevel(VOID) const {return m_nSkillMaxLevel;};
				INT GetMustUseWeaponFlag(VOID) const {return m_bMustUseWeapon;};
				INT GetDisableByFlag1(VOID) const {return m_nDisableByFlag1;};
				INT GetDisableByFlag2(VOID) const {return m_nDisableByFlag2;};
				INT GetDisableByFlag3(VOID) const {return m_nDisableByFlag3;};
				ID_t GetSkillClass(VOID) const {return m_nSkillClass;};
				//INT GetXinFaParam(VOID) const {return m_nXinFaParam;};
				INT GetRangedSkillFlag(VOID) const {return m_nRangedSkillFlag;};
				BOOL ForceBreakPreSkill(VOID) const {return m_bForceBreakPreSkill;};
				INT GetSkillType(VOID) const {return m_nSkillType;};
				CooldownID_t GetCooldownID(VOID) const {return m_nCooldownID;};
				BOOL GetTargetMustInSpecialState(VOID) const {return m_nTargetMustInSpecialState;};
				ID_t GetClassByUser(VOID) const {return m_nClassByUser;};
				ID_t GetPassiveFlag(VOID) const {return m_nPassiveFlag;};
				ID_t GetSelectType(VOID) const {return m_nSelectType;};
				ID_t GetOperateModeForPetSkill(VOID) const {return m_nOperateModeForPetSkill;};
				ID_t GetPetRateOfSkill(VOID) const {return m_nPetRateOfSkill;};
				ID_t GetTypeOfPetSkill(VOID) const {return m_nTypeOfPetSkill; };
				ID_t GetImpactIDOfSkill(VOID) const {return m_nImpactIDOfSkill; };
				ID_t GetTargetingLogic(VOID) const {return m_nTargetingLogic;};
				Time_t GetPlayActionTime(VOID) const {return m_nPlayActionTime;};
				FLOAT GetOptimalRangeMin(VOID) const {return m_fOptimalRangeMin;};
				FLOAT GetOptimalRangeMax(VOID) const {return m_fOptimalRangeMax;};
				INT GetStandFlag(VOID) const {return m_nStandFlag;};
				ID_t GetTargetLogicByStand(VOID) const {return m_nTargetLogicByStand;};
				ID_t GetTargetCheckByObjType(VOID) const {return m_nTargetCheckByObjType;};
				BOOL IsPartyOnly(VOID) const {return m_bPartyOnly;};
				INT GetChargesOrInterval(VOID) const {return m_nChargesOrInterval;};
				BOOL IsAutoShotSkill(VOID) const {return m_bAutoShot;};
				INT GetAccuracy(VOID) const {return m_nAccuracy;};
				INT GetCriticalRate(VOID) const {return m_nCriticalRate;};
				BOOL UseNormalAttackRate(VOID) const {return m_bUseNormalAttackRate;};
				Time_t GetActiveTime(VOID) const {return m_nActiveTime;};
				FLOAT GetRadius(VOID) const {return m_fRadius;};
				FLOAT GetAngle(VOID) const {return m_fAngle;};
				INT GetMaxTargetNumber(VOID) const {return m_nMaxTargetNumber;};
				BOOL CanInterruptAutoShot(VOID) const {return m_bCanInterruptAutoShot;};
				Time_t GetDelayTime(VOID) const {return m_nDelayTime;};
				ID_t GetSkillInstance(INT const nIdx) const 
				{
					if(0>nIdx||MAX_CHAR_SKILL_LEVEL<=nIdx)
					{
						AssertEx(FALSE,"[SkillTemplateData_T::GetSkillInstance]:nIdx Out of range!!");
						return INVALID_ID;
					}
					return m_aSkillInstance[nIdx];
				};
				CHAR const* Description(VOID) const {return m_szDescription;};
			protected:
			private:
				BOOL m_bInited; //�ü�¼�Ƿ��Ѿ���ʼ��
				SkillID_t m_nSkillID; //���ܱ��
				MenPaiID_t m_nMenPai;//���ɱ��
				CHAR const* m_szName;//���ܵ�����
				INT m_nSkillMaxLevel;//���ܵ����ȼ�
				INT m_nClientOnly1; //�ͻ���ר������
				BOOL m_bMustUseWeapon; //�˼��ܱ���ʹ������
				INT m_nDisableByFlag1; //�����ڱ��1
				INT m_nDisableByFlag2; //�����ڱ��1
				INT m_nDisableByFlag3; //�����ڱ��1
				ID_t m_nSkillClass;//����ϵ
				INT m_nXinFaParam_Nouse;//�ķ���������
				INT m_nRangedSkillFlag;//�Ƿ���Զ�̼���
				BOOL m_bForceBreakPreSkill;//�Ƿ�ǿ���ж���һ������ִ�еļ���
				ID_t m_nSkillType; // Charge, channel or instant shot skill
				CooldownID_t m_nCooldownID; //��ȴʱ���ID
				BOOL m_nTargetMustInSpecialState;// Ŀ�������: 0:��ģ�1:����; -1: ��Ч
				ID_t m_nClassByUser;// ��ʹ�������ͷ��࣬0:���, 1:����, 2:����, 3:��Ʒ,
				ID_t m_nPassiveFlag;// �������Ǳ������ܣ�0:��������,1:��������;
				ID_t m_nSelectType;//����ѡ����
				ID_t m_nOperateModeForPetSkill;//���＼�ܷ������� 0:�����ֶ���ѡ,1:AI�Զ�ִ��,2:��ǿ�������Եı�������
				ID_t m_nPetRateOfSkill; //���ܷ�������,ֻ�Գ��＼����Ч
				ID_t m_nTypeOfPetSkill; //���＼������,0:�﹦,1:����,2:����,3:����,4:����;
				ID_t m_nImpactIDOfSkill; //���＼�ܲ�����Ч��ID
				ID_t m_nTargetingLogic; //Ŀ��ѡȡ�߼�
				Time_t m_nPlayActionTime;//���ܶ������ŵ�ʱ��
				FLOAT m_fOptimalRangeMin;//����ʹ�÷�Χ�½�
				FLOAT m_fOptimalRangeMax;//����ʹ�÷�Χ�Ͻ�
				INT m_nStandFlag; //����Ǽ��ܵ�������ǣ�-1����������Ŀ�꣬0�����ԣ�1�Ǽ�����ǿĿ��
				ID_t m_nTargetLogicByStand; //����������ʲô��Ӫ��Ŀ�꣬�жԣ��Ѻã�������ȫ��������
				ID_t m_nTargetCheckByObjType; //���ܵ����ļ���ؼ��
				BOOL m_bPartyOnly;//����ֻ�������ڶ���
				INT m_nChargesOrInterval;//������Ч�����������е���Ч����
				BOOL m_bAutoShot;//�Զ������ͷż���
				INT m_nAccuracy;//������
				INT m_nCriticalRate;//������,���߽б�����
				BOOL m_bUseNormalAttackRate;//���ܵ���ȴ�Ƿ������﹥���ٶ�Ӱ��
				Time_t m_nActiveTime;//����ʱ��
				FLOAT m_fRadius;//���ð뾶
				FLOAT m_fAngle;//���ýǶ�
				INT m_nMaxTargetNumber;//�������Ŀ����
				BOOL m_bCanInterruptAutoShot; //�����ܻ��ж��Զ�������ܵ������ͷ�
				Time_t m_nDelayTime; //�ӳ�ʱ��
				ID_t m_aSkillInstance[MAX_CHAR_SKILL_LEVEL];//�������ķ�Ӱ�����������
				CHAR const* m_szDescription;//���ܵ�����
		};
		//SkillInstance
		class SkillInstanceData_T
		{
			public:
				enum
				{
					MAX_ID = 16384,
				};
				enum
				{
					DESCRIPTOR_NUMBER = 12,
					CONDITION_AND_DEPLETE_TERM_NUMBER = 3,
				};
				SkillInstanceData_T();
				~SkillInstanceData_T(){};
				BOOL IsInited(VOID) const {return m_bInited;};
				BOOL InitFromDB(DBC::DBCFile* pDB, INT const nRow);
				INT GetInstanceID(VOID) const {return m_nInstanceID;};
				INT GetIDForManagment(VOID) const {return m_nGUID;};
				CHAR const* GetDescription(VOID) const {return m_szDesc;};
				INT	GetStudyLevel(VOID) const { return m_nStudyLevel; }
				INT	GetStudyMoney(VOID) const { return m_nStudyMoney; }
				ID_t	GetLogicID(VOID) const {return m_nLogicID;};
				Time_t GetCooldownTime(VOID) const {return m_nCooldownTime;};
				Time_t GetChargeTime(VOID) const {return m_nChargeTime;};
				Time_t GetChannelTime(VOID) const {return m_nChannelTime;};
				ConDepTerm_T const* GetConDepTermByIndex(INT const nIdx) const
				{
					if(0<=nIdx&&CONDITION_AND_DEPLETE_TERM_NUMBER>nIdx)
						return &m_aConDepTerms[nIdx];
					return NULL;
				}
				INT GetTargetLevel(VOID) const {return m_nTargetLevel;};
				Descriptor_T const* GetDescriptorByIndex(INT const nIdx) const
				{
					if(0<=nIdx&&DESCRIPTOR_NUMBER>nIdx)
						return &m_aDescriptors[nIdx];
					return NULL;
				}
			protected:
			private:
				BOOL m_bInited;
				INT m_nInstanceID;
				INT m_nGUID;
				CHAR const* m_szDesc;
				INT	m_nStudyLevel;
				INT m_nStudyMoney;
				ID_t m_nLogicID;
				Time_t m_nCooldownTime;
				Time_t m_nChargeTime;
				Time_t m_nChannelTime;
				ConDepTerm_T m_aConDepTerms[CONDITION_AND_DEPLETE_TERM_NUMBER];
				INT m_nTargetLevel;
				Descriptor_T m_aDescriptors[DESCRIPTOR_NUMBER];
				CHAR const* m_szDescDetail;
		};
		//ImpactSEData_T
		class ImpactSEData_T
		{
			public:
				enum
				{
					MAX_ID = 1024,
				};
				ImpactSEData_T()
					:m_bInited(FALSE),m_nID(-1),m_nMutexID(-1)
					,m_nPriParam(0), m_bCanBeDispeled(FALSE)
					,m_bHostileFlag(FALSE), m_bCanBeManualCancel(FALSE)
					,m_szDescription(NULL)
				{};
				~ImpactSEData_T() {};
				BOOL IsInited(VOID) const {return m_bInited;};
				BOOL InitFromDB(DBC::DBCFile* pDB, INT const nRow);
				ID_t GetID(VOID) const{return m_nID;};
				ID_t GetMutexID(VOID) const {return m_nMutexID;};
				INT GetPriParam(VOID) const {return m_nPriParam;};
				BOOL IsStillOnWhenOwnerDead(VOID) const {return m_bStillOnWhenOwnerDead;};
				BOOL CanBeDispeled(VOID) const {return m_bCanBeDispeled;};
				BOOL IsHosttileImpact(VOID) const {return m_bHostileFlag;};
				BOOL CanBeManualCancel(VOID) const {return m_bCanBeManualCancel;};
				CHAR const* GetDescription(VOID) const {return m_szDescription;};
			protected:
			private:
				BOOL m_bInited;
				ID_t m_nID;
				ID_t m_nMutexID;
				INT m_nPriParam;
				BOOL m_bStillOnWhenOwnerDead;
				BOOL m_bCanBeDispeled;
				BOOL m_bHostileFlag;
				BOOL m_bCanBeManualCancel;
				CHAR const* m_szDescription;
		};		
		//SkillObjData //��ʱ����
		class SkillObjData_T
		{
			public:
				enum
				{
					MAX_ID = 255,
				};
				BOOL IsInited(VOID) const {return m_bInited;};
				BOOL InitFromDB(DBC::DBCFile* pDB, INT const nRow);
				INT DoLogic(VOID) const {return m_nDoLogic;};
				INT Action(VOID) const {return m_nAction;};
				INT SpecialEffect(VOID) const {return m_nSpecialEffect;};
				INT BindPosition(VOID) const{return m_nSEBind;};
				INT Model(VOID) const {return m_nModel;};
				INT Icon(VOID) const {return m_nIcon;};
				INT Friendliness(VOID) const {return m_nFriendliness;};
				BOOL IsPartyOnly(VOID) const {return m_bPartyOnly;};
				CHAR const* Description(VOID) const {return m_szDescription;};
				SkillObjData_T(VOID) {};
				~SkillObjData_T(VOID) {};
			protected:
			private:
				BOOL m_bInited;
				INT m_nDoLogic;
				INT m_nAction;
				INT m_nSpecialEffect;
				INT m_nSEBind;
				INT m_nModel;
				INT m_nIcon;
				INT m_nFriendliness;
				INT m_nTargetTypeRequirement;
				BOOL m_bPartyOnly;
				CHAR const* m_szDescription;
		};		
	};
	namespace Impact_Module
	{
		//ImpactData_T
		class ImpactData_T  //��׼����Ч������
		{
			public:
				enum
				{
					NUMBER_OF_PARAMS = 16,
					MAX_ID = 16384,
				};
				ImpactData_T() :
					m_bInited(FALSE),
					m_nDataIndex(INVALID_ID),
					m_szName(NULL),
					m_nLogicID(INVALID_ID),
					m_szLogicDesc(NULL),
					m_bIsOverTimed(FALSE),
					m_nImpactID(INVALID_ID),
					m_nMutexID(INVALID_ID),
					m_nLevel(0),
					m_nStandFlag(INVALID_ID),
					m_bRemainOnCorpse(FALSE),
					m_bCanBeDispeled(FALSE),
					m_bCanBeCanceled(FALSE),
					m_bNeedChannelSupport(FALSE),
					m_bIgnoreFliter(FALSE),
					m_bFadeOutWhenUnitOnDamage(FALSE),
					m_bFadeOutWhenUnitStartActions(FALSE),
					m_bFadeOutWhenUnitOffline(FALSE),
					m_bStillTimingWhenUnitOffline(FALSE),
					m_nContinuance(0),
					m_nInterval(0),
					m_bMutexByCaster(FALSE),
					m_szDescription(NULL)
				{};
				~ImpactData_T() {};
				BOOL IsInited(VOID) const {return m_bInited;};
				BOOL InitFromDB(DBC::DBCFile* pDB, INT const nRow);
				ID_t GetDataIndex(VOID) const {return m_nDataIndex;};
				CHAR const* GetName(VOID) const {return m_szName;};
				ID_t GetLogicID(VOID) const {return m_nLogicID;};
				CHAR const* GetLogicDescription(VOID) const {return m_szLogicDesc;};
				BOOL IsOverTimed(VOID) const {return m_bIsOverTimed;};
				ID_t GetImpactID(VOID) const {return m_nImpactID;};
				ID_t GetMutexID(VOID) const {return m_nMutexID;};
				ID_t GetLevel(VOID) const {return m_nLevel;};
				ID_t GetStandFlag(VOID) const {return m_nStandFlag;};
				BOOL IsRemainOnCorpse(VOID) const {return m_bRemainOnCorpse;};
				BOOL CanBeDispeled(VOID) const {return m_bCanBeDispeled;};
				BOOL CanBeCanceled(VOID) const {return m_bCanBeCanceled;};
				BOOL NeedChannelSupport(VOID) const {return m_bNeedChannelSupport;};
				BOOL IsFadeOutWhenUnitOnDamage(VOID) const {return m_bFadeOutWhenUnitOnDamage;};
				BOOL IsFadeOutWhenUnitStartActions(VOID) const {return m_bFadeOutWhenUnitStartActions;};
				BOOL IsFadeOutWhenUnitOffline(VOID) const {return m_bFadeOutWhenUnitOffline;};
				BOOL IsStillTimingWhenUnitOffline(VOID) const {return m_bStillTimingWhenUnitOffline;};
				BOOL IgnoreFliter(VOID) const {return m_bIgnoreFliter;};
				Time_t GetContinuance(VOID) const {return m_nContinuance;};
				Time_t GetInterval(VOID) const {return m_nInterval;};
				BOOL GetMutexByCasterFlag(VOID) const {return m_bMutexByCaster;};
				Descriptor_T const* GetDescriptorByIndex(INT nIdx) const
				{
					if(0<=nIdx&&NUMBER_OF_PARAMS>nIdx)
						return &m_aParams[nIdx];
					return NULL;
				};
				CHAR const* GetDescription(VOID) const {return m_szDescription;};
			protected:
			private:
				BOOL m_bInited;
				ID_t m_nDataIndex;
				CHAR const* m_szName;
				ID_t m_nLogicID;
				CHAR const* m_szLogicDesc;
				BOOL m_bIsOverTimed;
				ID_t m_nImpactID;
				ID_t m_nMutexID;
				ID_t m_nLevel;
				ID_t m_nStandFlag;
				BOOL m_bRemainOnCorpse;
				BOOL m_bCanBeDispeled;
				BOOL m_bCanBeCanceled;
				BOOL m_bNeedChannelSupport;
				BOOL m_bIgnoreFliter;
				BOOL m_bFadeOutWhenUnitOnDamage;
				BOOL m_bFadeOutWhenUnitStartActions;
				BOOL m_bFadeOutWhenUnitOffline;
				BOOL m_bStillTimingWhenUnitOffline;
				Time_t m_nContinuance;
				Time_t m_nInterval;
				BOOL m_bMutexByCaster;
				Descriptor_T m_aParams[NUMBER_OF_PARAMS];
				CHAR const* m_szDescription;
		};
	};
	namespace Special_Obj_Module
	{
		using namespace DBC;
		
		//SpecialObjData_T
		class SpecialObjData_T
		{
			public:
				enum
				{
					MAX_ID = 1023,
					DESCRIPTOR_NUMBER = 16,
				};
				SpecialObjData_T();
				~SpecialObjData_T(){};
				BOOL IsInited(VOID) const {return m_bInited;};
				BOOL InitFromDB(DBCFile* pDB, INT const nRow);
				SkillID_t GetID(VOID) const {return m_nDataID;};
				CHAR const* GetName(VOID) const {return m_szName;};
				ID_t GetClass(VOID) const {return m_nClass;};
				ID_t GetLogicID(VOID) const {return m_nLogicID;};
				ID_t GetStealthLevel(VOID) const {return m_nStealthLevel;}
				INT GetTrapUsedFlags(VOID) const {return m_nTrapUsedFlags;}
				INT GetActiveTimes(VOID) const {return m_nActiveTimes;}
				Time_t GetContinuance(VOID) const {return m_nContinuance;}
				Time_t GetInterval(VOID) const {return m_nInterval;}
				FLOAT GetTriggerRadius(VOID) const {return m_fTriggerRadius;}
				FLOAT GetEffectRadius(VOID) const {return m_fEffectRadius;}
				INT	GetEffectedObjNumber(VOID) const {return m_nEffectedObjNumber;}
				Descriptor_T const* GetDescriptorByIndex(INT const nIdx) const
				{
					if(0<=nIdx&&DESCRIPTOR_NUMBER>nIdx)
						return &m_aDescriptors[nIdx];
					return NULL;
				}
				CHAR const* Description(VOID) const {return m_szDescription;};
			protected:
			private:
				BOOL 	m_bInited; //�ü�¼�Ƿ��Ѿ���ʼ��
				INT 	m_nDataID; //����ID
				CHAR const* m_szName; //����
				//����Skip Tooltips(�ͻ�����ʾ�õ�˵��������)
				ID_t 	m_nClass; //���
				ID_t 	m_nLogicID; //�߼�ID
				INT		m_nStealthLevel; //������
				INT		m_nTrapUsedFlags; //����ר�õ�һЩ��ǵļ���
				INT		m_nActiveTimes; //���Լ����Ĵ���
				//����Skip �ͻ�����ʾ�õ��������
				Time_t 	m_nContinuance; //����ʱ��
				Time_t 	m_nInterval; //����ʱ����
				FLOAT	m_fTriggerRadius;//�����뾶
				FLOAT 	m_fEffectRadius; //Ӱ��뾶
				INT	  	m_nEffectedObjNumber; //Ӱ��������Ŀ
				Descriptor_T m_aDescriptors[DESCRIPTOR_NUMBER];//��������
				CHAR const* m_szDescription;//�ڲ�ʹ�õ�����
		};
	}
};
#endif //DATARECORDS_H
