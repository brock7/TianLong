// Obj_Bullet.h
// 
/////////////////////////////////////////////////////

#ifndef __OBJ_BULLET_H__
#define __OBJ_BULLET_H__

#pragma once

#include "Type.h"
#include "Obj_Effect.h"

struct SObject_BulletInit :
	public SObject_EffectInit
{
	ObjID_t		m_idSend;			// ������
	INT			m_nBulletID;		// �ӵ�ID
	INT			m_nSendLogicCount;	// �����ߵ��߼�����

	BOOL		m_bSingleTarget;	// ��Ŀ�ꣿ
	INT			m_idTarget;
	fVector3	m_fvTargetPos;

	SObject_BulletInit( VOID ){
		m_idSend			= INVALID_ID;
		m_nBulletID			= INVALID_ID;
		m_nSendLogicCount	= -1;

		m_bSingleTarget		= FALSE;
		m_idTarget			= INVALID_ID;
		m_fvTargetPos		= fVector3( -1.f, -1.f, -1.f );
	}

	virtual VOID Reset( VOID ){
		SObject_EffectInit::Reset();
		m_idSend			= INVALID_ID;
		m_nBulletID			= INVALID_ID;
		m_nSendLogicCount	= -1;

		m_bSingleTarget		= FALSE;
		m_idTarget			= INVALID_ID;
		m_fvTargetPos		= fVector3( -1.f, -1.f, -1.f );
	}
};

struct _DBC_BULLET_DATA;
class CObject_Bullet : public CObject_Effect
{
public:
	CObject_Bullet();
	virtual ~CObject_Bullet();

public:
	virtual	VOID Initial( VOID *pInit );

	virtual VOID Tick( VOID );

protected:
	VOID AlreadyHit( VOID );

protected:
	ObjID_t					m_idSend;			// ������
	const _DBC_BULLET_DATA	*m_pBulletData;		// �ӵ�����
	INT						m_nSendLogicCount;	// �����ߵ��߼�����

	BOOL					m_bSingleTarget;	// ��Ŀ�ꣿ
	INT						m_idTarget;
	fVector3				m_fvTargetPos;
	fVector3				m_fvStartPos;		// ��ʼ������
	FLOAT					m_fStartToEndDist;	// ��ʼ�㵽������ľ���

	BOOL					m_bAlreadyHit;		// �Ѿ�����

protected:
	WX_DECLARE_DYNAMIC(CObject_Bullet);
};

#endif // __OBJ_BULLET_H__
