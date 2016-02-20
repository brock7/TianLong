/*
���������͸��ͻ��˵Ĵ�����Ϣ
*/

#include "stdafx.h"
#include "GCStallError.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"

#include "TDException.h"

UINT GCStallErrorHandler::Execute( GCStallError* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			switch(pPacket->GetID())
			{
				//����ǿ��ȡ�����ν��׵Ĵ���
			case STALL_MSG::ERR_OWNER_INVALID:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"Ŀ�����ﲻ����");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_NEED_NEW_COPY:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"̯λ��Ϣ�Ѹı�,�����´�");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_NOT_ENOUGH_ROOM:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�ռ䲻��");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_NOT_ENOUGH_MONEY:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ǯ����");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_NOT_ENOUGH_ROOM_IN_STALL:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"̯λ����");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_CLOSE:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"̯������̯");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_ALREADY_LOCK:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���ܳ��۴���Ʒ");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_PET_LEVEL_TOO_HIGH:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���Ｖ�����,�޷�����");
					return PACKET_EXE_CONTINUE ;
					break;
				}

				//����ȡ�����ν��׵Ĵ���
			case STALL_MSG::ERR_NOT_ENOUGH_MONEY_TO_OPEN:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ǯ���㣬�޷���̯");
					break;
				}
			case STALL_MSG::ERR_ILLEGAL:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�Ƿ���Ϣ");
					return PACKET_EXE_CONTINUE ;
					break;
				}
			default:
				{
				
				}
				break;
			}
			
			//�����������Ʒ������
			//��Ҫ���������Ӧ��Ʒ������(ע�⣬��¡����ƷGUID��ͬ�����Կ�����������)
			for(INT i=0; i<STALL_BOX_SIZE; i++)
			{
				if(CDataPool::GetMe()->MyStallBox_GetItem(i))
				{
					__int64 ItemGUID = CDataPool::GetMe()->MyStallBox_GetItem(i)->GetGUID();

					tObject_Item* pBagItem = CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->UserBag_GetItemIndexByGUID(ItemGUID));
					if(pBagItem)
					{
						//ȥ������
						pBagItem->SetLock(FALSE);
					}
				}
			}


			//���Ŀ��̯λ
			CDataPool::GetMe()->OtStallBox_Clear();
			CDataPool::GetMe()->MyStallBox_Clear();

			//uiˢ��
			CActionSystem::GetMe()->UserStallSale_Update();
			CActionSystem::GetMe()->UserStallBuy_Update();
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
