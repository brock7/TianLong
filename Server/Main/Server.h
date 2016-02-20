
#ifndef __SERVER_H__
#define __SERVER_H__

#include "Type.h"




//��Ϸ���ģ��
//������ģ�鶼��ͨ����ģ������ʼ����ִ��

class Server
{
public :
	Server( ) ;
	~Server( ) ;

	//��ʼ������
	//___________________________________
	BOOL	InitConfig();

	//��ʼ��
	BOOL	InitServer( ) ;
	//��ѭ��������ѭ�����ջύ��ClientManagerģ����ʹ����ִ����Դ
	BOOL	Loop( ) ;
	//�˳�
	BOOL	ExitServer( ) ;

	//��������
	//Ϊÿ��ģ������ڴ�
	BOOL	NewStaticServer( ) ;
	//��ʼ������ģ��
	BOOL	InitStaticServer( ) ;

	//ֹͣģ��ִ�У������ô˽ӿں󣬻Ὣ����ӵ���̵߳�ģ������Ϊ���״̬
	//�������̵߳�ѭ��������һ��ѭ��������ͻ��˳�
	VOID	Stop( ) ;

	//�ȴ����������̶߳��˳���˺����ŷ���
	//�˺�����һ����������������������߳�û���˳����˺�����һֱ����
	VOID	WaitForAllThreadQuit( ) ;


public :
	CMyTimer	m_TimeToQuit ;


};

extern Server g_Server ;

class ServerExceptionHandler
{
public:
	ServerExceptionHandler();
	//VOID INTHandler(INT);
	//VOID TERMHandler(INT);
	//VOID ABORTHandler(INT);
	//VOID ILLHandler(INT);
	//VOID FPEHandler(INT);
	//VOID SEGHandler(INT);
	//VOID XFSZHandler(INT);
};

extern ServerExceptionHandler g_ServerExceptionHandler;



#endif
