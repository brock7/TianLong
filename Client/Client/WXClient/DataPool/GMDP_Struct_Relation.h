#pragma once

#include "Type.h"
#include "GameStruct_Relation.h"

// ��������
#define LIST_MEMBER_COUNT			20
#define RELATION_DESC_SIZE			32
#define	LOCATION_DESC_SIZE			32
#define	TEAM_DESC_SIZE				32

struct SDATA_RELATION_MEMBER
{
	GUID_t								m_GUID;								// GUID
	CHAR								m_szName[MAX_CHARACTER_NAME];		// ����

	RELATION_TYPE						m_RelationType;						// ��ϵ����
	INT									m_nFriendPoint;						// ���Ѷ�

	INT									m_nLevel;							// ��ɫ�ȼ�
	INT									m_nMenPai;							// ���� MENPAI_ATTRIBUTE
	INT									m_nPortrait;						// ͷ��
	GuildID_t							m_GuildID;							// ���ID
	CHAR								m_szGuildName[MAX_GUILD_NAME_SIZE];	// �������
	BOOL								m_bOnlineFlag;						// �Ƿ�����
	CHAR								m_szMood[MOOD_DATA_SIZE];			// ����
	CHAR								m_szTitle[MAX_CHARACTER_TITLE];		// �ƺ�
	SceneID_t							m_SceneID;							// ���ڳ���
	CHAR								m_szRelation[RELATION_DESC_SIZE];	// �����ϵ����
	CHAR								m_szLocation[LOCATION_DESC_SIZE];	// ����λ�ã����߻��߳�����
	INT									m_nTeamSize;						// ����������0 ��ʾδ��ӣ�
	CHAR								m_szTeamDesc[TEAM_DESC_SIZE];		// �������
	std::vector< INT >					m_vMailList;						// �ʼ������б�

	INT									m_nEnterOrder;						// ��ʱ���ѽ����˳��

	SDATA_RELATION_MEMBER()				{ CleanUp(); }
	VOID								CleanUp();
};

// ����
class RelationList
{
public:
	~RelationList();

	VOID								CleanUp();

	// ��������
	virtual BOOL						Add( SDATA_RELATION_MEMBER* pMember );

	// ���������Ƴ���ֻ�Ƴ�ָ�룬���ͷ�����
	virtual BOOL						Remove( INT idx );

	// ���������Ƴ���ֻ�Ƴ�ָ�룬���ͷ�����
	virtual BOOL						RemoveByGUID( GUID_t guid );

	// ���������Ƴ������ͷ�����
	virtual BOOL						Erase( INT idx );

	// ���������Ƴ������ͷ�����
	virtual BOOL						EraseByGUID( GUID_t guid );

	// �õ���ǰ��������
	inline INT							GetMemberCount();

	// �õ�һ����ϵ���ݣ������޸����ݣ�
	const SDATA_RELATION_MEMBER*		GetMember( INT idx );

	// �õ�һ����ϵ���ݣ������޸����ݣ�
	SDATA_RELATION_MEMBER*				GetMemberByGUID( GUID_t guid );

	// ͨ�����ֵõ�һ����ϵ���ݣ������޸����ݣ�
	SDATA_RELATION_MEMBER*				GetMemberByName( const CHAR* szName );
	
	// ͨ�����ֵõ��������е�λ�ã������޸����ݣ�
	INT									GetMemberIndexByName( const CHAR* szName );

	// �жϵ�ǰ�����Ƿ�����
	BOOL								IsFull();

	// �õ�ĳ�� GUID �������е�λ�ã���������ڣ��򷵻� -1
	inline INT							GetMemberIndex(GUID_t guid);

protected:

	// �ж��Ƿ�������Ҫ������
	virtual BOOL						IsFitType( SDATA_RELATION_MEMBER* pMember ) = 0;

	// ����ֵ��CSTR_LESS_THAN, CSTR_EQUAL, CSTR_GREATER_THAN,
	// ����ʱ���� 0
	static INT							CompareByName(const CHAR* szName1, const CHAR* szName2);

	// �Ƚ�������ϵ�˵������ŷ�˳��<0��ʾ���ȼ��ͣ�0��ʾ��ȣ�>0��ʾ���ȼ��ߣ�
	virtual INT							Compare(const SDATA_RELATION_MEMBER* pMember1, const SDATA_RELATION_MEMBER* pMember2);

protected:

	// ������ϵ����Ա
	std::vector<SDATA_RELATION_MEMBER*>	m_vecRelationMember;
};

// ��������
class FriendList : public RelationList
{
public:

protected:

	// �ж��Ƿ�������Ҫ������
	virtual BOOL						IsFitType( SDATA_RELATION_MEMBER* pMember );

	// ����һ�����ѵ����ȼ�����������Ŀǰ������ Compare ʹ�ã����Է���ֵ������
	INT									CalcPriority( const SDATA_RELATION_MEMBER* pMember );

	// �Ƚ�������ϵ�˵������ŷ�˳��<0��ʾ���ȼ��ͣ�0��ʾ��ȣ�>0��ʾ���ȼ��ߣ�
	virtual INT							Compare(const SDATA_RELATION_MEMBER* pMember1, const SDATA_RELATION_MEMBER* pMember2);

private:

	// ���һ�λ�ȡ��ϸ��Ϣ��ʱ�䣬���������ʹ�����ȡ���ں��ѽ��棨�� Tip����Ҫ��ʾ����Ϣ��
	//DWORD								m_dwTimeStamp;
};

// ��ʱ��ϵ����
class TempRelationList : public RelationList
{
public:

	// ��������
	virtual BOOL						Add( SDATA_RELATION_MEMBER* pMember );

protected:

	// �ж��Ƿ�������Ҫ������
	virtual BOOL						IsFitType( SDATA_RELATION_MEMBER* pMember );

private:

	INT									m_nMaxOrder;
};

// ������
class BlackList : public RelationList
{
public:

protected:

	// �ж��Ƿ�������Ҫ������
	virtual BOOL						IsFitType( SDATA_RELATION_MEMBER* pMember );

private:
};

class Relation
{
public:
	// �ӷ������˽���һ������
	BOOL								AddRelation( const _FRIEND_INFO* pFriend );

	// �ӷ������˽���һ�����������
	BOOL								AddRelation( const _BLACKNAME_INFO* pBlackName );

	// ���һ������������
	INT									GetListCount( RELATION_GROUP RelationGroup );

	// �ͻ��˻�ù�ϵ����Ϣ��������ʹ��
	const SDATA_RELATION_MEMBER*		GetRelationInfo( RELATION_GROUP RelationGroup, INT idx );

	// ����һ����ϵ��������Ϸ�����У�����ʱ��Ҫָ����Ҫ�������
	BOOL								AddRelation( RELATION_GROUP RelationGroup, SDATA_RELATION_MEMBER* pMember );

	// �������˷���Ϣ�����¹�ϵ����Ϣ
	BOOL								UpdateRelationInfo( RETURN_RELATION_INFO* pRelationInfo );

	// �ͻ����Ƴ�һ����ϵ��ͨ�������Ƴ���ʱ����
	BOOL								RemoveRelation( RELATION_GROUP RelationGroup, INT idx );

	// �ͻ����Ƴ�һ����ϵ��ͨ�������Ƴ���ʱ����
	BOOL								RemoveRelation( RELATION_GROUP RelationGroup, const CHAR* szName );

	// �ӷ��������յ�һ���Ƴ���ϵ�˵���Ϣ
	BOOL								RemoveRelation( GUID_t guid );

	// �ƶ�һ����ϵ��Srg ��ԭ�����飬guid ��Ҫ�ƶ������ GUID��Dtype ���ƶ���Ĺ�ϵ���ͣ�Drg ���ƶ������
	BOOL								MoveRelation( RELATION_GROUP Srg, RELATION_TYPE Dtype, RELATION_GROUP Drg, GUID_t guid );

	// ֪ͨ UI ˢ���б�Ĭ�ϸ��������б�
	VOID								UpdateUIList( RELATION_GROUP RelationGroup = RELATION_GROUP_FRIEND_ALL );

	// ֪ͨ UI ˢ�� RelationGroup �е� idx �Ź�ϵ�˵���ϸ��Ϣ
	VOID								UpdateUIInfo( RELATION_GROUP RelationGroup, INT idx );

	// �õ�һ�� GUID ���ڵ����Լ��������������û���ҵ����� RELATION_TYPE_STRANGER��������ܲ�������ʱ����
	RELATION_TYPE						GetRelationPosition( GUID_t guid, RELATION_GROUP& RelationGroup, INT& idx );

	// �õ�һ�� ���� ���ڵ����Լ��������������û���ҵ����� RELATION_TYPE_STRANGER��������ܲ�������ʱ����
	RELATION_TYPE						GetRelationByName( const CHAR* szName, INT& RelationGroup, INT& idx );

	// �������ߺ���
	BOOL								UpdateOnlineFriend( const _RELATION_ONLINE* pOnlineRelation );

	// ��ϵ������
	BOOL								RelationOnline(const CHAR* szName, const CHAR* szMood);

	// ��ϵ������
	BOOL								RelationOffLine(GUID_t guid);

	// �ж��Ƿ����Լ���������
	BOOL								IsBlackName( const CHAR* szName );

	// ��ĳ����������һ���ʼ����ʼ����е������ţ�
	BOOL								AddMail( const CHAR* szName, UINT uIndex );

	// �õ�ĳ�����ѵ���ʷ�ʼ�����
	UINT								GetMailCount( RELATION_GROUP RelationGroup, INT idx );

	// �õ�ĳ�����ѵĵ� uIndex ����ʷ�ʼ����ʼ���������
	UINT								GetMailIndex( RELATION_GROUP RelationGroup, INT idx, UINT uIndex );

	// ��ĳ������ʱ�������ؽ���ʷ��Ϣ
	VOID								RebuildMailHistory( RELATION_GROUP RelationGroup, const CHAR* szName );

	// �õ�����
	const CHAR*							GetMood( );

	// ��������
	VOID								SetMood( const CHAR* szMood );

	// �鿴��ҽ������Ϣ
	const SDATA_RELATION_MEMBER*		GetPlayerCheckInfo( );

protected:

	// ���һ������
	inline RelationList*				GetRelationList( RELATION_GROUP RelationGroup );

	// ���ݳ��� ID ��ó������֣������� szSceneName
	inline BOOL							GetLocationName( SceneID_t sid, CHAR* szSceneName );

	// ���ݹ�ϵ���ͻ����Ѻö�ȷ��˫����ϵ�������� m_szRelation
	inline BOOL							SetRelationDesc( SDATA_RELATION_MEMBER* pMember );

	// ���ݶ����������������ʾ���ݣ������� m_szTeamDesc
	inline BOOL							SetTeamDesc( SDATA_RELATION_MEMBER* pMember );

private:
	// MAX_MOOD_SIZE ��������������ַ�����
	CHAR								m_szMood[MOOD_DATA_SIZE];// ��������
	FriendList							m_FriendGroup1;			// ���� 1 ��
	FriendList							m_FriendGroup2;			// ���� 2 ��
	FriendList							m_FriendGroup3;			// ���� 3 ��
	FriendList							m_FriendGroup4;			// ���� 4 ��
	TempRelationList					m_TeamFriendGroup;		// ��ʱ������
	BlackList							m_BlackListGroup;		// ��������
	SDATA_RELATION_MEMBER				m_ViewPlayer;			// �鿴���
};
