#include "stdafx.h"


#include "CGBankAddItem.h"
#include "GCBankAddItem.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"
#include "ItemOperator.h"
#include "HumanItemLogic.h"

#include "GCBankItemInfo.h"
#include "GCItemInfo.h"

UINT CGBankAddItemHandler::Execute( CGBankAddItem* pPacket, Player* pPlayer )
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

	GCBankAddItem Msg;

	switch(pPacket->GetFromType())
	{
	case CGBankAddItem::EQUIP_POS:
		{
			pHuman->GetDB()->OverWriteEquip2Bank((HUMAN_EQUIP)indexFrom, indexTo);
			Msg.SetFromType(GCBankAddItem::EQUIP_POS);
			Msg.SetIndexFrom(indexFrom);
			Msg.SetIndexTo(indexTo);
		}
		break;
	case CGBankAddItem::BAG_POS:		//Bag->Bank
		{
			ItemContainer *pBankContainer  = pHuman->GetBankContain();
			ItemContainer *pBagContainer = HumanItemLogic::GetContainer(pHuman,indexFrom);

			Item *pBagItem  = pBagContainer->GetItem( pBagContainer->BagIndex2ConIndex(indexFrom) );
			//��ֹ�������
			if(pBagItem->IsEmpty())
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGBankAddItemHandler too many click") ;
				return PACKET_EXE_CONTINUE;
			}
			//Item *pBankItem = pBankContainer->GetItem( indexTo );

			//�ϲ���־
			INT	 bSplice = 0;
			INT  nSpliceTo = -1;
			UINT TempIndexTo = indexTo;

			//�Զ������ո�������1
			switch(TempIndexTo)
			{
			case CGBankAddItem::AUTO_POS_BOX1:
				{
					//�Ȳ�ѯ�ǲ����п��Ժϲ�����Ʒ����ֻ�ڱ��������У�
					if( pBagItem->IsCanLay() )
					{
						for( INT i=RENTBOX1_START_INDEX; i<RENTBOX2_START_INDEX; i++ )
						{
							Item *pBankItem = pBankContainer->GetItem(i);
							
							// ��֤�Ƿ���Ե��ӳɹ�
							if( pBankItem->GetItemTableIndex() == pBagItem->GetItemTableIndex() && 
								pBankItem->GetLayedNum() + pBagItem->GetLayedNum() <= pBankItem->GetMaxLayedNum() )
							{
								bSplice = 1;
								nSpliceTo = i;
								indexTo = i;
								break;
							}
						}
					}
					//û�п��Ե��ӵ���Ʒ����Ѱ��һ���ո�
					BYTE	indextemp = RENTBOX1_START_INDEX;
					if(0 == bSplice)
					{
						while( ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indextemp))  
							&&  indextemp < RENTBOX2_START_INDEX
							)
						{
							indextemp++;
						}

						if(indextemp == RENTBOX2_START_INDEX)
						{
							indexTo = indextemp-1;
						}
						else
						{
							indexTo = indextemp;
						}
					}
				}
				break;

			case CGBankAddItem::AUTO_POS_BOX2:
				{
					//�Ȳ�ѯ�ǲ����п��Ժϲ�����Ʒ����ֻ�ڱ��������У�
					if( pBagItem->IsCanLay() )
					{
						for( INT i=RENTBOX2_START_INDEX; i<RENTBOX3_START_INDEX; i++ )
						{
							Item *pBankItem = pBankContainer->GetItem(i);
							
							// ��֤�Ƿ���Ե��ӳɹ�
							if( pBankItem->GetItemTableIndex() == pBagItem->GetItemTableIndex() && 
								pBankItem->GetLayedNum() + pBagItem->GetLayedNum() <= pBankItem->GetMaxLayedNum() )
							{
								bSplice = 1;
								nSpliceTo = i;
								indexTo = i;
								break;
							}
						}
					}
					//û�п��Ե��ӵ���Ʒ����Ѱ��һ���ո�
					BYTE	indextemp = RENTBOX2_START_INDEX;
					if(0 == bSplice)
					{
						while( ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indextemp)) 
							&&  indextemp < RENTBOX3_START_INDEX
							)
						{
							indextemp++;
						}

						if(indextemp == RENTBOX3_START_INDEX)
						{
							indexTo = indextemp-1;
						}
						else
						{
							indexTo = indextemp;
						}
					}
				}
				break;

			case CGBankAddItem::AUTO_POS_BOX3:
				{
					//�Ȳ�ѯ�ǲ����п��Ժϲ�����Ʒ����ֻ�ڱ��������У�
					if( pBagItem->IsCanLay() )
					{
						for( INT i=RENTBOX3_START_INDEX; i<RENTBOX4_START_INDEX; i++ )
						{
							Item *pBankItem = pBankContainer->GetItem(i);
							
							// ��֤�Ƿ���Ե��ӳɹ�
							if( pBankItem->GetItemTableIndex() == pBagItem->GetItemTableIndex() && 
								pBankItem->GetLayedNum() + pBagItem->GetLayedNum() <= pBankItem->GetMaxLayedNum() )
							{
								bSplice = 1;
								nSpliceTo = i;
								indexTo = i;
								break;
							}
						}
					}
					//û�п��Ե��ӵ���Ʒ����Ѱ��һ���ո�
					BYTE	indextemp = RENTBOX3_START_INDEX;
					if(0 == bSplice)
					{
						while( ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indextemp)) 
							&&  indextemp < RENTBOX4_START_INDEX
							)
						{
							indextemp++;
						}

						if(indextemp == RENTBOX4_START_INDEX)
						{
							indexTo = indextemp-1;
						}
						else
						{
							indexTo = indextemp;
						}
					}
				}
				break;

			case CGBankAddItem::AUTO_POS_BOX4:
				{
					//�Ȳ�ѯ�ǲ����п��Ժϲ�����Ʒ����ֻ�ڱ��������У�
					if( pBagItem->IsCanLay() )
					{
						for( INT i=RENTBOX4_START_INDEX; i<RENTBOX5_START_INDEX; i++ )
						{
							Item *pBankItem = pBankContainer->GetItem(i);
							
							// ��֤�Ƿ���Ե��ӳɹ�
							if( pBankItem->GetItemTableIndex() == pBagItem->GetItemTableIndex() && 
								pBankItem->GetLayedNum() + pBagItem->GetLayedNum() <= pBankItem->GetMaxLayedNum() )
							{
								bSplice = 1;
								nSpliceTo = i;
								indexTo = i;
								break;
							}
						}
					}
					//û�п��Ե��ӵ���Ʒ����Ѱ��һ���ո�
					BYTE	indextemp = RENTBOX4_START_INDEX;
					if(0 == bSplice)
					{
						while( ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indextemp)) 
							&&  indextemp < RENTBOX5_START_INDEX
							)
						{
							indextemp++;
						}

						if(indextemp == RENTBOX5_START_INDEX)
						{
							indexTo = indextemp-1;
						}
						else
						{
							indexTo = indextemp;
						}
					}
				}
				break;

			case CGBankAddItem::AUTO_POS_BOX5:
				{
					//�Ȳ�ѯ�ǲ����п��Ժϲ�����Ʒ����ֻ�ڱ��������У�
					if( pBagItem->IsCanLay() )
					{
						for( INT i=RENTBOX5_START_INDEX; i<MAX_BANK_SIZE; i++ )
						{
							Item *pBankItem = pBankContainer->GetItem(i);
							
							// ��֤�Ƿ���Ե��ӳɹ�
							if( pBankItem->GetItemTableIndex() == pBagItem->GetItemTableIndex() && 
								pBankItem->GetLayedNum() + pBagItem->GetLayedNum() <= pBankItem->GetMaxLayedNum() )
							{
								bSplice = 1;
								nSpliceTo = i;
								indexTo = i;
								break;
							}
						}
					}
					//û�п��Ե��ӵ���Ʒ����Ѱ��һ���ո�
					BYTE	indextemp = RENTBOX5_START_INDEX;
					if(0 == bSplice)
					{
						while( ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indextemp)) 
							&&  indextemp < MAX_BANK_SIZE
							)
						{
							indextemp++;
						}

						if(indextemp == MAX_BANK_SIZE)
						{
							indexTo = indextemp-1;
						}
						else
						{
							indexTo = indextemp;
						}
					}
				}
				break;
			//���ֱ��ѡ����Ŀ���ǲ��ǿ��Ժϲ�
			default:
				{
					if( pBagItem->IsCanLay() )
					{
						Item *pItemFrom = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(indexFrom));
						Item *pItemTo   = pBankContainer->GetItem(indexTo);
						if( pItemFrom->GetItemTableIndex() == pItemTo->GetItemTableIndex() )
						{
							bSplice = 2;
							nSpliceTo = indexTo;
						}
					}
				}
				break;
			}

			//��֤λ�õĺϷ���
			if( indexFrom>=0 
				&& indexFrom<MAX_BAG_SIZE 
				&& indexTo>=0 
				&& (indexTo<(UINT)(pHuman->__GetBankEndIndex()) || (indexTo>=251 && indexTo<=255) )
				) 				
			{
				if(1 == bSplice)	//ִ�кϲ�
				{
					//��֤λ�����Ƿ��ж���
					if(ITEMREFPTRISVALID(HumanItemLogic::GetItem(pHuman, indexFrom)))
					{
						if(ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, nSpliceTo)))
						{
							g_ItemOperator.SpliceItem(pBagContainer,
								(UINT)pBagContainer->BagIndex2ConIndex(indexFrom),
								pBankContainer,
								(UINT)nSpliceTo);

							Item *pItemBag = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(indexFrom));
							Item *pItemBank= pBankContainer->GetItem(nSpliceTo);

							//��һ��֪ͨ�������е���Ϣ
							GCBankItemInfo MsgBankItemInfo;
							MsgBankItemInfo.setBankIndex((WORD)nSpliceTo);
							MsgBankItemInfo.setIsNull(pItemBank->IsEmpty());
							pItemBank->SaveValueTo(MsgBankItemInfo.getItem());
							pGamePlayer->SendPacket( &MsgBankItemInfo );

							//����һ��֪ͨ�������µ���Ϣ
							GCItemInfo MsgBagItemInfo;
							MsgBagItemInfo.setID((WORD)indexFrom);
							MsgBagItemInfo.setIsNull(pItemBag->IsEmpty());
							pItemBag->SaveValueTo(MsgBagItemInfo.getItem());
							pGamePlayer->SendPacket( &MsgBagItemInfo );
						}
					}
					g_pLog->FastSaveLog( LOG_FILE_1, "CGBankAddItemHandler Empty Item indexFrom = %d", indexFrom) ;
					return PACKET_EXE_CONTINUE;
				}
				else if(2 == bSplice)
				{
					//��֤λ�����Ƿ��ж���
					if(ITEMREFPTRISVALID(HumanItemLogic::GetItem(pHuman, indexFrom)))
					{
						if(ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, nSpliceTo)))
						{
							g_ItemOperator.MoveSpliceItem(pBagContainer,
								(UINT)pBagContainer->BagIndex2ConIndex(indexFrom),
								pBankContainer,
								(UINT)nSpliceTo);

							Item *pItemBag = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(indexFrom));
							Item *pItemBank= pBankContainer->GetItem(nSpliceTo);

							//��һ��֪ͨ�������е���Ϣ
							GCBankItemInfo MsgBankItemInfo;
							MsgBankItemInfo.setBankIndex((WORD)nSpliceTo);
							MsgBankItemInfo.setIsNull(pItemBank->IsEmpty());
							pItemBank->SaveValueTo(MsgBankItemInfo.getItem());
							pGamePlayer->SendPacket( &MsgBankItemInfo );

							//����һ��֪ͨ�������µ���Ϣ
							GCItemInfo MsgBagItemInfo;
							MsgBagItemInfo.setID((WORD)indexFrom);
							MsgBagItemInfo.setIsNull(pItemBag->IsEmpty());
							pItemBag->SaveValueTo(MsgBagItemInfo.getItem());
							pGamePlayer->SendPacket( &MsgBagItemInfo );
						}
					}
					g_pLog->FastSaveLog( LOG_FILE_1, "CGBankAddItemHandler Empty Item indexFrom = %d", indexFrom) ;
					return PACKET_EXE_CONTINUE;
				}
				else				//��ִ�кϲ�
				{
					//��֤λ�����Ƿ��ж���
					if(ITEMREFPTRISVALID(HumanItemLogic::GetItem(pHuman, indexFrom)))
					{
						if(ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indexTo))) //ִ���˽���
						{
							//�ж������ڵ������Ʒ�ǲ����ܹ����뱳����
							ItemContainer* pBankContainer = pHuman->GetBankContain();
							Item *pItem = pBankContainer->GetItem(indexTo); 

							ItemContainer *pHumanCon = HumanItemLogic::GetItemContain(pHuman, pItem->GetItemTableIndex());
							if( !pHumanCon )
							{
								//���ش�����Ϣ
								Msg.SetFromType(GCBankAddItem::ERROR_POS);
								break;
							}

							if( !pHumanCon->IsInContainer(indexFrom) )
							{
								//���ش�����Ϣ
								Msg.SetFromType(GCBankAddItem::ERROR_POS);
								break;
							}

							g_ItemOperator.ExchangeItem(pBagContainer,
								pBagContainer->BagIndex2ConIndex(indexFrom),
								pBankContainer,
								(UINT)indexTo);

							//ִ���˽���
							Msg.SetOperateType(GCBankAddItem::OPERATE_SWAP);
						}
						else //ִ�����ƶ�
						{
							g_ItemOperator.MoveItem(pBagContainer,
								pBagContainer->BagIndex2ConIndex(indexFrom),
								pBankContainer,
								(UINT)indexTo);

							//ִ�����ƶ�
							Msg.SetOperateType(GCBankAddItem::OPERATE_MOVE);
						}
						Msg.SetFromType(GCBankAddItem::BAG_POS);
						Msg.SetIndexFrom(indexFrom);
						Msg.SetIndexTo(indexTo);
						
					}
					else
					{
						Msg.SetFromType(GCBankAddItem::ERROR_POS);
					}
				}
			}
			else	//���ݲ��Ϸ�
			{
				Msg.SetFromType(GCBankAddItem::ERROR_POS);
			}
		}
		break;
	default:
		break;
	}

	pGamePlayer->SendPacket( &Msg ) ;
	g_pLog->FastSaveLog( LOG_FILE_1, "CGBankAddItemHandler: indexFrom=%d, indexTo=%d", indexFrom, indexTo ) ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
