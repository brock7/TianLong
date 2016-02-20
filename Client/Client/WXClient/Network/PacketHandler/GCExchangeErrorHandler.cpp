
#include "stdafx.h"
#include "GCExchangeError.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "TDException.h"


UINT GCExchangeErrorHandler::Execute( GCExchangeError* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			switch(pPacket->GetID())
			{
			//����ǿ��ȡ�����ν��׵Ĵ���
			case EXCHANGE_MSG::ERR_ALREADY_LOCKED:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���������������²���");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case EXCHANGE_MSG::ERR_NOT_ENOUGHT_EXROOM:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ɽ�����Ʒ���������ޣ������²���");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			//����ȡ�����ν��׵Ĵ���
			case EXCHANGE_MSG::ERR_SELF_IN_EXCHANGE:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�Լ����ڽ�����");
					break;
				}
			case EXCHANGE_MSG::ERR_TARGET_IN_EXCHANGE:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�Է����ڽ�����");
					break;
				}
			case EXCHANGE_MSG::ERR_DROP:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�Է�ȡ������");
					break;
				}
			case EXCHANGE_MSG::ERR_NOT_ENOUGHT_ROOM_OTHER:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ռ䲻��");
					break;
				}
			case EXCHANGE_MSG::ERR_NOT_ENOUGHT_ROOM_SELF:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ռ䲻��");
					break;
				}
			case EXCHANGE_MSG::ERR_NOT_ENOUGHT_MONEY_OTHER:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ǯ���㣬�����²���");
					break;
				}
			case EXCHANGE_MSG::ERR_NOT_ENOUGHT_MONEY_SELF:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ǯ���㣬�����²���");
					break;
				}
			case EXCHANGE_MSG::ERR_TOO_FAR:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ܹ����׵ķ�Χ");
					break;
				}

			case EXCHANGE_MSG::ERR_REFUSE_TRADE:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�Է��ܾ�����Ľ�������");
					break;
				}
			case EXCHANGE_MSG::ERR_PET_LEVEL_TOO_HIGH:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���Ｖ�����,�޷�����");
					break;
				}
			case EXCHANGE_MSG::ERR_ILLEGAL:
			default:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�Ƿ�״̬������ȡ��");
					break;
				}
			}

			//��Ҫ���������Ӧ��Ʒ������(ע�⣬��¡����ƷGUID��ͬ�����Կ�����������)
			for(INT i=0; i<EXCHANGE_BOX_SIZE; i++)
			{
				if(CDataPool::GetMe()->MyExBox_GetItem(i))
				{
					__int64 ItemGUID = CDataPool::GetMe()->MyExBox_GetItem(i)->GetGUID();

					tObject_Item* pBagItem = CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->UserBag_GetItemIndexByGUID(ItemGUID));
					if(pBagItem)
					{
						//ȥ������
						pBagItem->SetLock(FALSE);
					}
				}
			}

			//һ�����
			CDataPool::GetMe()->MyExBox_Clear();
			CDataPool::GetMe()->OtExBox_Clear();

			//����Action
			CActionSystem::GetMe()->UserExchangeOther_Update();
			CActionSystem::GetMe()->UserExchangeSelf_Update();


			//���±�����Ʒ��������ܻ���Ҫ����װ������Ʒ�����Ҫ��װ�����ϵĶ����ǲ��ǿ����������ף�
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

			//�رս��׽���
			CEventSystem::GetMe()->PushEvent(GE_SUCCEED_EXCHANGE_CLOSE);
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
