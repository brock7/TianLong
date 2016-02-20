#ifndef _SHOP_MANAGER_H_
#define _SHOP_MANAGER_H_
#include "Type.h"
#include "ItemManager.h"
#include "ItemBoxManager.h"
#include "Rand.h"

#define MAX_SHOP_PER_PERSON				10
//#define MAX_SOLDOUT_ITEM_PER_PERSON		5

class Obj_Human;
class Obj_Monster;

/*
_SHOP,һ���̵�
*/
struct _SHOP
{
	/*
	_MERCHANDISE_LIST,�̵��е���Ʒ�б�
	*/
	struct _MERCHANDISE_LIST
	{
		USHORT			m_ListCount;
		_ITEM_TYPE*		m_ListType;
		UINT*			m_ListTypeIndex;
		INT*			m_TypeCount;
		INT*			m_TypeMaxNum;
		FLOAT*			m_AppearRate;
		//INT*			m_pnPrice;				// �۸񣬵����̵���ҵ�λ���� CU_MONEY ʱ��Ч
		BOOL*			m_pbForSale;			// ����̵�ʹ�ã�������Ʒ
		INT				m_nCurrent;				// ����ʱ����

		//
		_MERCHANDISE_LIST() 
		{
			m_ListCount = 0;
			m_nCurrent = 0;
			m_ListType	= NULL;
			m_ListTypeIndex = NULL;
			m_TypeCount = NULL;
			m_AppearRate = NULL;
			//m_pnPrice = NULL;
			m_pbForSale = NULL;
		};
		//
		_MERCHANDISE_LIST(BYTE num)
		{
			m_ListCount		= num;
			m_nCurrent		= 0;
			m_ListType		= new _ITEM_TYPE[m_ListCount];
			m_ListTypeIndex	= new UINT[m_ListCount];
			m_TypeCount		= new INT[m_ListCount];
			m_TypeMaxNum	= new INT[m_ListCount];
			m_AppearRate	= new FLOAT[m_ListCount];
			//m_pnPrice		= new INT[m_ListCount];
			m_pbForSale		= new BOOL[m_ListCount];
		}
		~_MERCHANDISE_LIST()
		{
			SAFE_DELETE_ARRAY(m_ListType)
			SAFE_DELETE_ARRAY(m_ListTypeIndex)
			SAFE_DELETE_ARRAY(m_TypeCount)
			SAFE_DELETE_ARRAY(m_TypeMaxNum)
			SAFE_DELETE_ARRAY(m_AppearRate)
			//SAFE_DELETE_ARRAY(m_pnPrice)
			SAFE_DELETE_ARRAY(m_pbForSale)
		}

		//
		VOID	AddType(_ITEM_TYPE it, INT Count, INT MaxCount, /*INT nPrice,*/ FLOAT Rate)
		{
			Assert(!it.isNull());
			Assert(m_nCurrent<m_ListCount);
			m_ListType[m_nCurrent]		=	it;
			m_ListTypeIndex[m_nCurrent]	=	it.ToSerial();
			m_TypeCount[m_nCurrent]		=	Count;
			m_TypeMaxNum[m_nCurrent]	=	MaxCount;
			//m_pnPrice[m_nCurrent]		=	nPrice;
			m_pbForSale[m_nCurrent]		=	FALSE;
			m_AppearRate[m_nCurrent++]	=	Rate;
		}
	};

	_SHOP()
	{
		m_ShopId = -1;
		m_bIsRandShop = FALSE;
		m_nCountForSell = 0;
		m_nCurrencyUnit = CU_MONEY;
		m_nRepairLevel=0;	
		m_nBuyLevel=0;	
		m_nRepairType=-1;	
		m_nBuyType=-1;		
		m_nRepairSpend=0;	
		m_nRepairOkProb=0;
		m_scale = 0.0f;
		m_ItemList = NULL;
		m_refreshTime = 0;
		m_IsDyShop = FALSE;
		m_uSerialNum = 0;
		m_bCanBuyBack = FALSE;
		m_bCanMultiBuy	=	FALSE;
		m_szShopName[0] = 0;
	}
	_SHOP(INT itemNum)
	{
		m_ShopId = -1;
		m_bIsRandShop = FALSE;
		m_nCountForSell = 0;
		m_nCurrencyUnit = CU_MONEY;
		m_nRepairLevel=0;
		m_nBuyLevel=0;	
		m_nRepairType=-1;	
		m_nBuyType=-1;		
		m_nRepairSpend=0;	
		m_nRepairOkProb=0;
		m_scale = 0.0f;
		m_ItemList = new _MERCHANDISE_LIST(itemNum);
		m_refreshTime = 0;
		m_IsDyShop = FALSE;
		m_uSerialNum = 0;
		m_bCanBuyBack = FALSE;
		m_bCanMultiBuy	=	FALSE;
		m_szShopName[0] = 0;
	}


	~_SHOP()
	{
		SAFE_DELETE(m_ItemList)
	}

	UCHAR				GetSerialNum( ) { return m_uSerialNum; }
	VOID				UpdateSerialNum( ) { m_uSerialNum = (m_uSerialNum+1) % 255; }

	// ��֤�����ĺϷ��ԣ�[����̵����ã�����ʾ�б������ת�����̵��б��е�ʵ������]
	INT					ConvertIndex( INT nIndex );

	// ���������Ʒ�б�
	//VOID				SetRandPriceItemList(RandPriceItemList_t* pItemList);

	// ��������۸�
	BOOL				GenerateRandItemPrice();

	// �������۸�
	INT					GetRPItemValue(UINT uItemIdx, ValueType valuetype);

	//
	INT					m_ShopId;

	CHAR				m_szShopName[MAX_SHOP_NAME];

	INT					m_ShopType;		//0��ͨ�̵꣬ 1Ԫ���̵�
	// ����̵�����
	BOOL				m_bIsRandShop;
	INT					m_nCountForSell;			// ���������ѡ�������۵���Ʒ����
	// ����̵�����

	INT					m_nCurrencyUnit;			// ���ҵ�λ enum CURRENCY_UNIT
	INT					m_refreshTime;

	// ����ֻ�л��ҵ�λ�� CU_MONEY ʱ��Ч
	INT					m_nRepairLevel;				// ����ȼ�
	INT					m_nBuyLevel;				// �չ��ȼ�
	INT					m_nRepairType;				// ��������
	INT					m_nBuyType;					// �̵���չ�����
	FLOAT				m_nRepairSpend;				// ������
	FLOAT				m_nRepairOkProb;			// ����ɹ�����
	BOOL				m_bCanBuyBack;				// �Ƿ��ܻع�

	FLOAT				m_scale;
	// ����ֻ�л��ҵ�λ�� CU_MONEY ʱ��Ч

	_MERCHANDISE_LIST*	m_ItemList;
	BOOL				m_IsDyShop;
	BOOL				m_bCanMultiBuy;				//�Ƿ��ܹ���������Ʒ

	UCHAR				m_uSerialNum;				// ����̵���ˮ�ţ���¼������Ϊ������˲�ͬ�汾���̵����Ʒ���´�����

	_100_PER_RANDOM_TABLE		m_Rand100;
	//RandPriceItemList_t			m_aRPItemList;	

};

/*
ShopMgr,�������ݵķ��������
*/
class ShopMgr
{
public:
	friend class Obj_Human;
//initial+destory
public:
	ShopMgr();
	~ShopMgr();
public:
	virtual	BOOL		Init(){return TRUE;};
	virtual VOID		CleanUp();
	_SHOP*				GetShopByIndex(INT index){return &m_Shops[index];}
	_SHOP*				GetShopByID(INT id);
	INT					GetShopIndexByID(INT id);

//globle
public:
	static INT			ConvertItemType2Money(_ITEM_TYPE it);

//data
protected:
	_SHOP*				m_Shops;
	INT					m_Count;
};

/*
StaticShopManager,��̬̬���̵������,������Ϸ����ֻ��һ����̬�̵��������ʵ����
�����������в���仯���̵����Ϣ����ͬʱҲ�����ж�̬�̵�������������̵��ģ�森
*/
class StaticShopManager	: public ShopMgr
{
public:
	StaticShopManager(){;}
	~StaticShopManager();
public:
	BOOL				Init();
	VOID				CleanUp();
private:
	BOOL				LoadShopsFromFile( CHAR* filename ) ;	
};

/*
DynamicShopManager,����˼��,��̬���̵������,
����ÿ��ʵ������ÿ��NPC�ϣ���NPC���������ԣ���()
��̬���̵��������ʱ��ˢ�£��������Լ��ľֲ�����
*/
class DynamicShopManager : public ShopMgr
{

public:
	DynamicShopManager(Obj_Monster* pboss);
	~DynamicShopManager();
public:
	INT					GetShopsNum()const{return m_nCurrent;}
public:
	BOOL				Init();
	VOID				CleanUp();
	INT					AddDynamicShop(_SHOP* pSource);
	BOOL				Tick(UINT uTime);

private:
	INT					m_nCurrent;
	CMyTimer*			m_aRefeshTimer;
	Obj_Monster*		m_pBoss;

};

extern StaticShopManager* g_pStaticShopManager;

#endif