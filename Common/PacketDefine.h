#ifndef __PACKET_DEFINE_H__
#define __PACKET_DEFINE_H__


namespace Packets
{
	//��Ϣ����ֵ������ʽ��PACKET_XX_YYYYYY
	//XX��������Ϊ��GC��CG��CL��LC��GL��LG
	//G��Ϸ�������ˡ�C�ͻ��ˡ�L��½�������ˡ�S�������˳��򣨿����ǵ�½��Ҳ��������Ϸ��
	//YYYYYY��ʾ��Ϣ����
	//���磺PACKET_CG_ATTACK ��ʾ�ͻ��˷�����Ϸ�������˹��ڹ�������Ϣ
	enum PACKET_DEFINE
	{
		PACKET_NONE = 0 ,						//0����
/*------------------------GC-------------------------------------------------------------------------------------------*/
	/* ID:1 */PACKET_GC_TEST ,
		PACKET_GC_CONNECT ,											//�������յ�PACKET_CG_CONNECT���͸��ͻ���
		PACKET_GC_LEAVESCENE ,										//���������������ͻ���ĳ����뿪����Ϣ
		PACKET_GC_ENTERSCENE ,										//�����������ͻ��˽��볡����ȷ����Ϣ
		PACKET_GC_NEWITEM,											//���µĶ�����Ʒ�������Լ�����Ұ��Χ
		PACKET_GC_NEWITEMBOX,
		PACKET_GC_NEWMONSTER,										//���µĹ�������Լ�����Ұ��Χ
		PACKET_GC_NEWMONSTER_DEATH,									//����������OBJ
		PACKET_GC_NEWMONSTER_MOVE,									//�����ƶ���OBJ
	/* ID:10 */PACKET_GC_NEWPLAYER,									//���µ�������ҽ����Լ�����Ұ��Χ
		PACKET_GC_NEWPLAYER_DEATH,									//����������PLAYER
		PACKET_GC_NEWPLAYER_MOVE,									//�����ƶ���PLAYER
		PACKET_GC_PLAYERREALMOVE,									//����
		PACKET_GC_MONSTERREALMOVE,									//����
		PACKET_GC_CHARBASEATTRIB,									//ˢ�½�ɫ��������
		PACKET_GC_CHAREQUIPMENT,									//ˢ�½�ɫ��װ����
		PACKET_GC_DIE ,
		PACKET_GC_LEVEUP,
		PACKET_GC_LEVELUPRESULT,									//�����������
	/* ID:20 */PACKET_GC_MANUALATTRRESULT,							//����������
		PACKET_GC_RESETATTRRESULT,									//ϴ����
		PACKET_GC_MYEQUIPMENT,										//����
		PACKET_GC_OTHEREQUIPMENT,									//����
		PACKET_GC_DETAILATTRIB ,									//ˢ�µ�ǰ��������
		PACKET_GC_DETAILEQUIPMENTLIST,								//ˢ�µ�ǰ����װ���б�
		PACKET_GC_DETAILSKILLLIST,									//ˢ�µ�ǰ���Ǽ����б�
		PACKET_GC_DETAILSUBEFFECTLIST,								//ˢ�µ�ǰ���Ǹ��������б�
		PACKET_GC_MYBAGLIST,										//ˢ�����Ǳ���
		PACKET_GC_CHARIDLE,											//֪ͨCLIENT����IDLE״̬
	/* ID:30 */PACKET_GC_CHARJUMP,									//֪ͨCLIENT��Ծһ��
		PACKET_GC_CHARMOVE,											//֪ͨCLIENT�ƶ�
		PACKET_GC_MOVE ,											//����
		PACKET_GC_ARRIVE ,											//����
		PACKET_GC_CHARSKILL_CREATEBULLET,							//�����ӵ�
		PACKET_GC_CHARSKILL_EFFECT,									//��������Ч��(�˺�)
		PACKET_GC_CHARSKILL_GATHER,									//���ܾ���
		PACKET_GC_CHARSKILL_GATHER_MODIFY,							//���ܾ���ʱ����ӳ��ȵ�
		PACKET_GC_CHARSKILL_LEAD,									//��������
		PACKET_GC_CHARSKILL_LEAD_MODIFY,							//��������ʱ������̵ȵ�
	/* ID:40 */PACKET_GC_CHARSKILL_SEND,							//���ܷ���
		PACKET_GC_ATTACK ,											//����
		PACKET_GC_OTHERATTACK ,										//����
		PACKET_GC_ERRORATTACK ,										//����
		PACKET_GC_SKILL ,											//����
		PACKET_GC_OTHERSKILL ,										//����
		PACKET_GC_ERRORSKILL ,										//����
		PACKET_GC_SKILLPREPARE ,									//����
		PACKET_GC_USEITEMRESULT,									//ʹ�õ��߽��
		PACKET_GC_BOXITEMLIST,										//Obj_ItemBox ��Ʒ�б�
	/* ID:50 */PACKET_GC_PICKRESULT,								//��ȡ���
		PACKET_GC_USEEQUIPRESULT,									//װ��װ�����
		PACKET_GC_DELOBJECT,										//ɾ��OBJ
		PACKET_GC_UNEQUIPRESULT,									//ж��װ�����
		PACKET_GC_DISCARDITEMRESULT,								//������Ʒ���
		PACKET_GC_RETCHANGESCENE ,									//��������Ӧ�ͻ��˵ĳ����任����
		PACKET_GC_CHAT,												//˵��
		PACKET_GC_NOTIFYCHANGESCENE ,								//������֪ͨ�ͻ��˿��Ըı䳡����
		PACKET_GC_ITEMINFO,											//����������ĳ��װ������ϸ��Ϣ
		PACKET_GC_DETAILEQUIPLIST,									//��������������װ����ϸ��Ϣ
	/* ID:60 */PACKET_GC_NEWPORTAL,									//��һ�����͵���Կ�����
		PACKET_GC_DETAILXINFALIST,									//ˢ�µ�ǰ�����ķ��б�
		PACKET_GC_DISCARDEQUIPRESULT,								//������Ʒ���
		PACKET_GC_NOTIFYEQUIP,										//֪ͨ�ͻ��˰����������װ����
		PACKET_GC_NEWSKILLOBJ,										//����SkillObj����Ϣ
		PACKET_GC_USEGEMRESULT,										//ʹ�ñ�ʯ���
		PACKET_GC_REMOVEGEMRESULT,									//ж�ر�ʯ���
		PACKET_GC_CHARIMPACT,										//��ɫ����Ч��
		PACKET_GC_OPERATE_RESULT,									//�����ķ���
		PACKET_GC_USEEQUIPSKILL_RESULT,
	/* ID:70 */PACKET_GC_TEAMERROR,									//
		PACKET_GC_TEAMASKINVITE,									//
		PACKET_GC_TEAMLEADERASKINVITE,								//
		PACKET_GC_TEAMRESULT,										//
		PACKET_GC_TEAMASKAPPLY,										//
		PACKET_GC_SCRIPTCOMMAND,									//�ű����ͻ��˵�ָ��
		PACKET_GC_ABILITY_RESULT,									//����ܵĲ������ؽ��
		PACKET_GC_DETAILABILITYINFO,								//�������˷�������������Ϣ
		PACKET_GC_COOLDOWN_UPDATE,									//��ȴʱ�����
		PACKET_GC_ABILITY_ACTION,									//����ܵĲ�������
	/* ID:80 */PACKET_GC_ABILITY_SUCC,								//����ܵĳɹ���Ϣ
		PACKET_GC_NOTIFYTEAMINFO ,									//֪ͨ�ͻ���,��������Ϣ�仯
		PACKET_GC_MISSIONLIST ,										//���������б�
		PACKET_GC_MISSIONADD ,										//����һ������
		PACKET_GC_MISSIONREMOVE ,									//ɾ��һ������
		PACKET_GC_MISSIONMODIFY ,									//����һ���������
		PACKET_GC_CHANNELERROR ,									//
		PACKET_GC_CHANNELRESULT ,									//
		PACKET_GC_MAIL ,											//
		PACKET_GC_NOTIFYMAIL ,										//
	/* ID:90 */PACKET_GC_NEWPLATFORM,								//��������̨����Ϣ
		PACKET_GC_ABILITYLEVEL,										//�������Ϣ
		PACKET_GC_ABILITYEXP,										//�������������Ϣ
		PACKET_GC_PRESCRIPTION,										//������䷽��Ϣ
		PACKET_GC_ABANDON_ABILITY,									//���������
		PACKET_GC_ITEMBOXCHANGESTAGE,								//Obj_ItemBox�ı����
		PACKET_GC_CANPICKMISSIONITEMLIST,							//����ʰȡ��������Ʒ�б�
		PACKET_GC_ADDCANPICKMISSIONITEM,							//���һ������ʰȡ��������Ʒ
		PACKET_GC_REMOVECANPICKMISSIONITEM,							//ɾ��һ������ʰȡ��������Ʒ
		PACKET_GC_MODIFYSPEED,										//�����ٶȸı�
	/* ID:100 */PACKET_GC_MONSTERSPEAK,								//��������˵��
		PACKET_GC_TEAMMEMBERINFO,									//���ض�����Ϣ
		PACKET_GC_LOCK_TARGET,										//�������Ŀ��
		PACKET_GC_SHOPMERCHANDISELIST,								//��Ʒ�б�	
		PACKET_GC_SHOPSOLDLIST,										//�ع���Ʒ�б�
		PACKET_GC_SHOPUPDATEMERCHANDISELIST,						//������Ʒ�б�
		PACKET_GC_SHOPBUY,											//֪ͨ�ͻ������Ƿ�ɹ�
		PACKET_GC_SHOPSELL,											//֪ͨ�ͻ������Ƿ�ɹ�
		PACKET_GC_SHOPREPAIR,										//֪ͨ������������Ʒ
		PACKET_GC_MENPAIINFO,										//������Ϣ��ID��
	/* ID:110 */PACKET_GC_JOINMENPAI,								//�������ɵĽ��
		PACKET_GC_XINFASTUDYINFO,									//����ѧϰ���ķ�������������Ϣ
		PACKET_GC_STUDYXINFA_H,										//ѧϰ�Ľ��
		PACKET_GC_ABILITY_STUDYINFO,								//ѧϰ����ܵ���Ϣ
		PACKET_GC_CHARALLTITLES,									//������֪ͨ�ͻ��˴������еĳƺ�
		PACKET_GC_ACTIVETIME_UPDATE,								//����
		PACKET_GC_OBJ_TELEPORT,										//��ɫ˲��
		PACKET_GC_BANKBEGIN,										//֪ͨ�ͻ��˿�ʼ������
		PACKET_GC_BANKACQUIRELIST,									//�����е���Ʒ���͸��ͻ���
		PACKET_GC_BANKADDITEM,										//֪ͨ�ͻ��˼���Ʒ���
	/* ID:120 */PACKET_GC_BANKREMOVEITEM,							//֪ͨ�ͻ����Ƴ���Ʒ���
		PACKET_GC_BANKSWAPITEM,										//֪ͨ�ͻ��˴��������뱳����װ�����н�����Ʒ�Ľ��
		PACKET_GC_BANKMONEY,										//֪ͨ�ͻ��˴�ǮȡǮ���
		PACKET_GC_DETAILATTRIB_PET,									//�������ϸ����
		PACKET_GC_EXCHANGEERROR,									//���׳���
		PACKET_GC_EXCHANGEAPPLYI,									//���뽻��
		PACKET_GC_EXCHANGEESTABLISHI,								//����ȷ��
		PACKET_GC_EXCHANGESYNCHLOCK,								//����ͬ����
		PACKET_GC_EXCHANGESYNCHCONFIRMII,							//����ͬ��ȷ��
		PACKET_GC_EXCHANGESYNCHII,									//����ͬ��
	/* ID:130 */PACKET_GC_EXCHANGESUCCESSIII,						//���׳ɹ���Ϣ
		PACKET_GC_EXCHANGECANCEL,									//����ȡ��
		PACKET_GC_PLAYER_DIE,										//��������
		PACKET_GC_PLAYER_RELIVE,									//���Ǹ���
		PACKET_GC_MANIPULATEPETRET,									//�����������ķ��ؽ��
		PACKET_GC_ASK_TEAMFOLLOW,									//����Ա���ͽ�����Ӹ�������
		PACKET_GC_RETURN_TEAMFOLLOW,								//���ͻ��˷�����Ӹ���״̬
		PACKET_GC_TEAMFOLLOW_LIST,									//��Ӹ����б�
		PACKET_GC_TEAMFOLLOW_ERROR,									//��Ӹ����µĴ�����Ϣ
		PACKET_GC_MISSIONRESULT,									//����������
	/* ID:140 */PACKET_GC_CHAR_DIRECT_IMPACT, 						//Ч��:һ����Ч��
		PACKET_GC_CHAR_BUFF, 										//Ч��: ������Ч��
		PACKET_GC_DETAIL_BUFF,										//Ч��: ������Ч������ϸ����.��������,ֻ�����Լ�
		PACKET_GC_CHARSKILL_MISSED,									//����: ����û������
		PACKET_GC_CHAR_IMPACT_LIST_UPDATE,							//����Ч���б�(��Ҫ��Ϣ���㲥��)
		PACKET_GC_DETAIL_IMPACT_LIST_UPDATE,						//����Ч���б�(��ϸ��Ϣ��ֻ������Լ���)
		PACKET_GC_DETAIL_HEALS_AND_DAMAGES, 						//HP,MP,Rage,Strike Point�ı仯�����ͻ�����ʾ�˺���������
		PACKET_GC_TARGET_LIST_AND_HIT_FLAGS,						//Ŀ���б�ͻ���������ڿͻ�����ʾ�ӵ��ͻ������
		PACKET_GC_PACKAGE_SWAPITEM,									//�����������������Ʒ�Ľ��
		PACKET_GC_STALLAPPLY,										//������֪ͨ�ͻ��������ڴ˴���̯
	/* ID:150 */PACKET_GC_STALLESTABLISH,							//֪ͨ�ͻ����ڴ˴���̯
		PACKET_GC_STALLOPEN,										//̯λ��Ʒ�б�̯λ��Ϣ
		PACKET_GC_STALLADDITEM,										//���߿ͻ�����̯λ�м���Ʒ
		PACKET_GC_STALLREMOVEITEM,									//֪ͨ�ͻ�����̯λ���ó���Ʒ
		PACKET_GC_STALLITEMPRICE,									//֪ͨ��������̯λ�е���Ʒ�ı��ۼ�
		PACKET_GC_STALLBUY,											//֪ͨ�ͻ��˹��������Ʒ
		PACKET_GC_STALLCLOSE,										//֪ͨ�ͻ����Լ��رյ���
		PACKET_GC_STALLERROR,										//֪ͨ��̯����
		PACKET_GC_BBSMESSAGES,										//���԰����
		PACKET_GC_RELATION,											//��Ϸ��������ͻ��˷��ͺ��������Ϣ
	/* ID:160 */PACKET_GC_GUILD,									//�����Ϣ
		PACKET_GC_GUILDLIST,										//���ذ���б�(UCHAR, {GuildID_t,NAME,Level,UserCount,Status})
		PACKET_GC_GUILDERROR,										//���ذ���������(enum)
		PACKET_GC_GUILDRETURN,										//���ذ��������(enum)
		PACKET_GC_GUILDBASEINFO,									//��������Ϣ
		PACKET_GC_GUILDMEMBERS,										//������Ϣ
		PACKET_GC_GUILDAUTHORITY,									//���Ȩ�ޱ�
		PACKET_GC_ITEMSYNCH,										//��Ʒ�ƶ���Ϣ
		PACKET_GC_ITEMLIST,											//������Ʒ�б���ͻ���
		PACKET_GC_TEAM_LIST,										//���ض�Ա�б�
	/* ID:170 */PACKET_GC_NEWSPECIAL,								//����һ��SpecialObj���ͻ���
		PACKET_GC_UICOMMAND,										//���Ϳͻ��˵�ĳ���û��ӿ�����
		PACKET_GC_SPLITITEMRESULT,									//�����Ʒ
		PACKET_GC_BANKITEMINFO,										//������Ʒ��Ϣ
		PACKET_GC_SPECIAL_OBJ_ACT_NOW,								//���������ʾ��Ч����
		PACKET_GC_SPECIAL_OBJ_FADE_OUT,								//����������������
		PACKET_GC_PLAYERSHOPERROR,									//����̵�ĳ�����Ϣ
		PACKET_GC_PLAYERSHOPAPPLY,									//������֪ͨ�ͻ��˴�������̵�������ж���Ϣ
		PACKET_GC_PLAYERSHOPESTABLISH,								//����̵��Ƿ����ɹ�
		PACKET_GC_PLAYERSHOPACQUIRESHOPLIST,						//������֪ͨ�ͻ��˴�������̵�������ж���Ϣ
	/* ID:180 */PACKET_GC_PACKUP_PACKET,							//�����������Ľ��
		PACKET_GC_PLAYERSHOPONSALE,									//֪ͨ�ͻ�����Ʒ���¼�
		PACKET_GC_PLAYERSHOPMONEY,									//֪ͨ�ͻ��˽�Ǯ��ȡ
		PACKET_GC_RETSETTING,										//��ͻ��˷����û���������
		PACKET_GC_PLAYERSHOPOPENSTALL,								//���Ż�رչ�̨
		PACKET_GC_REMOVEPET,										//�޸��û���������
		PACKET_GC_PETPLACARDLIST,									//���﹫�������
		PACKET_GC_CITYERROR,										//�ͻ��˵ĳ��д�����Ϣ
		PACKET_GC_CITYNOTIFY,										//�ͻ��˵ĳ���֪ͨ
		PACKET_GC_CITYATTR,											//server�ѳ�������ˢ���ͻ���
	/* ID:190 */PACKET_GC_NEWPET,									//���µĳ�������Լ�����Ұ��Χ
		PACKET_GC_NEWPET_DEATH,										//���������ĳ���
		PACKET_GC_NEWPET_MOVE,										//�����ƶ��ĳ���
		PACKET_GC_MINORPASSWD,										//��������
		PACKET_GC_CHARMOVERESULT,									//�ƶ�ָ���ִ�з���
		PACKET_GC_PLAYERSHOPSALEOUT,								//�̳��̵�
		PACKET_GC_PLAYERSHOPBUYSHOP,								//����˵�
		PACKET_GC_CHARSTOPACTION,									//ֹͣAction�߼�
		PACKET_GC_GUILDAPPLY,										//���뽨������
		PACKET_GC_PRIVATEINFO,										//������ϣ�ˢ���ͻ��ˣ�
	/* ID:200 */PACKET_GC_WORLDTIME,								//Server->Client ˢ������ʱ��
		PACKET_GC_CHARACTION,										//��һ������
		PACKET_GC_CHARMODIFYACTION,									//���Ŷ�����ִ��
		PACKET_GC_PLAYER_CALLOF,									//����ȥĳ��
		PACKET_GC_FINGER,											//�������
		PACKET_GC_PLAYERSHOPRECORDLIST,								//��¼�б�
		PACKET_GC_PLAYERSHOPUPDATEPARTNERS,							//���ºϻ����б�
		PACKET_GC_PLAYERSHOPUPDATEFAVORITE,							//�����µ��ղؼ�����ˢ���̵��б�
		PACKET_GC_PLAYERSHOPTYPE,									//֪ͨ�ͻ��˸ı��̵�����
		PACKET_GC_PLAYERSHOPSTALLSTATUS,							//�����������ı�ĳ����̨״̬
	/* ID:210 */PACKET_GC_CHARDOACTION,								//��ĳ��ɫ��һ��ָ������
		PACKET_GC_ADDLOCKOBJ,										//�����������뱣���Ľ��
		PACKET_GC_NOTIFYRMB,										// 
		PACKET_GC_MISSIONDESC,										//������������
		PACKET_GC_FIRSTLOGIN,										//��ҵ�һ�ε�¼
		PACKET_GC_SYSTEMMSG,										//Server��Client����ϵͳ��Ϣ
		PACKET_GC_MAXCOUNT,											//����GC���ĸ���
/*------------------------GC-------------------------------------------------------------------------------------------*/

/*------------------------LC-------------------------------------------------------------------------------------------*/
		PACKET_LC_RETLOGIN = 400,									//Login ���ؿͻ��˵�������
	/* ID:401*/PACKET_LC_RETCHARLIST,								//���ؽ�ɫ�б�
		PACKET_LC_STATUS,											//���ؿͻ��˵��Ŷ�״̬
		PACKET_LC_RETCHARLOGIN,										//�����½ָ����ɫ
		PACKET_LC_RETCREATECHAR,									//���ش�����ɫ���
		PACKET_LC_RETDELETECHAR,									//����ɾ����ɫ���
		PACKET_LC_RETCONNECT,										//Login����Client����������
		PACKET_LC_MAXCOUNT,											//����LC���ĸ���
/*------------------------LC-------------------------------------------------------------------------------------------*/

/*------------------------CL-------------------------------------------------------------------------------------------*/
		PACKET_CL_ASKLOGIN = 450,									//�ͻ�������Login��¼
	/* ID:451*/PACKET_CL_ASKCHARLIST,								//�����ɫ�б�
		PACKET_CL_ASKCHARLOGIN,										//�����½ָ����ɫ
		PACKET_CL_ASKCREATECHAR,									//���󴴽���ɫ
		PACKET_CL_ASKDELETECHAR,									//����ɾ����ɫ
		PACKET_CL_CONNECT,											//Client����Login����
/*------------------------CL-------------------------------------------------------------------------------------------*/

/*------------------------CG��GW��WG��SS��LB��BL��LW��WL---------------------------------------------------------------*/
		PACKET_CG_TEST = 500,
	/* ID:501 */PACKET_CG_CONNECT ,									//�ͻ������ӵ���Ϸ�������˺��͵ĵ�һ����֤��Ϣ
		PACKET_CG_HEARTBEAT ,										//������Ϣ
		PACKET_CG_ENTERSCENE ,										//�ͻ�����ͼ����ĳ������ʱ������������������Ϣ
		PACKET_CG_LEAVESCENE ,										//�ͻ�����ͼ�뿪ĳ������ʱ������������������Ϣ
		PACKET_CG_ENVREQUEST ,										//�ͻ������������������Χ������Ϣ
		PACKET_CG_CHARASKBASEATTRIB,								//��ȡ��ɫ������
		PACKET_CG_CHARASKEQUIPMENT,									//��ȡ��װ����
		PACKET_CG_REQLEVELUP,										//��������
		PACKET_CG_REQMANUALATTR,									//�����������
		PACKET_CG_REQRESETATTR,										//ϴ������
	/* ID:511 */PACKET_CG_MYEQUIPMENT,								//����
		PACKET_CG_OTHEREQUIPMENT,									//����
		PACKET_CG_ASKDETAILATTRIB ,									//������µ�ǰ��������
		PACKET_CG_ASKDETAILEQUIPMENTLIST,							//������µ�ǰ����װ���б�
		PACKET_CG_ASKDETAILSKILLLIST,								//������µ�ǰ���Ǽ����б�
		PACKET_CG_ASKDETAILSUBEFFECTLIST,							//������µ�ǰ���Ǹ��������б�
		PACKET_CG_ASKMYBAGLIST,										//����ˢ�µ�ǰ����
		PACKET_CG_CHARIDLE,											//֪ͨSERVER����IDLE״̬
		PACKET_CG_CHARJUMP,											//֪ͨSERVER��Ծһ��
		PACKET_CG_CHARMOVE,											//֪ͨSERVER�ƶ�
	/* ID:521 */PACKET_CG_MOVE ,									//����
		PACKET_CG_CHARUSESKILL,										//֪ͨSERVERʹ�ü���
		PACKET_CG_ATTACK ,											//����
		PACKET_CG_SKILL ,											//����
		PACKET_CG_OPENITEMBOX,										//��Obj_ItemBox
		PACKET_CG_PICKBOXITEM,										//��ȡObj_ItemBox���һ����Ʒ
		PACKET_CG_USEEQUIP,											//װ��������װ��
		PACKET_CG_UNEQUIP,											//ж��װ��
		PACKET_CG_USEITEM,											//ʹ�õ���
		PACKET_CG_DISCARDITEM,										//�������������Ʒ
	/* ID:531 */PACKET_CG_DISCARDEQUIP,								//��������װ��
		PACKET_SS_CONNECT ,											//������֮����������ʱ��ĵ�һ��֪ͨ��Ϣ
		PACKET_GS_ASKDATA ,											//��Դ�������˳���Ҫ��õ�Player����
		PACKET_SG_RETDATA ,											//��GameServer�˳������û�����
		PACKET_GW_ASKUSERDATA ,										//���������ݷ�����Ҫ���û�����
		PACKET_WG_RETUSERDATA ,										//�����û�������Ϣ
		PACKET_GW_ASKCHANGESCENE ,									//���������ݷ����������û����ϣ�Ҫ��ת�Ƴ���
		PACKET_WG_RETCHANGESCENE ,									//����ת�Ƴ��������Ϣ
		PACKET_GW_NOTIFYUSER ,										//��Ϸ������֪ͨ����������û�״̬��Ҫ�ı�
		PACKET_WG_NOTIFYUSER ,										//���������֪ͨ��Ϸ�������û�״̬�仯
	/* ID:541 */PACKET_CG_COMMAND ,									//�ͻ����������������Ϸ�޸�ָ��
		PACKET_CG_ASKCHANGESCENE ,									//�ͻ�������ı䳡��
		PACKET_CG_ASKITEMINFO,										//����ĳ����Ʒ����ϸ��Ϣ
		PACKET_CG_ASKDETAILEQUIPLIST,								//��������װ������ϸ��Ϣ
		PACKET_CG_CHAT,												//˵��
		PACKET_CG_ASKDETAILXINFALIST,								//������µ�ǰ�����ķ��б�
		PACKET_CG_USEGEM,											//ʹ�ñ�ʯ
		PACKET_CG_REMOVEGEM,										//ж�ر�ʯ
		PACKET_CG_CHARDEFAULTEVENT,									//ȱʡ�¼�
		PACKET_CG_USEEQUIPSKILL,							
	/* ID:551 */PACKET_CG_USEABILITY,								//���䷽�������ָ��
		PACKET_CG_TEAMINVITE,										//�������
		PACKET_GW_TEAMINVITE,										//
		PACKET_WG_TEAMERROR,										//
		PACKET_WG_TEAMRESULT,										//
		PACKET_WG_NOTIFYTEAMINFO,									//
		PACKET_WG_TEAMASKINVITE,									//
		PACKET_CG_TEAMRETINVITE,									//��ӻ�Ӧ
		PACKET_GW_TEAMRETINVITE,									//
		PACKET_WG_TEAMLEADERASKINVITE,								//
	/* ID:561 */PACKET_CG_TEAMLEADERRETINVITE,						//�ӳ���Ӧ
		PACKET_GW_TEAMLEADERRETINVITE,								//
		PACKET_CG_TEAMLEAVE,										//���
		PACKET_GW_TEAMLEAVE,										//
		PACKET_CG_TEAMDISMISS,										//��ɢ����
		PACKET_GW_TEAMDISMISS,										//
		PACKET_CG_TEAMKICK,											//�߳���Ա
		PACKET_GW_TEAMKICK,											//
		PACKET_CG_TEAMAPPLY,										//�����������
		PACKET_GW_TEAMAPPLY,										//
	/* ID:571 */PACKET_WG_TEAMASKAPPLY,								//
		PACKET_GW_TEAMRETAPPLY,										//
		PACKET_CG_TEAMRETAPPLY,										//
		PACKET_CG_ASKDETAILABILITYINFO, 							//�ͻ�����ҵ��������Ϣ
		PACKET_CG_TEAMAPPOINT ,										//�����¶ӳ�
		PACKET_GW_TEAMAPPOINT ,										//
		PACKET_CG_ABILITYGAMBLE,									//��������䷽�ϳ�ָ��
		PACKET_CG_ASK_COOLDOWN_UPDATE,								//�ͻ������������ȴʱ����Ϣ --���ڲ�����.
		PACKET_CG_DOEVENT,											//�ͻ���ѡ���¼��б��е�ĳ��ִ��
		PACKET_CG_MISSIONACCEPT ,									//��������
	/* ID:581 */PACKET_CG_MISSIONREFUSE ,							//�ܾ�����
		PACKET_CG_MISSIONABANDON ,									//��������
		PACKET_CG_MISSIONCONTINUE ,									//��������
		PACKET_CG_MISSIONSUBMIT ,									//�ݽ�����
		PACKET_CG_EVENTREQUEST ,									//��ѯһ���ű��¼�
		PACKET_CG_ASKMISSIONLIST ,									//�����б��ѯ
		PACKET_CG_ASKMAIL ,											//
		PACKET_CG_CHANNELCREATE ,									//
		PACKET_CG_CHANNELDISMISS ,									//
		PACKET_CG_CHANNELINVITE ,									//
	/* ID:591 */PACKET_CG_CHANNELKICK ,								//
		PACKET_CG_MAIL ,											//
		PACKET_GW_CHANNELCREATE ,									//
		PACKET_GW_CHANNELDISMISS ,									//
		PACKET_GW_CHANNELINVITE ,									//
		PACKET_GW_CHANNELKICK ,										//
		PACKET_GW_CHAT ,											//
		PACKET_GW_MAIL ,											//
		PACKET_WG_CHANNELERROR ,									//
		PACKET_WG_CHANNELRESULT ,									//
	/* ID:601 */PACKET_WG_CHAT ,									//
		PACKET_WG_MAIL ,											//
		PACKET_CG_GEM_COMPOUND,										//��ʯ�ϳ���Ϣ
		PACKET_GW_ASKMAIL ,											// 
		PACKET_WG_NOTIFYMAIL ,										//
		PACKET_CG_QUERYITEMBOXSTAGE,								//����Itembox���
		PACKET_CG_ASKTEAMINFO,										//���������Ϣ
		PACKET_GW_ASKTEAMINFO,										//���������Ϣ
		PACKET_CG_ASKTEAMMEMBERINFO,								//���������Ϣ
		PACKET_CG_LOCK_TARGET,										//�������Ŀ��
	/* ID:611 */PACKET_GW_ASKTEAMMEMBERINFO,						//���������Ϣ
		PACKET_WG_TEAMMEMBERINFO,									//���ض�����Ϣ
		PACKET_CG_SHOPBUY,											//��Ҵ���Ʒ�б�ѡ�������Ʒ
		PACKET_CG_SHOPSELL,											//��Ҵ���Ʒ�б�ѡ��Ҫ������Ʒ
		PACKET_CG_SHOPREPAIR,										//֪ͨ������������Ʒ
		PACKET_CG_SHOPCLOSE,										//֪ͨ�ͻ��˽��״��ڹرչر�
		PACKET_CG_ASKJOINMENPAI,									//��������
		PACKET_CG_ASKSTUDYXINFA,									//����ѧϰĳ��ĳ���ķ�
		PACKET_CG_ASKSTUDYABILITY,									//��������ύ����ѧϰ����ܵ���Ϣ
		PACKET_CG_CHARALLTITLES,									//�ͻ��������������������еĳƺ�
	/* ID:621 */PACKET_CG_CHARUPDATECURTITLE,						//������֪ͨ�ͻ��˴������еĳƺ�
		PACKET_CG_ASK_ACTIVETIME_UPDATE,							//�ͻ���������¼�����Чʱ����Ϣ
		PACKET_GW_TEAMMEMBERENTERSCENE,								//��Ա�����³���
		PACKET_CG_BANKACQUIRELIST,									//���������ȡ�洢���б�
		PACKET_CG_BANKADDITEM,										//֪ͨ�������������м���Ʒ
		PACKET_CG_BANKREMOVEITEM,									//֪ͨ���������������Ƴ���Ʒ
		PACKET_CG_BANKSWAPITEM,										//֪ͨ���������������뱳����װ�����н�����Ʒ
		PACKET_CG_BANKMONEY,										//֪ͨ��������ǮȡǮ
		PACKET_CG_BANKCLOSE,										//֪ͨ�������رմ�������
		PACKET_CG_EXCHANGEAPPLYI,									//���״�
	/* ID:631 */PACKET_CG_EXCHANGEREPLYI,							//���״�����
		PACKET_CG_EXCHANGESYNCHLOCK,								//����ͬ����
		PACKET_CG_EXCHANGESYNCHITEMII,								//����ͬ����Ʒ
		PACKET_CG_EXCHANGESYNCHMONEYII,								//����ͬ����Ǯ
		PACKET_CG_EXCHANGEOKIII,									//����ͬ����Ϣ
		PACKET_CG_EXCHANGECANCEL,									//����ȡ��
		PACKET_GW_LEVEUP,											//��Ա�������¼���ˢ�µ� World ����
		PACKET_CG_PLAYER_DIE_RESULT,								//���������Ĳ������
		PACKET_GW_ASKSCENEDATA,										//��Ϸ�������������ݷ��������󳡾�������Ϣ
		PACKET_WG_RETSCENEDATA,										//�������ݷ������򳡾����ͳ���������Ϣ
	/* ID:641 */PACKET_GG_SCENENOTIFY,								//����֪ͨ��Ϣ
		PACKET_CG_SET_PETATTRIB,									//֪ͨ�����������ĳ�������
		PACKET_CG_MANIPULATEPET,									//������������,���ٻ�,�ջ�,�����Ȳ���
		PACKET_CG_ASK_TEAMFOLLOW,									//�ӳ����������Ӹ���
		PACKET_CG_STOP_TEAMFOLLOW,									//ĳ��Ա������ֹͣ�������״̬
		PACKET_CG_RETURN_TEAMFOLLOW,								//��Ա�ظ��Ƿ������Ӹ���״̬
		PACKET_GW_ENTER_TEAMFOLLOW,									//ĳ��Ա����������״̬
		PACKET_GW_STOP_TEAMFOLLOW,									//ĳ��Աֹͣ�������״̬
		PACKET_WG_TEAMFOLLOW_LIST,									//������Ӹ����б�
		PACKET_CG_MISSIONCHECK,										//��֤�����Ƿ��Ѿ����
	/* ID:651 */PACKET_CG_DISPEL_BUFF,								//�ͻ�������ȡ��������Ч��
		PACKET_CG_CHAR_STOP_LOGIC,									//�ͻ���������ֹ��ǰ�߼�
		PACKET_CG_PACKAGE_SWAPITEM,									//Ҫ�󽻻��������������Ʒ
		PACKET_CG_STALLAPPLY,										//������������ڴ˴���̯
		PACKET_CG_STALLESTABLISH,									//�ͻ��˽���̯λ����˰��
		PACKET_CG_STALLOPEN,										//�����������̯λ��Ʒ�б�
		PACKET_CG_STALLADDITEM,										//֪ͨ��������̯λ�м���Ʒ
		PACKET_CG_STALLREMOVEITEM,									//֪ͨ��������̯λ���ó���Ʒ
		PACKET_CG_STALLITEMPRICE,									//֪ͨ��������̯λ�е���Ʒ�ı��ۼ�
		PACKET_CG_STALLSHOPNAME,									//֪ͨ���������̸���
	/* ID:661 */PACKET_CG_STALLBUY,									//֪ͨ���������������Ʒ
		PACKET_CG_STALLCLOSE,										//֪ͨ�������Լ��رյ���
		PACKET_CG_BBSSYCHMESSAGES,									//���԰����
		PACKET_CG_BBSAPPLY,											//���԰����
		PACKET_CG_RELATION,											//�ͻ�������Ϸ���������ͺ��������Ϣ
		PACKET_GW_RELATION,											//��Ϸ���������������ݷ��������ͺ��������Ϣ
		PACKET_WG_RELATION,											//�������ݷ���������Ϸ���������ͺ��������Ϣ
		PACKET_CG_GUILD,											//�����Ϣ
		PACKET_GW_GUILD,											//�����Ϣ
		PACKET_WG_GUILD,											//�����Ϣ
	/* ID:671 */PACKET_WG_GUILDLIST,								//���ذ���б�(PlayerID_t, UCHAR, {GuildID_t,NAME,Level,UserCount,Status})
		PACKET_WG_GUILDERROR,										//���ذ���������(PlayerID_t, enum)
		PACKET_WG_GUILDRETURN,										//���ذ��������(PlayerID_t, enum)
		PACKET_WG_GUILDBASEINFO,									//��������Ϣ
		PACKET_WG_GUILDMEMBERS,										//������Ϣ
		PACKET_WG_GUILDAUTHORITY,									//���Ȩ�ޱ�
		PACKET_CG_ITEMSYNCH,										//��Ʒ�ƶ���Ϣ
		PACKET_LB_ASKAUTH,											//Login ����BillingSystem��֤
		PACKET_BL_RETAUTH,											//BillingSystem ������֤���
		PACKET_LW_ASKCHARLOGIN,										//�����ɫ��¼
	/* ID:681 */PACKET_WL_RETCHARLOGIN,								//World�����ɫ��¼
		PACKET_WG_TEAM_LIST,										//���ض�Ա�б�
		PACKET_CG_EXECUTESCRIPT,									//����ִ�з������˵�ĳ���ű�����
		PACKET_SS_SCENEPLAYERCOUNT,									//�������ݷ�����֪ͨ��Ϸ�������͵�½�����������е�������Ϣ
		PACKET_CG_SPLITITEM,										//�����Ʒ
		PACKET_CG_PLAYERSHOPAPPLY,									//����������봴������̵�
		PACKET_CG_PLAYERSHOPESTABLISH,								//�ͻ�ȷ���ж���Ϣ���ݽ��������
		PACKET_CG_PLAYERSHOPACQUIRESHOPLIST,						//����������봴������̵�
		PACKET_CG_PLAYERSHOPACQUIREITEMLIST,						//�ͻ�ȷ���ж���Ϣ���ݽ��������
		PACKET_GW_COMMAND,											//��Ҫ�������ݷ����������GMָ��
	/* ID:691 */PACKET_CG_PACKUP_PACKET,							//����������
		PACKET_CG_PLAYERSHOPONSALE,									//֪ͨ��������Ʒ���¼�
		PACKET_CG_PLAYERSHOPBUYITEM,								//֪ͨ������������Ʒ
		PACKET_CG_PLAYERSHOPMONEY,									//֪ͨ��������Ǯ��ȡ
		PACKET_CG_ASKSETTING,										//����õ��û���������
		PACKET_CG_MODIFYSETTING,									//�޸��û���������
		PACKET_CG_PLAYERSHOPNAME,									//�޸�����̵����
		PACKET_CG_PLAYERSHOPDESC,									//�޸�����̵������
		PACKET_CG_PLAYERSHOPOPENSTALL,								//���Ż�رչ�̨
		PACKET_GW_BATCHMAIL,										//����������Ϣ
	/* ID:701 */PACKET_GW_CITYAPPLYNEWCITY,							//��World�������뽨���µĳ���
		PACKET_WG_CITYAPPLYNEWCITY,									//�ظ�Server�������뽨���µĳ���
		PACKET_WG_CITYERROR,										//�ظ�Server���д�����Ϣ
		PACKET_WG_CITYATTR,											//worldˢ�³�������
		PACKET_CG_CITYASKATTR,										//������������������
		PACKET_GW_CITYASKINITINFO,									//����ʱ�������������г�ʼ����Ϣ
		PACKET_GW_CITYCLOSE,										//�رճ���
		PACKET_GW_CITYCLOSESUCCESS,									//�رճ��гɹ�
		PACKET_WG_CITYCLOSE,										//�رճ��гɹ�
		PACKET_WG_CITYCLOSESUCCESS,									//�رճ��гɹ�
	/* ID:711 */PACKET_WG_CITYINITINFO,								//���������Ϣ
		PACKET_GW_CITYOPT,											//���в���
		PACKET_CG_ISSUEPETPLACARD,									//�������﹫��
		PACKET_CG_STALLFIRSTPAGE,									//��̯�ĳ�ʼҳ��
		PACKET_CG_MINORPASSWD,										//��������
		PACKET_CG_CHARPOSITIONWARP,									//λ��ƫ���������
		PACKET_CG_PLAYERSHOPSALEOUT,								//�̳��̵�
		PACKET_CG_PLAYERSHOPBUYSHOP,								//����˵�
		PACKET_CG_GUILDAPPLY,										//���뽨������
		PACKET_CG_GUILDJOIN,										//������빤��
	/* ID:721 */PACKET_CG_GUILDASKMEMBERINFO,						//�쿴�����Ա
		PACKET_GW_CHANGEMENPAI,										//�ı�����
		PACKET_GW_UPDATETITLE,										//���³ƺ�
		PACKET_CG_ASKPRIVATEINFO,									//������ϣ�����
		PACKET_CG_APPLYPRIVATEINFO,									//����޸��������
		PACKET_WG_WORLDTIME,										//World->Server ˢ������ʱ��
		PACKET_CG_CHARSIT,											//�������
		PACKET_CG_PLAYER_CALLOF_RESULT,								//����ȥĳ�صĲ������
		PACKET_GW_CALLOFHUMAN,										//���˵���Ϣ
		PACKET_WG_CALLOFHUMAN,										//���˵���Ϣ
	/* ID:731 */PACKET_CG_STALL_DEFAULT_PAGE,						//��̯ȱʡҳ���ύ
		PACKET_CG_SETMOODTOHEAD,									//����������ʾ
		PACKET_CG_FINGER,											//�������
		PACKET_GW_FINGER,											//������ҵĽ��
		PACKET_WG_FINGER,											//������ҵĽ��
		PACKET_CG_PLAYERSHOPASKFORRECORD,							//��������̵꽻�׼�¼��������¼
		PACKET_CG_PLAYERSHOPPARTNER,								//�������
		PACKET_CG_PLAYERSHOPFAVORITE,								//�����ɾ���ղؼ�
		PACKET_CG_PLAYERSHOPTYPE,									//����������ı��̵�����
		PACKET_CG_PLAYERSHOPSIZE,									//�����������ı��̵��С
	/* ID:741 */PACKET_CG_CGASKLOCKOBJ,								//��������һ��Obj
		PACKET_CG_CHARMOODSTATE,									// �����¼�
		PACKET_GW_HEARTBEAT,										//������ҵ�������Ϣ��World
		PACKET_CG_MISSIONDESC,										//�õ���������
		PACKET_CG_FIRSTLOGIN,								
		PACKET_LW_ASKDELETECHAR,									//����ɾ����ɫ
		PACKET_WL_RETDELETECHAR,									//World����ɾ����ɫ
		PACKET_BW_CONNECT,											//Web��������Billing��������һ����֤KEY����Ϣ
		PACKET_BW_VALIDATEUSER,										//Billing����������Web��֤�û���������
		PACKET_WB_RET_VALIDATEUSER,									//Billing����������Web��֤�û���������
	/* ID:751 */PACKET_WG_SYSTEMMSG,								//World��Server����ϵͳ��Ϣ
		PACKET_GW_SYSTEMMSG,										//Server��World����ϵͳ��Ϣ
/*------------------------CG��GW��WG��SS��LB��BL��LW��WL---------------------------------------------------------------*/
		PACKET_MAX													//��Ϣ���͵����ֵ
	};
};


#endif

