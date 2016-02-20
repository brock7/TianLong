#pragma once
#include "TDObjectSystem.h"
/*
|
|	��Ϸ�е���Ʒ��غ�����	
|
*/
#define MAX_ITEM_CREATOR_NAME	13
class CObject_Item : public tObject_Item
{
	//-----------------------------------------------------
	//������
public:
	//�ӱ�����Դ���ж�ȡ��������,����һ��ITEM
	static CObject_Item*	NewItem(UINT idTable);
	//�ͷ�Item(Ӧ����Item���һ�������õĺ���)
	static VOID				DestroyItem(CObject_Item* pItem);
	//Ѱ����Ʒ
	static tObject_Item*	FindItem(UINT id);
	//�����ڱ�����ʹ�õļ��
	static BOOL				CheckUseInPackage(CObject_Item* pItem, STRING& szMsg);
	//-----------------------------------------------------
	//
public:
	//�ͻ���ID
	virtual INT				GetID(VOID) const { return m_idClient; }
			VOID			ChangeClientID(VOID);

	//GUID
	virtual VOID				SetGUID(USHORT idWorld, USHORT idServer, UINT idSerial);
	virtual VOID				GetGUID(USHORT& idWorld, USHORT& idServer, UINT& idSerail) const;
	virtual unsigned __int64	GetGUID(VOID) const;


	//-------------------------------------------------------------------------------------------------------------------------------
	//-- super tool tip ʹ��
	//-- 
	// �õ����ʹ�������Ʒ��Ҫ�ĵȼ�
	virtual INT				GetNeedLevel();

	// �õ���Ʒ�;ö�
	virtual INT				GetItemDur();

	// �õ���Ʒ����;ö�
	virtual INT				GetItemMaxDur();

	// �õ���Ʒ��������� 
	virtual INT				GetItemRepairCount();

	// �õ���Ʒ�İ���Ϣ 
	virtual INT				GetItemBindInfo();

	// �õ���Ʒ����npc�ļ۸�
	virtual INT				GetItemPrice();

	// �õ���Ʒ��������
	virtual LPCTSTR				GetManufacturer();

	// �õ���ɫ����
	virtual LPCTSTR			GetBaseWhiteAttrInfo();

	// �õ���չ��ɫ����
	LPCTSTR			GetExtBlueAttrInfo();



	//��Ʒ�����
	virtual ITEM_CLASS		GetItemClass(VOID) const	= 0;

	// ��Ʒ���·����Ժ� ��Ҫ getitemClass ItemQuality ��GetItemType����������ȷ����
	// 2006-3-30;
	virtual INT				GetItemTableQuality(VOID);
	virtual INT				GetItemTableType(VOID);


	//��Ʒ������
	virtual LPCTSTR			GetName(VOID) const			= 0;
	//��Ʒ����
	virtual LPCTSTR			GetDesc(VOID) const			= 0;
	//ͼ����
	virtual LPCTSTR			GetIconName(VOID) const		= 0;
	//��Ʒ����ϸ��ţ�����client�õ�index
	virtual INT				GetParticularID(VOID) const	{ return m_nParticularID; }
	//��ϸ����(������Ҫ������)
	virtual LPCTSTR			GetExtraDesc(VOID)			= 0;
	//������ϸ����
	virtual VOID			SetExtraInfo(const _ITEM * pItemInfo)= 0;
	//������Ʒ��������
	virtual VOID			SetManufacturer(const _ITEM * pItemInfo);
	//����
	virtual VOID			SetNumber(INT nNum) { m_nNum = nNum; }
	virtual	INT				GetNumber(VOID) const { return m_nNum; }
	//�����Ʒ������������
	virtual	INT				GetMaxOverLay(VOID) const	= 0;

	//�������,��ʱ����������Ʒ����ʾ
	virtual VOID			SetMax(INT nNum){m_nMax = nNum;}
	virtual INT				GetMax(VOID) const{return m_nMax;}

	//����
	virtual VOID			SetTypeOwner(ITEM_OWNER owner) { m_typeOwner = owner; }
	virtual ITEM_OWNER		GetTypeOwner(VOID) const { return m_typeOwner; }

	//��������������
	virtual INT				GetPosIndex(VOID) const { return m_nPosIndex; }
	virtual VOID			SetPosIndex(INT nPosIndex) { m_nPosIndex = nPosIndex; }

	//����Դ���е�λ��
	virtual INT				GetIdTable(VOID) const { return m_idTable; }
	
	//��ѯ�߼�����,һ�����ڽű�����
	virtual STRING			GetAttributeValue(LPCTSTR szValueName);

	//��¡��ϸ��Ϣ
	virtual VOID			Clone(const CObject_Item * pItemSource);

	//��Ʒ�Ƿ����� =TURE����
	virtual BOOL			IsLocked(){return m_bLocked;}
	virtual VOID			SetLock(BOOL bLock){m_bLocked = bLock;}

	//��Ʒ������֤
	virtual BOOL			Rule(INT ruletype);
	virtual STRING			RuleFailMessage(INT ruletype);

	//�õ���Ʒʹ�õ�Ŀ������
	virtual INT				GetItemTargetType();

	// �õ���Ʒ����������2006-4-28
	virtual LPCTSTR			GetItemTableTypeDesc();


	// ��Ʒ�ڿͻ��˵ı���״̬ 2006��3��24
	enum SAVE_ITEM_STATUS
	{
		NO_MORE_INFO = 0,		// û����ϸ��Ϣ��
		GET_MORE_INFO,			// �õ���ϸ��Ϣ
	};

	//������Ʒ����״̬ 2006��3��24
	void SetItemSaveStatus(SAVE_ITEM_STATUS nSaveStatus = NO_MORE_INFO);

	//��Ʒ�Ƿ�����ϸ��Ϣ2006��3��30
	BOOL CObject_Item::IsItemHasDetailInfo();

	virtual BOOL			GetByProtect(){return m_bByProtect;}
	virtual VOID			SetByProtect(BOOL bByProtect){m_bByProtect = bByProtect;}
	
	// �õ���Ʒ����������Ϣ
	virtual INT	GetItemLevelDesc(){	return 1;}

	virtual _ITEM* GetItemExtraInfoItem() const{return m_pExtraInfoItem;}

	virtual INT	GetItemTransferString(STRING& strResult) const;
	virtual VOID SetExtraInfoFromString(LPCTSTR strIn, INT nLen);
	virtual LPCTSTR			GetSysColour(){return ConvertSysColour(-1);}		//Ĭ�Ϸ��ذ�ɫ
	virtual INT				GetNeedJob(VOID) const	{ return -1; }
protected:
	LPCTSTR ConvertSysColour(INT nColor);
public:
	// Ϊ��Ʒ����ϵͳʹ�õ��ַ���ת������ [8/7/2006]
	static BOOL	Bin2Str_Transfer(const CHAR* pIn,UINT InLength,CHAR* pOut);
	static BOOL	Str2Bin_Transfer(const CHAR* pIn,UINT InLength,CHAR* pOut,UINT OutLimit,UINT& OutLength);
protected:
	INT			m_idClient;		//�ͻ���ID;

	ITEM_OWNER		m_typeOwner;	//����������
	INT				m_idOwner;		//������Object��ID���п�������ҵ�ServerID�������Ǳ����ServerID
	INT				m_nPosIndex;	//���ڵ�λ�ñ�ţ���Դ��ڰ��������ӣ����ڵ���Ҫ��������λ�����
	INT				m_nNum;			//����
	INT				m_nMax;			//���������ʱ����������Ʒ����ʾ

	SAVE_ITEM_STATUS	m_nItemSaveStatus;	//�ͻ�����Ʒ�ı���״̬�� 
											//
											// 0 �� ����ϸ��Ϣ
											// 1 �� �õ���ϸ��Ϣ�� 


	//-----------------------------------------------------
	//����Դ���еĶ���ID
	// 20000000 - 49999999		��ͨ���ߣ��������ϡ�ҩƷ�����ᡢ������Ʒ��
	// 50000000 - 59999999		��ʯ
	// 60000000 - 69999999		�ر�ͼ
	// 10200000 - 10299999		��ɫװ��
	// 10400000 - 10499999		��ɫװ��
	// 10100000 - 10199999		��ɫװ��
	INT			m_idTable;			

	//��Ʒ����ϸ��� = ((nClass*100+nQuality)*100+nType)*1000+nIndex
	INT			m_nParticularID;

	//����Ʒ�Ƿ����ڱ�ĳ������������ֻӰ��ͻ��˵Ĳ�����������Բ���ֱ��ӳ���������
	BOOL		m_bLocked;

	//�Ƿ�������뱣��
	BOOL		m_bByProtect;	

	//����������
	CHAR		m_Creator[MAX_ITEM_CREATOR_NAME];

	//������Ϣ�����������ַ���ʹ��
	_ITEM*		m_pExtraInfoItem;

	//��Ʒ����
public:
	struct Rule_t
	{
		BOOL		bCanDrop;		// �Ƿ�ɶ���
		BOOL		bCanOverLay;	// �Ƿ���ص�
		BOOL		bCanQuick;		// �Ƿ�ɷ�������
		BOOL		bCanSale;		// �Ƿ���Գ��۸�NPC�̵�
		BOOL		bCanExchange;	// �Ƿ���Խ���
		BOOL		bCanUse;		// �Ƿ����ʹ��
		BOOL		bPickBind;		// �Ƿ���ʰȡ�
		BOOL		bEquipBind;		// �Ƿ���װ���
		BOOL		bUnique;		// �Ƿ���Ψһ

		Rule_t()
		{
			bCanDrop	=	TRUE;		
			bCanOverLay	=	TRUE;		
			bCanQuick	=	TRUE;		
			bCanSale	=	TRUE;		
			bCanExchange=	TRUE;	
			bCanUse		=	TRUE;	
			bPickBind	=	TRUE;		
			bEquipBind	=	TRUE;		
			bUnique		=	TRUE;		
		}
	};

	Rule_t			m_Rule;

protected:
	//����Ψһ��ƷID
	static UINT	_CreateID(VOID);
	static std::map< UINT, CObject_Item* > s_mapAllItem;

protected:
	CObject_Item(UINT id);
	virtual ~CObject_Item();
};

//��Ҫ��ui����ʾ������
extern const char*  ITEMATTRIBUTE_NAME;				//��Ʒ��
extern const char*  ITEMATTRIBUTE_LEVEL;			//��Ʒ�ȼ�
extern const char*  ITEMATTRIBUTE_DAM;				//�𻵶�
extern const char*  ITEMATTRIBUTE_DAMMAX;			//�𻵶����ֵ
extern const char*  ITEMATTRIBUTE_PRICE;			//�۸�
extern const char*  ITEMATTRIBUTE_REPAIRNUM;		//����ʧ�ܴ���
extern const char*  ITEMATTRIBUTE_WHITE_ATT;		//���а�ɫ����
extern const char*  ITEMATTRIBUTE_BLUE_ATT;			//������ɫ����
extern const char*  ITEMATTRIBUTE_GREEN_ATT;		//������ɫ����
extern const char*  ITEMATTRIBUTE_PRODUCER;			//����
extern const char*  ITEMATTRIBUTE_DESC;				//��ϸ����
