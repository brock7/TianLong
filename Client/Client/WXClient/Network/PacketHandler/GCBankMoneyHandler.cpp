#include"stdafx.h"

#include "GCBankMoney.h"
#include "CGBankMoney.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "TDException.h"
UINT GCBankMoneyHandler::Execute( GCBankMoney* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			BYTE IsSave = pPacket->GetSaveType();
			INT	 Amount = pPacket->GetAmount();
			INT  AmountRMB = pPacket->GetAmountRMB();
			INT CurMoney = CDataPool::GetMe()->UserBank_GetBankMoney();
			INT CurRMB = CDataPool::GetMe()->UserBank_GetBankRMB();

			CHAR szMsg[256];
			if(IsSave & CGBankMoney::SAVE_MONEY)
			{
				CurMoney += Amount;
				//֪ͨ��ҽ��׳ɹ�

				_snprintf(szMsg, 256, "�����Ǯ�ɹ�");
				CDataPool::GetMe()->UserBank_SetBankMoney(CurMoney);
				//update ����
				CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK );
				//��ʾ��Ϣ
				CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE,szMsg	);

			}
			else if(IsSave & CGBankMoney::PUTOUT_MONEY)
			{
				CurMoney -= Amount;
				//֪ͨ��ҽ��׳ɹ�		

				_snprintf(szMsg, 256, "ȡ����Ǯ�ɹ�");

				CDataPool::GetMe()->UserBank_SetBankMoney(CurMoney);
				//update ����
				CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK );
				//��ʾ��Ϣ
				CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE,szMsg	);

			}
			else if(IsSave & CGBankMoney::UPDATE_MONEY)
			{//���볡��ʱ�ɷ�����ˢ�����������еĽ�Ǯ��
				CurMoney = Amount;

				CDataPool::GetMe()->UserBank_SetBankMoney(CurMoney);
				//update ����
				CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK );
			}

			if( IsSave & CGBankMoney::SAVE_RMB )
			{
				CurRMB += AmountRMB;
				//֪ͨ��ҽ��׳ɹ�

				_snprintf(szMsg, 256, "����Ԫ���ɹ�");
				CDataPool::GetMe()->UserBank_SetBankRMB(CurRMB);
				//update ����
				CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK );
				//��ʾ��Ϣ
				CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE,szMsg	);
			}
			else if( IsSave & CGBankMoney::PUTOUT_RMB )
			{
				CurRMB -= AmountRMB;
				//֪ͨ��ҽ��׳ɹ�		

				_snprintf(szMsg, 256, "ȡ��Ԫ���ɹ�");

				CDataPool::GetMe()->UserBank_SetBankRMB(CurRMB);
				//update ����
				CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK );
				//��ʾ��Ϣ
				CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE,szMsg	);
			}
			else if( IsSave & CGBankMoney::UPDATA_RMB )
			{
				CurRMB = AmountRMB;

				CDataPool::GetMe()->UserBank_SetBankRMB(CurRMB);
				//update ����
				CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK );
			}

		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
