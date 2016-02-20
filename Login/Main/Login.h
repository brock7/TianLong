#ifndef _LOGIN_H_
#define _LOGIN_H_

#include "Type.h"

class Login
{
public:
	Login();
	~Login();

public:
	//Login ��������ʼ��
	BOOL		Init();

	//Login ��������ѭ��
	BOOL		Loop();

	//Login �������˳�
	BOOL		Exit();


	//ֹͣģ��ִ�У������ô˽ӿں󣬻Ὣ����ӵ���̵߳�ģ������Ϊ���״̬
	//�������̵߳�ѭ��������һ��ѭ��������ͻ��˳�
	VOID		Stop( ) ;

protected:
	//����������ݿռ�
	BOOL		NewLogin();
	//��ʼ��������ݿռ�
	BOOL		InitLogin();
	BOOL		m_bExited;
};


extern Login	g_Login;

class LoginExceptionHandler
{
public:
	LoginExceptionHandler();
	//VOID INTHandler(INT);
	//VOID TERMHandler(INT);
	//VOID ABORTHandler(INT);
	//VOID ILLHandler(INT);
	//VOID FPEHandler(INT);
	//VOID SEGHandler(INT);
	//VOID XFSZHandler(INT);
};

extern LoginExceptionHandler g_LoginExceptionHandler;


#endif