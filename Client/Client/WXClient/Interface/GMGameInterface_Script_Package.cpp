#include "StdAfx.h"
#include "GMGameInterface_Script_Package.h"
#include "TDException.h"
#include "LuaPlus.h"
#include "..\Action\GMActionSystem.h"
#include "GameDefine.h"
#include "../Event/GMEventSystem.h"
#include "../Network/NetManager.h"
#include "../DataPool/GMDataPool.h"
#include "..\Object\ObjectManager.h"
#include "..\Object\Object.h"
#include "..\Object\Character\Obj_PlayerMySelf.h"
#include "..\Object\Character\AI\GMAI_MySelf.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "../procedure/GameProcedure.h"
#include "../procedure/GamePro_Main.h"

#include "CGStallApply.h"
#include "CGStallOpen.h"
#include "CGSplitItem.h"
#include "CGPackUpPacket.h"
#include "CGAskLockObj.h"

namespace SCRIPT_SANDBOX
{

PlayerPackage PlayerPackage::s_PlayerPackage;
LuaPlus::LuaObject* PlayerPackage::s_pMetaTable = NULL;

INT PlayerPackage::EnumItem(LuaPlus::LuaState* state)
{
	//const static STRING s_strBaseName		= "base";		//������Ʒ  0-19
	//const static STRING s_strMaterialName	= "material";	//������Ʒ 20-39
	//const static STRING s_strQuestName		= "quest";		//������Ʒ 40-59

	LuaStack args(state);

	//������
	if (!(args[2].IsString()))
	{
		TDThrow("LUA: PlayerPackage::EnumItem Wrong Param1");
	}
	if (!(args[3].IsInteger()))
	{
		TDThrow("LUA: PlayerPackage::EnumItem Wrong Param2");
	}

	STRING strPakageType = args[2].GetString();
	INT nIndex = args[3].GetInteger();
	
	//��ѯ��Ʒ
	//INT nOffIndex = -1;
	//if(strPakageType == s_strBaseName)
	//{
	//	nOffIndex = BASE_CONTAINER_OFFSET;
	//}
	//else if(strPakageType == s_strMaterialName)
	//{
	//	nOffIndex = MAT_CONTAINER_OFFSET;
	//}
	//else if(strPakageType == s_strQuestName)
	//{
	//	nOffIndex = TASK_CONTAINER_OFFSET;
	//}

	//if(nOffIndex >= 0)
	{
		tActionItem* pActionItem = CActionSystem::GetMe()->EnumAction(nIndex, NAMETYPE_PACKAGEITEM);
		if(pActionItem)
		{
			LuaObject objReturn = state->BoxPointer(pActionItem);
			objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
			objReturn.PushStack();

			if(((tObject_Item*)pActionItem->GetImpl())->IsLocked())
				state->PushInteger(1);
			else
				state->PushInteger(0);
			return 2;
		}
	}

	//�Ƿ�ActionItem
	LuaObject objReturn = state->BoxPointer(&(CActionItem::s_InvalidAction));
	objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
	objReturn.PushStack();


	state->PushInteger(0);
	return 2;
}

// �򿪰�̯�����ۣ�����
INT PlayerPackage::OpenStallSaleFrame(LuaPlus::LuaState* state)
{
	//�ж��Ƿ������״̬���߱���״̬��������״̬���ܰ�̯
	if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_ModelID() != -1 )
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����״̬�����ܰ�̯");
		return 0;
	}
	if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID() != -1 )
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���״̬�����ܰ�̯");
		return 0;
	}
	//�ж��û��ǲ��Ǵ����Զ��ܶ�״̬�У�����������״̬���ܰ�̯
	if(	CGameProcedure::s_pProcMain->IsAutoRun() )
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "�Զ�����״̬�����ܰ�̯");
		return 0;
	}
	//�����ս״̬�����ܰ�̯
	if( !CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID().IsNull() )
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "�����ս״̬�����ܰ�̯");
		return 0;
	}

	//��Ҫ���ж����û������ǲ��Ǵ��ڰ�̯״̬��
	if( CDataPool::GetMe()->MyStallBox_IsOpen() )
	{
		//�����ڰ�̯״̬ʱ 
		CGStallOpen msg;
		msg.SetObjID(CObjectManager::GetMe()->GetMySelf()->GetServerID());
		CNetManager::GetMe()->SendPacket(&msg);
	}
	else
	{
		//��Ӹ���״̬
		BOOL bTeamFollowMode = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_TeamFollowFlag();
		if(bTeamFollowMode)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��ǰ����Ӹ���״̬�����ܰ�̯");
			return 0;
		}
		// ��ͨ���治�ܽ��а�̯
		if(((CAI_MySelf*)CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI())->IsFollow())
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��ǰ�Ǹ���״̬�����ܰ�̯");
			return 0;
		}

		// ��Ҫ���ж��Ƿ���ҹ�30��
		if( 30 > CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level())
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ҫ�ȼ�30������ܰ�̯");
			return 0;
		}

		// �����ڷǰ�̯״̬�����ύ��̯�����������
		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level()>=30)
		{
			CGStallApply msg;
			CNetManager::GetMe()->SendPacket(&msg);
		}
		else
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"30����ſ��԰�̯");
		}

	}

	return 0;
}

//--------------------------------------------------------
// �����Ʒ����ʹ�õĽӿں���
INT PlayerPackage::GetSplitSum(LuaPlus::LuaState* state)
{
	state->PushInteger(CDataPool::GetMe()->Split_GetSum());
	return 1;
}

// ������ȷ����ֺ�Ĵ���
INT PlayerPackage::SplitItem(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	//������
	if (!(args[2].IsInteger()))
	{
		TDThrow("LUA: PlayerPackage::SplitItem Wrong Param1");
	}
	INT nNumber = args[2].GetInteger();

	CDataPool::GetMe()->Split_SetNumber(nNumber);


	CGSplitItem msg;
	if( CDataPool::GetMe()->Split_GetItemConta() == tObject_Item::IO_MYSELF_PACKET )
	{
		msg.SetContainer(CGSplitItem::BAG_CON);
	}
	else if(CDataPool::GetMe()->Split_GetItemConta() == tObject_Item::IO_MYSELF_BANK )
	{
		msg.SetContainer(CGSplitItem::BANK_CON);
	}
	msg.SetNumber(nNumber);
	msg.SetPosition(CDataPool::GetMe()->Split_GetItemPos());

	CNetManager::GetMe()->SendPacket(&msg);
	return 0;
}

//��õȴ������Ʒ������
INT	PlayerPackage::GetSplitName(LuaPlus::LuaState* state)
{
	INT nCon = CDataPool::GetMe()->Split_GetItemConta();
	INT nPos = CDataPool::GetMe()->Split_GetItemPos();

	if(nCon == -1 || nPos == -1)
	{
		state->PushString("");
		return 1;
	}
	
	tObject_Item* pItem;

	switch(nCon)
	{
	case tObject_Item::IO_MYSELF_PACKET:
		{
			pItem = CDataPool::GetMe()->UserBag_GetItem(nPos);
		}
		break;
	case tObject_Item::IO_MYSELF_BANK:
		{
			pItem = CDataPool::GetMe()->UserBank_GetItem(nPos);
		}
		break;
	default:
		break;
	}

	if(pItem)
		state->PushString(pItem->GetName());
	else
		state->PushString("");

	return 1;
}

// ȡ����Ʒ���
INT PlayerPackage::CancelSplitItem(LuaPlus::LuaState* state)
{
	INT nTypeOwner	=	CDataPool::GetMe()->Split_GetItemConta();
	INT	nItemPos	=	CDataPool::GetMe()->Split_GetItemPos();

	switch( nTypeOwner ) 
	{
	case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}
		break;
	case tObject_Item::IO_MYSELF_BANK:					// ��ҵ�����
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBank_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANK);
		}
		break;
	case tObject_Item::IO_MYSELF_EQUIP:					// ����������ϵ�װ��
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
		}
		break;
	default:
		break;
	}

	return 0;
}

//������Ʒ
INT PlayerPackage::PackUpPacket(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	//������
	if (!(args[2].IsInteger()))
	{
		TDThrow("LUA: PlayerPackage::PackUpPacket Wrong Param1");
	}
	INT nIndex = args[2].GetInteger();

	CGPackUpPacket msg;
	msg.SetConIndex(nIndex);

	CNetManager::GetMe()->SendPacket(&msg);
	return 0;
}
//������Ʒ
INT PlayerPackage::OpenPetList(LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_OPEN_PET_LIST);

	return 0;
}
//����������
INT PlayerPackage::OpenLockFrame(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	//������
	if (!(args[2].IsInteger()))
	{
		TDThrow("LUA: PlayerPackage::OpenLockFrame Wrong Param1");
	}
	INT nIndex = args[2].GetInteger();

	CEventSystem::GetMe()->PushEvent(GE_OPEN_ITEM_COFFER, nIndex);
	return 0;
}
//������Ʒ�����
INT PlayerPackage::Lock(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	//������
	if (!(args[2].IsString()))
	{
		TDThrow("LUA: PlayerPackage::Lock[2] Wrong Param1");
	}
	if (!(args[3].IsInteger()))
	{
		TDThrow("LUA: PlayerPackage::Lock[3] Wrong Param1");
	}

	std::string str = args[2].GetString();
	INT nIndex = args[3].GetInteger();

	if( str == "lock" )
	{
		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsMinorPwdSetup())
		{
			//������Ʒ�ļ������ù���,����������Ʒ����Ϣ��������
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nIndex);
			if(!pItem)
			{
				return 0;
			}
			//��������Ʒ�ǲ����Ѿ�����
			if(pItem->GetByProtect())
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ʒ�Ѿ�����");
				return 0;
			}

			_ITEM_GUID Guid;
			tObject_Item::ITEM_GUID temp;
			temp.m_idUnion = 0;
			pItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
			Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
			Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
			Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

			CGAskLockObj msg;
			msg.SetLockObj(CGAskLockObj::LOCK_ITEM);
			msg.SetLockType(CGAskLockObj::OPR_LOCK);
			msg.SetItemGUID(Guid);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else
		{
			//_____________________________
		}
	}
	else	//�������
	{
		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsMinorPwdUnlocked())
		{
			//������Ʒ�ļ������ù���,����������Ʒ����Ϣ��������
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nIndex);
			if(!pItem)
			{
				return 0;
			}
			//��������Ʒ�ǲ����Ѿ�����
			if( !pItem->GetByProtect() )
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ʒû�м���");
				return 0;
			}

			_ITEM_GUID Guid;
			tObject_Item::ITEM_GUID temp;
			temp.m_idUnion = 0;
			pItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
			Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
			Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
			Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

			CGAskLockObj msg;
			msg.SetLockObj(CGAskLockObj::LOCK_ITEM);
			msg.SetLockType(CGAskLockObj::OPR_UNLOCK);
			msg.SetItemGUID(Guid);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else
		{
			//_________________________________
		}
	}
	return 0;
}
//�����Ʒ�Ƿ�����
INT PlayerPackage::IsLock(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (!(args[2].IsInteger()))
	{
		TDThrow("LUA: PlayerPackage::IsLock[2] Wrong Param1");
	}
	INT nIndex = args[2].GetInteger();
	tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nIndex);

	if(!pItem)
	{
		state->PushInteger(0);
		return 1;
	}
	if(pItem->GetByProtect())
	{
		state->PushInteger(1);
	}
	else
	{
		state->PushInteger(0);
	}

	return 1;
}

}
