#include "StdAfx.h"

#include "GCPlayerShopError.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"

#include "TDException.h"
UINT GCPlayerShopErrorHandler::Execute( GCPlayerShopError* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			switch(pPacket->GetID())
			{
				//����ǿ��ȡ�����ν��׵Ĵ���
			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_LEVEL:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��ĵȼ�̫�ͣ���30���������Ұ�");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NAME_ALREADY_EXISTED:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�Բ����Ѿ���ͬ�����̴��ڣ�������ȡ����");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_MONEY_TO_NEW:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ľ�Ǯ��������Щ�ʽ��������Ұɡ�");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_SHOP_IN_POOL:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����ϵͳ�������ޣ������ٿ�����");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_ROOM_IN_STALL:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�˹�̨����");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_ALREADY_LOCK:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����Ʒ����");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NEED_NEW_COPY:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����Ѹı䣬�����´��̵�");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_ROOM:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��������");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_MONEY:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ǯ����");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_ALREADY_HAVE_ENOUGH_SHOP:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�޷����������");
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_SUCCESS_SELL:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����̵�ɹ�");
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_SUCCESS_CHANGE_DESC:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�޸��̵������ɹ�");
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_SUCCESS_CHANGE_NAME:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�޸��̵����ɹ�");
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_STILL_STALL_OPEN:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��ĵ��̻��й�̨��Ӫҵ");
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_SALE_OUT_SERIAL_IS_CHANGED:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����Ѹ�������,�����´�");
					return PACKET_EXE_CONTINUE ;
				}
				break;


			case PLAYERSHOP_MSG::ERR_SHOP_BASE_MONEY_TOO_LOW:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"������");
					return PACKET_EXE_CONTINUE ;
				}
				break;


			case PLAYERSHOP_MSG::ERR_SHOP_NOT_ENOUTH_MONEY_TO_SALE_OUT:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ǯ����");
					return PACKET_EXE_CONTINUE ;
				}
				break;


			case PLAYERSHOP_MSG::ERR_SHOP_NOT_ENOUTH_MONEY_TO_BUY_BACK:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "��Ǯ����");
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_NOT_ENOUTH_MONEY_TO_CHANGE_NAME:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "��Ǯ����");
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_PARTNER_LIST_FULL:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�ϻ����б��Ѿ�����");
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case PLAYERSHOP_MSG::ERR_SHOP_PARTNER_ALREADY_IN_LIST:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����Ѿ��Ǻϻ�����");
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case PLAYERSHOP_MSG::ERR_SHOP_PARTNER_NOT_FIND_IN_WORLD:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���ﲻ����");
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case PLAYERSHOP_MSG::ERR_SHOP_PARTNER_INVALID:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�Ƿ�����");
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case PLAYERSHOP_MSG::ERR_SHOP_PARTNER_LIST_EMPTY:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�Ѿ�û�кϻ�����");
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case PLAYERSHOP_MSG::ERR_SHOP_PARTNER_NOT_FIND_IN_LIST:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�޷�ɾ��");
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case PLAYERSHOP_MSG::ERR_SHOP_ADD_TO_FAVORITE:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�޷����");
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case PLAYERSHOP_MSG::ERR_SHOP_DEL_FROM_FAVORITE:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�޷�ɾ��");
					return PACKET_EXE_CONTINUE ;
				}
				break;

				//����ȡ�����ν��׵Ĵ���
			case PLAYERSHOP_MSG::ERR_ILLEGAL:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�Ƿ���Ϣ");
				}
			default:
				{

				}
				break;
			}

			//��ճ��е��̵�����
			CDataPool::GetMe()->PlayerShop_CleanUp(TRUE);
			CDataPool::GetMe()->PlayerShop_CleanUp(FALSE);

		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
