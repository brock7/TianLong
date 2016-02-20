#include "StdAfx.h"
#include "LCRetDeleteChar.h"
#include "gamestruct.h"
#include "..\..\event\GMEventSystem.h"
#include "..\..\procedure\GamePro_CharSel.h"

UINT LCRetDeleteCharHandler::Execute(LCRetDeleteChar* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION


		if(pPacket)
		{
			ASKDELETECHAR_RESULT res = pPacket->GetResult();
			switch(res)
			{
			case ASKDELETECHAR_SUCCESS:		//�ɹ�
				{
					CGameProcedure::s_pProcCharSel->SetCurSelDel(-1);
					//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "�õ���ɫ��Ϣ����! ");	
					break;
				}
			case ASKDELETECHAR_SERVER_BUSY://������æ������
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "������æ������! ");	
					break;
				}
			case ASKDELETECHAR_OP_TIMES:		//��������Ƶ��
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "�ٴ�ɾ��һ����ɫ��Ҫ15���ӣ�");
					break;
				}
			case ASKDELETECHARR_EMPTY:		//û�н�ɫɾ��
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "û�н�ɫɾ��! ");	
					break;
				}
			case ASKDELETECHAR_OP_ERROR:		//�����������
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "�����������! ");	
					break;
				}
			case ASKDELETECHAR_INTERNAL_ERROR: //�ڲ�����
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "�ڲ�����! ");	
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