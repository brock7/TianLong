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
#include "../object/character/obj_playermyself.h"
#include "../DataPool/GMDP_CharacterData.h"
#include "..\Procedure\GameProcedure.h"
#include "TDUISystem.h"

#include "CGBankMoney.h"
#include "CGBankRemoveItem.h"

#include "ExchangeMsgDefine.h"
#include "CGExchangeApplyI.h"
#include "CGExchangeReplyI.h"
#include "CGExchangeCancel.h"
#include "CGExchangeSynchLock.h"
#include "CGExchangeOkIII.h"
#include "CGExchangeSynchMoneyII.h"
#include "CGExchangeSynchItemII.h"
#include "CGItemSynch.h"

#include "TDUtil.h"


namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	Exchange Exchange::s_Exchange;
	LuaPlus::LuaObject* Exchange::s_pMetaTable = NULL;

	//�ĵ�����Ƿ����
	INT Exchange::IsPresent(LuaPlus::LuaState* state)
	{
		if(this == &s_Exchange)
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

	//���뽻��
	INT Exchange::SendExchangeApply(LuaPlus::LuaState* state)
	{
		// ����Լ����ڰ�̯�����ܽ���
		if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ڰ�̯����");
			return 0;
		}

		if(this == &s_Exchange)
		{
			CObject* pChar = (CObject*) CObjectManager::GetMe()->GetMainTarget();
			if(pChar && g_theKernel.IsKindOf(pChar->GetClass(), GETCLASS(CObject_Character)))
			{
				//���ж������ǲ��ǺϷ�
				if(pChar)
				{
					const fVector3 pMePos = CObjectManager::GetMe()->GetMySelf()->GetPosition();
					const fVector3 pOtherPos = ((CObject*)pChar)->GetPosition();

					FLOAT fDistance = 
						TDU_GetDist(fVector2(pMePos.x, pMePos.z),
									fVector2(pOtherPos.x, pOtherPos.z));

					if( EXCHANGE_MAX_DISTANCE < fDistance )
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����̫Զ������������");
						return 0;
					}
				}

				//�������Ѿ���������ҷ��ͽ�������
				if ( ((CObject_Character*)pChar)->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_DEAD )
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�������Ѿ�������������뽻��");
					return 0;
				}

				CGExchangeApplyI msg;
				msg.SetObjID(pChar->GetServerID());
				CNetManager::GetMe()->SendPacket(&msg);
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���������Ѿ�����");
				return 0;
			}
		}

		return 0;
	}

	// �򿪽��׶Ի���
	INT Exchange::OpenExchangeFrame(LuaPlus::LuaState* state)
	{
		//�ж��Լ�״̬�Ƿ�Ϸ�...
		if( CObjectManager::GetMe()->GetMySelf()->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_DEAD )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�Լ��������������뽻��");
			return 0;
		}

		// ����Լ����ڰ�̯�����ܽ���
		if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ڰ�̯����");
			return 0;
		}

		// �����ݳ��б���ȡ��һ���������Լ����׵��˵�ID
		if( CDataPool::GetMe()->IsStillAnyAppInList() )
		{
			INT nUserID = CDataPool::GetMe()->GetFromAppList();

			tObject *pObj = CObjectManager::GetMe()->FindServerObject(nUserID);

			//��һ�������ж������������ڽ�������������룬��ʾʧ��
			if(pObj)
			{
				const fVector3 pMePos = CObjectManager::GetMe()->GetMySelf()->GetPosition();
				const fVector3 pOtherPos = ((CObject*)pObj)->GetPosition();

				FLOAT fDistance = 
					TDU_GetDist(fVector2(pMePos.x, pMePos.z),
								fVector2(pOtherPos.x, pOtherPos.z));

				if( EXCHANGE_MAX_DISTANCE < fDistance )
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����̫Զ����������ʧ��");
					return 0;
				}
			}

			if(pObj != NULL)
			{
				//�ж�Ŀ��״̬�Ƿ�Ϸ�
				if( ((CObject_Character*)( CObjectManager::GetMe()->FindServerObject(nUserID) ))->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_DEAD )
				{
					
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�������������״̬�����ܽ���");
					return 0;
				}
				CGExchangeReplyI msg;
				msg.SetObjID(nUserID);
				CNetManager::GetMe()->SendPacket(&msg);
			}
			else
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ܹ����׵ķ�Χ");
			}
		}
		return 0;
	}

	// �����Լ����뽻�׵���Ʒ����
	// �Լ��ģ�"self"�����Է��ģ�"other"��
	INT Exchange::GetItemNum(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:Exchange::GetItemNum param parameter error");
		}

		std::string str	= args[2].GetString();
		if( str == "self" )		//�Լ�
		{
			state->PushInteger(CDataPool::GetMe()->MyExBox_GetItemNum());
			return 1;
		}
		if( str == "other" )	//�Է�
		{
			state->PushInteger(CDataPool::GetMe()->OtExBox_GetItemNum());
			return 1;
		}
		return 0;
	}

	// ����Action
	INT Exchange::GetItemAction(LuaPlus::LuaState* state)
	{
		return 1;
	}

	//�������׵Ľ�Ǯ��
	INT Exchange::GetMoney(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:Exchange::GetMoney param parameter error");
		}
		INT nMoney;
		INT nGoldCoin;	
		INT nSilverCoin;
		INT nCopperCoin;

		std::string str	= args[2].GetString();
		if( str == "self" )		//�Լ�
			nMoney = CDataPool::GetMe()->MyExBox_GetMoney();
		else if( str == "other" )	//�Է�
			nMoney = CDataPool::GetMe()->OtExBox_GetMoney();
			

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

	//��������
	INT Exchange::LockExchange(LuaPlus::LuaState* state)
	{
		//����������ȡ��������Ϣ
		CGExchangeSynchLock msg;
		if( CDataPool::GetMe()->MyExBox_GetLock() )
		{
			//�Լ���������״̬��ִ��ȡ�������Ĳ���
			msg.SetLockMyself(FALSE);
			//�����Լ�������
			CDataPool::GetMe()->MyExBox_SetLock(FALSE);
		}
		else
		{	
			//�Լ�������δ����״̬��ִ�������Ĳ���
			msg.SetLockMyself(TRUE);
			//�����Լ�������
			CDataPool::GetMe()->MyExBox_SetLock(TRUE);
		}
		CNetManager::GetMe()->SendPacket(&msg);

		//�����Լ��Ľ���
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_EXCHANGE);
		return 0;
	}
	
	//��������״̬
	INT Exchange::IsLocked(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:Exchange::GetMoney param parameter error");
		}

		BOOL bLocked;
		std::string str	= args[2].GetString();
		if( str == "self" )		//�Լ�
			bLocked = CDataPool::GetMe()->MyExBox_GetLock();
		else if( str == "other" )	//�Է�
			bLocked = CDataPool::GetMe()->OtExBox_GetLock();

		state->PushBoolean(bLocked==TRUE);
		return 1;
	}


	//ȷ������
	INT Exchange::AcceptExchange(LuaPlus::LuaState* state)
	{
		//���ͽ�����Ϣ
		CGExchangeOkIII msg;
		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}

	// ����ȡ��
	INT Exchange::ExchangeCancel(LuaPlus::LuaState* state)
	{
		// ����ȡ��������Ϣ
		CGExchangeCancel msg;
		CNetManager::GetMe()->SendPacket(&msg);

		// ֪ͨ�����Ǯ�Ի���ر�
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_INPUT_MONEY);
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_PET_LIST);

		return 0;
	}

	// �򿪳�����棨��ʱ�����򿪽�Ǯ�����
	INT Exchange::OpenPetFrame(LuaPlus::LuaState* state)
	{
		// �򿪽�Ǯ�����
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY,"exch");
		return 0;
	}

	// ��������ý�Ǯ
	INT Exchange::GetMoneyFromInput(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:Exchange::GetMoney param parameter error");
		}

		UINT nMoney = static_cast<UINT>(args[2].GetInteger());

		//�����ʱ�Լ��Ѿ���������״̬�������������Ч
		if( CDataPool::GetMe()->MyExBox_GetLock() )
		{
				
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���Ѿ������˽���");
			return 0;
		}

		//��Ҫ�ȼ����ǲ�������ô���Ǯ�����û�У��������ֵ
		//ͬʱ��Ҫ���鲻��Ϊ����
		CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
		if (nMoney > (UINT)pCharData->Get_Money()) 
		{
				
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����Я���Ľ�Ǯ����");
			return 0;
		}
		CDataPool::GetMe()->MyExBox_SetMoney(nMoney);
		//���ͽ�Ǯ��Ϣ
		CGExchangeSynchMoneyII msg;
		msg.SetMoney(nMoney);
		CNetManager::GetMe()->SendPacket(&msg);


		CEventSystem::GetMe()->PushEvent(GE_UPDATE_EXCHANGE);
		return 0;
	}

	//�������������б��Ƿ�Ϊ��
	INT Exchange::IsStillAnyAppInList(LuaPlus::LuaState* state)
	{
		state->PushBoolean(CDataPool::GetMe()->IsStillAnyAppInList() == TRUE);
		
		return 1;
	}

	//�������Լ����׵��˵�����
	INT Exchange::GetOthersName(LuaPlus::LuaState* state)
	{
		
		INT nObjId = CDataPool::GetMe()->GetExchangObjID();
		CObject_Character* pObj = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject( nObjId ));
		if(pObj)
		{
			state->PushString(pObj->GetCharacterData()->Get_Name());
			return 1;
		}

		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�Ҳ�������");
		state->PushString("������");
		return 1;
	}

	//ѡ��һ�������б��еĳ�����ȷ����Ĳ���
	INT Exchange::AddPet(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:Exchange:AddPet[2] param parameter error");
		}
		INT nPetIndex = args[2].GetInteger();		
		
		SDATA_PET* SelectPet = CDataPool::GetMe()->Pet_GetPet(nPetIndex);
		if( (!SelectPet) || (INVALID_ID == SelectPet->m_nIsPresent) )
		{
			return 0;
		}

		// ������׽��濪��
		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Exchange"))
		{
			CGExchangeSynchItemII msg;
			msg.SetOpt(EXCHANGE_MSG::OPT_ADDPET);
			msg.SetPetGuid(SelectPet->m_GUID);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		//�����̯���濪��
		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("StallSale"))
		{
			//��ѯ�����ǲ����Ѿ��ϼ�
			if(CDataPool::GetMe()->MyStallBox_IsPetOnStall(SelectPet->m_GUID))
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��������Ѿ��ϼ���");
				return 0;
			}

			//���ϼܽ���
			CDataPool::GetMe()->MyStallBox_SetSelectpetGuid(SelectPet->m_GUID);
			CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY,"st_pet");
		}
		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("PS_ShopMag"))
		{
			//������ŵ��̵���ͷȥ
			CGItemSynch msg;
			msg.SetOpt(CGItemSynch::OPT_MOVE_ITEM_AUTO);
			msg.SetFromType(CGItemSynch::POS_PET);
			msg.SetToType(CGItemSynch::POS_PLAYERSHOPPET);
			msg.SetPetGUID(SelectPet->m_GUID);

			CGAutoMovePetFromBagToPlayerShop_t ExtraDataInfo;
			ExtraDataInfo.m_ShopGuid		= CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
			ExtraDataInfo.m_nStallIndex		= (BYTE)CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
			msg.SetExtraInfoLength(ExtraDataInfo.GetSize());
			msg.SetExtraInfoData((BYTE*)&ExtraDataInfo);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("MissionReply"))
		{
			CEventSystem::GetMe()->PushEvent(GE_REPLY_MISSION_PET, nPetIndex);
		}
		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("PetSkillStudy"))
		{
			CEventSystem::GetMe()->PushEvent(GE_REPLY_MISSION_PET, nPetIndex);
		}

		return 0;
	}
	// ��ó���ĸ���
	INT Exchange::GetPetNum(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:Exchange::GetPetNum[2] param parameter error");
		}
		std::string str = args[2].GetString();
		if(str == "self")
		{
			state->PushInteger(CDataPool::GetMe()->MyExBox_GetCount());
		}
		else
		{
			state->PushInteger(CDataPool::GetMe()->OtExBox_GetCount());
		}

		return 1;
	}
	//
	INT Exchange::EnumPet(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:Exchange::EnumPet[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:Exchange::EnumPet[2] param parameter error");
		}

		INT nIndex = args[3].GetInteger();
		std::string str = args[2].GetString();
		if(str == "self")
		{
			SDATA_PET* pPet = CDataPool::GetMe()->MyExBox_GetPet(nIndex);
			if(pPet)
			{
				STRING szName = pPet->m_szName;
				state->PushString(szName.c_str());
			}
			else
			{
				state->PushString("");
			}
		}
		else
		{
			SDATA_PET* pPet = CDataPool::GetMe()->OtExBox_GetPet(nIndex);
			if(pPet)
			{
				STRING szName = pPet->m_szName;
				state->PushString(szName.c_str());
			}
			else
			{
				state->PushString("");
			}
		}
		return 1;
	}
	//�ӽ�����������һ������
	INT Exchange::DelSelectPet(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:Exchange::EnumPet[2] param parameter error");
		}

		INT nIndex = args[2].GetInteger();

		//���ӽ��׿������³������Ϣ
		CGExchangeSynchItemII msg;
		msg.SetOpt(EXCHANGE_MSG::OPT_REMOVEPET);
		msg.SetPetGuid(CDataPool::GetMe()->MyExBox_GetPet(nIndex)->m_GUID);

		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	//��ʾ������ϸ��Ϣ
	INT Exchange::ViewPetDesc(LuaPlus::LuaState* state)//("self",g_nSelectPet);
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:Exchange::ViewPetDesc[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:Exchange::ViewPetDesc[3] param parameter error");
		}
		
		std::string str = args[2].GetString();
		INT nIndex = args[3].GetInteger();

		SDATA_PET* pPet;
		if(str == "self")
		{
			pPet = CDataPool::GetMe()->MyExBox_GetPet( nIndex );
		}
		else
		{
			pPet = CDataPool::GetMe()->OtExBox_GetPet( nIndex );
		}
		if( (!pPet) || (INVALID_ID == pPet->m_nIsPresent) )
		{
			TDThrow("LUA:Exchange ViewPetDesc parameter error");
			return 0;
		}
		CDataPool::GetMe()->Pet_CopyToTarget(pPet);

		CEventSystem::GetMe()->PushEvent(GE_VIEW_EXCHANGE_PET);
		return 0;
	}
};


