/*
������֪ͨ�ͻ��˹ر�̯λ
*/
#include "stdafx.h"
#include "GCStallClose.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "TDException.h"


UINT GCStallCloseHandler::Execute( GCStallClose* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			//ȡ�������ж�Ӧ��Ʒ������״̬
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


			//���̯λ
			CDataPool::GetMe()->MyStallBox_Clear();

			//�����Ϣ
			CDataPool::GetMe()->MyStallMsg_ClearUp();

			//����ui���ر�StallMessage����
			CEventSystem::GetMe()->PushEvent(GE_CLOSE_STALL_MESSAGE);
			//uiˢ��
			CActionSystem::GetMe()->UserStallSale_Update();

			//ˢ�±�������
			CActionSystem::GetMe()->UserBag_Update();
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);


		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
