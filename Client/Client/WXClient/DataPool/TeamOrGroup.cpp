// TeamOrGroup.cpp
#include "StdAfx.h"
#include "../Network/NetManager.h"
#include "TeamOrGroup.h"
#include "TDException.h"
#include "..\Event\GMEventSystem.h"
#include "..\Object\Character\Obj_PlayerMySelf.h"
#include "..\Object\ObjectManager.h"
#include "GMDP_CharacterData.h"
#include "CGAskTeamMemberInfo.h"
#include "..\object\character\Obj_PlayerOther.h"
#include "..\FakeObject\GMFakeObjSystem.h"
#include "..\Procedure\GameProcedure.h"


#include "..\World\Scene.h"
#include "..\World\WorldManager.h"
//--------------------------------------------------------------------------------------------------------------------------
//
// �ڽ�������ʾ��ģ��.
//

// ģ�ͼ���.
UINT CModelShowInUI::m_TeamNumCount = 0;

CModelShowInUI::CModelShowInUI()
{
	
	m_pAvatar = NULL;								// ����UI��ʾ���߼�����.
	memset(m_szBufModel, 0, sizeof(m_szBufModel));	// ģ������.

	// �����µ�ģ��
	//CreateNewUIModel();

}

CModelShowInUI::~CModelShowInUI()
{
	DestroyUIModel();
}

// ɾ��uiģ��
BOOL CModelShowInUI::DestroyUIModel()
{
	if(m_pAvatar)
	{
		CFakeObjSystem::GetMe()->DestroyObject(m_szBufModel);
		m_pAvatar = NULL;
	}

	return FALSE;
}

// �����µ�ģ��
BOOL	CModelShowInUI::CreateNewUIModel()
{

	m_TeamNumCount++;
	memset(m_szBufModel, 0, sizeof(m_szBufModel));
	_stprintf(m_szBufModel, "uiModel%d", m_TeamNumCount);
	
	if(m_pAvatar)
	{
		DestroyUIModel();
	}

	//����Avatar
	m_pAvatar  = (CObject_PlayerOther*)(CFakeObjSystem::GetMe()->NewFakeObject(_T("CObject_PlayerOther"), m_szBufModel,"Camera_Main"));
	m_pAvatar->SetFaceDir(0);
	m_pAvatar->GetCharacterData()->Set_RaceID(0);
	//ͷ����ɫ
	m_pAvatar->GetCharacterData()->Set_HairColor(0);
	return FALSE;

}


// ����ģ����Ϣ
VOID CModelShowInUI::SetUIModelInfo(HUMAN_EQUIP point, INT nID)
{
	if(m_pAvatar)
	{
		if(HEQUIP_WEAPON == point)
		{
			char buf[512];
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "����id �� %d", nID);
			AxTrace(0, 0, buf);
		}
		m_pAvatar->GetCharacterData()->Set_Equip(point, nID);
	}
}

// ����ģ����Ϣ
VOID CModelShowInUI::SetFaceMeshId(INT nID)
{
	// ��������ģ��
	m_pAvatar->GetCharacterData()->Set_FaceMesh(nID);

		
}

// ����ģ����Ϣ
VOID CModelShowInUI::SetFaceHairId(INT nID)
{
	// ���÷��͵�ģ��.
	m_pAvatar->GetCharacterData()->Set_HairMesh(nID);


}

// ����ģ����Ϣ
VOID CModelShowInUI::SetHairColor(INT nID)
{
	//ͷ����ɫ
	m_pAvatar->GetCharacterData()->Set_HairColor(nID);
}

// �õ�uiģ������
const TCHAR* CModelShowInUI::GetUIModelName()
{
	
	return m_szBufModel;
	
}


//------------------------------------------------------------------------------------------------------------------------------










VOID CTeamOrGroup::CleanUp()
{
	m_Type = (TEAM_OR_GROUP_TYPE)0;
	m_ID = INVALID_ID;
	m_MyTeamID = 0xFF;
	m_LeaderID = INVALID_ID;
}

CTeamOrGroup::~CTeamOrGroup()
{
	if ( HasTeam() )
	{
		DismissTeam();
	}

	//ClearInviteTeam();
	//ClearProposer();
}

VOID CTeamOrGroup::CreateTeam(TeamMemberInfo* leader, TeamID_t TeamID)
{
	if( m_Type )
	{
		Assert(FALSE);
		return;
	}

	m_Type = TEAM_TYPE;
	m_ID = TeamID;
	SetLeaderGUID( leader->m_GUID );

	m_TeamMembers.push_back(leader);

	//֪ͨTalk Interface�����齨��
	SCRIPT_SANDBOX::Talk::s_Talk.TeamCreate(m_ID);
	//���ý�ɫ������
	CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_HaveTeamFlag(TRUE);
}

VOID CTeamOrGroup::DismissTeam()
{
	switch(m_Type)
	{
	case TEAM_TYPE:
		{ // ��������
			std::vector< TeamMemberInfo* >::iterator it_member;

			for( it_member = m_TeamMembers.begin(); it_member != m_TeamMembers.end(); ++it_member )
			{
				delete (*it_member); // �ͷŶ�Ա��Ϣ
			}

			m_TeamMembers.clear();
		}
		break;
	case GROUP_TYPE:
		{ // �Ŷӵ����
			std::vector< TeamInfo* >::iterator it_team;
			std::vector< TeamMemberInfo* >::iterator it_member;

			for( it_team = m_TeamInfo.begin(); it_team != m_TeamInfo.end(); ++it_team )
			{
				for( it_member = (*it_team)->m_TeamMembers.begin(); it_member != (*it_team)->m_TeamMembers.end(); ++it_member )
				{
					delete (*it_member); // �ͷŶ�Ա��Ϣ
				}

				(*it_team)->m_TeamMembers.clear();

				delete (*it_team);
			}

			m_TeamInfo.clear();
		}
		break;
	default:
		Assert(FALSE);
		return;
	}

	CleanUp();

	//֪ͨTalk Interface�������ɢ
	//SCRIPT_SANDBOX::Talk::s_Talk.TeamDestory();

	//���ý�ɫ������
	CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_HaveTeamFlag(FALSE);
	CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_TeamLeaderFlag(FALSE);
	CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_TeamFullFlag(FALSE);
	CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_TeamFollowFlag(FALSE);
}

TeamInfo* CTeamOrGroup::GetTeam(BYTE TeamIndex) const
{
	for(std::vector< TeamInfo* >::const_iterator it = m_TeamInfo.begin(); it != m_TeamInfo.end(); ++it)
	{
		if( (*it)->m_TeamID == TeamIndex )
		{
			return *it;
		}
	}

	return NULL;
}

INT CTeamOrGroup::GetMemberUIIndex(GUID_t guid, BYTE TeamIndex)
{	// ����� TeamIndex ����Ҫ����
	CTeamMembers* pTeamMembers;
	GUID_t myGUID = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();

	if ( guid == myGUID )
	{ // �Լ�û�����
		return INVALID_ID;
	}

	switch(m_Type)
	{
	case TEAM_TYPE:
		{ // ��������
			pTeamMembers = &m_TeamMembers;
		}
		break;
	case GROUP_TYPE:
		{ // �Ŷӵ����
			if(TeamIndex<1 || TeamIndex>MAX_TEAMS_IN_GROUP)
			{
				Assert(FALSE);
				return INVALID_ID;
			}

			if( m_MyTeamID != TeamIndex )
			{ // ��ʱ��������ʾ�Ŷӳ�Ա��Ϣ
				return INVALID_ID;
			}

			pTeamMembers = &(GetTeam(TeamIndex)->m_TeamMembers);
		}
		break;
	default:
		Assert(FALSE);
		return INVALID_ID;
	}

	std::vector< TeamMemberInfo* >::iterator it_member;
	INT i;

	for( it_member = pTeamMembers->begin(), i=1; it_member != pTeamMembers->end(); ++it_member, ++i )
	{
		if ( (*it_member)->m_GUID == myGUID )
		{ // �Լ���ռ�����
			--i;
			continue;
		}

		if( i > (INT)pTeamMembers->size() )
		{
			MessageBox(NULL, "Team UI Member Index Error occurred.", "Err", MB_OK);
		}

		if( (*it_member)->m_GUID == guid )
		{
			return i;
		}
	}

	return INVALID_ID;
}

const TeamMemberInfo* CTeamOrGroup::GetMemberByUIIndex(INT UIIndex, BYTE TeamIndex)
{
	CTeamMembers* pTeamMembers;
	GUID_t myGUID = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();

	switch(m_Type)
	{
	case TEAM_TYPE:
		{ // ��������
			pTeamMembers = &m_TeamMembers;
		}
		break;
	case GROUP_TYPE:
		{ // �Ŷӵ����
			if(TeamIndex<1 || TeamIndex>MAX_TEAMS_IN_GROUP)
			{
				Assert(FALSE);
				return NULL;
			}

			if( m_MyTeamID != TeamIndex )
			{ // ��ʱ��������ʾ�Ŷӳ�Ա��Ϣ
				return NULL;
			}

			pTeamMembers = &(GetTeam(TeamIndex)->m_TeamMembers);
		}
		break;
	default:
		Assert(FALSE);
		return NULL;
	}

	std::vector< TeamMemberInfo* >::iterator it_member;
	INT i;

	for( it_member = pTeamMembers->begin(), i=1; it_member != pTeamMembers->end(); ++it_member, ++i )
	{
		if ( (*it_member)->m_GUID == myGUID )
		{ // �Լ���ռ�����
			--i;
		}

		if ( i == UIIndex )
		{
			return (*it_member);
		}
	}

	return NULL;
}

INT CTeamOrGroup::GetTeamMemberCount(BYTE TeamIndex)
{
	switch(m_Type)
	{
	case 0:
		return 0;
	case TEAM_TYPE:
		{ // ��������
			return (INT)m_TeamMembers.size();
		}
		break;
	case GROUP_TYPE:
		{ // �Ŷӵ����
			if(TeamIndex<1 || TeamIndex>MAX_TEAMS_IN_GROUP)
			{
				Assert(FALSE);
				return 0;
			}

			return (INT)GetTeam(TeamIndex)->m_TeamMembers.size();
		}
		break;
	default:
		Assert(FALSE);
		return 0;
	}
}

VOID CTeamOrGroup::FillMyInfo()
{
	if ( !HasTeam() )
	{ // û�ж��鲻���в���
		return;
	}

	TeamMemberInfo* pMyTMInfo;
	GUID_t guid;

	guid = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();
	pMyTMInfo = GetMember(guid);

	if ( pMyTMInfo == NULL )
	{
		Assert( FALSE && "Error: I'm not in my team...'" );
		return;
	}

	FillMyInfo( pMyTMInfo );
}

// ����ģ����Ϣ
VOID	CTeamOrGroup::SetModelLook()
{
	if ( !HasTeam() )
	{ // û�ж��鲻���в���
		return;
	}

	TeamMemberInfo* pMyTMInfo;
	GUID_t guid;

	guid = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();
	pMyTMInfo = GetMember(guid);
	if ( pMyTMInfo == NULL )
	{
		return;
	}

	// ����uiģ��
	pMyTMInfo->m_UIModel.SetUIModelInfo(HEQUIP_WEAPON, pMyTMInfo->m_WeaponID);
	
	// ����uiģ��
	pMyTMInfo->m_UIModel.SetUIModelInfo(HEQUIP_CAP, pMyTMInfo->m_CapID);
		
	// ����uiģ��
	pMyTMInfo->m_UIModel.SetUIModelInfo(HEQUIP_ARMOR, pMyTMInfo->m_ArmourID);
		
	// ����uiģ��
	pMyTMInfo->m_UIModel.SetUIModelInfo(HEQUIP_CUFF, pMyTMInfo->m_CuffID);
	
	// ����uiģ��
	pMyTMInfo->m_UIModel.SetUIModelInfo(HEQUIP_BOOT, pMyTMInfo->m_FootID);


	if(pMyTMInfo->m_uFaceMeshID < 255)
	{
		// ��������
		pMyTMInfo->m_UIModel.SetFaceMeshId(pMyTMInfo->m_uFaceMeshID);
	}
		
	if(pMyTMInfo->m_uHairMeshID < 255)
	{
		// ���÷���
		pMyTMInfo->m_UIModel.SetFaceHairId(pMyTMInfo->m_uHairMeshID);
	}
	
	//if(pMyTMInfo->m_uHairColor < 255)
	//{
	//	// ������ɫ
	//	pMyTMInfo->m_UIModel.SetHairColor(pMyTMInfo->m_uHairColor);
	//}
	//else
	//{
	//	// ������ɫ
	//	pMyTMInfo->m_UIModel.SetHairColor(0);
	//}//

	// ������ɫ
	pMyTMInfo->m_UIModel.SetHairColor(pMyTMInfo->m_uHairColor);
	
}


VOID CTeamOrGroup::FillMyInfo(TeamMemberInfo* member)
{
	Assert( member );

	CObject_PlayerMySelf* pMe = CObjectManager::GetMe()->GetMySelf();

	Assert( member->m_GUID == pMe->GetServerGUID() );

	CCharacterData* pMyData = pMe->GetCharacterData();

	if(NULL == pMyData)
	{
		return ;
	}
	strncpy(member->m_szNick, pMyData->Get_Name(), MAX_CHARACTER_NAME-1);
	member->m_szNick[MAX_CHARACTER_NAME-1] = '\0';
	member->m_uFamily = pMyData->Get_MenPai();
	member->m_uDataID = pMyData->Get_RaceID();
	member->m_uLevel = pMyData->Get_Level();
	member->m_nPortrait = pMyData->Get_PortraitID();
	member->m_WorldPos.m_fX = pMe->GetPosition().x;
	member->m_WorldPos.m_fZ = pMe->GetPosition().z;
	member->m_nHP = pMyData->Get_HP();
	member->m_dwMaxHP = pMyData->Get_MaxHP();
	member->m_nMP = pMyData->Get_MP();
	member->m_dwMaxMP = pMyData->Get_MaxMP();
	member->m_nAnger = 100; // �ͻ���û��
	member->m_WeaponID = pMyData->Get_Equip(HEQUIP_WEAPON);
	member->m_CapID = pMyData->Get_Equip(HEQUIP_CAP);
	member->m_ArmourID = pMyData->Get_Equip(HEQUIP_ARMOR);
	member->m_CuffID = pMyData->Get_Equip(HEQUIP_CUFF);
	member->m_FootID = pMyData->Get_Equip(HEQUIP_BOOT);
	member->m_bDead = (pMe->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_DEAD);

	member->m_uFaceMeshID = pMyData->Get_FaceMesh();
	member->m_uHairMeshID = pMyData->Get_HairMesh();
	member->m_uHairColor  = pMyData->Get_HairColor();

}

VOID CTeamOrGroup::AddMember(TeamMemberInfo* member, TeamID_t TeamID, BYTE TeamIndex)
{
	TeamMemberInfo* pTMInfo = new TeamMemberInfo;
	//memcpy((VOID*)pTMInfo, member, sizeof(TeamMemberInfo));

	pTMInfo->m_GUID			= member->m_GUID;
	pTMInfo->m_OjbID		= member->m_OjbID;						//
	pTMInfo->m_SceneID		= member->m_SceneID;
	pTMInfo->m_uFamily		= member->m_uFamily;					// 2.����
	pTMInfo->m_uDataID		= member->m_uDataID;					// 3.�Ա�
	pTMInfo->m_nPortrait	= member->m_nPortrait;					// 5.ͷ��
	memcpy(pTMInfo->m_szNick, member->m_szNick, sizeof(pTMInfo->m_szNick));

	CObject_PlayerMySelf* pObj = CObjectManager::GetMe()->GetMySelf();
	switch(m_Type)
	{
	case 0:
		{
			CreateTeam(pTMInfo, TeamID);
		}
		break;
	case TEAM_TYPE:
		{ // ������˵����
			if(m_TeamMembers.size() >= MAX_TEAM_MEMBER)
			{
				Assert(FALSE);
				return;
			}

			m_TeamMembers.push_back(pTMInfo);
		}
		break;
	case GROUP_TYPE:
		{ // �ŶӼ��˵����
			if(TeamIndex<0 || TeamIndex>=MAX_TEAMS_IN_GROUP)
			{
				Assert(FALSE);
				return;
			}

			++TeamIndex; // Ĭ�ϴ��������Ϊ 0 ~ MAX_TEAMS_IN_GROUP-1

			TeamInfo* pTeamInfo = GetTeam(TeamIndex);

			if(pTeamInfo != NULL)
			{
				if(pTeamInfo->m_TeamMembers.size() >= MAX_TEAM_MEMBER)
				{
					Assert(FALSE);
					return;
				}
			}
			else
			{ // �����С�鲻���ڣ��򴴽�����������Ŷ�
				pTeamInfo = new TeamInfo;
				pTeamInfo->m_TeamID = TeamIndex;
				m_TeamInfo.push_back(pTeamInfo);

				//������
				if(pTeamInfo->m_TeamMembers.size() >= MAX_TEAM_MEMBER)
				{
					CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_TeamFullFlag(TRUE);
				}
			}

			pTeamInfo->m_TeamMembers.push_back(pTMInfo);

			if( pTMInfo->m_GUID == pObj->GetServerGUID() )
			{
				m_MyTeamID = TeamIndex;
			}
		}
		break;
	default:
		Assert(FALSE);
		return;
	}

	if( pTMInfo->m_GUID != pObj->GetServerGUID() )
	{ // ��Ҫ���ض��ѵ�ͷ�񴰿�
		// ����ö��ѵ�����
		CGAskTeamMemberInfo Msg;
		Msg.SetSceneID( pTMInfo->m_SceneID );
		Msg.setObjID( pTMInfo->m_OjbID );
		Msg.SetGUID( pTMInfo->m_GUID );

		CNetManager::GetMe()->SendPacket( &Msg );
	}
	else
	{
		FillMyInfo( pTMInfo );
	}

	// ����һ�������ģ��.
	pTMInfo->m_UIModel.CreateNewUIModel();
	// ����uiģ��
	// �����Ա�
	pTMInfo->m_UIModel.m_pAvatar->GetCharacterData()->Set_RaceID(pTMInfo->m_uDataID);
	
}

VOID CTeamOrGroup::DelMember(GUID_t guid)
{
	switch(m_Type)
	{
	case TEAM_TYPE:
		{ // �������
			std::vector< TeamMemberInfo* >::iterator it_member;

			for( it_member = m_TeamMembers.begin(); it_member != m_TeamMembers.end(); ++it_member )
			{
				if( (*it_member)->m_GUID == guid )
				{
					delete (*it_member); // �ͷŶ�Ա��Ϣ
					m_TeamMembers.erase(it_member);
					break;
				}
			}

			//���鲻����
			if(m_TeamMembers.size() < MAX_TEAM_MEMBER)
			{
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_TeamFullFlag(FALSE);
			}
		}
		break;
	case GROUP_TYPE:
		{ // �Ŷ����
			std::vector< TeamInfo* >::iterator it_team;
			std::vector< TeamMemberInfo* >::iterator it_member;

			BOOL bFind = FALSE;

			for( it_team = m_TeamInfo.begin(); it_team != m_TeamInfo.end(); ++it_team )
			{
				for( it_member = (*it_team)->m_TeamMembers.begin(); it_member != (*it_team)->m_TeamMembers.end(); ++it_member )
				{
					if( (*it_member)->m_GUID == guid )
					{
						delete (*it_member); // �ͷŶ�Ա��Ϣ
						(*it_team)->m_TeamMembers.erase(it_member);
						bFind = TRUE;

						if( (*it_team)->m_TeamMembers.size() < 1 )
						{ // ��С����ˣ����ͷŸ�С�������
							delete (*it_team);
							m_TeamInfo.erase(it_team);
						}

						break;
					}
				}

				if( bFind )
				{
					break;
				}
			}
		}
		break;
	default:
		Assert(FALSE);
		return;
	}

	if(guid == m_LeaderID)
	{ // choose a new leader
		switch(m_Type)
		{
		case TEAM_TYPE:
			{ // �������
				if( m_TeamMembers.size() < 1 )
				{ // ���ڴ˴���������ɢ�����
					Assert(FALSE);
					return;
				}

				SetLeaderGUID( m_TeamMembers[0]->m_GUID );
			}
			break;
		case GROUP_TYPE:
			{ // �Ŷ��������ʱ�������Ŷ�ѡ�����ų��Ĺ���
				Assert(FALSE);
			}
			break;
		default:
			Assert(FALSE);
			return;
		}
	}
}

VOID CTeamOrGroup::ExchangeMemberPosition(GUID_t guid1, GUID_t guid2, BYTE TeamIndex1, BYTE TeamIndex2)
{
	TeamMemberInfo *pTMInfo1, *pTMInfo2;

	pTMInfo1 = GetMember(guid1);
	if ( pTMInfo1 == NULL )
	{
		Assert(FALSE);
		return;
	}

	pTMInfo2 = GetMember(guid2);
	if ( pTMInfo2 == NULL )
	{
		Assert(FALSE);
		return;
	}

	switch(m_Type)
	{
	case TEAM_TYPE:
		{ // �������
			for( UINT i = 0; i < m_TeamMembers.size(); ++i )
			{
				if ( m_TeamMembers[i]->m_GUID == pTMInfo1->m_GUID )
				{
					m_TeamMembers[i] = pTMInfo2;
					continue;
				}

				if ( m_TeamMembers[i]->m_GUID == pTMInfo2->m_GUID )
				{
					m_TeamMembers[i] = pTMInfo1;
					continue;
				}
			}
		}
		break;
	case GROUP_TYPE:
		{ // �Ŷ��������ʱ�����ǣ�
		}
		break;
	default:
		Assert(FALSE);
		return;
	}
}

VOID CTeamOrGroup::UpdateMemberInfo(TeamMemberInfo* member, GUID_t guid)
{
	TeamMemberInfo* pTMInfo;

	pTMInfo = GetMember(guid);
	if( pTMInfo == NULL )
	{
		Assert(FALSE);
		return;
	}

	switch(m_Type)
	{
	case TEAM_TYPE:
		{ // �������
		}
		break;
	case GROUP_TYPE:
		{ // �Ŷ����
		}
		break;
	default:
		Assert(FALSE);
		return;
	}
}

// �����¶ӳ�
VOID CTeamOrGroup::Appoint(GUID_t guid)
{ // ����ط�Ҫ hardcode �ˣ����������
	switch(m_Type)
	{
	case TEAM_TYPE:
		{ // �������
			std::vector< TeamMemberInfo* >::iterator it_member;
			TeamMemberInfo *pTMInfo;

			for( it_member = m_TeamMembers.begin(); it_member != m_TeamMembers.end(); ++it_member )
			{
				if( (*it_member)->m_GUID == guid )
				{
					pTMInfo = *it_member; // �ͷŶ�Ա��Ϣ
					m_TeamMembers.erase(it_member);
					m_TeamMembers.insert(m_TeamMembers.begin(), pTMInfo);
					SetLeaderGUID( guid );
					break;
				}
			}
		}
		break;
	case GROUP_TYPE:
		{ // �Ŷ����
		}
		break;
	default:
		Assert(FALSE);
		return;
	}
}

TeamMemberInfo* CTeamOrGroup::GetMember(GUID_t guid)
{
	switch(m_Type)
	{
	case TEAM_TYPE:
		{ // �������
			std::vector< TeamMemberInfo* >::iterator it_member;

			for( it_member = m_TeamMembers.begin(); it_member != m_TeamMembers.end(); ++it_member )
			{
				if( (*it_member)->m_GUID == guid )
				{
					return (*it_member); // ���ض�Ա��Ϣ
				}
			}
		}
		break;
	case GROUP_TYPE:
		{ // �Ŷ����
			std::vector< TeamInfo* >::iterator it_team;
			std::vector< TeamMemberInfo* >::iterator it_member;

			BOOL bFind = FALSE;

			for( it_team = m_TeamInfo.begin(); it_team != m_TeamInfo.end(); ++it_team )
			{
				for( it_member = (*it_team)->m_TeamMembers.begin(); it_member != (*it_team)->m_TeamMembers.end(); ++it_member )
				{
					if( (*it_member)->m_GUID == guid )
					{
						return (*it_member); // �ͷŶ�Ա��Ϣ
					}
				}
			}
		}
		break;
	default:
		//assert(FALSE);
		return NULL;
	}

	return NULL;
}

// ����server id �ҵ��������
TeamMemberInfo*	CTeamOrGroup::GetMemberByServerId(INT iServerId)
{
	switch(m_Type)
	{
	case TEAM_TYPE:
		{ // �������
			std::vector< TeamMemberInfo* >::iterator it_member;

			for( it_member = m_TeamMembers.begin(); it_member != m_TeamMembers.end(); ++it_member )
			{
				if ( (*it_member)->m_OjbID == iServerId )
				{ 
					return (*it_member);
				}
			}
		}
		break;
	case GROUP_TYPE:
		{ // �Ŷ����
			std::vector< TeamInfo* >::iterator it_team;
			std::vector< TeamMemberInfo* >::iterator it_member;

			BOOL bFind = FALSE;

			for( it_team = m_TeamInfo.begin(); it_team != m_TeamInfo.end(); ++it_team )
			{
				for( it_member = (*it_team)->m_TeamMembers.begin(); it_member != (*it_team)->m_TeamMembers.end(); ++it_member )
				{
					if ( (*it_member)->m_OjbID == iServerId )
					{ 
						return (*it_member);
					}
				}
			}
		}
		break;
	default:
		//assert(FALSE);
		return NULL;
	}

	return NULL;
}

const TeamMemberInfo* CTeamOrGroup::GetMemberByIndex(UINT idx, BYTE TeamIndex)
{
	switch(m_Type)
	{
	case TEAM_TYPE:
		{ // ��������
			if( m_TeamMembers.size() > idx )
			{
				return m_TeamMembers[idx];
			}
		}
		break;
	case GROUP_TYPE:
		{ // �Ŷӵ����
			if(TeamIndex<1 || TeamIndex>MAX_TEAMS_IN_GROUP)
			{
				Assert(FALSE);
				return NULL;
			}

			if( GetTeam(TeamIndex)->m_TeamMembers.size() > idx )
			{
				return GetTeam(TeamIndex)->m_TeamMembers[idx];
			}
		}
		break;
	default:
		Assert(FALSE);
		return NULL;
	}

	return NULL;
}

VOID CTeamOrGroup::SetUIFlag(UI_ON_OPEN flag)
{
	if ( flag == 0 )
	{
		// ˢ�����ݽṹ���ر����루���룩�����ʱ��һЩ�����������
		switch(m_UIFlag)
		{
		case UI_INVITE_ON_OPEN:
			{
				while( m_InviteTeams.size() > MAX_INVITE_TEAM )
				{
					EraseInviteTeam(0);
				}
			}
			break;
		case UI_APPLY_ON_OPEN:
			{
				while( m_Proposers.size() > MAX_PROPOSER )
				{
					EraseProposer(0);
				}
			}
			break;
		default:
			Assert( FALSE );
			return;
		}
	}

	m_UIFlag = flag;
}

// ����һ���������
BOOL CTeamOrGroup::AddInviteTeam(InviteTeam* pTeam)
{
	for( INT i=0; i<(INT)m_InviteTeams.size(); ++i )
	{
		if( m_InviteTeams[i]->m_InviterGUID == pTeam->m_InviterGUID )
		{ // ͬ���������˾Ͳ�������
			return FALSE;
		}
	}

	m_InviteTeams.push_back(pTeam);

	if ( m_InviteTeams.size() > MAX_INVITE_TEAM )
	{ // ֻ�����������ر�ʱ������ʱ����
		if ( m_UIFlag == UI_ALL_CLOSE )
		{
			EraseInviteTeam(0);
		}
	}

	return TRUE;
}

// ���������õ�ĳ������
const InviteTeam* CTeamOrGroup::GetInviteTeamByIndex(INT idx)
{
	if ( idx<0 || idx>=(INT)m_InviteTeams.size() )
	{
		Assert( FALSE && "�޷�ȡ���������" );
		return NULL;
	}

	return m_InviteTeams[idx];
}

// ���ĳ���������
VOID CTeamOrGroup::EraseInviteTeam(INT idx)
{
	InviteTeam* pInviteTeam;

	if ( idx<0 || idx>=(INT)m_InviteTeams.size() )
	{
		Assert( FALSE );
		return;
	}

	pInviteTeam = m_InviteTeams[idx];

	for( INT i=0; i<(INT)(pInviteTeam->m_InvitersInfo.size()); ++i )
	{ // �����Ա��Ϣ
		SAFE_DELETE( pInviteTeam->m_InvitersInfo[i] );
	}

	CInviteTeams::iterator it;

	pInviteTeam->m_InvitersInfo.clear();
	SAFE_DELETE( m_InviteTeams[idx] ); // ɾ��������Ϣ
	it = m_InviteTeams.begin() + idx;
	m_InviteTeams.erase(it);
}

// ����������
VOID CTeamOrGroup::ClearInviteTeam()
{
	for( INT i=(INT)(m_InviteTeams.size())-1; i>-1; --i )
	{
		EraseInviteTeam(i);
	}

	m_InviteTeams.clear();
}

// ����һ��������
BOOL CTeamOrGroup::AddProposer(TeamCacheInfo* pProposer)
{
	for( INT i=0; i<(INT)m_Proposers.size(); ++i )
	{
		if( m_Proposers[i]->m_GUID == pProposer->m_GUID )
		{ // ͬ���������˾Ͳ�������
			return FALSE;
		}
	}

	m_Proposers.push_back(pProposer);

	if ( m_Proposers.size() > MAX_PROPOSER )
	{ // ֻ�����������ر�ʱ������ʱ����
		if ( m_UIFlag == UI_ALL_CLOSE )
		{
			EraseProposer(0);
		}
	}

	return TRUE;
}

// ���һ��������
VOID CTeamOrGroup::EraseProposer(INT idx)
{
	if ( idx<0 || idx>=(INT)m_Proposers.size() )
	{
		Assert( FALSE );
		return;
	}

	CProposers::iterator it;

	SAFE_DELETE( m_Proposers[idx] );
	it = m_Proposers.begin() + idx;
	m_Proposers.erase(it);
}

// ����������
VOID CTeamOrGroup::ClearProposer()
{
	for( INT i=(INT)(m_Proposers.size())-1; i>-1; --i )
	{
		EraseProposer(i);
	}

	m_Proposers.clear();
}

// ���������õ�ĳ��������
const TeamCacheInfo* CTeamOrGroup::GetProposerByIndex(INT idx)
{
	if ( idx<0 || idx>=(INT)m_Proposers.size() )
	{
		Assert( FALSE );
		return NULL;
	}

	return m_Proposers[idx];
}

VOID CTeamOrGroup::SetLeaderGUID(GUID_t guid) 
{ 
	if(guid == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
	{
		CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_TeamLeaderFlag(TRUE);
	}
	else
	{
		CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_TeamLeaderFlag(FALSE);
	}

	m_LeaderID = guid; 
}

GUID_t	CTeamOrGroup::GetMemberGUIDByUIIndex(INT UIIndex, BYTE TeamIndex)
{
	const TeamMemberInfo*	pInfo = GetMemberByUIIndex(UIIndex, TeamIndex);
	if(pInfo)
	{
		return pInfo->m_GUID;
	}
	return -1;
}
// ͨ����������, �õ�server id
INT	CTeamOrGroup::GetMemberServerIdByUIIndex(INT UIIndex, BYTE TeamIndex)
{
	INT iServerId = -1;

	CTeamMembers* pTeamMembers;
	GUID_t myGUID = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();

	switch(m_Type)
	{
	case TEAM_TYPE:
		{ // ��������
			pTeamMembers = &m_TeamMembers;
		}
		break;
	case GROUP_TYPE:
		{ // �Ŷӵ����
			if(TeamIndex<1 || TeamIndex>MAX_TEAMS_IN_GROUP)
			{
				Assert(FALSE);
				return -1;
			}

			if( m_MyTeamID != TeamIndex )
			{ // ��ʱ��������ʾ�Ŷӳ�Ա��Ϣ
				return -1;
			}

			pTeamMembers = &(GetTeam(TeamIndex)->m_TeamMembers);
		}
		break;
	default:
		Assert(FALSE);
		return -1;
	}

	std::vector< TeamMemberInfo* >::iterator it_member;
	INT i;

	for( it_member = pTeamMembers->begin(), i=1; it_member != pTeamMembers->end(); ++it_member, ++i )
	{
		if ( (*it_member)->m_GUID == myGUID )
		{ // �Լ���ռ�����
			--i;
		}

		if ( i == UIIndex )
		{
			return (*it_member)->m_OjbID;
		}
	}

	return -1;
}


// �Ƿ���ͬһ��������.
BOOL CTeamOrGroup::IsInScene(INT iIndex)
{
	switch(m_Type)
	{
	case TEAM_TYPE:
		{ // ��������
			if( (int)m_TeamMembers.size() > iIndex )
			{
				TeamMemberInfo* pInfo = m_TeamMembers[iIndex];
				if(pInfo)
				{
					if(pInfo->m_SceneID == CWorldManager::GetMe()->GetActiveSceneID())
					{
						return TRUE;
					}
				}
				else
				{
					return FALSE;
				}
					
			}
			else
			{
				return FALSE;
			}
		}
		break;
	case GROUP_TYPE:
		{ // �Ŷӵ����
			
			return FALSE;
		}
		break;
	default:
		{
			return FALSE;
		}
	}

	return FALSE;

}

// ���� Buff �б�
VOID CTeamOrGroup::UpdateImpactsList( INT ObjID, const SimpleImpactList* pSimpleImpactList )
{
	TeamMemberInfo* pTMInfo = GetMemberByServerId( ObjID );
	if( pTMInfo == NULL )
	{
		return;
	}

	pTMInfo->m_SimpleImpactList.SetImpactList( pSimpleImpactList );

	INT idx = GetMemberUIIndex( pTMInfo->m_GUID );
	if( idx != INVALID_ID )
	{
		CGameProcedure::s_pEventSystem->PushEvent(GE_TEAM_UPTEDATA_MEMBER_INFO, idx);
	}

}

// ���� Buff
VOID CTeamOrGroup::AddImpact( INT ObjID, ImpactID_t ImpactID )
{
	TeamMemberInfo* pTMInfo = GetMemberByServerId( ObjID );
	if( pTMInfo == NULL )
	{
		return;
	}

	pTMInfo->m_SimpleImpactList.AddImpact( ImpactID );

	INT idx = GetMemberUIIndex( pTMInfo->m_GUID );
	if( idx != INVALID_ID )
	{
		CGameProcedure::s_pEventSystem->PushEvent(GE_TEAM_UPTEDATA_MEMBER_INFO, idx);
	}
}

// ���� Buff
VOID CTeamOrGroup::RemoveImpact( INT ObjID, ImpactID_t ImpactID )
{
	TeamMemberInfo* pTMInfo = GetMemberByServerId( ObjID );
	if( pTMInfo == NULL )
	{
		return;
	}

	pTMInfo->m_SimpleImpactList.RemoveImpact( ImpactID );

	INT idx = GetMemberUIIndex( pTMInfo->m_GUID );
	if( idx != INVALID_ID )
	{
		CGameProcedure::s_pEventSystem->PushEvent(GE_TEAM_UPTEDATA_MEMBER_INFO, idx);
	}
}
