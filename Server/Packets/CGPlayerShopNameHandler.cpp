#include "stdafx.h"

#include "CGPlayerShopName.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCPlayerShopError.h"

UINT CGPlayerShopNameHandler::Execute( CGPlayerShopName* pPacket, Player* pPlayer )
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
	_PLAYERSHOP_GUID	nShopID				=	pPacket->GetShopID();				//�̵�ID
	BYTE				PlayerShopNameSize	=	pPacket->GetPlayerShopNameSize();	//�̵�����
	CHAR*				PlayerShopName		=	pPacket->GetPlayerShopName();		//����̵���

	PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
	PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
	Assert(pPlayerShop);

	//�ǲ����Լ��ĵ�.ֻ�е������ʸ���Ĵ�����
	BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;
	if(bIsMine == FALSE )
	{
		//log...
		return PACKET_EXE_CONTINUE;
	}


	if(pPlayerShop->GetShopStatus() == STATUS_PLAYER_SHOP_ON_SALE)
	{//�̵�δ����
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopNameHandler::Name=%s shop close"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;
	}

	//���������ϵĽ�Ǯ��10�� *��ҵָ�����޸ĳɹ���
	pPlayerShopManager->ClampComFactor();
	FLOAT fNeedMoney = (FLOAT)100000.0*pPlayerShopManager->GetComFactor();
	if(pHuman->GetMoney()<(UINT)fNeedMoney)
	{
		//֪ͨ�ͻ��˹����̵�ɹ�
		GCPlayerShopError Msg;
		Msg.SetID(PLAYERSHOP_MSG::ERR_SHOP_NOT_ENOUTH_MONEY_TO_CHANGE_NAME);
		pHuman->GetPlayer()->SendPacket(&Msg);
	}

	//��Ǯ
	pHuman->SetMoney(pHuman->GetMoney() - (UINT)fNeedMoney);
	
	//���ĵ���
	pPlayerShop->SetShopName(PlayerShopName, PlayerShopNameSize);

	//���ĳƺ�
	CHAR	szMsgTitle[256] = {0};
	sprintf(szMsgTitle, "%s���ƹ�", PlayerShopName);
	pHuman->SetShangDianName(szMsgTitle,(BYTE)strlen(szMsgTitle));
	pHuman->UpdateTitlesToClient();
	//֪ͨ�ͻ����̵����޸ĳɹ�
	GCPlayerShopError Msg;
	Msg.SetID(PLAYERSHOP_MSG::ERR_SHOP_SUCCESS_CHANGE_NAME);
	pHuman->GetPlayer()->SendPacket(&Msg);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopNameHandler::Name=%s"
		,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
