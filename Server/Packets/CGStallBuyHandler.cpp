#include "stdafx.h"
/*
����̯���Ļ���
*/

#include "CGStallBuy.h"
#include "GCStallBuy.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

#include "ItemContainer.h"
#include "ItemOperator.h"
#include "HumanItemLogic.h"
#include "Obj_Pet.h"

UINT CGStallBuyHandler::Execute( CGStallBuy* pPacket, Player* pPlayer )
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
	
	ObjID_t		ObjId		=	pPacket->GetObjID();
	_ITEM_GUID	ItemGuid	=	pPacket->GetObjGUID();
	PET_GUID_t	PetGuid		=	pPacket->GetPetGUID();
	UINT		nSerial		=	pPacket->GetSerial();
	GCStallError MsgError;

	//�жϾ����Ƿ���Դ�
	Obj_Character *pTarget = (Obj_Character*)(pScene->GetObjManager()->GetObj( ObjId ));
	if(pTarget->GetObjType() != Obj::OBJ_TYPE_HUMAN)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallBuyHandler::ObjName=%s, ERR_OWNER_INVALID"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE;
	}
	Obj_Human* pTargetHuman = (Obj_Human*)pTarget;
	if( pTargetHuman == NULL )
	{
		MsgError.SetID(STALL_MSG::ERR_OWNER_INVALID);
		pHuman->GetPlayer()->SendPacket(&MsgError);
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallBuyHandler::ObjName=%s, ERR_OWNER_INVALID"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE;
	}

	// ��ͬ��Ӫ�����ò鿴
	if( pHuman->IsEnemy( pTargetHuman ) )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGStallBuyHandler: %s cann't ask %s's detailattr ", pHuman->GetName(), pTargetHuman->GetName() ) ;
		return	PACKET_EXE_CONTINUE;
	}
	if(pTargetHuman->m_StallBox.GetStallStatus() != ServerStallBox::STALL_OPEN)
	{//Ŀ��û�а�̯
		GCStallError	Msg;
		Msg.SetID(STALL_MSG::ERR_CLOSE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallBuyHandler::ObjName=%s, ERR_CLOSE"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE;
	}

	if(PetGuid.IsNull())
	{//����Ʒ
		//̯��container
		ItemContainer*	pStallContainer = pTargetHuman->m_StallBox.GetContainer(); 
		INT	IndexInStall = pStallContainer->GetIndexByGUID(&ItemGuid);
		if(IndexInStall<0)
		{//��Ʒ������
			MsgError.SetID(STALL_MSG::ERR_NEED_NEW_COPY);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallBuyHandler::ObjID=%d, ERR_NEED_NEW_COPY:IndexInStall = %d"
				,pHuman->GetID(), IndexInStall) ;
			return PACKET_EXE_CONTINUE;
		}

		if(pTargetHuman->m_StallBox.GetSerialByIndex(IndexInStall) > nSerial)
		{//�汾�Ը���
			MsgError.SetID(STALL_MSG::ERR_NEED_NEW_COPY);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallBuyHandler::ObjID=%d, ERR_NEED_NEW_COPY: nSerial = %d, BoxSerial = %d"
				,pHuman->GetID(), nSerial, pTargetHuman->m_StallBox.GetSerialByIndex(IndexInStall)) ;
			return PACKET_EXE_CONTINUE;
		}

		//�ڱ������ҵ�����Ʒ
		//ItemContainer* pMatContainer	= pTargetHuman->GetMatContain();
		ItemContainer* pBaseContainer	= pTargetHuman->GetBaseContain();
		//ItemContainer* pBagContainer	= NULL;

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
		//	g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallBuyHandler::ObjID=%d, ERR_ILLEGAL:m_World = %d, m_Server = %d, m_Serial = %d "
		//		,pHuman->GetID(), ItemGuid.m_World, ItemGuid.m_Server, ItemGuid.m_Serial) ;
		//	return PACKET_EXE_CONTINUE ;
		//}

		INT		IndexInBag		=	pBaseContainer->GetIndexByGUID(&ItemGuid);
		Item*	pItem			=	pBaseContainer->GetItem(IndexInBag);
		UINT	MessItemTBIndex =	pItem->GetItemTableIndex();
		UINT	MessItemNum     =	pItem->GetLayedNum();
		
		UINT	ItemType		=	pItem->GetItemTableIndex();
		//1.����ռ�
		ItemContainer*		pMyContainer = HumanItemLogic::GetItemContain(pHuman, pItem->GetItemTableIndex());
		_EXCHANGE_ITEM_LIST			ItemListOtToMe;
		ItemListOtToMe.Init();
		ItemListOtToMe.AddItem(pItem);
		if( FALSE == HumanItemLogic::CanReceiveExchangeItemList( pHuman, ItemListOtToMe) )
		{//�ռ䲻��
			MsgError.SetID(STALL_MSG::ERR_NOT_ENOUGH_ROOM);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallBuyHandler::ObjID=%d, ERR_NOT_ENOUGH_ROOM"
				,pHuman->GetID()) ;
			return PACKET_EXE_CONTINUE;
		}

		//2.�����Ǯ
		UINT	CurPrice = pTargetHuman->m_StallBox.GetPriceByIndex(IndexInStall);
		if(pHuman->GetMoney()<CurPrice)
		{//��Ǯ����
			MsgError.SetID(STALL_MSG::ERR_NOT_ENOUGH_MONEY);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallBuyHandler::ObjID=%d, ERR_NOT_ENOUGH_MONEY"
				,pHuman->GetID()) ;
			return PACKET_EXE_CONTINUE;
		}

		//3.�ƶ���Ʒ
		//�Ƚ���
		BOOL	bFlag = FALSE;
		g_ItemOperator.UnlockItem( pBaseContainer, IndexInBag );

		//�Զ��Ҹ�,֧���Զ�����
		INT result = 
			g_ItemOperator.MoveItem
			(
			pBaseContainer,
			IndexInBag,
			bFlag,
			pMyContainer
			);

		

		if(result<0)
		{//����ʧ��
			MsgError.SetID(STALL_MSG::ERR_ILLEGAL);
			pGamePlayer->SendPacket(&MsgError);
			pTargetHuman->m_StallBox.CleanUp();
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallBuyHandler::ObjID=%d, ERR_ILLEGAL: IndexInBag = %d, result = %d "
				,pHuman->GetID(), IndexInBag, result) ;
			return PACKET_EXE_CONTINUE;
		}
		//����Log
		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType		=	ITEM_STALL_BOX_EXCHANGE;
		ItemLogParam.CharGUID	=	pTargetHuman->GetGUID();
		ItemLogParam.TargetGUID	=	pHuman->GetGUID();
		ItemLogParam.ItemGuid	=	ItemGuid;
		ItemLogParam.BagPos		=	result;
		ItemLogParam.ItemType	=	ItemType;
		SaveItemLog(&ItemLogParam);

		UINT	ToIndex = pMyContainer->ConIndex2BagIndex(result);

		//4.��Ǯ
		FLOAT CurPriceTemp = (FLOAT)CurPrice;
		FLOAT TradeTax     = (FLOAT)pTargetHuman->m_StallBox.GetTradeTax();
		pHuman->SetMoney((UINT)(pHuman->GetMoney() - CurPriceTemp));

		MONEY_LOG_PARAM	MoneyLogParam;
		MoneyLogParam.CharGUID	=	pHuman->GetGUID();
		MoneyLogParam.TargetGUID	=	pTargetHuman->GetGUID();
		MoneyLogParam.OPType	=	MONEY_EXCHANGE_STALL_OUTCOME;	
		MoneyLogParam.Count		=	(INT)CurPriceTemp;
		MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
		MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
		MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
		SaveMoneyLog(&MoneyLogParam);

		if(TradeTax > 100)
			TradeTax = 100;
		CurPriceTemp *= 1-(FLOAT)(TradeTax/100);
		pTargetHuman->SetMoney((UINT)(pTargetHuman->GetMoney() + CurPriceTemp));
		

		
		MoneyLogParam.CharGUID		=	pTargetHuman->GetGUID();
		MoneyLogParam.TargetGUID	=	pHuman->GetGUID();
		MoneyLogParam.OPType	=	MONEY_EXCHANGE_STALL_INCOME;	
		MoneyLogParam.Count		=	(INT)CurPriceTemp;
		MoneyLogParam.SceneID	=	pTargetHuman->getScene()->SceneID();
		MoneyLogParam.XPos		=	pTargetHuman->getWorldPos()->m_fX;
		MoneyLogParam.ZPos		=	pTargetHuman->getWorldPos()->m_fZ;
		SaveMoneyLog(&MoneyLogParam);

		if(TradeTax > 100)

		//5.����̯λ��
		pTargetHuman->m_StallBox.IncSerialByIndex(IndexInStall);
		pTargetHuman->m_StallBox.SetPriceByIndex(IndexInStall, 0);
		result = 
			g_ItemOperator.EraseItem
			(
			pStallContainer,
			IndexInStall
			);
		if(result<0)
		{//����ʧ��
			MsgError.SetID(STALL_MSG::ERR_ILLEGAL);
			pGamePlayer->SendPacket(&MsgError);
			pTargetHuman->m_StallBox.CleanUp();
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallBuyHandler::ObjID=%d, ERR_ILLEGAL: IndexInStall = %d, result = %d "
				,pHuman->GetID(), IndexInStall, result) ;
			return PACKET_EXE_CONTINUE;
		}
		
		

		//6.���׼�¼�������԰�
		ServerBBS* pBBS = pTargetHuman->m_StallBox.GetBBS();
		CHAR szExchangeMessage[MAX_BBS_MESSAGE_LENGTH];
		memset(szExchangeMessage,0, MAX_BBS_MESSAGE_LENGTH);

		UINT Gold		= (UINT)(CurPrice/10000);
		UINT Silver		= (UINT)((CurPrice%10000)/100);
		UINT copper		= (UINT)(CurPrice%100);
		sprintf(szExchangeMessage,"%s���� [#{_ITEM%d}] X %d, ������%d��%d��%dͭ", pHuman->GetName(), MessItemTBIndex, MessItemNum, Gold, Silver, copper);
		UINT	NewID = pBBS->NewMessageID();
		if(pBBS->AddNewMessageByID(NewID, szExchangeMessage, (INT)strlen(szExchangeMessage), "_SYSTEM") == FALSE)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallBuyHandler::ObjID=%d, NewID = %d"
				,pHuman->GetID(), NewID) ;
			return PACKET_EXE_CONTINUE;
		}

		//7.֪ͨ�ͻ���
		GCStallBuy	MsgBuy;
		MsgBuy.SetObjID(ObjId);
		MsgBuy.SetObjGUID(ItemGuid);
		MsgBuy.SetToIndex(ToIndex);
		MsgBuy.SetSerial(pTargetHuman->m_StallBox.GetSerialByIndex(IndexInStall));
		MsgBuy.SetToType(STALL_MSG::POS_BAG);
		pGamePlayer->SendPacket(&MsgBuy);
		pTargetHuman->GetPlayer()->SendPacket(&MsgBuy);

		g_pLog->FastSaveLog( LOG_FILE_1, "CGStallBuyHandler::ObjID=%d, ToIndex = %d, m_World = %d, m_Server = %d, m_Serial = %d"
			,pHuman->GetID(), ToIndex, ItemGuid.m_World, ItemGuid.m_Server, ItemGuid.m_Serial) ;

	}
	else
	{//�����
		ItemContainer*	pStallPetContainer = pTargetHuman->m_StallBox.GetPetContainer(); 
		INT	PetIndexInStall = pStallPetContainer->GetIndexByGUID(&PetGuid);
		if(PetIndexInStall<0)
		{//��Ʒ������
			MsgError.SetID(STALL_MSG::ERR_NEED_NEW_COPY);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallBuyHandler::ObjName=%s, ERR_NEED_NEW_COPY:PetIndexInStall = %d"
				,pHuman->GetName(), PetIndexInStall) ;
			return PACKET_EXE_CONTINUE;
		}

		if(pTargetHuman->m_StallBox.GetPetSerialByIndex(PetIndexInStall) != nSerial)
		{//�汾�Ը���
			MsgError.SetID(STALL_MSG::ERR_NEED_NEW_COPY);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallBuyHandler::ObjName=%s, ERR_NEED_NEW_COPY: nSerial = %d, BoxSerial = %d"
				,pHuman->GetName(), nSerial, pTargetHuman->m_StallBox.GetPetSerialByIndex(PetIndexInStall)) ;
			return PACKET_EXE_CONTINUE;
		}

		ItemContainer*		pOtBagPetContainer = pTargetHuman->GetPetContain();
		INT PetIndexInOtBag	=	pOtBagPetContainer->GetIndexByGUID(&PetGuid);
		Item* pItem = pOtBagPetContainer->GetItem(PetIndexInOtBag);	

		//1.����ռ�
		ItemContainer*		pMyBagPetContainer = pHuman->GetPetContain();
		_EXCHANGE_ITEM_LIST		ItemListOtToMe;
		ItemListOtToMe.Init();
		ItemListOtToMe.AddItem(pItem);
		if( FALSE == HumanItemLogic::CanReceiveExchangeItemList( pHuman, ItemListOtToMe) )
		{//�ռ䲻��
			MsgError.SetID(STALL_MSG::ERR_NOT_ENOUGH_ROOM);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallBuyHandler::ObjName=%s, ERR_NOT_ENOUGH_ROOM"
				,pHuman->GetName()) ;
			return PACKET_EXE_CONTINUE;
		}

		//2.�����Ǯ
		UINT	CurPrice = pTargetHuman->m_StallBox.GetPetPriceByIndex(PetIndexInStall);
		if(pHuman->GetMoney()<CurPrice)
		{//��Ǯ����
			MsgError.SetID(STALL_MSG::ERR_NOT_ENOUGH_MONEY);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallBuyHandler::ObjName=%s, ERR_NOT_ENOUGH_MONEY"
				,pHuman->GetName()) ;
			return PACKET_EXE_CONTINUE;
		}

		//2.5������Ｖ��
		INT nPetLevel = pItem->GetLevel();
		INT nHumanLevel = pHuman->GetLevel();
		if(nPetLevel>nHumanLevel)
		{
			MsgError.SetID(STALL_MSG::ERR_PET_LEVEL_TOO_HIGH);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallBuyHandler::ObjName=%s, ERR_PET_LEVEL_TOO_HIGH"
				,pHuman->GetName()) ;
			return PACKET_EXE_CONTINUE;
		}

		//�ٻس���
		Obj_Pet* pDestPet = pTargetHuman->GetPet();
		PET_GUID_t	DestPetGuid;
		if(pDestPet)
		{
			DestPetGuid = pDestPet->GetPetGUID();
		}
		if(DestPetGuid == pItem->GetPetGUID())
            pTargetHuman->ReCallPet();
	
		//3.�ƶ���Ʒ
		//�Ƚ���
		BOOL	bFlag = FALSE;
		g_ItemOperator.UnlockItem( pOtBagPetContainer, PetIndexInOtBag );

		//�Զ��Ҹ�,֧���Զ�����
		INT result = 
			g_ItemOperator.MoveItem
			(
			pOtBagPetContainer,
			PetIndexInOtBag,
			pMyBagPetContainer
			);

		if(result<0)
		{//����ʧ��
			MsgError.SetID(STALL_MSG::ERR_ILLEGAL);
			pGamePlayer->SendPacket(&MsgError);
			pTargetHuman->m_StallBox.CleanUp();
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallBuyHandler::ObjName=%s, ERR_ILLEGAL: PetIndexInOtBag = %d, result = %d "
				,pHuman->GetName(), PetIndexInOtBag, result) ;
			return PACKET_EXE_CONTINUE;
		}
		UINT	ToIndex = result;
		Item* pIt = pMyBagPetContainer->GetItem(ToIndex);
		
		PET_LOG_PARAM	PetLogParam;
		PetLogParam.CharGUID	=	pHuman->GetGUID();
		PetLogParam.OPType		=	PET_OP_STALL_EXCHANGE_GET;
		PetLogParam.PetGUID		=	pIt->GetPetGUID();
		PetLogParam.DataID		=	pIt->GetDataID();
		PetLogParam.SceneID		=	pHuman->getScene()->SceneID();
		PetLogParam.TargetGUID	=	pTargetHuman->GetGUID();
		PetLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
		PetLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
		SavePetLog(&PetLogParam);

		//4.��Ǯ
		FLOAT CurPriceTemp = (FLOAT)CurPrice;
		FLOAT TradeTax     = (FLOAT)pTargetHuman->m_StallBox.GetTradeTax();
		pHuman->SetMoney((UINT)(pHuman->GetMoney() - CurPriceTemp));

		MONEY_LOG_PARAM	MoneyLogParam;
		MoneyLogParam.CharGUID	=	pHuman->GetGUID();
		MoneyLogParam.TargetGUID	=	pTargetHuman->GetGUID();
		MoneyLogParam.OPType	=	MONEY_EXCHANGE_STALL_OUTCOME;	
		MoneyLogParam.Count		=	(INT)CurPriceTemp;
		MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
		MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
		MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
		SaveMoneyLog(&MoneyLogParam);

		if(TradeTax > 100)
			TradeTax = 100;
		CurPriceTemp *= 1-(FLOAT)(TradeTax/100);
		pTargetHuman->SetMoney((UINT)(pTargetHuman->GetMoney() + CurPriceTemp));

	
		MoneyLogParam.CharGUID		=	pTargetHuman->GetGUID();
		MoneyLogParam.TargetGUID	=	pHuman->GetGUID();
		MoneyLogParam.OPType	=	MONEY_EXCHANGE_STALL_INCOME;	
		MoneyLogParam.Count		=	(INT)CurPriceTemp;
		MoneyLogParam.SceneID	=	pTargetHuman->getScene()->SceneID();
		MoneyLogParam.XPos		=	pTargetHuman->getWorldPos()->m_fX;
		MoneyLogParam.ZPos		=	pTargetHuman->getWorldPos()->m_fZ;
		SaveMoneyLog(&MoneyLogParam);

		//5.����̯λ��
		pTargetHuman->m_StallBox.IncPetSerialByIndex(PetIndexInStall);
		pTargetHuman->m_StallBox.SetPetPriceByIndex(PetIndexInStall, 0);
		result = 
			g_ItemOperator.EraseItem
			(
			pStallPetContainer,
			PetIndexInStall
			);
		if(result<0)
		{//����ʧ��
			MsgError.SetID(STALL_MSG::ERR_ILLEGAL);
			pGamePlayer->SendPacket(&MsgError);
			pTargetHuman->m_StallBox.CleanUp();
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallBuyHandler::ObjName=%s, ERR_ILLEGAL: PetIndexInStall = %d, result = %d "
				,pHuman->GetName(), PetIndexInStall, result) ;
			return PACKET_EXE_CONTINUE;
		}

		//6.���׼�¼�������԰�
		ServerBBS* pBBS = pTargetHuman->m_StallBox.GetBBS();
		CHAR szExchangeMessage[MAX_BBS_MESSAGE_LENGTH];
		memset(szExchangeMessage,0, MAX_BBS_MESSAGE_LENGTH);

		UINT Gold		= (UINT)(CurPrice/10000);
		UINT Silver		= (UINT)((CurPrice%10000)/100);
		UINT copper		= (UINT)(CurPrice%100);
		sprintf(szExchangeMessage,"%s���� ����[%s], ������%d��%d��%dͭ", pHuman->GetName(), pIt->GetName(), Gold, Silver, copper);
		UINT	NewID = pBBS->NewMessageID();
		if(pBBS->AddNewMessageByID(NewID, szExchangeMessage, (INT)strlen(szExchangeMessage), "_SYSTEM") == FALSE)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallBuyHandler::ObjName=%s, NewID = %d"
				,pHuman->GetName(), NewID) ;
			return PACKET_EXE_CONTINUE;
		}

		//7.֪ͨ�ͻ���
		GCStallBuy	MsgBuy;
		MsgBuy.SetObjID(ObjId);
		MsgBuy.SetPetGUID(PetGuid);
		MsgBuy.SetToIndex(ToIndex);
		MsgBuy.SetSerial(pTargetHuman->m_StallBox.GetPetSerialByIndex(PetIndexInStall));
		MsgBuy.SetToType(STALL_MSG::POS_PET);
		pGamePlayer->SendPacket(&MsgBuy);
		pTargetHuman->GetPlayer()->SendPacket(&MsgBuy);

		g_pLog->FastSaveLog( LOG_FILE_1, "CGStallBuyHandler::ObjName=%d, ToIndex = %d, PetName = %s"
			,pHuman->GetName(), ToIndex,pIt->GetName()) ;
	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
