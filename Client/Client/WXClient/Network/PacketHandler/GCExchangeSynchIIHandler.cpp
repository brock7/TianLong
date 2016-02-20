/*
�ͻ����յ��Է��ͻ��˽��׺�����Ʒ,��Ǯ�ı仯
*/
#include "stdafx.h"
#include "GCExchangeSynchII.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Object\Item\Obj_Item_Equip.h"
#include "TDException.h"

using namespace EXCHANGE_MSG;
UINT GCExchangeSynchIIHandler::Execute( GCExchangeSynchII* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		BYTE	IsMyself	=	pPacket->GetIsMyself();		
		BYTE	Opt			=	pPacket->GetOpt();	
		BYTE	FromType	=	pPacket->GetFromType();
		BYTE	ToType		=	pPacket->GetToType();
		BYTE	FromIndex	=	pPacket->GetFromIndex();
		BYTE	ToIndex		=	pPacket->GetToIndex();
		UINT	Money		=	pPacket->GetMoney();
		PET_GUID_t	PetGuid = 	pPacket->GetPetGuid(); 

		if(IsMyself)
		{//�޸��Լ��Ľ��׺�
			switch(Opt)
			{
				case OPT_ADDITEM:
					{
						switch(FromType)
						{
						case POS_BAG:
							{
								tObject_Item* pBagItem = CDataPool::GetMe()->UserBag_GetItem(FromIndex);
								// ����FromIndex�е���Ʒ
								pBagItem->SetLock(TRUE);

								//��¡��Ʒ
								tObject_Item* pNewItem  = CObject_Item::NewItem( pBagItem->GetIdTable());
								((CObject_Item*)pNewItem)->Clone((CObject_Item*)pBagItem);

								CDataPool::GetMe()->MyExBox_SetItem(ToIndex, pNewItem);
		
							}
							break;
						case POS_EQUIP:
							{
							}
							break;
						case POS_PET:
							{
							}
							break;
						default:
							break;
						}
					}
					break;
				case OPT_REMOVEITEM:
					{
						switch(ToType)
						{
						case POS_BAG:
							{
								//��Ҫ���������Ӧ��Ʒ������(ע�⣬��¡����ƷGUID��ͬ�����Կ�����������)
								if(CDataPool::GetMe()->MyExBox_GetItem(FromIndex))
								{
									__int64 ItemGUID = CDataPool::GetMe()->MyExBox_GetItem(FromIndex)->GetGUID();
									tObject_Item* pBagItem = CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->UserBag_GetItemIndexByGUID(ItemGUID));
									if(pBagItem)
									{
										//ȥ������
										pBagItem->SetLock(FALSE);
									}
								}

								CDataPool::GetMe()->MyExBox_SetItem(FromIndex, NULL);
							}
							break;
						case POS_EQUIP:
							{
							}
							break;
						case POS_PET:
							{
							}
							break;
						default:
							break;
						}
					}
					break;
				case OPT_MONEY:
					{//��Ǯ�ı䲻�����Լ��ˣ�ʡ����Ϣ
					}
					break;
				case OPT_REMOVEMONEY:
					{//��Ǯ�ı䲻�����Լ��ˣ�ʡ����Ϣ
					}
					break;
				case OPT_ADDPET:
					{//������ﵽ���׺���
						INT IndexInBag = CDataPool::GetMe()->Pet_GetPetByGuid(PetGuid);
						//��ס���λ�õĳ���
						SDATA_PET* pPetData = CDataPool::GetMe()->Pet_GetPet(IndexInBag);
						CDataPool::GetMe()->MyExBox_SetPet(ToIndex, pPetData, FALSE);

						//��Ҫ֪ͨ��������������Ѿ�����
						// ��������
					}
					break;
				case OPT_REMOVEPET:
					{//�ӽ��׺����ó�����
						INT IndexInExBox = CDataPool::GetMe()->MyExBox_GetPetByGuid(PetGuid);
						CDataPool::GetMe()->MyExBox_SetPet(IndexInExBox, NULL, FALSE);
						INT IndexInBag = CDataPool::GetMe()->Pet_GetPetByGuid(PetGuid);
						//�������λ�õĳ�
						//......
					}
					break;
				default:
					break;
			}
			//����
			CActionSystem::GetMe()->UserExchangeSelf_Update();
		}
		else
		{//�޸ĶԷ��Ľ��׺�
			switch(Opt)
			{
			case OPT_ADDITEM:
				{
					EXCHANGE_MSG::_EXCHANGE_ITEM* pNewItem = pPacket->GetItem();
					tObject_Item* pItem = NULL;

					if(pNewItem->isBlueEquip)
					{
						pItem = CObject_Item::NewItem( pNewItem->item_data.m_ItemIndex);
						pItem->SetGUID(
							pNewItem->item_data.m_ItemGUID.m_World, 
							pNewItem->item_data.m_ItemGUID.m_Server, 
							pNewItem->item_data.m_ItemGUID.m_Serial);
						TDAssert(pItem);
						CDataPool::GetMe()->OtExBox_SetItem(ToIndex, pItem);
						CDataPool::GetMe()->OtExBox_SetItemExtraInfo(ToIndex, FALSE, &pNewItem->item_data);
					}
					else
					{
						pItem = CObject_Item::NewItem( pNewItem->item_guid);
						TDAssert(pItem);
						CDataPool::GetMe()->OtExBox_SetItem(ToIndex, pItem);
					}
				}
				break;
			case OPT_REMOVEITEM:
				{
					CDataPool::GetMe()->OtExBox_SetItem(FromIndex, NULL);
				}
				break;
			case OPT_MONEY:
				{
					CDataPool::GetMe()->OtExBox_SetMoney(Money);
				}
				break;
			case OPT_REMOVEMONEY:
				{//����
				}
				break;
			case OPT_ADDPET:
				{

				}
				break;
			case OPT_REMOVEPET:
				{
					INT IndexInExBox = CDataPool::GetMe()->OtExBox_GetPetByGuid(PetGuid);
					SDATA_PET* pPetData = CDataPool::GetMe()->OtExBox_GetPet(IndexInExBox);
					SAFE_DELETE(pPetData);
					CDataPool::GetMe()->OtExBox_SetPet(IndexInExBox, NULL, FALSE);
				}
				break;
			default:
				break;
			}
			//����
			CActionSystem::GetMe()->UserExchangeOther_Update();
		}
	}

	//ˢ�½��׺���
	CEventSystem::GetMe()->PushEvent(GE_UPDATE_EXCHANGE);
	//ˢ�±�������
	CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
	//���³����б����

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
