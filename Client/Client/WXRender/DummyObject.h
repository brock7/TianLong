#pragma once
#include "TDGfxSystem.h"

namespace Ogre
{
	class SceneNode;
}
class CFairyObj_ProjTex;

class CDummyObject : public tEntityNode
{
public:
	//�õ�����
	virtual LPCTSTR GetType(VOID) const	 { return EOBJ_TYPE_DUMMY; }
	//����Ⱦ�����ٽڵ�
	virtual VOID	Destroy(VOID);

	//-------------------------------------------------------
	//����λ�� (��Ϸ��������)
	virtual VOID	SetPosition(const fVector3& vPos);
	//������ת�Ƕ�
	virtual VOID	SetOrientation(const fVector3& vRotate);
	//�����Ƿ�ɼ�
	virtual VOID	SetVisible(BOOL bVisible);
	//�޷���ѡ��
	virtual	BOOL	GetRayQuery(VOID) const{ return FALSE; }

	//����һ����Ч
	virtual VOID	Attach_Effect(LPCTSTR szEffectName);
	//����һ��Ͷ������
	virtual VOID	Attach_ProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT fHeight=1.7f);

protected:
	Ogre::SceneNode*	m_pSceneNode;

	//����Ͷ����Ӱ
	std::vector< CFairyObj_ProjTex* >	m_vProjTex;

public:
	CDummyObject();
	virtual ~CDummyObject();
};