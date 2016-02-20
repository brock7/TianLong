#ifndef __GAMEDEFINE_GUILD_H__
#define __GAMEDEFINE_GUILD_H__

#define MAX_NUM_PER_POSITION		20

enum GUILD_STATUS
{
	GUILD_STATUS_INVALID			= 0,
	GUILD_STATUS_NORMAL,
};

enum GUILD_POSITION
{
	GUILD_POSITION_INVALID			= 0,

	GUILD_POSITION_TRAINEE,					// Ԥ�����ڣ�����׼
	GUILD_POSITION_MEMBER,					// ��ͨ����

	GUILD_POSITION_ELITE_MEMBER,			// ��Ӣ����
	GUILD_POSITION_COM,						// ��ҵ��Ա
	GUILD_POSITION_AGRI,					// ũҵ��Ա
	GUILD_POSITION_INDUSTRY,				// ��ҵ��Ա
	GUILD_POSITION_HR,						// ���¹�Ա

	GUILD_POSITION_ASS_CHIEFTAIN,			// ������
	GUILD_POSITION_CHIEFTAIN,				// ����

	GUILD_POSITION_SIZE,					// ְλ������
};



enum GUILD_AUTHORITY
{
	GUILD_AUTHORITY_INVALID			= 0,

	GUILD_AUTHORITY_ASSIGN			= 0x1,		// ְ�����Ȩ
	GUILD_AUTHORITY_AUTHORIZE		= 0x2,		// Ȩ�޵���Ȩ
	GUILD_AUTHORITY_RECRUIT			= 0x4,		// ���հ���Ȩ
	GUILD_AUTHORITY_EXPEL			= 0x8,		// ��������Ȩ
	GUILD_AUTHORITY_DEMISE			= 0x10,		// ����Ȩ
	GUILD_AUTHORITY_WITHDRAW		= 0x20,		// ֧ȡ����Ȩ
	GUILD_AUTHORITY_DEPOSIT			= 0x40,		// ������Ȩ
	GUILD_AUTHORITY_LEAVE			= 0x80,		// �뿪���Ȩ�����Ȩ���е���ࣩ
	GUILD_AUTHORITY_DIMISS			= 0x100,	// ��ɢ���Ȩ


	GUILD_AUTHORITY_NUMBER			= 8,	// Ȩ������

	// ���ڵ�ͨ��Ȩ��
	GUILD_AUTHORITY_MEMBER			= GUILD_AUTHORITY_DEPOSIT | GUILD_AUTHORITY_LEAVE,
	GUILD_AUTHORITY_ASSCHIEFTAIN	= GUILD_AUTHORITY_ASSIGN | GUILD_AUTHORITY_RECRUIT | GUILD_AUTHORITY_EXPEL | GUILD_AUTHORITY_LEAVE,
	GUILD_AUTHORITY_CHIEFTAIN		= 0x17F,	// û���뿪����Ȩ��
};

enum GUILD_RETURN_TYPE
{
	GUILD_RETURN_INVALID			= -1,

	// ��������
	GUILD_RETURN_CREATE,					// �����ɹ�
	GUILD_RETURN_RESPONSE,					// ��Ӧ�ɹ�

	// ���벿��
	GUILD_RETURN_JOIN,						// ���������б�ȴ���׼

	// Ȩ�޲���
	GUILD_RETURN_PROMOTE,					// 0001������ְ����ְ
	GUILD_RETURN_DEMOTE,					// 0001������ְ�񣬽�ְ
	GUILD_RETURN_AUTHORIZE,					// 0002������Ȩ�ޣ���Ȩ
	GUILD_RETURN_DEPRIVE_AUTHORITY,			// 0002������Ȩ�ޣ����Ȩ��
	GUILD_RETURN_RECRUIT,					// 0003�����հ���
	GUILD_RETURN_EXPEL,						// 0004����������
	GUILD_RETURN_DEMISE,					// 0005������
	GUILD_RETURN_WITHDRAW,					// 0006��֧ȡ���
	GUILD_RETURN_DEPOSIT,					// 0007��������
	GUILD_RETURN_LEAVE,						// 0008���뿪���
	GUILD_RETURN_REJECT,					// 0009���ܾ�����
	
	GUILD_RETURN_FOUND,						// ��ʽ�����������ﵽ��
	GUILD_RETURN_DISMISS,					// ��ɢ���
	GUILD_RETURN_CHANGEDESC,				// �޸İ����ּ

	GUILD_RETURN_NAME,						// ˢ�°����

	GUILD_RETURN_SIZE,
};

enum GUILD_ERROR_TYPE
{
	// ��������
	GUILD_ERROR_NOTHING				= 0,	// ûʲô���󡭡�
	GUILD_ERROR_WANTING				= -1,	// �����ϴ���������ֻ���������������Ĵ��������ɽű�����
	GUILD_ERROR_INVALID_NAME		= -2,	// ���ַǷ�
	GUILD_ERROR_DUPLICATED_NAME		= -3,	// ����
	GUILD_ERROR_GUILD_FULL			= -4,	// ���ܴ���������

	// ���벿��
	GUILD_ERROR_MEMBER_FULL			= -5,	// �����Ա��
	GUILD_ERROR_PROPOSER_FULL		= -6,	// ����������
	GUILD_ERROR_IN_GUILD			= -7,	// ����Ѽ�����
	GUILD_ERROR_NOT_EXIST			= -8,	// ��᲻����

	// Ȩ�޲���
	GUILD_ERROR_UNAUTHORIZED		= -9,	// û��Ȩ��
	GUILD_ERROR_NO_VACANCY			= -10,	// 0001������ְ��û��ְλ��ȱ
	GUILD_ERROR_NO_QUALIFICATION	= -11,	// 0002������Ȩ�ޣ���������Ա��ְλ�������ʸ�
	GUILD_ERROR_CASHBOX_FULL		= -12,	// 0007����������������

	GUILD_ERROR_ALREADY_MEMBER		= -13,	// �����Ѿ��ǰ���Ա
	GUILD_ERROR_MEMBER_NOT_EXIST	= -14,	// ���˲��ڰ����

	//����
	GUILD_ERROR_NO_ASS_CHIEF		= -15,	// û�и�����
	GUILD_ERROR_GUILD_ALREADY_EXIST = -16,	//	����Ѵ���

	//���ְλ������������������
	GUILD_ERROR_POS_FULL			= -17,	//���ְλ������������������

	GUILD_ERROR_ALREADY_IN_PROPOSER_LIST = -18,		//�������Ѿ��������б�����

	GUILD_ERROR_INVALID_CAMP			= -19,		//�����˵���Ӫ�Ƿ�



	// ��ɢ����

	GUILD_ERROR						= -64,	// ��������
};

enum GUILD_PACKET_TYPE
{
	GUILD_PACKET_INVALID			= -1,
	GUILD_PACKET_CG_ASKLIST,				// ѯ�ʰ���б�(UCHAR)
	GUILD_PACKET_CG_CREATE,					// �������
	GUILD_PACKET_CG_JOIN,					// ������
	GUILD_PACKET_CG_ASKINFO,				// ѯ�ʰ����Ϣ
	GUILD_PACKET_CG_APPOINT,				// �������
	GUILD_PACKET_CG_ADJUSTAUTHORITY,		// �������Ȩ��
	GUILD_PACKET_CG_RECRUIT,				// �������
	GUILD_PACKET_CG_EXPEL,					// �������
	GUILD_PACKET_CG_WITHDRAW,				// ��ȡ����
	GUILD_PACKET_CG_DEPOSIT,				// �������
	GUILD_PACKET_CG_LEAVE,					// �뿪���
	GUILD_PACKET_CG_DISMISS,				// ��ɢ���
	GUILD_PACKET_CG_DEMISE,					// ��ɢ���

	GUILD_PACKET_CG_CHANGEDESC,				// ���İ����ּ

	GUILD_PACKET_CG_GW_SEPARATOR,			// �ָ���

	GUILD_PACKET_GW_ASKLIST,				// ѯ�ʰ���б�(GUID_t, UCHAR)
	GUILD_PACKET_GW_CREATE,					// �������
	GUILD_PACKET_GW_JOIN,					// ������
	GUILD_PACKET_GW_ASKINFO,				// ѯ�ʰ����Ϣ
	GUILD_PACKET_GW_APPOINT,				// �������
	GUILD_PACKET_GW_ADJUSTAUTHORITY,		// �������Ȩ��
	GUILD_PACKET_GW_RECRUIT,				// �������
	GUILD_PACKET_GW_EXPEL,					// �������
	GUILD_PACKET_GW_WITHDRAW,				// ��ȡ����
	GUILD_PACKET_GW_DEPOSIT,				// �������
	GUILD_PACKET_GW_LEAVE,					// �뿪���
	GUILD_PACKET_GW_DISMISS,				// ��ɢ���
	GUILD_PACKET_GW_DEMISE,					// ����
	GUILD_PACKET_GW_ASKMEMBERLIST,			// ����Ա��Ϣ�б�
	GUILD_PACKET_GW_CHANGEDESC,				// ���İ����ּ
	
	GUILD_PACKET_GW_WG_SEPARATOR,			// �ָ���

	GUILD_PACKET_WG_LIST,					// ����б�
	GUILD_PACKET_WG_MEMBER_LIST,			// �����б�
	GUILD_PACKET_WG_GUILD_INFO,				// �����Ϣ
	GUILD_PACKET_WG_APPOINT_INFO,			// ����������Ϣ
	GUILD_PACKET_WG_SELF_GUILD_INFO,		// ���˰�����Ϣ

	GUILD_PACKET_WG_GC_SEPARATOR,			// �ָ���

	GUILD_PACKET_GC_LIST,					// ����б�
	GUILD_PACKET_GC_MEMBER_LIST,			// �����б�
	GUILD_PACKET_GC_GUILD_INFO,				// �����Ϣ
	GUILD_PACKET_GC_APPOINT_INFO,			// ����������Ϣ
	GUILD_PACKET_GC_SELF_GUILD_INFO,		// ���˰�����Ϣ
};


#endif // __GAMEDEFINE_GUILD_H__
