#include "stdafx.h"

#include "SSScenePlayerCount.h"
#include "ServerManager.h"
#include "TurnPlayerQueue.h"
#include "Log.h"

UINT SSScenePlayerCountHandler::Execute(SSScenePlayerCount* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION	
	
		TID CurrentThreadID = MyGetCurrentThreadID();

		if(CurrentThreadID == g_pServerManager->m_ThreadID)
		{
			//�޸�g_WorldPlayerCounter ������
			Assert(pPacket);
			g_WorldPlayerCounter.m_WorldPlayerCount = pPacket->GetTotalPlayerCount();
			//�޸ĳ�����������
			//todo..
		}
		else
		{
			AssertEx(FALSE,"SSScenePlayerCountHandler �߳���Դִ�д���!");
		}
		
		Log::SaveLog(LOGIN_LOGFILE,"SSScenePlayerCountHandler::Execute()....OK,WorldPlayerCount = %d",
			g_WorldPlayerCounter.m_WorldPlayerCount);
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION	

		return PACKET_EXE_ERROR;
}