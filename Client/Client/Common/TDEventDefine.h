
#pragma once

enum GAME_EVENT_ID
{
	GE_APPLICATION_INITED,			//��Ϸ�����ʼ��
	GE_ON_SCENE_TRANS,				//����ת��
	GE_SCENE_TRANSED,				//����ת�ƺ�
									// arg0- С��ͼ������
									//
	GE_ON_SCENE_UPDATED,			//��������(һ��������ҳ���)
									// arg0- С��ͼ������
									//
	GE_ON_SERVER_TRANS,				//������ת��
	GE_PLAYER_ENTER_WORLD,			//��������
	GE_PLAYER_LEAVE_WORLD,			//�˳�����
	GE_TEAM_CHANG_WORLD,			// ���Ѹı䳡����
	GE_SKILL_CHANGED,				//��ҵ�ĳ��ܷ����ı�,
									// arg0 - ����id
	GE_PACKAGE_ITEM_CHANGED,		//�����е�ĳ����Ʒ�����ı�
									// arg0 - ��Ʒ�ڰ����еı��

	GE_MAINTARGET_CHANGED,			//��ѡ��������
									// arg0 - ����ServerId

	
	GE_MAINTARGET_OPEND,			// ��target ����ѡ���Ƕ��ѵ������ �򿪴��� 2006-4-12
									

	GE_TOGLE_SKILL_BOOK,			// ��/�رռ�����
	GE_CLOSE_SKILL_BOOK,			// �رռ�����

	GE_TOGLE_PET_PAGE,				// �򿪳������
	GE_TOGLE_LIFE_PAGE,				// ������ܽ���
	GE_TOGLE_COMMONSKILL_PAGE,		// ����ͨ���ܽ���
	GE_SKILL_UPDATE,				//��Ҽ������ݷ����ı�

	GE_TOGLE_CONTAINER,				// ��/�رհ���

	//-----------------------------------------------------
	//��ɫ����
	GE_UNIT_HP,						//ĳ����Ԫ��HP�����仯
									// arg0 - ��Ԫ����
									//		"player"	- ����Լ�
									//		"target"	- ��Ŀ��
									//		"pet"		- �Լ��ĳ���
									//		"party*"	- ����(1-4)
									//		"partypet*"	- ���ѵĳ���(1-4)
									//		...
	GE_UNIT_MP,						//ĳ����Ԫ��MANA�����仯
									// arg0 - ��Ԫ����(ͬ��)

	GE_UNIT_RAGE,					//ĳ����Ԫ��ŭ�������仯
									// arg0 - ��Ԫ����(ͬ��)

	GE_UNIT_XINFA,					//ĳ����Ԫ���ķ��ȼ������仯
									// arg0 - ��Ԫ����(ͬ��)
									// arg1 - �ķ�����

	GE_UNIT_EXP,					//ĳ����Ԫ�ľ���ֵ�����仯
									// arg0 - ��Ԫ����(ͬ��)

	GE_UNIT_MONEY,					//ĳ����Ԫ��Ǯ�����仯
									// arg0 - ��Ԫ����(ͬ��)

	GE_UNIT_RACE_ID,				// ���ݱ��е�ID
	GE_UNIT_NAME,					// ��ɫ�����֣��ӷ���������
	GE_UNIT_CAMP_ID,				// ��ӪID
	GE_UNIT_LEVEL,					// �ȼ�
	GE_UNIT_MOVE_SPEED,				// �ƶ����ٶ�
	GE_UNIT_FIGHT_STATE,			// ս��״̬	
	GE_UNIT_MAX_EXP,				// ���EXP
	GE_UNIT_TITLE,					// �ƺ�
	GE_UNIT_STRIKEPOINT,			// ������
	GE_UNIT_RELATIVE,				// ��������

	//-------------
	//һ��ս������
	GE_UNIT_STR,					// �⹦
	GE_UNIT_SPR,					// �ڹ�
	GE_UNIT_CON,					// ��
	GE_UNIT_INT,					// ����
	GE_UNIT_DEX,					// �ǻ�
	GE_UNIT_POINT_REMAIN,			// ʣ����������

	//-------------
	//����ս������
	GE_UNIT_ATT_PHYSICS,			// ��������			
	GE_UNIT_ATT_MAGIC,				// ħ��������			
	GE_UNIT_DEF_PHYSICS,			// ���������			
	GE_UNIT_DEF_MAGIC,				// ħ��������			
	GE_UNIT_MAX_HP,					// ���������			
	GE_UNIT_MAX_MP,					// ���ħ����			
	GE_UNIT_HP_RE_SPEED,			// HP�ָ��ٶ�  ��/��	
	GE_UNIT_MP_RE_SPEED,			// MP�ָ��ٶ�  ��/��	
	GE_UNIT_HIT,					// ������				
	GE_UNIT_MISS,					// ������				
	GE_UNIT_CRIT_RATE,				// ������				
	GE_UNIT_ATT_SPEED,				// �����ٶ�	
	GE_UNIT_ATT_COLD,				// ������	
	GE_UNIT_DEF_COLD,				// ������	
	GE_UNIT_ATT_FIRE,				// �𹥻�	
	GE_UNIT_DEF_FIRE,				// �����	
	GE_UNIT_ATT_LIGHT,				// �繥��	
	GE_UNIT_DEF_LIGHT,				// �����	
	GE_UNIT_ATT_POSION,				// ������	
	GE_UNIT_DEF_POSION,				// ������	
	GE_UNIT_RMB,					// Ԫ��		//GE_UNIT_VIGOR,					// ����ֵ
	//GE_UNIT_MAX_VIGOR,				// ����ֵ����
	//GE_UNIT_ENERGY,					// ����ֵ
	//GE_UNIT_MAX_ENERGY,				// ����ֵ����

	GE_UNIT_MENPAI,					// ���ɺ�
	GE_UNIT_HAIR_MESH,				// -> DBC_CHARACTER_HAIR_GEO
	GE_UNIT_HAIR_COLOR,				// ͷ����ɫ
	GE_UNIT_FACE_MESH,				// -> DBC_CHARACTER_HEAD_GEO
	GE_UNIT_FACE_IMAGE,				// ͷ����Ϣ�ı� 2006-3-9
	GE_UNIT_EQUIP_VER,				// ��ɫ��װ��״̬�汾�ţ����ںͷ�����ͬ�����ݵ�����
	GE_UNIT_EQUIP,					// װ����(������)
	GE_UNIT_EQUIP_WEAPON,			// ����


	//--------------------------------------------------------------------------------

	GE_SHOW_CONTEXMENU,				//��ʾ�Ҽ��˵�
									// arg0 - �˵���
									//		"player"		- ����Լ�
									//		"other_player"  - �������
									//		"npc"			- npc
									// arg1 - ���λ��_x
									// arg2 - ���λ��_y

	GE_TOGLE_COMPOSE_WINDOW,		// �򿪹رպϳɽ���

	GE_TOGLE_CONSOLE,				// ��ʾ����̨

	GE_ON_SKILL_ACTIVE,				// ĳ�����ܿ�ʼʹ��
									// arg0 - ����ID

	GE_POSITION_MODIFY,				// �����λ�øı���

	GE_CHAT_MESSAGE,				// �õ�������Ϣ
									/*
									|  arg0 - ��������
									|		"near"		- �������
									|		"scene"		- ����
									|		"sys"		- ϵͳ
									|		"team"		- ����
									|		"guild"		- ���
									|		"user"		- �Խ�
									|		"private"	- ˽��
									|
									|  arg1 - ˵���ߵ�����
									|
									|  arg2 - ˵������
									|
									*/

	GE_CHAT_CHANNEL_CHANGED,		//����Ƶ�������ı�
	GE_CHAT_CHANGE_PRIVATENAME,		//�޸������е��������
	GE_CHAT_TAB_CREATE,				//��������ҳ��
	GE_CHAT_TAB_CREATE_FINISH,		//��������ҳ�����
	GE_CHAT_TAB_CONFIG,				//��������ҳ��
	GE_CHAT_TAB_CONFIG_FINISH,		//��������ҳ�����
	GE_CHAT_FACEMOTION_SELECT,		//�������ѡ��
	GE_CHAT_TEXTCOLOR_SELECT,		//����������ɫѡ��
	GE_CHAT_CONTEX_MENU,			//������صĿ�ݲ˵�
	GE_CHAT_ACTSET,					//���춯������
	GE_CHAT_ACT_SELECT,				//���춯�����������ʾ
	GE_CHAT_ACT_HIDE,				//���춯���������رգ���Ϊ�����������Ƚ����⣬��Ҫ֧����ק��
	GE_CHAT_PINGBI_LIST,			//�������ν���
	GE_CHAT_PINGBI_UPDATE,			//�����б����
	GE_CHAT_ADJUST_MOVE_CTL,		//֪ͨ���촰�ڵ�����С
	GE_CHAT_INPUTLANGUAGE_CHANGE,	//���뷨���
	GE_CHAT_SHOWUSERINFO,			//����鿴�����Ϣ����
	GE_CHAT_LOAD_TAB_CONFIG,		//����ҳ��������Ϣ

	GE_ACCELERATE_KEYSEND,			//���̿�ݼ�����
	
	GE_LOOT_OPENED,					// ���Ӵ�
	GE_LOOT_SLOT_CLEARED,			// �����е�ĳ��λ�����
									// arg0 - �����е�λ��

	GE_LOOT_CLOSED,					// ���ӹر�

	GE_PROGRESSBAR_SHOW,			// ��������ʾ
	GE_PROGRESSBAR_HIDE,			// ����������
	GE_PROGRESSBAR_WIDTH,			// ��������ȵ���
									// arg0 - �������İٷֱ� 1 < x < 100

	GE_CHANGE_BAR,					//�ı乤�����ϵ�Action
									// arg0 - Bar����
									// arg1 - Index
									// arg2 - ActionItem ID

	GE_TOGLE_MISSION,				//�����������
	GE_UPDATE_MISSION,				//ˢ�������б�
	GE_REPLY_MISSION,				//�ύ�������
	GE_UPDATE_REPLY_MISSION,		//ˢ���ύ�������

	GE_TOGLE_LARGEMAP,				// �رմ򿪴��ͼ
	GE_TOGLE_SCENEMAP,				// �رմ򿪴��ͼ
	GE_OPEN_MINIMAP,				// ��С��ͼ
	GE_OPEN_MINIMAPEXP,				// ����չС��ͼ

	GE_QUEST_EVENTLIST,				//�򿪺�npc��һ�ζԻ�ʱ�Ŀ�ִ�нű��б�
	GE_QUEST_INFO,					//�򿪺�npc�ڶ��ζԻ�(�ڽ�����ʱ��������������Ϣ���ȴ�����)
	GE_QUEST_REGIE,					//��������
	GE_QUEST_CONTINUE_DONE,			//����������ٴκ�npc�Ի������õ�������������Ϣ��(�������)
	GE_QUEST_CONTINUE_NOTDONE,		//����������ٴκ�npc�Ի������õ�������������Ϣ��(����δ���)
	GE_QUEST_AFTER_CONTINUE,		//���������֮�󡱣���Ʒѡ�����
	GE_QUEST_TIPS,					//��Ϸ�����е������չ��ʾ��Ϣ

	GE_TOGLE_COMPOSEITEM,			// ��/�ر� �ϳɽ���
									// arg0 - ����ܵ�����
	GE_TOGLE_COMPOSEGEM,			// ��/�ر� ��ʯ�ϳɽ���
									// 
	GE_ON_LIFEABILITY_ACTIVE,		// ĳ������ܿ�ʼʹ��
									// arg0 - ����ID
									// arg1 - �䷽
									// arg2 - ƽ̨
	GE_NEW_DEBUGMESSAGE,			//�µ�debug string����������Ļ����listbox��ʾ��


	//-------------------------------------------------------------------------------------------------------
	// �������Խ���
	GE_UPDATE_EQUIP,				// ����װ��
	GE_OPEN_BOOTH,					// �򿪻�����
	GE_CLOSE_BOOTH,					// �رջ�����
	GE_MANUAL_ATTR_SUCCESS_EQUIP,	// �ֶ��������Գɹ�.
	GE_CUR_TITLE_CHANGEED,			// ��ǰ����ĳƺŸı���.

	GE_UPDATE_BOOTH,				// ���ܸ���

	GE_OPEN_CHARACTOR,				// ������������
	GE_OPEN_EQUIP,					// ��װ����
	GE_TOGLE_JOINSCHOOL,			// �򿪼������ɵĶԻ���

	GE_UPDATE_CONTAINER,			// ����״̬����
	GE_IMPACT_SELF_UPDATE,			// �Լ�����Ч���� 
	GE_IMPACT_SELF_UPDATE_TIME,		// �Լ�����Чʱ��ĸ��� 
	GE_TOGLE_SKILLSTUDY,			// �����ķ���ѧϰ������
	GE_SKILLSTUDY_SUCCEED,			// �����ķ�ѧϰ�ɹ�

	GE_TOGLE_ABILITY_STUDY,			// �����ѧϰ����
	GE_OPEN_AGNAME,					// ��ѡ��ƺŽ���
	GE_CLOSE_AGNAME,				// �ر�ѡ��ƺŽ���
	
	GE_TOGLE_BANK,					// �����н���
	GE_UPDATE_BANK,					// �������н���
	GE_TOGLE_INPUT_MONEY,			// ������Ǯ�Ľ���
	GE_CLOSE_INPUT_MONEY,			// �ر������Ǯ����

	GE_RECEIVE_EXCHANGE_APPLY,		// �յ���������
	GE_STOP_EXCHANGE_TIP,			// ֹͣ����������ʾ
	GE_OPEN_EXCHANGE_FRAME,			// �򿪽��׶Ի���
	GE_UPDATE_EXCHANGE,				// ���½��׽���
	GE_CANCEL_EXCHANGE,				// ȡ������
	GE_SUCCEED_EXCHANGE_CLOSE,		// ���׳ɹ���֪ͨ�رս��׽��棨���ܻ���ر���ؽ��棩
	GE_UPDATE_PET_PAGE,				// ˢ�³������
	GE_UPDATE_LIFESKILL_PAGE,		// ˢ������ܽ���
	GE_OPEN_COMPOSE_ITEM,			// ��������Ʒ����
	GE_UPDATE_COMPOSE_ITEM,			// ˢ��������Ʒ����
	GE_OPEN_COMPOSE_GEM,			// �򿪱�ʯ�ϳ�/��Ƕ����
	GE_UPDATE_COMPOSE_GEM,			// ˢ�±�ʯ�ϳ�/��Ƕ����
	GE_AFFIRM_SHOW,					// �򿪷�������ȷ�Ͻ���


	GE_OPEN_STALL_SALE,				// ��̯(��)
	GE_OPEN_STALL_BUY,				// ��̯(��)
	GE_UPDATE_STALL_BUY,			// ����(��)
	GE_UPDATE_STALL_SALE,			// ����(��)
	GE_OPEN_STALL_RENT_FRAME,		// ��̯(���˰����ʾ)
	GE_STALL_SALE_SELECT,			// ��̯(��)ʱ��ѡ��̯λ�ϵ���Ʒ
	GE_STALL_BUY_SELECT,			// ��̯(��)ʱ��ѡ��̯λ�ϵ���Ʒ
	GE_OPEN_STALL_MESSAGE,			// ��̯(��Ϣ��������)
	GE_CLOSE_STALL_MESSAGE,			// ��̯(��ϵ��Ϣ����)

	GE_OPEN_DISCARD_ITEM_FRAME,		// ������Ʒ
	GE_OPEN_CANNT_DISCARD_ITEM,		// ����������Ʒ

	GE_SHOW_SPLIT_ITEM,				// �򿪲����ƽ�Ի���
	GE_HIDE_SPLIT_ITEM,				// �رղ����ƽ�Ի���

	GE_TOGLE_FRIEND_INFO,			// �򿪹رպ����б�Ի���
	GE_TOGLE_FRIEND,				// �򿪹رպ��ѶԻ���
	GE_UPDATE_FRIEND,				// ���º���������
	GE_UPDATE_FRIEND_INFO,			// �������ݽ����
	GE_OPEN_EMAIL,					// ���ż�����
	GE_OPEN_EMAIL_WRITE,			// ���ż�����
	GE_HAVE_MAIL,					// ���ʼ�
	GE_SEND_MAIL,					// �����ʼ�
	GE_UPDATE_EMAIL,				// ˢ�µ�ǰ�ʼ�
	GE_MOOD_SET,					// ��������
	GE_MOOD_CHANGE,					// ����ı���
	GE_OPEN_HISTROY,				// �����ʷ��Ϣ

	//ϵͳ�������
	GE_TOGLE_SYSTEMFRAME,			// ��ϵͳ������
	GE_TOGLE_VIEWSETUP,				// ����Ƶ���ý���
	GE_TOGLE_SOUNDSETUP,			// ���������ý���
	GE_TOGLE_UISETUP,				// �򿪽�������
	GE_TOGLE_INPUTSETUP,			// �򿪰�������
	GE_TOGLE_GAMESETUP,				// ��Ϸ������

	//����̵�
	GE_PS_OPEN_OTHER_SHOP,			// �򿪱��˵��̵�
	GE_PS_OPEN_MY_SHOP,				// ���Լ����̵�
	GE_PS_OPEN_CREATESHOP,			// �򿪴����̵����
	GE_PS_CLOSE_CREATESHOP,			// �رմ����̵����
	GE_PS_OPEN_SHOPLIST,			// ���̵��б����
	GE_PS_SELF_ITEM_CHANGED,		// �Լ��̵�����Ʒ�����ı�
	GE_PS_OTHER_SELECT,				// ѡ��һ����Ʒ
	GE_PS_SELF_SELECT,				// ѡ��һ����Ʒ

	GE_PS_UPDATE_MY_SHOP,			// �����Լ��̵������
	GE_PS_UPDATE_OTHER_SHOP,		// �����̵������

	GE_PS_OPEN_OTHER_TRANS,			// ���Լ����̳�״̬�е��̵�
	GE_PS_UPDATE_OTHER_TRANS,		// ����
	GE_PS_OTHER_TRANS_SELECT,		// ѡ��

	GE_OPEN_PS_MESSAGE_FRAME,		// ����̵��������Ҫ������ȷ�϶Ի���

	GE_PS_OPEN_MESSAGE,				// ������̵����Ϣ��
	GE_PS_UPDATE_MESSAGE,			// ��������̵����Ϣ��

	GE_OPEN_PET_LIST,				// ���׵ȹ���ʹ�õĳ����б�
	GE_VIEW_EXCHANGE_PET,			// ���׵ȹ�������ʾ����
	GE_CLOSE_PET_LIST,				// ���׵ȹ��̽�����ʱ��رճ����б����
	GE_UPDATE_PET_LIST,				// ���³����б����

	GE_OPEN_PRIVATE_INFO,			// �������Ϣ�Ի���

	GE_OPEN_CALLOF_PLAYER,			// ������Ҫ����ĶԻ���
	GE_NET_CLOSE,					// �Ͽ�����

	GE_OPEN_ITEM_COFFER,			// ��������Ʒ����

	GE_PS_INPUT_MONEY,				// ��������̵������Ǯ�Ŀ�

	//--------------------------------------------------------------------------------------------------------------------
	// �������¼�

	GE_TEAM_OPEN_TEAMINFO_DLG,			// �򿪶�����Ϣ�Ի���.
										// 
										// arg0 - ������, ��0��ʼ
										// 0 : ����������������
										// 1 : ��������������
										// 2 : �򿪶�����Ϣ
										// -1: �رնԻ���


	GE_TEAM_NOTIFY_APPLY,				// ֪ͨ�ӳ�, ��������������.
										// 
										// arg0 - ������, ��0��ʼ
										// 0 : ����������������
										// 1 : ��������������
										// ������˵ľ�����Ϣ���������Ϣ�����л��.


	GE_TEAM_APPLY,						// ����������������.
										// 
										// arg0 - �ַ���
										// �������.
	

	GE_TEAM_INVITE,						// ����������������.
										// 
										// arg0 - �ַ���
										// ���������.


	GE_TEAM_CLEAR_UI,					// ��ս���
										//
										// �޲���


	GE_TEAM_REFRESH_UI,					// ˢ�½���
										//
										// �޲���


	GE_TEAM_MEMBER_ENTER,				// ���µĶ�Ա����
										//
										// arg0 - ������, ��1 ��ʼ
										// ��ui�����е���ʾλ��


	GE_TEAM_UPTEDATA_MEMBER_INFO,		// ���¶�Ա��Ϣ
										// arg0 - ������, ��1 ��ʼ
										// ��ui�����е���ʾλ��


	GE_TEAM_MESSAGE,					// ������Ϣ��ʾ��Ϣ
										// arg0 - �ַ���
										// ��Ҫ��ʾ����Ϣ.

										// ע��, ������Ϣ��������ʾ���������Ϣ
										// ��: xxx�뿪����
										//     xxx�Ѿ���һ��������.


	GE_TEAM_MEMBER_INVITE,				// ��Ա����ĳһ���˼����������ӳ�ͬ��
										//
										// arg0 - ��Ա����
										// arg1 - �������˵�����

	GE_TEAM_FOLLOW_INVITE,				// �ӳ������Ա������Ӹ���ģʽ
										//
										// arg0 - �ӳ�����

	GE_TEAM_REFRESH_MEMBER,				// ˢ�¶�Ա��Ϣ
										//
										// arg0 - ��Ա��λ������

	/**********************************************************************************************************
	**
	** �����Ժ���Ҫɾ��
	**
	**
	**
	***********************************************************************************************************/

	GE_ON_TEAM_ENTER_MEMBER,		// ���¶�Ա���

	GE_SHOW_TEAM_YES_NO,			// ��ͬ����Ӵ���.

	GE_SHOW_TEAM_MEMBER_INFO,		// ��ʾ��Ա����ϸ��Ϣ.

	GE_SHOW_TEAM_MEMBER_NAME,		// ����ߵĶ����б������ʾ���ѵ�����

	GE_HIDE_ALL_PLAYER,				// �Լ��뿪�����, �������еĶ��ѽ���

	// ��Ա����ϸ��Ϣ
	GE_SHOW_TEAM_MEMBER_NICK,		// ����
		
	GE_SHOW_TEAM_MEMBER_FAMILY,		// ����
	GE_SHOW_TEAM_MEMBER_LEVEL,		// �ȼ�
	GE_SHOW_TEAM_MEMBER_POS,		// λ��
	GE_SHOW_TEAM_MEMBER_HP,			// hp
	GE_SHOW_TEAM_MEMBER_MP,			// mp
	GE_SHOW_TEAM_MEMBER_ANGER,		// ŭ��
	GE_SHOW_TEAM_MEMBER_DEAD_LINK,  // ������Ϣ
	GE_SHOW_TEAM_MEMBER_DEAD,		// ������Ϣ.

	GE_UPDATE_TEAM_MEMBER,			// ���¶�Ա��Ϣ
									// arg0 - ��Ա��ţ������У��Լ���ռ����ţ�
									// arg1 - ��Ա guid������ȡ�øö�Ա��Ϣ��

	GE_SHOW_TEAM_FUNC_MENU_MEMBER,	// ��ʾ��Ա�Ĺ��ܲ˵�
	GE_SHOW_TEAM_FUNC_MENU_LEADER,	// ��ʾ�ӳ��Ĺ��ܲ˵�
	
	//
	// �������¼�
	//------------------------------------------------------------------------------------------------------------------------

	
	//------------------------------------------------------------------------------------------------------------------------
	//
	// ��¼�������
	//

	GE_GAMELOGIN_SHOW_SYSTEM_INFO_CLOSE_NET,	// ��ʾϵͳ��Ϣ
												//
												// arg0 - �ַ��� : ��Ҫ��ʾ��ϵͳ��Ϣ.
												//
												// ���ȷ�ϰ�ť��Ͽ����硣
	
	GE_GAMELOGIN_SHOW_SYSTEM_INFO,				// ��ʾϵͳ��Ϣ
												//
												// arg0 - �ַ��� : ��Ҫ��ʾ��ϵͳ��Ϣ.


	GE_GAMELOGIN_CLOSE_SYSTEM_INFO,				// ��ʾ�ӳ��Ĺ��ܲ˵�

	
	GE_GAMELOGIN_OPEN_SELECT_SERVER,			// �ر�ѡ�����������
																					

	GE_GAMELOGIN_CLOSE_SELECT_SERVER,			// �ر�ѡ�����������


	GE_GAMELOGIN_OPEN_COUNT_INPUT,				// ���ʺ��������
																					

	GE_GAMELOGIN_CLOSE_COUNT_INPUT,				// �ر��ʺ��������

	GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON,	// ��ʾϵͳ��Ϣ, ����ʾ��ť
												//
												// arg0 - �ַ��� : ��Ҫ��ʾ��ϵͳ��Ϣ.

	GE_GAMELOGIN_OPEN_SELECT_CHARACTOR,			// ��ʾ����ѡ�����

	GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR,		// �ر�����ѡ�����

	GE_GAMELOGIN_OPEN_CREATE_CHARACTOR,			// ��ʾ���ﴴ������

	GE_GAMELOGIN_CLOSE_CREATE_CHARACTOR,		// �ر����ﴴ������

	GE_GAMELOGIN_REFRESH_ROLE_SELECT_CHARACTOR,	// ˢ��������Ϣ

	GE_GAMELOGIN_CLOSE_BACK_GROUND,				// �رձ�������

	GE_GAMELOGIN_SYSTEM_INFO_YESNO,				// ϵͳ��Ϣ��ʾ yes_no ����.
												//
												// ����0, ��ʾ���ַ���
												// ����1,	�Ի��������
												//			0 -- �Ƿ��˳���Ϸ
												//			1 -- �Ƿ�ɾ����ɫ
												//			2 -- �Ƿ�����ʺ�
												//			4 -- �Ƿ����Զ����

	GE_GAMELOGIN_SELECT_LOGIN_SERVER,			// ѡ��һ��login server
												//
												// ����0, iAreaIndex   ��������
												// ����1, iLoginServer ��������

	GE_GAMELOGIN_CLEAR_ACCOUNT,					// ����ʺ���ʾ.
												//

	GE_GAMELOGIN_SELECT_AREA,					// ����ʺ���ʾ.
												//
	GE_GAMELOGIN_SELECT_LOGINSERVER,			// ����ʺ���ʾ.
												//

	GE_GAMELOGIN_CREATE_CLEAR_NAME,				// ��մ�����ɫ�����֡�
												//

	GE_GAMELOGIN_CREATE_ROLE_OK,				// ������ɫ�ɹ���
												//
												
	

	//
	// ��¼�������
	//
	//------------------------------------------------------------------------------------------------------------------------


	//------------------------------------------------------------------------------------------------------------------------
	//
	// �Ƿ����ö�����������
	//

	GE_MINORPASSWORD_OPEN_SET_PASSWORD_DLG,			// �����ö����������
													//
													

	GE_MINORPASSWORD_OPEN_UNLOCK_PASSWORD_DLG,		// ��unlock������档

	GE_MINORPASSWORD_OPEN_CHANGE_PASSWORD_DLG,		// �����������

	GE_OPEN_SYSTEM_TIP_INFO_DLG,					// ��ʾϵͳ��Ϣ�Ի���

	
	//
	// �Ƿ����ö�����������
	//
	//------------------------------------------------------------------------------------------------------------------------
	
	GE_SUPERTOOLTIP,				//Tooltips
									/*
									| arg0 - ��ʾ/���� 1,0
									| arg1 - ���� "skill", "lifeability", "item", "xinfa", "pet_skill"
									| arg2, arg3 - ���λ��
									|
									*/
											 
	// ����������
	GE_RELIVE_SHOW,					// ��ʾ�������
									// arg0 - �Ƿ��и���İ�ť
									// arg1 - ʣ��ʱ��

	GE_RELIVE_HIDE,					// ���ظ������

	GE_RELIVE_REFESH_TIME,			// ˢ�������ϵ�ʱ��
									// ��ʾʱ��

	GE_OBJECT_CARED_EVENT,			//ĳ�߼������ĳЩ�����ı�
									/*
									|  arg0 - Object��ID
									|  arg1 - ����
									|			distance: ���뷢���ı� (arg2����)
									|			destroy:  ������
									|
									*/
	GE_UPDATE_MAP,					// ���µ�ͼ

	GE_UPDATE_SUPERTOOLTIP,
	GE_UI_COMMAND,
	GE_OTHERPLAYER_UPDATE_EQUIP,

	GE_VARIABLE_CHANGED,			//ĳ��ȫ�ֱ��������ı䣬 
									/*
									| arg0 - ������
									| arg1 - �µı���ֵ
									*/

	GE_TIME_UPDATE,					//��ʱ��ϵͳ��ʱ�������¼�			
	GE_FRIEND_INFO,					// �򿪺�����ϸ��Ϣ

	GE_UPDATE_TARGETPET_PAGE,		//��targetpet ui
	GE_UPDATE_PETSKILLSTUDY,		//���³��＼��ѧϰ����
	
	GE_UPDATE_PETINVITEFRIEND,		//���³������Ѱ����
	GE_REPLY_MISSION_PET,			//����ˢ��

	GE_GUILD_CREATE,				//��ᴴ���¼�
	GE_GUILD_CREATE_CONFIRM,		//��ᴴ��ȷ���¼�
	GE_GUILD_SHOW_LIST,				//��ʾ����б��¼�
	GE_GUILD_SHOW_MEMBERINFO,		//��ʾ�Լ�����Ա�������
	GE_GUILD_UPDATE_MEMBERINFO,		//���°���Ա����Ϣ
	GE_GUILD_SHOW_DETAILINFO,		//��ʾ�Լ������ϸ��Ϣ����
	GE_GUILD_SHOW_APPOINTPOS,		//��ʾ�Լ�������ְλ����
	GE_GUILD_DESTORY_CONFIRM,		//ɾ�����ȷ���¼�
	GE_GUILD_QUIT_CONFIRM,			//�˳����ȷ���¼�
	GE_GUILD_FORCE_CLOSE,			//ǿ�ư����ؽ���ر��¼�

	GE_CLOSE_MISSION_REPLY,			//�ر������ύUI
	GE_CLOSE_TARGET_EQUIP,			//�رղ鿴�Է���ɫ����UI
	GE_VIEW_RESOLUTION_CHANGED,		//��Ļ�ֱ��ʱ仯ʱ��֪ͨ��Ϣ
	GE_CLOSE_SYNTHESIZE_ENCHASE,	//�رպϳ�UI����ǶUI
	GE_TOGGLE_PETLIST,				//�򿪺͹رճ����б�
	GE_PET_FREE_CONFIRM,			//��������ȷ��

	GE_TOGLE_CONTAINER1,				// ��/�رհ���  //fujia 2007.10.24
	GE_TOGLE_CONTAINER2,				// ��/�رհ���
	GE_CHAT_OPEN,                       //�����õ�����  fujia 2007.12.29
	GE_CHAT_CLOSED, 
	GE_NEW_GONGGAOMESSAGE,			//ϵͳ������Ϣ
};
