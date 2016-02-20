//-----------------------------------------------------------------------------
// �ļ��� : LuaFnTbl_Ability.h
// ģ��	:	Script
// ����	 :  �������غ���
// �޸���ʷ:
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_ABILITY_H__
#define __LUAFNTBL_ABILITY_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "Obj_ItemBox.h"
#include "GameTable.h"
#include "ItemManager.h"
#include "GCNewItemBox.h"
#include "GCAbilityLevel.h"
#include "Ability.h"
#include "GCPrescription.h"
#include "GCAbilityExp.h"
#include "AbilityManager.h"
#include "Obj_Monster.h"
#include "StoreManager.h"
#include "GCNotifyEquip.h"
#include "MonsterManager.h"
#include "Obj_Human.h"
#include "HumanItemLogic.h"
#include "GCItemInfo.h"
#include "ItemBoxManager.h"
#include "GCAbilitySucc.h"
#include "ItemOperator.h"

namespace LuaFnTbl
{

	/*
	 *	��ö�ӦObjID ����ҵ�ȫ��ID(��������Ψһ)
	 *     ������Ա :
	 *     ��дʱ�� : 2006-1-20
	 *     �������� : LuaFnGetHumanGUID
	 *     ����˵�� :  
	 *			SceneID_t	SceneID   �������
	 *          ObjID_t		HumanID	  ��ɫ���
     *
	 *	   ����  ��ɫ��ȫ��ID
	 *     ����˵�� :
	 *
	 */

	INT	LuaFnGetHumanGUID(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		GUID_t	guid;


		BEGINHUMANDEFINE("LuaFnGetHumanGUID")
		guid = pHuman->GetGUID();
		Lua_PushNumber(L,guid);
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetGemEmbededCount
	*     ����˵�� : 
	*     ����˵�� :
	*/
	INT LuaFnGetGemEmbededCount(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		UINT BagPos			=	Lua_ValueToNumber(L,3);
	
		BEGINHUMANDEFINE("LuaFnGetGemEmbededCount")
		Item*	pEquipItem	=	HumanItemLogic::GetItem(pHuman,BagPos);
		if(pEquipItem)
		{
			Lua_PushNumber(L,(INT)pEquipItem->GetEquipGemCount());
			return 1;
		}
		else
		{
			Lua_PushNumber(L,-1);
			return 1;
		}
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetBagGemCount
	*     ����˵�� : 
	*
	*	  SceneID_t sceneId ������
	*	  ObjID_t	selfId  HumanID
	*	  UINT		BagPos	�������
	*
	*     ����˵�� :
	*/
	INT LuaFnGetBagGemCount(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId	=	Lua_ValueToNumber(L,1);
		ObjID_t selfId		=	Lua_ValueToNumber(L,2);	
		UINT BagPos			=	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetBagGemCount")
		
		Item*	pEquipItem	=	HumanItemLogic::GetItem(pHuman,BagPos);
		if(pEquipItem && pEquipItem->IsEquip())
		{
			
			Lua_PushNumber(L,(INT)pEquipItem->GetGemSlotMax());
			return 1;
		}
		else
		{
			Lua_PushNumber(L,-1);
			return 1;
		}
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-6-7
	*     �������� : LuaFnGetBagItemLevel
	*     ����˵�� : 
	*
	*				SceneID_t		sceneid
	*				ObjID_t			selfId
	*				UINT			BagIndex
	*				
	*     ����˵�� :
	*		������Ʒ����
	*/
	INT LuaFnGetBagItemLevel(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
        SceneID_t sceneId	= Lua_ValueToNumber(L,1);
		ObjID_t selfId		= Lua_ValueToNumber(L,2);
		UINT BagIndex		= Lua_ValueToNumber(L,3);

		Assert(BagIndex<MAX_BAG_SIZE);

		BEGINHUMANDEFINE("LuaFnGetBagItemLevel")

		Item* pItem = HumanItemLogic::GetItem(pHuman,BagIndex);
		if(pItem==NULL || pItem->IsEmpty())
		{
			RETURNFALSE;
		}
	
		Lua_PushNumber(L,pItem->GetRequireLevel());
		return 1;
		
	
		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE

	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-5-22
	*     �������� : LuaFnGetSlotCost
	*     ����˵�� : 
	*				SceneID_t			SceneID
	*				INT					SlotType;
	
	*     ����˵�� :
	*		   ���ر�ʯ������
	*/
	INT LuaFnGetSlotCost(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId	= Lua_ValueToNumber( L, 1 );
		INT SlotType		= Lua_ValueToNumber( L, 2 );

		//_SLOT_COST*	pResult = g_SlotCostInfoTbl.GetCostByIndex(SlotType);
		//if(pResult)
		//{
		//	Lua_PushNumber(L,TRUE);
		//	Lua_PushNumber(L,pResult->m_CostMoney);
		//	Lua_PushNumber(L,pResult->m_CostItem1);
		//	Lua_PushNumber(L,pResult->m_CostCount1);
		//	Lua_PushNumber(L,pResult->m_CostItem2);
		//	Lua_PushNumber(L,pResult->m_CostCount2);
		//	return 6;
		//}
		
		Lua_PushNumber(L,FALSE);
		return 1;

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-1-17
	*     �������� : LuaFnGetBagItemParam
	*     ����˵�� : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*				UINT				StartPos
	*				INT					Type
	*     ����˵�� :
	*		��ȡ��������Ʒ�Ĳ���
	*/
	INT LuaFnGetBagItemParam(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		UINT BagPos			=	Lua_ValueToNumber(L,3);
		UINT StartPos		=	Lua_ValueToNumber(L,4);
		INT Type			=   Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnGetBagItemParam")
			Item*	pBagItem	=	HumanItemLogic::GetItem(pHuman,BagPos);
		if(pBagItem)
		{

			if(Type<IPV_CHAR || Type>IPV_INT)
				Assert(FALSE);
			ItemParamValue ipv = (ItemParamValue)Type;
			Lua_PushNumber(L,(INT)pBagItem->GetItemParam(StartPos,ipv));
			return 1;
		}
		else
		{
			Lua_PushNumber(L,-1);
			return 1;
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-1-17
	*     �������� : LuaFnSetBagItemParam
	*     ����˵�� : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*				UINT				StartPos
	*				INT					Type
	*				INT					Value
	*     ����˵�� :
	*		���ñ�������Ʒ�Ĳ���
	*/
	INT LuaFnSetBagItemParam(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		UINT BagPos			=	Lua_ValueToNumber(L,3);
		UINT StartPos		=	Lua_ValueToNumber(L,4);
		INT Type			=	Lua_ValueToNumber(L,5);
		INT value			=	Lua_ValueToNumber(L,6);


		BEGINHUMANDEFINE("LuaFnSetBagItemParam")
		Item*	pBagItem	=	HumanItemLogic::GetItem(pHuman,BagPos);
		Assert(pBagItem);

		HumanItemLogic::SetBagItemParam(pHuman,BagPos,StartPos,Type,value);
	
		ENDHUMANDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-1-17
	*     �������� : LuaFnGetBagItemDur
	*     ����˵�� : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*     ����˵�� :
	*		��ȡ��������Ʒ���;ö�
	*/
	INT LuaFnGetBagItemDur(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		UINT BagPos			=	Lua_ValueToNumber(L,3);


		BEGINHUMANDEFINE("LuaFnGetBagItemDur")
		Item*	pBagItem	=	HumanItemLogic::GetItem(pHuman,BagPos);
		if(pBagItem)
		{
			Lua_PushNumber(L,(INT)pBagItem->GetDurPoints());
			return 1;
		}
		else
		{
			Lua_PushNumber(L,-1);
			return 1;
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-1-17
	*     �������� : LuaFnSetBagItemDur
	*     ����˵�� : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*				INT					Dur
	*     ����˵�� :
	*		���ñ�������Ʒ���;ö�
	*/
	INT LuaFnSetBagItemDur(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		UINT BagPos			=	Lua_ValueToNumber(L,3);
		INT Dur				=	Lua_ValueToNumber(L,4);


		BEGINHUMANDEFINE("LuaFnSetBagItemDur")
		Item*	pBagItem	=	HumanItemLogic::GetItem(pHuman,BagPos);
		Assert(pBagItem);
		
		HumanItemLogic::SetBagItemDur(pHuman,BagPos,Dur);
		
		GCItemInfo Msg;
		pBagItem->SaveValueTo(Msg.getItem());
		Msg.setID(BagPos);
		Msg.setIsNull(FALSE);
		pHuman->GetPlayer()->SendPacket(&Msg);
		ENDHUMANDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-3-27
	*     �������� : LuaFnGetBagItemIdent
	*     ����˵�� : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*     ����˵�� :
	*		��ȡ��������Ʒ�ļ�������
	*/
	INT LuaFnGetBagItemIdent(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		UINT BagPos			=	Lua_ValueToNumber(L,3);


		BEGINHUMANDEFINE("LuaFnGetBagItemIdent")
		Item*	pBagItem	=	HumanItemLogic::GetItem(pHuman,BagPos);
		if(pBagItem)
		{
			Lua_PushNumber(L,(INT)pBagItem->GetItemIdent());
			return 1;
		}
		else
		{
			Lua_PushNumber(L,-1);
			return 1;
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-3-27
	*     �������� : LuaFnSetBagItemDur
	*     ����˵�� : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*     ����˵�� :
	*		���ñ�������Ʒ�ļ�������
	*/
	INT LuaFnSetBagItemIdent(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		UINT BagPos			=	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetBagItemIdent")
		Item*	pBagItem	=	HumanItemLogic::GetItem(pHuman,BagPos);
		Assert(pBagItem);

		HumanItemLogic::SetBagItemIdent(pHuman,BagPos);

		GCItemInfo Msg;
		pBagItem->SaveValueTo(Msg.getItem());
		Msg.setID(BagPos);
		Msg.setIsNull(FALSE);
		pHuman->GetPlayer()->SendPacket(&Msg);
		ENDHUMANDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
    
	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetItemQuality
	*     ����˵�� : 
	*     ����˵�� :
	*/
	INT LuaFnGetItemQuality(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		UINT itemIndex = Lua_ValueToNumber(L, 1);
			
		TSerialHelper itemHelper(itemIndex);
		Lua_PushNumber(L, itemHelper.GetItemQual());
		return 1;

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	
	 *	��дʱ�䣺	2006-4-6	15:04
	 *	�������ƣ�	LuaFnGetItemType
	 *	����˵����
	 *				uSerialNumber
	 *					��Ʒ���к�
	 *
	 *	����˵����	������Ʒ���кŵõ���Ʒ�� Type
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnGetItemType(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		UINT uSerialNumber = Lua_ValueToNumber(L, 1);

		Lua_PushNumber(L, GetSerialType(uSerialNumber));
		return 1;

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	
	 *	��дʱ�䣺	2006-4-6	14:00
	 *	�������ƣ�	LuaFnGetBagEquipType
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					��ɫ���
	 *				nBagPos
	 *					��������
	 *
	 *	����˵����	ͨ���������Ӳ鵽����Ʒ������װ�����������װ�����򷵻� -1��
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnGetBagEquipType(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT nBagPos = Lua_ValueToNumber(L,3);
		
		BEGINHUMANDEFINE("LuaFnGetBagEquipType")
		Item* pItem = HumanItemLogic::GetItem(pHuman, nBagPos);
		if( pItem == NULL
			|| pItem->GetItemClass() != ICLASS_EQUIP
			)
		{
			Lua_PushNumber(L, -1);
		}
		else
		{
			Lua_PushNumber(L, pItem->GetEquipPoint());
		}

		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetItemIndex
	*     ����˵�� : 
	*     ����˵�� :
	*/
	INT LuaFnGetItemIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		UINT itemIndex = Lua_ValueToNumber(L, 1);
			
		TSerialHelper itemHelper(itemIndex);
		Lua_PushNumber(L, itemHelper.GetItemIndex());
		return 1;

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetItemBoxRequireAbilityID
	*     ����˵�� : 
	*     ����˵�� :
	*				���Obj_ItemBox��Ҫ������ܱ��
	*		SceneID
	*				�������
	*		ItemBoxID
	*				Obj_ItemBox���
	*
	*		����
	*			AbilityID
	*				��Ҫ����ܵı��
	*/
	INT	LuaFnGetItemBoxRequireAbilityID(Lua_State* L) 
	{												 
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t ItemBoxID		=	Lua_ValueToNumber(L,2);

		INT			ItemBoxType;

		BEGINSCENEDEFINE("LuaFnGetItemBoxRequireAbilityID")

		Obj_ItemBox* pIB = pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
		if(!pIB)
		{
			RETURNFALSE
		}

		ItemBoxType	=	pIB->GetType();

		_GROW_POINT_INFO*	pResult =	g_GrowPointInfoTbl.Get(ItemBoxType);
		
		if(!pResult)
		{
			RETURNFALSE
		}

		Lua_PushNumber(L,pResult->m_OpenAbility);
		return 1;
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddItemToBox
	*     ����˵�� : 
	*     ����˵�� :
	*				�����Ʒ��Obj_ItemBox
	*
	*
	*	  SceneID
	*			����ID
	*	  ItemBoxID
	*			Obj_ItemBox���
	*	  ItemCount
	*			����Obj_ItemBox����Ʒ����,�ܹ����10��
	*	  Item0
	*			0����Ʒ
	*	  ...
	*
	*     Item9 
	*			9����Ʒ
	*     
	*/
	INT LuaFnAddItemToBox(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t ItemBoxID		=	Lua_ValueToNumber(L,2);
		INT iBoxItemCount	=	Lua_ValueToNumber(L,3);


		if(iBoxItemCount>0)
		{
			BEGINSCENEDEFINE("LuaFnAddItemToBox")
			Obj_ItemBox* pIB = pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
			if(pIB)
			{
				ITEM_LOG_PARAM	ItemLogParam;
				ItemLogParam.OpType		= ITEM_CREATE_SCRIPT_TO_BOX;
				ItemLogParam.SceneID	= pScene->SceneID();
				ItemLogParam.XPos		= pIB->getWorldPos()->m_fX;
				ItemLogParam.ZPos		= pIB->getWorldPos()->m_fZ;
			
				for(INT iIndex=0;iIndex<iBoxItemCount;iIndex++)
				{
					INT iItemType = Lua_ValueToNumber(L,4+iIndex);
					INT iQuality = 9;

					_MY_TRY
					{
						pIB->CreateItem(&ItemLogParam,iItemType,iQuality);
						SaveItemLog(&ItemLogParam);
					}
					_MY_CATCH
					{
						SaveCodeLog( ) ;
					}
				}
			}
			ENDSCENEDEFINE
		}

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnItemBoxEnterScene
	*     ����˵�� : 
	*     ����˵�� :
	*				Obj_ItemBox���볡��
	*  
	*	  fx
	*				Obj_ItemBox���볡��X����
	*	  fz	
	*				Obj_ItemBox���볡��Z����
	*	  iBoxType  
	*				Obj_ItemBox����
	*	  SceneID	
	*				�������
	*	  
	*	  ItemCount  
	*				�������Ʒ���� ���10��
	*	  Item0    
	*				�����0����Ʒ
	*	  ������
	*	  Item9		�����9����Ʒ
	*
	*	  ItemBoxID 
	*               ���ص�ItemBoxID
	*/
	INT LuaFnItemBoxEnterScene(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		WORLD_POS pos;
		pos.m_fX		= Lua_ValueToNumber(L,1);
		pos.m_fZ		= Lua_ValueToNumber(L,2);
		INT iBoxType		= Lua_ValueToNumber(L,3);
		SceneID_t sceneId			= Lua_ValueToNumber(L,4);
		INT iBoxItemCount	= Lua_ValueToNumber(L,5);
		
		
		BEGINSCENEDEFINE("LuaFnItemBoxEnterScene")
		Obj_ItemBox* pIB = pScene->GetItemBoxManager()->CreateGrowPointItemBox(iBoxType);
		if(pIB)
		{
			ITEM_LOG_PARAM	ItemLogParam;
			ItemLogParam.OpType		= ITEM_CREATE_SCRIPT_TO_BOX;
			ItemLogParam.SceneID	= pScene->SceneID();
			ItemLogParam.XPos		= pos.m_fX;
			ItemLogParam.ZPos		= pos.m_fZ;

			for(INT iIndex=0;iIndex<iBoxItemCount;iIndex++)
			{
				INT iItemType = Lua_ValueToNumber(L,6+iIndex);
				Assert(iItemType>0);
				INT iQuality = 9;
				_MY_TRY
				{
					pIB->CreateItem(&ItemLogParam,iItemType,iQuality);
					SaveItemLog(&ItemLogParam);
				}
				_MY_CATCH
				{
					SaveCodeLog( ) ;
				}
			}
			pScene->ObjectEnterScene(pIB );
			pIB->setWorldPos( &pos );
			pIB->SetActiveFlag( TRUE );
			Lua_PushNumber(L,pIB->GetID());
			return 1;
		}
		else
		{
			RETURNFALSE
		}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnQueryHumanAbility
	*     ����˵�� : 
	*     ����˵�� :
	*				��ѯhuman Ability�Ƿ�ѧϰ
	*	
	*		sceneId
	*					�������
	*		selfId
	*					��ɫ���
	*		abilityID
	*					�����ID
	*
	*/
	INT LuaFnQueryHumanAbility(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

			SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
			ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
			AbilityID_t abilityId       =	Lua_ValueToNumber(L,3);

			BEGINHUMANDEFINE("LuaFnQueryHumanAbility")
			if(pHuman->__IsAbilityHaveLearned(abilityId))
			{
				Lua_PushNumber(L,1);
				return 1;
			}
			else
			{
				Lua_PushNumber(L,0);
				return 1;
			}
			ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnQueryHumanAbilityLevel
	*     ����˵�� : 
	*     ����˵�� :
	*				��ѯObj_Human����ܵȼ� SceneID,HumanID,AbilityID
	*/
	INT LuaFnQueryHumanAbilityLevel(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		AbilityID_t AbilityID       =	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnQueryHumanAbility")
		INT AbilityLevel =  pHuman->__GetAbilityLevel(AbilityID);
		Lua_PushNumber(L,AbilityLevel);
		return 1;
		ENDHUMANDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetHumanAbilityLevel
	*     ����˵�� : 
	*     ����˵�� :
	*				������������ܼ���,SceneID,HumanID,AbilityID,AbilityLevel
	*/
	INT LuaFnSetHumanAbilityLevel(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		AbilityID_t abilityId       =	Lua_ValueToNumber(L,3);
		INT abilityLvl		=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnSetHumanAbilityLevel")

		Ability* pAbility = g_pAbilityManager->GetAbility(abilityId);
		if ( abilityLvl>pAbility->AbilityLevelLimit() )
		{
			Assert(FALSE && "���ܵȼ�����������");
			return 0;
		}

		pHuman->__SetAbilityLevel(abilityId, abilityLvl);
		GCAbilityLevel Msg;
		Msg.setAbilityId(abilityId);
		Msg.setLevel(abilityLvl);
		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg );



		ABILITY_LOG_PARAM	AbilityLogParam;

		AbilityLogParam.CharGUID	=	pHuman->GetGUID();
		AbilityLogParam.AbilityID	=	abilityId;
		AbilityLogParam.Level		=	abilityLvl;
		AbilityLogParam.OPType		=	ABILITY_OP_LEVELUP;
		AbilityLogParam.SceneID		=	pHuman->getScene()->SceneID();
		AbilityLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
		AbilityLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
		SaveAbilityLog(&AbilityLogParam);

		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}

	/** ----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-10-28
	*     �������� : LuaFnSetAbilityOperaTime
	*     ����˵�� : 
	*     ����˵�� :
	*				��������ܲ���ʱ��
	*
	*	  SceneID
	*				�������
	*	  HumanID
	*				��ɫ���
	*	  OperaTime
	*				����ʱ��
	*
	*
	*
	*/
	INT LuaFnSetAbilityOperaTime(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

			SceneID_t sceneId  = Lua_ValueToNumber(L,1);
			ObjID_t selfId	 = Lua_ValueToNumber(L,2);
			UINT OperaTime	=	Lua_ValueToNumber(L,3);
			
			BEGINHUMANDEFINE("LuaFnSetAbilityOperaTime")

			AbilityOpera*	pAbilityOpera;
			pAbilityOpera = pHuman->GetAbilityOpera();
			Assert( pAbilityOpera );
			pAbilityOpera->m_nMaxTime	=	OperaTime;

			ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;

	}


	/** ----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-10-28
	*     �������� : LuaFnGetAbilityOperaRobotTime
	*     ����˵�� : 
	*     ����˵�� :
	*				�������ܹһ���ʱ��
	*
	*	  SceneID
	*				�������
	*	  HumanID
	*				��ɫ���
	*    ����
	*	  RobotTime
	*				����ʱ��
	*
	*
	*
	*/
	INT LuaFnGetAbilityOperaRobotTime(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId  = Lua_ValueToNumber(L,1);
		ObjID_t selfId	 = Lua_ValueToNumber(L,2);
		UINT		RobotTime;

		BEGINHUMANDEFINE("LuaFnGetAbilityOperaRobotTime")
			AbilityOpera*	pAbilityOpera;
			pAbilityOpera = pHuman->GetAbilityOpera();
			Assert( pAbilityOpera );
			RobotTime	=	 pAbilityOpera->m_nRobotTime;
			Lua_PushNumber(L,RobotTime);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** ----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-10-28
	*     �������� : LuaFnSetAbilityOperaRobotTime
	*     ����˵�� : 
	*     ����˵�� :
	*				��������ܹһ���ʱ��
	*
	*	  SceneID
	*				�������
	*	  HumanID
	*				��ɫ���
	*	  
	*	  RobotTime
	*				����ʱ��
	*
	*
	*
	*/
	INT LuaFnSetAbilityOperaRobotTime(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId  = Lua_ValueToNumber(L,1);
		ObjID_t selfId	 = Lua_ValueToNumber(L,2);
		UINT RobotTime	= Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetAbilityOperaRobotTime")
			AbilityOpera*	pAbilityOpera;
			pAbilityOpera = pHuman->GetAbilityOpera();
			Assert( pAbilityOpera );
			pAbilityOpera->m_nRobotTime	=	RobotTime;
		ENDHUMANDEFINE
		
		LUA_LEAVE_FUNCTION
		return 0 ;

	}

	/****************************************************************************
	 *	������Ա��	
	 *	��дʱ�䣺	2006-3-29	15:54
	 *	�������ƣ�	LuaFnSetAbilityExp
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				abilityId
	 *					����ܱ��
	 *				abilityExp
	 *					����ܾ���
	 *
	 *	����˵����	��������ܾ��飬���͵�ʱ�� / 100 ��Ϊ���������㷽��
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnSetAbilityExp(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		AbilityID_t abilityId       =	Lua_ValueToNumber(L,3);
		INT abilityExp		=	Lua_ValueToNumber(L,4);
		
		BEGINHUMANDEFINE("LuaFnSetAbilityExp")
			INT oldAbilityExp = pHuman->__GetAbilityExp(abilityId);

			pHuman->__SetAbilityExp(abilityId, abilityExp);

			if( abilityExp/100 - oldAbilityExp/100 >= 1 )
			{
				GCAbilityExp Msg;
				Msg.setAbilityId(abilityId);
				Msg.setExp(abilityExp);

				Player* pPlayer = pHuman->GetPlayer();
				pPlayer->SendPacket( &Msg );
			}

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetAbilityExpPlus
	*     ����˵�� : 		
	*		SceneID
	*				�������
	*		HumanID
	*				��ɫ���
	*		AbilityID
	*				����ܱ��
	*     ����˵�� :
	*				�������ܾ���
	*     ���㹫ʽ
	*	            1���PlayerAbilityLevel <= C  ��һ�ü��ܾ��� =B
	*				2��� B-(PlayerAbilityLevel-C)*D <0 ����һ�ü��ܾ���=0
	*				3���� ��һ�ü��ܾ���= B-(PlayerAbilityLevel-C)*D ֮��ȡ��
	*/
	INT	LuaFnGetAbilityExpPlus(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT itemTypeSn      =   Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetAbilityExpPlus")
		
		for(INT i=0;g_pAbilityManager->GetAbilityExpTblRowCount();i++)
		{	
			_AbilityExpTable* pAbilityExpTbl = g_pAbilityManager->GetAbilityExpTbl() + i;
			if(itemTypeSn == pAbilityExpTbl->m_AbilityItemTypeSn)
			{
				INT abilityId	 =  pAbilityExpTbl->m_AbilityId;
				INT abilityLevel =  pHuman->__GetAbilityLevel(abilityId);
				INT abilityExp;

				if( abilityLevel < pAbilityExpTbl->m_AbilityLevel)
					abilityExp = pAbilityExpTbl->m_AbilityBaseExp;
				else 
				{	
					abilityExp = pAbilityExpTbl->m_AbilityBaseExp  - (abilityLevel - pAbilityExpTbl->m_AbilityLevel) * pAbilityExpTbl->m_AbilityCostPercent;
					if( abilityExp < 0)
						abilityExp = 0;
				}
				
				Lua_PushNumber(L,abilityExp);
				return 1;
			}
		}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetAbilityExp
	*     ����˵�� : 		
	*		SceneID
	*				�������
	*		HumanID
	*				��ɫ���
	*		AbilityID
	*				����ܱ��
	*     ����˵�� :
	*				�������ܾ���
	*/
	INT	LuaFnGetAbilityExp(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		AbilityID_t abilityId       =	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetAbilityExp")
			INT				abilityExp;
		abilityExp = pHuman->__GetAbilityExp(abilityId);
		Lua_PushNumber(L,abilityExp);
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-10-17
	*     �������� : LuaFnSetPrescription
	*     ����˵�� : 
	*     ����˵�� :
	*				ѧ��ĳ��ϳ��䷽��ֻ�ܲ���ѧ�ᣬ���ܲ�����������ĳ���䷽������
	*				���䷽���������ɼ��ܷ��������ṩ��
	*/
	INT LuaFnSetPrescription(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		PrescriptionID_t PrescrID		=	Lua_ValueToNumber(L,3);
		BOOL LearnOrAbandon	=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnSetPrescription")

		// TRUE to learn and FALSE to abandon
		pHuman->__SetPrescrLearnedFlag(PrescrID, LearnOrAbandon);

		GCPrescription Msg;
		Msg.setPrescription(PrescrID);
		Msg.setLearnOrAbandon(LearnOrAbandon);

		pHuman->GetPlayer()->SendPacket( &Msg );
		return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-10-17
	*     �������� : LuaFnIsPrescrLearned
	*     ����˵�� : 
	*     ����˵�� :
	*				�ж��Ƿ��Ѿ�ѧ��ĳ���䷽
	*/
	INT LuaFnIsPrescrLearned(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		PrescriptionID_t PrescrID		=	Lua_ValueToNumber(L,3);
		BOOL 			bFlag;

		BEGINHUMANDEFINE("LuaFnIsPrescrLearned")

		bFlag = pHuman->__IsPrescrHaveLearned(PrescrID);
		Lua_PushNumber(L,bFlag);
		return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetAbilityObjIndex
	*     ����˵�� : 
	*     ����˵�� :
	*
	*/
	INT LuaFnGetAbilityObjIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT Index			=	Lua_ValueToNumber(L,3);
		INT				ObjIndex;

		BEGINHUMANDEFINE("LuaFnGetAbilityObjIndex")
		if( Index >= MAX_ABILITY_ITEM )
		{
			RETURNFALSE
		}
		ObjIndex = pHuman->GetAbilityOpera()->m_BagPos[Index];
		Lua_PushNumber(L, ObjIndex);
		return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetItemBoxRecycleTimes
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*				���øú�������ID,short
	*				ItemBoxID
	*					Obj_ItemBox ���
	*				RecycleTimes
	*					���մ���
	*				
	*				���itemBox���մ���
	*/
	INT		LuaFnSetItemBoxRecycleTimes(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t ItemBoxID		=	Lua_ValueToNumber(L,2);	
		INT RecycleTimes    =	Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnSetItemBoxRecycleTimes")
			Obj_ItemBox* pItemBox = pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
			Assert(pItemBox);
			pItemBox->SetRecycleTimes(RecycleTimes);
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetItemBoxRecycleTimes
	*     ����˵�� : 
	*     ����˵�� :
	*			   SceneID	
	*			   ���øú�������ID
	*				ItemBoxID
	*					Obj_ItemBox ���
	*			   ����
	*				RecycleTimes
	*					���մ���
	*
	*				����Obj_ItemBox���մ���
	*/
	INT		LuaFnGetItemBoxRecycleTimes(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			ItemBoxID		=	Lua_ValueToNumber(L,2);	
		INT				RecycleTimes;

		INT				Result	=	0;
		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);
		Obj_ItemBox*	pItemBox	=	pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
		if(pItemBox)
		{
			RecycleTimes = pItemBox->GetRecycleTimes();
			Lua_PushNumber(L,RecycleTimes);
			return 1;
		}
		else
		{
			RETURNFALSE
		}

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetItemBoxOwner
	*     ����˵�� : 
	*     ����˵�� :
	*			   SceneID	
	*			       ���øú�������ID
	*				ItemBoxID
	*					Obj_ItemBox ���
	*				OwnerID
	*					������ID			
	*				����Obj_ItemBox������
	*/
	INT		LuaFnSetItemBoxOwner(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			ItemBoxID		=	Lua_ValueToNumber(L,2);	
		GUID_t			OwnerID			=	Lua_ValueToNumber(L,3);

		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);
		Obj_ItemBox*	pItemBox	=	pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
		Assert(pItemBox);
		pItemBox->SetOwner(OwnerID);

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetItemBoxOpenFlag
	*     ����˵�� : 
	*     ����˵�� :
	*			   SceneID	
	*			       ���øú�������ID
	*				ItemBoxID
	*					Obj_ItemBox ���
	*/
	INT		LuaFnGetItemBoxOpenFlag(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			ItemBoxID		=	Lua_ValueToNumber(L,2);	
		BOOL			bFlag;

		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);
		Obj_ItemBox*	pItemBox	=	pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
		if(pItemBox)
		{
			bFlag	= pItemBox->GetOpenFlag();
			Lua_PushNumber(L,bFlag);
			return 1;
		}
		else
		{
			Lua_PushNumber(L,0);
			return 1;
		}
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetItemBoxOwner
	*     ����˵�� : 
	*     ����˵�� :
	*			   SceneID	
	*			       ���øú�������ID
	*				ItemBoxID
	*					Obj_ItemBox ���
	*			����
	*				OwnerID
	*					������ID	
	*
	*				���Obj_ItemBox������
	*/
	INT		LuaFnGetItemBoxOwner(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			ItemBoxID		=	Lua_ValueToNumber(L,2);	
		GUID_t			OwnerID;

		INT				Result	=	0;
		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);
		Obj_ItemBox*	pItemBox	=	pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
		if(pItemBox)
		{
			OwnerID = pItemBox->GetOwner();
			Lua_PushNumber(L,OwnerID);
			return 1;
		}
		else
		{
			RETURNFALSE
		}
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-1-20
	*     �������� : LuaFnAddGPOwner
	*     ����˵�� : 
	*     ����˵�� :
	*			   SceneID	
	*			       ���øú�������ID
	*			   OwnerID
	*					������ID	
	*
	*				
	*/
	INT		LuaFnAddGPOwner(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			OwnerID			=	Lua_ValueToNumber(L,2);	
		
		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);

		GrowPointManager* pGPManager = 	pScene->GetGrowPointManager();
		Assert(pGPManager);

		pGPManager->AddGrowPointOwner(OwnerID);
		

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/****************************************************************************
	 *	������Ա��	
	 *	��дʱ�䣺	2006-5-17	10:56
	 *	�������ƣ�	LuaFnSetItemCreator
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				nBagPos
	 *					��Ʒ�ڱ����е�λ��
	 *				szCreatorName
	 *					����������
	 *
	 *	����˵����	���������ĳ����Ʒ���Ӵ���������
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnSetItemCreator(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		INT nBagPos = Lua_ValueToNumber(L, 3);
		const CHAR* szCreatorName = Lua_ValueToString(L, 4);

		BEGINHUMANDEFINE("LuaFnSetItemCreator")
			Assert( szCreatorName );

			Item* pItem = HumanItemLogic::GetItem(pHuman, nBagPos);
			if ( pItem->IsEmpty() != TRUE )
			{ // ��������������
				ItemContainer* pContainer;

				pContainer = HumanItemLogic::GetItemContain( pHuman, pItem->GetItemTableIndex() );
				INT nIndex = pContainer->BagIndex2ConIndex( nBagPos );
				g_ItemOperator.SetItemCreator( pContainer, nIndex, szCreatorName );

				GCNotifyEquip Msg;
				Msg.SetBagIndex( nBagPos );
				pItem->SaveValueTo(Msg.GetItem());
				pHuman->GetPlayer()->SendPacket(&Msg);
			}

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-1-20
	*     �������� : LuaFnDelGPOwner
	*     ����˵�� : 
	*     ����˵�� :
	*			   SceneID	
	*			       ���øú�������ID
	*			   OwnerID
	*					������ID	
	*
	*				
	*/
	INT		LuaFnDelGPOwner(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			OwnerID			=	Lua_ValueToNumber(L,2);	

		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);

		GrowPointManager* pGPManager = 	pScene->GetGrowPointManager();
		Assert(pGPManager);

		pGPManager->DelGrowPointOwner(OwnerID);


		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-1-20
	*     �������� : LuaFnGetGPCountByOwner
	*     ����˵�� : 
	*     ����˵�� :
	*			   SceneID	
	*			       ���øú�������ID
	*			   OwnerID
	*					������ID	
	*			  ���ض�ӦOwnerID ���е�gpCount
	*				
	*/
	INT		LuaFnGetGPCountByOwner(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			OwnerID			=	Lua_ValueToNumber(L,2);	

		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);

		GrowPointManager* pGPManager = 	pScene->GetGrowPointManager();
		if(pGPManager)
		{
			INT Count = 	pGPManager->GetGrowPointCountByOwner(OwnerID);
			Lua_PushNumber(L,Count);
			return 1;
		}
		else
		{
			Lua_PushNumber(L,0);
			return 1;
		}

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetItemBoxWorldPosX
	*     ����˵�� : 
	*     ����˵�� :
	*			   SceneID	
	*			       ���øú�������ID
	*				ItemBoxID
	*					Obj_ItemBox ���
	*			����
	*				fX
	*					Obj_ItemBox��������X
	*
	*				���Obj_ItemBox��������X
	*/
	INT		LuaFnGetItemBoxWorldPosX(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			ItemBoxID		=	Lua_ValueToNumber(L,2);	
		FLOAT			fX;

		INT				Result	=	0;
		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);
		Obj_ItemBox*	pItemBox	=	pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
		if(pItemBox)
		{
			fX = pItemBox->getWorldPos()->m_fX;
			Lua_PushNumber(L,fX);
			return 1;
		}
		else
		{
			Lua_PushNumber(L,0);
			return 1;
		}
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetItemBoxWorldPosZ
	*     ����˵�� : 
	*     ����˵�� :
	*			   SceneID	
	*			       ���øú�������ID
	*				ItemBoxID
	*					Obj_ItemBox ���
	*			����
	*				fZ
	*					Obj_ItemBox��������Z
	*				���Obj_ItemBox��������Z
	*/
	INT		LuaFnGetItemBoxWorldPosZ(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			ItemBoxID		=	Lua_ValueToNumber(L,2);	
		FLOAT			fZ;

		INT				Result	=	0;
		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);
		Obj_ItemBox*	pItemBox	=	pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
		if(pItemBox)
		{
			fZ = pItemBox->getWorldPos()->m_fZ;
			Lua_PushNumber(L,fZ);
			return 1;
		}
		else
		{
			Lua_PushNumber(L,0);
			return 1;
		}
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetItemBoxMaxGrowTime
	*     ����˵�� : 
	*     ����˵�� :
	*     ����˵�� :
	*				SceneID
	*					�������
	*				ItemBoxID
	*					Obj_ItemBox���
	*			����
	*				maxGrowTime
	*					Obj_ItemBox�������ʱ��
	*
	*					���Obj_ItemBox�������ʱ��
	*/
	INT		LuaFnGetItemBoxMaxGrowTime(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			ItemBoxID		=	Lua_ValueToNumber(L,2);	
		UINT			maxGrowTime;

		INT				Result	=	0;
		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);
		Obj_ItemBox*	pItemBox	=	pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
		if(pItemBox)
		{
			maxGrowTime = pItemBox->GetMaxGrowTime();
			Lua_PushNumber(L,maxGrowTime);
			return 1;
		}
		else
		{
			Lua_PushNumber(L,0);
			return 1;
		}
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetItemBoxMaxGrowTime
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				ItemBoxID
	*					Obj_ItemBox���
	*				maxGrowTime
	*					Obj_ItemBox�������ʱ��
	*				����Obj_ItemBox�������ʱ��
	*/
	INT		LuaFnSetItemBoxMaxGrowTime(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			ItemBoxID		=	Lua_ValueToNumber(L,2);	
		UINT			maxGrowTime    =	Lua_ValueToNumber(L,3);

		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);
		Obj_ItemBox*	pItemBox	=	pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
		Assert(pItemBox);
		pItemBox->SetMaxGrowTime(maxGrowTime);

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetItemBoxPickOwnerTime
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				ItemBoxID
	*					Obj_ItemBox���
	*				PickProtTime
	*					���ñ���ʱ�䳤��
	*				����Obj_ItemBox�����߱���ʱ��
	*/
	INT		LuaFnSetItemBoxPickOwnerTime(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			ItemBoxID		=	Lua_ValueToNumber(L,2);	
		UINT			PickProtTime	=	Lua_ValueToNumber(L,3);

		INT				Result	=	0;
		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);
		Obj_ItemBox*	pItemBox	=	pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
		Assert(pItemBox);

		pItemBox->SetPickOwnerTime(PickProtTime);
		
		return 0;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnEnableItemBoxPickOwnerTime
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				ItemBoxID
	*					Obj_ItemBox���
	*				����ʱ�俪ʼ��ʱ
	*/
	INT		LuaFnEnableItemBoxPickOwnerTime(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		ObjID_t			ItemBoxID		=	Lua_ValueToNumber(L,2);	
		INT				Result	=	0;
		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);
		Obj_ItemBox*	pItemBox	=	pScene->GetItemBoxManager()->GetItemBox(ItemBoxID);
		Assert(pItemBox);

		pItemBox->EnablePickOwnerTime();

		return 0;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnCreateMonsterOnScene
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				MonsterType
	*					��������
	*				fx
	*					λ��x
	*				fz
	*					λ��z
	*				AIType
	*					����AI����
	*				AIScriptID
	*					����AIScriptID
	*
	*				�ڳ����������� SceneID,MonsterType,x,z,AIType
	*/
	INT	  LuaFnCreateMonsterOnScene(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		AssertEx( FALSE, "�˽ӿ��Ѿ���������ʹ�á�LuaFnCreateMonster��" ) ;
		return 0 ;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-24
	*     �������� : LuaFnCreateMonster
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				MonsterType
	*					��������
	*				fx
	*					λ��x
	*				fz
	*					λ��z
	*				AIType
	*					����AI����
	*				AIScriptID
	*					����AIScriptID
	*				ScriptID
	*					�¼��ű�ID
	*
	*				�ڳ����������� SceneID,MonsterType,x,z,AIType
	*/
	INT	  LuaFnCreateMonster(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		INT				MonsterType		=	Lua_ValueToNumber(L,2);	
		WORLD_POS pos;
		pos.m_fX						=	Lua_ValueToNumber(L,3);
		pos.m_fZ						=	Lua_ValueToNumber(L,4);
		INT				iAIType			=	Lua_ValueToNumber(L,5);	
		INT				iAIScript		=	Lua_ValueToNumber(L,6);	
		INT				ScriptID		=	Lua_ValueToNumber(L,7);	

		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);
		
		_OBJ_MONSTER_INIT	init;

		init.m_uDataID			=	MonsterType ;
		init.m_Pos.m_fX			=	pos.m_fX ;
		init.m_Pos.m_fZ			=	pos.m_fZ ;
		init.m_RespawnTime		=	-1;
		init.m_BaseAI			=	iAIType ;
		init.m_ExtAIScript		=   iAIScript;
		init.m_idScript			=	ScriptID ;

		ObjID_t ObjID = pScene->CreateTempMonster((const _OBJ_MONSTER_INIT*)&init) ;

		Lua_PushNumber(L,ObjID);
		return 1;


		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-24
	*     �������� : LuaFnDeleteMonster
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				ObjID
	*					�����
	*
	*				ɾ����̬���������Ĺ�(��NPC)��ע�⣺������ڵĹֲ��ܱ�ɾ��
	*/
	INT	  LuaFnDeleteMonster(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		INT				ObjID			=	Lua_ValueToNumber(L,2);	

		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);

		Obj* pObj = pScene->GetObjManager()->GetObj(ObjID) ;
		if( pObj ==NULL )
			return 0 ;

		if( pObj->GetObjType() != Obj::OBJ_TYPE_MONSTER )
			return 0 ;

		Obj_Monster* pMonster = (Obj_Monster*)pObj ;
        		
		pScene->DeleteTempMonster(pMonster);


		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddStorePointOnScene
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*						�������
	*				StorePointType
	*						�ر�������
	*				fx
	*						�ر���x
	*				fz		
	*						�ر���z
	*				
	*				�ڳ��������һ���ر���
	*/
	INT	LuaFnAddStorePointOnScene(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t		SceneID			=	Lua_ValueToNumber(L,1);
		INT				StorePointType	=	Lua_ValueToNumber(L,2);	
		WORLD_POS		pos;
		pos.m_fX						=	Lua_ValueToNumber(L,3);	
		pos.m_fZ						=	Lua_ValueToNumber(L,4);	

		Scene*		pScene		=	g_pSceneManager->GetScene(SceneID);
		Assert(pScene);

		STORE_POINT	point;
		point.m_Pos		=	pos;
		point.m_Type	=	StorePointType;
		pScene->GetStoreManager()->AddStorePoint(&point);

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : DelStorePointOnScene
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*						�������
	*				StorePointType
	*						�ر�������
	*				fx
	*						�ر���x
	*				fz		
	*						�ر���z
	*				
	*				�ڳ�����ɾ��һ���ر���
	*/
	INT	LuaFnDelStorePointOnScene(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		INT StorePointType	=	Lua_ValueToNumber(L,2);	
		INT index			=	Lua_ValueToNumber(L,3);	

		BEGINSCENEDEFINE("LuaFnCheckTimer")

		pScene->GetStoreManager()->DelStorePoint(index);

		WORLD_POS pos = pScene->GetStoreManager()->GetStorePointPos(index);

		pScene->GetGrowPointManager()->DecGrowPointTypeCount(StorePointType,pos.m_fX,pos.m_fZ);
		
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnFindStorePointOnScene
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*						�������
	*				StorePointType
	*						�ر�������
	*				fx
	*						�ر���x
	*				fz		
	*						�ر���z
	*				
	*				�ڳ��������һ���ر���
	*/
	INT	LuaFnFindStorePointOnScene(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		INT StorePointType	=	Lua_ValueToNumber(L,2);	
		WORLD_POS		pos;
		pos.m_fX						=	Lua_ValueToNumber(L,3);	
		pos.m_fZ						=	Lua_ValueToNumber(L,4);	

		BEGINSCENEDEFINE("LuaFnFindStorePointOnScene")

		STORE_POINT	point;
		point.m_Pos		=	pos;
		point.m_Type	=	StorePointType;

		INT	index;

		pScene->GetStoreManager()->FindStorePointByPos(pos,index);
		
		Lua_PushNumber(L,index);
		return 1;
		ENDSCENEDEFINE
		

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetStorePointType
	*     ����˵�� : 
	*				SceneID
	*						�������
	*				StorePointType
	*						�ر�������
	*				fx
	*						�ر���x
	*				fz		
	*						�ر���z
	*	
	*     ����˵�� :				
	*				�ڳ�������һ���ر�������
	*/
	INT	LuaFnGetStorePointType(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		INT index			=	Lua_ValueToNumber(L,2);	
		
		BEGINSCENEDEFINE("LuaFnGetStorePointType")
		INT GrowPointType = pScene->GetStoreManager()->GetStorePointType(index);
		Lua_PushNumber(L,GrowPointType);
		return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnTryRecieveItem
	*     ����˵�� : 
	*				SceneID
	*						�������
	*				HumanID
	*						������
    *				ItemType
	*						��Ʒ9λ��ʾ��
	*				Quality
	*						��ɫװ��Ʒ�ʶ�,����װ�������һ������
	*					
	*	
	*     ����˵�� :
	*				���Խ�����Ʒ
	*/
	INT	LuaFnTryRecieveItem(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT iItemId			=	Lua_ValueToNumber(L,3);	
		INT iQuality		=	Lua_ValueToNumber(L,4);	
		
		Assert(iItemId>0);

		BEGINHUMANDEFINE("LuaFnTryRecieveItem")

		Assert(iQuality>INVALID_ID);
		TSerialHelper	help(iItemId);

		_ITEM_TYPE		itemType = help.GetItemTypeStruct();

		UINT iBagIndex=INVALID_INDEX;
		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType		= ITEM_CREATE_FROM_ABILITY_SCRIPT;
		ItemLogParam.CharGUID	= pHuman->GetGUID();
		ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
		ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
		ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
	
		_MY_TRY
		{
			HumanItemLogic::CreateItemToBag(&ItemLogParam,pHuman,itemType.ToSerial(),iBagIndex, 0);

			if(iBagIndex==INVALID_INDEX) //����ʧ��
			{
				Lua_PushNumber(L,INVALID_INDEX);
				return 1;
			}

			SaveItemLog(&ItemLogParam);
			Item*	pDestItem = HumanItemLogic::GetItem(pHuman,iBagIndex);
			Assert(pDestItem);
			GCNotifyEquip	msg;
			msg.SetBagIndex(iBagIndex);
			pDestItem->SaveValueTo(msg.GetItem());
			pHuman->GetPlayer()->SendPacket(&msg);
		}
		_MY_CATCH
		{
			SaveCodeLog( ) ;
		}

		Lua_PushNumber(L,iBagIndex);
		return 1;

		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetHumanWorldX
	*     ����˵�� : 
	*     ����˵�� :
	*				���Obj_Human����������X ����SceneID,HumanID,����WorldX
	*/
	INT	LuaFnGetHumanWorldX(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		
		FLOAT			WorldX;

		BEGINHUMANDEFINE("LuaFnGetHumanWorldX")
		WorldX	=	pHuman->getWorldPos()->m_fX;

		Lua_PushNumber(L,WorldX);
		return	1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetHumanWorldZ
	*     ����˵�� : 
	*     ����˵�� :
	*				���Obj_Human����������Z ����SceneID,HumanID,����WorldZ
	*/
	INT	LuaFnGetHumanWorldZ(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);

		FLOAT			WorldZ;

		BEGINHUMANDEFINE("LuaFnGetHumanWorldZ")
		WorldZ	=	pHuman->getWorldPos()->m_fZ;
		Lua_PushNumber(L,WorldZ);
		return	1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetStoreMapWorldX
	*     ����˵�� : 

	*				SceneID
	*					�������
	*				HumanID
	*					������
	*				BagIndex
	*					����λ��
	*				fx
	*					�ر�ͼx
	*     ����˵�� :
	*			��òر�ͼλ��X
	*/
	INT	LuaFnGetStoreMapX(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		UINT bagIndex		=	Lua_ValueToNumber(L,3);

		FLOAT			MapX;

		BEGINHUMANDEFINE("LuaFnGetStoreMapX")
		
		Item*	pStoreMapItem = 	HumanItemLogic::GetItem(pHuman,bagIndex);
		Assert(pStoreMapItem);

		if(pStoreMapItem->GetItemClass()==ICLASS_STOREMAP)
		{
			MapX	=	pStoreMapItem->GetXPos();
			Lua_PushNumber(L,MapX);
			return	1;
		}
		else
		{
			AssertEx(FALSE,"����Ʒ���ǲر�ͼ");
			RETURNFALSE
		}
		ENDHUMANDEFINE


		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetStoreMapWorldZ
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				HumanID
	*					������
	*				BagIndex
	*					����λ��
	*				fz
	*					�ر�ͼz
	*			��òر�ͼλ��Z
	*/
	INT	LuaFnGetStoreMapZ(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		UINT bagIndex		=	Lua_ValueToNumber(L,3);
		FLOAT			MapZ;

		BEGINHUMANDEFINE("LuaFnGetStoreMapZ")

		Item*	pStoreMapItem = 	HumanItemLogic::GetItem(pHuman,bagIndex);
		Assert(pStoreMapItem);

		if(pStoreMapItem->GetItemClass()==ICLASS_STOREMAP)
		{
			MapZ	=	pStoreMapItem->GetZPos();
			Lua_PushNumber(L,MapZ);
			return	1;
		}
		else
		{
			AssertEx(FALSE,"����Ʒ���ǲر�ͼ");
			RETURNFALSE
		}
		ENDHUMANDEFINE


		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetStoreMapSceneID
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				HumanID
	*					������
	*				BagIndex
	*					����λ��
	*				MapSceneID
	*					�ر�ͼ����
	*     ����˵�� :
	*			��òر�ͼ����ID
	*/
	INT	LuaFnGetStoreMapSceneID(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		UINT bagIndex		=	Lua_ValueToNumber(L,3);

		SceneID_t		MapSceneID;

		BEGINHUMANDEFINE("LuaFnGetStoreMapSceneID")

		Item*	pStoreMapItem = 	HumanItemLogic::GetItem(pHuman,bagIndex);
		Assert(pStoreMapItem);

		if(pStoreMapItem->GetItemClass()==ICLASS_STOREMAP)
		{
			MapSceneID	=	pStoreMapItem->GetSceneID();
			Lua_PushNumber(L,MapSceneID);
			return	1;
		}
		else
		{
			AssertEx(FALSE,"����Ʒ���ǲر�ͼ");
			RETURNFALSE
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	*	������Ա��	
	*	��дʱ�䣺	2005-11-1	10:15
	*	�������ƣ�	LuaFnQueryAreaStandingTime
	*	����˵����
	*				sceneId
	*					�������
	*				selfId
	*					������ ID���޷�������
	*
	*	����˵����	��ѯ������¼�����ͣ����ʱ��
	*	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnQueryAreaStandingTime(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L, 1);
		ObjID_t selfId			=	Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnQueryAreaStandingTime")

		UINT uStandingTime = pHuman->NowTime();
		uStandingTime -= pHuman->GetAreaTimeStamp();

		Lua_PushNumber(L, uStandingTime);
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

	LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	*	������Ա��	
	*	��дʱ�䣺	2005-11-1	11:02
	*	�������ƣ�	LuaFnResetAreaStandingTime
	*	����˵����
	*				sceneId
	*					�������
	*				selfId
	*					������ ID���޷�������
	*				nTimeStampOffset
	*					�µ�ʱ���ƫ���������룩
	*
	*	����˵����	����������ҽ����¼������ʱ�䣨��Ϊ��һ�δ����¼���ʱ�����
	*	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnResetAreaStandingTime(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L, 1);
		ObjID_t selfId			=	Lua_ValueToNumber(L, 2);
		INT nTimeStampOffset			=	Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("ResetAreaStandingTime")

		UINT uNewTimeStamp;
		uNewTimeStamp = pHuman->NowTime() + nTimeStampOffset;
		pHuman->ResetAreaTimeStamp(uNewTimeStamp);

		return 0;
		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/****************************************************************************
	 *	������Ա��	
	 *	��дʱ�䣺	2006-4-10	15:06
	 *	�������ƣ�	LuaFnSendAbilitySuccessMsg
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				AbilityId
	 *					����� ID ��
	 *				RecipeId
	 *					�䷽ ID ��
	 *				ItemSerialNum
	 *					��Ʒ���к�
	 *
	 *	����˵����	��ͻ��˷�����Ʒ����ɹ�����Ϣ
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnSendAbilitySuccessMsg(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		INT AbilityId = Lua_ValueToNumber(L, 3);
		INT RecipeId = Lua_ValueToNumber(L, 4);
		UINT ItemSerialNum = Lua_ValueToNumber(L, 5);

		BEGINHUMANDEFINE("LuaFnSendAbilitySuccessMsg")

			GCAbilitySucc Msg;
			Msg.SetAbilityID( AbilityId );
			Msg.SetPrescriptionID( RecipeId );
			Msg.SetItemSerial( ItemSerialNum );
			pHuman->GetPlayer()->SendPacket( &Msg );

		return 0;
		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION
		return 0 ;
	}

	INT LuaFnPullOnEquip(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT nBagPos = Lua_ValueToNumber(L,3);
		
		BEGINHUMANDEFINE("LuaFnPullOnEquip")

		//ItemContainer* pBagContainer = pHuman->GetBaseContain();
		//ItemContainer* pEquipContainer = pHuman->GetEquipContain();
		//Item* pUseItem = HumanItemLogic::GetItem( pHuman, nBagPos );
		//Assert( pBagContainer );
		//Assert( pEquipContainer );
		//Assert( pUseItem );

		//HUMAN_EQUIP Equip_Point = (HUMAN_EQUIP)pUseItem->GetEquipPoint();
		//Item* pEquipItem = pEquipContainer->GetItem((UINT)Equip_Point);
		//Assert( pEquipItem );

		//INT ret;
		//BOOL bEquipStatus = (!pEquipItem->IsEmpty());
		//if(bEquipStatus)
		//{
		//	//װ������Ʒ,�滻����װ��������
		//	ret = g_ItemOperator.ExchangeItem( pBagContainer, pBagContainer->BagIndex2ConIndex(nBagPos), pEquipContainer, (INT)Equip_Point );
		//}
		//else
		//{
		//	ret = g_ItemOperator.MoveItem( pBagContainer, pBagContainer->BagIndex2ConIndex(nBagPos), pEquipContainer, (INT)Equip_Point );
		//}

		//ˢ��װ�������������Ӱ��
		//pHuman->ItemEffectFlush();
		//����ɼ�
		//if(pHuman->IsVisualPart(Equip_Point))
		//{
		//}

		Lua_PushNumber(L, -1);
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}



}

#endif