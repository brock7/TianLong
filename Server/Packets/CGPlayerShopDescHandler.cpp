#include "stdafx.h"

#include "CGPlayerShopDesc.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCPlayerShopError.h"

UINT CGPlayerShopDescHandler::Execute( CGPlayerShopDesc* pPacket, Player* pPlayer )
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
	_PLAYERSHOP_GUID	nShopID		=	pPacket->GetShopID();				//�̵�ID
	BYTE	PlayerShopDescSize		=	pPacket->GetPlayerShopDescSize();	//�̵������ĳ���
	CHAR*	PlayerShopDesc			=	pPacket->GetPlayerShopDesc();		//�̵�����������

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
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopDescHandler::Name=%s shop close"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;
	}

	pPlayerShop->SetShopDesc(PlayerShopDesc, PlayerShopDescSize);

	//֪ͨ�ͻ����̵������ɹ�
	GCPlayerShopError Msg;
	Msg.SetID(PLAYERSHOP_MSG::ERR_SHOP_SUCCESS_CHANGE_DESC);
	pHuman->GetPlayer()->SendPacket(&Msg);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopDescHandler::Name=%s"
		,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
