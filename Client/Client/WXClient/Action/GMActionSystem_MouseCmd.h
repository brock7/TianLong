/****************************************\
*										*
* 		  ����������-���ָ��			*
*										*
*	���ڴ������������������Ϊ���ѵ�	*
*			�������״̬				*
*										*
\****************************************/

#pragma once
#include "GMActionSystem.h"

//-------------------------------------
//����
class CActionItem_MouseCmd_Repair : public CActionItem
{
public:
	//�õ���������
	virtual ACTION_OPTYPE	GetType(VOID) const	{ return AOT_MOUSECMD_REPARE; }
	//�õ��ڲ�����(DONT USE THIS DATA!)
	virtual VOID*			GetImpl(VOID)		{ return (VOID*)(DWORD_PTR)AOT_MOUSECMD_REPARE;}

protected:
	static CActionItem_MouseCmd_Repair*	s_pMe;

public:
	CActionItem_MouseCmd_Repair(INT id);
	virtual ~CActionItem_MouseCmd_Repair();

	static CActionItem_MouseCmd_Repair*	GetMe(VOID) { return s_pMe; }
};

//-------------------------------------
//����
class CActionItem_MouseCmd_Identify : public CActionItem
{
public:
	//�õ���������
	virtual ACTION_OPTYPE	GetType(VOID) const	{ return AOT_MOUSECMD_IDENTIFY; }
	//�õ��ڲ�����(DONT USE THIS DATA!)
	virtual VOID*			GetImpl(VOID)		{ return (VOID*)(DWORD_PTR)AOT_MOUSECMD_IDENTIFY;}
	//����Default״̬(ֻ��ͨ��ActionSystem����)
	virtual VOID			SetDefaultState(BOOL bDefault);

protected:
	static CActionItem_MouseCmd_Identify*	s_pMe;

public:
	CActionItem_MouseCmd_Identify(INT id);
	virtual ~CActionItem_MouseCmd_Identify();

	static CActionItem_MouseCmd_Identify*	GetMe(VOID) { return s_pMe; }
};