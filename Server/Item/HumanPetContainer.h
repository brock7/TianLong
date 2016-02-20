
#ifndef _HUMAN_PET_CONTAINER_H_
#define _HUMAN_PET_CONTAINER_H_
#include "ItemContainer.h"
#include "DB_Struct.h"

class HumanDB;
struct _PET_DB_LOAD;

struct _HUMAN_PETCONTAINER_INIT: 
	public _ITEMCONTAINER_INIT
{
	HumanDB*				m_pHumanDB;
	ITEM_CONTAINER_TYPE		m_ICTType;
};

class Obj_Human;

class HumanPetContainer:public ItemContainer
{
	HumanDB*				m_pHumanDB;
	ITEM_CONTAINER_TYPE		m_ICTType;
public:
	virtual		BOOL		Init( const _ITEMCONTAINER_INIT* pInit ) ;//��ʼ������
protected :
	//������Ʒ������
	virtual		BOOL		SetItem( const INT nIndex, const Item* pItem );
	//ɾ����Ʒ
	virtual		BOOL		EraseItem(UINT uIndex);
	//������Ʒ����
	virtual	BOOL			SetItemValue(INT nIndex,const _PET_DB_LOAD* pPet);


	virtual BOOL  			SetPetGUID(INT nIndex,PET_GUID_t GUID) ;				//���ó���GUID
	virtual BOOL  			SetSpouseGUID(INT nIndex,PET_GUID_t GUID) ;			//���ó�����żGUID
	virtual BOOL			SetDataID(INT nIndex,INT ID);							//���ó���ģ��		
	virtual BOOL 			SetName(INT nIndex, const CHAR* pName);						//��������
	virtual BOOL   			SetNick(INT nIndex, const CHAR* pNick);						//�����ǳ�
	virtual BOOL  			SetLevel(INT nIndex,INT level);							//���õȼ�
	virtual BOOL  			SetTakeLevel(INT nIndex,INT takeLevel);					//����Я���ȼ�
	virtual BOOL  			SetAttackType(INT nIndex,INT attackType);				//���ý������ͣ���/����
	virtual BOOL  			SetAIType(INT nIndex,INT AIType);						//����AI����
	virtual BOOL  			SetCampData(INT nIndex,const _CAMP_DATA* pCamp);		//������Ӫ
	virtual BOOL  			SetHP(INT nIndex,INT hp);								//��������ֵ
	virtual BOOL  			SetLife(INT nIndex,INT Life);							//���õ�ǰ����
	virtual BOOL  			SetPetType(INT nIndex,BYTE PetType);					//���������죬Ұ��
	virtual BOOL  			SetGeneration(INT nIndex,BYTE Gen);						//������
	virtual BOOL  			SetHappiness(INT nIndex,BYTE byHappiness);						//���ֶ�		
	virtual BOOL  			SetStrPer(INT nIndex,INT strper);						//��������	
	virtual BOOL  			SetConPer(INT nIndex,INT conper);						//��������	
	virtual BOOL   			SetDexPer(INT nIndex,INT dexper);						//������
	virtual BOOL  			SetSprPer(INT nIndex,INT sprper);						//��������
	virtual BOOL   			SetIntPer(INT nIndex,INT intper);						//��������
	virtual BOOL			SetGenGu(INT nIndex,INT gengu);							//����
	virtual BOOL  			SetGrowRate(INT nIndex,FLOAT rate);						//�ɳ���
	virtual BOOL  			SetRemainPoint(INT nIndex,INT rPoint);					//һ������ʣ�����
	virtual BOOL  			SetExp(INT nIndex,INT exp) ;							//����ֵ
	virtual BOOL  			SetLvl1Attr(INT nIndex,CHAR_ATTR_LEVEL1 type,INT value);//����һ��ս�����ԣ����������ܺ�װ�����ӵĲ��֣�
	virtual BOOL  			SetSkill(INT nIndex,UINT SkillIndex,_PET_SKILL skill);	//���＼��
 
private:
	BOOL					SetDBDirty();
};



#endif