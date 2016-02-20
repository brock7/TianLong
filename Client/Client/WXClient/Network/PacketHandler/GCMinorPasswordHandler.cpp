#include "StdAfx.h"
#include "GCMinorPasswd.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\event\GMEventSystem.h"
#include "..\..\procedure\GamePro_CharSel.h"
#include "..\..\procedure\GamePro_CharCreate.h"

UINT GCMinorPasswdHandler::Execute( GCMinorPasswd* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GC_MINORPASSWD* pRecv;
	pRecv = pPacket->GetMinorPassword();

	switch( pRecv->m_Type )
	{
	case MRETT_PASSWDSETUP:
		{
			
			if(1 == pRecv->m_ReturnSetup.m_uFlag)
			{// �Ѿ����ù�������
				
				// ��������ɹ�
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdSetup(TRUE);
				// ����δ��������״̬����
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdUnlocked(FALSE);
			}
			else
			{// û�����ù�������
				
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdSetup(INVALID_ID);
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdUnlocked(INVALID_ID);
			}
			break;
		}
	case MRETT_DELETEPASSWDTIME:
		{

			TCHAR bufInfo[512];
			_stprintf(bufInfo, "���Ѿ�������ǿ�ƽ������,����%dСʱ���ٴε��ǿ�ƽ��������ȷ��", (pRecv->m_ReturnDeleteTime.m_uTime / (1000 * 60 *60)));
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, bufInfo);	
			break;
		}
	case MRETT_SETPASSWDSUCC:
		{
			// ��������ɹ�
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdSetup(TRUE);

			// ����δ��������״̬����
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdUnlocked(FALSE);
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "���ö�������ɹ�");	
			break;
		}
	case MRETT_MODIFYPASSWDSUCC:
		{
			// �޸�����ɹ�
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "�޸�����ɹ�");	
			break;
		}
	case MRETT_UNLOCKPASSWDSUCC:
		{
			// ��������ɹ�
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdUnlocked(TRUE);
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "��������ɹ�");
			break;
		}
	case MRETT_DELETEPASSWDCANCEL:
		{
			break;
		}
	case MRETT_DELETEPASSWDSUCC:
		{
			// ǿ�ƽӴ�����ɹ���
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdSetup(FALSE);
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "ǿ�ƽ������ɹ�");	
			break;
		}
	case MRETT_ERR_SETPASSWDFAIL:
		{
			// ���ö�������ʧ��
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_SetMinorPwdSetup(FALSE);
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "���ö�������ʧ��");	

			break;
		}
	case MRETT_ERR_MODIFYPASSWDFAIL:
		{// �޸�����ʧ��
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "�޸�����ʧ��");	
			break;
		}
	case MRETT_ERR_UNLOCKPASSWDFAIL:
		{// ����ʧ�ܡ�
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "��������ʧ��");	
			break;
		}
	case MRETT_ERR_DELETEPASSWDFAIL:
		{
			// ǿ�ƽӴ�����ʧ�ܡ�
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "ǿ�ƽ������ʧ��");	
			break;
		}
	default :
		Assert(FALSE);
		return 0;
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
