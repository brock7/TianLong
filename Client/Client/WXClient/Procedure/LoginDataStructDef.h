#ifndef __LOGINDATASTRUCTDEF_H__
#define __LOGINDATASTRUCTDEF_H__

#define  LOGINSERVER_IN_AREA_COUNT 6

// login server ״̬.
enum LOGIN_SERVER_STATUS
{
	LOGIN_SERVER_FULL	= 0, // ����
	LOGIN_SERVER_BUSY,		 // ӵ��
	LOGIN_SERVER_NORMAL,	 // ����
	LOGIN_SERVER_IDLE,		 // ����
	LOGIN_SERVER_STOP,		 // ά��

};

// ���������
enum NET_PROVIDE
{
	NET_PROVIDE_CNII	= 0, // ����
	NET_PROVIDE_CNC,		 // ��ͨ
	NET_PROVIDE_OTHER,		 // ����

};

// login server �ṹ��Ϣ
struct __LOGINSERVER_INOF
{
	string szLoginServerName;						// login server������
	string szIp;									// ip��ַ				
	int  iPort;										// �˿ں�
	LOGIN_SERVER_STATUS  iLoginServerStatus;		// login server��״̬.

};
typedef __LOGINSERVER_INOF LOGINSERVER_INOF, *PLOGINSERVER_INOF;


// ����������ṹ��Ϣ
struct __AREA_INFO	//�����������Ϣ
{
	string szAreaName;					// ��������
	NET_PROVIDE iProvide;				// ���繩Ӧ��
	vector<LOGINSERVER_INOF> LoginInfo;	// login server�ľ�����Ϣ

};
typedef __AREA_INFO AREA_INFO, *PAREA_INFO;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �Զ�ѡ���������Ҫ������
//

// login server �������ĵ���������
struct __LOGIN_SERVER_POS
{
	int iAreaIndex;
	int iLoginServerIndex;
};
typedef __LOGIN_SERVER_POS LOGIN_SERVER_POS, *PLOGIN_SERVER_POS;

// Login server ����
typedef std::vector<LOGIN_SERVER_POS> LOGIN_SERVER_POS_VECTOR;

// ���繩Ӧ����Ϣ
struct __PROVIDE_INFO
{
	LOGIN_SERVER_POS_VECTOR	IdleLoginServerVector;
	LOGIN_SERVER_POS_VECTOR	NorLoginServerVector;
	LOGIN_SERVER_POS_VECTOR	BusyLoginServerVector;
	LOGIN_SERVER_POS_VECTOR	FullLoginServerVector;		
};

typedef __PROVIDE_INFO PROVIDE_INFO, *PPROVIDE_INFO;



#endif //__LOGINDATASTRUCTDEF_H__