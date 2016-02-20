/********************************************************************
	��������:	2005��11��1��
	����ʱ��:	9:22
	�ļ�����:	ItemBoxPool.h
	�ļ�·��:	d:\Prj\Server\Server\Item\ItemBoxPool.h
	
	�ļ����ܣ� ?
	�޸ļ�¼��
	
*********************************************************************/



#ifndef		_ITEMBOX_POOL_H_
#define		_ITEMBOX_POOL_H_
#include	"Type.h"
#include	"ItemTypes.h"
#define		ITEM_POOL_INVAILD	-1				//��ЧObj_ItemBox���


class Obj_ItemBox;



 /*
  *		Obj_ItemBox	����
  */
class ItemBoxPool
{
	//Obj_ItemBox����
	Obj_ItemBox*		m_pItemBox;
	//����Itembox��Id
	INT*			m_pFreeItemID;
	//ʹ�õ�Obj_ItemBox��Id����
	INT*			m_pUseItemID;
	//����λ�ñ��
	INT				m_nFreePosition;
	//������
	MyLock			m_Lock;

	UINT			m_ItemBoxMaxCount;

public:
	ItemBoxPool();
	~ItemBoxPool();
	/*
	 *	���ItemBoxPool ��ʼ��,��Server->Init��ִ��
	 */
	BOOL			Init(UINT ItemBoxMaxCount = MAX_ITEM_BOX_POOL);
	
	/*
	 *	����һ���µ�Obj_ItemBox
	 *
	 *	
	 */
	
	Obj_ItemBox*		NewItemBox();
	
	/*	ɾ��һ��Itembox
	 *	
	 *	
	 *
	 */
	//����λ��ɾ��
	VOID			DelItemBox(UINT iItemPos);
	//����ָ��ɾ��
	VOID			DeleteObj(Obj_ItemBox*	pItem);

	UINT			GetItemBoxMaxCount() 
	{
		return m_ItemBoxMaxCount;
	};
};

extern ItemBoxPool* g_pItemBoxPool;


#endif
