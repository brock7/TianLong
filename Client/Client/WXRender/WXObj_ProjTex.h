#pragma once

/*
|	������WXObj�ϵ�Ͷ������,����ѡ��,��Ӱ�ߵ�,��ӡ��
|	
*/
#include "..\WxRender\RenderSystem.h"

namespace Ogre
{
	class SceneNode;
}

namespace WX
{
    class Effect;
}

class CWXObject;
class CWXObj_ProjTex
{
public:
	tEntityNode::PROJTEX_TYPE	GetType(VOID) const { return m_theType; }
	VOID						Show(BOOL bShow);
	VOID						SetAddHeight(FLOAT fAddHeight = 1.7f);
	FLOAT						GetAddHeight(void) const { return m_fAddHeight; }

protected:
	tEntityNode::PROJTEX_TYPE	m_theType;

    WX::Effect*					mProjectorEffect;

	//���ǻ�������
	BOOL						m_bShow;
	//���Ӹ߶�
	FLOAT						m_fAddHeight;

public:
	CWXObj_ProjTex(tEntityNode::PROJTEX_TYPE type, Ogre::SceneNode* pParentNode, FLOAT fAddHeight=170.0f);
	virtual ~CWXObj_ProjTex();
};