#include "StdAfx.h"
#include "GCCharFirstLogin.h"
#include "..\..\DataPool\GMDataPool.h"

UINT GCCharFirstLoginHandler :: Execute( GCCharFirstLogin* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	CDataPool::GetMe()->SetCharFirstLogin( pPacket->GetCharFirstLogin() );

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

