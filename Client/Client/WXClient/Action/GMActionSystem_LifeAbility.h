/****************************************\
*										*
* 		  ����������-�����			*
*										*
\****************************************/

#pragma once

#include "GMActionSystem.h"

class CActionItem_LifeAbility : public CActionItem
{
public:
	//�õ���������
	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_SKILL_LIFEABILITY;	}
	//�����ַ���
	virtual	LPCTSTR			GetType_String(VOID)	{ return NAMETYPE_LIFESKILL; }
	//�õ�����ID
	virtual INT				GetDefineID(VOID);
	//�õ�����
	virtual INT				GetNum(VOID)			{ return -1;	}
	//�õ��ڲ�����
	virtual VOID*			GetImpl(VOID)			{ return (VOID*)GetLifeAbilityImpl();	}
	//�õ�����
	virtual LPCTSTR			GetDesc(VOID);
	//�õ���ȴ״��ID
	virtual INT				GetCoolDownID(VOID)		{ return -1; }
	//�õ���������������
	virtual INT				GetPosIndex(VOID);
	//�Ƿ��ܹ��Զ���������
	virtual BOOL			AutoKeepOn(VOID)		{ return FALSE; }
	//�����
	virtual VOID			DoAction(VOID);
	//�Ƿ���Ч
	virtual BOOL			IsValidate(VOID)		{ return TRUE; }
	//ˢ��
	virtual VOID			Update(tActionReference* pRef) {}
	//�����ȴ�Ƿ����
	virtual BOOL			CoolDownIsOver(VOID)	{ return TRUE; }
	//�϶�����
	virtual VOID			NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName);

public:
	CActionItem_LifeAbility(INT nID);
	virtual ~CActionItem_LifeAbility();

	//���������
	VOID	Update_LifeAbility(const SCLIENT_LIFEABILITY* pAbility);

protected:
	//���������
	const SCLIENT_LIFEABILITY*	GetLifeAbilityImpl(VOID);

protected:
	//�����ID
	INT		m_idLifeAbilityImpl;

};