/****************************************\
*										*
* 			  ����������-����			*
*										*
\****************************************/

#pragma once
#include "GMActionSystem.h"

class CActionItem_Skill : public CActionItem
{
public:
	//�õ���������
	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_SKILL;	}
	//�����ַ���
	virtual	LPCTSTR			GetType_String(VOID)	{ return NAMETYPE_SKILL;	}
	//�õ�����ID
	virtual INT				GetDefineID(VOID);
	//�õ�����
	virtual INT				GetNum(VOID)			{ return -1;	}
	//�õ��ڲ�����
	virtual VOID*			GetImpl(VOID)			{ return (VOID*)GetSkillImpl();	}
	//�õ�����
	virtual LPCTSTR			GetDesc(VOID);
	//�õ���ȴ״��ID
	virtual INT				GetCoolDownID(VOID);
	//�õ���������������
	//	����			- �ڼ�������
	virtual INT				GetPosIndex(VOID);
	//�Ƿ��ܹ��Զ���������
	virtual BOOL			AutoKeepOn(VOID);
	//�����
	virtual VOID			DoAction(VOID);
	//�Ƿ���Ч
	virtual BOOL			IsValidate(VOID) { return TRUE; }
	//�����ȴ�Ƿ����
	virtual BOOL			CoolDownIsOver(VOID);
	//�϶�����
	virtual VOID			NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName);

	// �õ����ܵȼ�
	virtual INT				GetSkillLevel();

	// �õ��Ƿ�ѧϰ�˼���
	virtual BOOL			GetIsLearnedSkill();

	// �õ����ܵȼ�
	//INT	GetSkillXinfaLevel();

public:
	CActionItem_Skill(INT nID);
	virtual ~CActionItem_Skill();

	//���ݼ��ܸ���
	VOID	Update_Skill(const SCLIENT_SKILL* pSkill);

protected:
	//�õ���������
	const SCLIENT_SKILL*	GetSkillImpl(VOID);

protected:
	//���ڼ��ܲ���ʱ����������
	INT		m_idSkillImpl;

};