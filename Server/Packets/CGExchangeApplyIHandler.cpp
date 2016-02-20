#include "stdafx.h"
/*
��֤����ĺϷ��ԣ���ת��
*/

#include "CGExchangeApplyI.h"
#include "GCExchangeApplyI.h"
#include "GCExchangeError.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

UINT CGExchangeApplyIHandler::Execute( CGExchangeApplyI* pPacket, Player* pPlayer )
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

	ObjID_t		TargetID = pPacket->GetObjID();
	Obj_Human* pSourceHuman = pHuman;//���׷�����
	Obj_Human* pDestHuman = pScene->GetHumanManager()->GetHuman( TargetID );//���׶���

	//��֤
	if( pDestHuman == NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}
	// ��ͬ��Ӫ�����ò鿴
	if( pSourceHuman->IsEnemy( pDestHuman ) )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: %s cann't ask %s's DetailAttr ", pSourceHuman->GetName(), pDestHuman->GetName() ) ;
		return	PACKET_EXE_CONTINUE;
	}

	INT iSettingData = pDestHuman->GetDB()->GetSetting(SETTING_TYPE_GAME)->m_SettingData;
	if(SETTINGFLAGISTRUE(iSettingData, GSF_REFUSE_TRADE))
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_REFUSE_TRADE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: ID = %d ERR_REFUSE_TRADE", pSourceHuman->GetID()) ;
		return PACKET_EXE_CONTINUE;
	}
	if(pSourceHuman->m_ExchangBox.m_Status >= ServerExchangeBox::EXCHANGE_SYNCH_DATA)
	{//���������ڽ�����
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_SELF_IN_EXCHANGE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: ID = %d status = %d", pSourceHuman->GetID(), pSourceHuman->m_ExchangBox.m_Status) ;
		return PACKET_EXE_CONTINUE;
	}
	if(pDestHuman->m_ExchangBox.m_Status >= ServerExchangeBox::EXCHANGE_SYNCH_DATA)
	{//Ŀ�����ڽ�����
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_TARGET_IN_EXCHANGE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: ID = %d status = %d", pDestHuman->GetID(),	pDestHuman->m_ExchangBox.m_Status ) ;
		return PACKET_EXE_CONTINUE;
	}

	//����
	//������Ϣ��Ŀ������
	GCExchangeApplyI Msg;
	Msg.SetObjID(pSourceHuman->GetID());
	pDestHuman->GetPlayer()->SendPacket(&Msg);
	g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: SOURCEID = %d DESTID = %d", pSourceHuman->GetID(), pDestHuman->GetID() ) ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
