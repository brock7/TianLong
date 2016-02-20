#include "StdAfx.h"
#include "GCDelObject.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerOther.h"
#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "TDTimeSystem.h"
#include "..\..\Global.h"
#include "..\..\World\WorldManager.h"
#include "TDDBC_Struct.h"

UINT GCDelObjectHandler :: Execute( GCDelObject* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION
	//AxTrace(0, 2, "GCNewMonsterHandler");

	//��ǰ������������
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		//�ж��Ƿ��Ǳ�����
		if(CWorldManager::GetMe()->GetActiveScene()->GetSceneDefine()->nServerID != pPacket->getSceneID())
		{
			AxTrace(0, 2, "DelObject:SceneID Error!(%d != %d)", 
				pPacket->getSceneID(), 
				CWorldManager::GetMe()->GetActiveScene()->GetSceneDefine()->nServerID);
			return PACKET_EXE_CONTINUE;
		}

		//Ѱ��obj
		CObject_Character* pObj = (CObject_Character*)(pObjectManager->FindServerObject( (INT)pPacket->getObjID() ));
		if ( pObj == NULL )
			return PACKET_EXE_CONTINUE;

		if(pObj == CObjectManager::GetMe()->GetMainTarget())
		{
			CObjectManager::GetMe()->SetMainTarget(-1);
		}

		pObj->Release();
		CObjectManager::GetMe()->DestroyObject( pObj );
		//֮����ִ���κδ���
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

