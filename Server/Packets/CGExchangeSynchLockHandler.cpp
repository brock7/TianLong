#include "stdafx.h"
/*
�ͻ���֪ͨ������ͬ���Լ������仯���Է��ͻ���
*/

#include "CGExchangeSynchLock.h"
#include "GCExchangeSynchLock.h"
#include "GCExchangeSynchConfirmII.h"
#include "GCExchangeError.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

UINT CGExchangeSynchLockHandler::Execute( CGExchangeSynchLock* pPacket, Player* pPlayer )
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

	//��֤
	EXCHANGE_CERTIFY_EACH_OTHER(pHuman)
	Obj_Human* pDestHuman = pScene->GetHumanManager()->GetHuman( pHuman->m_ExchangBox.m_ObjID );

	if(pPacket->GetLockMyself())
	{//���Լ�����
		pHuman->m_ExchangBox.m_IsLocked = TRUE;
		if(pDestHuman->m_ExchangBox.m_IsLocked == TRUE)
		{//����Է��Ѿ�����
			//����˫����ʾȷ����ť
			pHuman->m_ExchangBox.m_CanConform = TRUE;
			pDestHuman->m_ExchangBox.m_CanConform = TRUE;

			//����ȴ����ȷ��״̬
			pHuman->m_ExchangBox.m_Status = ServerExchangeBox::EXCHANGE_WAIT_FOR_CONFIRM;
			pDestHuman->m_ExchangBox.m_Status = ServerExchangeBox::EXCHANGE_WAIT_FOR_CONFIRM;

			//֪ͨ�Է��Լ��Ѿ�����
			GCExchangeSynchLock Msg;
			Msg.SetIsMyself(FALSE);
			Msg.SetIsLocked(TRUE);
			pDestHuman->GetPlayer()->SendPacket(&Msg);

			//֪ͨ˫����ʾȷ����ť
			GCExchangeSynchConfirmII MsgToSelf, MsgToDes;
			MsgToSelf.SetIsEnable(TRUE);
			pHuman->GetPlayer()->SendPacket(&MsgToSelf);

			MsgToDes.SetIsEnable(TRUE);
			pDestHuman->GetPlayer()->SendPacket(&MsgToDes);
		}
		else
		{//�Է���û�м���
			//֪ͨ�Է��Լ��Ѿ�����
			GCExchangeSynchLock MsgToDes;
			MsgToDes.SetIsMyself(FALSE);
			MsgToDes.SetIsLocked(TRUE);
			pDestHuman->GetPlayer()->SendPacket(&MsgToDes);
		}

	}
	else
	{//���Լ�����
		pHuman->m_ExchangBox.m_IsLocked = FALSE;
		if(pDestHuman->m_ExchangBox.m_IsLocked == TRUE)
		{//����Է��Ѿ�����
			pDestHuman->m_ExchangBox.m_IsLocked = FALSE;
			GCExchangeSynchLock MsgToDes, MsgToSelf;

			//֪ͨ�Է�����˫����
			MsgToDes.SetIsBoth(TRUE);
			MsgToDes.SetIsLocked(FALSE);
			pDestHuman->GetPlayer()->SendPacket(&MsgToDes);
			
			//֪ͨ�Լ���Է���
			MsgToSelf.SetIsMyself(FALSE);
			MsgToSelf.SetIsLocked(FALSE);
			pHuman->GetPlayer()->SendPacket(&MsgToSelf);

		}
		else
		{//�Է�û�м�����ֱ��ͬ��
			GCExchangeSynchLock MsgToDes;
			MsgToDes.SetIsMyself (FALSE);
			MsgToDes.SetIsLocked(FALSE);
			pDestHuman->GetPlayer()->SendPacket(&MsgToDes);
		}

		//ǿ�л�״̬
		pHuman->m_ExchangBox.m_Status = ServerExchangeBox::EXCHANGE_SYNCH_DATA;
		pDestHuman->m_ExchangBox.m_Status = ServerExchangeBox::EXCHANGE_SYNCH_DATA;
		
		//�ر�ȷ����ť
		if(pHuman->m_ExchangBox.m_CanConform)
		{//֪ͨ�ͻ���
			pHuman->m_ExchangBox.m_CanConform = FALSE;
			GCExchangeSynchConfirmII MsgToSelf;
			MsgToSelf.SetIsEnable(FALSE);
			pHuman->GetPlayer()->SendPacket(&MsgToSelf);
		}

		//�ر�ȷ����ť
		if(pDestHuman->m_ExchangBox.m_CanConform)
		{//֪ͨ�ͻ���
			pDestHuman->m_ExchangBox.m_CanConform = FALSE;
			GCExchangeSynchConfirmII MsgToDes;
			MsgToDes.SetIsEnable(FALSE);
			pDestHuman->GetPlayer()->SendPacket(&MsgToDes);
		}
	}


	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
