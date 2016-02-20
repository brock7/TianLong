// GCAbilitySuccHandler.cpp
#include "StdAfx.h"
#include "GCAbilitySucc.h"
#include "TDUISystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Sound\GMSoundSystem.h"

#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "..\..\Interface\GMGameInterface.h"

UINT GCAbilitySuccHandler::Execute(GCAbilitySucc* pPacket,Player* pPlayer)
{
__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
		//����һ����ʱ�ĵ���
		CObject_Item* pItem = NULL;

		if( pPacket->GetItemSerial() != INVALID_ID )
		{
			if(bLog) pItem = CObject_Item::NewItem(pPacket->GetItemSerial());
		}

		//������ǵ���ɹ��������ؽ�������
		if(pPacket->GetAbilityID() != 9)
			CGameProcedure::s_pEventSystem->PushEvent(GE_PROGRESSBAR_HIDE);

		//�ж������ּ��ܣ�������Ӧ����
		switch(pPacket->GetAbilityID())
		{
		case 9://11
			CSoundSystemFMod::_PlayUISoundFunc(11+59);
			break;
		case 8://13
			CSoundSystemFMod::_PlayUISoundFunc(13+59);
			break;
		case 10://15
			CSoundSystemFMod::_PlayUISoundFunc(15+59);
			break;
			//19
		case 4:			//����
		case 5:			//����
		case 6:			//��������
		case 7:			//����ѧ
			CSoundSystemFMod::_PlayUISoundFunc(19+59);
			break;
		case 3:	//23 //21
			CSoundSystemFMod::_PlayUISoundFunc(23+59);
			break;
		default :
			break;
		}

		if(bLog)
		{
			STRING strTemp = "";
			if(pPacket->GetAbilityID() == 3)
			{
				ADDTALKMSG("��ɹ�����Ƕ��һ�ű�ʯ");
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "��Ƕ�ɹ�");
			}
			else if( pItem != NULL )
			{
				strTemp = COLORMSGFUNC("CREATE_ONE_ITEM", pItem->GetName());
				ADDTALKMSG(strTemp);
				CObject_Item::DestroyItem(pItem);
			}
			else if( pPacket->GetPrescriptionID() != INVALID_ID )
			{ // ������䷽�ϳ�ʧ��
				strTemp = "�ϳ�ʧ��";//COLORMSGFUNC("ability_create_failed");
				ADDTALKMSG(strTemp);
			}
		}

		// ����������ʲô�أ�
		//CObjectManager* pObjectManager = CObjectManager::GetMe();
		//SCommand_Object cmdCharacter;
		//cmdCharacter.m_wID = OC_LIFE_ABILITE_ACTION;
		//cmdCharacter.m_adwParam[0] = pPacket->GetAbilityID();
		//cmdCharacter.m_adwParam[1] = -1;
		//cmdCharacter.m_adwParam[2] = 2;
		//cmdCharacter.m_adwParam[3] = pPacket->GetPrescriptionID();
	
		//pObjectManager->GetMySelf()->PushCommand(&cmdCharacter);

		CGameProcedure::s_pGameInterface->Player_UseLifeAbility( pPacket->GetPrescriptionID() );
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
