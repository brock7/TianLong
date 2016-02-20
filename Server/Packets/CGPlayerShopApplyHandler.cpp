#include "stdafx.h"
/*
�ͻ��������̯
*/

#include "CGPlayerShopApply.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCPlayerShopApply.h"

UINT CGPlayerShopApplyHandler::Execute( CGPlayerShopApply* pPacket, Player* pPlayer )
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

	pScene->GetPlayerShopManager()->ClampComFactor();
	FLOAT CommercialFactor = pScene->GetPlayerShopManager()->GetComFactor();
	UINT Cost = (UINT)(300000*CommercialFactor*2*1.03);
	Cost = 10;

	GCPlayerShopError MsgError;

	//��֤�Ƿ����ʸ��̯
	//if(pHuman->GetLevel()<30)
	//{
	//	MsgError.SetID(PLAYERSHOP_MSG::ERR_NOT_ENOUGH_LEVEL);
	//	pGamePlayer->SendPacket(&MsgError);
	//	g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopApplyHandler::Name=%s Level = %d ", pHuman->GetName(), pHuman->GetLevel()) ;
	//	return PACKET_EXE_CONTINUE ;
	//}

	GCPlayerShopApply Msg;
	Msg.SetComFactor(CommercialFactor);
	Msg.SetCost(Cost);
	Msg.SetType(PLAYERSHOP_MSG::TYPE_ITEM);

	//��֤���,֪ͨ�ͻ���ȷ��
	pGamePlayer->SendPacket(&Msg);


	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
