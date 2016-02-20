// GCShopRepairHandler.cpp
#include "stdafx.h"
#include "GCShopRepair.h"
#include "TDUISystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Object\Item\Obj_Item_Equip.h"
#include "..\..\Event\GMEventSystem.h"

#include "..\..\Sound\GMSoundSystem.h"

UINT GCShopRepairHandler::Execute( GCShopRepair* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
			//�Ѿ������˾�����
			if(CDataPool::GetMe()->Booth_IsClose())
				return PACKET_EXE_CONTINUE ;

			//��Ʒ����
			BOOL	RepairAll = (BOOL)pPacket->GetAll();
			if(RepairAll)
			{//ȫ���Ļ����Լ��ްɣ�һ�������ϵ�װ��
				for(INT i = 0; i<HEQUIP_NUMBER; i++)
				{
					tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)i);
					if(!pItem)
						continue;
					((CObject_Item_Equip*)pItem)->SetDur(((CObject_Item_Equip*)pItem)->GetMaxDur());
				}

				if(bLog)
				{
					STRING strTemp = COLORMSGFUNC("REPAIR_ALL_OK");
					ADDTALKMSG(strTemp);
					strTemp = NOCOLORMSGFUNC("REPAIR_OK");
					ADDNEWDEBUGMSG(strTemp);
				}
			}
			else
			{//�ô��ӵ��һ��һ���ޣ����������ϵ�Ҳ�����Ǳ������
				INT nNum = pPacket->GetReListNum();
				tObject_Item* pItem;
				
				for(INT i=0; i<nNum; i++)
				{
					INT itemindex = pPacket->GetReList()[i].BagIndex;
					BOOL bMsg = FALSE;

					switch(pPacket->GetReList()[i].IsIn)
					{
					case GCShopRepair::BAG:
						pItem = CDataPool::GetMe()->UserBag_GetItem(itemindex);
						if(!pItem)
							continue;
						((CObject_Item_Equip*)pItem)->SetDur(((CObject_Item_Equip*)pItem)->GetMaxDur());
						//ˢ�±���
						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED, ((CObject_Item_Equip*)pItem)->GetPosIndex());
						//֪ͨtooltip����
						CEventSystem::GetMe()->PushEvent(GE_UPDATE_SUPERTOOLTIP);
						bMsg = TRUE;
						break;
					case GCShopRepair::EQUIP:
						pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)itemindex);
						if(!pItem)
							continue;
						((CObject_Item_Equip*)pItem)->SetDur(((CObject_Item_Equip*)pItem)->GetMaxDur());
						bMsg = TRUE;
						break;
					case GCShopRepair::BOX:
						break;
					default:
						break;
					}

					if(bLog && pItem)
					{
						STRING strTemp = COLORMSGFUNC("REPAIR_ITEM", pItem->GetName());
						ADDTALKMSG(strTemp);
						strTemp = NOCOLORMSGFUNC("REPAIR_OK");
						ADDNEWDEBUGMSG(strTemp);
					}
				}
			}
			
			CSoundSystemFMod::_PlayUISoundFunc(64);
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
