#include "stdafx.h"
/*
֪ͨ���������̵��Ѿ����̳�
*/

#include "CGPlayerShopSaleOut.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCPlayerShopSaleOut.h"
#include "GCPlayerShopError.h"

using namespace PLAYER_SHOP;
UINT CGPlayerShopSaleOutHandler::Execute( CGPlayerShopSaleOut* pPacket, Player* pPlayer )
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
	BYTE					bSaleOut	=	pPacket->GetSaleOut();
	UINT					uPrice		=	pPacket->GetPrice();
	BYTE					nSerial		=	pPacket->GetSerial();
	GCPlayerShopError		MsgError;

	PlayerShopManager*	pPlayerShopManager	= pScene->GetPlayerShopManager();
	PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
	Assert(pPlayerShop);

	if(pPlayerShop->GetOwnerGuid() != pHuman->GetGUID())
	{//ֻ�е�����Ȩ�޹رմ��̵�
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopSaleOutHandler::Name=%s Not Owner"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;
	}

	if(pPlayerShop->GetShopStatus() == STATUS_PLAYER_SHOP_ON_SALE && bSaleOut)
	{//�Ѿ��۳��ˣ�����������
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopSaleOutHandler::Name=%s Already Sale Out"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;
	}

	if(pPlayerShop->GetShopStatus() != STATUS_PLAYER_SHOP_ON_SALE && !bSaleOut)
	{//��û�̳���������������
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopSaleOutHandler::Name=%s Already Sale Out"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;
	}

	//���е꿪����
	for(INT i = 0; i<MAX_STALL_NUM_PER_SHOP;i++ )
	{
		PlayerStallBox* pPlayerStallBox = pPlayerShop->GetPlayerStallBoxByIndex(i);
		if(pPlayerStallBox->GetStallStatus() == STALL_OPEN)
		{
			MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_STILL_STALL_OPEN);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopSaleOutHandler::Name=%s Stall %d is still open"
				,pHuman->GetName(), i) ;
			return PACKET_EXE_CONTINUE ;
		}
	}

	if(nSerial != pPlayerShop->GetSerial())
	{//���кŸ���
		MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_SALE_OUT_SERIAL_IS_CHANGED);
		pGamePlayer->SendPacket(&MsgError);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopSaleOutHandler::Name=%s Serials had changed"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;
	}

	//������
	//�жϵ�ǰ�����Ƿ��30�� *��ҵָ��*�ѿ���̨��
	pPlayerShopManager->ClampComFactor();
	UINT uMoneyLevel =  static_cast<UINT>((FLOAT)pPlayerShop->GetNumStallOpened()*(FLOAT)pPlayerShopManager->GetComFactor()*300000.0);
	if(pPlayerShop->GetBaseMoney() < uMoneyLevel)
	{
		MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_BASE_MONEY_TOO_LOW);
		pGamePlayer->SendPacket(&MsgError);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopSaleOutHandler::Name=%s Base money is not enough"
			,pHuman->GetName()) ;
		
		return PACKET_EXE_CONTINUE ;
	}

	//��־�˵��Ѿ��۳�
	if(bSaleOut)
	{
		//��Ǯ�����̳�
		if(pHuman->GetMoney()<150000*pPlayerShopManager->GetComFactor())
		{
			MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_NOT_ENOUTH_MONEY_TO_SALE_OUT);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopSaleOutHandler::Name=%s Base money is not enough"
				,pHuman->GetName()) ;
			return PACKET_EXE_CONTINUE ;
		}

		pPlayerShop->SetShopStatus(STATUS_PLAYER_SHOP_ON_SALE);
		pPlayerShop->SetSaleOutPrice(uPrice);
	}
	else
	{
		//��Ǯ�����̻�
		if(pHuman->GetMoney()<50000*pPlayerShopManager->GetComFactor())
		{
			MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_NOT_ENOUTH_MONEY_TO_BUY_BACK);
			pGamePlayer->SendPacket(&MsgError);
			g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopSaleOutHandler::Name=%s cancel buy money "
				,pHuman->GetName()) ;
			return PACKET_EXE_CONTINUE ;
		}

		pPlayerShop->SetShopStatus(STATUS_PLAYER_SHOP_OPEN);
		pPlayerShop->SetSaleOutPrice(0);
	}

	BYTE uSerial = pPlayerShop->IncSerial();


	//֪ͨ�ͻ���
	GCPlayerShopSaleOut MsgToClient;
	MsgToClient.SetSaleOut(bSaleOut);
	MsgToClient.SetSerial(uSerial);
	pGamePlayer->SendPacket(&MsgToClient);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopSaleOutHandler::ObjName=%s Sale out = %d"
		,pHuman->GetName(), bSaleOut);

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
