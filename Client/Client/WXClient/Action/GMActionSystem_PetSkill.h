/****************************************\
*										*
* 			  ����������-���＼��		*
*										*
\****************************************/

#pragma once
#include "GMActionSystem.h"

class CActionItem_PetSkill : public CActionItem
{
public:
	//�õ���������
	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_PET_SKILL;	}
	//�����ַ���
	virtual	LPCTSTR			GetType_String(VOID)	{ return NAMETYPE_PETSKILL;	}
	//�õ�����ID
	virtual INT				GetDefineID(VOID);
	//�õ�����
	virtual INT				GetNum(VOID)			{ return m_nPetNum;	}
	//�õ��ڲ�����
	virtual VOID*			GetImpl(VOID)			{ return (VOID*)GetPetSkillImpl();	}
	//�õ�����
	virtual LPCTSTR			GetDesc(VOID);
	//�õ���ȴ״��ID
	virtual INT				GetCoolDownID(VOID);
	//�õ���������������
	//	����			- �ڼ�������
	virtual INT				GetPosIndex(VOID);
	//�Ƿ��ܹ��Զ���������
	virtual BOOL			AutoKeepOn(VOID) { return FALSE; }
	//�����
	virtual VOID			DoAction(VOID);
	//�Ƿ���Ч
	virtual BOOL			IsValidate(VOID) { return TRUE; }
	//�����ȴ�Ƿ����
	virtual BOOL			CoolDownIsOver(VOID);
	//�϶�����
	virtual VOID			NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName);

	virtual VOID			UpdateCoolDown(VOID);

public:
	CActionItem_PetSkill(INT nID);
	virtual ~CActionItem_PetSkill();

	//���ݼ��ܸ���
	VOID	Update_PetSkill(const PET_SKILL* pPetSkill);

protected:
	//�õ���������
	PET_SKILL*	GetPetSkillImpl(VOID);
	INT			GetMyPetLevel(VOID);

protected:
	//���ڼ��ܲ���ʱ����������
	INT		m_idPetSkillImpl;
	INT		m_nPetNum;

};