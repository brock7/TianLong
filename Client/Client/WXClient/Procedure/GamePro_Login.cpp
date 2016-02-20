#include "StdAfx.h"
#include "Gamepro_login.h"
#include "GamePro_Enter.h"
#include "GamePro_CharSel.h"
#include "..\Global.h"
#include "..\Network\NetManager.h"
#include "..\World\WorldManager.h"
#include "..\Object\ObjectManager.h"
#include "..\Object\Character\Obj_PlayerMySelf.h"
#include "CGConnect.h"
#include "CGEnterScene.h"
#include "TDVariable.h"
#include "TDInputSystem.h"
#include "TDEventDefine.h"
#include "TDUISystem.h"
#include "AxProfile.h"
#include "..\Event\GMEventSystem.h"
#include <string>


#include "CLConnect.h"					// ��¼__������ͬ����Ϣ
#include "CLAskLogin.h"					// ��¼__������֤�û�������
#include "CLAskCharList.h"				// ��¼__���͵õ���ɫ��Ϣ
#include "..\WxRender\RenderSystem.h"


CGamePro_Login::CGamePro_Login()
{
	//m_Status = LOGIN_DEBUG_SETTING;
	m_Status = LOGIN_SELECT_SERVER;
	m_bReLogin = FALSE;
	m_pSoundSource = NULL;

	// ��ȡ������������Ϣ.
	LoadLoginServerInfo();

	// �����Զ�ѡ����������ұ�.
	BuildAutoSelTable();

	// �û���
	m_strUserName = "";

	// ����
	m_strPassword = "";

	for(int i = 0; i < PROVIDE_COUNT; i++ )
	{
		// ��ǰѡ�������
		m_iAutoAreaIndex[i] = -1;

		// ��ǰѡ���login server ����
		m_iAutoLoginServerIndex[i] = -1;
	}

	// ǰһ���ҵ�����״̬���е�����.
	m_iPreFindIdle = -1;

	// ǰһ���ҵ�����״̬����������.
	m_iPreFindNor  = -1;

	// ǰһ���ҵ�����״̬ӵ��������.
	m_iPreFindBusy = -1;

	// ǰһ���ҵ�����״̬����������.
	m_iPreFindFull = -1;

}

CGamePro_Login::~CGamePro_Login()
{
}

VOID CGamePro_Login::FirstLogin(VOID)
{
	CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Mode", (INT)LOGIN_FIRST_LOGIN);
}

VOID CGamePro_Login::LoginForChangeScene(VOID)
{
	CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Mode", (INT)LOGIN_CHANGE_SCENE);
}

VOID CGamePro_Login::Init(VOID)
{
	//fujia 2008.1.16
	/*if (CGameProcedure::s_pVariableSystem->GetAs_Int("View_FullScreen"))
	{
		s_pEventSystem->PushEvent(GE_VARIABLE_CHANGED, "View_FullScreen", "1");
	}
	else
	{
		const STRING& strVariable = CGameProcedure::s_pVariableSystem->GetAs_String("View_Resoution");
		fVector2 fResoution = s_pVariableSystem->GetAs_Vector2("View_Resoution");

		s_pEventSystem->PushEvent(GE_VARIABLE_CHANGED,"View_Resoution", strVariable.c_str());
		s_pEventSystem->PushEvent(GE_CHAT_ADJUST_MOVE_CTL, (INT)fResoution.x, (INT)fResoution.y);
	}*/
	
	// ��ֹ�ı䴰�ڴ�С��
	LONG lStyle = ::GetWindowLong(g_hMainWnd, GWL_STYLE);
	lStyle = lStyle & ~WS_THICKFRAME ;
	lStyle = lStyle & ~WS_MAXIMIZEBOX ;
	::SetWindowLong(g_hMainWnd, GWL_STYLE, lStyle);//
	//Rresh title
	::PostMessage(g_hMainWnd, WM_NCACTIVATE, TRUE, 0);

	//--- for debug
	if(CGameProcedure::s_pVariableSystem->GetAs_Int("GameServer_ConnectDirect") == 1) return;
	//--- for debug

	//�򿪵�¼����
	if(LOGIN_SELECT_SERVER == m_Status)
	{
		s_pEventSystem->PushEvent(GE_GAMELOGIN_OPEN_SELECT_SERVER);
	}

	// ѡ��ɵķ�����
	SelectOldServer();
}

// �ڽ�����ѡ�������
VOID CGamePro_Login::SelectOldServer()
{
	int iPreCurSelArea		  = CGameProcedure::s_pVariableSystem->GetAs_Int("Login_Area");
    int iPreCurSelLoginServer = CGameProcedure::s_pVariableSystem->GetAs_Int("Login_Server");

	if((iPreCurSelArea < m_iAreaCount)&&(iPreCurSelArea >= 0))
	{
		int iLoginServerCount = GetAreaLoginServerCount(iPreCurSelArea);

		if((iPreCurSelLoginServer < iLoginServerCount)&&(iPreCurSelLoginServer >= 0))
		{
			// ѡ��ǰһ��ѡ��ķ�����.
			s_pEventSystem->PushEvent(GE_GAMELOGIN_SELECT_AREA,			iPreCurSelArea);		// ѡ������
			s_pEventSystem->PushEvent(GE_GAMELOGIN_SELECT_LOGINSERVER,	iPreCurSelLoginServer);	// ѡ��Login Server
			return;
		}
	}
}

VOID CGamePro_Login::Tick(VOID)
{
	CGameProcedure::Tick();

	switch(m_Status)
	{
	case LOGIN_DEBUG_SETTING:
		{
			if(!CGameProcedure::s_pUISystem)
			{
				SetStatus(CGamePro_Login::LOGIN_DISCONNECT);
			}
			else
			{
				//DO NOTING,WAIT UI...
			}
		}
		break;

	case LOGIN_SELECT_SERVER:// ѡ�������״̬
		{
			//--- for debug
			if(CGameProcedure::s_pVariableSystem->GetAs_Int("GameServer_ConnectDirect") == 1)
			{
				//ֱ���л���Change-Server����
				CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcChangeScene);
				return;
			}
			//--- for debug
			break;
		}
	case LOGIN_DISCONNECT:
		{
			s_pGfxSystem->PushDebugString("Connect to login server %s:%d...", m_szLoginServerAddr, m_nLoginServerPort);

			//��ʼ��¼
			SetStatus(LOGIN_CONNECTING);
			CNetManager::GetMe()->ConnectToServer(m_szLoginServerAddr, m_nLoginServerPort);

		}
		break;

	case LOGIN_CONNECTING:

		break;
	
	//���ӳɹ�
	case LOGIN_CONNECTED_OK:
		{

			// ����������֤����
			// SetStatus(LOGIN_ACCOUNT_REQUESTING);

		}
		break;

	//����ʧ��
	case LOGIN_CONNECT_FAILED:

		CNetManager::GetMe()->Close();
		SetStatus(LOGIN_SELECT_SERVER);
		break;

	
	// ������֤�û���������.
	case LOGIN_ACCOUNT_REQUESTING:
		{

			// �ж��Ƿ�ʱ, ��ʱ����ʾ������Ϣ.
			break;
		}
	case LOGIN_ACCOUNT_BEGIN_REQUESTING:
		{
			break;
		}
	//��¼��Ϣ��֤�ɹ�
	case LOGIN_ACCOUNT_OK:
		{
			// ����ѡ��ķ�����
			CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Area",   CGameProcedure::s_pProcLogIn->m_iCurSelAreaId, FALSE);
			CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Server", CGameProcedure::s_pProcLogIn->m_iCurSelLoginServerId, FALSE);

			if(m_bReLogin)
			{
				//ֱ�ӽ��볡��
				CGameProcedure::s_pProcEnter->SetStatus(CGamePro_Enter::ENTERSCENE_READY);
				CGameProcedure::s_pProcEnter->SetEnterType(ENTER_TYPE_FIRST);
				CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcEnter);
			}
			else
			{
				// ���õ�¼״̬Ϊ�״ε�¼���������л������ĵ�¼״̬��
				CGameProcedure::s_pProcLogIn->FirstLogin();

				//ת������ѡ��ѭ��
				CGameProcedure::SetActiveProc((CGameProcedure*)s_pProcCharSel);
								
			}
		}
		break;
	default:
		break;
	}
}

VOID CGamePro_Login::Render(VOID)
{
	if(s_pGfxSystem) s_pGfxSystem->RenderFrame();
}

VOID CGamePro_Login::Release(VOID)
{
}

VOID CGamePro_Login::ProcessInput(VOID)
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
			CGameProcedure::s_pEventSystem->PushEvent(GE_TOGLE_CONSOLE);
			event.bProcessed = true;
			break;

		case KC_D:
			{
				if(!(s_pInputSystem->IsKeyDown(KC_LMENU))) 
					break;

				if(s_pUISystem)
					s_pUISystem->Debug_SaveFontTexture();
			}
			break;

		default:
			break;
		}
	}
}



//------------------------------------------------------------------------------------------------------------
//
// �������ļ��õ�, login server ��ip��ַ�Ͷ˿ں�
//
//

// ���ö˿ں�.
VOID  CGamePro_Login::SetPort(INT iPort)
{
	m_nLoginServerPort = iPort;
}

// ����ip��ַ
VOID  CGamePro_Login::SetIpAddr(LPCTSTR szServerAddr)
{
	memcpy(m_szLoginServerAddr, szServerAddr, sizeof(m_szLoginServerAddr));
}


// ��ȡlogin server��Ϣ
//
// ����ֵ: 
//			0 -- ��ȡ�ɹ�
//			1 -- ʧ���Ҳ��������ļ�
//          2 -- ��������    
int  CGamePro_Login::LoadLoginServerInfo(std::string strLoginServerInfoFile)
{
	m_pAreaInfo.resize(0);
	char szAppName[100];
	char szName[128];
	char szIP[32];
	DWORD dwMerchant;
	DWORD dwPort;
	DWORD dwBelong;
	DWORD dwStat;
	for( int i = 1; i < 256; i ++ )
	{
		sprintf( szAppName, "REGION%d", i );
		if( ::GetPrivateProfileString(szAppName, "NAME", "", szName, 120, strLoginServerInfoFile.c_str()) == 0 ) continue;
		dwMerchant = (DWORD)::GetPrivateProfileInt(szAppName, "MERCHANT", 0XFFFFFFFF, strLoginServerInfoFile.c_str());
		if( dwMerchant == 0XFFFFFFFF ) continue;
		AREA_INFO info;
		info.szAreaName = szName;
		info.iProvide = (NET_PROVIDE)dwMerchant;
		m_pAreaInfo.push_back( info );
	}

	m_iAreaCount = (int)m_pAreaInfo.size();
	for( int i = 1; i < 1024; i ++ )
	{
		sprintf( szAppName, "LOGIN%d", i );
		if( ::GetPrivateProfileString(szAppName, "NAME", "", szName, 120, strLoginServerInfoFile.c_str()) == 0 ) continue;
		if( ::GetPrivateProfileString(szAppName, "IP", "", szIP, 30, strLoginServerInfoFile.c_str()) == 0 ) continue;
		dwPort = (DWORD)::GetPrivateProfileInt(szAppName, "PORT", 0XFFFFFFFF, strLoginServerInfoFile.c_str());
		if( dwPort == 0XFFFFFFFF ) continue;
		dwStat = (DWORD)::GetPrivateProfileInt(szAppName, "STATE", 0XFFFFFFFF, strLoginServerInfoFile.c_str());
		if( dwStat == 0XFFFFFFFF ) continue;
		dwBelong = (DWORD)::GetPrivateProfileInt(szAppName, "BELONG", 0XFFFFFFFF, strLoginServerInfoFile.c_str());
		if( dwBelong == 0XFFFFFFFF ) continue;

		if( dwBelong == 0 || dwBelong > (DWORD)m_iAreaCount ) continue;
		AREA_INFO& info = m_pAreaInfo[dwBelong-1];

		LOGINSERVER_INOF li;
		li.szLoginServerName = szName;
		li.szIp = szIP;
		li.iPort = dwPort;
		li.iLoginServerStatus = (LOGIN_SERVER_STATUS)dwStat;
		info.LoginInfo.push_back( li );
	}

	return LOAD_LOGIN_SERVER_SUCCESS;
}

// ��ʼ��������Ϣ
void CGamePro_Login::InitLoginAreaInfo()
{
	m_pAreaInfo.resize( 0 );
	m_iAreaCount = 0;
}


// ��ȡ������Ϣ.
/*int CGamePro_Login::ReadAreaInfo(std::string& strLoginServer, int iAreaIndex)
{
	if(iAreaIndex >= m_iAreaCount)
	{
		return LOAD_LOGIN_SERVER_OTHER_ERR;
	}

	char bufInfo[512];
	char bufTempInfo1[128];
	char bufTempInfo2[128];
	char bufTempInfo3[128];


	while(!feof(pFile))
	{
		
		// �õ����������.
		::ZeroMemory(bufInfo, sizeof(bufInfo));
		if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
		{
			return LOAD_LOGIN_SERVER_OTHER_ERR;
		}
		::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
		::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
		::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo3));
		::sscanf(bufInfo, "%s %s %s", bufTempInfo1, bufTempInfo2, bufTempInfo3);
		if(strcmp("��������", bufTempInfo1))
		{
			// ���û�еõ���ȷ������, һֱ��ȡ���������.
			continue;
		}

		// �������������.
		strncpy(m_pAreaInfo[iAreaIndex].szAreaName, bufTempInfo3, sizeof(m_pAreaInfo[iAreaIndex].szAreaName));

		// ��ȡ��Ӧ��.
		::ZeroMemory(bufInfo, sizeof(bufInfo));
		if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
		{
			return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;
		}
		::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
		::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
		::ZeroMemory(bufTempInfo3, sizeof(bufTempInfo3));
		::sscanf(bufInfo, "%s %s %d", bufTempInfo1, bufTempInfo2, &(m_pAreaInfo[iAreaIndex].iProvide));


		// ��ȡlogin server �ĸ���.
		::ZeroMemory(bufInfo, sizeof(bufInfo));
		if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
		{
			return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;
		}
		::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
		::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
		::ZeroMemory(bufTempInfo3, sizeof(bufTempInfo3));
		::sscanf(bufInfo, "%s %s %d", bufTempInfo1, bufTempInfo2, &(m_pAreaInfo[iAreaIndex].iLoginServerCount));

		int iReadRes = 0;
		for(int i = 0; i < m_pAreaInfo[iAreaIndex].iLoginServerCount; i++)	
		{
			iReadRes = ReadLoginInfo(pFile, iAreaIndex, i);
			if(iReadRes)
			{
				return iReadRes;
			}
		}

		// ����һ��������Ϣ�ͷ���
		return LOAD_LOGIN_SERVER_SUCCESS;

	}
	
	// û�ж�ȡ�������������.
	return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;
}*/


// ��ȡlogin server��Ϣ.
/*int CGamePro_Login::ReadLoginInfo(FILE* pFile, int iAreaIndex, int iLoginServerIndex)
{
	
	//if(NULL == pFile)
	//{
	//	return LOAD_LOGIN_SERVER_FILE_NOT_FINDED;
	//}

	//char bufInfo[512];
	//char bufTempInfo1[128];
	//char bufTempInfo2[128];
	//char bufTempInfo3[128];


	//while(!feof(pFile))
	//{
	//	//-------------------------------------------------------
	//	// �õ�login server ������.
	//	::ZeroMemory(bufInfo, sizeof(bufInfo));
	//	if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
	//	{
	//		return LOAD_LOGIN_SERVER_OTHER_ERR;
	//	}

	//	::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
	//	::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
	//	::ZeroMemory(bufTempInfo3, sizeof(bufTempInfo3));
	//	::sscanf(bufInfo, "%s %s %s", bufTempInfo1, bufTempInfo2, bufTempInfo3);
	//	if(strcmp("LoginServer����", bufTempInfo1))
	//	{
	//		// ���û�еõ�login server, һֱ��ȡ�������.
	//		continue;
	//	}

	//	// ����login server ������.
	//	//strncpy(
	//	//		m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].szLoginServerName, 
	//	//		bufTempInfo3, 
	//	//		sizeof(m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].szLoginServerName)
	//	//		);

	//	//--------------------------------------------------------
	//	// ��ȡ״̬
	//	::ZeroMemory(bufInfo, sizeof(bufInfo));
	//	if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
	//	{
	//		return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;
	//	}
	//	::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
	//	::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
	//	::ZeroMemory(bufTempInfo3, sizeof(bufTempInfo3));
	//	::sscanf(
	//				bufInfo, 
	//				"%s %s %d", 
	//				bufTempInfo1, 
	//				bufTempInfo2, 
	//				&(m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].iLoginServerStatus)
	//				);


	//	//--------------------------------------------------------
	//	// ��ȡip��ַ
	//	::ZeroMemory(bufInfo, sizeof(bufInfo));
	//	if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
	//	{
	//		return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;
	//	}
	//	::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
	//	::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
	//	::ZeroMemory(bufTempInfo3, sizeof(bufTempInfo3));
	//	::sscanf(
	//				bufInfo, 
	//				"%s %s %s", 
	//				bufTempInfo1, 
	//				bufTempInfo2, 
	//				m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].szIp
	//				);


	//	//---------------------------------------------------------
	//	// ��ȡ�˿ں�
	//	::ZeroMemory(bufInfo, sizeof(bufInfo));
	//	if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
	//	{
	//		return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;
	//	}
	//	::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
	//	::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
	//	::ZeroMemory(bufTempInfo3, sizeof(bufTempInfo3));
	//	//::sscanf(
	//	//			bufInfo, 
	//	//			"%s %s %d", 
	//	//			bufTempInfo1, 
	//	//			bufTempInfo2, 
	//	//			&(m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].iPort)
	//	//			);

	//	// ����һ��login server ����.
	//	return LOAD_LOGIN_SERVER_SUCCESS;

	//}
	
	// û�ж�ȡ�������������.
	return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;//

}*/


//-----------------------------------------------------------------------------------------------------------------------------
//
// �߼����ݲ���
//
//

// �õ�����ĸ���
int CGamePro_Login::GetAreaCount()
{
	return m_iAreaCount;
}

// �õ���������
const TCHAR* CGamePro_Login::GetAreaName(int iAreaIndex)
{
	if( iAreaIndex < 0 || iAreaIndex >= m_iAreaCount )
		return _T("");

	return m_pAreaInfo[iAreaIndex].szAreaName.c_str();
}

// �õ�������Ϣ
PAREA_INFO CGamePro_Login::GetAreaInfo(int iAreaIndex)
{
	if( iAreaIndex < 0 || iAreaIndex >= m_iAreaCount )
		return NULL;

	return &m_pAreaInfo[iAreaIndex];
}

// ͨ�����繩Ӧ��ѡ��һ��login server
int CGamePro_Login::AutoSelLoginServer(int iProvide)
{
	PPROVIDE_INFO pProvideInfo = NULL;
	int iAreaIndex        = -1;
	int iLoginServerIndex = -1;

	switch(iProvide)
	{
	case 0:// ���Ź�Ӧ��
		{
			pProvideInfo = &m_Provide0;
			break;
		}
	case 1:// ��ͨ��Ӧ��
		{
			pProvideInfo = &m_Provide1;
			break;
		}
	case 2:// ������Ӧ��
		{
			pProvideInfo = &m_Provide2;
			break;
		}
	default:
		{
			break;
		}
	}

	int iLoginServerCount = (int)pProvideInfo->IdleLoginServerVector.size();
	// ���ҿ��еķ�����
	if(iLoginServerCount)
	{
		if(m_iPreFindIdle + 1 <  iLoginServerCount)
		{
			m_iPreFindIdle++;
			
		}
		else
		{
			m_iPreFindIdle = 0;
		}

		iAreaIndex        = pProvideInfo->IdleLoginServerVector[m_iPreFindIdle].iAreaIndex;
		iLoginServerIndex = pProvideInfo->IdleLoginServerVector[m_iPreFindIdle].iLoginServerIndex;
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SELECT_LOGIN_SERVER, iAreaIndex, iLoginServerIndex);//
		return 0;
	}

	iLoginServerCount = (int)pProvideInfo->NorLoginServerVector.size();
	// ���������ķ�����
	if(iLoginServerCount)
	{
		if(m_iPreFindNor + 1 <  iLoginServerCount)
		{
			m_iPreFindNor++;
		}
		else
		{
			m_iPreFindNor = 0;
		}

		iAreaIndex        = pProvideInfo->NorLoginServerVector[m_iPreFindNor].iAreaIndex;
		iLoginServerIndex = pProvideInfo->NorLoginServerVector[m_iPreFindNor].iLoginServerIndex;
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SELECT_LOGIN_SERVER, iAreaIndex, iLoginServerIndex);//
		return 0;
	}

	iLoginServerCount = (int)pProvideInfo->BusyLoginServerVector.size();
	// ���ҷ�æ�ķ�����
	if(iLoginServerCount)
	{
		if(m_iPreFindBusy + 1 <  iLoginServerCount)
		{
			m_iPreFindBusy++;
		}
		else
		{
			m_iPreFindBusy = 0;
		}

		iAreaIndex        = pProvideInfo->BusyLoginServerVector[m_iPreFindBusy].iAreaIndex;
		iLoginServerIndex = pProvideInfo->BusyLoginServerVector[m_iPreFindBusy].iLoginServerIndex;
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SELECT_LOGIN_SERVER, iAreaIndex, iLoginServerIndex);//
		return 0;
	}

	iLoginServerCount = (int)pProvideInfo->FullLoginServerVector.size();
	// ���ұ����ķ�����
	if(iLoginServerCount)
	{
		if(m_iPreFindFull + 1 <  iLoginServerCount)
		{
			m_iPreFindFull++;
		}
		else
		{
			m_iPreFindFull = 0;
		}

		iAreaIndex        = pProvideInfo->FullLoginServerVector[m_iPreFindFull].iAreaIndex;
		iLoginServerIndex = pProvideInfo->FullLoginServerVector[m_iPreFindFull].iLoginServerIndex;
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SELECT_LOGIN_SERVER, iAreaIndex, iLoginServerIndex);//
		return 0;
	}


	return 0;
}

// �����Զ�ѡ��login server��Ҫ�����ݱ�
int CGamePro_Login::BuildAutoSelTable()
{
	//LOGIN_SERVER_POS LoginServerPos;
	//PPROVIDE_INFO			 pProvideInfo     = NULL;
	//LOGIN_SERVER_POS_VECTOR* pServerPosVector = NULL;
	//int iLoginServerCount = 0;
	//for(int i = 0; i < m_iAreaCount; i++)
	//{
		//switch(m_pAreaInfo[i].iProvide)
		//{
		//case 0:// ���Ź�Ӧ��
		//	{
		//		pProvideInfo = &m_Provide0;
		//		break;
		//	}
		//case 1:// ��ͨ��Ӧ��
		//	{
		//		pProvideInfo = &m_Provide1;
		//		break;
		//	}
		//case 2:// ������Ӧ��
		//	{
		//		pProvideInfo = &m_Provide2;
		//		break;
		//	}
		//}

		//iLoginServerCount = m_pAreaInfo[i].iLoginServerCount;
		//for(int j = 0; j < iLoginServerCount; j++)
		//{
		//	LoginServerPos.iAreaIndex        = i;
		//	LoginServerPos.iLoginServerIndex = j;
		//
		//	switch(m_pAreaInfo[i].LoginInfo[j].iLoginServerStatus)
		//	{
		//	case LOGIN_SERVER_FULL:
		//		{
		//			pServerPosVector = &(pProvideInfo->FullLoginServerVector);
		//			break;
		//		}
		//	case LOGIN_SERVER_BUSY:
		//		{
		//			pServerPosVector = &(pProvideInfo->BusyLoginServerVector);
		//			break;
		//		}
		//	case LOGIN_SERVER_NORMAL:
		//		{
		//			pServerPosVector = &(pProvideInfo->NorLoginServerVector);
		//			break;
		//		}
		//	case LOGIN_SERVER_IDLE:
		//		{
		//			pServerPosVector = &(pProvideInfo->IdleLoginServerVector);
		//			break;
		//		}
		//	default:
		//		{

		//		}

		//	}// switch(m_pAreaInfo[i].LoginInfo[j].iLoginServerStatus)

		//	pServerPosVector->push_back(LoginServerPos);

		//}// for(int j = 0; j < iLoginServerCount; j++)

	//}// for(int i = 0; i < m_iAreaCount; i++)

	return 0;
}

// �õ�������login server �ĸ���
int CGamePro_Login::GetAreaLoginServerCount(int iAreaIndex)
{
	if( iAreaIndex < 0 || iAreaIndex >= m_iAreaCount )
		return 0;

	return (int)(m_pAreaInfo[iAreaIndex].LoginInfo.size());
}

// �õ�������login server �ĸ���
PLOGINSERVER_INOF CGamePro_Login::GetAreaLoginServerInfo(int iAreaIndex, int iLoginServerIndex)
{
	if( iAreaIndex < 0 || iAreaIndex >= m_iAreaCount )
		return NULL;

	if( iLoginServerIndex >= (int)m_pAreaInfo[iAreaIndex].LoginInfo.size() )
		return NULL;

	return &(m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex]);
}


// ���ӵ�login server
int CGamePro_Login::ConnectToLoginServer(int iAreaIndex, int iLoginServerIndex)
{

	if( iAreaIndex < 0 || iAreaIndex >= m_iAreaCount )
		return 1;

	if( iLoginServerIndex >= (int)m_pAreaInfo[iAreaIndex].LoginInfo.size() )
		return 1;

	// ����ip��ַ�Ͷ˿ں�.
	SetIpAddr( m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].szIp.c_str() );
	SetPort( m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].iPort );

	// ���õ�ǰ��״̬Ϊ������״̬
	SetStatus(LOGIN_DISCONNECT);

	// ֪ͨ������ʾϵͳ��ʾ��Ϣ, �������ӷ�����.
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, "�������ӵ�������.....");

	return 0;

}


// ��¼ѡ��ķ�����login server
int CGamePro_Login::SelectLoginServer(int iAreaIndex, int iLoginServerIndex)
{

	// ��¼ѡ��ķ�����
	m_iCurSelAreaId			= iAreaIndex;
	m_iCurSelLoginServerId  = iLoginServerIndex;

	OpenCountInputDlg();

	return 0;
}


// ���ӵ�login server
int CGamePro_Login::ConnectToLoginServer()
{
	// ���õ�ǰ��״̬Ϊ������״̬
	SetStatus(LOGIN_DISCONNECT);

	// ֪ͨ������ʾϵͳ��ʾ��Ϣ, �������ӷ�����.
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, "�������ӵ�������.....");
	return 0;
}


//-------------------------------------------------------------------------------------------------------------------------------
//
// �������
//

// ���ʺ��������
int CGamePro_Login::OpenCountInputDlg()
{
	// ���ʺ��������
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_COUNT_INPUT);

	// �ر�ϵͳ����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// �رշ�����ѡ�����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SELECT_SERVER);

	// ����ѡ��ķ�����
	CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Area",   CGameProcedure::s_pProcLogIn->m_iCurSelAreaId, FALSE);
	CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Server", CGameProcedure::s_pProcLogIn->m_iCurSelLoginServerId, FALSE);

	// �����ʺ��������
	//SetStatus(LOGIN_ACCOUNT_BEGIN_REQUESTING);
	return 0;
}

// �򿪷�����ѡ�����
int CGamePro_Login::OpenSelectServerDlg()
{
	// �򿪷�����ѡ�����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_SELECT_SERVER);

	// �ر�ϵͳ����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// �ر��ʺ��������
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_COUNT_INPUT);

	// ���÷�����ѡ�����
	SetStatus(LOGIN_SELECT_SERVER);

	// �Ͽ���������
	CNetManager::GetMe()->Close();

	SelectOldServer();
	return 0;
}


// ������ѡ�����
int CGamePro_Login::OpenSelectCharacter()
{

	return 0;
}

// �л�������ѡ�����
int CGamePro_Login::ChangeToCharacterSel()
{

	
	// ����ѡ��ķ�����
	CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Area",   CGameProcedure::s_pProcLogIn->m_iCurSelAreaId, FALSE);
	CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Server", CGameProcedure::s_pProcLogIn->m_iCurSelLoginServerId, FALSE);

	// �ر�ϵͳ����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// �ر��ʺ��������
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_COUNT_INPUT);

	// ��ʾ����ѡ�����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_SELECT_CHARACTOR);

	//�л�������ѡ�����
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcCharSel);

	// �����û���
	CGameProcedure::s_pProcCharSel->SetUserName(m_strUserName.c_str());

	// ���뵽����ѡ����棬 ���uiģ�͡�
	CGameProcedure::s_pProcCharSel->m_bClearUIModel = TRUE;

	// ���͵õ���ɫ��Ϣ��Ϣ.
	CLAskCharList msg;
	msg.SetAccount((CHAR*)m_strUserName.c_str());
	CNetManager::GetMe()->SendPacket(&msg);


	return 0;
}



//-------------------------------------------------------------------------------------------------------------------------------
//
// ����������Ϣ����
//

// ����ͬ����Ϣ
int CGamePro_Login::SendClConnectMsg()
{
	CLConnect msg;

	// ��������������Ϣ
	CNetManager::GetMe()->SendPacket(&msg);
	return 0;
}

// ����û���������.
int CGamePro_Login::CheckAccount(CHAR* szUserName, CHAR* szPassword)
{
	if(NULL == szUserName)
	{
		// ֪ͨ������ʾϵͳ��ʾ��Ϣ, �û�������
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "�����û�������");
		return 0;
	}

	if(NULL == szPassword)
	{
		// ֪ͨ������ʾϵͳ��ʾ��Ϣ, �û�������
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "�����������");
		return 0;
	}

	m_strUserName = szUserName;
	m_strPassword = szPassword;
	

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ��֤�û���
	//
	int iPos = (int)m_strUserName.find('\'');
	if(-1 != iPos)
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "�û������ܺ��� \" \' \" ");
		return 0;
	}

    iPos = (int)m_strUserName.find('"');
	if(-1 != iPos)
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "�û������ܺ��� \"  \"  \" ");
		return 0;
	}

	iPos = (int)m_strUserName.find('\\');
	if(-1 != iPos)
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "�û������ܺ��� \" \\ \" ");
		return 0;
	}//


	//// ���Ƿ��ַ������в��ҡ�
	//int iFindPos = -1;
	//char bufInfo[512];
	//for(int i = 0; i < (int)m_UnUseStrVector.size(); i++)
	//{
	//	iFindPos = (int)m_strUserName.find(m_UnUseStrVector[i]);
	//	if(-1 != iFindPos)
	//	{
	//		memset(bufInfo, 0, sizeof(bufInfo));
	//		sprintf(bufInfo, "�û��������Ƿ�����%s", m_UnUseStrVector[i]);
	//		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, bufInfo);
	//		return 0;
	//	}

	//}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ��֤����
	//
	iPos = (int)m_strPassword.find('\'');
	if(-1 != iPos)
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "���벻�ܺ��� \" \' \" ");
		return 0;
	}

    iPos = (int)m_strPassword.find('"');
	if(-1 != iPos)
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "���벻�ܺ��� \"  \"  \" ");
		return 0;
	}

	iPos = (int)m_strPassword.find('\\');
	if(-1 != iPos)
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "���벻�ܺ��� \" \\ \" ");
		return 0;
	}//

	//// ���Ƿ��ַ������в���
	//for(int j = 0; j < (int)m_UnUseStrVector.size(); j++)
	//{
	//	iFindPos = (int)m_strPassword.find(m_UnUseStrVector[j]);
	//	if(-1 != iFindPos)
	//	{
	//		memset(bufInfo, 0, sizeof(bufInfo));
	//		sprintf(bufInfo, "��������Ƿ�����%s", m_UnUseStrVector[j]);
	//		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, bufInfo);
	//		return 0;
	//	}
	//}

	// �����������socket ����.
	ConnectToLoginServer(m_iCurSelAreaId, m_iCurSelLoginServerId);
	return 0;
}

// ����check msg ��Ϣ
int CGamePro_Login::SendCheckAccountMsg()
{
	// ������֤������Ϣ
	CLAskLogin msg;
	msg.SetAccount((CHAR*)m_strUserName.c_str());
	msg.SetPassWord((CHAR*)m_strPassword.c_str());
	msg.SetVersion(1005); // MagicKey
	CNetManager::GetMe()->SendPacket(&msg);
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, "������֤����....");

	s_pVariableSystem->SetVariable( "User_NAME", m_strUserName.c_str() );
	return 0;
}


// 
// 
//
//------------------------------------------------------------------------------------------------------------
