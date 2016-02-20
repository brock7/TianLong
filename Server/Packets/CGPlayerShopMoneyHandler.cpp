#include "stdafx.h"

#include "CGPlayerShopMoney.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCPlayerShopMoney.h"
#include "GCPlayerShopError.h"
#include "RecordOpt.h"

UINT CGPlayerShopMoneyHandler::Execute( CGPlayerShopMoney* pPacket, Player* pPlayer )
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
	_PLAYERSHOP_GUID	nShopID	=	pPacket->GetShopID();			//�̵�ID
	BYTE				nOpt	=	pPacket->GetOpt();				//����
	BYTE				nType	=	pPacket->GetType();				//�浽��
	UINT				uAmount	=	pPacket->GetAmount();			//����
	BYTE			nShopSerial =	pPacket->GetShopSerial();		//�̵����к�

	GCPlayerShopError MsgError;

	PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
	PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
	Assert(pPlayerShop);

	if(pPlayerShop->GetShopStatus() == STATUS_PLAYER_SHOP_ON_SALE)
	{//�̵�δ����
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopMoneyHandler::Name=%s shop close"
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

	GCPlayerShopMoney	MsgToClient;
	MsgToClient.SetShopID(nShopID);
	switch(nOpt)
	{
		case CGPlayerShopMoney::OPT_SAVE_MONEY:
			{//��Ǯ

				if(pHuman->GetMoney() < uAmount)
				{
					Assert(0);
					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:CGPlayerShopMoneyHandler::Name=%s, Out Of Money In Packet"
						,pHuman->GetName()) ;

					return PACKET_EXE_CONTINUE ;
				}

				if(nType == CGPlayerShopMoney::TYPE_BASE_MONEY)
				{
					UINT uBaseMoney = pPlayerShop->GetBaseMoney();
					if(uBaseMoney+uAmount > 10000000)
					{//����1000��
						Assert(0);
						g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:CGPlayerShopMoneyHandler::Name=%s, uBaseMoney+uAmount > 10000000"
							,pHuman->GetName()) ;
						return PACKET_EXE_CONTINUE ;
					}
					UINT	uRealBaseMoney =  (UINT)(uBaseMoney+(FLOAT)uAmount*(1-0.03));
					pPlayerShop->SetBaseMoney(uRealBaseMoney);

					//���뱾��AAA�����̱����г��루���У���������ͭ��AAAΪ����������
					RecordOpt::Excute(REC_INPUT_BASE, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)(uRealBaseMoney - uBaseMoney));
					
					MsgToClient.SetType(GCPlayerShopMoney::TYPE_BASE_MONEY);
					MsgToClient.SetAmount(uRealBaseMoney);
					MsgToClient.SetShopSerial(pPlayerShop->IncSerial());
					pGamePlayer->SendPacket(&MsgToClient);
				}
				else if(nType == CGPlayerShopMoney::TYPE_PROFIT_MONEY)
				{
					UINT uProfitMoney = pPlayerShop->GetProfitMoney();
					UINT uRealProfitMoney =  (UINT)(uProfitMoney+(FLOAT)uAmount*(1-0.03));
					pPlayerShop->SetProfitMoney(uRealProfitMoney);
					MsgToClient.SetType(GCPlayerShopMoney::TYPE_PROFIT_MONEY);
					MsgToClient.SetAmount(uRealProfitMoney);
					MsgToClient.SetShopSerial(pPlayerShop->IncSerial());

					//����Ӯ���ʽ�AAA������Ӯ���ʽ��г��루���У���������ͭ��AAAΪ����������
					RecordOpt::Excute(REC_INPUT_PROFIT, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)(uRealProfitMoney - uProfitMoney));
					
					pGamePlayer->SendPacket(&MsgToClient);
				}

				pHuman->SetMoney(pHuman->GetMoney() - uAmount);

				MONEY_LOG_PARAM	MoneyLogParam;
				MoneyLogParam.CharGUID	=	pHuman->GetGUID();
				MoneyLogParam.TargetGUID	=	pPlayerShop->GetOwnerGuid();
				MoneyLogParam.OPType	=	MONEY_PLAYERSHOP_SAVE;	
				MoneyLogParam.Count		=	uAmount;
				MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
				MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				SaveMoneyLog(&MoneyLogParam);
			}
			break;
		case CGPlayerShopMoney::OPT_GET_MONEY:
			{//ȡǮ
				if(nType == CGPlayerShopMoney::TYPE_BASE_MONEY)
				{
					UINT uBaseMoney = pPlayerShop->GetBaseMoney();
					if(uBaseMoney < uAmount)
					{
						Assert(0);
						g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:CGPlayerShopMoneyHandler::Name=%s, Out Of Money In ShopBase"
							,pHuman->GetName()) ;

						return PACKET_EXE_CONTINUE ;
					}
					pPlayerShop->SetBaseMoney(uBaseMoney-uAmount);
					MsgToClient.SetType(GCPlayerShopMoney::TYPE_BASE_MONEY);
					MsgToClient.SetAmount(uBaseMoney-uAmount);
					MsgToClient.SetShopSerial(pPlayerShop->IncSerial());
					pGamePlayer->SendPacket(&MsgToClient);
				}
				else if(nType == CGPlayerShopMoney::TYPE_PROFIT_MONEY)
				{
					UINT uProfitMoney = pPlayerShop->GetProfitMoney();
					if(uProfitMoney < uAmount)
					{
						Assert(0);
						g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:CGPlayerShopMoneyHandler::Name=%s, Out Of Money In ShopProfit"
							,pHuman->GetName()) ;
						return PACKET_EXE_CONTINUE ;
					}
					pPlayerShop->SetProfitMoney(uProfitMoney-uAmount);
					MsgToClient.SetType(GCPlayerShopMoney::TYPE_PROFIT_MONEY);
					MsgToClient.SetAmount(uProfitMoney-uAmount);
					MsgToClient.SetShopSerial(pPlayerShop->IncSerial());
					pGamePlayer->SendPacket(&MsgToClient);
				}
				pHuman->SetMoney(pHuman->GetMoney() + uAmount);

				MONEY_LOG_PARAM	MoneyLogParam;
				MoneyLogParam.CharGUID	=	pHuman->GetGUID();
				MoneyLogParam.TargetGUID	=	pPlayerShop->GetOwnerGuid();
				MoneyLogParam.OPType	=	MONEY_PLAYERSHOP_WITHDRAW;	
				MoneyLogParam.Count		=	uAmount;
				MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
				MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				SaveMoneyLog(&MoneyLogParam);
			}
			break;
		default:
			break;
	}
	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopMoneyHandler::Name=%s"
		,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
