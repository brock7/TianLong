#include "stdafx.h"
/********************************************************************
	��������:	2005��11��11��
	����ʱ��:	16:35
	�ļ�����:	ItemRuler.cpp
	�ļ�·��:	d:\Prj\Server\Server\Item\ItemRuler.cpp
	
	�ļ����ܣ� ��Ʒ������ص�ά��
	�޸ļ�¼��
	
*********************************************************************/

#include "ItemRuler.h"
#include "ItemTable.h"
#include "Rand.h"
#include "GameDefine2.h"
#include "TimeManager.h"
#include "ItemHelper.h"
#include "GameTable.h"
#include "Obj_Human.h"
#include "Obj_Monster.h"
#include "ScanOperator_ActiveTeammates.h"
#include "Config.h"
#include "Scene.h"
#include "ItemBoxManager.h"
#include "LogDefine.h"





VOID	ItemBoxRuler::CreateItemFromMonsterDrop(
								 INT iMonsterValue,					//�����ֵ
								 MONSTER_DROPBOXS& mDrop,			//���������
								 INT iPlayerLvl,					//��Ҽ���
								 INT iMonsterLvl,					//���Ｖ��
								 FLOAT fControlValue,				//���ϵ��
								 ItemBoxContaner& OutBox,			//������Obj_ItemBox����
								 FLOAT fWallow)
{		

__ENTER_FUNCTION
	
	double		dBaseDropRate;					//����������
	double		dDeltaDropRate	=	1.0;		//����������
	double		dCurrentRate;					//��ǰ�����������
	FLOAT		fCurrentRate;					//��ǰ��Ʒѡȡ�����
	UINT		iCanDropItemNumber;				//���Ե������Ʒ��
	
	
	

	for(INT i =0;i<MAX_MONSTER_DROPBOX;i++)
	{
		if(mDrop.m_DropBox[i]!= -1)
		{
			DROP_BOX_TB* pTb = g_ItemTable.GetDropBoxTB(mDrop.m_DropBox[i]);
			if(pTb)
			{
				
				 dBaseDropRate	=	(double)iMonsterValue/(double)pTb->m_DropValue; 
				
				 DROP_ATT_TB*	pDropAtt = g_ItemTable.GetDropAttTB(iMonsterLvl-iPlayerLvl);
				 if(pDropAtt)
				 {
					 dDeltaDropRate	=	pDropAtt->m_AttValue;
				 }
				 
				 dBaseDropRate	=	dBaseDropRate*dDeltaDropRate*g_Config.m_ConfigInfo.m_DropParam*fWallow;	
				
				 dCurrentRate = ItemRander::DoubleRand() ;

				if(dCurrentRate<dBaseDropRate)
				{
					iCanDropItemNumber = 0;
					for(INT j = 0;j<MAX_DROPBOX_CARRAGE;j++)
					{
						if(pTb->m_DropItem[j].ToUINT() > 0 )
							iCanDropItemNumber++;
						else 
							j = MAX_DROPBOX_CARRAGE;

					}
					if(iCanDropItemNumber>0 )
					{
						fCurrentRate = (FLOAT)rand()/RAND_MAX;
						UINT iIndex = (UINT)(fCurrentRate*iCanDropItemNumber);
						
						Assert(iIndex<MAX_DROPBOX_CARRAGE);
						OutBox.AddItemType(pTb->m_DropItem[iIndex],pTb->m_Quality[iIndex]);

					}



				}


			}
		}
		else
			i = MAX_MONSTER_DROPBOX;
		

	}

	

__LEAVE_FUNCTION
}



BOOL		ItemCreateRuler::CreateEquipAttrib(ITEM_LOG_PARAM* pLogParam,_ITEM_TYPE itemType,INT iPlayerLvl,_ITEM& it,INT& quality)
{
	__ENTER_FUNCTION
	Assert(pLogParam);

	switch(itemType.m_Quality) 
	{
		case EQUALITY_NORMAL:
			{
				return CreateCommonEquipAttrib(itemType,iPlayerLvl,it);
					
			}
			break;
		case EQUALITY_BLUE:
			{
				//��װ����������	
				return CreateBlueEquipAttrib(pLogParam,itemType,iPlayerLvl,it,quality);
			}
			break;
		case EQUALITY_YELLOW:
			{

			}
			break;
		case EQUALITY_GREEN:
			{
				return CreateGreenEquipAttrib(itemType,iPlayerLvl,it);
			}
			break;
		default:
			{
				Assert(FALSE);
				return FALSE;
			}
	}
	Assert(FALSE);

	__LEAVE_FUNCTION

	return FALSE;
}






BOOL	ItemCreateRuler::CreateCommonEquipAttrib(_ITEM_TYPE itemType,INT iPlayerLvl,_ITEM& it)
{
	__ENTER_FUNCTION
	
		COMMON_EQUIP_TB* pGET	=	g_ItemTable.GetWhiteItemTB(itemType.ToSerial());
		if(pGET)
		{

			it.m_ItemIndex						=	pGET->m_IndexID;
			it.m_RulerID						=	(CHAR)pGET->m_RulerID;
			it.GetEquipData()->m_EquipPoint		=	pGET->m_EquipPoint;
			it.GetEquipData()->m_CurDurPoint	=	pGET->m_MaxDur;
			it.GetEquipData()->m_CurDamagePoint	=	0;
			it.GetEquipData()->m_MaxDurPoint	=	pGET->m_MaxDur;
			it.GetEquipData()->m_GemMax			=	pGET->m_MaxGem;
			it.GetEquipData()->m_FaileTimes		=	0;
			it.GetEquipData()->m_NeedLevel		=	pGET->m_ReqLevel;
			it.GetEquipData()->m_StoneCount		=	0;
			it.GetEquipData()->m_BasePrice		=	pGET->m_BasePrice;
			it.GetEquipData()->m_SetNum			=	pGET->m_EquipSetNum;	
			it.GetEquipData()->m_MaxNum			=	pGET->m_EquipSetMaxNum;

			INT iAttrCount = 0;
			_ITEM_ATTR	itemAttrib;

			//��װ��������
			if(pGET->m_AttackSpeed>0)
			{
				itemAttrib.m_AttrType		=	IATTRIBUTE_BASE_ATTACK_TIME;
				itemAttrib.m_Value.m_Value	=	pGET->m_AttackSpeed;
				it.GetEquipData()->m_pAttr[iAttrCount]		=	itemAttrib;
				iAttrCount++;
			}

			if(pGET->m_MagicAttack>0)
			{
				itemAttrib.m_AttrType						=	IATTRIBUTE_BASE_ATTACK_M;
				itemAttrib.m_Value.m_Value					=	pGET->m_MagicAttack;
				it.GetEquipData()->m_pAttr[iAttrCount]		=	itemAttrib;
				iAttrCount++;
			}

			if(pGET->m_MagicDefense>0)
			{
				itemAttrib.m_AttrType						=	IATTRIBUTE_BASE_DEFENCE_M;
				itemAttrib.m_Value.m_Value					=	pGET->m_MagicDefense;
				it.GetEquipData()->m_pAttr[iAttrCount]		=	itemAttrib;
				iAttrCount++;
			}

			if(pGET->m_PhysicAttack>0)
			{
				itemAttrib.m_AttrType						=	IATTRIBUTE_BASE_ATTACK_P;
				itemAttrib.m_Value.m_Value					=	pGET->m_PhysicAttack;
				it.GetEquipData()->m_pAttr[iAttrCount]		=	itemAttrib;
				iAttrCount++;
			}

			if(pGET->m_PhysicDefense>0)
			{
				itemAttrib.m_AttrType					=	IATTRIBUTE_BASE_DEFENCE_P;
				itemAttrib.m_Value.m_Value				=	pGET->m_PhysicDefense;
				it.GetEquipData()->m_pAttr[iAttrCount]	=	itemAttrib;
				iAttrCount++;
			}

			if(pGET->m_Miss >0)
			{
				itemAttrib.m_AttrType					=	IATTRIBUTE_MISS;
				itemAttrib.m_Value.m_Value				=	pGET->m_Miss;
				it.GetEquipData()->m_pAttr[iAttrCount]	=	itemAttrib;
				iAttrCount++;
			}

			it.GetEquipData()->m_AttrCount = iAttrCount;
			return TRUE;
		}
	
		Assert(FALSE);

	__LEAVE_FUNCTION

		return FALSE;
}




BOOL	ItemCreateRuler::CreateGreenEquipAttrib(_ITEM_TYPE itemType,INT iPlayerLvl,_ITEM& it)
{
	__ENTER_FUNCTION

	GREEN_EQUIP_TB* pGET =	g_ItemTable.GetGreenItemTB(itemType.ToSerial());

	if(pGET)
	{	it.m_ItemIndex						=	pGET->m_IndexID;
		it.m_RulerID						=	(CHAR)pGET->m_RulerID;
		it.GetEquipData()->m_EquipPoint		=	pGET->m_EquipPoint;
		it.GetEquipData()->m_CurDurPoint	=	pGET->m_MaxDur;
		it.GetEquipData()->m_CurDamagePoint	=	0;
		it.GetEquipData()->m_MaxDurPoint	=	pGET->m_MaxDur;
		it.GetEquipData()->m_GemMax			=	pGET->m_MaxGem;
		it.GetEquipData()->m_FaileTimes		=	0;
		it.GetEquipData()->m_NeedLevel		=	pGET->m_ReqLevel;
		it.GetEquipData()->m_StoneCount		=	0;
		it.GetEquipData()->m_BasePrice		=	pGET->m_BasePrice;
		it.GetEquipData()->m_SetNum			=	pGET->m_EquipSetNum;	
		it.GetEquipData()->m_MaxNum			=	pGET->m_EquipSetMaxNum;

		INT iAttrCount = 0;
		_ITEM_ATTR	itemAttrib;
		//��װ��������
		if(pGET->m_AttackSpeed>0)
		{
			itemAttrib.m_AttrType		=	IATTRIBUTE_BASE_ATTACK_TIME;
			itemAttrib.m_Value.m_Value	=	pGET->m_AttackSpeed;
			it.GetEquipData()->m_pAttr[iAttrCount]		=	itemAttrib;
			iAttrCount++;
		}
		
		if(pGET->m_MagicAttack>0)
		{
			itemAttrib.m_AttrType						=	IATTRIBUTE_BASE_ATTACK_M;
			itemAttrib.m_Value.m_Value					=	pGET->m_MagicAttack;
			it.GetEquipData()->m_pAttr[iAttrCount]		=	itemAttrib;
			iAttrCount++;
		}

		if(pGET->m_MagicDefense>0)
		{
			itemAttrib.m_AttrType						=	IATTRIBUTE_BASE_DEFENCE_M;
			itemAttrib.m_Value.m_Value					=	pGET->m_MagicDefense;
			it.GetEquipData()->m_pAttr[iAttrCount]		=	itemAttrib;
			iAttrCount++;
		}
		
		if(pGET->m_PhysicAttack>0)
		{
			itemAttrib.m_AttrType						=	IATTRIBUTE_BASE_ATTACK_P;
			itemAttrib.m_Value.m_Value					=	pGET->m_PhysicAttack;
			it.GetEquipData()->m_pAttr[iAttrCount]		=	itemAttrib;
			iAttrCount++;
		}
		
		if(pGET->m_PhysicDefense>0)
		{
			itemAttrib.m_AttrType					=	IATTRIBUTE_BASE_DEFENCE_P;
			itemAttrib.m_Value.m_Value				=	pGET->m_PhysicDefense;
			it.GetEquipData()->m_pAttr[iAttrCount]	=	itemAttrib;
			iAttrCount++;
		}
		
		if(pGET->m_Miss >0)
		{
			itemAttrib.m_AttrType					=	IATTRIBUTE_BASE_MISS;
			itemAttrib.m_Value.m_Value				=	pGET->m_Miss;
			it.GetEquipData()->m_pAttr[iAttrCount]	=	itemAttrib;
			iAttrCount++;
		}
		//������������
		
		for(INT i =0;i<MAX_GREEN_ITEM_EXT_ATTR;i++)
		{		
				if(pGET->m_Attr[i].m_Value.m_Value>0)
				{
					itemAttrib.m_AttrType						=		pGET->m_Attr[i].m_AttrType;
					itemAttrib.m_Value.m_Value					=		pGET->m_Attr[i].m_Value.m_Value;
					it.GetEquipData()->m_pAttr[iAttrCount]		=		itemAttrib;
					iAttrCount++;
				}
				
		}
		it.GetEquipData()->m_AttrCount = iAttrCount;
		return TRUE;

	}
	else
	{
		Assert(FALSE);  
	}
	
	__LEAVE_FUNCTION

	return FALSE;
}


BOOL	ItemCreateRuler::CreateBlueEquipAttrib(ITEM_LOG_PARAM* pLogParam,_ITEM_TYPE itemType,INT iPlayerLvl,_ITEM& it,INT& quality)
{
	__ENTER_FUNCTION

	Assert(pLogParam);
	BLUE_EQUIP_TB* pGET =	g_ItemTable.GetBlueItemTB(itemType.ToSerial());

	if(pGET)
	{
		it.m_ItemIndex						=	pGET->m_IndexID;
		it.m_RulerID						=	(CHAR)pGET->m_RulerID;
		it.GetEquipData()->m_EquipPoint		=	pGET->m_EquipPoint;
		it.GetEquipData()->m_CurDurPoint	=	pGET->m_MaxDur;
		it.GetEquipData()->m_CurDamagePoint	=	0;
		it.GetEquipData()->m_MaxDurPoint	=	pGET->m_MaxDur;
		/*
		 *	��ʯ���ɼ��ʣ�����д��
		 */
		switch(pGET->m_MaxGem) 
		{
		case 0:
			{
				it.GetEquipData()->m_GemMax = 0;
			}
			break;
		case 1:
			{
				if(ItemRander::DoubleRand()>0.8)
				{
					it.GetEquipData()->m_GemMax = 1;
				}
				else
				{
					it.GetEquipData()->m_GemMax = 0;
				}
			}
			break;
		case 2:
			{
				double dRand = ItemRander::DoubleRand();
				if(dRand>0.9)
				{
					it.GetEquipData()->m_GemMax = 2;
				}
				else if(dRand>0.6)
				{
					it.GetEquipData()->m_GemMax = 1;
				}
				else
				{
					it.GetEquipData()->m_GemMax = 0;
				}

			}
			break;
		case 3:
			{	
				double dRand = ItemRander::DoubleRand();
				
				if(dRand>0.96)
				{
					it.GetEquipData()->m_GemMax = 3;
					
				}
				else if(dRand>0.8)
				{
					it.GetEquipData()->m_GemMax = 2;
				}
				else if(dRand>0.4)
				{
					it.GetEquipData()->m_GemMax = 1;
				}
				else 
				{
					it.GetEquipData()->m_GemMax = 0;
				}

			}
			break;
		default:
			Assert(FALSE);
		}
		
		
		it.GetEquipData()->m_FaileTimes		=	0;
		it.GetEquipData()->m_NeedLevel		=	pGET->m_ReqLevel;
		it.GetEquipData()->m_StoneCount		=	0;
		it.GetEquipData()->m_BasePrice		=	pGET->m_BasePrice;
		it.GetEquipData()->m_SetNum			=	pGET->m_EquipSetNum;	
		it.GetEquipData()->m_MaxNum			=	pGET->m_EquipSetMaxNum;
		
		pLogParam->GemMax					=	it.GetEquipData()->m_GemMax;

		INT			iAttrCount = 0;
		_ITEM_ATTR	itemAttrib; //��װ��������
		
		if(pGET->m_AttackSpeed>0)
		{
			itemAttrib.m_AttrType		=	IATTRIBUTE_BASE_ATTACK_TIME;
			itemAttrib.m_Value.m_Value	=	pGET->m_AttackSpeed;
			it.GetEquipData()->m_pAttr[iAttrCount]		=	itemAttrib;
			iAttrCount++;
		}

		if(pGET->m_MagicAttack>0)
		{
			itemAttrib.m_AttrType						=	IATTRIBUTE_BASE_ATTACK_M;
			itemAttrib.m_Value.m_Value					=	pGET->m_MagicAttack;
			it.GetEquipData()->m_pAttr[iAttrCount]		=	itemAttrib;
			iAttrCount++;
		}

		if(pGET->m_MagicDefense>0)
		{
			itemAttrib.m_AttrType						=	IATTRIBUTE_BASE_DEFENCE_M;
			itemAttrib.m_Value.m_Value					=	pGET->m_MagicDefense;
			it.GetEquipData()->m_pAttr[iAttrCount]		=	itemAttrib;
			iAttrCount++;
		}

		if(pGET->m_PhysicAttack>0)
		{
			itemAttrib.m_AttrType						=	IATTRIBUTE_BASE_ATTACK_P;
			itemAttrib.m_Value.m_Value					=	pGET->m_PhysicAttack;
			it.GetEquipData()->m_pAttr[iAttrCount]		=	itemAttrib;
			iAttrCount++;
		}

		if(pGET->m_PhysicDefense>0)
		{
			itemAttrib.m_AttrType					=	IATTRIBUTE_BASE_DEFENCE_P;
			itemAttrib.m_Value.m_Value				=	pGET->m_PhysicDefense;
			it.GetEquipData()->m_pAttr[iAttrCount]	=	itemAttrib;
			iAttrCount++;
		}

		if(pGET->m_Miss >0)
		{
			itemAttrib.m_AttrType					=	IATTRIBUTE_BASE_MISS;
			itemAttrib.m_Value.m_Value				=	pGET->m_Miss;
			it.GetEquipData()->m_pAttr[iAttrCount]	=	itemAttrib;
			iAttrCount++;
		}
		BOOL	bHaveBlueAttr = FALSE;

		//����������ϣ�������Ʒquality ���� ItemQuality ��
		if(quality>49) quality = 49;
		if(quality<0) quality = 0;

		ITEM_QUALITY_TB*	pQualityTB	=		g_ItemTable.GetItemQualityTB(quality);	
		if(pQualityTB)
		{
			
			
				INT BlueAttrCount = 0;
				for(INT iAttrIndex = 0;iAttrIndex<MAX_BLUE_ITEM_EXT_ATTR;iAttrIndex++)
				{
					INT iRandValue = ItemRander::IntRand();
					INT iCurrentValue = 0;
					for(INT iAttr =0;iAttr<IATTRIBUTE_NUMBER;iAttr++)
					{	
						if(pGET->m_AttrRate[iAttr]!= -1)
						{
							if(pGET->m_AttrRate[iAttr]+iCurrentValue>=iRandValue)
							{
								bHaveBlueAttr = TRUE;
								//����ظ�����
								BOOL existSameAttr = FALSE;

								for(INT existAttr = 0 ;existAttr<iAttrCount;existAttr++)
								{
									if(iAttr == it.GetEquipData()->m_pAttr[existAttr].m_AttrType)
									{
										existSameAttr = TRUE;
										existAttr=iAttrCount;
									}
								}
								if(existSameAttr)
									continue;
								
								double fRandValue	=	ItemRander::DoubleRand();
								INT iQualityLevel = CaculateQualityIndex(fRandValue,pQualityTB);

								it.GetEquipData()->m_pAttr[iAttrCount].m_AttrType		=	iAttr;
								//����Ʒ�ʶδ�������

								MINMAX_VALUE	TValue =	g_ItemTable.GetItemValue((ITEM_ATTRIBUTE)iAttr,iQualityLevel);
								
								
								
								Assert(TValue.m_maxValue>=TValue.m_minValue);
								Assert(TValue.m_minValue!=-1);
								if(TValue.m_minValue<TValue.m_maxValue)
								{
									
									it.GetEquipData()->m_pAttr[iAttrCount].m_Value.m_Value	= TValue.m_minValue+rand()%(TValue.m_maxValue-TValue.m_minValue+1);	
								}
								else if(TValue.m_minValue == TValue.m_maxValue)	
								{
									it.GetEquipData()->m_pAttr[iAttrCount].m_Value.m_Value	= TValue.m_minValue;	
								}
								pLogParam->ItemAttr[BlueAttrCount] = it.GetEquipData()->m_pAttr[iAttrCount];
								BlueAttrCount++;
								iAttrCount++;
								break;
							}
							else
							{
								iCurrentValue+= pGET->m_AttrRate[iAttr];
							}
						}
					}
				}

				Assert(iAttrCount<MAX_ITEM_ATTR);
				it.GetEquipData()->m_AttrCount	=	iAttrCount;

				if(bHaveBlueAttr)
				{
					it.SetItemIdent(FALSE);
				}
				else
				    it.SetItemIdent(TRUE);
		}
		else
		{
			Assert(FALSE); //��װ��Ӧ����quality ����
		}

		return TRUE;
	}
	else
	{
		Assert(FALSE);//�Ƿ�itemType
	}
	
	return FALSE;
	
	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	ItemCreateRuler::CreateGemAttrib(_ITEM_TYPE itemType,_ITEM& it)
{
	__ENTER_FUNCTION
	
	GEMINFO_TB*	pGET		=		g_ItemTable.GetGemInfoTB( itemType.ToSerial());

	if(pGET)
	{
		it.m_ItemIndex						=	pGET->m_nTableIndex;
		it.GetGemData()->m_nPrice			=	pGET->m_nPrice;
		it.m_RulerID						=	(CHAR)pGET->m_nRulerID;
		
		it.GetGemData()->m_Attr.m_AttrType			=	pGET->m_GenAttr.m_AttrType;
		it.GetGemData()->m_Attr.m_Value.m_Value		=	pGET->m_GenAttr.m_Value.m_Value;

		return TRUE;
	}
	else
		Assert(FALSE);
	
	return FALSE;
	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	ItemCreateRuler::CreateGemAttrib(UINT TableIndex,_ITEM& it)
{
	__ENTER_FUNCTION

	GEMINFO_TB*	pGET		=		g_ItemTable.GetGemInfoTB( TableIndex);

	if(pGET)
	{

		it.m_ItemIndex						=	pGET->m_nTableIndex;
		it.GetGemData()->m_nPrice			=	pGET->m_nPrice;
		it.m_RulerID						=	(CHAR)pGET->m_nRulerID;

		it.GetGemData()->m_Attr.m_AttrType			=	pGET->m_GenAttr.m_AttrType;
		it.GetGemData()->m_Attr.m_Value.m_Value		=	pGET->m_GenAttr.m_Value.m_Value;

		return TRUE;
	}
	else
		Assert(FALSE);

	return FALSE;

	__LEAVE_FUNCTION

		return FALSE;
}




BOOL	ItemCreateRuler::CreateMaterialAttrib(_ITEM_TYPE itemType,_ITEM& it)
{
	__ENTER_FUNCTION

		Assert(FALSE);

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	ItemCreateRuler::CreateMedicAttrib(_ITEM_TYPE itemType,_ITEM& it)
{
	__ENTER_FUNCTION

	COMMITEM_INFO_TB*	pGET		=		g_ItemTable.GetCommItemInfoTB( itemType.ToSerial());

	if(pGET)
	{
				
				it.m_ItemIndex						=	pGET->m_nTableIndex;
				
				it.GetMedicData()->m_nLevel			=	pGET->m_nLevel;	
				it.GetMedicData()->m_nBasePrice		=	pGET->m_nBasePrice;
				it.m_RulerID						=	(CHAR)pGET->m_nRulerID;	
				it.GetMedicData()->m_nLayedNum		=	pGET->m_nLayedNum;	
				it.GetMedicData()->m_nScriptID		=	pGET->m_nScriptID;	
				it.GetMedicData()->m_nSkillID		=	pGET->m_nSkillID;	
				it.GetMedicData()->m_nCount			=	1; //������Ʒ������һ��
				it.GetMedicData()->m_bCosSelf		=	pGET->m_bCosSelf;
				it.GetMedicData()->m_nReqSkill		=	pGET->m_nReqSkill;
				it.GetMedicData()->m_nReqSkillLevel	=	pGET->m_nReqSkillLevel;
				it.GetMedicData()->m_TargetType		=	pGET->m_TargetType;
				return TRUE;
	}
	else
		Assert(FALSE);
	return FALSE;
	
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL	ItemCreateRuler::CreateStoreMapAttrib(_ITEM_TYPE itemType,_ITEM& it)
{
	__ENTER_FUNCTION

	STORE_MAP_INFO_TB*	pGET		=		g_ItemTable.GetStoreMapTB(itemType.ToSerial());

	if(pGET)
	{

		it.m_ItemIndex						=	pGET->m_nTableIndex;
		//it.m_ItemType						=	pGET->m_ItemType;
		it.GetStoreMapData()->m_nLevel		=	pGET->m_nLevel;	
		it.m_RulerID						=	(CHAR)pGET->m_nRulerID;	
		it.GetStoreMapData()->m_xPos		=	pGET->m_xPos;
		it.GetStoreMapData()->m_zPos		=	pGET->m_zPos;
		it.GetStoreMapData()->m_SceneID		=	pGET->m_SceneID;
		it.GetStoreMapData()->m_GrowType	=	pGET->m_GrowPointType;
		
		return TRUE;
	}
	else
		Assert(FALSE);
	return FALSE;

	__LEAVE_FUNCTION
		return FALSE;
}

BOOL	ItemCreateRuler::CreateQuestItemAttrib(_ITEM_TYPE itemType,_ITEM& it)
{
	__ENTER_FUNCTION

	return	FALSE;

	__LEAVE_FUNCTION

	return	FALSE;
}




INT	ItemCreateRuler::CaculateQualityIndex(double fRandValue,ITEM_QUALITY_TB* pTB)
{
	__ENTER_FUNCTION
		
#define		MAX_QUALITY_RAND_INT	1000000

		INT LargeInt	=	(INT)(fRandValue*MAX_QUALITY_RAND_INT);
		
		INT SumInt		=	0;

		for(INT i = 0;i<MAX_ITEM_TABLE_QUALITYS;++i)
		{
			if(pTB->m_Quality[i]>0)
			{
				if(SumInt+pTB->m_Quality[i]>=LargeInt)
					return i;
				else
					SumInt	+=	pTB->m_Quality[i];
			}
			else
				continue;
		}
		return	-1;

	__LEAVE_FUNCTION
}

UINT	MonsterExpCaculateRuler::CaculateBaseExp(INT iMonsterLevel,INT iKillerLevel,UINT MonsterBaseExp)
{

	__ENTER_FUNCTION
	
		return (UINT)(MonsterBaseExp*g_ExpAttTbl.Get(iMonsterLevel-iKillerLevel));
	
	__LEAVE_FUNCTION

		return 0 ;
}

BOX_DISTRIBUTE_RULER	
MonsterDropRuler::GetMonsterDropRuler(Obj_Monster* pMonster)
{
	__ENTER_FUNCTION

		Assert(pMonster);
		
		INT	 iDataID		=	pMonster->GetDataID();
        
        
		MONSTER_DROPBOX_TB*	pDropBoxs = g_ItemTable.GetMonsterDropTB(iDataID);
		if(pDropBoxs)
		{
			return (BOX_DISTRIBUTE_RULER)pDropBoxs->m_DropType;
		}

		return BDR_UNKNOW;

	__LEAVE_FUNCTION

		return BDR_UNKNOW;

		
}


BOOL	MonsterDropRuler::CaculateCommOwnerList(Obj_Monster* pMonster)
{
	__ENTER_FUNCTION

		if(!pMonster)	
		{
			Assert(pMonster);
			return FALSE;
		}
		Scene*	pScene = pMonster->getScene();
		if(!pScene)
		{
			Assert(pScene);
			return FALSE;
		}
		


		Obj_Human*	 pFinalOwner	= 0;
		//������Ч�Ŷ�
		
		FLOAT	fSearchLength	=	pMonster->GetDropSearchRange();
		INT		uMaxTeamCount	=	pMonster->GetDropTeamCount();
		INT		nPercent		=	pMonster->GetMinDamagePercent();

		//��ͨ����Զֻѡһ������
		DAMAGE_MEM_LIST	 DRecord = pMonster->GetKillerRec(1,nPercent);

		BOOL	bFindTeam		=	FALSE;
		UINT	iCurrentDTIndex	=	0;
		
		Assert(DRecord.m_uCount<MAX_DAMAGE_REC_COUNT);

		for(UINT i = 0;i<DRecord.m_uCount;i++)
		{
			//���Ҽ���ĳ�Ա
			SCANOPERATOR_ACTIVETEAMMATES_INIT	SA_Init;
			//SA_Init.m_TeamID		=	DRecord.m_DamageRec[iCurrentDTIndex].m_TeamID;
			//SA_Init.m_MemberObjID	=	DRecord.m_DamageRec[iCurrentDTIndex].m_Killer;
			SA_Init.m_TeamID		=	pMonster->GetOccupantTeamID() ;
			SA_Init.m_MemberGUID	=	pMonster->GetOccupantGUID() ;
			SA_Init.m_fRadius		=	fSearchLength;
			SA_Init.m_nZoneRadius	=	3;
			SA_Init.m_bScanHuman	=	TRUE;
			SA_Init.m_Position		=	*pMonster->getWorldPos();
			SA_Init.m_ZoneID		=	pMonster->getZoneID();
			SA_Init.m_pScene		=	pMonster->getScene();
			ScanOperator_ActiveTeammates	ScanOpAT;
			ScanOpAT.Init(&SA_Init);
			pScene->Scan(&ScanOpAT);

			if(ScanOpAT.m_nActiveTeammateCount>1) //������
			{
				pFinalOwner	=	
					ScanOpAT.m_aActiveTeammate[rand()%ScanOpAT.m_nActiveTeammateCount];
				break;
			}
			else if(ScanOpAT.m_nActiveTeammateCount==1) //ֻ��һ����
			{
				pFinalOwner	= ScanOpAT.m_aActiveTeammate[0];
				//if(pFinalOwner->IsInValidRadius(pMonster->getWorldPos(),fSearchLength) == FALSE)
				//{
				//	pFinalOwner = NULL;
				//	continue; //û���ҵ�������
				//}
				break;
			}
			else	//û���ҵ�
			{
				//iCurrentDTIndex++;
				//continue;
				break ;
			}
		}
	

		if(!pFinalOwner)
			return FALSE;

		pMonster->GetOwnerList().AddOwner(pFinalOwner->GetID());

		return TRUE;

	__LEAVE_FUNCTION
		
		return FALSE;
}

BOOL	MonsterDropRuler::CaculateBossOwnerList(Obj_Monster* pMonster)
{
	__ENTER_FUNCTION

		if(!pMonster)	
		{
			Assert(pMonster);
			return FALSE;
		}
		Scene*	pScene = pMonster->getScene();
		if(!pScene)
		{
			Assert(pScene);
			return FALSE;
		}
		Obj_Human*	 pFinalOwner;
		//������Ч�Ŷ�
		BOOL	bFindTeam			=	FALSE;
		UINT	iCurrentDTIndex		=	0;

		UINT	uMaxTeamCount		=  pMonster->GetDropTeamCount();
		INT		nPercent			=  pMonster->GetMinDamagePercent();
		FLOAT	fSearchLength		=  pMonster->GetDropSearchRange();
		

		//Boss�ֿ���ѡ��������
		DAMAGE_MEM_LIST	 DRecord	= 
			pMonster->GetKillerRec(uMaxTeamCount,nPercent);

		if(DRecord.m_uCount==0) //û���ҵ�����
			return FALSE;

		UINT uTeamCount			=	DRecord.m_uCount;//����ʰȡ�Ķ������
		
		Assert(uTeamCount<MAX_DAMAGE_REC_COUNT);

		ScanOperator_ActiveTeammates	ScanOpAT;
		
		for(UINT i =0;i<uTeamCount;i++) //����ÿ���Ϸ����飬���Ҵ�����Ӧ�����
		{
			
			//���Ҽ���ĳ�Ա
			SCANOPERATOR_ACTIVETEAMMATES_INIT	SA_Init;
			//SA_Init.m_TeamID		=	DRecord.m_DamageRec[iCurrentDTIndex].m_TeamID;
			//SA_Init.m_MemberObjID	=	DRecord.m_DamageRec[iCurrentDTIndex].m_Killer;
			SA_Init.m_TeamID		=	pMonster->GetOccupantTeamID() ;
			SA_Init.m_MemberGUID	=	pMonster->GetOccupantGUID() ;
			SA_Init.m_fRadius		=	fSearchLength;
			SA_Init.m_nZoneRadius	=	5;
			SA_Init.m_bScanHuman	=	TRUE;
			SA_Init.m_Position		=	*pMonster->getWorldPos();
			SA_Init.m_ZoneID		=	pMonster->getZoneID();
			SA_Init.m_pScene		=	pMonster->getScene();

			ScanOpAT.Init(&SA_Init);
			pScene->Scan(&ScanOpAT);

			iCurrentDTIndex++;

			if(ScanOpAT.m_nActiveTeammateCount<1) //û���ҵ�
			{
				break ;
//				continue; 
			}
			else if(ScanOpAT.m_nActiveTeammateCount==1)//�ҵ�һ��
			{
				pFinalOwner	=	
					ScanOpAT.m_aActiveTeammate[0];
				//if(pFinalOwner->IsInValidRadius(pMonster->getWorldPos(),fSearchLength) == FALSE)
				//{
				//	continue;
				//}
				pMonster->GetOwnerList().AddOwner(pFinalOwner->GetID());
				break ;
			}
			else//�ҵ����
			{
				INT iActivePlayerCount  =  ScanOpAT.m_nActiveTeammateCount;
				for(UINT iDropBox =0;iDropBox<(UINT)iActivePlayerCount;iDropBox++)
				{
					pFinalOwner		= ScanOpAT.m_aActiveTeammate[iDropBox];
					if(pFinalOwner)
					{
						pMonster->GetOwnerList().AddOwner(pFinalOwner->GetID());
					}
				}
				break ;
			}
		}

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

#define MD_PICK_RANGE		(100.0f)
BOOL	MonsterDropRuler::CaculateCommDropRuler(Obj_Monster* pMonster)
{
	__ENTER_FUNCTION
		
		if(!pMonster)
		{
			Assert(FALSE);
			return FALSE;
		}
		
		Scene*	pScene = pMonster->getScene();
		if(!pScene)
		{
			Assert(pScene);
			return FALSE;
		}

		Obj_Human*	 pFinalOwner	= 0;

		if(pMonster->GetOwnerList().OwnerCount<1)
		{
			return FALSE;
		}
		INT randown = rand()%pMonster->GetOwnerList().OwnerCount ;
		
		ObjID_t	OwnerID = pMonster->GetOwnerList().OwnerDropList[randown].HumanID;
		
		CHAR_OWNER_DROP_LIST  TaskDropList = 
			pMonster->GetOwnerList().OwnerDropList[randown];

		pFinalOwner	=	pScene->GetHumanManager()->GetHuman(OwnerID);

		if(!pFinalOwner)
		{
			Assert(FALSE);
			return FALSE;
		}
		
		ItemBoxManager*	pIBManager = pScene->GetItemBoxManager();
		if(!pIBManager)
		{
			Assert(pIBManager);
			return FALSE;
		}
		
	
		UINT	iMonsterLevel	=	pMonster->GetLevel();
		UINT	iKillerLevel	=	pFinalOwner->GetLevel();
		INT		iDataID	        =	pMonster->GetDataID();
		FLOAT	fWallow			=	pFinalOwner->GetWallow();

	
		
		ItemBoxContaner	IBContaner = 
			pIBManager->CaculateItemDropFromMonster(iMonsterLevel,
			iKillerLevel,iDataID,fWallow);
		
		
		WORLD_POS	Pos	=	*pMonster->getWorldPos();

		if((IBContaner.m_nCount<=0) && (TaskDropList.DropCount==0)) //�ж��Ƿ������Ʒ
		{
			pMonster->GetOwnerList().CleanUp();
			return FALSE;
		}


		Obj_ItemBox* pItemBox = 
			pIBManager->CreateMonsterDropItembox(&Pos);	//����ItemBox;

		
		for(INT i=0;i<IBContaner.m_nCount;i++)
		{
			pItemBox->AddItem(&IBContaner,i);
		}
	

		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType		= ITEM_CREATE_FROM_MONSTER;
		ItemLogParam.SceneID	= pScene->SceneID();
		ItemLogParam.NpcType	= iDataID;
		ItemLogParam.XPos		= Pos.m_fX;
		ItemLogParam.ZPos		= Pos.m_fZ;

		for(UINT i =0;i<TaskDropList.DropCount;i++)
		{
			INT iQuality = 1;
			pItemBox->CreateItem(&ItemLogParam,TaskDropList.DropItemIndex[i],iQuality);
			SaveItemLog(&ItemLogParam);
		}
		

		pMonster->GetOwnerList().CleanUp();
		
		pItemBox->SetOwner(pFinalOwner->GetGUID());
		pItemBox->SetDropMonsterID(pMonster->GetID());
		pItemBox->SetActiveFlag(TRUE);
	
		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	MonsterDropRuler::CaculateBossDropRuler(Obj_Monster*	pMonster, BOX_DISTRIBUTE_RULER DropRuler)
{
	__ENTER_FUNCTION

	Assert(pMonster);

	Scene*	pScene = pMonster->getScene();
	Assert(pScene);

	ItemBoxManager*	pIBManager = pScene->GetItemBoxManager();
	Assert(pIBManager);
	
	UINT	randomindex=0 ;
	UINT	OwnerCount =	pMonster->GetOwnerList().OwnerCount;
	if( OwnerCount==0 )
		return TRUE ;
	
	if( DropRuler!=BDR_BOSS )
	{
		randomindex = rand()%OwnerCount ;
	}
		

	for(UINT iDropBox =0;iDropBox<(UINT)OwnerCount;iDropBox++)
	{
			
			ObjID_t	OwnerID			=   
				pMonster->GetOwnerList().OwnerDropList[iDropBox].HumanID;
			UINT	OwnerItemCount	=
				pMonster->GetOwnerList().OwnerDropList[iDropBox].DropCount;
			CHAR_OWNER_DROP_LIST	TaskDropList	=
				pMonster->GetOwnerList().OwnerDropList[iDropBox];

			Obj_Human*	 pFinalOwner	= 
				pScene->GetHumanManager()->GetHuman(OwnerID);

			if(!pFinalOwner)
			{
				continue;
			}

			UINT	iMonsterLevel	=	pMonster->GetLevel();
			UINT	iKillerLevel	=	pFinalOwner->GetLevel();
			INT		iDataID	        =	pMonster->GetDataID();
			FLOAT	fWallow			=	pFinalOwner->GetWallow();


			ItemBoxContaner	IBContaner = 
				pIBManager->CaculateItemDropFromMonster(iMonsterLevel,
				iKillerLevel,iDataID,fWallow);

			WORLD_POS	Pos	=	*pMonster->getWorldPos();

			if((IBContaner.m_nCount<=0) && (TaskDropList.DropCount==0)) //�ж��Ƿ������Ʒ
				continue;

			if( DropRuler!=BDR_BOSS && randomindex!=iDropBox && TaskDropList.DropCount==0 )
			{
				continue ;
			}

			//�����Ҷ�Ӧ��ItemBox
			Obj_ItemBox* pItemBox = pIBManager->CreateMonsterDropItembox(&Pos);	//����ItemBox;
		
		

			if( DropRuler==BDR_BOSS )
			{//�����boss���͵ģ��򶼻�����
				for(int i=0;i<IBContaner.m_nCount;i++)
				{
					pItemBox->AddItem(&IBContaner,i);
				}
			}
			else if( randomindex==iDropBox )
			{//�������boss�͹֣���ֻ��һ���ܱ�����
				for(int i=0;i<IBContaner.m_nCount;i++)
				{
					pItemBox->AddItem(&IBContaner,i);
				}
			}
			

			

			ITEM_LOG_PARAM	ItemLogParam;
			ItemLogParam.OpType		= ITEM_CREATE_FROM_MONSTER;
			ItemLogParam.SceneID	= pScene->SceneID();
			ItemLogParam.NpcType	= iDataID;
			ItemLogParam.XPos		= Pos.m_fX;
			ItemLogParam.ZPos		= Pos.m_fZ;
			for(UINT i=0;i<TaskDropList.DropCount;i++)
			{
				INT iQuality = 1;
				pItemBox->CreateItem(&ItemLogParam,TaskDropList.DropItemIndex[i],iQuality);
				SaveItemLog(&ItemLogParam);
			}
		
		
			//pScene->ObjectEnterScene(pItemBox);
			pItemBox->SetOwner(pFinalOwner->GetGUID());
			pItemBox->SetDropMonsterID(pMonster->GetID());
			pItemBox->SetActiveFlag(TRUE);
	}
	
	pMonster->GetOwnerList().CleanUp();

	return TRUE;

	__LEAVE_FUNCTION

	return	FALSE;
}


BOOL	ItemRulerCheck::CheckTypeRuler(ITEM_RULER_LIST ruler,const UINT uItemIndex)
{
	__ENTER_FUNCTION

	Assert(uItemIndex!=0);
	Assert(uItemIndex!=INVALID_ID);
	
	UCHAR	uClass		= GetSerialClass(uItemIndex);
	UCHAR	uQuality	= GetSerialQual(uItemIndex);

	switch(uClass)
	{
	case ICLASS_EQUIP:
		{
			switch(uQuality) 
			{
			case EQUALITY_NORMAL:
				{
					COMMON_EQUIP_TB* pGET	=	g_ItemTable.GetWhiteItemTB(uItemIndex);
					if(!pGET)
					{
						CHAR Buff[255];
						memset(Buff,0,255);
						sprintf(Buff,"�Ҳ�����Ӧ��Ʒ�ı���壬��ȷ����װ�����ȷ ItemType=%d",uItemIndex);
						AssertEx(pGET,Buff);
					}
					return CheckRuler(ruler,pGET->m_RulerID);
				}
				break;
			case EQUALITY_BLUE:
				{
					
					BLUE_EQUIP_TB* pGET =	g_ItemTable.GetBlueItemTB(uItemIndex);
					if(!pGET)
					{
						CHAR Buff[255];
						memset(Buff,0,255);
						sprintf(Buff,"�Ҳ�����Ӧ��Ʒ�ı���壬��ȷ����װ��Դ��ȷ ItemType=%d",uItemIndex);
						AssertEx(pGET,Buff);
					}
					return CheckRuler(ruler,pGET->m_RulerID);
				}
				break;
			case EQUALITY_GREEN:
				{
					GREEN_EQUIP_TB* pGET =	g_ItemTable.GetGreenItemTB(uItemIndex);
					if(!pGET)
					{
						CHAR Buff[255];
						memset(Buff,0,255);
						sprintf(Buff,"�Ҳ�����Ӧ��Ʒ�ı���壬��ȷ����װ��Դ��ȷ ItemType=%d",uItemIndex);
						AssertEx(pGET,Buff);
					}
					return CheckRuler(ruler,pGET->m_RulerID);
				}
				break;
			default:
				{
					AssertEx(FALSE,"�Ҳ�����Ӧ��Ʒ����");
					return FALSE;
				}
			}
			Assert(FALSE);
		}
		break;
	case ICLASS_MATERIAL: 
	case ICLASS_COMITEM:
	case ICLASS_TASKITEM: 
		{
			COMMITEM_INFO_TB*	pGET		=		g_ItemTable.GetCommItemInfoTB(uItemIndex);
			if(!pGET)
			{
				CHAR Buff[255];
				memset(Buff,0,255);
				sprintf(Buff,"�Ҳ�����Ӧ��Ʒ�ı���壬��ȷ��CommonItem��Դ��ȷ ItemType=%d",uItemIndex);
				AssertEx(pGET,Buff);
			}

			return CheckRuler(ruler,pGET->m_nRulerID);
		}
		break;
	case ICLASS_GEM:
		{
			GEMINFO_TB*	pGET		=		g_ItemTable.GetGemInfoTB(uItemIndex);
			if(!pGET)
			{
				CHAR Buff[255];
				memset(Buff,0,255);
				sprintf(Buff,"�Ҳ�����Ӧ��Ʒ�ı���壬��ȷ��Gem��Դ��ȷ ItemType=%d",uItemIndex);
				AssertEx(pGET,Buff);
			}
			return CheckRuler(ruler,pGET->m_nRulerID);
		}
		break;
	case ICLASS_STOREMAP:
		{
			STORE_MAP_INFO_TB*	pGET		=		g_ItemTable.GetStoreMapTB(uItemIndex);
			if(!pGET)
			{
				CHAR Buff[255];
				memset(Buff,0,255);
				sprintf(Buff,"�Ҳ�����Ӧ��Ʒ�ı���壬��ȷ���ر�ͼ��Դ��ȷ ItemType=%d",uItemIndex);
				AssertEx(pGET,Buff);
			}
			return CheckRuler(ruler,pGET->m_nRulerID);
			break;
		}
	case ICLASS_TALISMAN: 
		Assert(FALSE);
		break;
	case ICLASS_GUILDITEM: 
		Assert(FALSE);
		break;
	default:
		Assert(FALSE);
		break;
	}
	Assert(FALSE);
	return FALSE;
	__LEAVE_FUNCTION

	return FALSE;
}

