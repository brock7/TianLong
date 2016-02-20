/*
������Ŀͻ����յ���������
*/
#include "stdafx.h"
#include "GCExchangeApplyI.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Character\Obj_Character.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Interface\GMGameInterface.h"
#include "TDException.h"

UINT GCExchangeApplyIHandler::Execute( GCExchangeApplyI* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{//��ʼ��˫�����׺У��򿪽���
			//����ȴ�����
			ObjID_t Appid = pPacket->GetObjID();
			CObject_Character* pCharObj = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject( Appid ));
			ENUM_RELATION sCamp = CGameInterface::GetMe()->GetCampType( pCharObj, (CObject*)CObjectManager::GetMe()->GetMySelf() );
			if( sCamp != RELATION_FRIEND )
			{//�ǵ���
				return PACKET_EXE_CONTINUE;
			}

			CDataPool::GetMe()->AddToAppList(Appid);

			//֪ͨ������ʾ�û������������㽻��
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�յ�һ���µĽ�������");
			CEventSystem::GetMe()->PushEvent(GE_RECEIVE_EXCHANGE_APPLY);
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
