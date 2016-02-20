
#ifndef __GAMEDEFINE_H__
#define __GAMEDEFINE_H__

/////////////////////////////////////////////////////////////////////////////////
//��Ϸ������Ϣ�궨��
/////////////////////////////////////////////////////////////////////////////////

//��������ֵ
#define MAX_WORLD 255

#define MAX_WORLD_USER	3000

//�û���ɫ���ֿ�����������ֵ
#define MAX_CHARACTER_INPUTNAME 24

//GamePlayer������������
#define MAX_PLAYER 1024

//��ҳ�����
#define MAX_POOL_SIZE 1280
//��������
#define MAX_GUILD_SIZE 1024

//û�ڵ��ʼ���
#define MAX_MAILNODE_SIZE 102400

//�û���ɫ���ֽṹ |CharacterName|@|WorldID|00|
//					��ɫ����      1  3      2����
//ע�⣺��ɫ���ֺ���ĺ�׺����@WorldID�������û�������ɫʱ���Զ���ӽ��ģ�
//		�������˴���ʱ����ʹ���������֣�
//		�ͻ�����ʾ��ʱ�������ǰ��ɫ�Ǳ����紴��������ȥ������ĺ�׺��
//�û���ɫ���Ƶ����ֵ
#define MAX_CHARACTER_NAME 30

//��ҳƺų���
#define MAX_CHARACTER_TITLE 34

//NPC���������Ƴ���
#define NPC_NAME_LEN 32

//NPC�������ƺų���
#define NPC_TITLE_LEN 32

//��ɫ���ǳ�
#define MAX_NICK_NAME 34

//�̵������
#define MAX_SHOP_NAME 34

//̯λ������
#define MAX_STALL_NAME 34

//���Ե���Ŀ
#define MAX_BBS_MESSAGE_NUM 20

//���Եĳ���
#define MAX_BBS_MESSAGE_LENGTH 70

//��������������
#define MAX_BBS_MESSAGE_AUTHORLENGTH 40

//����������̳���
#define MIN_PWD 4

//�����������󳤶� + 1
#define MAX_PWD 17

//�û��ʺŵ����ֵ
#define MAX_ACCOUNT 16

//�û���������ֵ
#define MAX_PASSWORD 32

//ְҵ��Ϣ���ֵ
//#define MAX_JOBINFO_LENGTH	50
//��ҵԺУ��Ϣ���ֵ
//#define MAX_SCHOOLINFO_LENGTH	50
//ʡ����Ϣ���ֵ
//#define MAX_PROVINCE_LENGTH		16
//City��Ϣ���ֵ
//#define MAX_CITY_LENGTH		16
//Email��Ϣ���ֵ
//#define MAX_EMAIL_LENGTH	50
//����������ֵ
//#define MAX_LUCKWORD_LENGTH	255

//����ܴ�����ɫ������
#define MAX_CREATE_PLAYER_NUM 3

//CharacterManager��ȱʡ��ObjID�����С
#define DEFAULT_OBJ_COUNT 1024

//
#define ZONE_SIZE	10

//
#define MAX_SEND_TIME 1

//����Zone���ӵķ�Χ,0=1��(���������Լ��ڵ��Ǹ�����)��1=9��;2=25��;3=49��;
#define MAX_RADIUS 2

#define MAX_REFESH_OBJ_ZONE_RADIUS		2

#define MAX_REFESH_OBJ_ZONE_ARRAY_SIZE	((MAX_REFESH_OBJ_ZONE_RADIUS - 1) * (MAX_REFESH_OBJ_ZONE_RADIUS - 1) + 1 )

//�����ʰȡ��������
#define MAX_BOX_OP_DISTANCE				3.0f
//�������������
#define MAX_BAG_SIZE 60

#define MAX_EXTBAG_SIZE 140

#define MAX_NPC_DISTANCE	5.0

//Container ƫ��

//������ƫ��
#define BASE_CONTAINER_OFFSET	0

//���ϰ�ƫ��
//#define MAT_CONTAINER_OFFSET	20

//�����ƫ��
//#define TASK_CONTAINER_OFFSET	40

//װ��ƫ����
#define EQUIP_CONTAINER_OFFSET	200

//����ƫ����
#define BANK_CONTAINER_OFFSET	220


#define DB_BAG_POS_START			(BASE_CONTAINER_OFFSET)					// 0
#define	DB_BAG_POS_END				(MAX_BAG_SIZE+MAX_EXTBAG_SIZE-1)		// 199

#define DB_EQUIP_POS_START			(MAX_BAG_SIZE+MAX_EXTBAG_SIZE)			// 200
#define DB_EQUIP_POS_END			(DB_EQUIP_POS_START+HEQUIP_NUMBER-1)	// 208

#define DB_BANK_POS_START			(DB_EQUIP_POS_START+HEQUIP_TOTAL)		// 220
#define DB_BANK_POS_END				(MAX_BANK_SIZE+DB_BANK_POS_START-1)		// 319


#define MAX_BANK_MONEY		10000000

//��������ʾ
#define	INVALID_BAG_INDEX (-1)

//һ�������������ж��ٸ�����̵�
#define MAX_SHOP_NUM_PER_SCENE 512

//һ�����������ӵ�е�����̵����
#define MAX_SHOP_NUM_PER_PLAYER 2

//�̵��ղؼд�С
#define MAX_FAVORITE_SHOPS		10

//һ������̵�Ĺ�̨��
#define MAX_STALL_NUM_PER_SHOP 10

//һ������̵�ĺ��������
#define MAX_PARTNER_PER_SHOP 6

//���׼�¼�������
#define MAX_EXCHANGE_RECORD 40

//�����¼�������
#define MAX_MANAGER_RECORD 128

//����¼����
#define MAX_RECORD_LEN_ENTRY	128		

//����¼��
#define MAX_RECORD_NUM_ENTRY	128		

//ÿҳ��¼��
#define MAX_RECORD_NUM_PER_PAGE	10	

//һ������̵�����ֳ���
#define MAX_SHOP_NAME_SIZE 12

//һ������̵���������ݳ���
#define MAX_SHOP_DESC_SIZE 82

//�̵��˰��
#define	SHOP_TAX 0.03

//���е��������
#define MAX_BANK_SIZE 100

//���������Դ��Ǯ��
#define MAX_BANK_MONEY 10000000

//���������Դ��Ԫ��
#define MAX_BANK_RMB 100000

//��һ����������ʼ����
#define RENTBOX1_START_INDEX 0

//�ڶ�����������ʼ����
#define RENTBOX2_START_INDEX 20

//��������������ʼ����
#define RENTBOX3_START_INDEX 40

//���ĸ���������ʼ����
#define RENTBOX4_START_INDEX 60

//�������������ʼ����
#define RENTBOX5_START_INDEX 80

//���׺е��������
#define	MISSION_BOX_SIZE	4

//�����б������Ʒ���������
#define	QUESTLOG_BOX_SIZE	255

//���׺е��������
#define	EXCHANGE_BOX_SIZE	5

//���׺��г�����������
#define	EXCHANGE_PET_BOX_SIZE	5

//����˫����������������
#define EXCHANGE_MAX_DISTANCE	3

//̯λ�е��������
#define	STALL_BOX_SIZE		20

//̯λ�е��������(һ��Ҫ��STALL_BOX_SIZEһ���󣡣���)
#define	STALL_PET_BOX_SIZE		20

//����������߶��еĳ���
#define	MAX_EXCHANGE_APPLICANTS_NUM	10

//�ƺŵ��������
#define MAX_STRING_TITLE_SIZE 4

//��̬�ƺŵ��������
#define MAX_DYNAMIC_TITLE_SIZE 10

//��̬̬�ƺŵ��������
#define MAX_STATIC_TITLE_SIZE 6

//��̬̬�ƺŵ��������
#define MAX_TITLE_ID_SIZE 16

//�ƺŵ��������
#define MAX_TITLE_SIZE 20

//��Ʒ����Ƕ��ʯ��������
#define MAX_ITEM_GEM 3

//�����ɫװ��������Ը���
#define		MAX_BLUE_ITEM_EXT_ATTR				4

//�����Ʒ����
#define MAX_ITEM_ATTR (9+MAX_ITEM_GEM)

//�����ڴ�ItemSerial�̶�key
#define ITEM_SERIAL_KEY		536081

//��ʯ����߼���
#define MAX_GEM_QUALITY 9

//��������ʹ�ñ�����Ʒ��
#define MAX_ABILITY_ITEM 5

//���װ����װ����
#define MAX_ITEM_SET_ATTR 4

//���ҩƷЧ��
#define	MAX_MEDIC_EFFECT_NUM 3

#define MAX_GROW_POINT_RATE_NUM		4
#define MAX_LOGIC_FUNC_LEN			128
#define ITEM_LOGIC_FUNC_LEN			32
#define MAX_SCENE_GROW_POINT_NUM	128

#define MAX_SCENE_GP_OWNER			255

// �������ӵ�еļ�����Ŀ
#define MAX_PET_SKILL_COUNT		(6)

//Obj_ItemBox ��������Ʒ����
#define MAX_BOXITEM_NUMBER 10

//Obj_ItemBox ������ʰȡ����
#define MAX_ITEMBOX_PICK_DIST		(20.0f)

//���˿����������������
#define MAX_BOOTH_NUMBER	   96

//���˿��Իع������������
#define MAX_BOOTH_SOLD_NUMBER   5

//
#define MAX_PLAYER_EXP_LEVEL	100

//
#define MAX_100_PLUS_DELTA_LEVEL	201

//
#define DEFAULT_WASHPOINT_LEVEL		60

//�����弶�����Ե����
#define MAX_TABLE_DEFINE_LEVEL	 100

//�Ͽ�����󣬷����������������ݵ�ʱ�䣨���룩
#define TIME_TO_TRUE_QUIT 10000

//����������������ֵ
#define	MAXTYPE_NUMBER 1024 * 20

//����AI������������ֵ
#define MAXAI_NUMBER 256

//NPC̸�����������ֵ
#define MAXCHATTYPE_NUMBER 64

//NPC̸����¼�����ֵ
#define MAXCHAT_NUMBER 64

//��ɫ��ӵ�м��ܵ�����
#define MAX_CHAR_SKILL_NUM 256

//������󼶱�
#define MAX_CHAR_SKILL_LEVEL 12

//��ɫ���Ͽ���ӵ�еĸ���Ч���������
#define MAX_IMPACT_NUM 20

//��ɫ��ӵ�м����ķ������������ﱻ�����������ܵȼ���
#define MAX_CHAR_XINFA_NUM 128

//��ɫ��ӵ������ܵ�����
#define MAX_CHAR_ABILITY_NUM 64

//�����䷽�����Ŀ��
#define MAX_ABILITY_PRESCRIPTION_NUM (4 * 256)

//�����䷽��Ĵ浵�ֽ�ռ����
#define MAX_CHAR_PRESCRIPTION_BYTE (MAX_ABILITY_PRESCRIPTION_NUM>>3)

#define ABILITY_GAMBLE_NUM 5


//��������
#define MAX_MENPAI_NUM 9

//ÿ��������ӵ�еļ����ķ�������
#define MAX_MENPAI_XINFA_NUM 16

//���ķ�����
#define MAX_ALL_XINFA_NUM	55

//ÿ���ķ����ܹ��ﵽ�����ĵȼ�
#define MAX_XINFA_LEVEL_NUM	120

//ÿ��������ܹ��ﵽ�����ȼ�
#define MAX_ABILITY_LEVEL_NUM 60

//�����������
#define MAX_SKILL_NUMBER 1024

//�������ȼ�
#define MAX_SKILL_LEVEL 12

// ����������
#define MAX_MISSION_NUM				(4096)
#define MAX_CHAR_MISSION_FLAG_LEN	((MAX_MISSION_NUM+31)/32)

//��ɫ��ӵ�����������
#define MAX_CHAR_MISSION_NUM		(20)
enum
{
	QUESTTIME_LIST_SIZE = MAX_CHAR_MISSION_NUM,
};

//ÿ������Notify����
#define MAX_MISSION_NOTIFY_NUM      (4)

//��ɫ��ӵ����������
#define MAX_CHAR_MISSION_DATA_NUM	(256)

//��������
enum
{
	MISSION_TYPE_ZHAOREN = 1,	//���ˣ�����
	MISSION_TYPE_SONGXIN,		//���������
	MISSION_TYPE_SHAGUAI,		//ɱ��
	MISSION_TYPE_SHOUJI	,		//ɱ�֣�Ȼ��õ�������Ʒ
	//MISSION_TYPE_XILIE		= 0x10,
	//MISSION_TYPE_HUSONG		= 0x20,
	//MISSION_TYPE_CANGBAOTU	= 0x40,
	//MISSION_TYPE_BAOWEI		= 0x80,
	//MISSION_TYPE_ANSHA		= 0x100,
};

// ÿ����ɫ����ܿ���������������Ʒ�ĵ���
#define MAX_CHAR_CAN_PICK_MISSION_ITEM_NUM	(MAX_CHAR_MISSION_NUM*4)

//����OBJ�������
#define MAX_SKILLOBJ_NUMBER			1024

#define MAX_WEBSHOPINFO_NUMBER		3

//���ɴ�ITEM BOX�ľ���
#define MAX_OPEN_ITEMBOX_DIST		(2.f)

//���ɴ�ITEM BOX�ľ���ƽ��
#define MAX_OPEN_ITEMBOX_DIST_SQ (MAX_OPEN_ITEMBOX_DIST * MAX_OPEN_ITEMBOX_DIST)

//���ɴ����ű��¼��ľ���(�ͻ���)
#define MAX_CLIENT_DEFAULT_EVENT_DIST (3.f)

//���ɴ����ű��¼��ľ���(������)
#define MAX_SERVER_DEFAULT_EVENT_DIST	(MAX_CLIENT_DEFAULT_EVENT_DIST * 2.f)

//���ɴ����ű��¼��ľ���ƽ��(�ͻ���)
#define MAX_CLIENT_DEFAULT_EVENT_DIST_SQ (MAX_CLIENT_DEFAULT_EVENT_DIST * MAX_CLIENT_DEFAULT_EVENT_DIST)

//���ɴ����ű��¼��ľ���ƽ��(������)
#define MAX_SERVER_DEFAULT_EVENT_DIST_SQ (MAX_SERVER_DEFAULT_EVENT_DIST * MAX_SERVER_DEFAULT_EVENT_DIST)

//
#define MAX_ATOM_OPT_NUM 8

// ��ɫ���ܵ����·��������
#define MAX_CHAR_PATH_NODE_NUMBER	(16)

//��������ģ����Թ������󳡾�����
#define MAX_SCENE 1024

// һ�� zone ������Լ������� Area ����
#define MAX_AREA_IN_ZONE		10

//һ�������������õĳ������ֵ
#define MAX_COPYSCENE 128

//���Ķ�������
#define MAX_TEAM_MEMBER 6

// �Ŷ�������������
#define MAX_TEAMS_IN_GROUP 8

//��������
#define MAX_TEAMS 3000

//�Խ�����Ƶ�����������
#define MAX_CHATCHANNEL_MEMBER 11

//Ƶ������
#define MAX_CHANNELS 3000

// ��ౣ������������ʼ�
#define MAX_MAIL 20

//�ʼ����ݵ���ߴ�
#define MAX_MAIL_CONTEX 256

//��������˵������󳤶�
#define MAX_MONSTERSPEAK_CONTENT 64

//��С���Է�����Ʒ��Ѫ����
#define MIN_DAMAGE_PERCENT	(0.2f)

//������ֳ���
#define MAX_GUILD_NAME_SIZE 24

//������ֳ���
#define MAX_GUILD_NAME_SIZE_DB 32

//���ְλ���ֳ���
#define MAX_GUILD_POS_NAME_SIZE 24

//���ְλ����
#define MAX_GUILD_POS_NUM 8

//�����������
#define MAX_GUILD_DESC_SIZE 60

//�����������
#define MAX_GUILD_DESC_SIZE_DB 62

//����б�ÿ�εõ���������
#define MAX_GUILD_LIST_COUNT 50

#define MAX_PROPOSER_IN_GUILD		10
#define MAX_USER_IN_GUILD			160
#define USER_ARRAY_SIZE				MAX_USER_IN_GUILD + MAX_PROPOSER_IN_GUILD

#define MAX_SYSTEM_MSG_SIZE 256
// �Ѻö�����
#define MAX_FRIEND_POINT 9999

// �Ѻöȴﵽ��������ʾ
#define FRIEND_POINT_ENOUGH_NOTIFY 10

// �Ѻöȴ���Ҫ�����������
#define FRIEND_POINT_NEED_REMOVE_VERIFY 500

//��ϵ�����ޣ��������ѡ���������
#define MAX_RELATION_SIZE 100

//��������������
#define MAX_BLACK_SIZE 64

// ������鵵���ĳ���
#define MOOD_DATA_SIZE 32

// �����������֧�ֳ���
#define MAX_MOOD_SIZE 25

// �����ͽ����
#define MAX_PRENTICE_COUNT 2

// ���ʦ�µ���
#define MAX_MORAL_POINT 1000000

//���ݿ����Ƴ���
#define		DATABASE_STR_LEN			128
//���ݿ��û�������
#define		DB_USE_STR_LEN				32
//���ݿ����볤��
#define		DB_PASSWORD_STR_LEN			32
//DB ����ʱ����
#define		DB_OPERATION_TIME			500
//�˺ű����ɫ����
#define		DB_CHAR_NUMBER				5
//��ɫ�зǼ�ʱˢ������ͬ����ShareMemory��ʱ����
#define		DB_2_SM_TIME				60000 //60��

//���ݿ����ӳ�Ĭ�����Ӹ���
#define		DB_CONNECTION_COUNT			40

// �������ܽ��ܵĿ����˵���λ���������λ�Ĳ��
#define		DEF_SERVER_ADJUST_POS_WARP_DIST	(5.f)

// ���Ϊһ����Ʒ�����������Ҫ���ĵľ���
//#define		LOCK_A_OBJ_NEED_ENERGY		10

// 
//һ���м����������Ӫ
#define MAX_CAMP_NUM		2   
#define INVALID_CAMP		-1		//��Ч��Ӫ
//#define CAMP_1				0		//��һ�������Ӫ
//#define CAMP_2				1		//�ڶ��������Ӫ
//#define CAMP_3				10		//�������͵���Ӫ����10�ſ�ʼ
//#define CAMP_4				20		//�ж����͵���Ӫ����20�ſ�ʼ

//��Ӫ--��������
#define CAMP1_PLAYER		0
#define CAMP1_PET			1
#define CAMP1_MONSTER		2
#define CAMP1_NPC			3
#define CAMP2_PLAYER		4
#define CAMP2_PET			5
#define CAMP2_MONSTER		6
#define CAMP2_NPC			7
#define CAMP3_MONSTER		8	//�Ѻ�
#define CAMP3_NPC			9
#define CAMP4_MONSTER		10	//�ж�
#define CAMP4_NPC			11

/////////////////////////////////////////////////////////////////////////////////
//��Ϸ�������ݲ�����
/////////////////////////////////////////////////////////////////////////////////

//��GUID_t��ȡ�������
#define GETWORLD(u) ((u)/10000000)
//��GUID_t��ȡ���û��������к�
#define GETUSER(u) ((u)%10000000)
//�жϵ�ǰ�Ƿ��ǲ�������
#define ISTESTWORLD(w) ((w)>200?1:0)
//�ж�������Ƿ�Ϸ�
#define ISVALIDWORLD(w) (((w)>0)?(((w)<MAX_WORLD)?1:0):(0))


#define ABS(m) ((m)>0?(m):(m)*(-1))

#define LENGTH(x0,z0,x1,z1)  (ABS((x0)-(x1))+ABS((z0)-(z1)))

#define MAX_FILE_PATH  260

#define MAKE_COLOR(r,g,b,a)		(((r&0xFF)<<24)|((g&0xFF)<<16)|((b&0xFF)<<8)|(a&0xFF))
#define COLOR_R(color)			((color>>24)&0x000000FF)
#define COLOR_G(color)			((color>>16)&0x000000FF)
#define COLOR_B(color)			((color>>8)&0x000000FF)
#define COLOR_A(color)			((color)&0x000000FF)

#define GETHIGH(x) (x>>16)
#define GETLOW(x)  ((x<<16)>>16)


#endif
