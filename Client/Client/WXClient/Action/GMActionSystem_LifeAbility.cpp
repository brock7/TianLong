#include "StdAfx.h"
#include "GMActionSystem_LifeAbility.h"
#include "..\Object\ObjectManager.h"
#include "..\Object\Character\Obj_PlayerMySelf.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "..\Event\GMEventSystem.h"
#include "TDException.h"
#include "..\Procedure\GameProcedure.h"

CActionItem_LifeAbility::CActionItem_LifeAbility(INT nID)
	:CActionItem(nID)
{
}

CActionItem_LifeAbility::~CActionItem_LifeAbility()
{
}

VOID CActionItem_LifeAbility::Update_LifeAbility(const SCLIENT_LIFEABILITY* pAbility)
{
	TDAssert(pAbility);

	//����
	m_idLifeAbilityImpl = pAbility->m_pDefine->nID;
	//����
	m_strName = pAbility->m_pDefine->szName;
	//ͼ��
	m_strIconName = pAbility->m_pDefine->szIconName;

	//֪ͨUI
	UpdateToRefrence();
}

const SCLIENT_LIFEABILITY* CActionItem_LifeAbility::GetLifeAbilityImpl(VOID)
{
	return CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(m_idLifeAbilityImpl);
}

INT	CActionItem_LifeAbility::GetDefineID(VOID)
{
	const SCLIENT_LIFEABILITY* pAbility = GetLifeAbilityImpl();
	if(!pAbility) return -1;

	return pAbility->m_pDefine->nID;
}

LPCTSTR CActionItem_LifeAbility::GetDesc(VOID)
{
	return GetName();
}

INT CActionItem_LifeAbility::GetPosIndex(VOID)
{
	const SCLIENT_LIFEABILITY* pLifeAbility = GetLifeAbilityImpl();
	if(!pLifeAbility) return -1;

	return pLifeAbility->m_nPosIndex;
}

VOID CActionItem_LifeAbility::DoAction(VOID)
{
	const SCLIENT_LIFEABILITY* pAbility = (const SCLIENT_LIFEABILITY*)GetLifeAbilityImpl();
	if(!pAbility) return;

	//������ʱ���������ţ���ô��Ҫǿ�йرգ�����ͬʱ��������������
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Exchange"))
	{
		CEventSystem::GetMe()->PushEvent(GE_CANCEL_EXCHANGE);
	}

	CEventSystem::GetMe()->PushEvent(GE_OPEN_COMPOSE_ITEM, pAbility->m_pDefine->nID);
/*
	switch(pAbility->m_pDefine->nID)
	{
	case 1:		//���ģ��
	case 2:		//��ҩģ��
	case 4:		//����ģ��
//	case 3:		//�ӹ�ģ��
	case 5:		//����ģ��
	case 6:		//��������ģ��
	case 7:		//����ѧ
		CEventSystem::GetMe()->PushEvent(GE_OPEN_COMPOSE_ITEM, pAbility->m_pDefine->nID);
		break;
//	case 3:		//�ӹ�ģ��
//		CEventSystem::GetMe()->PushEvent(GE_OPEN_COMPOSE_GEM,1);
		break;
	}
*/
}

VOID CActionItem_LifeAbility::NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName)
{

}


