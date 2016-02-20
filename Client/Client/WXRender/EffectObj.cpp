#include "StdAfx.h"
#include "EffectObj.h"
#include "RenderSystem.h"
#include "OgreLogmanager.h"
#include "TDDBC_Struct.h"
#include "TDDataBase.h"
#include "Global.h"
#include "TDException.h"
#include "TDKernel.h"
#include "../EffectSystem/FairyEffectManager.h"
#include "../EffectSystem/FairyEffect.h"
#include <OgreSceneNode.h>

CEffectObject::CEffectObject(VOID)
{
}

CEffectObject::~CEffectObject()
{
	std::list< std::pair< Fairy::Effect*, Ogre::SceneNode* > >::iterator it;
	for(it = m_listEffectImpl.begin(); it!=m_listEffectImpl.end(); it++)
	{
		if(it->second)
		{
			Fairy::EffectManager::getSingleton().removeEffect(it->first);
		}
	}

	m_listEffectImpl.clear();
}

HANDLE CEffectObject::AddEffect(INT idEffect, BOOL bLoop, LPCTSTR /*szLocatorName*/)
{
	//������Чʵ��
	Fairy::Effect* pEffectIml = Fairy::EffectManager::getSingleton().createEffect("g01_jingangquan_01");
	if(!pEffectIml) return NULL;

	//���������ڵ�
	Ogre::SceneNode* pSceneNode = pEffectIml->createSceneNode();

	//����
	m_listEffectImpl.push_back(std::make_pair(pEffectIml, pSceneNode));

	return pEffectIml;

#if 0
	//-------------------------------------------------------------
	//�õ����ݿ�ϵͳģ��
	static const tDataBaseSystem* s_pDataBase = NULL;
	if(!s_pDataBase) s_pDataBase = (tDataBaseSystem*)g_pKernel->GetNode("bin\\dbc");
	TDAssert(s_pDataBase);

	//-------------------------------------------------------------
	//��DBC_EFFECT_DEFINE���Ҿ�����Ч
	static const tDataBase* s_pDBCEffect = NULL;
	if(!s_pDBCEffect) s_pDBCEffect = s_pDataBase->GetDataBase(DBC_EFFECT_DEFINE);
	TDAssert(s_pDBCEffect);

	//���Ҿ�����Ч
	const _DBC_EFFECT_DEFINE* pEffect = (const _DBC_EFFECT_DEFINE*)s_pDBCEffect->Search_Index_EQU(idEffect);
	//û���ҵ�
	if(!pEffect) return NULL;
	
	//����Effect����
	Ogre::Effect* pEffectIml = Ogre::EffectManager::getSingleton().createEffect(pEffect->pEffectType, pEffect->pParam1);
	if(!pEffectIml) return NULL;

	//����
	m_listEffectImpl.push_back(pEffectIml);

	return (HANDLE)pEffectIml;
#endif
}

VOID CEffectObject::DelEffect(HANDLE hEffect)
{
	std::list< std::pair< Fairy::Effect*, Ogre::SceneNode* > >::iterator it;

	for(it=m_listEffectImpl.begin(); it!=m_listEffectImpl.end(); it++)
	{
		//��������ɾ��
		if(it->first == (Fairy::Effect*)hEffect)
		{
			Fairy::EffectManager::getSingleton().removeEffect(it->first);
			m_listEffectImpl.erase(it);
			return;
		}
	}
}

VOID CEffectObject::Destroy(VOID)
{
	CRenderSystem::GetMe()->Destroy_FairyObject(this);
	return;
	//֮���ܵ����κδ���
}

//����λ�� (��Ϸ��������)
VOID CEffectObject::SetPosition(const fVector3& vPos)
{
	//����ת��
	fVector3 fvGfx;
	CRenderSystem::GetMe()->Axis_Trans(CRenderSystem::AX_GAME, vPos, CRenderSystem::AX_GFX, fvGfx);

	std::list< std::pair< Fairy::Effect*, Ogre::SceneNode* > >::iterator it;
	for(it=m_listEffectImpl.begin(); it!=m_listEffectImpl.end(); it++)
	{
		it->second->setPosition(Ogre::Vector3(fvGfx.x, fvGfx.y, fvGfx.z));
	}

#if 0
	Ogre::Matrix4 mxPosition = Ogre::Matrix4::IDENTITY;
	mxPosition.makeTrans(fvGfx.x, fvGfx.y, fvGfx.z);

	Ogre::MatrixList vMatrix;
	vMatrix.push_back(mxPosition);

	std::list< Ogre::Effect* >::iterator it;
	for(it=m_listEffectImpl.begin(); it!=m_listEffectImpl.end(); it++)
	{
		(*it)->execute(0, vMatrix);
	}
#endif
}

