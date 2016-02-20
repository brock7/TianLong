#include "stdafx.h"
/*
֪ͨ������Ҫ�����̵�
*/

#include "CGPlayerShopBuyShop.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCPlayerShopBuyShop.h"

using namespace PLAYER_SHOP;
UINT CGPlayerShopBuyShopHandler::Execute( CGPlayerShopBuyShop* pPacket, Player* pPlayer )
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

	_PLAYERSHOP_GUID		nShopID		= pPacket->GetShopID();			//�̵�ID
	BYTE					nSerial		= pPacket->GetSerial();	

	PlayerShopManager*	pPlayerShopManager	= pScene->GetPlayerShopManager();
	PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
	Assert(pPlayerShop);

	if(nSerial!= pPlayerShop->GetSerial())
	{//���кű仯
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopBuyShopHandler::Name=%s Serial changed"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;		
	}

	if(pPlayerShop->GetShopStatus() != STATUS_PLAYER_SHOP_ON_SALE)
	{//�����̳�״̬
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopBuyShopHandler::Name=%s Does not sale"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;
	}

	UINT  uShopPrice = pPlayerShop->GetSaleOutPrice();
	UINT  uSelfMoney = pHuman->GetMoney();
	CHAR  szOldOwner[MAX_CHARACTER_NAME] = {0};
	memcpy(szOldOwner, pPlayerShop->GetOwnerName(), MAX_CHARACTER_NAME);

	if(uSelfMoney < uShopPrice)
	{//��Ǯ����
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopBuyShopHandler::Name=%s Short of money ,shopprice = %d, money = %d"
			,pHuman->GetName(), uShopPrice, uSelfMoney) ;
		return PACKET_EXE_CONTINUE ;
	}

	if(pPlayerShop->GetShopType() == TYPE_PLAYER_SHOP_ITEM)
	{//��Ʒ��
		if(pHuman->GetShopGuid(0).isNull() == FALSE)
		{
			PlayerShop* pCurShop = pPlayerShopManager->GetPlayerShopByGUID(pHuman->GetShopGuid(0));
			if(!pCurShop)
			{
				pHuman->GetShopGuid(0).Reset();
			}
			else if(pCurShop->GetOwnerGuid() == pHuman->GetGUID())
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopBuyShopHandler::Name=%s Already has such item shop"
					,pHuman->GetName()) ;
				return PACKET_EXE_CONTINUE ;
			}
		}
	}
	else if(pPlayerShop->GetShopType() == TYPE_PLAYER_SHOP_PET)
	{//�����
		if(pHuman->GetShopGuid(1).isNull() == FALSE)
		{
			PlayerShop* pCurShop = pPlayerShopManager->GetPlayerShopByGUID(pHuman->GetShopGuid(1));
			if(!pCurShop)
			{
				pHuman->GetShopGuid(1).Reset();
			}
			else if(pCurShop->GetOwnerGuid() == pHuman->GetGUID())
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopBuyShopHandler::Name=%s Already has such pet shop"
					,pHuman->GetName()) ;
				return PACKET_EXE_CONTINUE ;
			}
		}
	}

	pPlayerShop->SetOwnerGuid(pHuman->GetGUID());
	pPlayerShop->SetOwnerName(pHuman->GetName());
	pPlayerShop->SetSaleOutPrice(0);
	pPlayerShop->SetShopStatus(STATUS_PLAYER_SHOP_OPEN);

	//��Ǯ
	pHuman->SetMoney(uSelfMoney-uShopPrice);

	//����ִ���ʼ�������Ҽ�Ǯ
	pScene->SendScriptMail(szOldOwner, MAIL_UPDATE_ATTR, MAIL_ATTR_MONEY, uShopPrice, pHuman->GetGUID());

	//֪ͨ�ʼ�
	CHAR szMsgContent[MAX_MAIL_CONTEX] = {0};
	sprintf(szMsgContent, "%s ��������ĵ���", pHuman->GetName());
	pScene->SendNormalMail(pHuman, szOldOwner, szMsgContent);

	MONEY_LOG_PARAM	MoneyLogParam;
	MoneyLogParam.CharGUID	=	pHuman->GetGUID();
	MoneyLogParam.TargetGUID	=	pPlayerShop->GetOwnerGuid();
	MoneyLogParam.OPType	=	MONEY_BUY_PLAYERSHOP_OUTCOME;	
	MoneyLogParam.Count		=	uShopPrice;
	MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
	MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
	MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
	SaveMoneyLog(&MoneyLogParam);

	

	//�̵�����
	if(pPlayerShop->GetShopType() == TYPE_PLAYER_SHOP_ITEM)
	{//��Ʒ��
		pHuman->SetShopGuid(0,pPlayerShop->GetShopGUID());
	}
	else if(pPlayerShop->GetShopType() == TYPE_PLAYER_SHOP_PET)
	{//�����
		pHuman->SetShopGuid(1,pPlayerShop->GetShopGUID());
	}

	//֪ͨ�ͻ��˹����̵�ɹ�
	GCPlayerShopError Msg;
	Msg.SetID(PLAYERSHOP_MSG::ERR_SHOP_SUCCESS_SELL);
	pHuman->GetPlayer()->SendPacket(&Msg);

	//��־�˵��Ѿ��۳�
	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopSaleOutHandler::ObjName=%s"
		,pHuman->GetName());

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
