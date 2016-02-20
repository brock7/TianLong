#ifndef __WORLD_USER_H__
#define __WORLD_USER_H__

#include "Type.h"
#include "IDTable.h"
#include "DB_Struct.h"

enum USER_STATUS
{
	US_NONE = 0 ,		//��
	US_HOLDUSERDATA ,	//ӵ�е�ǰ��ҵĵ�����Ϣ
	US_CHANGESCENE,		//��������л�����
	US_NORMAL ,			//���������Ϸ�У�һ������
	US_OFFLINE ,		//��Ҵ��ڶ���״̬��
	US_CRASH_DOWN,		//Server ���������
	US_WAIT_SHUTDOWN,	//Server �����˰�ȫͣ�������

	US_SIZE,
};

struct USER_GUILD_DATA
{
	UINT					m_uAccess;					//Ȩ��

	USER_GUILD_DATA( )
	{
		CleanUp( );
	}
	VOID CleanUp( )
	{
	}
};

struct _RELATION_USER
{
	GUID_t					m_UserGUID;
	RELATION_TYPE			m_RelationType;
	INT						m_nFriendPoint;				// Ŀǰ����ѺöȲ��ἴʱ���£��������ж��Ƿ���Ҫ��������ʾ

	VOID					CleanUp()
	{
		m_UserGUID			= INVALID_ID;
		m_RelationType		= RELATION_TYPE_NONE;
		m_nFriendPoint		= 0;
	}
};

struct _RELATION_LIST
{
	INT						m_nCount;
	_RELATION_USER			m_RelationUsers[MAX_RELATION_SIZE];

	VOID					CleanUp()
	{
		m_nCount			= 0;

		for( INT i=0; i<MAX_RELATION_SIZE; ++i )
		{
			m_RelationUsers[i].CleanUp();
		}
	}
};

struct USER_RELATION_DATA
{
	Flag16					m_Flag;							//������Ϣ��־���� enum GAME_SETTING_FLAG
	CHAR					m_szMood[MOOD_DATA_SIZE];		// ����
	_RELATION_LIST			m_RelationList;					// ��ϵ������

	IDTable					m_RelationGUIDTable;			// ���Ѻͺ��������ڴ˻���

	USER_RELATION_DATA()
	{
		CleanUp( );
		m_RelationGUIDTable.InitTable( MAX_RELATION_SIZE*2 );
	}
	VOID CleanUp( )
	{
		m_Flag.CleanUp( );
		memset( m_szMood, 0, sizeof(m_szMood) );
		m_RelationList.CleanUp();
		m_RelationGUIDTable.CleanUp();
	}
};


struct FULLUSERDATA;
//struct _CAMP_DATA;
class USER
{
public:

	USER( )
	{
		m_UserID			= INVALID_ID;
		CleanUp( );
	}
	void CleanUp( )
	{
		m_UserStatus		= US_NONE;
		m_pFullUserData		= NULL;
		m_PlayerID			= INVALID_ID;
		m_TeamID			= INVALID_ID;
		m_SceneID			= INVALID_ID;
		m_OldSceneID		= INVALID_ID;
		m_ChannelID			= INVALID_ID;
		m_uKey				= 0;
		m_SimpleData.CleanUp( );
		m_GuildData.CleanUp( );
		m_RelationData.CleanUp( );
		m_KickTime			= 0;
		m_LastTime			= 0;
	}

	// ȡ����ҵ� UserID
	inline ID_t				GetUserID() const;

	// ������ҵ� UserID
	inline VOID				SetUserID( ID_t id );

	// ȡ�����״̬
	inline UINT				UserStatus() const;

	// �������״̬
	inline VOID				SetUserStatus( UINT uStatus );

	// ȡ�������������
	inline FULLUSERDATA*	GetFullUserData() const;

	// ���������������
	inline VOID				SetFullUserData( FULLUSERDATA* pData );

	// ȡ����ҵ� ServerID
	inline ID_t				GetServerID() const;

	// ȡ����� PlayerID
	inline PlayerID_t		GetPlayerID() const;

	// ������� PlayerID
	inline VOID				SetPlayerID( PlayerID_t pid );

	// ȡ����� GUID
	inline GUID_t			GetGUID() const;

	// ������� GUID
	inline VOID				SetGUID( GUID_t guid );

	// ȡ����� SceneID
	inline SceneID_t		GetSceneID() const;

	// ������� SceneID
	inline VOID				SetSceneID( SceneID_t SceneID );

	// ȡ����� SceneID
	inline SceneID_t		GetOldSceneID() const;

	// ������� SceneID
	inline VOID				SetOldSceneID( SceneID_t SceneID );

	// �õ���ҳ�������
	inline const USER_SIMPLE_DATA* GetUserSimpleData() const;

	// ȡ���������
	inline const CHAR*		GetName() const;

	// �����������
	inline VOID				SetName( const CHAR* szName );

	// ȡ���˻�����
	inline const CHAR*		GetAccount() const;

	// �����˻�����
	inline VOID				SetAccount( const CHAR* szName );

	// ȡ�ù���GUID
	inline GUID_t			GetAccountGUID() const;

	// ���ù���GUID
	inline VOID				SetAccountGUID( UINT uAccountGUID );

	// ȡ������Ա�
	inline UINT				GetSex() const;

	// ��������Ա�
	inline VOID				SetSex( UINT uSex );

	// ȡ����ҵ����ɺ�
	inline INT				GetMenpai() const;

	// ������ҵ����ɺ�
	inline VOID				SetMenpai(INT nMenpai);

	// �õ���ҵȼ�
	inline INT				GetLevel() const;

	// ������ҵȼ�
	inline VOID				SetLevel(INT nLevel);

	// �õ���ҵ�ͷ��
	inline INT				GetPortrait() const;

	// �������ͷ��
	inline VOID				SetPortrait(INT nPortrait);

	// �õ�����ģ��
	inline UCHAR			GetFaceModel() const;

	// ��������ģ��
	inline VOID				SetFaceModel(UCHAR uFaceMeshID);

	// �õ�ͷ��ģ��
	inline UCHAR			GetHairModel() const;

	// ����ͷ��ģ��
	inline VOID				SetHairModel(UCHAR uHairMeshID);

	// �õ���ɫ
	inline UINT				GetHairColor() const;

	// ���÷�ɫ
	inline VOID				SetHairColor(UINT uHairColor);

	// ȡ����ҵĶ����
	inline TeamID_t			GetTeamID() const;

	// ������Ҷ����
	inline VOID				SetTeamID( TeamID_t tid );

	// ȡ����ҵĳƺ�
	inline const CHAR*		GetTitle() const;

	// ������ҵĳƺ�
	inline VOID				SetTitle(const CHAR* szTitle);

	// �õ���Ұ�� ID
	inline GuildID_t		GetGuildID() const;

	// ������Ұ�� ID
	inline VOID				SetGuildID(GuildID_t gid);

	// �õ���Ҵ���������Ƶ����
	inline ChannelID_t		GetChannelID() const;

	// ������Ҵ���������Ƶ����
	inline VOID				SetChannelID( ChannelID_t cid );

	// �õ��û���֤��
	inline UINT				GetKey() const;

	// �����û���֤��
	inline VOID				SetKey( UINT uKey );

	// ����������
	inline VOID				CleanUpGuildData();

	// �����ҹ�ϵ����
	inline VOID				CleanUpRelationData();

	// ����һ����ϵ��
	VOID					AddRelation( GUID_t guid, const _RELATION_USER* pUser );

	// �Ƴ�һ����ϵ��
	VOID					DelRelation( GUID_t guid );

	// �õ���ĳ��ҵĹ�ϵ״̬
	enum RELATION_TYPE		GetRelationType( GUID_t guid );

	// �ж� guid �Ƿ����ܺ���
	BOOL					IsGoodFriend(GUID_t guid);

	// ����һ�����ܺ���
	VOID					AddGoodFriend(GUID_t guid);

	// ȡ����ҵ�����
	inline const CHAR*		GetMood() const;

	// ��������
	inline VOID				SetMood( const CHAR* szMood );

	// ��������
	inline VOID				SetSettings(USHORT uSettings);

	// �Ƿ�ܾ�����Ϊ����
	inline BOOL				IsCannotAddFriend();

	// �Ƿ�ܾ��շ��ʼ�
	inline BOOL				IsCannotRecvMail();

	// �Ƿ�ܾ�İ���˵��ʼ�
	inline BOOL				IsCannotRecvStrangerMail();

	// �Ƿ�ܾ��������
	inline BOOL				IsRefuseTeamInvite();

	// ������������б�
	VOID					FillOnlineList( RETURN_ONLINE_LIST* pSend );

	// ֪ͨ��������������Ϣ
	VOID					InformOnlineMsg();

	// ֪ͨ��������������Ϣ
	VOID					InformOfflineMsg();

	// ������ҷ���ĳ�������ߵ���Ϣ
	VOID					SendRelationOnlineMsg(USER* pUser);

	// ������ҷ���ĳ�������ߵ���Ϣ
	VOID					SendRelationOfflineMsg(USER* pUser);

	// �����Ƿ���Է����ʼ���������ԵĻ����򷵻� 0�����򷵻ش���ţ�����Ųμ� enum RELATION_RETURN_TYPE
	UCHAR					ValidateMail(const MAIL* pMail);

	// �յ�һ���ʼ�
	BOOL					RecvMail(const MAIL* pMail);

	// ��������
	BOOL					HeartBeat( UINT uTime );

	// �����߳�ʱ��
	VOID					ResetKickTime( );

	const _CAMP_DATA*		GetUserCampData();

	VOID 					SetUserCampData( _CAMP_DATA* pData );

	BYTE					GetUserAge();

	VOID 					SetUserAge( BYTE age );
public:
	BOOL					IsEnemy( const _CAMP_DATA* pData );

private:
	ID_t					m_UserID;			//������ҵ��û� ID, �������û��б�������±�
	UINT					m_UserStatus;		//������ҵ�״̬�� enum USER_STATUS
	FULLUSERDATA*			m_pFullUserData;	//������ҵ���������
	PlayerID_t				m_PlayerID;			//���û����ڳ�����PlayerID
	SceneID_t				m_SceneID;			//���û����ڳ���ID
	SceneID_t				m_OldSceneID;		//���û����ڳ���ID
	TeamID_t				m_TeamID;			//���û����ڵĶ���ID
	ChannelID_t				m_ChannelID;		//���û�����������Ƶ����
	UINT					m_uKey;				//�û���֤��
	USER_SIMPLE_DATA		m_SimpleData;		//���û��Ļ�����Ϣ
	USER_GUILD_DATA			m_GuildData;		//�����������
	USER_RELATION_DATA		m_RelationData;		//��ϵ���������
	INT						m_KickTime;			//û����Ϣʱ����߳�ʣ��ʱ��
	UINT					m_LastTime;			//��һ�ε���ʱ��
	_CAMP_DATA				m_CampData;			//�����Ӫ
	BYTE					m_Age;				//�������
};

#include "User.inl"

#endif // __WORLD_USER_H__
