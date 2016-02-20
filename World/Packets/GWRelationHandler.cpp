#include "stdafx.h"

#include "ServerManager.h"
#include "ServerPlayer.h"
#include "OnlineUser.h"
#include "Log.h"
#include "ChatCenter.h"
#include "GuildManager.h"
#include "GWRelation.h"
#include "WGRelation.h"
#include "WGChat.h"
#include "Team.h"

UINT GWRelationHandler::Execute( GWRelation* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
	GW_RELATION* pRelationPacket = pPacket->GetRelation();
	WGRelation Msg;
	Msg.GetRelation()->CleanUp();
	Msg.SetGUID(pRelationPacket->GetGUID());
	UCHAR uErr = 0;

	_RELATION* pRelationData = NULL;
	USER* pTargetUser = NULL;

	USER* pUser = g_pOnlineUser->FindUser( pRelationPacket->GetGUID() );
	if ( pUser == NULL )
	{ //û�з����ڡ������û��б����Ӧ�� GUID ���û�
		Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User GUID=%X not find!",
			pRelationPacket->GetGUID() );

		Assert( FALSE && "�Ҳ�����Ϣ������" );
		return PACKET_EXE_CONTINUE;
	}

	Msg.GetRelation()->SetPlayerID( pUser->GetPlayerID() );

	switch( pRelationPacket->m_Type )
	{
	case REQ_RELATIONINFO:
		{
			REQUEST_RELATION_INFO& ReqRelation = pRelationPacket->m_RequestInfo;

			if ( ReqRelation.GetTargetGUID() != INVALID_ID )
			{
				pTargetUser = g_pOnlineUser->FindUser( ReqRelation.GetTargetGUID() );
			}
			else if ( strlen(ReqRelation.GetTargetName()) > 0 )
			{
				pTargetUser = g_pOnlineUser->FindUser( ReqRelation.GetTargetName() );
			}

			if ( pTargetUser == NULL )
			{ // ��ҿ��ܲ�����Ҳ���ܲ����ߣ��� Server ���ж�
				uErr = RET_TARGETNOTONLINE;

				Msg.GetRelation()->m_RelationGUID.CleanUp();
				Msg.GetRelation()->m_RelationGUID.SetTargetGUID( ReqRelation.GetTargetGUID() );

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User GUID=%X not find!",
					ReqRelation.GetTargetGUID() );
				break;
			}

			Msg.GetRelation()->m_Type = RET_RELATIONINFO;

			RETURN_RELATION_INFO& Relation = Msg.GetRelation()->m_RelationInfo;

			Relation.CleanUp();

			pRelationData = Relation.GetRelationData();
			// pRelationData �������� switch ֮�����

			Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...get User info of Name=%s!",
				pTargetUser->GetName() );
		}
		break;
	case REQ_VIEWPLAYER:
		{
			REQUEST_VIEW_PLAYER& Recv = pRelationPacket->m_ViewPlayer;

			if ( strlen( Recv.GetTargetName() ) > 0 )
			{
				pTargetUser = g_pOnlineUser->FindUser( Recv.GetTargetName() );
			}

			if ( pTargetUser == NULL )
			{ // ��ҿ��ܲ�����Ҳ���ܲ�����
				uErr = RET_ERR_TARGETNOTEXIST;

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s not find!",
					Recv.GetTargetName() );
				break;
			}

			Msg.GetRelation()->m_Type = RET_VIEWPLAYER;

			RETURN_VIEW_PLAYER& Send = Msg.GetRelation()->m_ViewPlayer;
			Send.CleanUp();

			// �������
			Send.SetGUID( pTargetUser->GetGUID() );
			Send.SetName( pTargetUser->GetName() );
			// ...

			Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...View Player: %s!",
				pTargetUser->GetName() );
		}
		break;
	case REQ_ADDFRIEND:
	case REQ_TEMPFRIEND_TO_FRIEND:
		{ // �����Ҫ��ӵĺ����Ƿ����ߣ��������º��ѵ���ϸ��Ϣ�����ﲻ���Ϸ����ж�
			REQUEST_ADD_RELATION_WITH_GROUP& ReqRelation = pRelationPacket->m_AddRelationWithGroup;

			if ( ReqRelation.GetTargetGUID() != INVALID_ID )
			{
				pTargetUser = g_pOnlineUser->FindUser( ReqRelation.GetTargetGUID() );
			}
			else if ( strlen(ReqRelation.GetTargetName()) > 0 )
			{
				pTargetUser = g_pOnlineUser->FindUser( ReqRelation.GetTargetName() );
			}

			if ( pTargetUser == NULL )
			{ //û�з����ڡ������û��б����Ӧ�����ֵ��û�
				uErr = RET_ERR_TARGETNOTEXIST;

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s not find!",
					ReqRelation.GetTargetName() );
				Assert( FALSE && "�Ҳ����ú���" );
				break;
			}

			// �ж� pTargetUser �Ƿ�ܾ�����Ϊ����
			if ( pTargetUser->IsCannotAddFriend() )
			{
				uErr = RET_ERR_CANNOT_ADDFRIEND;

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s can't be added!",
					ReqRelation.GetTargetName() );
				break;
			}

			if( pUser->IsEnemy( pTargetUser->GetUserCampData() ) )
			{
				uErr = RET_ERR_ISENEMY;

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s is enemy!",
					ReqRelation.GetTargetName() );
				break;
			}

			RELATION_TYPE RelationType = pUser->GetRelationType( pTargetUser->GetGUID() );
			if ( RelationType != RELATION_TYPE_NONE )
			{
				if ( RelationType == RELATION_TYPE_BLACKNAME )
				{
					uErr = RET_ERR_ISBLACKNAME;
				}
				else
				{
					uErr = RET_ERR_ISFRIEND;
				}

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s can't be added2!",
					ReqRelation.GetTargetName() );
				break;
			}

			// �����������
			_RELATION_USER RUser;
			RUser.m_UserGUID = pTargetUser->GetGUID();
			RUser.m_RelationType = RELATION_TYPE_FRIEND;
			RUser.m_nFriendPoint = 0;
			pUser->AddRelation( pTargetUser->GetGUID(), &RUser );

			// ����ȥ������Ϣ
			if ( pRelationPacket->m_Type == REQ_ADDFRIEND )
			{
				Msg.GetRelation()->m_Type = RET_ADDFRIEND;
			}
			else if ( pRelationPacket->m_Type == REQ_TEMPFRIEND_TO_FRIEND )
			{
				Msg.GetRelation()->m_Type = RET_TEMPFRIEND_TO_FRIEND;
			}

			RETURN_ADD_RELATION& Relation = Msg.GetRelation()->m_AddRelation;

			Relation.CleanUp();
			Relation.SetRelationType( ReqRelation.GetRelationType() );
			Relation.SetGroup( ReqRelation.GetGroup() );

			pRelationData = Relation.GetRelationData();
			// pRelationData �������� switch ֮�����

			Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s added to friend list!",
				ReqRelation.GetTargetName() );

			if ( pTargetUser->UserStatus() == US_NORMAL )
			{ // ֪ͨ���ӵ���
				WGRelation NotifyMsg;

				NotifyMsg.GetRelation()->CleanUp();
				NotifyMsg.GetRelation()->SetPlayerID( pTargetUser->GetPlayerID() );
				NotifyMsg.GetRelation()->m_Type = RET_ADDFRIENDNOTIFY;
				RETURN_NOTIFY_FRIEND& NotifyFriend = NotifyMsg.GetRelation()->m_NotifyFriend;

				NotifyFriend.CleanUp();
				NotifyFriend.SetGUID( pUser->GetGUID() );
				NotifyFriend.SetName( pUser->GetName() );

				ServerPlayer* pTServerPlayer = g_pServerManager->GetServerPlayer( pTargetUser->GetServerID() );
				if( pTServerPlayer==NULL )
				{
					Assert(FALSE);
					break;
				}

				pTServerPlayer->SendPacket( &NotifyMsg );
			}
		}
		break;
	case REQ_ADDTOBLACKLIST:
	case REQ_TEMPFRIEND_ADDTO_BLACKLIST:
		{ // �����Ҫ��ӵ��Ƿ����ߣ���������ϸ��Ϣ�����ﲻ���Ϸ����ж�
			REQUEST_ADD_RELATION& ReqRelation = pRelationPacket->m_AddRelation;

			if ( ReqRelation.GetTargetGUID() != INVALID_ID )
			{
				pTargetUser = g_pOnlineUser->FindUser( ReqRelation.GetTargetGUID() );
			}
			else if ( strlen(ReqRelation.GetTargetName()) > 0 )
			{
				pTargetUser = g_pOnlineUser->FindUser( ReqRelation.GetTargetName() );
			}

			if ( pTargetUser == NULL )
			{ //û�з����ڡ������û��б����Ӧ�����ֵ��û�
				uErr = RET_ERR_TARGETNOTEXIST;

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s not find!",
					ReqRelation.GetTargetName() );
				Assert( FALSE && "�Ҳ�������" );
				break;
			}

			RELATION_TYPE RelationType = pUser->GetRelationType( pTargetUser->GetGUID() );
			if ( RelationType != RELATION_TYPE_NONE )
			{
				if ( RelationType == RELATION_TYPE_FRIEND )
				{
					pUser->DelRelation( pTargetUser->GetGUID() );
				}
				else
				{
					switch( RelationType )
					{
					case RELATION_TYPE_BLACKNAME:
						uErr = RET_ERR_ISBLACKNAME;
						break;
					case RELATION_TYPE_MARRY:
						uErr = RET_ERR_SPOUSETOBLACKLIST;
						break;
					case RELATION_TYPE_MASTER:
						uErr = RET_ERR_MASTERTOBLACKLIST;
						break;
					case RELATION_TYPE_PRENTICE:
						uErr = RET_ERR_PRENTICETOBLACKLIST;
						break;
					case RELATION_TYPE_BROTHER:
						uErr = RET_ERR_BROTHERTOBLACKLIST;
						break;
					default:
						Assert(FALSE);
						return PACKET_EXE_CONTINUE;
					}

					Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s can't be added4!",
						ReqRelation.GetTargetName() );
					break;
				}
			}

			// ���������
			_RELATION_USER RUser;
			RUser.m_UserGUID = pTargetUser->GetGUID();
			RUser.m_RelationType = RELATION_TYPE_BLACKNAME;
			RUser.m_nFriendPoint = 0;
			pUser->AddRelation( pTargetUser->GetGUID(), &RUser );

			// ����ȥ������Ϣ
			if ( pRelationPacket->m_Type == REQ_ADDTOBLACKLIST )
			{
				Msg.GetRelation()->m_Type = RET_ADDTOBLACKLIST;
			}
			else if ( pRelationPacket->m_Type == REQ_TEMPFRIEND_ADDTO_BLACKLIST )
			{
				Msg.GetRelation()->m_Type = RET_TEMPFRIEND_ADDTO_BLACKLIST;
			}

			RETURN_ADD_RELATION& Relation = Msg.GetRelation()->m_AddRelation;

			Relation.CleanUp();
			Relation.SetRelationType( ReqRelation.GetRelationType() );

			pRelationData = Relation.GetRelationData();
			// pRelationData �������� switch ֮�����

			Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s added to black list!",
				ReqRelation.GetTargetName() );
		}
		break;
	case REQ_DELFRIEND:
	case REQ_DELFROMBLACKLIST:
		pUser->DelRelation( pRelationPacket->m_RelationGUID.GetTargetGUID() );
		return PACKET_EXE_CONTINUE;
	case REQ_TRANSITION:
		{
			RELATION_GUID_UCHAR& Relation = pRelationPacket->m_TransitionRelation;
			pUser->DelRelation( Relation.GetTargetGUID() );

			_RELATION_USER RUser;
			RUser.m_UserGUID = Relation.GetTargetGUID();
			RUser.m_RelationType = (RELATION_TYPE)Relation.GetRelationType();
			RUser.m_nFriendPoint = 0;
			pUser->AddRelation( Relation.GetTargetGUID(), &RUser );
			return PACKET_EXE_CONTINUE;
		}
		break;
	case REQ_NEWGOODFRIEND:
		{ // ����һ�����ܺ���
			pUser->AddGoodFriend( pRelationPacket->m_RelationGUID.GetTargetGUID() );
			return PACKET_EXE_CONTINUE;
		}
		break;
	case REQ_RELATIONONLINE:
		{ // ��ȡ���ѵ������б�
			Msg.GetRelation()->m_Type = RET_ONLINELIST;
			RETURN_ONLINE_LIST* pSend = &(Msg.GetRelation()->m_RelationOnline);
			pSend->CleanUp();
			pUser->FillOnlineList(pSend);
		}
		break;
	case REQ_MODIFYMOOD:
		{
			pUser->SetMood( pRelationPacket->m_ModifyMood.GetMood() );
			return PACKET_EXE_CONTINUE;
		}
		break;
	case REQ_MODIFYSETTINGS:
		{
			pUser->SetSettings( pRelationPacket->m_Settings.GetSettings() );
			return PACKET_EXE_CONTINUE;
		}
		break;
	case REQ_NOTIFY_ADDTEMPFRIEND:
		{
			REQUEST_VIEW_PLAYER& Recv = pRelationPacket->m_ViewPlayer;

			if ( strlen(Recv.GetTargetName()) > 0 )
			{
				pTargetUser = g_pOnlineUser->FindUser( Recv.GetTargetName() );
			}

			if ( pTargetUser == NULL )
			{ // ��ҿ��ܲ�����Ҳ���ܲ����ߣ��� Server ���ж�
				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User GUID=%s not find!",
					Recv.GetTargetName() );
				return PACKET_EXE_CONTINUE;
			}

			Msg.GetRelation()->m_Type = RET_NOTIFY_ADDTEMPFRIEND;
			REQUEST_VIEW_PLAYER* pSend = &(Msg.GetRelation()->m_PlayerName);
			pSend->CleanUp();
			pSend->SetTargetName( pUser->GetName() );
		}
		break;
	default :
		{
			Assert(FALSE);
			return PACKET_EXE_CONTINUE;
		}
	}

	if ( uErr > 0 )
	{
		Msg.GetRelation()->m_Type = uErr;
	}

	if ( pRelationData != NULL && pTargetUser != NULL )
	{
		pRelationData->SetGUID( pTargetUser->GetGUID() );
		pRelationData->SetName( pTargetUser->GetName() );
		pRelationData->SetLevel( pTargetUser->GetLevel() );
		pRelationData->SetMenPai( pTargetUser->GetMenpai() );
		pRelationData->SetPortrait( pTargetUser->GetPortrait() );
		pRelationData->SetGuildID( pTargetUser->GetGuildID() );
		pRelationData->SetGuildName( g_pGuildManager->GetGuildName(pTargetUser->GetGuildID()) );

		if ( pTargetUser->UserStatus() == US_NORMAL )
		{ // ֻ�����߲ŷ�ʣ����Ϣ
			pRelationData->SetOnlineFlag(1);
			pRelationData->SetMood( pTargetUser->GetMood() );
			pRelationData->SetTitle( pTargetUser->GetTitle() );
			pRelationData->SetSceneID( pTargetUser->GetSceneID() );

			if ( pTargetUser->GetTeamID() != INVALID_ID )
			{
				Team* pTeam = g_pTeamList->GetTeam( pTargetUser->GetTeamID() );
				if ( pTeam != NULL )
				{
					pRelationData->SetTeamSize( pTeam->MemberCount() );
				}
			}
		}
	}

	pServerPlayer->SendPacket( &Msg );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
