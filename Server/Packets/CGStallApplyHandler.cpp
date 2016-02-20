#include "stdafx.h"
/*
�ͻ��������̯
*/

#include "CGStallApply.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCStallApply.h"

UINT CGStallApplyHandler::Execute( CGStallApply* pPacket, Player* pPlayer )
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
	//��ñ���˰��
	UINT	PosTax		= 0;
	BYTE	TradeTax	= 0;
	GCStallApply	Msg;

	//��֤�Ƿ����ʸ��̯
	if(pHuman->GetLevel()<30)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallApplyHandler::ObjID=%d level = %d ", pHuman->GetID(), pHuman->GetLevel()) ;
		return PACKET_EXE_CONTINUE ;
	}

	//��̯�ص��ж�
	StallInfoManager* pStallInfoMgr = pScene->GetStallInfoManager();
	if(pStallInfoMgr)
	{
		UINT PosX = (UINT)pHuman->getWorldPos()->m_fX;
		UINT PosZ = (UINT)pHuman->getWorldPos()->m_fZ;
		BOOL bCanStall = pStallInfoMgr->CanStall(PosX, PosZ);
		if(bCanStall == FALSE)
		{
			Msg.SetIsCanStall(FALSE);
		}
		else
		{
			PosTax		= pStallInfoMgr->StallPosPayment(PosX, PosZ);
			TradeTax	= pStallInfoMgr->StallExchangeTax(PosX, PosZ);

			pHuman->m_StallBox.CleanUp();
			pHuman->m_StallBox.SetStallStatus(ServerStallBox::STALL_READY);
			pHuman->m_StallBox.GetBBS()->LoadTitle();

			Msg.SetIsCanStall(TRUE);
			Msg.SetPosTax(PosTax);
			Msg.SetTradeTax(TradeTax);
		}
	}

	//��֤���,֪ͨ�ͻ���ȷ��
	pGamePlayer->SendPacket(&Msg);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGStallApplyHandler::ObjID=%d ", pHuman->GetID()) ;
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
