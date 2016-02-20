/********************************************************************
	��������:	2005��11��1��
	����ʱ��:	13:48
	�ļ�����:	ItemHelper.h
	�ļ�·��:	d:\Prj\Server\Server\Item\ItemHelper.h
	
	�ļ����ܣ� ��Ʒϵͳ������
	�޸ļ�¼��
	
*********************************************************************/



#ifndef _ITEM_HELPER_H_
#define _ITEM_HELPER_H_
#include "Type.h"


//���кŰ�����
struct TSerialHelper {

	UINT	m_SN;

	TSerialHelper(){m_SN = 0;};
	TSerialHelper(UINT uiSN){m_SN = uiSN;};
	TSerialHelper(CHAR* pStr)
	{
		__ENTER_FUNCTION

			Assert(pStr!=NULL);

			m_SN = atoi(pStr);

		__LEAVE_FUNCTION
	}

	BOOL		GetItemTypeStruct( _ITEM_TYPE& it )
	{
		it.m_Class		=	GetItemClass();
		it.m_Index		=	GetItemIndex();
		it.m_Quality	=	GetItemQual();
		it.m_Type		=	GetItemType();

		return TRUE;
	}

	_ITEM_TYPE	GetItemTypeStruct()
	{
		_ITEM_TYPE it;

		it.m_Class		=	GetItemClass();
		it.m_Index		=	GetItemIndex();
		it.m_Quality	=	GetItemQual();
		it.m_Type		=	GetItemType();

		return it;
	}
	ITEM_CLASS	GetItemClass()
	{
		return	(ITEM_CLASS)GetSerialClass(m_SN);
	}
	VOID		SetClass(ITEM_CLASS rh)
	{
		m_SN = rh*1000000+m_SN%1000000;
	}
	UINT		GetItemQual()
	{
		return	GetSerialQual(m_SN);
	}

	VOID		SetItemQual(UINT uiQual)
	{
		m_SN = m_SN + (uiQual-GetItemQual())*10000;
	}

	UINT		GetItemType()
	{
		return GetSerialType(m_SN);
	}

	VOID		SetItemType(UINT	uiType)
	{
		m_SN  = m_SN + (uiType-GetItemType())*100;
	}

	UINT		GetItemIndex()
	{
		return	GetSerialIndex(m_SN);
	}
	VOID		SetItemIndex(UINT	uiIndex)
	{
		m_SN	=	m_SN +(uiIndex-GetItemIndex());
	}

};

//��Ʒ�����������
class ItemRander
{
public:
	static	double		DoubleRand();
	static	INT			IntRand();
};

#endif