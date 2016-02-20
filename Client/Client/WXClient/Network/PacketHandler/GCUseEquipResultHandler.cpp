#include "StdAfx.h"
#include "GCUseEquipResult.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Item\Obj_Item_Equip.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Sound\GMSoundSystem.h"

using namespace Packets;
UINT GCUseEquipResultHandler::Execute(GCUseEquipResult* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	//AxTrace(0, 2, "GCUseEquipResultHandler::Execute");
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		UseEquipResultCode ret = (UseEquipResultCode)pPacket->getResult();
		switch(ret)
		{
		case USEEQUIP_SUCCESS:
			{
				//---------------------------------------------------------
				//UI���ݳ�
				CDataPool* pDataPool = CDataPool::GetMe();

				//�������װ��
				tObject_Item* pItemAtBag = pDataPool->UserBag_GetItem(pPacket->getBagIndex());
				if(!pItemAtBag) return PACKET_EXE_CONTINUE;

				//���ϵ�װ��
				tObject_Item* pItemAtUser= pDataPool->UserEquip_GetItem( (HUMAN_EQUIP)pPacket->getEquipPoint());

				//װ����Ϣ����
				if(pItemAtBag->GetItemClass() != ICLASS_EQUIP || 
					((CObject_Item_Equip*)pItemAtBag)->GetItemType() != pPacket->getEquipPoint()) return PACKET_EXE_CONTINUE;

				//���������Ʒת�ƣ���ı�ͻ���id���Ա�ʾ�ڿͻ����ǲ�ͬ����
				((CObject_Item*)pItemAtBag)->ChangeClientID();
				if(pItemAtUser)	((CObject_Item*)pItemAtUser)->ChangeClientID();
				
				//---------------------------------------
				//ˢ�½�ɫ����
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_Equip(
					((const CObject_Item_Equip*)pItemAtBag)->GetItemType(),
					pItemAtBag->GetIdTable());

				//ˢ�����ݳ�
				pDataPool->UserEquip_SetItem(((const CObject_Item_Equip*)pItemAtBag)->GetItemType(), (tObject_Item*)pItemAtBag, FALSE);

				//---------------------------------------
				//ˢ�°�������
				if(pItemAtUser)
					pItemAtUser->SetGUID(	pPacket->getItemId().m_World, 
											pPacket->getItemId().m_Server,
											pPacket->getItemId().m_Serial);
				pDataPool->UserBag_SetItem(pPacket->getBagIndex(), (tObject_Item*)pItemAtUser, FALSE);
				CActionSystem::GetMe()->UserBag_Update();

				//�����¼�
				CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED, pPacket->getBagIndex());

				CSoundSystemFMod::_PlayUISoundFunc(67);
			}
			break;

		case USEEQUIP_LEVEL_FAIL:
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "��Ʒ��װ��ȼ�������Ŀǰ�ĵȼ�");
			}
			break;

		case USEEQUIP_TYPE_FAIL:
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "װ�����ʹ���");
			}
			break;

		case USEEQUIP_JOB_FAIL:
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "ְҵ��ƥ��");
			}
			break;

		default:
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "δ֪����");
			}
			break;
		}
		
		// �����������ϵ�װ����ActionSystem
		CActionSystem::GetMe()->UserEquip_Update();

		// ֪ͨ�����¼�
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);
	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}