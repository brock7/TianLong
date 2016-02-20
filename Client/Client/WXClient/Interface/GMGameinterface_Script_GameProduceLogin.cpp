//
// 2006-5-21
//
//

#include "stdafx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"
#include "../Procedure/GamePro_CharSel.h"
#include "../Procedure/GamePro_Login.h"
#include "../Procedure/GameProcedure.h"
#include "../Procedure/GamePro_CharCreate.h"

#include "../NetWork/NetManager.h"
#include "LuaPlus.h"
#include "../Global.h"
#include "../Event/GMEventSystem.h"
#include "TDException.h"
#include "../Action/GMActionSystem.h"
#include "../DataPool/GMUIDataPool.h"
#include "../DataPool/GMDataPool.h"
#include "../Object/ObjectManager.h"
#include "../Object/Object.h"
#include "../Object/Character/Obj_Character.h"
#include "../Input/GMInputSystem.h"
#include "../world/worldmanager.h"
#include "../object/character/obj_playermyself.h"
#include "ScriptCommandStruct.h"

#include "..\..\Common\database\tlbb_dbc.h"
#include "..\..\Common\BuffImpactMgr.h"
#include "TDDBC_Struct.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "..\Sound\GMSoundSystem.h"


namespace SCRIPT_SANDBOX
{

	// ���徲̬��Ա����.
	CGameProduce_Login CGameProduce_Login::s_GameProduceLogin;

	// ��̬lua������������.
	LuaPlus::LuaObject* CGameProduce_Login::s_pMetaTable = NULL;

	CGameProduce_Login::CGameProduce_Login()
	{

	}

	CGameProduce_Login::~CGameProduce_Login()
	{

	}

	//�õ�����������ĸ���.
	INT CGameProduce_Login::GetAreaCount(LuaPlus::LuaState* state)
	{

		int iAreaCount = 0;
		if(CGameProcedure::s_pProcLogIn)
		{
			iAreaCount = CGameProcedure::s_pProcLogIn->GetAreaCount();
		}
		
		state->PushInteger(iAreaCount);
		return 1;
		
	}
	
	//�õ����������������.
	INT CGameProduce_Login::GetAreaName(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int nAreaIndex = args[2].GetInteger();

		std::string strInfo = "";
		if(CGameProcedure::s_pProcLogIn)
		{
			strInfo = CGameProcedure::s_pProcLogIn->GetAreaName(nAreaIndex);
		}
	
		state->PushString(strInfo.c_str());
		
		return 1;
	}

	//�õ��������������Ϣ
	INT CGameProduce_Login::GetAreaServerInfo(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int nAreaIndex = args[2].GetInteger();

		std::string strInfoName = "";
		int iAreaProvide		= 0;
		if(CGameProcedure::s_pProcLogIn)
		{
			PAREA_INFO pInfo = CGameProcedure::s_pProcLogIn->GetAreaInfo(nAreaIndex);
			if(pInfo)
			{
				strInfoName  = pInfo->szAreaName;
				iAreaProvide = pInfo->iProvide;
			}
		}
	
		state->PushString(strInfoName.c_str());
		state->PushInteger(iAreaProvide);
		return 2;
	}


	//�õ������������, login server �ĸ���.
	INT CGameProduce_Login::GetAreaLoginServerCount(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int nAreaIndex = args[2].GetInteger();

		int iLoginServerCount = 0;
		if(CGameProcedure::s_pProcLogIn)
		{
			iLoginServerCount = CGameProcedure::s_pProcLogIn->GetAreaLoginServerCount(nAreaIndex);
		}
	
		state->PushInteger(iLoginServerCount);
		
		return 1;
	}


	// �õ�login server ����Ϣ.
	INT CGameProduce_Login::GetAreaLoginServerInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}


		int nAreaIndex		  = args[2].GetInteger();
		int nLoginServerIndex = args[3].GetInteger();

		PLOGINSERVER_INOF pLoginServerInfo = NULL;
		if(CGameProcedure::s_pProcLogIn)
		{
			pLoginServerInfo = CGameProcedure::s_pProcLogIn->GetAreaLoginServerInfo(nAreaIndex, nLoginServerIndex);
		
		}
	
		if(pLoginServerInfo)
		{
			// �õ�����������
			state->PushString(pLoginServerInfo->szLoginServerName.c_str());

			// �õ�������״̬
			state->PushInteger(pLoginServerInfo->iLoginServerStatus);
		}
		else
		{
			state->PushString("");
			state->PushInteger(0);
		}
	
		return 2;

	}

	// ���ӵ�login server
	INT CGameProduce_Login::ConnectToLoginServer(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}


		int nAreaIndex		  = args[2].GetInteger();
		int nLoginServerIndex = args[3].GetInteger();

		if(CGameProcedure::s_pProcLogIn)
		{
			
			CGameProcedure::s_pProcLogIn->ConnectToLoginServer(nAreaIndex, nLoginServerIndex);
		
		}
		return 0;
	}


	// �˵�������ѡ�����
	INT CGameProduce_Login::ExitToSelectServer(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::s_pProcLogIn)
		{
			
			CGameProcedure::s_pProcLogIn->OpenSelectServerDlg();
		
		}
		return 0;
	}


	// ��֤�û���������
	INT CGameProduce_Login::CheckAccount(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsString()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}


		const char* pUserName = args[2].GetString();
		const char* pPassword = args[3].GetString();

		if(CGameProcedure::s_pProcLogIn)
		{
			
			CGameProcedure::s_pProcLogIn->CheckAccount((CHAR*)pUserName, (CHAR*)pPassword);
		
		}
		return 0;
	}


	// ѡ��һ��������
	INT CGameProduce_Login::SelectLoginServer(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}


		int nAreaIndex		  = args[2].GetInteger();
		int nLoginServerIndex = args[3].GetInteger();

		if(CGameProcedure::s_pProcLogIn)
		{
			
			CGameProcedure::s_pProcLogIn->SelectLoginServer(nAreaIndex, nLoginServerIndex);
		
		}

		return 0;
	}

	// ������ѡ������л����ʺ��������
	INT CGameProduce_Login::ChangeToAccountInputDlgFromSelectRole(LuaPlus::LuaState* state)
	{
		
		if(CGameProcedure::s_pProcCharSel)
		{
			
			CGameProcedure::s_pProcCharSel->ChangeToAccountInput();
		
		}
		return 0;
	}

	// ������ѡ������л������ﴴ������
	INT CGameProduce_Login::ChangeToCreateRoleDlgFromSelectRole(LuaPlus::LuaState* state)
	{
		
		if(CGameProcedure::s_pProcCharSel)
		{
			if(CGameProcedure::s_pProcCharSel->GetCurRoleCount() >= 3)
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "��ɫ�Ѿ�����! ");	
				return 0;
			}
			CGameProcedure::s_pProcCharSel->ChangeToCreateRole();
		}
		return 0;
	}

	// �����ﴴ�������л�������ѡ�����
	INT CGameProduce_Login::ChangeToSelectRoleDlgFromCreateRole(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::s_pProcCharCreate)
		{
			
			CGameProcedure::s_pProcCharCreate->ChangeToSelectRole();
		
		}//
		return 0;
	}

	// �õ���ɫ�ĸ���
	INT CGameProduce_Login::GetRoleCount(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		int iRoleCount = 0;
		if(CGameProcedure::s_pProcCharSel)
		{
			
			iRoleCount = CGameProcedure::s_pProcCharSel->GetCurRoleCount();

		}//	
		
		state->PushInteger(iRoleCount);
		return 1;
	}

	// �õ���ɫ����Ϣ
	INT CGameProduce_Login::GetRoleInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int iRoleIndex = args[2].GetInteger();
		const DB_CHAR_BASE_INFO* pInfo = NULL;
		if(CGameProcedure::s_pProcCharSel)
		{
			
			pInfo = CGameProcedure::s_pProcCharSel->GetRoleInfo(iRoleIndex);
			if(pInfo)
			{
				// ����
				state->PushString(pInfo->m_Name);
				// ����
				state->PushInteger(pInfo->m_Menpai);
				// �ȼ�
				state->PushInteger(pInfo->m_Level);
				// ��Ӫ
				state->PushInteger(pInfo->m_Camp);
				return 4;
			}

		}//	
		
		// ����
		state->PushString("");
		// ����
		state->PushInteger(0);
		// �ȼ�
		state->PushInteger(0);
		// ��Ӫ
		state->PushInteger(0);
		return 4;
		
	}


	// �õ���ɫ��uiģ������
	INT CGameProduce_Login::GetRoleUIModleName(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int iRoleIndex = args[2].GetInteger();
		
		const TCHAR* pUIModleName = NULL;
		if(CGameProcedure::s_pProcCharSel)
		{
			pUIModleName = CGameProcedure::s_pProcCharSel->GetRoleUIName(iRoleIndex);
		}//	
		
		if(pUIModleName)
		{
			// ����
			state->PushString(pUIModleName);
		}
		else
		{
			// ����
			state->PushString("");
		}

		return 1;
	}

	// ������ɫ
	INT CGameProduce_Login::CreateRole(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		
		if(CGameProcedure::s_pProcCharCreate)
		{
			const char* pChar = args[2].GetString();
			int iSex = args[3].GetInteger();
			if(pChar)
			{
				int iStrLen = (int)::strlen(pChar);

				if(iStrLen > 12)
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "��ɫ������");
					return 0;
				}

				CGameProcedure::s_pProcCharCreate->m_iSex = iSex;
				strcpy(CGameProcedure::s_pProcCharCreate->m_szCreateRoleName, pChar);
				CGameProcedure::s_pProcCharCreate->CreateRole();
			}
			else
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "��ɫ������Ϊ��");
			}
		}
		return 0;
	}

	// ɾ����ɫ
	INT CGameProduce_Login::DelRole(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if(CGameProcedure::s_pProcCharSel)
		{
			int iRoleIndex = args[2].GetInteger();
			CGameProcedure::s_pProcCharSel->DelRole(iRoleIndex);

			
		}//	

		return 0;
		
	}

	// ɾ��ѡ��Ľ�ɫ
	INT CGameProduce_Login::DelSelRole(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::s_pProcCharSel)
		{

			CGameProcedure::s_pProcCharSel->DelSelRole();
			
		}//	

		return 0;
		
	}

	// ���ͽ�����Ϸ��Ϣ
	INT CGameProduce_Login::SendEnterGameMsg(LuaPlus::LuaState* state)
	{
		
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if(CGameProcedure::s_pProcCharSel)
		{
			int iRoleIndex = args[2].GetInteger();
			CGameProcedure::s_pProcCharSel->SendEnterGameMsg(iRoleIndex);

			
		}//	

		return 0;
	}

	// ѯ���Ƿ�ɾ����ɫ
	INT CGameProduce_Login::DelRole_YesNo(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int iRoleIndex = args[2].GetInteger();
		CGameProcedure::s_pProcCharSel->m_iCurSelRoleDel = iRoleIndex;
		std::vector< STRING > strParamInfo;

		if(CGameProcedure::s_pProcCharSel->IsValidIndex(iRoleIndex))
		{
			// 
			std::string strInfo = ""; 
			// ͨ�������õ���ɫ����Ϣ
			const DB_CHAR_BASE_INFO* pInfo = CGameProcedure::s_pProcCharSel->GetRoleInfo(iRoleIndex);
			if(pInfo)
			{
				//if(pInfo->m_Level < 9)
				{
					TCHAR szBuf[128];
					_stprintf(szBuf, _T("��ȷ��Ҫ�� %d�� �Ľ�ɫ#c00ff00 %s #cffffffɾ����?"), pInfo->m_Level, pInfo->m_Name);
					strParamInfo.push_back(szBuf);
					strParamInfo.push_back("1");
					((CEventSystem*)CGameProcedure::s_pEventSystem)->PushEvent( GE_GAMELOGIN_SYSTEM_INFO_YESNO, strParamInfo);
				}
			}
			else
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "��ѡ��һ��Ҫɾ���Ľ�ɫ.");
			}
		}
		else
		{
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "��ѡ��һ��Ҫɾ���Ľ�ɫ.");
		}
		
		return 0;
	}

	// ѯ���Ƿ��˳���Ϸ
	INT CGameProduce_Login::ExitGame_YesNo(LuaPlus::LuaState* state)
	{
		std::vector< STRING > strParamInfo;

		strParamInfo.push_back("ȷ��Ҫ�˳���Ϸ��?");
		strParamInfo.push_back("0");
		 
		((CEventSystem*)CGameProcedure::s_pEventSystem)->PushEvent( GE_GAMELOGIN_SYSTEM_INFO_YESNO, strParamInfo);
		return 0;
	}

	// ѯ���Ƿ��˵��ʺ��������
	INT CGameProduce_Login::ExitToAccountInput_YesNo(LuaPlus::LuaState* state)
	{
		std::vector< STRING > strParamInfo;
		strParamInfo.push_back("ȷ���˵��ʺ����������?");
		strParamInfo.push_back("2");
		 
		((CEventSystem*)CGameProcedure::s_pEventSystem)->PushEvent( GE_GAMELOGIN_SYSTEM_INFO_YESNO, strParamInfo);
		return 0;
	}

	// �Զ�ѡ��һ���뵱ǰѡ������繩Ӧ��ƥ���login server
	INT CGameProduce_Login::AutoSelLoginServer(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int iIndex = args[2].GetInteger();
		CGameProcedure::s_pProcLogIn->AutoSelLoginServer(iIndex);
   		return 0;
	}

	
	// �õ�Ů����ͷ��ĸ���
	INT CGameProduce_Login::GetWomanFaceCount(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		
		int iFaceCount = 0;

		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFace))
		{
			iFaceCount = CGameProcedure::s_pProcCharCreate->m_FaceMng.GetWomanFaceCount();
		}
			
		// Ů�������εĸ���
		state->PushInteger(iFaceCount);
		return 1;
	}

	// �õ�������ͷ��ĸ���
	INT CGameProduce_Login::GetManFaceCount(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		
		int iFaceCount = 0;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFace))
		{
			iFaceCount = CGameProcedure::s_pProcCharCreate->m_FaceMng.GetManFaceCount();
		}

		// ���������εĸ���
		state->PushInteger(iFaceCount);
		return 1;
	}

	// �õ�Ů����ͷ�������
	INT CGameProduce_Login::GetWomanFaceName(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// ͷ������
		int iIndex = args[2].GetInteger();
		LPCSTR pImageName = NULL;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFace))
		{
			LPCSTR pImageName = CGameProcedure::s_pProcCharCreate->m_FaceMng.GetFaceImageInfo(0, iIndex);
		}

		if(pImageName)
		{
			// ����
			state->PushString(pImageName);
		}
		else
		{
			// ����
			state->PushString("");
		}

		return 1;
	}

	// �õ�������ͷ�������
	INT CGameProduce_Login::GetManFaceName(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// ͷ������
		int iIndex = args[2].GetInteger();
		LPCSTR pImageName = NULL;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFace))
		{
			LPCSTR pImageName = CGameProcedure::s_pProcCharCreate->m_FaceMng.GetFaceImageInfo(1, iIndex);
		}

		if(pImageName)
		{
			// ����
			state->PushString(pImageName);
		}
		else
		{
			// ����
			state->PushString("");
		}

		return 1;
	}

	// �õ�����ͷ�������
	INT CGameProduce_Login::GetFaceName(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// ͷ������
		int iRace  = args[2].GetInteger();
		int iIndex = args[3].GetInteger();

		LPCSTR pImageName = NULL;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFace))
		{
			pImageName = CGameProcedure::s_pProcCharCreate->m_FaceMng.GetFaceImageInfo(iRace, iIndex);
		}

		if(pImageName)
		{
			// ����
			state->PushString(pImageName);
		}
		else
		{
			// ����
			state->PushString("");
		}

		return 1;
	}

	// ����ͷ��id
	INT CGameProduce_Login::SetFaceId(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// ͷ������
		int iRace  = args[2].GetInteger();
		int iIndex = args[3].GetInteger();

		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFace))
		{
			CGameProcedure::s_pProcCharCreate->SetFaceByRaceAndIndex(iRace, iIndex);
		}

		return 0;
	}


	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// �������β���.
	//
	
	// �õ�����model�ĸ���
	INT CGameProduce_Login::GetFaceModelCount(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// �Ա�
		int iRace  = args[2].GetInteger();

		int iModelCount = 0;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFaceModel))
		{
			iModelCount = CGameProcedure::s_pProcCharCreate->m_FaceModelmng.GetFaceModelCount(iRace);
		}

		state->PushInteger(iModelCount);
		return 1;
	}

	// �õ�����ģ�͵�����
	INT CGameProduce_Login::GetFaceModelName(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// ͷ������
		int iRace  = args[2].GetInteger();
		int iIndex = args[3].GetInteger();

		LPCSTR pImageName = NULL;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFaceModel))
		{
			//pImageName = CGameProcedure::s_pProcCharCreate->m_FaceModelmng.GetFaceModel(iRace, iIndex);
			// 2006-4-6
			pImageName = CGameProcedure::s_pProcCharCreate->m_FaceModelmng.GetFaceModelShowName(iRace, iIndex);
		}

		if(pImageName)
		{
			// ����
			state->PushString(pImageName);
		}
		else
		{
			// ����
			state->PushString("");
		}
		return 1;
	}

	// ��������ģ��id
	INT CGameProduce_Login::SetFaceModelId(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// ͷ������
		int iRace  = args[2].GetInteger();
		int iIndex = args[3].GetInteger();

		
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFaceModel))
		{
			CGameProcedure::s_pProcCharCreate->SetFaceModelByRaceAndIndex(iRace, iIndex);
		}

		return 0;
	}



	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ���ڷ��β���.
	//
	
	// �õ�����model�ĸ���
	INT CGameProduce_Login::GetHairModelCount(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// �Ա�
		int iRace  = args[2].GetInteger();

		int iModelCount = 0;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadHairModel))
		{
			iModelCount = CGameProcedure::s_pProcCharCreate->m_HairModelMng.GetHairModelCount(iRace);
		}

		state->PushInteger(iModelCount);
		return 1;
	}

	// �õ�����ģ�͵�����
	INT CGameProduce_Login::GetHairModelName(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// ͷ������
		int iRace  = args[2].GetInteger();
		int iIndex = args[3].GetInteger();

		LPCSTR pImageName = NULL;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadHairModel))
		{
			//pImageName = CGameProcedure::s_pProcCharCreate->m_HairModelMng.GetHairModel(iRace, iIndex);
			//2006-4-6
			pImageName = CGameProcedure::s_pProcCharCreate->m_HairModelMng.GetHairModelShowName(iRace, iIndex);
		}

		if(pImageName)
		{
			// ����
			state->PushString(pImageName);
		}
		else
		{
			// ����
			state->PushString("");
		}

		return 1;
	}

	// ��������ģ��id
	INT CGameProduce_Login::SetHairModelId(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// ͷ������
		int iRace  = args[2].GetInteger();
		int iIndex = args[3].GetInteger();

		
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFaceModel))
		{
			CGameProcedure::s_pProcCharCreate->SetHairModelByRaceAndIndex(iRace, iIndex);
		}

		return 0;
	}

	// �õ������Ӫ
	INT CGameProduce_Login::GetPlayerCamp(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		state->PushInteger( CGameProcedure::s_pProcCharCreate->m_iCamp );

		return 1;
	}

	INT CGameProduce_Login::SetPlayerCamp(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int iCamp  = args[2].GetInteger();
		if( iCamp != CAMP1_PLAYER && iCamp != CAMP2_PLAYER )
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		CGameProcedure::s_pProcCharCreate->m_iCamp = iCamp;

		return 0;
	}

	INT CGameProduce_Login::SetPlayerMenpai(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int iMenPai  = args[2].GetInteger();
		if( iMenPai != MATTRIBUTE_SHAOLIN && 
			iMenPai != MATTRIBUTE_WUDANG &&
			iMenPai != MATTRIBUTE_EMEI )
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		CGameProcedure::s_pProcCharCreate->m_iMenpai = iMenPai;

		return 0;
	}

	// �ر��������ӡ�2006��4��12
	INT CGameProduce_Login::CloseNetConnect(LuaPlus::LuaState* state)
	{

		CNetManager::GetMe()->Close();

		if(CGameProcedure::s_pProcCharSel)
		{
			CGameProcedure::s_pProcCharSel->ChangeToAccountInput();
		}
		return 0;
	}
}