//-----------------------------------------------------------------------------
// �ļ��� : LuaFnTbl_Mission.h
// ģ��	:	Script
// ����	 :	������ص�������
// �޸���ʷ:
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_MISSION_H__
#define __LUAFNTBL_MISSION_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "GCScriptCommand.h"
#include "SceneManager.h"
#include "ItemHelper.h"
#include "ItemManager.h"
#include "HumanItemLogic.h"
#include "MonsterManager.h"
#include "Obj_Monster.h"
#include "GCMissionResult.h"
#include "GCUICommand.h"
#include "GameTable.h"
#include "LogDefine.h"
#include "TimeManager.h"
#include "GCChat.h"

namespace LuaFnTbl
{
	enum
	{
		EA_Number,
		EA_State,
		EA_Index,
		EA_Count
	};

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnBeginEvent
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*				��EndEvent���ʹ��
	*				Ϊ��һ������ز�����׼��
	*/
	INT LuaFnBeginEvent(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnBeginEvent")
		pScene->mMisBuf.ResetMisBuf();

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0;
	}

	
	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddText
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				msg
	*					��ʾ����
	*     ����˵�� :
	*				����ַ�����ʾ
	*/
	INT LuaFnAddText(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);		
		const CHAR* msg = Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnAddText")
		//size_t len = min(_MisBuf::MISSIONCHARBUFLEN -1,strlen(msg));	
		//strncpy(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf,msg,len);
		Q_strncpyz(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf,msg,_MisBuf::MISSIONCHARBUFLEN);//�Ż�
		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_Str;		
		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	INT LuaFnAddSkillStudy(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);		
		INT SkillID = Lua_ValueToNumber(L,2);
		INT SkillLevel = Lua_ValueToNumber(L,3);
		INT Skilltype = Lua_ValueToNumber(L,4);
		BEGINSCENEDEFINE("LuaFnAddSkillStudy")

		if (pScene->mMisBuf.mUseNum >= _MisBuf::MISSIONBUFLEN) return 0;

		if (pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].GetBufUse() + (INT)(sizeof(INT)*3) >= pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].GetBufMaxSize())
		{
			pScene->mMisBuf.mUseNum++;
			if (pScene->mMisBuf.mUseNum >= _MisBuf::MISSIONBUFLEN) return 0;
		}

		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].AddBuf((VOID*)&SkillID, sizeof(INT));
		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].AddBuf((VOID*)&SkillLevel, sizeof(INT));
		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].AddBuf((VOID*)&Skilltype, sizeof(INT));
		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].mType = _MisBuf::_MisItemBuf::BufType_Skill;

		return 0;
		ENDSCENEDEFINE

			LUA_LEAVE_FUNCTION
			return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddNumber
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				id
	*					��Ŀ,INT
	*     ����˵�� :
	*				�����Ŀ��ʾ
	*/
	INT LuaFnAddNumber(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		INT id = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnAddNumber")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf);
		*pNumber = id;
		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_Int;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddNumText
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				id
	*					�ڼ�����ʾ,INT
	*				msg
	*					��ʾ����
	*				state 
	*					�Ƿ���Խ�����ͽ�����
	*				index
	*					�߻���������
	*     ����˵�� :
	*				�����Ŀ���ַ�����ʾ
	*				state��index���Բ���
	*/
	INT LuaFnAddNumText(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		INT id = Lua_ValueToNumber(L,2);
		const CHAR* msg = Lua_ValueToString(L,3);
		INT state;
		INT index;
		
		//
		if(Lua_GetTopIndex(L) < 4)
			state = -1;//���Խӵ�����
		else
			state = Lua_ValueToNumber(L,4);
		
		if(Lua_GetTopIndex(L) < 5)
			index = -1;//���Խӵ�����
		else
			index = Lua_ValueToNumber(L,5);

		BEGINSCENEDEFINE("LuaFnAddNumText")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_Number * sizeof(INT));
		*pNumber = id;
		INT* pState  = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_State * sizeof(INT));
		*pState = state;
		INT* pIndex  = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_Index * sizeof(INT));
		*pIndex = index;
		
		//size_t len = min(_MisBuf::MISSIONCHARBUFLEN -1,strlen(msg));	
		//strncpy(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + sizeof(INT),msg,strlen(msg));
		Q_strncpyz(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_Count * sizeof(INT),msg,_MisBuf::MISSIONCHARBUFLEN - EA_Count * sizeof(INT));
		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_IntStr;	

		return 0;
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-12-9
	*     �������� : LuaFnSetNumText
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				number
	*					�����õ�ֵ,short
	*     ����˵�� :
	*				���ýű�����������
	*/
	INT LuaFnSetNumText(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		INT number = Lua_ValueToNumber(L,1);
		lua_setglobal(L,"global_numtext");

		return 0;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-12-9
	*     �������� : LuaFnGetNumText
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				number
	*					�����õ�ֵ,short
	*     ����˵�� :
	*				ȡ���ű�����������
	*/
	INT LuaFnGetNumText(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		lua_getglobal(L,"global_numtext");
		INT number = Lua_ValueToNumber(L,1);
		Lua_PushNumber(L,number);
		return 1;

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddMoneyBonus
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				count
	*					������Ǯ��Ŀ
	*     ����˵�� :
	*				���Money
	*/
	INT LuaFnAddMoneyBonus(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		INT count = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnAddMoneyBonus")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf);
		*pNumber = count;
		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_Money;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddItemBonus
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				itemTypeSn
	*					������Ʒ��ItemType��9λ���
	*				count
	*					������Ŀ
	*     ����˵�� :
	*				���count�����ΪitemType����
	*/
	INT LuaFnAddItemBonus(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
			
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		INT itemTypeSn = Lua_ValueToNumber(L,2);
		INT count = Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnAddItemBonus")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf);
		*pNumber++ = itemTypeSn;
		*pNumber = count;

		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_Item;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddItemDemand
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				itemTypeSn
	*					������Ʒ��ItemType��9λ���
	*				count
	*					������Ŀ

	*     ����˵�� :
	*				����count�����ΪitemType����
	*/
	INT LuaFnAddItemDemand(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
			
		SceneID_t sceneId = Lua_ValueToNumber(L,1);;
		INT itemTypeSn = Lua_ValueToNumber(L,2);
		INT count = Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnAddItemDemand")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf);
		
		*pNumber++ = itemTypeSn ;
		*pNumber = count;

		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_Item;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddRandItemBonus
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*					��ʾ�����������
	*/
	INT LuaFnAddRandItemBonus(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
			
		BEGINSCENEDEFINE("LuaFnAddRandItemBonus")

		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_RandItem;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddRadioItemBonus
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				itemTypeSn
	*					��Ʒ��ItemType9λ���
	*				count
	*					������ѡһ��Ʒ����Ŀ
	*     ����˵�� :
	*				��Ӷ�ѡһ����
	*/
	INT LuaFnAddRadioItemBonus(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		INT itemTypeSn = Lua_ValueToNumber(L,2);
		INT count = Lua_ValueToNumber(L,3);
	
		BEGINSCENEDEFINE("LuaFnAddRadioItemBonus")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf);

		*pNumber++ = itemTypeSn;
		*pNumber = count;

		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_RadioItem;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnEndEvent
	*     ����˵�� : 
	*     ����˵�� :
	*				��BeginEvent���ʹ��
	*				֮�������ĳ��Dispatch***����
	*/
	INT LuaFnEndEvent(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
			return 0;

		LUA_LEAVE_FUNCTION
	}
	
	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDispatchEventList
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*     ����˵�� :
	*				��ͻ��˷��ظ�Npc�ɴ����¼��б�
	*				�ɴ����¼��ɸ�npc�Ĺҵ��¼�OnEnumerate�����ж�
	*				������������������,ͨ��AddNumText���
	*				BeginEvent
	*				AddText
	*				����: AddNumText
	*				EndEvent
	*/
	INT LuaFnDispatchEventList(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		ObjID_t targetId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDispatchEventList")
		ScriptParam_EventList paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;

		for(INT i= 0;i < pScene->mMisBuf.mUseNum;i++)
		{
			if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Str)
			{
				ScriptEventItem itemEvent;
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf;
				itemEvent.SetText( &scriptStr);
				paramEventList.AddItem( &itemEvent );
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Int)
			{
				//ScriptString scriptStr;
				//scriptStr = pScene->mMisBuf.Buff[i].buf;
				//INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				//ScriptEventItem itemEvent;
				//itemEvent.SetScriptID(*number,&scriptStr);
				//paramEventList.AddItem( &itemEvent );
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_IntStr)
			{
				INT number = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_Number * sizeof(INT));
				INT state = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_State * sizeof(INT));
				INT index = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_Index * sizeof(INT));
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf + EA_Count * sizeof(INT);
				ScriptEventItem itemEvent;
				itemEvent.SetScriptID(index,state,number,&scriptStr);
				paramEventList.AddItem( &itemEvent );
			}

		}

		Packets::GCScriptCommand Msg;
		Msg.SetEventListResponseCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;	
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDispatchRegieEventList
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*     ����˵�� :
	*				��ͻ��˷��ظ�Npc�ɴ����¼��б�
	*				�ɴ����¼��ɸ�npc�Ĺҵ��¼�OnEnumerate�����ж�
	*				������������������,ͨ��AddNumText���
	*				BeginEvent
	*				AddText
	*				����: AddNumText
	*				EndEvent
	*/
	INT LuaFnDispatchRegieEventList(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		ObjID_t targetId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDispatchRegieEventList")
		ScriptParam_EventList paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;

		for(INT i= 0;i < pScene->mMisBuf.mUseNum;i++)
		{
			if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Str)
			{
				ScriptEventItem itemEvent;
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf;
				itemEvent.SetText( &scriptStr);
				paramEventList.AddItem( &itemEvent );
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Int)
			{
				//ScriptString scriptStr;
				//scriptStr = pScene->mMisBuf.Buff[i].buf;
				//INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				//ScriptEventItem itemEvent;
				//itemEvent.SetScriptID(*number,&scriptStr);
				//paramEventList.AddItem( &itemEvent );
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_IntStr)
			{
				INT number = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_Number * sizeof(INT));
				INT state = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_State * sizeof(INT));
				INT index = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_Index * sizeof(INT));
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf + EA_Count * sizeof(INT);
				ScriptEventItem itemEvent;
				itemEvent.SetScriptID(index,state,number,&scriptStr);
				paramEventList.AddItem( &itemEvent );
			}

		}

		Packets::GCScriptCommand Msg;
		Msg.SetMissionRegieCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;	
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	INT LuaFnDispatchSkillStudyList(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		ObjID_t targetId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDispatchSkillStudyList")
		ScriptParam_SkillStudy paramEventList;
		paramEventList.Reset();
		paramEventList.m_yStudyCount = 0;
		INT nSkillCount = 0;
		for(INT i = 0; i < pScene->mMisBuf.mUseNum+1; i++)
		{
			if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Skill)
			{
				INT t_BufUse = 0;
				INT *pSkill = NULL;
				INT nBufUseSize = pScene->mMisBuf.Buff[i].GetBufUse()/12;
				for (INT j = 0; j <= nBufUseSize; j++)
				{
					pSkill = (INT*)(pScene->mMisBuf.Buff[i].buf+t_BufUse);
					paramEventList.m_aSkillItem[nSkillCount].nSkillID = *pSkill;
					t_BufUse += sizeof(INT);
					pSkill = (INT*)(pScene->mMisBuf.Buff[i].buf+t_BufUse);
					paramEventList.m_aSkillItem[nSkillCount].nLevel = *pSkill;
					t_BufUse += sizeof(INT);
					pSkill = (INT*)(pScene->mMisBuf.Buff[i].buf+t_BufUse);
					paramEventList.m_aSkillItem[nSkillCount].SkillType = *pSkill;
					t_BufUse += sizeof(INT);
					nSkillCount++;
					if ( nSkillCount >=MAX_SKILL_ITEM_COUNT )
					{
						break;
					}
				}
				if ( nSkillCount >=MAX_SKILL_ITEM_COUNT )
				{
					break;
				}
			}
		}
		paramEventList.m_yStudyCount = nSkillCount;
		paramEventList.m_nReserve = targetId;
		Packets::GCScriptCommand Msg;
		Msg.SetSkillStudyCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;	
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDispatchMissionInfo
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				scriptId
	*					�ű����
	*				misId
	*					������
	*     ����˵�� :
	*				��ͻ��˷���������Ϣ����
	*				
	*/
	INT LuaFnDispatchMissionInfo(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		ObjID_t targetId = Lua_ValueToNumber(L,3);
		ScriptID_t scriptId = Lua_ValueToNumber(L,4);
		MissionID_t misId = Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnDispatchMissionInfo")
		ScriptParam_MissionInfo paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;
		paramEventList.m_idMission = misId;
		paramEventList.m_idScript = scriptId;

		for(INT i= 0;i < pScene->mMisBuf.mUseNum;i++)
		{
			if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Str)
			{
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf;
				paramEventList.AddText( &scriptStr);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Item)
			{
				SMissionBonus bonus;

				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetItem(number[0],number[1]);
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_RandItem)
			{
				SMissionBonus bonus;
				bonus.SetItemRand();
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_RadioItem)
			{
				SMissionBonus bonus;
				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetItemRadio(number[0],number[1]);
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Money)
			{
				SMissionBonus bonus;
				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetMoney(*number);
				paramEventList.AddBonus(&bonus);
			}
		}

		Packets::GCScriptCommand Msg;
		Msg.SetMissionInfoResponseCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDispatchMissionDemandInfo
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				scriptId
	*					�ű����
	*				misId
	*					������
	*				done
	*					�Ƿ����
	*     ����˵�� :
	*				��ͻ��˷�������������
	*				done���ΪTRUE,�ͻ��˽���������������ɰ�ť
	*				֮ǰ��ʹ�õĺ�������Ϊ
	*				BeginEvent
	*				AddText,�������ַ���
	*				AddItemDemand,������Ʒ
	*				EndEvent
	*/
	INT LuaFnDispatchMissionDemandInfo(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		ObjID_t targetId = Lua_ValueToNumber(L,3);
		ScriptID_t scriptId = Lua_ValueToNumber(L,4);
		MissionID_t misId = Lua_ValueToNumber(L,5);
		INT done = (Lua_ValueToNumber(L,6));

		BEGINHUMANDEFINE("LuaFnDispatchMissionDemandInfo")
		ScriptParam_MissionDemandInfo paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;
		paramEventList.m_idScript = scriptId;
		paramEventList.m_idMission = misId;
		paramEventList.m_bDone = done;

		for(INT i= 0;i < pScene->mMisBuf.mUseNum;i++)
		{
			if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Str)
			{
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf;
				paramEventList.AddText( &scriptStr);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Item)
			{
				SMissionDemandItem demandItem;

				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				demandItem.m_uItemID = number[0];
				demandItem.m_yCount = (BYTE)(number[1]);
				paramEventList.AddDemandItem(&demandItem);
			}
		}


		Packets::GCScriptCommand Msg;
		Msg.SetMissionDemandInfoResponseCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDispatchMissionContinueInfo
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				scriptId
	*					�ű����
	*				misId
	*					������
	*     ����˵�� :
	*				��ͻ��˷��������������
	*/
	INT LuaFnDispatchMissionContinueInfo(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
			
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		ObjID_t targetId = Lua_ValueToNumber(L,3);
		ScriptID_t scriptId = Lua_ValueToNumber(L,4);
		MissionID_t misId = Lua_ValueToNumber(L,5);
		
		BEGINHUMANDEFINE("LuaFnDispatchMissionContinueInfo")
		ScriptParam_MissionContinueInfo paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;
		paramEventList.m_idScript = scriptId;
		paramEventList.m_idMission = misId;

		for(INT i= 0;i < pScene->mMisBuf.mUseNum;i++)
		{
			if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Str)
			{
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf;
				paramEventList.AddText( &scriptStr);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Item)
			{
				SMissionBonus bonus;

				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetItem(number[0],number[1]);
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_RandItem)
			{
				SMissionBonus bonus;
				bonus.SetItemRand();
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_RadioItem)
			{
				SMissionBonus bonus;
				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetItemRadio(number[0],number[1]);
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Money)
			{
				SMissionBonus bonus;
				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetMoney(*number);
				paramEventList.AddBonus(&bonus);
			}
		}

		Packets::GCScriptCommand Msg;
		Msg.SetMissionContinueInfoResponseCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDispatchMissionTips
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��ͻ��˷������������ʾ��Ϣ
	*				
	*/
	INT LuaFnDispatchMissionTips(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnDispatchMissionTips")
		ScriptParam_MissionTips paramMissionTips;
		paramMissionTips.Reset();
		if(pScene->mMisBuf.Buff[0].mType == _MisBuf::_MisItemBuf::BufType_Str)
		{
			paramMissionTips.m_strText = pScene->mMisBuf.Buff[0].buf;
		}

		Packets::GCScriptCommand Msg;
		Msg.SetMissionTipsCmd( &paramMissionTips );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;	
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-28
	*     �������� : LuaFnDispatchMissionResult
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��ͻ��˷���������֤���
	*				
	*/
	INT LuaFnDispatchMissionResult(Lua_State* L)
	{

		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber(L,1);
		ObjID_t selfId		= Lua_ValueToNumber(L,2);
		UINT IsFinished	= Lua_ValueToNumber(L,3);
		BEGINHUMANDEFINE("LuaFnDispatchMissionResult")

		Packets::GCMissionResult Msg;
		Msg.setIsFinished(IsFinished);
		pHuman->GetPlayer()->SendPacket(&Msg);

		return 0;	

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnMissionCom
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*     ����˵�� :
	*				�������
	*				֮����DelMission
	*/
	INT LuaFnMissionCom(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);	
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		MissionID_t misId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnMissionCom")
		pHuman->SetMissionHaveDone(misId,TRUE);

		return 0;
		ENDHUMANDEFINE
		
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMission
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*     ����˵�� :
	*				��õ�ǰ���ĳһ����ŵ�ֵ	//����,������,δ��
	*/
	INT LuaFnGetMission(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);	
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		MissionID_t misId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetMission")
		Lua_PushNumber(L,pHuman->IsHaveMission(misId));
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMissionIndexByID
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*     ����˵�� :
	*				���õ�ǰ���ĳһ����ŵ�ֵ
	*/
	INT LuaFnGetMissionIndexByID(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		MissionID_t misId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetMissionIndexByID")
		Lua_PushNumber(L,pHuman->GetMissionIndexByID(misId));
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION	
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMissionParam
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misIndex
	*					����λ������
	*				paramIndex
	*					�������ֶα��
	*     ����˵�� :
	*				 ȡ�������
	*/
	INT LuaFnGetMissionParam(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT misIndex = Lua_ValueToNumber(L,3);
		INT paramIndex = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnGetMissionParam")
		if( misIndex>=0 )
		{
			Lua_PushNumber( L, pHuman->GetMissionParam( misIndex, paramIndex ) );
			return 1;
		}
		else
		{
			Lua_PushNumber( L, 0 );
			return 1;
		}
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetMissionByIndex
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*				paramIndex
	*					�������ֶα��
	*				value
	*					��paramIndex�ֶε�ֵ
	*     ����˵�� :
	*				 �����������
	*/
	INT LuaFnSetMissionByIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
			
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT misIndex = Lua_ValueToNumber(L,3);
		INT paramIndex = Lua_ValueToNumber(L,4);
		INT value= Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnSetMissionByIndex")
		if( misIndex>=0 )
		{
			pHuman->SetMissionParam(misIndex,paramIndex,value);
			return 0;
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2006-1-18
	*     �������� : LuaFnGetMissionCount
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				 �õ��ѽ����������
	*				 û������ʱ����0
	*/
	INT LuaFnGetMissionCount(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnGetMissionCount")
			Lua_PushNumber(L,pHuman->GetConstMissionList()->m_Count);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddMission
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*				scriptId
	*					���ýű����
	*				killObjEvent
	*					�Ƿ�ص�OnKillObject
	*				enterAreaEvent
	*					�Ƿ�ص�OnEnterArea
	*				itemChangeEvent
	*					�Ƿ�ص�OnItemChange
	*     ����˵�� :
	*				������������misId��ŵ�����
	*				�ɹ�����1
	*/
	INT LuaFnAddMission(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		MissionID_t misId = Lua_ValueToNumber(L,3);
		ScriptID_t scriptId = Lua_ValueToNumber(L,4);
		BOOL killObjEvent = Lua_ValueToNumber(L,5) > 0 ? 1 : 0;
		BOOL enterAreaEvent = Lua_ValueToNumber(L,6)> 0 ? 1 : 0;
		BOOL itemChangedEvent = Lua_ValueToNumber(L,7)> 0 ? 1 : 0;

		BEGINHUMANDEFINE("LuaFnAddMission")
		INT ret = pHuman->AddMission( misId, scriptId, killObjEvent, enterAreaEvent, itemChangedEvent );
		ret = ( (ret==OR_OK) ? TRUE : FALSE ) ;

		Lua_PushNumber( L, ret );
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-13
	*     �������� : LuaFnAddMissionEx
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*				scriptId
	*					���ýű����
	*     ����˵�� :
	*				������������misId��ŵ�����
	*				�ɹ�����1
	*/
	INT LuaFnAddMissionEx(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		MissionID_t misId = Lua_ValueToNumber(L,3);
		ScriptID_t scriptId = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnAddMissionEx")
		INT ret = pHuman->AddMissionEx( misId, scriptId );
		ret = ( (ret==OR_OK) ? TRUE : FALSE ) ;

		Lua_PushNumber( L, ret );
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-13
	*     �������� : LuaFnSetMissionEvent
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*				eventId
	*					���ýű���ţ�0 KillObj, 1 EnterArea, 2 ItemChanged, 3 PetChanged
	*     ����˵�� :
	*				�޸��������misId��ŵ�������Ӧ�¼�
	*				�ɹ�����1
	*/
	INT LuaFnSetMissionEvent(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		MissionID_t misId = Lua_ValueToNumber(L,3);
		INT eventId = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnSetMissionEvent")
		INT ret = pHuman->SetMissionEvent( misId, eventId );
		ret = ( (ret==OR_OK) ? TRUE : FALSE ) ;

		Lua_PushNumber( L, ret );
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDelMission
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*     ����˵�� :
	*				ɾ������
	*/
	INT LuaFnDelMission(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		MissionID_t misId = Lua_ValueToNumber(L,3);
		
		BEGINHUMANDEFINE("LuaFnDelMission")
		pHuman->DelMission(misId);

		Lua_PushNumber( L, 1 );
		return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnIsMissionHaveDone
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*     ����˵�� :
	*				�Ƿ�����������
	*				����ֵ1Ϊ�ɹ�
	*				����ֵ0Ϊʧ��
	*/
	INT LuaFnIsMissionHaveDone(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		MissionID_t misId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnIsMissionHaveDone")
		Lua_PushNumber(L,pHuman->IsMissionHaveDone(misId));
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : IsHaveMission
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				misId
	*					������
	*     ����˵�� :
	*				����Ƿ�������������
	*				����ֵ����0Ϊ�ɹ�
	*				
	*/
	INT LuaFnIsHaveMission(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		MissionID_t misId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnIsHaveMission")
		Lua_PushNumber(L,pHuman->IsHaveMission(misId));
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnBeginAddItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*				��ʼ��ӵ��߲���
	*				��EndAddItem���ʹ��
	*/
	INT LuaFnBeginAddItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

			SceneID_t sceneId = Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnBeginAddItem")
			pScene->mItemList.Init();
			return 0;
		ENDSCENEDEFINE
		
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				itemTypeSn
	*					ItemType��9λ����(ʵ��Ϊ8λ)	
	*				count
	*					�����Ŀ
	*     ����˵�� :
	*				�������Ʒ��������count��itemType��ŵĵ���
	*/
	INT LuaFnAddItem(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		INT itemTypeSn = Lua_ValueToNumber(L,2);
		INT count = Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnAddItem")
		TSerialHelper help(itemTypeSn);
		_ITEM_TYPE	itemType =	help.GetItemTypeStruct();


		if(!itemType.isNull())
		{
			pScene->mItemList.AddType(itemType,count);
			
			return 0;
		}
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnEndAddItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				������ӵ��߲���
	*				��BeginAddItem���ʹ��
	*				����1��ʾ�пռ���������Ʒ
	*/
	INT LuaFnEndAddItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnEndAddItem")
			Lua_PushNumber(L,HumanItemLogic::CanReceiveItemList(pHuman,pScene->mItemList));
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddItemListToHuman
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				����Ʒ�б��е�������Ʒ���õ���Ұ���
	*				�ڵ���BeginAddItem��EndAddItem֮��ʹ��
	*
	*	  ��ע:
	*				�˺�����ص���ӦOnItemChanged�¼��Ľű�
	*/
	INT LuaFnAddItemListToHuman(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnAddItemListToHuman")
			//pHuman->ReceiveItemList(pScene->mItemList);

		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType		= ITEM_CREATE_FROM_SCRIPT;
		ItemLogParam.CharGUID	= pHuman->GetGUID();
		ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
		ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
		ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
	
		BOOL bRet =	HumanItemLogic::ReceiveItemList(&ItemLogParam,pHuman,pScene->mItemList, 0);

		if(bRet) SaveItemLog(&ItemLogParam);

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-11-16
	*     �������� : LuaFnGetMissionData
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				nIndex
	*					���ݿ�����
	*     ����˵�� :
	*				�õ�Humanȫ�����ݿ��е�nIndex������ֵ
	*				GetMissionData(L,18,12,3)
	*/
	INT LuaFnGetMissionData(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT nIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetMissionData")

		if( nIndex<0 || nIndex>=MAX_CHAR_MISSION_DATA_NUM )
		{
			Lua_PushNumber(L,-1);
			return 1;
		}
		else
		{
			Lua_PushNumber(L,pHuman->GetMissionData(nIndex));
			return 1;
		}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		
		RETURNFALSE
	}
	
	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-11-16
	*     �������� : LuaFnSetMissionData
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				nIndex
	*					���ݿ�����
	*				nData
	*					����ֵ
	*     ����˵�� :
	*				����Human��nIndex�����ݿ��ֵΪnData
	*				�ڵ���BeginAddItem��EndAddItem֮��ʹ��
	*/
	INT LuaFnSetMissionData(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId	= Lua_ValueToNumber(L,1);
		ObjID_t selfId		= Lua_ValueToNumber(L,2);
		INT nIndex			= Lua_ValueToNumber(L,3);
		INT nData			= Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnSetMissionData")
			if( nIndex<0 || nIndex>=MAX_CHAR_MISSION_DATA_NUM )
			{
				return 0;
			}
			pHuman->SetMissionData(nIndex,nData);
		ENDHUMANDEFINE
		
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/** ---------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-11-16
	*     �������� : LuaFnAddMonsterDropItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				MonsterID
	*					������
	*				HumanID
	*					��ɫ���
	*				ItemIndex
	*					�������Ʒ����
	*				
	*     ����˵�� :
	*			�ڹ����������һ������������Ʒ,������������
	*/
	INT LuaFnAddMonsterDropItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId	=	Lua_ValueToNumber(L,1);
		ObjID_t MonsterID	=	Lua_ValueToNumber(L,2);
		ObjID_t HumanID		=	Lua_ValueToNumber(L,3);
		UINT ItemIndex		=	Lua_ValueToNumber(L,4);
		
		BEGINSCENEDEFINE("LuaFnAddMonsterDropItem")
		Obj_Monster*	pMonster	=	
			pScene->GetMonsterManager()->GetMonster(MonsterID);
		if(!pMonster)
		{
			Assert(FALSE);
			return 0;
		}
		Assert(ItemIndex>0);
		Assert(ItemIndex!=INVALID_ID);
		pMonster->GetOwnerList().AddItem(ItemIndex,HumanID);

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}

	/** ---------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-11-16
	*     �������� : LuaFnGetMonsterOwnerCount
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				MonsterID
	*					������
	*				
	*     ����˵�� :
	*			��ȡ��������������
	*/
	INT LuaFnGetMonsterOwnerCount(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId	=	Lua_ValueToNumber(L,1);
		ObjID_t MonsterID	=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetMonsterOwnerCount")
			Obj_Monster*	pMonster	=	
			pScene->GetMonsterManager()->GetMonster(MonsterID);
		if(!pMonster)
		{
			Assert(FALSE);
			return 0;
		}

		Lua_PushNumber(L,pMonster->GetOwnerList().OwnerCount);
		return 1;
		ENDSCENEDEFINE

			LUA_LEAVE_FUNCTION

			RETURNFALSE
	}

	/** ---------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-11-16
	*     �������� : LuaFnGetMonsterOwnerID
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				MonsterID
	*					������
	*				OwnerOffSet
	*					������ƫ����
	*     ����˵�� :
	*			��ȡ���������߱��
	*			����������id
	*/
	INT LuaFnGetMonsterOwnerID(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t MonsterID		=	Lua_ValueToNumber(L,2);
		UINT OwnerOffSet		=	Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnGetMonsterOwnerID")
		Obj_Monster*	pMonster	=	
			pScene->GetMonsterManager()->GetMonster(MonsterID);
		if(!pMonster)
		{
			Assert(FALSE);
			RETURNFALSE
		}	
		
		Assert(OwnerOffSet<pMonster->GetOwnerList().OwnerCount);
		Lua_PushNumber(L,pMonster->GetOwnerList().OwnerDropList[OwnerOffSet].HumanID);
		return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** ---------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-2-8
	*     �������� : LuaFnBeginUICommand
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*			��ʼ����UICommand������
	*			�޷���
	*/
	INT LuaFnBeginUICommand(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId	=	Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnBeginUICommand")
			pScene->m_XParam.CleanUp() ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}

	/** ---------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-2-8
	*     �������� : LuaFnEndUICommand
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*     ����˵�� :
	*			��������UICommand������
	*			�޷���
	*/
	INT LuaFnEndUICommand(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId	=	Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnEndUICommand")
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}	

	/** ---------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-2-8
	*     �������� : LuaFnDispatchUICommand
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				nUIIndex
	*					��������ֵ
	*     ����˵�� :
	*			��������UICommand������
	*			�޷���
	*/
	INT LuaFnDispatchUICommand(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId	= Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT nUIIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDispatchUICommand")
			GCUICommand Msg ;
			Msg.SetParam(&pScene->m_XParam) ;
			Msg.SetUIIndex(nUIIndex) ;
			pHuman->GetPlayer()->SendPacket(&Msg) ;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}	
	
	/** ---------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-2-8
	*     �������� : LuaFnUICommand_AddInt
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				nValue
	*					����������ӵ�����,����
	*     ����˵�� :
	*			��������UICommand������
	*			�޷���
	*/
	INT LuaFnUICommand_AddInt(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		INT nValue			=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnUICommand_AddInt")
			pScene->m_XParam.AddIntValue(nValue) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}	

	/** ---------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-2-8
	*     �������� : LuaFnUICommand_AddString
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				szValue
	*					����������ӵ�����,�ַ���
	*     ����˵�� :
	*			��������UICommand������
	*			�޷���
	*/
	INT LuaFnUICommand_AddString(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		const CHAR* szValue			=	Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnUICommand_AddString")
			pScene->m_XParam.AddStrValue(szValue) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}	

	/** ---------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-2-10
	*     �������� : LuaFnGetQuestionsRecord
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				nIndex
	*					����ţ�����
	*     ����˵�� :
	*			��ȡ�����Ϣ
	*			����ֵ����������(�ַ���),
	*					ѡ��0(�ַ���),
	*					ѡ��1(�ַ���),
	*					ѡ��2(�ַ���),
	*					ѡ��3(�ַ���),
	*					ѡ��4(�ַ���),
	*					ѡ��5(�ַ���),
	*					��0(����),
	*					��1(����),
	*					��2(����),
	*					��3(����),
	*					��4(����),
	*					��5(����),
	*/
	INT LuaFnGetQuestionsRecord(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		INT nIndex			=	Lua_ValueToNumber(L,1);

		INT i ;
		for( i=0; i<MAX_QUESTIONS; i++ )
		{
			if( nIndex==g_QuestionLib.m_aQuestions[i].m_nIndex )
				break ;
		}
		if( i>=MAX_QUESTIONS )
		{
			AssertEx(FALSE,"������û��������з��֣����飡") ;
			return 0 ;
		}

		_QUESTION_DATA* pData = &(g_QuestionLib.m_aQuestions[i]) ;

		Lua_PushString(L,pData->m_szContex) ;
		Lua_PushString(L,pData->m_szOption0) ;
		Lua_PushString(L,pData->m_szOption1) ;
		Lua_PushString(L,pData->m_szOption2) ;
		Lua_PushString(L,pData->m_szOption3) ;
		Lua_PushString(L,pData->m_szOption4) ;
		Lua_PushString(L,pData->m_szOption5) ;
		Lua_PushNumber(L,pData->m_Key0);
		Lua_PushNumber(L,pData->m_Key1);
		Lua_PushNumber(L,pData->m_Key2);
		Lua_PushNumber(L,pData->m_Key3);
		Lua_PushNumber(L,pData->m_Key4);
		Lua_PushNumber(L,pData->m_Key5);
		return 13;

		LUA_LEAVE_FUNCTION

		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		return 13;
	}	

	/** ---------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-4-19
	*     �������� : LuaFnSetMissionDataBit
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				nIndex
	*					MissionData�ı������
	*				nBits
	*					ƫ�Ƶ�λ����0��31��
	*				nValue
	*					����, 0���0
	*     ����˵�� :
	*			�޷���
	*/
	INT LuaFnSetMissionDataBit(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		INT nIndex			=	Lua_ValueToNumber(L,3);
		INT nBits			=	Lua_ValueToNumber(L,4);
		INT nValue			=	Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnSetMissionDataBit")
			INT nDataValue = pHuman->GetMissionData(nIndex) ;
			if( nValue )
			{
				nDataValue |= (1<<nBits) ;
			}
			else
			{
				nDataValue &= (~(1<<nBits)) ;
			}
			pHuman->SetMissionData(nIndex,nDataValue) ;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}	

	/** ---------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-4-19
	*     �������� : LuaFnGetMissionDataBit
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				nIndex
	*					MissionData�ı������
	*				nBits
	*					ƫ�Ƶ�λ����0��31��
	*     ����˵�� :
	*			0 ���� 1
	*/
	INT LuaFnGetMissionDataBit(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		INT nIndex			=	Lua_ValueToNumber(L,3);
		INT nBits			=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnGetMissionDataBit")
			INT nDataValue = pHuman->GetMissionData(nIndex) ;
			INT nRet=nDataValue&(1<<nBits) ;
			Lua_PushNumber(L,nRet);
			return 1 ;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		Lua_PushNumber(L,0);
		return 1 ;
	}	

	//������������ϵͳ
	INT LuaFnMissionNotComplateInfoNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ObjID_t targetId		= Lua_ValueToNumber( L, 3 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 4 );
		MissionID_t MissionId	= Lua_ValueToNumber( L, 5 );
		SHORT Ident				= Lua_ValueToNumber( L, 6 );

		BEGINHUMANDEFINE("LuaFnMissionNotComplateInfoNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( MissionId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;
		//INT nMissionIndex = sceneId * 10000 + targetId;

		_MISSION_LIST_t Mis;
		//BOOL bFind = g_TableInit.GetMissionListInfo( nMissionIndex, scriptId, Ident, Mis );
		BOOL bFind = g_TableInit.GetMissionListInfo( scriptId, Mis );
		Assert( bFind );

		_MISSION_REWARD_t* pReward = NULL;
		if( Mis.nReward != -1 )
		{
			pReward = g_MissionRewardTable.Get( Mis.nReward );
			Assert( pReward );
		}
		_MISSION_DIALOG_t* pDialog = g_MissionDialogTable.Get( Mis.nDialog );
		Assert( pDialog );

		ScriptParam_MissionDemandInfo paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;
		paramEventList.m_idScript = scriptId;
		paramEventList.m_idMission = RealMissionID;
		paramEventList.m_bDone = 0;

		//GetDestScene
		//Scene* pDestScene = (Scene*)( g_pSceneManager->GetScene( Mis.nDestScene ) );
		//Assert( pDestScene );

		BOOL bConvert;
		CHAR szFormatString[2048];
		ScriptString scriptStr;
		//��������
		scriptStr = pDialog->szMissionName;
		paramEventList.AddText( &scriptStr );

		//�������
		scriptStr = pScene->FormatMissionString( pDialog->szMissionContinue, szFormatString, pHuman, &Mis, pData, pReward, bConvert );
		paramEventList.AddText( &scriptStr );

		Packets::GCScriptCommand Msg;
		Msg.SetMissionDemandInfoResponseCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnMissionComplateInfoNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ObjID_t targetId		= Lua_ValueToNumber( L, 3 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 4 );
		MissionID_t MissionId	= Lua_ValueToNumber( L, 5 );
		SHORT Ident				= Lua_ValueToNumber( L, 6 );

		BEGINHUMANDEFINE("LuaFnMissionComplateInfoNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( MissionId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;
		//INT nMissionIndex = sceneId * 10000 + targetId;

		_MISSION_LIST_t Mis;
		//BOOL bFind = g_TableInit.GetMissionListInfo( nMissionIndex, scriptId, Ident, Mis );
		BOOL bFind = g_TableInit.GetMissionListInfo( scriptId, Mis );
		Assert( bFind );

		_MISSION_REWARD_t* pReward = NULL;
		if( Mis.nReward != -1 )
		{
			pReward = g_MissionRewardTable.Get( Mis.nReward );
			Assert( pReward );
		}
		_MISSION_DIALOG_t* pDialog = g_MissionDialogTable.Get( Mis.nDialog );
		Assert( pDialog );

		ScriptParam_MissionContinueInfo paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;
		paramEventList.m_idScript = scriptId;
		paramEventList.m_idMission = RealMissionID;

		ScriptString scriptStr;
		//��������
		scriptStr = pDialog->szMissionName;
		paramEventList.AddText( &scriptStr );

		//�������
		scriptStr = pDialog->szMissionSuccess;
		paramEventList.AddText( &scriptStr );

		if( pReward )
		{
			SMissionBonus bonus;
			if( pReward->nMoney > 0 )
			{
				bonus.SetMoney( pReward->nMoney );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}

			//�̶�������Ʒ
			if( pReward->nItem1ID != -1 && pReward->nItem1Num > 0 )
			{
				bonus.SetItem( pReward->nItem1ID, pReward->nItem1Num );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}
			if( pReward->nItem2ID != -1 && pReward->nItem2Num > 0 )
			{
				bonus.SetItem( pReward->nItem2ID, pReward->nItem2Num );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}

			//ѡ������Ʒ
			if( pReward->nItem3ID != -1 && pReward->nItem3Num > 0 )
			{
				bonus.SetItemRadio( pReward->nItem3ID, pReward->nItem3Num );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}
			if( pReward->nItem4ID != -1 && pReward->nItem4Num > 0 )
			{
				bonus.SetItemRadio( pReward->nItem4ID, pReward->nItem4Num );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}
			if( pReward->nItem5ID != -1 && pReward->nItem5Num > 0 )
			{
				bonus.SetItemRadio( pReward->nItem5ID, pReward->nItem5Num );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}
			if( pReward->nItem6ID != -1 && pReward->nItem6Num > 0 )
			{
				bonus.SetItemRadio( pReward->nItem6ID, pReward->nItem6Num );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}
		}

		Packets::GCScriptCommand Msg;
		Msg.SetMissionContinueInfoResponseCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnIsHaveMissionNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		MissionID_t MissionId	= Lua_ValueToNumber( L, 3 );

		BEGINHUMANDEFINE("LuaFnIsHaveMissionNM")
		_MISSION_DATA_t* pData = g_MissionDataTable.Get( MissionId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;
		Lua_PushNumber(L,pHuman->IsHaveMission(RealMissionID));
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnIsMissionHaveDoneNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId	= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId		= Lua_ValueToNumber( L, 2 );
		MissionID_t misId	= Lua_ValueToNumber( L, 3 );

		BEGINHUMANDEFINE("LuaFnIsMissionHaveDoneNM")
		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;

		Lua_PushNumber(L,pHuman->IsMissionHaveDone(RealMissionID));

		return 1;

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnMissionCheckAcceptNM( Lua_State* L )
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber( L, 1 );
		ObjID_t selfId				=	Lua_ValueToNumber( L, 2 );
		ObjID_t targetId			=	Lua_ValueToNumber( L, 3 );
		MissionID_t	MissionID		=	Lua_ValueToNumber( L, 4 );

		BEGINHUMANDEFINE("LuaFnMissionCheckAcceptNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( MissionID );
		if( pData == NULL )
		{
			Lua_PushNumber( L, -1 ); //û����������
			return 1;
		}

		//�ȼ�
		if( pData->nLevel != -1 && pHuman->GetLevel() < pData->nLevel )
		{
			Lua_PushNumber( L, -2 );
			return 1;
		}
		//���ȼ�
		if( pData->nMaxLeve != -1 && pHuman->GetLevel() > pData->nMaxLeve )
		{
			Lua_PushNumber( L, -3 );
			return 1;
		}
		//��Ӫ
		if( pData->nCamp != -1 )
		{
			ENUM_RELATION eRelation = CalcRelationType( pHuman->GetCampData()->m_nCampID, pHuman->GetCampData()->m_nCampID, &g_CampAndStandDataMgr );
			if( eRelation != RELATION_FRIEND )
			{
				Lua_PushNumber( L, -4 );
				return 1;
			}
		}

		//��Ҫ��Ʒ1
		if( pData->nItem1ID != -1 && pData->nItem1Num > 0 )
		{
			INT nItemCount = 0;
			nItemCount += HumanItemLogic::CalcEquipItemCount( pHuman, pData->nItem1ID );
			nItemCount += HumanItemLogic::CalcBagItemCount( pHuman, pData->nItem1ID );
			if( nItemCount < pData->nItem1Num )
			{
				Lua_PushNumber( L, -5 );
				return 1;
			}
		}
		//��Ҫ��Ʒ2
		if( pData->nItem2ID != -1 && pData->nItem2Num > 0 )
		{
			INT nItemCount = 0;
			nItemCount += HumanItemLogic::CalcEquipItemCount( pHuman, pData->nItem2ID );
			nItemCount += HumanItemLogic::CalcBagItemCount( pHuman, pData->nItem2ID );
			if( nItemCount < pData->nItem2Num )
			{
				Lua_PushNumber( L, -6 );
				return 1;
			}
		}
		//��Ǯ
		if( pData->nMoney1 > 0 )
		{
			if( pHuman->GetMoney() < (UINT)pData->nMoney1 )
			{
				Lua_PushNumber( L, -8 );
				return 1;
			}
		}
		//ְҵ
		if( pData->nMenPai > 0 )
		{
			if( pHuman->GetMenPai() != pData->nMenPai )
			{
				Lua_PushNumber( L, -9 );
				return 1;
			}
		}
		//���������
		if( pData->nTotalTimes > 0 && pData->nPositionTag > 0 )
		{
			if( pHuman->GetMissionTotalTimes( pData->nPositionTag ) > pData->nTotalTimes )
			{
				Lua_PushNumber(L,-10);
				return 1;
			}
		}
		if( pData->nDayTimes > 0 && pData->nPositionTag > 0 )
		{
			if( pHuman->GetMissionLastDay( pData->nPositionTag ) == (INT)g_pTimeManager->GetDayTime() )
			{
				if( pHuman->GetMissionTotalDayTimes( pData->nPositionTag ) > pData->nDayTimes )
				{
					Lua_PushNumber( L, -11 );
					return 1;
				}
			}
		}
		if( pData->nTimeDist > 0 && pData->nPositionTag > 0 )
		{
			WORD LastTime = pHuman->GetMissionLastTime( pData->nPositionTag );
			LastTime += ( pData->nTimeDist / 60 ) * 100;
			LastTime += pData->nTimeDist % 60;
			BOOL ret = g_pTimeManager->FormatTodayTime( LastTime );
			INT LastDay = pHuman->GetMissionLastDay( pData->nPositionTag );
			if( ret ) LastDay ++;
			if( LastDay == (INT)g_pTimeManager->GetDayTime() )
			{
				if( LastTime > g_pTimeManager->GetTodayTime())
				{
					Lua_PushNumber( L, -12 );
					return 1;
				}
			}
		}
		if( pData->nMaxCycle > 0 && pData->nPositionTag > 0 )
		{
			if( pHuman->GetMissionTotalHuan( pData->nPositionTag ) > pData->nMaxCycle )
			{
				//��Ӧ�ó���������������������ʱ���������˵�ʱ��Ӧ�ø�����
				Lua_PushNumber( L, -13 );
				return 1;
			}
		}
		if( pData->nPreMission > 0 )
		{
			if( !pHuman->IsMissionHaveDone(pData->nPreMission) )
			{
				//�ж�ǰ������û�����
				Lua_PushNumber( L, -14 );
				return 1;
			}
		}

		Lua_PushNumber( L, 1 );
		return 1;

		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	INT LuaFnMissionCheckSubmitNM( Lua_State* L )
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber( L, 1 );
		ObjID_t selfId				=	Lua_ValueToNumber( L, 2 );
		ObjID_t targetId			=	Lua_ValueToNumber( L, 3 );
		MissionID_t	MissionID		=	Lua_ValueToNumber( L, 4 );

		BEGINHUMANDEFINE("LuaFnMissionCheckSubmitNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( MissionID );
		if( pData == NULL )
		{
			Lua_PushNumber( L, -1 ); //û����������
			return 1;
		}

		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;
		UINT uMissionIndex = pHuman->GetMissionIndexByID( RealMissionID );
		if( uMissionIndex == UINT_MAX )
		{
			Lua_PushNumber( L, -2 ); // �������û�������
			return 1;
		}

		//��Ҫ��Ʒ4
		if( pData->nItem4ID != -1 && pData->nItem4Num > 0 )
		{
			INT nItemCount = 0;
			nItemCount += HumanItemLogic::CalcEquipItemCount( pHuman, pData->nItem4ID );
			nItemCount += HumanItemLogic::CalcBagItemCount( pHuman, pData->nItem4ID );
			if( nItemCount < pData->nItem4Num )
			{
				Lua_PushNumber( L, -10 );
				return 1;
			}
		}
		//��Ҫ��Ʒ5
		if( pData->nItem5ID != -1 && pData->nItem5Num > 0 )
		{
			INT nItemCount = 0;
			nItemCount += HumanItemLogic::CalcEquipItemCount( pHuman, pData->nItem5ID );
			nItemCount += HumanItemLogic::CalcBagItemCount( pHuman, pData->nItem5ID );
			if( nItemCount < pData->nItem5Num )
			{
				Lua_PushNumber( L, -11 );
				return 1;
			}
		}
		//��Ҫ��Ʒ6
		if( pData->nItem6ID != -1 && pData->nItem6Num > 0 )
		{
			INT nItemCount = 0;
			nItemCount += HumanItemLogic::CalcEquipItemCount( pHuman, pData->nItem6ID );
			nItemCount += HumanItemLogic::CalcBagItemCount( pHuman, pData->nItem6ID );
			if( nItemCount < pData->nItem6Num )
			{
				Lua_PushNumber( L, -12 );
				return 1;
			}
		}
		//��Ҫ��Ʒ7
		if( pData->nItem7ID != -1 && pData->nItem7Num > 0 )
		{
			INT nItemCount = 0;
			nItemCount += HumanItemLogic::CalcEquipItemCount( pHuman, pData->nItem7ID );
			nItemCount += HumanItemLogic::CalcBagItemCount( pHuman, pData->nItem7ID );
			if( nItemCount < pData->nItem7Num )
			{
				Lua_PushNumber( L, -13 );
				return 1;
			}
		}

		INT nKillObj = 1;
		INT nKillNum = 0;
		INT nNonceTach = 4;

		//��Ҫɱ����1
		if( pData->nMonster1ID != -1 && pData->nMonster1Num > 0 )
		{
			nKillNum  = pHuman->GetMissionParam( uMissionIndex, nKillObj++ );
			if( nKillNum < pData->nMonster1Num )
			{
				Lua_PushNumber( L, -20 );
				return 1;
			}
		}

		//��Ҫɱ����2
		if( pData->nMonster2ID != -1 && pData->nMonster2Num > 0 )
		{
			nKillNum  = pHuman->GetMissionParam( uMissionIndex, nKillObj++ );
			if( nKillNum < pData->nMonster2Num )
			{
				Lua_PushNumber( L, -21 );
				return 1;
			}
		}

		//��Ҫɱ����3
		if( pData->nMonster3ID != -1 && pData->nMonster3Num > 0 )
		{
			nKillNum  = pHuman->GetMissionParam( uMissionIndex, nKillObj++ );
			if( nKillNum < pData->nMonster3Num )
			{
				Lua_PushNumber( L, -22 );
				return 1;
			}
		}

		//��Ҫɱ����4
		if( pData->nMonster4ID != -1 && pData->nMonster4Num > 0 )
		{
			nKillNum  = pHuman->GetMissionParam( uMissionIndex, nKillObj++ );
			if( nKillNum < pData->nMonster4Num )
			{
				Lua_PushNumber( L, -23 );
				return 1;
			}
		}

		//��Ǯ
		if( pData->nMoney2 > 0 )
		{
			if( pHuman->GetMoney() < (UINT)pData->nMoney2 )
			{
				Lua_PushNumber( L, -30 );
				return 1;
			}
		}
		//�ܻ�
		if ( pData->nMissionType == 5 && pData->nMaxCycle > 0)
		{
			nNonceTach = pHuman->GetMissionParam( uMissionIndex, nNonceTach);
			if ( nNonceTach < pData->nMaxCycle )
			{
				Lua_PushNumber( L, -50 );
				return 1;
			}
		}

		Lua_PushNumber( L, 1 );
		return 1;

		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	INT LuaFnAddMissionTextNM( Lua_State* L )
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ObjID_t targetId		= Lua_ValueToNumber( L, 3 );
		INT scriptId			= Lua_ValueToNumber( L, 4 );
		MissionID_t MissionId	= Lua_ValueToNumber( L, 5 );
		SHORT Ident				= Lua_ValueToNumber( L, 6 );
		INT state;
		INT index;

		if(Lua_GetTopIndex(L) < 7)
			state = -1;//���Խӵ�����
		else
			state = Lua_ValueToNumber( L, 7 );

		if(Lua_GetTopIndex(L) < 8)
			index = -1;//���Խӵ�����
		else
			index = Lua_ValueToNumber( L, 8 );

		BEGINSCENEDEFINE("LuaFnAddMissionTextNM")

		//INT nMissionIndex = sceneId * 10000 + targetId;

		_MISSION_LIST_t Mis;
		//BOOL bFind = g_TableInit.GetMissionListInfo( nMissionIndex, scriptId, Ident, Mis );
		BOOL bFind = g_TableInit.GetMissionListInfo( scriptId, Mis );
		Assert( bFind );

		_MISSION_DIALOG_t* pData = g_MissionDialogTable.Get( Mis.nDialog );
		if( pData )
		{
			INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_Number * sizeof(INT));
			*pNumber = scriptId;
			INT* pState  = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_State * sizeof(INT));
			*pState = state;
			INT* pIndex  = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_Index * sizeof(INT));
			*pIndex = index;

			Q_strncpyz(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_Count * sizeof(INT), pData->szMissionName, _MisBuf::MISSIONCHARBUFLEN - EA_Count * sizeof(INT));
			pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_IntStr;	
		}


		return 0;

		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	INT LuaFnAddMissionNM( Lua_State* L )
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		MissionID_t misId		= Lua_ValueToNumber( L, 3 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 4 );

		BOOL killObjEvent     = FALSE;
		BOOL enterAreaEvent   = FALSE;
		BOOL itemChangedEvent = FALSE;

		BEGINHUMANDEFINE("LuaFnAddMissionNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;
		if( ( pData->nItem4ID != -1 && pData->nItem4Num > 0 ) ||
			( pData->nItem5ID != -1 && pData->nItem5Num > 0 ) ||
			( pData->nItem6ID != -1 && pData->nItem6Num > 0 ) ||
			( pData->nItem7ID != -1 && pData->nItem7Num > 0 ) )
		{
			itemChangedEvent = TRUE;
		}
		if( ( pData->nMonster1ID != -1 && pData->nMonster1Num > 0 ) ||
			( pData->nMonster2ID != -1 && pData->nMonster2Num > 0 ) ||
			( pData->nMonster3ID != -1 && pData->nMonster3Num > 0 ) ||
			( pData->nMonster4ID != -1 && pData->nMonster4Num > 0 ) )
		{
			killObjEvent = TRUE;
		}

		BOOL bCanAdd = TRUE;
		if( pData->nItem3ID != -1 && pData->nItem3Num > 0 )
		{
			pScene->mItemList.Init();
			TSerialHelper help( pData->nItem3ID );
			_ITEM_TYPE itemType = help.GetItemTypeStruct();
			if( !itemType.isNull() )
			{
				pScene->mItemList.AddType( itemType, pData->nItem3Num );
			}
			else
			{
				bCanAdd = FALSE;
			}
			if( bCanAdd )
			{
				if( HumanItemLogic::CanReceiveItemList( pHuman, pScene->mItemList ) == TRUE )
				{
					ITEM_LOG_PARAM ItemLogParam;
					ItemLogParam.OpType		= ITEM_CREATE_FROM_SCRIPT;
					ItemLogParam.CharGUID	= pHuman->GetGUID();
					ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
					ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
					ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
					HumanItemLogic::ReceiveItemList( &ItemLogParam, pHuman, pScene->mItemList, 0 );
					SaveItemLog(&ItemLogParam);
				}
				else
				{
					bCanAdd = FALSE;
				}
			}
		}
		INT ret = FALSE;
		if( bCanAdd )
		{
			ret = pHuman->AddMission( RealMissionID, scriptId, killObjEvent, enterAreaEvent, itemChangedEvent );
			ret = ( ( ret == OR_OK ) ? TRUE : FALSE ) ;
		}

		Lua_PushNumber( L, ret );
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnMisMsg2PlayerNM( Lua_State* L )
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ObjID_t targetId		= Lua_ValueToNumber( L, 3 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 4 );
		MissionID_t MissionId	= Lua_ValueToNumber( L, 5 );
		SHORT Ident				= Lua_ValueToNumber( L, 6 );
		ENUM_CHAT_TYPE type;

		if(Lua_GetTopIndex(L) < 7)
			type = CHAT_TYPE_NORMAL;
		else
			type = (ENUM_CHAT_TYPE)((INT)(Lua_ValueToNumber( L, 7 )));

		BEGINHUMANDEFINE("LuaFnMisMsg2PlayerNM")

		//INT nMissionIndex = sceneId * 10000 + targetId;

		_MISSION_LIST_t Mis;
		//BOOL bFind = g_TableInit.GetMissionListInfo( nMissionIndex, scriptId, Ident, Mis );
		BOOL bFind = g_TableInit.GetMissionListInfo( scriptId, Mis );
		Assert( bFind );

		_MISSION_DIALOG_t* pDialog = g_MissionDialogTable.Get( Mis.nDialog );
		Assert( pDialog );

		CHAR szMsg[1024] = {0};
		tsnprintf( szMsg, 1024, "#Y��������%s", pDialog->szMissionName );

		GCChat Msg ;
		Msg.SetChatType( type );
		Msg.SetContexSize( (BYTE)strlen( szMsg ) );
		Msg.SetContex( szMsg ) ;
		Msg.SetSourNameSize( (BYTE)strlen( pHuman->GetName() ) ) ;
		Msg.SetSourName( pHuman->GetName() ) ;
		Msg.SetSourID( pHuman->GetID() ) ;
		Msg.SetSourCamp( pHuman->GetCampData()->m_nCampID );

		pHuman->GetPlayer()->SendPacket( &Msg ) ;

		Lua_PushNumber(L,1);
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnGetMonsterWayInfoNM( Lua_State* L )
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ObjID_t targetId		= Lua_ValueToNumber( L, 3 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 4 );
		MissionID_t MissionId	= Lua_ValueToNumber( L, 5 );
		SHORT Ident				= Lua_ValueToNumber( L, 6 );

		BEGINSCENEDEFINE("LuaFnGetMonsterWayInfoNM")

		if( Ident != 1 && Ident != 3 )
			return 0;

		//INT nMissionIndex = sceneId * 10000 + targetId;

		_MISSION_LIST_t Mis;
		//BOOL bFind = g_TableInit.GetMissionListInfo( nMissionIndex, scriptId, Ident, Mis );
		BOOL bFind = g_TableInit.GetMissionListInfo( scriptId, Mis );
		if( bFind == FALSE )
			return 0;

		_MISSION_DIALOG_t* pDialog = g_MissionDialogTable.Get( Mis.nDialog );
		Assert( pDialog );

		if( Mis.nDestScene < 0 )
			return 0;

		CHAR szName[NPC_NAME_LEN] = "Ŀ��";
		UINT ServerID = g_Config.m_SceneInfo.m_pScene[Mis.nDestScene].m_ServerID ;
		if( ServerID == g_Config.m_ConfigInfo.m_ServerID )
		{//����ǵ�ǰ�������ĳ���
			if( g_Config.m_SceneInfo.m_pScene[Mis.nDestScene].m_IsActive!=0 )
			{//�����ǰ������Active��
				Scene* pDestScene = (Scene*)( g_pSceneManager->GetScene( Mis.nDestScene ) );
				Assert( pDestScene );

				strncpy(szName, pDestScene->GetMonsterManager()->GetMonster( Mis.nSubmitorID )->GetName(), NPC_NAME_LEN-1);
			}
		}


		Lua_PushNumber( L, Mis.nDestScene );
		Lua_PushNumber( L, Mis.nDestX );
		Lua_PushNumber( L, Mis.nDestZ );
		Lua_PushString( L, szName ) ;
		
		return 4;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0;
	}
	
	INT LuaFnListMissionsNM( Lua_State* L )
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber( L, 1 );
		ObjID_t selfId				=	Lua_ValueToNumber( L, 2 );
		ObjID_t targetId			=	Lua_ValueToNumber( L, 3 );

		BEGINSCENEDEFINE("LuaFnListMissionsNM")

		INT nMissionIndex = sceneId * 10000 + targetId;
		if( targetId == INVALID_ID ) nMissionIndex = 0;

		map<INT,vector<_MISSION_LIST_t> >::iterator it;
		it = g_PromulgatorAndSubmitorIndex.find( nMissionIndex );
		if( it != g_PromulgatorAndSubmitorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "OnEnumerate", sceneId, selfId, targetId, 3 );
			}
		}
		it = g_PromulgatorIndex.find( nMissionIndex );
		if( it != g_PromulgatorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "OnEnumerate", sceneId, selfId, targetId, 1 );
			}
		}
		it = g_SubmitorIndex.find( nMissionIndex );
		if( it != g_SubmitorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "OnEnumerate", sceneId, selfId, targetId, 2 );
			}
		}
		
		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}

	INT LuaFnRequestMissionNM( Lua_State* L )
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber( L, 1 );
		ObjID_t selfId				=	Lua_ValueToNumber( L, 2 );
		ObjID_t targetId			=	Lua_ValueToNumber( L, 3 );
		INT		eventId				=	Lua_ValueToNumber( L, 4 );

		BEGINSCENEDEFINE("LuaFnRequestMissionNM")

		INT nMissionIndex = sceneId * 10000 + targetId;
		//if( targetId == INVALID_ID ) nMissionIndex = 0;--����������ʱ����Ҫ

		map<INT,vector<_MISSION_LIST_t> >::iterator it;
		it = g_PromulgatorAndSubmitorIndex.find( nMissionIndex );
		if( it != g_PromulgatorAndSubmitorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "OnDefaultEvent", sceneId, selfId, targetId, 3 );
					return 0;
				}
			}
		}
		it = g_PromulgatorIndex.find( nMissionIndex );
		if( it != g_PromulgatorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "OnDefaultEvent", sceneId, selfId, targetId, 1 );
					return 0;
				}
			}
		}
		it = g_SubmitorIndex.find( nMissionIndex );
		if( it != g_SubmitorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "OnDefaultEvent", sceneId, selfId, targetId, 2 );
					return 0;
				}
			}
		}
		
		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}

	INT LuaFnAddDispatchMissionInfoNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ObjID_t targetId		= Lua_ValueToNumber( L, 3 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 4 );
		MissionID_t misId		= Lua_ValueToNumber( L, 5 );
		SHORT Ident				= Lua_ValueToNumber( L, 6 );

		BEGINHUMANDEFINE("LuaFnAddDispatchMissionInfoNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;
		//INT nMissionIndex = sceneId * 10000 + targetId;

		_MISSION_LIST_t Mis;
		//BOOL bFind = g_TableInit.GetMissionListInfo( nMissionIndex, scriptId, Ident, Mis );
		BOOL bFind = g_TableInit.GetMissionListInfo( scriptId, Mis );
		Assert( bFind );

		_MISSION_REWARD_t* pReward = NULL;
		if( Mis.nReward != -1 )
		{
			pReward = g_MissionRewardTable.Get( Mis.nReward );
			Assert( pReward );
		}
		_MISSION_DIALOG_t* pDialog = g_MissionDialogTable.Get( Mis.nDialog );
		Assert( pDialog );

		ScriptParam_MissionInfo paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;
		paramEventList.m_idMission = RealMissionID;
		paramEventList.m_idScript = scriptId;

		//GetDestScene
		//Scene* pDestScene = (Scene*)( g_pSceneManager->GetScene( Mis.nDestScene ) );
		//Assert( pDestScene );

		//Mission Name
		BOOL bConvert;
		CHAR szFormatString[2048];
		ScriptString scriptStr;
		scriptStr = pDialog->szMissionName;
		paramEventList.AddText( &scriptStr );

		scriptStr = pScene->FormatMissionString( pDialog->szMissionDesc, szFormatString, pHuman, &Mis, pData, pReward, bConvert );
		paramEventList.AddText( &scriptStr );

		scriptStr = "#Y����Ŀ��:";
		paramEventList.AddText( &scriptStr );

		scriptStr = pScene->FormatMissionString( pDialog->szMissionTarget, szFormatString, pHuman, &Mis, pData, pReward, bConvert );
		paramEventList.AddText( &scriptStr );

		if( pReward )
		{
			SMissionBonus bonus;
			if( pReward->nMoney > 0 )
			{
				bonus.SetMoney( pReward->nMoney );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}

			//�̶�������Ʒ
			if( pReward->nItem1ID != -1 && pReward->nItem1Num > 0 )
			{
				bonus.SetItem( pReward->nItem1ID, 6 );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}
			if( pReward->nItem2ID != -1 && pReward->nItem2Num > 0 )
			{
				bonus.SetItem( pReward->nItem2ID, pReward->nItem2Num );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}

			//ѡ������Ʒ
			if( pReward->nItem3ID != -1 && pReward->nItem3Num > 0 )
			{
				bonus.SetItemRadio( pReward->nItem3ID, 3 );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}
			if( pReward->nItem4ID != -1 && pReward->nItem4Num > 0 )
			{
				bonus.SetItemRadio( pReward->nItem4ID, pReward->nItem4Num );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}
			if( pReward->nItem5ID != -1 && pReward->nItem5Num > 0 )
			{
				bonus.SetItemRadio( pReward->nItem5ID, pReward->nItem5Num );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}
			if( pReward->nItem6ID != -1 && pReward->nItem6Num > 0 )
			{
				bonus.SetItemRadio( pReward->nItem6ID, pReward->nItem6Num );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}
		}

		Packets::GCScriptCommand Msg;
		Msg.SetMissionInfoResponseCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	INT LuaFnOnMissionAcceptNM( Lua_State* L )
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber( L, 1 );
		ObjID_t selfId				=	Lua_ValueToNumber( L, 2 );
		ObjID_t targetId			=	Lua_ValueToNumber( L, 3 );
		INT		eventId				=	Lua_ValueToNumber( L, 4 );

		BEGINSCENEDEFINE("LuaFnOnMissionAcceptNM")

		INT nMissionIndex = sceneId * 10000 + targetId;
		//if( targetId == INVALID_ID ) nMissionIndex = 0;--����������ʱ����Ҫ

		map<INT,vector<_MISSION_LIST_t> >::iterator it;
		it = g_PromulgatorAndSubmitorIndex.find( nMissionIndex );
		if( it != g_PromulgatorAndSubmitorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "CheckAndAccept", sceneId, selfId, targetId, 3 );
					return 0;
				}
			}
		}
		it = g_PromulgatorIndex.find( nMissionIndex );
		if( it != g_PromulgatorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "CheckAndAccept", sceneId, selfId, targetId, 1 );
					return 0;
				}
			}
		}
		it = g_SubmitorIndex.find( nMissionIndex );
		if( it != g_SubmitorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "CheckAndAccept", sceneId, selfId, targetId, 2 );
					return 0;
				}
			}
		}
		
		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}

	INT LuaFnOnContinueMissionNM( Lua_State* L )
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber( L, 1 );
		ObjID_t selfId				=	Lua_ValueToNumber( L, 2 );
		ObjID_t targetId			=	Lua_ValueToNumber( L, 3 );
		INT		eventId				=	Lua_ValueToNumber( L, 4 );

		BEGINSCENEDEFINE("LuaFnOnContinueMissionNM")

		INT nMissionIndex = sceneId * 10000 + targetId;
		if( targetId == INVALID_ID ) nMissionIndex = 0;

		map<INT,vector<_MISSION_LIST_t> >::iterator it;
		it = g_PromulgatorAndSubmitorIndex.find( nMissionIndex );
		if( it != g_PromulgatorAndSubmitorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "OnContinue", sceneId, selfId, targetId, 3 );
					return 0;
				}
			}
		}
		it = g_PromulgatorIndex.find( nMissionIndex );
		if( it != g_PromulgatorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "OnContinue", sceneId, selfId, targetId, 1 );
					return 0;
				}
			}
		}
		it = g_SubmitorIndex.find( nMissionIndex );
		if( it != g_SubmitorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "OnContinue", sceneId, selfId, targetId, 2 );
					return 0;
				}
			}
		}

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}


	INT LuaFnSubmitMissionNM( Lua_State* L )
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber( L, 1 );
		ObjID_t selfId				=	Lua_ValueToNumber( L, 2 );
		ObjID_t targetId			=	Lua_ValueToNumber( L, 3 );
		INT		eventId				=	Lua_ValueToNumber( L, 4 );
		INT		selectId			=	Lua_ValueToNumber( L, 5 );

		BEGINSCENEDEFINE("LuaFnSubmitMissionNM")

		INT nMissionIndex = sceneId * 10000 + targetId;
		if( targetId == INVALID_ID ) nMissionIndex = 0;

		map<INT,vector<_MISSION_LIST_t> >::iterator it;
		it = g_PromulgatorAndSubmitorIndex.find( nMissionIndex );
		if( it != g_PromulgatorAndSubmitorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDDD( Mis.nScriptID, "OnSubmit", sceneId, selfId, targetId, selectId, 3 );
					return 0;
				}
			}
		}
		it = g_PromulgatorIndex.find( nMissionIndex );
		if( it != g_PromulgatorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDDD( Mis.nScriptID, "OnSubmit", sceneId, selfId, targetId, selectId, 1 );
					return 0;
				}
			}
		}
		it = g_SubmitorIndex.find( nMissionIndex );
		if( it != g_SubmitorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDDD( Mis.nScriptID, "OnSubmit", sceneId, selfId, targetId, selectId, 2 );
					return 0;
				}
			}
		}
		
		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}

	INT LuaFnMissionComplateNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ObjID_t targetId		= Lua_ValueToNumber( L, 3 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 4 );
		MissionID_t misId		= Lua_ValueToNumber( L, 5 );
		INT selId				= Lua_ValueToNumber( L, 6 );
		SHORT Ident				= Lua_ValueToNumber( L, 7 );

		BEGINHUMANDEFINE("LuaFnMissionComplateNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;
		//INT nMissionIndex = sceneId * 10000 + targetId;

		_MISSION_LIST_t Mis;
		//BOOL bFind = g_TableInit.GetMissionListInfo( nMissionIndex, scriptId, Ident, Mis );
		BOOL bFind = g_TableInit.GetMissionListInfo( scriptId, Mis );
		Assert( bFind );

		_MISSION_REWARD_t* pReward = NULL;
		if( Mis.nReward != -1 )
		{
			pReward = g_MissionRewardTable.Get( Mis.nReward );
			Assert( pReward );
		}
		_MISSION_DIALOG_t* pDialog = g_MissionDialogTable.Get( Mis.nDialog );
		Assert( pDialog );

		pScene->mItemList.Init();
		if( pReward )
		{
			//�̶�������Ʒ
			if( pReward->nItem1ID != -1 && pReward->nItem1Num > 0 )
			{
				TSerialHelper help( pReward->nItem1ID );
				_ITEM_TYPE itemType = help.GetItemTypeStruct();
				if( !itemType.isNull() )
				{
					pScene->mItemList.AddType( itemType, pReward->nItem1Num );
				}
			}
			if( pReward->nItem2ID != -1 && pReward->nItem2Num > 0 )
			{
				TSerialHelper help( pReward->nItem2ID );
				_ITEM_TYPE itemType = help.GetItemTypeStruct();
				if( !itemType.isNull() )
				{
					pScene->mItemList.AddType( itemType, pReward->nItem2Num );
				}
			}

			//ѡ������Ʒ
			if( selId > 0 )
			{
				BOOL bFind = FALSE;
				if( pReward->nItem3ID == selId )
				{
					TSerialHelper help( pReward->nItem3ID );
					_ITEM_TYPE itemType = help.GetItemTypeStruct();
					if( !itemType.isNull() )
					{
						pScene->mItemList.AddType( itemType, pReward->nItem3Num );
						bFind = TRUE;
					}
				}
				else if( pReward->nItem4ID == selId )
				{
					TSerialHelper help( pReward->nItem4ID );
					_ITEM_TYPE itemType = help.GetItemTypeStruct();
					if( !itemType.isNull() )
					{
						pScene->mItemList.AddType( itemType, pReward->nItem4Num );
						bFind = TRUE;
					}
				}
				else if( pReward->nItem5ID == selId )
				{
					TSerialHelper help( pReward->nItem5ID );
					_ITEM_TYPE itemType = help.GetItemTypeStruct();
					if( !itemType.isNull() )
					{
						pScene->mItemList.AddType( itemType, pReward->nItem5Num );
						bFind = TRUE;
					}
				}
				else if( pReward->nItem6ID == selId )
				{
					TSerialHelper help( pReward->nItem6ID );
					_ITEM_TYPE itemType = help.GetItemTypeStruct();
					if( !itemType.isNull() )
					{
						pScene->mItemList.AddType( itemType, pReward->nItem6Num );
						bFind = TRUE;
					}
				}

				if( bFind == FALSE )
				{
					Lua_PushNumber( L, -2 );
					return 1;
				}
			}

			if( HumanItemLogic::CanReceiveItemList( pHuman, pScene->mItemList ) == 0 )
			{
				Lua_PushNumber( L, -3 );
				return 1;
			}
		}

		//�۳�������Ʒ
		ITEM_LOG_PARAM ItemLogParam;
		ItemLogParam.OpType = ITEM_SCRIPT_DELETE;
		if( pData->nItem3ID != -1 && pData->nItem3Num > 0 )
		{
			if( HumanItemLogic::EraseItem( &ItemLogParam, pHuman, pData->nItem3ID, pData->nItem3Num ) == FALSE )
			{
				Lua_PushNumber( L, -4 );
				return 1;
			}
		}
		//�۳��Ͻ���Ʒ
		if( pData->nItem4ID != -1 && pData->nItem4Num > 0 )
		{
			if( HumanItemLogic::EraseItem( &ItemLogParam, pHuman, pData->nItem4ID, pData->nItem4Num ) == FALSE )
			{
				Lua_PushNumber( L, -4 );
				return 1;
			}
		}
		if( pData->nItem5ID != -1 && pData->nItem5Num > 0 )
		{
			if( HumanItemLogic::EraseItem( &ItemLogParam, pHuman, pData->nItem5ID, pData->nItem5Num ) == FALSE )
			{
				Lua_PushNumber( L, -4 );
				return 1;
			}
		}
		if( pData->nItem6ID != -1 && pData->nItem6Num > 0 )
		{
			if( HumanItemLogic::EraseItem( &ItemLogParam, pHuman, pData->nItem6ID, pData->nItem6Num ) == FALSE )
			{
				Lua_PushNumber( L, -4 );
				return 1;
			}
		}
		if( pData->nItem7ID != -1 && pData->nItem7Num > 0 )
		{
			if( HumanItemLogic::EraseItem( &ItemLogParam, pHuman, pData->nItem7ID, pData->nItem7Num ) == FALSE )
			{
				Lua_PushNumber( L, -4 );
				return 1;
			}
		}

		//������Ʒ
		if( pScene->mItemList.m_ListCount > 0 )
		{
			ItemLogParam.OpType		= ITEM_CREATE_FROM_SCRIPT;
			ItemLogParam.CharGUID	= pHuman->GetGUID();
			ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
			ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
			ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
			BOOL bRet =	HumanItemLogic::ReceiveItemList( &ItemLogParam, pHuman, pScene->mItemList, 0 );
			if( bRet )
			{
				SaveItemLog(&ItemLogParam);
			}
			else
			{
				Lua_PushNumber( L, -3 );
				return 1;
			}
		}

		//������Ǯ
		if( pReward && pReward->nMoney > 0 )
		{
			pHuman->SetMoney( pHuman->GetMoney() + pReward->nMoney );

			MONEY_LOG_PARAM	MoneyLogParam;
			MoneyLogParam.CharGUID	=	pHuman->GetGUID();
			MoneyLogParam.OPType	=	MONEY_SCRIPT_COST;	
			MoneyLogParam.Count		=	pReward->nMoney;
			MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
			MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SaveMoneyLog(&MoneyLogParam);
		}

		//��������
		if( pReward && pReward->nExp > 0 )
		{
			pHuman->SetAlterExp( pReward->nExp ) ;
		}

		//ɾ������
		pHuman->DelMission( RealMissionID );

		//�����������
		pHuman->SetMissionHaveDone( RealMissionID, TRUE );

		CHAR szMsg[1024] = {0};
		tsnprintf( szMsg, 1024, "#Y�������%s", pDialog->szMissionName );

		GCChat Msg ;
		Msg.SetChatType( 8 );
		Msg.SetContexSize( (BYTE)strlen( szMsg ) );
		Msg.SetContex( szMsg ) ;
		Msg.SetSourNameSize( (BYTE)strlen( pHuman->GetName() ) ) ;
		Msg.SetSourName( pHuman->GetName() ) ;
		Msg.SetSourID( pHuman->GetID() ) ;
		Msg.SetSourCamp( pHuman->GetCampData()->m_nCampID );

		pHuman->GetPlayer()->SendPacket( &Msg ) ;

		Lua_PushNumber( L, 1 );
		return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnGetNeedItemNumNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 3 );
		MissionID_t misId		= Lua_ValueToNumber( L, 4 );
		INT itemId				= Lua_ValueToNumber( L, 5 );

		BEGINHUMANDEFINE("LuaFnGetNeedItemNumNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );

		//_MISSION_LIST_t Mis;
		//BOOL bFind = g_TableInit.GetMissionListInfo( scriptId, Mis );
		//Assert( bFind );

		//_MISSION_DIALOG_t* pDialog = g_MissionDialogTable.Get( Mis.nDialog );
		//Assert( pDialog );

		if( pData->nItem4ID == itemId )
		{
			Lua_PushNumber( L, pData->nItem4Num );
			return 1;
		}
		if( pData->nItem5ID == itemId )
		{
			Lua_PushNumber( L, pData->nItem5Num );
			return 1;
		}
		if( pData->nItem6ID == itemId )
		{
			Lua_PushNumber( L, pData->nItem6Num );
			return 1;
		}
		if( pData->nItem7ID == itemId )
		{
			Lua_PushNumber( L, pData->nItem7Num );
			return 1;
		}

		RETURNFALSE
		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnGetMissionIndexByIDNM(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId	= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId		= Lua_ValueToNumber( L, 2 );
		MissionID_t misId	= Lua_ValueToNumber( L, 3 );

		BEGINHUMANDEFINE("LuaFnGetMissionIndexByIDNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;

		Lua_PushNumber( L, pHuman->GetMissionIndexByID(RealMissionID) );
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION	
		RETURNFALSE
	}

	INT LuaFnDelMissionNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId	= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId		= Lua_ValueToNumber( L, 2 );
		MissionID_t misId	= Lua_ValueToNumber( L, 3 );
		
		BEGINHUMANDEFINE("LuaFnDelMissionNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;

		pHuman->DelMission( RealMissionID );

		Lua_PushNumber( L, 1 );
		return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnGetNeedKillObjNumNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 3 );
		MissionID_t misId		= Lua_ValueToNumber( L, 4 );
		INT objdataId			= Lua_ValueToNumber( L, 5 );

		BEGINHUMANDEFINE("LuaFnGetNeedKillObjNumNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );

		if( pData->nMonster1ID == objdataId )
		{
			Lua_PushNumber( L, pData->nMonster1Num );
			Lua_PushNumber( L, 1 );
			return 2;
		}
		if( pData->nMonster2ID == objdataId )
		{
			Lua_PushNumber( L, pData->nMonster2Num );
			Lua_PushNumber( L, 2 );
			return 2;
		}
		if( pData->nMonster3ID == objdataId )
		{
			Lua_PushNumber( L, pData->nMonster3Num );
			Lua_PushNumber( L, 3 );
			return 2;
		}
		if( pData->nMonster4ID == objdataId )
		{
			Lua_PushNumber( L, pData->nMonster4Num );
			Lua_PushNumber( L, 4 );
			return 2;
		}

		RETURNFALSE
		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnIfFinishdKillObjNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 3 );
		MissionID_t misId		= Lua_ValueToNumber( L, 4 );
		INT objdataId			= Lua_ValueToNumber( L, 5 );

		BEGINHUMANDEFINE("LuaFnIfFinishdKillObjNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;

		UINT misIndex = pHuman->GetMissionIndexByID(RealMissionID);
		BOOL bFinish = TRUE;

		if( pData->nMonster1ID != -1 && pData->nMonster1Num > 0 )
		{
			INT nNum = pHuman->GetMissionParam( misIndex, 1 );
			if( nNum < pData->nMonster1Num ) bFinish = FALSE;
		}
		if( pData->nMonster2ID != -1 && pData->nMonster2Num > 0  )
		{
			INT nNum = pHuman->GetMissionParam( misIndex, 2 );
			if( nNum < pData->nMonster2Num ) bFinish = FALSE;
		}
		if( pData->nMonster3ID != -1 && pData->nMonster3Num > 0  )
		{
			INT nNum = pHuman->GetMissionParam( misIndex, 3 );
			if( nNum < pData->nMonster3Num ) bFinish = FALSE;
		}
		if( pData->nMonster4ID != -1 && pData->nMonster4Num > 0  )
		{
			INT nNum = pHuman->GetMissionParam( misIndex, 4 );
			if( nNum < pData->nMonster4Num ) bFinish = FALSE;
		}

		Lua_PushNumber( L, bFinish );
		return 1;

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnGetMissionTypeNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 3 );
		MissionID_t misId		= Lua_ValueToNumber( L, 4 );

		BEGINHUMANDEFINE("LuaFnGetMissionTypeNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );

		Lua_PushNumber( L, pData->nMissionType );
		return 1;

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnGetMissionItemNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 3 );
		MissionID_t misId		= Lua_ValueToNumber( L, 4 );
		INT ObjIndex			= Lua_ValueToNumber( L, 5 );

		BEGINHUMANDEFINE("LuaFnGetMissionItemNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );

		switch( ObjIndex )
		{
		case 1:
			Lua_PushNumber( L, pData->nItem4Num );
			Lua_PushNumber( L, pData->nItem4ID );
			return 2;
		case 2:
			Lua_PushNumber( L, pData->nItem5Num );
			Lua_PushNumber( L, pData->nItem5ID );
			return 2;
		case 3:
			Lua_PushNumber( L, pData->nItem6Num );
			Lua_PushNumber( L, pData->nItem6ID );
			return 2;
		case 4:
			Lua_PushNumber( L, pData->nItem7Num );
			Lua_PushNumber( L, pData->nItem7ID );
			return 2;
		default:
			break;
		}

		RETURNFALSE

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnFinishKillObjGetItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 3 );
		MissionID_t misId		= Lua_ValueToNumber( L, 4 );
		INT itemdataId			= Lua_ValueToNumber( L, 5 );

		BEGINHUMANDEFINE("LuaFnFinishKillObjGetItem")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;

		UINT misIndex = pHuman->GetMissionIndexByID(RealMissionID);

		if( pData->nItem4ID == itemdataId && pData->nItem4Num > 0 )
		{
			pHuman->SetMissionParam( misIndex, 1, pData->nMonster1Num );
		}
		if( pData->nItem5ID == itemdataId && pData->nItem5Num > 0 )
		{
			pHuman->SetMissionParam( misIndex, 2, pData->nMonster2Num );
		}
		if( pData->nItem6ID == itemdataId && pData->nItem6Num > 0 )
		{
			pHuman->SetMissionParam( misIndex, 3, pData->nMonster3Num );
		}
		if( pData->nItem7ID == itemdataId && pData->nItem7Num > 0 )
		{
			pHuman->SetMissionParam( misIndex, 4, pData->nMonster4Num );
		}

		return 0;

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnGetMissionNameNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 3 );
		MissionID_t misId		= Lua_ValueToNumber( L, 4 );

		BEGINHUMANDEFINE("LuaFnGetMissionNameNM")

		_MISSION_DIALOG_t* pData = g_MissionDialogTable.Get( misId );
		Assert( pData );

		Lua_PushString( L, pData->szMissionName );
		return 1;

		RETURNFALSE

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnGetFrontMissiontId(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 3 );
		MissionID_t misId		= Lua_ValueToNumber( L, 4 );

		BEGINHUMANDEFINE("LuaFnGetFrontMissiontId")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );

		Lua_PushNumber( L, pData->nPreMission );
		return 1;

		RETURNFALSE

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}


	INT LuaFnGetAfterMissiontId(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 3 );
		MissionID_t misId		= Lua_ValueToNumber( L, 4 );

		BEGINHUMANDEFINE("LuaFnGetAfterMissiontId")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );

		Lua_PushNumber( L, pData->nAftMission );
		return 1;

		RETURNFALSE

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnSetMissionHaveDoneFlagNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		MissionID_t misId		= Lua_ValueToNumber( L, 3 );
		BOOL IsDone				= Lua_ValueToNumber( L, 4 );

		BEGINHUMANDEFINE("LuaFnSetMissionHaveDoneFlagNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;

		if (IsDone)
            pHuman->SetMissionHaveDone( RealMissionID, TRUE );
		else
			pHuman->SetMissionHaveDone( RealMissionID, FALSE );

		return 0;

		RETURNFALSE

			ENDHUMANDEFINE
			LUA_LEAVE_FUNCTION
			RETURNFALSE
	}


	INT LuaFnGetMissionMaxCycleNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 3 );
		MissionID_t misId		= Lua_ValueToNumber( L, 4 );

		BEGINHUMANDEFINE("LuaFnGetMissionMaxCycleNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );

		Lua_PushNumber( L, pData->nMaxCycle );
		return 1;

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}





















}

#endif