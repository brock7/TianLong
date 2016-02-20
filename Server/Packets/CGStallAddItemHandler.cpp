#include "stdafx.h"

/*
�ͻ�����̯λ��Ŷ���
*/
#include "CGStallAddItem.h"
#include "GCStallAddItem.h"

#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

#include "ItemOperator.h"
#include "HumanItemLogic.h"

UINT CGStallAddItemHandler::Execute( CGStallAddItem* pPacket, Player* pPlayer )
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

	//����
	_ITEM_GUID	ItemGuid = pPacket->GetObjGUID();
	UINT		ItemPrice = pPacket->GetPrice();
	BYTE		FromType = pPacket->GetFromType();
	PET_GUID_t	PetGuid  = pPacket->GetPetGUID();

	if(pHuman->m_StallBox.GetStallStatus() != ServerStallBox::STALL_OPEN)
	{
		GCStallError	Msg;
		Msg.SetID(STALL_MSG::ERR_ILLEGAL);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallAddItemHandler::ObjID=%d ERR_ILLEGAL: != ServerStallBox::STALL_OPEN", pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;
	}
	
	ItemContainer*		pStallContainer		= pHuman->m_StallBox.GetContainer();
	ItemContainer*		pStallPetContainer	= pHuman->m_StallBox.GetPetContainer();

	GCStallError		MsgError;
	GCStallAddItem		MsgAddItem;

	switch(FromType)
	{
	case STALL_MSG::POS_BAG :
		{
			//ItemContainer* pMatContainer	= pHuman->GetMatContain();
			ItemContainer* pBaseContainer	= pHuman->GetBaseContain();
			//ItemContainer* pBagContainer	=	NULL;

			//if(pMatContainer->GetIndexByGUID(&ItemGuid) >= 0)
			//{
			//	pBagContainer = pMatContainer;
			//}
			//else if(pBaseContainer->GetIndexByGUID(&ItemGuid) >=0 )
			//{
			//	pBagContainer = pBaseContainer;
			//}
			//else
			//{//guid�Ƿ�
			//	Assert(0);
			//	GCStallError	Msg;
			//	Msg.SetID(STALL_MSG::ERR_ILLEGAL);
			//	pGamePlayer->SendPacket(&Msg);
			//	g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallAddItemHandler::ObjID=%d, ERR_ILLEGAL:World = %d, Server = %d, Serial = %d "
			//		,pHuman->GetID(), ItemGuid.m_World, ItemGuid.m_Server, ItemGuid.m_Serial) ;
			//	return PACKET_EXE_CONTINUE ;
			//}

			INT	IndexInBag = pBaseContainer->GetIndexByGUID(&ItemGuid);
			
			//�Ȳ�һ��̯λ�����ǲ����Ѿ��и���Ʒ��
			for(INT i = 0; i<STALL_BOX_SIZE; i++)
			{
				if(pStallContainer->GetItem(i)->GetGUID() == ItemGuid )
				{//��Ʒ�Ѿ���t̯λ���У���������Ϣ
					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallAddItemHandler::ObjID=%d, Already in box "
						,pHuman->GetID()) ;
					return PACKET_EXE_CONTINUE ;
				}
			}

			//������̯λ��
			//�Զ��Ҹ�
			INT result = 
				g_ItemOperator.CopyItem
				(
				pBaseContainer,
				IndexInBag,
				pStallContainer
				);

			if(result>=0)
			{//�����ɹ���
				//������Ϣ��˫���ͻ���
				Item* pIt = pStallContainer->GetItem(result);
				if(pIt->IsEmpty() == FALSE)
				{
					//����������Ʒ
					g_ItemOperator.LockItem( pBaseContainer, IndexInBag );

					//���ü۸�
					pHuman->m_StallBox.SetPriceByIndex(result, ItemPrice);

					//���кŵ���
					pHuman->m_StallBox.IncSerialByIndex(result);

					//������Ϣ���ͻ���
					MsgAddItem.SetFromType( STALL_MSG::POS_BAG );
					MsgAddItem.SetToIndex(result);
					MsgAddItem.SetPrice(ItemPrice);
					MsgAddItem.SetObjGUID(ItemGuid);
					MsgAddItem.SetSerial(pHuman->m_StallBox.GetSerialByIndex(result));

					pGamePlayer->SendPacket(&MsgAddItem);
				}
				else
				{
					MsgError.SetID(STALL_MSG::ERR_ILLEGAL);
					pGamePlayer->SendPacket(&MsgError);
					pHuman->m_StallBox.CleanUp();
					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallAddItemHandler::ObjID=%d, ERR_ILLEGAL: pIt->IsEmpty() == FALSE"
						,pHuman->GetID()) ;
					return PACKET_EXE_CONTINUE ;
				}
			}
			else
			{//����ʧ��
				if(result == ITEMOE_DESTOPERATOR_FULL)
				{
					MsgError.SetID(STALL_MSG::ERR_NOT_ENOUGH_ROOM_IN_STALL);
					pGamePlayer->SendPacket(&MsgError);
					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallAddItemHandler::ObjID=%d, ERR_NOT_ENOUGH_ROOM_IN_STALL"
						,pHuman->GetID()) ;
					return PACKET_EXE_CONTINUE ;
				}
				else if(result == ITEMOE_SOUROPERATOR_LOCK)
				{
					MsgError.SetID(STALL_MSG::ERR_ALREADY_LOCK);
					pGamePlayer->SendPacket(&MsgError);
					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallAddItemHandler::ObjID=%d, ERR_ALREAD_LOCK"
						,pHuman->GetID()) ;
					return PACKET_EXE_CONTINUE ;
				}
			}

		}
		break;

	case STALL_MSG::POS_EQUIP :
		{
		}
		break;

	case STALL_MSG::POS_PET :
		{//�ӳ����б����ó����ﵽ���׺���
			ItemContainer*	pPetContainer = pHuman->GetPetContain();
			BYTE	FromIndex = pPetContainer->GetIndexByGUID( &PetGuid );

			//�Ȳ�һ�齻�׺����ǲ����Ѿ��и���Ʒ��
			for(INT i = 0; i<EXCHANGE_PET_BOX_SIZE; i++)
			{
				if(pStallPetContainer->GetItem(i)->GetPetGUID() == pPetContainer->GetItem(FromIndex)->GetPetGUID() )
				{//��Ʒ�Ѿ��ڽ��׺��У���������Ϣ
					return PACKET_EXE_CONTINUE ;
				}
			}

			//�Զ��Ҹ�
			INT result = 
				g_ItemOperator.CopyItem
				(
				pPetContainer,
				FromIndex,
				pStallPetContainer
				);

			if(result>=0)
			{//�����ɹ���
				//������Ϣ��˫���ͻ���
				Item* pIt = pStallPetContainer->GetItem(result);
				if(pIt->IsEmpty() == FALSE)
				{
					//����������Ʒ
					g_ItemOperator.LockItem( pPetContainer, FromIndex );

					//���ü۸�
					pHuman->m_StallBox.SetPetPriceByIndex(result, ItemPrice);

					//���кŵ���
					pHuman->m_StallBox.IncPetSerialByIndex(result);

					//������Ϣ���ͻ���
					MsgAddItem.SetFromType( STALL_MSG::POS_PET );
					MsgAddItem.SetToIndex(result);
					MsgAddItem.SetPrice(ItemPrice);
					MsgAddItem.SetPetGUID(PetGuid);
					MsgAddItem.SetSerial(pHuman->m_StallBox.GetPetSerialByIndex(result));

					pGamePlayer->SendPacket(&MsgAddItem);
				}
				else
				{
					MsgError.SetID(STALL_MSG::ERR_ILLEGAL);
					pGamePlayer->SendPacket(&MsgError);
					pHuman->m_StallBox.CleanUp();
					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallAddItemHandler::ObjName=%s, ERR_ILLEGAL: pIt->IsEmpty() == FALSE"
						,pHuman->GetName()) ;
					return PACKET_EXE_CONTINUE ;
				}
			}
			else
			{//����ʧ��
				if(result == ITEMOE_DESTOPERATOR_FULL)
				{
					MsgError.SetID(STALL_MSG::ERR_NOT_ENOUGH_ROOM_IN_STALL);
					pGamePlayer->SendPacket(&MsgError);
					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallAddItemHandler::ObjName=%d, ERR_NOT_ENOUGH_ROOM_IN_STALL"
						,pHuman->GetName()) ;
					return PACKET_EXE_CONTINUE ;
				}
				else if(result == ITEMOE_SOUROPERATOR_LOCK)
				{
					MsgError.SetID(STALL_MSG::ERR_ALREADY_LOCK);
					pGamePlayer->SendPacket(&MsgError);
					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallAddItemHandler::ObjName=%d, ERR_ALREAD_LOCK"
						,pHuman->GetName()) ;
					return PACKET_EXE_CONTINUE ;
				}
			}
		}
		break;

	default:
		break;
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGStallAddItemHandler::ObjName=%d"
		,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
