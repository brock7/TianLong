#include "StdAfx.h"
#include "..\Network\NetManager.h"
#include "GamePro_CharCreate.h"
#include "GamePro_CharSel.h"

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
#include "..\Event\GMEventSystem.h"
#include "GameStruct.h"
#include "CLAskCreateChar.h"


CGamePro_CharCreate::CGamePro_CharCreate()
{
	m_pAvatar[0] = NULL;
	m_pAvatar[1] = NULL;

	m_Status = CHAR_CREATE_CREATEING;

	m_iFaceColor1 = 0;
	m_iFaceModle1 = 0;
	m_iHairColor1 = 0;
	m_iHairModle1 = 0;

	m_iIsLoadFace = 0;

	m_iIsLoadFaceModel = 0;

	m_iIsLoadHairModel = 0;

}

CGamePro_CharCreate::~CGamePro_CharCreate()
{
	if(m_pAvatar[0])
	{
		CFakeObjSystem::GetMe()->DestroyObject("CreateRole_Woman");
	}

	if(m_pAvatar[1])
	{
		CFakeObjSystem::GetMe()->DestroyObject("CreateRole_Man");
	}
}

VOID CGamePro_CharCreate::Init(VOID)
{
	s_pGfxSystem->Camera_SetEyePos(fVector3(-100,    325, 1000));
	s_pGfxSystem->Camera_SetLookAtPos(fVector3(-100, 320, 0));

	if(0 == m_iIsLoadFace)
	{
		int res = m_FaceMng.GetCharacterFaceIfo();
		if(0 == res)
		{
			m_iIsLoadFace = 1;
		}
	}

	if(0 == m_iIsLoadFaceModel)
	{
		int res = m_FaceModelmng.GetCharacterModel();
		if(0 == res)
		{
			m_iIsLoadFaceModel = 1;
		}
	}


	if(0 == m_iIsLoadHairModel)
	{
		int res = m_HairModelMng.GetCharacterModel();
		if(0 == res)
		{
			m_iIsLoadHairModel = 1;
		}
	}

	// ����ģ����Ϣ.
	CreateModel();
}

VOID CGamePro_CharCreate::Tick(VOID)
{
	CGameProcedure::Tick();
	//CGameProcedure::ProcessActiveInput();
}

VOID CGamePro_CharCreate::Render(VOID)
{
	if(s_pGfxSystem) s_pGfxSystem->RenderFrame();
}

VOID CGamePro_CharCreate::Release(VOID)
{

}

VOID CGamePro_CharCreate::MouseInput(VOID)
{

}

// �л���ѡ���������
void CGamePro_CharCreate::ChangeToSelectRole()
{
	
	// �ر�ϵͳ����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// ������ѡ�����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_SELECT_CHARACTOR);

	// �ر����ﴴ������
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_CREATE_CHARACTOR);

	// �������ʾģ�͡�
	CGameProcedure::s_pProcCharSel->m_bClearUIModel = FALSE;

	//�л�������ѡ�����
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcCharSel);
}


VOID CGamePro_CharCreate::ProcessInput(VOID)
{
	
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
				//-------------------------------------------------------
				//���� '1', �ص�����ѡ������.
				//CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcCharSel);
			}
			break;
		case KC_2:
			{
				
			}
		
		default:
			break;
		}
	}

}

// ��������ģ��
int CGamePro_CharCreate::CreateModel()
{
	if(m_pAvatar[0])
	{
		return TRUE;
	}

	//����Avatar
	m_pAvatar[0]  = (CObject_PlayerOther*)(CFakeObjSystem::GetMe()->NewFakeObject(_T("CObject_PlayerOther"), _T("CreateRole_Woman"), "Camera_Main"));
	m_pAvatar[1]  = (CObject_PlayerOther*)(CFakeObjSystem::GetMe()->NewFakeObject(_T("CObject_PlayerOther"), _T("CreateRole_Man"), "Camera_Main"));

	// Ůģ��
	m_pAvatar[0]->SetFaceDir(0);
	m_pAvatar[0]->GetCharacterData()->Set_RaceID(0);
	//����ȱʡͷ����ɫ
	m_pAvatar[0]->GetCharacterData()->Set_HairColor(0);
	
	// ��ģ�� 
	m_pAvatar[1]->SetFaceDir(0);
	m_pAvatar[1]->GetCharacterData()->Set_RaceID(1);
	//����ȱʡͷ����ɫ
	m_pAvatar[1]->GetCharacterData()->Set_HairColor(0);
	
	return TRUE;

}

// ������ɫ
int	CGamePro_CharCreate::CreateRole()
{
	const char* szInvalid = " #\\/`~!@~$%^&*()-_+=|{}[];:'\"<>,.?";
	const char* szHZBlank = "��"; //���Ŀո�

	CLAskCreateChar msg;
	
	std::string strName = m_szCreateRoleName;
	if(strName.empty())
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "���ֲ���Ϊ��");	
		return 1;
	}

	//�����Ƿ��зǷ�ascii�ַ�
	STRING::size_type nInvalidPos = strName.find_first_of(szInvalid);
	STRING strInvalidChar;
	if(nInvalidPos == STRING::npos)
	{
		//���Ŀո�
		nInvalidPos = strName.find(szHZBlank);
		strInvalidChar = szHZBlank;
	}
	else
	{
		//�Ƿ��ַ�
		strInvalidChar = strName.substr(nInvalidPos, 1);
	}

	if(nInvalidPos != STRING::npos)
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("ERRORSpecialString");
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());	
		return 0;
	}

	if(CGameProcedure::s_pUISystem)
	{
		if(!CGameProcedure::s_pUISystem->CheckStringFilter(strName)||(!CGameProcedure::s_pUISystem->CheckStringFullCompare(strName)))
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("ERRORSpecialString");
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
			return 0;
		}
	}

	msg.SetName(m_szCreateRoleName);	//���ý�ɫ����
	msg.SetFaceColor(m_iFaceColor1);    //�����沿��ɫ
	msg.SetFaceModel(m_iFaceModle1);    //�����沿ģ��
	msg.SetHairColor(m_iHairColor1);    //����ͷ����ɫ
	msg.SetHairModel(m_iHairModle1);    //����ͷ��ģ��
	msg.SetSex(m_iSex);					//�����Ա�
	msg.SetHeadID(m_iFaceId);			//����ͷ��id 
	msg.SetCharGuid(0);					//Ŀǰ֧�ֽ�ɫguid�ͻ��˼��㣬�Ժ��ΪLogin 
	msg.SetCharCamp(m_iCamp);			//������Ӫ
	msg.SetMenPai(m_iMenpai);			//��������

	CNetManager::GetMe()->SendPacket(&msg);

	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, "���ڲ�����");	
	return 0;
}

// �л�����ɫѡ�����
void CGamePro_CharCreate::ChangeToRoleSel()
{
	// ��ʾ����ѡ�����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_SELECT_CHARACTOR);

	// �ر�ϵͳ����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// �ر����ﴴ������
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_CREATE_CHARACTOR);

	// �������ʾģ�͡�
	CGameProcedure::s_pProcCharSel->m_bClearUIModel = FALSE;
	
	//�л�������ѡ�����
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcCharSel);

	

}

// ����ͷ��id
void CGamePro_CharCreate::SetFace(int iFaceId)
{
	m_iFaceId = iFaceId;
}

// ͨ���Ա����������ͷ��id
void CGamePro_CharCreate::SetFaceByRaceAndIndex(int iRace, int iIndex)
{
	switch(iRace)
	{
	case 0:
		{
			SetFace(m_FaceMng.GetWomanFaceId(iIndex));
			break;
		}
	case 1:
		{
			SetFace(m_FaceMng.GetManFaceId(iIndex));
			break;
		}
	}
}

// ��������id
void	CGamePro_CharCreate::SetFaceModel(int iFaceModelId)
{
	m_iFaceModle1 = iFaceModelId;
}

// ͨ���Ա����������ͷ��id
void CGamePro_CharCreate::SetFaceModelByRaceAndIndex(int iRace, int iIndex)
{
	int iRaceIndex = iRace;
	switch(iRace)
	{
	case 0:
		{
			
			SetFaceModel(m_FaceModelmng.GetWomanFaceModelId(iIndex));//
			break;
		}
	case 1:
		{
			SetFaceModel(m_FaceModelmng.GetManFaceModelId(iIndex));
			break;
		}
	}
	
	m_pAvatar[iRaceIndex]->GetCharacterData()->Set_FaceMesh(m_iFaceModle1);
}


// ��������id
void	CGamePro_CharCreate::SetHairModel(int iHairModelId)
{
	m_iHairModle1 = iHairModelId;
}

// ͨ���Ա����������ͷ��id
void CGamePro_CharCreate::SetHairModelByRaceAndIndex(int iRace, int iIndex)
{
	int iRaceIndex = iRace;

	switch(iRace)
	{
	case 0:
		{
			SetHairModel(m_HairModelMng.GetWomanHairModelId(iIndex));
			break;
		}
	case 1:
		{
			SetHairModel(m_HairModelMng.GetManHairModelId(iIndex));
			break;
		}
	}

	m_pAvatar[iRaceIndex]->GetCharacterData()->Set_HairMesh(m_iHairModle1);
}



