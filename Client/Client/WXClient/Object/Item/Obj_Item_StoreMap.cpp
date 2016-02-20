#include "StdAfx.h"
#include "Obj_Item_StoreMap.h"
#include "TDException.h"

CObject_Item_StoreMap::CObject_Item_StoreMap(INT id)
		: CObject_Item(id)
{
	m_theBaseDef = NULL;
}

CObject_Item_StoreMap::~CObject_Item_StoreMap()
{
}

VOID CObject_Item_StoreMap::AsStoreMap(const _DBC_ITEM_STOREMAP* pStoreMapDefine)
{
	TDAssert(pStoreMapDefine);

	m_theBaseDef = pStoreMapDefine;
	m_nParticularID = ( ( ( ( ( m_theBaseDef->nClass * 100 ) + m_theBaseDef->nQuality ) * 100 ) + m_theBaseDef->nType ) * 1000 ) + m_theBaseDef->nIndex;

}

LPCTSTR  CObject_Item_StoreMap::GetIconName(VOID) const
{
	return m_theBaseDef->szIcon;
}

//������ϸ����
VOID CObject_Item_StoreMap::SetExtraInfo(const _ITEM * pItemInfo)
{
	TDAssert(pItemInfo);

	//���û��ຯ��������_ITEM�ṹ
	CObject_Item::SetExtraInfo(pItemInfo);

	const STORE_MAP_INFO& infoStoreMap = pItemInfo->m_StoreMap;

	SetNumber(pItemInfo->GetItemCount());
	SetManufacturer(pItemInfo);
}

LPCTSTR  CObject_Item_StoreMap::GetExtraDesc(VOID)
{
	//_snprintf(szTemp, 1024, 
	//	"ID=%d\n"
	//	"����: %s(%s)\n"
	//	"����ȼ�: %d\n"
	//	"����: %d\n"
	//	"-----------\n"
	//	"%s\n",
	//	m_ID.m_idOrg.m_uSerial,
	//	GetName(),	GetDesc(),
	//	m_theBaseDef->nLevelRequire,
	//	GetNumber(),
	//	m_theBaseDef->szDesc);//

	m_strExtraDesc = m_theBaseDef->szDesc;
	return m_strExtraDesc.c_str();
}

VOID  CObject_Item_StoreMap::Clone(const CObject_Item * pItemSource)
{
	SetNumber(((CObject_Item_StoreMap*)pItemSource)->GetNumber());
	CObject_Item::Clone(pItemSource);
}

// 2006-3-30;
// �õ�quality��Ϣ
INT	CObject_Item_StoreMap::GetItemTableQuality(VOID)
{
	return m_theBaseDef->nQuality;
}

// 2006-3-30;
// �õ�type��Ϣ
INT	CObject_Item_StoreMap::GetItemTableType(VOID)
{
	return m_theBaseDef->nType;
}