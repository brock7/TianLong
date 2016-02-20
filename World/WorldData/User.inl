#ifndef __USER_INL__
#define __USER_INL__

#include "Config.h"

// ȡ����ҵ� UserID
ID_t USER::GetUserID() const
{
	return m_UserID;
}

// ������ҵ� UserID
VOID USER::SetUserID( ID_t id )
{
	m_UserID = id;
}

// ȡ�����״̬
UINT USER::UserStatus() const
{
	return m_UserStatus;
};

// �������״̬
VOID USER::SetUserStatus( UINT uStatus )
{
	if( uStatus > US_NONE && uStatus < US_SIZE )
	{
		m_UserStatus = uStatus;
	}
	else
	{
		Assert( FALSE && "uStatus Exception!" );
	}
}

// ȡ�������������
FULLUSERDATA* USER::GetFullUserData() const
{
	return m_pFullUserData;
}

// ���������������
VOID USER::SetFullUserData( FULLUSERDATA* pData )
{
	m_pFullUserData = pData;
}

// ȡ����ҵ� ServerID
ID_t USER::GetServerID() const
{
	if( GetSceneID()<0 ) return INVALID_ID ;
	return g_Config.SceneID2ServerID(GetSceneID()) ;
}

// ȡ����� PlayerID
PlayerID_t USER::GetPlayerID() const
{
	return m_PlayerID;
}

// ������� PlayerID
VOID USER::SetPlayerID( PlayerID_t pid )
{
	m_PlayerID = pid;
}

// ȡ����� GUID
GUID_t USER::GetGUID() const
{
	return m_SimpleData.m_GUID;
}

// ������� GUID
VOID USER::SetGUID( GUID_t guid )
{
	m_SimpleData.m_GUID = guid;
}

// ȡ����� SceneID
SceneID_t USER::GetSceneID() const
{
	return m_SceneID;
}

// ������� SceneID
VOID USER::SetSceneID( SceneID_t SceneID )
{
	m_SceneID = SceneID;
}

// ȡ����� SceneID
SceneID_t USER::GetOldSceneID() const
{
	return m_OldSceneID;
}

// ������� SceneID
VOID USER::SetOldSceneID( SceneID_t SceneID )
{
	m_OldSceneID = SceneID;
}

// �õ���ҳ�������
const USER_SIMPLE_DATA* USER::GetUserSimpleData() const
{
	return &m_SimpleData;
}

// ȡ���������
const CHAR* USER::GetName() const
{
	return m_SimpleData.m_Name;
}

// �����������
VOID USER::SetName( const CHAR* szName )
{
	Assert(szName);
	strncpy( m_SimpleData.m_Name, szName, sizeof(m_SimpleData.m_Name)-1 );
}

// ȡ���˻�����
const CHAR* USER::GetAccount() const
{
	return m_SimpleData.m_Account;
}

// �����˻�����
VOID USER::SetAccount( const CHAR* szName )
{
	Assert(szName);
	strncpy( m_SimpleData.m_Account, szName, sizeof(m_SimpleData.m_Account)-1 );
}

// ȡ�ù���GUID
inline GUID_t USER::GetAccountGUID() const
{
	return m_SimpleData.m_AccountGUID;
}

// ���ù���GUID
inline VOID USER::SetAccountGUID( UINT uAccountGUID )
{
	m_SimpleData.m_AccountGUID = uAccountGUID;
}

// ȡ������Ա�
UINT USER::GetSex() const
{
	return m_SimpleData.m_uSex;
}

// ��������Ա�
VOID USER::SetSex( UINT uSex )
{
	m_SimpleData.m_uSex = uSex;
}

// ȡ����ҵ����ɺ�
INT USER::GetMenpai() const
{
	return m_SimpleData.m_uMenPai;
}

// ������ҵ����ɺ�
VOID USER::SetMenpai(INT nMenpai)
{
	m_SimpleData.m_uMenPai = nMenpai;
}

// �õ���ҵȼ�
INT USER::GetLevel() const
{
	return m_SimpleData.m_nLevel;
}

// ������ҵȼ�
VOID USER::SetLevel(INT nLevel)
{
	m_SimpleData.m_nLevel = nLevel;
}

// �õ���ҵ�ͷ��
INT USER::GetPortrait() const
{
	return m_SimpleData.m_nPortrait;
}

// �������ͷ��
VOID USER::SetPortrait(INT nPortrait)
{
	m_SimpleData.m_nPortrait = nPortrait;
}

// �õ�����ģ��
UCHAR USER::GetFaceModel() const
{
	return m_SimpleData.m_uFaceMeshID;
}

// ��������ģ��
VOID USER::SetFaceModel(UCHAR uFaceMeshID)
{
	m_SimpleData.m_uFaceMeshID = uFaceMeshID;
}

// �õ�ͷ��ģ��
UCHAR USER::GetHairModel() const
{
	return m_SimpleData.m_uHairMeshID;
}

// ����ͷ��ģ��
VOID USER::SetHairModel(UCHAR uHairMeshID)
{
	m_SimpleData.m_uHairMeshID = uHairMeshID;
}

// �õ���ɫ
UINT USER::GetHairColor() const
{
	return m_SimpleData.m_uHairColor;
}

// ���÷�ɫ
VOID USER::SetHairColor(UINT uHairColor)
{
	m_SimpleData.m_uHairColor = uHairColor;
}

// ȡ����ҵĶ����
TeamID_t USER::GetTeamID() const
{
	return m_TeamID;
}

// ������Ҷ����
VOID USER::SetTeamID( TeamID_t tid )
{
	m_TeamID = tid;
}

// ȡ����ҵĳƺ�
const CHAR* USER::GetTitle() const
{
	return m_SimpleData.m_szTitle;
}

// ������ҵĳƺ�
VOID USER::SetTitle(const CHAR* szTitle)
{
	Assert(szTitle);
	strncpy( m_SimpleData.m_szTitle, szTitle, MAX_CHARACTER_TITLE-1 );
}

// �õ���Ұ�� ID
GuildID_t USER::GetGuildID() const
{
	return m_SimpleData.m_GuildID;
}

// ������Ұ�� ID
VOID USER::SetGuildID(GuildID_t gid)
{
	m_SimpleData.m_GuildID = gid;
}

// �õ���Ҵ���������Ƶ����
ChannelID_t USER::GetChannelID() const
{
	return m_ChannelID;
}

// ������Ҵ���������Ƶ����
VOID USER::SetChannelID( ChannelID_t cid )
{
	m_ChannelID = cid;
}

// �õ��û���֤��
UINT USER::GetKey() const
{
	return m_uKey;
}

// �����û���֤��
VOID USER::SetKey( UINT uKey )
{
	m_uKey = uKey;
}

// ����������
VOID USER::CleanUpGuildData()
{
	m_SimpleData.m_GuildID = INVALID_ID;
	m_GuildData.CleanUp();
}

// �����ҹ�ϵ����
VOID USER::CleanUpRelationData()
{
	m_RelationData.CleanUp();
}

// ȡ����ҵ�����
const CHAR* USER::GetMood() const
{
	return m_RelationData.m_szMood;
}

// ��������
VOID USER::SetMood( const CHAR* szMood )
{
	Assert( szMood );
	strncpy( m_RelationData.m_szMood, szMood, MOOD_DATA_SIZE-1 );
}

// ��������
VOID USER::SetSettings(USHORT uSettings)
{
	m_RelationData.m_Flag.m_wFlags = uSettings;
}

// �Ƿ�ܾ�����Ϊ����
BOOL USER::IsCannotAddFriend()
{
	return m_RelationData.m_Flag.isSetBit(GSF_CANNOT_ADD_FRIEND);
}

// �Ƿ�ܾ��շ��ʼ�
BOOL USER::IsCannotRecvMail()
{
	return m_RelationData.m_Flag.isSetBit(GSF_CANNOT_RECV_MAIL);
}

// �Ƿ�ܾ�İ���˵��ʼ�
BOOL USER::IsCannotRecvStrangerMail()
{
	return m_RelationData.m_Flag.isSetBit(GSF_CANNOT_RECV_STRANGER_MAIL);
}

// �Ƿ�ܾ��������
BOOL USER::IsRefuseTeamInvite()
{
	return m_RelationData.m_Flag.isSetBit(GSF_REFUSE_TEAM_INVITE);
}

#endif // __USER_INL__
