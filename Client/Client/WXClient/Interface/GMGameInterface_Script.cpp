#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"
#include "../Procedure/GamePro_Login.h"
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
#include "tddbc_struct.h"
#include "../object/character/obj_playermyself.h"
#include "tdutil.h"
#include "ScriptCommandStruct.h"
#include "CGEventRequest.h"
#include "CGTeamInvite.h"
#include "CGAskDetailEquipList.h"
#include "..\DBC\GMDataBase.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "GMInterface_Script_Tooltip.h"
#include "../world/scene.h"
#include "..\Procedure\GameProcedure.h"
#include "..\Procedure\GamePro_main.h"
#include "TDUISystem.h"
#include "..\Sound\GMSoundSystem.h"
#include "CGDiscardItem.h"
#include "CGDiscardEquip.h"
#include "CGCharIdle.h"
#include "..\Object\Item\Obj_Item_Equip.h"
#include "..\Object\ObjectCommandDef.h"
#include "CGExecuteScript.h"
#include "..\WxRender\RenderSystem.h"
#include "CGMinorPasswd.h"								// ��������2������
#include "..\Action\GMActionSystem_MouseCmd.h"
#include "CGAskDetailSkillList.h"

namespace SCRIPT_SANDBOX
{
	const FLOAT PI_2 = 2 * __PI;
//��½�����У����ڵ��ȷ����Ĳ���(-- for debug)
INT Lua_ServerLogin(LuaPlus::LuaState* state)
{
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcLogIn)
	{
		CSoundSystemFMod::_PlayUISoundFunc(68);
		if(CGameProcedure::s_pProcLogIn->GetStatus() == CGamePro_Login::LOGIN_DEBUG_SETTING)
		{
			CGameProcedure::s_pProcLogIn->SetStatus(CGamePro_Login::LOGIN_DISCONNECT);
		}
	}
	return 0;
}

//Ҫ���˳�����
INT Lua_QuitApplication(LuaPlus::LuaState* state)
{
	PostMessage(g_hMainWnd, WM_CLOSE, 0XC0DE, 0XC0DE);
	return 0;
}

//Ҫ���/�رռ�����
INT Lua_ToggleSkillBook(LuaPlus::LuaState* state)
{
	/*INT m_ID;*/
	if(CGameProcedure::s_pUISystem && 
		(CGameProcedure::s_pUISystem->IsWindowShow("LifeSkill") || 
		 CGameProcedure::s_pUISystem->IsWindowShow("ActionSkill") ||
		 CGameProcedure::s_pUISystem->IsWindowShow("CommonSkill") ) ) 
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_SKILL_BOOK);
	else
		/*m_ID = CObjectManager::GetMe()->GetMySelf()->GetServerID();
		CGAskDetailSkillList	msgMySkill;
		msgMySkill.setTargetID(m_ID);
		CNetManager::GetMe()->SendPacket(&msgMySkill);*/
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_COMMONSKILL_PAGE);
	//CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH,10);
	return 0;
}

//Ҫ��򿪼�����
INT Lua_OpenSkillBook(LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_TOGLE_SKILL_BOOK);
	//CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH,10);
	return 0;
}
//Ҫ���/�رճ������
INT Lua_TogglePetPage(LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_TOGLE_PET_PAGE);
	//CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_SHOW, "");
	//CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH,10);
	return 0;
}
//Ҫ���/�ر��Զ���ֽ���
INT Lua_ToAUTOHit(LuaPlus::LuaState* state)
{
	//fujia
	std::vector< STRING > strParamInfo;

	strParamInfo.push_back("ȷ��Ҫ�����Զ������?");
	strParamInfo.push_back("4");

	((CEventSystem*)CGameProcedure::s_pEventSystem)->PushEvent( GE_GAMELOGIN_SYSTEM_INFO_YESNO, strParamInfo);
	return 0;
}
//Ҫ�������ܽ���
INT Lua_OpenLifePage(LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_TOGLE_LIFE_PAGE);
//	CEventSystem::GetMe()->PushEvent(GE_UPDATE_TARGETPET_PAGE,0);

	//CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH,10);
	return 0;
}

//Ҫ�����ͨ���ܽ���
INT Lua_OpenCommonSkillPage(LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_TOGLE_COMMONSKILL_PAGE);
	//CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH,10);
	return 0;
}
INT Lua_UpdateMinimap( LuaPlus::LuaState* state )
{
	CGameProcedure::m_bNeedFreshMinimap = true;
	return 0;
}
INT Lua_OpenMinimap( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if( !args[ 1 ].IsString() ) return 0;
	if( strcmp( args[ 1 ].GetString(), "Minimap" ) == 0 ) // ��С��ͼ
	{
		CEventSystem::GetMe()->PushEvent( GE_OPEN_MINIMAP, "1" );
		
	}
	else if( strcmp( args[ 1 ].GetString(), "MinimapExp" ) == 0) // ����չС��ͼ
	{
		CEventSystem::GetMe()->PushEvent( GE_OPEN_MINIMAPEXP, "1" ); 
	}
	CGameProcedure::m_bNeedFreshMinimap = true;
	CGameProcedure::m_bWaitNeedFreshMinimap = true;
	return 0;
}
// �رմ򿪴��ͼ
INT Lua_ToggleLargeMap( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if( !args[ 1 ].IsString() ) 
		CEventSystem::GetMe()->PushEvent( GE_TOGLE_LARGEMAP, "2" );
		return 0;
	CEventSystem::GetMe()->PushEvent( GE_TOGLE_LARGEMAP, args[ 1 ].GetString() );
	CGameProcedure::m_bNeedFreshMinimap = true;
	CGameProcedure::m_bWaitNeedFreshMinimap = true;
	return 0;
}
// �رմ򿪳�����ͼ
INT Lua_ToggleSceneMap( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	std::vector< STRING > vParam;
	vParam.push_back( CWorldManager::GetMe()->GetActiveScene()->GetSceneDefine()->szSceneMap );
	if( !args[ 1 ].IsString() ) 
		vParam.push_back( "2" );
	else
		vParam.push_back( args[ 1 ].GetString() );
	
	CEventSystem::GetMe()->PushEvent(GE_TOGLE_SCENEMAP, vParam );
	CGameProcedure::m_bNeedFreshMinimap = true;
	CGameProcedure::m_bWaitNeedFreshMinimap = true;
	return 0;
}


//�򿪰���
INT Lua_ToggleContainer(LuaPlus::LuaState* state)  //fujia 2007.10.24
{
	CEventSystem::GetMe()->PushEvent(GE_TOGLE_CONTAINER);
	return 0;
}
INT Lua_ToggleContainer1(LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_TOGLE_CONTAINER1);
	return 0;
}
INT Lua_ToggleContainer2(LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_TOGLE_CONTAINER2);
	return 0;
}
//������
INT Lua_ToggleMission(LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_TOGLE_MISSION);
	return 0;
}
INT Lua_PushDebugMessage(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsString()))
	{
		TDThrow("LUA: Lua_PushDebugMessage Wrong Param");
	}
	STRING Msg = args[1].GetString();

	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,args[1].GetString());

	return 0;
}

INT Lua_Get_XParam_INT(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsInteger()))
	{
		TDThrow("LUA: Lua_Get_XParam_INT Wrong Param");
	}
	INT ret = (INT)CDataPool::GetMe()->X_PARAM_Get()->GetIntValue(args[1].GetInteger());

	state->PushInteger(ret);

	return 1;
}

INT Lua_Get_XParam_STR(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsInteger()))
	{
		TDThrow("LUA: Lua_Get_XParam_STR Wrong Param");
	}
	char* ret = CDataPool::GetMe()->X_PARAM_Get()->GetStrValue(args[1].GetInteger());
	if(!ret) return 0;

	state->PushString(ret);

	return 1;
}

INT Lua_Set_XSCRIPT_Function_Name(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsString()))
	{
		TDThrow("LUA: Lua_Set_XSCRIPT_Function_Name Wrong Param");
	}

	char* str_temp = (char*)args[1].GetString();

	if(!str_temp) return 0;
	CDataPool::GetMe()->X_XCRIPT_Get()->SetFunName(str_temp);

	return 0;
}

INT Lua_Set_XSCRIPT_Parameter(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsInteger() && args[2].IsInteger()))
	{
		TDThrow("LUA: Lua_Set_XSCRIPT_Parameter Wrong Param");
	}
	CDataPool::GetMe()->X_XCRIPT_Get()->SetParam(args[1].GetInteger(),args[2].GetInteger());

	return 0;
}

INT Lua_Set_XSCRIPT_ScriptID(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsInteger()))
	{
		TDThrow("LUA: Lua_Set_XSCRIPT_ScriptID Wrong Param");
	}
	CDataPool::GetMe()->X_XCRIPT_Get()->SetScriptID(args[1].GetInteger());

	return 0;
}

INT Lua_Set_XSCRIPT_ParamCount(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsInteger()))
	{
		TDThrow("LUA: Lua_Set_XSCRIPT_ParamCount Wrong Param");
	}
	CDataPool::GetMe()->X_XCRIPT_Get()->SetParamCount(args[1].GetInteger());

	return 0;
}

INT Lua_Clear_XSCRIPT(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	CDataPool::GetMe()->X_XCRIPT_Get()->CleanUp();

	return 0;
}

INT Lua_Send_XSCRIPT(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	CGExecuteScript msg;

	X_SCRIPT* pXScript_DataPool = CDataPool::GetMe()->X_XCRIPT_Get();
	X_SCRIPT* pXScript_Send = msg.GetScript();

	memcpy(pXScript_Send,pXScript_DataPool,sizeof(X_SCRIPT));
	
	CNetManager::GetMe()->SendPacket( &msg );
	
	return 0;
}

/*
//�������ݳ��ڴӷ���������������ʦ��������Ϣ
INT Lua_GetMenpaiInfo(LuaPlus::LuaState* state)
{
	INT idMenpai = (INT)CDataPool::GetMe()->JoinMenpai_GetMenpaiInfo();
	state->PushNumber( idMenpai );

	//�п��ܽ�Ҫ��չ����������
	return 1;
}
//��������
INT Lua_MenpaiFrameJoin(LuaPlus::LuaState* state)
{
	INT idMenpai = (INT)CDataPool::GetMe()->JoinMenpai_GetMenpaiInfo();

	CDataPool::GetMe()->SendJoinMenpaiEvent();
	return 0;
}
*/

//�����ķ��ȼ�
//INT Lua_GetXinfaLevel(LuaPlus::LuaState* state)
//{
//	LuaStack args(state);
//
//	if (!(args[1].IsInteger()))
//	{
//		TDThrow("LUA: Lua_GetXinfaLevel Wrong Param");
//	}
//	INT nXinfaID = args[1].GetInteger();
//
//	const SCLIENT_XINFA* pXinFa = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_XinFa(nXinfaID);
//	if(!pXinFa) 
//	{
//		state->PushInteger(-1);
//	}
//	else
//	{
//		state->PushInteger(pXinFa->m_nLevel);
//	}
//
//	return 1;
//}

//fujia 
//ͨ���������õ���ͬ�ȼ�����ID
INT GetStudySkillLevelId(int SkillId,int skillLevel )
{
	//���ؼ������
	DBC_DEFINEHANDLE(pSpendDBC, DBC_SKILL_DATA);

	INT nNum = (INT)pSpendDBC->GetRecordsNum();
	for(INT i=0; i<nNum; i++)
	{
		const _DBC_SKILL_DATA* pSpend = (const _DBC_SKILL_DATA*)pSpendDBC->Search_LineNum_EQU(i);
		if(pSpend->m_nID != SkillId) 
			continue;
		INT nSkilllevel	= pSpend->m_anSkillByLevel[skillLevel-1];

		//const _DBC_XINFA_STUDYSPEND* pSpend = (const _DBC_XINFA_STUDYSPEND*)pSpendDBC->Search_LineNum_EQU(i);
		//if(pSpend->nStudyLevel != nXinfaLevel) 
		//	continue;
		return nSkilllevel;
	}
	return -1;
}

//�õ�����ͼ��
INT Lua_GetSkillImage(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (!(args[1].IsInteger()))
	{
		TDThrow("LUA: Lua_GetSkillImage Wrong Param");
	}
	INT SkillId = args[1].GetInteger();
	//���ؼ������
	DBC_DEFINEHANDLE(pSpendDBC, DBC_SKILL_DATA);

	INT nNum = (INT)pSpendDBC->GetRecordsNum();
	for(INT i=0; i<nNum; i++)
	{
		const _DBC_SKILL_DATA* pSpend = (const _DBC_SKILL_DATA*)pSpendDBC->Search_LineNum_EQU(i);
		if(pSpend->m_nID != SkillId) 
			continue;

		LPCSTR nSkillImage	=  pSpend->m_lpszIconName;
		state->PushString((char *)nSkillImage);
		break;
	}
	return 1;
}
//�õ�ѧϰ���ܵĽ�ǮҪ��
INT Lua_GetStudySkillMoney(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (!(args[1].IsInteger()))
	{
		TDThrow("LUA: Lua_GetUplevelSkillSpendMoney Wrong Param");
	}

	if (!(args[2].IsInteger()))
	{
		TDThrow("LUA: Lua_GetUplevelSkillSpendMoney Wrong Param");
	}
	INT skillId = args[1].GetInteger();
	INT skillLvl = args[2].GetInteger();

	INT nSkilllevel = GetStudySkillLevelId(skillId,skillLvl);
	
	//���صȼ����ı�
	DBC_DEFINEHANDLE(pSpendDBC, DBC_SKILLDATA_V1_DEPLETE);

	INT mNum = (INT)pSpendDBC->GetRecordsNum();
	for(INT i=0; i<mNum; i++)
	{
		const _DBC_SKILLDATA_V1_DEPLETE* pSpend = (const _DBC_SKILLDATA_V1_DEPLETE*)pSpendDBC->Search_LineNum_EQU(i);
		if(pSpend->nId != nSkilllevel) 
			continue;
		INT nNeedMoney	= pSpend->nNeedMoney;

		//const _DBC_XINFA_STUDYSPEND* pSpend = (const _DBC_XINFA_STUDYSPEND*)pSpendDBC->Search_LineNum_EQU(i);
		//if(pSpend->nStudyLevel != nXinfaLevel) 
		//	continue;
		state->PushNumber( nNeedMoney );

		// ͬʱ���ת��Ϊ��ҡ����ҡ�ͭ�ҵĸ�ʽ���
		INT nMoney = nNeedMoney;
		INT nGoldCoin;	
		INT nSilverCoin;
		INT nCopperCoin;

		nCopperCoin = nMoney % 100;

		if( nMoney >= 100 )
		{
			nSilverCoin = ((nMoney-nCopperCoin)/100) % 100;
		}
		else
		{
			nSilverCoin = 0;
		}
		if ( nMoney >= 10000 )
		{
			nGoldCoin = (((nMoney-nCopperCoin)/100)-nSilverCoin)/100;
		}
		else
			nGoldCoin = 0;

		state->PushInteger(nGoldCoin);
		state->PushInteger(nSilverCoin);
		state->PushInteger(nCopperCoin);

		break;
	}
	return 4;
}
//�õ�ѧϰ���ܵĵȼ�Ҫ��
INT Lua_GetStudySkillName(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (!(args[1].IsInteger()))
	{
		TDThrow("LUA: Lua_GetUplevelSkillNeedLevel Wrong Param");
	}

	if (!(args[2].IsInteger()))
	{
		TDThrow("LUA: Lua_GetUplevelSkillNeedLevel Wrong Param");
	}
	INT skillId = args[1].GetInteger();
	INT skillLvl = args[2].GetInteger();

	INT nSkilllevel = GetStudySkillLevelId(skillId,skillLvl);

	//���صȼ����ı�
	DBC_DEFINEHANDLE(pSpendDBC, DBC_SKILLDATA_V1_DEPLETE);

	INT mNum = (INT)pSpendDBC->GetRecordsNum();
	for(INT i=0; i<mNum; i++)
	{
		const _DBC_SKILLDATA_V1_DEPLETE* pSpend1 = (const _DBC_SKILLDATA_V1_DEPLETE*)pSpendDBC->Search_LineNum_EQU(i);
		if(pSpend1->nId != nSkilllevel) 
			continue;
		LPCSTR nSkillName	= pSpend1->szEffectDesc;
		state->PushString((char *)nSkillName);
		break;
	}
	return 1;
}

//�õ�ѧϰ��������
INT Lua_GetStudySkillMS(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (!(args[1].IsInteger()))
	{
		TDThrow("LUA: Lua_GetStudySkillMS Wrong Param");
	}

	if (!(args[2].IsInteger()))
	{
		TDThrow("LUA: Lua_GetStudySkillMS Wrong Param");
	}
	INT skillId = args[1].GetInteger();
	INT skillLvl = args[2].GetInteger();

	INT nSkilllevel = GetStudySkillLevelId(skillId,skillLvl);

	//���صȼ����ı�
	DBC_DEFINEHANDLE(pSpendDBC, DBC_SKILLDATA_V1_DEPLETE);

	INT mNum = (INT)pSpendDBC->GetRecordsNum();
	for(INT i=0; i<mNum; i++)
	{
		const _DBC_SKILLDATA_V1_DEPLETE* pSpend1 = (const _DBC_SKILLDATA_V1_DEPLETE*)pSpendDBC->Search_LineNum_EQU(i);
		if(pSpend1->nId != nSkilllevel) 
			continue;
		LPCSTR nSkillMS	= pSpend1->Skill_Desc_Interface;
		state->PushString((char *)nSkillMS);
		break;
	}
	return 1;
}
//�õ�ѧϰ���ܵĵȼ�Ҫ��
INT Lua_GetStudySkillLevel(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (!(args[1].IsInteger()))
	{
		TDThrow("LUA: Lua_GetUplevelSkillNeedLevel Wrong Param");
	}

	if (!(args[2].IsInteger()))
	{
		TDThrow("LUA: Lua_GetUplevelSkillNeedLevel Wrong Param");
	}
	INT skillId = args[1].GetInteger();
	INT skillLvl = args[2].GetInteger();

	INT nSkilllevel = GetStudySkillLevelId(skillId,skillLvl);
	
	//���صȼ����ı�
	DBC_DEFINEHANDLE(pSpendDBC, DBC_SKILLDATA_V1_DEPLETE);

	INT mNum = (INT)pSpendDBC->GetRecordsNum();
	for(INT i=0; i<mNum; i++)
	{
		const _DBC_SKILLDATA_V1_DEPLETE* pSpend1 = (const _DBC_SKILLDATA_V1_DEPLETE*)pSpendDBC->Search_LineNum_EQU(i);
		if(pSpend1->nId != nSkilllevel) 
			continue;
		INT nNeedLevel	= pSpend1->nNeedLevel;
		state->PushInteger(nNeedLevel);
		break;
	}
	return 1;
}

//�����ķ�������Ҫ�Ľ�Ǯ,����Ĳ���ΪҪѧϰ���ķ��ȼ�
INT Lua_GetUplevelXinfaSpendMoney(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (!(args[1].IsInteger()))
	{
		TDThrow("LUA: Lua_GetUplevelXinfaSpendMoney Wrong Param");
	}

	if (!(args[2].IsInteger()))
	{
		TDThrow("LUA: Lua_GetUplevelXinfaSpendMoney Wrong Param");
	}

	INT nXinfaId = args[1].GetInteger();
	INT nXinfaLevel = args[2].GetInteger();

	//�����ķ��������ı�
	DBC_DEFINEHANDLE(pSpendDBC, DBC_XINFA_STUDYSPEND);
	
	INT nNum = (INT)pSpendDBC->GetRecordsNum();
	for(INT i=0; i<nNum; i++)
	{
		const _DBC_XINFA_STUDYSPEND* pSpend = (const _DBC_XINFA_STUDYSPEND*)pSpendDBC->Search_LineNum_EQU(i);
		if(pSpend->nStudyLevel != nXinfaLevel) 
			continue;
		INT nNeedMoney	= pSpend->StudySpend[nXinfaId].dwSpendMoney;

		//const _DBC_XINFA_STUDYSPEND* pSpend = (const _DBC_XINFA_STUDYSPEND*)pSpendDBC->Search_LineNum_EQU(i);
		//if(pSpend->nStudyLevel != nXinfaLevel) 
		//	continue;
		state->PushNumber( nNeedMoney );

		// ͬʱ���ת��Ϊ��ҡ����ҡ�ͭ�ҵĸ�ʽ���
		INT nMoney = nNeedMoney;
		INT nGoldCoin;	
		INT nSilverCoin;
		INT nCopperCoin;

		nCopperCoin = nMoney % 100;

		if( nMoney >= 100 )
		{
			nSilverCoin = ((nMoney-nCopperCoin)/100) % 100;
		}
		else
		{
			nSilverCoin = 0;
		}

		if ( nMoney >= 10000 )
		{
			nGoldCoin = (((nMoney-nCopperCoin)/100)-nSilverCoin)/100;
		}
		else
			nGoldCoin = 0;

		state->PushInteger(nGoldCoin);
		state->PushInteger(nSilverCoin);
		state->PushInteger(nCopperCoin);

		break;
	}
	return 4;
}
//�����ķ�������Ҫ�ľ���ֵ,����Ĳ���ΪҪѧϰ���ķ��ȼ�
INT Lua_GetUplevelXinfaSpendExp(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsInteger()))
	{
		TDThrow("LUA: Lua_GetUplevelXinfaSpendExp Wrong Param");
	}
	if (!(args[2].IsInteger()))
	{
		TDThrow("LUA: Lua_GetUplevelXinfaSpendExp Wrong Param");
	}
	INT nXinfaId = args[1].GetInteger();
	INT nXinfaLevel = args[2].GetInteger();

	//�����ķ��������ı�
	DBC_DEFINEHANDLE(pSpendDBC, DBC_XINFA_STUDYSPEND);
	INT nNum = (INT)pSpendDBC->GetRecordsNum();
	for(INT i=0; i<nNum; i++)
	{
		const _DBC_XINFA_STUDYSPEND* pSpend = (const _DBC_XINFA_STUDYSPEND*)pSpendDBC->Search_LineNum_EQU(i);
		if(pSpend->nStudyLevel != nXinfaLevel) 
			continue;
		INT nNeedExp	= pSpend->StudySpend[nXinfaId].dwSpendExperience;
		state->PushNumber( nNeedExp );
	}
	return 1;
}

//ѧϰ���ܽ���
INT lua_SkillsStudyFrame_study(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsInteger()))
	{
		TDThrow("LUA: lua_SkillsStudyFrame_study Wrong Param");
	}
	if (!(args[2].IsInteger()))
	{
		TDThrow("LUA: lua_SkillsStudyFrame_study Wrong Param");
	}
	if (!(args[3].IsInteger()))
	{
		TDThrow("LUA: lua_SkillsStudyFrame_study Wrong Param");
	}
	CDataPool::GetMe()->SendStudySkillEvent(args[1].GetInteger(),args[2].GetInteger(),args[3].GetInteger());

	return 0;
}

INT	Lua_GetActionNum(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsString()))
	{
		TDThrow("LUA: GetActionNum Wrong Param");
	}

	state->PushNumber( CActionSystem::GetMe()->GetActionNum(args[1].GetString()));

	return 1;
}

INT Lua_IsWindowShow(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsString()))
	{
		TDThrow("LUA: Lua_IsWindowShow Wrong Param");
	}
	BOOL ret = CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow(args[1].GetString());
	state->PushBoolean( ret != FALSE );

	return 1;
}
//�õ���������
INT	Lua_EnumAction(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsInteger()))
	{
		TDThrow("LUA: EnumAction Wrong Param1");
	}
	if (!(args[2].IsString()))
	{
		TDThrow("LUA: EnumAction Wrong Param2");
	}

	INT nIndex = args[1].GetInteger();
	const char* szFilter = args[2].GetString();

	tActionItem* pActionItem = CActionSystem::GetMe()->EnumAction(nIndex, szFilter);

	if(pActionItem) 
	{
		LuaObject objReturn = state->BoxPointer(pActionItem);
		objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
		objReturn.PushStack();
	}
	else
	{
		LuaObject objReturn = state->BoxPointer(&(CActionItem::s_InvalidAction));
		objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
		objReturn.PushStack();
	}

	return 1;
}

INT	Lua_ShowContexMenu(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsString()))
	{
		TDThrow("LUA:ShowContexMenu Wrong Param");
	}

	LPCTSTR szMenuName = args[1].GetString();

	POINT ptMouse = CInputSystem::GetMe()->MouseGetPos();

	std::vector< STRING > vParam;

	//arg0 - MenuNema
	vParam.push_back(szMenuName);

	vParam.push_back("-1");

	//arg2, arg3 MouseX, MouseY
	CHAR szTemp[32];

	_snprintf(szTemp, 32, "%d", ptMouse.x);
	vParam.push_back(szTemp);

	_snprintf(szTemp, 32, "%d", ptMouse.y);
	vParam.push_back(szTemp);

	CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);

	return 0;
}

INT Lua_QuestFrameOptionClicked(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsNumber()))
	{
		TDThrow("LUA:QuestFrameOptionClicked Wrong Param1");
	}

	if (!(args[2].IsNumber()))
	{
		TDThrow("LUA:QuestFrameOptionClicked Wrong Param2");
	}

	if (!(args[3].IsNumber()))
	{
		TDThrow("LUA:QuestFrameOptionClicked Wrong Param3");
	}

	INT nIndex = (INT)args[1].GetNumber();
	INT nExIndex1 = (INT)args[2].GetNumber();
	INT nExIndex2 = (INT)args[3].GetNumber();

	CUIDataPool::GetMe()->SendSelectEvent(nIndex,nExIndex1,nExIndex2);

	//CGameProcedure::s_pGfxSystem->PushDebugString("CGEventRequest");
	return 0;
}

//��������
INT Lua_QuestFrameAcceptClicked(LuaPlus::LuaState* state)
{
	CUIDataPool::GetMe()->SendAcceptEvent();
	return 0;
}
//�ܾ�����
INT Lua_QuestFrameRefuseClicked(LuaPlus::LuaState* state)
{
	CUIDataPool::GetMe()->SendRefuseEvent();
	return 0;
}
//��������
INT Lua_QuestFrameMissionContinue(LuaPlus::LuaState* state)
{
	CUIDataPool::GetMe()->SendContinueEvent();
	return 0;
}
//��������
INT Lua_QuestFrameMissionAbnegate(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if ( !(args[1].IsInteger()) ) return 0;
	CUIDataPool::GetMe()->SendAbandonEvent(args[1].GetInteger());
	return 0;
}
//�������
INT Lua_QuestFrameMissionComplete(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsNumber()))
	{
		TDThrow("LUA:QuestFrameMissionComplete Wrong Param");
	}

	INT nIndex = (INT)args[1].GetNumber();

	//Ϊ�˶�Ӧ��������1��ʼ���
	CUIDataPool::GetMe()->SendCompleteEvent( nIndex );
	return 0;
}


// �õ���ǰ�����óߴ�
INT Lua_GetSceneSize( LuaPlus::LuaState* state)
{
	const _DBC_SCENE_DEFINE* pSceneDef = CWorldManager::GetMe()->GetActiveScene()->GetSceneDefine();
	state->PushNumber( pSceneDef->nXSize );
	state->PushNumber( pSceneDef->nZSize );
	return 2;
}
INT Lua_GetSceneID( LuaPlus::LuaState* state)
{
	state->PushInteger( CWorldManager::GetMe()->GetActiveSceneID() );
	return 1;
}
// �õ���ǰС��ͼ�óߴ�
INT Lua_GetMinimapSize( LuaPlus::LuaState* state)
{

	state->PushNumber( 256 );
	state->PushNumber( 256 );
	return 2;
}
INT Lua_ResetCamera( LuaPlus::LuaState* state )
{
	//FLOAT fSub = max( FLOAT( nMousePosXDiff /10.0f ), 1.0f) / 100;
	float fCurrentDirect = float( CGameProcedure::s_pGfxSystem->Camera_GetDirection() );
	if( fCurrentDirect < 0 )
	{
		float ftmp = float( int( fCurrentDirect / PI_2 ) );
		ftmp *= PI_2;
		fCurrentDirect = fCurrentDirect - ftmp + PI_2;
	}
	else 
	{
		fCurrentDirect = fCurrentDirect - float( int( fCurrentDirect / PI_2 ) * PI_2 );
	}
	CGameProcedure::s_pGfxSystem->Camera_Direction( fCurrentDirect );
	if( fCurrentDirect < __PI  )
		CGamePro_Main::m_nResetCamera = 1;
	else 
		CGamePro_Main::m_nResetCamera = -1;
	return 0;
}

//Ҫ���/�رռ�����
INT Lua_ToggleComposeWnd(LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_TOGLE_COMPOSE_WINDOW);
	return 0;
}
//�õ�ĳһ���䷽������
INT Lua_GetMethodNumber( LuaPlus::LuaState* state)
{
	LuaStack args(state);
	// ��һ��ֵΪ�䷽���ͣ��ڶ���Ϊ�䷽���к�
	if (!(args[1].IsInteger()))
	{
		TDThrow("LUA: EnumAction Wrong Param");
	}

	INT nType = args[1].GetInteger();		//  �䷽����

	return 0;
}
//ö���䷽����
INT	Lua_EnumMethod(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	// ��һ��ֵΪ�䷽���ͣ��ڶ���Ϊ�䷽���к�
	if (!(args[1].IsInteger()))
	{
		//INT t1 = args[1].GetType();
		//INT t2 = args[1].GetType();
		TDThrow("LUA: EnumAction Wrong Param");
	}

	INT nType = args[1].GetInteger();		//  �䷽����
	INT nIndex = args[ 2 ].GetInteger();	//  �䷽���к�
	return 3;
}

//����������Ϣ 
/*
1. Ƶ��
2. �������ͣ�������˽�˻��Ǻ���
3. �����id,����ж���Ļ�
4. ���������
5. ����Ķ��󣬰ѻ�����˭��
*/
INT Lua_SendChatMessage( LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if( !args[ 1 ].IsString() ) return 0; // ��Ϣ�ַ��� 
	if( !args[ 2 ].IsString() ) return 0; // Ŀ�������ַ���
	if( !args[ 3 ].IsInteger() ) return 0; //Ƶ��id
	if( !args[ 4 ].IsInteger() ) return 0; // ��Ϣ����
	if( !args[ 5 ].IsInteger() ) return 0;	// �Ŷ�id

	CGameProcedure::s_pGameInterface->SendChatCommand( args[ 3 ].GetInteger(),
		args[ 4 ].GetInteger(),
		args[ 5 ].GetInteger(),
		args[ 1 ].GetString(),
		args[ 2 ].GetString() );
	return 0;
}

INT Lua_SendGMCommand(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if( !args[ 1 ].IsString() ) return 0; // ��Ϣ�ַ��� 

	CGameProcedure::s_pGameInterface->GM_SendCommand(args[1].GetString());
	return 0;
}

INT Lua_SendAUTOMove(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() ) return 0; // x����
	if( !args[ 2 ].IsInteger() ) return 0; // Z���� 
	CGameProcedure::s_pGameInterface->Player_MoveTo(fVector2((FLOAT)args[1].GetInteger(),(FLOAT)args[2].GetInteger()));
	return 0;
}

INT Lua_GetCurChatProperty( LuaPlus::LuaState* state )
{
	state->PushInteger( 0 );
	state->PushInteger( 0 );
	state->PushInteger( 0 );
	state->PushString( "" );
	return 4;
}

INT Lua_AxTrace(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	
	if(!(args[1].IsInteger()) || !(args[2].IsInteger()) || !(args[3].IsString()))
		return 0;

	AxTrace(args[1].GetInteger(), args[2].GetInteger(), "%s", args[3].GetString());

	return 0;
}

INT Lua_ComposeItem_Begin(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (! (args[1].IsInteger()) || !(args[2].IsInteger()) )
	{
		TDThrow("LUA: ComposeItem Wrong Param");

	}
	if( CObjectManager::GetMe()->GetMySelf()->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_ABILITY_ACTION )
		return 0;
	INT Make_Count = args[2].GetInteger();

	if(Make_Count < 1)
		return 0;

//	INT	nAbility		= args[1].GetInteger();
	INT	nPrescription	= args[1].GetInteger();
//	ObjID_t	n_guidPlatform	= args[3].GetInteger();
//	AxTrace(nAbility, nPrescription, "%s", n_guidPlatform);
	CGameProcedure::s_pGameInterface->Player_UseLifeAbility( nPrescription, Make_Count);

	return 0;
}

INT Lua_ComposeItem_Cancel(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	//if (! (args[1].IsInteger()) )
	//{
	//	TDThrow("LUA: ComposeItem_Cancel Wrong Param");
	//}
//	INT	nAbility		= args[1].GetInteger();
//	INT	nPrescription	= args[1].GetInteger();
//	ObjID_t	n_guidPlatform	= args[3].GetInteger();
//	AxTrace(nAbility, nPrescription, "%s", n_guidPlatform);

	//anyway��hide prograssbar first. 
	CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_HIDE);
	CObject_PlayerMySelf *pMySelf = CObjectManager::GetMe()->GetMySelf();
	// ��������ô����
	//SCommand_Object pCmd;
	//pCmd.m_wID = OC_IDLE;
	//pCmd.m_abParam[2] = TRUE;
	////when the cancel button is clicked, the logic of client enter idle first.
	////then send the idle command to server.
	////it is not the best measure,but i can't get the better way.
	//pMySelf->DoCommand(&pCmd);
	//����������ͷ�����Ϣ
	CGCharIdle msg;
	CNetManager::GetMe()->SendPacket( &msg );

	return 0;
}


INT Lua_OpenBooth( LuaPlus::LuaState* state) 
{
	//����ʹ��
	return 0;
}



INT Lua_CloseBooth( LuaPlus::LuaState* state)
{
	CGameProcedure::s_pGameInterface->Booth_Close();
	return 0;
}

INT Lua_RepairAll( LuaPlus::LuaState* state)
{
	//����Ƿ�����Ҫ�������Ʒ
	BOOL bNeed = FALSE;
	for(INT i = 0; i<HEQUIP_NUMBER; i++)
	{
		CObject_Item_Equip* pItem = (CObject_Item_Equip*)(CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)i));
		if(!pItem)
			continue;
		if(pItem->GetItemDur() < pItem->GetItemMaxDur())
		{
			bNeed = TRUE;
			break;
		}
	}

	//��Ҫ�����ʱ����֪ͨ���������޸�
	if(bNeed)
	{
		CGameProcedure::s_pGameInterface->Booth_Repair(true,0);
	}
	else
	{
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
		if(bLog)	
		{
			ADDNEWDEBUGMSG(STRING("û����Ʒ��Ҫ����"));
		}
	}

	return 0;
}

INT Lua_RepairOne( LuaPlus::LuaState* state)
{
	//�������Ϊ����״̬
	CGameProcedure::s_pGameInterface->Booth_EnterRepair();
	return 0;
}

//�򿪹رճƺŽ���
INT Lua_OpenSelfEquip( LuaPlus::LuaState* state) 
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() ) 
	{
		return 0;
	}

	CEventSystem::GetMe()->PushEvent(GE_OPEN_EQUIP, args[1].GetInteger());

	return 0;
}
//�رճƺŽ���
INT Lua_CloseTitleList( LuaPlus::LuaState* state) 
{
	CEventSystem::GetMe()->PushEvent(GE_CLOSE_AGNAME);

	return 0;
}


//�򿪳ƺŽ���
INT Lua_OpenTitleList( LuaPlus::LuaState* state) 
{
	CEventSystem::GetMe()->PushEvent(GE_OPEN_AGNAME);

	return 0;
}


INT Lua_GetCurrentSceneName( LuaPlus::LuaState* state )
{
	const _DBC_SCENE_DEFINE* pSceneDef = CWorldManager::GetMe()->GetActiveScene()->GetSceneDefine();
	state->PushString( pSceneDef->szName );
	return 1;
}

INT Lua_GetCurrentSceneNameById( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	int nIndex=args[1].GetInteger();

	DBC_DEFINEHANDLE(s_pSceneNameDBC, DBC_SCENE_DEFINE);
	const _DBC_SCENE_DEFINE* pSceneDef = (const _DBC_SCENE_DEFINE*)s_pSceneNameDBC->Search_Index_EQU(nIndex);
	state->PushString( pSceneDef->szName );
	return 1;
}


//����̵��������չ�����
INT Lua_GetShopType( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if( !args[1].IsString() )
	{
		TDThrow("LUA: Lua_GetShopType[1] param parameter error");
	}
	std::string str = args[1].GetString();

	if(str == "Repair")
	{
		state->PushInteger(CDataPool::GetMe()->Booth_GetRepairType());
		return 1;
	}
	else if( str == "buy" )
	{
		state->PushInteger(CDataPool::GetMe()->Booth_GetBuyType());
		return 1;
	}
	else if(str == "type")
	{
		state->PushInteger(CDataPool::GetMe()->Booth_GetShopType());
		return 1;
	}

	return 0;
}

INT Lua_GetOperationType( LuaPlus::LuaState* state )
{
	state->PushInteger(CDataPool::GetMe()->Booth_GetRepairType());
	return 1;
}

INT Lua_OpenCharacter( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() ) return 0;
	CEventSystem::GetMe()->PushEvent( GE_OPEN_CHARACTOR, args[ 1 ].GetInteger() );
	return 0;
}

INT Lua_OpenEquip( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() ) return 0;

	CEventSystem::GetMe()->PushEvent(GE_OPEN_EQUIP, args[1].GetInteger());

	return 0;
}

INT Lua_GetActionInfo( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() ) return 0;

	return 1;
}



//��������
INT Lua_AskLevelUp(LuaPlus::LuaState* state)
{
	CGameInterface::GetMe()->Player_AskLevelUp();
	return 0;
}

//����/�رչһ�
INT Lua_AutoHit(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() ) return 0;
	CGameInterface::GetMe()->Player_AUTOHit(args[ 1 ].GetInteger());
	return 0;
}

//---------------------------------------------------------------------------------------------------------
// ������.

// ��Ա��ӳ��Ĺ��ܲ˵�.
INT Lua_ShowTeamFuncMenu(LuaPlus::LuaState* state)	
{
	
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() )
	{
		return 0;
	}

	// ��¼��ǰѡ��Ķ���.
	CUIDataPool::GetMe()->m_iSelectedTeamMember = args[ 1 ].GetInteger();
	
	// �õ��Լ���guid
	INT MyGUID   = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();

	// �õ��ӳ���guid
	UINT LeaderId = CUIDataPool::GetMe()->GetTeamOrGroup()->GetLeaderGUID();

	//�õ����λ��
	POINT ptMouse = CInputSystem::GetMe()->MouseGetPos();
	CHAR szXPos[32]; _snprintf(szXPos, 32, "%d", ptMouse.x);
	CHAR szYPos[32]; _snprintf(szYPos, 32, "%d", ptMouse.y);
	CHAR szIndex[ 32 ]; 
	_snprintf(szIndex, 32, "%d", CUIDataPool::GetMe()->m_iSelectedTeamMember );
	std::vector< STRING > strParamInfo;
	if(MyGUID == LeaderId)
	{

		strParamInfo.push_back("Team_Leader");
		strParamInfo.push_back("-1");
		strParamInfo.push_back(szXPos);
		strParamInfo.push_back(szYPos);
		strParamInfo.push_back(szIndex);
		((CEventSystem*)CGameProcedure::s_pEventSystem)->PushEvent( GE_SHOW_CONTEXMENU, strParamInfo);
		//����Լ��Ƕӳ�. �򿪶ӳ�����.
		//CEventSystem::GetMe()->PushEvent( GE_SHOW_TEAM_FUNC_MENU_LEADER, args[ 1 ].GetInteger() );
		//CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, "Team_Leader");
	}
	else
	{
		strParamInfo.push_back("Team_Member");
		strParamInfo.push_back("-1");
		strParamInfo.push_back(szXPos);
		strParamInfo.push_back(szYPos);
		strParamInfo.push_back(szIndex);
		((CEventSystem*)CGameProcedure::s_pEventSystem)->PushEvent( GE_SHOW_CONTEXMENU, strParamInfo);
		// ����Լ��Ƕ�Ա, �򿪶�Ա����.
		//CEventSystem::GetMe()->PushEvent( GE_SHOW_TEAM_FUNC_MENU_MEMBER, args[ 1 ].GetInteger() );
		//CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, "Team_Member");//
	}

	return 0;
}

// ���õ�ǰѡ��Ķ�Ա
INT Lua_SetCurSelMember(LuaPlus::LuaState* state)	
{
	
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() )
	{
		return 0;
	}

	// ��¼��ǰѡ��Ķ���.

	CUIDataPool::GetMe()->m_iSelectedTeamMember = args[ 1 ].GetInteger();
	return 0;
}




// ��ʾ������Ϣ�Ի���.
INT Lua_ShowTeamInfoDlg(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() )
	{
		return 0;
	}

	int iOpenType = 0;

	// �õ��򿪶�����Ϣ������.
	// 0 : �򿪶�����Ϣ
	// 1 : ��������������
	// 2 : ����������������
	iOpenType = args[ 1 ].GetInteger();

	CEventSystem::GetMe()->PushEvent(GE_TEAM_OPEN_TEAMINFO_DLG, iOpenType);
	return 0;
}

// ��˸��ť.
INT Lua_FlashTeamButton(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() )
	{
		return 0;
	}

	int iOpenType = 0;

	// �õ��򿪶�����Ϣ������.
	// 0 : �򿪶�����Ϣ
	// 1 : ��������������
	// 2 : ����������������
	iOpenType = args[ 1 ].GetInteger();
	CEventSystem::GetMe()->PushEvent(GE_TEAM_NOTIFY_APPLY, iOpenType);
	return 0;
}

// ������.
//--------------------------------------------------------------------------------------------------------

// ����һ��ȫ�ֱ�������
INT Lua_SetGlobalInteger(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (! (args[1].IsNumber()) )
	{
		TDThrow("LUA: Lua_SetGlobalInteger Wrong Param");
	}

	if (! (args[2].IsString()) )
	{
		TDThrow("LUA: Lua_SetGlobalInteger Wrong Param");
	}

	int nNumber = args[1].GetInteger();
	std::string str = args[2].GetString();

	lua_pushnumber(state->GetCState(),nNumber);
	lua_setglobal(state->GetCState(),str.c_str());

	return 0;
}

// ���һ��ȫ�ֱ���
INT Lua_GetGlobalInteger(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (! (args[1].IsString()) )
	{
		TDThrow("LUA: Lua_GetGlobalInteger Wrong Param");
	}
	std::string str = args[1].GetString();
	
	lua_getglobal(state->GetCState(),str.c_str());
	INT nNumber = (INT)lua_tonumber(state->GetCState(),-1);

	state->PushNumber(nNumber);
	return 1;
}

// Ҫװ������ϸ��Ϣ
INT Lua_AskEquipDetial( LuaPlus::LuaState* state)
{
	//��ϸװ��(���������ݳ��д洢��Ʒ��ϸ����)
	if ( CObjectManager::GetMe()->GetMySelf() != NULL )
	{
		CGAskDetailEquipList msgAskMyDetailEquip;
		msgAskMyDetailEquip.setObjID(CObjectManager::GetMe()->GetMySelf()->GetServerID());
		msgAskMyDetailEquip.SetAskMode(ASK_EQUIP_ALL);
		CNetManager::GetMe()->SendPacket(&msgAskMyDetailEquip);
	}

	return 0;
}

// ������Ʒ
INT Lua_DiscardItem(LuaPlus::LuaState* state)
{
	//������Ʒ,���ݲ�ͬ����Ʒ��Դ����Ҫ���Ͳ�ͬ��������Ϣ
	INT nTypeOwner	=	CDataPool::GetMe()->DisCard_GetItemConta();
	INT	nItemPos	=	CDataPool::GetMe()->DisCard_GetItemPos();
	switch( nTypeOwner ) 
	{
	case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			if(!pItem) break;
			//��֤�Ƿ���Զ���
			if(!pItem->Rule(tObject_Item::RULE_DROP))
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_DROP).c_str());
				break;
			}

			CGDiscardItem msg;
			msg.SetOpt(CGDiscardItem::FromBag);
			msg.SetBagIndex(nItemPos);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		break;
	case tObject_Item::IO_MYSELF_EQUIP:					// ����������ϵ�װ��
		{
			break;
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

			if(!pItem) break;
			//��֤�Ƿ���Զ���
			if(!pItem->Rule(tObject_Item::RULE_DROP))
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_DROP).c_str());
				break;
			}

			CGDiscardEquip msg;
			msg.SetEquipPoint(nItemPos);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		break;
	case tObject_Item::IO_MYSELF_BANK:					// ��ҵ�����
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBank_GetItem(nItemPos);

			if(!pItem) break;
			//��֤�Ƿ���Զ���
			if(!pItem->Rule(tObject_Item::RULE_DROP))
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_DROP).c_str());
				break;
			}

			CGDiscardItem msg;
			msg.SetOpt(CGDiscardItem::FromBank);
			msg.SetBagIndex(nItemPos);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		break;
	default:
		break;
	}

	return 0;
}

// ȡ����Ʒ������״̬
INT Lua_DiscardItemCancelLocked( LuaPlus::LuaState* state)
{
	INT nTypeOwner	=	CDataPool::GetMe()->DisCard_GetItemConta();
	INT	nItemPos	=	CDataPool::GetMe()->DisCard_GetItemPos();

	switch( nTypeOwner ) 
	{
	case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
		}
		break;
	case tObject_Item::IO_MYSELF_EQUIP:					// ����������ϵ�װ��
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
		}
		break;
	case tObject_Item::IO_MYSELF_BANK:					// ��ҵ�����
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBank_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
		}
		break;
	default:
		break;
	}

	CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
	return 0;
}

// ��������
INT Lua_PlaySound(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (! (args[1].IsNumber()) )
	{
		TDThrow("LUA: Lua_SetGlobalInteger Wrong Param");
	}

	int nSoundIndex = args[1].GetInteger();
	CSoundSystemFMod::_PlayUISoundFunc(nSoundIndex);
	return 0;
}

INT	Lua_CloseAllWindow( LuaPlus::LuaState* state)
{
	CGameProcedure::s_pUISystem->CloseAllWindow();
	return 0;
}

// target���ڵ����Ĳ˵�
INT	Lua_OpenTargetMenu( LuaPlus::LuaState* state)
{
	INT iObjectId = 0;
	
	if(NULL == CObjectManager::GetMe()->GetMainTarget())
	{
		return 0;
	}

	iObjectId = CObjectManager::GetMe()->GetMainTarget()->GetServerID();
	CGameProcedure::s_pGameInterface->Object_ShowContexMenu(iObjectId);
	return 0;
}


INT Lua_GetMousePos( LuaPlus::LuaState* state)
{
	POINT ptMouse = CInputSystem::GetMe()->MouseGetPos();
	state->PushInteger( ptMouse.x );
	state->PushInteger( ptMouse.y );
	return 2;
}

// ��ʾϵͳ��ʾ�Ի���2006��3��27
INT Lua_ShowSystemTipInfo( LuaPlus::LuaState* state)
{
	
	LuaStack args(state);
	if (! (args[1].IsString()) )
	{
		TDThrow("LUA: Lua_ShowSystemTipInfo Wrong Param");
	}

	const char* pInfo =  args[1].GetString();
	CEventSystem::GetMe()->PushEvent(GE_OPEN_SYSTEM_TIP_INFO_DLG, pInfo);
	return 0;
}

// �򿪸��Ķ�������Ի���2006��3��27
INT Lua_OpenChangeMinorPasswordDlg( LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_MINORPASSWORD_OPEN_CHANGE_PASSWORD_DLG);
	return 0;
}

// �򿪽������롣2006��3��27
INT Lua_OpenUnLockeMinorPasswordDlg( LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_MINORPASSWORD_OPEN_UNLOCK_PASSWORD_DLG);
	return 0;
}

// ������������档2006��3��28
INT Lua_OpenSetMinorPasswordDlg( LuaPlus::LuaState* state)
{

	CEventSystem::GetMe()->PushEvent(GE_MINORPASSWORD_OPEN_SET_PASSWORD_DLG);
	return 0;

}


// �������롣2006��3��28
INT Lua_SendSetMinorPassword( LuaPlus::LuaState* state)
{

	LuaStack args(state);
	if (! (args[1].IsString()) )
	{
		TDThrow("LUA: Lua_SendSetMinorPassword Wrong Param");
	}

	const char* pPassword =  args[1].GetString();

	CGMinorPasswd Msg;
	CG_MINORPASSWD* pMinorPasswd = Msg.GetMinorPassword();
	pMinorPasswd->CleanUp();
	pMinorPasswd->m_Type = MREQT_SETPASSWD;
	pMinorPasswd->m_OnePasswd.CleanUp();
	pMinorPasswd->m_OnePasswd.SetPasswd( pPassword );

	CNetManager::GetMe()->SendPacket(&Msg);

	return 0;
}

// �޸����롣2006��3��28
INT Lua_SendModifyMinorPassword( LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (! (args[1].IsString()) )
	{
		TDThrow("LUA: Lua_SendModifyMinorPassword Wrong Param");
	}

	const char* pPassword =  args[1].GetString();

	CGMinorPasswd Msg;
	CG_MINORPASSWD* pMinorPasswd = Msg.GetMinorPassword();
	pMinorPasswd->CleanUp();
	pMinorPasswd->m_Type = MREQT_MODIFYPASSWD;
	pMinorPasswd->m_OnePasswd.CleanUp();
	pMinorPasswd->m_OnePasswd.SetPasswd( pPassword );

	CNetManager::GetMe()->SendPacket(&Msg);

	return 0;
}

// �������롣2006��3��28
INT Lua_UnLockMinorPassword( LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (! (args[1].IsString()) )
	{
		TDThrow("LUA: Lua_UnLockMinorPassword Wrong Param");
	}

	const char* pPassword =  args[1].GetString();

	CGMinorPasswd Msg;
	CG_MINORPASSWD* pMinorPasswd = Msg.GetMinorPassword();
	pMinorPasswd->CleanUp();
	pMinorPasswd->m_Type = MREQT_UNLOCKPASSWD;
	pMinorPasswd->m_OnePasswd.CleanUp();
	pMinorPasswd->m_OnePasswd.SetPasswd( pPassword );

	CNetManager::GetMe()->SendPacket(&Msg);

	return 0;
}

// ǿ�ƽ������롣2006��3��28
INT Lua_ForceUnLockMinorPassword( LuaPlus::LuaState* state)
{
	CGMinorPasswd Msg;
	CG_MINORPASSWD* pMinorPasswd = Msg.GetMinorPassword();
	pMinorPasswd->CleanUp();
	pMinorPasswd->m_Type = MREQT_DELETEPASSWD;
	pMinorPasswd->m_OnePasswd.CleanUp();

	CNetManager::GetMe()->SendPacket(&Msg);

	return 0;
}


// �޸����롣2006��3��28
INT Lua_ModifyMinorPassword( LuaPlus::LuaState* state)
{

	LuaStack args(state);
	if (! (args[1].IsString()) )
	{
		TDThrow("LUA: Lua_UnLockMinorPassword Wrong Param");
	}

	const char* pOldPassword =  args[1].GetString();

	if (! (args[2].IsString()) )
	{
		TDThrow("LUA: Lua_UnLockMinorPassword Wrong Param");
	}

	const char* pNewPassword =  args[2].GetString();

	CGMinorPasswd Msg;
	CG_MINORPASSWD* pMinorPasswd = Msg.GetMinorPassword();
	pMinorPasswd->CleanUp();
	pMinorPasswd->m_Type = MREQT_MODIFYPASSWD;
	pMinorPasswd->m_ModifyPasswd.CleanUp();

	// ����Ҫ�޸ĵ�����
	pMinorPasswd->m_ModifyPasswd.SetOldPasswd(pOldPassword);
	pMinorPasswd->m_ModifyPasswd.SetNewPasswd(pNewPassword);

	CNetManager::GetMe()->SendPacket(&Msg);

	return 0;
}

INT Lua_GetCurrentTime( LuaPlus::LuaState* state)
{
	SYSTEMTIME systime;
	::GetLocalTime( &systime );
	
	state->PushInteger( int( systime.wHour ) );
	state->PushInteger( int( systime.wMinute ) );
	state->PushInteger( int( systime.wMilliseconds ) );
	return 3;
}

INT Lua_GetCurMousePos( LuaPlus::LuaState* state)
{
	POINT ptMouse = CGameProcedure::s_pInputSystem->MouseGetPos();

	state->PushInteger(ptMouse.x);
	state->PushInteger(ptMouse.y);

	return 2;
}

//// �������֡����������ֵ�ID 2006��3��28
//INT Lua_PlayMusic( LuaPlus::LuaState* state)
//{
//	LuaStack args(state);
//	if (! (args[1].IsString()) )
//	{
//		TDThrow("LUA: Lua_PlayMusic Wrong Param");
//	}
//
//	INT iId =  args[1].GetInteger();
//
//
//}
//
//
//// ֹͣ���֡����������ֵľ�� 2006��3��28
//INT Lua_StopMusic( LuaPlus::LuaState* state)
//{
//
//}//


}
