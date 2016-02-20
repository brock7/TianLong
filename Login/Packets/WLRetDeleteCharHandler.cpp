#include "stdafx.h"

#include "WLRetDeleteChar.h"
#include "ProcessManager.h"
#include "ProcessPlayerManager.h"
#include "ServerManager.h"
#include "LoginPlayer.h"
#include "LCRetCharLogin.h"
#include "PlayerPool.h"
#include "TimeManager.h"
#include "DBLogicManager.h"
#include "DBCharFullData.h"
#include "DB_Struct.h"
#include "LWAskCharLogin.h"
#include "PacketFactoryManager.h"
#include "Log.h"
#include "DBThreadManager.h"
#include "LCRetCharList.h"
#include "LCRetDeleteChar.h"
#include "DBDeleteCharOp.h"
#include "DBCharList.h"


UINT WLRetDeleteCharHandler::Execute(WLRetDeleteChar* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	
		TID CurrentThreadID = MyGetCurrentThreadID();

		if(CurrentThreadID == g_pServerManager->m_ThreadID)
		{
			if (g_pDBThreadManager->SendPacket(pPacket,pPacket->GetPlayerID()))
			{
				return PACKET_EXE_NOTREMOVE;
			}
			else
			{//DB ѹ���������û����³���
				LCRetDeleteChar* pMsg = (LCRetDeleteChar*)g_pPacketFactoryManager->CreatePacket(PACKET_LC_RETDELETECHAR);
				pMsg->SetAccount(pPacket->GetAccount());
				pMsg->SetResult(ASKDELETECHAR_SERVER_BUSY);
				g_pProcessManager->SendPacket(pMsg,pPacket->GetPlayerID());
				Log::SaveLog( LOGIN_LOGFILE, "WLRetDeleteCharHandler....Fails!, code = ASKDELETECHAR_SERVER_BUSY") ;
				return PACKET_EXE_CONTINUE;
			}
		}
		else if(g_pDBThreadManager->IsPoolTID(CurrentThreadID))
		{
			PlayerID_t	PlayerID	= pPacket->GetPlayerID();
			LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);
			Assert(pLoginPlayer);

			ASKDELETECHAR_RESULT	Result;
			LCRetDeleteChar* pMsg = (LCRetDeleteChar*)g_pPacketFactoryManager->CreatePacket(PACKET_LC_RETDELETECHAR);
			if(!pMsg)
			{
				AssertEx(FALSE,"���� LCRetDeleteChar ��Ϣʧ��");
			}

			//�������Ƿ�����
			if( pPacket->GetResult() == ASKDELETECHAR_ONLINE )
			{
				pMsg->SetAccount(pPacket->GetAccount());
				pMsg->SetResult(ASKDELETECHAR_ONLINE);
				g_pProcessManager->SendPacket(pMsg,PlayerID);
				Log::SaveLog( LOGIN_LOGFILE, "WLRetDeleteCharHandler::Execute()....�����������Ϸ!") ;
				return PACKET_EXE_CONTINUE;
			}

			//
			if(pLoginPlayer->GetDBOperating() == TRUE)
			{
				pMsg->SetAccount(pPacket->GetAccount());
				pMsg->SetResult(ASKDELETECHAR_SERVER_BUSY);
				g_pProcessManager->SendPacket(pMsg,PlayerID);
				Log::SaveLog( LOGIN_LOGFILE, "WLRetDeleteCharHandler::Execute()....���ݿ������ͻ!") ;
				return PACKET_EXE_NOTREMOVE;
			}

			//���GUID �Ƿ���ȷ
			if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount())!= 0)
			{
				//Ӧ����һ�δ������
				Log::SaveLog(LOGIN_LOGFILE, "ERROR: CLAskDeleteChar DBOperation Errors,acc = %s,Packet acc = %s",
					pLoginPlayer->GetAccount(),pPacket->GetAccount()) ;
				return PACKET_EXE_CONTINUE;
			}

			pLoginPlayer->SetDBOperating(TRUE);

			LCRetCharList* pRetListMsg = (LCRetCharList*)g_pPacketFactoryManager->CreatePacket(PACKET_LC_RETCHARLIST)	;
			if(!pRetListMsg)
			{

				AssertEx(FALSE,"���� LCRetCharList ��Ϣʧ��");
			}

			INT CharNumber = pLoginPlayer->GetCharNumber();
			if(CharNumber==0)
			{
				pRetListMsg->SetAccount(pPacket->GetAccount());
				Result = ASKDELETECHARR_EMPTY; //��ɫ����
			}
			else if(CharNumber ==-1)
			{
				pRetListMsg->SetAccount(pPacket->GetAccount());
				Result = ASKDELETECHAR_OP_ERROR;
			}
			else
			{

				ODBCInterface*	pInterface = g_pDBThreadManager->GetInterface(CurrentThreadID);
				Assert(pInterface);

				//ɾ����¼
				DBDeleteCharOp DeleteCharOp(pInterface);
				DeleteCharOp.SetAccount(pPacket->GetAccount());
				DeleteCharOp.SetCharGuid(pPacket->GetPlayerGUID());
				BOOL bRetDelete = 	DeleteCharOp.Delete();
				if(!bRetDelete)
				{
					Log::SaveLog(LOGIN_LOGFILE,"DeleteCharOp.Delete()....Get Errors: %s ",DeleteCharOp.GetErrorMessage());
					INT ErrorCode = DeleteCharOp.GetErrorCode();
					pMsg->SetAccount(pPacket->GetAccount());
					pMsg->SetResult(ASKDELETECHAR_INTERNAL_ERROR);
					g_pProcessManager->SendPacket(pMsg,PlayerID);
					pLoginPlayer->SetDBOperating(FALSE);
					Log::SaveLog( LOGIN_LOGFILE, "WLRetDeleteCharHandler::Execute()....Fails! code = ASKDELETECHAR_INTERNAL_ERROR \
													CharGUID = %d,Account = %s",pPacket->GetPlayerGUID(),pPacket->GetAccount()) ;
					return PACKET_EXE_NOTREMOVE;

				}
				DeleteCharOp.ParseResult(&Result);

				//�����µĽ�ɫ�б�
				DBCharList	CharListObject(pInterface);
				CharListObject.SetAccount(pPacket->GetAccount());
				BOOL bRetLoad = 	CharListObject.Load();
				if(bRetLoad)
				{

					CharListObject.ParseResult(pRetListMsg->GetCharBaseInfo());
					pRetListMsg->SetAccount(pPacket->GetAccount());
					pRetListMsg->SetResult(ASKCHARLIST_SUCCESS);
					pRetListMsg->SetCharNumber(CharListObject.GetCharNumber());

					if(CharListObject.GetCharNumber()<(UINT)CharNumber)
					{
						Result = ASKDELETECHAR_SUCCESS;
					}
					else
					{
						Result = ASKDELETECHAR_INTERNAL_ERROR;
					}
					pLoginPlayer->SetCharNumber(CharListObject.GetCharNumber());

					for(INT i=0;i<pLoginPlayer->GetCharNumber();i++)
					{
						pLoginPlayer->SetCharGUID(pRetListMsg->GetCharBaseInfo(i)->m_GUID,i);
					}
				}
				else //����ʧ�ܣ������ǶϿ�������
				{
					Log::SaveLog(LOGIN_LOGFILE,"CharListObject.Load()....Get Errors: %s ",
						CharListObject.GetErrorMessage());
					pRetListMsg->SetAccount(pPacket->GetAccount());
					pRetListMsg->SetResult(ASKCHARLIST_OP_FAILS);
					pRetListMsg->SetCharNumber(CharListObject.GetCharNumber());
				}

			}


			pMsg->SetAccount(pPacket->GetAccount());
			pMsg->SetResult(Result);
			g_pProcessManager->SendPacket(pMsg,PlayerID);
			g_pProcessManager->SendPacket(pRetListMsg,PlayerID);
			pLoginPlayer->SetDBOperating(FALSE);


			Log::SaveLog( LOGIN_LOGFILE, "WLRetDeleteCharHandler::Execute()....OK!CharGUID = %X,Account = %s",
				pPacket->GetPlayerGUID(),pPacket->GetAccount()) ;
			return PACKET_EXE_NOTREMOVE;
		}

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}