#include "stdafx.h"
// CGShopBuyHandler.cpp

#include "CGShopBuy.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCNotifyEquip.h"
#include "Log.h"
#include "GCShopBuy.h"
#include "ShopManager.h"
#include "GCShopUpdateMerchandiseList.h"
#include "GCShopSoldList.h"
#include "HumanItemLogic.h"
#include "ItemTable.h"
#include "GCNotifyEquip.h"

#define BUY_BACK_INDEX		200
UINT CGShopBuyHandler::Execute( CGShopBuy* pPacket, Player* pPlayer )
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

	BYTE		m_nndex			=	pPacket->GetIndex();	//��Դλ������
	UINT		UniqueID		=	pPacket->GetUniqueID();
	//_ITEM_GUID	ItemGuid		=	pPacket->GetItemGUID();
	//UINT		uBuyNum			=	pPacket->GetBuyNum();
	
	SceneID_t	SceneID =	UniqueID>>16;
	ObjID_t		NpcObjID=	UniqueID&0xffff;

	if(SceneID != pScene->SceneID())
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGShopBuyHandler Illegal scene") ;
		return PACKET_EXE_CONTINUE ; 
	}

    //�����ж�
	Obj* pNpcObj = (Obj*) (pScene->GetObjManager()->GetObj(NpcObjID));
	if(pNpcObj == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGShopBuyHandler Illegal Obj") ;
		return PACKET_EXE_CONTINUE ; 
	}

	FLOAT fDist = MySqrt(pNpcObj->getWorldPos(), pHuman->getWorldPos());

	if(fDist>MAX_NPC_DISTANCE)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGShopBuyHandler Out Of Range") ;
		return PACKET_EXE_CONTINUE ; 
	}

	//��Ӫ�ж�
	if( pHuman->IsEnemy(  (Obj_Character*)pNpcObj  ) )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGShopBuyHandler GUID:%d buy item with wrong camp", pHuman->GetGUID() ) ;
		return PACKET_EXE_CONTINUE ; 
	}

	if(pHuman->Shop( )	== NULL)
	{//û�д򿪽��״��ھ���
		g_pLog->FastSaveLog( LOG_FILE_1, "Empty Shop!!! GUID = %X",  pHuman->GetGUID()) ;
		return PACKET_EXE_CONTINUE ;
	}

	//if( uBuyNum == 0 || uBuyNum > 20)
	//{//���������Ϸ����ж�
	//	g_pLog->FastSaveLog( LOG_FILE_1, "uBuyNum overflow !!! GUID = %X uBuyNum = %d",  pHuman->GetGUID(), uBuyNum) ;
	//	return PACKET_EXE_CONTINUE ;
	//}

	//���̵�ʱ������̵��ָ���Ѿ�������human�����ˣ��ر��̵�ʱ���ᱻ���
	_SHOP*		pShop	=	pHuman->Shop( )	;
	INT		itemNumber	=	0;

	//��ʱ�涨200����Ϊ�ع�����Ʒ��
	if(m_nndex >= BUY_BACK_INDEX)
	{//�ڻع������в���
		_ITEM tempItem;
		UINT  uPrice = 0;
		BYTE  uCoinType = 0;

		//ֻ�����ж����۶��������ǲ����ж���
		if(!pHuman->GetCurFromSoldList(tempItem, uPrice, uCoinType))
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "SoldList is empty GUID = %X",  pHuman->GetGUID()) ;
			return PACKET_EXE_CONTINUE ;
		}

		itemNumber = tempItem.GetItemCount();
		INT iPrice = (INT)uPrice;

		if( uCoinType == 0 )//��ͨ�̵�
		{
			if( ((INT)pHuman->GetMoney()) < iPrice )
			{//��Ǯ����
				GCShopBuy Msg ;
				Msg.SetBuyOk((BYTE)GCShopBuy::BUY_MONEY_FAIL);
				pHuman->GetPlayer()->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGShopBuyHandler not enough money iprice =%d GetMoney = %d  GUID = %X", iPrice, pHuman->GetMoney(),
					pHuman->GetGUID()) ;

				return PACKET_EXE_CONTINUE ;
			}
		}
		else if( uCoinType == 1 )//Ԫ���̵�
		{
			if( ((INT)pHuman->GetRMB()) < iPrice )
			{//Ԫ������
				GCShopBuy Msg ;
				Msg.SetBuyOk((BYTE)GCShopBuy::BUY_RMB_FAIL);
				pHuman->GetPlayer()->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGShopBuyHandler not enough money iprice =%d GetRMB = %d  GUID = %X", iPrice, pHuman->GetRMB(),
					pHuman->GetGUID()) ;

				return PACKET_EXE_CONTINUE ;
			}
		}
		else
		{
			GCShopBuy Msg ;
			Msg.SetBuyOk((BYTE)GCShopBuy::BUY_MONEY_FAIL);
			pHuman->GetPlayer()->SendPacket( &Msg ) ;
			g_pLog->FastSaveLog( LOG_FILE_1, "CGShopBuyHandler not enough money iprice =%d GetRMB = %d  GUID = %X", iPrice, pHuman->GetRMB(),
				pHuman->GetGUID()) ;

			return PACKET_EXE_CONTINUE ;
		}
		
		if(!pHuman->RecieveOneFromSoldList())
		{
			GCShopBuy Msg ;
			Msg.SetBuyOk((BYTE)GCShopBuy::BUY_BAG_FULL);
			pHuman->GetPlayer()->SendPacket( &Msg ) ;
			g_pLog->FastSaveLog( LOG_FILE_1, "CGShopBuyHandler Fail to del from soldlist GUID = %X  ",pHuman->GetGUID()) ;
			return PACKET_EXE_CONTINUE ;
		}
		else
		{//�ӻع��б����湺��ɹ�	
			if( uCoinType == 0 )
				pHuman->SetMoney( pHuman->GetMoney() - iPrice);
			else if( uCoinType == 1 )
				pHuman->SetRMB( pHuman->GetRMB() - iPrice );
			pHuman->NotifyMoneyChange(iPrice, FALSE);

			MONEY_LOG_PARAM	MoneyLogParam;
			MoneyLogParam.CharGUID	=	pHuman->GetGUID();
			MoneyLogParam.OPType	=	(uCoinType == 1) ? MONEY_SHOP_RMB_COST : MONEY_SHOP_COST;
			MoneyLogParam.Count		=	iPrice;
			MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
			MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SaveMoneyLog(&MoneyLogParam);


			GCShopBuy Msg ;
			Msg.SetBuyOk( (BYTE)GCShopBuy::BUY_OK );
			Msg.SetIndex( tempItem.m_ItemIndex );
			Msg.SetNum( tempItem.GetItemCount() );
			pHuman->GetPlayer()->SendPacket( &Msg ) ;

			//���»ع���ť
			GCShopSoldList::_MERCHANDISE_ITEM	SoldItem;
			_ITEM tempit;
			UINT  uPrice = 0;
			BYTE  nCoinType = 0;
			if(pHuman->GetCurFromSoldList(tempit, uPrice, nCoinType))
			{//�ع����л��ж���
				SoldItem.item_data = tempit;
				SoldItem.iPrice	   = uPrice;
				GCShopSoldList	MsgSold;
				MsgSold.SetMerchadiseNum(1);
				MsgSold.SetMerchadiseList(&SoldItem);
				pPlayer->SendPacket( &MsgSold );
			}
			else
			{//�ع����Ѿ�����
				GCShopSoldList	MsgSold;
				MsgSold.SetMerchadiseNum(0);
				pPlayer->SendPacket( &MsgSold );
			}
		}
	}
	else
	{//�����˹ҵ��̵������

		//һ�ε��������ĸ���
		INT itemNumber	=	pShop->m_ItemList->m_TypeCount[m_nndex]; 

		if(pShop->m_ItemList->m_TypeMaxNum[m_nndex] == 0)
		{//�����ˣ�ֱ�ӷ��أ�
			//Ӧ�ò����ߵ����
			//���ȱ��ﲻӦ�ó���0,
			//��������ʱ���������֪ͨ����������֮�ϵĿͻ���
			//�ڽ�����ɾ�������Ʒ���������Ҳ�������ٵ������Ʒ��

			GCShopBuy Msg ;
			Msg.SetBuyOk((BYTE)GCShopBuy::BUY_NO_MERCH);
			pPlayer->SendPacket( &Msg ) ;
			g_pLog->FastSaveLog( LOG_FILE_1, "CGShopBuyHandler  Buy out GUID = %X" , pHuman->GetGUID() );
			return PACKET_EXE_CONTINUE ;
		}

		//����۸�
		UINT BaseMoney = ShopMgr::ConvertItemType2Money(pShop->m_ItemList->m_ListType[m_nndex]);
		if(BaseMoney<0) BaseMoney = 0;
		BaseMoney *= static_cast<INT>(pShop->m_scale);
		BaseMoney *= itemNumber;
		//

		if( pShop->m_ShopType == 0 )
		{
			if( pHuman->GetMoney() < BaseMoney )
			{//��Ǯ����
				GCShopBuy Msg ;
				Msg.SetBuyOk((BYTE)GCShopBuy::BUY_MONEY_FAIL);
				pHuman->GetPlayer()->SendPacket( &Msg ) ;
				return PACKET_EXE_CONTINUE ;
			}
		}
		else if( pShop->m_ShopType == 1 )
		{
			if( pHuman->GetRMB() < (INT)BaseMoney )
			{//Ԫ������
				GCShopBuy Msg ;
				Msg.SetBuyOk((BYTE)GCShopBuy::BUY_RMB_FAIL);
				pHuman->GetPlayer()->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGShopBuyHandler not enough money iprice =%d GetRMB = %d  GUID = %X", BaseMoney, pHuman->GetRMB(),
					pHuman->GetGUID()) ;

				return PACKET_EXE_CONTINUE ;
			}
		}
		else
		{
			if( pHuman->GetMoney() < BaseMoney )
			{//��Ǯ����
				GCShopBuy Msg ;
				Msg.SetBuyOk((BYTE)GCShopBuy::BUY_MONEY_FAIL);
				pHuman->GetPlayer()->SendPacket( &Msg ) ;
				return PACKET_EXE_CONTINUE ;
			}
		}
		
		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType		= ITEM_CREATE_FROM_SHOP;
		ItemLogParam.CharGUID	= pHuman->GetGUID();
		ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
		ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
		ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
		ItemLogParam.ShopGUID	= pShop->m_ShopId;


		if(!HumanItemLogic::CreateMultiItemToBag(&ItemLogParam,
												 pHuman,
												 pShop->m_ItemList->m_ListType[m_nndex].ToSerial(),
												 itemNumber,
												 pShop->m_ShopType))
		//if(!pHuman->ReceiveMultiItem(&item, itemNumber))
		{
			GCShopBuy Msg ;
			Msg.SetBuyOk((BYTE)GCShopBuy::BUY_BAG_FULL);
			pHuman->GetPlayer()->SendPacket( &Msg ) ;
			return PACKET_EXE_CONTINUE ;
		}
		else
		{	
			SaveItemLog(&ItemLogParam);

			if( pShop->m_ShopType == 0 )
				pHuman->SetMoney( pHuman->GetMoney() - BaseMoney);
			else if( pShop->m_ShopType == 1 )
				pHuman->SetRMB( pHuman->GetRMB() - BaseMoney );

			MONEY_LOG_PARAM	MoneyLogParam;
			MoneyLogParam.CharGUID	=	pHuman->GetGUID();
			MoneyLogParam.OPType	=	(pShop->m_ShopType == 1) ? MONEY_SHOP_RMB_COST : MONEY_SHOP_COST;
			MoneyLogParam.Count		=	BaseMoney;
			MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
			MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SaveMoneyLog(&MoneyLogParam);


			GCShopBuy Msg ;
			Msg.SetBuyOk((BYTE)GCShopBuy::BUY_OK);
			Msg.SetIndex(pShop->m_ItemList->m_ListType[m_nndex].ToSerial());
			Msg.SetNum(itemNumber);
			pHuman->GetPlayer()->SendPacket( &Msg ) ;

			//Ҫ���´����˵Ķ�̬��Ʒ��ֻ�ڷ������޸ģ���������
			if(pShop->m_IsDyShop && pShop->m_ItemList->m_TypeMaxNum[m_nndex] > 0)
			{
				//��ס,ֻ��m_TypeMaxNum�ǿ�д�ģ�������һ����һ��
				pShop->m_ItemList->m_TypeMaxNum[m_nndex] -= 1;

				//�����Ϣ,ֻ���Լ�����
				GCShopUpdateMerchandiseList::_MERCHANDISE_ITEM	MerchandiseChanged;
				MerchandiseChanged.byNumber = pShop->m_ItemList->m_TypeMaxNum[m_nndex];
				MerchandiseChanged.idTable	= pShop->m_ItemList->m_ListTypeIndex[m_nndex];

				GCShopUpdateMerchandiseList	Msg;
				Msg.SetMerchadiseNum(1);//һ��Ҫ����������������List
				Msg.SetMerchadiseList(&MerchandiseChanged);
				pHuman->GetPlayer()->SendPacket( &Msg ) ;
			}
			return PACKET_EXE_CONTINUE ;
		}
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGShopBuyHandler m_nndex=%d",
		m_nndex) ;

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
