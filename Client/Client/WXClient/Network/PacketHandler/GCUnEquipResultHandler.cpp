#include "StdAfx.h"
#include "GCUnEquipResult.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\WxRender\RenderSystem.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Object\Item\Obj_Item_Equip.h"
#include "..\..\DataPool\GMDP_Struct_CharacterData.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Sound\GMSoundSystem.h"

UINT GCUnEquipResultHandler::Execute( GCUnEquipResult* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	//AxTrace(0, 2, "GCUnEquipResultHandler::Execute");

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		UnEquipResultCode ret = (UnEquipResultCode)pPacket->getResult();
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

		switch(ret)
		{
		case UNEQUIP_SUCCESS:
			{
				//---------------------------------------------------
				//���ݳ�
				tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)pPacket->getEquipPoint());
				if(!pItem) return PACKET_EXE_CONTINUE;

				// �ı���Ʒ��id, �Ա�����µ�action ��ť
				((CObject_Item*)pItem)->ChangeClientID();

				CDataPool::GetMe()->UserBag_SetItem(pPacket->getBagIndex(), pItem, TRUE);

				CDataPool::GetMe()->UserEquip_SetItem((HUMAN_EQUIP)pPacket->getEquipPoint(), NULL, FALSE);

				//---------------------------------------------------
				//�߼���
				CObject_PlayerMySelf * pMySelf = CObjectManager::GetMe()->GetMySelf();
				pMySelf->UnEquipItem((HUMAN_EQUIP)pPacket->getEquipPoint());

				//---------------------------------------
				//ˢ�½�ɫ����
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_Equip(
					(HUMAN_EQUIP)pPacket->getEquipPoint(), -1);

				// �����������ϵ�װ����ActionSystem
				CActionSystem::GetMe()->UserEquip_Update();
				CActionSystem::GetMe()->UserBag_Update();

				// ֪ͨ�����¼�
				CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);
				CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

				CSoundSystemFMod::_PlayUISoundFunc(67);

			}
			break;

		case UNEQUIP_BAG_FULL:
			{
				if(bLog) 
				{
					ADDNEWDEBUGMSG(STRING("��������!"));
				}
				CSoundSystemFMod::_PlayUISoundFunc(96);
			}
			break;

		case UNEQUIP_HAS_ITEM:
			{
				if(bLog) 
				{
					ADDNEWDEBUGMSG(STRING("��λ���Ѿ�����Ʒ!"));
				}
				CSoundSystemFMod::_PlayUISoundFunc(96);
			}
			break;
		}


	}
	
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}