/****************************************\
*										*
* 			������Ӱ���ϵͳ			*
*										*
\****************************************/

#pragma once
#include "TDHelperSystem.h"
#include "..\Interface\GMinterface_Lua.h"

namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}

class CHelperSystem : public tHelperSystem, public LuaExport< CHelperSystem >
{
	//-----------------------------------------------------------
	//����ӿ�
	//-----------------------------------------------------------
public:
	// ��ת��ָ����������
	void	GotoAnchorHelp(const STRING& strAnchor);

	//-----------------------------------------------------------
	// ������Lua�ĺ���
	//-----------------------------------------------------------
public:
	INT Lua_GotoHelper(LuaPlus::LuaState* state);

protected:
	//������������
	void	CreateHelperProcess(const STRING& strAnchor);

	//-----------------------------------------------------------
	//���ļ̳�
	//-----------------------------------------------------------
public:
	virtual VOID	Initial(VOID*);
	virtual VOID	Release(VOID);
	virtual VOID	Tick(VOID);
	
protected:
	//��������������ܴ�����
	STRING	m_strCmdWnd;
	//��������
	HANDLE	m_hHelperProcess;

public:
	CHelperSystem();
	virtual ~CHelperSystem();

protected:
	WX_DECLARE_DYNAMIC(CHelperSystem);
};
