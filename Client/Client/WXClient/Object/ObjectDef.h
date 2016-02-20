// ObjectDef.h
//
//////////////////////////////////////////////////////

#pragma once

#include "GameDefine.h"

// �ͻ���Ԥ��ָ����߼�������ֵ
#define DEF_CLIENT_LOGIC_COUNT_ADD_STEP		(100)

// ��ɫλ��У���ľ���
#define DEF_CHARACTER_POS_ADJUST_DIST		(2.f)

// �ͻ��˷���ɫλ�ý�����Ϣ�ľ��뷧ֵ  //fujia 2008.1.15   0.08f
#define DEF_CLIENT_ADJUST_POS_WARP_DIST		(DEF_SERVER_ADJUST_POS_WARP_DIST * 0.02f)

// ����ս��״̬��Ҫ������ʱ��
#define FIGHT_STATE_DURATION_TIME			(8000)

// �Ա�
enum ENUM_GENDER
{
	GENDER_INVALID	= -1,
	GENDER_MALE,
	GENDER_FEMALE
};

// ��������
enum ENUM_WEAPON_TYPE
{
	WEAPON_TYPE_INVALID	= -1,
	WEAPON_TYPE_NONE,		// ����
	WEAPON_TYPE_SPEAR,		// ��ǹ
	WEAPON_TYPE_LONG,		// ��
	WEAPON_TYPE_DSHORT,		// ˫��
	WEAPON_TYPE_SHORT,		// ����
	WEAPON_TYPE_FAN,		// ����
	WEAPON_TYPE_RING,		// ����
	WEAPON_TYPE_MONSTER,	// ����
	WEAPON_TYPE_NPC,		// NPC�����ǲ����Ա������� Monster

	WEAPON_TYPE_NUMBERS
};

#define ACTION_TYPE_NUMBERS	(10)
// ������
enum ENUM_BASE_ACTION
{
	BASE_ACTION_INVALID	= -1,
	BASE_ACTION_N_IDLE			=  0,			// (��ƽ)��Ϣ
	BASE_ACTION_N_IDLE_EX0		=  1,			// (��ƽ)��ϢС����1
	BASE_ACTION_N_IDLE_EX1		=  2,			// (��ƽ)��ϢС����2
	BASE_ACTION_N_WALK			=  3,			// (��ƽ)����
	BASE_ACTION_N_RUN			=  4,			// (��ƽ)�ܲ�
	BASE_ACTION_N_SIT_DOWN		=  5,			// (��ƽ)����
	BASE_ACTION_N_SIT_IDLE		=  6,			// (��ƽ)������Ϣ
	BASE_ACTION_N_SIT_STAND		=  7,			// (��ƽ)��������
	BASE_ACTION_N_RESERVE_00	=  8,			// ����
	BASE_ACTION_N_JUMP_N		=  9,			// (��ƽ)������Ծ��
	BASE_ACTION_N_RESERVE_01	= 10,			// ����
	BASE_ACTION_N_RESERVE_02	= 11,			// ����
	BASE_ACTION_N_JUMP_R		= 12,			// (��ƽ)�ƶ�����Ծ
	BASE_ACTION_N_RESERVE_03	= 13,			// ����
	BASE_ACTION_F_IDLE			= 14,			// (ս��)��Ϣ
	BASE_ACTION_F_WALK			= 15,			// (ս��)����
	BASE_ACTION_F_RUN			= 16,			// (ս��)�ܲ�
	BASE_ACTION_F_RESERVE_04	= 17,			// ����
	BASE_ACTION_F_JUMP_N		= 18,			// (ս��)������Ծ��
	BASE_ACTION_F_RESERVE_05	= 19,			// ����
	BASE_ACTION_F_RESERVE_06	= 20,			// ����
	BASE_ACTION_F_JUMP_R		= 21,			// (ս��)�ƶ�����Ծ
	BASE_ACTION_F_RESERVE_07	= 22,			// ����
	BASE_ACTION_F_BE_HIT		= 23,			// (ս��)������
	BASE_ACTION_F_DIE			= 24,			// (ս��)����
	BASE_ACTION_F_CADAVER		= 25,			// (ս��)ʬ��
	//BASE_ACTION_GATHER_MINE		= 26,			// �ɿ�
	//BASE_ACTION_GATHER_HERBS	= 27,			// �ɲ�ҩ
	//BASE_ACTION_FISH			= 28,			// ����

	//BASE_ACTION_M_IDLE_0			= 29,			// (������)��Ϣ
	//BASE_ACTION_M_IDLE_EX0_0		= 30,			// (������)��ϢС����1
	//BASE_ACTION_M_IDLE_EX1_0		= 31,			// (������)��ϢС����2
	//BASE_ACTION_M_WALK_0			= 32,			// (������)����
	//BASE_ACTION_M_RUN_0				= 33,			// (������)�ܲ�
	//BASE_ACTION_M_RESERVE_08		= 34,			// ����
	//BASE_ACTION_M_JUMP_N_0			= 35,			// (������)������Ծ��
	//BASE_ACTION_M_RESERVE_09		= 36,			// (����
	//BASE_ACTION_M_RESERVE_10		= 37,			// ����
	//BASE_ACTION_M_JUMP_R_0			= 38,			// (������)�ƶ�����Ծ
	//BASE_ACTION_M_RESERVE_11		= 39,			// ����

	//BASE_ACTION_M_IDLE_1			= 40,			// (������)��Ϣ
	//BASE_ACTION_M_IDLE_EX0_1		= 41,			// (������)��ϢС����1
	//BASE_ACTION_M_IDLE_EX1_1		= 42,			// (������)��ϢС����2
	//BASE_ACTION_M_WALK_1			= 43,			// (������)����
	//BASE_ACTION_M_RUN_1				= 44,			// (������)�ܲ�
	//BASE_ACTION_M_RESERVE_12		= 45,			// ����
	//BASE_ACTION_M_JUMP_N_1			= 46,			// (������)������Ծ��
	//BASE_ACTION_M_RESERVE_13		= 47,			// ����
	//BASE_ACTION_M_RESERVE_14		= 48,			// ����
	//BASE_ACTION_M_JUMP_R_1			= 49,			// (������)�ƶ�����Ծ
	//BASE_ACTION_M_RESERVE_15		= 50,			// ����

	BASE_ACTION_ABILITY_LEVEL_UP	= 92,			// ���������
	BASE_ACTION_XINFA_LEVEL_UP		= 93,			// �ķ�����
	BASE_ACTION_LEVEL_UP			= 99,			// ����

	BASE_ACTION_NUMBERS
};

// ��Ч�󶨵�
enum ENUM_LOCATOR
{
	LOCATOR_INVALID	= -1,
	LOCATOR_CENTER,			// ��������
	LOCATOR_HEAD,			// ͷ��
	LOCATOR_HAND_L,			// ����
	LOCATOR_HAND_R,			// ����
	LOCATOR_WEAPON_L,		// ������
	LOCATOR_WEAPON_R,		// ������
	LOCATOR_FOOT_L,			// ���
	LOCATOR_FOOT_R,			// �ҽ�
	LOCATOR_FOOT_CENTER,	// �ŵ�����
	LOCATOR_CALVARIA,		// ͷ��ƫ��
	LOCATOR_HAND_L_AND_R,	// ���ֺ�����
	LOCATOR_HAND_CENTER,	// ��������

	LOCATOR_NUMBERS
};

// return	:	������Ч�İ󶨵�
extern INT GetLocatorName( ENUM_LOCATOR eLocator, const char **paszOutLocatorName );



extern const char *szIDSTRING_WEAPON;
extern const char *szIDSTRING_CAP;
extern const char *szIDSTRING_ARMOUR;
extern const char *szIDSTRING_CUFF;
extern const char *szIDSTRING_BOOT;
extern const char *szIDSTRING_NECKLACE;
extern const char *szIDSTRING_SASH;
extern const char *szIDSTRING_RING;

extern const char *szIDSTRING_FACE_MESH;
extern const char *szIDSTRING_FACE_MAT;

extern const char *szIDSTRING_HAIR_MESH;
extern const char *szIDSTRING_HAIR_MAT;

extern const char *szIDSTRING_MAINBODY_MESH;
extern const char *szIDSTRING_MAINBODY_MAT;

extern const char *szIDSTRING_FOOT_MESH;
extern const char *szIDSTRING_FOOT_MAT;

extern const char *szIDSTRING_CAP_MESH;
extern const char *szIDSTRING_CAP_MAT;

extern const char *szIDSTRING_ARM_MESH;
extern const char *szIDSTRING_ARM_MAT;

//extern const char *szIDSTRING_CURRENT_ACTION;

extern const char *szIDSTRING_CURRENT_LEFTWEAPON;
extern const char *szIDSTRING_CURRENT_RIGHTWEAPON;
 
extern const char *szIDSTRING_WEAPON_MAT; 