#include "StdAfx.h"
#include "GMScriptSystem.h"
#include "TDException.h"
#include "../Interface/GMGameInterface_Script.h"
#include "../Action/GMActionSystem.h"
#include "../NetWork/NetManager.h"
#include "../Variable/GMVariable.h"
#include "../Procedure/Gameprocedure.h"
#include "../Interface/GMInterface_Script_Tooltip.h"
#include "../Interface/GMGameInterface_Script_Package.h"
#include "../Interface/GMInterface_Script_PetInviteFriend.h"
#include "../Interface/GMInterface_Script_Guild.h"
#include "TDTimeSystem.h"
#include "../Global.h"
#include "TDUtil.h"
#include "../Interface/GMInterface_Script_SystemSetup.h"      
#include "../Procedure/GameProcedure.h"
#include "../WxRender/ResourceProvider.h"


#define FILE_SCRIPT		"Script.dat"

//=========================================================================
CScriptEnvironment::CScriptEnvironment(LPCTSTR szName, LPCTSTR szFileName)
{
	TDAssert(szName);

	//��������Table
	m_strName = szName;
	m_strFileName = szFileName;
	m_strNameInLua = m_strName+"_Env";
	m_theEnvObj = CScriptSystem::GetMe()->GetLuaState()->GetGlobals().CreateTable(m_strNameInLua.c_str());
}

CScriptEnvironment::~CScriptEnvironment()
{
}

INT CScriptEnvironment::DoFile(LPCTSTR szScriptFile)
{
	char* lpAddress = NULL;
	DWORD dwSize = CGameProcedure::s_pResourceProvider->loadResource( szScriptFile, lpAddress, "General" );
	if( dwSize > 0 )
	{
		INT result = CScriptSystem::GetMe()->GetLuaState()->DoString(lpAddress);
		CGameProcedure::s_pResourceProvider->unloadResource( lpAddress, dwSize );
		return result;
	}
	return 0;
}

void CScriptEnvironment::DoString_WithCPPException(LPCTSTR szString)
{
	try
	{
		//����ɻ�����
		STRING strOldEnv = CScriptSystem::GetMe()->GetActiveEnvironment();
		//�趨����
		CScriptSystem::GetMe()->SetActiveEnvironment(m_strNameInLua.c_str());
		//ִ�к���
		CScriptSystem::GetMe()->GetLuaState()->DoString(szString);
		//�ָ�����
		if(!strOldEnv.empty())
		{
			CScriptSystem::GetMe()->SetActiveEnvironment(strOldEnv.c_str());
		}
	}
	catch(const std::exception& e)
	{
		static STRING strCPPException;
		strCPPException = e.what();
		
		//ת��Ϊ�ṹ���쳣
		LPVOID pException = &strCPPException;
		::RaiseException(
			0XE000C0DE, 
			EXCEPTION_NONCONTINUABLE,				// ���ɼ��������ش���
			1,										// 1������
			(CONST ULONG_PTR *)&pException);		// ָ��̬�쳣�ڴ�
	}
	catch(...)
	{
		throw;
	}
}

void CScriptEnvironment::DoString_WithGenException(LPCTSTR szString)
{
	CHAR szTitle[MAX_PATH];
	_snprintf(szTitle, MAX_PATH, "LUA: %s", m_strFileName.c_str());

	__try 
	{
		DoString_WithCPPException(szString);
	}
	__except (tProcessInnerException(GetExceptionInformation(), g_hMainWnd, szTitle), EXCEPTION_EXECUTE_HANDLER) {}
}

VOID CScriptEnvironment::DoString(LPCTSTR szString)
{
	//�����κ��쳣
	if(!CScriptSystem::GetMe()->IsEnableLuaCrash())
	{
		try{
		//����ɻ�����
		STRING strOldEnv = CScriptSystem::GetMe()->GetActiveEnvironment();
		//�趨����
		CScriptSystem::GetMe()->SetActiveEnvironment(m_strNameInLua.c_str());
		//ִ�к���
		CScriptSystem::GetMe()->GetLuaState()->DoString(szString);
		//�ָ�����
		if(!strOldEnv.empty())
		{
			CScriptSystem::GetMe()->SetActiveEnvironment(strOldEnv.c_str());
		}
		}catch(...) {}
		return;
	}

	//����ṹ���쳣��ִ��
	DoString_WithGenException(szString);
}

VOID CScriptEnvironment::DoFunction(LPCTSTR szFunction, LPCTSTR szParam)
{
	CHAR szDoFunction[MAX_PATH];
	_snprintf(szDoFunction, MAX_PATH, "%s(%s);", szFunction, szParam ? szParam : "");
	DoString(szDoFunction);
}

//=========================================================================
LuaPlus::LuaState* CScriptSystem::_GetLuaState(void)
{
	//��֤���Ա���ʼ��
	static LuaPlus::LuaStateOwner s_theLuaState;
	return s_theLuaState.Get();
}


WX_IMPLEMENT_DYNAMIC(CScriptSystem, GETCLASS(tScriptSystem));
CScriptSystem* CScriptSystem::s_pMe = NULL;
CScriptSystem::CScriptSystem()
{
	s_pMe = this;
}

CScriptSystem::~CScriptSystem()
{
	TDAssert(m_listScriptEnv.empty());
}

tScriptEnvironment* CScriptSystem::NewEnvironment(LPCTSTR szName, LPCTSTR szFileName)
{
	CScriptEnvironment* pNewEnv = new CScriptEnvironment(szName, szFileName);
	m_listScriptEnv.push_back(pNewEnv);

	return (tScriptEnvironment*)pNewEnv;
}

VOID CScriptSystem::DestroyEnvironment(tScriptEnvironment* pEnv)
{
	std::list< CScriptEnvironment* >::iterator it;
	for(it=m_listScriptEnv.begin(); it!=m_listScriptEnv.end(); it++)
	{
		if((*it) == pEnv)
		{
			delete pEnv;
			m_listScriptEnv.erase(it);
			return;
		}
	}
}

const STRING& CScriptSystem::GetQuestFile(int nID)
{
	const static STRING strEmpty = "";

	std::map< int, STRING >::iterator itFind;

	itFind = m_mapQuestFile.find(nID);
	if(itFind == m_mapQuestFile.end()) return strEmpty;

	return itFind->second;
}

VOID CScriptSystem::Initial(VOID*)
{
	//��������ű��б�
	_loadQuestFile();

	//---------------------------------------------------------------------------------
	//�Ƿ��׳��쳣
	BOOL bHave;
	m_bEnableLuaCrash = CGameProcedure::s_pVariableSystem->GetAs_Int("EnableLuaCrash", &bHave);
	if(!bHave) m_bEnableLuaCrash = TRUE;

	LuaPlus::LuaState* pState = _GetLuaState();
	LuaPlus::LuaObject objGlobal = _GetLuaState()->GetGlobals();

	//---------------------------------------------------------------------------------
	//ע��ȫ�ֺ���
	objGlobal.Register("ServerLogin", SCRIPT_SANDBOX::Lua_ServerLogin);
	objGlobal.Register("QuitApplication", SCRIPT_SANDBOX::Lua_QuitApplication);
	objGlobal.Register("ToggleSkillBook", SCRIPT_SANDBOX::Lua_ToggleSkillBook);
	objGlobal.Register("OpenSkillBook", SCRIPT_SANDBOX::Lua_OpenSkillBook);
	objGlobal.Register("TogglePetPage", SCRIPT_SANDBOX::Lua_TogglePetPage);
	objGlobal.Register("ToAUTOHit", SCRIPT_SANDBOX::Lua_ToAUTOHit);
	objGlobal.Register("OpenLifePage", SCRIPT_SANDBOX::Lua_OpenLifePage);
	objGlobal.Register("OpenCommonSkillPage", SCRIPT_SANDBOX::Lua_OpenCommonSkillPage);
	objGlobal.Register("ToggleContainer", SCRIPT_SANDBOX::Lua_ToggleContainer);//fujia 2007.10.24
	objGlobal.Register("ToggleContainer1", SCRIPT_SANDBOX::Lua_ToggleContainer1);
	objGlobal.Register("ToggleContainer2", SCRIPT_SANDBOX::Lua_ToggleContainer2);
	objGlobal.Register("ToggleMission", SCRIPT_SANDBOX::Lua_ToggleMission);
	objGlobal.Register("GetActionNum", SCRIPT_SANDBOX::Lua_GetActionNum);
	objGlobal.Register("EnumAction", SCRIPT_SANDBOX::Lua_EnumAction);
	objGlobal.Register("ShowContexMenu", SCRIPT_SANDBOX::Lua_ShowContexMenu);
	objGlobal.Register("QuestFrameOptionClicked", SCRIPT_SANDBOX::Lua_QuestFrameOptionClicked);
	objGlobal.Register("ComposeItem_Begin", SCRIPT_SANDBOX::Lua_ComposeItem_Begin);
	objGlobal.Register("ComposeItem_Cancel", SCRIPT_SANDBOX::Lua_ComposeItem_Cancel);
	objGlobal.Register("QuestFrameAcceptClicked", SCRIPT_SANDBOX::Lua_QuestFrameAcceptClicked);
	objGlobal.Register("QuestFrameRefuseClicked", SCRIPT_SANDBOX::Lua_QuestFrameRefuseClicked);
	objGlobal.Register("QuestFrameMissionContinue", SCRIPT_SANDBOX::Lua_QuestFrameMissionContinue);
	objGlobal.Register("QuestFrameMissionComplete", SCRIPT_SANDBOX::Lua_QuestFrameMissionComplete);
	objGlobal.Register("QuestFrameMissionAbnegate", SCRIPT_SANDBOX::Lua_QuestFrameMissionAbnegate);
	objGlobal.Register("PushDebugMessage", SCRIPT_SANDBOX::Lua_PushDebugMessage);
	objGlobal.Register("Get_XParam_INT", SCRIPT_SANDBOX::Lua_Get_XParam_INT);
	objGlobal.Register("Get_XParam_STR", SCRIPT_SANDBOX::Lua_Get_XParam_STR);
	objGlobal.Register("Clear_XSCRIPT", SCRIPT_SANDBOX::Lua_Clear_XSCRIPT);
	objGlobal.Register("Send_XSCRIPT", SCRIPT_SANDBOX::Lua_Send_XSCRIPT);
	objGlobal.Register("Set_XSCRIPT_Parameter", SCRIPT_SANDBOX::Lua_Set_XSCRIPT_Parameter);
	objGlobal.Register("Set_XSCRIPT_Function_Name", SCRIPT_SANDBOX::Lua_Set_XSCRIPT_Function_Name);
	objGlobal.Register("Set_XSCRIPT_ScriptID", SCRIPT_SANDBOX::Lua_Set_XSCRIPT_ScriptID);
	objGlobal.Register("Set_XSCRIPT_ParamCount", SCRIPT_SANDBOX::Lua_Set_XSCRIPT_ParamCount);
	objGlobal.Register("IsWindowShow", SCRIPT_SANDBOX::Lua_IsWindowShow);
	objGlobal.Register("CloseAllWindow", SCRIPT_SANDBOX::Lua_CloseAllWindow );
	objGlobal.Register("SkillsStudyFrame_study", SCRIPT_SANDBOX::lua_SkillsStudyFrame_study);
	objGlobal.Register("GetUplevelXinfaSpendMoney", SCRIPT_SANDBOX::Lua_GetUplevelXinfaSpendMoney);
	objGlobal.Register("GetUplevelXinfaSpendExp", SCRIPT_SANDBOX::Lua_GetUplevelXinfaSpendExp);
	objGlobal.Register("GetStudySkillMoney", SCRIPT_SANDBOX::Lua_GetStudySkillMoney);
	objGlobal.Register("GetStudySkillLevel", SCRIPT_SANDBOX::Lua_GetStudySkillLevel);
	objGlobal.Register("GetStudySkillName", SCRIPT_SANDBOX::Lua_GetStudySkillName);
	//objGlobal.Register("GetXinfaLevel", SCRIPT_SANDBOX::Lua_GetXinfaLevel);
	objGlobal.Register("GetStudySkillMS", SCRIPT_SANDBOX::Lua_GetStudySkillMS);
	objGlobal.Register("GetSkillImage", SCRIPT_SANDBOX::Lua_GetSkillImage);
	//********************************************************************************************
	// �ϳɽ���
	objGlobal.Register("ToggleComposeWnd", SCRIPT_SANDBOX::Lua_ToggleComposeWnd);
	// ö�����е��䷽
	objGlobal.Register("EnumMethod", SCRIPT_SANDBOX::Lua_EnumMethod);
	// �õ�ĳһ���䷽������
	objGlobal.Register("GetMethodNumber", SCRIPT_SANDBOX::Lua_GetMethodNumber);
	objGlobal.Register("SendChatMessage", SCRIPT_SANDBOX::Lua_SendChatMessage);
	objGlobal.Register("SendGMCommand", SCRIPT_SANDBOX::Lua_SendGMCommand);
	objGlobal.Register("AxTrace", SCRIPT_SANDBOX::Lua_AxTrace);
	objGlobal.Register("SendAUTOMove", SCRIPT_SANDBOX::Lua_SendAUTOMove);
	objGlobal.Register("ToggleLargeMap", SCRIPT_SANDBOX::Lua_ToggleLargeMap);
	objGlobal.Register("ToggleSceneMap", SCRIPT_SANDBOX::Lua_ToggleSceneMap);

	objGlobal.Register("UpdateMinimap", SCRIPT_SANDBOX::Lua_UpdateMinimap);
	objGlobal.Register("OpenMinimap", SCRIPT_SANDBOX::Lua_OpenMinimap);

	//�򿪹ر����˽��׽���

	objGlobal.Register("OpenBooth", SCRIPT_SANDBOX::Lua_OpenBooth);
	objGlobal.Register("CloseBooth", SCRIPT_SANDBOX::Lua_CloseBooth);
	objGlobal.Register("RepairAll", SCRIPT_SANDBOX::Lua_RepairAll);
	objGlobal.Register("RepairOne", SCRIPT_SANDBOX::Lua_RepairOne);

	objGlobal.Register("GetOperationType", SCRIPT_SANDBOX::Lua_GetOperationType);
	objGlobal.Register("GetShopType", SCRIPT_SANDBOX::Lua_GetShopType);

	objGlobal.Register("GetCurrentSceneName", SCRIPT_SANDBOX::Lua_GetCurrentSceneName);
	objGlobal.Register("GetCurrentSceneNameById", SCRIPT_SANDBOX::Lua_GetCurrentSceneNameById);
	objGlobal.Register("GetSceneSize", SCRIPT_SANDBOX::Lua_GetSceneSize);
	objGlobal.Register("GetSceneID", SCRIPT_SANDBOX::Lua_GetSceneID);
	objGlobal.Register("ResetCamera", SCRIPT_SANDBOX::Lua_ResetCamera);
	objGlobal.Register("OpenCharacter", SCRIPT_SANDBOX::Lua_OpenCharacter);
	objGlobal.Register("OpenEquip", SCRIPT_SANDBOX::Lua_OpenEquip);
	objGlobal.Register("AskLevelUp", SCRIPT_SANDBOX::Lua_AskLevelUp);
	objGlobal.Register("AutoHit", SCRIPT_SANDBOX::Lua_AutoHit);
	//�򿪹رճƺŽ���
	objGlobal.Register("OpenTitleList", SCRIPT_SANDBOX::Lua_OpenTitleList);
	objGlobal.Register("CloseTitleList", SCRIPT_SANDBOX::Lua_CloseTitleList);

	// ��װ������
	objGlobal.Register("OpenEquip", SCRIPT_SANDBOX::Lua_OpenSelfEquip);
	

	// ����װ������ϸ��Ϣ
	objGlobal.Register("AskEquipDetial", SCRIPT_SANDBOX::Lua_AskEquipDetial);

	//---------------------------------------------------------------------------------------------------------------------------------------------
	// ȫ�ֺ���������.
	//

	// ע������Ҽ��˵���
	objGlobal.Register("Show_Team_Func_Menu", SCRIPT_SANDBOX::Lua_ShowTeamFuncMenu);

	// ���õ�ǰѡ��Ķ�Ա
	objGlobal.Register("SetCurSelMember", SCRIPT_SANDBOX::Lua_SetCurSelMember);

	// ��ʾ����Ի���
	objGlobal.Register("ShowTeamInfoDlg",     SCRIPT_SANDBOX::Lua_ShowTeamInfoDlg);

	// ��˸���鰴ť
	objGlobal.Register("FlashTeamButton",     SCRIPT_SANDBOX::Lua_FlashTeamButton);

	// ����һ����Ʒ
	objGlobal.Register("DiscardItem",     SCRIPT_SANDBOX::Lua_DiscardItem);

	// ȡ������һ����Ʒ
	objGlobal.Register("DiscardItemCancelLocked",     SCRIPT_SANDBOX::Lua_DiscardItemCancelLocked);

	// ����һ����Ч
	objGlobal.Register("PlaySound",     SCRIPT_SANDBOX::Lua_PlaySound);
	
	//
	// ȫ�ֺ���������
	//---------------------------------------------------------------------------------------------------------------------------------------------

	// ����һ��ȫ�ֱ���
	objGlobal.Register("SetGlobalInteger",     SCRIPT_SANDBOX::Lua_SetGlobalInteger);

	// ���һ��ȫ�ֱ���
	objGlobal.Register("GetGlobalInteger",     SCRIPT_SANDBOX::Lua_GetGlobalInteger);

	// ���target����, �Ҽ��򿪲˵�
	objGlobal.Register("OpenTargetMenu",     SCRIPT_SANDBOX::Lua_OpenTargetMenu);
	
	// ���target����, �Ҽ��򿪲˵�
	objGlobal.Register("GetMousePos",     SCRIPT_SANDBOX::Lua_GetMousePos);
	
	// ��ʾϵͳ��ʾ��Ϣ��
	objGlobal.Register("ShowSystemTipInfo",     SCRIPT_SANDBOX::Lua_ShowSystemTipInfo);

	// �򿪸��Ķ�������Ի���2006��3��27
	objGlobal.Register("OpenChangeMinorPasswordDlg",     SCRIPT_SANDBOX::Lua_OpenChangeMinorPasswordDlg);

	// �򿪽������롣2006��3��27
	objGlobal.Register("OpenUnLockeMinorPasswordDlg",     SCRIPT_SANDBOX::Lua_OpenUnLockeMinorPasswordDlg);

	
	// ������������档2006��3��28
	objGlobal.Register("OpenSetMinorPasswordDlg",     SCRIPT_SANDBOX::Lua_OpenSetMinorPasswordDlg);

	// �������롣2006��3��28
	objGlobal.Register("SendSetMinorPassword",     SCRIPT_SANDBOX::Lua_SendSetMinorPassword);

	// �޸����롣2006��3��28
	objGlobal.Register("SendModifyMinorPassword",     SCRIPT_SANDBOX::Lua_SendModifyMinorPassword);

	// �������롣2006��3��28
	objGlobal.Register("UnLockMinorPassword",     SCRIPT_SANDBOX::Lua_UnLockMinorPassword);

	// ǿ�ƽ������롣2006��3��28
	objGlobal.Register("ForceUnLockMinorPassword",     SCRIPT_SANDBOX::Lua_ForceUnLockMinorPassword);

	// �޸����롣2006��3��28
	objGlobal.Register("ModifyMinorPassword",     SCRIPT_SANDBOX::Lua_ModifyMinorPassword);

	objGlobal.Register("GetCurrentTime",     SCRIPT_SANDBOX::Lua_GetCurrentTime);

	// ������λ����Ļ���꣩
	objGlobal.Register("GetCurMousePos",     SCRIPT_SANDBOX::Lua_GetCurMousePos);
	
	//---------------------------------------------------------------------------------------------------------------------------------------------
	//ע���ɫ Target
	LuaObject metatable_Character = objGlobal.CreateTable("CharacterMetaTable");
	metatable_Character.SetObject("__index", metatable_Character);

	metatable_Character.RegisterObjectFunctor("IsPresent", &SCRIPT_SANDBOX::Character::IsPresent);
	metatable_Character.RegisterObjectFunctor("GetName", &SCRIPT_SANDBOX::Character::GetName);
	metatable_Character.RegisterObjectFunctor("GetHPPercent", &SCRIPT_SANDBOX::Character::GetHPPercent);
	metatable_Character.RegisterObjectFunctor("GetMPPercent", &SCRIPT_SANDBOX::Character::GetMPPercent);
	metatable_Character.RegisterObjectFunctor("GetRagePercent", &SCRIPT_SANDBOX::Character::GetRagePercent);
	metatable_Character.RegisterObjectFunctor("Follow", &SCRIPT_SANDBOX::Character::Follow );
	metatable_Character.RegisterObjectFunctor("GetPos", &SCRIPT_SANDBOX::Character::GetPos );

	// �����ȼ�
	metatable_Character.RegisterObjectFunctor("GetLevel", &SCRIPT_SANDBOX::Character::GetLevel );
	metatable_Character.RegisterObjectFunctor("GetData", &SCRIPT_SANDBOX::Character::GetData );
	metatable_Character.RegisterObjectFunctor("SendAskDetail", &SCRIPT_SANDBOX::Character::SendAskDetail );
	metatable_Character.RegisterObjectFunctor("Set_To_Private", &SCRIPT_SANDBOX::Character::Set_To_Private );	
	metatable_Character.RegisterObjectFunctor("Close_Before_TargetEquip_UI", &SCRIPT_SANDBOX::Character::Close_Before_TargetEquip_UI );	
	metatable_Character.RegisterObjectFunctor("TargetEquip_ChangeModel", &SCRIPT_SANDBOX::Character::TargetEquip_ChangeModel );	

	
	//---------------------------------------------------------------------------------------------------------------------------------------------
	// Target ������
	//

	// �����������
	metatable_Character.RegisterObjectFunctor("SendTeamRequest", &SCRIPT_SANDBOX::Character::SendTeamRequest );

	// ������������������
	metatable_Character.RegisterObjectFunctor("SendTeamApply", &SCRIPT_SANDBOX::Character::SendTeamApply );

	//
	// Target ������
	//---------------------------------------------------------------------------------------------------------------------------------------------

	metatable_Character.RegisterObjectFunctor("GetBuffNumber", &SCRIPT_SANDBOX::Character::GetBuffNumber );
	metatable_Character.RegisterObjectFunctor("GetBuffIconNameByIndex", &SCRIPT_SANDBOX::Character::GetBuffIconNameByIndex );
	metatable_Character.RegisterObjectFunctor("GetDialogNpcName", &SCRIPT_SANDBOX::Character::GetDialogNpcName );
	metatable_Character.RegisterObjectFunctor("GetShopNpcName", &SCRIPT_SANDBOX::Character::GetShopNpcName );
	//metatable_Character.RegisterObjectFunctor("GetXinfaNpcName", &SCRIPT_SANDBOX::Character::GetXinfaNpcName );



	//---------------------------------------------------------------------------------------------------------------------------------------------
	//
	// ��ǰ��target �Ƿ��Ƕ���2006-4-3
	//
	metatable_Character.RegisterObjectFunctor("IsTargetTeamMember", &SCRIPT_SANDBOX::Character::IsTargetTeamMember );

	// ��ǰ��target �Ƿ�Ϸ�2006��4��12
	metatable_Character.RegisterObjectFunctor("IsTargetValide", &SCRIPT_SANDBOX::Character::IsTargetValide );


	
	// ��ǰ��target Team���� 2006��4��12
	metatable_Character.RegisterObjectFunctor("TargetFrame_Update_Name_Team", &SCRIPT_SANDBOX::Character::TargetFrame_Update_Name_Team );

	
	// ��ǰ��target Team hp 2006��4��12
	metatable_Character.RegisterObjectFunctor("TargetFrame_Update_HP_Team", &SCRIPT_SANDBOX::Character::TargetFrame_Update_HP_Team );

	
	// ��ǰ��target Team mp 2006��4��12
	metatable_Character.RegisterObjectFunctor("TargetFrame_Update_MP_Team", &SCRIPT_SANDBOX::Character::TargetFrame_Update_MP_Team );

	
	// ��ǰ��target Team iRange 2006��4��12
	metatable_Character.RegisterObjectFunctor("TargetFrame_Update_Rage_Team", &SCRIPT_SANDBOX::Character::TargetFrame_Update_Rage_Team );

	
	// ��ǰ��target Team iLevel 2006��4��12
	metatable_Character.RegisterObjectFunctor("TargetFrame_Update_Level_Team", &SCRIPT_SANDBOX::Character::TargetFrame_Update_Level_Team );

	// ��ǰ��target Team ͷ�� 2006��4��12
	metatable_Character.RegisterObjectFunctor("TargetFrame_Update_Icon_Team", &SCRIPT_SANDBOX::Character::TargetFrame_Update_Icon_Team );

	
	metatable_Character.RegisterObjectFunctor("IsEnemy", &SCRIPT_SANDBOX::Character::IsEnemy );


	LuaObject obj_Target = pState->BoxPointer(&(SCRIPT_SANDBOX::Character::s_Target));
	obj_Target.SetMetaTable(metatable_Character);
	objGlobal.SetObject("Target", obj_Target);

	//---------------------------------------------------------------------------------
	//ע����� Player
	LuaObject metatable_PlayerMySelf = objGlobal.CreateTable("PlayerMySelfMetaTable");
	metatable_PlayerMySelf.SetObject("__index", metatable_PlayerMySelf);

	metatable_PlayerMySelf.RegisterObjectFunctor("GetData", &SCRIPT_SANDBOX::PlayerMySelf::GetData);
	metatable_PlayerMySelf.RegisterObjectFunctor("GetPos",&SCRIPT_SANDBOX::PlayerMySelf::GetPos );
	metatable_PlayerMySelf.RegisterObjectFunctor("IsPresent", &SCRIPT_SANDBOX::PlayerMySelf::IsPresent);
	metatable_PlayerMySelf.RegisterObjectFunctor("GetName", &SCRIPT_SANDBOX::PlayerMySelf::GetName);
	metatable_PlayerMySelf.RegisterObjectFunctor("GetBuffNumber", &SCRIPT_SANDBOX::PlayerMySelf::GetBuffNumber);
	metatable_PlayerMySelf.RegisterObjectFunctor("GetBuffIconNameByIndex", &SCRIPT_SANDBOX::PlayerMySelf::GetBuffIconNameByIndex);
	metatable_PlayerMySelf.RegisterObjectFunctor("GetBuffToolTipsByIndex", &SCRIPT_SANDBOX::PlayerMySelf::GetBuffToolTipsByIndex);
	metatable_PlayerMySelf.RegisterObjectFunctor("DispelBuffByIndex", &SCRIPT_SANDBOX::PlayerMySelf::DispelBuffByIndex);
	metatable_PlayerMySelf.RegisterObjectFunctor("GetBuffTimeTextByIndex", &SCRIPT_SANDBOX::PlayerMySelf::GetBuffTimeTextByIndex);

	//ѧϰ��������
	metatable_PlayerMySelf.RegisterObjectFunctor("GetAbilityInfo", &SCRIPT_SANDBOX::PlayerMySelf::GetAbilityInfo);
	metatable_PlayerMySelf.RegisterObjectFunctor("GetSkillInfo", &SCRIPT_SANDBOX::PlayerMySelf::GetSkillInfo);
	metatable_PlayerMySelf.RegisterObjectFunctor("GetXinfaInfo", &SCRIPT_SANDBOX::PlayerMySelf::GetSkillClassInfo);

	metatable_PlayerMySelf.RegisterObjectFunctor("AskLeanAbility", &SCRIPT_SANDBOX::PlayerMySelf::AskLeanAbility);

	//�ƺ����
	metatable_PlayerMySelf.RegisterObjectFunctor("GetAgnameNum", &SCRIPT_SANDBOX::PlayerMySelf::GetAgnameNum);
	metatable_PlayerMySelf.RegisterObjectFunctor("EnumAgname", &SCRIPT_SANDBOX::PlayerMySelf::EnumAgname);
	metatable_PlayerMySelf.RegisterObjectFunctor("GetCurrentAgname", &SCRIPT_SANDBOX::PlayerMySelf::GetCurrentAgname);
	metatable_PlayerMySelf.RegisterObjectFunctor("AskChangeCurrentAgname", &SCRIPT_SANDBOX::PlayerMySelf::AskChangeCurrentAgname);
	metatable_PlayerMySelf.RegisterObjectFunctor("SetNullAgname", &SCRIPT_SANDBOX::PlayerMySelf::SetNullAgname);					// ��ѡ��ƺ�, 2006-3-14


	

	

	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	// װ�����Խ���
	//

	// �����������Ե�����Ϣ
	metatable_PlayerMySelf.RegisterObjectFunctor("SendAskManualAttr", &SCRIPT_SANDBOX::PlayerMySelf::SendAskManualAttr);

	// �õ���ǰ�ƺ�
	metatable_PlayerMySelf.RegisterObjectFunctor("GetCurTitle", &SCRIPT_SANDBOX::PlayerMySelf::GetCurTitle);


	//
	// װ�����Խ���
	//---------------------------------------------------------------------------------------------------------------------------------------------------------


	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	// ���������������
	//

	// ͬ��������
	metatable_PlayerMySelf.RegisterObjectFunctor("AgreeJoinTeam", &SCRIPT_SANDBOX::PlayerMySelf::SendAgreeJoinTeam);
	// �ܾ��������
	metatable_PlayerMySelf.RegisterObjectFunctor("RejectJoinTeam", &SCRIPT_SANDBOX::PlayerMySelf::SendRejectJoinTeam);//


	// ͬ��������(��������������)
	metatable_PlayerMySelf.RegisterObjectFunctor("SendAgreeJoinTeam_Apply", &SCRIPT_SANDBOX::PlayerMySelf::SendAgreeJoinTeam_Apply);

	// �ܾ��������(��������������)
	metatable_PlayerMySelf.RegisterObjectFunctor("SendRejectJoinTeam_Apply", &SCRIPT_SANDBOX::PlayerMySelf::SendRejectJoinTeam_Apply);
	
	
	// ͬ��������(��Ա����ĳ�˼������)
	metatable_PlayerMySelf.RegisterObjectFunctor("SendAgreeJoinTeam_TeamMemberInvite", &SCRIPT_SANDBOX::PlayerMySelf::SendAgreeJoinTeam_TeamMemberInvite);
	// �ܾ��������(��Ա����ĳ�˼������)
	metatable_PlayerMySelf.RegisterObjectFunctor("SendRejectJoinTeam_TeamMemberInvite", &SCRIPT_SANDBOX::PlayerMySelf::SendRejectJoinTeam_TeamMemberInvite);
	

	// ͬ�������Ӹ���ģʽ
	metatable_PlayerMySelf.RegisterObjectFunctor("SendAgreeTeamFollow", &SCRIPT_SANDBOX::PlayerMySelf::SendAgreeTeamFollow);

	// �ܾ�������Ӹ���ģʽ
	metatable_PlayerMySelf.RegisterObjectFunctor("SendRefuseTeamFollow", &SCRIPT_SANDBOX::PlayerMySelf::SendRefuseTeamFollow);

	// �뿪����
	metatable_PlayerMySelf.RegisterObjectFunctor("LeaveTeam", &SCRIPT_SANDBOX::PlayerMySelf::LeaveTeam);//

	// ��ɢ����
	metatable_PlayerMySelf.RegisterObjectFunctor("DismissTeam", &SCRIPT_SANDBOX::PlayerMySelf::DismissTeam);//

	// �߳���ǰѡ�еĶ�Ա.
	metatable_PlayerMySelf.RegisterObjectFunctor("KickTeamMember", &SCRIPT_SANDBOX::PlayerMySelf::KickTeamMember);//

	// �Խ�����.
	metatable_PlayerMySelf.RegisterObjectFunctor("CreateTeamSelf", &SCRIPT_SANDBOX::PlayerMySelf::CreateTeamSelf);//

	// �����ӳ�.
	metatable_PlayerMySelf.RegisterObjectFunctor("AppointLeader", &SCRIPT_SANDBOX::PlayerMySelf::AppointLeader);//

	// �Ƿ�����Ӹ���״̬
	metatable_PlayerMySelf.RegisterObjectFunctor("InTeamFollowMode", &SCRIPT_SANDBOX::PlayerMySelf::InTeamFollowMode);//

	// ȡ������
	metatable_PlayerMySelf.RegisterObjectFunctor("StopFollow", &SCRIPT_SANDBOX::PlayerMySelf::StopFollow);//

	// ��Ӹ���
	metatable_PlayerMySelf.RegisterObjectFunctor("TeamFrame_AskTeamFollow", &SCRIPT_SANDBOX::PlayerMySelf::TeamFrame_AskTeamFollow);//

	// �õ�team member guid
	metatable_PlayerMySelf.RegisterObjectFunctor("GetTeamMemberGUID", &SCRIPT_SANDBOX::PlayerMySelf::GetTeamMemberGUID);//

	// ����
	metatable_PlayerMySelf.RegisterObjectFunctor("SendReliveMessage_OutGhost", &SCRIPT_SANDBOX::PlayerMySelf::SendReliveMessage_OutGhost);//

	// ����
	metatable_PlayerMySelf.RegisterObjectFunctor("SendReliveMessage_Relive", &SCRIPT_SANDBOX::PlayerMySelf::SendReliveMessage_Relive);//

	// ���ö�������
	metatable_PlayerMySelf.RegisterObjectFunctor("SetSupperPassword", &SCRIPT_SANDBOX::PlayerMySelf::SetSupperPassword);//
	

	//
	// ���������������
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------

	// �Ҽ��˵����
	metatable_PlayerMySelf.RegisterObjectFunctor("ShowMySelfContexMenu", &SCRIPT_SANDBOX::PlayerMySelf::ShowMySelfContexMenu);
	// ���ѡ���Լ�
	metatable_PlayerMySelf.RegisterObjectFunctor("SelectMyselfAsTarget", &SCRIPT_SANDBOX::PlayerMySelf::SelectMyselfAsTarget);

	LuaObject obj_PlayerMySelf = pState->BoxPointer(&(SCRIPT_SANDBOX::PlayerMySelf::s_MySelf));

	obj_PlayerMySelf.SetMetaTable(metatable_PlayerMySelf);
	objGlobal.SetObject("Player", obj_PlayerMySelf);

	//---------------------------------------------------------------------------------
	//ע�� ���ư�ť����
	CActionItem::s_pMetaTable = new LuaPlus::LuaObject;
	*CActionItem::s_pMetaTable = objGlobal.CreateTable("ActionButtonMetaTable");
	CActionItem::s_pMetaTable->SetObject("__index", *CActionItem::s_pMetaTable);

	CActionItem::s_pMetaTable->RegisterObjectFunctor("GetID", &CActionItem::Lua_GetID);
	CActionItem::s_pMetaTable->RegisterObjectFunctor("GetType", &CActionItem::Lua_GetType);
	CActionItem::s_pMetaTable->RegisterObjectFunctor("GetName", &CActionItem::Lua_GetName);
	CActionItem::s_pMetaTable->RegisterObjectFunctor("GetOwnerXinfa", &CActionItem::Lua_GetOwnerXinfa);
	CActionItem::s_pMetaTable->RegisterObjectFunctor("GetPetSkillOwner", &CActionItem::Lua_GetPetSkillOwner);
	CActionItem::s_pMetaTable->RegisterObjectFunctor("GetDefineID", &CActionItem::Lua_GetDefineID);
	CActionItem::s_pMetaTable->RegisterObjectFunctor("GetPrice", &CActionItem::Lua_GetPrice);
	CActionItem::s_pMetaTable->RegisterObjectFunctor("GetNum", &CActionItem::Lua_GetNum);
	CActionItem::s_pMetaTable->RegisterObjectFunctor("GetDesc", &CActionItem::Lua_GetDesc);

	//---------------------------------------------------------------------------------
	//ע�����ݳص�������
	SCRIPT_SANDBOX::DataPool::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::DataPool::s_pMetaTable = objGlobal.CreateTable("DataPoolMetaTable");
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::DataPool::s_pMetaTable);

	//������Ϣ
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetMissionInfo_Num", &SCRIPT_SANDBOX::DataPool::GetMissionInfo_Num);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetMissionInfo_Text", &SCRIPT_SANDBOX::DataPool::GetMissionInfo_Text);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetMissionInfo_Bonus", &SCRIPT_SANDBOX::DataPool::GetMissionInfo_Bonus);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetMissionInfo_Kind", &SCRIPT_SANDBOX::DataPool::GetMissionInfo_Kind);

	//����������Ϣ
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetMissionDemand_Num", &SCRIPT_SANDBOX::DataPool::GetMissionDemand_Num);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("CloseMissionFrame", &SCRIPT_SANDBOX::DataPool::CloseMissionFrame);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetMissionDemand_Text", &SCRIPT_SANDBOX::DataPool::GetMissionDemand_Text);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetMissionDemand_Item", &SCRIPT_SANDBOX::DataPool::GetMissionDemand_Item);
	//Continue
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetMissionContinue_Num", &SCRIPT_SANDBOX::DataPool::GetMissionContinue_Num);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetMissionContinue_Text", &SCRIPT_SANDBOX::DataPool::GetMissionContinue_Text);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetMissionContinue_Bonus", &SCRIPT_SANDBOX::DataPool::GetMissionContinue_Bonus);

	//��ҵ�ǰ��������Ϣ
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMission_Num", &SCRIPT_SANDBOX::DataPool:: GetPlayerMission_Num);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMission_InUse", &SCRIPT_SANDBOX::DataPool:: GetPlayerMission_InUse);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMission_Description", &SCRIPT_SANDBOX::DataPool::GetPlayerMission_Description);	
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMissionCustom_Num", &SCRIPT_SANDBOX::DataPool::GetPlayerMissionCustom_Num);	
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMissionCustom", &SCRIPT_SANDBOX::DataPool::GetPlayerMissionCustom);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMission_DataRound", &SCRIPT_SANDBOX::DataPool::GetPlayerMission_DataRound);	
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("Mission_Abnegate_Popup", &SCRIPT_SANDBOX::DataPool::Mission_Abnegate_Popup);	
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("EnumPlayerMission_ItemAction", &SCRIPT_SANDBOX::DataPool::EnumPlayerMission_ItemAction);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("Mission_Abnegate_Special_Quest_Popup", &SCRIPT_SANDBOX::DataPool::Mission_Abnegate_Special_Quest_Popup);

	
	
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMission_ItemCountNow", &SCRIPT_SANDBOX::DataPool::GetPlayerMission_ItemCountNow);

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMission_BillName", &SCRIPT_SANDBOX::DataPool::GetPlayerMission_BillName);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMission_Name", &SCRIPT_SANDBOX::DataPool::GetPlayerMission_Name);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMission_Info", &SCRIPT_SANDBOX::DataPool::GetPlayerMission_Info);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMission_Variable", &SCRIPT_SANDBOX::DataPool::GetPlayerMission_Variable);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMission_Display", &SCRIPT_SANDBOX::DataPool::GetPlayerMission_Display);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMission_RemainTime", &SCRIPT_SANDBOX::DataPool::GetPlayerMission_RemainTime);

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMissionDemand_Num", &SCRIPT_SANDBOX::DataPool:: GetPlayerMissionDemand_Num);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMissionDemand_Item", &SCRIPT_SANDBOX::DataPool:: GetPlayerMissionDemand_Item);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMissionDemandKill_Num", &SCRIPT_SANDBOX::DataPool:: GetPlayerMissionDemandKill_Num);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMissionDemand_NPC", &SCRIPT_SANDBOX::DataPool:: GetPlayerMissionDemand_NPC);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMissionBonus_Num", &SCRIPT_SANDBOX::DataPool::GetPlayerMissionBonus_Num);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMissionBonus_Item", &SCRIPT_SANDBOX::DataPool::GetPlayerMissionBonus_Item);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMission_Memo", &SCRIPT_SANDBOX::DataPool::GetPlayerMission_Memo);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMission_Level", &SCRIPT_SANDBOX::DataPool::GetPlayerMission_Level);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMission_Kind", &SCRIPT_SANDBOX::DataPool::GetPlayerMission_Kind);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerMission_IsElite", &SCRIPT_SANDBOX::DataPool::GetPlayerMission_IsElite);

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetNPCEventList_Num", &SCRIPT_SANDBOX::DataPool::GetNPCEventList_Num);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetNPCEventList_Item", &SCRIPT_SANDBOX::DataPool::GetNPCEventList_Item);
	//����������أ��й��䷽���֡�
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPrescrList_Num", &SCRIPT_SANDBOX::DataPool::GetPrescrList_Num);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPrescrList_Item", &SCRIPT_SANDBOX::DataPool::GetPrescrList_Item);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPrescrList_Item_LifeAbility", &SCRIPT_SANDBOX::DataPool::GetPrescrList_Item_LifeAbility);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPrescrList_Item_Requirement", &SCRIPT_SANDBOX::DataPool::GetPrescrList_Item_Requirement);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPrescrList_Item_Result", &SCRIPT_SANDBOX::DataPool::GetPrescrList_Item_Result);

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetBoothList_Num", &SCRIPT_SANDBOX::DataPool::GetBoothList_Num );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetBoothGood_ID", &SCRIPT_SANDBOX::DataPool::GetBoothGood_ID );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetBoothGood_Name", &SCRIPT_SANDBOX::DataPool::GetBoothGood_Name );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetBoothGood_Price", &SCRIPT_SANDBOX::DataPool::GetBoothGood_Price );
	

    //-----------------------------------------------------------------------------------------------------------------------------------------------------
	// ���ݳ�������
	//


	// �õ������ж�Ա����Ϣ
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetTeamMemberInfo",  &SCRIPT_SANDBOX::DataPool::GetTeamMemInfo);

	// �õ������ж�Ա��GUID
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetTeamMemGUIDByUIIndex",  &SCRIPT_SANDBOX::DataPool::GetTeamMemGUIDByUIIndex);


	// ͨ�������õ������ж�Ա����Ϣ
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetTeamMemInfoByIndex",  &SCRIPT_SANDBOX::DataPool::GetTeamMemInfoByIndex);


	// �õ������ж�Ա�ĸ���
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetTeamMemberCount", &SCRIPT_SANDBOX::DataPool::GetTeamMemCount);


	// �Ƿ��Ƕӳ�
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("IsTeamLeader", &SCRIPT_SANDBOX::DataPool::IsTeamLeader);


	// �õ��������ĸ���
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetInviteTeamCount", &SCRIPT_SANDBOX::DataPool::GetInviteTeamCount);


	// �õ�ĳ���������Ķ�Ա�ĸ���.
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetInviteTeamMemberCount", &SCRIPT_SANDBOX::DataPool::GetInviteTeamMemberCount);


	// �õ��������Ķ�Ա��Ϣ
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetInviteTeamMemberInfo", &SCRIPT_SANDBOX::DataPool::GetInviteTeamMemberInfo);


	// �ӳ��򿪶�����Ϣ
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("SetSelectTeamMember_Leader", &SCRIPT_SANDBOX::DataPool::SetSelectTeamMember_Leader);


	// �õ������˵ĸ���
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetApplyMemberCount", &SCRIPT_SANDBOX::DataPool::GetApplyMemberCount);


	// �õ���������Ϣ
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetApplyMemberInfo", &SCRIPT_SANDBOX::DataPool::GetApplyMemberInfo);

	// ����ѡ�е�������
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("SetCurSelApply", &SCRIPT_SANDBOX::DataPool::SetCurSelApply);

	// ����ѡ�е�������
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("EraseApply", &SCRIPT_SANDBOX::DataPool::EraseApply);

	// ���������
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("ClearAllApply", &SCRIPT_SANDBOX::DataPool::ClearAllApply);

	// ���ö��������
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("SetTeamFrameOpenFlag",     &SCRIPT_SANDBOX::DataPool::SetTeamFrameOpenFlag);

	// �õ��������ģ������
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetInviteTeamMemberUIModelName",     &SCRIPT_SANDBOX::DataPool::GetInviteTeamMemberUIModelName);

	// �õ������Աģ������
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetTeamMemUIModelName",     &SCRIPT_SANDBOX::DataPool::GetTeamMemUIModelName);

	// �õ�������ģ������
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetApplyMemberUIModelName",     &SCRIPT_SANDBOX::DataPool::GetApplyMemberUIModelName);

	// �������е��Լ���Ϣ
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("SetSelfInfo",     &SCRIPT_SANDBOX::DataPool::SetSelfInfo);

	// ��Ա�Ƿ��ڳ�����
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("IsTeamMemberInScene",     &SCRIPT_SANDBOX::DataPool::IsTeamMemberInScene);

	// ͨ�����ѽ���ѡ�����, ��Ϊtarget
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("SelectAsTargetByUIIndex",     &SCRIPT_SANDBOX::DataPool::SelectAsTargetByUIIndex);



	//
	// ���ݳ����������
	//------------------------------------------------------------------------------------------------------------------------------------------------------



	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPacketItem", &SCRIPT_SANDBOX::DataPool::GetPacketItem );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPacketItemMaxNumber", &SCRIPT_SANDBOX::DataPool::GetPacketItemMaxNumber );

	// �����б����
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetFriendNumber", &SCRIPT_SANDBOX::DataPool::GetFriendNumber );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetFriend", &SCRIPT_SANDBOX::DataPool::GetFriend );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("AddFriend", &SCRIPT_SANDBOX::DataPool::AddFriend );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("DelFriend", &SCRIPT_SANDBOX::DataPool::DelFriend );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("ShowFriendInfo", &SCRIPT_SANDBOX::DataPool::ShowFriendInfo );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("ShowChatInfo", &SCRIPT_SANDBOX::DataPool::ShowChatInfo);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("SetMood", &SCRIPT_SANDBOX::DataPool::SetMood );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetMood", &SCRIPT_SANDBOX::DataPool::GetMood );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("EditMood", &SCRIPT_SANDBOX::DataPool::EditMood );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetFriendByName", &SCRIPT_SANDBOX::DataPool::GetFriendByName );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("OpenHistroy", &SCRIPT_SANDBOX::DataPool::OpenHistroy );
	

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetMailNumber", &SCRIPT_SANDBOX::DataPool::GetMailNumber );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetMail", &SCRIPT_SANDBOX::DataPool::GetMail );
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("OpenMail", &SCRIPT_SANDBOX::DataPool::OpenMail);
	
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("SendMail", &SCRIPT_SANDBOX::DataPool::SendMail);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetNextMail", &SCRIPT_SANDBOX::DataPool::GetNextMail);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("OpenFriendList", &SCRIPT_SANDBOX::DataPool::OpenFriendList);

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("ThrowToBlackList", &SCRIPT_SANDBOX::DataPool::ThrowToBlackList);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("ThrowToList", &SCRIPT_SANDBOX::DataPool::ThrowToList);

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("OpenHistroy", &SCRIPT_SANDBOX::DataPool::OpenHistroy);

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("Change_MyHairColor", &SCRIPT_SANDBOX::DataPool::Change_MyHairColor);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("Change_MyHairStyle", &SCRIPT_SANDBOX::DataPool::Change_MyHairStyle);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("Change_MyHairStyle_Item", &SCRIPT_SANDBOX::DataPool::Change_MyHairStyle_Item);


	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("Get_MyHairColor", &SCRIPT_SANDBOX::DataPool::Get_MyHairColor);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("Get_MyHairStyle", &SCRIPT_SANDBOX::DataPool::Get_MyHairStyle);
	
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("Change_RectifyColor", &SCRIPT_SANDBOX::DataPool::Change_RectifyColor);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("Change_GetColorLumination", &SCRIPT_SANDBOX::DataPool::Change_GetColorLumination);

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("IsCanDoAction", &SCRIPT_SANDBOX::DataPool::IsCanDoAction);

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("Check_StringCode", &SCRIPT_SANDBOX::DataPool::Check_StringCode);



	//------------------------------------------------------------------------------------------------------------------------------------------------------
	//
	// target ѡ�п��ǲ��ڵ�ǰ�Ķ���ʱ
	//

	// ��ǰ��target Team���� 2006��4��12
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("TargetFrame_Update_Name_Team", &SCRIPT_SANDBOX::DataPool::TargetFrame_Update_Name_Team);

	// ��ǰ��target Team hp 2006��4��12
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("TargetFrame_Update_HP_Team", &SCRIPT_SANDBOX::DataPool::TargetFrame_Update_HP_Team);

	// ��ǰ��target Team mp 2006��4��12
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("TargetFrame_Update_MP_Team", &SCRIPT_SANDBOX::DataPool::TargetFrame_Update_MP_Team);
	
	// ��ǰ��target Team iRange 2006��4��12
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("TargetFrame_Update_Rage_Team", &SCRIPT_SANDBOX::DataPool::TargetFrame_Update_Rage_Team);

	// ��ǰ��target Team iLevel 2006��4��12
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("TargetFrame_Update_Level_Team", &SCRIPT_SANDBOX::DataPool::TargetFrame_Update_Level_Team);

	// ��ǰ��target Team ͷ�� 2006��4��12
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("TargetFrame_Update_Icon_Team", &SCRIPT_SANDBOX::DataPool::TargetFrame_Update_Icon_Team);

//	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("NetClose", &SCRIPT_SANDBOX::DataPool::NetClose);

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("ToggleShowPetList", &SCRIPT_SANDBOX::DataPool::ToggleShowPetList);

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetNPCIDByServerID", &SCRIPT_SANDBOX::DataPool::GetNPCIDByServerID);

	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetUIColor", &SCRIPT_SANDBOX::DataPool::GetUIColor);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerSkill_ID", &SCRIPT_SANDBOX::DataPool::GetPlayerSkill_ID);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerSkill_Level", &SCRIPT_SANDBOX::DataPool::GetPlayerSkill_Level);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerSkill_Type", &SCRIPT_SANDBOX::DataPool::GetPlayerSkill_Type);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetPlayerSkill_Num", &SCRIPT_SANDBOX::DataPool::GetPlayerSkill_Num);
	SCRIPT_SANDBOX::DataPool::s_pMetaTable->RegisterObjectFunctor("GetNPCobj_ID", &SCRIPT_SANDBOX::DataPool::GetNPCobj_ID);
	
	LuaObject obj_DataPool = pState->BoxPointer(&(SCRIPT_SANDBOX::DataPool::s_DataPool));


	obj_DataPool.SetMetaTable(*SCRIPT_SANDBOX::DataPool::s_pMetaTable);
	objGlobal.SetObject("DataPool", obj_DataPool);


	//---------------------------------------------------------------------------------
	//ע���������ʦ��������
	SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable = objGlobal.CreateTable("AbilityTeacherTable");
	SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable);

	SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable->RegisterObjectFunctor("IsPresent", &SCRIPT_SANDBOX::AbilityTeacher::IsPresent);
	SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable->RegisterObjectFunctor("GetAbilityID", &SCRIPT_SANDBOX::AbilityTeacher::GetAbilityID);
	SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable->RegisterObjectFunctor("GetNeedExp", &SCRIPT_SANDBOX::AbilityTeacher::GetNeedExp);
	SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable->RegisterObjectFunctor("GetNeedMoney", &SCRIPT_SANDBOX::AbilityTeacher::GetNeedMoney);
	SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable->RegisterObjectFunctor("GetNeedSkillExp", &SCRIPT_SANDBOX::AbilityTeacher::GetNeedSkillExp);
	SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable->RegisterObjectFunctor("GetServerData", &SCRIPT_SANDBOX::AbilityTeacher::GetServerData);

	LuaObject obj_Ability = pState->BoxPointer(&(SCRIPT_SANDBOX::AbilityTeacher::s_AbilityTeacher));
	obj_Ability.SetMetaTable(*SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable);
	objGlobal.SetObject("AbilityTeacher", obj_Ability);

	//---------------------------------------------------------------------------------
	//ע����ﵼ������
	SCRIPT_SANDBOX::Pet::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::Pet::s_pMetaTable = objGlobal.CreateTable("PetTable");
	SCRIPT_SANDBOX::Pet::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::Pet::s_pMetaTable);
	
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("IsPresent", &SCRIPT_SANDBOX::Pet::IsPresent);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetPet_Count", &SCRIPT_SANDBOX::Pet::GetPet_Count);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetPetList_Appoint", &SCRIPT_SANDBOX::Pet::GetPetList_Appoint);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("Go_Fight", &SCRIPT_SANDBOX::Pet::Go_Fight);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("Go_Relax", &SCRIPT_SANDBOX::Pet::Go_Relax);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("Go_Free", &SCRIPT_SANDBOX::Pet::Go_Free);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("Feed", &SCRIPT_SANDBOX::Pet::Feed);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("Dome", &SCRIPT_SANDBOX::Pet::Dome);

	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetName", &SCRIPT_SANDBOX::Pet::GetName);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetID", &SCRIPT_SANDBOX::Pet::GetID);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetNaturalLife", &SCRIPT_SANDBOX::Pet::GetNaturalLife);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetMaxLife", &SCRIPT_SANDBOX::Pet::GetMaxLife);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetHappy", &SCRIPT_SANDBOX::Pet::GetHappy);

	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetConsort", &SCRIPT_SANDBOX::Pet::GetConsort);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetSex", &SCRIPT_SANDBOX::Pet::GetSex);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetBasic", &SCRIPT_SANDBOX::Pet::GetBasic);

	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetLoyalgGade", &SCRIPT_SANDBOX::Pet::GetLoyalgGade);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetLevel", &SCRIPT_SANDBOX::Pet::GetLevel);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetType", &SCRIPT_SANDBOX::Pet::GetType);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetHP", &SCRIPT_SANDBOX::Pet::GetHP);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetMaxHP", &SCRIPT_SANDBOX::Pet::GetMaxHP);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetMP", &SCRIPT_SANDBOX::Pet::GetMP);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetMaxMP", &SCRIPT_SANDBOX::Pet::GetMaxMP);

	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetStrAptitude", &SCRIPT_SANDBOX::Pet::GetStrAptitude);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetPFAptitude", &SCRIPT_SANDBOX::Pet::GetPFAptitude);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetDexAptitude", &SCRIPT_SANDBOX::Pet::GetDexAptitude);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetIntAptitude", &SCRIPT_SANDBOX::Pet::GetIntAptitude);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetStaAptitude", &SCRIPT_SANDBOX::Pet::GetStaAptitude);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetStr", &SCRIPT_SANDBOX::Pet::GetStr);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetInt", &SCRIPT_SANDBOX::Pet::GetInt);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetDex", &SCRIPT_SANDBOX::Pet::GetDex);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetPF", &SCRIPT_SANDBOX::Pet::GetPF);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetSta", &SCRIPT_SANDBOX::Pet::GetSta);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetPotential", &SCRIPT_SANDBOX::Pet::GetPotential);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetExp", &SCRIPT_SANDBOX::Pet::GetExp);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetPhysicsAttack", &SCRIPT_SANDBOX::Pet::GetPhysicsAttack);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetMagicAttack", &SCRIPT_SANDBOX::Pet::GetMagicAttack);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetPhysicsRecovery", &SCRIPT_SANDBOX::Pet::GetPhysicsRecovery);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetMagicRecovery", &SCRIPT_SANDBOX::Pet::GetMagicRecovery);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetMiss", &SCRIPT_SANDBOX::Pet::GetMiss);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetShootProbability", &SCRIPT_SANDBOX::Pet::GetShootProbability);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("Add_Attribute", &SCRIPT_SANDBOX::Pet::Add_Attribute);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("Change_Name", &SCRIPT_SANDBOX::Pet::Change_Name);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetIsFighting", &SCRIPT_SANDBOX::Pet::GetIsFighting);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("SetModel", &SCRIPT_SANDBOX::Pet::SetModel);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetDeathWarrant", &SCRIPT_SANDBOX::Pet::GetDeathWarrant);
	//add for petskillstudy
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("SetSkillStudyModel", &SCRIPT_SANDBOX::Pet::SetSkillStudyModel);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("ShowTargetPet", &SCRIPT_SANDBOX::Pet::ShowTargetPet);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("SkillStudy_Do", &SCRIPT_SANDBOX::Pet::SkillStudy_Do);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("SkillStudyUnlock", &SCRIPT_SANDBOX::Pet::SkillStudyUnlock);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("SkillStudy_MenPaiSkill_Created", &SCRIPT_SANDBOX::Pet::SkillStudy_MenPaiSkill_Created);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("SkillStudy_MenPaiSkill_Clear", &SCRIPT_SANDBOX::Pet::SkillStudy_MenPaiSkill_Clear);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetGUID", &SCRIPT_SANDBOX::Pet::GetGUID);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("ShowPetList", &SCRIPT_SANDBOX::Pet::ShowPetList);
	//add for contexmenu
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("ShowMyPetContexMenu", &SCRIPT_SANDBOX::Pet::ShowMyPetContexMenu);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("HandlePetMenuItem", &SCRIPT_SANDBOX::Pet::HandlePetMenuItem);
	//add for impact
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetPetPortraitByIndex", &SCRIPT_SANDBOX::Pet::GetPetPortraitByIndex);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetPetImpactNum", &SCRIPT_SANDBOX::Pet::GetPetImpactNum);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetPetImpactIconNameByIndex", &SCRIPT_SANDBOX::Pet::GetPetImpactIconNameByIndex);

	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("EnumPetSkill", &SCRIPT_SANDBOX::Pet::Lua_EnumPetSkill);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("Select_Pet", &SCRIPT_SANDBOX::Pet::Select_Pet);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetAIType", &SCRIPT_SANDBOX::Pet::GetAIType);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("GetSkillPassive", &SCRIPT_SANDBOX::Pet::GetSkillPassive);
	SCRIPT_SANDBOX::Pet::s_pMetaTable->RegisterObjectFunctor("Free_Confirm", &SCRIPT_SANDBOX::Pet::Free_Confirm);

	
	
	
	LuaObject obj_Pet = pState->BoxPointer(&(SCRIPT_SANDBOX::Pet::s_Pet));
	obj_Pet.SetMetaTable(*SCRIPT_SANDBOX::Pet::s_pMetaTable);
	objGlobal.SetObject("Pet", obj_Pet);

	//---------------------------------------------------------------------------------
	//ע��������Ѱ嵼������
	SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable = objGlobal.CreateTable("PetInviteFriendTable");
	SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable);

	SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable->RegisterObjectFunctor("GetInviteNum", &SCRIPT_SANDBOX::PetInviteFriend::GetInviteNum);
	SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable->RegisterObjectFunctor("GetHumanINFO", &SCRIPT_SANDBOX::PetInviteFriend::GetHumanINFO);
	SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable->RegisterObjectFunctor("GetInviteMsg", &SCRIPT_SANDBOX::PetInviteFriend::GetInviteMsg);
	SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable->RegisterObjectFunctor("SetPetModel", &SCRIPT_SANDBOX::PetInviteFriend::SetPetModel);
	SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable->RegisterObjectFunctor("ShowTargetPet", &SCRIPT_SANDBOX::PetInviteFriend::ShowTargetPet);
	SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable->RegisterObjectFunctor("GetPetINFO", &SCRIPT_SANDBOX::PetInviteFriend::GetPetINFO);

	LuaObject obj_PetInviteFriend = pState->BoxPointer(&(SCRIPT_SANDBOX::PetInviteFriend::s_PetInviteFriend));
	obj_PetInviteFriend.SetMetaTable(*SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable);
	objGlobal.SetObject("PetInviteFriend", obj_PetInviteFriend);

	//---------------------------------------------------------------------------------
	//ע��Է����ﵼ������
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::TargetPet::s_pMetaTable = objGlobal.CreateTable("TargetPetTable");
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::TargetPet::s_pMetaTable);
	
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("IsPresent", &SCRIPT_SANDBOX::TargetPet::IsPresent);
//	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetPet_Count", &SCRIPT_SANDBOX::TargetPet::GetPet_Count);
//	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetPetList_Appoint", &SCRIPT_SANDBOX::TargetPet::GetPetList_Appoint);
//	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("Go_Fight", &SCRIPT_SANDBOX::TargetPet::Go_Fight);
//	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("Go_Relax", &SCRIPT_SANDBOX::TargetPet::Go_Relax);

	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetName", &SCRIPT_SANDBOX::TargetPet::GetName);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetID", &SCRIPT_SANDBOX::TargetPet::GetID);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetNaturalLife", &SCRIPT_SANDBOX::TargetPet::GetNaturalLife);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetMaxLife", &SCRIPT_SANDBOX::TargetPet::GetMaxLife);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetHappy", &SCRIPT_SANDBOX::TargetPet::GetHappy);

	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetConsort", &SCRIPT_SANDBOX::TargetPet::GetConsort);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetSex", &SCRIPT_SANDBOX::TargetPet::GetSex);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetBasic", &SCRIPT_SANDBOX::TargetPet::GetBasic);

	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetLoyalgGade", &SCRIPT_SANDBOX::TargetPet::GetLoyalgGade);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetLevel", &SCRIPT_SANDBOX::TargetPet::GetLevel);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetType", &SCRIPT_SANDBOX::TargetPet::GetType);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetHP", &SCRIPT_SANDBOX::TargetPet::GetHP);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetMaxHP", &SCRIPT_SANDBOX::TargetPet::GetMaxHP);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetMP", &SCRIPT_SANDBOX::TargetPet::GetMP);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetMaxMP", &SCRIPT_SANDBOX::TargetPet::GetMaxMP);

	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetStrAptitude", &SCRIPT_SANDBOX::TargetPet::GetStrAptitude);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetPFAptitude", &SCRIPT_SANDBOX::TargetPet::GetPFAptitude);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetDexAptitude", &SCRIPT_SANDBOX::TargetPet::GetDexAptitude);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetIntAptitude", &SCRIPT_SANDBOX::TargetPet::GetIntAptitude);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetStaAptitude", &SCRIPT_SANDBOX::TargetPet::GetStaAptitude);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetStr", &SCRIPT_SANDBOX::TargetPet::GetStr);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetInt", &SCRIPT_SANDBOX::TargetPet::GetInt);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetDex", &SCRIPT_SANDBOX::TargetPet::GetDex);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetPF", &SCRIPT_SANDBOX::TargetPet::GetPF);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetSta", &SCRIPT_SANDBOX::TargetPet::GetSta);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetPotential", &SCRIPT_SANDBOX::TargetPet::GetPotential);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetExp", &SCRIPT_SANDBOX::TargetPet::GetExp);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetPhysicsAttack", &SCRIPT_SANDBOX::TargetPet::GetPhysicsAttack);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetMagicAttack", &SCRIPT_SANDBOX::TargetPet::GetMagicAttack);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetPhysicsRecovery", &SCRIPT_SANDBOX::TargetPet::GetPhysicsRecovery);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetMagicRecovery", &SCRIPT_SANDBOX::TargetPet::GetMagicRecovery);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetMiss", &SCRIPT_SANDBOX::TargetPet::GetMiss);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetShootProbability", &SCRIPT_SANDBOX::TargetPet::GetShootProbability);
//	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("Add_Attribute", &SCRIPT_SANDBOX::TargetPet::Add_Attribute);
//	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("Change_Name", &SCRIPT_SANDBOX::TargetPet::Change_Name);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetIsFighting", &SCRIPT_SANDBOX::TargetPet::GetIsFighting);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("SetModel", &SCRIPT_SANDBOX::TargetPet::SetModel);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("CopyMyPet", &SCRIPT_SANDBOX::TargetPet::CopyMyPet);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetDeathWarrant", &SCRIPT_SANDBOX::TargetPet::GetDeathWarrant);
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable->RegisterObjectFunctor("GetAIType", &SCRIPT_SANDBOX::TargetPet::GetAIType);



	LuaObject obj_TargetPet = pState->BoxPointer(&(SCRIPT_SANDBOX::TargetPet::s_TargetPet));
	obj_TargetPet.SetMetaTable(*SCRIPT_SANDBOX::TargetPet::s_pMetaTable);
	objGlobal.SetObject("TargetPet", obj_TargetPet);

	//---------------------------------------------------------------------------------
	//ע������Լ�������
	SCRIPT_SANDBOX::Bank::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::Bank::s_pMetaTable = objGlobal.CreateTable("BankTable");
	SCRIPT_SANDBOX::Bank::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::Bank::s_pMetaTable);

	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("IsPresent", &SCRIPT_SANDBOX::Bank::IsPresent);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("OpenSaveFrame", &SCRIPT_SANDBOX::Bank::OpenSaveFrame);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("OpenGetFrame", &SCRIPT_SANDBOX::Bank::OpenGetFrame);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("GetInputMoney", &SCRIPT_SANDBOX::Bank::GetInputMoney);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("SaveMoneyToBank", &SCRIPT_SANDBOX::Bank::SaveMoneyToBank);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("GetMoneyFromBank", &SCRIPT_SANDBOX::Bank::GetMoneyFromBank);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("SaveRMBToBank", &SCRIPT_SANDBOX::Bank::SaveRMBToBank);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("GetRMBFromBank", &SCRIPT_SANDBOX::Bank::GetRMBFromBank);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("GetBankMoney", &SCRIPT_SANDBOX::Bank::GetBankMoney);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("GetBankRMB", &SCRIPT_SANDBOX::Bank::GetBankRMB);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("MoveItemToPacket", &SCRIPT_SANDBOX::Bank::MoveItemToPacket);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("GetRentBoxNum", &SCRIPT_SANDBOX::Bank::GetRentBoxNum);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("GetRentBoxInfo", &SCRIPT_SANDBOX::Bank::GetRentBoxInfo);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("SetCurRentIndex", &SCRIPT_SANDBOX::Bank::SetCurRentIndex);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("TransformCoin", &SCRIPT_SANDBOX::Bank::TransformCoin);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("Close", &SCRIPT_SANDBOX::Bank::Close);
	SCRIPT_SANDBOX::Bank::s_pMetaTable->RegisterObjectFunctor("EnumItem", &SCRIPT_SANDBOX::Bank::EnumItem);

	LuaObject obj_Bank = pState->BoxPointer(&(SCRIPT_SANDBOX::Bank::s_Bank));
	obj_Bank.SetMetaTable(*SCRIPT_SANDBOX::Bank::s_pMetaTable);
	objGlobal.SetObject("Bank", obj_Bank);


	//---------------------------------------------------------------------------------
	//ע�����֮��Ľ���
	SCRIPT_SANDBOX::Exchange::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::Exchange::s_pMetaTable = objGlobal.CreateTable("ExchangeTable");
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::Exchange::s_pMetaTable);

	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("IsPresent", &SCRIPT_SANDBOX::Exchange::IsPresent);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("SendExchangeApply", &SCRIPT_SANDBOX::Exchange::SendExchangeApply);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("OpenExchangeFrame", &SCRIPT_SANDBOX::Exchange::OpenExchangeFrame);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("GetItemNum", &SCRIPT_SANDBOX::Exchange::GetItemNum);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("GetItemAction", &SCRIPT_SANDBOX::Exchange::GetItemAction);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("GetMoney", &SCRIPT_SANDBOX::Exchange::GetMoney);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("LockExchange", &SCRIPT_SANDBOX::Exchange::LockExchange);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("IsLocked", &SCRIPT_SANDBOX::Exchange::IsLocked);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("AcceptExchange", &SCRIPT_SANDBOX::Exchange::AcceptExchange);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("ExchangeCancel", &SCRIPT_SANDBOX::Exchange::ExchangeCancel);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("OpenPetFrame", &SCRIPT_SANDBOX::Exchange::OpenPetFrame);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("GetMoneyFromInput", &SCRIPT_SANDBOX::Exchange::GetMoneyFromInput);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("IsStillAnyAppInList", &SCRIPT_SANDBOX::Exchange::IsStillAnyAppInList);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("GetOthersName", &SCRIPT_SANDBOX::Exchange::GetOthersName);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("AddPet", &SCRIPT_SANDBOX::Exchange::AddPet);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("GetPetNum", &SCRIPT_SANDBOX::Exchange::GetPetNum);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("EnumPet", &SCRIPT_SANDBOX::Exchange::EnumPet);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("DelSelectPet", &SCRIPT_SANDBOX::Exchange::DelSelectPet);
	SCRIPT_SANDBOX::Exchange::s_pMetaTable->RegisterObjectFunctor("ViewPetDesc", &SCRIPT_SANDBOX::Exchange::ViewPetDesc);

	LuaObject obj_Exchange = pState->BoxPointer(&(SCRIPT_SANDBOX::Exchange::s_Exchange));
	obj_Exchange.SetMetaTable(*SCRIPT_SANDBOX::Exchange::s_pMetaTable);
	objGlobal.SetObject("Exchange", obj_Exchange);

	//---------------------------------------------------------------------------------
	// ע�������
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::LifeAbility::s_pMetaTable = objGlobal.CreateTable("LifeAbilityTable");
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::LifeAbility::s_pMetaTable);

//	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetEquip_GemBore_Count", &SCRIPT_SANDBOX::LifeAbility::GetEquip_GemBore_Count);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetLifeAbility_Number", &SCRIPT_SANDBOX::LifeAbility::GetLifeAbility_Number);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetPrescr_Material_Number", &SCRIPT_SANDBOX::LifeAbility::GetPrescr_Material_Number);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetPrescr_Material", &SCRIPT_SANDBOX::LifeAbility::GetPrescr_Material);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetPrescrList_Item_FromNum", &SCRIPT_SANDBOX::LifeAbility::GetPrescrList_Item_FromNum);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetPrescr_Item_Maximum", &SCRIPT_SANDBOX::LifeAbility::GetPrescr_Item_Maximum);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetPrescr_Material_Hold_Count", &SCRIPT_SANDBOX::LifeAbility::GetPrescr_Material_Hold_Count);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("Update_Synthesize", &SCRIPT_SANDBOX::LifeAbility::Update_Synthesize);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("Open_Compose_Gem_Page", &SCRIPT_SANDBOX::LifeAbility::Open_Compose_Gem_Page);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("Do_Enchase", &SCRIPT_SANDBOX::LifeAbility::Do_Enchase);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("Do_Combine", &SCRIPT_SANDBOX::LifeAbility::Do_Combine);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetEquip_Gem", &SCRIPT_SANDBOX::LifeAbility::GetEquip_Gem);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetEquip_GemCount", &SCRIPT_SANDBOX::LifeAbility::GetEquip_GemCount);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("Enchase_Preparation", &SCRIPT_SANDBOX::LifeAbility::Enchase_Preparation);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("Compound_Preparation", &SCRIPT_SANDBOX::LifeAbility::Compound_Preparation);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("Can_Enchase", &SCRIPT_SANDBOX::LifeAbility::Can_Enchase);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("Can_Combine", &SCRIPT_SANDBOX::LifeAbility::Can_Combine);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("Lock_Packet_Item", &SCRIPT_SANDBOX::LifeAbility::Lock_Packet_Item);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetPrescr_Explain", &SCRIPT_SANDBOX::LifeAbility::GetPrescr_Explain);
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable->RegisterObjectFunctor("GetNpcId", &SCRIPT_SANDBOX::LifeAbility::GetNpcId);

	
	LuaObject obj_LifeAbility = pState->BoxPointer(&(SCRIPT_SANDBOX::LifeAbility::s_LifeAbility));
	obj_LifeAbility.SetMetaTable(*SCRIPT_SANDBOX::LifeAbility::s_pMetaTable);
	objGlobal.SetObject("LifeAbility", obj_LifeAbility);

	//---------------------------------------------------------------------------------
	//������
	SCRIPT_SANDBOX::Guild::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::Guild::s_pMetaTable = objGlobal.CreateTable("GuildTable");
	SCRIPT_SANDBOX::Guild::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::Guild::s_pMetaTable);

	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("CreateGuild", &SCRIPT_SANDBOX::Guild::CreateGuild);
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("CreateGuildConfirm", &SCRIPT_SANDBOX::Guild::CreateGuildConfirm);
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("AskGuildDetailInfo", &SCRIPT_SANDBOX::Guild::AskGuildDetailInfo);
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("AskGuildMembersInfo", &SCRIPT_SANDBOX::Guild::AskGuildMembersInfo);
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("AskGuildAppointPosInfo", &SCRIPT_SANDBOX::Guild::AskGuildAppointPosInfo);
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GetGuildNum", &SCRIPT_SANDBOX::Guild::GetGuildNum);
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GetGuildInfo", &SCRIPT_SANDBOX::Guild::GetGuildInfo);
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("JoinGuild", &SCRIPT_SANDBOX::Guild::JoinGuild);
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GetMembersNum", &SCRIPT_SANDBOX::Guild::GetMembersNum);
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GetMembersInfo", &SCRIPT_SANDBOX::Guild::GetMembersInfo);
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GetMyGuildInfo", &SCRIPT_SANDBOX::Guild::GetMyGuildInfo);
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GetMyGuildDetailInfo", &SCRIPT_SANDBOX::Guild::GetMyGuildDetailInfo);
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GetMyGuildPower", &SCRIPT_SANDBOX::Guild::GetMyGuildPower);
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("KickGuild", &SCRIPT_SANDBOX::Guild::KickGuild);
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("RecruitGuild", &SCRIPT_SANDBOX::Guild::RecruitGuild);
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("QuitGuild", &SCRIPT_SANDBOX::Guild::QuitGuild);
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("AdjustMemberAuth", &SCRIPT_SANDBOX::Guild::AdjustMemberAuth);
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("DemisGuild", &SCRIPT_SANDBOX::Guild::DemisGuild);
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("DestoryGuild", &SCRIPT_SANDBOX::Guild::DestoryGuild);

	// [4/16/2006]
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("PrepareMembersInfomation", &SCRIPT_SANDBOX::Guild::PrepareMembersInfomation);
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GetShowMembersIdx", &SCRIPT_SANDBOX::Guild::GetShowMembersIdx);
	SCRIPT_SANDBOX::Guild::s_pMetaTable->RegisterObjectFunctor("GetShowTraineesIdx", &SCRIPT_SANDBOX::Guild::GetShowTraineesIdx);

	LuaObject obj_Guild = pState->BoxPointer(&(SCRIPT_SANDBOX::Guild::s_Guild));
	obj_Guild.SetMetaTable(*SCRIPT_SANDBOX::Guild::s_pMetaTable);
	objGlobal.SetObject("Guild", obj_Guild);


	//---------------------------------------------------------------------------------
	//��Ұ���
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable = objGlobal.CreateTable("PlayerPackage");
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable);

	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("EnumItem", &SCRIPT_SANDBOX::PlayerPackage::EnumItem);
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("OpenStallSaleFrame", &SCRIPT_SANDBOX::PlayerPackage::OpenStallSaleFrame);
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("GetSplitSum", &SCRIPT_SANDBOX::PlayerPackage::GetSplitSum);
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("SplitItem", &SCRIPT_SANDBOX::PlayerPackage::SplitItem);
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("CancelSplitItem", &SCRIPT_SANDBOX::PlayerPackage::CancelSplitItem);
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("PackUpPacket", &SCRIPT_SANDBOX::PlayerPackage::PackUpPacket);
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("GetSplitName", &SCRIPT_SANDBOX::PlayerPackage::GetSplitName);
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("OpenPetList", &SCRIPT_SANDBOX::PlayerPackage::OpenPetList);
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("OpenLockFrame", &SCRIPT_SANDBOX::PlayerPackage::OpenLockFrame);
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("Lock", &SCRIPT_SANDBOX::PlayerPackage::Lock);
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable->RegisterObjectFunctor("IsLock", &SCRIPT_SANDBOX::PlayerPackage::IsLock);

	LuaObject obj_PlayerPackage = pState->BoxPointer(&(SCRIPT_SANDBOX::PlayerPackage::s_PlayerPackage));
	obj_PlayerPackage.SetMetaTable(*SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable);
	objGlobal.SetObject("PlayerPackage", obj_PlayerPackage);

	//---------------------------------------------------------------------------------
	//Tooltip
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable = objGlobal.CreateTable("SuperTooltips");
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable);

	// �õ���������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetTitle",	   &SCRIPT_SANDBOX::SuperTooltips::GetTitle);

	// �õ�����ͼ��
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetIcon",	   &SCRIPT_SANDBOX::SuperTooltips::GetIcon);

	// �õ���������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemDesc",  &SCRIPT_SANDBOX::SuperTooltips::GetItemDesc);
    //�õ����嶨�㴫����ز���;
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemTransferParam",  &SCRIPT_SANDBOX::SuperTooltips::GetItemTransferParam);
    //�õ���������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemIndex",  &SCRIPT_SANDBOX::SuperTooltips::GetItemIndex);
	// �õ�ʹ����Ʒ��Ҫ�ĵȼ�
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemLevel", &SCRIPT_SANDBOX::SuperTooltips::GetItemLevel);

	// �õ���Ʒ�;�
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemDur", &SCRIPT_SANDBOX::SuperTooltips::GetItemDur);

	// �õ���Ʒ���������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemRepairCount", &SCRIPT_SANDBOX::SuperTooltips::GetItemRepairCount);

	// �õ���Ʒ�İ���Ϣ
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemBindInfo", &SCRIPT_SANDBOX::SuperTooltips::GetItemBindInfo);

	// �õ���Ʒ�ļ۸�
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemPrice", &SCRIPT_SANDBOX::SuperTooltips::GetItemPrice);

	// �õ���Ʒ��֪����
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemManufacturer", &SCRIPT_SANDBOX::SuperTooltips::GetItemManufacturer);

	// �õ���Ʒ�İ�ɫ��������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemBaseWhiteAttrInfo", &SCRIPT_SANDBOX::SuperTooltips::GetItemBaseWhiteAttrInfo);
		
	// �õ���Ʒ����ɫ��������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemExtBlueAttrInfo", &SCRIPT_SANDBOX::SuperTooltips::GetItemExtBlueAttrInfo);

	// �õ���Ʒ����ɫɫ��������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemGreenAttrInfo", &SCRIPT_SANDBOX::SuperTooltips::GetItemGreenAttrInfo);
	

	// �õ���Ʒ������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemType", &SCRIPT_SANDBOX::SuperTooltips::GetItemType);

	// �õ�װ����Ʒ��(��, ��, ��)
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemEquipQuantity", &SCRIPT_SANDBOX::SuperTooltips::GetItemEquipQuantity);

	// �õ���Ʒ����������(��װ��������������Ʒ)
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemClassType", &SCRIPT_SANDBOX::SuperTooltips::GetItemClassType);

	// �õ�װ���ϵı�ʯ��Ϣ
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetGemInEquipInfo", &SCRIPT_SANDBOX::SuperTooltips::GetGemInEquipInfo);

	// �õ��õ��ķ��ȼ�
	//SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetXinfaLevel", &SCRIPT_SANDBOX::SuperTooltips::GetXinfaLevel);

	// �Ƿ�ѧ�Ἴ��
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetIsLearnedSkill", &SCRIPT_SANDBOX::SuperTooltips::GetIsLearnedSkill);


	// �õ���������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetSkillLevel", &SCRIPT_SANDBOX::SuperTooltips::GetSkillLevel);
	

	// �õ���������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetSkillDesc", &SCRIPT_SANDBOX::SuperTooltips::GetSkillDesc);

	// �õ���ʯ�ȼ�
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetGemLevel", &SCRIPT_SANDBOX::SuperTooltips::GetGemLevel);

	// �õ���ʯ����
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetGemArribInfo", &SCRIPT_SANDBOX::SuperTooltips::GetGemArribInfo);

	// �õ�ҩƷ��ʯ����
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemQuality", &SCRIPT_SANDBOX::SuperTooltips::GetItemQuality);

	// �õ�װ��������������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetEquipMaxRepair", &SCRIPT_SANDBOX::SuperTooltips::GetEquipMaxRepair);

	// �õ������ķ�
	//SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetSkillXinfaLevel", &SCRIPT_SANDBOX::SuperTooltips::GetSkillXinfaLevel);

	// �õ�����Ʒ���е����͡�
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemTableType", &SCRIPT_SANDBOX::SuperTooltips::GetItemTableType);

	// �õ��䷽��Ҫ�ļ��ܵ����ơ�
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetPrescrSkillName", &SCRIPT_SANDBOX::SuperTooltips::GetPrescrSkillName);

	// �õ��䷽��Ҫ�ļ��ܵĵȼ�
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetPrescrNeedSkillLevel", &SCRIPT_SANDBOX::SuperTooltips::GetPrescrNeedSkillLevel);

	// �õ��䷽��Ҫ�ļ��ܵĵ�ǰ�ĵȼ�
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetPrescrNeedSkillCurLevel", &SCRIPT_SANDBOX::SuperTooltips::GetPrescrNeedSkillCurLevel);

	// ����������Ʒ����ϸ��Ϣ��
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("SendAskItemInfoMsg", &SCRIPT_SANDBOX::SuperTooltips::SendAskItemInfoMsg);

	// �õ���ɫװ���ļ�����Ϣ
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("IsBlueEquipIdentify", &SCRIPT_SANDBOX::SuperTooltips::IsBlueEquipIdentify);

	// NPC �̵��Ƿ��
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("IsNpcShopOpen", &SCRIPT_SANDBOX::SuperTooltips::IsNpcShopOpen);
	
	//npc�Ƿ����
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("IsPresent", &SCRIPT_SANDBOX::SuperTooltips::IsPresent);

	//�õ���������2006-4-28
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetTypeDesc", &SCRIPT_SANDBOX::SuperTooltips::GetTypeDesc);

	//�õ���Ʒ�Ƿ��ڱ�����2006-5-10
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("IsItemInPacket", &SCRIPT_SANDBOX::SuperTooltips::IsItemInPacket);

	//�õ���Ʒ�Ƿ񱻱���
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("IsProtected", &SCRIPT_SANDBOX::SuperTooltips::GetItem_IsProtected);

	// �õ��Ƿ���ʾ�۸� 2006-5-18
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("IsShowPrice", &SCRIPT_SANDBOX::SuperTooltips::IsShowPrice);

	// �õ���ʾ�۸� 2006-5-18
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetShowPrice", &SCRIPT_SANDBOX::SuperTooltips::GetShowPrice);

	// �õ���Ʒ���εȼ����� 2006-5-18
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemLevelDesc", &SCRIPT_SANDBOX::SuperTooltips::GetItemLevelDesc);

	// �õ���λ��������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetAnchorValue", &SCRIPT_SANDBOX::SuperTooltips::GetAnchorValue);

	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("IsItemSuit", &SCRIPT_SANDBOX::SuperTooltips::IsItemSuit);
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetItemSuitNumber", &SCRIPT_SANDBOX::SuperTooltips::GetItemSuitNumber);
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("IsItemExist", &SCRIPT_SANDBOX::SuperTooltips::IsItemExist);
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetSuitAtt", &SCRIPT_SANDBOX::SuperTooltips::GetSuitAtt);

	// �õ���Ʊ������
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetTwoMoneyValue", &SCRIPT_SANDBOX::SuperTooltips::GetTwoMoneyValue);

	//SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetCashValue", &SCRIPT_SANDBOX::SuperTooltips::GetCashValue);
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable->RegisterObjectFunctor("GetSysColour", &SCRIPT_SANDBOX::SuperTooltips::GetSysColour);

	LuaObject obj_SuperTooltips = pState->BoxPointer(&(SCRIPT_SANDBOX::SuperTooltips::s_SupperTooltip));
	obj_SuperTooltips.SetMetaTable(*SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable);
	objGlobal.SetObject("SuperTooltips", obj_SuperTooltips);
	
	//---------------------------------------------------------------------------------
	//��̯���(Sale)
	SCRIPT_SANDBOX::StallSale::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::StallSale::s_pMetaTable = objGlobal.CreateTable("StallSale");
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::StallSale::s_pMetaTable);

	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("IsPresent", &SCRIPT_SANDBOX::StallSale::IsPresent);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("GetPosTax", &SCRIPT_SANDBOX::StallSale::GetPosTax);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("GetTradeTax", &SCRIPT_SANDBOX::StallSale::GetTradeTax);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("ReferItemPrice", &SCRIPT_SANDBOX::StallSale::ReferItemPrice);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("ModifStallName", &SCRIPT_SANDBOX::StallSale::ModifStallName);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("ModifItemPrice", &SCRIPT_SANDBOX::StallSale::ModifItemPrice);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("CloseStall", &SCRIPT_SANDBOX::StallSale::CloseStall);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("ItemReprice", &SCRIPT_SANDBOX::StallSale::ItemReprice);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("AgreeBeginStall", &SCRIPT_SANDBOX::StallSale::AgreeBeginStall);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("DeleteItem", &SCRIPT_SANDBOX::StallSale::DeleteItem);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("GetPrice", &SCRIPT_SANDBOX::StallSale::GetPrice);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("OpenMessageSale", &SCRIPT_SANDBOX::StallSale::OpenMessageSale);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("ApplyAd", &SCRIPT_SANDBOX::StallSale::ApplyAd);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("CloseStallMessage", &SCRIPT_SANDBOX::StallSale::CloseStallMessage);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("GetStallName", &SCRIPT_SANDBOX::StallSale::GetStallName);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("GetAdvertise", &SCRIPT_SANDBOX::StallSale::GetAdvertise);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("GetGuid", &SCRIPT_SANDBOX::StallSale::GetGuid);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("GetPetNum", &SCRIPT_SANDBOX::StallSale::GetPetNum);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("EnumPet", &SCRIPT_SANDBOX::StallSale::EnumPet);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("PetUpStall", &SCRIPT_SANDBOX::StallSale::PetUpStall);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("PetReprice", &SCRIPT_SANDBOX::StallSale::PetReprice);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("GetDefaultPage", &SCRIPT_SANDBOX::StallSale::GetDefaultPage);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("SetDefaultPage", &SCRIPT_SANDBOX::StallSale::SetDefaultPage);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("ViewPetDesc", &SCRIPT_SANDBOX::StallSale::ViewPetDesc);
	SCRIPT_SANDBOX::StallSale::s_pMetaTable->RegisterObjectFunctor("SetSelectPet", &SCRIPT_SANDBOX::StallSale::SetSelectPet);


	LuaObject obj_StallSale = pState->BoxPointer(&(SCRIPT_SANDBOX::StallSale::s_StallSale));
	obj_StallSale.SetMetaTable(*SCRIPT_SANDBOX::StallSale::s_pMetaTable);
	objGlobal.SetObject("StallSale", obj_StallSale);

	//---------------------------------------------------------------------------------
	//��̯���(Buy)
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::StallBuy::s_pMetaTable = objGlobal.CreateTable("StallBuy");
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::StallBuy::s_pMetaTable);

	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("IsPresent", &SCRIPT_SANDBOX::StallBuy::IsPresent);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("GetTradeTax", &SCRIPT_SANDBOX::StallBuy::GetTradeTax);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("OpenStall", &SCRIPT_SANDBOX::StallBuy::OpenStall);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("GetPrice", &SCRIPT_SANDBOX::StallBuy::GetPrice);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("BuyItem", &SCRIPT_SANDBOX::StallBuy::BuyItem);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("OpenMessageBuy", &SCRIPT_SANDBOX::StallBuy::OpenMessageBuy);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("GetStallName", &SCRIPT_SANDBOX::StallBuy::GetStallName);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("GetAdvertise", &SCRIPT_SANDBOX::StallBuy::GetAdvertise);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("CloseStallMessage", &SCRIPT_SANDBOX::StallBuy::CloseStallMessage);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("GetGuid", &SCRIPT_SANDBOX::StallBuy::GetGuid);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("GetStallerName", &SCRIPT_SANDBOX::StallBuy::GetStallerName);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("GetPetNum", &SCRIPT_SANDBOX::StallBuy::GetPetNum);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("EnumPet", &SCRIPT_SANDBOX::StallBuy::EnumPet);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("GetDefaultPage", &SCRIPT_SANDBOX::StallBuy::GetDefaultPage);
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable->RegisterObjectFunctor("BuyPet", &SCRIPT_SANDBOX::StallBuy::BuyPet);

	LuaObject obj_StallBuy = pState->BoxPointer(&(SCRIPT_SANDBOX::StallBuy::s_StallBuy));
	obj_StallBuy.SetMetaTable(*SCRIPT_SANDBOX::StallBuy::s_pMetaTable);
	objGlobal.SetObject("StallBuy", obj_StallBuy);

	//---------------------------------------------------------------------------------
	//��̯��Ϣ
	SCRIPT_SANDBOX::StallBbs::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::StallBbs::s_pMetaTable = objGlobal.CreateTable("StallBbs");
	SCRIPT_SANDBOX::StallBbs::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::StallBbs::s_pMetaTable);

	SCRIPT_SANDBOX::StallBbs::s_pMetaTable->RegisterObjectFunctor("GetMessageNum", &SCRIPT_SANDBOX::StallBbs::GetMessageNum);
	SCRIPT_SANDBOX::StallBbs::s_pMetaTable->RegisterObjectFunctor("EnumMessage", &SCRIPT_SANDBOX::StallBbs::EnumMessage);
	SCRIPT_SANDBOX::StallBbs::s_pMetaTable->RegisterObjectFunctor("AddMessage", &SCRIPT_SANDBOX::StallBbs::AddMessage);
	SCRIPT_SANDBOX::StallBbs::s_pMetaTable->RegisterObjectFunctor("ReplyMessage", &SCRIPT_SANDBOX::StallBbs::ReplyMessage);

	LuaObject obj_StallBbs = pState->BoxPointer(&(SCRIPT_SANDBOX::StallBbs::s_StallBbs));
	obj_StallBbs.SetMetaTable(*SCRIPT_SANDBOX::StallBbs::s_pMetaTable);
	objGlobal.SetObject("StallBbs", obj_StallBbs);

	//---------------------------------------------------------------------------------
	//�����ύ
	SCRIPT_SANDBOX::MissionReply::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::MissionReply::s_pMetaTable = objGlobal.CreateTable("MissionReply");
	SCRIPT_SANDBOX::MissionReply::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::MissionReply::s_pMetaTable);

	SCRIPT_SANDBOX::MissionReply::s_pMetaTable->RegisterObjectFunctor("IsPresent", &SCRIPT_SANDBOX::MissionReply::IsPresent);
	SCRIPT_SANDBOX::MissionReply::s_pMetaTable->RegisterObjectFunctor("OpenPetFrame", &SCRIPT_SANDBOX::MissionReply::OpenPetFrame);
	SCRIPT_SANDBOX::MissionReply::s_pMetaTable->RegisterObjectFunctor("OnContinue", &SCRIPT_SANDBOX::MissionReply::OnContinue);
	SCRIPT_SANDBOX::MissionReply::s_pMetaTable->RegisterObjectFunctor("EnumItem", &SCRIPT_SANDBOX::MissionReply::EnumItem);

	LuaObject obj_MissionReply = pState->BoxPointer(&(SCRIPT_SANDBOX::MissionReply::s_MissionReply));
	obj_MissionReply.SetMetaTable(*SCRIPT_SANDBOX::MissionReply::s_pMetaTable);
	objGlobal.SetObject("MissionReply", obj_MissionReply);


	//---------------------------------------------------------------------------------
	//����ӿ�
	SCRIPT_SANDBOX::Talk::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::Talk::s_pMetaTable = objGlobal.CreateTable("Talk");
	SCRIPT_SANDBOX::Talk::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::Talk::s_pMetaTable);

	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("GetChannelNumber", &SCRIPT_SANDBOX::Talk::GetChannelNumber);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("GetChannel", &SCRIPT_SANDBOX::Talk::GetChannel);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("GetChannelHeader", &SCRIPT_SANDBOX::Talk::GetChannelHeader);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("SendChatMessage", &SCRIPT_SANDBOX::Talk::SendChatMessage);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("InsertHistory", &SCRIPT_SANDBOX::Talk::InsertHistory);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("SetMaxSaveNumber", &SCRIPT_SANDBOX::Talk::SetMaxSaveNumber);

	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("CreateTab", &SCRIPT_SANDBOX::Talk::CreateTab);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("ConfigTab", &SCRIPT_SANDBOX::Talk::ConfigTab);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("CreateTabFinish", &SCRIPT_SANDBOX::Talk::CreateTabFinish);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("ConfigTabFinish", &SCRIPT_SANDBOX::Talk::ConfigTabFinish);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("SaveTab", &SCRIPT_SANDBOX::Talk::SaveTab);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("ClearTab", &SCRIPT_SANDBOX::Talk::ClearTab);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("GetChannelType", &SCRIPT_SANDBOX::Talk::GetChannelType);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("SelectFaceMotion", &SCRIPT_SANDBOX::Talk::SelectFaceMotion);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("SelectTextColor", &SCRIPT_SANDBOX::Talk::SelectTextColor);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("SaveOldTalkMsg", &SCRIPT_SANDBOX::Talk::SaveOldTalkMsg);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("ChangeCurrentChannel", &SCRIPT_SANDBOX::Talk::ChangeCurrentChannel);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("ModifyChatTxt", &SCRIPT_SANDBOX::Talk::ModifyChatTxt);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("ShowContexMenu", &SCRIPT_SANDBOX::Talk::ShowContexMenu);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("ContexMenuTalk", &SCRIPT_SANDBOX::Talk::ContexMenuTalk);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("GetCurrentSelectName", &SCRIPT_SANDBOX::Talk::GetCurrentSelectName);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("GetTalkTemplateString", &SCRIPT_SANDBOX::Talk::GetTalkTemplateString_Lua);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("EnumChatMood", &SCRIPT_SANDBOX::Talk::EnumChatMood);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("ShowChatMood", &SCRIPT_SANDBOX::Talk::ShowChatMood);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("HideChatMood", &SCRIPT_SANDBOX::Talk::HideChatMood);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("HandleMenuAction", &SCRIPT_SANDBOX::Talk::HandleMenuAction);

	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("ShowPingBi", &SCRIPT_SANDBOX::Talk::ShowPingBi);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("GetPingBiNum", &SCRIPT_SANDBOX::Talk::GetPingBiNum);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("GetPingBiName", &SCRIPT_SANDBOX::Talk::GetPingBiName);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("DelPingBi", &SCRIPT_SANDBOX::Talk::DelPingBi);
	SCRIPT_SANDBOX::Talk::s_pMetaTable->RegisterObjectFunctor("GetCurInputLanguage", &SCRIPT_SANDBOX::Talk::GetCurInputLanguage);

	LuaObject obj_Talk = pState->BoxPointer(&(SCRIPT_SANDBOX::Talk::s_Talk));
	obj_Talk.SetMetaTable(*SCRIPT_SANDBOX::Talk::s_pMetaTable);
	objGlobal.SetObject("Talk", obj_Talk);

	//---------------------------------------------------------------------------------
	//Npc Shop
	SCRIPT_SANDBOX::NpcShop::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::NpcShop::s_pMetaTable = objGlobal.CreateTable("NpcShop");
	SCRIPT_SANDBOX::NpcShop::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::NpcShop::s_pMetaTable);
	SCRIPT_SANDBOX::NpcShop::s_pMetaTable->RegisterObjectFunctor("Close", &SCRIPT_SANDBOX::NpcShop::Close);
	SCRIPT_SANDBOX::NpcShop::s_pMetaTable->RegisterObjectFunctor("GetNpcId", &SCRIPT_SANDBOX::NpcShop::GetNpcId);
	SCRIPT_SANDBOX::NpcShop::s_pMetaTable->RegisterObjectFunctor("GetNpcCamp", &SCRIPT_SANDBOX::NpcShop::GetNpcCamp);
	//SCRIPT_SANDBOX::NpcShop::s_pMetaTable->RegisterObjectFunctor("EnumCallBackItem", &SCRIPT_SANDBOX::NpcShop::EnumCallBackItem);
	SCRIPT_SANDBOX::NpcShop::s_pMetaTable->RegisterObjectFunctor("GetCallBackNum", &SCRIPT_SANDBOX::NpcShop::GetCallBackNum);
	SCRIPT_SANDBOX::NpcShop::s_pMetaTable->RegisterObjectFunctor("EnumItemPrice", &SCRIPT_SANDBOX::NpcShop::EnumItemPrice);
	SCRIPT_SANDBOX::NpcShop::s_pMetaTable->RegisterObjectFunctor("EnumItemMaxOverlay", &SCRIPT_SANDBOX::NpcShop::EnumItemMaxOverlay);
	SCRIPT_SANDBOX::NpcShop::s_pMetaTable->RegisterObjectFunctor("EnumItemName", &SCRIPT_SANDBOX::NpcShop::EnumItemName);
	//SCRIPT_SANDBOX::NpcShop::s_pMetaTable->RegisterObjectFunctor("BulkBuyItem", &SCRIPT_SANDBOX::NpcShop::BulkBuyItem);
	//SCRIPT_SANDBOX::NpcShop::s_pMetaTable->RegisterObjectFunctor("GetShopType", &SCRIPT_SANDBOX::NpcShop::GetShopType);
	//SCRIPT_SANDBOX::NpcShop::s_pMetaTable->RegisterObjectFunctor("GetRepairAllPrice", &SCRIPT_SANDBOX::NpcShop::GetRepairAllPrice);

	LuaObject obj_NpcShop = pState->BoxPointer(&(SCRIPT_SANDBOX::NpcShop::s_NpcShop));
	obj_NpcShop.SetMetaTable(*SCRIPT_SANDBOX::NpcShop::s_pMetaTable);
	objGlobal.SetObject("NpcShop", obj_NpcShop);

	//---------------------------------------------------------------------------------
	//System Setup
	SCRIPT_SANDBOX::SystemSetup* pSystemSetup = new SCRIPT_SANDBOX::SystemSetup();
	pSystemSetup->RegisterFunction("Texture", &SCRIPT_SANDBOX::SystemSetup::Lua_Texture);
	//ִ����Ӧһ���˵�����
	pSystemSetup->RegisterFunction("Texture", &SCRIPT_SANDBOX::SystemSetup::Lua_Texture);
	pSystemSetup->RegisterFunction("Display", &SCRIPT_SANDBOX::SystemSetup::Lua_Display);
	pSystemSetup->RegisterFunction("Sound", &SCRIPT_SANDBOX::SystemSetup::Lua_Sound);
	pSystemSetup->RegisterFunction("OpenSetup", &SCRIPT_SANDBOX::SystemSetup::Lua_OpenSetup);
	pSystemSetup->RegisterFunction("ViewSetup", &SCRIPT_SANDBOX::SystemSetup::Lua_ViewSetup);
	pSystemSetup->RegisterFunction("SoundSetup", &SCRIPT_SANDBOX::SystemSetup::Lua_SoundSetup);
	pSystemSetup->RegisterFunction("UISetup", &SCRIPT_SANDBOX::SystemSetup::Lua_UISetup);
	pSystemSetup->RegisterFunction("InputSetup", &SCRIPT_SANDBOX::SystemSetup::Lua_InputSetup);
	pSystemSetup->RegisterFunction("GameSetup", &SCRIPT_SANDBOX::SystemSetup::Lua_GameSetup);
	pSystemSetup->RegisterFunction("GameHelp", &SCRIPT_SANDBOX::SystemSetup::Lua_GameHelp);
	pSystemSetup->RegisterFunction("BackGame", &SCRIPT_SANDBOX::SystemSetup::Lua_BackGame);
	//��Ƶ���
	pSystemSetup->RegisterFunction("View_GetData", &SCRIPT_SANDBOX::SystemSetup::Lua_View_GetData);
	pSystemSetup->RegisterFunction("View_SetData", &SCRIPT_SANDBOX::SystemSetup::Lua_View_SetData);
	pSystemSetup->RegisterFunction("View_SaveToFile", &SCRIPT_SANDBOX::SystemSetup::Lua_View_SaveToFile);
	//��Ϸ����
	pSystemSetup->RegisterFunction("GameGetData", &SCRIPT_SANDBOX::SystemSetup::Lua_GameGetData);
	pSystemSetup->RegisterFunction("SaveGameSetup", &SCRIPT_SANDBOX::SystemSetup::Lua_SaveGameSetup);
	//�����Ϣ
	pSystemSetup->RegisterFunction("AskPrivateInfo", &SCRIPT_SANDBOX::SystemSetup::Lua_AskPrivateInfo);
	pSystemSetup->RegisterFunction("ApplyPrivateInfo", &SCRIPT_SANDBOX::SystemSetup::Lua_ApplyPrivateInfo);
	pSystemSetup->RegisterFunction("GetPrivateInfo", &SCRIPT_SANDBOX::SystemSetup::Lua_GetPrivateInfo);
	pSystemSetup->RegisterFunction("OpenPrivatePage", &SCRIPT_SANDBOX::SystemSetup::Lua_OpenPrivatePage);
	pSystemSetup->RegisterFunction("SetPrivateInfo", &SCRIPT_SANDBOX::SystemSetup::Lua_SetPrivateInfo);
	pSystemSetup->RegisterFunction("OpenEquipFrame", &SCRIPT_SANDBOX::SystemSetup::Lua_OpenEquipFrame);
	pSystemSetup->RegisterFunction("OpenPetFrame", &SCRIPT_SANDBOX::SystemSetup::Lua_OpenPetFrame);
	pSystemSetup->RegisterFunction("GetCaredObjId", &SCRIPT_SANDBOX::SystemSetup::Lua_GetCaredObjId);


	//---------------------------------------------------------------------------------
	//PlayerShop
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::PlayerShop::s_pMetaTable = objGlobal.CreateTable("PlayerShop");
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::PlayerShop::s_pMetaTable);

	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("CreateShop", &SCRIPT_SANDBOX::PlayerShop::CreateShop);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetShopNum", &SCRIPT_SANDBOX::PlayerShop::GetShopNum);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("EnumShop", &SCRIPT_SANDBOX::PlayerShop::EnumShop);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("AskOpenShop", &SCRIPT_SANDBOX::PlayerShop::AskOpenShop);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("EnumItem", &SCRIPT_SANDBOX::PlayerShop::EnumItem);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("EnumShopInfo", &SCRIPT_SANDBOX::PlayerShop::EnumShopInfo);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetSelectIndex", &SCRIPT_SANDBOX::PlayerShop::GetSelectIndex);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("RetackItem", &SCRIPT_SANDBOX::PlayerShop::RetackItem);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("InputMoney", &SCRIPT_SANDBOX::PlayerShop::InputMoney);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("UpStall", &SCRIPT_SANDBOX::PlayerShop::UpStall);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("SetCurSelectPage", &SCRIPT_SANDBOX::PlayerShop::SetCurSelectPage);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetCurSelectPage", &SCRIPT_SANDBOX::PlayerShop::GetCurSelectPage);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("DealMoney", &SCRIPT_SANDBOX::PlayerShop::DealMoney);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("ApplyMoney", &SCRIPT_SANDBOX::PlayerShop::ApplyMoney);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("InfoMoney", &SCRIPT_SANDBOX::PlayerShop::InfoMoney);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetStallNum", &SCRIPT_SANDBOX::PlayerShop::GetStallNum);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("BuyItem", &SCRIPT_SANDBOX::PlayerShop::BuyItem);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("IsSelectOnSale", &SCRIPT_SANDBOX::PlayerShop::IsSelectOnSale);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("DownSale", &SCRIPT_SANDBOX::PlayerShop::DownSale);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("ClearSelect", &SCRIPT_SANDBOX::PlayerShop::ClearSelect);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetMoney", &SCRIPT_SANDBOX::PlayerShop::GetMoney);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetCommercialFactor", &SCRIPT_SANDBOX::PlayerShop::GetCommercialFactor);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetShopInfo", &SCRIPT_SANDBOX::PlayerShop::GetShopInfo);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("IsOpenStall", &SCRIPT_SANDBOX::PlayerShop::IsOpenStall);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("OpenStall", &SCRIPT_SANDBOX::PlayerShop::OpenStall);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("AskStallData", &SCRIPT_SANDBOX::PlayerShop::AskStallData);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("EnumPet", &SCRIPT_SANDBOX::PlayerShop::EnumPet);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("SetSelectPet", &SCRIPT_SANDBOX::PlayerShop::SetSelectPet);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetCanOpenShopType", &SCRIPT_SANDBOX::PlayerShop::GetCanOpenShopType);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("SetCurSelectPetIndex", &SCRIPT_SANDBOX::PlayerShop::SetCurSelectPetIndex);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetObjPrice", &SCRIPT_SANDBOX::PlayerShop::GetObjPrice);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("Modify", &SCRIPT_SANDBOX::PlayerShop::Modify);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetObjName", &SCRIPT_SANDBOX::PlayerShop::GetObjName);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("ViewPetDesc", &SCRIPT_SANDBOX::PlayerShop::ViewPetDesc);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("ClearSelectPos", &SCRIPT_SANDBOX::PlayerShop::ClearSelectPos);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("Transfer", &SCRIPT_SANDBOX::PlayerShop::Transfer);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("IsSaleOut", &SCRIPT_SANDBOX::PlayerShop::IsSaleOut);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("BuyShop", &SCRIPT_SANDBOX::PlayerShop::BuyShop);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("CloseShop", &SCRIPT_SANDBOX::PlayerShop::CloseShop);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("OpenMessage", &SCRIPT_SANDBOX::PlayerShop::OpenMessage);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetMessageNum", &SCRIPT_SANDBOX::PlayerShop::GetMessageNum);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetCurPageMessageNum", &SCRIPT_SANDBOX::PlayerShop::GetCurPageMessageNum);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("EnumMessage", &SCRIPT_SANDBOX::PlayerShop::EnumMessage);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("DealFriend", &SCRIPT_SANDBOX::PlayerShop::DealFriend);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetFriendNum", &SCRIPT_SANDBOX::PlayerShop::GetFriendNum);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("EnumFriend", &SCRIPT_SANDBOX::PlayerShop::EnumFriend);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("ChangeShopNum", &SCRIPT_SANDBOX::PlayerShop::ChangeShopNum);
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable->RegisterObjectFunctor("GetNpcId", &SCRIPT_SANDBOX::PlayerShop::GetNpcId);

	LuaObject obj_PlayerShop = pState->BoxPointer(&(SCRIPT_SANDBOX::PlayerShop::s_PlayerShop));
	obj_PlayerShop.SetMetaTable(*SCRIPT_SANDBOX::PlayerShop::s_pMetaTable);
	objGlobal.SetObject("PlayerShop", obj_PlayerShop);

	//---------------------------------------------------------------------------------
	//Friend
	SCRIPT_SANDBOX::Friend::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::Friend::s_pMetaTable = objGlobal.CreateTable("Friend");
	SCRIPT_SANDBOX::Friend::s_pMetaTable->SetObject("__index", *SCRIPT_SANDBOX::Friend::s_pMetaTable);

	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor("Close", &SCRIPT_SANDBOX::Friend::Close);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor("EnumName", &SCRIPT_SANDBOX::Friend::EnumName);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor("OpenGrouping", &SCRIPT_SANDBOX::Friend::OpenGrouping);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor("OpenMenu", &SCRIPT_SANDBOX::Friend::OpenMenu);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor("AskTeam", &SCRIPT_SANDBOX::Friend::AskTeam);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor("GetHistroyNumber", &SCRIPT_SANDBOX::Friend::GetHistroyNumber);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor("GetHistroyData", &SCRIPT_SANDBOX::Friend::GetHistroyData);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor("SetCurrentTeam", &SCRIPT_SANDBOX::Friend::SetCurrentTeam);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor("GetCurrentTeam", &SCRIPT_SANDBOX::Friend::GetCurrentTeam);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor("SetCurrentSelect", &SCRIPT_SANDBOX::Friend::SetCurrentSelect);//__
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor("GetCurrentSelect", &SCRIPT_SANDBOX::Friend::GetCurrentSelect);//__
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor("CallOf", &SCRIPT_SANDBOX::Friend::CallOf);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor("ViewFeel", &SCRIPT_SANDBOX::Friend::ViewFeel);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor("IsMoodInHead", &SCRIPT_SANDBOX::Friend::IsMoodInHead);
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor("IsPlayerIsFriend", &SCRIPT_SANDBOX::Friend::IsPlayerIsFriend);//__
	SCRIPT_SANDBOX::Friend::s_pMetaTable->RegisterObjectFunctor("InviteTeam", &SCRIPT_SANDBOX::Friend::InviteTeam);//__
		
	

	LuaObject obj_Friend = pState->BoxPointer(&(SCRIPT_SANDBOX::Friend::s_Friend));
	obj_Friend.SetMetaTable(*SCRIPT_SANDBOX::Friend::s_pMetaTable);
	objGlobal.SetObject("Friend", obj_Friend);

	//-----------------------------------------------------------------------------------------------------
	//
	// ��¼���̵������� 2006-5-21
	//
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable = new LuaPlus::LuaObject;
	*SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable = objGlobal.CreateTable("GameProduceLogin");

	// �õ�����ĸ���
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetServerAreaCount",		&SCRIPT_SANDBOX::CGameProduce_Login::GetAreaCount);

	// �õ����������
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetServerAreaName",		&SCRIPT_SANDBOX::CGameProduce_Login::GetAreaName);

	// �õ�������login server�ĸ���
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetAreaLoginServerCount",	&SCRIPT_SANDBOX::CGameProduce_Login::GetAreaLoginServerCount);

	// �õ�login server����Ϣ
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetAreaLoginServerInfo",	&SCRIPT_SANDBOX::CGameProduce_Login::GetAreaLoginServerInfo);

	// ���ӵ�login server
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("ConnectToLoginServer",	&SCRIPT_SANDBOX::CGameProduce_Login::ConnectToLoginServer);

	// �򿪷�����ѡ�����
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("ExitToSelectServer",	&SCRIPT_SANDBOX::CGameProduce_Login::ExitToSelectServer);

	// ��֤�û���������
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("CheckAccount",	&SCRIPT_SANDBOX::CGameProduce_Login::CheckAccount);

	// ѡ��һ��login server������
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("SelectLoginServer",	&SCRIPT_SANDBOX::CGameProduce_Login::SelectLoginServer);

	// ������ѡ������л����ʺ��������
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("ChangeToAccountInputDlgFromSelectRole",	&SCRIPT_SANDBOX::CGameProduce_Login::ChangeToAccountInputDlgFromSelectRole);

	// ������ѡ������л������ﴴ������
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("ChangeToCreateRoleDlgFromSelectRole",	&SCRIPT_SANDBOX::CGameProduce_Login::ChangeToCreateRoleDlgFromSelectRole);

	// �����ﴴ�������л�������ѡ�����
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("ChangeToSelectRoleDlgFromCreateRole",	&SCRIPT_SANDBOX::CGameProduce_Login::ChangeToSelectRoleDlgFromCreateRole);
	
	// �õ���ǰ��ɫ�ĸ���
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetRoleCount",	&SCRIPT_SANDBOX::CGameProduce_Login::GetRoleCount);
	
	// �õ���ɫ����Ϣ
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetRoleInfo",	&SCRIPT_SANDBOX::CGameProduce_Login::GetRoleInfo);

	// �õ���ɫUI modle ����
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetRoleUIModleName",	&SCRIPT_SANDBOX::CGameProduce_Login::GetRoleUIModleName);

	// ����һ����ɫ
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("CreateRole",	&SCRIPT_SANDBOX::CGameProduce_Login::CreateRole);

	//ɾ��һ����ɫ
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("DelRole",	&SCRIPT_SANDBOX::CGameProduce_Login::DelRole);

	//���ͽ�����Ϸ��Ϣ
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("SendEnterGameMsg",	&SCRIPT_SANDBOX::CGameProduce_Login::SendEnterGameMsg);


	// ѯ���Ƿ�ɾ����ɫ
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("DelRole_YesNo",	&SCRIPT_SANDBOX::CGameProduce_Login::DelRole_YesNo);

	// ѯ���Ƿ��˳���Ϸ
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("ExitGame_YesNo",	&SCRIPT_SANDBOX::CGameProduce_Login::ExitGame_YesNo);

	// ѯ���Ƿ��˵��ʺ��������
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("ExitToAccountInput_YesNo",	&SCRIPT_SANDBOX::CGameProduce_Login::ExitToAccountInput_YesNo);

	// ɾ��ѡ�еĽ�ɫ
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("DelSelRole",	&SCRIPT_SANDBOX::CGameProduce_Login::DelSelRole);

	//�õ��������������Ϣ
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetAreaServerInfo",	&SCRIPT_SANDBOX::CGameProduce_Login::GetAreaServerInfo);

	// �Զ�ѡ��һ���뵱ǰѡ������繩Ӧ��ƥ���login server
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("AutoSelLoginServer",	&SCRIPT_SANDBOX::CGameProduce_Login::AutoSelLoginServer);

	// �õ�Ů����ͷ��ĸ���.
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetWomanFaceCount",	&SCRIPT_SANDBOX::CGameProduce_Login::GetWomanFaceCount);

	// �õ�������ͷ��ĸ���.
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetManFaceCount",	&SCRIPT_SANDBOX::CGameProduce_Login::GetManFaceCount);

	// �õ�Ů����ͷ�����Ϣ
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetWomanFaceName",	&SCRIPT_SANDBOX::CGameProduce_Login::GetWomanFaceName);

	// �õ�������ͷ�����Ϣ.
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetManFaceName",	&SCRIPT_SANDBOX::CGameProduce_Login::GetManFaceName);

	// �õ�����ͷ�����Ϣ.
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetFaceName",	&SCRIPT_SANDBOX::CGameProduce_Login::GetFaceName);

	// ����ͷ��id
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("SetFaceId",	&SCRIPT_SANDBOX::CGameProduce_Login::SetFaceId);

	// �õ����εĸ���
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetFaceModelCount",	&SCRIPT_SANDBOX::CGameProduce_Login::GetFaceModelCount);

	// �õ�����ģ�͵�����
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetFaceModelName",	&SCRIPT_SANDBOX::CGameProduce_Login::GetFaceModelName);

	// ��������ģ��id
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("SetFaceModelId",	&SCRIPT_SANDBOX::CGameProduce_Login::SetFaceModelId);

	// �õ����͵ĸ���
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetHairModelCount",	&SCRIPT_SANDBOX::CGameProduce_Login::GetHairModelCount);

	// �õ�����ģ�͵�����
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetHairModelName",	&SCRIPT_SANDBOX::CGameProduce_Login::GetHairModelName);

	// ���÷���ģ��id
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("SetHairModelId",	&SCRIPT_SANDBOX::CGameProduce_Login::SetHairModelId);

	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("GetPlayerCamp",	&SCRIPT_SANDBOX::CGameProduce_Login::GetPlayerCamp);

	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("SetPlayerCamp",	&SCRIPT_SANDBOX::CGameProduce_Login::SetPlayerCamp);

	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("SetPlayerMenpai",	&SCRIPT_SANDBOX::CGameProduce_Login::SetPlayerMenpai);

	// �ر�����״̬��
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable->RegisterObjectFunctor("CloseNetConnect",	&SCRIPT_SANDBOX::CGameProduce_Login::CloseNetConnect);

	LuaObject obj_GameProduceLogin = pState->BoxPointer(&(SCRIPT_SANDBOX::CGameProduce_Login::s_GameProduceLogin));
	obj_GameProduceLogin.SetMetaTable(*SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable);
	objGlobal.SetObject("GameProduceLogin", obj_GameProduceLogin);

}

VOID CScriptSystem::Tick(void)
{
	const UINT COLLECT_TIME = 3*1000; //3m
	static INT s_nLastCollectTime = 0;

	UINT nTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();
	if(CGameProcedure::s_pTimeSystem->CalSubTime(s_nLastCollectTime, nTimeNow) >= COLLECT_TIME)
	{
		//�ͷ������ڴ�
		CScriptSystem::GetMe()->GetLuaState()->CollectGarbage();

		s_nLastCollectTime = nTimeNow;
	}
}

VOID CScriptSystem::Release(VOID)
{
	//����֤��
	delete SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable;
	SCRIPT_SANDBOX::PetInviteFriend::s_pMetaTable = NULL;

	//������
	delete SCRIPT_SANDBOX::Guild::s_pMetaTable;
	SCRIPT_SANDBOX::Guild::s_pMetaTable = NULL;

	//ϵͳ����
	delete SCRIPT_SANDBOX::SystemSetup::GetMe();

	//�����ύ
	delete SCRIPT_SANDBOX::Talk::s_pMetaTable;
	SCRIPT_SANDBOX::Talk::s_pMetaTable = NULL;

	//�����ύ
	delete SCRIPT_SANDBOX::MissionReply::s_pMetaTable;
	SCRIPT_SANDBOX::MissionReply::s_pMetaTable = NULL;

	//��̯���(Buy)
	delete SCRIPT_SANDBOX::StallBuy::s_pMetaTable;
	SCRIPT_SANDBOX::StallBuy::s_pMetaTable = NULL;

	//MissionReply
	delete SCRIPT_SANDBOX::MissionReply::s_pMetaTable;
	SCRIPT_SANDBOX::MissionReply::s_pMetaTable = NULL;

	//NpcShop
	delete SCRIPT_SANDBOX::NpcShop::s_pMetaTable;
	SCRIPT_SANDBOX::NpcShop::s_pMetaTable = NULL;

	//Friend
	delete SCRIPT_SANDBOX::Friend::s_pMetaTable;
	SCRIPT_SANDBOX::Friend::s_pMetaTable = NULL;

	//PlayerShop
	delete SCRIPT_SANDBOX::PlayerShop::s_pMetaTable;
	SCRIPT_SANDBOX::PlayerShop::s_pMetaTable = NULL;

	//��̯���(Sale)
	delete SCRIPT_SANDBOX::StallSale::s_pMetaTable;
	SCRIPT_SANDBOX::StallSale::s_pMetaTable = NULL;

	//��̯��Ϣ
	delete SCRIPT_SANDBOX::StallBbs::s_pMetaTable;
	SCRIPT_SANDBOX::StallBbs::s_pMetaTable = NULL;

	//Tooltip
	delete SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable;
	SCRIPT_SANDBOX::SuperTooltips::s_pMetaTable = NULL;

	//��Ұ���
	delete SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable;
	SCRIPT_SANDBOX::PlayerPackage::s_pMetaTable = NULL;

	//���֮��Ľ���
	delete SCRIPT_SANDBOX::Exchange::s_pMetaTable;
	SCRIPT_SANDBOX::Exchange::s_pMetaTable = NULL;

	//����Լ�������
	delete SCRIPT_SANDBOX::Bank::s_pMetaTable;
	SCRIPT_SANDBOX::Bank::s_pMetaTable = NULL;

	//���ﵼ������
	delete SCRIPT_SANDBOX::Pet::s_pMetaTable;
	SCRIPT_SANDBOX::Pet::s_pMetaTable = NULL;

	delete SCRIPT_SANDBOX::TargetPet::s_pMetaTable;
	SCRIPT_SANDBOX::TargetPet::s_pMetaTable = NULL;

	//�����
	delete SCRIPT_SANDBOX::LifeAbility::s_pMetaTable;
	SCRIPT_SANDBOX::LifeAbility::s_pMetaTable = NULL;

	//�������ʦ��������
	delete SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable;
	SCRIPT_SANDBOX::AbilityTeacher::s_pMetaTable = NULL;

	//��¼���浼������.2006-2-21
	delete SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable;
	SCRIPT_SANDBOX::CGameProduce_Login::s_pMetaTable = NULL;

	//���� ���ݳؽӿ�
	delete SCRIPT_SANDBOX::DataPool::s_pMetaTable;
	SCRIPT_SANDBOX::DataPool::s_pMetaTable = NULL;

	//���� ���ư�ť����
	delete CActionItem::s_pMetaTable;
	CActionItem::s_pMetaTable = NULL;

	std::list< CScriptEnvironment* >::iterator it;
	for(it=m_listScriptEnv.begin(); it!=m_listScriptEnv.end(); it++)
	{
		if(*it)
		{
			delete (CScriptEnvironment*)(*it);
			*it = NULL;
		}
	}

	m_listScriptEnv.clear();
}

void CScriptSystem::SetActiveEnvironment(LPCTSTR szEnvName)
{
	m_strEnvName = szEnvName;

	CHAR szSetEnvScript[MAX_PATH];
	_snprintf(szSetEnvScript, MAX_PATH, "setmetatable(_G, {__index = %s});", 
		szEnvName);
	GetLuaState()->DoString(szSetEnvScript);
}

void CScriptSystem::_loadQuestFile(void)
{
	const char* szQuestFileIndexFile = "Script.dat";
	const char* szBlankString = " \t";

	//-----------------------------
	//Open file
	char* lpAddress = NULL;
	DWORD dwSize = CGameProcedure::s_pResourceProvider->loadResource(szQuestFileIndexFile, lpAddress, "General");
	if(dwSize > 0)
	{
		register const char* pMemory = lpAddress;
		const char* pDeadEnd = lpAddress + dwSize + 1;

		char szLine[1024] = {0};
		bool bParseBegin = false;
		do
		{
			//��ȡһ��
			pMemory = TDU_GetLineFromMemory(szLine, 1024, pMemory, pDeadEnd);
			if(!pMemory) break;

			STRING strLine = szLine;
			//Trim left and right
			strLine = strLine.substr(strLine.find_first_not_of(szBlankString));
			strLine = strLine.substr(0, strLine.find_last_not_of(szBlankString)+1);
			//too short
			if(strLine.size() < 1) break;

			if(strLine[0] == ';') continue;	//ע����

			if(!bParseBegin)
			{
				//��ʼ����
				if(_stricmp(strLine.c_str(), "MGR_TXT") == 0)
				{
					bParseBegin = true;
				}
			}
			else
			{
				STRING::size_type nEqu = strLine.find('=');
				if(nEqu == STRING::npos) continue;

				int nID = atoi(strLine.substr(0, nEqu).c_str());
				STRING strFile = strLine.substr(nEqu+1);

				m_mapQuestFile.insert(std::make_pair(nID, strFile));
			}
		}while(true);

		CGameProcedure::s_pResourceProvider->unloadResource( lpAddress, dwSize );
	}
	else
	{
		TDThrow("Can't open quest file index file: %s", szQuestFileIndexFile);
		return;
	}
}