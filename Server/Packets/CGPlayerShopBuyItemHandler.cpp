#include "stdafx.h"

#include "CGPlayerShopBuyItem.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCPlayerShopError.h"
#include "GCItemSynch.h"
#include "ItemOperator.h"
#include "HumanItemLogic.h"
#include "ItemSynchStructDefine.h"

#include "RecordOpt.h"
using namespace ITEM_SYNCH_STRUCT;

/*
10���Ϊ2%��
9���Ϊ4%��
8���Ϊ4%��
7���Ϊ5%��
6���Ϊ5%��
5���Ϊ6%��
4���Ϊ6%��
3���Ϊ7%��
2���Ϊ7%��
1���Ϊ8%��

*/
static double	g_ExchageTrade[10] = 
{
	0.08,
	0.07,
	0.07,
	0.06,
	0.06,
	0.05,
	0.05,
	0.04,
	0.04,
	0.02
};
UINT CGPlayerShopBuyItemHandler::Execute( CGPlayerShopBuyItem* pPacket, Player* pPlayer )
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

	_PLAYERSHOP_GUID		nShopID	=	pPacket->GetShopID();			//�̵�ID
	BYTE		nStallIndex			=	pPacket->GetStallIndex();		//��̨ID
	_ITEM_GUID	ItemGuid			=	pPacket->GetObjGUID();			//��ƷGUID
	PET_GUID_t	PetGuid				=	pPacket->GetPetGuid();			//����GUID
	UINT		uSerial				=	pPacket->GetSerial();			//��ǰ���к�

	GCPlayerShopError	MsgError;
	//������Ϣ���ͻ���
	GCItemSynch MsgToClient;
	UINT		uFinalIndex		= 0;
	UINT		uFinalSerial	= 0;

	PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
	PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
	Assert(pPlayerShop);

	if(pPlayerShop->GetShopStatus() == STATUS_PLAYER_SHOP_ON_SALE)
	{//�̵�δ����
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopBuyItemHandler::Name=%s shop close"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;
	}

	PlayerStallBox* pStallBox		= pPlayerShop->GetPlayerStallBoxByIndex(nStallIndex);
	Assert(pStallBox);

	if(PetGuid.IsNull())
	{//����Ʒ
		ItemContainer*	pStallContainer = pStallBox->GetContainer();

		if(pStallBox->GetStallStatus() != PLAYER_SHOP::STALL_OPEN)
		{//�����Ѿ��رմι�̨
			MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopBuyItemHandler::ObjName=%s, ERR_NEED_NEW_COPY: pStallBox->GetStallStatus() != PLAYER_SHOP::STALL_OPEN"
				,pHuman->GetName()) ;
			return PACKET_EXE_CONTINUE;
		}

		INT	IndexInStall = pStallContainer->GetIndexByGUID(&ItemGuid);
		if(IndexInStall<0)
		{//�����Ѿ��ı����Ʒ
			MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopBuyItemHandler::ObjName=%s, ERR_NEED_NEW_COPY: IndexInStall = %d"
				,pHuman->GetName(), IndexInStall) ;
			return PACKET_EXE_CONTINUE;
		}

		if(!pStallBox->CanSale(IndexInStall))
		{//����Ʒ�Ѿ��¼�
			MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopBuyItemHandler::ObjName=%s, ERR_NEED_NEW_COPY: IndexInStall = %d"
				,pHuman->GetName(), IndexInStall) ;
			return PACKET_EXE_CONTINUE;
		}

		if( pStallBox->GetSerialByIndex(IndexInStall) != uSerial)
		{//�۸��Ѿ��޸�
			MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopBuyItemHandler::ObjName=%s, ERR_NEED_NEW_COPY: Serial = %d, BoxSerial = %d"
				,pHuman->GetName(), uSerial, pStallBox->GetSerialByIndex(IndexInStall)) ;
			return PACKET_EXE_CONTINUE;
		}

		Item* pItem = pStallContainer->GetItem(IndexInStall);
		UINT	MessItemTBIndex =	pItem->GetItemTableIndex();
		UINT	MessItemNum     =	pItem->GetLayedNum();

		ItemContainer* pBagContainer = HumanItemLogic::GetItemContain(pHuman, pItem->GetItemTableIndex());
		
		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType			=	ITEM_PLAYERSHOP_EXCHANGE;
		ItemLogParam.ItemGuid		=	pItem->GetGUID();
		ItemLogParam.ItemType		=	pItem->GetItemTableIndex();

		//1.����ռ�
		_EXCHANGE_ITEM_LIST			ItemListOtToMe;
		ItemListOtToMe.Init();
		ItemListOtToMe.AddItem(pItem);
		if( FALSE == HumanItemLogic::CanReceiveExchangeItemList( pHuman, ItemListOtToMe) )
		{//�ռ䲻��
			MsgError.SetID(PLAYERSHOP_MSG::ERR_NOT_ENOUGH_ROOM);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopBuyItemHandler::ObjName=%s, ERR_NOT_ENOUGH_ROOM"
				,pHuman->GetName()) ;
			return PACKET_EXE_CONTINUE;
		}

		//2.�����Ǯ
		UINT	CurPrice = pStallBox->GetPriceByIndex(IndexInStall);
		if(pHuman->GetMoney()<CurPrice)
		{//��Ǯ����
			MsgError.SetID(PLAYERSHOP_MSG::ERR_NOT_ENOUGH_MONEY);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopBuyItemHandler::ObjName=%s, ERR_NOT_ENOUGH_MONEY"
				,pHuman->GetName()) ;
			return PACKET_EXE_CONTINUE;
		}

		//3.�ƶ���Ʒ
		//�Զ��Ҹ�,֧���Զ�����
		INT result = 
			g_ItemOperator.MoveItem
			(
			pStallContainer,
			IndexInStall,
			pBagContainer
			);

		if(result<0)
		{//����ʧ��
			MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopBuyItemHandler::ObjName=%s, ERR_ILLEGAL: IndexInStall = %d, result = %d "
				,pHuman->GetName(), IndexInStall, result) ;
			return PACKET_EXE_CONTINUE;
		}

		uFinalIndex	= pBagContainer->ConIndex2BagIndex(result);


		ItemLogParam.CharGUID		=	pPlayerShop->GetOwnerGuid();
		ItemLogParam.TargetGUID		=	pHuman->GetGUID();
		ItemLogParam.XPos			=	pHuman->getWorldPos()->m_fX;
		ItemLogParam.ZPos			=	pHuman->getWorldPos()->m_fZ;
		ItemLogParam.BagPos			=	uFinalIndex;
		ItemLogParam.SceneID		=	pHuman->getScene()->SceneID();
		SaveItemLog(&ItemLogParam);


		//4.��Ǯ
		FLOAT CurPriceTemp = (FLOAT)CurPrice;
		pHuman->SetMoney((UINT)(pHuman->GetMoney() - CurPriceTemp));
		Assert(pPlayerShop->GetNumStallOpened()>=1 && pPlayerShop->GetNumStallOpened()<=10);
		CurPriceTemp = (FLOAT)(CurPriceTemp * (1-g_ExchageTrade[pPlayerShop->GetNumStallOpened()-1]));

		MONEY_LOG_PARAM	MoneyLogParam;
		MoneyLogParam.CharGUID		=	pHuman->GetGUID();
		MoneyLogParam.TargetGUID	=	pPlayerShop->GetOwnerGuid();
		MoneyLogParam.OPType		=	MONEY_EXCHANGE_PLAYERSHOP_OUTCOME;	
		MoneyLogParam.Count			=	(INT)CurPriceTemp;
		MoneyLogParam.SceneID		=	pHuman->getScene()->SceneID();
		MoneyLogParam.XPos			=	pHuman->getWorldPos()->m_fX;
		MoneyLogParam.ZPos			=	pHuman->getWorldPos()->m_fZ;
		SaveMoneyLog(&MoneyLogParam);

		pPlayerShopManager->ClampComFactor();
		pPlayerShop->UpdateMaxBaseMoney(pPlayerShopManager->GetComFactor());
		pPlayerShop->AddMoney((UINT)CurPriceTemp);

	
		//5.�����̵��̨
		uFinalSerial	=	pStallBox->IncSerialByIndex(IndexInStall);
		pStallBox->SetCanSale(IndexInStall, FALSE);
		pStallBox->SetPriceByIndex(IndexInStall, 0);

		//�۳�AA����ã�������ͭ����AAΪ��Ʒ����
		RecordOpt::Excute(REC_EXCHANGEITEM, pPlayerShop->GetExchangeRecord(), (INT)MessItemTBIndex, (INT)MessItemNum, (INT)CurPriceTemp);

		//������Ϣ���ͻ���
		MsgToClient.SetOpt(GCItemSynch::OPT_MOVE_ITEM);
		MsgToClient.SetFromType(GCItemSynch::POS_PLAYERSHOP);
		MsgToClient.SetToType(GCItemSynch::POS_BAG);
		MsgToClient.SetToIndex(uFinalIndex);
		MsgToClient.SetItemGuid(ItemGuid);

		GCMoveItemFromPlayerShopToBag_t ExtraInfoData;
		ExtraInfoData.m_ShopGuid		=	nShopID;
		ExtraInfoData.m_nStallIndex		=	nStallIndex;
		ExtraInfoData.m_Flag			=	0;
		ExtraInfoData.m_uSerial			=	uFinalSerial;
		MsgToClient.SetExtraInfoLength(ExtraInfoData.GetSize());
		MsgToClient.SetExtraInfoData((BYTE*)&ExtraInfoData);
		pGamePlayer->SendPacket(&MsgToClient);
	}
	else
	{//�����

		ItemContainer*	pStallContainer = pStallBox->GetPetContainer();

		if(pStallBox->GetStallStatus() != PLAYER_SHOP::STALL_OPEN)
		{//�����Ѿ��رմι�̨
			MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopBuyItemHandler::ObjName=%s, ERR_NEED_NEW_COPY: pStallBox->GetStallStatus() != PLAYER_SHOP::STALL_OPEN"
				,pHuman->GetName()) ;
			return PACKET_EXE_CONTINUE;
		}

		INT	IndexInStall = pStallContainer->GetIndexByGUID(&PetGuid);
		if(IndexInStall<0)
		{//�����Ѿ��ı����Ʒ
			MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopBuyItemHandler::ObjName=%s, ERR_NEED_NEW_COPY: IndexInStall = %d"
				,pHuman->GetName(), IndexInStall) ;
			return PACKET_EXE_CONTINUE;
		}

		if(!pStallBox->CanSale(IndexInStall))
		{//����Ʒ�Ѿ��¼�
			MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopBuyItemHandler::ObjName=%s, ERR_NEED_NEW_COPY: IndexInStall = %d"
				,pHuman->GetName(), IndexInStall) ;
			return PACKET_EXE_CONTINUE;
		}

		if( pStallBox->GetSerialByIndex(IndexInStall) != uSerial)
		{//�۸��Ѿ��޸�
			MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopBuyItemHandler::ObjName=%s, ERR_NEED_NEW_COPY: Serial = %d, BoxSerial = %d"
				,pHuman->GetName(), uSerial, pStallBox->GetSerialByIndex(IndexInStall)) ;
			return PACKET_EXE_CONTINUE;
		}

		Item* pItem = pStallContainer->GetItem(IndexInStall);
		PET_LOG_PARAM	PetLogParam;
		PetLogParam.PetGUID		=	pItem->GetPetGUID();
		PetLogParam.DataID		=	pItem->GetDataID();
		ItemContainer* pBagContainer = pHuman->GetPetContain();
		
		
		//1.����ռ�
		_EXCHANGE_ITEM_LIST			ItemListOtToMe;
		ItemListOtToMe.Init();
		ItemListOtToMe.AddItem(pItem);
		if( FALSE == HumanItemLogic::CanReceiveExchangeItemList( pHuman, ItemListOtToMe) )
		{//�ռ䲻��
			MsgError.SetID(PLAYERSHOP_MSG::ERR_NOT_ENOUGH_ROOM);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopBuyItemHandler::ObjName=%s, ERR_NOT_ENOUGH_ROOM"
				,pHuman->GetName()) ;
			return PACKET_EXE_CONTINUE;
		}

		//2.�����Ǯ
		UINT	CurPrice = pStallBox->GetPriceByIndex(IndexInStall);
		if(pHuman->GetMoney()<CurPrice)
		{//��Ǯ����
			MsgError.SetID(PLAYERSHOP_MSG::ERR_NOT_ENOUGH_MONEY);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopBuyItemHandler::ObjName=%s, ERR_NOT_ENOUGH_MONEY"
				,pHuman->GetName()) ;
			return PACKET_EXE_CONTINUE;
		}

		//2.5������Ｖ��
		INT nPetLevel = pItem->GetLevel();
		INT nHumanLevel = pHuman->GetLevel();
		if(nPetLevel>nHumanLevel)
		{
			MsgError.SetID(PLAYERSHOP_MSG::ERR_PET_LEVEL_TOO_HIGH);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallBuyHandler::ObjName=%s, ERR_PET_LEVEL_TOO_HIGH"
				,pHuman->GetName()) ;
			return PACKET_EXE_CONTINUE;
		}

		//3.�ƶ���Ʒ
		//�Զ��Ҹ�,֧���Զ�����
		INT result = 
			g_ItemOperator.MoveItem
			(
			pStallContainer,
			IndexInStall,
			pBagContainer
			);

		if(result<0)
		{//����ʧ��
			MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopBuyItemHandler::ObjName=%s, ERR_ILLEGAL: IndexInStall = %d, result = %d "
				,pHuman->GetName(), IndexInStall, result) ;
			return PACKET_EXE_CONTINUE;
		}
				
		PetLogParam.CharGUID	=	pHuman->GetGUID();
		PetLogParam.OPType		=	PET_OP_PLAYERSHOP_EXCHANGE_GET;
		PetLogParam.SceneID		=	pHuman->getScene()->SceneID();
		PetLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
		PetLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
		SavePetLog(&PetLogParam);

		uFinalIndex	= result;

		//4.��Ǯ
		FLOAT CurPriceTemp = (FLOAT)CurPrice;
		pHuman->SetMoney((UINT)(pHuman->GetMoney() - CurPriceTemp));

		MONEY_LOG_PARAM	MoneyLogParam;
		MoneyLogParam.CharGUID	=	pHuman->GetGUID();
		MoneyLogParam.TargetGUID	=	pPlayerShop->GetOwnerGuid();
		MoneyLogParam.OPType	=	MONEY_EXCHANGE_PLAYERSHOP_OUTCOME;	
		MoneyLogParam.Count		=	(INT)CurPriceTemp;
		MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
		MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
		MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
		SaveMoneyLog(&MoneyLogParam);

		pPlayerShopManager->ClampComFactor();
		pPlayerShop->UpdateMaxBaseMoney(pPlayerShopManager->GetComFactor());
		pPlayerShop->AddMoney((UINT)CurPriceTemp);

		//5.�����̵��̨
		uFinalSerial	=	pStallBox->IncSerialByIndex(IndexInStall);
		pStallBox->SetCanSale(IndexInStall, FALSE);
		pStallBox->SetPriceByIndex(IndexInStall, 0);

		pItem = pBagContainer->GetItem(uFinalIndex);
		//�۳�AA����ã�������ͭ����AAΪ��������
		RecordOpt::Excute(REC_EXCHANGEPET, pPlayerShop->GetExchangeRecord(), (CHAR*)pItem->GetName(), (INT)1, (INT)CurPriceTemp);

		//������Ϣ���ͻ���
		MsgToClient.SetOpt(GCItemSynch::OPT_MOVE_ITEM);
		MsgToClient.SetFromType(GCItemSynch::POS_PLAYERSHOPPET);
		MsgToClient.SetToType(GCItemSynch::POS_PET);
		MsgToClient.SetToIndex(uFinalIndex);
		MsgToClient.SetPetGUID(PetGuid);
		
		GCMovePetFromPlayerShopToBag_t ExtraInfoData;
		ExtraInfoData.m_ShopGuid		=	nShopID;
		ExtraInfoData.m_nStallIndex		=	nStallIndex;
		ExtraInfoData.m_Flag			=	0;
		ExtraInfoData.m_uSerial			=	uFinalSerial;
		MsgToClient.SetExtraInfoLength(ExtraInfoData.GetSize());
		MsgToClient.SetExtraInfoData((BYTE*)&ExtraInfoData);
		pGamePlayer->SendPacket(&MsgToClient);
	}


	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopBuyItemHandler::ObjName=%s"
		,pHuman->GetName()) ;

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
