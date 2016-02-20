#include "stdafx.h"
/*
�ͻ��������̯
*/

#include "CGPlayerShopAskForRecord.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "RecordOpt.h"
#include "GCPlayerShopRecordList.h"

UINT CGPlayerShopAskForRecordHandler::Execute( CGPlayerShopAskForRecord* pPacket, Player* pPlayer )
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
	
	_PLAYERSHOP_GUID	nShopID	 = pPacket->GetShopID();
	BYTE				bType	 = pPacket->GetType();
	BYTE				bPage	 = pPacket->GetPage();


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


	RecordContainer_t*	pRecContainer = NULL;

	//�����׼�¼
	if(bType == CGPlayerShopAskForRecord::TYPE_EXCHANGE_RECORD)
	{
		pRecContainer = pPlayerShop->GetExchangeRecord();
	}
	//��������¼
	else if(bType == CGPlayerShopAskForRecord::TYPE_MANAGER_RECORD)
	{
		pRecContainer = pPlayerShop->GetManagerRecord();
	}

	GCPlayerShopRecordList::MessageEntry_t MessageList[MAX_RECORD_NUM_PER_PAGE];
	CHAR* pszMes = NULL; 
	
	RecordOpt::BeginEnumEntry(pRecContainer);

	for(INT i = 0; i<MAX_RECORD_NUM_PER_PAGE*bPage; i++)
	{
		pszMes = RecordOpt::EnumEntry(pRecContainer);
	}

	//��ҳ�ǿյ�
	if(pszMes == NULL && bPage != 0)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopAskForRecordHandler::ObjName=%s, this page is empty"
			,pHuman->GetName());
		return PACKET_EXE_CONTINUE ;
	}

	INT k = 0;
	for(INT i = 0; i<MAX_RECORD_NUM_PER_PAGE; i++)
	{
		pszMes = RecordOpt::EnumEntry(pRecContainer);
		if(pszMes)
		{
			memcpy(MessageList[k++].szContent, pszMes, MAX_RECORD_LEN_ENTRY);
		}
	}

	RecordOpt::EndEnumEntry(pRecContainer);

	GCPlayerShopRecordList MsgToClient;
	MsgToClient.SetShopID(nShopID);
	MsgToClient.SetEntryNum(k);
	MsgToClient.SetEntryList(MessageList);
	MsgToClient.SetPage((INT)bPage);

	pGamePlayer->SendPacket(&MsgToClient);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopAskForRecordHandler::ObjName=%s"
		,pHuman->GetName());

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
