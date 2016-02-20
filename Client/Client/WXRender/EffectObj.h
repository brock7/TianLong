#pragma once
#include "TDGfxSystem.h"

namespace Fairy
{
	class Effect;
};
namespace Ogre
{
	class SceneNode;
};

//��Ч��Ⱦʵ��
class CEffectObject : public tEntityNode
{
public:
	//�õ�����
	virtual LPCTSTR GetType(VOID) const	 { return EOBJ_TYPE_EFFECT; }
	//����Ⱦ�����ٽڵ�
	virtual VOID	Destroy(VOID);

	//-------------------------------------------------------
	//����λ�� (��Ϸ��������)
	virtual VOID	SetPosition(const fVector3& vPos);
	//������ת�Ƕ�
	virtual VOID	SetOrientation(const fVector3& vRotate)	{	}
	//�����Ƿ�ɼ�
	virtual VOID	SetVisible(BOOL bVisible)				{	}
	//ȡ��"ͷ��״̬��"����Ļ�ϵ�λ��,�������FALSE����ʾ����Ļ֮��,����û�иõ�
	virtual BOOL	GetInfoBoardPos(iVector2& ivPos)		{ return FALSE; }
	//�����Ƿ��ܱ����ѡ��
	virtual VOID	SetRayQuery(BOOL bQuery){}
	virtual	BOOL	GetRayQuery(VOID) const{ return FALSE; }

	//-------------------------------------------------------
	//����һ����Ч
	/*
	|		����ǽ�ɫ�������ҽӵ�szLocatorNameָ���Ĺҽӵ���, 
	|	������ǽ�ɫ�������ֱ������һ����Ч, �������ɵ���Ч����
	|		
	|		idEffect		- ��ЧID(DBC_EFFECT_DEFINE)
	|		bLoop			- �Ƿ�ѭ������
	|		szLocatorName	- ����ǰ��ڽ�ɫ�ϵ���Ч��ָ���󶨵�
	|
	*/
	virtual HANDLE	AddEffect(INT idEffect, BOOL bLoop=TRUE, LPCTSTR szLocatorName = NULL);
	//ж����Ч
	virtual VOID	DelEffect(HANDLE hEffect);

public:
	CEffectObject();
	~CEffectObject();

private:
	//Effect Impl
	std::list< std::pair< Fairy::Effect*, Ogre::SceneNode* > >	m_listEffectImpl;
};