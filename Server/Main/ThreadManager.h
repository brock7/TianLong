

#ifndef __THREADMANAGER_H__
#define __THREADMANAGER_H__

#include "Type.h"
#include "ThreadPool.h"
#include "ServerThread.h"



//�������߳����⣬�����̶߳��ɴ�ģ��������

class ThreadManager
{
public :
	ThreadManager( ) ;
	~ThreadManager( ) ;

	//��ʼ��
	BOOL				Init( UINT MaxSceneCount = MAX_SCENE ) ;
	//���������߳�
	BOOL				Start( ) ;
	//ֹͣ�����߳�
	BOOL				Stop( ) ;

	//ȡ�õ�ǰ���̳߳�
	ThreadPool*			GetThreadPool(){ return m_pThreadPool ; } ;

	//ȡ�õ�ǰ�ķ������߳�
	ServerThread*		GetServerThread(){ 
		return m_pServerThread ; 
	} ;

	//ȡ�õ�ǰ���е��߳�����
	UINT				GetTotalThreads(){ return m_nThreads ; } ;


protected :
	ThreadPool*			m_pThreadPool ;
	ServerThread*		m_pServerThread ;
	UINT				m_nThreads ;




};

extern ThreadManager*	g_pThreadManager ;




#endif
