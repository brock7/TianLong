/********************************************************************
	��������:	2005��11��2��
	����ʱ��:	16:54
	�ļ�����:	ItemTable.h
	�ļ�·��:	d:\Prj\Server\Server\Item\ItemTable.h
	
	�ļ����ܣ� ��Ʒ�������ݱ����
	�޸ļ�¼��
	
*********************************************************************/

#ifndef _ITEM_TABLE_H_
#define _ITEM_TABLE_H_

#include "ItemTypes.h"
#include "Type.h"
#include "GameStruct_Item.h"
#include "GameDefine2.h"

#define SELF_CONS(x)	x(){memset(this,0,sizeof(x));}
#define DEFAULT_CONS(theClass,parentClass)	theClass(){memset(this,0,sizeof(theClass)); parentClass();}



//װ�����еĻ�������
struct EQUIP_TB 
{
	SELF_CONS(EQUIP_TB);
	INT						m_IndexID;
	_ITEM_TYPE				m_ItemType;
	BYTE					m_EquipPoint;					//װ���
	CHAR					m_RulerID;						//������
	BYTE					m_ReqLevel;						//��Ҫ�ȼ�
	INT						m_MaxDur;						//����;�
	UINT					m_BasePrice;					//�۳��۸�
	INT						m_RepaireLevel;					//����۸�
	INT						m_PhysicAttack;					//������
	INT						m_MagicAttack;					//ħ������
	INT						m_PhysicDefense;				//�������
	INT						m_MagicDefense;					//ħ������
	INT						m_AttackSpeed;					//�����ٶ�
	INT						m_Miss;							//����	
	BYTE					m_CanRepaireTimes;				//���������
	BYTE					m_MaxGem;						//�����Ƕ
	INT						m_EquipSetNum;					//��װ���
	INT						m_EquipSetMaxNum;				//�����װ��Ŀ
};


//������ṹ
struct DROP_BOX_TB {

	SELF_CONS(DROP_BOX_TB);
	UINT				m_DropBoxId;						//DropBox�ı�ű�
	INT					m_DropValue;						//DropBox��ֵ������������
	_ITEM_TYPE			m_DropItem[MAX_DROPBOX_CARRAGE];	//DropBoxЯ����Ʒ��
	INT					m_Quality[MAX_DROPBOX_CARRAGE];		//��ƷƷ�ʱ�	
};

//��Ʒ�����νṹ��
struct ITEM_QUALITY_TB {

	SELF_CONS(ITEM_QUALITY_TB)
	UINT		m_ItemLevel;
	INT			m_Quality[MAX_ITEM_TABLE_QUALITYS];
};


struct MINMAX_VALUE 
{
	SELF_CONS(MINMAX_VALUE)
	INT			m_minValue;
	INT			m_maxValue;
};


enum  MINMAX_TYPE
{
	VT_MIN,
	VT_MAX
};
//��Ʒ��ֵ�νṹ��
struct ITEM_VALUE_TB 
{
	SELF_CONS(ITEM_VALUE_TB)
	UINT					m_ValueType;
	MINMAX_VALUE			m_Values[MAX_ITEM_TABLE_VALUES];
};


//��ͨװ����
struct COMMON_EQUIP_TB :public	EQUIP_TB
{
	DEFAULT_CONS(COMMON_EQUIP_TB,EQUIP_TB);
	INT						m_Job;								//ְҵ����
};

//��ɫװ����
struct BLUE_EQUIP_TB:public EQUIP_TB 
{
	DEFAULT_CONS(BLUE_EQUIP_TB,EQUIP_TB);
	INT						m_Job;								//ְҵ����
	INT						m_AttrRate[IATTRIBUTE_NUMBER];		//�����������
};

//��ɫװ����
struct GREEN_EQUIP_TB :public	EQUIP_TB
{
	DEFAULT_CONS(GREEN_EQUIP_TB,EQUIP_TB);
	INT						m_Job;								//ְҵ����
	_ITEM_ATTR				m_Attr[MAX_GREEN_ITEM_EXT_ATTR];	//��ɫװ����������
};





//���������ṹ��
struct MONSTER_DROPBOX_TB 
{
	SELF_CONS(MONSTER_DROPBOX_TB);
	UINT					m_MonsterId;
	UINT					m_MonsterValue;
	UINT					m_DropType;
	MONSTER_DROPBOXS		m_DropBoxs;
};


//��ʯ��ṹ��
struct GEMINFO_TB 
{
	GEMINFO_TB()
	{
		m_nTableIndex	=	0;
		m_ResourceID	=	0;
		m_nRulerID		=	0;
		m_nPrice		=	0;
		m_ItemType.CleanUp();
		m_GenAttr.CleanUp();
	}
	UINT					m_nTableIndex;;
	_ITEM_TYPE				m_ItemType;
	WORD					m_ResourceID;
	CHAR					m_nRulerID;
	UINT					m_nPrice;
	_ITEM_ATTR				m_GenAttr;
};



//��ͨ��Ʒ��ṹ��
struct COMMITEM_INFO_TB 
{
	COMMITEM_INFO_TB()
	{
		m_nTableIndex		=	0;
		m_nLevel			=	0;
		m_nBasePrice		=	0;
		m_nLayedNum			=	0;
		m_nScriptID			=	0;
		m_nSkillID			=	0;
		m_nRulerID			=	0;
		m_bCosSelf			=	FALSE;
		m_nReqSkill			=	-1;
		m_nReqSkillLevel	=	-1;
		m_TargetType		=	0;
		m_ItemType.CleanUp();
		
	}

	INT					m_nTableIndex;
	_ITEM_TYPE			m_ItemType;
	INT					m_nLevel;
	UINT				m_nBasePrice;
	CHAR				m_nRulerID;	 //����ID
	INT					m_nLayedNum; //��������
	INT					m_nScriptID;
	INT					m_nSkillID;
	BOOL				m_bCosSelf;
	INT					m_nReqSkill;
	INT					m_nReqSkillLevel;
	BYTE				m_TargetType;
};


//�ر�ͼ�ṹ��
struct STORE_MAP_INFO_TB 
{
	STORE_MAP_INFO_TB()
	{
		m_nTableIndex	=	0;
		m_ItemType.CleanUp();
		m_ResourceID	=	0;
		m_nRulerID		=	0;
		m_nLevel		=	0;
		m_nBasePrice	=	0;
		m_xPos			=	-1.0f;
		m_zPos			=	-1.0f;
		m_SceneID		=	-1;
		m_GrowPointType	=	-1;

	}
	INT						m_nTableIndex;;
	_ITEM_TYPE				m_ItemType;
	WORD					m_ResourceID;
	CHAR					m_nRulerID;
	INT						m_nLevel;
	UINT					m_nBasePrice;
	FLOAT					m_xPos;
	FLOAT					m_zPos;
	INT						m_SceneID;
	INT						m_GrowPointType;
	INT						m_ScriptID;
	
};

//����˥����ṹ��
struct DROP_ATT_TB 
{
	SELF_CONS(DROP_ATT_TB);
	INT				m_DeltaLevel;
	FLOAT			m_AttValue;
};


//װ�����Ͻṹ��
struct EQUIP_SET_TB
{
	SELF_CONS(EQUIP_SET_TB);
	INT				m_nEquipSetSerial;
	INT				m_nAttrCount;
	_ITEM_ATTR		m_ItemAttr[MAX_ITEM_SET_ATTR];
};


//��Ʒ����ṹ��
struct ITEM_RULER_TB :public _ITEM_RULER{
	
	SELF_CONS(ITEM_RULER_TB);
	INT		m_RulerIndex;
	
};


/*
 *��Ʒ��ر���Դ������	
 */
class ItemTable
{
	public:
		ItemTable() ;
		~ItemTable() ;

		VOID	CleanUp( ) ;
		BOOL	Init();


	public:
		MONSTER_DROPBOX_TB*			GetMonsterDropTB(INT iMonsterType);
		DROP_BOX_TB*				GetDropBoxTB(INT iDropBox);
		
		GREEN_EQUIP_TB*				GetGreenItemTB(UINT itemSerial);
		COMMON_EQUIP_TB*			GetWhiteItemTB(UINT itemSerial);
		BLUE_EQUIP_TB*				GetBlueItemTB(UINT itemSerial);
		
		ITEM_QUALITY_TB*			GetItemQualityTB(INT ItemLevel);
		MINMAX_VALUE				GetItemValue(ITEM_ATTRIBUTE iAtt,INT QualityLevel);

		DROP_ATT_TB*				GetDropAttTB(INT iDeltaLevel);

		GEMINFO_TB*					GetGemInfoTB(UINT	itemSerial);
	

		COMMITEM_INFO_TB*			GetCommItemInfoTB(UINT	itemSerial);

		EQUIP_SET_TB*				GetEquipSetTB(INT EquipSetSerial);
		
		ITEM_RULER_TB*				GetRuleValueByID(INT& iIndex);

		STORE_MAP_INFO_TB*			GetStoreMapTB(UINT	itemSerial);;

	protected:

		VOID						InitWhiteItemTable();
		VOID						InitGreenItemTable();
		VOID						InitBlueItemTable();
		VOID						InitGoldenItemTable();
		VOID						InitDropBoxTable();
		VOID						InitMonsterDropBoxTable();
		VOID						InitItemLevelTable();
		VOID						InitItemValueTable();
		VOID						InitGemInfoTable();
		VOID						InitCommItemInfoTable();
		VOID						InitDropAttTable();
		VOID						InitEquipSetAttrTable();
		VOID						InitItemRulerTable();
		VOID						InitStoreMapTable();
		

	private:
		UINT						m_nCommonItemCount;
		COMMON_EQUIP_TB*			m_pCommonEquipTableData;

		UINT						m_nGreenItemCount;
		GREEN_EQUIP_TB*				m_pGreenEquipTableData;

		UINT						m_nBlueItemCount;
		BLUE_EQUIP_TB*				m_pBlueEquipTableData;

		UINT						m_nGoldenItemCount;

		UINT						m_nDropBoxCount;
		DROP_BOX_TB*				m_pDropBoxTableData;

		UINT						m_nMonsterDropBoxCount;
		MONSTER_DROPBOX_TB*			m_pMonsterDropTableData;


		UINT						m_nItemLevelCount;
		ITEM_QUALITY_TB*			m_pItemQualityData;

		UINT						m_nValueCount;
		ITEM_VALUE_TB*				m_pItemValueData;	


		UINT						m_nGemCount;
		GEMINFO_TB*					m_pGemInfoData;

		UINT						m_nCommItemCount;
		COMMITEM_INFO_TB*			m_pCommItemInfoData;

		UINT						m_nDropAttCount;
		DROP_ATT_TB*				m_pDropAttData;

		UINT						m_nEquipSetCount;
		EQUIP_SET_TB*				m_pEquipSetData;

		UINT						m_nItemRulerCount;
		INT							m_nHashOffSet;
		ITEM_RULER_TB*				m_pItemRulerData;

		UINT						m_nStoreMapCount;
		STORE_MAP_INFO_TB*			m_pStoreMapData;
		
};

BYTE	GetItemTileMax(_ITEM_TYPE& it);


extern ItemTable g_ItemTable ;

#endif
