#include "stdafx.h"

#include "CGPlayerShopSize.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCPlayerShopError.h"
#include "GCPlayerShopStallStatus.h"

UINT CGPlayerShopSizeHandler::Execute( CGPlayerShopSize* pPacket, Player* pPlayer )
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
		return PACKET_EXE_ERROR ;
	}
	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
	_PLAYERSHOP_GUID	nShopID		=	pPacket->GetShopID();		//�̵�ID
	BYTE				nOpt		=	pPacket->GetOpt();			//����

	PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
	PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
	Assert(pPlayerShop);

	//�ǲ����Լ��ĵ�.ֻ�е������ʸ���Ĵ�����
	BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;
	if(bIsMine == FALSE )
	{
		//log...
		return PACKET_EXE_CONTINUE;
	}

	if(pPlayerShop->GetShopStatus() == STATUS_PLAYER_SHOP_ON_SALE)
	{//�̵�δ����
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopNameHandler::Name=%s shop close"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;
	}

	//	���������ϵĽ�Ǯ<30��*��ҵָ��*2*103%��������Ŀʧ����Ϣ�������ϵ��ʽ�����֧�����Ź�̨�ķ��á���ͬʱ�رա����ŵ���ȷ�ϡ�
	//	���������ϵĽ�Ǯ��30��*��ҵָ��*2*103%�����ųɹ���
	FLOAT fNeedMoney = 0.0;
	if(nOpt == CGPlayerShopSize::TYPE_EXPAND)
	{
		pPlayerShopManager->ClampComFactor();
		FLOAT fNeedMoney = (FLOAT)(300000.0*pPlayerShopManager->GetComFactor()*2.00*1.03);
		if(pHuman->GetMoney()<(UINT)fNeedMoney)
		{
			//֪ͨ�ͻ��˹����̵�ɹ�
			GCPlayerShopError Msg;
			Msg.SetID(PLAYERSHOP_MSG::ERR_SHOP_NOT_ENOUTH_MONEY_TO_EXPAND);
			pHuman->GetPlayer()->SendPacket(&Msg);
			return PACKET_EXE_CONTINUE ;
		}
	}

	BYTE	iCurStallNum = pPlayerShop->GetNumStallOpened();
	BYTE	iNewStallIndex = 0; 
	GCPlayerShopStallStatus Msg;

	if(nOpt == CGPlayerShopSize::TYPE_EXPAND)
	{
		if(iCurStallNum == MAX_STALL_NUM_PER_SHOP)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopSizeHandler::Name=%s, No More Stall!!"
				,pHuman->GetName()) ;
			return PACKET_EXE_CONTINUE ;
		}
		//���ÿ��ŵĹ�̨��
		pPlayerShop->SetNumStallOpened(iCurStallNum+1);
		PlayerStallBox*		pNewPlayerStallBox	= pPlayerShop->GetPlayerStallBoxByIndex(iCurStallNum);
		pNewPlayerStallBox->CleanUp();
		pNewPlayerStallBox->SetStallStatus(PLAYER_SHOP::STALL_CLOSE);
		iNewStallIndex = iCurStallNum;

		Msg.SetShopID(nShopID);
		Msg.SetStallIndex(iNewStallIndex);
		Msg.SetStatus(PLAYER_SHOP::STALL_CLOSE);
		Msg.SetFinalStallNum(iCurStallNum+1);

	}
	else if(nOpt == CGPlayerShopSize::TYPE_SHRINK)
	{
		if(iCurStallNum == 0)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopSizeHandler::Name=%s, Already Empty!!"
				,pHuman->GetName()) ;
			return PACKET_EXE_CONTINUE ;
		}

		//���ÿ��ŵĹ�̨��
		pPlayerShop->SetNumStallOpened(iCurStallNum-1);
		PlayerStallBox*		pNewPlayerStallBox	= pPlayerShop->GetPlayerStallBoxByIndex(iCurStallNum-1);
		pNewPlayerStallBox->SetStallStatus(PLAYER_SHOP::STALL_INVALID);
		iNewStallIndex = iCurStallNum-1;

		Msg.SetShopID(nShopID);
		Msg.SetStallIndex(iNewStallIndex);
		Msg.SetStatus(PLAYER_SHOP::STALL_INVALID);
		Msg.SetFinalStallNum(iCurStallNum-1);
	}

	//	�۳���Ǯ��
	//	30��*��ҵָ��*2*103%/2���뱾��
	//	�����Ϊϵͳ��ȡ
	pHuman->SetMoney(pHuman->GetMoney() - (UINT)fNeedMoney);
	pPlayerShop->SetBaseMoney((UINT)(pPlayerShop->GetBaseMoney() + fNeedMoney/2));
	
	pHuman->GetPlayer()->SendPacket(&Msg);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopSizeHandler::Name=%s"
		,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
