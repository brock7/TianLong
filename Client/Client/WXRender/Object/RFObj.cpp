#include "StdAfx.h"
#include "RFObj.h"
#include "TDException.h"
#include "../RenderSystem.h"
#include "Core/WXSystem.h"
#include "Core/WXObjectFactoryManager.h"
#include <OgreVector3.h>

#define		FOBJ_POSITION			_T("position")				//λ��
#define		FOBJ_ORIENTATION		_T("orientation")			//��ת

CWXObj_Object::CWXObj_Object(LPCTSTR szType)
{
	WX::System* pRenderSystem = CRenderSystem::GetMe()->GetRenderSystem();
	//���д���ʵ��
	m_pWXObject = pRenderSystem->getObjectFactoryManager()->createInstance(WX::String(szType));
	//��ogre�д�����Ⱦ����
	m_pWXObject->createRenderInstance( pRenderSystem );
	//��thisָ�뱣����WXData��
	m_pWXObject->setData((WX::ulong)this);	
	//�ɱ����ѡ��
	m_bRayQuery = TRUE;
}

CWXObj_Object::~CWXObj_Object()
{
}

VOID CWXObj_Object::Destroy()
{
	//����WXObj
	m_pWXObject.reset();

	CWXObject::Destroy();
}

VOID CWXObj_Object::SetPosition(const fVector3& vPos)
{
	TDAssert(m_pWXObject);

	//����ת��
	fVector3 fvGfxPos;
	CRenderSystem::GetMe()->Axis_Trans(CRenderSystem::AX_GAME, vPos, CRenderSystem::AX_GFX, fvGfxPos);

	m_pWXObject->setProperty(FOBJ_POSITION, Ogre::Vector3(fvGfxPos.x, fvGfxPos.y, fvGfxPos.z));
}

VOID CWXObj_Object::SetOrientation(const fVector3& vRotate)
{
	TDAssert(m_pWXObject);

	Ogre::Quaternion qu(Ogre::Radian(vRotate.y), Ogre::Vector3::UNIT_Y);
	m_pWXObject->setProperty(FOBJ_ORIENTATION, qu);
}

VOID CWXObj_Object::SetWXObjectProperty(const char* szPropertyName, const char* szPropertyValue)
{
	TDAssert(m_pWXObject);

	m_pWXObject->setPropertyAsString(szPropertyName, szPropertyValue);
}
