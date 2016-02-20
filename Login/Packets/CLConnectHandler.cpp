#include "stdafx.h"

#include "CLConnect.h"
#include "LCRetConnect.h"
#include "LoginPlayerManager.h"
#include "ServerManager.h"
#include "LoginPlayer.h"
#include "LCRetConnect.h"
#include "Log.h"

UINT CLConnectHandler::Execute(CLConnect* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		//����߳�ִ����Դ�Ƿ���ȷ
		if(MyGetCurrentThreadID()!= g_pLoginPlayerManager->m_ThreadID)
		{
			return PACKET_EXE_CONTINUE;
		}

		LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);

		Assert(pLoginPlayer);
		
		//���Login ��������æ�̶ȣ�����һ����Ը���С��IP ��Port
		
		_SERVER_DATA* pLoginInfo	= g_pServerManager->GetCurrentServerInfo();
		Assert(pLoginInfo);

		LCRetConnect	Msg;
		Msg.SetResult(LOGINCR_SUCCESS);
		Msg.SetLoginIP(pLoginInfo->m_IP0);
		Msg.SetLoginPort(pLoginInfo->m_Port0);
		pLoginPlayer->SendPacket(&Msg);
		Log::SaveLog( LOGIN_LOGFILE, "CLConnectHandler::Execute(...)...OK,IP = %s,Port = %d",Msg.GetLoginIP(),Msg.GetLoginPort()) ;

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;

}