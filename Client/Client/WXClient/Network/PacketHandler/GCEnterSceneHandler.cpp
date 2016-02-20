#include "StdAfx.h"
#include "GCEnterScene.h"
#include "..\..\Global.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Procedure\GamePro_Enter.h"
#include "..\..\Procedure\GamePro_Main.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\event\GMEventSystem.h"
#include "TDVariable.h"

UINT GCEnterSceneHandler::Execute( GCEnterScene* pPacket, Player* pPlayer )
{
	//AxTrace(0, 2, "GCEnterSceneHandler::Execute");

	//��ǰ�����ǵ�¼����
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcEnter)
	{
		//�������
		if(pPacket->getReturn() == 0)
		{
			//------------------------------------------------------------------
			//������������
			CGameProcedure::s_pVariableSystem->SetAs_Int("MySelf_ID", (INT)pPacket->getObjID());
			CGameProcedure::s_pVariableSystem->SetAs_Vector2("MySelf_Pos", pPacket->getEnterPos().m_fX, pPacket->getEnterPos().m_fZ);

			//�Ƿ�����ҳ���
			//-- for debug

			BOOL bUserCityMode	= pPacket->getIsCity();
			int nCitySceneID	= pPacket->getSceneID();
			int nCityLevel		= pPacket->getCityLevel();

			//-- for debug

			//------------------------------------------------------------------
			//����Ҫ����ĳ���
			if(bUserCityMode)
			{
				CGameProcedure::s_pProcEnter->SetSceneID(nCitySceneID, nCityLevel);
			}
			else                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
			{
				//��ͨ�������ڶ������������еȼ�������Ϊ-1
				CGameProcedure::s_pProcEnter->SetSceneID(pPacket->getSceneID(), -1);
			}
			//���õ�¼����״̬,ʹ֮������һ��״̬
			CGameProcedure::s_pProcEnter->SetStatus(CGamePro_Enter::ENTERSCENE_OK);
			//���볡��
			CGameProcedure::s_pProcEnter->EnterScene();
		}
		else
		{
			//���������
			CGameProcedure::s_pProcEnter->SetStatus(CGamePro_Enter::ENTERSCENE_FAILED );
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_CLOSE_NET, "���볡�������󱻷������ܾ�");	
		}
	}

	return PACKET_EXE_CONTINUE ;
}