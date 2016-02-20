#include "stdafx.h"

#include "LCRetCharLogin.h"
#include "LoginPlayer.h"
#include "ProcessPlayerManager.h"
#include "Log.h"

UINT LCRetCharLoginHandler::Execute(LCRetCharLogin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	
	if(MyGetCurrentThreadID() == g_pProcessPlayerManager->m_ThreadID)
	{	//��DB->ProcessPlayerManager �������ݲ������
		LoginPlayer*	pLoginPlayer = (LoginPlayer*)pPlayer ;
		if( pLoginPlayer==NULL )
		{
			Assert(FALSE) ;
			return PACKET_EXE_CONTINUE ;
		}
		pLoginPlayer->SendPacket(pPacket);
		Log::SaveLog(LOGIN_LOGFILE,"LCRetCharLoginHandler::Execute() ....OK");
	}
	else
	{
		AssertEx(FALSE,"LCRetCharLoginHandler �߳���Դִ�д���!");
	}
	
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}