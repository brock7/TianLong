#include "StdAfx.h"
#include "GCAbilityExp.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\DataPool\GMDP_CharacterData.h"


UINT GCAbilityExpHandler::Execute( GCAbilityExp* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//����UI���ݳ�
		CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pDataPool);

		//--------------------------------------------------------------
		//�����ˢ��
/*
		SCommand_DPC cmdTemp;
		cmdTemp.m_wID			= DPC_UPDATE_LIFE_ABILITY;
		cmdTemp.m_adwParam[0]	= 3;
		cmdTemp.m_adwParam[1]	= (UINT)(pPacket->getAbilityId());
		cmdTemp.m_adwParam[2]	= (UINT)(pPacket->getExp());
		pDataPool->OnCommand_( &cmdTemp );
*/		
		CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_LifeAbility(
			pPacket->getAbilityId(), -1, pPacket->getExp());

		//�����ʾ��Ϣ
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
		if(bLog && CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(pPacket->getAbilityId()))
		{
			LPCTSTR pAbilityName = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(
				pPacket->getAbilityId())->m_pDefine->szName;
			UINT exp = pPacket->getExp()/100;
			CHAR szText[_MAX_PATH];
			_snprintf( szText, _MAX_PATH, "���#cCCFFCC%s#W���ܵ����������ӵ�#R%d#W", pAbilityName, exp );
			ADDTALKMSG(szText);
		}
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
