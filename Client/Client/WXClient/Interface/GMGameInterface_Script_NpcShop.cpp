#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "LuaPlus.h"
#include "../Event/GMEventSystem.h"


namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	NpcShop NpcShop::s_NpcShop;
	LuaPlus::LuaObject* NpcShop::s_pMetaTable = NULL;

	//�ر�
	INT NpcShop::Close(LuaPlus::LuaState* state)
	{
		//�ڵ��Quest��"�ټ�"��ʱ�򣬹ر�NpcShop
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_BOOTH);

		return 0;
	}
	//����̵�NpcID
	INT NpcShop::GetNpcId(LuaPlus::LuaState* state)
	{
		state->PushInteger(CDataPool::GetMe()->Booth_GetShopNpcId());

		return 1;
	}
	//����̵����Ʒ
	//INT NpcShop::EnumCallBackItem(LuaPlus::LuaState* state)
	//{
	//	LuaStack args(state);
	//	if (!(args[2].IsInteger()))
	//	{
	//		TDThrow("LUA:NpcShop::EnumCallBackItem[2] param parameter error");
	//		return 0;
	//	}
	//	INT nIndex = args[2].GetInteger();

	//	tActionItem* pActionItem = CActionSystem::GetMe()->EnumAction(nIndex, NAMETYPE_CALLBACK);
	//	if(pActionItem)
	//	{
	//		LuaObject objReturn = state->BoxPointer(pActionItem);
	//		objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
	//		objReturn.PushStack();

	//		return 1;
	//	}

	//	return 1;
	//}
	//����̵����Ʒ�۸�
	INT NpcShop::EnumItemPrice(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:NpcShop::EnumItemPrice[2] param parameter error");
			return 0;
		}
		INT nIndex = args[2].GetInteger();

		state->PushInteger(CDataPool::GetMe()->Booth_GetItemPrice(nIndex));
		return 1;
	}

	//����̵����Ʒ����������
	INT NpcShop::EnumItemMaxOverlay(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:NpcShop::EnumItemMaxOverlay[2] param parameter error");
			return 0;
		}
		INT nIndex = args[2].GetInteger();

		tObject_Item* pBoothItem = CDataPool::GetMe()->Booth_GetItem(nIndex);
		if(pBoothItem)
		{
			state->PushInteger(pBoothItem->GetMaxOverLay());
		}
		else
		{
			state->PushInteger(-1);
		}
		return 1;
	}

	//����̵����Ʒ������
	INT NpcShop::EnumItemName(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:NpcShop::EnumItemName[2] param parameter error");
			return 0;
		}
		INT nIndex = args[2].GetInteger();

		tObject_Item* pBoothItem = CDataPool::GetMe()->Booth_GetItem(nIndex);
		if(pBoothItem)
		{
			state->PushString(pBoothItem->GetName());
		}
		else
		{
			state->PushString("");
		}
		return 1;
	}

	//һ�ι�������Ʒ
	//INT NpcShop::BulkBuyItem(LuaPlus::LuaState* state)
	//{
	//	LuaStack args(state);
	//	if (!(args[2].IsInteger()))
	//	{
	//		TDThrow("LUA:NpcShop::BulkBuyItem[2] param parameter error");
	//		return 0;
	//	}
	//	if (!(args[3].IsInteger()))
	//	{
	//		TDThrow("LUA:NpcShop::BulkBuyItem[3] param parameter error");
	//		return 0;
	//	}
	//	INT nIndex = args[2].GetInteger();
	//	INT nNumber = args[3].GetInteger();

	//	tObject_Item* pBoothItem = CDataPool::GetMe()->Booth_GetItem(nIndex);
	//	if(pBoothItem)
	//	{
	//		nNumber = (nNumber < 1 || nNumber > pBoothItem->GetMaxOverLay())?1:nNumber;

	//		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money() >= 
	//			(INT)(CDataPool::GetMe()->Booth_GetItemPrice(nIndex)*nNumber))
	//		{
	//			CGShopBuy msg;

	//			msg.SetUniqueID(CDataPool::GetMe()->Booth_GetShopUniqueId());
	//			msg.SetIndex(pBoothItem->GetPosIndex());
	//			msg.SetSerialNum(CDataPool::GetMe()->Booth_GetSerialNum());
	//			msg.SetBuyNum(nNumber);

	//			CNetManager::GetMe()->SendPacket( &msg );
	//			return 0;
	//		}
	//	}

	//	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "��Ǯ����");

	//	return 0;
	//}

	//��ûع���Ʒ������
	INT NpcShop::GetCallBackNum(LuaPlus::LuaState* state)
	{	
		INT nItemNum = CDataPool::GetMe()->Booth_GetSoldNumber();
		if(nItemNum >= 0)
			state->PushInteger(nItemNum);
		else
			state->PushInteger(0);

		return 1;
	}
	//����̵����ϸ����
	//INT NpcShop::GetShopType(LuaPlus::LuaState* state)
	//{
	//	// �����ﵼ���̵���ص����е�����
	//	LuaStack args(state);
	//	if (!(args[2].IsString()))
	//	{
	//		TDThrow("LUA:NpcShop::GetShopType[2] param parameter error");
	//	}

	//	std::string str = args[2].GetString();
	//	if(str == "repair")
	//	{
	//		state->PushInteger(CDataPool::GetMe()->Booth_GetRepairType());
	//	}
	//	else if(str == "callback")
	//	{
	//		state->PushInteger(CDataPool::GetMe()->Booth_GetCallBack());
	//	}
	//	else if(str == "unit")
	//	{
	//		state->PushInteger(CDataPool::GetMe()->Booth_GetCurrencyUnit());
	//	}
	//	else if(str == "buy")//�չ�
	//	{
	//		state->PushInteger(CDataPool::GetMe()->Booth_GetBuyType());
	//	}

	//	return 1;
	//}
	// ��õ�ǰ�������װ������������۸񣬣���Ҫ�������Npc������۸�ϵ�����ܲ��������������
	//INT NpcShop::GetRepairAllPrice(LuaPlus::LuaState* state)
	//{
	//	INT nRepairPrice = 0;
	//	FLOAT fRepairSpend = CDataPool::GetMe()->GetMe()->Booth_GetRepairSpend();

	//	for(INT i=HEQUIP_WEAPON; i<HEQUIP_NUMBER; i++)
	//	{
	//		tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)i);
	//		if(pItem)
	//		{
	//			if( pItem->GetItemClass() == ICLASS_EQUIP )
	//			{
	//				if(pItem->GetItemDur() < pItem->GetItemMaxDur())
	//				{
	//					//��������۸�
	//					nRepairPrice += (INT)(((CObject_Item_Equip*)pItem)->GetRepairPrice() * fRepairSpend);
	//				}
	//			}
	//		}
	//	}
	//	state->PushInteger( nRepairPrice );

	//	return 1;
	//}

	//�õ�NPC����Ӫ
	INT NpcShop::GetNpcCamp(LuaPlus::LuaState* state)
	{
		return 0; 
	}
};

