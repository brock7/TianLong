#include "StdAfx.h"
#include "GMInterface_Script_Tooltip.h"
#include "TDException.h"
#include "LuaPlus.h"
#include "TDActionSystem.h"
#include "..\action\GMActionSystem_Item.h"
#include "..\action\GMActionSystem_XinFa.h"
#include "..\action\GMActionSystem_Skill.h"
#include "..\object\item\Obj_Item_Medicine.h"
#include "..\DBC\GMDataBase.h"
#include "..\Procedure\GameProcedure.h"
#include "..\DataPool\GMDataPool.h"
//
//�ű�����ɳ�䣬��Щ����Ӧ���ɽű�����
//
namespace SCRIPT_SANDBOX
{

	SuperTooltips SuperTooltips::s_SupperTooltip;
	LuaPlus::LuaObject* SuperTooltips::s_pMetaTable = NULL;
	SuperTooltips::SuperTooltips()
	{
		m_pActionItem = NULL;
	}

	SuperTooltips::~SuperTooltips()
	{
	}

	//����Ϸ�߼����ü����е�ActionItem
	VOID SuperTooltips::SetActionItem(CActionItem* pActionitem)
	{
		m_pActionItem = pActionitem;
		if(m_pActionItem)
		{
			m_nActionId = m_pActionItem->GetID();
		}
		
	}
		
	INT SuperTooltips::GetTitle(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(m_pActionItem->GetName());
		return 1;
	}


	INT SuperTooltips::GetIcon(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(m_pActionItem->GetIconName());
		return 1;
	}

	INT SuperTooltips::GetAttributeValue(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		//������
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA: SuperTooltips GetAttributeValue Wrong Param1");
		}

		//ͨ��ActionItem��ѯ�߼�����
		state->PushString(m_pActionItem->GetAttributeValue(args[2].GetString()).c_str());

		return 1;
	}

	INT	SuperTooltips::GetItemDesc(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		//�õ���ϸ˵��.
		state->PushString(m_pActionItem->GetDesc());
		return 1;
	}
	INT SuperTooltips::GetItemTransferParam( LuaPlus::LuaState* state)
	{
		/*
		���㴫����Ҫ����
		1������id 

		2����Ʒʹ�ô���  
		3��x����
		4.z����
		
		*/
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		CObject_Item* pItem = (CObject_Item*)((CActionItem_Item*)m_pActionItem)->GetImpl();
		if( pItem && (pItem->GetItemExtraInfoItem()) )
		{

			int sceneid=GETLOW(pItem->GetItemExtraInfoItem()->GetItemParamValue( 0, IPV_INT ));
			int nUseCount=GETHIGH(pItem->GetItemExtraInfoItem()->GetItemParamValue( 0, IPV_INT ));
			state->PushInteger( sceneid );
			state->PushInteger( nUseCount );
			state->PushInteger( pItem->GetItemExtraInfoItem()->GetItemParamValue( 4, IPV_INT ) );
			state->PushInteger( pItem->GetItemExtraInfoItem()->GetItemParamValue( 8, IPV_INT ) );
	
			return 4;
		}
		state->PushInteger( -1 );
		state->PushInteger( -1 );
		state->PushInteger( -1 );
		state->PushInteger( -1 );
		
		return 4;

	}
	/*
	��Ʒ����
	*/
	INT SuperTooltips::GetItemIndex( LuaPlus::LuaState* state)
	{
		
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		CObject_Item* pItem = (CObject_Item*)((CActionItem_Item*)m_pActionItem)->GetImpl();
		if( pItem && (pItem->GetItemExtraInfoItem()) )
		{
			state->PushInteger( pItem->GetItemExtraInfoItem()->m_ItemIndex );
			return 1;
		}
		state->PushInteger( -1 );

		return 1;

	}

	INT	SuperTooltips::GetItemLevel(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			return 1;
		}

		state->PushInteger(m_pActionItem->GetItemLevel());
		return 1;
	}

	//��ѯTooltip����Ʒ�;�
	INT	SuperTooltips::GetItemDur(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			state->PushInteger(-1);
			return 2;
		}

		state->PushInteger(m_pActionItem->GetItemDur());
		state->PushInteger(m_pActionItem->GetItemMaxDur());
		return 2;
	}

	//��ѯTooltip����Ʒ���������
	INT	SuperTooltips::GetItemRepairCount(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			return 1;
		}

		state->PushInteger(m_pActionItem->GetItemRepairCount());
	
		return 1;
	}

	//
	// �õ�����������
	INT	SuperTooltips::GetEquipMaxRepair(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(!pActionItem_Item)
		{
			state->PushInteger(-1);
			return 1;
		}

		state->PushInteger(pActionItem_Item->GetItemMaxRepair());
		return 1;
	
	}

	//��ѯTooltip����Ʒ�İ���Ϣ
	INT	SuperTooltips::GetItemBindInfo(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			return 1;
		}

		state->PushInteger(m_pActionItem->GetItemBindInfo());
	
		return 1;
	}

	//��ѯTooltip����Ʒ������npc�ļ۸�
	INT	SuperTooltips::GetItemPrice(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			return 1;
		}

		//state->PushInteger(m_pActionItem->GetItemPrice());

		INT iPrice = CDataPool::GetMe()->ItemBox_GetCurNpcBuyPrice(m_pActionItem->GetPosIndex());
		state->PushInteger(iPrice);

		return 1;
	}

	//��ѯTooltip����Ʒ��������
	INT	SuperTooltips::GetItemManufacturer(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(m_pActionItem->GetManufacturer());
	
		return 1;
	}

	//��ѯTooltip����Ʒ�İ�ɫ����
	INT	SuperTooltips::GetItemBaseWhiteAttrInfo(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(m_pActionItem->GetBaseWhiteAttrInfo());
		return 1;
	}

	
	//��ѯTooltip����Ʒ����ɫ����
	INT	SuperTooltips::GetItemGreenAttrInfo(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(!pActionItem_Item)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(pActionItem_Item->GetGreenAttrInfo());
		return 1;
	}

	//��ѯTooltip����Ʒ����ɫ����
	INT	SuperTooltips::GetItemExtBlueAttrInfo(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(m_pActionItem->GetExtBlueAttrInfo());
		return 1;
	}

	
	//��ѯTooltip����Ʒ������
	INT	SuperTooltips::GetItemType(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(m_pActionItem->GetType_String());
		return 1;
	}


	//��ѯTooltip��װ����Ʒ��(��, ��, ��)
	INT	SuperTooltips::GetItemEquipQuantity(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
	
		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(!pActionItem_Item)
		{
			state->PushInteger(0);
			return 1;
		}

		state->PushInteger(pActionItem_Item->GetEquipQuantity());
		return 1;

	}

	//��ѯTooltip��װ������������
	INT	SuperTooltips::GetItemClassType(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(!pActionItem_Item)
		{
			state->PushInteger(0);
			return 1;
		}

		state->PushInteger(pActionItem_Item->GetItemType());
		return 1;
	}

	//��ѯTooltip��װ���ı�ʯ��Ϣ
	//
	//
	//   
	// ��������5��
	//   0[int] - ��ʯ����
	//   1[str] - ��ʯ1��ͼ���ַ��� "" - ��ʾ��ͼ��
	//	 2[str] - ��ʯ2��ͼ���ַ��� "" - ��ʾ��ͼ��
	//	 3[str] - ��ʯ2��ͼ���ַ��� "" - ��ʾ��ͼ��
	//	 4[str] - ��ʯ�������Ե�����
	INT	SuperTooltips::GetGemInEquipInfo(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			// �õ���ʯ�ĸ���
			STRING strInfo = _T("");
			INT iCount = pActionItem_Item->GetGemInEquipCount();

			state->PushInteger(iCount);

			for(int i = 0; i < 3; i++)
			{
				strInfo = _T("");
				pActionItem_Item->GetGemIcon(i, strInfo);
				state->PushString(strInfo.c_str());
			}

			strInfo = _T("");
			pActionItem_Item->GetGemInEquipDesc(strInfo);
			state->PushString(strInfo.c_str());
		}
		else//
		{

			state->PushInteger(0);
			state->PushString("");
			state->PushString("");
			state->PushString("");
			state->PushString("");

		}

		return 5;
	}


	

	//----------------------------------------------------------------------------------------------------------------------
	// �ķ�����
	//
	
	// �õ��ķ��ȼ�
	// ��������1��
	// 0[int] - ��ǰѧ����ķ��ȼ�.
	//INT	SuperTooltips::GetXinfaLevel(LuaPlus::LuaState* state)
	//{
	//	// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
	//	m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

	//	if(!m_pActionItem)
	//	{
	//		state->PushInteger(0);
	//		return 1;
	//	}

	//	CActionItem_XinFa* pXinfaAction = (CActionItem_XinFa*)m_pActionItem;
	//	if(pXinfaAction)
	//	{
	//		state->PushInteger(pXinfaAction->GetXinfaLevel());
	//		return 1;
	//	}

	//	state->PushInteger(0);
	//	return 1;
	//	
	//}

	INT SuperTooltips::GetItem_IsProtected(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(false);
			return 1;
		}

		CObject_Item* pItem = (CObject_Item*)((CActionItem_Item*)m_pActionItem)->GetImpl();
		if(!pItem)
		{
			state->PushInteger(false);
			return 1;
		}

		state->PushBoolean(pItem->GetByProtect()==TRUE);
		return 1;
	}

	//----------------------------------------------------------------------------------------------------------------------
	// ���ܺ���
	//

	// �õ����ܵȼ�
	// ��������1��
	// 0[int] - ��ǰѧ��ļ��ܵȼ�.
	INT	SuperTooltips::GetSkillLevel(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(0);
			return 1;
		}

		CActionItem_Skill* pAction = (CActionItem_Skill*)m_pActionItem;
		if(pAction)
		{
			state->PushInteger(pAction->GetSkillLevel());
			return 1;
		}

		state->PushInteger(0);
		return 1;
	}


	//----------------------------------------------------------------------------------------------------------------------
	// �õ��Ƿ�ѧ�Ἴ��
	//

	// �õ��Ƿ�ѧ�Ἴ��
	// ��������1��
	// 0[int] - �Ƿ�ѧ�� 1 - ѧ��, 0 - ûѧ��
	INT	SuperTooltips::GetIsLearnedSkill(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Skill* pAction = (CActionItem_Skill*)m_pActionItem;
		if(pAction)
		{
			state->PushInteger(pAction->GetIsLearnedSkill());
			return 1;
		}

		state->PushInteger(0);
		return 1;
	}

	//----------------------------------------------------------------------------------------------------------------------
	// �õ���������
	//

	// �õ���������
	// ��������1��
	// 0[str] - �õ���������
	INT	SuperTooltips::GetSkillDesc(LuaPlus::LuaState* state)
	{

		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Skill* pAction = (CActionItem_Skill*)m_pActionItem;

		if(pAction)
		{
			state->PushString(pAction->GetDesc());
			return 1;
		}

		state->PushString(_T(""));
		return 1;
	}

		
    //----------------------------------------------------------------------------------------------------------------------
	//
	// ��ʯ��Ϣ
	//

	// �õ���ʯ�ȼ�
	// ��������1��
	// 0[int] - ��ʯ�ȼ�
	INT	SuperTooltips::GetGemLevel(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			
			state->PushInteger(pActionItem_Item->GetGemLevel());
			return 1;
		}

		state->PushInteger(-1);
		return 1;
	}

	// �õ���ʯ����
	// ��������1��
	// 0[str] - ��ʯ����
	INT	SuperTooltips::GetGemArribInfo(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			
			state->PushString(pActionItem_Item->GetGemArribInfo());
			return 1;
		}

		state->PushString("");
		return 1;
	}


	//----------------------------------------------------------------------------------------------------------------------
	//
	// ҩƷ��ʯ��Ϣ
	//

	// �õ�ҩƷ��ʯƷ��
	// ��������1��
	// 0[int] - ҩƷ��ʯƷ��
	INT	SuperTooltips::GetItemQuality(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			
			state->PushInteger(pActionItem_Item->GetItemQuality());
			return 1;
		}

		state->PushInteger(-1);
		return 1;
	}


	// �õ����ܶ�Ӧ���ķ��ȼ�
	//
	// ��������1��
	// 0[int] - �ķ��ȼ�
	//INT	SuperTooltips::GetSkillXinfaLevel(LuaPlus::LuaState* state)
	//{
	//	// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
	//	m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

	//	CActionItem_Skill* pAction = NULL;

	//	pAction = (CActionItem_Skill*)m_pActionItem;
	//	if(pAction)
	//	{
	//		state->PushInteger(pAction->GetSkillXinfaLevel());
	//		return 1;
	//	}

	//	state->PushInteger(0);
	//	return 1;

	//}

	// �õ���Ʒ�ڱ��е�����
	// ��������1��
	// 0[int] - ���е����͡�
	INT	SuperTooltips::GetItemTableType(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			
			state->PushInteger(pActionItem_Item->GetMedicineItemTableType());
			return 1;
		}

		state->PushInteger(-1);
		return 1;
	}


	// �õ��䷽��Ҫ�ļ������֡�
	// ��������1��
	// 0[int] - ���е����͡�
	INT	SuperTooltips::GetPrescrSkillName(LuaPlus::LuaState* state)
	{

		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			//CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)pActionItem_Item->GetItemImpl();
			//if(pItem)
			//{
			//	INT nIndex = pItem->m_theBaseDef->nScriptID - 700000;

			//	//�򿪱�ʯ���ݱ�
			//	DBC_DEFINEHANDLE(s_pItem_Compound, DBC_LIFEABILITY_ITEMCOMPOSE);
			//	_DBC_LIFEABILITY_ITEMCOMPOSE* pData = (_DBC_LIFEABILITY_ITEMCOMPOSE*)s_pItem_Compound->Search_Index_EQU(nIndex);

			//	if(pData)
			//	{
			//		state->PushString(pData->szName);
			//		return 1;
			//	}
			//	//const SCLIENT_PRESCR* pPrescr = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr(nIndex);
			//	//if(pPrescr)
			//	//{
			//	//	STRING strTemp = "#r#cFFCF00��ѧ��";
			//	//	m_strExtraDesc += strTemp;
			//	//}//

			//}//

			LPCTSTR pNeedSkillName = pActionItem_Item->GetPrescrNeedSkillName();
			if(pNeedSkillName)
			{
				state->PushString(pNeedSkillName);
				return 1;
			}
		}//


		state->PushString("");
		return 1;
	}


	
	// �õ��䷽��Ҫ�ļ��ܵȼ���
	// ��������1��
	// 0[int] - ���еĵȼ���
	INT	SuperTooltips::GetPrescrNeedSkillLevel(LuaPlus::LuaState* state)
	{

		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			int iNeedLevel = pActionItem_Item->GetPrescrNeedSkillLevel();
			state->PushInteger(iNeedLevel);
			return 1;
			
		}//


		state->PushInteger(-1);
		return 1;
	}


	// �õ��䷽��Ҫ���ܵĵ�ǰ�ȼ���
	// ��������1��
	// 0[int] - �䷽��Ҫ�ĵ�ǰ�ȼ���
	INT	SuperTooltips::GetPrescrNeedSkillCurLevel(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			int iNeedCurLevel = pActionItem_Item->GetPrescrNeedSkillCurLevel();
				
			state->PushInteger(iNeedCurLevel);
			return 1;
			
		}//

		state->PushInteger(0);
		return 1;
	}

	// �����������item����ϸ��Ϣ
	// 
	// 
	INT	SuperTooltips::SendAskItemInfoMsg(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(m_pActionItem->GetType() != AOT_ITEM) return 0;

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			pActionItem_Item->SendAskItemInfoMsg();
			
		}//

		return 0;
	}


		
	// �õ���ɫװ���Ƿ����
	// 
	// 
	INT	SuperTooltips::IsBlueEquipIdentify(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;
		//if(AOT_EQUIP == pActionItem_Item->GetType())
		if(pActionItem_Item)
		{
			INT nIdentify = pActionItem_Item->GetEquipIdentify();
			state->PushInteger(nIdentify);
		}
		else
		{
			
			state->PushInteger(0);
		}

		return 1;
	}


	
	//
	// npc�̵��Ƿ�򿪡�
	// 
	INT	SuperTooltips::IsNpcShopOpen(LuaPlus::LuaState* state)
	{

		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Shop"))
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}

		return 1;
	}

	INT SuperTooltips::IsPresent(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		state->PushInteger(m_pActionItem ? 1 : 0);
		return 1;
	}

	//�õ�������Ϣ2006��4��28
	INT SuperTooltips::GetTypeDesc(LuaPlus::LuaState* state)
	{

		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;
		if(pActionItem_Item)
		{
			state->PushString(pActionItem_Item->GetTypeDesc());
		}

		return 1;
	}

	//�õ���Ʒ�Ƿ��ڱ�����2006-5-10
	INT SuperTooltips::IsItemInPacket(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if(!m_pActionItem)
		{
			state->PushString(0);
			return 1;
		}

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		// ʹ�þ���ı�������λ��
		CObject_Item* pItem = (CObject_Item*)pActionItem_Item->GetItemImpl();
		if(NULL == pItem)
		{
			state->PushInteger(0);
			return 1;
		}

		if(tObject_Item::IO_MYSELF_PACKET == pItem->GetTypeOwner())
		{
			state->PushInteger(1);
			return 1;
		}

		state->PushInteger(0);
		return 1;
	}

	//
	// �õ��Ƿ���ʾ�۸� 2006-5-18
	//
	INT SuperTooltips::IsShowPrice(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if(!m_pActionItem)
		{
			state->PushInteger(0);
			return 1;
		}


		state->PushInteger(m_pActionItem->IsViewToolTopsPrice());
		return 1;
	}

	//
	// �õ���ʾ�۸� 2006-5-18
	//
	INT SuperTooltips::GetShowPrice(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if(!m_pActionItem)
		{
			state->PushInteger(0);
			return 1;
		}


		state->PushInteger(m_pActionItem->GetViewToolTopsPrice());
		return 1;
	}

	//
	// �õ���Ʒ���εȼ����� 2006-5-18
	//
	INT SuperTooltips::GetItemLevelDesc(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣2006��4��15
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if(!m_pActionItem)
		{
			state->PushInteger(0);
			return 1;
		}

		state->PushInteger(m_pActionItem->GetItemLevelDesc());
		return 1;
	}

	INT SuperTooltips::GetAnchorValue( LuaPlus::LuaState* state)
	{
		/*
		1��Param1 - SHORT(16) - Key����λ��ʶ�������ϣ�Ҳ��Client����Tooltips�����ݣ�����չ��
		��key==10ʱ������Tooltips��������ʾԭʼ��Ϣ��
		����
		2��Param2 - BYTE(8)   - ���ʹ�ô���
		3��Param3 - BYTE(8)   - ʣ��ʹ�ô���
		4��Param4 - SHORT(16) - �������
		5��Param5 - SHORT(16) - X����
		6��Param6 - SHORT(16) - Y����
		*/
		CObject_Item* pItem = (CObject_Item*)((CActionItem_Item*)m_pActionItem)->GetImpl();
		if( pItem && (pItem->GetItemExtraInfoItem()) )
		{

			state->PushInteger( pItem->GetItemExtraInfoItem()->GetItemParamValue( 0, IPV_SHORT ) );
			state->PushInteger( pItem->GetItemExtraInfoItem()->GetItemParamValue( 2, IPV_CHAR ) );
			state->PushInteger( pItem->GetItemExtraInfoItem()->GetItemParamValue( 3, IPV_CHAR ) );
			int nSceneID = pItem->GetItemExtraInfoItem()->GetItemParamValue( 4, IPV_SHORT );
			DBC_DEFINEHANDLE(s_pSceneDBC, DBC_SCENE_DEFINE);
			const _DBC_SCENE_DEFINE* pSceneDef = (const _DBC_SCENE_DEFINE*)s_pSceneDBC->Search_Index_EQU(nSceneID);
			if( pSceneDef )
				state->PushString( pSceneDef->szName );
			else
				state->PushString( "û���������" );
			state->PushInteger( pItem->GetItemExtraInfoItem()->GetItemParamValue( 6, IPV_SHORT ) );
			state->PushInteger( pItem->GetItemExtraInfoItem()->GetItemParamValue( 8, IPV_SHORT ) );
			return 6;
		}
		state->PushInteger( -1 );
		state->PushInteger( -1 );
		state->PushInteger( -1 );
		state->PushString( "" );
		state->PushInteger( -1 );
		state->PushInteger( -1 );
		return 6;

	}

	INT SuperTooltips::IsItemSuit(  LuaPlus::LuaState* state )
	{
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if(!m_pActionItem)
		{
			state->PushInteger( 0 );
			return 1;
		}

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		// ʹ�þ���ı�������λ��
		CObject_Item_Equip* pItem = (CObject_Item_Equip*)pActionItem_Item->GetItemImpl();
		if( pItem)
		{

			int nID = -1;
			switch( pItem->GetEquipQuantity() )
			{
			case CObject_Item_Equip::GREEN_EQUIP:
			case CObject_Item_Equip::PURPLE_EQUIP:
				nID = pItem->GetEquipBaseDefine().pDefineAsGreen->nSetID;
				break;
			case CObject_Item_Equip::BLUE_EQUIP:
				nID = pItem->GetEquipBaseDefine().pDefineAsBlue->nSetID;
				break;
			case CObject_Item_Equip::WHITE_EQUIP:
				nID = pItem->GetEquipBaseDefine().pDefineAsWhite->nSetID;
				break;
			default:
				break;
			}

			if( nID == -1 )
			{
				state->PushInteger( 1 );
			}
			else
			{
				state->PushInteger( 0 );
			}
		}
		else
			state->PushInteger( 0 );
		return 1;
	}

	// �õ���װ������
	INT SuperTooltips::GetItemSuitNumber( LuaPlus::LuaState* state )
	{
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if(!m_pActionItem)
		{
			state->PushInteger( 0 );
			return 1;
		}

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		// ʹ�þ���ı�������λ��
		CObject_Item_Equip* pItem = (CObject_Item_Equip*)pActionItem_Item->GetItemImpl();
		if( pItem )
		{

			int nID = -1;
			int nNumber = 0;
			switch( pItem->GetEquipQuantity() )
			{
			case CObject_Item_Equip::GREEN_EQUIP:
			case CObject_Item_Equip::PURPLE_EQUIP:
				nID = pItem->GetEquipBaseDefine().pDefineAsGreen->nSetID;
				nNumber = pItem->GetEquipBaseDefine().pDefineAsGreen->nSetNum;
				break;
			case CObject_Item_Equip::BLUE_EQUIP:
				nID = pItem->GetEquipBaseDefine().pDefineAsBlue->nSetID;
				nNumber = pItem->GetEquipBaseDefine().pDefineAsBlue->nSetNum;
				break;
			case CObject_Item_Equip::WHITE_EQUIP:
				nID = pItem->GetEquipBaseDefine().pDefineAsWhite->nSetID;
				nNumber = pItem->GetEquipBaseDefine().pDefineAsWhite->nSetNum;
				break;
			default:
				break;
			}

			if( nID == -1 ) // ���û����װ��Ϊ0
			{
				nNumber = 0;
			}
			state->PushInteger( nNumber );
		}
		else
			state->PushInteger( 0 );

		return 1;
	}

	// �ж��Լ��ǲ����еڼ�����װ
	INT SuperTooltips::IsItemExist( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if(!m_pActionItem)
		{
			state->PushInteger( 0 );
			state->PushString( "" );
			return 2;
		}

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		// ʹ�þ���ı�������λ��
		CObject_Item_Equip* pItem = (CObject_Item_Equip*)pActionItem_Item->GetItemImpl();
		if( pItem)
		{

			int nID = -1;
			int nNumber = 0;
			int nType = pItem->GetEquipQuantity();
			switch( nType )
			{
			case CObject_Item_Equip::PURPLE_EQUIP:
			case CObject_Item_Equip::GREEN_EQUIP:
				{
					nID = pItem->GetEquipBaseDefine().pDefineAsGreen->nSetID;
					nNumber = pItem->GetEquipBaseDefine().pDefineAsGreen->nSetNum;
					DBC_DEFINEHANDLE(s_pItem_GreenEquip, DBC_ITEM_EQUIP_GREEN);
					int nIndex = 0;
					for(INT i=0; i<(INT)s_pItem_GreenEquip->GetRecordsNum(); i++)
					{
						const _DBC_ITEM_EQUIP_GREEN* pGreenEquip = (const _DBC_ITEM_EQUIP_GREEN*)s_pItem_GreenEquip->Search_LineNum_EQU((UINT)i);

						if( pGreenEquip && pGreenEquip->nSetID == nID )
						{
							if( args[ 2 ].GetInteger() == nIndex ) // �������Ҫ�ҵ��Ǹ�
							{
								// �ж�������û�����װ��
								for( int iEquit = 0; iEquit < HEQUIP_NUMBER; iEquit ++ )
								{
									// ���id��ȣ���˵���Ѿ�����
									tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP(iEquit) );
									if( pItem )
									{
										if( pItem->GetIdTable() == pGreenEquip->nID )
										{

											state->PushInteger( 1 );
											state->PushString( pGreenEquip->szName );
											return 2;
										}
									}
								}
								state->PushInteger( 0 );
								state->PushString( pGreenEquip->szName );
								return 2;
							}
							nIndex ++;

						}
					}
					break;
				}
			case CObject_Item_Equip::BLUE_EQUIP:
				{
					nID = pItem->GetEquipBaseDefine().pDefineAsBlue->nSetID;
					nNumber = pItem->GetEquipBaseDefine().pDefineAsBlue->nSetNum;
					DBC_DEFINEHANDLE(s_pItem_BlueEquip, DBC_ITEM_EQUIP_BLUE);
					int nIndex = 0;
					for(INT i=0; i<(INT)s_pItem_BlueEquip->GetRecordsNum(); i++)
					{
						const _DBC_ITEM_EQUIP_BLUE* pGreenEquip = (const _DBC_ITEM_EQUIP_BLUE*)s_pItem_BlueEquip->Search_LineNum_EQU((UINT)i);

						if( pGreenEquip && pGreenEquip->nSetID == nID )
						{
							if( args[ 2 ].GetInteger() == nIndex ) // �������Ҫ�ҵ��Ǹ�
							{
								// �ж�������û�����װ��
								for( int iEquit = 0; iEquit < HEQUIP_NUMBER; iEquit ++ )
								{
									// ���id��ȣ���˵���Ѿ�����
									tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP(iEquit) );
									if( pItem )
									{
										if( pItem->GetIdTable() == pGreenEquip->nID )
										{
											// ��
											state->PushInteger( 1 );
											state->PushString( pGreenEquip->szName );
											return 2;
										}
									}
								}
								state->PushInteger( 0 );
								state->PushString( pGreenEquip->szName );
								return 2;
							}
							nIndex ++;
						}
					}
					break;
				}
				break;
			case CObject_Item_Equip::WHITE_EQUIP:
				{
					nID = pItem->GetEquipBaseDefine().pDefineAsWhite->nSetID;
					nNumber = pItem->GetEquipBaseDefine().pDefineAsWhite->nSetNum;
					DBC_DEFINEHANDLE(s_pItem_GreenEquip, DBC_ITEM_EQUIP_WHITE);
					int nIndex = 0;
					for(INT i=0; i<(INT)s_pItem_GreenEquip->GetRecordsNum(); i++)
					{
						const _DBC_ITEM_EQUIP_WHITE* pGreenEquip = (const _DBC_ITEM_EQUIP_WHITE*)s_pItem_GreenEquip->Search_LineNum_EQU((UINT)i);

						if( pGreenEquip && pGreenEquip->nSetID == nID )
						{
							if( args[ 2 ].GetInteger() == nIndex ) // �������Ҫ�ҵ��Ǹ�
							{
								// �ж�������û�����װ��
								for( int iEquit = 0; iEquit < HEQUIP_NUMBER; iEquit ++ )
								{
									// ���id��ȣ���˵���Ѿ�����
									tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP(iEquit) );
									if( pItem )
									{
										if( pItem->GetIdTable() == pGreenEquip->nID )
										{

											state->PushInteger( 1 );
											state->PushString( pGreenEquip->szName );
											return 2;
										}
									}
								}
								state->PushInteger( 0 );
								state->PushString( pGreenEquip->szName );
								return 2;
							}
							nIndex ++;

						}
					}
					break;
				}
			default:
				break;
			}
		}
		state->PushInteger( 0 );
		state->PushString( "" );
		return 2;
	}

	// �õ���װ������
	INT SuperTooltips::GetSuitAtt( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if(!m_pActionItem)
		{
			state->PushString( "" );
			return 1;
		}

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		// ʹ�þ���ı�������λ��
		CObject_Item_Equip* pItem = (CObject_Item_Equip*)pActionItem_Item->GetItemImpl();
		if( pItem)
		{
			state->PushString( pItem->GetSuitAttrInfo() );
		}
		else
			state->PushString( "" );
		return 1;
	}

	INT SuperTooltips::GetTwoMoneyValue(LuaPlus::LuaState* state)
	{
		/*
		��Ʊ��Ʒ�� TICKET_ITEM_INDEX
		1��Param1 - SHORT(16) - ��Ʊ��ǰ�ļ�ֵ
		2��Param2 - SHORT(16) - ��Ʊ���ļ�ֵ

		���̻�����Ʒ�ŷ�Χ 
		*/

		CObject_Item* pItem = (CObject_Item*)((CActionItem_Item*)m_pActionItem)->GetImpl();
		if( pItem && (pItem->GetItemExtraInfoItem()) && pItem->GetIdTable() == TICKET_ITEM_INDEX)	//��Ʊ
		{

			state->PushInteger( pItem->GetItemExtraInfoItem()->GetItemParamValue( TICKET_MERCHANDISE_ITEM_PARAM_BUY_PRICE_START, TICKET_MERCHANDISE_ITEM_PARAM_BUY_PRICE_TYPE ) );
			state->PushInteger( pItem->GetItemExtraInfoItem()->GetItemParamValue( TICKET_MERCHANDISE_ITEM_PARAM_SCENE_START, TICKET_MERCHANDISE_ITEM_PARAM_SCENE_TYPE ) );
			return 2;
		}

		if( pItem && (pItem->GetItemExtraInfoItem()) && ITEM_IDX_IS_TICKET_MERCHANDISE(pItem->GetIdTable()))		//���̻���
		{

			state->PushInteger( pItem->GetItemExtraInfoItem()->GetItemParamValue( TICKET_MERCHANDISE_ITEM_PARAM_BUY_PRICE_START, TICKET_MERCHANDISE_ITEM_PARAM_BUY_PRICE_TYPE ) );
			state->PushInteger( pItem->GetItemExtraInfoItem()->GetItemParamValue( TICKET_MERCHANDISE_ITEM_PARAM_SALE_PRICE_START, TICKET_MERCHANDISE_ITEM_PARAM_SALE_PRICE_TYPE ) );
			return 2;
		}
		state->PushInteger( -1 );
		state->PushInteger( -1 );
		return 2;
	}

	INT SuperTooltips::GetSysColour(LuaPlus::LuaState* state)
	{
		CObject_Item* pItem = (CObject_Item*)((CActionItem_Item*)m_pActionItem)->GetImpl();
		if(pItem)
		{
			state->PushString(pItem->GetSysColour());
			return 1;
		}
		state->PushString("");
		return 1;
	}

}