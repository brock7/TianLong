// CharacterStruct.h
//
//////////////////////////////////////////////////////

#ifndef __CHARACTERSTRUCT_H__
#define __CHARACTERSTRUCT_H__

#pragma once

#include "Type.h"
#include <map>
#include "GameStruct_Impact.h"

// ��Ч��
struct SEffectSet
{
protected:
	UINT	m_uEffect;

public:
	SEffectSet( VOID )
	{
		m_uEffect = NULL;
	}

	virtual VOID Reset( VOID )
	{
		m_uEffect = 0;
	}

	VOID SetEffect( UINT uEffect )
	{
		m_uEffect = uEffect;
	}

	UINT GetEffect( VOID )const
	{
		return m_uEffect;
	}
};

struct SImpactEffect :	public SEffectSet
{
protected:
	UINT		m_dwRefCount;
	UINT		m_dwImpactID;
	BOOL		m_bLineEffect;			// ���ߵ���Ч
	ObjID_t		m_CreatorID;			// ������ID
	LPCSTR		m_lpszCreatorLocator;	// �����ߵİ󶨵�

public:
	SImpactEffect( VOID )
	{
		m_dwRefCount			= 0;
		m_dwImpactID			= -1;
		m_bLineEffect			= FALSE;
		m_CreatorID				= INVALID_ID;
		m_lpszCreatorLocator	= NULL;
	}

	virtual VOID Reset( VOID )
	{
		m_dwRefCount			= 0;
		m_dwImpactID			= -1;
		m_bLineEffect			= FALSE;
		m_CreatorID				= INVALID_ID;
		m_lpszCreatorLocator	= NULL;
		SEffectSet::Reset();
	}

	VOID	SetImpactID( UINT dwImpactID )	{ m_dwImpactID = dwImpactID;	}
	UINT	GetImpactID( VOID )const		{ return m_dwImpactID;			}
	UINT	GetRefCount( VOID )const		{ return m_dwRefCount;			}
	VOID	AddRefCount( VOID )				{ m_dwRefCount++;				}
	VOID	DecRefCount( VOID )
	{
		if ( m_dwRefCount > 0 )
			m_dwRefCount--;
	}
	BOOL	IsLineEffect(VOID)const{return m_bLineEffect;}
	VOID	SetLineEffect(BOOL bSet){m_bLineEffect = bSet;}
	ObjID_t	GetCreatorID(VOID)const{return m_CreatorID;}
	VOID	SetCreatorID(ObjID_t nCreatorID){m_CreatorID = nCreatorID;}
	LPCSTR	GetCreatorLocator(VOID)const{return m_lpszCreatorLocator;}
	VOID	SetCreatorLocator(LPCSTR lpszCreatorLocator){m_lpszCreatorLocator = lpszCreatorLocator;}
};
typedef std::map< UINT, SImpactEffect*> CImpactEffectMap;


struct _LOGIC_EVENT_BULLET
{
	INT				m_nBulletID;
	const CHAR		*m_pszSenderLocator;
	BOOL			m_bHitTargetObj;		// Ŀ���Ƿ�Ϊ��ɫ
	ObjID_t			m_nTargetID;
	FLOAT			m_fTargetX, m_fTargetZ;

	_LOGIC_EVENT_BULLET(VOID)
	{
		m_nBulletID			= INVALID_ID;
		m_pszSenderLocator	= NULL;
		m_bHitTargetObj		= TRUE;
		m_nTargetID			= INVALID_ID;
		m_fTargetX			= -1.f;
		m_fTargetZ			= -1.f;
	}

	VOID Reset(VOID)
	{
		m_nBulletID			= INVALID_ID;
		m_pszSenderLocator	= NULL;
		m_bHitTargetObj		= TRUE;
		m_nTargetID			= INVALID_ID;
		m_fTargetX			= -1.f;
		m_fTargetZ			= -1.f;
	}
};

enum ENUM_LOGIC_EVENT_TYPE
{
	LOGIC_EVENT_TYPE_INVALID	= -1,
	LOGIC_EVENT_TYPE_BULLET,
	LOGIC_EVENT_TYPE_DAMAGE,

	LOGIC_EVENT_TYPE_NUMBERS
};

struct _LOGIC_EVENT
{
	UINT					m_uBeginTime;			// ����Ч�����õĿ�ʼʱ��
	UINT					m_uRemoveTime;			// ����Ч�����õĳ���ʱ��
	ObjID_t					m_nSenderID;			// �����ߵ�ID
	INT						m_nSenderLogicCount;	// �����ߵ��߼�����

	INT						m_nEventType;			// ���� ENUM_LOGIC_EVENT_TYPE
	//union
	//{
		_DAMAGE_INFO			m_damage;			// ����Ч�����������˺���Ϣ
		_LOGIC_EVENT_BULLET		m_bullet;			// �ӵ�
	//};

	_LOGIC_EVENT( VOID )
	{
		m_uBeginTime	= 0;
		m_uRemoveTime	= 1000;
	}

	VOID Init(ObjID_t nSenderID, INT nSenderLogicCount, const _DAMAGE_INFO *pDamageInfo)
	{
		m_nSenderID			= nSenderID;
		m_nSenderLogicCount	= nSenderLogicCount;

		m_nEventType		= LOGIC_EVENT_TYPE_DAMAGE;
		m_damage			= *pDamageInfo;
	}

	VOID Init(ObjID_t nSenderID, INT nSenderLogicCount, const _LOGIC_EVENT_BULLET *pBulletInfo)
	{
		m_nSenderID			= nSenderID;
		m_nSenderLogicCount	= nSenderLogicCount;

		m_nEventType		= LOGIC_EVENT_TYPE_BULLET;
		m_bullet			= *pBulletInfo;
	}

	VOID Reset( VOID )
	{
		m_uBeginTime	= 0;
		m_uRemoveTime	= 1000;

		m_damage.Reset();
		m_bullet.Reset();
	}
};

typedef std::list< _LOGIC_EVENT* >		CLogicEventList;


#endif // __CHARACTERSTRUCT_H__
