#include "StdAfx.h"

#include "..\Network\NetManager.h"
#include "GamePro_CharSel.h"
#include "Gamepro_login.h"
#include "GamePro_Enter.h"
#include "GamePro_CharCreate.h"
#include ".\Gamepro_ChangeScene.h"
#include "CGEnterScene.h"
#include "..\Global.h"
//#include "..\Network\NetManager.h"


#include "..\Input\GMInputSystem.h"
#include "..\Object\ObjectManager.h"
#include "..\Object\Character\Obj_PlayerOther.h"
#include "..\GameCommand.h"
#include "..\Object\ObjectCommandDef.h"
#include "..\object\ObjectDef.h"
#include "..\object\character\Obj_PlayerOther.h"
#include "..\FakeObject\GMFakeObjSystem.h"
#include "..\object\Character\Obj_Character.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "..\WxRender\RenderSystem.h"
#include "TDGfxSystem.h"
#include "..\Event\GMEventSystem.h"
#include "CLAskDeleteChar.h"
#include "CLAskCharLogin.h"
#include "GameStruct.h"
//--------------------------------------------------------------------------------------------------------------------------
//
// �ڽ�������ʾ��ģ��.
//

// ģ�ͼ���.
UINT CModelShowInLoginUI::m_TeamNumCount = 0;

CModelShowInLoginUI::CModelShowInLoginUI()
{
	
	m_pAvatar = NULL;								// ����UI��ʾ���߼�����.
	memset(m_szBufModel, 0, sizeof(m_szBufModel));	// ģ������.

	// �����µ�ģ��
	//CreateNewUIModel();

}

CModelShowInLoginUI::~CModelShowInLoginUI()
{
	DestroyUIModel();
}

// ɾ��uiģ��
BOOL CModelShowInLoginUI::DestroyUIModel()
{
	if(m_pAvatar)
	{
		CFakeObjSystem::GetMe()->DestroyObject(m_szBufModel);
		m_pAvatar = NULL;
	}

	memset(m_szBufModel, 0, sizeof(m_szBufModel));	// ģ������.
	return FALSE;
}

// �����µ�ģ��
BOOL	CModelShowInLoginUI::CreateNewUIModel()
{
	if(m_pAvatar)
	{
		DestroyUIModel();
	}
	
	static int iUiModelCount = 0;
	iUiModelCount++;
	m_TeamNumCount = ::timeGetTime();
	m_TeamNumCount += iUiModelCount;
	memset(m_szBufModel, 0, sizeof(m_szBufModel));
	_stprintf(m_szBufModel, "uiModel%d", m_TeamNumCount);
	
	//����Avatar
	m_pAvatar  = (CObject_PlayerOther*)(CFakeObjSystem::GetMe()->NewFakeObject(_T("CObject_PlayerOther"), m_szBufModel, "Camera_Main"));
	m_pAvatar->SetFaceDir(0);
	m_pAvatar->GetCharacterData()->Set_RaceID(0);
	return FALSE;

}


// ����ģ����Ϣ
VOID CModelShowInLoginUI::SetUIModelInfo(HUMAN_EQUIP point, INT nID)
{
	if(m_pAvatar)
	{
		m_pAvatar->GetCharacterData()->Set_Equip(point, nID);
	}
}

// �õ�uiģ������
const TCHAR* CModelShowInLoginUI::GetUIModelName()
{
	
	return m_szBufModel;
	
}


//------------------------------------------------------------------------------------------------------------------------------




CGamePro_CharSel::CGamePro_CharSel()
{
	m_Status = CHARSEL_CHARSHOW;
	m_bIsLoadScene		= false;
	m_iCharacterCount	= 0;

	m_bClearUIModel = true;
}

CGamePro_CharSel::~CGamePro_CharSel()
{
	ClearUIModel();
}

VOID CGamePro_CharSel::Init(VOID)
{

	//return;
	//s_pGfxSystem->Camera_SetCurrent(tGfxSystem::CHAR_VIEW);
	//
	//if(!m_bIsLoadScene)
	//{
	//	s_pGfxSystem->Scene_Load(0, "CharacterSelTest.Scene");
	//	m_bIsLoadScene = true;
	//}

	//s_pGfxSystem->Camera_SetEyePos(fVector3(-100,    325, -1000));
	//s_pGfxSystem->Camera_SetLookAtPos(fVector3(-100, 320, 0));//

	if(m_bClearUIModel)
	{
		ClearUIModel();
	}

}

VOID CGamePro_CharSel::Tick(VOID)
{
	CGameProcedure::Tick();
	//CGameProcedure::ProcessActiveInput();

	switch(m_Status)
	{
	case CHARSEL_CHARSHOW:
		{
		
			break;
		}

	case CHARSEL_SELDONE:
		{
			break;
		}
	case CHARSEL_CONNNECT_GAME_SERVER_READY:// ����Ϸ������������׼����, ��������.
		{
			break;
		}
	}

}

VOID CGamePro_CharSel::Render(VOID)
{
	if(s_pGfxSystem) s_pGfxSystem->RenderFrame();
}

VOID CGamePro_CharSel::Release(VOID)
{
}

VOID CGamePro_CharSel::CloseRequest(VOID)
{

}

VOID CGamePro_CharSel::SetStatus(PLAYER_CHARSEL_STATUS status)
{
	m_Status = status;
}

VOID CGamePro_CharSel::ProcessInput(VOID)
{
	////-------------------------------------------------------
	////���� 'A'
	//if(s_pInputSystem->IsKeyPress(KC_A))
	//{
	//	m_Status = CHARSEL_SELDONE;
	//}

	////-------------------------------------------------------
	////���� '1' ������ɫ����
	//if(s_pInputSystem->IsKeyPress(KC_1))
	//{
	//	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcCharCreate);
	//}

	////-------------------------------------------------------
	////���� '2' ������Ϸ����.
	//if(s_pInputSystem->IsKeyPress(KC_2))
	//{
	//	m_bIsLoadScene = false;
	//	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcLogIn);
	//}//

	//�����¼�����
	InputEventQueue& inputQueue = s_pInputSystem->GetInputQueue();
	for(INT i=0; i<(INT)inputQueue.size(); i++)
	{
		InputEvent& event = inputQueue[i];
		if(event.bProcessed) continue;
		if(event.nEvent != EVENT_KEY_UP) continue;
		switch(event.kIndex)
		{
		case KC_GRAVE:
			{
			}
			break;
		case KC_1:
			{
				CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcCharCreate);
			}
			break;
		case KC_2:
			{
				m_Status = CHARSEL_SELDONE;
				m_bIsLoadScene = false;
			}
		
		default:
			break;
		}
	}//

}


// �����û�����
void CGamePro_CharSel::SetUserName(const char* pUserName)
{
	if(pUserName)
	{
		m_strUserName = pUserName;
	}
}

// �л����ʺ��������
void CGamePro_CharSel::ChangeToAccountInput()
{
	// �ر�ϵͳ����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// ���ʺ��������
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_COUNT_INPUT);

	// �ر�����ѡ�����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR);

	//�л�������ѡ�����
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcLogIn);
	if(CGameProcedure::s_pProcLogIn)
	{
		CGameProcedure::s_pProcLogIn->SetStatus(CGamePro_Login::LOGIN_ACCOUNT_BEGIN_REQUESTING);
	}

	m_iCharacterCount = 0;
	// �Ͽ���������
	CNetManager::GetMe()->Close();
	
}

	// �л��������������
void CGamePro_CharSel::ChangeToCreateRole()
{
	// �����ﴴ������
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_CREATE_CHARACTOR);

	// �ر�ϵͳ����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// �ر�����ѡ�����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR);


	//�л�������ѡ�����
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcCharCreate);

	if(CGameProcedure::s_pProcCharCreate)
	{
		CGameProcedure::s_pProcCharCreate->CallInit();
	}

	
}


// ���һ������
int CGamePro_CharSel::AddCharacter(const DB_CHAR_BASE_INFO& CharacterInfo)
{
	if(m_iCharacterCount < MAX_SHOW_IN_UI)
	{
		memcpy(m_CharacterInfo + m_iCharacterCount, &CharacterInfo, sizeof(DB_CHAR_BASE_INFO));
		m_Character[m_iCharacterCount].CreateNewUIModel();

		int iRaceId		= m_CharacterInfo[m_iCharacterCount].m_Sex;
		int iFaceMeshId = m_CharacterInfo[m_iCharacterCount].m_FaceModel;
		int iFaceHairId = m_CharacterInfo[m_iCharacterCount].m_HairModel;
		UINT iHairColor = m_CharacterInfo[m_iCharacterCount].m_HairColor;
		_CAMP_DATA TempCampData;
		TempCampData.m_nCampID = m_CharacterInfo[m_iCharacterCount].m_Camp;
		
		// �����Ա�
		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_RaceID(iRaceId);

		// ��������ģ��
		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_FaceMesh(iFaceMeshId);

		// ���÷��͵�ģ��.
		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_HairMesh(iFaceHairId);

		//ͷ����ɫ
		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_HairColor(iHairColor);

		//��Ӫ
		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_CampData( &TempCampData );

		const DB_CHAR_EQUIP_LIST& equipInfo = m_CharacterInfo[m_iCharacterCount].m_EquipList;
		// ����װ��.
		for(int iEquipPoint = HEQUIP_WEAPON; iEquipPoint < HEQUIP_NUMBER; iEquipPoint++)
		{
			m_Character[m_iCharacterCount].SetUIModelInfo((HUMAN_EQUIP)iEquipPoint, equipInfo.m_Equip[iEquipPoint]);
		}//

		m_iCharacterCount++;

		return 0;
	}
	else
	{
		return -1;
	}

}



// �õ���ǰ��ɫ�ĸ���
int  CGamePro_CharSel::GetCurRoleCount()
{

	return m_iCharacterCount;
}

// ͨ�������õ���ɫ����Ϣ
const DB_CHAR_BASE_INFO* CGamePro_CharSel::GetRoleInfo(int iIndex)
{

	if((iIndex < MAX_SHOW_IN_UI)&&(iIndex >= 0))
	{

		return (m_CharacterInfo + iIndex);
	}
	
	return NULL;
}


// ͨ�������õ���ɫ��UI ģ������
const TCHAR* CGamePro_CharSel::GetRoleUIName(int iIndex)
{
	if((iIndex < MAX_SHOW_IN_UI)&&(iIndex >= 0))
	{

		return m_Character[iIndex].GetUIModelName();
	}
	
	return NULL;
}


// ɾ��һ����ɫ
int CGamePro_CharSel::DelRole(int iRoleIndex)
{
	if((iRoleIndex < m_iCharacterCount)&&(iRoleIndex >= 0))
	{
		if(iRoleIndex < MAX_SHOW_IN_UI)
		{
			CLAskDeleteChar msg;

			msg.SetCharGuid(m_CharacterInfo[iRoleIndex].m_GUID);

			// ɾ����ɫ
			CNetManager::GetMe()->SendPacket(&msg);
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, "����ɾ����ɫ.....");	
			return 0;
			
		}
	}

	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "��ѡ��һ����ɫ");	
	return 1;
}


// ������Ϸ
int CGamePro_CharSel::SendEnterGameMsg(int iRoleIndex)
{
	if((iRoleIndex < m_iCharacterCount)&&(iRoleIndex >= 0))
	{
		if(iRoleIndex < MAX_SHOW_IN_UI)
		{
			CLAskCharLogin msg;

			m_EnterGameGUID = m_CharacterInfo[iRoleIndex].m_GUID;
			msg.SetCharGuid(m_EnterGameGUID);

			// ɾ����ɫ
			CNetManager::GetMe()->SendPacket(&msg);
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, "׼��������Ϸ.....");	
			return 0;
		}
	}

	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "��ѡ��һ����ɫ");	
	return 1;
}


// �л�����������������
void CGamePro_CharSel::ChangeToServerConnect()
{
	// �л�������game server����
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcChangeScene);

	// ����game server����Ϊ�Ͽ�״̬
	CGameProcedure::s_pProcChangeScene->SetStatus(CGamePro_ChangeScene::CHANGESCENE_DISCONNECT);


}

// λ���Ƿ�Ϸ�
bool CGamePro_CharSel::IsValidIndex(int iRoleIndex)
{

	if((iRoleIndex < m_iCharacterCount)&&(iRoleIndex >= 0))
	{
		if(iRoleIndex < MAX_SHOW_IN_UI)
		{
			return true;
		}
	}

	return false;
}

// ɾ��ѡ��Ľ�ɫ
int CGamePro_CharSel::DelSelRole()
{

	if((m_iCurSelRoleDel < m_iCharacterCount)&&(m_iCurSelRoleDel >= 0))
	{
		if(m_iCurSelRoleDel < MAX_SHOW_IN_UI)
		{
			CLAskDeleteChar msg;
			msg.SetCharGuid(m_CharacterInfo[m_iCurSelRoleDel].m_GUID);

			// ɾ����ɫ
			CNetManager::GetMe()->SendPacket(&msg);
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, "����ɾ����ɫ.....");	
			return 0;
		}
	}

	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "��ѡ��һ����ɫ");	
	return 1;
}

void CGamePro_CharSel::SetCurSelDel(int iIndex)
{
	m_iCurSelRoleDel = -1;
}

void CGamePro_CharSel::ClearUIModel()
{
	for(int i = 0; i < MAX_SHOW_IN_UI; i++)
	{
		m_Character[i].DestroyUIModel();
	}
}



