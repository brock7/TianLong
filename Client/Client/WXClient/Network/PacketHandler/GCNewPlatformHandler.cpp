// GCNewPlatformHandler.cpp
#include "StdAfx.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\TripperObj\GMTripperObj_ItemBox.h"
#include "..\..\Object\TripperObj\GMTripperObj_Resource.h"
#include "..\..\Object\TripperObj\GMTripperObj_Platform.h"
#include "..\..\World\WorldManager.h"
#include "TDException.h"
#include "GCNewPlatform.h"


UINT GCNewPlatformHandler::Execute( GCNewPlatform* pPacket, Player* pPlayer )
{
//__ENTER_FUNCTION
	//AxTrace(0, 2, "GCNewItemBox::Execute");
	//��ǰ������������
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();
		
		//���λ���Ƿ�Ϸ�
		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ)))
		{
			TDThrow("ERROR POSITION @ GCNewPlatformHandler");
		}

		//����ƽ̨������
		CTripperObject_Platform* pPlatform = (CTripperObject_Platform*)CObjectManager::GetMe()->NewObject( "CTripperObject_Platform", pPacket->getObjID());
		if(!(pPlatform->SetPlatformID(pPacket->getType())))
		{
			//�Ƿ���ƽ̨ID
			CObjectManager::GetMe()->DestroyObject(pPlatform);
		}
		pPlatform->Initial(NULL);	
		//����λ��
		pPlatform->SetMapPosition( fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ));
		pPlatform->SetFaceDir( pPacket->getDir() );
	}

	return PACKET_EXE_CONTINUE ;

//__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
