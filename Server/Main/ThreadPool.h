

#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "Type.h"
#include "Thread.h"

//ϵͳ��������ӵ�е��߳�����
#define MAX_THREAD MAX_SCENE


class ThreadPool
{
public :
	ThreadPool( ) ;
	~ThreadPool( ) ;

	//���������߳�
	BOOL			Start( ) ;
	//ֹͣ�����߳�
	BOOL			Stop( ) ;

	//���һ���̵߳��̳߳�
	BOOL			AddThread( Thread* pThread ) ;

	//ɾ��һ���߳�
	BOOL			DelThread( TID id ) ;

	//�����߳�IDȡ���߳�ָ��
	Thread*			GetThread( TID id ) ;

	//�����߳�����ȡ���߳�ָ��
	Thread*			GetThreadByIndex( UINT index ) ;



	VOID			Lock( ){ m_Lock.Lock() ; } ;
	VOID			Unlock( ){ m_Lock.Unlock() ; } ;



private :
	Thread*			m_pThread[MAX_THREAD] ;
	UINT			m_Count ;
	UINT			m_Position ;
	MyLock			m_Lock ;

};



#endif
