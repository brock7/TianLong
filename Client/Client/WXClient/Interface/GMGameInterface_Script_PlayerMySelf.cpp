#include "stdafx.h"
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
#include "CGTeamInvite.h"								// ������������Ϣ				
#include "CGTeamRetInvite.h"							// ������������Ϣ����
#include "CGTeamRetApply.h"								// �ش�ĳ��������������Ϣ
#include "CGTeamLeaderRetInvite.h"						// �ش��Ա������������Ϣ
#include "CGReqManualAttr.h"							// �����ֶ���ӵ���.
#include "CGAskTeamFollow.h"
#include "CGReturnTeamFollow.h"
#include "CGStopTeamFollow.h"

#include "CGPlayerDieResult.h"
#include "..\..\Common\database\tlbb_dbc.h"
#include "..\..\Common\BuffImpactMgr.h"
#include "TDDBC_Struct.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "CGAskLeanAbility.h"
#include "..\DBC\GMDataBase.h"

namespace SCRIPT_SANDBOX
{
	

	//===============================================================
	PlayerMySelf PlayerMySelf::s_MySelf;

	// �õ���ҵ�����
	INT	PlayerMySelf::GetName(LuaPlus::LuaState* state)
	{
		state->PushString( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name());
		return 1;
	}
	// �õ���ɫ���ϵ�buff������
	//INT PlayerMySelf::GetBuffNumber( LuaPlus::LuaState* state )
	//{
	//	return 1;
	//}
	//INT PlayerMySelf::GetBuffByIndex( LuaPlus::LuaState* state )
	//{
	//	return 1;
	//}
	//�õ���ɫ���ϵ�buff������
	INT     PlayerMySelf::GetBuffNumber( LuaPlus::LuaState* state )
	{
		CDataPool *pDataPool = CDataPool::GetMe();
		state->PushNumber(pDataPool->BuffImpact_GetCount());
		return 1;
	}

	INT		PlayerMySelf::GetBuffIconNameByIndex( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		INT nIndex = args[2].GetInteger();
		CDataPool *pDataPool = CDataPool::GetMe();
		const _BUFF_IMPACT_INFO *pBuffImpactInfo = pDataPool->BuffImpact_GetByIndex( nIndex );
		if ( pBuffImpactInfo != NULL )
		{
			_BUFF_IMPACT *pBuffImpact = GetBuffImpactMgr()->GetBuffImpact( pBuffImpactInfo->m_nBuffID );
			if ( pBuffImpact != NULL )
			{
				if ( strlen( pBuffImpact->m_szIconName ) > 0 )
				{
					state->PushString(pBuffImpact->m_szIconName);
					return 1;
				}
				else
				{
					Assert( strlen( pBuffImpact->m_szIconName ) > 0 && "PlayerMySelf::GetBuffIconNameByIndex" );
				}
			}
			else
			{
				Assert( pBuffImpact != NULL > 0 && "PlayerMySelf::GetBuffIconNameByIndex" );
			}
		}
		else
		{
			Assert( pBuffImpactInfo != NULL > 0 && "PlayerMySelf::GetBuffIconNameByIndex" );
		}
		state->PushString("");
		return 1;
	}

	INT		PlayerMySelf::GetBuffToolTipsByIndex( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffToolTipsByIndex param parameter error");
		}

		INT nIndex = args[2].GetInteger();
		CDataPool *pDataPool = CDataPool::GetMe();
		const _BUFF_IMPACT_INFO *pBuffImpactInfo = pDataPool->BuffImpact_GetByIndex( nIndex );
		if ( pBuffImpactInfo != NULL )
		{
			_BUFF_IMPACT *pBuffImpact = GetBuffImpactMgr()->GetBuffImpact( pBuffImpactInfo->m_nBuffID );
			if ( pBuffImpact != NULL )
			{
				if ( strlen( pBuffImpact->m_pszInfo ) > 0 )
				{
					state->PushString( pBuffImpact->m_pszInfo );
					return 1;
				}
				else
				{
					Assert( strlen( pBuffImpact->m_pszInfo ) > 0 && "PlayerMySelf::GetBuffToolTipsByIndex" );
				}
			}
			else
			{
				Assert( pBuffImpact != NULL > 0 && "PlayerMySelf::GetBuffToolTipsByIndex" );
			}
		}
		else
		{
			Assert( pBuffImpactInfo != NULL > 0 && "PlayerMySelf::GetBuffToolTipsByIndex" );
		}
		state->PushString("");
		return 1;
	}
	INT		PlayerMySelf::DispelBuffByIndex( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:DispelBuffByIndex param parameter error");
		}

		INT nIndex = args[2].GetInteger();
		CDataPool *pDataPool = CDataPool::GetMe();
		pDataPool->BuffImpact_DispelByIndex( nIndex );
		return 1;
	}
	INT		PlayerMySelf::GetBuffTimeTextByIndex( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffTimeTextByIndex param parameter error");
		}

		INT nIndex = args[2].GetInteger();
		CDataPool *pDataPool = CDataPool::GetMe();
		INT nTime = pDataPool->BuffImpact_GetTimeByIndex( nIndex );
		state->PushInteger(nTime/1000);
		return 1;
	}
	INT		PlayerMySelf::SendReliveMessage_OutGhost( LuaPlus::LuaState* state )
	{
		CGPlayerDieResult msgDieResult;
		msgDieResult.SetResultCode( DIE_RESULT_CODE_OUT_GHOST );
		CNetManager::GetMe()->SendPacket(&msgDieResult);
		return 1;
	}

	INT		PlayerMySelf::SendReliveMessage_Relive( LuaPlus::LuaState* state )
	{
		CGPlayerDieResult msgDieResult;
		msgDieResult.SetResultCode( DIE_RESULT_CODE_RELIVE );
		CNetManager::GetMe()->SendPacket(&msgDieResult);
		return 1;
	}

	INT		PlayerMySelf::GetData( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if( !args[ 2 ].IsString() ) return 0;

		CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();

		std::string  str = args[ 2 ].GetString();
		if( str == "LEVEL" )	//�ȼ�
		{
			state->PushInteger(pCharData->Get_Level());
			return 1;
		}
		else if( str == "HP" )					//����ֵ
		{
			state->PushInteger(pCharData->Get_HP());
			return 1;
		}
		else if( str == "MP" )//ħ��ֵ
		{
			state->PushInteger(pCharData->Get_MP());
			return 1;
		}
		else if( str == "EXP" )					//����
		{
			if(pCharData->Get_Exp() < 0)
			{
				state->PushInteger(0);
			}
			else
			{
				state->PushInteger(pCharData->Get_Exp());
			}
			return 1;
		}
		else if( str == "NEEDEXP" )				//��������
		{
			state->PushInteger(pCharData->Get_MaxExp());
			return 1;
		}
		else if( str == "STRIKEPOINT" )				//������
		{
			state->PushInteger(pCharData->Get_StrikePoint());
			return 1;
		}
		else if( str == "MONEY" )				//����
		{
			state->PushInteger(pCharData->Get_Money());

			//ͬʱ���ת��Ϊ������ͭ�ҵ���ֵ
			INT nMoney = pCharData->Get_Money();

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
			return 4;
		}
		else if( str == "RMB" )			//����Ԫ������
		{
			state->PushInteger(pCharData->Get_RMB());
			return 1;
		}
		else if( str == "STR" )					//���� ����
		{
			state->PushInteger(pCharData->Get_STR());
			return 1;
		}
		else if( str == "SPR" )					//���� ����
		{
			state->PushInteger(pCharData->Get_SPR());
			return 1;
		}
		else if( str == "CON" )					//���� ����
		{
			state->PushInteger(pCharData->Get_CON());
			return 1;
		}
		else if( str == "INT" )					//���� ����
		{
			state->PushInteger(pCharData->Get_INT());
			return 1;
		}								   
		else if( str == "DEX" )					//�� ����
		{
			state->PushInteger(pCharData->Get_DEX());
			return 1;
		}								   
		else if( str == "POINT_REMAIN" )			//ʣ�����
		{
			state->PushInteger(pCharData->Get_PointRemain());
			return 1;
		}								   
		else if( str == "ATT_PHYSICS" )			//��������
		{
			state->PushInteger(pCharData->Get_AttPhysics());
			return 1;
		}								   
		else if( str == "DEF_PHYSICS" )			//���������
		{
			state->PushInteger(pCharData->Get_DefPhysics());
			return 1;
		}
		else if( str == "ATT_MAGIC" )			//ħ��������
		{
			state->PushInteger(pCharData->Get_AttMagic());
			return 1;
		}								   
		else if( str == "DEF_MAGIC" )			//ħ��������
		{
			state->PushInteger(pCharData->Get_DefMagic());
			return 1;
		}								   
		else if( str == "MAXHP" )				//�������ֵ
		{
			state->PushInteger(pCharData->Get_MaxHP());
			return 1;
		}								   
		else if( str == "MAXMP" )				//���ħ��ֵ
		{			
			state->PushInteger(pCharData->Get_MaxMP());
			return 1;
		}								   
		else if( str == "RAGE" )				//ŭ��
		{			
			state->PushInteger(pCharData->Get_Rage());
			return 1;
		}								   
		else if( str == "MAXRAGE" )				//���ŭ��
		{			
			state->PushInteger(BASE_MAX_RAGE);
			return 1;
		}								   
		else if( str == "HP_RESPEED" )			//HP�ָ��ٶ�  ��/��
		{			
			state->PushInteger(pCharData->Get_HPRespeed());
			return 1;
		}								   
		else if( str == "MP_RESPEED" )			//MP�ָ��ٶ�  ��/��
		{
			state->PushInteger(pCharData->Get_MPRespeed());
			return 1;
		}								   
		else if( str == "HIT" )					//������
		{
			state->PushInteger(pCharData->Get_Hit());
			return 1;
		}								   
		else if( str == "MISS" )					//������
		{
			state->PushInteger(pCharData->Get_Miss());
			return 1;
		}								   
		else if( str == "CRITRATE" )				//������
		{
			state->PushInteger(pCharData->Get_CritRate());
			return 1;								   
		}								   
		else if( str == "MOVESPEED" )			//�ƶ��ٶ�
		{
			state->PushNumber(pCharData->Get_MoveSpeed());
			return 1;								   
		}								   
		else if( str == "ATTACKSPEED" )			//�����ٶ�
		{
			state->PushInteger(pCharData->Get_AttackSpeed());
			return 1;
		}
		else if( str == "ATTACKCOLD" )			//������
		{
			state->PushInteger(pCharData->Get_AttCold());
			return 1;								   
		}								   
		else if( str == "DEFENCECOLD" )			//������
		{
			state->PushInteger(pCharData->Get_DefCold());
			return 1;								   
		}								   
		else if( str == "ATTACKFIRE" )			//�𹥻�
		{
			state->PushInteger(pCharData->Get_AttFire());
			return 1;								   
		}								   
		else if( str == "DEFENCEFIRE" )			//�����
		{
			state->PushInteger(pCharData->Get_DefFire());
			return 1;								   
		}								   
		else if( str == "ATTACKLIGHT" )			//�繥��
		{
			state->PushInteger(pCharData->Get_AttLight());
			return 1;								   
		}								   
		else if( str == "DEFENCELIGHT" )			//�����
		{
			state->PushInteger(pCharData->Get_DefLight());
			return 1;								   
		}								   
		else if( str == "ATTACKPOISON" )			//������
		{
			state->PushInteger(pCharData->Get_AttPosion());
			return 1;								   
		}								   
		else if( str == "DEFENCEPOISON" )		//������
		{
			state->PushInteger(pCharData->Get_DefPosion());
			return 1;								   
		}								   
		else if( str == "MEMPAI" )				//����
		{
			state->PushInteger(pCharData->Get_MenPai());
			return 1;								   
		}								   
		else if( str == "CAMP" )					//��Ӫ  fujia 2007.12.14
		{
			state->PushInteger(pCharData->Get_CampData()->m_nCampID);
			return 1;
		}
		//else if( str == "VIGOR")
		//{
		//	state->PushInteger(pCharData->Get_Vigor());
		//	return 1;
		//}
		//else if( str == "MAXVIGOR")	
		//{
		//	state->PushInteger(pCharData->Get_MaxVigor());
		//	return 1;
		//}
		else if( str == "GUILD")
		{
			state->PushInteger(pCharData->Get_Guild());
		}
		//else if( str == "ENERGY")
		//{
		//	state->PushInteger(pCharData->Get_Energy());
		//	return 1;
		//}
		//else if( str == "MAXENERGY")
		//{
		//	state->PushInteger(pCharData->Get_MaxEnergy());
		//	return 1;
		//}
		else if( str == "PORTRAIT" )			//����ͷ��
		{
			
			// �õ���Һ��Լ���ͷ����Ϣ
			LPCSTR pImageName = NULL;
			DBC_DEFINEHANDLE(s_pFaceImageDBC, DBC_CHARACTER_FACE)
			//�����ﶨ���
			const _DBC_CHAR_FACE* pFaceImage = NULL;
			pFaceImage = (const _DBC_CHAR_FACE*)(s_pFaceImageDBC->Search_Index_EQU(pCharData->Get_PortraitID()));
				
			if(pFaceImage)
			{
				pImageName = pFaceImage->pImageSetName;
			}
			//else // Ĭ��״̬����ʾͷ��ͼ�꣬ ͷ��ͼ����ʾ��
			//{
			//	pFaceImage = (const _DBC_CHAR_FACE*)(s_pFaceImageDBC->Search_LineNum_EQU(0));
			//	pImageName = pFaceImage->pImageSetName;
			//}//
					
			if(pImageName)
			{
				state->PushString(pImageName);
			}
			else
			{
				state->PushString(_T(""));
			}

			return 1;
		}
		

		state->PushInteger(-1);
		return 1;
	}

	//�����ҵ�ĳ�����Ϣ
	INT PlayerMySelf::GetSkillInfo( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !args[2].IsInteger() ) return 0;

		if( !args[3].IsString() ) return 0;

		INT	nSkillID	 = args[2].GetInteger();
		std::string  str = args[3].GetString();

		const SCLIENT_SKILL* pSkill = 
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill(nSkillID);

		if(str == "name")			// ����
		{
			if(pSkill)
				state->PushString(pSkill->m_pDefine->m_lpszName);
			else
				state->PushString("");
		}
		else if(str == "learn")		// �Ƿ�ѧ��
		{
			if(pSkill)
				state->PushBoolean(pSkill->m_bLeaned == TRUE);
			else
				state->PushBoolean(false);
		}
		else if(str == "iconname")	// ͼ���ļ���
		{
			if(pSkill)
				state->PushString(pSkill->m_pDefine->m_lpszIconName);
			else
				state->PushString("");
		}
		else if(str == "passivity")	// �Ƿ��Ǳ�������
		{	
			if(pSkill)
				state->PushInteger(pSkill->m_pDefine->m_nPassiveFlag);
		}
		else if(str == "explain")	// ��ϸ����
		{	
			if(pSkill)
				state->PushString(pSkill->m_pDefine->m_pszDesc);
			else
				state->PushString("");
		}
		else if(str == "skilldata")	// ��������
		{	
			if(pSkill)
				state->PushString(pSkill->GetSkillDesc_Interface());
			else
				state->PushString("");
		}
		else if(str == "level")	// ���ܵȼ�
		{	
			
			if(pSkill)
			{
				state->PushInteger(pSkill->m_nLevel);
			}
		}
		return 1;
	}

	//�����ҵ�ĳ���ķ���Ϣ
	INT PlayerMySelf::GetSkillClassInfo( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !args[2].IsInteger() ) return 0;

		if( !args[3].IsString() ) return 0;

		INT	nXinfaID	 = args[2].GetInteger();
		std::string  str = args[3].GetString();

		const SCLIENT_SKILLCLASS* pSkillClass = 
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_SkillClass(nXinfaID);

		if(str == "name")			// ����
		{
			state->PushString(pSkillClass->m_pDefine->pszName);
		}
		//else if(str == "level")		// �ȼ�
		//{
		//	state->PushInteger(pSkillClass->m_nLevel);
		//}
		else if(str == "iconname")	// ͼ���ļ���
		{
			state->PushString(pSkillClass->m_pDefine->pszIconName);
		}
		else if(str == "explain")	// ��ϸ����
		{	
			state->PushString(pSkillClass->m_pDefine->pszDesc);
		}
		return 1;
	}

	//�����ҵ�ĳ���������Ϣ
	INT PlayerMySelf::GetAbilityInfo( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !args[2].IsInteger() ) return 0;

		if( !args[3].IsString() ) return 0;


		INT	nAbilityID	 = args[2].GetInteger();
		std::string  str = args[3].GetString();

		const SCLIENT_LIFEABILITY* pLifeAbility = 
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(nAbilityID);

		if(str == "name")			// ����
		{
			state->PushString(pLifeAbility->m_pDefine->szName);
		}
		else if(str == "level")		// �ȼ�
		{
			state->PushInteger(pLifeAbility->m_nLevel);
		}
		else if(str == "maxlevel")	// �ȼ�����
		{
			state->PushInteger(pLifeAbility->m_pDefine->nLevelMax);
		}
		else if(str == "skillexp")	// ������
		{
			state->PushInteger(pLifeAbility->m_nExp);
		}
		else if(str == "iconname")	// ͼ���ļ���
		{
			state->PushString(pLifeAbility->m_pDefine->szIconName);
		}
		else if(str == "explain")	// ��ϸ����
		{	
			state->PushString(pLifeAbility->m_pDefine->szExplain);
		}
		else if(str == "popup")	// �Ƿ񵯳��¼�����
		{	// 1.�����ϳɽ��棬2.������Ƕ���棬-1.��������
			state->PushInteger(pLifeAbility->m_pDefine->nPopup);
		}
		return 1;
	}

	// ��ɫ����ѧϰ�����
	INT PlayerMySelf::AskLeanAbility(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:PlayerMySelf param parameter error");
		}

		INT nAbilityID = args[2].GetInteger();

		// ������������
		const SCLIENT_LIFEABILITY* pLifeAbility = 
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(nAbilityID);

		if(!pLifeAbility) return 0;

		// �ñ����������ж��Ƿ�������������
		CObject_PlayerMySelf *pMyself = CObjectManager::GetMe()->GetMySelf();
		if(!pMyself) return 0;
		
		CCharacterData* pData = pMyself->GetCharacterData();
		if(!pData) return 0;

		if( pData->Get_Exp() < CDataPool::GetMe()->StudyAbility_GetNeedExp() )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����ֵ����");
			return 0;
		}
		if( pData->Get_Money() < CDataPool::GetMe()->StudyAbility_GetNeedMoney() )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "��Ǯ����" );
			return 0;
		}
		if( pLifeAbility->m_nExp < CDataPool::GetMe()->StudyAbility_GetSkillExp() )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���������Ȳ���" );
			return 0;
		}
		//����ȼ��Ƿ�����
		if( CDataPool::GetMe()->StudyAbility_GetLevelLimit() > pData->Get_Level() )
		{
			char szText[_MAX_PATH];
			_snprintf( szText, _MAX_PATH, "����ȼ��������뵽%d���Ժ�����", CDataPool::GetMe()->StudyAbility_GetLevelLimit() );
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, szText );
			return 0;
		}

		//�������������������
		CGAskLeanAbility Msg;
		Msg.SetAbilityTeacherInfo(nAbilityID);
		Msg.SetNeedMoney(CDataPool::GetMe()->StudyAbility_GetNeedMoney());
		Msg.SetNeedExpy(CDataPool::GetMe()->StudyAbility_GetNeedExp());
		Msg.SetNeedSkillExpy(CDataPool::GetMe()->StudyAbility_GetSkillExp());

		CNetManager::GetMe()->SendPacket(&Msg);
		return 0;
	}


	INT PlayerMySelf::GetPos( LuaPlus::LuaState* state)
	{
		
		CObject_PlayerMySelf* pCharObj = CObjectManager::GetMe()->GetMySelf();
		if( pCharObj )
		{
			const _DBC_SCENE_DEFINE* pSceneDef = CWorldManager::GetMe()->GetActiveScene()->GetSceneDefine();
			FLOAT x = pCharObj->GetPosition().x * 512 / pSceneDef->nXSize;
			FLOAT y = pCharObj->GetPosition().z * 512 / pSceneDef->nZSize;
			state->PushNumber( x );
			state->PushNumber( y );
			state->PushInteger( INT( pCharObj->GetFaceDir() * 180.f / TDU_PI ) );
		}
		else
		{
			state->PushNumber( 0 );
			state->PushNumber( 0 );
			state->PushInteger( 0 );
		}
		return 3;
	}

	INT PlayerMySelf::IsPresent(LuaPlus::LuaState* state)
	{
		if( CObjectManager::GetMe()->GetMySelf() )
		{
			state->PushBoolean(true);
		}
		else
		{
			state->PushBoolean(FALSE);
		}
		return 1;
	}
	
	//�ƺ����
	INT PlayerMySelf::GetAgnameNum(LuaPlus::LuaState* state)
	{
		INT	AgnameNum = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_TitleNum();
		Assert(AgnameNum<=MAX_TITLE_SIZE);
		state->PushInteger(AgnameNum);
		return 1;
	}

	INT PlayerMySelf::EnumAgname(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: EnumAgname Wrong Param1");
		}
		if (!(args[3].IsString()))
		{
			TDThrow("LUA: EnumAgname Wrong Param2");
		}

		INT nIndex = args[2].GetInteger();
		const char* szFilter = args[3].GetString();

		//
		_TITLE_ Title = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Title(nIndex);
		if(Title.bFlag == _TITLE_::INVALID_TITLE )
		{
			state->PushString("�п�");
		}
		else if(Title.bFlag == _TITLE_::ID_TITLE)
		{
			INT TitleTBIndex = Title.ID;
			if(TitleTBIndex>0)
			{
				DBC_DEFINEHANDLE(pTitleDBC, DBC_TITLE_NAME);
				const _DBC_TITLE_NAME* pTitle = (const _DBC_TITLE_NAME*)pTitleDBC->Search_LineNum_EQU(TitleTBIndex);
				if(strncmp(szFilter, "name", 4) == 0)
					state->PushString( pTitle->szTitle);
				else
					state->PushString( pTitle->szTitleDesc);
			}
		}
		else
		{
			if(strncmp(szFilter, "name", 4) == 0)
				state->PushString( Title.szTitleData );
			else
				state->PushString( " " );

		}
	
		return 1;
	}

	INT PlayerMySelf::GetCurrentAgname(LuaPlus::LuaState* state)
	{
		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitleType() == _TITLE::MOOD_TITLE)
		{
			state->PushString("");
			return 1;
		}
		const char* szCurTitle = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitle();
		/*if(strcmp(szCurTitle, "") != 0)
		{*/
			state->PushString(szCurTitle);
		/*}*/
		return 1;
		//INT TitleID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitle();
		//state->PushInteger(TitleID);

	/*	_TITLE_	Title = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Title(TitleID);

		if(Title.bFlag == _TITLE_::INVALID_TITLE )
		{
			state->PushString("�п�");
		}
		else if(Title.bFlag == _TITLE_::ID_TITLE)
		{
			INT TitleTBIndex = Title.ID;
			if(TitleTBIndex>0)
			{
				DBC_DEFINEHANDLE(pTitleDBC, DBC_TITLE_NAME);
				const _DBC_TITLE_NAME* pTitle = (const _DBC_TITLE_NAME*)pTitleDBC->Search_LineNum_EQU(TitleTBIndex);
				state->PushString( pTitle->szTitle);
			}
		}
		else
		{
			state->PushString( Title.szTitleData );
		}*/
	}

	INT PlayerMySelf::AskChangeCurrentAgname(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: AskChangeCurrentAgname Wrong Param1");
		}
		INT nIndex = args[2].GetInteger();
		
		_TITLE_ Title = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Title(nIndex);
	
		if(Title.bFlag == _TITLE_::INVALID_TITLE )
		{
			CGameInterface::GetMe()->Title_UpdateCurTitle(Title.bType, 0);
		}
		else if(Title.bFlag == _TITLE_::ID_TITLE)
		{
			INT TitleTBIndex = Title.ID;
			if(TitleTBIndex>0)
			{
				DBC_DEFINEHANDLE(pTitleDBC, DBC_TITLE_NAME);
				const _DBC_TITLE_NAME* pTitle = (const _DBC_TITLE_NAME*)pTitleDBC->Search_LineNum_EQU(TitleTBIndex);
				CGameInterface::GetMe()->Title_UpdateCurTitle(Title.bType, Title.ID);
			}
		}
		else
		{
			CGameInterface::GetMe()->Title_UpdateCurTitle( Title.bType, 0 );
		}
		
		return 0;
	}


	
	INT PlayerMySelf::SetNullAgname(LuaPlus::LuaState* state)
	{
		_TITLE_ Title;
		CGameInterface::GetMe()->Title_UpdateCurTitle(_TITLE::NO_TITLE, 0);
		return 0;
	}


	// �����ֶ��������Ե��� 
	INT PlayerMySelf::SendAskManualAttr(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: SendAskManualAttr[2] Wrong Param1");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA: SendAskManualAttr[3] Wrong Param1");
		}

		if (!(args[4].IsInteger()))
		{
			TDThrow("LUA: SendAskManualAttr[4] Wrong Param1");
		}

		if (!(args[5].IsInteger()))
		{
			TDThrow("LUA: SendAskManualAttr[5] Wrong Param1");
		}

		if (!(args[6].IsInteger()))
		{
			TDThrow("LUA: SendAskManualAttr[6] Wrong Param1");
		}


		CGReqManualAttr msg;
		
		int iStr = args[2].GetInteger();
		int iSpr = args[3].GetInteger();
		int iCon = args[4].GetInteger();
		int iInt = args[5].GetInteger();
		int iDex = args[6].GetInteger();

		if((0 == iStr)&&
			( 0 == iSpr)&&
			( 0 == iCon)&&
			( 0 == iInt)&&
			( 0 == iDex))
		{
			return 0;
		}

		msg.SetStr(iStr);
		msg.SetSpr(iSpr);
		msg.SetCon(iCon);
		msg.SetInt(iInt);
		msg.SetDex(iDex);

		CNetManager::GetMe()->SendPacket(&msg);
	
		return 0;
	}
	
	//----------------------------------------------------------------------------------------------------------
	// ������
	//
	
	//----
	// ͬ��������(����������)
	INT	PlayerMySelf::SendAgreeJoinTeam(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: AskChangeCurrentAgname Wrong Param1");
		}
		int iTeamIndex = args[2].GetInteger();

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGTeamRetInvite msg;
		
			//msg.SetGUID( CUIDataPool::GetMe()->GetTeamInvitorGUID());
			// �޸�֮���һ������
			GUID_t invitorGUID = CUIDataPool::GetMe()->GetTeamInvitorGUID(iTeamIndex);
			if(INVALID_GUID == invitorGUID)
			{
				return 0; 
			}
			msg.SetGUID(invitorGUID);

			msg.SetReturn(1);
			CNetManager::GetMe()->SendPacket(&msg);

			// ͬ�����һ�������, ��յ������Ķ���.
			CUIDataPool::GetMe()->ClearAllInviteTeam();
		}
		return 0;
	}

	// �ܾ��������(����������)
	INT PlayerMySelf::SendRejectJoinTeam(LuaPlus::LuaState* state)
	{
		//if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		//{
		//	CGTeamRetInvite msg;
		//
		//	msg.SetGUID( CUIDataPool::GetMe()->GetTeamInvitorGUID());
		//	msg.SetReturn(0);
		//	CNetManager::GetMe()->SendPacket(&msg);

		//	// ɾ��һ���ܾ��Ķ���.
		//	CUIDataPool::GetMe()->EraseCurRejectTeam();
		//}
		//return 0;//

		// �õ���������
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: AskChangeCurrentAgname Wrong Param1");
		}
		int iTeamIndex = args[2].GetInteger();


		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGTeamRetInvite msg;
		
			GUID_t invitorGUID = CUIDataPool::GetMe()->GetTeamInvitorGUID(iTeamIndex);
			if(INVALID_GUID == invitorGUID)
			{
				return 0; 
			}
			msg.SetGUID(invitorGUID);
			msg.SetReturn(0);
			CNetManager::GetMe()->SendPacket(&msg);

			// ɾ��һ���ܾ��Ķ���.
			CUIDataPool::GetMe()->EraseCurRejectTeam();
		}
		return 0;//

	}

		
	
	//-----
	// ͬ��������(��������������)
	INT	PlayerMySelf::SendAgreeJoinTeam_Apply(LuaPlus::LuaState* state)
	{
		//if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		//{
		//	CGTeamRetApply msg;
		//
		//	msg.SetSourGUID( CUIDataPool::GetMe()->GetSourceGUID());

		//	msg.SetReturn(1);
		//	CNetManager::GetMe()->SendPacket(&msg);
		//}
		//return 0;//

		
		// �õ���������
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: SendAgreeJoinTeam_Apply[2] Wrong Param1");
		}
		int iApplyIndex = args[2].GetInteger();

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGTeamRetApply msg;
			GUID_t id = CUIDataPool::GetMe()->GetApplySourceGUID(iApplyIndex);
		
			if(INVALID_GUID == id)
			{
				return 0;
			}
			msg.SetSourGUID(id);
			msg.SetReturn(1);
			CNetManager::GetMe()->SendPacket(&msg);
		}
		return 0;//

	}

	// �ܾ��������(��������������)
	INT PlayerMySelf::SendRejectJoinTeam_Apply(LuaPlus::LuaState* state)
	{
		//if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		//{
		//	CGTeamRetApply msg;
		//
		//	msg.SetSourGUID( CUIDataPool::GetMe()->GetSourceGUID());
		//	msg.SetReturn(0);
		//	CNetManager::GetMe()->SendPacket(&msg);
		//}
		//return 0;//

		
		// �õ���������
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: SendRejectJoinTeam_Apply[2] Wrong Param1");
		}
		int iApplyIndex = args[2].GetInteger();

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGTeamRetApply msg;
			GUID_t id = CUIDataPool::GetMe()->GetApplySourceGUID(iApplyIndex);
			if(INVALID_GUID == id)
			{
				return 0;
			}

			msg.SetSourGUID(id);
			msg.SetReturn(0);
			CNetManager::GetMe()->SendPacket(&msg);
		}
		return 0;//
	}



	//-----
	// ͬ��������(��Ա����ĳ�˼������)
	INT	PlayerMySelf::SendAgreeJoinTeam_TeamMemberInvite(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGTeamLeaderRetInvite msg;

			// �õ���������
			LuaStack args(state);
			if (!(args[2].IsInteger()))
			{
				TDThrow("LUA: SendRejectJoinTeam_Apply[2] Wrong Param1");
			}
			int iIndex = args[2].GetInteger();

			GUID_t SId;
			GUID_t DId;
			if(CUIDataPool::GetMe()->GetMemberInviteGUID(iIndex, SId, DId))
			{
				msg.SetSourGUID( SId );
				msg.SetDestGUID( DId );
				msg.SetReturn(1);
				CNetManager::GetMe()->SendPacket(&msg);
			}
		}
		return 0;
	}

	// �ܾ��������(��Ա����ĳ�˼������)
	INT PlayerMySelf::SendRejectJoinTeam_TeamMemberInvite(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGTeamLeaderRetInvite msg;
		
			
			// �õ���������
			LuaStack args(state);
			if (!(args[2].IsInteger()))
			{
				TDThrow("LUA: SendRejectJoinTeam_Apply[2] Wrong Param1");
			}
			int iIndex = args[2].GetInteger();

			GUID_t SId;
			GUID_t DId;
			if(CUIDataPool::GetMe()->GetMemberInviteGUID(iIndex, SId, DId))
			{
				msg.SetSourGUID( SId );
				msg.SetDestGUID( DId );
				msg.SetReturn(0);
				CNetManager::GetMe()->SendPacket(&msg);
			}
		}
		return 0;
	}

	// ͬ�������Ӹ���ģʽ
	INT PlayerMySelf::SendAgreeTeamFollow(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGReturnTeamFollow Msg;

			Msg.SetReturn( TRUE );
			CNetManager::GetMe()->SendPacket(&Msg);
		}
		return 0;
	}

	// �ܾ�������Ӹ���ģʽ
	INT PlayerMySelf::SendRefuseTeamFollow(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "��ܾ�������Ӹ���ģʽ��" );

			CGReturnTeamFollow Msg;
			Msg.SetReturn( FALSE );
			CNetManager::GetMe()->SendPacket(&Msg);
		}
		return 0;
	}




	
	// �뿪����
	INT PlayerMySelf::LeaveTeam(LuaPlus::LuaState* state)					
	{

		if(CUIDataPool::GetMe()->GetTeamOrGroup())
		{
			if(CUIDataPool::GetMe()->GetTeamOrGroup()->GetTeamMemberCount())
			{
				CUIDataPool::GetMe()->LeaveTeam();
			}
		}
		return 0;
	}

	// ��ɢ����
	INT PlayerMySelf::DismissTeam(LuaPlus::LuaState* state)					
	{
		if(CUIDataPool::GetMe()->GetTeamOrGroup())
		{
			if(CUIDataPool::GetMe()->GetTeamOrGroup()->GetTeamMemberCount())
			{
				CUIDataPool::GetMe()->DismissTeam();
				SCRIPT_SANDBOX::Talk::s_Talk.TeamDestory();
			}
		}
		return 0;
	}

	// ɾ����ǰѡ�ж�Ա
	INT PlayerMySelf::KickTeamMember(LuaPlus::LuaState* state)			
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:KickTeamMember");
		}	

		INT iIndex = args[2].GetInteger();

		if(CUIDataPool::GetMe()->GetTeamOrGroup())
		{
			if(CUIDataPool::GetMe()->GetTeamOrGroup()->GetTeamMemberCount())
			{
				CUIDataPool::GetMe()->KickTeamMember(iIndex);
			}
		}
		return 0;
	}

	
	// ɾ����ǰѡ�ж�Ա
	INT PlayerMySelf::KickTeamMemberByIndex(LuaPlus::LuaState* state)			
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:KickTeamMemberByIndex");
		}	

		INT iIndex = args[2].GetInteger();
		
		if(CUIDataPool::GetMe()->GetTeamOrGroup())
		{
			if(CUIDataPool::GetMe()->GetTeamOrGroup()->GetTeamMemberCount())
			{

				CUIDataPool::GetMe()->KickTeamMember(iIndex);
			}
		}

		return 0;
	}


	//�Խ�����
	INT	PlayerMySelf::CreateTeamSelf(LuaPlus::LuaState* state)
	{
		if ( CUIDataPool::GetMe()->GetTeamOrGroup()->HasTeam() )
		{
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "���Ѿ��ڶ������ˣ������Խ����顣" );
			return 0;
		}

		CGTeamInvite msg;
		msg.SetSourObjID(CObjectManager::GetMe()->GetMySelf()->GetServerID());
		msg.SetDestName(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name());
		CNetManager::GetMe()->SendPacket(&msg);

		return 0;
	}

		
	//�����ӳ���Ϣ
	INT	PlayerMySelf::AppointLeader(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:AppointLeader");
		}	

		INT iIndex = args[2].GetInteger();

		if(CUIDataPool::GetMe()->GetTeamOrGroup())
		{
			if(CUIDataPool::GetMe()->GetTeamOrGroup()->GetTeamMemberCount())
			{

				CUIDataPool::GetMe()->AppointLeader(iIndex);
			}
		}
		return 0;

	}

	// �Ƿ�����Ӹ���״̬
	INT	PlayerMySelf::InTeamFollowMode(LuaPlus::LuaState* state)
	{
		CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();

		BOOL bMode = pCharData->Get_TeamFollowFlag();

		if( bMode )
		{
			state->PushBoolean(true);
		}
		else
		{
			state->PushBoolean(false);
		}

		return 1;
	}

	// ȡ������
	INT	PlayerMySelf::StopFollow(LuaPlus::LuaState* state)
	{
		CGStopTeamFollow Msg;
		CNetManager::GetMe()->SendPacket(&Msg);
		return 0;
	}

	// ��Ӹ���
	INT PlayerMySelf::TeamFrame_AskTeamFollow(LuaPlus::LuaState* state)
	{
		// ������ڰ�̯״̬��������������
		if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ڰ�̯����");
			return 0;
		}

		CGAskTeamFollow Msg;
		CNetManager::GetMe()->SendPacket(&Msg);
		return 0;
	}


	// �õ���Աguid
	INT PlayerMySelf::GetTeamMemberGUID(LuaPlus::LuaState* state)
	{

		// �õ���������
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: Send Add Friend Team");
		}
		int iIndex = args[2].GetInteger();

		GUID_t MyGUID;
		GUID_t SelGUID;
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			const TeamMemberInfo* pInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMemberByIndex(iIndex);

			if(pInfo)
			{
				MyGUID  = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();
				SelGUID = pInfo->m_GUID;

				if(MyGUID != SelGUID)
				{
					state->PushInteger(SelGUID);
					return 1;
				}
				
			}
				
		}

		state->PushInteger(-1);
		return 1;
	}

	// ������
	//----------------------------------------------------------------------------------------------------------


	
	// �õ���ҵĳƺ�
	INT	PlayerMySelf::GetCurTitle(LuaPlus::LuaState* state)
	{
		//���鲻�ܳ����ڳƺŵ�λ��
		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitleType() == _TITLE::MOOD_TITLE)
		{
			state->PushString("");
			return 1;
		}
		state->PushString( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitle());
		return 1;
	}
	
	// �Ҽ��˵����
	INT PlayerMySelf::ShowMySelfContexMenu(LuaPlus::LuaState* state)
	{
		INT myId = CObjectManager::GetMe()->GetMySelf()->GetServerID();
		CGameInterface::GetMe()->Object_ShowContexMenu(myId, TRUE);
		return 0;
	}
	
	// ���ѡ���Լ�
	INT	PlayerMySelf::SelectMyselfAsTarget(LuaPlus::LuaState* state)
	{
		INT myId = CObjectManager::GetMe()->GetMySelf()->GetServerID();
		CGameInterface::GetMe()->Object_SelectAsMainTarget(myId, 1);
		return 0;
	}

	// ���ö������� 2006-4-29
	INT	PlayerMySelf::SetSupperPassword(LuaPlus::LuaState* state)
	{
		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsMinorPwdSetup(FALSE))
		{
			// ������ù����룬 ���޸����롣
			CEventSystem::GetMe()->PushEvent(GE_MINORPASSWORD_OPEN_CHANGE_PASSWORD_DLG);
		}
		else
		{
			// ������ù����룬 ���������롣
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsMinorPwdSetup();
		}
		return 0;
	}
}