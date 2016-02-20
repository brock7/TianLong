#include "stdafx.h"

#include "CGPlayerShopOpenStall.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCPlayerShopOpenStall.h"
#include "GCPlayerShopError.h"
#include "RecordOpt.h"

using namespace PLAYER_SHOP;
UINT CGPlayerShopOpenStallHandler::Execute( CGPlayerShopOpenStall* pPacket, Player* pPlayer )
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
	_PLAYERSHOP_GUID	nShopID			=	pPacket->GetShopID();			//�̵�ID
	BYTE				nStallIndex		=	pPacket->GetStallIndex();		//��̨����
	BYTE				bOpen			=	pPacket->GetOpen();				//�Ƿ���
	BYTE				nShopSerial		=	pPacket->GetShopSerial();
	GCPlayerShopError MsgError;
	PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
	PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
	Assert(pPlayerShop);
	if(pPlayerShop->GetShopStatus() == STATUS_PLAYER_SHOP_ON_SALE)
	{//�̵�δ����
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopOpenStallHandler::Name=%s shop close"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;
	}

	//�ǲ����Լ��ĵ�
	BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;

	//�ǲ����Լ����Թ���ĵ�
	BOOL bCanManager = pPlayerShop->IsPartner(pHuman->GetGUID());

	if(bIsMine == FALSE)
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

	PlayerStallBox* pPlayerStallBox = pPlayerShop->GetPlayerStallBoxByIndex(nStallIndex);
	BOOL	bRefresh = FALSE;
	if(pPlayerStallBox->GetStallStatus() == STALL_CLOSE && bOpen)
	{
		pPlayerStallBox->SetStallStatus(STALL_OPEN);
		BYTE nStallOnSale = pPlayerShop->GetNumStallOnSale();
		pPlayerShop->SetNumStallOnSale(nStallOnSale+1);
		bRefresh = TRUE;
		//���ţ�AAA�����˵�N���̨��AAAΪ����������NΪ��̨����������
		RecordOpt::Excute(REC_OPEN, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)nStallIndex);
		
	}
	else if(pPlayerStallBox->GetStallStatus() == STALL_OPEN && !bOpen)
	{
		pPlayerStallBox->SetStallStatus(STALL_CLOSE);
		BYTE nStallOnSale = pPlayerShop->GetNumStallOnSale();
		pPlayerShop->SetNumStallOnSale(nStallOnSale-1);
		bRefresh = TRUE;
		//���ȣ�AAA�����˵�N���̨��AAAΪ����������NΪ��̨����������
		RecordOpt::Excute(REC_CLOSE, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)nStallIndex);
	}

	if(bRefresh)
	{
		GCPlayerShopOpenStall MsgToClient;
		MsgToClient.SetShopID(nShopID);
		MsgToClient.SetStallIndex(nStallIndex);
		MsgToClient.SetOpen(bOpen);
		MsgToClient.SetShopSerial(pPlayerShop->IncSerial());
		pGamePlayer->SendPacket(&MsgToClient);
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopOpenStallHandler::Name=%s"
		,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
