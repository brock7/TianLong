/*
֪ͨ�ͻ���ȷ�ϣ��۸�˰�ա�
*/
#include "stdafx.h"
#include "GCStallApply.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "TDException.h"

UINT GCStallApplyHandler::Execute( GCStallApply* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{

		BYTE	IsCanStall	=	pPacket->GetIsCanStall();
		UINT	PosTax		=	pPacket->GetPosTax();
		BYTE	TradeTax	=	pPacket->GetTradeTax();

		if(IsCanStall == TRUE)
		{
			//��ʾ�û�ȷ���Ի���
			CDataPool::GetMe()->MyStallBox_SetPosTax(PosTax);
			CDataPool::GetMe()->MyStallBox_SetTradeTax(TradeTax);

			CEventSystem::GetMe()->PushEvent(GE_OPEN_STALL_RENT_FRAME);
		}
		else
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�������̯Ҫ�󣬰�̯����ʧ��");
		}
	}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
