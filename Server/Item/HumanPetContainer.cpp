#include "stdafx.h"
#include "HumanPetContainer.h"
#include "HumanDB.h"

BOOL HumanPetContainer::Init(const _ITEMCONTAINER_INIT* pInit )
{
	__ENTER_FUNCTION

	if(!ItemContainer::Init(pInit))
		return FALSE;

	const _HUMAN_PETCONTAINER_INIT*	pHICInit = 
		static_cast<const _HUMAN_PETCONTAINER_INIT*>(pInit);

	if(pHICInit==NULL)
		return FALSE;

	Assert(pHICInit->m_pHumanDB);

	m_ICTType	=	pHICInit->m_ICTType;
	m_pHumanDB	=	pHICInit->m_pHumanDB;
	
	return TRUE;
	
	__LEAVE_FUNCTION

	return	FALSE;
}

BOOL	HumanPetContainer::SetItem(const INT nIndex, const Item* pItem )
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);
	BOOL	bRet = ItemContainer::SetItem(nIndex,pItem);
	
	if(bRet)
	{
		SetDBDirty();
	}
	return	bRet;

	__LEAVE_FUNCTION
	
	return  FALSE;
}

//ɾ����Ʒ
BOOL		HumanPetContainer::EraseItem(UINT uIndex)
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL	bRet	=	ItemContainer::EraseItem(uIndex);
	
	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;
	
	__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL		HumanPetContainer::SetItemValue(INT nIndex,const _PET_DB_LOAD* pPet)
{
	__ENTER_FUNCTION
	
	Assert(m_pHumanDB);
	Assert(pPet);
	
	BOOL bRet	=	ItemContainer::SetItemValue(nIndex,pPet);
	
	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	HumanPetContainer::SetPetGUID(INT nIndex,PET_GUID_t GUID)				//���ó���GUID
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);
	
	BOOL bRet	=	ItemContainer::SetPetGUID(nIndex,GUID);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	HumanPetContainer::SetSpouseGUID(INT nIndex,PET_GUID_t GUID)
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);
	
	BOOL bRet	=	ItemContainer::SetSpouseGUID(nIndex,GUID);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL	HumanPetContainer::SetDataID(INT nIndex,INT ID)							//���ó���ģ��		
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetDataID(nIndex,ID);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL 	HumanPetContainer::SetName(INT nIndex, const CHAR* pName)						//��������
{

	__ENTER_FUNCTION
	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetName(nIndex,pName);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}
BOOL   	HumanPetContainer::SetNick(INT nIndex, const CHAR* pNick)						//�����ǳ�
{

	__ENTER_FUNCTION
	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetNick(nIndex,pNick);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}
BOOL  	HumanPetContainer::SetLevel(INT nIndex,INT level)							//���õȼ�
{

	__ENTER_FUNCTION
	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetLevel(nIndex,level);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	HumanPetContainer::SetTakeLevel(INT nIndex,INT takeLevel)					//����Я���ȼ�
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetTakeLevel(nIndex,takeLevel);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}
BOOL  	HumanPetContainer::SetAttackType(INT nIndex,INT attackType)				//���ý������ͣ���/����
{
	__ENTER_FUNCTION

		Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetAttackType(nIndex,attackType);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}
BOOL  	HumanPetContainer::SetAIType(INT nIndex,INT AIType)						//����AI����
{
	__ENTER_FUNCTION

		Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetAIType(nIndex,AIType);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}
BOOL  	HumanPetContainer::SetCampData(INT nIndex,const _CAMP_DATA* pCamp)		//������Ӫ
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetCampData(nIndex,pCamp);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}
BOOL  	HumanPetContainer::SetHP(INT nIndex,INT hp)								//��������ֵ
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetHP(nIndex,hp);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	HumanPetContainer::SetLife(INT nIndex,INT Life)							//���õ�ǰ����
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetLife(nIndex,Life);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	HumanPetContainer::SetPetType(INT nIndex,BYTE PetType)				//���������죬Ұ��
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetPetType(nIndex,PetType);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	HumanPetContainer::SetGeneration(INT nIndex,BYTE Gen)						//������
{	
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetGeneration(nIndex,Gen);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;

}
BOOL  	HumanPetContainer::SetHappiness(INT nIndex,BYTE byHappiness)						//���ֶ�		
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetHappiness(nIndex,byHappiness);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	HumanPetContainer::SetStrPer(INT nIndex,INT strper)						//��������
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetStrPer(nIndex,strper);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	HumanPetContainer::SetConPer(INT nIndex,INT conper)						//��������	
{	
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetConPer(nIndex,conper);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}	
BOOL   	HumanPetContainer::SetDexPer(INT nIndex,INT dexper)						//������
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetDexPer(nIndex,dexper);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	HumanPetContainer::SetSprPer(INT nIndex,INT sprper)						//��������
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetSprPer(nIndex,sprper);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL   	HumanPetContainer::SetIntPer(INT nIndex,INT intper)						//��������
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetIntPer(nIndex,intper);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL	HumanPetContainer::SetGenGu(INT nIndex,INT gengu)							//����
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetGenGu(nIndex,gengu);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	HumanPetContainer::SetGrowRate(INT nIndex,FLOAT rate)						//�ɳ���
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetGrowRate(nIndex,rate);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}
BOOL  	HumanPetContainer::SetRemainPoint(INT nIndex,INT rPoint)					//һ������ʣ�����
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetRemainPoint(nIndex,rPoint);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	HumanPetContainer::SetExp(INT nIndex,INT exp)							//����ֵ
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetExp(nIndex,exp);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	HumanPetContainer::SetLvl1Attr(INT nIndex,CHAR_ATTR_LEVEL1 type,INT value)//����һ��ս�����ԣ����������ܺ�װ�����ӵĲ��֣�
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetLvl1Attr(nIndex,type,value);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	HumanPetContainer::SetSkill(INT nIndex,UINT SkillIndex,_PET_SKILL skill)	//���＼��
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetSkill(nIndex,SkillIndex,skill);

	if(bRet)
	{
		SetDBDirty();
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	HumanPetContainer::SetDBDirty()
{
	Assert(m_pHumanDB);
	switch(m_ICTType)
	{
	case ICT_HUMAN_PET_CONTAINER:
		{
			m_pHumanDB->SetDBEnumAttrDirty((CHAR_ATTR_DB)(CATTR_DB_PET)); 
		}
		break;

	default:
		{
			AssertEx(FALSE,"��Ч��������");
			return FALSE;
		}
	}
	return TRUE;
}


