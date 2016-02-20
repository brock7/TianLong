
#ifndef __GAMESTRUCT_ITEM_H__
#define __GAMESTRUCT_ITEM_H__

#include "Type.h"


class SocketInputStream;
class SocketOutputStream;


BYTE		GetSerialType(UINT Serial);
BYTE		GetSerialClass(UINT Serial);
BYTE		GetSerialQual(UINT Serial);
UINT		GetSerialIndex(UINT Serial);







//��Χ
struct _RANGE_VALUE
{
	WORD				m_MinValue ;
	WORD				m_MaxValue ;
};

struct _ITEM_SKILL 
{
	BOOL				m_bActive;
	WORD				m_SkillID;
	WORD				m_SkillRate;

	BOOL				IsActive()
	{
		return m_bActive;
	}

	VOID				SetActive(BOOL bActive)
	{	
		m_bActive = bActive;
	}

	VOID	CleanUp()
	{
		memset(this,0,sizeof(_ITEM_SKILL));
	}

	_ITEM_SKILL()
	{
		CleanUp();
	}
};

//��Ʒ����ֵ
struct _ITEM_VALUE
{
	SHORT			m_Value ;
	VOID	CleanUp( )
	{
		memset( this, 0, sizeof(*this) ) ;
	}

	BOOL	operator == (_ITEM_VALUE& iV)
	{
		return iV.m_Value = m_Value;
	}
};

//��Ʒ����
struct _ITEM_ATTR
{
	BYTE				m_AttrType ;//�������ͨװ������m_AttrType��enum ITEM_ATTRIBUTE
									//    ����ʾ�������ͣ�m_Value��ʾ��������ֵ
									//�������ɫ����װ����m_AttrType��ʾ��ǰ��װ��
									//    ������ϵ�װ��������m_Value��ʾ��װ��������
	_ITEM_VALUE			m_Value ;

	VOID	CleanUp( )
	{
		memset( this, 0, sizeof(*this) ) ;
	}
	
	BOOL	operator==(_ITEM_ATTR& iA)
	{
		return	(iA.m_AttrType == m_AttrType) 
			   &&(iA.m_Value == m_Value);

	}
	
};



//��ƷΨһID
struct _ITEM_GUID
{
	BYTE	m_World ;		//�����: (��)101
	BYTE	m_Server ;		//����˳���ţ�(��)5
	INT		m_Serial ;		//��Ʒ���кţ�(��)123429
	
	_ITEM_GUID()
	{
		m_Serial = 0;
		m_Server = 0;
		m_World	 = 0;	
	}
	VOID Reset(VOID)
	{
		m_Serial = 0;
		m_Server = 0;
		m_World	 = 0;	
	}
	_ITEM_GUID& operator=(_ITEM_GUID const& rhs)
	{
		m_Serial = rhs.m_Serial;
		m_Server = rhs.m_Server;
		m_World  = rhs.m_World;
		return *this;
	}
	BOOL	operator ==(_ITEM_GUID& Ref)	const
	{
		return (Ref.m_Serial==m_Serial)&&(Ref.m_Server==m_Server)&&(Ref.m_World==m_World);
	}

	BOOL	operator ==(const _ITEM_GUID& Ref)	const
	{
		return (Ref.m_Serial==m_Serial)&&(Ref.m_Server==m_Server)&&(Ref.m_World==m_World);
	}

	BOOL	isNull() const
	{
		return (m_World ==0)&&(m_Serial==0)&&(m_Server == 0);
	}
	
};

struct _ITEM_TYPE
{
	
	struct  
	{
			UINT	m_Class : 7 ;		//�������ͣ����磺װ���ࡢ����Ʒ����
			UINT	m_Quality: 7 ;		//���Σ����磺��ɫװ������ɫװ������
			UINT	m_Type: 7 ;		//��𣬱��磺�󵶡���ǹ����
			UINT	m_Index: 11 ;		//��Ʒ�ţ����磺��Ҷ����ԧ�쵶����
	};
	
	UINT	ToUINT()
	{
		UINT	uid = 0 ;

		uid	+= m_Class;
		uid= uid<<7;
		uid	+= m_Quality;
		uid= uid<<7;
		uid	+= m_Type;
		uid= uid<<11;
		uid += m_Index;

		return uid;

	}

	UINT	ToSerial()	const
	{
		UINT Serial;
		Serial = m_Class;
		Serial	= Serial*100+m_Quality;
		Serial	= Serial*100+m_Type;
		Serial	= Serial*1000+m_Index;
		return Serial;
	}

	

	BOOL	isNull() const
	{
		return (m_Class==0)&&(m_Quality==0)&&(m_Type==0)&&(m_Index == 0);
	}

	BOOL	operator==(_ITEM_TYPE& Ref) const
	{
		return (m_Class==Ref.m_Class)&&(m_Quality==Ref.m_Quality)&&(m_Type==Ref.m_Type)&&(m_Index==Ref.m_Index);
	}
	
	BOOL	operator>(_ITEM_TYPE& Ref) const
	{
		return ToSerial()>Ref.ToSerial();
	}

	BOOL	operator<(_ITEM_TYPE& Ref) const
	{
		return ToSerial()<Ref.ToSerial();		
	}
	VOID	CleanUp()
	{
		m_Class		=	0;
		m_Quality	=	0;
		m_Type		=	0;
		m_Index		=	0;
	}


};


_ITEM_TYPE	ConvertSerial2ItemType(UINT Serial);


struct _ITEM_RULER
{
	BOOL	m_Discard;		//����
	BOOL	m_Tile;			//�ص�
	BOOL	m_ShortCut;		//���
	BOOL	m_CanSell;		//����
	BOOL	m_CanExchange;	//����
	BOOL	m_CanUse;		//ʹ��
	BOOL	m_PickBind;		//ʰȡ��
	BOOL	m_EquipBind;	//װ����
	BOOL	m_Unique;		//�Ƿ�Ψһ


	BOOL	CanDiscard()	{return m_Discard;}		//����
	BOOL	CanTile()		{return m_Tile;}		//�ص�
	BOOL	CanShortCut()	{return m_ShortCut;}	//���
	BOOL	CanSell()		{return m_CanSell;}		//����
	BOOL	CanExchange()	{return m_CanExchange;}	//����
	BOOL	CanUse()		{return m_CanUse;}		//ʹ��
	BOOL	isPickBind()	{return m_PickBind;}	//ʰȡ��
	BOOL	isEquipBind()	{return m_EquipBind;}	//װ����
	BOOL	isUnique()		{return m_Unique;}		//�Ƿ�Ψһ
};


enum	ITEM_RULER_LIST
{
	IRL_DISCARD,			//����
	IRL_TILE,				//�ص�
	IRL_SHORTCUT,			//���
	IRL_CANSELL,			//����
	IRL_CANEXCHANGE,		//����
	IRL_CANUSE,				//ʹ��
	IRL_PICKBIND,			//ʰȡ��
	IRL_EQUIPBIND,			//װ����
	IRL_UNIQUE				//�Ƿ�Ψһ
};



//��Ƕ��ʯ��Ϣ
struct _ITEM_GEMINFO
{
	UINT	m_GemType;
};




struct GEM_INFO
{
	
	_ITEM_ATTR	m_Attr;
	UINT		m_nPrice;
	

	VOID	CleanUp()
	{
		m_nPrice	=	0;
		
		m_Attr.CleanUp();
	}
};


struct STORE_MAP_INFO 
{
	INT						m_nLevel;
	FLOAT					m_xPos;
	FLOAT					m_zPos;
	INT						m_SceneID;
	INT						m_GrowType;
	
};

struct MEDIC_INFO
{
	
	BYTE					m_nCount;	   //��ǰ����
	BYTE					m_nLevel;
	BYTE					m_nLayedNum;   //��������
	BYTE					m_nReqSkillLevel;

	UINT					m_nBasePrice;
	INT						m_nScriptID;
	INT						m_nSkillID;
	BOOL					m_bCosSelf;	   //�Ƿ������Լ�
	INT						m_nReqSkill;
	BYTE					m_TargetType;  //��Ʒѡ������
	
	INT GetTileNum()		{return m_nCount;}

	INT GetMaxTileNum()		{return m_nLayedNum;}

	VOID	CleanUp()
	{
		m_nLevel			=	0;
		m_nBasePrice		=	0;
		m_nLayedNum			=	0;
		m_nScriptID			=	INVALID_ID;
		m_nSkillID			=	0;
		m_nCount			=	0;
		m_bCosSelf			=	FALSE;
		m_nReqSkill			=	-1;
		m_nReqSkillLevel	=	-1;
		m_TargetType		=	0;
	}
};


struct EQUIP_INFO 
{
	SHORT				m_SetNum;					//��װ���
	UINT				m_BasePrice;				//�۳��۸�
	CHAR				m_MaxNum;					//��װ
	BYTE				m_EquipPoint;				//��Ʒװ���
	BYTE				m_MaxDurPoint ;				//����;�ֵ
	BYTE				m_NeedLevel ;				//����ȼ�
	BYTE				m_GemMax ;					//���ʯ����
	//////////////////////////////////////////////////////////////////////////
	//����Ϊ�̶�����
	BYTE				m_FaileTimes ;				//����ʧ�ܴ���
	BYTE				m_CurDurPoint ;				//��ǰ�;�ֵ
	USHORT				m_CurDamagePoint;			//��ǰ���˶�
	BYTE				m_AttrCount;				//���Ե�����
	BYTE				m_StoneCount;				//��Ƕ��ʯ������
	_ITEM_ATTR			m_pAttr[MAX_ITEM_ATTR];		//��Ʒ����
	_ITEM_GEMINFO		m_pGemInfo[MAX_ITEM_GEM] ;	//��Ʒ����Ƕ�ı�ʯ
	
	

	VOID CleanUp()
	{
		memset(this,0,sizeof(EQUIP_INFO));
	};


	BOOL				CanUseSkill();
	WORD				GetUseSkillId();

	BOOL				CanRandSkill();
	WORD				GetRandSkillId();

};


#define MAX_ITEM_LIST_COUNT			8

struct _ITEM_LIST 
{
	INT				m_ListCount;
	_ITEM_TYPE		m_ListType[MAX_ITEM_LIST_COUNT];
	INT				m_TypeCount[MAX_ITEM_LIST_COUNT];

	_ITEM_LIST()
	{
		memset(this,0,sizeof(_ITEM_LIST));
	}
	
	VOID	AddType(_ITEM_TYPE it,INT Count)
	{
		Assert(Count>0);
		Assert(!it.isNull());
		
		BOOL bFindSame = FALSE;
		for(INT i=0;i<m_ListCount;i++)
		{
			if(m_ListType[i]==it)
			{
				m_TypeCount[i]+=Count;
				bFindSame = TRUE;
				break;
			}
		}

		if(!bFindSame)
		{
			m_TypeCount[m_ListCount] = Count;
			m_ListType[m_ListCount]  = it;	
			m_ListCount++;
		}
	}
	
	VOID Init()
	{
		memset(this,0,sizeof(_ITEM_LIST));
	}
};


//��Ʒ��Ϣ, ����������Ʒ����������
#define	ITEMISVALID(ITEM) (ITEM.m_ItemIndex != 0 )//��Ʒ�Ƿ�Ϸ�
#define	ITEMPTRISVALID(PITEM) ( (PITEM == NULL) ? FALSE:PITEM->m_ItemIndex )//��Ʒָ���Ƿ�Ϸ�
#define	ITEMREFPTRISVALID(PITEM) ( (PITEM == NULL) ? FALSE:PITEM->GetItemTableIndex() )//��Ʒָ���Ƿ�Ϸ�

#define MAX_FIX_ATTR_LENGTH		100
#define MAX_PET_SKILL_LENGTH	50
#define MAX_ITEM_CREATOR_NAME	13
#define MAX_ITEM_PARAM			3
#define MAX_ITEM_STRING_LENGTH	255


enum ItemParamValue
{
	IPV_CHAR	=	0,
	IPV_SHORT	=	1,
	IPV_INT		=	2	
};

struct _ITEM
{
	_ITEM_GUID			m_ItemGUID ;				//��Ʒ�̶���Ϣ�����ܸı䣩
	UINT				m_ItemIndex;				//��Ʒ����
													/*
													|	    1 ~ 10000		��ɫװ��
													|	10001 ~ 20000		��ɫװ��	
													|	20001 ~ 30000		��ɫװ��
													|	30001 ~ 35000		ҩƿ
													|	35001 ~ 40000		��ʯ
													|						...
													*/

	CHAR				m_RulerID;
	CHAR				m_nsBind;
	CHAR				m_Creator[MAX_ITEM_CREATOR_NAME];
	INT					m_Param[MAX_ITEM_PARAM];
	union 
	{
		EQUIP_INFO			m_Equip;
		GEM_INFO			m_Gem;
		MEDIC_INFO			m_Medic;
		STORE_MAP_INFO		m_StoreMap;
	};

	_ITEM( )
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		memset( this, 0, sizeof(*this) ) ;
	}
	
	BYTE		ItemClass( )	const
	{
			return GetSerialClass(m_ItemIndex);
	};

	BYTE		ItemType()		const
	{
		return GetSerialType(m_ItemIndex);
	}

	BYTE		GetQual()	const
	{
		return GetSerialQual(m_ItemIndex);
	}

	BYTE		GetIndex()	const
	{
		return GetSerialIndex(m_ItemIndex);
	}

	EQUIP_INFO*	   GetEquipData() const
	{
		return	(EQUIP_INFO*)(&m_Equip);
	}

	GEM_INFO*	  GetGemData()	const
	{
		return	(GEM_INFO*)(&m_Gem);
	}

	MEDIC_INFO*		GetMedicData()	const
	{
		return (MEDIC_INFO*)(&m_Medic);
	}

	STORE_MAP_INFO*	GetStoreMapData()	const
	{
		return (STORE_MAP_INFO*)(&m_StoreMap);
	}
	//////////////////////////////////////////////////////////////////////////
	// ��Ʒ��������
	// �ܵ�����Ʒ		���� ʵ�ʴ�С
	// ���ܵ�����Ʒ		���� 1
	//
	BYTE	GetItemCount()		const;

	VOID	SetItemCount(INT nCount);

	BYTE	GetItemTileMax()	const;
	
	BOOL	isFullTile()		const;


	//���ӵ�������
	BOOL	IncCount(UINT nCount =1);	
	//���ٵ�������
	BOOL	DecCount(UINT nCount =1);


	BOOL	IsNullType()	const
	{
		return m_ItemIndex == 0;
	}
	
	VOID	Read(SocketInputStream& iStream);
	VOID	Write(SocketOutputStream& oStream) const;
	
	BOOL	WriteFixAttr(CHAR* pIn,INT BuffLength);
	BOOL	ReadFixAttr(CHAR* pOut,INT& OutLength,INT BuffLength);

	BOOL	WriteVarAttr(CHAR* pIn,INT BuffLength);
	BOOL	ReadVarAttr (CHAR* pOut,INT& OutLength,INT BuffLength);

	BOOL	IsFixAttr(BYTE attrType);
	BYTE	GetFixAttrCount();
	
	BOOL	GetItemBind() const;
	VOID	SetItemBind(BOOL bBind);

	BOOL	GetItemIdent() const;
	VOID	SetItemIdent(BOOL bIdent);

	BOOL	GetItemPLock() const;	//��������
	VOID	SetItemPLock(BOOL bIdent);
	
	INT		GetItemFailTimes() const;
	VOID	SetItemFailTimes(INT nTimes);
	//������
	BOOL	GetCreatorVar() const;
	VOID	SetCreatorVar(BOOL bCreator);

	INT		GetItemParamValue(UINT Start,const ItemParamValue& ipv);
	VOID	SetItemParam(UINT start,ItemParamValue& ipv,INT value);
};

//pItem			��Ҫת����_ITEM
//pOut			ת�������������
//OutLength		������ݵĳ���
//BuffLength	pOut ��ʵ�ʳ���
BOOL		Item2String(_ITEM* pItem,CHAR* pOut,INT& OutLength,INT BuffLength);
BOOL		String2Item(CHAR* pIn,INT BuffLength,_ITEM* pItem);


//A,B ����
VOID	SwapItem(_ITEM* pItemA,_ITEM* pItemB);
//A ����B �ռ�
//ͬʱA ���
VOID	OverWriteItem(_ITEM* pItemA,_ITEM* pItemB);

UINT	GetItemRealSize(const _ITEM& it);



struct _ITEM_EQUIP : public _ITEM
{


	_ITEM_EQUIP( )
	{
	};
};

struct _ITEM_EXPEND : public _ITEM
{

	_ITEM_EXPEND( )
	{
	};
};

struct _ITEM_MATERIAL : public _ITEM
{

	_ITEM_MATERIAL( )
	{
	};
};

struct _ITEM_GEM : public _ITEM
{

	_ITEM_GEM( )
	{
	};
};

struct _ITEM_CURIOSA : public _ITEM
{

	_ITEM_CURIOSA( )
	{
	};
};

struct _ITEM_TALISMAN : public _ITEM
{

	_ITEM_TALISMAN( )
	{
	};
};

struct _ITEM_GUILDITEM : public _ITEM
{

	_ITEM_GUILDITEM( )
	{
	};
};

struct _ITEM_TASKITEM : public _ITEM
{

	_ITEM_TASKITEM( )
	{
	};
};


struct  ItemBoxContaner
{

	ItemBoxContaner(){
		memset(m_Item,0,sizeof(_ITEM)*MAX_BOXITEM_NUMBER);
		memset(m_nQuality,0,sizeof(INT)*MAX_BOXITEM_NUMBER);
		m_nCount = 0;
	}

	VOID	AddItemType(_ITEM_TYPE& it,INT iQuality)
	{
		m_Item[m_nCount].m_ItemIndex = it.ToSerial();
		m_nQuality[m_nCount]	=	iQuality;
		m_nCount++;
	}
	_ITEM				m_Item[MAX_BOXITEM_NUMBER]; 
	INT					m_nQuality[MAX_BOXITEM_NUMBER];
	INT					m_nCount;
	UINT				m_uDropType;
};


#endif
