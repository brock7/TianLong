/////////////////////////////////////////////////////////////////////////////////
//�ļ�����ItemContainer.h
//������������Ʒ�����ķ�װ�࣬ͨ����������ȡ�����ö����Ʒ�������Ϣ
//�޸ļ�¼��2005-11-2����
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __ITEMCONTAINER_H__
#define __ITEMCONTAINER_H__

#include "Type.h"
#include "Item.h"

#define MAX_ITEMCONTAINER_SIZE 128

struct _ITEMCONTAINER_INIT
{
	INT						m_nContainerSize ;//������С
	
};



class ItemContainer
{
friend class ItemOperator ;
	//ItemContainerType
	
public :
	ItemContainer( ) ;
	virtual ~ItemContainer( ) ;

	/*
	 *	ITEM ʹ�÷���
	 */

	//ͨ����Ʒ��GUIDȡ�ô���Ʒ�������е�λ��
	INT					GetIndexByGUID( const _ITEM_GUID* pGuid ) ;
	//ͨ�������GUIDȡ�ô���Ʒ�������е�λ��
	INT					GetIndexByGUID( const PET_GUID_t* pGuid ) ;
	//ͨ����Ʒ������ȡ�ô���Ʒ�������е�λ��
	INT					GetIndexByType( const UINT uType ) ;
	//ͨ����Ʒ������ȡ�ô���Ʒ�������е�λ��
	INT					GetIndexByType( const UINT uType ,UINT emptyCount) ;

	//ͨ��������λ��ȡ����Ʒ
	Item*				GetItem( const INT nIndex ) ;

	

	
	//ȡ��һ��û������Ʒλ��,���INVALID_INDEX ���ʾû�ҵ�
	INT					GetNoFullItemIndex( UINT uItemIndex);
	//������ŵ�Bag���
	virtual		UINT	ConIndex2BagIndex(UINT uConIndex){return uConIndex;}
	//������ŵ��������
	virtual		UINT	BagIndex2ConIndex(UINT uBagIndex){return uBagIndex;}
	//����һ��nIndex�ǲ��������Container��
	virtual		BOOL	IsInContainer(UINT uBagIndex);

	
public:
	/*
	 *	PET �� ITEM ���÷���
	 */

	virtual BOOL		Init( const _ITEMCONTAINER_INIT* pInit ) ;//��ʼ������

	VOID				CleanUp( ) ;//��������ڵ�������Ʒ����
	//���������ո����
	virtual		UINT	CalcItemSpace();	
	//�����м��������ո����
	//����ʱ����Ʒ����������ס������ռ�ʱ���Ա���������ס����Ʒ��������Ʒ�Ϳ��Խ����ˡ�
	UINT				CalcExchangeItemSpace();	
	//ȡ��һ���յ���Ʒλ��, �������INVALID_INDEX��������
	INT					GetEmptyItemIndex( ) ;
	//ȡ��������С
	INT					GetContainerSize( ){ return m_nContainerSize ; }

	BOOL				SetItemPWLock(INT nIndex,BOOL bLock);

public:
	/*
	 *	PET ʹ�÷���
	 */

protected :
	//����Ʒ���õ���Ӧ����λ��
	virtual BOOL		SetItem( const INT nIndex, const Item* pItem );

	//�ı�Container �Ĵ�С
	BOOL				ChangeSize(const INT nSize);

	//������Ʒ�ص�����
	virtual VOID		SetItemLayCount(INT nIndex, INT nCount) ;
	//������Ʒ�ص�����
	virtual BOOL		DecItemLayCount(INT nIndex, INT nCount=1) ;
	//������Ʒ�ص�����
	virtual BOOL		IncItemLayCount(INT nIndex, INT nCount=1);
	//ɾ����Ʒ
	virtual	BOOL		EraseItem(UINT uIndex);
	//������Ʒ�;ö�
	virtual	BOOL		SetItemDur(INT nIndex, INT nDur);
	//������Ʒ���˵�
	virtual	BOOL		SetItemDamagePoint(INT nIndex, INT nPoint);
	//������Ʒ�;ö�
	virtual	BOOL		SetItemMaxDur(INT nIndex, INT nDur);
	//������Ʒ����
	virtual	BOOL		SetItemValue(INT nIndex,_ITEM*	pItem);
	//������Ʒ�ﶨ
	virtual	BOOL		SetItemBind(INT nIndex);
	//������Ʒ������Ϣ
	virtual BOOL		SetItemIdent(INT nIndex);
	//������Ʒ�������
	virtual BOOL		SetItemFailTimes(INT nIndex, INT nTimes);

	//������Ʒ����
	virtual BOOL		AddItemAttr(INT nIndex,_ITEM_ATTR iA);
	//ɾ����Ʒ����
	virtual	BOOL		DelItemAttr(INT nIndex,_ITEM_ATTR iA);
	//ɾ����ʯ��Ϣ
	virtual BOOL		DelGemInfo(INT nIndex,INT GemIndex);
	//��ӱ�ʯ��Ϣ
	virtual	BOOL		AddGemInfo(INT nIndex,INT& GemIndex,UINT GemType);
	//������ƷGUID
	virtual BOOL		SetItemGuid(INT nIndex,_ITEM_GUID* pGUID);
	//������ƷGUID
	virtual BOOL		SetItemGuid(INT nIndex,PET_GUID_t* pGUID);
	//������
	virtual	BOOL		SetItemCreator(INT nIndex,const CHAR* CreatorName);
	//��Ʒ����
	virtual BOOL		SetItemParam(INT nIndex,UINT start,INT type,INT value);
	
	//����PET ����
	//������Ʒ����
	virtual	BOOL		SetItemValue(INT nIndex,const _PET_DB_LOAD*	pPet);



	virtual BOOL  		SetPetGUID(INT nIndex,PET_GUID_t GUID) ;				//���ó���GUID
	virtual BOOL  		SetSpouseGUID(INT nIndex,PET_GUID_t GUID) ;				//���ó�����żGUID
	virtual BOOL		SetDataID(INT nIndex,INT ID);							//���ó���ģ��		
	virtual BOOL 		SetName(INT nIndex,const CHAR* pName);					//��������
	virtual BOOL   		SetNick(INT nIndex,const CHAR* pNick);					//�����ǳ�
	virtual BOOL  		SetLevel(INT nIndex,INT level);							//���õȼ�
	virtual BOOL  		SetTakeLevel(INT nIndex,INT takeLevel);					//����Я���ȼ�
	virtual BOOL  		SetAttackType(INT nIndex,INT attackType);				//���ý������ͣ���/����
	virtual BOOL  		SetAIType(INT nIndex,INT AIType);						//����AI����
	virtual BOOL  		SetCampData(INT nIndex,const _CAMP_DATA* pCamp);		//������Ӫ
	virtual BOOL  		SetHP(INT nIndex,INT hp);								//��������ֵ
	virtual BOOL  		SetLife(INT nIndex,INT Life);							//���õ�ǰ����
	virtual BOOL  		SetPetType(INT nIndex,BYTE PetType);					//���������죬Ұ��
	virtual BOOL  		SetGeneration(INT nIndex,BYTE Gen);						//������
	virtual BOOL  		SetHappiness(INT nIndex,BYTE byHappiness);				//���ֶ�		
	virtual BOOL  		SetStrPer(INT nIndex,INT strper);						//��������	
	virtual BOOL  		SetConPer(INT nIndex,INT conper);						//��������	
	virtual BOOL   		SetDexPer(INT nIndex,INT dexper);						//������
	virtual BOOL  		SetSprPer(INT nIndex,INT sprper);						//��������
	virtual BOOL   		SetIntPer(INT nIndex,INT intper);						//��������
	virtual BOOL		SetGenGu(INT nIndex,INT gengu);							//����
	virtual BOOL  		SetGrowRate(INT nIndex,FLOAT rate);						//�ɳ���
	virtual BOOL  		SetRemainPoint(INT nIndex,INT rPoint);					//һ������ʣ�����
	virtual BOOL  		SetExp(INT nIndex,INT exp) ;							//����ֵ
	virtual BOOL  		SetLvl1Attr(INT nIndex,CHAR_ATTR_LEVEL1 type,INT value);//����һ��ս�����ԣ����������ܺ�װ�����ӵĲ��֣�
	virtual BOOL  		SetSkill(INT nIndex,UINT SkillIndex,_PET_SKILL skill);	//���＼��
	
	INT					m_nContainerSize ;					//������С
private :
	
	
	Item				m_aItem[MAX_ITEMCONTAINER_SIZE] ;	//��������Ʒ�ռ�
	
};

#endif
