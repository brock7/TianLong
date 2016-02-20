#include "StdAfx.h"
#include "GCGuildReturn.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\Object\Character\Obj_Character.h"

UINT GCGuildReturnHandler::Execute( GCGuildReturn* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		_GUILD_RETURN* pReturn = pPacket->GetGuildReturn();
		CHAR szText[_MAX_PATH];

		_snprintf(szText, _MAX_PATH, "������������");

		switch( pReturn->m_ReturnType )
		{
		case GUILD_RETURN_RESPONSE:
			{
				_snprintf(szText, _MAX_PATH, "%s��Ӧ�����Ľ�����", pReturn->m_SourName);
			}
			break;
		case GUILD_RETURN_CREATE:
			{
				_snprintf(szText, _MAX_PATH, "�������ɹ���");
			}
			break;
		case GUILD_RETURN_JOIN:
			{
				_snprintf(szText, _MAX_PATH, "���Ѿ���%s��ᷢ�������룬��ȴ��ð�ᴦ��");
			}
			break;
		case GUILD_RETURN_EXPEL:
			{
				STRING strTemp = "";
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{//�Լ����߳����
					CDataPool::GetMe()->Guild_ClearMemInfo();
					CDataPool::GetMe()->Guild_ClearDetailInfo();
					//ˢ�½���
					CEventSystem::GetMe()->PushEvent(GE_GUILD_FORCE_CLOSE);
					strTemp = "%s��%s�����������";
					_snprintf(szText, _MAX_PATH, strTemp.c_str(), pReturn->m_SourName, pReturn->m_DestName);
				}
				else
				{//���˿���
					INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
					for(INT i = 0; i<iMemNum; i++)
					{
						if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
						{//�ı�ְλ
							CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->CleanUp();
							break;
						}
					}
					//ˢ�½���
					CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
				}
			}
			break;
		case GUILD_RETURN_REJECT:
			{
				STRING strTemp = "";
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{//�Լ������뱻�ܾ���
					CDataPool::GetMe()->Guild_ClearMemInfo();
					CDataPool::GetMe()->Guild_ClearDetailInfo();
					//ˢ�½���
					CEventSystem::GetMe()->PushEvent(GE_GUILD_FORCE_CLOSE);

					_snprintf(szText, _MAX_PATH, "%s���ܾ����������", pReturn->m_GuildName);
				}
				else
				{//���˿���
					INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
					for(INT i = 0; i<iMemNum; i++)
					{
						if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
						{//�ı�ְλ
							CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->CleanUp();
							break;
						}
					}
					_snprintf(szText, _MAX_PATH, "��ܾ���%s������", pReturn->m_DestName);

					//ˢ�½���
					CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
				}
			}
			break;
		case GUILD_RETURN_LEAVE:
			{
				//�����ݳ��в���������,�ı�ְλ
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{//���Լ��˳����
					CDataPool::GetMe()->Guild_ClearMemInfo();
					CDataPool::GetMe()->Guild_ClearDetailInfo();

					_snprintf(szText, _MAX_PATH, "�����뿪���");
					//ˢ�½���
					CEventSystem::GetMe()->PushEvent(GE_GUILD_FORCE_CLOSE);
				}
				else
				{//�������ڿ���ĳ���˳����
					INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
					for(INT i = 0; i<iMemNum; i++)
					{
						if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
						{//�ı�ְλ
							CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->CleanUp();
							break;
						}
					}
					
					//���Ƶ��
					//
					//ˢ�½���
					CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
				}
			}
			break;
		case GUILD_RETURN_RECRUIT:
			{
				STRING strTemp = "";
				//�����ݳ��в���������,�ı�ְλ
				INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
				for(INT i = 0; i<iMemNum; i++)
				{
					if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
					{//�ı�ְλ
						CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_bPosition = (BYTE)GUILD_POSITION_MEMBER;
						break;
					}
				}

				CDataPool::GetMe()->Guild_SetMaxMemNum(CDataPool::GetMe()->Guild_GetMaxMemNum()+1);

				//ˢ�½���
				CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);

				//��������
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					_snprintf(szText, _MAX_PATH, "��ӭ����%s,�������Э�������Ұ�", pReturn->m_GuildName);
				}

				//������
				if(pReturn->m_GUIDChanged == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					_snprintf(szText, _MAX_PATH, "�°���%s����ɹ�", pReturn->m_DestName);
				}

				strTemp = COLORMSGFUNC("Guild_New_Member", pReturn->m_DestName, pReturn->m_GuildName);
				//��Ӱ��������Ϣ
				ADD_GUILD_MSG(strTemp);
			}
			break;
		case GUILD_RETURN_FOUND:
			{
				_snprintf(szText, _MAX_PATH, "�����ʽ������");
			}
			break;
		case GUILD_RETURN_DISMISS:
			{
				_snprintf(szText, _MAX_PATH, "����ɢ��");
			}
			break;
		case GUILD_RETURN_PROMOTE:
			{
				//����ַ���
				//�����ݳ��в���������,�ı�ְλ
				INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
				for(INT i = 0; i<iMemNum; i++)
				{
					if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
					{//�ı�ְλ
						CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_bPosition = (BYTE)pReturn->m_PosID;
						break;
					}
				}

				//��İ��Ȩ�޸ı��ˡ�
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					_snprintf(szText, _MAX_PATH, "��İ��Ȩ�޸ı���");
				}
				else
				{
					_snprintf(szText, _MAX_PATH, "�����ɹ�");
				}

				CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
			}
			break;
		case GUILD_RETURN_DEMOTE:
			{
				//����ַ���
				//�����ݳ��в���������,�ı�ְλ
				INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
				for(INT i = 0; i<iMemNum; i++)
				{
					if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
					{//�ı�ְλ
						CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_bPosition = (BYTE)pReturn->m_PosID;
						break;
					}
				}

				//��İ��Ȩ�޸ı��ˡ�
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					_snprintf(szText, _MAX_PATH, "��İ��Ȩ�޸ı���");
				}
				else
				{
					_snprintf(szText, _MAX_PATH, "�����ɹ�");
				}

				
				CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
			}
			break;
		case GUILD_RETURN_AUTHORIZE:
		case GUILD_RETURN_DEPRIVE_AUTHORITY:
			{

			}
			break;
		case GUILD_RETURN_DEMISE:
			{
				if(pReturn->m_GUIDChanged == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					//���Ȩ�޸ı��ˣ���ϲ���Ϊ��������
					_snprintf(szText, _MAX_PATH, "��ϲ���Ϊ������");
					break;
				}

				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					//�����ɹ����Ȩ�޸ı���
					_snprintf(szText, _MAX_PATH, "�����ɹ�");
					break;
				}

				INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
				for(INT i = 0; i<iMemNum; i++)
				{
					if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUIDChanged)
					{//�ı�ְλ
						CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_bPosition = GUILD_POSITION_CHIEFTAIN;
					}

					if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
					{//�ı�ְλ
						CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_bPosition = GUILD_POSITION_MEMBER;
					}
				}

				CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
			}
			break;
		case GUILD_RETURN_WITHDRAW:
		case GUILD_RETURN_DEPOSIT:
		default:
			Assert(FALSE);
			return 0;
		}

		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, szText );
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

