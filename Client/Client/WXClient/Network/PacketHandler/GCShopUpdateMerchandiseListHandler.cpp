// GCShopUpdateMerchandiseListHandler.cpp
#include "stdafx.h"
#include "GCShopUpdateMerchandiseList.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerOther.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "TDException.h"

UINT GCShopUpdateMerchandiseListHandler::Execute( GCShopUpdateMerchandiseList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			//�Ѿ������˾�����
			if(CDataPool::GetMe()->Booth_IsClose())
				return PACKET_EXE_CONTINUE ;

			//���µ���Ʒ�ĸ���
			INT nNum = pPacket->GetMerchadiseNum();
			//��ӵ����ݳ���
			for(INT i=0; i<nNum; i++)
			{
				//������Ʒʵ��
				tObject_Item* pItem = CDataPool::GetMe()->Booth_GetItemByID(pPacket->GetMerchadiseList()[i].idTable);
				TDAssert(pItem);
	
				//�����µ�����
				INT	num = (pPacket->GetMerchadiseList()[i].byNumber<0)? 0:pPacket->GetMerchadiseList()[i].byNumber;
				pItem->SetMax(num);
			}
			//���µ�ActionSystem
			if(nNum > 0) CActionSystem::GetMe()->Booth_Update();
			//���µ�UI
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_BOOTH);
		}

		return PACKET_EXE_CONTINUE ;

		__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
