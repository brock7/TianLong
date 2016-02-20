#include "stdafx.h"

#include "CLAskLogin.h"
#include "LoginPlayer.h"
#include "LoginPlayerManager.h"
#include "Log.h"
#include "LCRetLogin.h"
#include "BLRetAuth.h"
#include "LBAskAuth.h"
#include "ServerManager.h"
#include "PacketFactoryManager.h"


UINT	CLAskLoginHandler::Execute(CLAskLogin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		//����߳�ִ����Դ�Ƿ���ȷ
		Assert( MyGetCurrentThreadID()== g_pLoginPlayerManager->m_ThreadID) ;

	LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);

	Assert(pLoginPlayer);
	
	//����û��Ƿ�ָ��״̬
	//ֻ�и����ӽ�����û��ſ��Է������Ϣ
	if(pLoginPlayer->GetPlayerStatus()!= PS_LOGIN_CONNECT)
	{
		return PACKET_EXE_ERROR;
	}

	if(pPacket->GetVersion()!= CURRENT_LOGIN_VERSION)
	{
		LCRetLogin Msg;
		Msg.SetAccount(pPacket->GetAccount());
		Msg.SetResult(LOGINR_VERSION_FAIL);
		pLoginPlayer->SendPacket(&Msg);
		Log::SaveLog( LOGIN_LOGFILE, "CLAskLoginHandler::Execute()....Client version is't compatible,current version= %d,Account = %s ",
			CURRENT_LOGIN_VERSION,pLoginPlayer->GetAccount() ) ;
		return PACKET_EXE_CONTINUE;
	}

	CHAR	Account[MAX_ACCOUNT+1];
	strncpy(Account,pPacket->GetAccount(),MAX_ACCOUNT);
	Account[MAX_ACCOUNT] = '\0';
	if(strlen(Account) == 0||!StrSafeCheck(Account,MAX_ACCOUNT+1))
	{
		LCRetLogin Msg;
		Msg.SetAccount(pPacket->GetAccount());
		Msg.SetResult(LOGINR_AUTH_FAIL);
		pLoginPlayer->SendPacket(&Msg);
		Log::SaveLog( LOGIN_LOGFILE, "CLAskLoginHandler::Execute()....Account is empty ",
			CURRENT_LOGIN_VERSION,pLoginPlayer->GetAccount() ) ;
		return PACKET_EXE_CONTINUE;
		
	}
	
	//���û��ͻ��˰汾��¼ס
	pLoginPlayer->SetVersion(pPacket->GetVersion());
	pLoginPlayer->SetPlayerStatus(PS_LOGIN_WAIT_AUTH);
	pLoginPlayer->SetAccount(pPacket->GetAccount());


	if (g_Config.m_LoginInfo.m_AskAuthType != _LOGIN_INFO::USE_BILLING)
	{
		;
	}
	
	//����Ϣ���͵�BillingSystem,������״̬ΪPS_LOGIN_WAIT_AUTH
	LBAskAuth* pMsg = (LBAskAuth*)g_pPacketFactoryManager->CreatePacket(PACKET_LB_ASKAUTH);
	if(!pMsg)
	{
		AssertEx(FALSE,"���� LBAskAuth ��Ϣʧ��");
	}

	pMsg->SetAccount(pPacket->GetAccount());
	pMsg->SetPassWord(pPacket->GetPassWord());
	pMsg->SetPlayerID(pLoginPlayer->PlayerID());
	g_pServerManager->SendPacket(pMsg,BILLING_PLAYER_ID);

	Log::SaveLog( LOGIN_LOGFILE, "CLAskLoginHandler::Execute(...) Account:%s ...OK ", pPacket->GetAccount()) ;
		
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}