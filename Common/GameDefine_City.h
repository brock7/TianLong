/********************************************************************************
*	�ļ�����	GameDefine_City.h
*
*	����˵����	���еĶ���
*	�޸ļ�¼��
*********************************************************************************/

#ifndef __GAMEDEFINE_CITY_H__
#define __GAMEDEFINE_CITY_H__


#define	MAX_CITY_PER_WORLD	255
#define MAX_CITY_PORT		12
#define MAX_CITY_BUILDING	2048
#define MAX_CITY_PORT_GROUP	54

#define	MAX_CITY_PORT_NUM		1024

//���е���󼶱�
#define MAX_CITY_LEVEL	5

//�������ֳ���
#define MAX_CITY_NAME_SIZE_DB 26

//�������ֳ���
#define MAX_CITY_NAME_SIZE 24

//�����н�������������
#define MAX_CITY_BUILDING_NUM 32

//�����н��������󼶱�
#define MAX_BUILDING_LEVEL	 5

//�о�ʱ���ܶ�Ӧ�Ĳ�ͬ������
#define MAX_BUILDING_LEVEL_RESEARCH	 5

//�䷽��
#define MAX_RESEARCH_ITEM_PER_BUILDING	 100

//�̵�һ��ˢ�¿���������Ʒ����
#define MAX_ITEM_PER_SHOP	 5

//һ���������ɽ�������ҵ·��(�Ѻó���)
#define MAX_FRIEND_GUILD	 8

enum CITY_ERROR_TYPE
{
	CITY_ERROR_NOTHING				= 0,	// ûʲô���󡭡�
	CITY_ERROR_GUID_ON_WORKING,				//��ǰ�������ڽ��и�Σ���������ܽ��г��в���
};

enum CITY_RATE_TYPE
{
	IND_RATE = 0,			//��ҵ��0
	AGR_RATE,				//ũҵ��1
	COM_RATE,				//��ҵ��2
	DEF_RATE,				//������3
	TECH_RATE,				//�Ƽ���4
	AMBI_RATE,				//������5

	EXTRA_RATE,				//ʣ�����

	MAX_RATE,
};
//!!32λ����
enum ENUM_UPDATE_CITY_ATT
{
	CITY_UPDATE_CITY_LEVEL = 0,
	CITY_UPDATE_CITY_NAME,
	CITY_UPDATE_BUILDING,
	CITY_UPDATE_OPT,
	CITY_UPDATE_PROGRESS,
	CITY_UPDATE_CITY_MAINTAIN_STATUS,
	CITY_UPDATE_CITY_6_RATE,
	CITY_UPDATE_CITY_6_LV,
	CITY_UPDATE_MONEY,
	CITY_OPT_BUY_RESEARCH,
	CITY_OPT_SELL_MERCHANDISE,
	CITY_UPDATE_CITY_FRIEND_GUILD_LIST,

	CITY_UPDATE_MAX,
};

enum ENUM_CITY_OPT
{
	CITY_OPT_NONE	=	0,
	CITY_OPT_LEVEL_UP,
	CITY_OPT_CANCEL_LEVEL_UP,
	CITY_OPT_SUCCESS_LEVEL_UP,
	CITY_OPT_BUILD,
	CITY_OPT_LEVEL_DOWN,
	CITY_OPT_DESTORY,
	CITY_OPT_RESEARCH,
	CITY_OPT_CANCEL_RESEARCH,
	CITY_OPT_SUCCESS_RESEARCH,
	CITY_OPT_BUY_RESEARCH_ITEM,
	CITY_OPT_SELL_TICKET_ITEM,
	CITY_OPT_ADD_FRIEND_GUILD,
	CITY_OPT_REMOVE_FRIEND_GUILD,

	CITY_OPT_MAX,
};

enum BUILDING_TYPE
{
	BUILDING_XIANYA = 0,					//����0
	BUILDING_XIANGFANG,						//�᷿1
	BUILDING_JIUSI,							//����2
	BUILDING_QIANZHUANG,					//Ǯׯ3
	BUILDING_FANGJUFANG,					//����4
	BUILDING_DUANTAI,						//��̨5
	BUILDING_WUJUFANG,						//���6
	BUILDING_MICANG,						//�ײ�7
	BUILDING_CHENGQIANG,					//��ǽ8
	BUILDING_JIFANG,						//����9
	BUILDING_YISHE,							//ҽ��10
	BUILDING_WUFANG,						//���11
	BUILDING_JIANTA,						//����12
	BUILDING_SHUFANG,						//�鷿13
	BUILDING_QIJI,							//�漣14
	BUILDING_XIAOCHANG,						//У��15
	BUILDING_HUOBINGTA,						//�����16
	BUILDING_DAQI,							//����17

	BUILDING_MAX,

};

enum PROGRESS_TYPE
{
	PROGRESS_BUILDING = 0,					//����
	PROGRESS_RESEARCH,						//�о�

	PROGRESS_MAX,

};

enum	MAINTAIN_STATUS
{
	MAINTAIN_NONE =0, 
	MAINTAIN_LOW,							//�׶�ά��״̬
	MAINTAIN_NORMAL,						//��ͨά��״̬
	MAINTAIN_HIGH,							//�߶�ά��״̬
};

//����
enum	COMMERCE_SCOPE
{
	INVALID_COMMERCE = -1,
	MAIN_CITIES,							//3����0
	YAN_MEN,								//����1
	HANG_ZHOU,								//����2
	DIAN_YU,								//����3
	LIAO_DI,								//�ɵ�4
	LING_NAN,								//����5
	SHI_CHENG,								//ʯ��6
	SAI_WAI,								//����7
	HAI_KOU,								//����8
	MIAO_YU,								//����9
	MAX_COMMERCE_SCOPE,
};

enum ValueType
{	
	SALE_PRICE_VALUE = 0,
	BUY_PRICE_VALUE,
};

#define MAX_RPITEM_PER_WORLD				255

#endif // 
