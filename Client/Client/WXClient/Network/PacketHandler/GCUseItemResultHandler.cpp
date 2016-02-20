#include "StdAfx.h"
#include "GCUseItemResult.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\event\GMEventSystem.h"

UINT GCUseItemResultHandler::Execute(GCUseItemResult* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
 		 USEITEM_RESULT res = pPacket->GetResult();
		switch(res) 
		{
		case USEITEM_SUCCESS:
			//ҩƿʹ�óɹ�  how can i get that???
			break;
		case USEITEM_CANNT_USE:
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "����Ʒ������ʹ��");
			}
			break;
		case USEITEM_LEVEL_FAIL:
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "�ȼ�����");
			}
			break;
		case USEITEM_TYPE_FAIL:
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "��Ʒ���ʹ���!");
			}
			break;
		case USEITEM_TARGET_TYPE_FAIL:
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "Ŀ�����ʹ���!");
			}
			break;
		case USEITEM_SKILL_FAIL:
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "��Ʒ���Ӽ��ܴ���!");
			}
			break;
		case USEITEM_IDENT_TYPE_FAIL: //�������ʹ���
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "���ܼ���������Ʒ��");
			}
			break;
		case USEITEM_IDENT_TARGET_TYPE_FAIL://Ŀ�����ʹ���
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "���ܼ���������Ʒ��");
			}
			break;
		case USEITEM_IDENT_LEVEL_FAIL://����ȼ�����
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "��������ȼ�������");
			}
			break;
		default:
			break;
		}

	}

	// ȡ������״̬
	CDataPool::GetMe()->Identify_UnLock();

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}