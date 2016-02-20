#include "StdAfx.h"
#include "LCRetCreateChar.h"
#include "..\..\event\GMEventSystem.h"
#include "..\..\procedure\GamePro_CharCreate.h"

UINT	LCRetCreateCharHandler::Execute(LCRetCreateChar* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

			
		if(pPacket)
		{
			ASKCREATECHAR_RESULT	res = 	pPacket->GetResult();

			switch(res)
			{
			case ASKCREATECHAR_SUCCESS:			//�ɹ�
				{
					//CGameProcedure::s_pProcCharCreate->ChangeToRoleSel();

					CGameProcedure::s_pProcCharCreate->m_Status = CGamePro_CharCreate::CHAR_CREATE_CREATE_OK;
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CREATE_CLEAR_NAME );
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CREATE_ROLE_OK );
					break;
				}
			case ASKCREATECHAR_SERVER_BUSY:		//������æ������
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "������æ������! ");	
					break;
				}
			case ASKCREATECHAR_OP_TIMES:		//��������Ƶ��
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "�ٴδ���һ����ɫ��Ҫ15���ӣ�");
					break;
				}
			case ASKCREATECHAR_FULL:			//��ɫ�Ѿ�����
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "��ɫ�Ѿ�����! ");	
					break;
				}
			case ASKCREATECHAR_OP_ERROR:		//�����������
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "�����������! ");	
					break;
				}
			case ASKCREATECHAR_INTERNAL_ERROR:  //�ڲ�����
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "�ڲ�����! ");	
					break;
				}
			case ASKCREATECHAR_SAME_NAME:
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "��ɫ�����Ѿ�����");
					break;
				}
			case ASKCREATECHAR_INVALID_NAME:
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "��ɫ������");
					break;
				}
			default:							// δ֪����
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