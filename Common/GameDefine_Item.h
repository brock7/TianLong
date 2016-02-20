
#ifndef __GAMEDEFINE_ITEM_H__
#define __GAMEDEFINE_ITEM_H__

enum HUMAN_EQUIP
{
	HEQUIP_WEAPON		=0,		//����	WEAPON
	HEQUIP_CAP			=1,		//ñ��	DEFENCE
	HEQUIP_ARMOR		=2,		//����	DEFENCE
	HEQUIP_CUFF			=3,		//����	DEFENCE
	HEQUIP_BOOT			=4,		//Ь	DEFENCE
	HEQUIP_SASH			=5,		//����	ADORN
	HEQUIP_RING			=6,		//����	ADORN
	HEQUIP_NECKLACE		=7,		//����	ADORN
	HEQUIP_RIDER		=8,		//���	ADORN
	HEQUIP_UNKNOW1		=9,
	HEQUIP_UNKNOW2		=10,
	HEQUIP_UNKNOW3		=11,
	HEQUIP_UNKNOW4		=12,
	HEQUIP_UNKNOW5		=13,
	HEQUIP_UNKNOW6		=14,
	HEQUIP_UNKNOW7		=15,
	HEQUIP_UNKNOW8		=16,
	HEQUIP_UNKNOW9		=17,
	HEQUIP_UNKNOW10		=18,
	HEQUIP_UNKNOW11		=19,
	HEQUIP_TOTAL		=20,
	HEQUIP_NUMBER		=9,		//����װ��������
};

enum ITEM_ATTRIBUTE
{
	IATTRIBUTE_POINT_MAXHP		=0,		//����������HP������
	IATTRIBUTE_RATE_MAXHP		=1,		//���ٷֱ�����HP������
	IATTRIBUTE_RESTORE_HP		=2,		//�ӿ�HP�Ļظ��ٶ�

	IATTRIBUTE_POINT_MAXMP		=3,		//����������MP������
	IATTRIBUTE_RATE_MAXMP		=4,		//���ٷֱ�����MP������
	IATTRIBUTE_RESTORE_MP		=5,		//�ӿ�MP�Ļظ��ٶ�
	
	IATTRIBUTE_COLD_ATTACK		=6,		//������
	IATTRIBUTE_COLD_RESIST		=7,		//���ֿ�
	IATTRIBUTE_COLD_TIME		=8,		//���ٱ����ٻ�ʱ��
	IATTRIBUTE_FIRE_ATTACK		=9,		//�𹥻�
	IATTRIBUTE_FIRE_RESIST		=10,	//��ֿ�
	IATTRIBUTE_FIRE_TIME		=11,	//���ٻ��ճ���ʱ��
	IATTRIBUTE_LIGHT_ATTACK		=12,	//�繥��
	IATTRIBUTE_LIGHT_RESIST		=13,	//��ֿ�
	IATTRIBUTE_LIGHT_TIME		=14,	//���ٵ��ѣ��ʱ��
	IATTRIBUTE_POISON_ATTACK	=15,	//������
	IATTRIBUTE_POISON_RESIST	=16,	//���ֿ�
	IATTRIBUTE_POISON_TIME		=17,	//�����ж�ʱ��
	IATTRIBUTE_RESIST_ALL		=18,	//���ٷֱȵ����������Թ���
	
	IATTRIBUTE_ATTACK_P			=19,	//������
	IATTRIBUTE_RATE_ATTACK_P	=20,	//���ٷֱ�����������
	IATTRIBUTE_RATE_ATTACK_EP	=21,	//��װ�������������ٷֱȼӳ�
	IATTRIBUTE_DEFENCE_P		=22,	//�������
	IATTRIBUTE_RATE_DEFENCE_P	=23,	//���ٷֱ������������
	IATTRIBUTE_RATE_DEFENCE_EP	=24,	//��װ��������������ٷֱȼӳ�
	IATTRIBUTE_IMMUNITY_P		=25,	//���ٷֱȵ��������˺�
	
	IATTRIBUTE_ATTACK_M			=26,	//ħ������
	IATTRIBUTE_RATE_ATTACK_M	=27,	//���ٷֱ�����ħ������
	IATTRIBUTE_RATE_ATTACK_EM	=28,	//��װ������ħ�������ٷֱȼӳ�
	IATTRIBUTE_DEFENCE_M		=29,	//ħ������
	IATTRIBUTE_RATE_DEFENCE_M	=30,	//���ٷֱ�����ħ������
	IATTRIBUTE_RATE_DEFENCE_EM	=31,	//��װ������ħ�������ٷֱȼӳ�
	IATTRIBUTE_IMMUNITY_M		=32,	//���ٷֱȵ���ħ���˺�
	
	IATTRIBUTE_ATTACK_SPEED		=33,	//�����ٶ�
	IATTRIBUTE_SKILL_TIME		=34,	//ħ����ȴ�ٶ�
	
	IATTRIBUTE_HIT				=35,	//����
	IATTRIBUTE_MISS				=36,	//����
	IATTRIBUTE_2ATTACK_RATE		=37,	//����һ����˫���������İٷֱ�
	IATTRIBUTE_NO_DEFENCE_RATE	=38,	//���ӶԷ������ĸ���

	IATTRIBUTE_SPEED_RATE		=39,	//�ƶ��ٶȰٷֱ�

	IATTRIBUTE_DAMAGE_RET		=40,	//�˺�����
	IATTRIBUTE_DAMAGE2MANA		=41,	//�˺�����������
	
	IATTRIBUTE_STR				=42,	//��������
	IATTRIBUTE_SPR				=43,	//��������
	IATTRIBUTE_CON				=44,	//��������
	IATTRIBUTE_INT				=45,	//���Ӷ���
	IATTRIBUTE_DEX				=46,	//������
	IATTRIBUTE_LUK				=47,	//��������
	IATTRIBUTE_ALL				=48,	//�������е�����һ������
	
	IATTRIBUTE_HP_THIEVE		=49,	//����͵ȡ(���˺���)
	IATTRIBUTE_MP_THIEVE		=50,	//����͵ȡ(���˺���)

	IATTRIBUTE_USESKILL			=51,	//����ĳ��ʹ�ü���
	IATTRIBUTE_RAND_SKILL		=52,	//����ĳ���������
	IATTRIBUTE_SKILL_RATE		=53,	//������ܷ�������

	IATTRIBUTE_BASE_ATTACK_P	=54,	//����������
	IATTRIBUTE_BASE_ATTACK_M	=55,	//����ħ������
	IATTRIBUTE_BASE_ATTACK_TIME =56,	//���������ٶȣ�ֻ����ͨ������
	IATTRIBUTE_BASE_DEFENCE_P	=57,	//�����������
	IATTRIBUTE_BASE_DEFENCE_M	=58,	//����ħ������
	IATTRIBUTE_BASE_MISS		=59,	//��������



	IATTRIBUTE_NUMBER, //��Ʒ������������
};

enum ITEM_CLASS
{
	ICLASS_EQUIP		=1,	//����WEAPON������DEFENCE������ADORN
	ICLASS_MATERIAL		=2,	//ԭ��
	ICLASS_COMITEM		=3,	//ҩƷ
	ICLASS_TASKITEM		=4,	//������Ʒ
	ICLASS_GEM			=5, //��ʯ
	ICLASS_STOREMAP		=6,	//�ر�ͼ
	ICLASS_TALISMAN		=7,	//����---???
	ICLASS_GUILDITEM	=8,	//�����Ʒ
	ICLASS_NUMBER, //��Ʒ���������
};

// ICLASS_COMITEM
enum COMMON_ITEM_TYPE
{
	COMMON_ITEM_TYPE    =0,
	COMMON_ITEM_1,			// ��ʱδ���� �ɲ߻���
	COMMON_ITEM_2,			// ��ʱδ����
	COMMON_ITEM_3,			// ��ʱδ����
	COMITEM_PRESCR		=4, // �䷽
	COMITEM_WPIDENT		=6,	// ������������
	COMITEM_ARIDENT		=7,	// ���߼�������
	COMITEM_NCIDENT		=8,	// ������������
};

enum COMMON_ITEM_QUAL
{
	COMITEM_QUAL_MIS	=5,	//����
};

enum EQUIP_QUALITY
{
	EQUIP_QUALITY		=0,
	EQUALITY_NORMAL		=1,	//��ͨ��ɫװ��
	EQUALITY_BLUE		=2,	//��ɫװ��
	EQUALITY_YELLOW		=3,	//��ɫװ��
	EQUALITY_GREEN		=4,	//��ɫ��װ

	EQUALITY_NUMBER, //װ��Ʒ������
};

enum WEAPON_TYPE
{
	WTYPE_DAO			=0,	//��
	WTYPE_QIANG			=1,	//��ǹ
	WTYPE_1DUAN			=2,	//����
	WTYPE_2DUAN			=3,	//˫��
	WTYPE_SHAN			=4,	//��
	WTYPE_HUAN			=5,	//��

	WTYPE_NUMBER, //������������ WEAPON
};

enum DEFENCE_TYPE
{
	DTYPE_CAP			=0,	//ñ��
	DTYPE_ARMOR			=1,	//����
	DTYPE_CUFF			=2,	//����
	DTYPE_BOOT			=3,	//Ь

	DTYPE_NUMBER, //������������ DEFENCE
};

enum ADORN_TYPE
{
	ATYPE_NECKLACE		=0,	//����
	ATYPE_SASH			=1,	//����
	ATYPE_RING			=2,	//����

	ATYPE_NUMBER, //������������ ADORN
};

enum ENUM_UPDATE_EQIPMENT
{
	UPDATE_EQIPMENT_WEAPON_ID	= 0,	// ����
	UPDATE_EQIPMENT_CAP_ID,				// ñ��
	UPDATE_EQIPMENT_ARMOUR_ID,			// �·�
	UPDATE_EQIPMENT_WRIST_ID,			// ����
	UPDATE_EQIPMENT_FOOT_ID,			// ѥ��
	UPDATE_EQIPMENT_NUMBERS
};

enum ITEMBOX_TYPE
{
	ITYPE_DROPBOX	=	-1,
	ITYPE_GROWPOINT

};

//ʰȡ����
enum	PICK_RULER
{
	IPR_FREE_PICK,	//����ʰȡ
	IPR_BET_PICK,	//Ͷ����ʰȡ
	IPR_TURN_PICK,	//����ʰȡ

};

//�������
enum	BOX_DISTRIBUTE_RULER
{
	BDR_COMMON	=	 0,
	BDR_BOSS	=	 1,
	BDR_UNKNOW	=	 0xFFFF
};

//��Ʒ�������
enum	ITEM_DISTRIBUTE_RULER
{
	ITEMDR_RANDOM,
	ITEMDR_ALLGET
};

//ItemContainer ����
enum	ITEM_CONTAINER_TYPE
{
	ICT_UNKNOW_CONTAINER,
	ICT_BASE_CONTAINER,
	//ICT_TASK_CONTAINER,
	//ICT_MAT_CONTAINER,
	ICT_EQUIP_CONTAINER,
	ICT_BANK_CONTAINER,
	ICT_ITEMBOX_CONTAINER,
	ICT_EXCHANGEBOX_CONTAINER,
	ICT_HUMAN_PET_CONTAINER,
};

// Ŀ������
enum ENUM_ITEM_TARGET_TYPE
{
	ITEM_TARGET_TYPE_INVALID	= -1,
	ITEM_TARGET_TYPE_NONE,				// ����Ŀ��		:	��
	ITEM_TARGET_TYPE_POS,				// λ��			:	TargetPos
	ITEM_TARGET_TYPE_DIR,				// ����			:	TargetDir
	ITEM_TARGET_TYPE_ITEM,				// ����			:	TargetItemIndex
	ITEM_TARGET_TYPE_SELF,				// ����			:	TargetObj
	ITEM_TARGET_TYPE_SELF_PET,			// ���ѵĳ���	:	TargetObj,TargetPetGUID
	ITEM_TARGET_TYPE_FRIEND,			// �Ѻ�Ŀ��		:	TargetObj
	ITEM_TARGET_TYPE_FRIEND_PLAYER,		// �Ѻ����		:	TargetObj
	ITEM_TARGET_TYPE_FRIEND_MONSTER,	// �Ѻù���		:	TargetObj
	ITEM_TARGET_TYPE_FRIEND_PET,		// �Ѻó���		:	TargetObj
	ITEM_TARGET_TYPE_ENEMY,				// �ж�Ŀ��		:	TargetObj
	ITEM_TARGET_TYPE_ENEMY_PLAYER,		// �ж����		:	TargetObj
	ITEM_TARGET_TYPE_ENEMY_MONSTER,		// �жԹ���		:	TargetObj
	ITEM_TARGET_TYPE_ENEMY_PET,			// �жԳ���		:	TargetObj
	ITEM_TARGET_TYPE_ALL_CHARACTER,		// ���н�ɫ		:	TargetObj

	ITEM_TARGET_TYPE_NUMBERS
};

// ���߷�����
#define ITEM_PET_SKILL_STUDY_BEGIN		30402000	//���＼������С���
#define ITEM_PET_SKILL_STUDY_END		30403000	//���＼���������

#define ITEM_PET_RETURN_BABAY_BEGIN		30503011	//���ﻹͯ����С��ţ�ע��30503011����Ҳ�ǺϷ��Ļ�ͯ����ţ�
#define ITEM_PET_RETURN_BABAY_END		30503020	//���ﻹͯ������ţ�ע��30503020����Ҳ�ǺϷ��Ļ�ͯ����ţ�

#define	ITEM_DOME_MEDICINE				30601000	//ѱ������ 
#define	ITEM_MEAT_MEDICINE				30602000	//��ʳ���� 
#define	ITEM_GRASS_MEDICINE				30603000	//������� 
#define	ITEM_WORM_MEDICINE				30604000	//������� 
#define	ITEM_PADDY_MEDICINE				30605000	//�������

#define ITEM_PET_FEED_MEDICINE_MAX		ITEM_PADDY_MEDICINE+1000

#define ITEM_PET_ADD_LIFE_BEGIN			ITEM_PET_FEED_MEDICINE_MAX	//�����ӳ�������С���
#define ITEM_PET_ADD_LIFE_END			30607000	//�����ӳ����������

//����
//����Ʊ���
#define	MAX_TICKET_VALUE	10000000	

//���̹�Ʊ����(������Ʒ��һ�������������Ʒ)
#define TICKET_ITEM_INDEX	 40002000

//���˹�Ʊ����(������Ʒ��һ�������������Ʒ)
#define TICKET_RIVER_TRANSPORTATION_INDEX	 30001000

//������Ʒ������ʼ
#define TICKET_MERCHANDISE_INDEX_BEGIN	 20400001

//������Ʒ������β
#define TICKET_MERCHANDISE_INDEX_END	 20400200

//�����Ƿ�����������Ʒ��������
#define IS_TICKET_MERCHANDISE_INDEX(idx)	((idx>=TICKET_MERCHANDISE_INDEX_BEGIN && idx<TICKET_MERCHANDISE_INDEX_END)? TRUE : FALSE)

//����Ʒ�Ƿ������̹�Ʊ
#define ITEM_IS_TICKET(pitem)	((pitem != NULL) ? ((pitem->GetItemTableIndex() == TICKET_ITEM_INDEX) ? TRUE:FALSE):FALSE)

//���ϵĹ�Ʊ�����Ƿ�Ϸ�
#define	TICKET_NUM_IS_IN_LAW(phuman) ((HumanItemLogic::CalcBagItemCount(phuman,TICKET_ITEM_INDEX) == 1)? TRUE:FALSE)

//�������̻����ϵļ۸����ݵ�start
#define	TICKET_MERCHANDISE_ITEM_PARAM_BUY_PRICE_START	0		

//�������̻����ϵļ۸����ݵ�����
#define	TICKET_MERCHANDISE_ITEM_PARAM_BUY_PRICE_TYPE	IPV_INT

//�������̻����ϵĳ��ۼ۸��start
#define	TICKET_MERCHANDISE_ITEM_PARAM_SALE_PRICE_START	4		

//�������̻����ϵĳ��ۼ۸������
#define	TICKET_MERCHANDISE_ITEM_PARAM_SALE_PRICE_TYPE	IPV_INT	

//�������̻����ϵĳ���ID���ݵ�start
#define	TICKET_MERCHANDISE_ITEM_PARAM_SCENE_START	8		

//�������̻����ϵļ۸����ݵ�����
#define	TICKET_MERCHANDISE_ITEM_PARAM_SCENE_TYPE	IPV_SHORT

//����������Ʊ�ϵĵ�ǰ�ֽ����ݵ�start
#define	TICKET_ITEM_PARAM_CUR_MONEY_START	0		

//����������Ʊ�ϵĵ�ǰ�ֽ����ݵ�type
#define	TICKET_ITEM_PARAM_CUR_MONEY_TYPE	IPV_INT

//����������Ʊ�ϵĵ�ǰ�ֽ����޵�start
#define	TICKET_ITEM_PARAM_MAX_MONEY_START	4		

//����������Ʊ�ϵĵ�ǰ�ֽ����޵�type
#define	TICKET_ITEM_PARAM_MAX_MONEY_TYPE	IPV_INT


//����Ʒ�Ƿ��ǹ�Ʊ��Ʒ
#define	ITEM_IS_TICKET_MERCHANDISE(pitem)	((pitem != NULL) ? ((pitem->GetItemTableIndex() >= TICKET_MERCHANDISE_INDEX_BEGIN && pitem->GetItemTableIndex() < TICKET_MERCHANDISE_INDEX_END) ? TRUE:FALSE):FALSE)	

//����Ʒ�����Ƿ��ǹ�Ʊ��Ʒ
#define	ITEM_IDX_IS_TICKET_MERCHANDISE(itemidx)	( (itemidx >= TICKET_MERCHANDISE_INDEX_BEGIN && itemidx < TICKET_MERCHANDISE_INDEX_END) ? TRUE:FALSE)	

enum	REFRESH_ITEM_TYPE
{
	REFRESH_TICKET_MERCHANDISE = 0,
	REFRESH_MAX,
};

//��Ʒ��Ϣλ����
enum ITEM_EXT_INFO
{
	IEI_BIND_INFO		=	0x00000001,	//����Ϣ
	IEI_IDEN_INFO		=	0x00000002, //������Ϣ
	IEI_PLOCK_INFO		=	0x00000004, //���������Ѿ�����
	IEI_BLUE_ATTR		=	0x00000008, //�Ƿ���������
	IEL_CREATOR			=	0x00000010,	//�Ƿ��д�����
};


#endif
