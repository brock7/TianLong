
#pragma once

#include "Type.h"

//�������İ���ְλ
struct GuildAppointPos_t
{
	STRING		m_szPosName;			//ְλ��
	INT			m_PosID;				//ְλID
	GuildAppointPos_t()
	{
		CleanUp();
	}
	VOID CleanUp()
	{
		m_PosID			= -1;
		m_szPosName		= "";
	}
};

//���ɵ���ϸ��Ϣ
struct GuildDetailInfo_t
{
	STRING			m_GuildName;
	STRING			m_GuildCreator;
	STRING			m_GuildChairMan;
	STRING			m_CityName;
	BYTE			m_nLevel;
	INT				m_nPortSceneID;				//��ڳ���
	INT				m_MemNum;					//����
	INT				m_Longevity;				//���� 
	INT				m_Contribute;				//���׶�
	INT				m_Honor;					//����
	INT				m_FoundedMoney;				//�����ʽ�
	INT				m_nIndustryLevel;			//��ҵ��
	INT				m_nAgrLevel;				//ũҵ��
	INT				m_nComLevel;				//��ҵ��
	INT				m_nDefLevel;				//������
	INT				m_nTechLevel;				//�Ƽ���
	INT				m_nAmbiLevel;				//���Ŷ�

	GuildDetailInfo_t()
	{
		CleanUp();
	}

	VOID CleanUp()
	{
		m_GuildName			= "";
		m_GuildCreator		= "";
		m_GuildChairMan		= "";
		m_CityName			= "";
		m_nLevel			=	0;
		m_nPortSceneID		=	0;			//��ڳ���
		m_MemNum			=	0;			//����
		m_Longevity			=	0;			//���� 
		m_Contribute		=	0;			//���׶�
		m_Honor				=	0;			//����
		m_FoundedMoney		=	0;			//�����ʽ�
		m_nIndustryLevel	=	0;			//��ҵ��
		m_nAgrLevel			=	0;			//ũҵ��
		m_nComLevel			=	0;			//��ҵ��
		m_nDefLevel			=	0;			//������
		m_nTechLevel		=	0;			//�Ƽ���
		m_nAmbiLevel		=	0;			//���Ŷ�
	}

};

//������Ϣ
struct GuildMemberInfo_t
{
	STRING		m_szName;
	GUID_t		m_Guid;
	BYTE		m_bLevel;
	BYTE		m_bMenPaiID;
	INT			m_iCurContribute;
	INT			m_iMaxContribute;
	INT			m_iJoinTime;
	INT			m_iLogOutTime;
	BYTE		m_bIsOnline;
	BYTE		m_bPosition;
	
	GuildMemberInfo_t()
	{
		CleanUp();
	}
	VOID CleanUp()
	{
		m_szName			=	"";
		m_Guid				=	INVALID_ID;
		m_bLevel			=	0;
		m_bMenPaiID			=	0;
		m_iCurContribute	=	0;
		m_iMaxContribute	=	0;
		m_iJoinTime			=	0;
		m_iLogOutTime		=	0;
		m_bIsOnline			=	0;
		m_bPosition			=	0;
	}

};

//�����б���ÿһ�������
struct GuildInfo_t
{
	INT					m_GuildID;
	STRING				m_ChiefName;
	STRING				m_GuildName;
	STRING				m_GuildDesc;
	STRING				m_CityName;
	INT					m_PortSceneID;
	UCHAR				m_uGuildStatus;
	UCHAR				m_uGuildUserCount;
	BYTE				m_bGuildLevel;
	INT					m_nFoundedTime;

	GuildInfo_t()
	{
		CleanUp();
	}

	VOID CleanUp()
	{
		m_GuildID			= INVALID_ID;
		m_ChiefName			=	"";
		m_GuildName			=	"";
		m_GuildDesc			=	"";
		m_CityName			=	"";
		m_uGuildStatus		=	0;
		m_uGuildUserCount	=	0;
		m_bGuildLevel		=	0;
		m_PortSceneID		=	-1;
		m_nFoundedTime		=	0;
	}
};