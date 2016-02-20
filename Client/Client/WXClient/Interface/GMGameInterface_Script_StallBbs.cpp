#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "LuaPlus.h"
#include "../Global.h"
#include "../Object/ObjectManager.h"
#include "../Object/Object.h"
#include "../Object/Character/Obj_Character.h"
#include "../Object/ObjectManager.h"
#include "../Event/GMEventSystem.h"
#include "../Network/NetManager.h"
#include "../DataPool/GMDataPool.h"
#include "../Action/GMActionSystem.h"
#include "../Object/ObjectManager.h"
#include "../object/character/obj_playermyself.h"
#include "../DataPool/GMDP_CharacterData.h"
#include "../procedure/GameProcedure.h"

#include "CGBBSSychMessages.h"

namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	StallBbs StallBbs::s_StallBbs;
	LuaPlus::LuaObject* StallBbs::s_pMetaTable = NULL;

	//�����Ϣ������
	INT StallBbs::GetMessageNum(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA: StallBbs::GetMessageNum Wrong Param1");
		}

		std::string str = args[2].GetString();
		if(str == "sale")
		{
			state->PushInteger(CDataPool::GetMe()->MyStallMsg_GetMessageNum());
		}
		else if(str == "buy")
		{
			state->PushInteger(CDataPool::GetMe()->OtStallMsg_GetMessageNum());
		}
		return 1;
	}
	//���(ͬʱ������Ϣ���������֡�ʱ�䡢��Ϣ���ݣ��Լ��ظ�)
	INT StallBbs::EnumMessage(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: StallBbs::EnumMessage Wrong Param1");
		}
		INT nIndex = args[2].GetInteger();
	
		if (!(args[3].IsString()))
		{
			TDThrow("LUA: StallBbs::EnumMessage Wrong Param1");
		}

		std::string str = args[3].GetString();
		if(str == "sale")
		{
			//����Ϣ��ID
			state->PushInteger(CDataPool::GetMe()->MyStallMsg_GetIDByIndex(nIndex));

			//������Ϣ�ߵ�����
			state->PushString(CDataPool::GetMe()->MyStallMsg_GetAuthorNameByIndex(nIndex));
			//����ʱ��
			char szTime[16];
			INT nHour;
			INT nMin;
			nHour = CDataPool::GetMe()->MyStallMsg_GetHourByIndex(nIndex);
			nMin  = CDataPool::GetMe()->MyStallMsg_GetMinByIndex(nIndex);
			sprintf(szTime, "%0.2d:%0.2d",nHour,nMin);
			state->PushString(szTime);

			//��Ϣ����
			state->PushString(CDataPool::GetMe()->MyStallMsg_GetProposedMessageByIndex(nIndex));
			//�Ƿ��ɻظ���Ϣ
			if(0 == CDataPool::GetMe()->MyStallMsg_GetHasReplyByIndex(nIndex))
			{
				state->PushBoolean(false);
				state->PushString("");
			}
			else
			{
				state->PushBoolean(true);
				//�ظ���Ϣ
				state->PushString(CDataPool::GetMe()->MyStallMsg_GetReplyMessageByIndex(nIndex));
			}
		}
		else if(str == "buy")
		{
			//����Ϣ��ID
			state->PushInteger(CDataPool::GetMe()->OtStallMsg_GetIDByIndex(nIndex));

			//������Ϣ�ߵ�����
			state->PushString(CDataPool::GetMe()->OtStallMsg_GetAuthorNameByIndex(nIndex));
			//����ʱ��
			char szTime[16];
			INT nHour;
			INT nMin;
			nHour = CDataPool::GetMe()->OtStallMsg_GetHourByIndex(nIndex);
			nMin  = CDataPool::GetMe()->OtStallMsg_GetMinByIndex(nIndex);
			sprintf(szTime,"%0.2d:%0.2d",nHour,nMin);
			state->PushString(szTime);
			//��Ϣ����
			state->PushString(CDataPool::GetMe()->OtStallMsg_GetProposedMessageByIndex(nIndex));
			//�Ƿ��ɻظ���Ϣ
			if(0 == CDataPool::GetMe()->OtStallMsg_GetHasReplyByIndex(nIndex))
			{
				state->PushBoolean(false);
				state->PushString("");
			}
			else
			{
				state->PushBoolean(true);
				//�ظ���Ϣ
				state->PushString(CDataPool::GetMe()->OtStallMsg_GetReplyMessageByIndex(nIndex));
			}
		}

		return 6;
	}
	//���һ���µ���Ϣ��̯������Ҷ�Ҫʹ�ã�
	INT StallBbs::AddMessage(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA: StallBbs::AddMessage[2] Wrong Param1");
		}

		if (!(args[3].IsString()))
		{
			TDThrow("LUA: StallBbs::AddMessage[3] Wrong Param1");
		}

		std::string strMessage = args[2].GetString();
		std::string strWho = args[3].GetString();
		
		//���ݿյ�ʱ�򲻷�����Ϣ����һ����ʾ
		if(strMessage == "")
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ϣ����Ϊ��");
			return 0;
		}

		//����Ƿ񳤶ȳ�����󳤶ȣ������Ͷ���β��
		if(strMessage.size() > MAX_BBS_MESSAGE_LENGTH)
			strMessage.resize(MAX_BBS_MESSAGE_LENGTH);

		CGBBSSychMessages msg;
		if(strWho == "sale")
			msg.SetObjID(CObjectManager::GetMe()->GetMySelf()->GetServerID());
		else if(strWho == "buy")
			msg.SetObjID(CDataPool::GetMe()->OtStallBox_GetObjID());
		else
			return 0;

		msg.SetOpt(CGBBSSychMessages::OPT_NEW_MESSAGE);
		msg.SetLength((BYTE)strMessage.size());
		msg.SetMessage((CHAR*)strMessage.c_str());

		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	//�ظ�һ����Ϣ(̯��ʹ��)
	INT StallBbs::ReplyMessage(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: StallBbs::ReplyMessage Wrong Param1");
		}
		INT nMessgaeId = args[2].GetInteger();

		if (!(args[3].IsString()))
		{
			TDThrow("LUA: StallBbs::ReplyMessage Wrong Param1");
		}
		std::string strMessage = args[3].GetString();
		
		//���ݿյ�ʱ�򲻷�����Ϣ����һ����ʾ
		if(strMessage == "")
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ϣ����Ϊ��");
			return 0;
		}

		//����Ƿ񳤶ȳ�����󳤶ȣ������Ͷ���β��
		if(strMessage.size() > MAX_BBS_MESSAGE_LENGTH)
			strMessage.resize(MAX_BBS_MESSAGE_LENGTH);

		CGBBSSychMessages msg;
		msg.SetObjID(CObjectManager::GetMe()->GetMySelf()->GetServerID());
		msg.SetOpt(CGBBSSychMessages::OPT_REPLY_MESSAGE);
		msg.SetLength((BYTE)strMessage.size());
		msg.SetMessage((CHAR*)strMessage.c_str());
		msg.SetID(nMessgaeId);

		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}

}
