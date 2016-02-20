/****************************************\
*										*
*			  DummyObject				*
*										*
\****************************************/

#pragma once
#include "RFObject.h"

namespace WX
{
	class Effect;
	class EffectElement;
};

class CRFObj_Dummy : public CWXObject
{
public:
	//�õ�����
	virtual EntityType	GetType(VOID) const	 { return ETYPE_DUMMY; }
	//����Ⱦ�����ٽڵ�
	virtual VOID		Destroy(VOID);

	//����λ�� (��Ϸ��������)
	virtual VOID	SetPosition(const fVector3& vPos);
	//������ת�Ƕ�
	virtual VOID	SetOrientation(const fVector3& vRotate);
	//�����Ƿ�ɼ�
	virtual VOID	SetVisible(BOOL bVisible);
	//����һ����Ч
	virtual VOID	Attach_Effect(LPCTSTR szEffectName);
	virtual VOID	Detach_Effect(void);
	//����һ��Ͷ������
	virtual VOID	Attach_ProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT Ring_Range=100.0f, FLOAT fHeight=1.7f );

	virtual VOID	SetLeftWeaponEffect(LPCTSTR szEffectName, UINT color) {};
	virtual VOID	SetRightWeaponEffect(LPCTSTR szEffectName, UINT color) {};

	virtual VOID	SetRightWeaponMat(LPCTSTR szWeaponMat, LPCTSTR szMatName) {};
	virtual VOID	SetLeftWeaponMat(LPCTSTR szWeaponMat, LPCTSTR szMatName) {};
protected:
	//�����ڵ�
	Ogre::SceneNode*	m_pSceneNode;
	//����Ͷ����Ӱ
	std::vector< CWXObj_ProjTex* >	m_vProjTex;
	//��������Ч
	WX::Effect*						m_pEffect;

public:
	CRFObj_Dummy();
	virtual ~CRFObj_Dummy();
};