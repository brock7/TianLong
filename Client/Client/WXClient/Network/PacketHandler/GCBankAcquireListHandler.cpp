#include"stdafx.h"

#include "GCBankAcquireList.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerOther.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "TDException.h"
UINT GCBankAcquireListHandler::Execute( GCBankAcquireList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			//���ԭ����Ʒ�б�
			CDataPool::GetMe()->UserBank_Clear();

			//���������Ǯ��
			INT	Money = pPacket->GetBankMoney();
			CDataPool::GetMe()->UserBank_SetBankMoney(Money);

			//��ǰ���еĴ�С
			INT	BankSize = pPacket->GetBankSize();
			CDataPool::GetMe()->UserBank_SetBankEndIndex(BankSize);

			GCBankAcquireList::_BANK_ITEM*	ItemList = pPacket->GetBankItemList();
			INT nNum = pPacket->GetItemNum();
			INT bankpos = 0;
			tObject_Item* pItem = NULL;

			//��ӵ����ݳ���
			for(INT i=0; i<nNum; i++)
			{
				bankpos = ItemList[i].bankindex;

				if(ItemList[i].isBlueEquip)
				{
					pItem = CObject_Item::NewItem( ItemList[i].item_data.m_ItemIndex);
					TDAssert(pItem);
					pItem->SetGUID(
						ItemList[i].item_data.m_ItemGUID.m_World, 
						ItemList[i].item_data.m_ItemGUID.m_Server,
						ItemList[i].item_data.m_ItemGUID.m_Serial);
					CDataPool::GetMe()->UserBank_SetItem(bankpos, pItem);
					CDataPool::GetMe()->UserBank_SetItemExtraInfo(bankpos, FALSE, &ItemList[i].item_data);
				}
				else
				{
					pItem = CObject_Item::NewItem( ItemList[i].item_guid);
					TDAssert(pItem);
					CDataPool::GetMe()->UserBank_SetItem(bankpos, pItem);
				}
				pItem->SetPosIndex(bankpos);
				pItem->SetNumber(ItemList[i].byNumber);
			}
			
			CActionSystem::GetMe()->UserBank_Update();

			INT nBankNpcId = CDataPool::GetMe()->UserBank_GetNpcId();
			CEventSystem::GetMe()->PushEvent(GE_TOGLE_BANK,nBankNpcId);
		}


		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
