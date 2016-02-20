#include "stdafx.h"
#include "User.h"

#include "Config.h"
#include "OnlineUser.h"

#include "ServerManager.h"
#include "ServerPlayer.h"

#include "WGRelation.h"

#include "MailCenter.h"
#include "WGMail.h"
#include "WGNotifyMail.h"

#include "Log.h"

#include "Combat/CampAndStand.h"
#include "WorldTable.h"

// ����һ����ϵ��
VOID USER::AddRelation( GUID_t guid, const _RELATION_USER* pRUser )
{
__ENTER_FUNCTION

	Assert( guid != INVALID_ID );
	Assert( pRUser );

	if( m_RelationData.m_RelationList.m_nCount >= MAX_RELATION_SIZE )
	{
		Assert( m_RelationData.m_RelationList.m_nCount < MAX_RELATION_SIZE );
		return;
	}

	INT nStartPos = m_RelationData.m_RelationList.m_nCount;

	for( INT i=0; i<MAX_RELATION_SIZE; ++i )
	{
		if( m_RelationData.m_RelationList.m_RelationUsers[nStartPos].m_UserGUID == INVALID_ID )
		{
			m_RelationData.m_RelationList.m_RelationUsers[nStartPos] = *pRUser;
			++(m_RelationData.m_RelationList.m_nCount);
			m_RelationData.m_RelationGUIDTable.Add( guid, &(m_RelationData.m_RelationList.m_RelationUsers[nStartPos]) );
			break;
		}

		if( (++nStartPos) >= MAX_RELATION_SIZE )
		{
			nStartPos = 0;
		}
	}

__LEAVE_FUNCTION
}

// �Ƴ�һ����ϵ��
VOID USER::DelRelation( GUID_t guid )
{
__ENTER_FUNCTION

	Assert( guid != INVALID_ID );

	for( INT i=0; i<MAX_RELATION_SIZE; ++i )
	{
		if( m_RelationData.m_RelationList.m_RelationUsers[i].m_UserGUID == guid )
		{
			m_RelationData.m_RelationList.m_RelationUsers[i].CleanUp();
			--(m_RelationData.m_RelationList.m_nCount);
			m_RelationData.m_RelationGUIDTable.Remove( guid );
			break;
		}
	}

__LEAVE_FUNCTION
}

// �õ���ĳ��ҵĹ�ϵ״̬
enum RELATION_TYPE USER::GetRelationType( GUID_t guid )
{
__ENTER_FUNCTION

	const _RELATION_USER* pRUser;
	pRUser = (const _RELATION_USER*)m_RelationData.m_RelationGUIDTable.Get(guid);
	if( pRUser != NULL )
	{
		return (RELATION_TYPE)pRUser->m_RelationType;
	}

__LEAVE_FUNCTION

	return RELATION_TYPE_NONE;
}

// �ж� guid �Ƿ������ܺ����б���
BOOL USER::IsGoodFriend(GUID_t guid)
{
__ENTER_FUNCTION

	const _RELATION_USER* pRUser;
	pRUser = (const _RELATION_USER*)m_RelationData.m_RelationGUIDTable.Get(guid);
	if( (pRUser != NULL) && (pRUser->m_nFriendPoint >= g_Config.m_ConfigInfo.m_nPromptPoint) )
	{
		return TRUE;
	}

__LEAVE_FUNCTION

	return FALSE;
}

// ����һ�����ܺ���
VOID USER::AddGoodFriend(GUID_t guid)
{
__ENTER_FUNCTION

	_RELATION_USER* pRUser;
	pRUser = (_RELATION_USER*)m_RelationData.m_RelationGUIDTable.Get(guid);
	if( (pRUser != NULL) )
	{
		pRUser->m_nFriendPoint = g_Config.m_ConfigInfo.m_nPromptPoint;
	}

__LEAVE_FUNCTION
}

// ������������б�
VOID USER::FillOnlineList( RETURN_ONLINE_LIST* pSend )
{
__ENTER_FUNCTION

	Assert( pSend );
	const _RELATION_USER* pRUser;

	for( INT i=0; i<MAX_RELATION_SIZE; ++i )
	{
		pRUser = &(m_RelationData.m_RelationList.m_RelationUsers[i]);
		if( (pRUser->m_UserGUID != INVALID_ID) && (pRUser->m_nFriendPoint >= g_Config.m_ConfigInfo.m_nPromptPoint) )
		{
			USER* pFriend;

			pFriend = g_pOnlineUser->FindUser( pRUser->m_UserGUID );
			if( pFriend != NULL && pFriend->IsGoodFriend( GetGUID() ) ) // �������
			{
				_RELATION_ONLINE OnlineRelation;
				OnlineRelation.SetGUID( pFriend->GetGUID() );
				OnlineRelation.SetMood( pFriend->GetMood() );
				pSend->AddOnlineRelation( &OnlineRelation );
			}
		}
	}

__LEAVE_FUNCTION
}

// ֪ͨ��������������Ϣ
VOID USER::InformOnlineMsg()
{
__ENTER_FUNCTION

	const _RELATION_USER* pRUser;

	for( INT i=0; i<MAX_RELATION_SIZE; ++i )
	{
		pRUser = &(m_RelationData.m_RelationList.m_RelationUsers[i]);
		if( (pRUser->m_UserGUID != INVALID_ID) && (pRUser->m_nFriendPoint >= g_Config.m_ConfigInfo.m_nPromptPoint) )
		{
			USER* pFriend;

			pFriend = g_pOnlineUser->FindUser( pRUser->m_UserGUID );
			if( pFriend != NULL && pFriend->IsGoodFriend( GetGUID() ) ) // �������
			{
				pFriend->SendRelationOnlineMsg( this );
			}
		}
	}

__LEAVE_FUNCTION
}

// ֪ͨ��������������Ϣ
VOID USER::InformOfflineMsg()
{
__ENTER_FUNCTION

	const _RELATION_USER* pRUser;

	for( INT i=0; i<MAX_RELATION_SIZE; ++i )
	{
		pRUser = &(m_RelationData.m_RelationList.m_RelationUsers[i]);
		if( (pRUser->m_UserGUID != INVALID_ID) && (pRUser->m_nFriendPoint >= g_Config.m_ConfigInfo.m_nPromptPoint) )
		{
			USER* pFriend;

			pFriend = g_pOnlineUser->FindUser( pRUser->m_UserGUID );
			if( pFriend != NULL && pFriend->IsGoodFriend( GetGUID() ) ) // �������
			{
				pFriend->SendRelationOfflineMsg( this );
			}
		}
	}

__LEAVE_FUNCTION
}

// ������ҷ���ĳ�������ߵ���Ϣ
VOID USER::SendRelationOnlineMsg(USER* pUser)
{
__ENTER_FUNCTION

	Assert( pUser );

	if( UserStatus() != US_NORMAL )
	{
		return;
	}

	ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( GetServerID() );
	if( pServerPlayer == NULL )
	{
		Assert( pServerPlayer );
		return;
	}

	WGRelation Msg;

	WG_RELATION* pRelation = Msg.GetRelation();

	pRelation->SetPlayerID( GetPlayerID() );
	pRelation->m_Type = RET_RELATIONONLINE;
	pRelation->m_NotifyOnline.SetTargetName( pUser->GetName() );
	pRelation->m_NotifyOnline.SetMood( pUser->GetMood() );

	pServerPlayer->SendPacket( &Msg );

__LEAVE_FUNCTION
}

// ������ҷ���ĳ�������ߵ���Ϣ
VOID USER::SendRelationOfflineMsg(USER* pUser)
{
__ENTER_FUNCTION

	Assert( pUser );

	if( UserStatus() != US_NORMAL )
	{
		return;
	}

	ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( GetServerID() );
	if( pServerPlayer == NULL )
	{
		Assert( pServerPlayer );
		return;
	}

	WGRelation Msg;

	WG_RELATION* pRelation = Msg.GetRelation();

	pRelation->SetPlayerID( GetPlayerID() );
	pRelation->m_Type = RET_RELATIONOFFLINE;
	pRelation->m_RelationGUID.SetTargetGUID( pUser->GetGUID() );

	pServerPlayer->SendPacket( &Msg );

__LEAVE_FUNCTION
}

// �����Ƿ���Է����ʼ���������ԵĻ����򷵻� 0�����򷵻ش���ţ�����Ųμ� enum RELATION_RETURN_TYPE
UCHAR USER::ValidateMail(const MAIL* pMail)
{
__ENTER_FUNCTION

	if( pMail == NULL )
	{
		Assert( pMail );
		return RET_ERR_RELATIONUNKNOWN;
	}

	UCHAR uFlag = 0;

	//
	//MAIL_TYPE_SYSTEM��MAIL_TYPE_SCRIPT���͵��ʼ��������
	//
	if( pMail->m_uFlag == MAIL_TYPE_NORMAL || pMail->m_uFlag == MAIL_TYPE_NORMAL2 )
	{
		if( IsCannotRecvMail() )
		{ // ���������ʼ�
			uFlag = RET_ERR_CANNOTRECEIVEMAIL;
		}
		else
		{
			_CAMP_DATA TempCamp;
			TempCamp.m_nCampID = pMail->m_Camp;
			if( IsEnemy( &TempCamp ) )
			{
				uFlag = RET_ERR_ISENEMY;
			}
			else
			{
				RELATION_TYPE RelationType = GetRelationType(pMail->m_GUID);

				if( RelationType == RELATION_TYPE_BLACKNAME )
				{ // ������Ҳ������ȫ���մ����Ա������桭��
					uFlag = RET_ERR_CANNOTRECEIVEMAIL;
				}
				else if( (RelationType == RELATION_TYPE_NONE) && IsCannotRecvStrangerMail() )
				{ // ����İ�����ʼ�
					uFlag = RET_ERR_NOTRECVSTRANGEMAIL;
				}
			}
		}
	}

	return uFlag;

__LEAVE_FUNCTION

	return RET_ERR_RELATIONUNKNOWN;
}

// �յ�һ���ʼ�
BOOL USER::RecvMail(const MAIL* pMail)
{
__ENTER_FUNCTION

	Assert( pMail );

	ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( GetServerID() );
	if( pServerPlayer == NULL )
	{
		Assert(pServerPlayer);
		return FALSE;
	}

	if( pMail->m_uFlag == MAIL_TYPE_SCRIPT )
	{ //������͵��ǽű��ʼ�����ֱ�ӷ�����Ŀ���û����ڷ�����ִ��
		WGMail Msg;
		Msg.SetPlayerID( m_PlayerID );
		Msg.GetMailList()->m_Count = 1;
		Msg.GetMailList()->m_aMail[0] = *pMail;
		Msg.GetMailList()->m_TotalLeft = 0;

		pServerPlayer->SendPacket( &Msg );
	}
	else
	{ //֪ͨ���û������ʼ���
		UCHAR uFlag = ValidateMail(pMail);

		if( uFlag )
		{ // �ʼ����ھ����ʼ�
			USER* pSUser = g_pOnlineUser->FindUser( pMail->m_szSourName );
			if ( pSUser != NULL )
			{
				ServerPlayer* pSServerPlayer = g_pServerManager->GetServerPlayer( pSUser->GetServerID() );
				if( pSServerPlayer == NULL )
				{
					Assert(FALSE);
					return PACKET_EXE_CONTINUE;
				}

				WGRelation Msg;
				Msg.GetRelation()->CleanUp();
				Msg.GetRelation()->SetPlayerID( pSUser->GetPlayerID() );
				Msg.GetRelation()->m_Type = uFlag;
				pSServerPlayer->SendPacket( &Msg );
			}

			return FALSE;
		}

		UINT uMailCount = g_pMailCenter->MailTo( pMail );

		WGNotifyMail Msg;
		Msg.SetPlayerID( m_PlayerID );
		Msg.SetGUID( m_SimpleData.m_GUID );
		Msg.SetMailCount( (BYTE)(uMailCount>255?255:uMailCount) );

		pServerPlayer->SendPacket( &Msg );
	}

__LEAVE_FUNCTION

	return TRUE;
}

BOOL USER::HeartBeat( UINT uTime )
{
__ENTER_FUNCTION

	if( m_LastTime==0 ) m_LastTime = uTime;
	UINT uDeltaTime = uTime-m_LastTime;
	m_LastTime = uTime;

	switch( m_UserStatus )
	{
	case US_NONE:
		return TRUE;
	case US_NORMAL:
	case US_HOLDUSERDATA:
	case US_OFFLINE:
	case US_CRASH_DOWN:
	case US_WAIT_SHUTDOWN:
		{
			if( m_KickTime>0 )
			{
				if( (UINT)m_KickTime<=uDeltaTime )
				{//�߳�
					GUID_t guiduser = GetGUID();
					UINT uStatus = m_UserStatus ;
					g_pOnlineUser->OnUserRemove( this );
					g_pOnlineUser->DelUser( this );
					Log::SaveLog( WORLD_LOGFILE, "HeartBeat...User GUID=%X Status=%d", 
						guiduser, uStatus );
				}
				else
				{//
					m_KickTime -= uDeltaTime;
				}
			}
		}
		break;
	};

__LEAVE_FUNCTION

	return TRUE;
}

VOID USER::ResetKickTime( )
{
	m_KickTime = g_Config.m_ConfigInfo.m_KickUserTime;
}

const _CAMP_DATA* USER::GetUserCampData()
{
	return &m_CampData;
}

VOID USER::SetUserCampData( _CAMP_DATA* pData )
{
	m_CampData = *pData;
}

BOOL USER::IsEnemy( const _CAMP_DATA* pData )
{
	if( pData == NULL )
	{
		Assert(FALSE && "�����ÿյ���Ӫ�������ж�");
		return TRUE;
	}

	ENUM_RELATION eRelation = CalcRelationType( m_CampData.m_nCampID, pData->m_nCampID, &g_CampAndStandDataMgr );
	if ( eRelation == RELATION_FRIEND )
		return FALSE;
	else
		return TRUE;
}

BYTE					USER::GetUserAge()
{
	return m_Age;
}

VOID 					USER::SetUserAge( BYTE age )
{
	m_Age = age;
}
