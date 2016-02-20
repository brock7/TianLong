#ifndef _CHAR_CONFIG_H_
#define _CHAR_CONFIG_H_
#include "Type.h"

struct FULLUSERDATA;

struct START_SCENE //�����ص�
{
	INT		nSceneNum ; //��������
	FLOAT	fStartX ; //�����ص�
	FLOAT	fStartZ ;
};

struct SKILL_DATA
{
	INT nSkillID;
	INT nMenPai;
};

struct ATTR_DATA
{
	INT str1;
	INT spr1;
	INT con1;
	INT int1;
	INT dex1;
};

struct BASE_VALUE_TBL
{
	//����Ӱ����������*��������
	INT		m_Table[AINFOTYPE_NUMBER][MATTRIBUTE_NUMBER] ;

	BASE_VALUE_TBL( )
	{
		memset( m_Table, 0, sizeof(INT)*AINFOTYPE_NUMBER*MATTRIBUTE_NUMBER ) ;
	};
	INT		Get( UINT AttrInfo, UINT MenPai ){
		Assert( AttrInfo<AINFOTYPE_NUMBER && MenPai<MATTRIBUTE_NUMBER ) ;
		return m_Table[AttrInfo][MenPai] ;
	};
};

class CharConfig
{
public:

	CharConfig();
	~CharConfig();
	BOOL		Init();
	BOOL		InitCharAttr(FULLUSERDATA* pData);	//��Login �ϳ�ʼ����ɫ������Ϣ

public:

	VOID InitDefaultCharData();

	VOID InitSkillData();

	VOID InitLevel1Attr();

	VOID InitBaseValueTable();

	FULLUSERDATA*	m_pUserData; //��ɫ����

	START_SCENE*	m_StartScene; //��������Ӫ����

	SKILL_DATA*		m_SkillData; //��������
	INT				m_SkillData_Count;

	ATTR_DATA		m_Level1Attr[MATTRIBUTE_NUMBER]; //�ȼ�1����������

	BASE_VALUE_TBL	m_BaseValueTbl;
};


extern CharConfig g_CharConfig;

#endif