#include "stdafx.h"


#include "CGBankRemoveItem.h"
#include "GCBankRemoveItem.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"
#include "ItemOperator.h"
#include "HumanItemLogic.h"

UINT CGBankRemoveItemHandler::Execute( CGBankRemoveItem* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	UINT indexFrom = pPacket->GetIndexFrom();
	UINT indexTo   = pPacket->GetIndexTo();

	Item *pBankItem = HumanItemLogic::GetBankItem(pHuman, indexFrom);
	if(pBankItem->IsEmpty())
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGBankRemoveItemHandler: indexFrom=%d, indexTo=%d too many ask", indexFrom, indexTo ) ;
		return PACKET_EXE_CONTINUE;
	}

	GCBankRemoveItem Msg;

	switch(pPacket->GetToType())
	{
	case 	CGBankRemoveItem::EQUIP_POS:
		{
			pHuman->GetDB()->OverWriteBank2Equip((HUMAN_EQUIP)indexTo, indexFrom);
			Msg.SetToType(GCBankRemoveItem::EQUIP_POS);
			Msg.SetIndexFrom(indexFrom);
			Msg.SetIndexTo(indexTo);
		}
		break;
	case 	CGBankRemoveItem::BAG_POS:		//Bank->Bag
		{

			//�Զ������ո�
			if(indexTo == CGBankRemoveItem::AUTO_POS_BAG)
			{
				//��֤λ�����Ƿ��ж���
				if(ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indexFrom)))
				{
					ItemContainer*	pBagContainer =
						HumanItemLogic::GetItemContain(pHuman,HumanItemLogic::GetBankItem(pHuman, indexFrom)->GetItemTableIndex());

					ItemContainer* pBankContainer = pHuman->GetBankContain();

					//HumanItemLogic::RecieveItemToBag(pHuman, pBankContainer, indexFrom);

					INT result 	= 
							g_ItemOperator.MoveItem(pBankContainer,
							(UINT)indexFrom,
							pBagContainer
							);

					if(result>=0)
					{
						Msg.SetToType(GCBankRemoveItem::BAG_POS);
						Msg.SetIndexFrom(indexFrom);
						Msg.SetIndexTo(pBagContainer->ConIndex2BagIndex(result));
						Msg.SetOperateType(GCBankRemoveItem::OPERATE_MOVE);
					}
					else
					{
						Msg.SetToType(GCBankRemoveItem::ERROR_POS);
					}
				}
				else
				{
					Msg.SetToType(GCBankRemoveItem::ERROR_POS);
				}
			}

			//��֤λ�õĺϷ���
			else if( indexTo>=0
				&& indexTo<MAX_BAG_SIZE 
				&& indexFrom>=0 
				&& indexFrom<(UINT)(pHuman->__GetBankEndIndex()) 
				)
			{
				//��֤λ�����Ƿ��ж���
				if(ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indexFrom)))
				{
					//��֤�����Ʒ�ǲ����ܹ�����ָ����λ�ã�
					ItemContainer* pBankContainer = pHuman->GetBankContain();
					Item *pItem = pBankContainer->GetItem(indexFrom); 

					ItemContainer *pHumanCon = HumanItemLogic::GetItemContain(pHuman, pItem->GetItemTableIndex());
					if( !pHumanCon )
					{
						//���ش�����Ϣ
						Msg.SetToType(GCBankRemoveItem::ERROR_POS);
						break;
					}

					if( !pHumanCon->IsInContainer(indexTo) )
					{
						//���ش�����Ϣ
						Msg.SetToType(GCBankRemoveItem::ERROR_POS);
						break;
					}
					

					if(ITEMREFPTRISVALID(HumanItemLogic::GetItem(pHuman, indexTo)))
					{
						ItemContainer*	pBagContainer =
							HumanItemLogic::GetContainer(pHuman,indexTo);

						//ItemContainer* pBankContainer = pHuman->GetBankContain();

						g_ItemOperator.ExchangeItem(pBagContainer,
							pBagContainer->BagIndex2ConIndex(indexTo),
							pBankContainer,
							(UINT)indexFrom);

						Msg.SetOperateType(GCBankRemoveItem::OPERATE_SWAP); // ������Ʒ
					}
					else
					{
						ItemContainer*	pBagContainer =
							HumanItemLogic::GetContainer(pHuman,indexTo);

						//ItemContainer* pBankContainer = pHuman->GetBankContain();

						g_ItemOperator.MoveItem(pBankContainer,
							(UINT)indexFrom,
							pBagContainer,
							pBagContainer->BagIndex2ConIndex(indexTo)
							);
						
						Msg.SetOperateType(GCBankRemoveItem::OPERATE_MOVE);
					}

					//pHuman->GetDB()->SwapBankWithBag(indexTo, indexFrom);
					Msg.SetToType(GCBankRemoveItem::BAG_POS);
					Msg.SetIndexFrom(indexFrom);
					Msg.SetIndexTo(indexTo);
				}
				else
				{
					Msg.SetToType(GCBankRemoveItem::ERROR_POS);
				}
			}
			else
			{
				Msg.SetToType(GCBankRemoveItem::ERROR_POS);
			}
			
		}
		break;
	default:
		break;
	}
	pGamePlayer->SendPacket(&Msg);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGBankRemoveItemHandler: indexFrom=%d, indexTo=%d", indexFrom, indexTo ) ;

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
