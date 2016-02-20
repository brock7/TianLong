#include "StdAfx.h"
#include "FairyObj.h"
#include "RenderSystem.h"
#include "FairyObjectFactoryManager.h"
#include "OgreVector3.h"
#include "FairyActorObject.h"
#include "FairyActor.h"
#include "TDException.h"
#include "FairySystem.h"
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include "Global.h"
#include "TDDBC_Struct.h"
#include "TDDataBase.h"
#include "FairyObj_ProjTex.h"
#include "TDKernel.h"
#include "SkeletonMesh.h"
#include <OgreLogmanager.h>

//===========================================================
// FairyObject��ͨ������������		(FOBJ = Fairy Object Property Name)
#define		FOBJ_POSITION			_T("position")				//λ��
#define		FOBJ_ORIENTATION		_T("orientation")			//��ת
#define		FOBJ_SCALE				_T("scale")					//��������
#define		FOBJ_MESH_FILE			_T("mesh name")				//��̬���������ļ�
#define		FOBJ_ACTOR_FILE			_T("actor name")			//����ģ����Դ��


//===========================================================
// Actor Object��ͨ������������		(DOPN = Actor Object Property Name)
#define		AOBJ_CUR_ACTION			_T("CurrAction")			//��ǰ����
#define		AOBJ_CUR_ACTION_RATION	_T("ActionRatio")			//��ǰ�������ٶ�


CFairyObject::CFairyObject(LPCTSTR szType)
{
	Fairy::System* pFairySystem = CRenderSystem::GetMe()->GetFairySystem();
	//��Fairy�д���ʵ��
	m_pFairyObj = pFairySystem->getObjectFactoryManager()->createInstance(Fairy::String(szType));
	//��ogre�д�����Ⱦ����
	m_pFairyObj->createRenderInstance( pFairySystem );
	//��thisָ�뱣����FairyData��
	m_pFairyObj->setData((Fairy::ulong)this);	

	m_bRayQuery = TRUE;

	m_pUnitSelect = NULL;
	m_pUnitSelectNode = NULL;
	m_pShadowBlob = NULL;
	m_pShadowBlobNode = NULL;

	m_bVisible = TRUE;
}

CFairyObject::~CFairyObject(VOID)
{
//	SetSelectRing(FALSE);
//	SetShadowBlob(FALSE);
	for(INT i=0; i<(INT)m_vProjTex.size(); i++)
	{
		delete m_vProjTex[i];
		m_vProjTex[i] = NULL;
	}

	m_vProjTex.clear();

	m_pFairyObj.reset();
}

VOID CFairyObject::Destroy(VOID)
{
	CRenderSystem::GetMe()->Destroy_FairyObject(this);
	return;
	//֮���ܵ����κδ���
}

VOID CFairyObject::SetMeshFile(LPCTSTR szMeshFile)
{
	TDAssert(szMeshFile && m_pFairyObj);

	m_pFairyObj->setProperty(FOBJ_MESH_FILE, Fairy::String(szMeshFile));
}

VOID CFairyObject::SetActorFile(LPCTSTR szActorFile)
{
	TDAssert(szActorFile&& m_pFairyObj);

	m_pFairyObj->setProperty(FOBJ_ACTOR_FILE, Fairy::String(szActorFile));
#if 0
	//�õ����ж����б�
	{
		//ȷ��������ģ��
		if(m_pFairyObj->getType() != EOBJ_TYPE_ACTOR) return;

		//ȷ����Դ�Ѿ�Load
		Fairy::Actor* pActor = ((Fairy::ActorObject*)m_pFairyObj.get())->getActor();
		if(!pActor) return;

		Ogre::LogManager::getSingleton().logMessage("Load ActorFile " + Ogre::String(szActorFile));

		std::vector< STRING > vAllAnim = pActor->getImpl()->GetObjAllAnimName();
		for(int i=0; i<(int)vAllAnim.size(); i++)
		{
			Ogre::LogManager::getSingleton().logMessage(Ogre::String(vAllAnim[i].c_str()));
		}
	}
#endif
}

VOID CFairyObject::SetPosition(const fVector3& vPos)
{
	TDAssert(m_pFairyObj);

	//����ת��
	fVector3 fvGfxPos;
	CRenderSystem::GetMe()->Axis_Trans(CRenderSystem::AX_GAME, vPos, CRenderSystem::AX_GFX, fvGfxPos);

	m_pFairyObj->setProperty(FOBJ_POSITION, Ogre::Vector3(fvGfxPos.x, fvGfxPos.y, fvGfxPos.z));
}

VOID CFairyObject::SetOrientation(const fVector3& vRotate)
{
	Ogre::Quaternion qu(Ogre::Radian(vRotate.y), Ogre::Vector3::UNIT_Y);
	m_pFairyObj->setProperty(FOBJ_ORIENTATION, qu);
}

VOID CFairyObject::SetVisible(BOOL bVisible)
{
	if(m_bVisible == bVisible) return;

	//ȷ��������ģ��
	if(m_pFairyObj->getType() != EOBJ_TYPE_ACTOR) return;
	
	//ȷ����Դ�Ѿ�Load
	Fairy::Actor* pActor = ((Fairy::ActorObject*)m_pFairyObj.get())->getActor();
	if(!pActor) return;

	Ogre::SceneNode* pParentNode = ((Fairy::ActorObject*)m_pFairyObj.get())->getSceneNode();
	Ogre::SceneNode* pActorNode = (((Fairy::ActorObject*)m_pFairyObj.get())->getActor())->getSceneNode();

	if(bVisible)
	{
		pParentNode->addChild(pActorNode);
	}
	else
	{
		pParentNode->removeChild(pActorNode);
	}

	m_bVisible = bVisible;
}

VOID CFairyObject::SetActorObjectProperty(LPCTSTR szName, LPCTSTR szValue)
{
	TDAssert(m_pFairyObj);

	//ȷ��������ģ��
	if(m_pFairyObj->getType() != EOBJ_TYPE_ACTOR) return;

	//ȷ����Դ�Ѿ�Load
	Fairy::Actor* pActor = ((Fairy::ActorObject*)m_pFairyObj.get())->getActor();
	if(!pActor) return;

//	AxTrace(-1, 0, "<DObject|Set %s to %s>", szName, szValue);

	try
	{
		std::vector< std::string > vName, vValue;
		vName.push_back(szName);
		vValue.push_back(szValue);

		pActor->getImpl()->ChangeAttributes(vName, vValue);
	}
	catch(...)
	{
	}
}

VOID CFairyObject::GetActorObjectProperty(LPCTSTR szName, STRING& szValue)
{
	TDAssert(m_pFairyObj);

	//ȷ��������ģ��
	if(m_pFairyObj->getType() != EOBJ_TYPE_ACTOR) return;

	//ȷ����Դ�Ѿ�Load
	Fairy::Actor* pActor = ((Fairy::ActorObject*)m_pFairyObj.get())->getActor();
	if(!pActor) return;

	DWORD dwHits;
	pActor->getImpl()->GetObjAttribute(DString(szName), szValue, dwHits);
}

VOID CFairyObject::GetActorLocator(LPCTSTR szName, fVector3& fvPosition)
{
	TDAssert(m_pFairyObj);

	//ȷ��������ģ��
	if(m_pFairyObj->getType() != EOBJ_TYPE_ACTOR) return;

	//ȷ����Դ�Ѿ�Load
	Fairy::Actor* pActor = ((Fairy::ActorObject*)m_pFairyObj.get())->getActor();
	if(!pActor) return;

	DObject_NT* pDObj = ((Fairy::ActorObject*)m_pFairyObj.get())->getActor()->getImpl();

	Ogre::Matrix4 mx;
	pActor->getImpl()->GetLocatorWorldTransform(DString(szName), mx);

	fVector3 fvGfx(mx[0][3], mx[1][3], mx[2][3]);

	CRenderSystem::GetMe()->Axis_Trans(CRenderSystem::AX_GFX, fvGfx, CRenderSystem::AX_GAME, fvPosition);
}

VOID CFairyObject::SetAnimationEndEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam)
{
	TDAssert(m_pFairyObj);

	//ȷ��������ģ��
	if(m_pFairyObj->getType() != EOBJ_TYPE_ACTOR) return;

	//ȷ����Դ�Ѿ�Load
	Fairy::Actor* pActor = ((Fairy::ActorObject*)m_pFairyObj.get())->getActor();
	if(!pActor) return;
	
	pActor->getImpl()->SetAnimationEndEventListener((_OnAnimationEnd)pFunc, dwParam);
}

VOID CFairyObject::SetProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT fHeight)
{
	//��λ���е�ProjTex
	CFairyObj_ProjTex* pShadowProj = NULL;
	for(INT i=0; i<(INT)m_vProjTex.size(); i++)
	{
		if(m_vProjTex[i]->GetType() == type)
		{
			pShadowProj = m_vProjTex[i];
			break;
		}
	}

	//�߶�ת�� GAME->GFX
	//ȡ�÷���ϵ��
	fVector3& fvScale = CRenderSystem::GetMe()->GetSacle();
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
		pShadowProj = new CFairyObj_ProjTex(type, ((Fairy::PlacementObject*)GetFairyObj().get())->getSceneNode(), fHeight);

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

BOOL CFairyObject::GetInfoBoardPos(iVector2& ivPos)
{
	const STRING s_strInfoBoardLocator = "ͷ��״̬��";

	//ȷ��������ģ��
	if(m_pFairyObj->getType() != EOBJ_TYPE_ACTOR) return FALSE;

	//ȷ����Դ�Ѿ�Load
	Fairy::Actor* pActor = ((Fairy::ActorObject*)m_pFairyObj.get())->getActor();
	if(!pActor) return FALSE;

	DObject_NT* pDObj = ((Fairy::ActorObject*)m_pFairyObj.get())->getActor()->getImpl();

	//ȡ����ά����
	Ogre::Matrix4 mx;
	BOOL bFind = pActor->getImpl()->GetLocatorWorldTransform(s_strInfoBoardLocator, mx);
	if(!bFind) return FALSE;

	//ת��Ϊ��Ļ����
	fVector3 fvGfx(mx[0][3], mx[1][3], mx[2][3]);
	fVector3 fvScreen;
	BOOL bVisible = CRenderSystem::GetMe()->Axis_Trans(CRenderSystem::AX_GFX, fvGfx, CRenderSystem::AX_SCREEN, fvScreen);
	if(!bVisible) return FALSE;


	ivPos.x = (INT)fvScreen.x;
	ivPos.y = (INT)fvScreen.y;

	return TRUE;
}

VOID CFairyObject::Skill_Enter(BOOL bAnim, LPCTSTR szSkillName, BOOL bLoop)
{
	//ȷ��������ģ��
	if(m_pFairyObj->getType() != EOBJ_TYPE_ACTOR) return;
	//ȷ����Դ�Ѿ�Load
	Fairy::Actor* pActor = ((Fairy::ActorObject*)m_pFairyObj.get())->getActor();
	if(!pActor) return;
	DObject_NT* pDObj = pActor->getImpl();

	pDObj->CreateSkill(szSkillName, bLoop==TRUE, bAnim==TRUE);
}

//����ȱʡ����
VOID CFairyObject::Skill_SetDefaultAnim(LPCTSTR szAnimName)
{
	//ȷ��������ģ��
	if(m_pFairyObj->getType() != EOBJ_TYPE_ACTOR) return;
	//ȷ����Դ�Ѿ�Load
	Fairy::Actor* pActor = ((Fairy::ActorObject*)m_pFairyObj.get())->getActor();
	if(!pActor) return;
	DObject_NT* pDObj = pActor->getImpl();

	pDObj->SetDefaultAnimation(szAnimName);
}

