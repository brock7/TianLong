/********************************************************************
	��������:	2005��11��2��
	����ʱ��:	16:54
	�ļ�����:	ItemRuler.h
	�ļ�·��:	d:\Prj\Server\Server\Item\ItemRuler.h
	
	�ļ����ܣ� ��Ʒϵͳ�Ĺ�����,���紴������\�������\ʹ�ù����\ʰȡ����
	�޸ļ�¼��
	
*********************************************************************/

#ifndef _ITEM_RULER_H_
#define _ITEM_RULER_H_


#include "ItemTable.h"
#include "Obj_ItemBox.h"
#include "ItemHelper.h"


class ItemBoxRuler
{
public:

	//ItemBox��������

	VOID		CreateItemFromMonsterDrop(INT iMonsterValue,
										 MONSTER_DROPBOXS& mDrop,
										 INT iPlayerLvl,
										 INT iMonsterLvl,
										 FLOAT fControlValue,
										 ItemBoxContaner&	OutBox,
										 FLOAT fWallow);





};




class ItemCreateRuler
{

public:

	static	BOOL		CreateEquipAttrib(ITEM_LOG_PARAM*	pLogParam,_ITEM_TYPE itemType,INT iPlayerLvl,_ITEM& it,INT& quality);
	static	BOOL		CreateGreenEquipAttrib(_ITEM_TYPE itemType,INT iPlayerLvl,_ITEM& it);
	static	BOOL		CreateCommonEquipAttrib(_ITEM_TYPE itemType,INT iPlayerLvl,_ITEM& it);
	static	BOOL		CreateBlueEquipAttrib(ITEM_LOG_PARAM* pLogParam,_ITEM_TYPE itemType,INT iPlayerLvl,_ITEM& it,INT& quality);
	static	BOOL		CreateGemAttrib(_ITEM_TYPE itemType,_ITEM& it);
	static	BOOL		CreateGemAttrib(UINT TableIndex,_ITEM& it);
	static	BOOL		CreateMedicAttrib(_ITEM_TYPE itemType,_ITEM& it);
	static	BOOL		CreateMaterialAttrib(_ITEM_TYPE itemType,_ITEM& it);
	static	BOOL		CreateQuestItemAttrib(_ITEM_TYPE itemType,_ITEM& it);
	static	BOOL		CreateStoreMapAttrib(_ITEM_TYPE itemType,_ITEM& it);
	static	INT			CaculateQualityIndex(double fRandValue,ITEM_QUALITY_TB* pTB);

};






/*����Exp���������
 *	
 */
class MonsterExpCaculateRuler
{
public:

	UINT				CaculateBaseExp(INT iMonsterLevel,INT iKillerLevel,UINT MonsterBaseExp);
};


/*
 *		������Ʒ����
 *		�Լ��̶�����
 */

class Obj_Monster;
class Obj_Human;

class MonsterDropRuler
{
public:

	static	BOX_DISTRIBUTE_RULER	
						GetMonsterDropRuler(Obj_Monster*	pMonster);

	static	BOOL		CaculateCommOwnerList(Obj_Monster*	pMonster);
	static	BOOL		CaculateBossOwnerList(Obj_Monster*	pMonster);

	static	BOOL		CaculateCommDropRuler(Obj_Monster*	pMonster);

	static	BOOL		CaculateBossDropRuler(Obj_Monster*	pMonster, BOX_DISTRIBUTE_RULER DropRuler);

};

/* ��Ʒ��������
 *	
 */

class ItemRulerCheck
{
public:
	/* �����鷽��
	 *	
	 *		��������    
	 *			ITEM_RULER_LIST ruler			������
	 *		
	 *			const CHAR& iIndex				����ID
	 *
	 *		����
	 *
	 *			BOOL							0 ��ʾû��ͨ��  1 ��ʾͨ���ù���
	 *
	 */
	static BOOL	CheckRuler(ITEM_RULER_LIST ruler,const CHAR& iRuler)
	{

		__ENTER_FUNCTION

		INT iRulerID = iRuler;
		switch(ruler)
		{
		case IRL_DISCARD:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->CanDiscard();
			}
			break;
		case IRL_TILE:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->CanTile();
			}
			break;
		case IRL_SHORTCUT:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->CanShortCut();
			}
			break;
		case IRL_CANSELL:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->CanSell();
			}
			break;
		case IRL_CANEXCHANGE:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->CanExchange();
			}
			break;
		case IRL_CANUSE:
			{
				return	g_ItemTable.GetRuleValueByID(iRulerID)->CanUse();
			}
			break;
		case IRL_PICKBIND:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->isPickBind();
			}
			break;
		case IRL_EQUIPBIND:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->isEquipBind();
			}
			break;
		case IRL_UNIQUE:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->isUnique();
			}
			break;
		default:
			{
				Assert(FALSE);
			}
			break;
		}
		return FALSE;

	__LEAVE_FUNCTION

		return FALSE ;
	
	}



	/*
	 *		��鷽��
	 *		
	 *		��������    
	 *			ITEM_RULER_LIST ruler			������
	 *		
	 *			const CHAR& iIndex				����ID
	 *		
	 *		����
	 *		
	 *			BOOL							0 ��ʾû��ͨ��  1 ��ʾͨ���ù���
	 *	
	 *	
	 *	
	 *
	 */

	static	BOOL	CheckTypeRuler(ITEM_RULER_LIST ruler,const UINT uItemIndex);
	
};







#endif