#include "StdAfx.h"
#include "Obj_Item_Gem.h"
#include "TDException.h"
#include "../../Procedure/GameProcedure.h"


const char* g_szGemAttName[] = 
{
	"equip_attr_maxhp",								//0   350
	"equip_attr_maxhp",								//1	  350
	"equip_attr_restorehp",							//2	  351
	"equip_attr_maxmp",								//3	  353
	"equip_attr_maxmp",								//4	  353
	"equip_attr_restoremp",							//5	  354
	"equip_attr_attack_cold",						//6	  356
	"equip_attr_resist_cold",						//7	  357
	"equip_attr_time_chihuan",						//8	  358
	"equip_attr_attack_fire",						//9	  359
	"equip_attr_resist_fire",						//10  360
	"equip_attr_time_ruohua",						//11  361
	"equip_attr_attack_light",						//12  362
	"equip_attr_resist_light",						//13  363
	"equip_attr_time_xuanyun",						//14  364
	"equip_attr_attack_poison",						//15  365
	"equip_attr_resist_poison",						//16  366
	"equip_attr_time_zhongdu",						//17  367
	"equip_attr_resist_all",						//18  368
	"equip_attr_attack_p",							//19  371
	"equip_attr_attack_p",							//20  371
	"equip_attr_attack_ep",							//21  372
	"equip_attr_defence_p",							//22  377
	"equip_attr_defence_p",							//23  377
	"equip_attr_defence_ep",						//24  378
	"equip_attr_immunity_p",						//25  383
	"equip_attr_attack_m",							//26  374
	"equip_attr_attack_m",							//27  374
	"equip_attr_attack_em",							//28  375
	"equip_attr_defence_m",							//29  380
	"equip_attr_defence_m",							//30  380
	"equip_attr_defence_em",						//31  381
	"equip_attr_immunity_m",						//32  384
	"equip_attr_attackspeed",						//33  386
	"equip_attr_cooldowm",							//34  387
	"equip_attr_hit",								//35  389
	"equip_attr_miss",								//36  390
	"equip_attr_2attack",							//37  391
	"equip_attr_nodefence",							//38  392
	"equip_attr_movespeed",							//39  388
	"equip_attr_damageret",							//40  393
	"equip_attr_damage2mp",							//41  394
	"equip_attr_str",								//42  395
	"equip_attr_spr",								//43  396
	"equip_attr_con",								//44  397
	"equip_attr_int",								//45  398
	"equip_attr_dex",								//46  399
	"equip_yuliu",									//47  499
	"equip_attr_all",								//48  401
	"equip_attr_hp_thieve",							//49  404
	"equip_attr_mop_thieve",						//50  405
	"equip_yuliu",									//51  499
	"equip_yuliu",									//52  499
	"equip_yuliu",									//53  499
	"equip_base_attack_p",							//54  330
	"equip_yuliu",									//55  499
	"equip_base_attack_m",							//56  332
	"equip_yuliu",									//57  499
	"equip_yuliu",									//58  499
	"equip_yuliu",									//69  499
	"equip_base_defence_p",							//60  334
	"equip_yuliu",									//61  499
	"equip_base_defence_m",							//62  336
	"equip_yuliu",									//63  499
	"equip_base_miss",								//64  338
	"equip_yuliu",									//65  499

};
//
//
//
//const char* g_szGemAttName[] = 
//{
//	"����HP������",						   //0 				
//	"�ٷֱ�����HP������",				   //1
//	"�ӿ�HP�Ļظ��ٶ�",					   //2
//	"����MP������",						   //3
//	"�ٷֱ�����MP������",				   //4
//	"�ӿ�MP�Ļظ��ٶ�",					   //5
//	"������",							   //6
//	"���ֿ�",							   //7
//	"���ٱ����ٻ�ʱ��",					   //8
//	"�𹥻�",							   //9
//	"��ֿ�",							   //10
//	"���ٻ��ճ���ʱ��",					   //11
//	"�繥��",							   //12
//	"��ֿ�",							   //13
//	"���ٵ��ѣ��ʱ��",					   //14
//	"������",							   //15
//	"���ֿ�",							   //16
//	"�����ж�ʱ��",						   //17
//	"���ٷֱȵ����������Թ���",			   //18
//	"������",							   //19
//	"���ٷֱ�����������",				   //20
//	"��װ�������������ٷֱȼӳ�",		   //21
//	"�������",							   //22
//	"���ٷֱ������������",				   //23
//	"��װ��������������ٷֱȼӳ�",		   //24
//	"���ٷֱȵ��������˺�",				   //25
//	"ħ������",							   //26
//	"���ٷֱ�����ħ������",				   //27
//	"��װ������ħ�������ٷֱȼӳ�",		   //28
//	"ħ������",							   //29
//	"���ٷֱ�����ħ������",				   //30
//	"��װ������ħ�������ٷֱȼӳ�",		   //31
//	"���ٷֱȵ���ħ���˺�",				   //32
//	"�����ٶ�(���ι������ʱ��)",		   //33
//	"ħ����ȴ�ٶ�",						   //34
//	"����",								   //35
//	"����",								   //36
//	"����һ����˫���������İٷֱ�",		   //37
//	"���ӶԷ���������",					   //38
//	"�ƶ��ٶȰٷֱ�",					   //39
//	"�˺�����",							   //40
//	"�˺�����������",					   //41
//	"����",								   //42
//	"����",								   //43
//	"����",								   //44
//	"����",								   //45
//	"��",								   //46
//	"����",								   //47
//	"�������е�����һ������",			   //48
//	"����͵ȡ",							   //49
//	"����͵ȡ",							   //50
//	"����ĳ�����ܵȼ�",					   //51
//	"�������м��ܵȼ�",					   //52
//	"���⼼�ܷ�������",					   //53
//	"����������",						   //54
//	"����ħ������",						   //55
//	"���������ٶ�",						   //56
//	"�����������",						   //57
//	"����ħ������",						   //58
//	"��������",							   //59
//};//	

CObject_Item_Gem::CObject_Item_Gem(INT id)   
		: CObject_Item(id)				  
{
	m_theBaseDef = NULL;
//	m_HostOfAppend = NULL;
}

CObject_Item_Gem::~CObject_Item_Gem()
{
}

VOID CObject_Item_Gem::AsGem(const _DBC_ITEM_GEM* pGemDefine)
{
	TDAssert(pGemDefine);

	m_theBaseDef = pGemDefine;
	m_nParticularID = ( ( ( ( ( m_theBaseDef->nClass * 100 ) + m_theBaseDef->nQuality ) * 100 ) + m_theBaseDef->nType ) * 1000 ) + m_theBaseDef->nGemIndex;


}

LPCTSTR  CObject_Item_Gem::GetIconName(VOID) const
{
	return m_theBaseDef->szIcon;
}

//������ϸ����
VOID CObject_Item_Gem::SetExtraInfo(const _ITEM * pItemInfo)
{
	TDAssert(pItemInfo);

	//���û��ຯ��������_ITEM�ṹ
	CObject_Item::SetExtraInfo(pItemInfo);

	SetNumber(pItemInfo->GetItemCount());
	SetManufacturer(pItemInfo);
}

LPCTSTR  CObject_Item_Gem::GetExtraDesc(VOID)
{
	//CHAR szTemp[1024];

	//_snprintf(szTemp, 1024, 
	//	"ID=%d\n"
	//	"����: %s\n"
	//	"-------------------\n"
	//	"%s",
	//	m_ID.m_idOrg.m_uSerial, 
	//	m_theBaseDef->szName,
	//	m_theBaseDef->szDesc);//

	m_strExtraDesc = m_theBaseDef->szDesc;
	return m_strExtraDesc.c_str();
}

VOID	CObject_Item_Gem::Clone(const CObject_Item * pItemSource)
{
	SetNumber(((CObject_Item_Gem*)pItemSource)->GetNumber());
	CObject_Item::Clone(pItemSource);
}

// �õ���ʯ�ĵȼ�
INT		CObject_Item_Gem::GetGemLevel()
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nQuality;
	}

	return 0;

}

// �õ���Ʒ����npc�ļ۸�
INT		CObject_Item_Gem::GetItemPrice()
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nPrice;
	}

	return 0;
}

// �õ���ʯ������Ϣ
LPCTSTR	CObject_Item_Gem::GetGemArribInfo()
{
	
	static TCHAR szAttribInfo[1024];
	TCHAR	szInfo[512];

	memset(szAttribInfo, 0, sizeof(szAttribInfo));
	STRING strAttrib = _T("");

	STRING strTemp = "";
		
	if(m_theBaseDef)
	{
		for(int i = 0; i < 60; i++)
		{
			memset(szInfo, 0, sizeof(szInfo));
			if(-1 != m_theBaseDef->nAttr[i])
			{
				strTemp = NOCOLORMSGFUNC(g_szGemAttName[i]);
				_stprintf(szInfo, _T("%s+%d\n"), strTemp.c_str(), m_theBaseDef->nAttr[i]);
				//_stprintf(szInfo, _T("%s:%d\n"), g_szGemAttName[i], m_theBaseDef->nAttr[i]);
				strAttrib += szInfo;
			}
		}

		_stprintf(szAttribInfo, _T("%s"), strAttrib.c_str());
		return szAttribInfo;
	}

	return szAttribInfo;


}

// 2006-3-30;
// �õ�quality��Ϣ
INT	CObject_Item_Gem::GetItemTableQuality(VOID)
{
	return m_theBaseDef->nQuality;
}

// 2006-3-30;
// �õ�type��Ϣ
INT	CObject_Item_Gem::GetItemTableType(VOID)
{
	return m_theBaseDef->nType;
}


// �õ���Ʒ����������2006-4-28
LPCTSTR	CObject_Item_Gem::GetItemTableTypeDesc()
{
	return m_theBaseDef->szTypeDesc;
}
