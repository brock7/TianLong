#include "stdafx.h"
/*
�ͻ���������Ʒ�ϼ�
*/

#include "CGPlayerShopOnSale.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCPlayerShopOnSale.h"
#include "GCPlayerShopError.h"
#include "RecordOpt.h"

UINT CGPlayerShopOnSaleHandler::Execute( CGPlayerShopOnSale* pPacket, Player* pPlayer )
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

	_PLAYERSHOP_GUID nShopID	=	pPacket->GetShopID();			//�̵�ID
	BYTE		nStallIndex		=	pPacket->GetStallIndex();		//��̨ID
	_ITEM_GUID	ItemGuid		=	pPacket->GetObjGUID();			//��ƷGUID
	UINT		uSerial			=	pPacket->GetSerial();			//��ǰ���к�
	UINT		uPrice			=	pPacket->GetPrice();			//�۸�
	BYTE		bIsOnSale		=	pPacket->GetIsOnSale();			//�Ƿ��ϼ�
	PET_GUID_t	PetGuid			=	pPacket->GetPetGuid();			//����Guid
	BYTE		nShopSerial		=	pPacket->GetShopSerial();		//�̵����к�

	UINT		uFinalSerial	=	0;

	GCPlayerShopError	MsgError;
	GCPlayerShopOnSale	MsgToClient;

	PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
	PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
	Assert(pPlayerShop);

	if(pPlayerShop->GetShopStatus() == STATUS_PLAYER_SHOP_ON_SALE)
	{//�̵�δ����
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopOnSaleHandler::Name=%s shop close"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;
	}

	//�ǲ����Լ��ĵ�
	BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;

	//�ǲ����Լ����Թ���ĵ�
	BOOL bCanManager = pPlayerShop->IsPartner(pHuman->GetGUID());

	if(bIsMine == FALSE && bCanManager == FALSE)
	{
		//log...
		return PACKET_EXE_CONTINUE;
	}

	if(nShopSerial != pPlayerShop->GetSerial())
	{
		MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_SALE_OUT_SERIAL_IS_CHANGED);
		pGamePlayer->SendPacket(&MsgError);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGItemSynchHandler::Name=%s Serials had changed"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;
	}

	PlayerStallBox* pStallBox		= pPlayerShop->GetPlayerStallBoxByIndex(nStallIndex);
	Assert(pStallBox);
	
	ItemContainer*	pStallContainer = NULL;
	INT	IndexInStall = 0;
	if(PetGuid.IsNull())
	{
		pStallContainer = pStallBox->GetContainer();
		IndexInStall = pStallContainer->GetIndexByGUID(&ItemGuid);
	}
	else
	{
		pStallContainer = pStallBox->GetPetContainer();
		IndexInStall = pStallContainer->GetIndexByGUID(&PetGuid);
	}

	if(IndexInStall<0)
	{
		MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
		pGamePlayer->SendPacket(&MsgError);
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopOnSaleHandler::ObjName=%s, ERR_NEED_NEW_COPY: IndexInStall = %d"
			,pHuman->GetName(), IndexInStall) ;
		return PACKET_EXE_CONTINUE;
	}

	if( pStallBox->GetSerialByIndex(IndexInStall) != uSerial)
	{
		MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
		pGamePlayer->SendPacket(&MsgError);
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopOnSaleHandler::ObjName=%s, ERR_NEED_NEW_COPY: Serial = %d, BoxSerial = %d"
			,pHuman->GetName(), uSerial, pStallBox->GetSerialByIndex(IndexInStall)) ;
		return PACKET_EXE_CONTINUE;
	}

	if(bIsOnSale)
	{//��Ʒ�ϼ�
		pStallBox->SetCanSale(IndexInStall, TRUE);
		pStallBox->SetPriceByIndex(IndexInStall, uPrice);
		uFinalSerial	=	pStallBox->IncSerialByIndex(IndexInStall);

		Item* pItem = pStallContainer->GetItem(IndexInStall);
		if(PetGuid.IsNull())
		{
			//�ϼܣ�AAA�ϼ���BBBC�������Ϊ�����У���������ͭ��AAAΪ����������BBBΪ��Ʒ��CΪ������
			RecordOpt::Excute(REC_ONSALEITEM, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)(pItem->GetItemIndex()), (INT)(pItem->GetLayedNum()), (INT)uPrice);
		}
		else
		{
			//�ϼܣ�AAA�ϼ���BBBC�������Ϊ�����У���������ͭ��AAAΪ����������BBBΪ��Ʒ��CΪ������
			RecordOpt::Excute(REC_ONSALEPET, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (CHAR*)(pItem->GetName()), (INT)(1), (INT)uPrice);
		}

		MsgToClient.SetShopID(nShopID);
		MsgToClient.SetStallIndex(nStallIndex);
		MsgToClient.SetObjGUID(ItemGuid);
		MsgToClient.SetPetGuid(PetGuid);
		MsgToClient.SetSerial(uFinalSerial);
		MsgToClient.SetPrice(uPrice);
		MsgToClient.SetIsOnSale(TRUE);
		MsgToClient.SetShopSerial(pPlayerShop->IncSerial());
	}
	else
	{//��Ʒ�¼�
		pStallBox->SetCanSale(IndexInStall, FALSE);
		UINT uOldPrice = pStallBox->GetPriceByIndex(IndexInStall);
		pStallBox->SetPriceByIndex(IndexInStall, 0);
		uFinalSerial	=	pStallBox->IncSerialByIndex(IndexInStall);

		Item* pItem = pStallContainer->GetItem(IndexInStall);
		if(PetGuid.IsNull())
		{
			//�ϼܣ�AAA�ϼ���BBBC�������Ϊ�����У���������ͭ��AAAΪ����������BBBΪ��Ʒ��CΪ������
			RecordOpt::Excute(REC_OFFSALEITEM, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)(pItem->GetItemIndex()), (INT)(pItem->GetLayedNum()), (INT)uOldPrice);
		}
		else
		{
			//�ϼܣ�AAA�ϼ���BBBC�������Ϊ�����У���������ͭ��AAAΪ����������BBBΪ��Ʒ��CΪ������
			RecordOpt::Excute(REC_OFFSALEPET, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (CHAR*)(pItem->GetName()), (INT)(1), (INT)uOldPrice);
		}
		
		MsgToClient.SetShopID(nShopID);
		MsgToClient.SetStallIndex(nStallIndex);
		MsgToClient.SetObjGUID(ItemGuid);
		MsgToClient.SetPetGuid(PetGuid);
		MsgToClient.SetSerial(uFinalSerial);
		MsgToClient.SetPrice(0);
		MsgToClient.SetIsOnSale(FALSE);
		MsgToClient.SetShopSerial(pPlayerShop->IncSerial());
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopOnSaleHandler::ObjName=%s"
		,pHuman->GetName());

	pGamePlayer->SendPacket(&MsgToClient);

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
