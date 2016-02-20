/////////////////////////////////////////////////////////////////////////////////
//�ļ�����Item.h
//������������Ʒ���ݵķ�װ�࣬ͨ����������ȡ������һ����Ʒ����Ϣ
//�޸ļ�¼��2005-11-2����
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef __ITEM_H__
#define __ITEM_H__

#include "Type.h"

struct _PET_DB_LOAD;
struct  PET_GUID_t;
struct _PET_SKILL;


class _ITEM_INIT
{
public:
	_ITEM_INIT(const _ITEM*	pItem)
	{
		m_pItem = pItem ;
		m_pPet  = NULL;
	}

	_ITEM_INIT(const _PET_DB_LOAD* pPet)
	{
		m_pPet	= pPet;
		m_pItem = NULL;
	}
	const	_ITEM*				m_pItem ;//��Ʒ��ϸ����
	const	_PET_DB_LOAD*		m_pPet;	 //������ϸ����
};

enum ITEM_DATA_TYPE
{
	IDT_ITEM,
	IDT_PET,
};

struct _CAMP_DATA;
class Item
{
friend class ItemOperator ;
friend class ItemContainer ;



public :
	Item( ) ;
	~Item( ) ;

	ITEM_DATA_TYPE			GetDataType(){ return m_Type;}//ȡ����������

	VOID					SaveValueTo(_ITEM* pItem);
	VOID					SaveValueTo(_PET_DB_LOAD* pPet);
	inline  _ITEM_GUID		GetGUID( ) ;					//ȡ����ƷGUID
	inline  UINT			GetItemTableIndex( ) const; 	//ȡ����Ʒ�������Ʒ�������
	inline	UCHAR			GetItemClass()		 const;		//ȡ����Ʒ��	enum ITEM_CLASS
	inline  UCHAR			GetItemQual()		 const;		//ȡ����Ʒ��Ʒ��
	inline	UCHAR			GetItemType()		 const;		//ȡ����Ʒ��С��WEAPON_TYPE DEFENCE_TYPE ��
	inline	UCHAR			GetItemIndex()		 const;		//ȡ����Ʒ����ˮ��



	BOOL					IsCanEquipBind()	const;		//ȡ����Ʒ�Ƿ���а�����
	BOOL					IsRuler(ITEM_RULER_LIST	ruler)	const;			//ȡ����Ʒ�Ƿ����ִ��ָ������
	
	UINT					GetPrice()	const;
	INT						GetItemParam(UINT start,ItemParamValue& ipv);
	/*=============================================
	 *	װ������
	 =============================================*/
	inline USHORT			GetSetNum()					const;			//��װ���
	inline BYTE				GetMaxSetCount()			const;			//��װ����
	inline BYTE				GetEquipPoint()				const;			//��Ʒװ���
	inline BYTE				GetMaxDurPoint()			const ;			//����;�ֵ
	inline BYTE				GetRequireLevel()			const;			//����ȼ�
	inline BYTE				GetGemSlotMax()				const;			//���ʯ����
	inline BYTE				GetFailTimes()				const;			//����ʧ�ܴ���
	inline BYTE				GetDurPoints()				const; 			//��ǰ�;�ֵ
	inline BYTE				GetAttrCount()				const;			//���Ե�����
	inline USHORT			GetDamagePoint()			const;			//��Ʒ�����ۼ�ֵ
	inline BOOL				GetItemBind()				const;			//��Ʒ�Ƿ��
	inline BOOL				GetItemIdent()				const;			//��Ʒ�Ƿ����
	inline BYTE				GetEquipGemCount()			const;			//��Ƕ��ʯ������
	inline const _ITEM_ATTR		GetEquipAttr(UCHAR	nIndex)		const;		//���װ������
	inline const _ITEM_GEMINFO	GetEquipGemInfo(UCHAR nIndex)	const;		//���װ���ϱ�ʯ�ı����Ϣ

	/*==========================================
	 *	��ͨ��Ʒ����
	 ============================================*/
	
	inline BYTE				GetItemLevel()			const;				//��Ʒ����,�߻�����
	inline BYTE				GetReqSkillLevel()		const;				//��ü��ܼ�������
	inline INT				GetReqSkill()			const;				//��Ҫӵ�еļ��ܺ�
	inline INT				GetScriptID()			const;				//�����ؽű����
	inline INT				GetSkillID()			const;				//�����ؼ��ܱ��
	inline BOOL				IsCosSelf()				const;				//�Ƿ������Լ�
	inline BYTE				GetTargetType()			const;
	
	/*==============================================
	 *��ʯ����	
	 ===============================================*/
	inline const _ITEM_ATTR		GetGemAttr()			const;				//��ʯ����

	/*==========================================
	 *�ر�ͼ����	
	============================================*/

	inline INT				GetMapLevel()			const;				//�ر�ͼ����
	inline FLOAT			GetXPos()				const;				//x����
	inline FLOAT			GetZPos()				const;				//z����
	inline INT				GetSceneID()			const;				//�������
	inline INT				GetGrowType()			const;				//��Ӧ����������
public:
	//ITEM ��PET ���ú���
	
	BOOL					Init( const _ITEM_INIT* pInit );//��ʼ����Ʒ
	BOOL					IsEmpty( ) ;					//�ж���Ʒ�Ƿ�Ϊ�գ���û����Ʒ����
	BOOL					IsEquip( ) const;				//�жϴ���Ʒ�Ƿ�Ϊһ��װ��
	BOOL					IsLock( ){ return m_bLock ; } ; //�жϴ���Ʒ�Ƿ�����
	BOOL					IsCanLay( )			const;		//ȡ����Ʒ�Ƿ���Ե���
	inline	INT				GetLayedNum( )  const;			//ȡ�õ�����Ʒ������
	inline	INT				GetMaxLayedNum( ) const;		//ȡ���ܹ�������Ʒ���������
	inline  BOOL			IsFull()	const;				//�Ƿ��Ѿ��ﵽ��������Ŀ

	inline  BOOL			IsPWLock()	const;				//�����뱣��
public:
	//���ﺯ��

	inline  PET_GUID_t			GetPetGUID( ) ;					//ȡ�ó���GUID
	inline  PET_GUID_t			GetSpouseGUID( ) ;				//ȡ�ó�����żGUID
	inline	INT					GetDataID();					//ȡ�ó���ģ��		
	inline  const CHAR*			GetName()	const;				//ȡ������
	inline  const CHAR*			GetNick() const;				//ȡ���ǳ�
	inline  INT					GetLevel() const;				//ȡ�õȼ�
	inline  INT					GetTakeLevel() const;			//ȡ��Я���ȼ�
	inline  INT					GetAttackType() const;			//ȡ�ý������ͣ���/����
	inline  INT					GetAIType() const;				//ȡ��AI����
	inline  const _CAMP_DATA	*GetCampData() const;			//ȡ����Ӫ
	inline  INT					GetHP() const;					//ȡ������ֵ
	inline  INT					GetBaseMaxHP() const;			//ȡ������ֵ����
	inline  INT					GetLife() const;				//ȡ�õ�ǰ����
	inline  BYTE				GetPetType() const;				//���������죬Ұ��
	inline  BYTE				GetGeneration() const;			//������
	inline  BYTE				GetHappiness() const;			//���ֶ�		
	inline  INT					GetStrPer() const;				//��������	
	inline  INT					GetConPer() const;				//��������	
	inline  INT 				GetDexPer() const;				//������
	inline  INT					GetSprPer() const;				//��������
	inline  INT 				GetIntPer() const;				//��������
	inline	INT					GetGenGu() const;				//����
	inline  FLOAT				GetGrowRate() const ;			//�ɳ���
	inline  INT					GetRemainPoint() const;			//һ������ʣ�����
	inline  INT					GetExp() const;					//����ֵ
	inline  INT					GetLvl1Attr(CHAR_ATTR_LEVEL1 type)const;// ����һ��ս�����ԣ����������ܺ�װ�����ӵĲ��֣�
	inline  _PET_SKILL			GetSkill(UINT SkillIndex);		// ���＼��
	inline  VOID				SetCooldown(CooldownID_t nID, Time_t nTime);
	inline	BOOL				IsCooldowned(CooldownID_t nID) const;
	inline	VOID				HeartBeat_Cooldown(Time_t nDeltaTime);
	inline	Time_t				GetCooldownRemained(CooldownID_t nID) const;
	inline	VOID				SetCoinType(BYTE nType) {m_nCoinType = nType;}
	inline	BYTE				GetCoinType(VOID) { return m_nCoinType; }

	/*==============================================
	*���ܽӿ�
	===============================================*/
	//����
	BOOL						IsInExchange(){return m_bIsInExchange;}
	VOID						SetInExchange(BOOL bIsInExchange){m_bIsInExchange = bIsInExchange;}

protected :
	VOID					Lock( ){ m_bLock = TRUE ; }
	VOID					Unlock( ){ m_bLock = FALSE ; }
	VOID					SetItemValue(_ITEM* pItem);
	VOID					CleanUp( ) ;//�����Ʒ����


private :
	ITEM_DATA_TYPE			m_Type;
	_ITEM*					m_pItem ;					//��Ʒ��ϸ����
	_PET_DB_LOAD*			m_pPet;						//
	BOOL					m_bLock ;
	BOOL					m_bIsInExchange;			//��Ǵ���Ʒ���ڱ��ŵ���������
	BYTE					m_nCoinType;				//��������
};

#include "Item.inl"

struct _EXCHANGE_ITEM_LIST 
{
	INT				m_ListCount;
	Item*			m_ListItem[EXCHANGE_BOX_SIZE+EXCHANGE_PET_BOX_SIZE];

	_EXCHANGE_ITEM_LIST()
	{
		memset(this,0,sizeof(_EXCHANGE_ITEM_LIST));
	}

	VOID	AddItem(Item* it)
	{
		INT i;

		for(i=0;i<EXCHANGE_BOX_SIZE+EXCHANGE_PET_BOX_SIZE;i++)
		{
			if(m_ListItem[i] == NULL)
			{
				break;
			}
		}

		if(i == EXCHANGE_BOX_SIZE+EXCHANGE_PET_BOX_SIZE)
		{
			Assert(0);
			return;
		}

		m_ListCount++;
		m_ListItem[i] = it;
	}

	VOID Init()
	{
		memset(this,0,sizeof(_EXCHANGE_ITEM_LIST));
	}
};



#endif
