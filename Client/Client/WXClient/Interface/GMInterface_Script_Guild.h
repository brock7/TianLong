/***********************************\
*									*
*		�����ؽӿ�				*
*		2006-03-24					*
*									*
\***********************************/

#include "Type.h"

namespace LuaPlus
{
	class LuaState;
	class LuaObject;
};

namespace SCRIPT_SANDBOX
{
	class Guild
	{

	//LuaPlus������ؽӿ�
	public:
		//�������
		INT	CreateGuild(LuaPlus::LuaState* state);

		//�������ȷ��
		INT CreateGuildConfirm(LuaPlus::LuaState* state);

		//��World��������ϸ��Ϣ
		INT AskGuildDetailInfo(LuaPlus::LuaState* state);

		//��World�������Ա��Ϣ
		INT AskGuildMembersInfo(LuaPlus::LuaState* state);

		//��World������ְλ��Ϣ
		INT AskGuildAppointPosInfo(LuaPlus::LuaState* state);

		//������а�������
		INT	GetGuildNum(LuaPlus::LuaState* state);

		//�������������Ϣ
		INT GetGuildInfo(LuaPlus::LuaState* state);

		//������
		INT JoinGuild(LuaPlus::LuaState* state);

		//�˳����
		INT	QuitGuild(LuaPlus::LuaState* state);

		//�߳����,�ܾ�����
		INT KickGuild(LuaPlus::LuaState* state);

		//���ɻ�Ա
		INT RecruitGuild(LuaPlus::LuaState* state);

		//����Լ��İ�����Ϣ
		INT GetMembersNum(LuaPlus::LuaState* state);

		//Lua��ʾ��list�ؼ���λ��[4/16/2006]
		INT GetShowMembersIdx(LuaPlus::LuaState* state);
		INT GetShowTraineesIdx(LuaPlus::LuaState* state);

		//����Լ��İ�����ϸ��Ϣ
		INT	GetMembersInfo(LuaPlus::LuaState* state);

		//����Լ��İ�����Ϣ
		INT GetMyGuildInfo(LuaPlus::LuaState* state);

		//����Լ����ɵ���ϸ��Ϣ
		INT GetMyGuildDetailInfo(LuaPlus::LuaState* state);

		//����Լ��İ���Ȩ��
		INT GetMyGuildPower(LuaPlus::LuaState* state);

        //�޸����а���ְλ
		INT	AdjustMemberAuth(LuaPlus::LuaState* state);

		//�����λ������
		INT ChangeGuildLeader(LuaPlus::LuaState* state);

		//���ɾ��
		INT	DestoryGuild(LuaPlus::LuaState* state);

		//�޸İ����Ϣ
		INT FixGuildInfo(LuaPlus::LuaState* state);

		//�������
		INT DemisGuild(LuaPlus::LuaState* state);

		//׼������Ա����[4/16/2006]
		INT PrepareMembersInfomation(LuaPlus::LuaState* state);

	//���ú���
	public:
		//��ʾ��ʾ��Ϣ
		// msgType		��Ϣ�ţ��������ֵ����ȡ��Ӧ������
		// bTalk		��Ҫ��ʾ�����촰��
		// bTip			��Ҫ��ʾ����Ļ�м����ʾ
		VOID ShowMsg(INT msgType, BOOL bTalk = FALSE, BOOL bTip = TRUE);

		struct Name_Idx
		{
			STRING	m_MemberName;		//��ӦDataPool��GuildMemberInfo_t�ṹ���m_szName
			INT		m_MemberIdx;		//��ӦDataPool��GuildMemberInfo_t������ֵ
			INT		m_Position;			//�ڰ��е�ְλ

			Name_Idx()
			{
				m_MemberName.erase();
				m_MemberIdx = -1;
				m_Position = GUILD_POSITION_INVALID;
			}

			VOID	CleanUp()
			{
				m_MemberName.erase();
				m_MemberIdx = -1;
				m_Position = GUILD_POSITION_INVALID;
			}
		};

		struct ShowColor
		{
			STRING	m_OnlineLeaderColor;		//�����쵼����ʾ��ɫ
			STRING	m_LeaveLeaderColor;			//�����쵼����ʾ��ɫ
			STRING	m_OnlineMemberColor;		//���߰�����ʾ��ɫ
			STRING	m_LeaveMemberColor;			//���߰�����ʾ��ɫ
			STRING	m_OnlineTraineeColor;		//������������ʾ��ɫ
			STRING	m_LeaveTraineeColor;		//������������ʾ��ɫ

			ShowColor()
			{
				m_OnlineLeaderColor = "#B";			//��ɫ
				m_OnlineMemberColor = "#W";			//��ɫ
				m_OnlineTraineeColor = "#W";

				m_LeaveLeaderColor = "#c808080";	//��ɫ
				m_LeaveMemberColor = "#c808080";
				m_LeaveTraineeColor = "#c808080";
			}
		};
	private:
		//������ʱ��ת��
		VOID ConvertServerTime(INT dTime, STRING& strTime);

		//���°����Ӧ����ʾ��Ϣ
		VOID PerpareMembersBeforeShow();

		//ѡ����ʾ��ɫ
		STRING GetShowColor_For_Lua(INT idx);

		std::vector<STRING>		m_MsgArray;			//���ShowMsg����ʹ��
		INT						m_iMemberBak;		//�ı�ְλʱ�ı�����Ҫ�޸��ĸ���ҵ�ְλ��Ϣ

		//��Ա�б�
		typedef		std::vector<Name_Idx>	MEMBER_TABLE;
		typedef		std::vector<INT>		MEMBER_INDEX_TABLE;

		MEMBER_INDEX_TABLE						m_AllMembers;			//���г�Ա
		MEMBER_INDEX_TABLE						m_AllTrainees;			//����Ԥ����Ա
		ShowColor								m_LuaShowColors;		//Lua����ʾ����ɫ
	public:
		Guild();
		virtual ~Guild();

		static Guild s_Guild;
		static LuaPlus::LuaObject* s_pMetaTable;
	};

	class SimpleCmpFunc
	{
	public:
		static BOOL less_strcmp(const Guild::Name_Idx& s1, const Guild::Name_Idx& s2);
		static BOOL less_strcoll(const Guild::Name_Idx& s1, const Guild::Name_Idx& s2);

		static BOOL less_position(const Guild::Name_Idx&s1, const Guild::Name_Idx& s2);
	};
};