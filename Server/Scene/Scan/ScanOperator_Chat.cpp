#include "stdafx.h"

#include "ScanOperator_Chat.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "GamePlayer.h"
#include "ChatPipe.h"
#include "PlayerPool.h"
#include "ScenePlayerManager.h"
#include "Packet.h"
#include "CGChat.h"



ScanOperator_Chat::ScanOperator_Chat( )
{
	m_pPacket = NULL ;
	m_nChatType = 0 ;
	m_GuildID = INVALID_ID ;
	m_MenpaiID = MATTRIBUTE_WUMENPAI ;
}

ScanOperator_Chat::~ScanOperator_Chat( )
{
}

BOOL ScanOperator_Chat::Init( SCANOPERATOR_CHAT_INIT* pInit )
{
__ENTER_FUNCTION
	
	if( pInit==NULL )
	{
		Assert( FALSE ) ;
		return FALSE ;
	}

	m_pPacket	= pInit->m_pPacket ;
	m_nChatType = pInit->m_nChatType ;
	m_GuildID	= pInit->m_GuildID ;
	m_MenpaiID  = pInit->m_MenpaiID ;

	return 	ScanOperator::Init( pInit ) ;

__LEAVE_FUNCTION

	return TRUE ;
}

BOOL ScanOperator_Chat::On_BeforeScan( )
{
__ENTER_FUNCTION

	if( m_ZoneID!=INVALID_ID )
	{//����ǳ��������ͨ������Ϣ���������������
		return TRUE ;
	}

	if( m_nChatType == CHAT_TYPE_SYSTEM ||
		m_nChatType == CHAT_TYPE_SCENE )
	{//ȫ�����㲥
		UINT nPlayerCount = GetScene()->GetScenePlayerManager()->GetPlayerNumber() ;
		for( UINT i=0; i<nPlayerCount; i++ )
		{
			PlayerID_t* PlayerIDs = GetScene()->GetScenePlayerManager()->GetPlayers() ;
			if( PlayerIDs[i] == INVALID_ID )
				continue ;

			Player* pPlayer = g_pPlayerPool->GetPlayer(PlayerIDs[i]) ;
			if( pPlayer )
			{
				pPlayer->SendPacket( m_pPacket ) ;
				GetScene()->GetChatPipe()->m_nValidCount -- ;
			}
		}
	}
	else if( m_nChatType == CHAT_TYPE_GUILD )
	{//Ѱ�Ҵ˳�����ĳ�����ɵĳ�Ա����
		UINT nPlayerCount = GetScene()->GetScenePlayerManager()->GetPlayerNumber() ;
		for( UINT i=0; i<nPlayerCount; i++ )
		{
			PlayerID_t* PlayerIDs = GetScene()->GetScenePlayerManager()->GetPlayers() ;
			if( PlayerIDs[i] == INVALID_ID )
				continue ;

			GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerIDs[i]) ;
			if( pGamePlayer && pGamePlayer->GetHuman()->GetGuildID()==m_GuildID )
			{
				pGamePlayer->SendPacket( m_pPacket ) ;
				GetScene()->GetChatPipe()->m_nValidCount -- ;
			}
		}
	}
	else if( m_nChatType == CHAT_TYPE_MENPAI )
	{//Ѱ�Ҵ˳�����ĳ�����ɵĳ�Ա����
		UINT nPlayerCount = GetScene()->GetScenePlayerManager()->GetPlayerNumber() ;
		for( UINT i=0; i<nPlayerCount; i++ )
		{
			PlayerID_t* PlayerIDs = GetScene()->GetScenePlayerManager()->GetPlayers() ;
			if( PlayerIDs[i] == INVALID_ID )
				continue ;

			GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerIDs[i]) ;
			if( pGamePlayer && pGamePlayer->GetHuman()->GetMenPai()==m_MenpaiID )
			{
				pGamePlayer->SendPacket( m_pPacket ) ;
				GetScene()->GetChatPipe()->m_nValidCount -- ;
			}
		}
	}

	return FALSE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT ScanOperator_Chat::On_FindObj( Obj* pObj )
{
__ENTER_FUNCTION

	if( pObj==NULL )
	{
		Assert( FALSE ) ;
		return SCANRETURN_RETURN ;
	}

	if( !pObj->IsActiveObj() 
		|| pObj->GetObjType()!=Obj::OBJ_TYPE_HUMAN )
	{
		Assert(FALSE) ;
		return SCANRETURN_RETURN ;
	}

	Obj_Human* pHuman = (Obj_Human*)pObj ;
					
	pHuman->GetPlayer()->SendPacket( m_pPacket ) ;
	GetScene()->GetChatPipe()->m_nValidCount -- ;


__LEAVE_FUNCTION

	return SCANRETURN_CONTINUE ;
}




