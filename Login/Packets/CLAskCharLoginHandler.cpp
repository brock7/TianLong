#include "stdafx.h"

#include "CLAskCharLogin.h"
#include "LoginPlayer.h"
#include "ProcessPlayerManager.h"
#include "TimeManager.h"
#include "DBLogicManager.h"
#include "LCRetCharLogin.h"
#include "DBCharFullData.h"
#include "DB_Struct.h"
#include "LWAskCharLogin.h"
#include "ServerManager.h"
#include "TurnPlayerQueue.h"
#include "Log.h"
#include "DBThreadManager.h"


UINT CLAskCharLoginHandler::Execute(CLAskCharLogin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	
	TID CurrentThreadID = MyGetCurrentThreadID();
	
	if(CurrentThreadID== g_pProcessPlayerManager->m_ThreadID)
	{
		LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);
		Assert(pLoginPlayer);

		if(pLoginPlayer->GetPlayerStatus()!=PS_LOGIN_NORMAL)
		{
			return PACKET_EXE_CONTINUE;
		}
		
		UINT uTime = g_pTimeManager->CurrentTime();
		if(uTime<pLoginPlayer->m_LastDBOpTime+DB_OPERATION_TIME)
		{
			//�û���������Ƶ��
			LCRetCharLogin Msg;
			Msg.SetResult(ASKCHARLOGIN_OP_TIMES);
			pLoginPlayer->SendPacket(&Msg);

			Log::SaveLog( LOGIN_LOGFILE, "CLAskCharLoginHandler::Execute()....Fails!,code = ASKCHARLOGIN_OP_TIMES") ;
			return PACKET_EXE_CONTINUE;
		}
		GUID_t	TryLoginGuid = pPacket->GetCharGuid();
		if(!pLoginPlayer->IsGUIDOwner(TryLoginGuid))
		{
			//�û������Ƿ�,�û����������ɫ��������
			LCRetCharLogin Msg;
			Msg.SetResult(ASKCHARLOGIN_NOT_OWNER);
			pLoginPlayer->SendPacket(&Msg);
			Log::SaveLog( LOGIN_LOGFILE, "CLAskCharLoginHandler::Execute()....Fails!,code = ASKCHARLOGIN_NOT_OWNER") ;
			return PACKET_EXE_CONTINUE;
		}

		//����Ƿ��ǺϷ�SID
		
		//if(g_DBSceneTable.isValidSceneID(pPacket->GetSceneID(),CT_NORMAL))
		//{
		//	pLoginPlayer->SetChooseSceneID(pPacket->GetSceneID());
		//}
		//else
		//{
		//	LCRetCharLogin Msg;
		//	Msg.SetResult(ASKCHARLOGIN_NOT_OWNER);
		//	pLoginPlayer->SendPacket(&Msg);
		//	DiskLog(LOGIN_LOGFILE, "CLAskCharLoginHandler::Execute()....Choose Scene error,acc=%s,guid=%X",
		//	pLoginPlayer->GetAccount(),pPacket->GetCharGuid()) ;
		//	return PACKET_EXE_CONTINUE;
		//}

		pLoginPlayer->SetUserKey(rand());
		pPacket->SetPlayerID(pLoginPlayer->PlayerID());
		if(g_pDBThreadManager->SendPacket(pPacket,pLoginPlayer->PlayerID()))
		{   //����ɹ�������Ϣ���͵�DB����
			//���޸����ݿ����ʱ��Ĳ����ƶ���ʵ�����ݿ�������еĲ���
			return PACKET_EXE_NOTREMOVE;	
		}
		else
		{//DB ѹ���������û����³���
			LCRetCharLogin Msg;
			Msg.SetResult(ASKCHARLOGIN_SERVER_BUSY);
			pLoginPlayer->SendPacket(&Msg);
			pLoginPlayer->m_LastDBOpTime = uTime;
			Log::SaveLog( LOGIN_LOGFILE, "CLAskCharLoginHandler::Execute()....Fails!, code = ASKCHARLOGIN_SERVER_BUSY") ;

			return PACKET_EXE_CONTINUE;
		}
	}
	else if(g_pDBThreadManager->IsPoolTID(CurrentThreadID))
	{//����Ҽ��뵽World ������
		
		PlayerID_t	PlayerID			= pPacket->GetPlayerID();
		GUID_t	    PlayerCharGUID		= pPacket->GetCharGuid();
		//SceneID_t	PlayerSceneID			= pPacket->GetSceneID();

		UINT        QueuePos;

		LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);
		Assert(pLoginPlayer);

		GUID_t	TryLoginGuid = pPacket->GetCharGuid();
		if(!pLoginPlayer->IsGUIDOwner(TryLoginGuid))
		{
			//һ�δ�������ݲ���
			Log::SaveLog(LOGIN_LOGFILE, "CLAskCharLoginHandler Wrong operation,acc=%s,guid=%X",
				pLoginPlayer->GetAccount(),pPacket->GetCharGuid()) ;
			return PACKET_EXE_CONTINUE;
		}
	
		
		if(g_pWorldPlayerQueue->AddInPlayer(PlayerID,
			pLoginPlayer->GetAccount(),
			pPacket->GetCharGuid(),
			pLoginPlayer->GetPlayerAge(),
			QueuePos))
		{ //����ɹ�
			
			Log::SaveLog( LOGIN_LOGFILE, "CLAskCharLoginHandler::Execute()....OK!, PlayerCharGUID = %X ",PlayerCharGUID ) ;

			return PACKET_EXE_CONTINUE;
			
		}
		else //����ʧ��
		{
			//queue ������,World���� ,���ǲ�Ӧ�ó��֣���ΪWorld״̬�Ѿ�֪ͨ��Login
			//���Ҳ���������Ҷ����Ŷ�
			Log::SaveLog( LOGIN_LOGFILE, "CLAskCharLoginHandler::Execute()....Fail,World is Full,PlayerCharGUID = %X ",PlayerCharGUID ) ;
		}
		
	}
	else
	{
		Log::SaveLog( LOGIN_LOGFILE, "ERROR::CLAskCharLoginHandler::Execute() ... Fail, Invalid Thread Handler") ;
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}
	
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
