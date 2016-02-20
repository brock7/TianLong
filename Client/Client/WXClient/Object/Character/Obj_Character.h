// Obj_Character.h
// 
/////////////////////////////////////////////////////////

#ifndef __OBJ_CHARACTER_H__
#define __OBJ_CHARACTER_H__

#pragma once

#include "Type.h"
#include "..\Obj_Dynamic.h"
#include "..\..\World\Path.h"
#include "..\ObjectDef.h"
#include "CharacterStruct.h"

/**
	��ɫϵ�еĻ��࣬��̳г� PlayerOther,PlayerMySelf, PlayerNPC��
*/
namespace DBC
{
	class DBCFile;
}

class CAI_Base;
class CObjectCommand_Logic;
class CCharacterData;
struct SDATA_CHARACTER;
struct _DBC_CHAR_RACE;
class tCreatureBoard;
class tSoundSource;
class CObject_Character : public CObject_Dynamic
{
public:
	//��ɫobj������
	enum CHARACTER_TYPE
	{
		CT_INVALID	= -1,

		CT_PLAYERMYSELF,		//����Լ�
		CT_PLAYEROTHER,			//�������
		CT_MONSTER				//����/NPC
	};

	//��ǰ��ɫ��Object���߼�״̬
	enum ENUM_CHARACTER_LOGIC
	{
		CHARACTER_LOGIC_INVALID	= -1,
		CHARACTER_LOGIC_IDLE,
		CHARACTER_LOGIC_ACTION,
		CHARACTER_LOGIC_MOVE,
		CHARACTER_LOGIC_SKILL_GATHER,
		CHARACTER_LOGIC_SKILL_LEAD,
		CHARACTER_LOGIC_SKILL_SEND,
		CHARACTER_LOGIC_ABILITY_ACTION,
		CHARACTER_LOGIC_DEAD,
		CHARACTER_LOGIC_STALL,

		CHARACTER_LOGIC_NUMBERS
	};

	//----------------------------------------------------------
	//�����ӿ�
	//----------------------------------------------------------
public:
	//���ؽ�ɫ����
	virtual CHARACTER_TYPE	GetCharacterType( VOID ) const { return CT_INVALID;	}
	//��ȡͷ����Ϣ��
	tCreatureBoard*			GetInfoBoard(void) { return m_pInfoBoard; }
	//�Ƿ��ܹ�����Ϊ��Ŀ��ѡ��
	virtual BOOL			CanbeSelect(VOID) const { return true; }
	//
	virtual UINT			GetIdleInterval(VOID)const{ return UINT_MAX; }
	UINT					CalcIdleRandInterval(VOID);

	BOOL					IsDie(VOID)const;

public:
	//----------------------------------------------------------
	//��ɫ�߼��������
	//----------------------------------------------------------
	//�õ��߼�����
	CCharacterData*			GetCharacterData(VOID) { return m_pCharacterData; }
	const CCharacterData*	GetConstCharacterData(VOID)const{ return m_pCharacterData; }
	//��ȡ������ɫ�������
	STRING					GetNameWithColor(void);

protected:
	friend class CCharacterData;
	friend class CAI_MySelf;
	virtual VOID			OnDataChanged_RaceID(VOID);
	virtual VOID			OnDataChanged_ModelID(VOID);
	virtual VOID			OnDataChanged_MountID( VOID );
	virtual VOID			OnDataChanged_Name(VOID);
	virtual VOID			OnDataChanged_CurTitles(VOID);
	virtual VOID			OnDataChanged_MoveSpeed(VOID);
	virtual VOID			OnDataChanged_StallName(VOID);
	virtual VOID			OnDataChanged_IsInStall(VOID);
	virtual VOID			OnDataChanged_TeamLeaderFlag(VOID);
	virtual VOID			OnDataChanged_Die(VOID);
	virtual VOID			OnDataChanged_StealthLevel(VOID);
	virtual VOID			OnDataChanged_FightState(VOID);
	virtual VOID			OnDataChanged_Sit(VOID);
	virtual VOID			OnDataChanged_Level(VOID);

	//��ɫ�߼�����
	CCharacterData*			m_pCharacterData;

	//----------------------------------------------------------
	// ����
	//----------------------------------------------------------
public:
	ENUM_WEAPON_TYPE		GetWeaponType( VOID ) const				{ return m_theWeaponType; }

protected:
	// ��������
	VOID					SetWeaponType(ENUM_WEAPON_TYPE type)	{ m_theWeaponType = type; }

	//��ǰ��������
	ENUM_WEAPON_TYPE		m_theWeaponType;

	//----------------------------------------------------------
	//��ɫ����Ч���߼����
	//----------------------------------------------------------
public:
	const CImpactEffectMap*	GetImpactEffectMap( VOID )const	{ return &m_mapImpactEffect; }
	VOID					RemoveAllImpact( VOID );

protected:
	VOID					UpdateBuffEffect(VOID);

	VOID					ChangeImpact( INT idImpact, ObjID_t nCreatorID, BOOL bEnable, BOOL bAttrRefesh, BOOL bShowEnableEffect );
	BOOL					InsertImpact( INT idImpact, ObjID_t nCreatorID, BOOL bShowEnableEffect );
	VOID					RemoveImpact( INT idImpact );
	VOID					UpdatePetFrame();

	VOID					Tick_UpdateEffect(VOID);
	VOID					UpdateEffectTargetPos(const SImpactEffect *pImpactEffect);

	VOID					SetElement_Title(LPCTSTR szTitle, INT nType);

protected:
	//����Ч������������Ч
	CImpactEffectMap		m_mapImpactEffect;		

public:
	// ��Ծ���
	VOID					SetbJumping( BOOL bJumping ) { m_bJumping = bJumping; }
	BOOL					GetbJumping( VOID ) const { return m_bJumping; }
	BOOL					Jump( VOID );

private:
	BOOL					m_bJumping;

public:
	BOOL					SitDown(VOID);
	BOOL					StandUp(VOID);

	//----------------------------------------------------------
	// ָ�����
	//----------------------------------------------------------
public:
	BOOL					ModifyMoveCommand(const INT nLogicCount, const BYTE numPos, const WORLD_POS* targetPos);
	// ѹ��һ��ָ��
	virtual BOOL			PushCommand( const SCommand_Object *pCmd );

protected:
	// ִ�п����˵�ģ��ָ�ֻ����AI���ã������ط����ã��������ջ����ָ���
	RC_RESULT				DoSimulantCommand(const SCommand_Object *pCmd);

	virtual RC_RESULT		DoCommand( const SCommand_Object *pCmd );
	virtual RC_RESULT		OnCommand( const SCommand_Object *pCmd );

	// �������ָ��
	virtual BOOL			ProcessLogicCommand( VOID );
	BOOL					DoNextLogicCommand(VOID);
	BOOL					DoLogicCommand(CObjectCommand_Logic *pLogicCmd);

	VOID					SetLogicSpeed(FLOAT fSpeed);
	FLOAT					GetLogicSpeed(VOID)const{ return m_fLogic_Speed; }

	VOID					OnLogicSpeedChanged(VOID);

protected:
	BOOL						IsLogicCommandListEmpty(VOID);
	BOOL						PushLogicCommand(CObjectCommand_Logic *pCmd);
	const CObjectCommand_Logic	*GetNextLogicCommand(VOID)const;
	CObjectCommand_Logic		*NextLogicCommand(VOID);
	CObjectCommand_Logic		*FindLogicCommand(INT nLogicCount);

	virtual BOOL				CleanupLogicCommandList( VOID );

protected:
	typedef std::list<CObjectCommand_Logic*>	CObjectLogicCommandList;
	CObjectLogicCommandList						m_listLogicCommand;
	FLOAT										m_fLogic_Speed;					// ��ǰ��Ҿִ�е��ٶ�

	//----------------------------------------------------------
	//��ɫ��Ϊ�߼����
	//----------------------------------------------------------
public:
	// �õ�AI
	CAI_Base *CharacterLogic_GetAI(VOID)
	{
		return m_pTheAI;
	}

	// �õ���ǰ���߼�״̬
	ENUM_CHARACTER_LOGIC CharacterLogic_Get( VOID )const
	{
		return m_eCharacterLogic;
	}

	// ��ǰ�߼��Ƿ�ֹͣ
	BOOL CharacterLogic_IsStopped( VOID )const
	{
		return m_bCharacterLogicStopped;
	}

	virtual BOOL IsLogicReady( INT nLogicCount )const;

	INT GetLastModifyPosLogicCount(VOID)const
	{
		return m_nLastModifyPosLogicCount;
	}

	BOOL IsIdle(VOID)const
	{
		return (CharacterLogic_Get() == CHARACTER_LOGIC_IDLE)?(TRUE):(FALSE);
	}

	BOOL IsMoving(VOID)const
	{
		return (CharacterLogic_Get() == CHARACTER_LOGIC_MOVE && !CharacterLogic_IsStopped())?(TRUE):(FALSE);
	}

protected:
	//���õ�ǰ���߼�״̬
	VOID CharacterLogic_Set(ENUM_CHARACTER_LOGIC eLogic);

	// �߼�ֹͣ����
	VOID CharacterLogic_Stop(BOOL bFinished);

	// �߼�ֹͣ�¼����κ��߼����κ�����ֹͣ���õ���(��������ĵ�������������һ�߼�֮ǰ)
	// bFinished	:	�Ƿ�Ϊ�ɹ�ִ�����
	virtual VOID CharacterLogic_OnStopped(BOOL bFinished);

	VOID SetLastModifyPosLogicCount(INT nLogicCount)
	{
		m_nLastModifyPosLogicCount = nLogicCount;
	}

    //����ĳһ���߼�״̬
	BOOL			Start_Idle(VOID);
	BOOL			Start_Action(CObjectCommand_Logic *pLogicCommand);
	BOOL			Start_Move(CObjectCommand_Logic *pLogicCommand);
	BOOL			Start_MagicCharge(CObjectCommand_Logic *pLogicCommand);
	BOOL			Start_MagicChannel(CObjectCommand_Logic *pLogicCommand);
	BOOL			Start_MagicSend(CObjectCommand_Logic *pLogicCommand);
	BOOL			Start_Dead( BOOL bPlayDieAni );
	BOOL			Start_Ability(CObjectCommand_Logic *pLogicCommand);
	BOOL			Start_Stall(BOOL bPlayAni);

protected:
	//��һ���߼�״̬�µ��߼�Tick
	virtual BOOL	Tick_Idle(UINT uElapseTime);
	virtual BOOL	Tick_Action(UINT uElapseTime);
	virtual BOOL	Tick_Move(UINT uElapseTime);
	virtual BOOL	Tick_MagicCharge(UINT uElapseTime);
	virtual BOOL	Tick_MagicChannel(UINT uElapseTime);
	virtual BOOL	Tick_MagicSend(UINT uElapseTime);
	virtual BOOL	Tick_Dead(UINT uElapseTime);
	virtual BOOL	Tick_Ability(UINT uElapseTime);
	virtual BOOL	Tick_Stall(UINT uElapseTime);

protected:
	CObjectCommand_Logic	*GetCurrentLogicCommand(VOID);
	VOID					SetCurrentLogicCommand(CObjectCommand_Logic *pLogicCommand);

	VOID					CheckMoveStop(VOID);

protected:
	//��ĳ���߼�״̬�µĲ���
	struct SLogicParam_Idle
	{
		UINT		m_uIdleInterval;				// �������ʱ��������һ�����ж���
		UINT		m_uLastIdleMotionTime;			// ��һ�������ж�����ʱ��
		UINT		m_uStartTime;					// �����߼�����ʼʱ��
	};

	struct SLogicParam_Move
	{
		INT			m_nCurrentNodeIndex;			// ��ǰ�ڵ������ֵ
		WORLD_POS	m_posSaveStart;					// ���浱ǰ����ʼ��
		WORLD_POS	m_posSaveTarget;				// ���浱ǰ��Ŀ���
	};

	struct SLogicParam_MagicCharge
	{
		UINT		m_uCurrentTime;					// ��ǰ����ʱ��
	};

	struct SLogicParam_MagicChannel
	{
		UINT		m_uCurrentTime;					// ��ǰ����ʱ��
	};

	struct SLogicParam_MagicSend
	{
		INT			m_nSaveMagicID;					// ��ǰ����ID
		BOOL		m_bBeAttackEffectShowed;		// ��˴ι����������ı������ߵı����Ƿ��Ѿ�����
		BOOL		m_bCanBreak;					// �Ƿ���Խ������߼���
		INT			m_nActionIndex;					// Ϊ���ö�����˳�򲥷�
		INT			m_nActionCount;					// ��������
		BOOL		m_bDoNextAction_Concatenation;	// �����ܵ���ʽ����ΪSKILL_ACTION_TYPE_CONCATENATIONʱ����
		UINT		m_uAnimationTime;				// ���Ѿ����ŵ�ʱ��
		UINT		m_uAnimationEndElapseTime;		// ����ֹͣ��ʱ����
	};

	struct SLogicParam_Ability
	{
		UINT		m_uTotalTime;
		UINT		m_uCurrentTime;
	};

	struct SLogicParam_Sit
	{
		BOOL		m_bStandUp;
	};

	struct SLogicParam_Stall
	{
		enum ENUM_STALL_ANIMATION
		{
			STALL_ANIMATION_INVALID	= -1,
			STALL_ANIMATION_SITDOWN,
			STALL_ANIMATION_IDLE,
			STALL_ANIMATION_STANDUP,

			STALL_ANIMATION_NUMBER
		};
		INT			m_nCurrentAnimation;			// ENUM_STALL_ANIMATION
	};

	//��ǰ���߼�״̬
	ENUM_CHARACTER_LOGIC		m_eCharacterLogic;
	//��ǰ�߼��Ƿ�ֹͣ
	BOOL						m_bCharacterLogicStopped;
	//AI����
	CAI_Base*					m_pTheAI;

	INT							m_nLastServerLogicCount;		// ���õ��ķ������˵��߼�����
	INT							m_uLastServerTime;				// ���õ��ķ�������ʱ��
	INT							m_nLastModifyPosLogicCount;		// ���ִ�еĸı�λ�õ�ָ��
	CObjectCommand_Logic		*m_pCurrentLogicCommand;		// ��ǰ����ִ�е�ָ��
	SLogicParam_Idle			m_paramLogic_Idle;
	SLogicParam_Move			m_paramLogic_Move;
	SLogicParam_MagicCharge		m_paramLogic_MagicCharge;
	SLogicParam_MagicChannel	m_paramLogic_MagicChannel;
	SLogicParam_MagicSend		m_paramLogic_MagicSend;
	SLogicParam_Ability			m_paramLogic_Ability;
	SLogicParam_Sit				m_paramLogic_Sit;
	SLogicParam_Stall			m_paramLogic_Stall;

	//----------------------------------------------------------
	// �˺��Ĺ�������
	//----------------------------------------------------------
public:
	BOOL					IsMySelf(INT ObjID) const;
	BOOL					IsMyPet(INT ObjID) const;
	VOID					ShowLogicEvent( INT nServerObjID, INT nLogicCount, BOOL bShowAll = FALSE );

protected:
	VOID					AddLogicEvent(const _LOGIC_EVENT *pLogicEvent);
	VOID					RemoveLogicEvent( INT nLogicCount );
	VOID					RemoveAllLogicEvent( VOID );

	VOID					Update_LogicEvent( VOID );
	// ��˴ι����������ı������ߵı����Ƿ��Ѿ�����
	BOOL					IsBeAttackEffectShowed( VOID ) const
	{
		return m_paramLogic_MagicSend.m_bBeAttackEffectShowed;
	}

	VOID					DoLogicEvent(const _LOGIC_EVENT *pLogicEvent);
	VOID					DoLogicEvent_Damage(const _LOGIC_EVENT *pLogicEvent);
	VOID					DoLogicEvent_Bullet(const _LOGIC_EVENT *pLogicEvent);
	VOID					DisplayDamageBoard(const _DAMAGE_INFO* pDamageInfo);
	VOID					DisplayMissImmuAndSoOn(const _DAMAGE_INFO* pDamageInfo);

protected:
	CLogicEventList			m_listLogicEvent;

	UINT					m_uTime_LogicEventListTick;

	//--------------------------------------------------------
	// ��ɫ�������
	//--------------------------------------------------------
protected:
	//����Ⱦ�㴴����Ⱦָ��
	virtual VOID			CreateRenderInterface(VOID);
	virtual VOID			ReleaseRenderInterface(VOID);

	virtual VOID			CreateMountRenderInterface(VOID);
	virtual VOID			ReleaseMountRenderInterface( VOID );

	// ѡ�л���С
	virtual FLOAT			GetProjtexRange(VOID) { return 100.0f; }
	// ��Ӱ��С
	virtual FLOAT			GetShadowRange(VOID)  { return 100.0f; }

protected:
	
	VOID					ChangeActionSpeed(FLOAT fSpeed);

	BOOL					IsAnimationEnd(VOID)const{return m_bAnimationEnd;}
	VOID					SetAnimationEnd(BOOL bSet){m_bAnimationEnd = bSet;}

	VOID					UpdateTransparent(VOID);
	VOID					SetTransparent(FLOAT fTransparency, FLOAT fTime = 1.f);

	//ˢ�µ�ǰ����
	VOID					FreshAnimation( VOID );

protected:
	// nActionSetID	:	ENUM_BASE_ACTION or ����
	// nWeaponType	:	ENUM_WEAPON_TYPE
	virtual LPCSTR			GetCharActionNameByActionSetID(INT nActionSetID, INT nWeaponType, BOOL *pbHideWeapon, INT *pnAppointedWeaponID);
	virtual LPCSTR			GetMountActionNameByActionSetID(INT nActionSetID, INT nWeaponType);

	virtual INT				GetCharModelID( VOID )const;
	virtual INT				GetMountModelID( VOID )const;

	virtual INT				CalcCharModelID( VOID )const;

	BOOL					IsFightState(VOID)const{ return m_uFightStateTime > 0; }
	VOID					SetFightState(BOOL bSet);

protected:
	VOID					UpdateCharModel(VOID);
	VOID					UpdateMountModel(VOID);
	virtual VOID			UpdateCharRace(VOID){}
	VOID					UpdateCharActionSetFile(VOID);
	VOID					UpdateMountActionSetFile(VOID);

	virtual VOID			OnHideWeapon(INT nAppointedWeaponID){}
	virtual VOID			OnShowWeapon(VOID){}

	BOOL					IsHideWeapon(VOID)const
	{
		return m_bHideWeapon;
	}

protected:
	const DBC::DBCFile		*m_pCharActionSetFile;
	const DBC::DBCFile		*m_pMountActionSetFile;

	tEntityNode*			m_pMountRenderInterface;		// �������Ⱦ��ӿ�

	INT						m_nCharModelID;
	INT						m_nMountModelID;
	//���������ͷ����Ϣ��߶�ƫ��
	FLOAT					m_fMountAddHeight;
	BOOL					m_bHideWeapon;

	BOOL					m_bAnimationEnd;

	UINT					m_uFightStateTime;

protected:
	//--------------------------------------------------------
	//��ǰ����Ⱦ����ִ�еĶ����������¼�֪ͨ����
	static bool			_OnAnimationEnd(LPCTSTR szAnimationName, UINT dwParam);
	virtual BOOL		OnAnimationEnd(LPCTSTR szAnimationName);

	//--------------------------------------------------------
	//��ǰ����Ⱦ����ִ�еĶ�������ֹͣ���¼�֪ͨ����
	static bool			_OnAnimationCanBreak(LPCTSTR szAnimationName, UINT dwParam);
	virtual BOOL		OnAnimationCanBreak(LPCTSTR szAnimationName);

	//--------------------------------------------------------
	//��ǰ����Ⱦ����ִ�еĶ����������¼�֪ͨ����
	static bool			_OnAnimationHit(LPCTSTR szAnimationName, UINT dwParam);
	virtual BOOL		OnAnimationHit(LPCTSTR szAnimationName);

	//--------------------------------------------------------
	//��ǰ����Ⱦ����ִ�еĶ�����Ļ�𶯵��¼�֪ͨ����
	static bool			_OnAnimationShakeEvent(LPCTSTR szAnimationName, UINT dwParam);
	virtual BOOL		OnAnimationShakeEvent(LPCTSTR szAnimationName);

	//--------------------------------------------------------
	//֪ͨһ�������¼�����
	virtual VOID		NotifyPhyEvent(PHY_EVENT_ID eventid, VOID* pParam = NULL);

	//--------------------------------------------------------
	//�ƶ�·�����
	//----------------------------------------------------------
public:
	//����Ŀ��㴴������·��
	virtual VOID		OnMoveStop( VOID );
	VOID					ChangeAction(INT nSetID, FLOAT fSpeed, BOOL bLoop, FLOAT fFuseParam=0.3 );
	//--------------------------------------------------------
	//�������
	//----------------------------------------------------------
public:
	virtual INT					CannotBeAttack( VOID )const { return 0;}
	virtual const _CAMP_DATA	*GetCampData(VOID) const;
	virtual ObjID_t				GetOwnerID(VOID)const;

	//--------------------------------------------------------
	//ͷ����Ϣ�����
	//--------------------------------------------------------
protected:
	//UI�ӿ�
	tCreatureBoard*			m_pInfoBoard;
	//λ��
	fVector2				m_fvInfoBoardPos;

	//--------------------------------------------------------
	// ��ɫ��Ч���
	//--------------------------------------------------------
protected:
	//�Ų�����
	tSoundSource*			m_pWlakSound;
	//���������
	tSoundSource*			m_pLifeAbilitySound;

	//--------------------------------------------------------
	// ��ɫ���춯�����
	//--------------------------------------------------------
public:
	VOID					SetChatMoodAction(STRING& strOrder);	//���ö����б�

protected:
	VOID					ClearAllChatMoodAction();				//���������ʣ��Ķ���
	VOID					PlayChatMoodAction();					//ÿ�δӶ����ﵯ��һ���������в��ţ���ɫ�����߼����봦��idle״̬����
	BOOL					IsHaveChatMoodAction();					// �Ƿ������춯��

	BOOL					IsChatMoodActionPlaying(){return m_bIsChatMoodPlaying;};
	VOID					SetChatMoodActionPlaying(BOOL bPlaying){m_bIsChatMoodPlaying = bPlaying;};

protected:
	BOOL					m_bIsChatMoodPlaying;
	BOOL					m_bNeedStandUp;
	BOOL					m_bStanding;
	struct CHAT_MOOD_DATA
	{
		INT			m_ActionId;
		BOOL		m_bLoop;

		CHAT_MOOD_DATA()
		{
			m_ActionId = -1;
			m_bLoop = FALSE;
		};
	};

	typedef std::list< CHAT_MOOD_DATA >			CHAT_MOOD_LIST;
	CHAT_MOOD_LIST			m_listChatMoodAction;

	//--------------------------------------------------------
	//������¼�
	//--------------------------------------------------------
public:
	BOOL	AddDropBoxEvent(ObjID_t idItemBox, GUID_t idOwner, const WORLD_POS *pCreatePos);

private:
	BOOL			m_bDropBox_HaveData;
	ObjID_t			m_nDropBox_ItemBoxID;
	GUID_t			m_DropBox_OwnerGUID;
	WORLD_POS		m_posDropBox_CreatePos;

	//--------------------------------------------------------
	//���ļ̳�
	//--------------------------------------------------------
public:
	CObject_Character();
	virtual ~CObject_Character();

	// ��ɫ��ʼ��
	virtual VOID			Initial(VOID* pParam);
	// �ͷ�������ӵ�е���Դ
	virtual VOID			Release(VOID);
	///�߼�����
	virtual VOID			Tick(VOID);
	////����Ƿ��ٿɼ�
	//virtual VOID			Tick_CheckAlive(VOID);
	//��Ϣ��Tick
	virtual VOID			Tick_UpdateInfoBoard(VOID);

	virtual VOID			SetPosition(const fVector3& fvPosition);
	virtual VOID			SetRotation(const fVector3& fvRotation);
	virtual VOID			Enable(UINT dwFlag);
	virtual VOID			Disalbe(UINT dwFlag);


	//---- for debuger
	virtual STRING			GetDebugDesc(VOID);
	//---- for debuger

	void			Say( const STRING& str );
protected:
	//--------------------------------------------------------
	WX_DECLARE_DYNAMIC(CObject_Character);
	//TD_DECLARE_LOGICAL(true);
};

#endif // __OBJ_CHARACTER_H__
