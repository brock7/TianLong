#include "stdafx.h"

#include "BLRetAuth.h"
#include "LoginPlayer.h"
#include "ServerPlayer.h"
#include "PlayerPool.h"
#include "LoginPlayerManager.h"
#include "LCRetLogin.h"
#include "ServerManager.h"
#include "ProcessManager.h"
#include "ProcessPlayerManager.h"
#include "TurnPlayerQueue.h"
#include "Log.h"
#include "TimeManager.h"


#define MAX_WRONGPW_TIMES 3

UINT BLRetAuthHandler::Execute(BLRetAuth* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	PlayerID_t PlayerID ;
	LoginPlayer* pLoginPlayer = NULL ;
	Assert( pPlayer ) ;
	if( pPlayer->IsServerPlayer() )
	{
		PlayerID = pPacket->GetPlayerID();

		pLoginPlayer = g_pPlayerPool->GetPlayer(PlayerID) ;
		if( pLoginPlayer==NULL )
		{
			Assert(FALSE) ;
			return PACKET_EXE_CONTINUE ;
		}
	}
	else if( pPlayer->IsLoginPlayer() )
	{
		PlayerID = pPlayer->PlayerID() ;
		pLoginPlayer = (LoginPlayer*)pPlayer ;
		
		if( pLoginPlayer==NULL )
		{
			Assert(FALSE) ;
			return PACKET_EXE_CONTINUE ;
		}
	}
	else
	{
		Assert(FALSE) ;
		return PACKET_EXE_CONTINUE ;
	}

	TID CurrentThreadID = MyGetCurrentThreadID();
	
	if(CurrentThreadID == g_pServerManager->m_ThreadID)
	{
		if(pLoginPlayer->GetPlayerStatus() == PS_LOGIN_WAIT_AUTH)
		{   //ServerManager ��ִ��
			g_pLoginPlayerManager->SendPacket(pPacket,PlayerID);
			return PACKET_EXE_NOTREMOVE; 
		}
		return PACKET_EXE_CONTINUE;
	}
	else if ( CurrentThreadID == g_pLoginPlayerManager->m_ThreadID)
	{

		//���GUID �Ƿ���ȷ
		if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount())!= 0)
		{
			//Ӧ����һ�δ������
			Log::SaveLog(LOGIN_LOGFILE, "ERROR: BLRetAuthHandler::Execute Errors,acc = %s,Packet acc = %s",
				pLoginPlayer->GetAccount(),pPacket->GetAccount()) ;
			return PACKET_EXE_CONTINUE;
		}

		if (pLoginPlayer->GetPlayerStatus() == PS_LOGIN_WAIT_AUTH)
		{
			if(pPacket->GetResult() == LOGINR_SUCCESS)
			{
				pLoginPlayer->SetAccount(pPacket->GetAccount());
				pLoginPlayer->SetPlayerStatus(PS_LOGIN_AUTHED);
				pLoginPlayer->SetPlayerAge( (BYTE)pPacket->GetAge() );
				//�ѿͻ��˴ӵ�ǰLoginPlayerManager ���ͷ�
				g_pLoginPlayerManager->DelPlayer(pLoginPlayer->PlayerID());
				//������ProcessManager ������Ϣ
				g_pProcessManager->SendPacket(pPacket,pLoginPlayer->PlayerID());
				//�޸Ŀͻ���״̬Ϊ PS_LOGIN_WAIT_PROCESS_TURN
				pLoginPlayer->SetPlayerStatus(PS_LOGIN_WAIT_PROCESS_TURN);
				return PACKET_EXE_NOTREMOVE ;
			}
			else
			{
				pLoginPlayer->SetAccount(pPacket->GetAccount());
				pLoginPlayer->SetWrongPWCount(pLoginPlayer->GetWrongPWCount()+1);
				LCRetLogin Msg;
				Msg.SetAccount(pLoginPlayer->GetAccount());
				Msg.SetResult(LOGINR_AUTH_FAIL);
				pLoginPlayer->SendPacket(&Msg);

				if(pLoginPlayer->GetWrongPWCount()>=MAX_WRONGPW_TIMES)
				{
					Log::SaveLog(LOGIN_LOGFILE, "ERROR: BLRetAuthHandler Reach Max Password Wrong Times acc=%s",
						pLoginPlayer->GetAccount()) ;
					return PACKET_EXE_ERROR;
				}
				else
					return PACKET_EXE_CONTINUE;
			}
			
		}
	}
	else if (CurrentThreadID == g_pProcessPlayerManager->m_ThreadID )
	{
		//���GUID �Ƿ���ȷ
		if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount())!= 0)
		{
			//Ӧ����һ�δ������
			Log::SaveLog(LOGIN_LOGFILE, "ERROR: BLRetAuthHandler::Execute Process Errors,acc = %s,Packet acc = %s",
				pLoginPlayer->GetAccount(),pPacket->GetAccount()) ;
			return PACKET_EXE_CONTINUE;
		}

		//���뵽g_pProcessPlayerManager �У��ܹ������Ӧ��HeartBeat
		g_pProcessPlayerManager->AddPlayer(pLoginPlayer);
		//�ȴ���login ���
		LCRetLogin Msg;
		Msg.SetAccount(pLoginPlayer->GetAccount());
		Msg.SetResult(LOGINR_SUCCESS);
		pLoginPlayer->SendPacket(&Msg);
		UINT QueuePos;
		//���뵽g_pProcessPlayerQueue �У��ܹ��Կͻ����Ŷ�
		if(g_pProcessPlayerQueue->AddInPlayer(pLoginPlayer->PlayerID(),
			pLoginPlayer->GetAccount(),QueuePos))
		{
			pLoginPlayer->SetQueuePos(QueuePos);
			//���õ�ǰ���״̬
			pLoginPlayer->SetPlayerStatus(PS_LOGIN_PROCESS_TURN);
			pLoginPlayer->SetLastSendTurnTime(g_pTimeManager->CurrentTime());	
			//���Ϳ�ʼ�Ŷ���Ϣ(HeartBeat ��ִ�У�
		}
		else
		{	//�Ŷ���Ҷ�����MAX_TURN_PLAYER��
			//����ֻ�ܶϿ�����ҵ���������
			BOOL boo = pLoginPlayer->FreeOwn() ;
			Log::SaveLog( LOGIN_LOGFILE, "ERROR: BLRetAuthHandler::FreeOwn " ) ;
			Assert( boo ) ;
			return PACKET_EXE_ERROR ;

		}
	}
	
	Log::SaveLog( LOGIN_LOGFILE, "BLRetAuthHandler::Execute()....OK! " ) ;

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}