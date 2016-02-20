#include "StdAfx.h"
#include <map>
#include <deque>
#include "..\Network\NetManager.h"
#include "GMGameInterface.h"
#include "TDException.h"
#include "TDTimeSystem.h"
#include "TDEventDefine.h"
#include "TDUISystem.h"
#include "TDUtil.h"
#include "LuaPlus.h"
#include "../Global.h"
#include "../Object/ObjectManager.h"
#include "../Object/Object.h"
#include "../Object/Character/Obj_Character.h"
#include "../object/character/obj_playermyself.h"
#include "../DataPool/GMDataPool.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "..\Procedure\GameProcedure.h"
#include "../Event/GMEventSystem.h"
#include "..\Input\GMInputSystem.h"
#include "../DataPool/GMDP_Struct_Relation.h"
#include "../DBC/GMDataBase.h"
#include "../Action/GMActionSystem.h"
#include "../World/WorldManager.h"
#define MAX_SYSCHAT_SIZE 300
//
//�ű�����ɳ�䣬��Щ����Ӧ���ɽű�����
//
namespace SCRIPT_SANDBOX
{
	//��ǰ���ڴ���Ƶ��
	const char*		NAMETYPE_TALK_NEAR		= "near";
	const char*		NAMETYPE_TALK_SCENE		= "scene";
	const char*		NAMETYPE_TALK_SYSTEM	= "system";
	const char*		NAMETYPE_TALK_TEAM		= "team";
	const char*		NAMETYPE_TALK_GUILD		= "guild";
	const char*		NAMETYPE_TALK_USER		= "user";
	const char*		NAMETYPE_TALK_PRIVATE	= "private";
	const char*		NAMETYPE_TALK_SELF		= "self";
	const char*		NAMETYPE_TALK_MENPAI	= "menpai";

	//�ַ���ͷ��ʶ���
	const INT		MESSAGE_HEAD_LENGTH		= 3;	//������Ϣͷ����
	const char		MESSAGETYPE_TALK_NEAR[MESSAGE_HEAD_LENGTH+1]		= "/D ";	//��������
	const char		MESSAGETYPE_TALK_WORD[MESSAGE_HEAD_LENGTH+1]		= "/W ";	//��������
	const char		MESSAGETYPE_TALK_TEAM[MESSAGE_HEAD_LENGTH+1]		= "/T ";	//�������
	const char		MESSAGETYPE_TALK_GUILD[MESSAGE_HEAD_LENGTH+1]		= "/B ";	//��������
	const char		MESSAGETYPE_TALK_MENPAI[MESSAGE_HEAD_LENGTH+1]		= "/M ";	//��������

	//�������������������
	const INT		TALK_TO_USER_MAXNUM				= 3;

	//�Խ�Ƶ��������ӵ�����������
	const INT		CHANNEL_INVITE_USER_MAXNUM		=11;

	//˵����ʱ����λ���룩
	const INT		CHAT_TALK_SLICE					= 5;

	//10����������Ի�ʱ��˵���������λ���룩
	const INT		CHAT_TALK_LV10_SLICE			= 10;

	const INT		CHAT_PINGBI_MAXUSER_NUMBERS		= 20;
	//GM�����ǰ���ַ�,���ȱ�����2�����ַ�����Ϊ���ͺ�����ֻ���ͷ2�����ַ�
	const char*		GM_TALK_HEADER			= "!!";

	//���춯����ǰ���ַ�
	const char*		CHAT_ACT_HEADER			="*";

	Talk Talk::s_Talk;
	LuaPlus::LuaObject* Talk::s_pMetaTable = NULL;

	Talk::Talk()
	{
		Channel* newChannel = new Channel;
		
		newChannel->SetType(CHAT_TYPE_NORMAL);
		m_listChannel.push_back(newChannel);
		
		newChannel = new Channel;
		newChannel->SetType(CHAT_TYPE_SCENE);
		m_listChannel.push_back(newChannel);

		m_SelfCh = new SelfChannel;
		m_SelfCh->SetType(CHAT_TYPE_SELF);

		m_SysCh = new SystemChannel;
		m_SysCh->SetType(CHAT_TYPE_SYSTEM);

		m_PrvCh = NULL;
		m_UsrCh = NULL;
		m_TeamCh = NULL;
		m_MenPaiCh = NULL;
		m_GuildCh = NULL;

		m_MaxSaveNum = 0;
		m_PrevSendPos = -1;
		m_PrevGMCmdPos = -1;
		m_bPaoPaoRule = FALSE;
	}

	VOID Talk::Initial()
	{
		static BOOL bFirst = FALSE;

		if(bFirst) return;

		//ע��ʱ���¼��ص�����
		CGameProcedure::s_pEventSystem->RegisterEventHandle("TIME_UPDATE", _SendTalkHelpMsg);
		CGameProcedure::s_pEventSystem->RegisterEventHandle("VIEW_RESOLUTION_CHANGED", _HandleWindowSizeChange);

		//�����������ж������ɣ�����Ӧ��action button
		GenTalkActMap();
		CActionSystem::GetMe()->UserChatMood_Update();

		//�����������
		std::list< Channel* >::iterator it;
		for(it=m_listChannel.begin(); it != m_listChannel.end(); it++)
		{
			(*it)->LoadChannelConfig();
		}

		bFirst = TRUE;
	}

	Talk::~Talk()
	{
		SAFE_DELETE(m_SelfCh);
		SAFE_DELETE(m_PrvCh);
		SAFE_DELETE(m_SysCh);

		//below is deleted by m_listChannel
		//SAFE_DELETE(m_UsrCh);
		//SAFE_DELETE(m_TeamCh);
		//SAFE_DELETE(m_MenPaiCh);
		//SAFE_DELETE(m_GuildCh);

		std::list< Channel* >::iterator it;
		for(it=m_listChannel.begin(); it != m_listChannel.end(); it++)
		{
			SAFE_DELETE(*it);
		}

		m_listChannel.clear();
	}
		
	//HistoryMsg Class
	Talk::HistoryMsg::HistoryMsg(ENUM_CHAT_TYPE type)
	{
		m_Name = "";
		m_ChannelType = type;
		m_Data = "";
		m_Time = CGameProcedure::s_pTimeSystem->GetTimeNow();
	}

	VOID Talk::HistoryMsg::SetMsgData(STRING data, CHAR* file, INT line)
	{
#ifdef _MSG_POSITION_SHOW_
		if(file)
		{
			CHAR Info[128] = {0};
			_snprintf(Info, 127, "#W DataEmpty(%s)DataSize(%u)File:%s(Line:%d)", (data.empty())?"Yes":"False",data.size(), file, line);
			m_Data = data + Info;
		}
		else
		{
			m_Data = data;
		}
#else
		m_Data = data;
#endif
	}

	INT Talk::HistoryMsg::SetByPacket(GCChat *packet)
	{
		if(NULL == packet)
		{
			Assert(FALSE);
			return -1;
		}

		CHAR szTmp[MAX_CHAT_SIZE];
		memset(szTmp, 0, MAX_CHAT_SIZE);

		memcpy(szTmp, packet->GetSourName(), packet->GetSourNameSize());
		//talker
		m_Name = szTmp;

		memset(szTmp, 0, MAX_CHAT_SIZE);
		memcpy(szTmp, packet->GetContex(), packet->GetContexSize());
		//talk contex
		m_Data = szTmp;

		//channel type
		m_ChannelType = (ENUM_CHAT_TYPE)packet->GetChatType();

		//saveTime
		m_Time = CGameProcedure::s_pTimeSystem->GetTimeNow();

		return 0;
	}

	//Channel Class
	BOOL Talk::Channel::TalkNeedCheck()
	{
		BOOL bOk = TRUE;

		TALK_NEED_ARRAY::iterator it;
		for(UINT i = 0; i < CHAT_NEED_NUMBER; ++i)
		{
			switch(m_need[i].type)
			{
			case CHAT_NEED_NONE:
				break;
			case CHAT_NEED_MP:
				{
					CObject* pObj = (CObject*)CObjectManager::GetMe()->GetMySelf();
					INT curMp = ((CObject_Character*)pObj)->GetCharacterData()->Get_MP();

					if(curMp - m_need[i].value < 0)	//��Ӧ����ֵ���޸��ڷ�����������
						bOk = FALSE;
				}
				break;
			//case CHAT_NEED_ENERGY:
			//	{
			//		CObject* pObj = (CObject*)CObjectManager::GetMe()->GetMySelf();
			//		INT curEnergy = ((CObject_Character*)pObj)->GetCharacterData()->Get_Energy();

			//		if(curEnergy - m_need[i].value < 0)	//��Ӧ����ֵ���޸��ڷ�����������
			//			bOk = FALSE;
			//	}
			//	break;
			//case CHAT_NEED_VIGOR:
			//	{
			//		CObject* pObj = (CObject*)CObjectManager::GetMe()->GetMySelf();
			//		INT curVigor = ((CObject_Character*)pObj)->GetCharacterData()->Get_Vigor();

			//		if(curVigor - m_need[i].value < 0)	//��Ӧ����ֵ���޸��ڷ�����������
			//			bOk = FALSE;
			//	}
			//	break;
			default:
				break;
			}

			if(!bOk) break;
		}

		return bOk;
	}

	BOOL Talk::Channel::TalkTimeCheck(UINT dwTimeNow)
	{
		BOOL bOk = TRUE;

		if(0 == m_lastSendTime)
		{
			m_lastSendTime = dwTimeNow;
		}
		else
		{
			UINT dwTime = CGameProcedure::s_pTimeSystem->CalSubTime(m_lastSendTime,dwTimeNow);
			if(dwTime/1000 > GetSliceTime())
			{
				m_lastSendTime = dwTimeNow;
			}
			else
			{
				bOk = FALSE;
			}
		}

		return bOk;
	}

	VOID Talk::Channel::SetNeed(INT tp, INT value)
	{
		if(tp >= CHAT_NEED_NUMBER || tp < 0) return; 
		m_need[tp].type = (CHAT_NEED_TYPE)tp;
		m_need[tp].value = value;
	}

	VOID Talk::Channel::LoadChannelConfig()
	{
		const tDataBase* pTalkConfig = CDataBaseSystem::GetMe()->GetDataBase(DBC_TALK_CONFIG);
		if(pTalkConfig)
		{
			const _DBC_TALK_CONFIG* pLine = (const _DBC_TALK_CONFIG*)pTalkConfig->Search_LineNum_EQU(m_channelType);
			if(pLine)
			{
				m_channelName = pLine->szChannelName;
				m_channelHeader = pLine->szChannelHeader;
				m_TimeSlice = pLine->nTimeSlice;

				SetNeed(pLine->nNeedType1, pLine->nNeedValue1);
				SetNeed(pLine->nNeedType2, pLine->nNeedValue2);
				SetNeed(pLine->nNeedType3, pLine->nNeedValue3);
			}
		}
	}

	VOID Talk::Channel::SetSendPacket(CGChat *packet, TalkAnalyze* pAna)
	{
		if(!packet || !pAna) return;

		packet->SetChatType(pAna->getChannelType());
		packet->SetContex(pAna->getStr(SEND_STR).c_str());
		packet->SetContexSize((BYTE)pAna->getStr(SEND_STR).size());
	}

	//PrivChannel Class
	VOID Talk::PrivateChannel::Clean()
	{
		m_GUID = INVALID_GUID;
		m_InviteQue.clear();
	}

	INT Talk::PrivateChannel::AddToQue(STRING user)
	{
		if(user.empty())
		{
			TDAssert(FALSE);
			return -1;
		}

		if(m_InviteQue.size() == CHANNEL_INVITE_USER_MAXNUM)
		{
			return 1;	//�Ѿ������������
		}

		if(0 <= IsInQue(user))
		{
			return 2;	//�Ѿ������������
		}

		m_InviteQue.push_back(user);

		return 0;
	}

	INT Talk::PrivateChannel::IsInQue(STRING user)
	{
		for(UINT i = 0; i < m_InviteQue.size(); ++i)
		{
			if(user == m_InviteQue[i]) return i;
		}
		return -1;
	}

	INT Talk::PrivateChannel::KickFromQue(STRING user)
	{
		std::deque< STRING >::iterator it;
		for(it = m_InviteQue.begin(); it != m_InviteQue.end(); it++)
		{
			if(user == *it)
			{
				m_InviteQue.erase(it);
			}
		}
		return 0;
	}

	STRING Talk::PrivateChannel::GetUserByIndex(UINT idx)
	{
		STRING strR = "";
		if(idx >= m_InviteQue.size())
		{
			return strR;
		}

		strR = m_InviteQue[idx];
		return strR;
	}

	//UserChannel Class
	INT Talk::UserChannel::AddUser(STRING user)
	{
		if(user.empty())
		{
			return -1;
		}
		
		if(0 <= IsInQue(user))
		{
			return 2;	//�Ѿ���������������
		}
		
		m_UserQue.push_back(user);

		if(m_UserQue.size() > TALK_TO_USER_MAXNUM)
		{
			m_UserQue.pop_front();	//�������������ɶ���
		}

		return 0;
	}

	INT Talk::UserChannel::IsInQue(STRING user)
	{
		for(UINT i = 0; i < m_UserQue.size(); ++i)
		{
			if(user == m_UserQue[i]) return i;
		}
		return -1;
	}

	VOID Talk::UserChannel::Clean()
	{
		m_UserQue.clear();

		m_curSelPos = -1;
	}

	STRING Talk::UserChannel::GetUserByIndex(UINT idx)
	{
		STRING strR = "";
		if(idx >= m_UserQue.size())
		{
			return strR;
		}

		strR = m_UserQue[idx];
		return strR;
	}

	STRING Talk::UserChannel::SelectFirstUser()
	{
		STRING strR = "";
		if(m_UserQue.empty())
		{
			return strR;
		}
		
		m_curSelPos = 0;
		strR = *(m_UserQue.begin());
		return strR;
	}

	STRING Talk::UserChannel::SelectLastUser()
	{
		STRING strR = "";
		if(m_UserQue.empty())
		{
			return strR;
		}

		m_curSelPos = (INT)m_UserQue.size()-1;
		strR = *(m_UserQue.rbegin());
		return strR;
	}

	STRING Talk::UserChannel::SelectNextUser(BOOL dir)
	{
		STRING strR = "";
		if(m_UserQue.empty() || m_curSelPos < 0)
		{
			return strR;
		}

		if(dir)
		{
			m_curSelPos++;
			if(m_curSelPos == (INT)m_UserQue.size())	m_curSelPos = 0;
		}
		else
		{
			m_curSelPos--;
			if(0 > m_curSelPos) m_curSelPos = (INT)m_UserQue.size()-1;
		}

		strR = m_UserQue[m_curSelPos];
		return strR;
	}

	BOOL Talk::UserChannel::SetSelectByUser(STRING name)
	{
		if(m_UserQue.empty() || name.empty())
		{
			return FALSE;
		}

		UINT i = 0;
		for(; i < m_UserQue.size(); ++i)
		{
			if(name == GetUserByIndex(i)) break;
		}

		if(i != m_UserQue.size()) 
		{
			m_curSelPos = i;
			return TRUE;
		}
		else
			return FALSE;
	}

	VOID Talk::UserChannel::SetSendPacket(CGChat *packet, TalkAnalyze* pAna)
	{
		if(!packet || !pAna) return;
		if(pAna->getChannelType() != CHAT_TYPE_TELL) return;

		Talk::Channel::SetSendPacket(packet, pAna);
		//�������Ķ���
		packet->SetTargetSize((BYTE)pAna->getStr(PRIVATE_STR).size());
		packet->SetTargetName((CHAR*)pAna->getStr(PRIVATE_STR).c_str());

		//���˽�Ķ���
		AddUser(pAna->getStr(PRIVATE_STR));
	}

	VOID Talk::TeamChannel::SetSendPacket(CGChat *packet, TalkAnalyze* pAna)
	{
		if(!packet || !pAna) return;
		if(pAna->getChannelType() != CHAT_TYPE_TEAM) return;

		Talk::Channel::SetSendPacket(packet, pAna);
		packet->SetTeamID(m_teamID);
	}

	VOID Talk::MenPaiChannel::SetSendPacket(CGChat *packet,TalkAnalyze* pAna)
	{
		if(!packet || !pAna) return;
		if(pAna->getChannelType() != CHAT_TYPE_MENPAI) return;

		Talk::Channel::SetSendPacket(packet, pAna);
		packet->SetMenpaiID((BYTE)m_menpaiID);
	}

	VOID Talk::GuildChannel::SetSendPacket(CGChat *packet, TalkAnalyze* pAna)
	{
		if(!packet || !pAna) return;
		if(pAna->getChannelType() != CHAT_TYPE_GUILD) return;

		Talk::Channel::SetSendPacket(packet, pAna);
		packet->SetGuildID(m_guildID);
	}
//---------------------------------------------------------------------------------------------------------------------
//�¼��ص�������ÿ1���ӵ���һ��
	VOID Talk::_SendTalkHelpMsg(const EVENT *pEvent, UINT dwOwnerData)
	{
		static UINT m_count = 0;

		m_count++;
		if(m_count > 300) //ÿ5����
		{
			if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
			{
				Talk::s_Talk.ShowTalkHelpMsg();
			}
			m_count = 0;
		}
	}

	VOID Talk::_HandleWindowSizeChange(const EVENT *pEvent, UINT dwOwnerData)
	{
		INT wmsg=0;
		static INT m_height = 0;

		if(pEvent->vArg.size() > 0)
		{
			wmsg = atoi(pEvent->vArg[0].c_str());

			if(wmsg == WM_SIZE || wmsg == WM_EXITSIZEMOVE)
			{
				RECT rect;
				GetClientRect(g_hMainWnd, &rect);

				if(m_height != rect.bottom-rect.top)
				{
					m_height = rect.bottom-rect.top;
					Talk::s_Talk.TalkDisplaySizeChanged(rect.right-rect.left, m_height);
				}
			}
		}
	}
//---------------------------------------------------------------------------------------------------------------------
//LuaPlus������ؽӿ�
	INT	Talk::GetChannelNumber(LuaPlus::LuaState* state)	//�����ҿ�����ΪĬ��Ƶ������Ŀ
	{
		state->PushInteger((INT)m_listChannel.size());

		return 1;
	}

	INT	Talk::GetChannel(LuaPlus::LuaState* state)
	{
		//������
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: Talk GetChannel Wrong Param1");
			return -1;
		}

		INT nChannelNum = args[2].GetInteger();

		INT nTempIndex = 0;
		std::list< Channel* >::iterator it;
		for(it=m_listChannel.begin(); nTempIndex<nChannelNum; it++, nTempIndex++);
		
		//����Ҫ��Ƶ��
		Channel* theChannel = *it;
		if(theChannel->GetType() != CHAT_TYPE_TELL)
		{
			state->PushString(ChannelType2String(theChannel->GetType()).c_str());
			state->PushString(theChannel->GetName().c_str());
			return 2;
		}
		else
		{
			state->PushString(ChannelType2String(theChannel->GetType()).c_str());

			UserChannel *UsrCh = (UserChannel*)theChannel;
			for(UINT i = 0; i < UsrCh->GetUserNum(); ++i)
				state->PushString(UsrCh->GetUserByIndex(i).c_str());

			return (INT)UsrCh->GetUserNum()+1;
		}
	}

	INT	Talk::GetChannelHeader(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsString()))
		{
			TDThrow("LUA: Talk GetChannelHeader Wrong Param1");
			return -1;
		}

		STRING strType = args[2].GetString();
		ENUM_CHAT_TYPE type = String2ChannelType(strType);

		/* Bug:Maybe Channel not Create when Lua call this func.
		if(type != CHAT_TYPE_INVALID)
		{
			std::list< Channel* >::iterator it;
			for(it=m_listChannel.begin(); it!=m_listChannel.end(); it++)
			{
				if((*it)->GetType() == type)
				{
					state->PushString((*it)->GetHeader().c_str());
					break;
				}
			}
		}
		*/

		const tDataBase* pTalkConfig = CDataBaseSystem::GetMe()->GetDataBase(DBC_TALK_CONFIG);
		if(pTalkConfig)
		{
			const _DBC_TALK_CONFIG* pLine = (const _DBC_TALK_CONFIG*)pTalkConfig->Search_LineNum_EQU(type);
			if(pLine)
			{
				state->PushString(pLine->szChannelHeader);
			}
		}

		return 1;
	}

	INT	Talk::SendChatMessage(LuaPlus::LuaState* state)
	{
		//������
		LuaStack args(state);
		if (!(args[2].IsString() && args[3].IsString()))
		{
			TDThrow("LUA: Talk GetChannel Wrong Param1 or Param2");
			return 0;
		}

		//check procedure
		if(CGameProcedure::GetActiveProcedure() != (CGameProcedure*)CGameProcedure::s_pProcMain) return 0;

		STRING strTalkCh = args[2].GetString();
		STRING strTalkMsg = args[3].GetString();

		//���õ�������Ϣ�����ٶȿ��Ʊ���
		INT bSend = SEND_FAILED_NOREASON;	//�Ƿ���

		//�����ַ�������
		SendTalkAnalyze saN(String2ChannelType(strTalkCh), strTalkMsg);
		saN.doAnalyze(STRING(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name()));
		if(!saN.IsOk())
		{
            if(!(saN.getErrStr().empty()))
			{
				ADDTALKMSG(saN.getErrStr());
			}
			return 0;
		}

		//GM�������
		if(saN.getStrType() == STRINGTYPE_GM)
		{
			SendGMCommand(saN.getStr(ORGINAL_STR).c_str());
			return 0;
		}

		//����Ƶ���Ƿ����
		Talk::Channel *pChannel = NULL;
		std::list<Talk::Channel *>::iterator it = m_listChannel.begin();
		for(; it != m_listChannel.end(); it++)
		{
			if((*it)->GetType() == saN.getChannelType())
			{
				pChannel = *it;
				break;
			}
		}

		if(NULL == pChannel) return 0;

		//�������͹��첻ͬ����Ϣ����
		CGChat   msg;
		pChannel->SetSendPacket(&msg, &saN);

		ENUM_CHAT_TYPE chatType = (ENUM_CHAT_TYPE)msg.GetChatType();

		//����Ƶ��������ʱ������
		if(pChannel->isClose())
			bSend = SEND_FAILED_LV10;
		else if(!(pChannel->TalkNeedCheck()))
			bSend = SEND_FAILED_NEED;
		else
		{
			if(pChannel->TalkTimeCheck(CGameProcedure::s_pTimeSystem->GetTimeNow()))
				bSend = SEND_CHAT_OK;
			else
				bSend = SEND_FAILED_TIME_SLICE;
		}

		//���˽�Ķ����ǲ��Ǳ�������
		if(chatType == CHAT_TYPE_TELL)
		{
			if(IsInPingBi(STRING(msg.GetTargetName())))
			{
				bSend = SEND_FAILED_PINGBI;
			}
		}

		if(SEND_CHAT_OK == bSend)
		{
			CNetManager::GetMe()->SendPacket(&msg);

			//����������ʷ��¼
			HistoryMsg hmsg;
			hmsg.SetChannelType(chatType);
			hmsg.SetMsgName(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name());
			hmsg.SetMsgData(saN.getStr(HISTORY_STR));
			hmsg.SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());

			AddToSendHistoryQue(hmsg);

			//������Ϣ��Ҫ�Լ���ӵ��Լ������촰����ʾ
			if(chatType == CHAT_TYPE_TELL)
			{
				STRING strType = ChannelType2String(hmsg.GetChannelType());
				
				std::vector< STRING > strParam;
				strParam.push_back(strType);
				strParam.push_back(msg.GetTargetName());	//˽�ĵ����ƴ���ʾ�Է�������
				strParam.push_back(TalkFixTail(STRING(msg.GetContex())));
				
				if(CEventSystem::GetMe())
				{
					CEventSystem::GetMe()->PushEvent(GE_CHAT_MESSAGE, strParam);
				}
				
				//�޸ĳ���ȷ��������Ϣ
				hmsg.SetMsgName(msg.GetTargetName());
				hmsg.SetMsgData(msg.GetContex());

				//������ʷ��Ϣ
				AddToRecvHistoryQue(hmsg);
			}
		}
		else
		{
			if(bSend == SEND_FAILED_LV10)
			{
				ADDTALKMSG(STRING("#R�ȼ������������ڴ�Ƶ��˵����"));
			}
			else if(bSend == SEND_FAILED_NEED)
			{
				ADDTALKMSG(STRING("#R�������㣬����˵����"));
			}
			else if(bSend == SEND_FAILED_PINGBI)
			{
				STRING strTemp = "";
				strTemp = COLORMSGFUNC("Talk_Obj_Shield", msg.GetTargetName());
				ADDTALKMSG(strTemp);
			}
			else
			{
				ADDTALKMSG(STRING("#R�벻Ҫ˵���ٶ�̫�죡#Bлл��"));
			}
		}

		//������Ϣ��Ҫ��ӻ����봰�ڣ���������´μ�������
		if(chatType == CHAT_TYPE_TELL)
		{
			state->PushString(msg.GetTargetName());
			return 1;
		}
	
		return 0;
	}

	INT Talk::CreateChannel(LuaPlus::LuaState *state)
	{
		
		if(NULL != m_PrvCh)
		{
			return 1;				//�Ѿ��������Խ�Ƶ��
		}

		GUID_t myGUID = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();	

		CGChannelCreate msg;
		msg.SetGUID(myGUID);
		CNetManager::GetMe()->SendPacket(&msg);

		return 0;
	}

	INT Talk::JoinUserToChannel(LuaPlus::LuaState *state)
	{
		if(NULL == m_PrvCh)
		{
			return 1;				//�������Խ�Ƶ��
		}

		LuaStack args(state);
		if(!(args[2].IsString()))
		{
			TDThrow("LUA: Talk JoinUserToChannel Wrong Param1");
			return -1;
		}

		STRING user = args[2].GetString();

		return 0;
	}

	INT Talk::KickUserFromChannel(LuaPlus::LuaState *state)
	{
		if(NULL == m_PrvCh)
		{
			return 1;				//�������Խ�Ƶ��
		}
		
		LuaStack args(state);
		if(!(args[2].IsString()))
		{
			TDThrow("LUA: Talk KickUserFromChannel Wrong Param1");
			return -1;
		}

		STRING user = args[2].GetString();

		return 0;
	}

	INT Talk::DestoryChannel(LuaPlus::LuaState *state)
	{

		if(NULL == m_PrvCh)
		{
			return 1;				//�������Խ�Ƶ��
	}
	
		GUID_t myGUID = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();	
		
		CGChannelDismiss msg;
		msg.SetGUID(myGUID);
		CNetManager::GetMe()->SendPacket(&msg);

		return 0;
	}

	INT	Talk::InsertHistory(LuaPlus::LuaState *state)
	{
		LuaStack args(state);
		if(!(args[2].IsInteger()))
		{
			TDThrow("LUA: Talk InsertHistory Wrong Param1");
			return -1;
		}

		INT nIdx = args[2].GetInteger();
		STRING strConfig = args[3].GetString();

		INT idx = 0;
		//CHAR szHis[128] = {0};

		switch(nIdx)
		{
		case 0:	//ȫ����Ϣ
			{
				std::deque< HistoryMsg >::iterator it = m_RecvHisQue.begin();

				for(;it != m_RecvHisQue.end();it++)
				{
					STRING strType = ChannelType2String((*it).GetChannelType());
					CHAR szIdx[16] = {0};
					_snprintf(szIdx,15,"%d", idx);

					std::vector< STRING > strParam;
					strParam.push_back(strType);
					strParam.push_back((*it).GetMsgName());
					strParam.push_back((*it).GetMsgData());
					strParam.push_back(szIdx);
					
					if(CEventSystem::GetMe())
					{
						CEventSystem::GetMe()->PushEvent(GE_CHAT_MESSAGE, strParam);
					}
					idx++;

					//_snprintf(szHis, 127, "HistoryMsg(%d):(%s)%s\n", idx, (*it).GetMsgName().c_str(),(*it).GetMsgData().c_str());
					//OutputDebugString(szHis);
				}
			}
			break;
		case 1:	//ϵͳ��˽�ġ����
			/*
			{
				std::deque< HistoryMsg >::iterator it = m_RecvHisQue.begin();
				for(;it != m_RecvHisQue.end();it++)
				{
					if((*it).GetChannelType() == CHAT_TYPE_SYSTEM ||
					   (*it).GetChannelType() == CHAT_TYPE_SELF ||
					   (*it).GetChannelType() == CHAT_TYPE_TEAM)
					{
						STRING strType = ChannelType2String((*it).GetChannelType());
						
						std::vector< STRING > strParam;
						strParam.push_back(strType);
						strParam.push_back((*it).GetMsgName());
						strParam.push_back((*it).GetMsgData());
						
						if(CEventSystem::GetMe())
						{
							CEventSystem::GetMe()->PushEvent(GE_CHAT_MESSAGE, strParam);
						}
					}
				}
			}
			break;
			*/
		case 2:
		case 3:
		case 4:
			//�Խ�Ƶ������Ҫ������Ӧ�������ִ�����ѡ���Ե������Ϣ
			{
				INT icfg[CHAT_TYPE_NUMBER];
				for(INT i = 0; i < CHAT_TYPE_NUMBER; ++i) icfg[i] = 0;
				//getconfig
				for(UINT i = 0; i < strConfig.size() && i < CHAT_TYPE_NUMBER; ++i) icfg[i] = (strConfig.at(i) == '1')?1:0;

				std::deque< HistoryMsg >::iterator it = m_RecvHisQue.begin();
				for(; it != m_RecvHisQue.end();it++)
				{
					if(icfg[(*it).GetChannelType()] == 1)
					{
						STRING strType = ChannelType2String((*it).GetChannelType());
						CHAR szIdx[16] = {0};
						_snprintf(szIdx,15,"%d", idx);

						std::vector< STRING > strParam;
						strParam.push_back(strType);
						strParam.push_back((*it).GetMsgName());
						strParam.push_back((*it).GetMsgData());
						strParam.push_back(szIdx);
						
						if(CEventSystem::GetMe())
						{
							CEventSystem::GetMe()->PushEvent(GE_CHAT_MESSAGE, strParam);
						}
					}
					idx++;
				}
			}
			break;
		default:
			break;
		}

		return 0;
	}

	INT	Talk::SetMaxSaveNumber(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsInteger()))
		{
			TDThrow("LUA: Talk SetMaxSaveNumber Wrong Param1");
			return -1;
		}

		INT maxNum = args[2].GetInteger();
		if(0 < maxNum) m_MaxSaveNum = maxNum;

		return 0;
	}

	INT	Talk::CreateTab(LuaPlus::LuaState* state)
	{
		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_TAB_CREATE);
		}

		return 0;
	}

	INT	Talk::ConfigTab(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsString()))
		{
			TDThrow("LUA: Talk ConfigTab Wrong Param1");
			return -1;
		}

		if(!(args[3].IsString()))
		{
			TDThrow("LUA: Talk ConfigTab Wrong Param2");
			return -1;
		}

		std::vector< STRING > strParam;
		strParam.push_back(args[2].GetString());
		strParam.push_back(args[3].GetString());

		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_TAB_CONFIG, strParam);
		}

		return 0;
	}

	INT Talk::CreateTabFinish(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsString()) || !(args[3].IsString()) || !(args[4].IsString()))
		{
			TDThrow("LUA: Talk CreateTabFinish Wrong Param");
			return -1;
		}

		STRING strChannelName = args[2].GetString();
		std::vector< STRING > strParam;
		strParam.push_back(args[2].GetString());
		strParam.push_back(args[3].GetString());
		strParam.push_back(args[4].GetString());

		//�����ַ�����
		if(!CGameProcedure::s_pUISystem->CheckStringFilter(strChannelName))
		{
			STRING strErr = COLORMSGFUNC("InvalidSpecialString");
			ADDTALKMSG(strErr);
			strParam[2] = "cancel";
		}

		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_TAB_CREATE_FINISH, strParam);
		}

		return 0;
	}

	INT Talk::ConfigTabFinish(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsString()) || !(args[3].IsString()) || !(args[4].IsString()))
		{
			TDThrow("LUA: Talk ConfigTabFinish Wrong Param");
			return -1;
		}

		std::vector< STRING > strParam;
		strParam.push_back(args[2].GetString());
		strParam.push_back(args[3].GetString());
		strParam.push_back(args[4].GetString());
		
		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_TAB_CONFIG_FINISH, strParam);
		}

		return 0;
	}

	INT Talk::SaveTab(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsInteger()))
		{
			TDThrow("LUA: Talk SaveTab Wrong Param1");
			return -1;
		}
		if(!(args[3].IsString()))
		{
			TDThrow("LUA: Talk SaveTab Wrong Param2");
			return -1;
		}
		if(!(args[4].IsString()))
		{
			TDThrow("LUA: Talk SaveTab Wrong Param3");
			return -1;
		}

		INT tabId = args[2].GetInteger();			//tab����
		STRING tabName = args[3].GetString();		//tab����
		STRING tabConfig = args[4].GetString();		//tab����
        
		//1~4Ϊ����Խ�tabҳ��
		if(tabId < 1 || tabId > 4) return 0;
		//tabҳ���������6��ANSI�ַ���
		if(tabName.empty() || tabName.size() > 6) return 0;
		//tabҳ�����ñ���Ϊ9����0/1��ɵ��ִ�����
		if(tabConfig.size() != CHAT_TYPE_NUMBER) return 0;
		for(UINT i=0; i< tabConfig.size(); ++i)
		{
			if(tabConfig.at(i) != 48 && tabConfig.at(i) != 49) return 0;
		}

		TDAssert(5 == sizeof(_OWN_SETTING));

		CHAR szTabName[6+1] = {0};
		strcpy(szTabName,tabName.c_str());
		//��������
		CGModifySetting pk1;
		pk1.SetType(SETTING_TYPE_CHAT_TAB1_PART1+2*(tabId-1));
		_OWN_SETTING *pSetting1 = pk1.GetValue();
		pSetting1->CleanUp();
		memcpy(pSetting1, szTabName, sizeof(_OWN_SETTING));

		CGModifySetting pk2;
		pk2.SetType(SETTING_TYPE_CHAT_TAB1_PART2+2*(tabId-1));
		_OWN_SETTING *pSetting2 = pk2.GetValue();
		pSetting2->CleanUp();
		memcpy(&(pSetting2->m_SettingType), szTabName+sizeof(_OWN_SETTING), sizeof(BYTE));
		for(UINT i=0; i< tabConfig.size(); ++i)
		{
			if(tabConfig.at(i) != 48) pSetting2->m_SettingData |= 1<<i;
		}

		//���͵�������
		CNetManager::GetMe()->SendPacket(&pk1);
		CNetManager::GetMe()->SendPacket(&pk2);

		return 0;
	}

	INT Talk::ClearTab(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsInteger()))
		{
			TDThrow("LUA: Talk ClearTab Wrong Param1");
			return -1;
		}

		INT tabId = args[2].GetInteger();
		if(tabId < 1 || tabId > 4) return 0;
        
		CGModifySetting pk1;
		pk1.SetType(SETTING_TYPE_CHAT_TAB1_PART1+2*(tabId-1));
		pk1.GetValue()->CleanUp();

		CGModifySetting pk2;
		pk2.SetType(SETTING_TYPE_CHAT_TAB1_PART2+2*(tabId-1));
		pk2.GetValue()->CleanUp();

		CNetManager::GetMe()->SendPacket(&pk1);
		CNetManager::GetMe()->SendPacket(&pk2);

		return 0;
	}

	INT Talk::GetChannelType(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsString()))
		{
			TDThrow("LUA: Talk GetChannelType Wrong Param1");
			return -1;
		}

		STRING strType = args[2].GetString();

		state->PushInteger((INT)String2ChannelType(strType));
		return 1;
	}

	INT	Talk::SelectFaceMotion(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsString()))
		{
			TDThrow("LUA: Talk SelectFaceMotion Wrong Param1");
			return -1;
		}

		if(!(args[3].IsString()))
		{
			TDThrow("LUA: Talk SelectFaceMotion Wrong Param2");
			return -1;
		}

		std::vector< STRING > strParam;
		strParam.push_back(args[2].GetString());
		strParam.push_back(args[3].GetString());

		if(0 == strcmp(args[2].GetString(), "sucess"))
		{
			STRING strColor = args[3].GetString();
			for(INT i = 0; i < (INT)strColor.size(); ++i)
			{
				PostMessage(g_hMainWnd, WM_CHAR, (WPARAM)strColor.at(i),0);
			}
			return 0;
		}

		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_FACEMOTION_SELECT, strParam);
		}

		return 0;
	}

	INT	Talk::SelectTextColor(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsString()))
		{
			TDThrow("LUA: Talk SelectTextColor Wrong Param1");
			return -1;
		}

		if(!(args[3].IsString()))
		{
			TDThrow("LUA: Talk SelectTextColor Wrong Param2");
			return -1;
		}

		std::vector< STRING > strParam;
		strParam.push_back(args[2].GetString());
		strParam.push_back(args[3].GetString());

		if(0 == strcmp(args[2].GetString(), "sucess"))
		{
			STRING strColor = args[3].GetString();
			for(INT i = 0; i < (INT)strColor.size(); ++i)
			{
				PostMessage(g_hMainWnd, WM_CHAR, (WPARAM)strColor.at(i),0);
			}
			return 0;
		}

		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_TEXTCOLOR_SELECT, strParam);
		}

		return 0;
	}

	INT Talk::SaveOldTalkMsg(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsString()) || !(args[3].IsString()))
		{
			TDThrow("LUA: Talk SaveOldTalkMsg Wrong Param");
			return -1;
		}

		STRING strTalkCh = args[2].GetString();
		STRING strMsg = args[3].GetString();
		
		if(strMsg.empty()) return 0;
		//if(strMsg.at(0) == '@') return 0;
		//if(IsActCommand(strMsg.c_str())) return 0;

		HistoryMsg hmsg;
		hmsg.SetChannelType(String2ChannelType(strTalkCh));
		hmsg.SetMsgName(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name());
		hmsg.SetMsgData(strMsg);
		hmsg.SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());

		AddToSendHistoryQue(hmsg);

		return 0;
	}

	INT Talk::ChangeCurrentChannel(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsString()))
		{
			TDThrow("LUA: Talk ChangeCurrentChannel Wrong Param1");
			return -1;
		}

		if(!(args[3].IsString()))
		{
			TDThrow("LUA: Talk ChangeCurrentChannel Wrong Param2");
			return -1;
		}

		if(!(args[4].IsInteger()))
		{
			TDThrow("LUA: Talk ChangeCurrentChannel Wrong Param3");
			return -1;
		}


		STRING strCh = args[2].GetString();
		STRING strName = args[3].GetString();

		INT step = args[4].GetInteger();
		step = (0 >= step)?-1:1;

		ENUM_CHAT_TYPE curtype = String2ChannelType(strCh);
		if(CHAT_TYPE_INVALID != curtype)
		{
			std::list<Talk::Channel *>::iterator it = m_listChannel.begin();
			for(;it != m_listChannel.end(); it++)
			{
				if(curtype == (*it)->GetType()) break;
			}

			if(it != m_listChannel.end()) //lua�ű���������Ƶ��һ��Ҫ�Ϸ�
			{
				std::list<Talk::Channel *>::iterator itfin;

				if(CHAT_TYPE_TELL == curtype && m_UsrCh) //��ǰѡ����ǡ����ġ�Ƶ��
				{
					if(m_UsrCh->IsLastUserSelect() && step >= 0)
					{
						itfin = ++it;
						if(itfin == m_listChannel.end()) itfin = m_listChannel.begin();
					}
					else if(m_UsrCh->IsFirstUserSelect() && step < 0)
					{
						if(it == m_listChannel.begin())
						{
							std::list<Talk::Channel *>::iterator itend = m_listChannel.end();
							itfin = --itend;
						}
						else 
							itfin = --it;
					}
					else
					{
						itfin = it;
					}

					if(CEventSystem::GetMe())
					{
						std::vector< STRING > strParam;
						strParam.push_back(ChannelType2String((*itfin)->GetType()));

						if(CHAT_TYPE_TELL == (*itfin)->GetType())
							strParam.push_back(m_UsrCh->SelectNextUser(step));
						else
							strParam.push_back((*itfin)->GetName());
							
						//return value for lua.
						for(int i=0; i < 2; ++i)
							state->PushString(strParam[i].c_str());
					}//end of if(CEventSystem::GetMe())

				}
				else if(CHAT_TYPE_TELL != curtype) //��ǰѡ��Ĳ��ǡ����ġ�Ƶ��
				{
					if(step >= 0)
					{
						itfin = ++it;
						if(itfin == m_listChannel.end()) itfin = m_listChannel.begin();
					}
					else if(step < 0)
					{
						if(it == m_listChannel.begin())
						{
							std::list<Talk::Channel *>::iterator itend = m_listChannel.end();
							itfin = --itend;
						}
						else 
							itfin = --it;
					}

					if(CEventSystem::GetMe())
					{
						std::vector< STRING > strParam;
						strParam.push_back(ChannelType2String((*itfin)->GetType()));

						if(CHAT_TYPE_TELL == (*itfin)->GetType() && step >= 0)
							strParam.push_back(m_UsrCh->SelectFirstUser());
						else if(CHAT_TYPE_TELL == (*itfin)->GetType() && step < 0)
							strParam.push_back(m_UsrCh->SelectLastUser());
						else
							strParam.push_back((*itfin)->GetName());
							
						//return value for lua.
						for(int i=0; i < 2; ++i)
							state->PushString(strParam[i].c_str());
					}//end of if(CEventSystem::GetMe())
				}
			}// end of if(it != m_listChannel.end())
			else
			{
				TDThrow("LUA: Talk ChangeCurrentChannel Wrong Current Channel Send from Lua!<2>");
				return -1;
			}
		}

		return 2;
	}

	INT Talk::ModifyChatTxt(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsString()) || !(args[3].IsString()) || !(args[4].IsString()))
		{
			TDThrow("LUA: Talk ModifyChatTxt Wrong Param");
			return -1;
		}

		STRING strType = args[2].GetString();
		STRING strName = args[3].GetString();
		STRING strOldTxt = args[4].GetString();

		STRING strNewTxt = strOldTxt;
		//remove msg head.
		if(!strOldTxt.empty())
		{
			if(strOldTxt.at(0) == '/' && strOldTxt.at(1) != '/')
			{
				if(STRING::npos == strOldTxt.find_first_of(" "))
					strNewTxt = "";
				else
					strNewTxt = strOldTxt.substr(strOldTxt.find_first_of(" ")+1);
			}

			if(2 > strOldTxt.size())
				strNewTxt = "";
		}

		ENUM_CHAT_TYPE type = String2ChannelType(strType);

		if(CHAT_TYPE_TELL == type)
		{
			//set selected user idx in m_UsrCh.
			if(m_UsrCh)
			{
				for(UINT i=0; i < m_UsrCh->GetUserNum(); ++i)
				{
					if(strName == m_UsrCh->GetUserByIndex(i))
					{
						m_UsrCh->SetSelectByUser(strName);
						break;
					}
				}
			}
			
			STRING strFinal = "/" + strName + " " +strNewTxt;
			state->PushString(strFinal.c_str());
		}
		else
			state->PushString(strNewTxt.c_str());

		return 1;
	}

	INT	Talk::ShowContexMenu(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsString()))
		{
			TDThrow("LUA: Talk ShowContexMenu Wrong Param1");
			return -1;
		}

		m_PrevMenuLink = args[2].GetString();
		if(m_PrevMenuLink.empty())
			return 0;

		POINT pt = CInputSystem::GetMe()->MouseGetPos();
		CHAR szXPos[32]; _snprintf(szXPos, 32, "%d", pt.x);
		CHAR szYPos[32]; _snprintf(szYPos, 32, "%d", pt.y);

		std::vector< STRING > vParam;

		vParam.push_back("chat_private");
		vParam.push_back("-1");
		vParam.push_back(szXPos);
		vParam.push_back(szYPos);
		CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);

		return 0;
	}

	INT	Talk::ContexMenuTalk(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsString()))
		{
			if(m_PrevMenuLink.empty()) return 0;
		}
		else if(args[2].IsString())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_CHANGE_PRIVATENAME, args[2].GetString());
			return 0;
		}

		CEventSystem::GetMe()->PushEvent(GE_CHAT_CHANGE_PRIVATENAME, m_PrevMenuLink.c_str());
		m_PrevMenuLink.erase();

		return 0;
	}
	INT Talk::GetCurrentSelectName( LuaPlus::LuaState* state )
	{
		state->PushString( m_PrevMenuLink.c_str() );
		return 1;
	}

	INT Talk::HandleMenuAction(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsString()))
		{
			TDThrow("LUA: Talk HandleMenuAction Wrong Param1");
			return -1;
		}

		STRING strOp = args[2].GetString();

		if(strOp == "Name")
		{
			if(m_PrevMenuLink.empty()) return 0;
			state->PushString(m_PrevMenuLink.c_str());
		}
		else if(strOp == "PingBi")
		{
			if(m_PrevMenuLink.empty()) return 0;
			AddPingBi(m_PrevMenuLink);
		}
		m_PrevMenuLink.erase();

		return 1;
	}

	INT Talk::GetTalkTemplateString_Lua(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsString()))
		{
			TDThrow("LUA: Talk GetTalkTemplateString_Lua Wrong Param1");
			return -1;
		}
		if(!(args[3].IsString()))
		{
			TDThrow("LUA: Talk GetTalkTemplateString_Lua Wrong Param2");
			return -1;
		}

		STRING strTalker = args[2].GetString();
		STRING strContex = args[3].GetString();

		LuaTalkAnalyze luN(CHAT_TYPE_INVALID, strContex);
		luN.doAnalyze(strTalker);

		if(!(luN.IsOk()))
		{
			if(!(luN.getErrStr().empty())) 
			{
				ADDTALKMSG(luN.getErrStr());
			}
			return 0;
		}

		state->PushString(luN.getStr(TALK_STR).c_str());

		return 1;
	}

	INT Talk::EnumChatMood(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		//������
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: EnumChatMood Wrong Param1");
		}

		INT nIndex = args[2].GetInteger();

		//��ѯ����
		if(nIndex >= 0)
		{
			tActionItem* pActionItem = CActionSystem::GetMe()->EnumAction(nIndex , NAMETYPE_CHATMOOD);
			if(pActionItem)
			{
				LuaObject objReturn = state->BoxPointer(pActionItem);
				objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
				objReturn.PushStack();

				return 1;
			}
		}

		//�Ƿ�ActionItem
		LuaObject objReturn = state->BoxPointer(&(CActionItem::s_InvalidAction));
		objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
		objReturn.PushStack();

		return 1;
	}

	INT Talk::ShowChatMood(LuaPlus::LuaState* State)
	{
		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_ACT_SELECT);
		}

		return 0;
	}

	INT Talk::HideChatMood(LuaPlus::LuaState* State)
	{
		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_ACT_HIDE);
		}

		return 0;
	}

	INT Talk::ShowPingBi(LuaPlus::LuaState* State)
	{
		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_PINGBI_LIST);
		}
		return 0;
	}

	INT Talk::GetPingBiNum(LuaPlus::LuaState* State)
	{
		State->PushInteger((INT)m_PingBiQue.size());
		return 1;
	}

	INT Talk::GetPingBiName(LuaPlus::LuaState* State)
	{
		LuaStack args(State);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: GetPingBiName Wrong Param1");
		}

		INT lidx = args[2].GetInteger();

		if(lidx > (INT)m_PingBiQue.size() || lidx < 0) return 0;
		if(m_PingBiQue.empty()) return 0;

		State->PushString(m_PingBiQue.at(lidx).c_str());

		return 1;
	}

	INT Talk::DelPingBi(LuaPlus::LuaState* State)
	{
		LuaStack args(State);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: GetPingBiName Wrong Param1");
		}

		INT lidx = args[2].GetInteger();

		if(lidx > (INT)m_PingBiQue.size() || lidx < 0) return 0;
		if(m_PingBiQue.empty()) return 0;

		DelPingBi(m_PingBiQue.at(lidx));

		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_PINGBI_UPDATE);
		}

		return 0;
	}

	INT Talk::GetCurInputLanguage(LuaPlus::LuaState* state)
	{
		INT imeState = CGameProcedure::s_pUISystem->GetCurrentIMEState();
		
		if(1 == imeState)
		{
			state->PushInteger(2052);
		}
		else if(2 == imeState)
		{
			state->PushInteger(1033);
		}
		else
		{
			state->PushInteger(1);
		}

		return 1;
	}
//---------------------------------------------------------------------------------------------------------------------
//���紦����ؽӿ�
	INT Talk::HandleRecvTalkPacket(GCChat *pPacket)
	{
		if( NULL == pPacket)
		{
			TDAssert(FALSE);
			return -1;
		}

		// ����������ں����������ʾ
		if( CDataPool::GetMe()->GetRelation()->IsBlackName( pPacket->GetSourName() ) )
		{
			return 0;
		}

		// ����������������б������ʾ
		if(IsInPingBi(STRING(pPacket->GetSourName())))
		{
			return 0;
		}

		//_CAMP_DATA Camp_A, Camp_B;
		//Camp_A.m_nCampID = pPacket->GetSourCamp();
		//Camp_B.m_nCampID = CObjectManager::GetMe()->GetMySelf()->GetCampData()->m_nCampID;

		ENUM_RELATION eCampType = CalcRelationType( pPacket->GetSourCamp(), 
			CObjectManager::GetMe()->GetMySelf()->GetCampData()->m_nCampID,
			CGameProcedure::s_pCampDataMgr );
		if( eCampType != RELATION_FRIEND )
		{
			return 0;
			//CHAR* pContex = pPacket->GetContex();
			//for( INT i = 0; i < pPacket->GetContexSize(); i ++ )
			//{
			//	pContex[i] = g_Disordered_String[rand()%20];
			//}
		}

		HistoryMsg msg;
		if( 0 == msg.SetByPacket(pPacket))
		{

			STRING strType = ChannelType2String(msg.GetChannelType());
			
			RecvTalkAnalyze reN(msg.GetChannelType(), msg.GetMsgData());
			reN.doAnalyze(msg.GetMsgName());

			if(!reN.IsOk())
			{
				if(!(reN.getErrStr().empty())) 
				{
					ADDTALKMSG(reN.getErrStr());
				}
				return 0;
			}

			if(!HandleSpecialSelfString(&reN))
			{
				std::vector< STRING > strParam;
				strParam.push_back(ChannelType2String((ENUM_CHAT_TYPE)reN.getChannelType()));
				strParam.push_back(reN.getTalker());
				strParam.push_back(reN.getStr(TALK_STR));
				
				if(CEventSystem::GetMe())
				{
					CEventSystem::GetMe()->PushEvent(GE_CHAT_MESSAGE, strParam);
					if(reN.getChannelType() == CHAT_TYPE_SCENE)
					{
						CHAR strMsg[MAX_SYSCHAT_SIZE]={0};
						sprintf(strMsg,"%s˵:%s",reN.getTalker().c_str(),reN.getStr(TALK_STR).c_str());
						CEventSystem::GetMe()->PushEvent(GE_NEW_GONGGAOMESSAGE,strMsg);

					}
					
				}

				msg.SetChannelType((ENUM_CHAT_TYPE)reN.getChannelType());
				msg.SetMsgData(reN.getStr(HISTORY_STR));
				//������ʷ��Ϣ
				AddToRecvHistoryQue(msg);

				//������Ϣ��ʾ
				if(!(reN.getStr(PAOPAO_STR).empty()))
				{
					INT extLength = 0;
					STRING strContext = "#cCC99FF[����]#W";
					switch(reN.getChannelType()) {
					case CHAT_TYPE_TEAM:
						extLength += (INT)(strContext.size());
						break;
					default:
						break;
					}
					SetPaoPaoTxt(reN.getTalker(), reN.getStr(PAOPAO_STR), reN.getStrType(), extLength);
				}

				//���춯�����Ҫ����������Ķ���
				if(reN.getStrType() == STRINGTYPE_ACT)
				{
					STRING strBackUp = reN.getStr(ORGINAL_STR);
					STRING strAct = strBackUp.substr(strlen(CHAT_ACT_HEADER), strBackUp.find(" ")-strlen(CHAT_ACT_HEADER));
					DoAct(strAct.c_str(), reN.getTalker().c_str());
				}
			}
		}

		return 0;
	}

	/*
	HandleSpecialSelfString					�������˽ű������õ����������Ϣ
	msg:
		"@*;flagadd;100.0;103.0;tipmsg"		��ӷ����
		"@*;flashadd;150.0;103.0;tipmsg"	��������
		"@*;flagdel;100.0;103.0;tipmsg"		ɾ�������
		"@*;flashdel;150.0;103.0;tipmsg"	ɾ�������
		"@*;npcpaopao;idServer;idTable"		��������
	 */
	BOOL Talk::HandleSpecialSelfString(TalkAnalyze* pAna)
	{
		if(!pAna) return FALSE;
		if(pAna->getChannelType() != CHAT_TYPE_SELF && pAna->getChannelType() != CHAT_TYPE_SYSTEM) return FALSE;
		
		BOOL bHandle = TRUE;

		switch(pAna->getStrType()) 
		{
		case STRINGTYPE_FLAGADD:
			//��ӷ����
			HandleFlagAdd(pAna->getStr(ORGINAL_STR).c_str());
			break;
		case STRINGTYPE_FLAGDEL:
			//ɾ�������
			HandleFlagDel(pAna->getStr(ORGINAL_STR).c_str());
			break;
		case STRINGTYPE_FLASHADD:
			//��������
			HandleFlashAdd(pAna->getStr(ORGINAL_STR).c_str());
			break;
		case STRINGTYPE_FLASHDEL:
			//ɾ�������
			HandleFlashDel(pAna->getStr(ORGINAL_STR).c_str());
			break;
		case STRINGTYPE_FLAG_NPC_ADD:	
			//��NPC���������ָʾ��
			HandleFlagNPCAdd(pAna->getStr(ORGINAL_STR).c_str());
			break;
		case STRINGTYPE_FLAG_POS_ADD:
			//��x,z�����ָʾ��
			HandleFlagPOSAdd(pAna->getStr(ORGINAL_STR).c_str());
			break;
		case STRINGTYPE_FLASH_NPC_ADD:	
			//��NPC��������ӻ�ɫָʾ��
			HandleFlashNPCAdd(pAna->getStr(ORGINAL_STR).c_str());
			break;
		case STRINGTYPE_FLASH_POS_ADD:	
			//��x,z����ӻ�ɫָʾ��
			HandleFlashPOSAdd(pAna->getStr(ORGINAL_STR).c_str());
			break;
		case STRINGTYPE_MONSTER_PAOPAO:
			//��������
			HandleMonsterPaoPao(pAna->getStr(ORGINAL_STR).c_str());
			break;
		case STRINGTYPE_SERVERMSG_TALK:
			//�������˴��͹�������ʾ��Ϣ
			HandleServerMsgTalk(pAna->getStr(ORGINAL_STR).c_str(),pAna->getChannelType());
			break;
		default:
			bHandle = FALSE;
			break;
		}

		return bHandle;
	}

	VOID Talk::HandleFlagAdd(LPCTSTR msg)
	{
		std::vector<std::string> vCmd;
		INT nNumber = DBC::DBCFile::_ConvertStringToVector(msg,vCmd,";",TRUE,TRUE);

		std::list< MAP_POS_DEFINE >* pActiveList = CWorldManager::GetMe()->GetObjectListByClass(5);
		if(!pActiveList) return;

		if(nNumber != 6)
		{
			TDThrow("Talk HandleSpecialSelfString: Not Enough StringRule.");
			return;
		}

		float x,z;
		DWORD sceneId;
		sscanf(vCmd[SELF_SCENEID].c_str(), " %u", &sceneId);
		sscanf(vCmd[SELF_X].c_str(), " %f", &x);
		sscanf(vCmd[SELF_Z].c_str(), " %f", &z);

		MAP_POS_DEFINE data;
		data.dwSceneID = sceneId;
		data.pos.x = x;
		data.pos.z = z;
		_snprintf(data.name, sizeof(data.name),"%s", vCmd[SELF_TIP].c_str());

		pActiveList->push_back(data);
	}

	VOID Talk::HandleFlagDel(LPCTSTR msg)
	{
		std::vector<std::string> vCmd;
		INT nNumber = DBC::DBCFile::_ConvertStringToVector(msg,vCmd,";",TRUE,TRUE);

		std::list< MAP_POS_DEFINE >* pActiveList = CWorldManager::GetMe()->GetObjectListByClass(5);
		if(!pActiveList) return;

		if(nNumber != 6)
		{
			TDThrow("Talk HandleSpecialSelfString: Not Enough StringRule.");
			return;
		}

		float x,z;
		DWORD sceneId;
		sscanf(vCmd[SELF_SCENEID].c_str(), " %u", &sceneId);
		sscanf(vCmd[SELF_X].c_str(), " %f", &x);
		sscanf(vCmd[SELF_Z].c_str(), " %f", &z);

		std::list< MAP_POS_DEFINE >::iterator it = pActiveList->begin();
		while(it != pActiveList->end())
		{
			if(x == (*it).pos.x && z == (*it).pos.z && sceneId == (*it).dwSceneID)
			{
				pActiveList->erase(it);
				it = pActiveList->begin();
				continue;
			}

			it++;
		}
	}

	VOID Talk::HandleFlashAdd(LPCTSTR msg)
	{
		std::vector<std::string> vCmd;
		INT nNumber = DBC::DBCFile::_ConvertStringToVector(msg,vCmd,";",TRUE,TRUE);

		std::list< MAP_POS_DEFINE >* pFlashLish = CWorldManager::GetMe()->GetObjectListByClass(7);
		if(!pFlashLish) return;

		if(nNumber != 6)
		{
			TDThrow("Talk HandleSpecialSelfString: Not Enough StringRule.");
			return;
		}

		float x,z;
		DWORD sceneId;
		sscanf(vCmd[SELF_SCENEID].c_str(), " %u", &sceneId);
		sscanf(vCmd[SELF_X].c_str(), " %f", &x);
		sscanf(vCmd[SELF_Z].c_str(), " %f", &z);

		MAP_POS_DEFINE data;
		data.dwSceneID = sceneId;
		data.pos.x = x;
		data.pos.z = z;
		_snprintf(data.name, sizeof(data.name),"%s", vCmd[SELF_TIP].c_str());

		pFlashLish->clear();
		pFlashLish->push_back(data);
	}

	VOID Talk::HandleFlashDel(LPCTSTR msg)
	{
		std::vector<std::string> vCmd;
		INT nNumber = DBC::DBCFile::_ConvertStringToVector(msg,vCmd,";",TRUE,TRUE);

		std::list< MAP_POS_DEFINE >* pFlashLish = CWorldManager::GetMe()->GetObjectListByClass(7);
		if(!pFlashLish) return;

		if(nNumber != 6)
		{
			TDThrow("Talk HandleSpecialSelfString: Not Enough StringRule.");
			return;
		}

		float x,z;
		DWORD sceneId;
		sscanf(vCmd[SELF_SCENEID].c_str(), " %u", &sceneId);
		sscanf(vCmd[SELF_X].c_str(), " %f", &x);
		sscanf(vCmd[SELF_Z].c_str(), " %f", &z);

		std::list< MAP_POS_DEFINE >::iterator it = pFlashLish->begin();
		while(it != pFlashLish->end())
		{
			if(x == (*it).pos.x && z == (*it).pos.z && sceneId == (*it).dwSceneID)
			{
				pFlashLish->erase(it);
				it = pFlashLish->begin();
				continue;
			}

			it++;
		}
	}

	VOID Talk::HandleFlagNPCAdd(LPCTSTR msg)
	{
		std::vector<std::string> vCmd;
		INT nNumber = DBC::DBCFile::_ConvertStringToVector(msg,vCmd,";",TRUE,TRUE);

		if(nNumber != 6)
		{
			TDThrow("Talk HandleSpecialSelfString: FlagNPCAdd error.");
			return;
		}

		float x,z;
		INT sceneId;
		sscanf(vCmd[SELF_SCENEID].c_str(), " %d", &sceneId);
		sscanf(vCmd[SELF_X].c_str(), " %f", &x);
		sscanf(vCmd[SELF_Z].c_str(), " %f", &z);

		if(CActivePosManager::GetMe())
		{
			CActivePosManager::GetMe()->AddActivePos(true, vCmd[SELF_NPC_NAME].c_str(), x,z, sceneId);
		}
	}

	VOID Talk::HandleFlagPOSAdd(LPCTSTR msg)
	{
		std::vector<std::string> vCmd;
		INT nNumber = DBC::DBCFile::_ConvertStringToVector(msg,vCmd,";",TRUE,TRUE);

		if(nNumber != 6)
		{
			TDThrow("Talk HandleSpecialSelfString: FlagPOSAdd error.");
			return;
		}

		float x,z;
		INT sceneId;
		sscanf(vCmd[SELF_SCENEID].c_str(), " %d", &sceneId);
		sscanf(vCmd[SELF_X].c_str(), " %f", &x);
		sscanf(vCmd[SELF_Z].c_str(), " %f", &z);

		if(CActivePosManager::GetMe())
		{
			CActivePosManager::GetMe()->AddActivePos(false, vCmd[SELF_TIP].c_str(), x,z, sceneId);
		}
	}

	VOID Talk::HandleFlashNPCAdd(LPCTSTR msg)
	{
		std::vector<std::string> vCmd;
		INT nNumber = DBC::DBCFile::_ConvertStringToVector(msg,vCmd,";",TRUE,TRUE);

		if(nNumber != 6)
		{
			TDThrow("Talk HandleSpecialSelfString: FlashNPCAdd error.[%s]", msg);
			return;
		}

		float x,z;
		INT sceneId;
		sscanf(vCmd[SELF_SCENEID].c_str(), " %d", &sceneId);
		sscanf(vCmd[SELF_X].c_str(), " %f", &x);
		sscanf(vCmd[SELF_Z].c_str(), " %f", &z);

		if(CActivePosManager::GetMe())
		{
			CActivePosManager::GetMe()->AddFlashPos(true, vCmd[SELF_NPC_NAME].c_str(), x,z, sceneId);
		}
	}

	VOID Talk::HandleFlashPOSAdd(LPCTSTR msg)
	{
		std::vector<std::string> vCmd;
		INT nNumber = DBC::DBCFile::_ConvertStringToVector(msg,vCmd,";",TRUE,TRUE);

		if(nNumber != 6)
		{
			TDThrow("Talk HandleSpecialSelfString: FlashPOSAdd error.[%s]", msg);
			return;
		}

		float x,z;
		INT sceneId;
		sscanf(vCmd[SELF_SCENEID].c_str(), " %d", &sceneId);
		sscanf(vCmd[SELF_X].c_str(), " %f", &x);
		sscanf(vCmd[SELF_Z].c_str(), " %f", &z);

		if(CActivePosManager::GetMe())
		{
			CActivePosManager::GetMe()->AddFlashPos(false, vCmd[SELF_TIP].c_str(), x,z, sceneId);
		}
	}

	VOID Talk::HandleMonsterPaoPao(LPCTSTR msg)
	{
		std::vector<std::string> vCmd;
		INT nNumber = DBC::DBCFile::_ConvertStringToVector(msg,vCmd,";",TRUE,TRUE);

		if(nNumber != 4)
		{
			TDThrow("Talk HandleSpecialSelfString: npcpaopao error.");
			return;
		}

		INT idServer, idMsg;
		sscanf(vCmd[SELF_MONSTER_SERVER_ID].c_str(), " %d", &idServer);
		sscanf(vCmd[SELF_MONSTER_PAOPAO_ID].c_str(), " %d", &idMsg);

		SetMonsterPaoPaoText(idServer, idMsg);
	}


	VOID Talk::HandleServerMsgTalk(LPCTSTR msg, INT channelType)
	{
		std::vector<std::string> vCmd;
		INT nNumber = DBC::DBCFile::_ConvertStringToVector(msg,vCmd,";",TRUE,TRUE);

		if(nNumber < 3)
		{
			TDThrow("Talk HandleSpecialSelfString: srvmsg error.");
			return;
		}

		enum {
			P1 = SELF_SERVERMSG_PARAM1,
			P2,
			P3,
			P4,
			P5,
			P6,
			P7,
			P8,
			P9,
			P10,
		};
		INT paramNum =(INT)(vCmd.size()-SELF_SERVERMSG_PARAM1);
		STRING strTmp= "";
		STRING strHeader = vCmd[SELF_SERVERMSG_TEMPLAT].substr(0,4);
		if(strHeader == "DBG:")
		{
			vCmd[SELF_SERVERMSG_TEMPLAT] = vCmd[SELF_SERVERMSG_TEMPLAT].substr(4);
		}

		switch(paramNum) {
		case 0:
			strTmp = COLORMSGFUNC(vCmd[SELF_SERVERMSG_TEMPLAT].c_str());
			break;
		case 1:
			strTmp = COLORMSGFUNC(vCmd[SELF_SERVERMSG_TEMPLAT].c_str(), vCmd[P1].c_str());
			break;
		case 2:
			strTmp = COLORMSGFUNC(vCmd[SELF_SERVERMSG_TEMPLAT].c_str(), vCmd[P1].c_str(), vCmd[P2].c_str());
			break;
		case 3:
			strTmp = COLORMSGFUNC(vCmd[SELF_SERVERMSG_TEMPLAT].c_str(), vCmd[P1].c_str(), vCmd[P2].c_str(),
				vCmd[P3].c_str());
			break;
		case 4:
			strTmp = COLORMSGFUNC(vCmd[SELF_SERVERMSG_TEMPLAT].c_str(), vCmd[P1].c_str(), vCmd[P2].c_str(),
				vCmd[P3].c_str(),vCmd[P4].c_str());
			break;
		case 5:
			strTmp = COLORMSGFUNC(vCmd[SELF_SERVERMSG_TEMPLAT].c_str(), vCmd[P1].c_str(), vCmd[P2].c_str(),
				vCmd[P3].c_str(),vCmd[P4].c_str(),vCmd[P5].c_str());
			break;
		case 6:
			strTmp = COLORMSGFUNC(vCmd[SELF_SERVERMSG_TEMPLAT].c_str(), vCmd[P1].c_str(), vCmd[P2].c_str(),
				vCmd[P3].c_str(),vCmd[P4].c_str(),vCmd[P5].c_str(),vCmd[P6].c_str());
			break;
		case 7:
			strTmp = COLORMSGFUNC(vCmd[SELF_SERVERMSG_TEMPLAT].c_str(), vCmd[P1].c_str(), vCmd[P2].c_str(),
				vCmd[P3].c_str(),vCmd[P4].c_str(),vCmd[P5].c_str(),vCmd[P6].c_str(),vCmd[P7].c_str());
			break;
		case 8:
			strTmp = COLORMSGFUNC(vCmd[SELF_SERVERMSG_TEMPLAT].c_str(), vCmd[P1].c_str(), vCmd[P2].c_str(),
				vCmd[P3].c_str(),vCmd[P4].c_str(),vCmd[P5].c_str(),vCmd[P6].c_str(),vCmd[P7].c_str(),vCmd[P8].c_str());
			break;
		case 9:
			strTmp = COLORMSGFUNC(vCmd[SELF_SERVERMSG_TEMPLAT].c_str(), vCmd[P1].c_str(), vCmd[P2].c_str(),
				vCmd[P3].c_str(),vCmd[P4].c_str(),vCmd[P5].c_str(),vCmd[P6].c_str(),vCmd[P7].c_str(),vCmd[P8].c_str(),
				vCmd[P9].c_str());
			break;
		case 10:
			strTmp = COLORMSGFUNC(vCmd[SELF_SERVERMSG_TEMPLAT].c_str(), vCmd[P1].c_str(), vCmd[P2].c_str(),
				vCmd[P3].c_str(),vCmd[P4].c_str(),vCmd[P5].c_str(),vCmd[P6].c_str(),vCmd[P7].c_str(),vCmd[P8].c_str(),
				vCmd[P9].c_str(),vCmd[P10].c_str());
			break;
		default:
			break;
		}

		if(strTmp.empty()) return;
		if(strHeader == "DBG:")
		{
			ADDNEWDEBUGMSG(strTmp);
			return;
		}

		switch(channelType) {
		case CHAT_TYPE_SYSTEM:
			{
				ADD_SYSTEM_MSG(strTmp);
			}
			break;
		case CHAT_TYPE_SELF:
			{
				ADDTALKMSG(strTmp);
			}
			break;
		default:
			break;
		}
	}

	INT Talk::HandleChannelResultPacket(GCChannelResult *pPacket)
	{
		if( NULL == pPacket)
		{
			TDAssert(FALSE);
			return -1;
		}

		switch(pPacket->GetReturn())
		{
		case CHANNEL_RESULT_CREATE:
			{
			}
			break;
		case CHANNEL_RESULT_ADDMEMBER:
		{
			}
			break;
		case CHANNEL_RESULT_DISMISS:
			{
			}
				break;
		case CHANNEL_RESULT_KICKMEMBER:
			{
			}
			break;
		default:
			break;
		}

		return 0;
	}
	
	INT Talk::HandleChannelErrorPacket(GCChannelError *pPacket)
	{
		if( NULL == pPacket)
	{
			TDAssert(FALSE);
			return -1;
		}

		switch(pPacket->GetErrorCode())
		{
		case CHANNEL_ERROR_HASCHANNEL:
			{
			}
				break;
		case CHANNEL_ERROR_CHANNELFULL:
			{
			}
			break;
		case CHANNEL_ERROR_NOCHANNEL:
			{
		}
			break;
		case CHANNEL_ERROR_CHANNELMEMBERFULL:
			{
			}
			break;
		case CHANNEL_ERROR_MEMBEREXIST:
			{
			}
			break;
		case CHANNEL_ERROR_NOTINCHANNEL:
			{
			}
			break;
		default:
			break;
	}

		return 0;
	}
//---------------------------------------------------------------------------------------------------------------------
//���ú����ӿ�
	STRING Talk::ChannelType2String(ENUM_CHAT_TYPE type)
	{
		STRING strType;
		switch(type)
		{
		case CHAT_TYPE_NORMAL:
			strType = NAMETYPE_TALK_NEAR;
			break;
		case CHAT_TYPE_SCENE:
			strType = NAMETYPE_TALK_SCENE;
			break;
		case CHAT_TYPE_CHANNEL:
			strType = NAMETYPE_TALK_PRIVATE;
			break;
		case CHAT_TYPE_SYSTEM:
			strType = NAMETYPE_TALK_SYSTEM;
			break;
		case CHAT_TYPE_TELL:
			strType = NAMETYPE_TALK_PRIVATE;
			break;
		case CHAT_TYPE_GUILD:
			strType = NAMETYPE_TALK_GUILD;
			break;
		case CHAT_TYPE_SELF:
			strType = NAMETYPE_TALK_SELF;
			break;
		case CHAT_TYPE_TEAM:
			strType = NAMETYPE_TALK_TEAM;
			break;
		case CHAT_TYPE_MENPAI:
			strType = NAMETYPE_TALK_MENPAI;
			break;
		default:
			strType = "";
			break;
		}
		
		//AxTrace(0,0,"strType = %s", strType.c_str());

		return strType;
	}

	ENUM_CHAT_TYPE Talk::String2ChannelType(STRING strType)
	{
		ENUM_CHAT_TYPE type = CHAT_TYPE_INVALID;
		
		if(strType == NAMETYPE_TALK_NEAR)
			type = CHAT_TYPE_NORMAL;
		else if(strType == NAMETYPE_TALK_SCENE)
			type = CHAT_TYPE_SCENE;
		else if(strType == NAMETYPE_TALK_TEAM)
			type = CHAT_TYPE_TEAM;					//??? TEAM, GUILD
		else if(strType == NAMETYPE_TALK_GUILD)
			type = CHAT_TYPE_GUILD;
		else if(strType == NAMETYPE_TALK_USER)
			type = CHAT_TYPE_TELL;
		else if(strType == NAMETYPE_TALK_PRIVATE)
			type = CHAT_TYPE_TELL;
		else if(strType == NAMETYPE_TALK_MENPAI)
			type = CHAT_TYPE_MENPAI;
		else if(strType == NAMETYPE_TALK_SELF)
			type = CHAT_TYPE_SELF;
		else if(strType == NAMETYPE_TALK_SYSTEM)
			type = CHAT_TYPE_SYSTEM;

		return type;
	}

	VOID Talk::UserCreate()
	{
		if(NULL == m_UsrCh)
		{
			m_UsrCh = new Talk::UserChannel();
			m_UsrCh->LoadChannelConfig();
			m_listChannel.push_back(m_UsrCh);
		}
	}

	VOID Talk::TeamCreate(TeamID_t id)
	{
		if(id < 0)
		{
			TDThrow("ERROR found in Talk::TeamCreate");
		}

		if(NULL != m_TeamCh)
			return;

		m_TeamCh = new TeamChannel;
		m_TeamCh->SetTeamID(id);
		m_TeamCh->LoadChannelConfig();

		m_listChannel.push_back(m_TeamCh);
	}

	VOID Talk::TeamDestory()
	{
		if(NULL == m_TeamCh)
			return;

		std::list<Channel*>::iterator it;
		for(it = m_listChannel.begin(); it != m_listChannel.end(); ++it)
		{
			if(*it == m_TeamCh)
			{
				m_listChannel.erase(it);
				break;
			}
		}

		SAFE_DELETE(m_TeamCh);

		if(CGameProcedure::s_pEventSystem)
		{
            CGameProcedure::s_pEventSystem->PushEvent(GE_CHAT_CHANNEL_CHANGED);
		}
	}

	VOID Talk::MenPaiJoin(MenPaiID_t id)
	{
		if(id < 0)
		{
			TDThrow("ERROR found in Talk::MenPaiJoin");
		}

		if(NULL != m_MenPaiCh)
			return;

		m_MenPaiCh = new MenPaiChannel;
		m_MenPaiCh->SetMenPaiID(id);
		m_MenPaiCh->LoadChannelConfig();

		m_listChannel.push_back(m_MenPaiCh);
	}

	VOID Talk::MenPaiLeave()
	{
		if(NULL == m_MenPaiCh)
			return;

		std::list<Channel*>::iterator it;
		for(it = m_listChannel.begin(); it != m_listChannel.end(); ++it)
		{
			if(*it == m_MenPaiCh)
			{
				m_listChannel.erase(it);
				break;
			}
		}

		SAFE_DELETE(m_MenPaiCh);

		if(CGameProcedure::s_pEventSystem)
		{
            CGameProcedure::s_pEventSystem->PushEvent(GE_CHAT_CHANNEL_CHANGED);
		}
	}

	VOID Talk::GuildJoin(GuildID_t id)
	{
		if(id < 0)
		{
			TDThrow("ERROR found in Talk::GuildJoin");
		}

		if(NULL != m_GuildCh)
			return;

		m_GuildCh = new GuildChannel;
		m_GuildCh->SetGuildID(id);
		m_GuildCh->LoadChannelConfig();

		m_listChannel.push_back(m_GuildCh);
	}

	VOID Talk::GuildLeave()
	{
		if(NULL == m_GuildCh)
			return;

		std::list<Channel*>::iterator it;
		for(it = m_listChannel.begin(); it != m_listChannel.end(); ++it)
		{
			if(*it == m_GuildCh)
			{
				m_listChannel.erase(it);
				break;
			}
		}

		SAFE_DELETE(m_GuildCh);

		if(CGameProcedure::s_pEventSystem)
		{
            CGameProcedure::s_pEventSystem->PushEvent(GE_CHAT_CHANNEL_CHANGED);
		}
	}

	ENUM_CHAT_TYPE Talk::GetChatTypeFromMessage(STRING &strMsg)
	{
		STRING strHead = strMsg.substr(0,MESSAGE_HEAD_LENGTH);
		ENUM_CHAT_TYPE retType = CHAT_TYPE_INVALID;

		if(strHead == MESSAGETYPE_TALK_NEAR)
			retType = CHAT_TYPE_NORMAL;
		else if(strHead == MESSAGETYPE_TALK_WORD)			//??? ���������磿
			retType = CHAT_TYPE_SCENE;
		else if(strHead == MESSAGETYPE_TALK_TEAM)
			retType = CHAT_TYPE_TEAM;
		else if(strHead == MESSAGETYPE_TALK_GUILD)
			retType = CHAT_TYPE_GUILD;
		else if(strHead == MESSAGETYPE_TALK_MENPAI)
			retType = CHAT_TYPE_MENPAI;						
		else if(strHead.size() < MESSAGE_HEAD_LENGTH)		//����MESSAGE_HEAD_LENGTH���ַ�����Ϣ��������ͨ��Ϣ
			retType = CHAT_TYPE_INVALID;
		else if(strHead.at(0) == '/' && strHead.at(1) != '/')
			retType = CHAT_TYPE_TELL;

		return retType;
	}

	STRING Talk::GetTalkTemplateString(STRING& strMsg, STRING& strTalker)
	{
		STRING strKey, strResult, strTarget, strFinal = "";
		UINT idx = TALK_TEMPLATE_MAX;

		const CHAR* pHyperB = "#aB{";
		const CHAR* pHyperM = "}";
		const CHAR* pHyperE = "#aE";

		//get talk template key & idx
		if(strMsg.find(" ") != STRING::npos)
		{
			strKey = strMsg.substr(1,strMsg.find(" ")-1);
			strTarget = strMsg.substr(strMsg.find(" ")+1);

			if(strTarget.size() > MAX_CHARACTER_NAME || strTarget.size() < 2) return strFinal; //��ɫ����̫�̻���̫��

			if(strTarget == strTalker) //˵������
				idx = TALK_TEMPLATE_MYSELF;
			else
				idx = TALK_TEMPLATE_OTHER;
		}
		else
		{
			strKey = strMsg.substr(1);
			idx = TALK_TEMPLATE_NOBODY;
		}

		if(strKey.empty()) return strFinal;

		//get talk tempalte string
		strResult = CGameProcedure::s_pUISystem->getTalkTemplate(strKey, idx);
		
		//generate real talker and target
		// $N --- TalkerName  $n --- TargetName
		STRING strRealTalker, strRealTarget;
		if(strResult.find("$N") != STRING::npos)
		{
			//˵���ߺͿͻ��˽�ɫ����ͬ
			if(strTalker == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name())
			{
				strRealTalker = "��";
			}
			else
			{
				//strRealTalker = pHyperB + strTalker + pHyperM + strTalker + pHyperE;
				strRealTalker = "#R" + strTalker + "#W";
			}
		}
		else
			return strFinal;

		if(idx == TALK_TEMPLATE_OTHER && strResult.find("$n") != STRING::npos)
		{
			//Ŀ���ߺͿͻ��˽�ɫ����ͬ
			if(strTarget == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name())
			{
				strRealTarget = "��";
			}
			else
			{
				//strRealTarget = pHyperB + strTarget + pHyperM + strTarget + pHyperE;
				strRealTarget = "#R" + strTarget + "#W";
			}
		}
		else if(idx == TALK_TEMPLATE_OTHER && strResult.find("$n") == STRING::npos)
		{
			return strFinal;
		}

		//generate final template string.
		strFinal = CGameProcedure::s_pUISystem->ModifyChatTemplate(strResult, strRealTalker, strRealTarget);

		return strFinal;
	}

	STRING Talk::GetTalkActString(STRING& strMsg, STRING& strTalker)
	{
		STRING strKey, strResult, strTarget, strFinal = "";
		UINT idx = TALK_ACT_NUMBER;

		const CHAR* pHyperB = "#aB{";
		const CHAR* pHyperM = "}";
		const CHAR* pHyperE = "#aE";

		//get talk act key & idx
		if(strMsg.find(" ") != STRING::npos)
		{
			strKey = strMsg.substr(1,strMsg.find(" ")-1);
			strTarget = strMsg.substr(strMsg.find(" ")+1);

			if(strTarget.size() > MAX_CHARACTER_NAME || strTarget.size() < 2) return strFinal; //��ɫ����̫�̻���̫��

			if(strTarget == strTalker) //˵������
				idx = TALK_ACT_MYSELF;
			else
				idx = TALK_ACT_OTHER;
		}
		else
		{
			strKey = strMsg.substr(1);
			idx = TALK_ACT_NOBODY;
		}

		if(strKey.empty()) return strFinal;

		//get talk tempalte string
		strResult = s_Talk.GetTalkActMsg(strKey.c_str(), idx);

		//generate real talker and target
		// $N --- TalkerName  $n --- TargetName
		STRING strRealTalker, strRealTarget;
		if(strResult.find("$N") != STRING::npos)
		{
			//˵���ߺͿͻ��˽�ɫ����ͬ
			if(strTalker == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name())
			{
				strRealTalker = "��";
			}
			else
			{
				//strRealTalker = pHyperB + strTalker + pHyperM + strTalker + pHyperE;
				strRealTalker = "#R" + strTalker + "#W";
			}
		}
		else
			return strFinal;

		if(idx == TALK_ACT_OTHER && strResult.find("$n") != STRING::npos)
		{
			//Ŀ���ߺͿͻ��˽�ɫ����ͬ
			if(strTarget == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name())
			{
				strRealTarget = "��";
			}
			else
			{
				//strRealTarget = pHyperB + strTarget + pHyperM + strTarget + pHyperE;
				strRealTarget = "#R" + strTarget + "#W";
			}
		}
		else if(idx == TALK_ACT_OTHER && strResult.find("$n") == STRING::npos)
		{
			strRealTarget = "#R" + strTarget + "#W";
		}

		//generate final template string.
		strFinal = CGameProcedure::s_pUISystem->ModifyChatTemplate(strResult, strRealTalker, strRealTarget);
		return strFinal;
	}

	VOID Talk::SetPaoPaoTxt(STRING& strUser, STRING& strTxt, INT strType, INT extLength)
	{
		CObject_Character* pObj = CObjectManager::GetMe()->FindCharacterByName(strUser);
		if(pObj && pObj->GetInfoBoard()->isShow())
		{
			pObj->Say(TalkFixTail(strTxt));
		}
	}

	VOID Talk::SetMonsterPaoPaoText(INT idServer, INT idMsg)
	{
		CObject_Character* pObj = (CObject_Character*)CObjectManager::GetMe()->FindServerObject(idServer);
		if(pObj && !g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
		{
			pObj = NULL;
		}

		if(pObj && pObj->GetInfoBoard()->isShow())
		{
			const tDataBase* pDB = CDataBaseSystem::GetMe()->GetDataBase(DBC_MONSTER_PAOPAO);
			if(pDB)
			{
				const _DBC_MONSTER_PAOPAO* pLine = (const _DBC_MONSTER_PAOPAO*)pDB->Search_Index_EQU(idMsg);
				if(pLine)
				{
					pObj->Say(STRING(pLine->szPaoPaoTxt));
				}
			}
		}
	}

	VOID Talk::AddPingBi(STRING& strUser)
	{
		if(!IsInPingBi(strUser))
		{
			m_PingBiQue.push_back(strUser);
			if((INT)m_PingBiQue.size() > CHAT_PINGBI_MAXUSER_NUMBERS) m_PingBiQue.pop_front();

			if(CEventSystem::GetMe())
			{
				CEventSystem::GetMe()->PushEvent(GE_CHAT_PINGBI_UPDATE);
			}
		}
	}

	VOID Talk::DelPingBi(STRING& strUser)
	{
		if(m_PingBiQue.empty()) return;

		std::deque<STRING>::iterator it;
		for(it = m_PingBiQue.begin(); it!= m_PingBiQue.end(); it++)
		{
			if(*it == strUser) break;
		}

		if(it != m_PingBiQue.end())
		{
			m_PingBiQue.erase(it);
		}
	}

	BOOL Talk::IsInPingBi(STRING& strUser)
	{
		if(m_PingBiQue.empty()) return FALSE;

		std::deque<STRING>::iterator it;
		for(it = m_PingBiQue.begin(); it!= m_PingBiQue.end(); it++)
		{
			if(*it == strUser) break;
		}

		return (it == m_PingBiQue.end())?FALSE:TRUE;
	}

	VOID Talk::ClearPingBi()
	{
		m_PingBiQue.erase(m_PingBiQue.begin(), m_PingBiQue.end());
	}

	VOID Talk::ShowTalkHelpMsg()
	{
		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level() < 10)
		{
			if(!(CGameProcedure::s_pUISystem->getTalkRandHelpMsg().empty()))
			{
				STRING strHelp = "[#G����#W]";
				strHelp += CGameProcedure::s_pUISystem->getTalkRandHelpMsg();

				ADDTALKMSG(strHelp);
			}
		}
	}

	VOID Talk::TalkDisplaySizeChanged(INT width, INT height)
	{
		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_ADJUST_MOVE_CTL, width, height);
		}
	}

	STRING Talk::TalkFixTail(STRING &strIn)
	{
		STRING strOut = "";
		const BYTE byANSIBegin		= 0X20;
		const BYTE byANSIEnd		= 0X80;

		const BYTE by1GBKBegin	= 0X81;
		const BYTE by1GBKEnd	= 0XFE;

		const BYTE by2GBKBegin1	= 0X40;
		const BYTE by2GBKEnd1	= 0X7E;
		const BYTE by2GBKBegin2	= 0X80;
		const BYTE by2GBKEnd2	= 0XFE;

		const CHAR * const szERROR = "<ERROR>";

		//Ԥ�ȷ�����ڴ�
		INT nSourSize = (INT)strIn.size();
		strOut.reserve(nSourSize+1);
		strOut = "";

		for(INT i=0; i<nSourSize; )
		{
			BYTE byChar = strIn[i];

			//��׼ANSIӢ���ַ�
			if(byChar >= byANSIBegin && byChar <= byANSIEnd || byChar == '\r' || byChar == '\n' || byChar == '\t')
			{
				strOut += byChar;
				i++; 
				continue;
			}
			//���ֵ�һ���ֽ�
			else if(byChar >= by1GBKBegin && byChar <= by1GBKEnd)
			{
				//�������
				if(i+1 == nSourSize) goto CHECKSTRING_ERROR;

				//ȡ���ڶ����ֽ�
				BYTE byNext = strIn[++i];
				if(	!( byNext >= by2GBKBegin1 && byNext <= by2GBKEnd1 || byNext >= by2GBKBegin2 && byNext <= by2GBKEnd2) )
				{
					//�Ƿ��ĺ���
					goto CHECKSTRING_ERROR;
				}

				//���������ĺ���
				CHAR szTemp[8];
				szTemp[0] = byChar;
				szTemp[1] = byNext;
				szTemp[2] = 0;
				strOut += szTemp;

				i++;
				continue;
			}
			else goto CHECKSTRING_ERROR;
		}

CHECKSTRING_ERROR:

		return strOut;
	}

	INT	Talk::AddToSendHistoryQue(HistoryMsg& msg)
	{
		m_SendHisQue.push_back(msg);

		if((INT)(m_SendHisQue.size()) > m_MaxSaveNum)
		{
			m_SendHisQue.pop_front();
		}
		
		m_PrevSendPos = -1;
		return 0;
	}

	INT	Talk::AddToRecvHistoryQue(HistoryMsg& msg)
	{
		m_RecvHisQue.push_back(msg);

		if((INT)(m_RecvHisQue.size()) > m_MaxSaveNum)
		{
			m_RecvHisQue.pop_front();
		}

		return 0;
	}

	INT	Talk::AddToGMCmdHistoryQue(HistoryMsg& msg)
	{
		m_GMCmdHisQue.push_back(msg);

		if((INT)(m_GMCmdHisQue.size()) > m_MaxSaveNum)
		{
			m_GMCmdHisQue.pop_front();
		}

		m_PrevGMCmdPos = -1;
		return 0;
	}

	VOID Talk::AddUserToUserChannel(STRING &strUser)
	{
		if(strUser.empty()) return;

		if(NULL == m_UsrCh)
		{
			m_UsrCh = new Talk::UserChannel();
			m_UsrCh->LoadChannelConfig();
			m_listChannel.push_back(m_UsrCh);
		}

		m_UsrCh->AddUser(strUser);
	}

	VOID Talk::SetTalkRule()
	{
		INT usrLv = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level();
		const tDataBase* pTalkConfig = CDataBaseSystem::GetMe()->GetDataBase(DBC_TALK_CONFIG);

		//��ǰƵ��������Ϣ���ͼ��
		std::list<Talk::Channel *>::iterator it = m_listChannel.begin();
		for(; it != m_listChannel.end(); it++)
		{
			//Ƶ���Ƿ����ĵȼ����
			if(pTalkConfig)
			{
				const _DBC_TALK_CONFIG* pLine = (const _DBC_TALK_CONFIG*)pTalkConfig->Search_LineNum_EQU((*it)->GetType());
				if(pLine)
				{
					if( usrLv >= pLine->nNeedLv)
					{
						(*it)->Open();
					}
					else
					{
						(*it)->Close();
					}
				}
			}//end if(pTalkConfig)

		}// end for

		//������������
		m_bPaoPaoRule = (usrLv < 10)?TRUE:FALSE;
		//TDU_Log("m_bPaoPaoRule : %d", (INT)m_bPaoPaoRule);
	}

	VOID Talk::LoadTabSetting(GCRetSetting* pk)
	{
		if(!pk) return;
		TDAssert(5 == sizeof(_OWN_SETTING));

		INT tabId = 1;
		for(INT i=SETTING_TYPE_CHAT_TAB1_PART1; i < SETTING_TYPE_CHAT_TAB4_PART2; i += 2, ++tabId)
		{
			_OWN_SETTING* pSetting1 = &(pk->GetSetting()[i]);
			_OWN_SETTING* pSetting2 = &(pk->GetSetting()[i+1]);
			if(!pSetting1 || !pSetting2) break;

			if(pSetting1->m_SettingType == 0 && pSetting1->m_SettingData == 0) break;

			CHAR szTabName[6+1] = {0};
			memcpy(szTabName, pSetting1, sizeof(_OWN_SETTING));
			memcpy(szTabName+sizeof(_OWN_SETTING), &(pSetting2->m_SettingType), sizeof(BYTE));

			STRING strConfig;
			strConfig.erase();

			for(UINT i=0; i< CHAT_TYPE_NUMBER; ++i)
			{
				(pSetting2->m_SettingData & 1<<i)?strConfig += "1":strConfig += "0";
			}

			//OK,֪ͨ���촰�ڴ�����Tabҳ��
			CEventSystem::GetMe()->PushEvent(GE_CHAT_LOAD_TAB_CONFIG, szTabName, strConfig.c_str(), tabId);
		}
	}

	VOID Talk::HandleKey_Shift_ArrowUp(INT type)
	{
		switch(type)
		{
		case 0:	//normal send his
			if(m_SendHisQue.empty()) return;

			//caculate send pos.
			if(m_PrevSendPos >= 0)
			{
				m_PrevSendPos = (INT)((m_PrevSendPos - 1 < 0)?(m_SendHisQue.size()-1):m_PrevSendPos-1);
			}
			else
			{
				m_PrevSendPos = (INT)(m_SendHisQue.size()-1);
			}

			//push accelerate key event.
			if(CEventSystem::GetMe())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "save_old");

				std::vector< STRING > strParam;
				strParam.push_back("shift_up");
				strParam.push_back(m_SendHisQue[m_PrevSendPos].GetMsgData());
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, strParam);
			}

			//AxTrace(0,0,"m_PrevSendPos:%d", m_PrevSendPos);
			break;
		case 1: //gm cmd send his
			if(m_GMCmdHisQue.empty()) return;

			//caculate send pos.
			if(m_PrevGMCmdPos >= 0)
			{
				m_PrevGMCmdPos = (INT)((m_PrevGMCmdPos - 1 < 0)?(m_GMCmdHisQue.size()-1):m_PrevGMCmdPos-1);
			}
			else
			{
				m_PrevGMCmdPos = (INT)(m_GMCmdHisQue.size()-1);
			}

			//push accelerate key event.
			if(CEventSystem::GetMe())
			{
				//CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "save_old");

				std::vector< STRING > strParam;
				strParam.push_back("shift_up");
				strParam.push_back(m_GMCmdHisQue[m_PrevGMCmdPos].GetMsgData());
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, strParam);
			}
			break;
		default:
			break;
		}
	}

	VOID Talk::HandleKey_Shift_ArrowDown(INT type)
	{
		switch(type)
		{
		case 0: //normal send his
			if(m_SendHisQue.empty()) return;

			//caculate send pos.
			if(m_PrevSendPos >=0)
			{
				m_PrevSendPos = (INT)((m_PrevSendPos+1 == m_SendHisQue.size())?0:m_PrevSendPos+1);
			}
			else
			{
				m_PrevSendPos = 0;
			}

			//push accelerate key event.
			if(CEventSystem::GetMe())
			{
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "save_old");

				std::vector< STRING > strParam;
				strParam.push_back("shift_down");
				strParam.push_back(m_SendHisQue[m_PrevSendPos].GetMsgData());
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, strParam);
			}

			//AxTrace(0,0,"m_PrevSendPos:%d", m_PrevSendPos);
			break;
		case 1: //gm cmd send his
			if(m_GMCmdHisQue.empty()) return;

			//caculate send pos.
			if(m_PrevGMCmdPos >=0)
			{
				m_PrevGMCmdPos = (INT)((m_PrevGMCmdPos+1 == m_GMCmdHisQue.size())?0:m_PrevGMCmdPos+1);
			}
			else
			{
				m_PrevGMCmdPos = 0;
			}

			//push accelerate key event.
			if(CEventSystem::GetMe())
			{
				//CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "save_old");

				std::vector< STRING > strParam;
				strParam.push_back("shift_down");
				strParam.push_back(m_GMCmdHisQue[m_PrevGMCmdPos].GetMsgData());
				CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, strParam);
			}
			break;
		default:
			break;
		}
	}

	VOID Talk::HandleKey_Alt_S()
	{
		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "send_msg");
		}
	}

	VOID Talk::HandleKey_Ctrl_Enter()
	{
		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "send_msg");
		}
	}

	VOID Talk::HandleKey_Alt_FaceMotion()
	{
		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_face");
		}
	}

	VOID Talk::HandleKey_PageUp()
	{
		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "page_up");
		}
	}

	VOID Talk::HandleKey_PageDown()
	{
		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "page_down");
		}
	}

	VOID Talk::SendGMCommand(LPCTSTR msg)
	{
		//����GMָ��
		CGameInterface::GetMe()->GM_SendCommand(msg+2);

		//����������ʷ
		HistoryMsg hmsg(CHAT_TYPE_SELF);
		hmsg.SetMsgName(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name());
		hmsg.SetMsgData(STRING(msg));
		hmsg.SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());

		AddToGMCmdHistoryQue(hmsg);
	}

	VOID Talk::DoAct(LPCTSTR act, LPCTSTR user)
	{
		if(act && user)
		{
			CObject_Character* pObj = CObjectManager::GetMe()->FindCharacterByName(user);
			if(pObj)
			{
				STRING strOrder = GetTalkActOrder(act);
				//�ֲ�������
				if(strOrder != "besit")
				{
					pObj->SetChatMoodAction(strOrder);
				}
				else
				{
					//������Ҫ��������
					pObj->SitDown();
				}
				//���Ŷ�������
				//pObj->PlayChatMoodAction();
			}
		}
	}

	VOID Talk::GenTalkActMap()
	{
		const tDataBase* pActDBC = CDataBaseSystem::GetMe()->GetDataBase(DBC_TALK_ACT);
		TALK_ACT_STRUCT	act;

		if(pActDBC)
		{
			INT num = pActDBC->GetRecordsNum();
			for(INT i=0; i < num; ++i)
			{
				act.m_actMsg.resize(0);
				act.m_actOrder = "";
				act.m_iconName.erase();
				act.m_tip.erase();

				const _DBC_TALK_ACT* pLine = (const _DBC_TALK_ACT*)pActDBC->Search_LineNum_EQU(i);

				TALK_ACTMAP::iterator it = m_TalkActMap.find(pLine->szCmd);
				//û�ҵ��Ĳ������ұ������
				if(it == m_TalkActMap.end())
				{
					act.m_actMsg.push_back(pLine->szNobody);
					act.m_actMsg.push_back(pLine->szMyself);
					act.m_actMsg.push_back(pLine->szOther);

					act.m_actOrder = STRING(pLine->szAct);
					//��һ�б��������£���Ϊ������Ҫ��������
					if(i == 0) act.m_actOrder = "besit";

					act.m_actIdx = (INT)m_TalkActMap.size();

					act.m_iconName = STRING(pLine->szIconName);
					act.m_tip = STRING(pLine->szToolTip);

					m_TalkActMap.insert(std::make_pair(pLine->szCmd, act));
				}
			}
		}// end of if(pActDBC)
	}

	STRING Talk::GetTalkActMsg(LPCTSTR act, INT idx)
	{
		STRING strRet = "";
		if(act && idx >= 0 && idx < TALK_ACT_NUMBER)
		{
			TALK_ACTMAP::iterator it = m_TalkActMap.find(act);
			if(it != m_TalkActMap.end())
			{
				TALK_ACT_STRUCT* pAct = &(it->second);
				if(pAct && idx < (INT)pAct->m_actMsg.size())
				{
					strRet = pAct->m_actMsg.at(idx);
				}
			}
		}
		return strRet;
	}

	STRING Talk::GetTalkActOrder(LPCTSTR act)
	{
		STRING strRet = "";
		if(act)
		{
			TALK_ACTMAP::iterator it = m_TalkActMap.find(act);
			if(it != m_TalkActMap.end())
			{
				TALK_ACT_STRUCT* pAct = &(it->second);
				if(pAct)
				{
					strRet = pAct->m_actOrder;
				}
			}
		}
		return strRet;
	}

	STRING Talk::FindTalkActKey(INT idx)
	{
		STRING strRet = "";
		
		if(idx >= 0  && idx < (INT)m_TalkActMap.size())
		{
			TALK_ACTMAP::iterator it = m_TalkActMap.begin();
			for(; it != m_TalkActMap.end(); it++)
			{
				TALK_ACT_STRUCT* pAct = &(it->second);

				if(idx == pAct->m_actIdx)
					strRet = it->first;
			}
		}

		return strRet;
	}

	TALK_ACT_STRUCT* Talk::GetTalkActStructByID(INT idx)
	{
		TALK_ACT_STRUCT* pAct = NULL;

		if(idx >= 0 && idx < (INT)m_TalkActMap.size())
		{
			TALK_ACTMAP::iterator it = m_TalkActMap.begin();
			for(; it != m_TalkActMap.end(); it++)
			{
				TALK_ACT_STRUCT* pActNow = &(it->second);
				if(idx == pActNow->m_actIdx)
					pAct = pActNow;
			}
		}

		return pAct;
	}
//------------------------------------------------------------------------------------------------------  [3/31/2006]
//�ַ�������
	TalkAnalyze::TalkAnalyze(INT ty, STRING str)
	{
		m_OrInputStr = str;
		m_OrChannelTye = ty;

		m_ChannelType = CHAT_TYPE_INVALID;
		m_Ok = FALSE;
		m_StrType = STRINGTYPE_INVALID;

		m_HeaderStr.erase();
	}

	STRING TalkAnalyze::getStr(INT type)
	{
		STRING strRet = "";
		if(!m_Ok) return strRet;

		switch(type) 
		{
		case ORGINAL_STR:
			strRet = m_OrInputStr;
			break;
		case SEND_STR:
			strRet = m_SendStr;
			break;
		case TALK_STR:
			strRet = m_TalkStr;
			break;
		case PAOPAO_STR:
			strRet = m_PaoPaoStr;
			break;
		case HISTORY_STR:
			strRet = m_HistoryStr;
			break;
		case PRIVATE_STR:
			strRet = m_TargetStr;
			break;
		default:
			break;
		}

		return strRet;
	}

	VOID TalkAnalyze::doAnalyze(STRING &strTalker)
	{
		m_PrevResult = m_OrInputStr;

		JudgeChannelType();
		MoveHeader();
		JudgeStringType();
		RulerCheck();
		GenAllStr();

		m_PrevResult.erase();
	}

	VOID TalkAnalyze::JudgeChannelType()
	{
		if(m_PrevResult.empty()) return;

		m_ChannelType = Talk::s_Talk.GetChatTypeFromMessage(m_PrevResult);
		if(CHAT_TYPE_INVALID == m_ChannelType)
		{
			m_ChannelType = m_OrChannelTye;
		}

		if(m_ChannelType == CHAT_TYPE_TELL)
		{
			Talk::s_Talk.UserCreate();
		}
	}

	VOID TalkAnalyze::MoveHeader()
	{
		if(m_PrevResult.empty()) return;

		INT sz = (INT)m_PrevResult.size();
		CHAR cH = '\0';

		if(sz>=1) cH = m_PrevResult.at(0);
		if(cH == '/')
		{
			STRING::size_type sF = m_PrevResult.find_first_of(" ");
			if(sF != STRING::npos && sz >= 2)
			{
				if(m_ChannelType == CHAT_TYPE_TELL)
				{
					m_TargetStr = m_PrevResult.substr(1, sF-1);
					if(m_TargetStr.size() > MAX_CHARACTER_NAME) m_TargetStr.erase();
				}

				m_HeaderStr = m_PrevResult.substr(0, sF);
				m_PrevResult = m_PrevResult.substr(sF);
				if(!(m_PrevResult.empty()) && m_PrevResult.size() >= 2)
				{
					m_HeaderStr += m_PrevResult.substr(0,1);
					m_PrevResult = m_PrevResult.substr(1);
				}
				else
				{
					m_PrevResult.erase();
					m_HeaderStr.erase();
				}
			}
			else
			{
				m_PrevResult.erase();
				m_HeaderStr.erase();
			}
		}
	}

	VOID TalkAnalyze::JudgeStringType()
	{
		if(m_PrevResult.empty()) return;
		
		INT sz = (INT)m_PrevResult.size();
		CHAR cH[2] = {0};

		if(sz>=1) cH[0] = m_PrevResult.at(0);
		if(sz>=2) cH[1] = m_PrevResult.at(1);

		struct st{
			TALKANALYZE_STRTYPE_ENUM	ty;
			const CHAR*					pCmd;
			INT							num;
		};

		//@*�������ͱ�
		static st typeTable[] = {
			{ STRINGTYPE_FLAGADD,			"flagadd",		6 },
			{ STRINGTYPE_FLASHADD,			"flashadd",		6 },
			{ STRINGTYPE_FLAGDEL,			"flagdel",		6 },
			{ STRINGTYPE_FLASHDEL,			"flashdel",		6 },

			{ STRINGTYPE_FLAG_NPC_ADD,		"flagNPC",		6 },
			{ STRINGTYPE_FLAG_POS_ADD,		"flagPOS",		6 },
			{ STRINGTYPE_FLASH_NPC_ADD,		"flashNPC",		6 },
			{ STRINGTYPE_FLASH_POS_ADD,		"flashPOS",		6 },
			{ STRINGTYPE_MONSTER_PAOPAO,	"npcpaopao",	4 },
			{ STRINGTYPE_SERVERMSG_TALK,	"SrvMsg",		3 },	//��������������3��
		};
		
		//@@	Ŀ�겻����
		//@*	������������
		//@		���ֱ���
		//*		���ﶯ��
		if(cH[0] == '@')
		{
			if(cH[1] == '@') 
			{
				m_StrType = STRINGTYPE_NOUSER;
			}
			else if(cH[1] == '*')
			{
				std::vector<std::string> vCmd;
				INT nNumber = DBC::DBCFile::_ConvertStringToVector(m_PrevResult.c_str(),vCmd,";",TRUE,TRUE);

				for(INT i = 0; i < sizeof(typeTable)/sizeof(st); ++i)
				{
					if(nNumber >= typeTable[i].num && 0 == strcmp(vCmd[1].c_str(), typeTable[i].pCmd))
						m_StrType = typeTable[i].ty;
				}
			}
			else
			{
				m_StrType = STRINGTYPE_TXTACT;
			}
		}
		else if(cH[0] == '*')
		{
			m_StrType = STRINGTYPE_ACT;
		}
		else if(cH[0] == '!' && cH[1] == '!')
		{
			m_StrType = STRINGTYPE_GM;
		}
		else
		{
			m_StrType = STRINGTYPE_NORMAL;
		}
	}

	VOID TalkAnalyze::GenAllStr()
	{
		if(m_PrevResult.empty()) return;

		switch(m_StrType)
		{
		case STRINGTYPE_NORMAL:			//��ͨ��Ϣ
		case STRINGTYPE_TXTACT:			//���ֱ���
		case STRINGTYPE_ACT:			//�������ж���
			GenSendStr();
			GenTalkStr();
			GenPaoPaoStr();
			GenHistoryStr();
			break;
		case STRINGTYPE_NOUSER:			//˽��Ŀ�겻���ߣ�����������������ʾ��Ϣ
			GenTalkStr();
			GenHistoryStr();
			break;
		case STRINGTYPE_MONSTER_PAOPAO:	//��������
		case STRINGTYPE_SERVERMSG_TALK:	//Server�˷��͹�������ʾ��Ϣ��Ϣ
		case STRINGTYPE_GM:				//GM����
		case STRINGTYPE_FLAGADD:		//��ͼָʾ�����
		case STRINGTYPE_FLASHADD:		//��ͼ��˸�����
		case STRINGTYPE_FLAGDEL:		//��ͼָʾ��ɾ��
		case STRINGTYPE_FLASHDEL:		//��ͼ��˸��ɾ��

		case STRINGTYPE_FLAG_NPC_ADD:	//��NPC���������ָʾ��
		case STRINGTYPE_FLAG_POS_ADD:	//��x,z�����ָʾ��
		case STRINGTYPE_FLASH_NPC_ADD:	//��NPC��������ӻ�ɫָʾ��
		case STRINGTYPE_FLASH_POS_ADD:	//��x,z����ӻ�ɫָʾ��
			break;
		case STRINGTYPE_INVALID:
		default:
			break;
		}
		
	}

	BOOL TalkAnalyze::IsOk()
	{
		m_Ok = TRUE;

		switch(m_StrType)
		{
		case STRINGTYPE_NORMAL:			//��ͨ��Ϣ
			{
				if(m_SendStr.empty() && m_TalkStr.empty()) m_Ok = FALSE;
				if(!(CGameProcedure::s_pUISystem->CheckStringFilter(m_SendStr)))
				{
					m_ErrStr = COLORMSGFUNC("InvalidSpecialString");
					m_Ok = FALSE;
				}
			}
			break;
		case STRINGTYPE_TXTACT:			//���ֱ���
		case STRINGTYPE_ACT:			//�������ж���
			{
				//if(m_ChannelType != CHAT_TYPE_NORMAL) m_Ok = FALSE; //������ǿ���л����Ƶ����Ҫ���ݲ��ж����Ҫ��
				if(m_SendStr.empty() && m_PaoPaoStr.empty()) m_Ok = FALSE;
			}
			break;
		case STRINGTYPE_NOUSER:			//˽��Ŀ�겻���ߣ�����������������ʾ��Ϣ
			{
				if(m_ChannelType != CHAT_TYPE_SELF) m_Ok = FALSE;
				if(m_TalkStr.empty() || m_HistoryStr.empty()) m_Ok = FALSE;
			}
			break;
		case STRINGTYPE_GM:				//GM����
			{
				if(m_OrInputStr.size() < 2) m_Ok = FALSE;
			}
			break;
		case STRINGTYPE_SERVERMSG_TALK:	//Server�˷��͹�������ʾ��Ϣ��Ϣ
			{
				if(m_ChannelType != CHAT_TYPE_SELF && m_ChannelType != CHAT_TYPE_SYSTEM) m_Ok = FALSE;
				if(m_OrInputStr.size() < 2) m_Ok = FALSE;
			}
			break;
		case STRINGTYPE_MONSTER_PAOPAO:	//��������
		case STRINGTYPE_FLAGADD:		//��ͼָʾ�����
		case STRINGTYPE_FLASHADD:		//��ͼ��˸�����
		case STRINGTYPE_FLAGDEL:		//��ͼָʾ��ɾ��
		case STRINGTYPE_FLASHDEL:		//��ͼ��˸��ɾ��

		case STRINGTYPE_FLAG_NPC_ADD:	//��NPC���������ָʾ��
		case STRINGTYPE_FLAG_POS_ADD:	//��x,z�����ָʾ��
		case STRINGTYPE_FLASH_NPC_ADD:	//��NPC��������ӻ�ɫָʾ��
		case STRINGTYPE_FLASH_POS_ADD:	//��x,z����ӻ�ɫָʾ��
			{
				if(m_ChannelType != CHAT_TYPE_SELF) m_Ok = FALSE;
				if(m_OrInputStr.size() < 2) m_Ok = FALSE;
			}
			break;
		case STRINGTYPE_INVALID:
		default:
			m_Ok = FALSE;
			break;
		}

		if(m_ChannelType == CHAT_TYPE_INVALID) m_Ok = FALSE;
		return m_Ok;
	}

	VOID TalkAnalyze::RulerCheck()
	{
		BOOL bCheck = Talk::s_Talk.m_bPaoPaoRule;
		if(!bCheck || m_PrevResult.empty()) return;

		//��Ҫ��������ܹ����͵�������Ŀ fujia 2007.12.06
		if(m_StrType == STRINGTYPE_NORMAL && m_ChannelType != CHAT_TYPE_TELL)
		{
			STRING str128 = m_PrevResult.substr(0, 128);
			m_PrevResult = Talk::s_Talk.TalkFixTail(str128);
		}
	}

	STRING TalkAnalyze::RemoveInvalidControlChar(const STRING& strSrc)
	{
		const CHAR KeyStart = '#';
		STRING strOut;
		STRING strErr;

		STRING::size_type sB = 0;
		STRING::size_type sE = strSrc.find_first_of(KeyStart);
		STRING::size_type sLen = strSrc.size();

		do
		{	
			if(sE == STRING::npos)
			{
				//copy last
				strOut += strSrc.substr(sB);
				break;
			}

			//copy normal
			strOut += strSrc.substr(sB, sE-sB);

			//get op
			STRING strOp = strSrc.substr(sE+1, 1);

			//erase invalid control char
			//#- is invalid control[4/22/2006]
			if(	strOp == "R" || strOp == "G" || strOp == "B" ||
				strOp == "K" || strOp == "Y" || strOp == "W" ||
				strOp == "b" || strOp == "u" || strOp == "r" ||
				strOp == "n" || strOp == "#" || strOp == "-")
			{
				//ok valid control char
				strOut += strSrc.substr(sE, 2);
				sE += 2;
			}
			else if(strOp == "c" || strOp == "e")
			{
				STRING strColor = strSrc.substr(sE+2, 6);
				STRING strValid;

				for(INT i = 0; i < (INT)strColor.size(); ++i)
				{
					if(	(strColor.at(i) >= 48 && strColor.at(i) <= 57)  ||		/*0-9*/
						(strColor.at(i) >= 65 && strColor.at(i) <= 70)  ||		/*A-F*/
						(strColor.at(i) >= 97 && strColor.at(i) <= 102))		/*a-f*/
					{
						strValid += strColor.substr(i,1);
					}
					else
					{
						break;
					}
				}

				if(strValid.size() != 6)
				{
					//invalid extend color input, skip
					sE += 2 + strValid.size();
					strErr += " Invalid #" + strOp + strValid + "!!!";
				}
				else
				{
					//��Ҫ����Ƿ񿿽���ɫ�����������ɫ����ɫ���޸ġ�
					INT color;
					sscanf(strSrc.substr(sE+2, 6).c_str(), " %6X", &color);
					INT blue = color&0xFF;
					INT green = (color>>8)&0xFF;
					INT red = (color>>16)&0xFF;

					if(red > 0xDC && green + blue < 0x64) //0xA0 -> 160, 0x64 -> 100
					{
						if((float)green/red < 0.3f)
						{
							green = (INT)(0.3f*red);
							blue = 0x64-green;
						}
						else if((float)blue/red < 0.3f)
						{
							blue = (INT)(0.3f*red);
							green = 0x64-blue;
						}
						red = 0xDC;

						CHAR tmp[16] = {0};
						_snprintf(tmp, 15,"%02X%02X%02X", red,green,blue);
						strOut += strSrc.substr(sE,2) + tmp;
					}
					else
					{
						//ok valid control char
						strOut += strSrc.substr(sE, 2 + 6);
					}
					sE += 2 + 6;
				}
			}
			else if(strOp.size() >= 1 && strOp.at(0) >= 48 && strOp.at(0) <= 57)
			{
				STRING strNum2 = strSrc.substr(sE+2,1);
				if(strNum2.size() >= 1 && strNum2.at(0) >= 48 && strOp.at(0) <= 57)
				{
					//ok valid Animate numbers( Two )
					strOut += strSrc.substr(sE, 2 + 1);
					sE += 2 + 1;
				}
				else
				{
					//ok valid Animate number( One )
					strOut += strSrc.substr(sE, 2);
					sE += 2;
				}
			}
			else if(strOp == "a") //user can't input hyper-link type string
			{
				//invalid control char in input, skip
				STRING strHyperlink = strSrc.substr(sE+2);
				STRING::size_type sTemp = strHyperlink.find("#aE");
				if( strHyperlink[0] == 'B' && sTemp != STRING::npos )
				{// find #aB....#aE
					strOut += strSrc.substr(sE, sTemp + 5);
					sE += sTemp + 5;
				}
				else
				{
					sE += 2;
					strErr += " Invalid #" + strOp + "!!!";
				}
			}
			else if(strOp == "{")	//ok, check magic #{} string.
			{
				//item element is valid. ex: #{_INFOID123}
				STRING strItemElement = strSrc.substr(sE+2, 7);
				if( strncmp( strItemElement.c_str(), "_INFOID", 7 ) == 0 )
				{
					//ok, valid item element found.
					strOut += strSrc.substr(sE, 2 + 7);
					sE += 2 + 7;
				}
				else if( strncmp( strItemElement.c_str(), "_ITEM", 5 ) == 0 )
				{
					strOut += strSrc.substr(sE, 2 + 5);
					sE += 2 + 5;
				}
				else if( strncmp( strItemElement.c_str(), "_TIME", 5 ) == 0 )
				{
					strOut += strSrc.substr(sE, 2 + 5);
					sE += 2 + 5;
				}
				else if( strncmp( strItemElement.c_str(), "_MONEY", 6 ) == 0 )
				{
					strOut += strSrc.substr(sE, 2 + 6);
					sE += 2 + 6;
				}
				else
				{
					//all other things is not allowed.
					//invalid control char in input, skip
					sE += 2;
					strErr += " Invalid #" + strOp + "!!!";
				}
			}
			else
			{
				//single # , skip
				sE += 1;
				strErr += " Invalid Single \'#\'!!!";
			}

			if(sE >= sLen) break;

			//save new begin point
			sB = sE;

			//find next KeyStart
			sE = strSrc.find(KeyStart, sB);

		}while(TRUE);

		return strOut;
	}

	STRING TalkAnalyze::RemoveSpaceChar(const STRING& strIn)
	{
		const CHAR ansiSpace = ' ';			//Ӣ�Ŀո�
		const CHAR doubleSpace[3] = "��";	//���Ŀո�
		STRING strMid;
		STRING strOut;

		STRING::size_type sB = 0;
		STRING::size_type sE = strIn.find_first_of(ansiSpace);
		STRING::size_type sLen = strIn.size();

		//ɾ��Ӣ�Ŀո�
		do
		{	
			if(sE == STRING::npos)
			{
				//copy last
				strMid += strIn.substr(sB);
				break;
			}

			//copy normal
			strMid += strIn.substr(sB, sE-sB);

			//erase ansi space
			sE += 1;

			if(sE >= sLen) break;

			//save new begin point
			sB = sE;

			//find next KeyStart
			sE = strIn.find(ansiSpace, sB);

		}while(TRUE);

		//ɾ�����Ŀո�
		sE = 0;
		sLen = strMid.size();

		do
		{	
			if(sE+2 > sLen)
			{
				//copy last
				strOut += strMid.substr(sE);
				break;
			}

			//get 2 char
			STRING strOp = strMid.substr(sE, 2);

			if(strOp.at(0) != doubleSpace[0] || strOp.at(1) != doubleSpace[1])
			{
				//copy normal
				strOut += strMid.substr(sE, 2);
			}

			sE += 2;

		}while(TRUE);

		return strOut;
	}
// ------------------------------------------------------------------------------------------------------------------------ [3/31/2006]
	//������Ϣʱ����
	VOID SendTalkAnalyze::GenSendStr()
	{
		//��ͨ�����ִ�
		if(m_StrType == STRINGTYPE_NORMAL)
		{
			m_SendStr = m_PrevResult;
		}

		//���ֱ���
		if(m_StrType == STRINGTYPE_TXTACT)
		{
			//check
			STRING strKey = m_PrevResult.substr(0,m_PrevResult.find(" "));
			STRING strTalker = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name();
			m_SendStr = Talk::s_Talk.GetTalkTemplateString( strKey, strTalker );
			if(m_SendStr.empty()) 
			{
				m_ErrStr = "#R�����ģ��ʹ�ã����飡#Bлл��";
				return;
			}

			m_SendStr = strKey;
		}

		//���ж���
		if(m_StrType == STRINGTYPE_ACT)
		{
			//check
			STRING strAct = m_PrevResult.substr(strlen(CHAT_ACT_HEADER), m_PrevResult.find(" ")-strlen(CHAT_ACT_HEADER));
			m_SendStr = Talk::s_Talk.GetTalkActMsg(strAct.c_str(), TALK_ACT_NOBODY);
			if(m_SendStr.empty())
			{
				m_ErrStr = "#R����ʹ�ô���";//COLORMSGFUNC("GMGameInterface_Script_Talk_Info_Act_Error");
				return;
			}

			m_SendStr = CHAT_ACT_HEADER + strAct;

		}
        
		//��Ӳ������󵽷����ִ���
		if(m_StrType == STRINGTYPE_ACT || m_StrType == STRINGTYPE_TXTACT)
		{
			STRING::size_type sF = m_PrevResult.find_first_of(" ");
			if(sF != STRING::npos)
			{
				STRING strOp = m_PrevResult.substr(sF);
				if(strOp == " u")
				{
					CObject* pObj = (CObject*)CObjectManager::GetMe()->GetMainTarget();
					if(pObj && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
					{
						m_SendStr += " " + STRING(((CObject_Character*)pObj)->GetCharacterData()->Get_Name());
					}
				}
				else if(strOp == " i")
				{
					CObject* pObj = (CObject*)CObjectManager::GetMe()->GetMySelf();
					m_SendStr += " " + STRING(((CObject_Character*)pObj)->GetCharacterData()->Get_Name());
				}
			}
		}

		m_SendStr = RemoveInvalidControlChar(m_SendStr);
	}

	VOID SendTalkAnalyze::GenHistoryStr()
	{
		if(m_StrType == STRINGTYPE_NORMAL || m_StrType == STRINGTYPE_ACT || m_StrType == STRINGTYPE_TXTACT )
		{
			m_HistoryStr = m_HeaderStr + m_PrevResult;
		}
	}

	BOOL SendTalkAnalyze::IsOk()
	{
		TalkAnalyze::IsOk();
		if(m_Ok)
		{
			//���ֱ��顢���ﶯ�������ڡ���ǰ��Ƶ����ʹ��
			if(getStrType() == STRINGTYPE_ACT || getStrType() == STRINGTYPE_TXTACT)
			{
				if(getChannelType() != CHAT_TYPE_NORMAL)
				{
					if(getChannelType() == getOrChannelType())
					{
						if(CGameProcedure::s_pEventSystem)
						{
							CGameProcedure::s_pEventSystem->PushEvent(GE_CHAT_CHANNEL_CHANGED, "force_near");
						}
						m_ChannelType = CHAT_TYPE_NORMAL;
					}
					else
					{
						m_ErrStr = "#R�����ڴ�Ƶ����ʹ��";//COLORMSGFUNC("GMGameInterface_Script_Talk_Info_CantUseInThisChannel");
						m_Ok = FALSE;
					}
				}
			}

			//����ȴû���������
			if(getStrType() == STRINGTYPE_NORMAL && getStr(PRIVATE_STR).empty())
			{
				if(getChannelType() == CHAT_TYPE_TELL)
				{
					if(CGameProcedure::s_pEventSystem)
					{
						CGameProcedure::s_pEventSystem->PushEvent(GE_CHAT_CHANNEL_CHANGED);
					}
					m_ChannelType = CHAT_TYPE_NORMAL;
				}
			}

			//������ҵ������ﺬ�зǷ��ַ�
			if(getStrType() == STRINGTYPE_NORMAL && !(getStr(PRIVATE_STR).empty()))
			{
				if(getChannelType() == CHAT_TYPE_TELL)
				{
					if(!TDU_CheckStringValid(getStr(PRIVATE_STR).c_str()))
					{
						m_ErrStr = "#R���Ķ������Ƿ�";//COLORMSGFUNC("GMGameInterface_Script_Talk_Info_InvalidTargetUser");
						m_Ok = FALSE;
					}

					if(0 == strcmp(getStr(PRIVATE_STR).c_str(),CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name()))
					{
						m_ErrStr = "���ܸ��Լ�˽��";//COLORMSGFUNC("GMGameInterface_Script_Talk_Info_InvalidCantMySelf");
						m_Ok = FALSE;
					}

					if(!(CGameProcedure::s_pUISystem->CheckStringFilter(getStr(PRIVATE_STR))))
					{
						m_ErrStr = COLORMSGFUNC("InvalidSpecialString");
						m_Ok = FALSE;
					}
				}
			}


			//ֻ����ɫ���Ʒ����ִ�������
			if(m_StrType == STRINGTYPE_NORMAL || m_StrType == STRINGTYPE_ACT || m_StrType == STRINGTYPE_TXTACT )
			{
				STRING strNoColor;
				if(CGameProcedure::s_pUISystem)		CGameProcedure::s_pUISystem->ParserString_NoColor(m_SendStr, strNoColor, TRUE);
				if(strNoColor.empty())
				{
					m_ErrStr = "#Rû�п��Է��͵�����";//COLORMSGFUNC("GMGameInterface_Script_Talk_Info_InvalidTalkContent");
					m_Ok = FALSE;
				}
			}

			//ֻ�пո���ִ�������
			if(m_StrType == STRINGTYPE_NORMAL || m_StrType == STRINGTYPE_ACT || m_StrType == STRINGTYPE_TXTACT )
			{
				STRING strNoColor = RemoveSpaceChar(m_SendStr);
				
				if(strNoColor.empty())
				{
					m_ErrStr = COLORMSGFUNC("ERRORSpecialString");
					m_Ok = FALSE;
				}
			}
		}

		return m_Ok;
	}
// --------------------------------------------------------------------------------------------------------------- [3/31/2006]
	//������Ϣʱ����
	VOID RecvTalkAnalyze::doAnalyze(STRING& strTalker)
	{
		m_PrevResult = m_OrInputStr;
		m_ChannelType = m_OrChannelTye;

		m_Talker = strTalker;
		JudgeStringType();
		GenAllStr();

		m_PrevResult.erase();
	}

	VOID RecvTalkAnalyze::GenTalkStr()
	{
		//��ͨ�����ִ�
		if(m_StrType == STRINGTYPE_NORMAL)
		{
			m_TalkStr = m_PrevResult;
		}

		//���ֱ���&���ж���
		if(m_StrType == STRINGTYPE_TXTACT || m_StrType == STRINGTYPE_ACT)
		{
			m_TalkStr = m_PrevResult;
		}

		//Ŀ�겻����
		if(m_StrType == STRINGTYPE_NOUSER)
		{
			m_TalkStr = COLORMSGFUNC("Talk_Obj_Offline", m_PrevResult.c_str()+2);
		}
	}

	VOID RecvTalkAnalyze::GenPaoPaoStr()
	{
		if(CHAT_TYPE_NORMAL == m_ChannelType || CHAT_TYPE_TEAM == m_ChannelType)
		{
			m_PaoPaoStr = (CHAT_TYPE_TEAM == m_ChannelType) ? "#cCC99FF[����]#W": "";
			//��ͨ�����ִ�
			if(m_StrType == STRINGTYPE_NORMAL)
			{
				m_PaoPaoStr += m_PrevResult;
			}

			//���ֱ���
			if(m_StrType == STRINGTYPE_TXTACT)
			{
				m_PaoPaoStr = Talk::s_Talk.GetTalkTemplateString(m_PrevResult, m_Talker);
			}

			//���ж���
			if(m_StrType == STRINGTYPE_ACT)
			{
				m_PaoPaoStr = Talk::s_Talk.GetTalkActString(m_PrevResult, m_Talker);
			}
		}
	}

	VOID RecvTalkAnalyze::GenHistoryStr()
	{
		//��ͨ�����ִ�&���ֱ���&���ж���
		if(m_StrType == STRINGTYPE_NORMAL || m_StrType == STRINGTYPE_TXTACT || m_StrType == STRINGTYPE_ACT)
		{
			m_HistoryStr = m_PrevResult;
		}

		//Ŀ�겻����
		if(m_StrType == STRINGTYPE_NOUSER)
		{
			m_HistoryStr = COLORMSGFUNC("Talk_Obj_Offline", m_PrevResult.c_str()+2);
		}
	}
// --------------------------------------------------------------------------------------------------------------- [4/3/2006]
	//Lua�ű�Ҫ��ʱ����
	VOID LuaTalkAnalyze::doAnalyze(STRING& strTalker)
	{
		m_PrevResult = m_OrInputStr;
		m_ChannelType = m_OrChannelTye;

		m_Talker = strTalker;
		JudgeStringType();
		GenAllStr();

		m_PrevResult.erase();
	}

	VOID LuaTalkAnalyze::GenTalkStr()
	{
		//��ͨ����
		if(m_StrType == STRINGTYPE_NORMAL)
		{
			m_TalkStr = m_PrevResult;
		}

		//���ֱ���
		if(m_StrType == STRINGTYPE_TXTACT)
		{
			STRING myName = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name();

			m_TalkStr = Talk::s_Talk.GetTalkTemplateString(m_PrevResult, m_Talker);
			if(m_TalkStr.empty() && m_Talker != myName)
			{
				m_TalkStr = "#c00CCFF[#aB{" + m_Talker + "}" + m_Talker + "#aE]��" + m_PrevResult;
			}
			else if(m_TalkStr.empty() && m_Talker == myName)
			{
				m_TalkStr = "#W[" + m_Talker + "]��" + m_PrevResult;
			}

		}

		//���ж���
		if(m_StrType == STRINGTYPE_ACT)
		{
			STRING myName = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name();

			m_TalkStr = Talk::s_Talk.GetTalkActString(m_PrevResult, m_Talker);
			if(m_TalkStr.empty() && m_Talker != myName)
			{
				m_TalkStr = "#c00CCFF[#aB{" + m_Talker + "}" + m_Talker + "#aE]��" + m_PrevResult;
			}
			else if(m_TalkStr.empty() && m_Talker == myName)
			{
				m_TalkStr = "#W[" + m_Talker + "]��" + m_PrevResult;
			}
		}
	}

	BOOL LuaTalkAnalyze::IsOk()
	{
		if(!m_Ok && CHAT_TYPE_INVALID == m_ChannelType)
		{
			m_Ok = TRUE;
		}

		return m_Ok;
	}
// --------------------------------------------------------------------------------------------------------------- [4/29/2006]
	//�ͻ������������Ϣ�ķ�װ
	VOID Global_Msg_Func::addTalkMsg(LPCTSTR szMsg, ENUM_CHAT_TYPE chatType, CHAR* file, INT line)
	{
		if(szMsg)
		{
			return addTalkMsg(STRING(szMsg), chatType, file, line);
		}
	}

	VOID Global_Msg_Func::addTalkMsg(STRING szMsg, ENUM_CHAT_TYPE chatType, CHAR* file, INT line)
	{
		if(szMsg.empty())	return;

		STRING strType = Talk::s_Talk.ChannelType2String(chatType);
		STRING strName = "";

		Talk::HistoryMsg tlkmsg(chatType); \
		tlkmsg.SetMsgName(strName); \
		tlkmsg.SetMsgData(szMsg, file, line); \
		tlkmsg.SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());

		Talk::s_Talk.AddToRecvHistoryQue(tlkmsg);

		std::vector< STRING > strParam;
		strParam.push_back(strType);
		strParam.push_back(tlkmsg.GetMsgName());
		strParam.push_back(tlkmsg.GetMsgData());

		if(CEventSystem::GetMe())
		{
			CEventSystem::GetMe()->PushEvent(GE_CHAT_MESSAGE, strParam);
		}
	}
}
