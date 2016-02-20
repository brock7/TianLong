#ifndef __LUAFNTBL_SHOP_H__
#define __LUAFNTBL_SHOP_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "GCShopMerchandiseList.h"
#include "GCShopSoldList.h"
#include "ShopManager.h"
#include "GCMenpaiInfo.h"
#include "Obj_Monster.h"
#include "ShopManager.h"
#include "GCXinfaStudyInfo.h"
#include "GCXinfaStudyInfo.h"
#include "GCAbilityTeacherInfo.h"
#include "GCPlayerShopError.h"
#include "GCPlayerShopApply.h"
#include "GCPlayerShopAcquireShopList.h"
#include "GCItemList.h"
#include "GCDetailAttrib_Pet.h"
#include "GameTable.h"
#include "RecordOpt.h"
#include "CGPlayerShopAcquireItemList.h"
#include "GCPlayerShopUpdatePartners.h"
using namespace ITEM_SYNCH_STRUCT;

namespace LuaFnTbl
{
	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-10-12
	*     �������� : LuaFnDispatchShopItem
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				ShopId
	*					�̵���
	*				
	*/
	INT	LuaFnDispatchShopItem(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);
		ObjID_t ShopId			=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnDispatchShopItem")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(targetId));
			Assert(pObj);
			if(pObj && IsCharacterObj( pObj->GetObjType() ))
			{
				Obj_Monster* pNpc = (Obj_Monster*)pObj;
				Assert(pNpc);
				if(pNpc)
				{

					_SHOP* pShop =	NULL;

					//�Ȳ��Լ����Ϲ�û���̵�
					if(pNpc->GetShopManager() == NULL)
					{//û�У�ֱ���ھ�̬���в�������̵�
                        pShop	=	g_pStaticShopManager->GetShopByID(ShopId);
					}
					else
					{//�Լ������ж�̬��
						//�����Լ�������
						pShop	=	pNpc->GetShopManager()->GetShopByID(ShopId);
						if(!pShop)
						{//û�У��ص���̬������
							 pShop	=	g_pStaticShopManager->GetShopByID(ShopId);
						}
					}

					//��û�У�һ����shopid������
					if(!pShop)
						Assert(0);

					//�����Ϣ
					GCShopMerchandiseList::_MERCHANDISE_ITEM	MerchandiseList[MAX_BOOTH_NUMBER];

					INT k = 0;

					if( pCharacter->IsEnemy( pNpc ) == FALSE )//���ǵж���Ӫ��NPC���������㶫��
					{
						for(INT i = 0;i<pShop->m_ItemList->m_ListCount;i++)
						{
							MerchandiseList[k].idTable		=	pShop->m_ItemList->m_ListTypeIndex[i];
							if(pShop->m_IsDyShop == TRUE)
								MerchandiseList[k].MaxNumber	=	pShop->m_ItemList->m_TypeMaxNum[i];
							else
								MerchandiseList[k].MaxNumber	=	-1;

							//�۸�ļ��㣬
							UINT BaseMoney = ShopMgr::ConvertItemType2Money(pShop->m_ItemList->m_ListType[i]);
							BaseMoney *= static_cast<INT>(pShop->m_scale);
							MerchandiseList[k].nPrice		=	BaseMoney;

							MerchandiseList[k++].byNumber	=	pShop->m_ItemList->m_TypeCount[i];
						}

					}

					GCShopMerchandiseList	Msg;
					Msg.SetMerchadiseNum(k);//һ��Ҫ����������������List
					Msg.SetMerchadiseList(MerchandiseList);

					Msg.SetBuyType(pShop->m_nBuyType);
					Msg.SetBuyLevel(pShop->m_nBuyLevel);
					Msg.SetRepairLevel(pShop->m_nRepairLevel);
					Msg.SetRepairType(pShop->m_nRepairType);
					Msg.SetUniqueID(pNpc->GetUniqueID());
					Msg.SetCanBuyBack(pShop->m_bCanBuyBack);
					Msg.SetScale(pShop->m_scale);
					Msg.SetCurrencyUnit( pShop->m_nCurrencyUnit );
					Msg.SetSerialNum( pShop->GetSerialNum() );
					Msg.SetRepairOkProb( pShop->m_nRepairOkProb );
					Msg.SetRepairSpend( pShop->m_nRepairSpend );
					Msg.SetBuyMulti(pShop->m_bCanMultiBuy);
					Msg.SetShopType(pShop->m_ShopType);

					//��������ID
					Msg.SetObjID(pNpc->GetID());

					//���˿�ʼ�����˽���
					pHuman->Shop(pShop);//�м�!!!������ɺ�һ��Ҫ��������NULL!!!
					//pShop->Custom(pHuman);//�м�!!!������ɺ�һ��Ҫ��������NULL!!!
					Player* pPlayer = pHuman->GetPlayer();
					pPlayer->SendPacket( &Msg ) ;

					GCShopSoldList::_MERCHANDISE_ITEM	SoldItem;
					_ITEM	tempit;
					UINT	uPrice = 0;
					BYTE	nCoinType = 0;
					if(pHuman->GetCurFromSoldList(tempit, uPrice, nCoinType))
					{//��������
						SoldItem.item_data	= tempit;
						SoldItem.iPrice		= uPrice;
						GCShopSoldList	MsgSold;
						MsgSold.SetMerchadiseNum(1);
						MsgSold.SetMerchadiseList(&SoldItem);
						pPlayer->SendPacket( &MsgSold );
					}
					else
					{
						GCShopSoldList	MsgSold;
						MsgSold.SetMerchadiseNum(0);
						MsgSold.SetMerchadiseList(&SoldItem);
						pPlayer->SendPacket( &MsgSold );
					}
				}
			}
		ENDHUMANDEFINE
		return	0;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-2-23
	*     �������� : LuaFnApplyPlayerShop
	*     ����˵�� : ֪ͨ�ͻ��˵������뿪��
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				
	*/
	INT	LuaFnApplyPlayerShop(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnApplyPlayerShop")

			PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();

			pPlayerShopManager->ClampComFactor();
			FLOAT CommercialFactor = pScene->GetPlayerShopManager()->GetComFactor();
			UINT Cost = (UINT)(300000*CommercialFactor*2*1.03);
			
			GCPlayerShopError MsgError;

			//��֤�Ƿ����ʸ��̯
			if(pHuman->GetLevel()<30)
			{
				MsgError.SetID(PLAYERSHOP_MSG::ERR_NOT_ENOUGH_LEVEL);
				pHuman->GetPlayer()->SendPacket(&MsgError);
				return 0 ;
			}

			BOOL bItem  =TRUE;
			BOOL bPet  =TRUE;
			
			//��Ҫ����֤�ǲ����Ѿ����������̵���
			if( !pHuman->GetShopGuid(0).isNull() )
			{
				PlayerShop* pCurShop = pPlayerShopManager->GetPlayerShopByGUID(pHuman->GetShopGuid(0));
				if(!pCurShop)
				{
					pHuman->GetShopGuid(0).Reset();
					bItem = TRUE;
				}
				else if(pCurShop->GetOwnerGuid() == pHuman->GetGUID())
				{
					bItem = FALSE;
				}
				else
				{
					pHuman->GetShopGuid(0).Reset();
					bItem = TRUE;
				}
			}
			if( !pHuman->GetShopGuid(1).isNull() )
			{
				PlayerShop* pCurShop = pPlayerShopManager->GetPlayerShopByGUID(pHuman->GetShopGuid(1));
				if(!pCurShop)
				{
					pHuman->GetShopGuid(1).Reset();
					bPet = TRUE;
				}
				else if(pCurShop->GetOwnerGuid() == pHuman->GetGUID())
				{
					bPet = FALSE;
				}
				else
				{
					pHuman->GetShopGuid(1).Reset();
					bPet = TRUE;
				}
			}

			if(!bItem && !bPet)
			{
				GCPlayerShopError MsgError;
				MsgError.SetID(PLAYERSHOP_MSG::ERR_ALREADY_HAVE_ENOUGH_SHOP);
				pHuman->GetPlayer()->SendPacket(&MsgError);
			}
			else
			{
				GCPlayerShopApply Msg;
				Msg.SetComFactor(CommercialFactor);
				Msg.SetCost(Cost);
				Msg.SetNpcId(targetId);
				if(bItem&&bPet)
				{
					Msg.SetType(PLAYERSHOP_MSG::TYPE_BOTH);
				}
				else if(bItem)
				{
					Msg.SetType(PLAYERSHOP_MSG::TYPE_ITEM);
				}
				else if(bPet)
				{
					Msg.SetType(PLAYERSHOP_MSG::TYPE_PET);
				}

				//��֤���,֪ͨ�ͻ���ȷ��
				pHuman->GetPlayer()->SendPacket(&Msg);
			}
		ENDHUMANDEFINE
			return	0;

		LUA_LEAVE_FUNCTION
			return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-2-23
	*     �������� : LuaFnDispatchPlayerShopList
	*     ����˵�� : ���ʹ˳����е��̵��б�
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				
	*/
	INT	LuaFnDispatchPlayerShopList(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDispatchPlayerShopList")

			SHOP_INFO_t aShopList[MAX_SHOP_NUM_PER_SCENE];
			UINT ListIndex = 0;
			PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
			pPlayerShopManager->ClampComFactor();
			FLOAT	fComFactor	= pPlayerShopManager->GetComFactor();
			_PLAYERSHOP_GUID	bValid[MAX_FAVORITE_SHOPS];
			INT	k = 0;

			for(UINT i = 0; i<MAX_SHOP_NUM_PER_SCENE; i++)
			{
				PlayerShop*	pPlayerShop	= pPlayerShopManager->GetPlayerShopByIndex(i);
				//ֻ��ʾ���ŵ��̵�
				if(!pPlayerShop)
					continue;
				if(pPlayerShop->GetShopStatus() != PLAYER_SHOP::STATUS_PLAYER_SHOP_OPEN)
					continue;
				Assert(pPlayerShop);

				if(pHuman->IsInFavorite(pPlayerShop->GetShopGUID()))
				{
					aShopList[ListIndex].m_bIsInFavor = TRUE;
					bValid[k++] = pPlayerShop->GetShopGUID();
				}

				aShopList[ListIndex].m_ID				=	pPlayerShop->GetShopGUID();
				aShopList[ListIndex].m_StallNumOnSale	=	pPlayerShop->GetNumStallOnSale();
				aShopList[ListIndex].m_StallNumOpened	=	pPlayerShop->GetNumStallOpened();
				aShopList[ListIndex].m_Type				=	(BYTE)pPlayerShop->GetShopType();
				aShopList[ListIndex].m_OwnerGuid		=	pPlayerShop->GetOwnerGuid();
				aShopList[ListIndex].m_Year				=	pPlayerShop->GetFoundedYear();	
				aShopList[ListIndex].m_Month			=	pPlayerShop->GetFoundedMonth();	
				aShopList[ListIndex].m_Day				=	pPlayerShop->GetFoundedDay();
				memcpy(aShopList[ListIndex].m_OwnerName, pPlayerShop->GetOwnerName(), MAX_CHARACTER_NAME);
				memcpy(aShopList[ListIndex].m_ShopName, pPlayerShop->GetShopName(), MAX_SHOP_NAME_SIZE);
				memcpy(aShopList[ListIndex].m_ShopDesc, pPlayerShop->GetShopDesc(), MAX_SHOP_DESC_SIZE);
				ListIndex++;
			}

			//�����Լ����ղؼ�,������̵��Ѿ�������,���߱��̳���,ɾ��ԭ����.
			pHuman->UpdateFavorite(bValid);

			GCPlayerShopAcquireShopList Msg;
			Msg.SetShopNum(ListIndex);
			Msg.SetComFactor(fComFactor);
			Msg.SetShopList(aShopList);
			Msg.SetNpcId(targetId);

			pHuman->GetPlayer()->SendPacket(&Msg);

		ENDHUMANDEFINE
			return	0;

		LUA_LEAVE_FUNCTION
			return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-3-18
	*     �������� : LuaFnDispatchPlayerShopSaleOutList
	*     ����˵�� : ���ʹ˳����е��̳����̵��б�
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				
	*/
	INT	LuaFnDispatchPlayerShopSaleOutList(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDispatchPlayerShopSaleOutList")

			SHOP_INFO_t aShopList[MAX_SHOP_NUM_PER_SCENE];
			UINT ListIndex = 0;
			PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
			pPlayerShopManager->ClampComFactor();
			FLOAT	fComFactor	= pPlayerShopManager->GetComFactor();

			for(UINT i = 0; i<MAX_SHOP_NUM_PER_SCENE; i++)
			{
				PlayerShop*	pPlayerShop	= pPlayerShopManager->GetPlayerShopByIndex(i);
				//ֻ��ʾ���ŵ��̵�
				if(!pPlayerShop)
					continue;
				if(pPlayerShop->GetShopStatus() != PLAYER_SHOP::STATUS_PLAYER_SHOP_ON_SALE)
					continue;
				Assert(pPlayerShop);
				aShopList[ListIndex].m_ID				=	pPlayerShop->GetShopGUID();
				aShopList[ListIndex].m_StallNumOnSale	=	pPlayerShop->GetNumStallOnSale();
				aShopList[ListIndex].m_StallNumOpened	=	pPlayerShop->GetNumStallOpened();
				aShopList[ListIndex].m_Type				=	(BYTE)pPlayerShop->GetShopType();
				aShopList[ListIndex].m_OwnerGuid		=	pPlayerShop->GetOwnerGuid();
				aShopList[ListIndex].m_Year				=	pPlayerShop->GetFoundedYear();	
				aShopList[ListIndex].m_Month			=	pPlayerShop->GetFoundedMonth();	
				aShopList[ListIndex].m_Day				=	pPlayerShop->GetFoundedDay();	
				memcpy(aShopList[ListIndex].m_OwnerName, pPlayerShop->GetOwnerName(), MAX_CHARACTER_NAME);
				memcpy(aShopList[ListIndex].m_ShopName, pPlayerShop->GetShopName(), MAX_SHOP_NAME_SIZE);
				memcpy(aShopList[ListIndex].m_ShopDesc, pPlayerShop->GetShopDesc(), MAX_SHOP_DESC_SIZE);
				ListIndex++;
			}

			GCPlayerShopAcquireShopList Msg;
			Msg.SetShopNum(ListIndex);
			Msg.SetComFactor(fComFactor);
			Msg.SetShopList(aShopList);
			Msg.SetNpcId(targetId);

			pHuman->GetPlayer()->SendPacket(&Msg);

		ENDHUMANDEFINE
			return	0;

		LUA_LEAVE_FUNCTION
			return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-10-20
	*     �������� : LuaFnDispatchXinfaInfo
	*     ����˵�� : 
	*     ����˵�� : �����ķ���ʦʹ��
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				MenpaiId
	*					�����ʦ��ID������ID��
	*/
	INT LuaFnDispatchXinfaLevelInfo(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);
		ObjID_t MenpaiId		=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnDispatchXinfaLevelInfo")
		{
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(targetId));
			Assert(pObj);
			//��ͻ��˷��������ķ���ʦ��������Ϣ
			GCXinfaStudyInfo msg;
			msg.SetMenpaiInfo_ID(MenpaiId);
			msg.SetTeacherId(targetId);

			if(pObj && IsCharacterObj( pObj->GetObjType() ))
			{
				Player* pPlayer = pHuman->GetPlayer();
				pPlayer->SendPacket( &msg ) ;
			}
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-10-20
	*     �������� : LuaFnDispatchMenpaiInfo
	*     ����˵�� : 
	*     ����˵�� : ���ɵ�ʦʹ��
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				MenpaiId
	*					�����ʦ��ID������ID��
	*/				
	INT LuaFnDispatchMenpaiInfo(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);
		ObjID_t MenpaiId		=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnDispatchMenpaiInfo")
		{
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(targetId));
			Assert(pObj);

			//��ͻ��˷������ɽ�ʦ��������Ϣ
			GCMenpaiInfo Msg;
			Msg.SetMenpaiInfo_ID((short)MenpaiId);

			if(pObj && IsCharacterObj( pObj->GetObjType() ))
			{
				Player* pPlayer = pHuman->GetPlayer();
				pPlayer->SendPacket( &Msg ) ;
			}
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-10-20
	*     �������� : LuaFnDispatchAbilityInfo
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				AbilityId
	*					�����ʦ���̵�����ܵ�ID
	*/
	INT LuaFnDispatchAbilityInfo(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);
		UINT scriptId		=	Lua_ValueToNumber(L,4);
		ObjID_t AbilityId		=	Lua_ValueToNumber(L,5);
		UINT nNeedMoney		=	Lua_ValueToNumber(L,6);
		UINT nNeedExp		=	Lua_ValueToNumber(L,7);
		UINT nNeedSkillExp	=	Lua_ValueToNumber(L,8);
		INT nLevelLimit		=	Lua_ValueToNumber(L,9);


		BEGINHUMANDEFINE("LuaFnDispatchAbilityInfo")
		{
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(targetId));
			Assert(pObj);

			//��ͻ��˷��ͽ�ʦ���������Ϣ
			GCAbilityTeacherInfo Msg;
			GCAbilityTeacherInfo::_AbilityTeacherInfo StudyInfo;
			StudyInfo.m_nAbilityID	= AbilityId;
			StudyInfo.m_uSpendMoney	= nNeedMoney;
			StudyInfo.m_uSpendExp	= nNeedExp;
			StudyInfo.m_uSkillExp	= nNeedSkillExp;
			StudyInfo.m_uTeacherId	= targetId;
			StudyInfo.m_uScriptId	= scriptId;
			StudyInfo.m_nLevelLimit = nLevelLimit;

			Msg.SetAbilityTeacherInfo(StudyInfo);

			if(pObj && IsCharacterObj( pObj->GetObjType() ))
			{
				Player* pPlayer = pHuman->GetPlayer();
				pPlayer->SendPacket( &Msg ) ;
			}
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-01-09
	*     �������� : LuaFnJoinMenpai
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				AbilityId
	*					�����ʦ���̵�����ܵ�ID
	*/
	INT LuaFnJoinMenpai(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);
		ObjID_t MenpaiId		=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnJoinMenpai")
		{
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(targetId));
			Assert(pObj);

			//1�����������ķ�����
			if(pObj && IsCharacterObj( pObj->GetObjType() ))
			{
				//ˢ�������ķ��ͼ��ܵ��ͻ���
				SKILL_LOG_PARAM SkillLogParam;
				SkillLogParam.CharGUID	=	pHuman->GetGUID();
				SkillLogParam.SceneID	=	pHuman->getScene()->SceneID();
				SkillLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				SkillLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;

				SkillLogParam.OPType	=	SKILL_OP_SCRIPT_CHANGEMENPAI;

				//XINFA_LOG_PARAM XinFaLogParam;
				//XinFaLogParam.CharGUID	=	pHuman->GetGUID();
				//XinFaLogParam.SceneID	=	pHuman->getScene()->SceneID();
				//XinFaLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				//XinFaLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				//XinFaLogParam.OPType	=	XINFA_OP_SCRIPT_CHANGEMENPAI;	

				//ˢ���ķ��ͼ���
				pHuman->ChangeMenpai(&SkillLogParam,MenpaiId);

				//ˢ���������Ե������ͻ���
				pHuman->ChangeMenpaiPoints();
			}
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-4-26
	*     �������� : LuaFnGetShopName
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				iShopType
	*					�̵�����
	*     ����˵�� :
	*				���ָ�������ϵ�����̵���
	*/
	INT LuaFnGetShopName(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber(L,1);
		ObjID_t selfId		= Lua_ValueToNumber(L,2);
		INT iShopType	= Lua_ValueToNumber(L,3);

		if( iShopType!=0 && iShopType!=1 )
		{
			Lua_PushString(L,"");
			return 1;
		}

		BEGINHUMANDEFINE("LuaFnGetShopName")

		_PLAYERSHOP_GUID	ShopGuid = pHuman->GetShopGuid(iShopType);
		if(ShopGuid.isNull()) 
		{
			Lua_PushString(L,"");
			return 1;
		}

		PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
		PlayerShop* pPlayerShop = pPlayerShopManager->GetPlayerShopByGUID(ShopGuid);
		if(pPlayerShop == NULL)
		{
			Lua_PushString(L,"");
			return 1;
		}

		if(pPlayerShop->GetOwnerGuid() != pHuman->GetGUID())
		{
			pHuman->GetShopGuid(iShopType).Reset();
			Lua_PushString(L,"");
			return 1;
		}
		
		Lua_PushString(L,pPlayerShop->GetShopName());
		return 1;

		ENDHUMANDEFINE
		Lua_PushString(L,"");
		return 1;

		LUA_LEAVE_FUNCTION

		Lua_PushString(L,"");
		return 1;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-4-26
	*     �������� : LuaFnOpenPlayerShop
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				iShopType
	*					�̵�����
	*     ����˵�� :
	*				���ָ�������ϵ�����̵���
	*/
	INT LuaFnOpenPlayerShop(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber(L,1);
		ObjID_t selfId		= Lua_ValueToNumber(L,2);
		INT iShopType	= Lua_ValueToNumber(L,3);

		if( iShopType!=0 && iShopType!=1 )
		{
			return 0;
		}

		BEGINHUMANDEFINE("LuaFnOpenPlayerShop")
		_PLAYERSHOP_GUID	nShopGuid = pHuman->GetShopGuid(iShopType);
		if(nShopGuid .isNull()) 
		{
			return 0;
		}

		PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
		PlayerShop* pPlayerShop = pPlayerShopManager->GetPlayerShopByGUID(nShopGuid);
		if(pPlayerShop == NULL)
		{
			return 0;
		}

		if(pPlayerShop->GetOwnerGuid() != pHuman->GetGUID())
		{
			pHuman->GetShopGuid(iShopType).Reset();
			return 0;
		}

		//������Ʒ�б�
		GCItemList	MsgItemList;
		
		//����̵��Ƿ��Ѿ��̳�
		BOOL bIsSaleOut = (pPlayerShop->GetShopStatus() == PLAYER_SHOP::STATUS_PLAYER_SHOP_ON_SALE)? TRUE:FALSE;

		//����̵��Ƿ��ǳ����
		BOOL bIsPetShop = (pPlayerShop->GetShopType() == PLAYER_SHOP::TYPE_PLAYER_SHOP_PET)? TRUE:FALSE;

		//�ǲ����Լ��ĵ�
		BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;

		PlayerStallBox*		pCurStallBox	= NULL;
		ItemContainer*		pCurContainer	= NULL;
		ItemContainer*		pCurPetContainer	= NULL;

		GCItemList::_MSG_ITEM ItemInMsg[STALL_BOX_SIZE*MAX_STALL_NUM_PER_SHOP];
		UINT				ItemNumInMsg = 0;
		UINT				uCurItemPrice = 0;
		UINT				uCurItemSerial = 0;
		Item*				pCurItem = NULL;
		BYTE	aStallIsOpen[MAX_STALL_NUM_PER_SHOP];
		memset(aStallIsOpen, 0, MAX_STALL_NUM_PER_SHOP);

		//��ҳ��
		INT		nStallIndex = 0;
		UINT	uBaseMoney	 = 0;
		UINT	uProfitMoney = 0;

		uBaseMoney		= pPlayerShop->GetBaseMoney();
		uProfitMoney	= pPlayerShop->GetProfitMoney();

		GCPlayerShopItemListForSelf_t ExtraInfoData;
		pCurStallBox = pPlayerShop->GetPlayerStallBoxByIndex(nStallIndex);
		if(TRUE)
		{//������Ʒ������
			for(UINT i = 0; i<MAX_STALL_NUM_PER_SHOP; i++)
			{
				PlayerStallBox* pTmpStallBox = pPlayerShop->GetPlayerStallBoxByIndex(i);
				aStallIsOpen[i] = (BYTE)pTmpStallBox->GetStallStatus();
			}

			if(bIsPetShop)
			{
				ExtraInfoData.m_bIsItemList = 0;
				ExtraInfoData.m_bUIFlag = CGPlayerShopAcquireItemList::ASK_OPEN_STALLL;

				pCurPetContainer = pCurStallBox->GetPetContainer();

				//ѭ��д��
				for(INT i = 0; i<pCurPetContainer->GetContainerSize(); i++)
				{
					if( pCurPetContainer->GetItem(i)->IsEmpty() == FALSE )
					{//�ж���
						INT	nPrice	= pCurStallBox->GetPriceByIndex(i);
						INT	nSerial	= pCurStallBox->GetSerialByIndex(i);

						Item* pIt = pCurPetContainer->GetItem(i);
						GCDetailAttrib_Pet PetMsgDetail;
						//��װGCDetailAttrib_Pet�ṹ
						Obj_Human::CalculatePetDetailAttrib(PetMsgDetail, pIt);
						PetMsgDetail.SetTradeIndex( i );
						BYTE nOnSale = pCurStallBox->CanSale(i);
						PET_EXTRA_STRUCT::GCPlayerShopPetView_t ExtraPetInfo;
						ExtraPetInfo.m_bFlag			= TYPE_PLAYERSHOP;
						ExtraPetInfo.m_nPrice			= nPrice;
						ExtraPetInfo.m_nSerial			= nSerial;
						ExtraPetInfo.m_bIsMine			= TRUE;
						ExtraPetInfo.m_nStallIndex		= nStallIndex;
						ExtraPetInfo.m_bIsOnSale		= nOnSale;

						PetMsgDetail.SetExtraInfoLength(ExtraPetInfo.GetSize());
						PetMsgDetail.SetExtraInfoData((BYTE*)&ExtraPetInfo);
						pHuman->GetPlayer()->SendPacket( &PetMsgDetail );		
					}
					else
					{//û�г��﷢���к�
						uCurItemSerial	=	pCurStallBox->GetSerialByIndex(i);
						ItemInMsg[ItemNumInMsg].nType		= GCItemList::TYPE_SERIALS;//���к�
						ItemInMsg[ItemNumInMsg].nIndex		= i;
						GCPlayerShopItemListEachSerialForSelf_t ExtraInfoData;
						ExtraInfoData.m_nStallIndex			= nStallIndex;
						ExtraInfoData.m_ItemSerial			= uCurItemSerial;
						ItemInMsg[ItemNumInMsg].ExtraLength	= ExtraInfoData.GetSize();
						memcpy(ItemInMsg[ItemNumInMsg].ExtraInfo, &ExtraInfoData, ItemInMsg[ItemNumInMsg].ExtraLength);
						ItemNumInMsg++;
					}
				}
			}
			else
			{
				ExtraInfoData.m_bIsItemList = 1;
				ExtraInfoData.m_bUIFlag = CGPlayerShopAcquireItemList::ASK_OPEN_STALLL;

				pCurContainer = pCurStallBox->GetContainer();
				for(UINT j = 0; j<STALL_BOX_SIZE; j++)
				{
					pCurContainer = pCurStallBox->GetContainer();
					pCurItem = pCurContainer->GetItem(j);
					if( !pCurItem->IsEmpty() )
					{//����Ʒ����
						uCurItemPrice	=	pCurStallBox->GetPriceByIndex(j);
						uCurItemSerial	=	pCurStallBox->GetSerialByIndex(j);
						pCurItem->SaveValueTo(&ItemInMsg[ItemNumInMsg].item_data);
						ItemInMsg[ItemNumInMsg].nType			= GCItemList::TYPE_ITEM;//��Ʒ
						ItemInMsg[ItemNumInMsg].nIndex			= j;

						GCPlayerShopItemListEachItemForSelf_t ExtraInfoData;
						ExtraInfoData.m_nStallIndex				=	nStallIndex;
						ExtraInfoData.m_ItemPrice				=	uCurItemPrice;	
						ExtraInfoData.m_ItemSerial				=	uCurItemSerial;
						ExtraInfoData.m_IsMine					=	TRUE;
						ExtraInfoData.m_IsOnSale				=	(BYTE)pCurStallBox->CanSale(j);	
						ItemInMsg[ItemNumInMsg].ExtraLength		=	ExtraInfoData.GetSize();
						memcpy(ItemInMsg[ItemNumInMsg].ExtraInfo, &ExtraInfoData, ItemInMsg[ItemNumInMsg].ExtraLength);
						ItemNumInMsg++;
					}
					else
					{//û����Ʒ�����к�
						uCurItemSerial	=	pCurStallBox->GetSerialByIndex(j);
						ItemInMsg[ItemNumInMsg].nType		= GCItemList::TYPE_SERIALS;//���к�
						ItemInMsg[ItemNumInMsg].nIndex		= j;
						GCPlayerShopItemListEachSerialForSelf_t ExtraInfoData;
						ExtraInfoData.m_nStallIndex			= nStallIndex;
						ExtraInfoData.m_ItemSerial			= uCurItemSerial;
						ItemInMsg[ItemNumInMsg].ExtraLength	= ExtraInfoData.GetSize();
						memcpy(ItemInMsg[ItemNumInMsg].ExtraInfo, &ExtraInfoData, ItemInMsg[ItemNumInMsg].ExtraLength);
						ItemNumInMsg++;
					}
				}
			}
		}

		MsgItemList.SetOpt(GCItemList::OPT_ADD_ITEM_LIST);
		MsgItemList.SetItemNum(ItemNumInMsg);
		MsgItemList.SetItemList(ItemInMsg);

		ExtraInfoData.m_ShopGuid		=	nShopGuid;
		ExtraInfoData.m_bFlag			=	GCPlayerShopItemListForSelf_t::FOR_MANAGER;
		ExtraInfoData.m_uBaseMoney		=	uBaseMoney;
		ExtraInfoData.m_uProfitMoney	=	uProfitMoney;
		ExtraInfoData.m_OwnerGuid		=	pPlayerShop->GetOwnerGuid();
		ExtraInfoData.m_bIsSaleOut		=	bIsSaleOut;
		ExtraInfoData.m_uSaleOutPrice	=	pPlayerShop->GetSaleOutPrice();
		ExtraInfoData.m_Serial			=	pPlayerShop->GetSerial();
		ExtraInfoData.m_ExRecListNum	=	RecordOpt::GetRecordNum(pPlayerShop->GetExchangeRecord());
		ExtraInfoData.m_MaRecListNum	=	RecordOpt::GetRecordNum(pPlayerShop->GetManagerRecord());
		ExtraInfoData.m_bStallNum		=	pPlayerShop->GetNumStallOpened();
		ExtraInfoData.m_bType			=	pPlayerShop->GetShopType();
		pPlayerShopManager->ClampComFactor();
		ExtraInfoData.m_ComFactor		=	pPlayerShopManager->GetComFactor();
		memcpy((CHAR*)ExtraInfoData.m_OwnerName, pPlayerShop->GetOwnerName(), MAX_CHARACTER_NAME);
		memcpy((CHAR*)ExtraInfoData.m_ShopName, pPlayerShop->GetShopName(), MAX_SHOP_NAME_SIZE);
		memcpy((CHAR*)ExtraInfoData.m_ShopDesc, pPlayerShop->GetShopDesc(), MAX_SHOP_DESC_SIZE);
		memcpy((CHAR*)ExtraInfoData.m_aStallIsOpen, (CHAR*)aStallIsOpen, MAX_STALL_NUM_PER_SHOP);
		MsgItemList.SetExtraLength(ExtraInfoData.GetSize());
		MsgItemList.SetExtraInfo((CHAR*)&ExtraInfoData);
	
		pHuman->GetPlayer()->SendPacket(&MsgItemList);

		//���ͺϻ�����Ϣ
		GCPlayerShopUpdatePartners					MsgPartnerToClient;
		GCPlayerShopUpdatePartners::MsgPartner_t	SendingArray[MAX_PARTNER_PER_SHOP];

		Partner_t*	pCurParList = pPlayerShop->GetPartnerList();
		INT k = 0;
		for(INT i = 0; i<MAX_PARTNER_PER_SHOP; i++)
		{
			if(pCurParList[i].m_Guid != INVALID_GUID)
			{
				SendingArray[k].CleanUp();
				SendingArray[k].m_Guid = pCurParList[i].m_Guid;
				memcpy(SendingArray[k++].m_szName, pCurParList[i].m_szName, MAX_CHARACTER_NAME);
			}
		}

		MsgPartnerToClient.SetShopID(nShopGuid);
		MsgPartnerToClient.SetPartnerNum(k);
		MsgPartnerToClient.SetPartnerList(SendingArray);

		pHuman->GetPlayer()->SendPacket(&MsgPartnerToClient);

		return 0;

		ENDHUMANDEFINE
		return 0;

		LUA_LEAVE_FUNCTION

		return 0;
	}
}

#endif