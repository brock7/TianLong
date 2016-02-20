#include "stdafx.h"
/*
ͬ���ͻ�����Ʒ�仯
*/
#include "CGItemSynch.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

#include "ItemOperator.h"
#include "HumanItemLogic.h"

#include "GCItemSynch.h"

#include "GCPlayerShopError.h"
#include "LogDefine.h"
#include "Obj_Pet.h"
#include "RecordOpt.h"

using namespace ITEM_SYNCH_STRUCT;

UINT CGItemSynchHandler::Execute( CGItemSynch* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	//����
	BYTE		Opt				=	pPacket->GetOpt();
	//�����ĸ�����
	BYTE		FromType		=	pPacket->GetFromType();
	//�ŵ��ĸ�����
	BYTE		ToType			=	pPacket->GetToType();
	//�ŵ�Ŀ�������ĸ�λ��
	BYTE		ToIndex			=	pPacket->GetToIndex();
	//���ƶ������guid
	_ITEM_GUID	ItemGuid		=	pPacket->GetItemGUID();
	//���ƶ������guid
	PET_GUID_t	PetGuid			=	pPacket->GetPetGUID();
	//������Ϣ����
	BYTE		ExtraInfoL		=	pPacket->GetExtraInfoLength();
	//������Ϣ����
	BYTE*		pExtraInfo		=	pPacket->GetExtraInfoData();
	//�������к�
	UINT		uFinalSerial	=	0;
	//����Ŀ��λ��
	UINT		uFinalToIndex	=	0;
	//����̵������Ϣ
	GCPlayerShopError	MsgError;
	//������Ϣ���ͻ���
	GCItemSynch			MsgToClient;

	switch(Opt)
	{
		case CGItemSynch::OPT_MOVE_ITEM_AUTO:
			{//�Զ��Ҹ�
				switch(FromType)
				{
				case CGItemSynch::POS_BAG:
					{
						switch(ToType)
						{
							case CGItemSynch::POS_PLAYERSHOP:
								{//����->�̵�
									CGAutoMoveItemFromBagToPlayerShop_t ExtraData;
                                    Assert(ExtraInfoL == ExtraData.GetSize());
									memcpy(&ExtraData, pExtraInfo, ExtraInfoL);

									_PLAYERSHOP_GUID	nShopID		= ExtraData.m_ShopGuid;
									BYTE				nStallIndex	= ExtraData.m_nStallIndex;
									BYTE				nShopSerial = ExtraData.m_nShopSerial;

									PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
									PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
									Assert(pPlayerShop);
									if(pPlayerShop->GetShopType() == PLAYER_SHOP::TYPE_PLAYER_SHOP_PET)
									{//�̵����ʹ���
										MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL:BAG->SHOP"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}

									if(nShopSerial != pPlayerShop->GetSerial())
									{
										MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_SALE_OUT_SERIAL_IS_CHANGED);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "CGItemSynchHandler::Name=%s Serials had changed"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}

									//�ǲ����Լ��ĵ�
									BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;

									//�ǲ����Լ����Թ���ĵ�
									BOOL bCanManager = pPlayerShop->IsPartner(pHuman->GetGUID());

									if(bIsMine == FALSE && bCanManager == FALSE)
									{
										//log...
										return PACKET_EXE_CONTINUE;
									}

									PlayerStallBox* pStallBox		= pPlayerShop->GetPlayerStallBoxByIndex(nStallIndex);
									Assert(pStallBox);
									ItemContainer*  pStallContainer = pStallBox->GetContainer();
									//ItemContainer* pMatContainer	= pHuman->GetMatContain();
									ItemContainer* pBaseContainer	= pHuman->GetBaseContain();
									//ItemContainer* pBagContainer	= NULL;

//									if(pMatContainer->GetIndexByGUID(&ItemGuid) >= 0)
//									{
//										pBagContainer = pMatContainer;
//									}
//									else if(pBaseContainer->GetIndexByGUID(&ItemGuid) >=0 )
//									{
//										pBagContainer = pBaseContainer;
//									}
//									else
//									{//guid�Ƿ�
////										Assert(0);
//										MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
//										pGamePlayer->SendPacket(&MsgError);
//										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL:World = %d, Server = %d, Serial = %d "
//											,pHuman->GetName(), ItemGuid.m_World, ItemGuid.m_Server, ItemGuid.m_Serial) ;
//										return PACKET_EXE_CONTINUE ;
//									}

									INT	IndexInBag = pBaseContainer->GetIndexByGUID(&ItemGuid);

									//������̯λ��
									//�Զ��Ҹ�
									INT result = 
										g_ItemOperator.MoveItem
										(
										pBaseContainer,
										IndexInBag,
										pStallContainer
										);

									if(result>=0)
									{//�ƶ��ɹ�
										
										
										//������Ϣ���ͻ���
										Item* pIt = pStallContainer->GetItem(result);
										if(pIt->IsEmpty() == FALSE)
										{


											ITEM_LOG_PARAM	ItemLogParam;
											ItemLogParam.OpType			=	ITEM_PLAYERSHOP_UP;
											ItemLogParam.ItemGuid		=	ItemGuid;
											ItemLogParam.ItemType		=	pIt->GetItemTableIndex();
											ItemLogParam.BagPos			=	IndexInBag;
											ItemLogParam.CharGUID		=	pHuman->GetGUID();
											ItemLogParam.XPos			=	pHuman->getWorldPos()->m_fX;
											ItemLogParam.ZPos			=	pHuman->getWorldPos()->m_fZ;
											ItemLogParam.SceneID		=	pHuman->getScene()->SceneID();
											SaveItemLog(&ItemLogParam);
											

											//���кŵ���
											uFinalSerial = pStallBox->IncSerialByIndex(result);
											
											//�ϻ���AAA�ϻ���BBBC������AAAΪ����������BBBΪ��Ʒ��CΪ������
											RecordOpt::Excute(REC_ADDITEM, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)pIt->GetItemTableIndex(), (INT)pIt->GetLayedNum());
											
											//������Ϣ���ͻ���
											MsgToClient.SetOpt(GCItemSynch::OPT_MOVE_ITEM);
											MsgToClient.SetFromType(GCItemSynch::POS_BAG);
											MsgToClient.SetToType(GCItemSynch::POS_PLAYERSHOP);
											MsgToClient.SetToIndex(result);
											MsgToClient.SetItemGuid(ItemGuid);

											GCMoveItemFromBagToPlayerShop_t ExtraInfoData;
											ExtraInfoData.m_ShopGuid		=	nShopID;
											ExtraInfoData.m_nStallIndex		=	nStallIndex;
											ExtraInfoData.m_uSerial			=	uFinalSerial;
											ExtraInfoData.m_nShopSerial		=	pPlayerShop->IncSerial();
											MsgToClient.SetExtraInfoLength(ExtraInfoData.GetSize());
											MsgToClient.SetExtraInfoData((BYTE*)&ExtraInfoData);
											pGamePlayer->SendPacket(&MsgToClient);
										}
									}
									else
									{//ʧ��
										if(result == ITEMOE_DESTOPERATOR_FULL)
										{
											MsgError.SetID(PLAYERSHOP_MSG::ERR_NOT_ENOUGH_ROOM_IN_STALL);
											pGamePlayer->SendPacket(&MsgError);
											g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NOT_ENOUGH_ROOM_IN_STALL"
												,pHuman->GetName()) ;
											return PACKET_EXE_CONTINUE ;
										}
										else
										{
											Assert(0);
											MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
											pGamePlayer->SendPacket(&MsgError);
											g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL, POS_BAG->POS_PLAYERSHOP AUTO POS"
												,pHuman->GetName()) ;
											return PACKET_EXE_CONTINUE ;
										}
									}
								}
								break;
							default:
								break;
						}
					}
					break;
				case CGItemSynch::POS_PET:
					{
						switch(ToType)
						{
						case CGItemSynch::POS_PLAYERSHOPPET:
							{//����->�̵�

								CGAutoMovePetFromBagToPlayerShop_t ExtraData;
								Assert(ExtraInfoL == ExtraData.GetSize());
								memcpy(&ExtraData, pExtraInfo, ExtraInfoL);

								_PLAYERSHOP_GUID	nShopID		= ExtraData.m_ShopGuid;
								BYTE				nStallIndex	= ExtraData.m_nStallIndex;
								BYTE				nShopSerial = ExtraData.m_nShopSerial;

								PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
								PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
								Assert(pPlayerShop);
								if(pPlayerShop->GetShopType() != PLAYER_SHOP::TYPE_PLAYER_SHOP_PET)
								{//�̵����ʹ���
									MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL:TYPE ERROR PET->SHOP"
										,pHuman->GetName()) ;
									return PACKET_EXE_CONTINUE ;
								}

								//�ǲ����Լ��ĵ�
								BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;

								//�ǲ����Լ����Թ���ĵ�
								BOOL bCanManager = pPlayerShop->IsPartner(pHuman->GetGUID());

								if(bIsMine == FALSE && bCanManager == FALSE)
								{
									//log...
									return PACKET_EXE_CONTINUE;
								}

								if(nShopSerial != pPlayerShop->GetSerial())
								{
									MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_SALE_OUT_SERIAL_IS_CHANGED);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "CGItemSynchHandler::Name=%s Serials had changed"
										,pHuman->GetName()) ;
									return PACKET_EXE_CONTINUE ;
								}

								PlayerStallBox* pStallBox		= pPlayerShop->GetPlayerStallBoxByIndex(nStallIndex);
								Assert(pStallBox);
								ItemContainer*	pStallContainer = pStallBox->GetPetContainer();

								ItemContainer*	pPetContainer = pHuman->GetPetContain();
								INT	FromIndex = pPetContainer->GetIndexByGUID( &PetGuid );

								//�ٻس���
								Item* pPetItem	= pPetContainer->GetItem(FromIndex);
								Obj_Pet* pMyPet	= pHuman->GetPet();
								PET_GUID_t	MyPetGuid;
								if(pMyPet)
								{
									MyPetGuid = pMyPet->GetPetGUID();
								}
								if(MyPetGuid == pPetItem->GetPetGUID())
									pHuman->ReCallPet();

								if(FromIndex<0)
								{
//									Assert(0);
									MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL:PetGuid "
										,pHuman->GetName()) ;
									return PACKET_EXE_CONTINUE ;
								}

								//������̯λ��
								//�Զ��Ҹ�
								INT result = 
									g_ItemOperator.MoveItem
									(
									pPetContainer,
									FromIndex,
									pStallContainer
									);

								if(result>=0)
								{//�ƶ��ɹ�
									//������Ϣ���ͻ���
									Item* pIt = pStallContainer->GetItem(result);
									if(pIt->IsEmpty() == FALSE)
									{
										PET_LOG_PARAM	PetLogParam;
										PetLogParam.CharGUID	=	pHuman->GetGUID();
										PetLogParam.OPType		=	PET_OP_PLAYERSHOP_UP;
										PetLogParam.PetGUID		=	pIt->GetPetGUID();
										PetLogParam.DataID		=	pIt->GetDataID();
										PetLogParam.SceneID		=	pHuman->getScene()->SceneID();
										PetLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
										PetLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
										SavePetLog(&PetLogParam);

										//���кŵ���
										uFinalSerial = pStallBox->IncSerialByIndex(result);

										//�ϻ���AAA�ϻ���BBBC������AAAΪ����������BBBΪ��Ʒ��CΪ������
										RecordOpt::Excute(REC_ADDPET, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (CHAR*)pIt->GetName(), (INT)1);

										//������Ϣ���ͻ���
										MsgToClient.SetOpt(GCItemSynch::OPT_MOVE_ITEM);
										MsgToClient.SetFromType(GCItemSynch::POS_PET);
										MsgToClient.SetToType(GCItemSynch::POS_PLAYERSHOPPET);
										MsgToClient.SetToIndex(result);
										MsgToClient.SetPetGUID(PetGuid);

										GCMovePetFromBagToPlayerShop_t ExtraInfoData;
										ExtraInfoData.m_ShopGuid		=	nShopID;
										ExtraInfoData.m_nStallIndex		=	nStallIndex;
										ExtraInfoData.m_uSerial			=	uFinalSerial;
										ExtraInfoData.m_nShopSerial		=	pPlayerShop->IncSerial();
										MsgToClient.SetExtraInfoLength(ExtraInfoData.GetSize());
										MsgToClient.SetExtraInfoData((BYTE*)&ExtraInfoData);
										pGamePlayer->SendPacket(&MsgToClient);
									}
								}
								else
								{//ʧ��
									if(result == ITEMOE_DESTOPERATOR_FULL)
									{
										MsgError.SetID(PLAYERSHOP_MSG::ERR_NOT_ENOUGH_ROOM_IN_STALL);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NOT_ENOUGH_ROOM_IN_STALL"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}
									else
									{
										Assert(0);
										MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL, POS_BAG->POS_PLAYERSHOP AUTO POS"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}
								}
							}
							break;
						default:
							break;
						}
					}
					break;
				case CGItemSynch::POS_PLAYERSHOP:
					{
						switch(ToType)
						{
						case CGItemSynch::POS_BAG:
							{//�̵�->����

								CGAutoMoveItemFromPlayerShopToBag_t ExtraData;
								Assert(ExtraInfoL == ExtraData.GetSize());
								memcpy(&ExtraData, pExtraInfo, ExtraInfoL);
								_PLAYERSHOP_GUID	nShopID			= ExtraData.m_ShopGuid;
								BYTE				nStallIndex		= ExtraData.m_nStallIndex;
								UINT				uSerial			= ExtraData.m_uSerial;
								UINT				nShopSerial		= ExtraData.m_nShopSerial;

								PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
								PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
								Assert(pPlayerShop);
								PlayerStallBox* pStallBox		= pPlayerShop->GetPlayerStallBoxByIndex(nStallIndex);
								Assert(pStallBox);
								ItemContainer*	pStallContainer = pStallBox->GetContainer();

								//�ǲ����Լ��ĵ�
								BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;

								//�ǲ����Լ����Թ���ĵ�
								BOOL bCanManager = pPlayerShop->IsPartner(pHuman->GetGUID());

								if(bIsMine == FALSE && bCanManager == FALSE)
								{
									//log...
									return PACKET_EXE_CONTINUE;
								}

								if(nShopSerial != pPlayerShop->GetSerial())
								{
									MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_SALE_OUT_SERIAL_IS_CHANGED);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "CGItemSynchHandler::Name=%s Serials had changed"
										,pHuman->GetName()) ;
									return PACKET_EXE_CONTINUE ;
								}

								INT	IndexInStall = pStallContainer->GetIndexByGUID(&ItemGuid);
								if(IndexInStall<0)
								{
									MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NEED_NEW_COPY: IndexInStall = %d"
										,pHuman->GetName(), IndexInStall) ;
									return PACKET_EXE_CONTINUE;
								}

								if( pStallBox->GetSerialByIndex(IndexInStall) != uSerial)
								{
									MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NEED_NEW_COPY: Serial = %d, BoxSerial = %d"
										,pHuman->GetName(), uSerial, pStallBox->GetSerialByIndex(IndexInStall)) ;
									return PACKET_EXE_CONTINUE;
								}

								Item* pItem = pStallContainer->GetItem(IndexInStall);

								ItemContainer* pBagContainer = HumanItemLogic::GetItemContain(pHuman, pItem->GetItemTableIndex());
								
								ITEM_LOG_PARAM	ItemLogParam;
								ItemLogParam.OpType			=	ITEM_PLAYERSHOP_DOWN;
								ItemLogParam.ItemGuid		=	pItem->GetGUID();
								ItemLogParam.ItemType		=	pItem->GetItemTableIndex();
								

								//����ռ�
								_EXCHANGE_ITEM_LIST			ItemListOtToMe;
								ItemListOtToMe.Init();
								ItemListOtToMe.AddItem(pItem);
								if( FALSE == HumanItemLogic::CanReceiveExchangeItemList( pHuman, ItemListOtToMe) )
								{//�ռ䲻��
									MsgError.SetID(PLAYERSHOP_MSG::ERR_NOT_ENOUGH_ROOM);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NOT_ENOUGH_ROOM"
										,pHuman->GetName()) ;
									return PACKET_EXE_CONTINUE;
								}

								//�ƶ���Ʒ
								//�Ƚ���
								//�Զ��Ҹ�,֧���Զ�����
								INT result = 
									g_ItemOperator.MoveItem
									(
									pStallContainer,
									IndexInStall,
									pBagContainer
									);

								if(result<0)
								{//ʧ��
									if(result == ITEMOE_DESTOPERATOR_FULL)
									{
										MsgError.SetID(PLAYERSHOP_MSG::ERR_NOT_ENOUGH_ROOM);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NOT_ENOUGH_ROOM_IN_STALL"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}
									else
									{
										Assert(0);
										MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL, POS_PLAYERSHOP->POS_BAG AUTO POS"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}
								}
								pItem = pBagContainer->GetItem(result);
								UINT	ToBagIndex = pBagContainer->ConIndex2BagIndex(result);
								
								ItemLogParam.BagPos			=	ToBagIndex;
								ItemLogParam.CharGUID		=	pHuman->GetGUID();
								ItemLogParam.XPos			=	pHuman->getWorldPos()->m_fX;
								ItemLogParam.ZPos			=	pHuman->getWorldPos()->m_fZ;
								ItemLogParam.SceneID		=	pHuman->getScene()->SceneID();
								SaveItemLog(&ItemLogParam);
								
								//ȡ�أ�AAAȡ����BBBC������AAAΪ����������BBBΪ��Ʒ��CΪ������
								RecordOpt::Excute(REC_DELITEM, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)pItem->GetItemTableIndex(), (INT)pItem->GetLayedNum());

								//����Ϣ֪ͨ�ͻ���
								//���кŵ���
								uFinalSerial = pStallBox->IncSerialByIndex(IndexInStall);
								MsgToClient.SetOpt(GCItemSynch::OPT_MOVE_ITEM);
								MsgToClient.SetFromType(GCItemSynch::POS_PLAYERSHOP);
								MsgToClient.SetToType(GCItemSynch::POS_BAG);
								MsgToClient.SetToIndex(ToBagIndex);
								MsgToClient.SetItemGuid(ItemGuid);

								GCMoveItemFromPlayerShopToBag_t ExtraInfoData;
								ExtraInfoData.m_ShopGuid		=	nShopID;
								ExtraInfoData.m_nStallIndex		=	nStallIndex;
								ExtraInfoData.m_Flag			=	1;
								ExtraInfoData.m_uSerial			=	uFinalSerial;
								ExtraInfoData.m_nShopSerial		=	pPlayerShop->IncSerial();
								MsgToClient.SetExtraInfoLength(ExtraInfoData.GetSize());
								MsgToClient.SetExtraInfoData((BYTE*)&ExtraInfoData);
								pGamePlayer->SendPacket(&MsgToClient);
							}
							break;
						default:
							break;
						}
					}
					break;
				case CGItemSynch::POS_PLAYERSHOPPET:
					{
						switch(ToType)
						{
							case CGItemSynch::POS_PET:
								{
									CGAutoMovePetFromPlayerShopToBag_t ExtraData;
									Assert(ExtraInfoL == ExtraData.GetSize());
									memcpy(&ExtraData, pExtraInfo, ExtraInfoL);
									_PLAYERSHOP_GUID	nShopID			= ExtraData.m_ShopGuid;
									BYTE				nStallIndex		= ExtraData.m_nStallIndex;
									UINT				uSerial			= ExtraData.m_uSerial;
									BYTE				nShopSerial		= ExtraData.m_nShopSerial;

									PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
									PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
									Assert(pPlayerShop);
									PlayerStallBox* pStallBox		= pPlayerShop->GetPlayerStallBoxByIndex(nStallIndex);
									Assert(pStallBox);
									ItemContainer*	pStallContainer = pStallBox->GetPetContainer();

									//�ǲ����Լ��ĵ�
									BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;

									//�ǲ����Լ����Թ���ĵ�
									BOOL bCanManager = pPlayerShop->IsPartner(pHuman->GetGUID());

									if(bIsMine == FALSE && bCanManager == FALSE)
									{
										//log...
										return PACKET_EXE_CONTINUE;
									}

									if(nShopSerial != pPlayerShop->GetSerial())
									{
										MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_SALE_OUT_SERIAL_IS_CHANGED);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "CGItemSynchHandler::Name=%s Serials had changed"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}

									INT	IndexInStall = pStallContainer->GetIndexByGUID(&PetGuid);
									if(IndexInStall<0)
									{
										MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NEED_NEW_COPY: IndexInStall = %d"
											,pHuman->GetName(), IndexInStall) ;
										return PACKET_EXE_CONTINUE;
									}

									if( pStallBox->GetSerialByIndex(IndexInStall) != uSerial)
									{
										MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NEED_NEW_COPY: Serial = %d, BoxSerial = %d"
											,pHuman->GetName(), uSerial, pStallBox->GetSerialByIndex(IndexInStall)) ;
										return PACKET_EXE_CONTINUE;
									}

									ItemContainer*	pPetContainer = pHuman->GetPetContain();
									/*INT	IndexInBag = pPetContainer->GetIndexByGUID(&PetGuid);
									if(IndexInBag<0)
									{
										MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
										pHuman->m_StallBox.CleanUp();
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL: IndexInBag = %d"
											,pHuman->GetName(), IndexInBag) ;
										return PACKET_EXE_CONTINUE;
									}*/

									Item* pItem = pStallContainer->GetItem(IndexInStall);
									PET_LOG_PARAM	PetLogParam;
									PetLogParam.PetGUID		=	pItem->GetPetGUID();
									PetLogParam.DataID		=	pItem->GetDataID();

									//����ռ�
									_EXCHANGE_ITEM_LIST			ItemListOtToMe;
									ItemListOtToMe.Init();
									ItemListOtToMe.AddItem(pItem);
									if( FALSE == HumanItemLogic::CanReceiveExchangeItemList( pHuman, ItemListOtToMe) )
									{//�ռ䲻��
										MsgError.SetID(PLAYERSHOP_MSG::ERR_NOT_ENOUGH_ROOM);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NOT_ENOUGH_ROOM"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE;
									}

									//�ƶ���Ʒ
									//�Ƚ���
									//�Զ��Ҹ�,֧���Զ�����
									INT result = 
										g_ItemOperator.MoveItem
										(
										pStallContainer,
										IndexInStall,
										pPetContainer
										);

									if(result<0)
									{//ʧ��
										if(result == ITEMOE_DESTOPERATOR_FULL)
										{
											MsgError.SetID(PLAYERSHOP_MSG::ERR_NOT_ENOUGH_ROOM);
											pGamePlayer->SendPacket(&MsgError);
											g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NOT_ENOUGH_ROOM_IN_STALL"
												,pHuman->GetName()) ;
											return PACKET_EXE_CONTINUE ;
										}
										else
										{
											Assert(0);
											MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
											pGamePlayer->SendPacket(&MsgError);
											g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL, POS_PLAYERSHOP->POS_BAG AUTO POS"
												,pHuman->GetName()) ;
											return PACKET_EXE_CONTINUE ;
										}
									}
									
									PetLogParam.CharGUID	=	pHuman->GetGUID();
									PetLogParam.OPType		=	PET_OP_PLAYERSHOP_DOWN;
								
									PetLogParam.SceneID		=	pHuman->getScene()->SceneID();
									PetLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
									PetLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
									SavePetLog(&PetLogParam);

									//����Ϣ֪ͨ�ͻ���
									//���кŵ���
									uFinalSerial = pStallBox->IncSerialByIndex(IndexInStall);

									Item* pPetItem = pPetContainer->GetItem(result);

									//ȡ�أ�AAAȡ����BBBC������AAAΪ����������BBBΪ��Ʒ��CΪ������
									RecordOpt::Excute(REC_DELPET, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (CHAR*)pPetItem->GetName(), (INT)1);


									MsgToClient.SetOpt(GCItemSynch::OPT_MOVE_ITEM);
									MsgToClient.SetFromType(GCItemSynch::POS_PLAYERSHOPPET);
									MsgToClient.SetToType(GCItemSynch::POS_PET);
									MsgToClient.SetToIndex(result);
									MsgToClient.SetPetGUID(PetGuid);

									GCMovePetFromPlayerShopToBag_t ExtraInfoData;
									ExtraInfoData.m_ShopGuid		=	nShopID;
									ExtraInfoData.m_nStallIndex		=	nStallIndex;
									ExtraInfoData.m_Flag			=	1;
									ExtraInfoData.m_uSerial			=	uFinalSerial;
									ExtraInfoData.m_nShopSerial		=	pPlayerShop->IncSerial();
									MsgToClient.SetExtraInfoLength(ExtraInfoData.GetSize());
									MsgToClient.SetExtraInfoData((BYTE*)&ExtraInfoData);
									pGamePlayer->SendPacket(&MsgToClient);

								}
								break;
							default:
								break;
						}
					}
					break;

				default:
					break;
				}
			}
			break;
		case CGItemSynch::OPT_MOVE_ITEM_MANU:
			{
				switch(FromType)
				{
				case CGItemSynch::POS_BAG:
					{
						switch(ToType)
						{
						case CGItemSynch::POS_PLAYERSHOP:
							{//����->�̵�,ָ��λ��

								CGManuMoveItemFromBagToPlayerShop_t ExtraData;
                                Assert(ExtraInfoL == ExtraData.GetSize());
								memcpy(&ExtraData, pExtraInfo, ExtraInfoL);
								BYTE				nToIndex	= ToIndex;
								_PLAYERSHOP_GUID	nShopID		= ExtraData.m_ShopGuid;
								BYTE				nStallIndex	= ExtraData.m_nStallIndex;
								UINT				uSerial		= ExtraData.m_uSerial;//�̵����λ�õ����к�(Ŀ��λ�����к�)
								BYTE				nShopSerial	= ExtraData.m_nShopSerial;

                                PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
								PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
								Assert(pPlayerShop);
								PlayerStallBox* pStallBox		= pPlayerShop->GetPlayerStallBoxByIndex(nStallIndex);
								Assert(pStallBox);
								ItemContainer*	pStallContainer = pStallBox->GetContainer();

								//�ǲ����Լ��ĵ�
								BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;

								//�ǲ����Լ����Թ���ĵ�
								BOOL bCanManager = pPlayerShop->IsPartner(pHuman->GetGUID());

								if(bIsMine == FALSE && bCanManager == FALSE)
								{
									//log...
									return PACKET_EXE_CONTINUE;
								}

								if(nShopSerial != pPlayerShop->GetSerial())
								{
									MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_SALE_OUT_SERIAL_IS_CHANGED);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "CGItemSynchHandler::Name=%s Serials had changed"
										,pHuman->GetName()) ;
									return PACKET_EXE_CONTINUE ;
								}

								Assert(pStallContainer);

								if( pStallBox->GetSerialByIndex(nToIndex) != uSerial)
								{//Ŀ��λ�õ���Ʒ�Ѿ��ı�
									MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NEED_NEW_COPY: Serial = %d, BoxSerial = %d"
										,pHuman->GetName(), uSerial, pStallBox->GetSerialByIndex(nToIndex)) ;
									return PACKET_EXE_CONTINUE;
								}

								//ItemContainer* pMatContainer	= pHuman->GetMatContain();
								ItemContainer* pBaseContainer	= pHuman->GetBaseContain();
								//ItemContainer* pBagContainer	= NULL;

								//if(pMatContainer->GetIndexByGUID(&ItemGuid) >= 0)
								//{
								//	pBagContainer = pMatContainer;
								//}
								//else if(pBaseContainer->GetIndexByGUID(&ItemGuid) >=0 )
								//{
								//	pBagContainer = pBaseContainer;
								//}
								//else
								//{//guid�Ƿ�
								////	Assert(0);
								//	MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
								//	pGamePlayer->SendPacket(&MsgError);
								//	g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL:World = %d, Server = %d, Serial = %d "
								//		,pHuman->GetName(), ItemGuid.m_World, ItemGuid.m_Server, ItemGuid.m_Serial) ;
								//	return PACKET_EXE_CONTINUE ;
								//}

								INT	IndexInBag = pBaseContainer->GetIndexByGUID(&ItemGuid);

								Item* pDestItem = pStallContainer->GetItem(nToIndex);
								if(!pDestItem->IsEmpty())
								{//��λ���Ѿ��ж����ˣ�ֻ���뱳���е���Ʒ����
									//����
									INT result = 
										g_ItemOperator.ExchangeItem
										(
										pBaseContainer,
										IndexInBag,
										pStallContainer,
										nToIndex
										);

									if(result>=0)
									{//�ƶ��ɹ�
										//������Ϣ���ͻ���
										Item* pIt = pStallContainer->GetItem(nToIndex);
										if(!pIt->IsEmpty())
										{
											//���кŵ���
											uFinalSerial = pStallBox->IncSerialByIndex(nToIndex);

											Item* pItemInBag = pBaseContainer->GetItem(IndexInBag);

											//�ϻ���AAAȡ����BBBC������AAAΪ����������BBBΪ��Ʒ��CΪ������
											RecordOpt::Excute(REC_DELITEM, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)pItemInBag->GetItemTableIndex(), (INT)pItemInBag->GetLayedNum());

											//�ϻ���AAA�ϻ���BBBC������AAAΪ����������BBBΪ��Ʒ��CΪ������
											RecordOpt::Excute(REC_ADDITEM, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)pIt->GetItemTableIndex(), (INT)pIt->GetLayedNum());
											
											//������Ϣ���ͻ���
											MsgToClient.SetOpt(GCItemSynch::OPT_EXCHANGE_ITEM);
											MsgToClient.SetFromType(GCItemSynch::POS_BAG);
											MsgToClient.SetToType(GCItemSynch::POS_PLAYERSHOP);
											MsgToClient.SetToIndex(nToIndex);
											MsgToClient.SetItemGuid(ItemGuid);

											GCExchangeItemFromBagToPlayerShop_t ExtraInfoData;
											ExtraInfoData.m_ShopGuid	= nShopID;
											ExtraInfoData.m_nStallIndex = nStallIndex;
											ExtraInfoData.m_uSerial		= uFinalSerial;
											ExtraInfoData.m_nShopSerial	= pPlayerShop->IncSerial();
											MsgToClient.SetExtraInfoLength(ExtraInfoData.GetSize());
											MsgToClient.SetExtraInfoData((BYTE*)&ExtraInfoData);
											pGamePlayer->SendPacket(&MsgToClient);
										}
									}
									else
									{//ʧ��
										Assert(0);	
										MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL, POS_BAG->POS_PLAYERSHOP MANU POS ExchangeItem"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}
								}
								else
								{//��λ��û�������ƶ�����
										INT result = 
										g_ItemOperator.MoveItem
										(
										pBaseContainer,
										IndexInBag,
										pStallContainer,
										nToIndex
										);

									if(result>=0)
									{//�ƶ��ɹ�
										//������Ϣ���ͻ���
										Item* pIt = pStallContainer->GetItem(nToIndex);
										if(!pIt->IsEmpty())
										{
											//���кŵ���
											uFinalSerial = pStallBox->IncSerialByIndex(nToIndex);

											//�ϻ���AAA�ϻ���BBBC������AAAΪ����������BBBΪ��Ʒ��CΪ������
											RecordOpt::Excute(REC_ADDITEM, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)pIt->GetItemTableIndex(), (INT)pIt->GetLayedNum());

											//������Ϣ���ͻ���
											MsgToClient.SetOpt(GCItemSynch::OPT_MOVE_ITEM);
											MsgToClient.SetFromType(GCItemSynch::POS_BAG);
											MsgToClient.SetToType(GCItemSynch::POS_PLAYERSHOP);
											MsgToClient.SetToIndex(nToIndex);
											MsgToClient.SetItemGuid(ItemGuid);

											GCMoveItemFromBagToPlayerShop_t ExtraInfoData;
											ExtraInfoData.m_ShopGuid	= nShopID;
											ExtraInfoData.m_nStallIndex = nStallIndex;
											ExtraInfoData.m_uSerial		= uFinalSerial;
											ExtraInfoData.m_nShopSerial	= pPlayerShop->IncSerial();
											MsgToClient.SetExtraInfoLength(ExtraInfoData.GetSize());
											MsgToClient.SetExtraInfoData((BYTE*)&ExtraInfoData);
											pGamePlayer->SendPacket(&MsgToClient);
										}
									}
									else
									{//ʧ��
										Assert(0);
										MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL, POS_BAG->POS_PLAYERSHOP MANU POS MoveItem"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}
								}
							}
							break;
						default:
							break;
						}
					}
					break;
				case CGItemSynch::POS_PET:
					{
						switch(ToType)
						{
						case CGItemSynch::POS_PLAYERSHOP:
							{//����->�̵�

							}
							break;
						default:
							break;
						}
					}
					break;
				case CGItemSynch::POS_PLAYERSHOP:
					{
						switch(ToType)
						{
						case CGItemSynch::POS_BAG:
							{//�̵�->����
								CGManuMoveItemFromPlayerShopToBag_t ExtraData;
								Assert(ExtraInfoL == ExtraData.GetSize());
								memcpy(&ExtraData, pExtraInfo, ExtraInfoL);
								BYTE				nToIndex	= ToIndex;
								_PLAYERSHOP_GUID	nShopID		= ExtraData.m_ShopGuid;
								BYTE				nStallIndex	= ExtraData.m_nStallIndex;
								UINT				uSerial		= ExtraData.m_uSerial;//�̵����λ�õ����к�
								BYTE				nShopSerial	= ExtraData.m_nShopSerial;

								PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
								PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
								Assert(pPlayerShop);
								PlayerStallBox* pStallBox		= pPlayerShop->GetPlayerStallBoxByIndex(nStallIndex);
								Assert(pStallBox);
								ItemContainer*	pStallContainer = pStallBox->GetContainer();

								//�ǲ����Լ��ĵ�
								BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;

								//�ǲ����Լ����Թ���ĵ�
								BOOL bCanManager = pPlayerShop->IsPartner(pHuman->GetGUID());

								if(bIsMine == FALSE && bCanManager == FALSE)
								{
									//log...
									return PACKET_EXE_CONTINUE;
								}

								if(nShopSerial != pPlayerShop->GetSerial())
								{
									MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_SALE_OUT_SERIAL_IS_CHANGED);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "CGItemSynchHandler::Name=%s Serials had changed"
										,pHuman->GetName()) ;
									return PACKET_EXE_CONTINUE ;
								}

								Assert(pStallContainer);

								INT	IndexInStall = pStallContainer->GetIndexByGUID(&ItemGuid);
								if(IndexInStall<0)
								{
									MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NEED_NEW_COPY: IndexInStall = %d"
										,pHuman->GetName(), IndexInStall) ;
									return PACKET_EXE_CONTINUE;
								}

								if( pStallBox->GetSerialByIndex(IndexInStall) > uSerial)
								{
									MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NEED_NEW_COPY: Serial = %d, BoxSerial = %d"
										,pHuman->GetName(), uSerial, pStallBox->GetSerialByIndex(IndexInStall)) ;
									return PACKET_EXE_CONTINUE;
								}

								Item* pItem = pStallContainer->GetItem(IndexInStall);

								ItemContainer* pBagContainer = HumanItemLogic::GetItemContain(pHuman, pItem->GetItemTableIndex());
								Item* pDestItem = pBagContainer->GetItem(nToIndex);
								
								if(!pDestItem->IsEmpty())
								{//��λ���Ѿ��ж����ˣ�ֻ���뱳���е���Ʒ����
									//����
									INT result = 
										g_ItemOperator.ExchangeItem
										(
										pStallContainer,
										IndexInStall,
										pBagContainer,
										nToIndex
										);

									if(result>=0)
									{//�ƶ��ɹ�
										//������Ϣ���ͻ���
										Item* pIt = pBagContainer->GetItem(nToIndex);
										if(!pIt->IsEmpty())
										{
											//���кŵ���
											uFinalSerial = pStallBox->IncSerialByIndex(IndexInStall);
											uFinalToIndex	= pBagContainer->ConIndex2BagIndex(nToIndex);

											Item* pItemInStall = pStallContainer->GetItem(IndexInStall);

											//�ϻ���AAAȡ����BBBC������AAAΪ����������BBBΪ��Ʒ��CΪ������
											RecordOpt::Excute(REC_DELITEM, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)pIt->GetItemTableIndex(), (INT)pIt->GetLayedNum());

											//�ϻ���AAA�ϻ���BBBC������AAAΪ����������BBBΪ��Ʒ��CΪ������
											RecordOpt::Excute(REC_ADDITEM, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)pItemInStall->GetItemTableIndex(), (INT)pItemInStall->GetLayedNum());

											//������Ϣ���ͻ���
											MsgToClient.SetOpt(GCItemSynch::OPT_EXCHANGE_ITEM);
											MsgToClient.SetFromType(GCItemSynch::POS_PLAYERSHOP);
											MsgToClient.SetToType(GCItemSynch::POS_BAG);
											MsgToClient.SetToIndex(uFinalToIndex);
											MsgToClient.SetItemGuid(ItemGuid);

											GCExchangeItemFromPlayerShopToBag_t ExtraInfoData;
											ExtraInfoData.m_ShopGuid		= nShopID;
											ExtraInfoData.m_nStallIndex		= nStallIndex;
											ExtraInfoData.m_uSerial			= uFinalSerial;
											ExtraInfoData.m_nShopSerial		= pPlayerShop->IncSerial();
											MsgToClient.SetExtraInfoLength(ExtraInfoData.GetSize());
											MsgToClient.SetExtraInfoData((BYTE*)&ExtraInfoData);
											pGamePlayer->SendPacket(&MsgToClient);
										}
									}
									else
									{//ʧ��
										Assert(0);
										MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL, POS_PLAYERSHOP->POS_BAG MANU POS ExchangeItem"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;										
									}
								}
								else
								{//��λ��û�������ƶ�����
									//������̯λ��
									//�Զ��Ҹ�
									INT result = 
										g_ItemOperator.MoveItem
										(
										pStallContainer,
										IndexInStall,
										pBagContainer,
										nToIndex
										);

									if(result>=0)
									{//�ƶ��ɹ�
										//������Ϣ���ͻ���
										Item* pIt = pBagContainer->GetItem(nToIndex);
										if(!pIt->IsEmpty())
										{
											//���кŵ���
											uFinalSerial = pStallBox->IncSerialByIndex(IndexInStall);
											uFinalToIndex = pBagContainer->ConIndex2BagIndex(nToIndex);

											//�ϻ���AAAȡ����BBBC������AAAΪ����������BBBΪ��Ʒ��CΪ������
											RecordOpt::Excute(REC_DELITEM, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)pIt->GetItemTableIndex(), (INT)pIt->GetLayedNum());

											//������Ϣ���ͻ���
											MsgToClient.SetOpt(GCItemSynch::OPT_MOVE_ITEM );
											MsgToClient.SetFromType(GCItemSynch::POS_PLAYERSHOP);
											MsgToClient.SetToType(GCItemSynch::POS_BAG);
											MsgToClient.SetToIndex(uFinalToIndex);
											MsgToClient.SetItemGuid(ItemGuid);

											GCMoveItemFromPlayerShopToBag_t ExtraInfoData;
											ExtraInfoData.m_ShopGuid	= nShopID;
											ExtraInfoData.m_nStallIndex = nStallIndex;
											ExtraInfoData.m_Flag		= 1;
											ExtraInfoData.m_uSerial		= uFinalSerial;
											ExtraInfoData.m_nShopSerial	= pPlayerShop->IncSerial();
											MsgToClient.SetExtraInfoLength(ExtraInfoData.GetSize());
											MsgToClient.SetExtraInfoData((BYTE*)&ExtraInfoData);
											pGamePlayer->SendPacket(&MsgToClient);
										}
									}
									else
									{//ʧ��
										Assert(0);
										MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL, POS_PLAYERSHOP->POS_BAG MANU POS MoveItem"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}
								}
							}
							break;
						case CGItemSynch::POS_PLAYERSHOP:
							{//�̵�->�̵�

								CGManuMoveItemFromPlayerShopToPlayerShop_t ExtraData;
								Assert(ExtraInfoL == ExtraData.GetSize());
								memcpy(&ExtraData, pExtraInfo, ExtraInfoL);
								BYTE nToIndex				= ToIndex;
								_PLAYERSHOP_GUID	nShopID = ExtraData.m_ShopGuid;
								BYTE nStallIndex			= ExtraData.m_nStallIndex;
								UINT uSerialSource			= ExtraData.m_uSerialSource;//�̵���Դλ�õ����к�
								UINT uSerialDest			= ExtraData.m_uSerialDest;//�̵���Ŀ��λ�õ����к�
								BYTE nShopSerial			= ExtraData.m_nShopSerial;
							
								PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
								PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
								PlayerStallBox* pStallBox		= pPlayerShop->GetPlayerStallBoxByIndex(nStallIndex);
								ItemContainer*	pStallContainer = pStallBox->GetContainer();

								Assert(pStallContainer);

								INT	IndexInStallSource	= pStallContainer->GetIndexByGUID(&ItemGuid);
								INT	IndexInStallDest	= nToIndex;

								if(nShopSerial != pPlayerShop->GetSerial())
								{
									MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_SALE_OUT_SERIAL_IS_CHANGED);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "CGItemSynchHandler::Name=%s Serials had changed"
										,pHuman->GetName()) ;
									return PACKET_EXE_CONTINUE ;
								}

								if( IndexInStallSource<0 )
								{
									MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NEED_NEW_COPY: IndexInStallSource = %d, IndexInStallDest = %d"
										,pHuman->GetName(), IndexInStallSource, IndexInStallDest) ;
									return PACKET_EXE_CONTINUE;
								}

								if( pStallBox->GetSerialByIndex(IndexInStallSource) > uSerialSource || pStallBox->GetSerialByIndex(IndexInStallDest) > uSerialDest)
								{
									MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NEED_NEW_COPY: SerialSource = %d, BoxSerialDest = %d, SerialDest = %d, BoxSerialDeset = %d"
										,pHuman->GetName(), uSerialSource, pStallBox->GetSerialByIndex(IndexInStallSource), uSerialDest, pStallBox->GetSerialByIndex(IndexInStallDest)) ;
									return PACKET_EXE_CONTINUE;
								}

								Item* pItemSource	= pStallContainer->GetItem(IndexInStallSource);
								Item* pItemDest		= pStallContainer->GetItem(IndexInStallDest);

								if(!pItemDest->IsEmpty())
								{//��λ���Ѿ��ж����ˣ�ֻ�����λ���е���Ʒ����
									//����
									INT result = 
										g_ItemOperator.ExchangeItem
										(
										pStallContainer,
										IndexInStallSource,
										pStallContainer,
										IndexInStallDest
										);

									if(result>=0)
									{//�ƶ��ɹ�
										//������Ϣ���ͻ���
										Item* pIt = pStallContainer->GetItem(IndexInStallDest);
										if(!pIt->IsEmpty())
										{
											//���кŵ���
											UINT uFinalSerialSource	= pStallBox->IncSerialByIndex(IndexInStallSource);
											UINT uFinalSerialDest	= pStallBox->IncSerialByIndex(IndexInStallDest);
											
											//������Ϣ���ͻ���
											MsgToClient.SetOpt(GCItemSynch::OPT_EXCHANGE_ITEM);
											MsgToClient.SetFromType(GCItemSynch::POS_PLAYERSHOP);
											MsgToClient.SetToType(GCItemSynch::POS_PLAYERSHOP);
											MsgToClient.SetToIndex(IndexInStallDest);
											MsgToClient.SetItemGuid(ItemGuid);

											GCExchangeItemFromPlayerShopToPlayerShop_t ExtraInfoData;
											ExtraInfoData.m_ShopGuid		= nShopID;
											ExtraInfoData.m_nStallIndex		= nStallIndex;
											ExtraInfoData.m_uSerialSource	= uFinalSerialSource;
											ExtraInfoData.m_uSerialDest		= uFinalSerialDest;
											ExtraInfoData.m_nShopSerial		= pPlayerShop->IncSerial();
											MsgToClient.SetExtraInfoLength(ExtraInfoData.GetSize());
											MsgToClient.SetExtraInfoData((BYTE*)&ExtraInfoData);
											pGamePlayer->SendPacket(&MsgToClient);
										}
									}
									else
									{//ʧ��
										Assert(0);
										MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL, POS_PLAYERSHOP->POS_PLAYERSHOP, MANU POS ExchangeItem"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}
								}
								else
								{//��λ��û�������ƶ�����
									//������̯λ��
									//�Զ��Ҹ�
									INT result = 
										g_ItemOperator.MoveItem
										(
										pStallContainer,
										IndexInStallSource,
										pStallContainer,
										IndexInStallDest
										);

									if(result>=0)
									{//�ƶ��ɹ�
										//������Ϣ���ͻ���
										Item* pIt = pStallContainer->GetItem(IndexInStallDest);
										if(!pIt->IsEmpty())
										{
											//���кŵ���
											UINT uFinalSerialSource	= pStallBox->IncSerialByIndex(IndexInStallSource);
											UINT uFinalSerialDest	= pStallBox->IncSerialByIndex(IndexInStallDest);
											
											//������Ϣ���ͻ���
											MsgToClient.SetOpt(GCItemSynch::OPT_MOVE_ITEM );
											MsgToClient.SetFromType(GCItemSynch::POS_PLAYERSHOP);
											MsgToClient.SetToType(GCItemSynch::POS_PLAYERSHOP);
											MsgToClient.SetToIndex(IndexInStallDest);
											MsgToClient.SetItemGuid(ItemGuid);

											GCMoveItemFromPlayerShopToPlayerShop_t ExtraInfoData;
											ExtraInfoData.m_ShopGuid		= nShopID;
											ExtraInfoData.m_nStallIndex		= nStallIndex;
											ExtraInfoData.m_uSerialSource	= uFinalSerialSource;
											ExtraInfoData.m_uSerialDest		= uFinalSerialDest;
											ExtraInfoData.m_nShopSerial		= pPlayerShop->IncSerial();
											MsgToClient.SetExtraInfoLength(ExtraInfoData.GetSize());
											MsgToClient.SetExtraInfoData((BYTE*)&ExtraInfoData);
											pGamePlayer->SendPacket(&MsgToClient);
										}
									}
									else
									{//ʧ��
										Assert(0);
										MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL, POS_PLAYERSHOP->POS_PLAYERSHOP MANU POS MoveItem"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}
								}
							}
							break;
						default:
							break;
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
	}
	g_pLog->FastSaveLog( LOG_FILE_1, "CGItemSynchHandler::ObjName=%s"
		,pHuman->GetName()) ;
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
