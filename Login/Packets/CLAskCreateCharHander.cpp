#include "stdafx.h"

#include "CLAskCreateChar.h"
#include "ProcessPlayerManager.h"
#include "LoginPlayer.h"
#include "TimeManager.h"
#include "DBLogicManager.h"
#include "LCRetCreateChar.h"
#include "DBCreateCharOp.h"
#include "PacketFactoryManager.h"
#include "ProcessManager.h"
#include "LCRetCharList.h"
#include "DBCharList.h"
#include "DBCharFullData.h"
#include "DB_Struct.h"
#include "Log.h"
#include "CharConfig.h"
#include "DBThreadManager.h"



UINT	CLAskCreateCharHandler::Execute(CLAskCreateChar* pPacket, Player* pPlayer)
{
	__ENTER_FUNCTION

	TID CurrentThreadID = MyGetCurrentThreadID();

	if(CurrentThreadID == g_pProcessPlayerManager->m_ThreadID) 
	{
		LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);
		Assert(pLoginPlayer);

		if( !StrSafeCheck( pPacket->GetAccount(), MAX_ACCOUNT ) )
		{
			Log::SaveLog(LOGIN_LOGFILE, "ERROR: CLAskDeleteCharHandler Get IllegalString,acc = %s",
				pLoginPlayer->GetAccount()) ;
			return PACKET_EXE_ERROR;
		}

		if( CheckIllegalString( pPacket->GetName(), MAX_CHARACTER_NAME, CHECK_STRING_CHARNAME ) )
		{
			LCRetCreateChar Msg;
			Msg.SetAccount(pPacket->GetAccount());
			Msg.SetResult(ASKCREATECHAR_INVALID_NAME);

			//����DB����Ƶ����Ϣ
			pLoginPlayer->SendPacket(&Msg);

			Log::SaveLog( LOGIN_LOGFILE, "CLAskCreateCharHandler....Fails!, code = ASKCREATECHAR_INVALID_NAME") ;

			return PACKET_EXE_CONTINUE;
		}

		if(pLoginPlayer->GetPlayerStatus()!=PS_LOGIN_NORMAL)
		{
			//û��Ȩ�ޣ�ʲôҲ����
			return PACKET_EXE_CONTINUE;
		}

		UINT uTime = g_pTimeManager->CurrentTime();

		if(uTime<pLoginPlayer->m_LastDBOpTime+DB_OPERATION_TIME)
		{
			LCRetCreateChar Msg;
			Msg.SetAccount(pPacket->GetAccount());
			Msg.SetResult(ASKCREATECHAR_OP_TIMES);

			//����DB����Ƶ����Ϣ
			pLoginPlayer->SendPacket(&Msg);

			Log::SaveLog( LOGIN_LOGFILE, "CLAskCreateCharHandler....Fails!, code = ASKCREATECHAR_OP_TIMES") ;

			return PACKET_EXE_CONTINUE;
		}

		pPacket->SetPlayerID(pLoginPlayer->PlayerID());
		pPacket->SetAccount(pLoginPlayer->GetAccount());
		//����Ҳ�������DB ����
		//��������������ʱ��

		if(g_pDBThreadManager->SendPacket(pPacket,pLoginPlayer->PlayerID()))
		{//����ɹ�������Ϣ���͵�DB����
			pLoginPlayer->m_LastDBOpTime = uTime;	
			return PACKET_EXE_NOTREMOVE;	
		}
		else
		{//DB ѹ���������û����³���
			LCRetCreateChar Msg;
			Msg.SetAccount(pPacket->GetAccount());
			Msg.SetResult(ASKCREATECHAR_SERVER_BUSY);
			pLoginPlayer->SendPacket(&Msg);

			Log::SaveLog( LOGIN_LOGFILE, "CLAskCreateCharHandler....Fails!, code = ASKCREATECHAR_SERVER_BUSY") ;
			return PACKET_EXE_CONTINUE;
		}

	}
	else if(g_pDBThreadManager->IsPoolTID(CurrentThreadID))
	{

		PlayerID_t	PlayerID	= pPacket->GetPlayerID();

		GUID_t	PlayerCharGUID = 1;

		LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);
		Assert(pLoginPlayer);

		ASKCREATECHAR_RESULT	Result;
		LCRetCreateChar* pMsg = (LCRetCreateChar*)g_pPacketFactoryManager->CreatePacket(PACKET_LC_RETCREATECHAR);
		if(!pMsg)
		{
			AssertEx(FALSE,"���� LCRetCreateChar ��Ϣʧ��");
		}

		if(pLoginPlayer->GetDBOperating() == TRUE)
		{
			pMsg->SetAccount(pPacket->GetAccount());
			pMsg->SetResult(ASKCREATECHAR_SERVER_BUSY);
			g_pProcessManager->SendPacket(pMsg,PlayerID);

			Log::SaveLog(LOGIN_LOGFILE, "CLAskCreateCharHandler::Execute()....���ݿ������ͻ!,acc = %s",
				pPacket->GetAccount()) ;
			return PACKET_EXE_CONTINUE;
		}

		//���GUID �Ƿ���ȷ
		if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount())!= 0)
		{
			//Ӧ����һ�δ������
			Log::SaveLog(LOGIN_LOGFILE, "ERROR: CLAskCreateChar DBOperation Errors,acc = %s,Packet acc = %s",
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

		if(CharNumber>=MAX_CREATE_PLAYER_NUM )
		{
			pRetListMsg->SetAccount(pPacket->GetAccount());
			Result = ASKCREATECHAR_FULL; //��ɫ����
		}
		else if(CharNumber ==-1)
		{
			pRetListMsg->SetAccount(pPacket->GetAccount());
			Result = ASKCREATECHAR_OP_ERROR;
		}
		else
		{
			CampID_t PlayerCampType ;
			if( CharNumber > 0 )
				PlayerCampType = pLoginPlayer->GetPlayerCamp(); //�����������ɫ��ǿ������һ����ɫ����ӪID
			else
			{
				PlayerCampType = pPacket->GetCharCamp();
				pLoginPlayer->SetPlayerCamp( PlayerCampType );
			}

			if( PlayerCampType != CAMP1_PLAYER && PlayerCampType != CAMP2_PLAYER )
			{
				pMsg->SetAccount(pPacket->GetAccount());
				pMsg->SetResult(ASKCREATECHAR_OP_ERROR);
				g_pProcessManager->SendPacket(pMsg,PlayerID);

				Log::SaveLog( LOGIN_LOGFILE, "CLAskCreateCharHandler::Execute()....�����Ӫ���ݴ���!") ;
				return PACKET_EXE_CONTINUE;
			}
			if( pPacket->GetMenPai() != MATTRIBUTE_SHAOLIN &&
				pPacket->GetMenPai() != MATTRIBUTE_WUDANG &&
				pPacket->GetMenPai() != MATTRIBUTE_EMEI )
			{
				//Ŀǰ��ֻ��3��ְҵ
				pMsg->SetAccount(pPacket->GetAccount());
				pMsg->SetResult(ASKCREATECHAR_OP_ERROR);
				g_pProcessManager->SendPacket(pMsg,PlayerID);

				Log::SaveLog( LOGIN_LOGFILE, "CLAskCreateCharHandler::Execute()....���ѡ���������ݴ���!") ;
				return PACKET_EXE_CONTINUE;
			}

			ODBCInterface*	pInterface = g_pDBThreadManager->GetInterface(CurrentThreadID);
			Assert(pInterface);

			//�����¼
			DBCreateCharOp CreateCharOp(pInterface);
			CreateCharOp.SetAccount(pPacket->GetAccount());
			CreateCharOp.SetCharName(pPacket->GetName());
			CreateCharOp.SetCharGuid(pPacket->GetCharGuid());
			CreateCharOp.SetSex(pPacket->GetSex());
			CreateCharOp.SetHairColor(pPacket->GetHairColor());
			CreateCharOp.SetHairModel(pPacket->GetHairModel());
			CreateCharOp.SetFaceColor(pPacket->GetFaceColor());
			CreateCharOp.SetFaceModel(pPacket->GetFaceModel());
			CreateCharOp.SetHeadID(pPacket->GetHeadID());
			CreateCharOp.SetCampID(PlayerCampType);
			CreateCharOp.SetMenPaiID(pPacket->GetMenPai());
			BOOL bRetAddNew	 = CreateCharOp.AddNew();
			if(!bRetAddNew)
			{ 

				Log::SaveLog(LOGIN_LOGFILE,"CreateCharOp.AddNew()...Get Errors: %s",CreateCharOp.GetErrorMessage());

				//����½�ɫʧ��
				pMsg->SetAccount(pPacket->GetAccount());

				INT  ErrorCode = CreateCharOp.GetErrorCode();

				switch(ErrorCode) 
				{
				case ODBCBase::DB_SAME_PRI_KEY: //�û���Ҫ֪���Ĵ���
					{
						pMsg->SetResult(ASKCREATECHAR_SAME_NAME);
						Log::SaveLog( LOGIN_LOGFILE, "CLAskCreateCharHandler::Execute()....Fails! DBCreateCharOP ErrorCode = ASKCREATECHAR_SAME_NAME") ;
					}
					break;
				default:
					{
						pMsg->SetResult(ASKCREATECHAR_INTERNAL_ERROR);
						Log::SaveLog( LOGIN_LOGFILE, "CLAskCreateCharHandler::Execute() ...Fails! DBCreateCharOP ErrorCode = %d",ErrorCode) ;
						break;
					}
				}

				g_pProcessManager->SendPacket(pMsg,PlayerID);
				pLoginPlayer->SetDBOperating(FALSE);
				return PACKET_EXE_NOTREMOVE;
			}
			CreateCharOp.ParseResult(&Result);

			PlayerCharGUID = CreateCharOp.GetCharGuid();

			Assert(PlayerCharGUID!=INVALID_ID);
			//���û�������

			FULLUSERDATA		UserData;
			DBCharFullData		CharFullDataObject(pInterface);
			CharFullDataObject.SetCharGuid(PlayerCharGUID);

			BOOL bRetLoad =	CharFullDataObject.Load();
			if(!bRetLoad)
			{	
				//��֤����ʧ��
				pMsg->SetAccount(pPacket->GetAccount());
				pMsg->SetResult(ASKCREATECHAR_INTERNAL_ERROR);
				g_pProcessManager->SendPacket(pMsg,PlayerID);
				pLoginPlayer->SetDBOperating(FALSE);
				Log::SaveLog( LOGIN_LOGFILE, "CLAskCreateCharHandler::Execute() ...CharFullDataObject.Load()....Fails!") ;
				return PACKET_EXE_NOTREMOVE;
			}

			CharFullDataObject.ParseResult(&UserData);



			g_CharConfig.InitCharAttr(&UserData);

			BOOL bRetSave = CharFullDataObject.Save(&UserData); //�����ʼ����Ϣ
			if(!bRetSave)
			{
				//��ʼ����������ʧ��
				pMsg->SetAccount(pPacket->GetAccount());
				pMsg->SetResult(ASKCREATECHAR_INTERNAL_ERROR);
				g_pProcessManager->SendPacket(pMsg,PlayerID);
				Log::SaveLog( LOGIN_LOGFILE, "CLAskCreateCharHandler::Execute() ...CharFullDataObject.Save()....Fails!") ;
				pLoginPlayer->SetDBOperating(FALSE);
				return PACKET_EXE_NOTREMOVE;
			}
			bRetSave	=	CharFullDataObject.ParseResult(&UserData);//����ȫ������
			if(!bRetSave)
			{
				//��ʼ����������ʧ��
				pMsg->SetAccount(pPacket->GetAccount());
				pMsg->SetResult(ASKCREATECHAR_INTERNAL_ERROR);
				g_pProcessManager->SendPacket(pMsg,PlayerID);
				Log::SaveLog( LOGIN_LOGFILE, "CLAskCreateCharHandler::Execute() ...CharFullDataObject.ParseResult....Fails!") ;
				pLoginPlayer->SetDBOperating(FALSE);
				return PACKET_EXE_NOTREMOVE;
			}

			//�����µĽ�ɫ�б�
			DBCharList	CharListObject(pInterface);
			CharListObject.SetAccount(pPacket->GetAccount());
			bRetLoad = 	CharListObject.Load();
			if(bRetLoad)
			{
				CharListObject.ParseResult(pRetListMsg->GetCharBaseInfo());

				pRetListMsg->SetAccount(pPacket->GetAccount());
				pRetListMsg->SetResult(ASKCHARLIST_SUCCESS);
				pRetListMsg->SetCharNumber(CharListObject.GetCharNumber());

				if(CharListObject.GetCharNumber()> (UINT)CharNumber)
				{
					Result = ASKCREATECHAR_SUCCESS;
				}
				else
				{
					Result = ASKCREATECHAR_INTERNAL_ERROR;
				}
				pLoginPlayer->SetCharNumber(CharListObject.GetCharNumber());

				for(INT i=0;i<pLoginPlayer->GetCharNumber();i++)
				{
					pLoginPlayer->SetCharGUID(pRetListMsg->GetCharBaseInfo(i)->m_GUID,i);
				}
			}
			else //����ʧ�ܣ������ǶϿ�������
			{
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
		Log::SaveLog( LOGIN_LOGFILE, "CLAskCreateCharHandler::Execute()....OK! \
									 Account = %s CharGuid = %X CharName = %s",\
									 pPacket->GetAccount(),PlayerCharGUID,pPacket->GetName()) ;
		return PACKET_EXE_NOTREMOVE;
	}

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}