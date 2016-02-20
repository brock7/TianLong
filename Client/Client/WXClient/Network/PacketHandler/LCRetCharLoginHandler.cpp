#include "StdAfx.h"
#include "LCRetCharLogin.h"
#include "..\..\event\GMEventSystem.h"
#include "..\..\procedure\GamePro_CharSel.h"
#include "..\..\procedure\GamePro_CharCreate.h"
#include "..\..\procedure\Gamepro_ChangeScene.h"
#include "TDVariable.h"
UINT LCRetCharLoginHandler::Execute(LCRetCharLogin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION


		if(pPacket)
		{
			ASKCHARLOGIN_RESULT res;
			res = pPacket->GetResult();
			switch(res)
			{
			case ASKCHARLOGIN_SUCCESS:		//�ɹ�
				{
					UINT	iPort	  = pPacket->GetServerPort();
					CHAR*	pServerIP = pPacket->GetServerIP();
					UINT	iUserKey  = pPacket->GetUserKey();

					// ��¼������ip��ַ��ȫ�ֱ���
					CGameProcedure::s_pVariableSystem->SetVariable("GameServer_Address", pServerIP);

					// ��¼�������˿ں���ȫ�ֱ���
					CGameProcedure::s_pVariableSystem->SetAs_Int("GameServer_Port", iPort);
					
					// ��¼������Ϸ��ɫ��guid
					CGameProcedure::s_pVariableSystem->SetAs_Int("User_GUID", CGameProcedure::s_pProcCharSel->m_EnterGameGUID);

					// ���볡����user key
					CGameProcedure::s_pVariableSystem->SetAs_Int("GameServer_Key", iUserKey);

					// �л�����������������
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLEAR_ACCOUNT );
					CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcChangeScene);
					CGameProcedure::s_pProcChangeScene->SetStatus(CGamePro_ChangeScene::CHANGESCENE_DISCONNECT);

					break;
				}
			case ASKCHARLIST_WORLD_FULL:		//��������
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "����������!");	
					break;
				}
			case ASKCHARLOGIN_SERVER_BUSY:	//������æ������
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "������æ������! ");	
					break;
				}
			case ASKCHARLOGIN_LOADDB_ERROR:	//��ɫ�������
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "��ɫ�������! ");	
					break;
				}
			case ASKCHARLOGIN_OP_TIMES:		//��������Ƶ��
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "��������Ƶ��!");	
					break;
				}
			case ASKCHARLOGIN_NOT_OWNER:	//���ǽ�ɫ��������
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "���ǽ�ɫ��������! ");	
					break;
				}
			case ASKCHARLOGIN_SERVER_STOP:	//������ά��
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "������ά��! ");	
					break;
				}
			case ASKCHARLOGIN_CHANGE_SCENE:	//�л�����
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "��ɫ���л�����! ");	
					break;
				}
			default:
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "δ֪����! ");	
					break;
				}
			}
		}
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}
