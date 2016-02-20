// AI_MySelf.h
//
//////////////////////////////////////////////////////

#pragma once

#include "TDMath.h"
#include "GMAI_Base.h"
#include "Type.h"

// AIҪ����һ������LOGIC
enum ENUM_MYSELF_AI
{
	MYSELF_AI_INVALID	= -1,
	MYSELF_AI_IDLE,					//������
	MYSELF_AI_MOVE,					//�ƶ���
	MYSELF_AI_USE_SKILL,			//����ʹ����
	MYSELF_AI_ACTIVE_TRIPPEROBJ,	//��TripperObj������
	MYSELF_AI_DEFAULT_EVENT,		//��npc̸��������
	//MYSELF_AI_DEAD,					//����
	MYSELF_AI_FOLLOW,				//����
	MYSELF_AI_GUAJI,				//�һ�
	//...

	MYSELF_AI_NUMBERS
};

class CPath;
class CAI_MySelf : public CAI_Base
{
public:
	//�߼���
	virtual BOOL Tick( VOID );

	//����
	virtual VOID Reset(VOID);

	virtual BOOL PushCommand_MoveTo(FLOAT fDestX, FLOAT fDestZ);
	virtual BOOL PushCommand_AutoHit(INT isAutoHit);
	virtual BOOL PushCommand_Jump(VOID);
	virtual BOOL PushCommand_UseSkill(INT idSkill, GUID_t guidTarget, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir);

	// ѹ��һ��ָ��
	virtual BOOL PushCommand( const SCommand_AI *pCmd );

	BOOL IsMoving(VOID)const
	{
		return GetMySelfAI() == MYSELF_AI_MOVE;
	}

	BOOL IsFollow(VOID)const
	{
		return GetMySelfAI() == MYSELF_AI_FOLLOW;
	}

	//��ǰAI�߼�״̬
	ENUM_MYSELF_AI GetMySelfAI(VOID)const
	{
		return m_eMySelfAI;
	}

protected:
	VOID				SetForbidTime(UINT nForbidTime);

protected:
	SCommand_AI			m_SaveCommand;
	UINT				m_uForbidTime;		// ��ֹ������ʱ��
	UINT				m_uForbidStartTime;	// ��ֹ��������ʼʱ��
	UINT				StartGuaJi;	// ��ֹ��������ʼʱ��

protected:
	//AIָ��
	virtual RC_RESULT	OnCommand( const SCommand_AI *pCmd );

protected:
	RC_RESULT			AI_MoveTo( FLOAT fDestX, FLOAT fDestZ );

	// ��Ծ���
public:
	BOOL				IsCanJump( VOID );
	RC_RESULT			Jump( VOID );
	INT					Find_Platform( INT idAbility );
	RC_RESULT			ComposeItem( INT idPrescription );

public:
	// �Ƿ�������ĳ��������ָ��
	BOOL IsLimitCmd( const SCommand_AI *pCmd );

protected:
	//-------------------------------------
	//����ĳ�߼�״̬
	RC_RESULT Enter_Idle( VOID );
	RC_RESULT Enter_Move( FLOAT fDestX, FLOAT fDestZ );
	RC_RESULT Enter_UseSkill( INT idSkill, INT nLevel, GUID_t guidTarget, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir );
	RC_RESULT Enter_ActiveTripperObj( INT idItemBox );
	RC_RESULT Enter_DefaultEvent( INT idTargetObj );
	//RC_RESULT Enter_Dead( VOID );
	BOOL Enter_Follow( ObjID_t idTargetObj );

protected:
	//-------------------------------------
	//ĳ�߼�״̬�е�Tick��
	BOOL Tick_Idle( VOID );
	BOOL Tick_Move( VOID );
	BOOL Tick_UseSkill( VOID );
	BOOL Tick_ActiveTripperObj( VOID );
	BOOL Tick_DefaultEvent( VOID );
	//BOOL Tick_Dead( VOID );
	BOOL Tick_Follow( VOID );
	BOOL Tick_AutoHit( VOID );
protected:
	virtual VOID OnAIStopped( VOID );
	VOID StopFollow( VOID );
	fVector3 GetTargetPos(fVector3 MePos, fVector3 TargetPos, FLOAT fDist);

protected:
	VOID			SetMySelfAI(ENUM_MYSELF_AI eAI)	{ m_eMySelfAI = eAI; }

	//���������������
	VOID			SendMoveMessage( const CPath *pPath, INT nLogicCount );
	VOID			SendUseSkillMessage( INT idSkill, INT nLevel, GUID_t guidTarget, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir );
	VOID			SendActiveTripperObjMessage( INT idTripperObj );
	VOID			SendComposeItemMessage( INT idAbility, INT idPrescription,	ObjID_t guidPlatform );
	VOID			SendDefaultEventMessage( INT idTargetObj );

	//�ж������Ƿ����ʹ�ü���
	RC_RESULT		IsCanUseSkill( INT idSkill, INT idLevel, GUID_t guidTarget, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir, BOOL& bNeedMove );
	BOOL			IsCanFollow();
	SCommand_AI		ConvertCommand( const SCommand_Object* pCmd );

private:
	ENUM_MYSELF_AI	m_eMySelfAI;

protected:
	struct SAIParam_Move
	{
		WORLD_POS	m_posTarget;
	};

	struct SAIParam_UseSkill
	{
		SkillID_t	m_idSkill;
		BYTE		m_SkillLevel;
		GUID_t		m_guidTarget;
		INT			m_idTarget;
		fVector3	m_posTarget;
		FLOAT		m_fDir;
		fVector3	m_posPrevTarget;
	};

	struct SAIParam_ActiveTripperObj
	{
		INT		m_idTripperObj;
	};

	struct SAIParam_DefaultEvent
	{
		INT			m_idTargetObj;
		fVector3	m_posPrevTarget;
	};

	struct SAIParam_Follow
	{
		const static INT		m_nMaxTickCount = 10;
		INT						m_nTickCount;
		ObjID_t					m_idTargetObj;
		fVector3				m_LastPos;
	};

	SAIParam_Move				m_paramAI_Move;
	SAIParam_UseSkill			m_paramAI_UseSkill;
	SAIParam_ActiveTripperObj	m_paramAI_ActiveTripperObj;
	SAIParam_DefaultEvent		m_paramAI_DefaultEvent;
	SAIParam_Follow				m_paramAI_Follow;

public:
	CAI_MySelf(CObject_Character* pCharObj);
	virtual ~CAI_MySelf( VOID );
};
