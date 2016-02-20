#include "StdAfx.h"
#include "GMActionSystem_Item.h"
#include "..\Object\Item\Obj_Item.h"
#include "..\Object\Item\Obj_Item_Equip.h"
#include "..\DataPool\GMDataPool.h"
#include "..\DBC\GMDataBase.h"
#include "..\Procedure\GameProcedure.h"
#include "..\Interface\GMGameInterface.h"
#include "..\NetWork\NetManager.h"
#include "CGBankAddItem.h"
#include "CGBankRemoveItem.h"
#include "CGBankSwapItem.h"
#include "CGPlayerShopApply.h"
#include "ExchangeMsgDefine.h"
#include "CGExchangeSynchItemII.h"
#include "TDUISystem.h"
#include "..\Event\GMEventSystem.h"
#include "CGUnEquip.h"									// ʹ��ж��װ����Ϣ
#include "CGPackage_SwapItem.h"
#include "..\object\item\Obj_Item_Equip.h"
#include "..\object\item\Obj_Item_Gem.h"
#include "..\object\item\Obj_Item_Medicine.h"

#include "..\Object\ObjectManager.h"
#include "..\Object\Object.h"
#include "..\Object\Character\Obj_PlayerMySelf.h"
#include "..\DataPool\GMDP_CharacterData.h"

#include "CGStallRemoveItem.h"

#include "CGDiscardEquip.h"
#include "CGDiscardItem.h"

#include "..\Sound\GMSoundSystem.h"

#include "..\DBC\GMDataBase.h"
#include "GameStruct_Skill.h"

#include "..\Input\GMInputSystem.h"
#include "CGItemSynch.h"
#include "CGAskItemInfo.h"
#include "CGUseItem.h"
#include "CGShopBuy.h"


#include "GMActionSystem_MouseCmd.h"

CActionItem_Item::CActionItem_Item(INT nID)
	:CActionItem(nID)
{
}

CActionItem_Item::~CActionItem_Item()
{
}

VOID CActionItem_Item::Update_Item(tObject_Item* pItem)
{
	TDAssert(pItem);

	//����
	m_idItemImpl = pItem->GetID();
	//����
	m_strName = pItem->GetName();
	//ͼ��
	if ( pItem->GetIconName() != NULL )
		m_strIconName = pItem->GetIconName();
	//֪ͨUI
	UpdateToRefrence();
}

tObject_Item* CActionItem_Item::GetItemImpl(VOID)
{
	return CObject_Item::FindItem(m_idItemImpl);
}

LPCTSTR CActionItem_Item::GetType_String(VOID)
{
	if(!GetItemImpl()) return "";

	switch( GetItemImpl()->GetTypeOwner() )
	{
	case tObject_Item::IO_ITEMBOX:
		return NAMETYPE_LOOTITEM;
	case tObject_Item::IO_BOOTH:
		return NAMETYPE_BOOTITEM;
	case tObject_Item::IO_BOOTH_CALLBACK:
		return NAMETYPE_CALLBACK;
	case tObject_Item::IO_MYSELF_BANK:
		return NAMETYPE_BANKITEM;
	case tObject_Item::IO_MYSELF_EQUIP:
		return NAMETYPE_EQUIP;
	case tObject_Item::IO_MYEXBOX:
		return NAMETYPE_EXCHANGESELF;
	case tObject_Item::IO_OTHEREXBOX:
		return NAMETYPE_EXCHANGEOTHER;
	case tObject_Item::IO_MISSIONBOX:
		return NAMETYPE_MISSIONREFER;
	case tObject_Item::IO_MYSTALLBOX:
		return NAMETYPE_STALLSELF;
	case tObject_Item::IO_OTSTALLBOX:
		return NAMETYPE_STALLOTHER;
	case tObject_Item::IO_QUESTVIRTUALITEM:
		return NAMETYPE_VIRTUALITEM;
	case tObject_Item::IO_PLAYEROTHER_EQUIP:
		return NAMETYPE_OTHEREQUIP;
	case tObject_Item::IO_PS_OTHERBOX:
		return NAMETYPE_PS_OTHER;
	case tObject_Item::IO_GEM_SEPARATE:
		return NAMETYPE_SPLITGEM;
	case tObject_Item::IO_PS_SELFBOX:
		return NAMETYPE_PS_SELF;
	case tObject_Item::IO_CITY_RESEARCH:
		return NAMETYPE_CITY_RESEARCH;
	case tObject_Item::IO_CITY_SHOP:
		return NAMETYPE_CITY_SHOP;
	case tObject_Item::IO_TRANSFER_ITEM:
		return NAMETYPE_TRANSFER_ITEM;
	case tObject_Item::IO_QUESTUI_DEMAND:
		return NAMETYPE_QUESTUI_DEMAND;
	case tObject_Item::IO_QUESTUI_REWARD:
		return NAMETYPE_QUESTUI_REWARD;
	default:
		return NAMETYPE_PACKAGEITEM;
	}
}

INT	CActionItem_Item::GetDefineID(VOID)
{
	tObject_Item* pItem = GetItemImpl();
	if(!pItem) return -1;

	return pItem->GetIdTable();
}

INT CActionItem_Item::GetNum(VOID)
{
	tObject_Item* pItem = GetItemImpl();

	if(pItem) 
		return pItem->GetNumber();
	else
	{
		//AxTrace(0, 1, "InvalidItem:%d", m_idItemImpl);
		return -1;
	}
}

LPCTSTR CActionItem_Item::GetDesc(VOID)
{
	tObject_Item* pItem = GetItemImpl();
	if(!pItem) return NULL;

	LPCTSTR szExtraInfo = pItem->GetExtraDesc();
	return szExtraInfo;//

}

INT CActionItem_Item::GetPosIndex(VOID)
{
	tObject_Item* pItem = GetItemImpl();
	if(!pItem) return -1;

	return pItem->GetPosIndex();
}

VOID CActionItem_Item::DoSubAction(VOID)
{
	CObject_Item* pItem = (CObject_Item*)GetItemImpl();
	if(!pItem) return;

	//���������Ʒ
	if(	pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)
	{
		//��סShift
		if( CInputSystem::GetMe()->IsKeyDown(KC_LSHIFT) || CInputSystem::GetMe()->IsKeyDown(KC_RSHIFT) )
		{
			//�ж������Ʒ�Ƿ�߱���ֵ�����,��������һ�����Բ��
			if(pItem->GetNumber() > 1)
			{
				//�Ƚ����ܴ��ڵ��Ѿ���������Ʒ�������
				INT nCon = CDataPool::GetMe()->Split_GetItemConta();
				INT nPos = CDataPool::GetMe()->Split_GetItemPos();
				switch( nCon ) 
				{
				case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
					{
						tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nPos);

						if(pItem)
						{
							pItem->SetLock(FALSE);
						}
					}
					break;
				case tObject_Item::IO_MYSELF_BANK:					// ��ҵ�����
					{
						tObject_Item* pItem = CDataPool::GetMe()->UserBank_GetItem(nPos);

						if(pItem)
						{
							pItem->SetLock(FALSE);
						}
					}
					break;
				default:
					break;
				}

				//��¼�����Ʒ��λ�õ����ݳ�
				CDataPool::GetMe()->Split_SetItemConta(pItem->GetTypeOwner());
				CDataPool::GetMe()->Split_SetItemPos(this->GetPosIndex());
				CDataPool::GetMe()->Split_SetSum(pItem->GetNumber());

				//�򿪲�ֶԻ���
				CEventSystem::GetMe()->PushEvent(GE_SHOW_SPLIT_ITEM);		

				// ���������Ʒ
				pItem->SetLock(TRUE);

				//֪ͨ��������״̬
				CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
			}
			else
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����Ʒ���ɲ��");
			}
			return;
		}

		//��Ʒ�����ǿ���
		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Shop"))
		{
			//������״̬
			if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
			{
				//�����ǲ���װ��,ֻ��װ����������
				if(pItem->GetItemClass() == ICLASS_EQUIP)
				{
					if( pItem->GetItemDur() < pItem->GetItemMaxDur() )
					{
						CGameProcedure::s_pGameInterface->Booth_Repair(0, pItem->GetPosIndex());
					}
					else
					{
						//����ʧ����Ч
						CSoundSystemFMod::_PlayUISoundFunc(47);
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����Ʒ����Ҫ����");
					}
				}
				else
				{
					//����ʧ����Ч
					CSoundSystemFMod::_PlayUISoundFunc(47);
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����Ʒ����Ҫ����");
				}

				return;
			}
		}

		
		switch(pItem->GetItemClass())
		{
		case ICLASS_EQUIP:
			{
				if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())
				{
					// ʹ�ü�������2006-3-30
					UseIdentify();
					return; 
				}
				break;
			}
		case ICLASS_COMITEM:
			{
				int iType		= pItem->GetItemTableType();
				int iQuanlity	= pItem->GetItemTableQuality();

				if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "��Ʒ����Ҫ����");
					return; 
				}

				if(5 == iQuanlity)
				{
					if((iType >= COMITEM_WPIDENT)&&(iType <= COMITEM_NCIDENT))				
					{
						PrepareUseIdentify();
					}
					else
					{
						if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())
						{
							// ʹ�ü�������2006-3-30
							CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "ֻ�ܼ���������װ��");
							return; 
						}
					}
				}
				break;
			}
		default:
			{
				if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())
				{
					// ʹ�ü�������2006-3-30
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "ֻ�ܼ���������װ��");
					return; 
				}
				break;
			}
		
		}
	}
	
}

VOID CActionItem_Item::DoAction(VOID)
{
	CObject_Item* pItem = (CObject_Item*)GetItemImpl();
	if(!pItem) return;

	//�ȼ�⡰Shift���ǲ��ǰ���״̬������ǣ���ô�ȴ����ֵĲ���
	if( CInputSystem::GetMe()->IsKeyDown(KC_LSHIFT) || CInputSystem::GetMe()->IsKeyDown(KC_RSHIFT) )
	{
		//�޸ģ�Ŀǰ��������DoActionʵ�ֲ�֣�����ʹ��DoSubAction���в��
		if( pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_BANK )
		{
			//�ж������Ʒ�Ƿ�߱���ֵ�����,��������һ�����Բ��
			if(pItem->GetNumber() > 1)
			{
				//�Ƚ����ܴ��ڵ��Ѿ���������Ʒ�������
				INT nCon = CDataPool::GetMe()->Split_GetItemConta();
				INT nPos = CDataPool::GetMe()->Split_GetItemPos();
				switch( nCon ) 
				{
				case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
					{
						tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nPos);

						if(pItem)
						{
							pItem->SetLock(FALSE);
						}
					}
					break;
				case tObject_Item::IO_MYSELF_BANK:					// ��ҵ�����
					{
						tObject_Item* pItem = CDataPool::GetMe()->UserBank_GetItem(nPos);

						if(pItem)
						{
							pItem->SetLock(FALSE);
						}
					}
					break;
				default:
					break;
				}

				//��¼�����Ʒ��λ�õ����ݳ�
				CDataPool::GetMe()->Split_SetItemConta(pItem->GetTypeOwner());
				CDataPool::GetMe()->Split_SetItemPos(this->GetPosIndex());
				CDataPool::GetMe()->Split_SetSum(pItem->GetNumber());

				//�򿪲�ֶԻ���
				CEventSystem::GetMe()->PushEvent(GE_SHOW_SPLIT_ITEM);

				// ���������Ʒ
				pItem->SetLock(TRUE);

				//֪ͨ��������״̬
				CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANK);
			}
			else
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����Ʒ���ɲ��");
			}
		}
		return;
	}

	STRING strTemp = "";

	//������Ʒ���ڵĹ���
	switch(pItem->GetTypeOwner())
	{
		//��ҵİ�̯���棨����
	case tObject_Item::IO_MYSTALLBOX:			//�Լ���̯λ��
		{
			//ֱ��֪ͨ���棬�Լ�ѡ�еı��
			CEventSystem::GetMe()->PushEvent(GE_STALL_SALE_SELECT,this->GetPosIndex());
		}
		break;
		//��ҵİ�̯���棨��
	case tObject_Item::IO_OTSTALLBOX:			//�Է���̯λ��
		{
			//ֱ��֪ͨ���棬�Լ�ѡ�еı��
			CEventSystem::GetMe()->PushEvent(GE_STALL_BUY_SELECT,this->GetPosIndex());
		}
		break;
		//��ҽ�����ĶԻ�����
	case tObject_Item::IO_MISSIONBOX:
		{
			//�������
			if(pItem)
			{
				INT nBagIndex = CDataPool::GetMe()->UserBag_GetItemIndexByGUID(pItem->GetGUID());

				tObject_Item* pBagItem = CDataPool::GetMe()->UserBag_GetItem(nBagIndex);
				pBagItem->SetLock(FALSE);
			}

			//������BOX�е�Item���һ�ξ�������ʧ
			CDataPool::GetMe()->MissionBox_SetItem(this->GetPosIndex(), NULL);

			CActionSystem::GetMe()->UserMission_Update();
			//���͸�����Ϣ
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_REPLY_MISSION);
			//���±���
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}
		break;
		//��ҵ���������������
	case tObject_Item::IO_MYSELF_BANK:
		{
			CGBankRemoveItem msg;
			msg.SetToType(CGBankRemoveItem::BAG_POS);
			msg.SetIndexFrom(this->GetPosIndex());
			msg.SetIndexTo(255);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		break;
		//�������е���Ʒ
	case tObject_Item::IO_ITEMBOX:
		{
			//����
			if(pItem->GetItemClass() == ICLASS_TASKITEM)
			{
				INT idTable = pItem->GetIdTable();
				INT nCount = CDataPool::GetMe()->UserBag_CountItemByIDTable(idTable);

				DBC_DEFINEHANDLE(s_pItem_Medic, DBC_ITEM_MEDIC);
				//������¼
				const _DBC_ITEM_MEDIC* pMedic = (const _DBC_ITEM_MEDIC*)s_pItem_Medic->Search_Index_EQU(idTable);
				if( pMedic->nMaxHold != INVALID_ID && nCount >= pMedic->nMaxHold )
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����ʰȡ������Ʒ��");				
					break;
				}
			}
			CGameProcedure::s_pGameInterface->ItemBox_PickItem(pItem);
			break;
		}
		break;
		//����Լ����ϵİ���
	case tObject_Item::IO_MYSELF_PACKET:
		{
			DoAction_Packet();
		}
		break;

	case tObject_Item::IO_BOOTH:
		//���˵Ļ���,���ͽ�������
		{
			//	�������ָ��󣬲����ٹ�����Ʒ
			if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����������Ʒ");
				break;
			}

			if( CDataPool::GetMe()->Booth_GetShopType() == 0 )//�������
			{
				if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money() >= pItem->GetItemPrice())
				{
					CGameProcedure::s_pGameInterface->Booth_BuyItem( pItem );
				}
				else
				{
					if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money() >= pItem->GetItemPrice())
					{
						CGameProcedure::s_pGameInterface->Booth_BuyItem( pItem );
					}
					else
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ǯ����");
					}
				}
			}
			else if( CDataPool::GetMe()->Booth_GetShopType() == 1 )//Ԫ��
			{
				if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_RMB() >= pItem->GetItemPrice())
				{
					CGameProcedure::s_pGameInterface->Booth_BuyItem( pItem );
				}
				else
				{
					if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_RMB() >= pItem->GetItemPrice())
					{
						CGameProcedure::s_pGameInterface->Booth_BuyItem( pItem );
					}
					else
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"Ԫ������");
					}
				}
			}
		}
		break;
	case tObject_Item::IO_BOOTH_CALLBACK:		//�ع���Ʒ 
		{
			if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����������Ʒ");
				break;
			}

			if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money() >= 
				(INT)(CDataPool::GetMe()->Booth_GetSoldPrice(this->GetPosIndex())))
			{
				_ITEM_GUID Guid;
				tObject_Item::ITEM_GUID temp;
				temp.m_idUnion = 0;

				pItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
				Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
				Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
				Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

				CGShopBuy msg;
				msg.SetUniqueID(CDataPool::GetMe()->Booth_GetShopUniqueId());
				msg.SetIndex(pItem->GetPosIndex()+200);
				msg.SetItemGUID(Guid);
				msg.SetSerialNum(CDataPool::GetMe()->Booth_GetSerialNum());

				CNetManager::GetMe()->SendPacket( &msg );
			}
			else
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ǯ����");
			}
		}
		break;
	case tObject_Item::IO_MYEXBOX:
		// ���׽�����Լ�����Ʒ����ͷ
		{
			//�����϶���Ʒ����Ϣ
			CGExchangeSynchItemII msg;
			msg.SetOpt(EXCHANGE_MSG::OPT_REMOVEITEM);
			//msg.SetFromType(EXCHANGE_MSG::);
			msg.SetFromIndex(this->GetPosIndex());
			msg.SetToType(EXCHANGE_MSG::POS_BAG);
			msg.SetToIndex(0);						//�ɷ���������
			CNetManager::GetMe()->SendPacket(&msg);
		}
		break;

	case tObject_Item::IO_MYSELF_EQUIP :
		// ���action item��װ������.
		{
			if(!CDataPool::GetMe()->Booth_IsClose())
			{
				//������״̬
				if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
				{
					//�������ǲ���װ��,ֻ��װ����������
					if(pItem->GetItemClass() == ICLASS_EQUIP)
					{
						if( pItem->GetItemDur() < pItem->GetItemMaxDur() )
						{
							CGameProcedure::s_pGameInterface->Booth_Repair(0, pItem->GetPosIndex(), FALSE);
						}
						else
						{
							CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����Ʒ����Ҫ����");
						}
					}
				}
			}
			else
			{
				CGUnEquip msg;
				CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)pItem;
				if(pItemEquip->GetItemClass() != ICLASS_EQUIP)
				{
					return;
				}

				// ����ж��װ����Ϣ.
				msg.setEquipPoint((BYTE)(pItemEquip->GetItemType()));
				msg.setBagIndex(-1);

				CNetManager::GetMe()->SendPacket( &msg );
			}
		}
		break;
	case tObject_Item::IO_PS_SELFBOX:
		{
			//ѡ�е���Ʒ��Ҫ�ڽ��л�ҳ���������
			//����̵꣨�Լ��ģ�
			// ��¼��ǰѡ�е���Ʒ
			INT nConIndex = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
			CDataPool::GetMe()->PlayerShop_SetMySelectConTa(nConIndex);
			CDataPool::GetMe()->PlayerShop_SetMySelectPos(this->GetPosIndex()%20);

			CEventSystem::GetMe()->PushEvent(GE_PS_SELF_SELECT);
		}
		break;
	case tObject_Item::IO_PS_OTHERBOX:
		{
			//����̵꣨���˵ģ�
			CDataPool::GetMe()->PlayerShop_SetOtSelectPos(this->GetPosIndex()%20);
						
			CEventSystem::GetMe()->PushEvent(GE_PS_OTHER_SELECT);
		}
		break;
	default:
		break;

	}
}

BOOL CActionItem_Item::IsValidate()
{
	tObject_Item* pItem = GetItemImpl();
	if(!pItem) return FALSE;

	if( pItem->GetTypeOwner() == tObject_Item::IO_BOOTH 
		||pItem->GetTypeOwner() == tObject_Item::IO_BOOTH_CALLBACK
		||pItem->GetTypeOwner() == tObject_Item::IO_CITY_SHOP)
	{
		return (pItem->GetMax() == 0)? FALSE : TRUE;
	}

	return TRUE;
}

VOID CActionItem_Item::Update(tActionReference* pRef)
{
	tObject_Item* pItem = GetItemImpl();
	if(!pItem) return;

	if( pItem->GetTypeOwner() == tObject_Item::IO_BOOTH 
		||pItem->GetTypeOwner() == tObject_Item::IO_BOOTH_CALLBACK
		||pItem->GetTypeOwner() == tObject_Item::IO_CITY_SHOP)
	{
		//ˢ���������
		INT nMaxNumber = pItem->GetMax();
		if(nMaxNumber > 1)
		{
			pRef->Enable();
			CHAR szTemp[32];
			_snprintf(szTemp, 32, "%d", nMaxNumber);
			pRef->SetCornerChar(tActionReference::ANP_TOPLEFT, szTemp);
		}
		else if(nMaxNumber == 0)
		{
			pRef->Disable();
			pRef->SetCornerChar(tActionReference::ANP_TOPLEFT, "");
		}
		else
		{
			pRef->Enable();
			pRef->SetCornerChar(tActionReference::ANP_TOPLEFT, "");
		}
	}
}

// �϶�����
VOID CActionItem_Item::NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName)
{
	//�϶�����ʾ���ٵĵط�
	if( bDestory )
	{
		DestoryItem(szSourceName);
		return;
	}
	//�϶����հ׵ط�
	if(!szTargetName || szTargetName[0]=='\0' || GetItemImpl()==0)
	{
		return;
	}

	CHAR cSourceName = szSourceName[0];
	CHAR cTargetType = szTargetName[0];
	INT nOldTargetId = -1;
	INT nIndex =-1;
	//����ǿ�����������������ط��ϣ������Լ�����
	if(  cSourceName == 'M' && 
		 cTargetType != 'M' )
		return;

	switch(cTargetType)
	{
	case 'S':		//��̯����
		{
			INT nIndex = szTargetName[1]-'0';
			nIndex = nIndex*10 + szTargetName[2]-'0';

			//����ǲ����ɱ��������Ϲ�����(ֻ�ܽ�����ұ����ڵ���Ʒ)
			if(this->GetItemImpl()->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)
			{
				if(this->GetPosIndex() >= 40)
				{
					//������Ʒ
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "�����ϼ�������Ʒ");
					break;
				}
				// �ж���Ʒ�Ƿ��Ѿ���
				tObject_Item* pItem = this->GetItemImpl();
				if(pItem)
				{
					if(pItem->GetItemBindInfo() == 1)		//==1 �ǰ�
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����Ʒ�Ѿ���");
						break;
					}
				}

				// ���϶�����Ʒ��Ϊ��ǰ����Ʒ
				CDataPool::GetMe()->MyStallBox_SetCurSelectItem(this->GetItemImpl());
				
				// ����������ۼ۸�ĶԻ���
				CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY, "price");
			}
		}
		break;
	case 'Q':		//�����ύ����(MissionReply)
		{
			INT nIndex = szTargetName[1]-'0';

			//������Ʒ����Դ��������ʲô��Ϣ����ʱֻ֧�ֱ�����
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:			//������ҵı���
				{
					INT nGUID = this->GetItemImpl()->GetIdTable();
					//��¡��Ʒ
					tObject_Item* pNewItem  = CObject_Item::NewItem( this->GetItemImpl()->GetIdTable());
					((CObject_Item*)pNewItem)->Clone((CObject_Item*)this->GetItemImpl());

					//�����ﱣ��ֱ��֪ͨ������Ӱ�ť����������������Ϣ��
					CDataPool::GetMe()->MissionBox_SetItem(GetPosIndex(), pNewItem);

					// ���������ж�Ӧ�������Ʒ
					tObject_Item* pMissionItem = this->GetItemImpl();
					pMissionItem->SetLock(TRUE);

					CActionSystem::GetMe()->UserMission_Update();
					//���͸�����Ϣ
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_REPLY_MISSION);
					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				}
				break;
			default:
				break;
			}				
		}
		break;
	case 'B':		//����
		{
			INT nIndex = szTargetName[1]-'0';
			nIndex = nIndex*10 + szTargetName[2]-'0';

			//������Ʒ����Դ��������ʲô��Ϣ
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			//������Ʒ���ܷ�������
			if(this->GetItemImpl()->GetItemClass() == ICLASS_TASKITEM)
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "������Ʒ���ܷ���");
				return;
			}
			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:			//������ҵı���
				{
					CGBankAddItem msg;
					msg.SetFromType(CGBankAddItem::BAG_POS);
					msg.SetIndexFrom(this->GetPosIndex());
					msg.SetIndexTo(nIndex + this->GetCurBankRentBegin());
					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			case tObject_Item::IO_MYSELF_BANK:				//���������ڲ�
				{
					CGBankSwapItem msg;
					msg.SetFromType(CGBankSwapItem::BANK_POS);
					msg.SetIndexFrom(this->GetPosIndex());
					msg.SetIndexTo(nIndex + this->GetCurBankRentBegin());
					msg.SetToType(CGBankSwapItem::BANK_POS);
					if( msg.GetIndexFrom() == msg.GetIndexTo() )
						break;
					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			default:
				break;
			}

		}
		break;

	case 'P':		//����
		{
			INT nTargetIndex = atoi(szTargetName+1)-1;
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			
			switch(nTypeOwner)
			{
			//Bank -> Package
			case tObject_Item::IO_MYSELF_BANK:
				{
					CGBankRemoveItem msg;
					msg.SetToType(CGBankRemoveItem::BAG_POS);
					msg.SetIndexFrom(this->GetPosIndex());
					msg.SetIndexTo(nTargetIndex);

					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			//Package -> Package
			case tObject_Item::IO_MYSELF_PACKET:
				{
					//ͬһ��
					if(this->GetPosIndex() == nTargetIndex) break;

					//��ͬ��
					CGPackage_SwapItem msg;
					msg.SetPackageIndex1(GetPosIndex());
					msg.SetPackageIndex2(nTargetIndex);

					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			//SelfEquip -> package
			case tObject_Item::IO_MYSELF_EQUIP:
				{ 
					//����ж��װ������Ϣ
					CGUnEquip msg;
					msg.setEquipPoint(this->GetPosIndex());
					msg.setBagIndex((BYTE)nTargetIndex);

					CNetManager::GetMe()->SendPacket(&msg);
					break;
				}
			//playerShop -> package
			case tObject_Item::IO_PS_SELFBOX:
				{
					_ITEM_GUID Guid;
					tObject_Item::ITEM_GUID temp;
					temp.m_idUnion = 0;

					tObject_Item* pSelectItem = this->GetItemImpl();
					if(NULL == pSelectItem)
					{
						return;
					}
					
					//��Ҫ���ж������Ʒ�Ƿ������ƶ�������
					//��ѯ�����Ʒ�ǲ��Ǵ����ϼܵ�״̬
					INT nIndex = pSelectItem->GetPosIndex();
					INT nConIndex = nIndex/20;
					INT nPosition = nIndex%20;
					if(CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE, nConIndex, nPosition))
					{
						//�Ѿ��ϼܣ�����ȡ��
						return;
					}

					pSelectItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
					Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
					Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
					Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

					CGItemSynch msg;
					msg.SetOpt(CGItemSynch::OPT_MOVE_ITEM_MANU);
					msg.SetFromType(CGItemSynch::POS_PLAYERSHOP);
					msg.SetToIndex(nTargetIndex);
					msg.SetToType(CGItemSynch::POS_BAG);
					msg.SetItemGUID(Guid);

					INT nPage = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
					CGManuMoveItemFromPlayerShopToBag_t ExtraDataInfo;
					ExtraDataInfo.m_ShopGuid	= CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
					ExtraDataInfo.m_nStallIndex = (BYTE)nPage;
					ExtraDataInfo.m_uSerial		= CDataPool::GetMe()->PlayerShop_GetItemSerial(TRUE,nPage,this->GetPosIndex()%20);
					msg.SetExtraInfoLength(ExtraDataInfo.GetSize());
					msg.SetExtraInfoData((BYTE*)&ExtraDataInfo);
				
					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			default:
				break;
			}
		}
		break;
	case 'E':		//���׽���,ֻ���϶����Լ��ģ��ұߵģ�������ͷ��1~6��
		{
			INT nIndex = szTargetName[1]-'0';

			//������Ʒ����Դ��������ʲô��Ϣ
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch(nTypeOwner)
			{
			case tObject_Item::IO_MYSELF_PACKET:			//������ҵı���
				{
					// �ж���Ʒ�Ƿ��Ѿ���
					tObject_Item* pItem = this->GetItemImpl();
					if(pItem)
					{
						if(pItem->GetItemBindInfo() == 1)		//==1 �ǰ�
						{
							CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����Ʒ�Ѿ���");
							break;
						}
					}

					//��֤�Ƿ���Խ���
					if(!this->GetItemImpl()->Rule(tObject_Item::RULE_EXCHANGE))
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, this->GetItemImpl()->RuleFailMessage(tObject_Item::RULE_EXCHANGE).c_str());
						break;
					}

					//�����϶���Ʒ����Ϣ
					CGExchangeSynchItemII msg;
					msg.SetOpt(EXCHANGE_MSG::OPT_ADDITEM);	
					msg.SetFromType(EXCHANGE_MSG::POS_BAG);
					msg.SetFromIndex(this->GetPosIndex());
					msg.SetToIndex(0);						//�ɷ���������
					
					//��⽻�����ǲ����п�λ
					if( !CDataPool::GetMe()->MyExBox_IsEmpty() )
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�������Ѿ�����");
						break;
					}

					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			default:
				break;
			}
			
		}
		break;
	case 'R':		//���еĽ����ϵ����������
		{
			INT nIndex = szTargetName[1]-'0';

			//Ŀ����������Ѿ����ˣ��͸�����ʾ�����ڷ������Ϣ
			if(FALSE == CDataPool::GetMe()->UserBank_IsEmpty(nIndex))
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"Ŀ�������Ѿ�����");
				break;
			}

			//������Ʒ����Դ��������ʲô��Ϣ
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			//������Ʒ���ܷ�������
			if(this->GetItemImpl()->GetItemClass() == ICLASS_TASKITEM)
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "������Ʒ���ܴ�������");
				break;
			}

			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:			//������ҵı���
				{
					CGBankAddItem msg;
					msg.SetFromType(CGBankAddItem::BAG_POS);
					msg.SetIndexFrom(this->GetPosIndex());
					msg.SetIndexTo(256 - nIndex);
					
					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			case tObject_Item::IO_MYSELF_BANK:				//�������
				{
					CGBankSwapItem msg;
					msg.SetFromType(CGBankSwapItem::BANK_POS);
					msg.SetIndexFrom(this->GetPosIndex());
					msg.SetIndexTo(256 - nIndex);
					msg.SetToType(CGBankSwapItem::BANK_POS);

					//�������ͬһ���������ӣ���ô�Ͳ�����Ϣ��
					if( CActionSystem::GetMe()->GetCurBankRentBoxIndex() == nIndex )
					{
						break;
					}
					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			default:
				break;
			}

		}
		break;
	case 'G':		//��ʯ�ϳ�/��Ƕ����
		{
			INT nIndex = szTargetName[1]-'0';

			//������Ʒ����Դ��������ʲô��Ϣ
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:			//������ҵı���
				{
//						CDataPool::GetMe()->EnchasePage_SetItem(nIndex,this->GetItemImpl());
					if(GetItemImpl()->GetItemClass() == ICLASS_EQUIP)
					{
						if(((CObject_Item_Equip*)GetItemImpl())->GetEquipQuantity() == CObject_Item_Equip::BLUE_EQUIP && 
							((CObject_Item_Equip*)GetItemImpl())->GetEquipAttrib() == CObject_Item_Equip::EQUIP_ATTRIB_UNIDENTIFY)
						{
							STRING strTemp = "����Ʒ��Ҫ����";
							ADDNEWDEBUGMSG(strTemp);
							break;
						}
					}
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_COMPOSE_GEM, nIndex, this->GetItemImpl()->GetPosIndex());
					/*
						����FromIndexΪ��ɫ...
					*/
				}
				break;
			default:
				break;
			}

		}
		break;
	case 'T':		//���＼��ѧϰ/��ͯ/�ӳ�����/����ѱ��/��������
		{
			INT nIndex = szTargetName[1]-'0';
			//������Ʒ����Դ��������ʲô��Ϣ
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:			//������ҵı���
				{
					tObject_Item *pObj = this->GetItemImpl();
					if(pObj && !(pObj->IsLocked()))
					{
                        if(pObj->GetItemClass() == ICLASS_COMITEM)
						{
							CObject_Item_Medicine* pMedicine_Item = (CObject_Item_Medicine*)pObj;

							if(pMedicine_Item->GetItemTargetType() == ITEM_TARGET_TYPE_SELF_PET)
							{
								INT itemId = pMedicine_Item->GetIdTable();
								BOOL isIdOk = FALSE;
								switch(szTargetName[2]) {
								case 'S':
									//���޼�����
									isIdOk = (itemId > ITEM_PET_SKILL_STUDY_BEGIN && itemId < ITEM_PET_SKILL_STUDY_END)?TRUE:FALSE;
									break;
								case 'R':
									//���޻�ͯ��
									isIdOk = (itemId >= ITEM_PET_RETURN_BABAY_BEGIN && itemId <= ITEM_PET_RETURN_BABAY_END)?TRUE:FALSE;
									break;
								case 'A':
									//�����ӳ���������
									isIdOk = (itemId > ITEM_PET_ADD_LIFE_BEGIN && itemId < ITEM_PET_ADD_LIFE_END)?TRUE:FALSE;
									break;
								default:
									break;
								}

								if(isIdOk)
								{
									//lock item
									pMedicine_Item->SetLock(TRUE);
									CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

									//notify petskillstudy ui
									CEventSystem::GetMe()->PushEvent(GE_UPDATE_PETSKILLSTUDY, nIndex, this->GetItemImpl()->GetPosIndex());
								}
								else
								{
									STRING strTemp = "�����Ʒ������������";
									ADDNEWDEBUGMSG(strTemp);
								}
							}
							else
							{
								STRING strTemp = "�����Ʒ���Ǹ��Լ�����ʹ�õ�Ӵ";
								ADDNEWDEBUGMSG(strTemp);
							}
						}
						else
						{
							STRING strTemp = "�����Ʒ������������";
							ADDNEWDEBUGMSG(strTemp);
						}
					}
				}
				break;
			default:
				break;
			}
		}
		break;
	case 'Z':		//����װ������
		{
			INT nIndex = szTargetName[1]-'0';

			//������Դ��ȷ����δ���
			//���������ط���ֱ�Ӹ�����ʾ����װ��
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch( nTypeOwner )
			{
			case tObject_Item::IO_MYSELF_PACKET:			//���Ա������ͷ���װ����Ϣ
				{
					CObject_Item* pItem = (CObject_Item*)GetItemImpl();
					if(!pItem) return;
					//�ж�����
					if(pItem->GetItemClass() == ICLASS_EQUIP)
					{
						int iNeedLevel   = 0;
						int iPlayerLevel = 0;

						tObject_Item* pItem = GetItemImpl();
						if(pItem)
						{
							//��֤�Ƿ����ʹ��
							if(!pItem->Rule(tObject_Item::RULE_USE))
							{
								CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_USE).c_str());
								break;
							}

							iNeedLevel = pItem->GetNeedLevel();
						}

						CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
						if(pCharData)
						{
							iPlayerLevel = pCharData->Get_Level();
						}
						
						if(iPlayerLevel >= iNeedLevel)
						{
							CGameProcedure::s_pGameInterface->PacketItem_UserEquip( pItem->GetPosIndex() );
						}
						else
						{
							CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "�ȼ�����");
						}
					}
					else
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����Ʒ�޷�װ��");
					}
				}
				break;
			case tObject_Item::IO_MYSELF_EQUIP:				//�����Լ�װ��װ��
				{
				}
				break;
			case tObject_Item::IO_MYSELF_BANK:				//�������У�Ҳ����װ����Ϣ
				{
				}
				break;
			case tObject_Item::IO_MYEXBOX:					//���Խ��׶Ի���
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ʒ���ڽ���");
				}
				break;
			case tObject_Item::IO_BOOTH_CALLBACK:
			case tObject_Item::IO_CITY_SHOP:
			case tObject_Item::IO_BOOTH:					//�������˻���
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���ȹ������Ʒ");
				}
				break;
			case tObject_Item::IO_MYSTALLBOX:				//�����Լ���̯λ
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����Ʒ���ڰ�̯������");
				}
				break;
			default:
				break;
			}
		}
		break;
	case 'A':		//����̵�
		{
			INT nIndex = szTargetName[1]-'0';
			nIndex = nIndex*10 + szTargetName[2]-'0';

			//�����������ǲ����Ѿ�����Ʒ�����������Ʒ�ǲ����Ѿ��ϼ�
			INT nConIndex = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
			tObject_Item* pShopItem = CDataPool::GetMe()->PlayerShop_GetItem(TRUE, nConIndex, nIndex);
			if(pShopItem)
			{
				if(CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE, nConIndex, nIndex))
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "Ŀ��λ�õ���Ʒ�Ѿ��ϼ�");
					return;
				}
			}

			//����ǲ����ɱ��������Ϲ�����(ֻ�ܽ�����ұ����ڵ���Ʒ)
			if(this->GetItemImpl()->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)
			{
				// �ж���Ʒ�Ƿ��Ѿ���
				tObject_Item* pItem = this->GetItemImpl();
				if(pItem)
				{
					if(pItem->GetItemBindInfo() == 1)		//==1 �ǰ�
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����Ʒ�Ѿ���");
						break;
					}
					//��Ҫ���ж��ǲ����ܹ��������е���Ʒ
					if( GetItemImpl() && GetItemImpl()->GetItemClass() == ICLASS_TASKITEM )
					//if(pItem->GetPosIndex() >= TASK_CONTAINER_OFFSET )
					{
						//������Ʒ���ܷ�������
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"������Ʒ���ܷ���");
						break;
					}

				}
				//ִ�н���Ʒת�Ƶ��̵�ȥ�Ĳ���
				_ITEM_GUID Guid;
				tObject_Item::ITEM_GUID temp;
				temp.m_idUnion = 0;

				tObject_Item* pSelectItem = this->GetItemImpl();
				if(NULL == pSelectItem)
				{
					return;
				}

				pSelectItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
				Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
				Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
				Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

				CGItemSynch msg;
				msg.SetOpt(CGItemSynch::OPT_MOVE_ITEM_MANU);
				msg.SetFromType(CGItemSynch::POS_BAG);
				msg.SetToIndex(nIndex);
				msg.SetToType(CGItemSynch::POS_PLAYERSHOP);
				msg.SetItemGUID(Guid);


				INT nPage = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
				CGManuMoveItemFromBagToPlayerShop_t ExtraInfoData;
				ExtraInfoData.m_ShopGuid		= CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
				ExtraInfoData.m_nStallIndex		= (BYTE)nPage;
				ExtraInfoData.m_uSerial			= CDataPool::GetMe()->PlayerShop_GetItemSerial(TRUE,nPage,nIndex);
				msg.SetExtraInfoLength(ExtraInfoData.GetSize());
				msg.SetExtraInfoData((BYTE*)&ExtraInfoData);
			
				CNetManager::GetMe()->SendPacket(&msg);
			}
			//�ڱ����ڵ���λ��
			else if(this->GetItemImpl()->GetTypeOwner() == tObject_Item::IO_PS_SELFBOX)
			{
				//����ŵ��Լ����ӣ���ô����Ҫ���������Ϣ
				if(this->GetPosIndex() == nIndex)
				{
					return;
				}
				//ִ�н���Ʒת�Ƶ��̵�ȥ�Ĳ���
				_ITEM_GUID Guid;
				tObject_Item::ITEM_GUID temp;
				temp.m_idUnion = 0;

				tObject_Item* pSelectItem = this->GetItemImpl();
				if(NULL == pSelectItem)
				{
					return;
				}

				INT nIndex1 = pSelectItem->GetPosIndex();
				INT nConIndex = nIndex1/20;
				INT nPosition = nIndex1%20;

				if( 1 == CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE, nConIndex, nPosition) )
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����Ʒ�Ѿ��ϼ�");
					return;
				}

				pSelectItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
				Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
				Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
				Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

				CGItemSynch msg;
				msg.SetOpt(CGItemSynch::OPT_MOVE_ITEM_MANU);
				msg.SetFromType(CGItemSynch::POS_PLAYERSHOP);
				msg.SetToIndex(nIndex);
				msg.SetToType(CGItemSynch::POS_PLAYERSHOP);
				msg.SetItemGUID(Guid);

				INT nPage = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
				CGManuMoveItemFromPlayerShopToPlayerShop_t ExtraInfoData;
				ExtraInfoData.m_ShopGuid		= CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
				ExtraInfoData.m_nStallIndex		= (BYTE)nPage;
				ExtraInfoData.m_uSerialSource	= CDataPool::GetMe()->PlayerShop_GetItemSerial(TRUE,nPage,this->GetPosIndex()%20);
				ExtraInfoData.m_uSerialDest		= CDataPool::GetMe()->PlayerShop_GetItemSerial(TRUE,nPage,nIndex);
				msg.SetExtraInfoLength(ExtraInfoData.GetSize());
				msg.SetExtraInfoData((BYTE*)&ExtraInfoData);
			
				CNetManager::GetMe()->SendPacket(&msg);
			}
		}
		break;
	case 'M':		//���˵�
		{
			//��ѯ��Դ��ֻ����Դ�ڱ���
			if(this->GetItemImpl()->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)
			{
				if( this->GetItemImpl()->Rule(tObject_Item::RULE_QUICK) )
				{
					INT nIndex = szTargetName[1]-'0';
					nIndex = szTargetName[2]-'0' + nIndex*10 ;
					//��ѯĿ��λ��ԭ��������
					nOldTargetId = CActionSystem::GetMe()->MainMenuBar_Get(nIndex);
					CActionSystem::GetMe()->MainMenuBar_Set(nIndex, GetID() );
				}
				else
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����Ʒ��������������");
				}
			}
		}
		break;

	case 'Y':		//����ͷ��
		{
			CObject_Item* pItem = (CObject_Item*)GetItemImpl();
			//����װ������
			switch(pItem->GetItemClass())
			{
			case ICLASS_COMITEM:	//ҩƿ
			case ICLASS_TASKITEM:	//������Ʒ��
			case ICLASS_STOREMAP:	//�ر�ͼ
				{		//��̯ʱ���������ʹ����Ʒ
					if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ڰ�̯����");
						break;
					}

					//��֤�Ƿ����ʹ��
					if(!pItem->Rule(tObject_Item::RULE_USE))
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_USE).c_str());
						break;
					}

					//�����ȴ�Ƿ����
					if(!CoolDownIsOver()) 
					{
						CActionSystem::GetMe()->SetSetoutAction(this);
						CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( OR_COOL_DOWNING ) );
						break;
					}

					CObject *pTargetObj = (CObject*)(CObjectManager::GetMe()->GetMainTarget());
					CGameProcedure::s_pGameInterface->PacketItem_UserItem(
						this, pTargetObj? pTargetObj->GetServerID() : -1, fVector2(-1, -1));
				}
				break;
			}

		}
		break;
	case 'L':		//�Լ�ͷ��
		{
			CObject_Item* pItem = (CObject_Item*)GetItemImpl();
			//����װ������
			switch(pItem->GetItemClass())
			{
			case ICLASS_COMITEM:	//ҩƿ
			case ICLASS_TASKITEM:	//������Ʒ��
			case ICLASS_STOREMAP:	//�ر�ͼ
				{		//��̯ʱ���������ʹ����Ʒ
					if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ڰ�̯����");
						break;
					}

					//��֤�Ƿ����ʹ��
					if(!pItem->Rule(tObject_Item::RULE_USE))
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_USE).c_str());
						break;
					}

					//�����ȴ�Ƿ����
					if(!CoolDownIsOver()) 
					{
						CActionSystem::GetMe()->SetSetoutAction(this);
						CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( OR_COOL_DOWNING ) );
						break;
					}

					//����Ŀ��ʹ����Ʒ
					CObject* pMainTarget = (CObject*)CObjectManager::GetMe()->GetMainTarget();
					CGameProcedure::s_pGameInterface->PacketItem_UserItem(this, 
						CObjectManager::GetMe()->GetMySelf()->GetServerID(), fVector2(-1, -1));
				}
				break;
			}

		}
		break;
	default:
		break;
	}

	switch(cSourceName)
	{
	case 'M':
		{
			INT nIndex = szSourceName[1]-'0';
			nIndex = szSourceName[2]-'0' + nIndex*10 ;
			CActionSystem::GetMe()->MainMenuBar_Set(nIndex, nOldTargetId);
			//AxTrace(0,0,"nIndex",)
		}
		break;
	default:
		break;
	}
//	CEventSystem::GetMe()->PushEvent(GE_UPDATE_SUPERTOOLTIP);
	CEventSystem::GetMe()->PushEvent(GE_SUPERTOOLTIP, "0");
}
INT CActionItem_Item::GetCurBankRentBegin(VOID)
{
	INT nRentIndex = CActionSystem::GetMe()->GetCurBankRentBoxIndex();
	INT nBeginIndex = -1;
	switch(nRentIndex) 
	{
	case 1:
		{
			nBeginIndex = RENTBOX1_START_INDEX;
		}
		break;
	case 2:
		{
			nBeginIndex = RENTBOX2_START_INDEX;
		}
		break;
	case 3:
		{
			nBeginIndex = RENTBOX3_START_INDEX;
		}
		break;
	case 4:
		{
			nBeginIndex = RENTBOX4_START_INDEX;
		}
		break;
	case 5:
		{
			nBeginIndex = RENTBOX5_START_INDEX;
		}
		break;
	default:
		return -1;
	}

	return nBeginIndex;
}


//��ѯ�߼�����
STRING CActionItem_Item::GetAttributeValue(LPCTSTR szAttributeName)
{
	tObject_Item* pItem = GetItemImpl();

	if(!pItem) return "";
	else return pItem->GetAttributeValue(szAttributeName);
}


// ��װ��������ж��
void CActionItem_Item::UnUseEquip()
{
	CObject_Item* pItem = (CObject_Item*)GetItemImpl();
	if(!pItem) 
	{
		return;
	}

	CGUnEquip msg;
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)pItem;
	if(pItemEquip->GetItemClass() != ICLASS_EQUIP)
	{
		return;
	}

	// ����ж��װ����Ϣ.
	msg.setEquipPoint((BYTE)(pItemEquip->GetItemType()));
	CNetManager::GetMe()->SendPacket( &msg );
}

// �õ���Ʒ�ĵȼ�
INT	CActionItem_Item::GetItemLevel()
{
	tObject_Item* pItem = GetItemImpl();
	
	if(pItem)
	{
		return pItem->GetNeedLevel();
	}
	else
	{
		return -1;
	}

}

// �õ���Ʒ���;�
INT	CActionItem_Item::GetItemDur()
{
	tObject_Item* pItem = GetItemImpl();
	
	if(pItem)
	{
		return pItem->GetItemDur();
	}
	else
	{
		return -1;
	}

}



// �õ���Ʒ������;�
INT	CActionItem_Item::GetItemMaxDur()
{
	tObject_Item* pItem = GetItemImpl();
	
	if(pItem)
	{
		return pItem->GetItemMaxDur();
	}
	else
	{
		return -1;
	}
}

// �õ���Ʒ���������
INT	CActionItem_Item::GetItemRepairCount()
{
	tObject_Item* pItem = GetItemImpl();
	
	if(pItem)
	{
		return pItem->GetItemRepairCount();
	}
	else
	{
		return -1;
	}
}

// �õ���Ʒ�İ���Ϣ 
INT	CActionItem_Item::GetItemBindInfo()
{
	tObject_Item* pItem = GetItemImpl();
	if(pItem)
	{
		return pItem->GetItemBindInfo();
	}
	else
	{
		return -1;
	}
}

// �õ���Ʒ����npc�ļ۸�
INT	CActionItem_Item::GetItemPrice()
{
	tObject_Item* pItem = GetItemImpl();
	if(pItem)
	{
		return pItem->GetItemPrice();
	}
	else
	{
		return -1;
	}
	
}

// �õ���Ʒ��������
LPCTSTR	CActionItem_Item::GetManufacturer()
{
	tObject_Item* pItem = GetItemImpl();
	if(pItem)
	{
		return pItem->GetManufacturer();
	}
	else
	{
		return _T("");
	}
	
}


// �õ���ɫ����
LPCTSTR	CActionItem_Item::GetBaseWhiteAttrInfo()
{
	tObject_Item* pItem = GetItemImpl();
	if(pItem)
	{
		return pItem->GetBaseWhiteAttrInfo();
	}
	else
	{
		return _T("");
	}

	
}

// �õ���ɫ����
LPCTSTR	CActionItem_Item::GetGreenAttrInfo()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetGreenAttrInfo();
	}
	else
	{
		return _T("");
	}

	
}


// �õ���չ��ɫ����
LPCTSTR	CActionItem_Item::GetExtBlueAttrInfo()
{

	tObject_Item* pItem = GetItemImpl();
	if(pItem)
	{
		return pItem->GetExtBlueAttrInfo();
	}
	else
	{
		return _T("");
	}
}

// �õ�װ��������(��, ��, ��)
INT CActionItem_Item::GetEquipQuantity()
{
	tObject_Item* pItem = GetItemImpl();
	if(pItem)
	{
		CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)pItem;
		return pItemEquip->GetEquipQuantity();
	}
	else
	{
		return 0;
	}
}


// �õ�action�����õ�item ������(����װ��, ������Ʒ)
INT	CActionItem_Item::GetItemType()
{

	STRING strTypeName = GetType_String();

	if(("equip" == strTypeName)			|| 
	    ("packageitem" == strTypeName)	||
		("bankitem" == strTypeName)		||
		("boothitem" == strTypeName)	||
		("lootitem" == strTypeName)		||
		("virtualitem" == strTypeName)  ||
		("ex_self" == strTypeName)		||
		("ex_other" == strTypeName)		||
		("st_self" == strTypeName)		||
		("st_other" == strTypeName)		||
		("ps_self" == strTypeName)		||
		("ps_other" == strTypeName)		||
		("mission" == strTypeName)	)	

	{
		CObject_Item* pItem = (CObject_Item*)GetItemImpl();
		if(pItem)
		{
			return pItem->GetItemClass();
		}
		else
		{
			return -1;
		}
	}

	return -1;
	
}

// �õ�װ���ϵı�ʯ����
INT CActionItem_Item::GetGemInEquipCount()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	
	if(pItemEquip)
	{
		// �õ��Ѿ���Ƕ��ʯ�ĸ���
		//INT iCount = pItemEquip->GetGemCount();

		// �õ�������Ƕ��ʯ�ĸ���
		INT iCount = pItemEquip->GetGemMaxCount();
		return iCount;
	}
	else
	{
		return 0;
	}
	
}

// �õ���ʯ��ͼ��
VOID CActionItem_Item::GetGemIcon(INT iGemIndex, STRING& strIconName)
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	
	if(pItemEquip)
	{
		
		pItemEquip->GetGemIcon(iGemIndex, strIconName);
	}
	else
	{
		strIconName = _T("");
	}

	return;
}

// �õ�װ���ϵı�ʯ����
VOID CActionItem_Item::GetGemInEquipDesc(STRING& strDesc)
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	STRING strInfo = _T("");
	strDesc = _T("");
	
	if(pItemEquip)
	{
		int iCount = pItemEquip->GetGemCount();
		for(int i = 0; i < iCount; i++)
		{
			strInfo = _T("");//ÿ�ű�ʯ���¼��㸽������
			pItemEquip->GetGemExtAttr(i, strInfo);
			strDesc += strInfo;
		}
	}
	else
	{
		strDesc = _T("");
	}

	return;
}

// �õ�װ��������������
INT CActionItem_Item::GetItemMaxRepair()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		pItemEquip->GetMaxRepair();
	}
	
	return -1;
}

// �õ���ʯ�ȼ�
INT CActionItem_Item::GetGemLevel()
{
	CObject_Item_Gem* pItem = (CObject_Item_Gem*)GetItemImpl();
	if(pItem)
	{
		return pItem->GetGemLevel();
	}

	return -1;
}

// �õ���ʯ������Ϣ
LPCTSTR	CActionItem_Item::GetGemArribInfo()
{
	static TCHAR szInfo[32] = {0};

	CObject_Item_Gem* pItem = (CObject_Item_Gem*)GetItemImpl();
	if(pItem)
	{
		return pItem->GetGemArribInfo();
	}

	return szInfo;
}

// �õ�ԭ�ϻ�ҩƷ������
INT CActionItem_Item::GetItemQuality()
{
	CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();

	if(pItem)
	{
		return pItem->GetQuantity();
	}

	return -1;
}

// �õ�����Ʒ�ڱ��е�����
INT CActionItem_Item::GetMedicineItemTableType()
{
	CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();

	if(pItem)
	{
		return pItem->GetMedicineItemTableType();
	}

	return -1;
}

//�õ���ȴ״��ID
INT	CActionItem_Item::GetCoolDownID(VOID)
{
	CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();
	if(pItem && pItem->GetItemClass() == ICLASS_COMITEM)
	{
		//ֻ�б��������Ʒ����CoolDown
		if(pItem->GetTypeOwner() != tObject_Item::IO_MYSELF_PACKET)
			return -1;

		INT nSkillTableID = pItem->GetItemSkillID();

		if(nSkillTableID != -1)
		{
			//���
			DBC_DEFINEHANDLE(s_pSkillTable, DBC_SKILL_DATA);
			//������¼
			const _DBC_SKILL_DATA* pSkill = (const _DBC_SKILL_DATA*)s_pSkillTable->Search_Index_EQU((UINT)nSkillTableID);
			if(!pSkill) return NULL;

			return pSkill->m_nCooldownID;
		}
	}
	return -1;
}

//�����ȴ�Ƿ����
BOOL CActionItem_Item::CoolDownIsOver(VOID)
{
	INT nCoolDownID = GetCoolDownID();

	//����û����ȴ��Action
	if(INVALID_ID == nCoolDownID) return TRUE;
	//ȡ����ȴ������
	const CDataPool::COOLDOWN_GROUP* pCoolDownGroup =
		CDataPool::GetMe()->CoolDownGroup_Get(nCoolDownID);

	if( pCoolDownGroup && pCoolDownGroup->nTime <= 0 && CDataPool::GetMe()->CommonCoolDown_Get() <= 0 ) 
		return TRUE;
	else 
		return FALSE;
}

//DoAction_Packet
VOID CActionItem_Item::DoAction_Packet(VOID)
{
	CObject_Item* pItem = (CObject_Item*)this->GetItemImpl();
	if(!pItem) return;

	BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

	if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())
	{
		// ȡ������2006��4��15
		CancelIdentify();
		return;
	}

	//�������̵꿪�ţ�
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("PS_ShopMag"))
	{
		
		if(!(CDataPool::GetMe()->PlayerShop_GetShopType(TRUE) == 1))
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "��Ʒ���ܷ�������");
			return;
		}

		if(pItem->GetItemBindInfo() == 1)		//==1 �ǰ�
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����Ʒ�Ѿ���");
			return;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
		}

		//��Ҫ���ж��ǲ����ܹ��������е���Ʒ
		if( GetItemImpl() && GetItemImpl()->GetItemClass() == ICLASS_TASKITEM )
		//if(pItem->GetPosIndex() >= TASK_CONTAINER_OFFSET )
		{
			//������Ʒ���ܷ�������
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"������Ʒ���ܷ���");
			return;
		}

		//ִ�н���Ʒת�Ƶ��̵�ȥ�Ĳ���
		_ITEM_GUID Guid;
		tObject_Item::ITEM_GUID temp;
		temp.m_idUnion = 0;

		tObject_Item* pSelectItem = this->GetItemImpl();
		if(NULL == pSelectItem)
		{
			return;
		}
		pSelectItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
		Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
		Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
		Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

		CGItemSynch msg;
		msg.SetOpt(CGItemSynch::OPT_MOVE_ITEM_AUTO);
		msg.SetFromType(CGItemSynch::POS_BAG);
		msg.SetToType(CGItemSynch::POS_PLAYERSHOP);
		msg.SetItemGUID(Guid);

		CGAutoMoveItemFromBagToPlayerShop_t ExtraDataInfo;
		ExtraDataInfo.m_ShopGuid		= CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
		ExtraDataInfo.m_nStallIndex		= (BYTE)CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
		ExtraDataInfo.m_nShopSerial		= (BYTE)CDataPool::GetMe()->PlayerShop_GetShopSerial(TRUE);
		msg.SetExtraInfoLength(ExtraDataInfo.GetSize());
		msg.SetExtraInfoData((BYTE*)&ExtraDataInfo);
	
		if(pItem->GetByProtect())
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ʒ�Ѽ���");
		}
		else
		{
			CNetManager::GetMe()->SendPacket(&msg);
		}


		return;
	}
	//�����̯���濪�ţ������Ʒ����ɵ���̯������(�м���Ҫ����һ��ȷ�ϲ���)
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("StallSale"))
	{
		// �ж���Ʒ�Ƿ��Ѿ���
		if(pItem->GetItemBindInfo() == 1)		//==1 �ǰ�
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����Ʒ�Ѿ���");
			return;
		}
		//�ж���Ʒ�ǲ���������Ʒ
		if( GetItemImpl() && GetItemImpl()->GetItemClass() == ICLASS_TASKITEM )
		//if( this->GetPosIndex() >= TASK_CONTAINER_OFFSET )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "������Ʒ�����ϼ�����");
			return;
		}
		//��ѯ̯λ���ǲ��ǻ��п�λ
		if(CDataPool::GetMe()->MyStallBox_IsHaveEmpty(1) == FALSE)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "̯λ���Ѿ�û�пո�");
			return;
		}

		//�ȱ���ѡ�е���Ʒ
		CDataPool::GetMe()->MyStallBox_SetCurSelectItem(this->GetItemImpl());
		// ͬʱ��¼Item���ڵ�����
		CDataPool::GetMe()->MyStallBox_SetSelectItemInPos(tObject_Item::IO_MYSELF_PACKET);
		//��һ���������۵ĶԻ���
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY,"price");

		return;
	}
	//����ύ������Ʒ�Ľ��濪��,���������Ʒ�ɵ��ύ����
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("MissionReply"))
	{
		//��Ҫ��MissionBOX��Ѱ�ҿյ�λ��
		INT nIndex = -1;
		for(int i=0;i<3;i++)
		{
			if( CDataPool::GetMe()->MissionBox_GetItem(i) == NULL )
			{
				nIndex = i;
				break;
			}
		}
		if(nIndex == -1)
		{
			//û�пո���Է�����Ʒ�ˣ�������ʾ���˳�
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"������Ʒ�Ѿ�������");
			return;
		}

		//��¡��Ʒ
		tObject_Item* pNewItem  = CObject_Item::NewItem( this->GetItemImpl()->GetIdTable());
		((CObject_Item*)pNewItem)->Clone((CObject_Item*)this->GetItemImpl());

		//�����ﱣ��ֱ��֪ͨ������Ӱ�ť����������������Ϣ��
		CDataPool::GetMe()->MissionBox_SetItem(nIndex, pNewItem);

		// ���������ж�Ӧ�������Ʒ
		pItem->SetLock(TRUE);

		CActionSystem::GetMe()->UserMission_Update();
		// ���͸�����Ϣ
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_REPLY_MISSION);
		// ����������Ϣ
		CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

		return;
	}
	//������н���ʱ�򿪵ģ�������������
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Bank"))
	{
		//��Ҫ���ж��ǲ����ܹ��������е���Ʒ
		if( GetItemImpl() && GetItemImpl()->GetItemClass() == ICLASS_TASKITEM )
		//if(pItem->GetPosIndex() >= TASK_CONTAINER_OFFSET )
		{
			//������Ʒ���ܷ�������
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"������Ʒ���ܴ�������");
			return;
		}	
		
		//��Ҫ��⵱ǰ���д򿪵��ǵڼ���������
		INT nRentIndex = CActionSystem::GetMe()->GetCurBankRentBoxIndex();

		CGBankAddItem msg;
		msg.SetFromType(CGBankAddItem::BAG_POS);
		msg.SetIndexFrom(this->GetPosIndex());

		if(nRentIndex == 1)
			msg.SetIndexTo(CGBankAddItem::AUTO_POS_BOX1);
		else if(nRentIndex == 2)
			msg.SetIndexTo(CGBankAddItem::AUTO_POS_BOX2);
		else if(nRentIndex == 3)
			msg.SetIndexTo(CGBankAddItem::AUTO_POS_BOX3);
		else if(nRentIndex == 4)
			msg.SetIndexTo(CGBankAddItem::AUTO_POS_BOX4);
		else if(nRentIndex == 5)
			msg.SetIndexTo(CGBankAddItem::AUTO_POS_BOX5);

		//�������������ǲ����п�λ
		if( !CDataPool::GetMe()->UserBank_IsEmpty( nRentIndex ) )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"Ŀ�������Ѿ�����");
			return;
		}

		CNetManager::GetMe()->SendPacket(&msg);

		return;
	}
	// ������׽��濪�ţ������������Ʒ���뽻����
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Exchange"))
	{
		// �ж���Ʒ�Ƿ��Ѿ���
		if(pItem->GetItemBindInfo() == 1)		//==1 �ǰ�
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "����Ʒ�Ѿ���");
			return;
		}
		//�ж��ǲ����ܹ��������е���Ʒ
		if( pItem->GetItemClass() == ICLASS_TASKITEM )
		//if(pItem->GetPosIndex() >= TASK_CONTAINER_OFFSET )
		{
			//������Ʒ���ܽ���
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"������Ʒ���ܽ��н���");
			return;
		}
		//��֤�Ƿ���Խ���
		if(!pItem->Rule(tObject_Item::RULE_EXCHANGE))
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_EXCHANGE).c_str());
			return;
		}
		
		//�����϶���Ʒ�����׽������Ϣ
		// ��Ҫ�ȼ���Ƿ��׽�������
		CGExchangeSynchItemII msg;
		msg.SetOpt(EXCHANGE_MSG::OPT_ADDITEM);	
		msg.SetFromType(EXCHANGE_MSG::POS_BAG);
		msg.SetFromIndex(this->GetPosIndex());
		msg.SetToIndex(0);						//�ɷ���������

		if(pItem->GetByProtect())
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ʒ�Ѽ���");
		}
		else
		{
			CNetManager::GetMe()->SendPacket(&msg);
		}

		return;
	}

	// ��Ƕ����
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Enchase"))
	{
		if( GetItemImpl()->GetByProtect() )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ʒ�Ѽ���");
			return;
		}

		if(GetItemImpl()->GetItemClass() == ICLASS_EQUIP)
		{
			if(((CObject_Item_Equip*)GetItemImpl())->GetEquipQuantity() == CObject_Item_Equip::BLUE_EQUIP && 
				((CObject_Item_Equip*)GetItemImpl())->GetEquipAttrib() == CObject_Item_Equip::EQUIP_ATTRIB_UNIDENTIFY)
			{
				STRING strTemp = "����Ʒ��Ҫ����";
				ADDNEWDEBUGMSG(strTemp);
				return;
			}
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_COMPOSE_GEM, 1, this->GetItemImpl()->GetPosIndex());
		}
		if(GetItemImpl()->GetItemClass() == ICLASS_GEM)
		{
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_COMPOSE_GEM, 2, this->GetItemImpl()->GetPosIndex());
		}
		

		return;
	}

	//�����Ʒ�����ǿ���,�����������
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Shop"))
	{
		//�Ƿ�������״̬
		if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
		{
			//ȡ������
			CGameProcedure::s_pGameInterface->Skill_CancelAction();
		}
		//ִ������
		else
		{
			//������Ʒ��������
			if( pItem->GetItemClass() == ICLASS_TASKITEM )
			//if(pItem->GetPosIndex() < TASK_CONTAINER_OFFSET )
			{
				//������Ʒ����������Ϣ��ʾ
				if(bLog)
				{
					STRING strTemp = "�����Ʒ���ܳ���";//NOCOLORMSGFUNC("stall_sold_failure");
					ADDNEWDEBUGMSG(strTemp);
				}
			}
			else
			{
				CGameProcedure::s_pGameInterface->Booth_SellItem( pItem );
			}
		}

		return;
	}
	else
	{
		//����װ������
		switch(pItem->GetItemClass())
		{
		case ICLASS_EQUIP:
			//װ��
			{
				//ʹ��
				CGameProcedure::s_pGameInterface->PacketItem_UserEquip( pItem->GetPosIndex() );
			}
			break;

		case ICLASS_COMITEM:	//ҩƿ
		case ICLASS_TASKITEM:	//������Ʒ��
		case ICLASS_STOREMAP:	//�ر�ͼ
			{
				int iType = pItem->GetItemTableType();

				//��̯ʱ���������ʹ����Ʒ
				if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ڰ�̯����");
					break;
				}

				//��֤�Ƿ����ʹ��
				if(!pItem->Rule(tObject_Item::RULE_USE))
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_USE).c_str());
					break;
				}

				//�����ȴ�Ƿ����
				if(!CoolDownIsOver()) 
				{
					CActionSystem::GetMe()->SetSetoutAction(this);
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( OR_COOL_DOWNING ) );
					break;
				}
				CObject* pMainTarget = (CObject*)CObjectManager::GetMe()->GetMainTarget();

				CGameProcedure::s_pGameInterface->PacketItem_UserItem(this, 
					pMainTarget ? pMainTarget->GetServerID() : -1, fVector2(-1, -1));
			}
			break;

		case ICLASS_GEM:
			//��ʯ
			{
			}
			break;
		}
	}
}

//������Ʒ�Ĳ���
VOID CActionItem_Item::DestoryItem(LPCSTR szSourceName)
{
	CHAR cSourceType = szSourceName[0];
	switch(cSourceType)
	{
	case 'M':		//���˵�
		{
			INT nIndex = szSourceName[1]-'0';
			nIndex = szSourceName[2]-'0' + nIndex*10 ;
			CActionSystem::GetMe()->MainMenuBar_Set(nIndex, -1 );
			return;
		}
		break;
	default:
		break;
	}
	INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
	if( nTypeOwner == tObject_Item::IO_MYSELF_PACKET || 
		//nTypeOwner == tObject_Item::IO_MYSELF_EQUIP  ||
		nTypeOwner == tObject_Item::IO_MYSELF_BANK)
	{
		// ������Ʒ�������٣���һ����ʾ
		if(nTypeOwner == tObject_Item::IO_MYSELF_PACKET && GetItemImpl() && GetItemImpl()->GetItemClass() == ICLASS_TASKITEM )
		{
			// ��Ҫ��ѯ���Ƿ��������װ��
			//CEventSystem::GetMe()->PushEvent(GE_OPEN_CANNT_DISCARD_ITEM,this->GetItemImpl()->GetName());
			// ������Ļ��ʾ

			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"������Ʒ���ܶ���");
		}
		else
		{
			// ��ȡ�����ܴ��ڵ�����
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
					
			// ��Ҫ��ѯ���Ƿ��������װ��
			CEventSystem::GetMe()->PushEvent(GE_OPEN_DISCARD_ITEM_FRAME,this->GetItemImpl()->GetName());

			// ���������Ʒ
			tObject_Item *pObj = this->GetItemImpl();
			pObj->SetLock(TRUE);

			//֪ͨ��������״̬
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

			CDataPool::GetMe()->DisCard_SetItemConta(this->GetItemImpl()->GetTypeOwner());
			CDataPool::GetMe()->DisCard_SetItemPos(this->GetPosIndex());
		}
	}
}

// �õ��䷽��Ҫ�ļ������ơ�
LPCTSTR CActionItem_Item::GetPrescrNeedSkillName()
{
	CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();
	if(pItem)
	{
		INT nIndex = pItem->GetBaseDefine()->nScriptID - 700000;


		// �����
		DBC_DEFINEHANDLE(s_ItemCompound, DBC_LIFEABILITY_ITEMCOMPOSE);
		_DBC_LIFEABILITY_ITEMCOMPOSE* pLifeInfo = (_DBC_LIFEABILITY_ITEMCOMPOSE*)s_ItemCompound->Search_Index_EQU(nIndex);

		//�䷽��
		DBC_DEFINEHANDLE(s_pAbility, DBC_LIFEABILITY_DEFINE);
		_DBC_LIFEABILITY_DEFINE* pData = (_DBC_LIFEABILITY_DEFINE*)s_pAbility->Search_Index_EQU(pLifeInfo->nLifeAbility);

		if(pData)
		{
			return pData->szName;
		}
	}

	return _T("");
			
}

// �õ��䷽��Ҫ�ļ��ܵȼ���
int CActionItem_Item::GetPrescrNeedSkillLevel()
{

	CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();
	if(pItem)
	{
		INT nIndex = pItem->GetBaseDefine()->nScriptID - 700000;

//		int iNeedSkillLevel = pItem->m_theBaseDef->nLevelRequire;
		DBC_DEFINEHANDLE(s_ItemCompound, DBC_LIFEABILITY_ITEMCOMPOSE);
		const _DBC_LIFEABILITY_ITEMCOMPOSE* pLifeInfo = (_DBC_LIFEABILITY_ITEMCOMPOSE*)s_ItemCompound->Search_Index_EQU(nIndex);

		return pLifeInfo->nLifeAbility_Level;
	//	return iNeedSkillLevel;
	}

	return 0;
}
bool CActionItem_Item::IsPrescrStudy()
{
	//CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();
	//if(pItem)
	//{
	//	const SCLIENT_PRESCR* pPrescr = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr(pItem->GetBaseDefine()->nRecipeID);
	//	if(pPrescr) 
	//		return true;
	//}
	return false;
}
// �õ��䷽��Ҫ���ܵĵ�ǰ�ȼ���
int CActionItem_Item::GetPrescrNeedSkillCurLevel()
{
	CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();
	if(pItem)
	{
		INT nIndex = pItem->GetBaseDefine()->nScriptID - 700000;

		const SCLIENT_PRESCR* pPrescr = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr(nIndex);
		if(pPrescr)
		{
			return INVALID_ID;
		}
		else
		{
			DBC_DEFINEHANDLE(s_ItemCompound, DBC_LIFEABILITY_ITEMCOMPOSE);
			const _DBC_LIFEABILITY_ITEMCOMPOSE* pLifeInfo = (_DBC_LIFEABILITY_ITEMCOMPOSE*)s_ItemCompound->Search_Index_EQU(nIndex);
			
			const SCLIENT_LIFEABILITY* pLifeAbility = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(pLifeInfo->nLifeAbility);

			if(!pLifeAbility)
				return 0;

			return pLifeAbility->m_nLevel;
			

		}
	}

	return 0;
}

// ����������ϸ������Ϣ
void CActionItem_Item::SendAskItemInfoMsg()
{
	if(GetItemImpl()->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)
	{
		CObject_Item* pItem = (CObject_Item*)GetItemImpl();

		if((pItem)&&(pItem->IsItemHasDetailInfo()))
		{
			int iIndex = GetItemImpl()->GetPosIndex();
			CGAskItemInfo msg;
			msg.setID(iIndex);
			CNetManager::GetMe()->SendPacket(&msg);
		}
	}
}

// �õ�װ���ļ�����Ϣ2006��3��29
INT CActionItem_Item::GetEquipIdentify()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetEquipAttrib();
	}

	return 0;
	
}

// ʹ�ü�������2006-3-30
VOID CActionItem_Item::UseIdentify()
{
	CObject_Item* pItem = (CObject_Item*)GetItemImpl();
	if(ICLASS_EQUIP != pItem->GetItemClass())
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "ֻ�ܼ�������");
		return;
	}

	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		if(CObject_Item_Equip::BLUE_EQUIP == pItemEquip->GetEquipQuantity())
		{
			if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())
			{
				if(CObject_Item_Equip::EQUIP_ATTRIB_IDENTIFY == pItemEquip->GetEquipAttrib())
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "�Ѿ���������");
				}
				else
				{
					//if(pItemEquip->GetNeedLevel() > (CDataPool::GetMe()->m_iIdentifyLevel + 9))
					//{
					//	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���ܼ����Ⱦ��ἶ��ߵĵ���");
					//	return;
					//}//

					// ʹ�þ���ı�������λ��
					//ʹ3�ּ�������ͨ�� fujia 2008.1.2
					//CObject_Item* pItem = (CObject_Item*)CDataPool::GetMe()->UserBag_GetItem( CDataPool::GetMe()->m_iIdentifyInBagIndex);

					//int iIdentifyType = pItem->GetItemTableType();
					//switch(iIdentifyType)
					//{
					//case CObject_Item_Medicine::IDEN_EQUIP:
					//	{
					//		if(!pItemEquip->IsWeapon())
					//		{
					//			// ֻ�ܼ�������
					//			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "ֻ�ܼ�������");
					//			return;
					//		}

					//		break;
					//	}
					//case CObject_Item_Medicine::IDEN_DEFINCE:
					//	{
					//		if(!pItemEquip->IsDefence())
					//		{
					//			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "ֻ�ܼ�������");
					//			return;
					//		}
					//		break;
					//	}
					//case CObject_Item_Medicine::IDEN_APPEND:
					//	{
					//		if(!pItemEquip->IsAppend())
					//		{
					//			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "ֻ�ܼ�����Ʒ");
					//			return;
					//		}
					//		break;
					//	}

					//default:
					//	{
					//		break;
					//	}

					//}

					// ���ͼ�����Ϣ
					CGUseItem msg;
					msg.SetBagIndex( CDataPool::GetMe()->m_iIdentifyInBagIndex );
					msg.SetTargetItem(pItemEquip->GetPosIndex());
					CNetManager::GetMe()->SendPacket(&msg);
				}
			}
		}
		else
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "ֻ�ܼ�����ɫװ��");
		}
	}
	
}



	// ȡ������״̬2006��4��15
VOID CActionItem_Item::CancelIdentify()
{
	// ����action button
	CDataPool::GetMe()->Identify_UnLock();
}

// ׼��ʹ�ü�������2006-3-30
VOID CActionItem_Item::PrepareUseIdentify()
{
	if(CActionSystem::GetMe()->GetDefaultAction()==CActionItem_MouseCmd_Identify::GetMe())
	{
		//�Ѿ��������״̬
		return;
	}

	// ʹ�þ���ı�������λ��
	CObject_Item* pItem = (CObject_Item*)GetItemImpl();
	if(pItem)
	{
		if(ICLASS_COMITEM == pItem->GetItemClass())
		{
			int iType = pItem->GetItemTableType();
			if((iType >= COMITEM_WPIDENT)&&(iType <= COMITEM_NCIDENT))
			{
				CDataPool::GetMe()->m_iIdentifyInBagIndex = pItem->GetPosIndex();
				CDataPool::GetMe()->m_iIdentifyLevel	  = pItem->GetNeedLevel();

				GetItemImpl()->SetLock(TRUE);
				CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

				// ��¼����λ�á�
				CDataPool::GetMe()->Identify_Lock(pItem->GetPosIndex());
			}
		}
	}
}

	
// �õ���������
LPCTSTR	CActionItem_Item::GetTypeDesc()
{
	// ʹ�þ���ı�������λ��
	CObject_Item* pItem = (CObject_Item*)GetItemImpl();
	if(pItem)
	{
		return pItem->GetItemTableTypeDesc();
	}

	return "";
}

// �Ƿ���Ҫ��ToolTip����ʾ�۸�
INT CActionItem_Item::IsViewToolTopsPrice()
{
	//��ʾToolTips�ϵļ۸�����������������������������Shop����������µ�
	//1.��Shop���濪������ʱ�򱳰��е���Ʒ��ʾ
	//2.Shop�ϵĻع���Ʒ��ʾ
	if( CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Shop") )
	{
		if( NAMETYPE_PACKAGEITEM == GetType_String() )
		{
			// ����۸�
			if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
			{
				//�����Npc�ǲ����ܹ����������Ʒ
				if(CDataPool::GetMe()->Booth_IsCanRepair(this->GetItemImpl()))
				{
					return TOOLTOPS_VIEW_PRICE_REPAIR;
				}
			}

			// ����Ҫ�������NPC�ǲ����չ��������
			else if(CDataPool::GetMe()->Booth_IsCanBuy(this->GetItemImpl()))
			{
				return TOOLTOPS_VIEW_PRICE_SELL;
			}
		}
		if( NAMETYPE_EQUIP == GetType_String() )
		{
			// ����۸�
			if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
			{
				return TOOLTOPS_VIEW_PRICE_REPAIR;
			}
		}
		if( NAMETYPE_CALLBACK == GetType_String() )
		{
			return TOOLTOPS_VIEW_PRICE_CALLBACK;
		}
	}

	//3.��Ʊ���������̵�����Ƿ�򿪣��������Ƿ���������
	// �õ�supertooltip��ֱ���� [9/11/2006]
	//if(NAMETYPE_PACKAGEITEM == GetType_String()			//1.������
	//	&& GetDefineID() == TICKET_ITEM_INDEX			//2.��Ʊ����Ʒ���
	//	&& GetItemType() == ICLASS_MATERIAL				//3.ԭ������
	//	&& !(((CObject_Item*)GetItemImpl())->IsItemHasDetailInfo()))		//4.��չ��Ϣ�Ѿ���ȡ
	//{
	//	return TOOLTOPS_VIEW_LOOT_SILVER;
	//}

	return TOOLTOPS_VIEW_PRICE_NO;
}

// �����ToolTip����ʾ�ļ۸�
INT CActionItem_Item::GetViewToolTopsPrice()
{
	//��ʾToolTips�ϵļ۸�����������������������������Shop����������µ�
	//1.��Shop���濪������ʱ�򱳰��е���Ʒ��ʾ
	//2.Shop�ϵĻع���Ʒ��ʾ
	if( CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Shop") )
	{
		if( NAMETYPE_PACKAGEITEM == GetType_String() )
		{
			if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
			{
				FLOAT fRepairSpend = CDataPool::GetMe()->GetMe()->Booth_GetRepairSpend();

				if( this->GetItemImpl() && this->GetItemImpl()->GetItemClass() == ICLASS_EQUIP )
				{
					return (INT)(((CObject_Item_Equip*)this->GetItemImpl())->GetRepairPrice() * fRepairSpend);
				}
				else
				{
					return 0;
				}
			}
			else if(CDataPool::GetMe()->Booth_IsCanBuy(this->GetItemImpl()))
			{
				return CDataPool::GetMe()->ItemBox_GetCurNpcBuyPrice(GetPosIndex());
			}
		}
		if( NAMETYPE_EQUIP == GetType_String() )
		{
			// ����۸�
			if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
			{
				FLOAT fRepairSpend = CDataPool::GetMe()->GetMe()->Booth_GetRepairSpend();

				if( this->GetItemImpl() && this->GetItemImpl()->GetItemClass() == ICLASS_EQUIP )
				{
					return (INT)(((CObject_Item_Equip*)this->GetItemImpl())->GetRepairPrice() * fRepairSpend);
				}
				else
				{
					return 0;
				}
			}
		}
		if( NAMETYPE_CALLBACK == GetType_String() )
		{
			return CDataPool::GetMe()->Booth_GetSoldPrice(GetPosIndex());
		}
	}

	//3.��Ʊ���������̵�����Ƿ�򿪣��������Ƿ���������
	// �õ�supertooltip��ֱ���� [9/11/2006]
	//if(NAMETYPE_PACKAGEITEM == GetType_String()			//1.������
	//	&& GetDefineID() == TICKET_ITEM_INDEX			//2.��Ʊ����Ʒ���
	//	&& GetItemType() == ICLASS_MATERIAL				//3.ԭ������
	//	&& !(((CObject_Item*)GetItemImpl())->IsItemHasDetailInfo()))		//4.��չ��Ϣ�Ѿ���ȡ
	//{
	//	if(((CObject_Item*)GetItemImpl())->GetItemExtraInfoItem())	//Ϊ�˰�ȫ�����Ǽ��һ�°ɡ�
	//	{
	//		INT nMoney = ((CObject_Item*)GetItemImpl())->GetItemExtraInfoItem()->GetItemParamValue( 0, IPV_INT );
	//		return (nMoney >= 0)?nMoney:0;
	//	}
	//}

	return 0;
}

// �õ���Ʒ���εȼ����� 2006-5-18
INT CActionItem_Item::GetItemLevelDesc()
{
	// ʹ�þ���ı�������λ��
	CObject_Item* pItem = (CObject_Item*)GetItemImpl();
	if(pItem)
	{
		if(ICLASS_COMITEM == pItem->GetItemClass())
		{
			return pItem->GetItemLevelDesc();
		}
	}

	return 0;

}
