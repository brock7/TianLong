#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "LuaPlus.h"
#include "../Global.h"
#include "../Procedure/GameProcedure.h"
#include "TDUISystem.h"
#include "../Object/ObjectManager.h"
#include "../Object/Object.h"
#include "../Object/Character/Obj_Character.h"
#include "../Object/ObjectManager.h"
#include "../Event/GMEventSystem.h"
#include "../Network/NetManager.h"
#include "../DataPool/GMDataPool.h"
#include "../object/character/obj_playermyself.h"
#include "../DataPool/GMDP_CharacterData.h"
#include "../Action/GMActionSystem.h"
#include "TDUtil.h"

#include "CGStallEstablish.h"
#include "CGStallAddItem.h"
#include "CGStallRemoveItem.h"
#include "CGStallClose.h"
#include "CGStallItemPrice.h"
#include "CGStallShopName.h"
#include "CGBBSApply.h"
#include "CGBBSSychMessages.h"
#include "CGStallDefaultPage.h"

namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	StallSale StallSale::s_StallSale;
	LuaPlus::LuaObject* StallSale::s_pMetaTable = NULL;

	//�ĵ�����Ƿ����
	INT StallSale::IsPresent(LuaPlus::LuaState* state)
	{
		if(this == &s_StallSale)
		{
			if(CObjectManager::GetMe()->GetMainTarget())
			{
				state->PushBoolean(true);
			}
			else
			{
				state->PushBoolean(FALSE);
			}
		}
		else
			TDThrow("Not a valid obj");

		return 1;
	}


	//��������̯λ��
	INT StallSale::GetPosTax(LuaPlus::LuaState* state)
	{
		state->PushInteger( CDataPool::GetMe()->MyStallBox_GetPosTax() );
		return 1;
	}
	//��������˰��
	INT StallSale::GetTradeTax(LuaPlus::LuaState* state)
	{
		state->PushInteger( CDataPool::GetMe()->MyStallBox_GetTradeTax() );
		return 1;
	}
	//�ύ��Ʒ�۸�ͬʱҪ�ύ��Ʒ�����޸ļ۸�ʹ�ò�ͬ�ģ�
	INT StallSale::ReferItemPrice(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:StallSale::ReferItemPrice param parameter error");
		}

        UINT nMoney = static_cast<UINT>(args[2].GetInteger());

		//��Ʒ���۲���Ϊ0
		if(nMoney==0)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "�ϼ�ʧ�ܣ��ϼ���Ʒ�۸���Ϊ��");
			return 0;
		}
		
		if(nMoney<=1000000000)
		{//С��10w������ϼ�
			// ������Ϣ���߷�����
			_ITEM_GUID Guid;
			tObject_Item::ITEM_GUID temp;
			temp.m_idUnion = 0;

			CGStallAddItem msg;
			msg.SetFromType(STALL_MSG::POS_BAG);

			tObject_Item* pItem = CDataPool::GetMe()->MyStallBox_GetCurSelectItem();
			if(pItem == NULL)
				return 0;

			//��֤�Ƿ���Խ���
			if(!pItem->Rule(tObject_Item::RULE_EXCHANGE))
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_EXCHANGE).c_str());
				return 0;
			}

			pItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
			AxTrace(0, 0, "ReferItemPrice:: pItem %s, GUID=%u", pItem->GetName(), pItem->GetGUID());
			Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
			Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
			Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

			msg.SetObjGUID(Guid);
			msg.SetPrice(nMoney);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "��̯ʱ���ý��г���10������ϵĽ���");
		}
		return 0;
	}
	// ����̯λ����
	INT StallSale::ModifStallName(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:StallSale::ModifStallName[2] param parameter error");
			return 0;
		}
		std::string str	= args[2].GetString();

		//���ǲ���ʹ�õ���
		//��֤�̵����ƵĺϷ���
		if(!CGameProcedure::s_pUISystem->CheckStringFilter(str))
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("ERRORSpecialString");
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
			return 0;
		}
		//��֤�̵����ƵĺϷ���
		if(!TDU_CheckStringValid(str.c_str()))
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("ERRORSpecialString");
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
			return 0;
		}


		if(str.size() > MAX_STALL_NAME)
		{
			str.resize(MAX_STALL_NAME);
		}
		CGStallShopName msg;
		msg.SetStallName((CHAR*)str.c_str());
		msg.SetStallNameSize((BYTE)str.size());
		CNetManager::GetMe()->SendPacket(&msg);
		
		return 0;
	}
	// ����Item�۸�(������۸�ĶԻ�Frame)
	INT StallSale::ModifItemPrice(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:StallSale::ModifItemPrice[2] param parameter error");
		}
		std::string str	= args[2].GetString();
		if(str == "item")
		{
			// ����������ۼ۸�ĶԻ���
			CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY, "reprice");
		}
		else
		{
			// ����������ۼ۸�ĶԻ���
			CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY, "petrepice");
		}
		return 0;
	}
	// ��̯���ˣ��ȴ�������ȷ��֮���ٹرնԻ���
	INT StallSale::CloseStall(LuaPlus::LuaState* state)
	{
		CGStallClose msg;
		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	// ��Ʒ�����¶���
	INT StallSale::ItemReprice(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:StallSale::ItemReprice[2] param parameter error");
		}
		if (!(args[3].IsInteger()))  //ActionID
		{
			TDThrow("LUA:StallSale::ItemReprice[3] param parameter error");
		}
		if (!(args[4].IsInteger()))  // Index
		{
			TDThrow("LUA:StallSale::ItemReprice[4] param parameter error");
		}

        UINT nMoney		= static_cast<UINT>(args[2].GetInteger());
		INT nActionID	= args[3].GetInteger();
		INT nIndex		= args[4].GetInteger();

		// ʹ������ȥ�����Ʒָ��
		CActionItem * pActionItem = (CActionItem *)CActionSystem::GetMe()->GetAction(nActionID);

		if(!pActionItem)
			return 0;
		tObject_Item* pItem = (tObject_Item*)pActionItem->GetImpl();

		//�Ƚϼ۸��Ƿ��иĶ�
		if(CDataPool::GetMe()->MyStallBox_GetItemPrice(nIndex) == nMoney)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���ļ۸�͵�ǰ�ļ۸���ͬ��");
			return 0;
		}
		if(nMoney > 1000000000)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��̯ʱ���ý��г���ʮ������ϵĽ���");
			return 0;
		}

		_ITEM_GUID Guid;
		tObject_Item::ITEM_GUID temp;
		temp.m_idUnion = 0;
		pItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
		Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
		Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
		Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

		// ������Ϣ���߷�����
		CGStallItemPrice msg;
		msg.SetObjGUID(Guid);
		msg.SetPrice(nMoney);
		msg.SetSerial(CDataPool::GetMe()->MyStallBox_GetItemSerial(nIndex));

		CNetManager::GetMe()->SendPacket(&msg);

		return 0;
	}
	// ���Ϳ�ʼ��̯����Ϣ��ʼ��̯
	INT StallSale::AgreeBeginStall(LuaPlus::LuaState* state)
	{
		//�ж��Ƿ������״̬���߱���״̬��������״̬���ܰ�̯
		if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_ModelID() != -1 )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����״̬�����ܰ�̯");
			return 0;
		}
		if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID() != -1 )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���״̬�����ܰ�̯");
			return 0;
		}

		if( !CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID().IsNull() )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "�����ս״̬�����ܰ�̯");
			return 0;
		}

		CGStallEstablish msg;
		CNetManager::GetMe()->SendPacket(&msg);

		return 0;
	}
	//ɾ�������ϵ�һ����Ʒ���߳���¼ܣ�
	INT StallSale::DeleteItem(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:StallSale::DeleteItem[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:StallSale::DeleteItem[3] param parameter error");
		}
		std::string str = args[2].GetString();
        INT nActionID = args[3].GetInteger();

		if(str == "item")
		{
			// ʹ������ȥ�����Ʒָ��
			CActionItem * pActionItem = (CActionItem *)CActionSystem::GetMe()->GetAction(nActionID);

			if(!pActionItem)
				return 0;
			tObject_Item* pItem = (tObject_Item*)pActionItem->GetImpl();

			// ������Ϣ���߷�����
			_ITEM_GUID Guid;
			tObject_Item::ITEM_GUID temp;
			temp.m_idUnion = 0;

			CGStallRemoveItem msg;
			//tObject_Item* p= CDataPool::GetMe()->MyStallBox_GetCurSelectItem();
			pItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);

			Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
			Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;
			Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

			msg.SetObjGUID(Guid);
			msg.SetToType(STALL_MSG::POS_BAG);
			msg.SetSerial(CDataPool::GetMe()->MyStallBox_GetItemSerial(pItem->GetPosIndex()));

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else
		{
			if(CDataPool::GetMe()->MyStallBox_GetPet(nActionID) == NULL)
			{
				return 0;
			}
			//�¼�һ������
			CGStallRemoveItem msg;
			msg.SetToType(STALL_MSG::POS_PET);
			msg.SetPetGUID(CDataPool::GetMe()->MyStallBox_GetPet(nActionID)->m_GUID);
			msg.SetSerial(CDataPool::GetMe()->MyStallBox_GetPetSerial(nActionID));

			CNetManager::GetMe()->SendPacket(&msg);
		}
		return 0;
	}

	//���ָ��λ�õĶ���
	INT StallSale::GetPrice(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:StallSale::GetPrice[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:StallSale::GetPrice[3] param parameter error");
		}
		std::string str = args[2].GetString();
        INT nIndex = args[3].GetInteger();
		INT nMoney;
		if(str == "item")
		{
			nMoney = CDataPool::GetMe()->MyStallBox_GetItemPrice(nIndex);
		}
		else
		{
			nMoney = CDataPool::GetMe()->MyStallBox_GetPetPrice(nIndex);
		}
		INT nGoldCoin;	
		INT nSilverCoin;
		INT nCopperCoin;

		nCopperCoin = nMoney % 100;

		if( nMoney >= 100 )
		{
			nSilverCoin = ( (nMoney-nCopperCoin)/100 ) % 100;
		}
		else
		{
			nSilverCoin = 0;
		}

		if ( nMoney >= 10000 )
		{
			nGoldCoin = (  ( (nMoney-nCopperCoin)/100 )-nSilverCoin  )/100;
		}
		else
			nGoldCoin = 0;

		state->PushInteger( nMoney );
		state->PushInteger( nGoldCoin );
		state->PushInteger( nSilverCoin );
		state->PushInteger( nCopperCoin );

		return 4;
	}
	//��̯λ��Ϣ
	INT StallSale::OpenMessageSale(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("StallMessage"))
		{
			CEventSystem::GetMe()->PushEvent(GE_CLOSE_STALL_MESSAGE);
			return 0;
		}
		//����������Ϣ����Ϣ(�����Լ���ID)
		CGBBSApply msg;
		msg.SetObjID(CObjectManager::GetMe()->GetMySelf()->GetServerID());

		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	//����̯λ���ܣ�����
	INT StallSale::ApplyAd(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:StallSale::ApplyAd param parameter error");
		}
		std::string str	= args[2].GetString();

		if(str.size()>MAX_BBS_MESSAGE_LENGTH)
		{
			str.resize(MAX_BBS_MESSAGE_LENGTH);
		}
		CDataPool::GetMe()->MyStallMsg_SetTitle(str.c_str());

		CGBBSSychMessages msg;
		msg.SetOpt(CGBBSSychMessages::OPT_SET_TITLE);
		msg.SetLength((BYTE)str.size());
		msg.SetMessage((CHAR*)str.c_str());
		msg.SetObjID(CObjectManager::GetMe()->GetMySelf()->GetServerID());
		CNetManager::GetMe()->SendPacket(&msg);

		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���ѳɹ��ĸ����˹���");

		return 0;
	}
	//�ر�
	INT StallSale::CloseStallMessage(LuaPlus::LuaState* state)
	{
		//�ر�StallMessage
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_STALL_MESSAGE);
		//�ر�InputMoney
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_INPUT_MONEY);
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_PET_LIST);
		return 0;
	}
	//���̯λ��
	INT StallSale::GetStallName(LuaPlus::LuaState* state)
	{
		LPCTSTR pszStallName = CDataPool::GetMe()->MyStallBox_GetStallName();
		state->PushString(pszStallName);
		return 1;
	}
	//��ù����
	INT StallSale::GetAdvertise(LuaPlus::LuaState* state)
	{
		state->PushString(CDataPool::GetMe()->MyStallMsg_GetTitle());
		return 1;
	}
	//�����Լ���GUID
	INT StallSale::GetGuid(LuaPlus::LuaState* state)
	{
		UINT unNum = CDataPool::GetMe()->MyStallBox_GetOwnerGUID();
		
		CHAR str[32];
		sprintf(str,"%X",unNum);

		state->PushString(str);
		return 1;
	}
	// ��ó���ĸ���
	INT StallSale::GetPetNum(LuaPlus::LuaState* state)
	{
		state->PushInteger(CDataPool::GetMe()->MyStallBox_GetCount());
		return 1;
	}
	// ��ó���
	INT StallSale::EnumPet(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:StallSale::EnumPet[2] param parameter error");
		}
		INT nIndex = args[2].GetInteger();

		SDATA_PET* pPet = CDataPool::GetMe()->MyStallBox_GetPet(nIndex);
		if(pPet)
		{
			STRING szName = pPet->m_szName;
			state->PushString(szName.c_str());
		}
		else
		{
			state->PushString("");
		}
		return 1;
	}
	// �����ϼ�
	INT StallSale::PetUpStall(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:StallSale::PetUpStall[2] param parameter error");
		}
		INT nMoney = args[2].GetInteger();

		//���Ƴ���İ�̯�۸�
		if(nMoney > 1000000000)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��̯ʱ���ý��г���ʮ������ϵĽ���");
			return 0;
		}
		if(nMoney <= 0)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�ϼ�ʧ�ܣ��ϼܼ۸���Ϊ��");
			return 0;
		}

		PET_GUID_t PetGuid;
		CDataPool::GetMe()->MyStallBox_GetSelectpetGuid(PetGuid);

		CGStallAddItem msg;
		msg.SetFromType(STALL_MSG::POS_PET);
		msg.SetPetGUID(PetGuid);
		msg.SetPrice(nMoney);

		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	// ������ļ۸�
	INT StallSale::PetReprice(LuaPlus::LuaState* state)
	{
		//���ĳ���۸�
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:StallSale::ItemReprice[2] param parameter error");
		}
		INT nMoney = args[2].GetInteger();

		if(nMoney > 1000000000)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��̯ʱ���ý��г���ʮ������ϵĽ���");
			return 0;
		}
		if(nMoney <= 0)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�ļ�ʧ�ܣ��ϼܼ۸���Ϊ��");
			return 0;
		}

		INT nPetId = CDataPool::GetMe()->MyStallBox_GetnSelectPetOnStall();

		SDATA_PET *pPet = CDataPool::GetMe()->MyStallBox_GetPet(nPetId);

		if(!pPet)
		{
			return 0;
		}

		CGStallItemPrice msg;
		msg.SetPetGUID(pPet->m_GUID);
		msg.SetPrice(nMoney);
		msg.SetSerial( CDataPool::GetMe()->MyStallBox_GetPetSerial(nPetId) );

		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	// ���ȱʡҳ
	INT StallSale::GetDefaultPage(LuaPlus::LuaState* state)
	{
		INT nPage = CDataPool::GetMe()->MyStallBox_GetDefaultPage();
		
		state->PushInteger(nPage);
		return 1;
	}
	// ���ȱʡҳ
	INT StallSale::SetDefaultPage(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:StallSale::SetDefaultPage[2] param parameter error");
		}
		INT nPage = args[2].GetInteger();
		CDataPool::GetMe()->MyStallBox_SetDefaultPage(nPage);

		//������Ϣ֪ͨ������
		CGStallDefaultPage msg;
		msg.SetDefaultPage((BYTE)nPage );

		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	//��ʾ������ϸ��Ϣ
	INT StallSale::ViewPetDesc(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:StallSale::ViewPetDesc[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:StallSale::ViewPetDesc[3] param parameter error");
		}
		std::string str = args[2].GetString();
		INT nIndex = args[3].GetInteger();
		
		SDATA_PET* My_Pet;
		if(str == "self")
		{
			My_Pet = CDataPool::GetMe()->MyStallBox_GetPet(nIndex);
		}
		else
		{
			My_Pet = CDataPool::GetMe()->OtStallBox_GetPet(nIndex);
		}
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			TDThrow("LUA:StallSale ViewPetDesc parameter error");
			return 0;
		}
		CDataPool::GetMe()->Pet_CopyToTarget(My_Pet);

		CEventSystem::GetMe()->PushEvent(GE_VIEW_EXCHANGE_PET);
		return 0;
	}
	//����ѡ�е��Լ�̯λ�ϵĳ���
	INT StallSale::SetSelectPet(LuaPlus::LuaState* state)//MyStallBox_SetnSelectPetOnStall
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:StallSale::SetSelectPet[2] param parameter error");
		}

		CDataPool::GetMe()->MyStallBox_SetnSelectPetOnStall(args[2].GetInteger());
		return 0;
	}

}
