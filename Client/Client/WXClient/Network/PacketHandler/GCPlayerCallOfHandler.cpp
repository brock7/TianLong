// GCPlayerCallOfHandler.cpp

#include "StdAfx.h"

#include "GCPlayerCallOf.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\DataPool\TeamOrGroup.h"
#include "..\..\DataPool\GMUIDataPool.h"


using namespace Packets;

UINT GCPlayerCallOfHandler::Execute( GCPlayerCallOf* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	//��ʾ�Ի���
	//ȷ����ȡ����ʱ���� CGPlayerCallOfResult
	GUID_t PlayerGuid = pPacket->GetCallerGUID();

	// �������б���ȡ�ҵ������ҵ����� 
	TeamMemberInfo* pPlayerInfo;
	pPlayerInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMember(PlayerGuid);
	if(!pPlayerInfo)
	{
		return PACKET_EXE_CONTINUE;
	}

	CEventSystem::GetMe()->PushEvent(GE_OPEN_CALLOF_PLAYER, pPlayerInfo->m_szNick, "", pPacket->GetTime());

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
