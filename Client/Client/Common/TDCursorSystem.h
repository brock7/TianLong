#pragma once
#include "TDNode.h"

enum ENUM_CURSOR_TYPE
{
	CURSOR_WINBASE = 0,

	CURSOR_NORMAL,			//��ͨ
	CURSOR_ATTACK,			//����
	CURSOR_AUTORUN,			//�Զ�������
	CURSOR_PICKUP,			//ʰȡ��Ʒ
	CURSOR_UNREACHABLE,		//�޷����������
	CURSOR_MINE,			//�ɿ�
	CURSOR_HERBS,			//��ҩ
	CURSOR_FISH,			//����
	CURSOR_SPEAK,			//̸��
	CURSOR_INTERACT,		//����
	CURSOR_REPAIR,			//����
	CURSOR_HOVER,			//��꼤��(�ҽ���Ʒ...)
	CURSOR_IDENTIFY,		//������
	CURSOR_ADDFRIEND,		//��Ӻ���
	CURSOR_EXCHANGE,		//��Ӻ���
	CURSOR_CATCH_PET,		//׽��


	CURSOR_NUMBER,
	
};

class tCursorSystem : public tNode
{
public:
	//���������
	virtual VOID		SetCursor(ENUM_CURSOR_TYPE nType)	= 0;
	//�õ���ǰ���
	virtual HCURSOR		GetCursor(VOID)						= 0;

	//����UI����ģʽ
	virtual VOID		EnterUICursor(HCURSOR hCursor)		= 0;
	virtual VOID		LeaveUICursor(VOID)					= 0;

	//��ӦWM_SETCURSOR
	virtual VOID		OnSetCursor(VOID)					= 0;

	//��ʾ/�������
	virtual VOID		ShowCursor(BOOL bShow)				= 0;

protected:
	WX_DECLARE_DYNAMIC( tCursorSystem );
};

