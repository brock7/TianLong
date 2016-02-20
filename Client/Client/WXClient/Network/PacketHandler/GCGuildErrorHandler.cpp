#include "StdAfx.h"
#include "GCGuildError.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"

UINT GCGuildErrorHandler::Execute( GCGuildError* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CHAR szText[_MAX_PATH];

		_snprintf(szText, _MAX_PATH, "����������");

		switch( pPacket->GetErrorCode() )
		{
		case GUILD_ERROR_WANTING			:
			{
				_snprintf(szText, _MAX_PATH, "�����㴴��������");
			}
			break;
		case GUILD_ERROR_INVALID_NAME		:
			{
				_snprintf(szText, _MAX_PATH, "������ƷǷ���");
			}
			break;
		case GUILD_ERROR_DUPLICATED_NAME	:
			{
				_snprintf(szText, _MAX_PATH, "������������������ظ���");
			}
			break;
		case GUILD_ERROR_GUILD_FULL			:
			{
				_snprintf(szText, _MAX_PATH, "�޷����������ᡣ");
			}
			break;
		case GUILD_ERROR_MEMBER_FULL		:
			{
				_snprintf(szText, _MAX_PATH, "�����Ա�ˡ�");
			}
			break;
		case GUILD_ERROR_PROPOSER_FULL		:
			{
				_snprintf(szText, _MAX_PATH, "������������");
			}
			break;
		case GUILD_ERROR_IN_GUILD			:
			{
				_snprintf(szText, _MAX_PATH, "���Ѽ����ᡣ");
			}
			break;
		case GUILD_ERROR_NOT_EXIST			:
			{
				_snprintf(szText, _MAX_PATH, "��᲻���ڡ�");
			}
			break;
		case GUILD_ERROR_UNAUTHORIZED		:
			{
				_snprintf(szText, _MAX_PATH, "û��Ȩ�ޡ�");
			}
			break;
		case GUILD_ERROR_NO_VACANCY			:
			{
				_snprintf(szText, _MAX_PATH, "û��ְλ��ȱ��");
			}
			break;
		case GUILD_ERROR_NO_QUALIFICATION	:
			{
				_snprintf(szText, _MAX_PATH, "��������Ա��ְλ�������ʸ�");
			}
			break;
		case GUILD_ERROR_CASHBOX_FULL		:
			{
				_snprintf(szText, _MAX_PATH, "�����������");
			}
			break;
		case GUILD_ERROR_POS_FULL		:
			{
				_snprintf(szText, _MAX_PATH, "���ְλ��������������������");
			}
			break;
		case GUILD_ERROR					:
			{
				_snprintf(szText, _MAX_PATH, "δ֪������");
			}
			break;
		case GUILD_ERROR_GUILD_ALREADY_EXIST:
			{
				_snprintf(szText, _MAX_PATH, "����Ѿ����ڡ�");
			}
			break;
		case GUILD_ERROR_ALREADY_IN_PROPOSER_LIST:
			{
				_snprintf(szText, _MAX_PATH, "���Ѿ��������б����ˡ�");
			}
			break;
		case GUILD_ERROR_INVALID_CAMP:
			{
				_snprintf(szText, _MAX_PATH, "��Ҫ����İ����������һ����Ӫ��");
			}
			break;
		default:
			Assert(FALSE);
			return 0;
		}

		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, szText );
//		CGameProcedure::s_pEventSystem->PushEvent( GE_CHAT_MESSAGE, "���", szText, 0 );
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
