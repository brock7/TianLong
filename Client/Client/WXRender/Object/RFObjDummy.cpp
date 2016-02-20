#include "StdAfx.h"
#include "RFObjDummy.h"
#include "../RenderSystem.h"
#include "../Global.h"
#include "TDException.h"
#include "Core/WXSystem.h"
#include <OgreSceneManager.h>
#include "../WXObj_ProjTex.h"
#include "EffectSystem/WXEffectManager.h"
#include "EffectSystem/WXEffect.h"
#include "EffectSystem/WXEffectElement.h"
#include "EffectSystem/WXProjectorElement.h"


CRFObj_Dummy::CRFObj_Dummy()
{
	//�����հ׽ڵ㣬���Ը�����Ч����ProjTexture
	m_pSceneNode = 
		CRenderSystem::GetMe()->GetRenderSystem()->getBaseSceneNode()->createChildSceneNode();

	m_pEffect = NULL;
}

CRFObj_Dummy::~CRFObj_Dummy()
{
	if(m_pSceneNode)
	{
		if ( m_pEffect != NULL )
		{
			WX::EffectManager::getSingleton().removeEffect(m_pEffect);
			m_pEffect = NULL;
		}
		CRenderSystem::GetMe()->GetRenderSystem()->getSceneManager()->destroySceneNode(m_pSceneNode->getName());
	}
}

VOID CRFObj_Dummy::Destroy(VOID)
{
	//ɾ��ͶӰ
	for(INT i=0; i<(INT)m_vProjTex.size(); i++)
	{
		delete m_vProjTex[i]; 	m_vProjTex[i] = NULL;
	}
	m_vProjTex.clear();

	//ɾ������
	CWXObject::Destroy();
}

//����λ�� (��Ϸ��������)
VOID CRFObj_Dummy::SetPosition(const fVector3& vPos)
{
	if(!m_pSceneNode) return;

	//����ת��
	fVector3 fvGfx;
	CRenderSystem::GetMe()->Axis_Trans(CRenderSystem::AX_GAME, 
		vPos, CRenderSystem::AX_GFX, fvGfx);

	m_pSceneNode->setPosition(fvGfx.x, fvGfx.y, fvGfx.z);
}

//������ת
VOID CRFObj_Dummy::SetOrientation(const fVector3& vRotate)
{
	//Todo ...	
	if(!m_pSceneNode) return;

	m_pSceneNode->setOrientation( Ogre::Quaternion(Ogre::Radian(vRotate.y), Ogre::Vector3::UNIT_Y) );
}

//�����Ƿ�ɼ�
VOID CRFObj_Dummy::SetVisible(BOOL bVisible)
{
	TDThrow("Not support!");
}

//����Ͷ������
VOID CRFObj_Dummy::Attach_ProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT Ring_Range, FLOAT fHeight)
{
	//��λ���е�ProjTex
	CWXObj_ProjTex* pShadowProj = NULL;
	for(INT i=0; i<(INT)m_vProjTex.size(); i++)
	{
		if(m_vProjTex[i]->GetType() == type)
		{
			pShadowProj = m_vProjTex[i];
			break;
		}
	}
	
	if(Ring_Range != 100.0f && m_pEffect)
	{
		for (unsigned short i=0; i < m_pEffect->getNumElements(); i++)
		{
			WX::EffectElement *eff_element = m_pEffect->getElement(i);

			if (eff_element->getType() == "Projector")
			{
				static_cast<WX::ProjectorElement*>(eff_element)->setProjectionSize(Ring_Range);
			}
		}
	}
	
	//�߶�ת�� GAME->GFX
	//ȡ�÷���ϵ��
	fVector3 fvScale = CRenderSystem::GetMe()->GetSacle();
	fHeight = fHeight*fvScale.y;

	if(bShow)
	{
		//�Ѿ�����
		if(pShadowProj)
		{
			pShadowProj->Show(bShow);
			pShadowProj->SetAddHeight(fHeight);
			return;
		}

		//����
		pShadowProj = new CWXObj_ProjTex(type, m_pSceneNode, fHeight);

		m_vProjTex.push_back(pShadowProj);
	}
	else
	{
		//�Ѿ�����
		if(pShadowProj)
		{
			pShadowProj->Show(bShow);
			pShadowProj->SetAddHeight(fHeight);
			return;
		}
	}

}

VOID CRFObj_Dummy::Attach_Effect(LPCTSTR szEffectName)
{
	if ( m_pEffect != NULL )
	{
		assert( m_pEffect == NULL && "CRFObj_Dummy::Attach_Effect" );
		Detach_Effect();
	}

	//�����µ���Ч
	m_pEffect = WX::EffectManager::getSingleton().createEffect(szEffectName);
	if ( m_pEffect != NULL )
	{
		//�ҽӵ������ڵ�
		//m_pSceneNode->addChild(m_pEffect->createSceneNode());
		m_pEffect->createSceneNode(m_pSceneNode);
	}
}

VOID CRFObj_Dummy::Detach_Effect(void)
{
	if ( m_pSceneNode != NULL )
	{
		if ( m_pEffect != NULL )
		{
			WX::EffectManager::getSingleton().removeEffect(m_pEffect);
			m_pEffect = NULL;
		}
	}
}
