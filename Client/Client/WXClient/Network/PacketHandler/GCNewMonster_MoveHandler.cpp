#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerNPC.h"
#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "..\..\NetWork\NetManager.h"
#include "CGCharAskBaseAttrib.h"
#include "GCNewMonster_Move.h"
#include "..\..\World\WorldManager.h"
#include "TDException.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Object\GMObjectLoadQueue.h"
#include "..\WxRender\RenderSystem.h"

UINT GCNewMonster_MoveHandler :: Execute( GCNewMonster_Move* pPacket, Player* pPlayer ) 
{
//__ENTER_FUNCTION
	//AxTrace(0, 2, "GCNewMonsterHandler");
	//return PACKET_EXE_CONTINUE;
	//AxTrace(0, 2, "********NewMonster_Move Begin %d", pPacket->getObjID());
	//��ǰ������������
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		//���λ���Ƿ�Ϸ�
		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ)))
		{
			TDThrow("ERROR POSITION @ GCNewMonster_MoveHandler");
		}
		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pPacket->getTargetPos().m_fX, pPacket->getTargetPos().m_fZ)))
		{
			TDThrow("ERROR POSITION @ GCNewMonster_MoveHandler target");
		}

		//�������
		CObject_PlayerNPC* pNPC = (CObject_PlayerNPC*)pObjectManager->FindServerObject( (INT)pPacket->getObjID() );
		fVector3 fvGame;
		CGameProcedure::s_pGfxSystem->Axis_Trans(
			CRenderSystem::AX_PLAN, fVector3(pPacket->getWorldPos().m_fX, 0.0f, pPacket->getWorldPos().m_fZ),
			CRenderSystem::AX_GAME, fvGame);

		if ( pNPC == NULL )
		{
			pNPC = pObjectManager->NewPlayerNPC( (INT)pPacket->getObjID() );

			SObjectInit initPlayerNPC;
			initPlayerNPC.m_fvPos	= fvGame;
			initPlayerNPC.m_fvRot	= fVector3( 0.f, 0.f, 0.f );

			pNPC->Initial( &initPlayerNPC );
		}
		else
		{
			fVector2 mapPos(fvGame.x, fvGame.z);
			pNPC->SetMapPosition(mapPos);
			pNPC->Enable( OSF_VISIABLE );
			pNPC->Disalbe( OSF_OUT_VISUAL_FIELD );
		}
		if( pPacket->getIsNPC() )
			pNPC->SetNpcType(NPC_TYPE_NPC);
		else
			pNPC->SetNpcType(NPC_TYPE_MONSTER);

		pNPC->GetCharacterData()->Set_MoveSpeed(pPacket->getMoveSpeed());

		// update attrib command
		//{
		//	FLOAT fMoveSpeed = pPacket->getMoveSpeed();
		//	SCommand_Object cmdTemp;
		//	cmdTemp.m_wID			= OC_UPDATE_ATTRIB;
		//	cmdTemp.m_adwParam[0]	= (1<<UPDATE_CHAR_ATT_MOVE_SPEED);
		//	cmdTemp.m_apParam[1]	= (VOID*)(&fMoveSpeed);
		//	pNPC->PushCommand(&cmdTemp );

		//}

		pNPC->GetCharacterData()->Set_MoveSpeed(pPacket->getMoveSpeed());

		// move to command
		{
			WORLD_POS posTarget;
			posTarget.m_fX	= pPacket->getTargetPos().m_fX;
			posTarget.m_fZ	= pPacket->getTargetPos().m_fZ;

			SCommand_Object cmdTemp;
			cmdTemp.m_wID			= OC_MOVE;
			cmdTemp.m_auParam[0]	= 0;
			cmdTemp.m_anParam[1]	= pPacket->getHandleID();
			cmdTemp.m_anParam[2]	= 1;
			cmdTemp.m_apParam[3]	= &posTarget;
			pNPC->PushCommand(&cmdTemp );
		}

		//����Ask����
		CObjectManager::GetMe()->GetLoadQueue()->TryAddLoadTask(pNPC->GetID(), CObject_Character::CT_MONSTER);

		// tempcode{
		// �˰治����������æ�ͻ����Ӻ���Ϣ�Ĵ���
		//CGCharAskBaseAttrib msgAskBaseAttrib;
		//msgAskBaseAttrib.setTargetID( (INT)pPacket->getObjID() );
		//CNetManager::GetMe()->SendPacket( &msgAskBaseAttrib );

		//AxTrace(0, 0, "[%d]Ask %d", m_hmObject.size(), pPacket->getObjID());
		// }


		//ͬ����Ⱦ��
		char szTemp[MAX_PATH];
		_snprintf(szTemp, MAX_PATH, "GCNewMonster_Move(%.1f,%.1f)", 
			pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ);
		pNPC->PushDebugString(szTemp);
		pNPC->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());

		//AxTrace(0, 2, "********NewMonster_Move End");
	}

	return PACKET_EXE_CONTINUE ;

//__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

