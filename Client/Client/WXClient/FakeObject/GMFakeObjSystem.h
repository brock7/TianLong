/****************************************\
*										*
* 	  ������ui����ʾ������������ӿ�	*
*										*
\****************************************/

#pragma once
#include "TDFakeObjSystem.h"

class CFakeObjSystem : public tFakeObjSystem
{
	//-----------------------------------------------------------
	//���ܽӿ�
	//-----------------------------------------------------------
protected:
	struct FakeObject
	{
		STRING			strName;		//���֣�UI�ؼ����ݸ����ֿ���
		INT				nObjectID;		//�߼�Obj����ID
		BOOL			bVisible;		//��ǰ�ö����Ƿ���Ҫ��ʾ
		STRING			strWindowName;	//��ʾ��ģ�͵Ĵ�����
		STRING			strCameraName;	//�ô��ڵ����������
	};

public:
	//����/������ui����ʾ��obj����
	virtual tObject*		NewFakeObject(LPCTSTR szClassName, LPCTSTR szObjName, LPCTSTR szCameraName);
	virtual VOID			DestroyObject(LPCTSTR szObjName);

	//UI��Ҫ��ʾĳ����
	virtual VOID			OnUIHook(LPCTSTR szWindowName, LPCTSTR szObjName);
	//UI������Ҫ��ʾĳ����
	virtual VOID			OnUIUnHook(LPCTSTR szWindowName);

	//UI��ʾ/����
	virtual	VOID			OnUIShown(LPCTSTR szWindowName, float fAspectRatio);
	virtual VOID			OnUIHiden(LPCTSTR szWindowName);

	//��תģ��
	virtual VOID			RotateObj_Begin(LPCTSTR szObjName, FLOAT fAngle);
	virtual VOID			RotateObj_End(LPCTSTR szObjName);
	virtual VOID			Obj_ChangeAction(LPCTSTR szObjName, INT SkillID,INT SkillLoop);

	//-----------------------------------------------------------
	//�ܹ����obj�������32��
	//-----------------------------------------------------------
protected:

	//FakeObj�������32��
	std::map< STRING, FakeObject > m_listFakeMap;
	
	//�Դ�����Ϊ����
	std::map< STRING, FakeObject* > m_listFakeUI;

	//������ת�е�ģ��
	FakeObject*						m_pRotateObj;
	FakeObject*						m_pActionObj;
	FLOAT							m_fRotateSpeed;

	//-----------------------------------------------------------
	//���ļ̳�
	//-----------------------------------------------------------
public:
	virtual VOID	Initial(VOID*);
	virtual VOID	Release(VOID);
	virtual VOID	Tick(VOID);

	
public:
	CFakeObjSystem();
	virtual ~CFakeObjSystem();

	static CFakeObjSystem*	GetMe(void) { return s_pMe; }

protected:
	static CFakeObjSystem* s_pMe;
protected:
	WX_DECLARE_DYNAMIC(CFakeObjSystem);
};