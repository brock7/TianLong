/*
��̯״̬��ʼ��ʼ���Լ���̯λ��
*/
#include "stdafx.h"
#include "..\NetManager.h"
#include "GCStallEstablish.h"
#include "CGStallOpen.h"
#include "CGStallShopName.h"
#include "CGBBSSychMessages.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\Object\Character\Obj_Character.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "TDException.h"



UINT GCStallEstablishHandler::Execute( GCStallEstablish* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			//�����Լ�objid
			CGStallOpen OpenMsg;
			OpenMsg.SetObjID(CObjectManager::GetMe()->GetMySelf()->GetServerID());
			CNetManager::GetMe()->SendPacket(&OpenMsg);
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
