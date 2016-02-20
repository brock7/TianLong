#include "stdafx.h"
#include "CGPackage_SwapItem.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"
#include "ItemOperator.h"
#include "HumanItemLogic.h"
#include "GCPackage_SwapItem.h"

#include "GCItemInfo.h"

UINT CGPackage_SwapItemHandler :: Execute( CGPackage_SwapItem* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION
	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR;
	}

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	//�õ���Ҫ��������������
	INT index1 = pPacket->GetPackageIndex1();
	INT index2 = pPacket->GetPackageIndex2();

	//�ж��Ƿ���ͬһ��������
	ItemContainer* pBagContainer = HumanItemLogic::GetContainer(pHuman, index1);
	if(pBagContainer != HumanItemLogic::GetContainer(pHuman, index2)) return PACKET_EXE_CONTINUE;

	//������Ʒ�ײ㽻����Ʒ
	Item* pItem1 = HumanItemLogic::GetItem(pHuman, index1);
	Item* pItem2 = HumanItemLogic::GetItem(pHuman, index2);



	//ת��Index����
	INT index_container1 = pBagContainer->BagIndex2ConIndex(index1);
	INT index_container2 = pBagContainer->BagIndex2ConIndex(index2);

	BOOL bSucc = FALSE;
	//������Ʒ���ǿո�
	if(pItem1->IsEmpty() && pItem2->IsEmpty())
	{
		return PACKET_EXE_CONTINUE;
	}
	// Item1 -> Item2(EMPTY)
	else if(!(pItem1->IsEmpty()) && pItem2->IsEmpty())
	{
		bSucc = g_ItemOperator.MoveItem(pBagContainer, index_container1, pBagContainer, index_container2) >= 0;
	}
	// Item1(EMPTY) <- Item2
	else if(pItem1->IsEmpty() && !(pItem2->IsEmpty()))
	{
		bSucc = g_ItemOperator.MoveItem(pBagContainer, index_container2, pBagContainer, index_container1) >= 0;
	}
	// Item1 <-> Item2
	else
	{
		ITEM_LOG_PARAM	ItemLogParam;

		//�ϲ���ͬ����Ʒ
		//1���ж�������Ʒ�ǲ��ǿ��Ե��ӵ�
		if(pItem1->GetItemTableIndex() == pItem2->GetItemTableIndex() && pItem1->IsCanLay())
		{
			int Count  = pItem1->GetLayedNum();
			//2��ת����Ʒ
			if(g_ItemOperator.MoveItem(pBagContainer, index_container1, index_container2) >= 0)
			{
				//�ϲ��ɹ��������֪ͨ�ͻ��ˣ�����ֻ����Index����Ҫ�������ݣ�
				_ITEM temp1;
				pItem1->SaveValueTo(&temp1);
				GCItemInfo msg1;
				msg1.setID(index1);
				msg1.setIsNull(pItem1->IsEmpty());
				msg1.setItem(&temp1);
				pGamePlayer->SendPacket(&msg1);

				_ITEM temp2;
				pItem2->SaveValueTo(&temp2);
				GCItemInfo msg2;
				msg2.setID(index2);
				msg2.setIsNull(pItem2->IsEmpty());
				msg2.setItem(&temp2);
				pGamePlayer->SendPacket(&msg2);


				ITEM_LOG_PARAM	ItemLogParam;
				ItemLogParam.OpType		= ITEM_TILED_BAG_DEST;
				ItemLogParam.CharGUID	= pHuman->GetGUID();
				ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
				ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
				ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
				ItemLogParam.BagPos		= index2;
				ItemLogParam.Count		= Count;
				ItemLogParam.ItemGuid	= pItem2->GetGUID();	
				SaveItemLog(&ItemLogParam);


				ItemLogParam.OpType		= ITEM_TILED_BAG_DEST;
				ItemLogParam.CharGUID	= pHuman->GetGUID();
				ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
				ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
				ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
				ItemLogParam.BagPos		= index1;
				ItemLogParam.Count		= Count;
				ItemLogParam.ItemGuid	= pItem1->GetGUID();	
				SaveItemLog(&ItemLogParam);

				return PACKET_EXE_CONTINUE ;
			}
		}

		// ��������Ʒ
		bSucc = g_ItemOperator.ExchangeItem(pBagContainer, index_container1, pBagContainer, index_container2) == ITEMOE_SUCCESS;
	}

	GCPackage_SwapItem msg;
	msg.SetResult(bSucc?1:0);
	msg.SetPackageIndex1(index1);
	msg.SetPackageIndex2(index2);

	pGamePlayer->SendPacket(&msg);
	g_pLog->FastSaveLog( LOG_FILE_1, "CGPackage_SwapItem: index1=%d, index2=%d Succ=%s", 
		index1, index2, bSucc?"TRUE":"FALSE" ) ;


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

