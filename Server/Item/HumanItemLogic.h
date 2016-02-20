/********************************************************************
	��������:	2005��11��10��
	����ʱ��:	10:58
	�ļ�����:	HumanItemLogic.h
	�ļ�·��:	d:\Prj\Server\Server\Item\HumanItemLogic.h
	
	�ļ����ܣ� Obj_Human ����Ʒ�����ķ�װ
	�޸ļ�¼��
	
*********************************************************************/

#ifndef _HUMAN_ITEM_LOGIC_H_
#define _HUMAN_ITEM_LOGIC_H_
#include "Type.h"

class Obj_Human;
class Item;
class ItemContainer;
struct _EXCHANGE_ITEM_LIST ;
struct ITEM_LOG_PARAM;

class HumanItemLogic
{
public:
	
	//ͨ����Ʒ���ͱ��,ȡ��Human����Ʒ������
	static	ItemContainer*	GetItemContain(Obj_Human*	pHuman,UINT uItemIndex);
	
	/*
	 *	��������
	 *  ��Human����ɾ��uCount������ΪuItemIndex ����Ʒ
	 */
	static	BOOL			EraseItem(ITEM_LOG_PARAM* pLogParam,Obj_Human*	pHuman,UINT uItemIndex,UINT uCount);

	//��Human����ɾ����uBagIndex λ���ϵ���Ʒ
	static	BOOL			EraseItem(ITEM_LOG_PARAM* pLogParam,Obj_Human*	pHuman,UINT uBagIndex);

	//����Human����uBagIndex λ���ϵ���Ʒ
	static  BOOL			DecItemLayCount(Obj_Human*,UINT uBagIndex,UINT uCount =1);
	//����Human����uBagIndex λ���ϵ���Ʒ,uBagIndex ����һ��Ҫ�ж���
	static  BOOL			IncItemLayCount(Obj_Human*,UINT uBagIndex,UINT uCount =1);
	
	//uItemIndex ��ָ��Ҫ�������ɿռ����Ʒ����
	//Number ��ָ��Ҫ���ɵ� uItemIndex ���͵���Ʒ����
	//NewSpace ���ڱ������� Number �� uItemIndex ���͵���Ʒ��Ҫ���±����ռ�
	static	BOOL			CalcItemSpace(Obj_Human* pHuman,UINT uItemIndex,INT Number,INT& NewSpace);
	
	//���㱳���ո��ܺ�
	static	INT				CalcBagSpace(Obj_Human* pHuman,UINT uItemIndex);
	
	//�����Ƿ��������ItemList
	static	BOOL			CanReceiveItemList(Obj_Human* pHuman,_ITEM_LIST& List);

	//����ItemList
	static	BOOL			ReceiveItemList(ITEM_LOG_PARAM* pLogParam,Obj_Human* pHuman,_ITEM_LIST& List, BYTE nCoinType);

	//���Human����λ��ΪuIndex��Item
	static	Item*			GetItem(Obj_Human*	pHuman,UINT uBagIndex);
	//���Human�����ж�ӦGUID����Ʒλ��
	static	INT				GetItemPosByGUID(Obj_Human* pHuman,_ITEM_GUID guid);
	//���Human�����ж�ӦItemIndex����Ʒλ��
	static	INT				GetItemPosByType(Obj_Human* pHuman,UINT uItemIndex);

	
	//���Human����λ��ΪuIndex����������
	static ItemContainer*	GetContainer(Obj_Human*	pHuman,UINT uBagIndex);

	//���Human������Ʒ���ΪuItemIndex��ͳ�Ƹ���
	static	INT				CalcBagItemCount(Obj_Human* pHuman,UINT uItemIndex);

	//����human������Ʒ���;ö�
	static	BOOL			SetBagItemDur(Obj_Human*	pHuman,UINT uBagIndex, INT iDur);
	//����Human��������Ʒ�ļ�������
	static	BOOL			SetBagItemIdent(Obj_Human*	pHuman,UINT uBagIndex);
	//����Human��������Ʒ�Ĳ���
	static	BOOL			SetBagItemParam(Obj_Human* pHuman,UINT uBagIndex,UINT Start,INT Type,INT value);


	//����human������Ʒ������;ö�
	static	BOOL			SetBagItemMaxDur(Obj_Human*	pHuman,UINT uBagIndex, INT iDur);

	
	//�����Ϸ���uCount������ΪItemIndex ����Ʒ
	static	BOOL			CreateMultiItemToBag(ITEM_LOG_PARAM* pLogParam, Obj_Human*	pHuman,UINT uItemIndex,UINT uCount, BYTE nCoinType);

	//�����Ϸ���һ������ΪuItemIndex ����Ʒ
	static	BOOL			CreateItemToBag(ITEM_LOG_PARAM* pLogParam, Obj_Human*	pHuman,UINT uItemIndex,UINT& uBagIndex,BYTE nCoinType, UINT uQuality = 1);

	//������֮���ƶ���Ʒ�Ĳ���
	static	BOOL			MoveItem(ITEM_LOG_PARAM* pLogParam,Obj_Human*	pHuman,ItemContainer*	pContainer,UINT uIndex,UINT& uBagPos);
	
	//������֮���з���Ʒ

	static BOOL				SplitItem(Obj_Human* pHuman,
									  ItemContainer* pSourContainer, 
									  UCHAR uSourIndex,  
									  UCHAR uCount,  
									  ItemContainer* pDestContainer,
									  INT nDestIndex /* =INVALID_INDEX  */);

	//������֮��ϲ���Ʒ
	static	BOOL			SpliceItem(Obj_Human* pHuman,
									   ItemContainer* pSourContainer, 
									   UCHAR uSourIndex,
									   ItemContainer* pDestContainer,
									   UCHAR uDestIndex);

	/*
	 *	װ������
	 */
	
	/*
	 *	�����Ѿ�װ������Ʒ���;ö�	
	 *
	 *	HUMAN_EQUIP EquipPoint	װ���λ��
	 *
	 *  INT Dur					�;öȵ���
	 */
	static	BOOL			SetEquipDur(Obj_Human*	pHuman,HUMAN_EQUIP EquipPoint,INT iDur);

	/*
	*	�����Ѿ�װ������Ʒ������;ö�	
	*
	*	HUMAN_EQUIP EquipPoint	װ���λ��
	*
	*  INT Dur					�;öȵ���
	*/
	static	BOOL			SetEquipMaxDur(Obj_Human*	pHuman,HUMAN_EQUIP EquipPoint,INT iDur);

	/*	ɾ����ɫ�Ѿ�װ���ϵ���Ʒ
	 *
	 *	EquipPoint װ������
	 */ 
	static	BOOL			EraseEquip(Obj_Human* pHuman,HUMAN_EQUIP	EquipPoint);

	/*
	 *	���װ���ϵ���Ʒ
	 *	EquipPoint װ������
     */
	static	Item*			GetEquip(Obj_Human* pHuman,HUMAN_EQUIP EquipPoint);
	
	
	//���Human����Equip���ΪuItemIndex��ͳ�Ƹ���
	static	INT				CalcEquipItemCount(Obj_Human* pHuman,UINT uItemIndex);


	/*
	 *	���в���
	 */
	static	Item*			GetBankItem(Obj_Human* pHuman,UINT BankIndex);
	
	/*
	*	����Ʒ������
	*/
	static	BOOL			RecieveItemToBag(Obj_Human* pHuman, ItemContainer* pContainer, UINT uIndex );

	/*
	*	�����в�����Ʒ�ܲ��ܱ�����
	*/
	static	BOOL			CanReceiveExchangeItemList(Obj_Human* pHuman,_EXCHANGE_ITEM_LIST& List);	

};


#endif