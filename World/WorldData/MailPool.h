
/////////////////////////////////////////////////////////////////////////////////
//�ļ�����MailPool.h
//�����������ʼ����ݼ��û��ڵ����ݳ�
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __MAILPOOL_H__
#define __MAILPOOL_H__


#include "Type.h"
#include "MailDefine.h"

//û�ڵ��ʼ���
#define MAX_MAILNODE_SIZE 102400

//�û��ڵ�����
#define MAX_USERNODE_SIZE 4096


//�ʼ����ݳ�
class MailNodePool
{
public :
	MailNodePool( ) ;
	~MailNodePool( ) ;

	BOOL				Init( UINT MaxMailNode = MAX_MAILNODE_SIZE ) ;
	VOID				CleanUp( ) ;

	MailNode*			NewNode( ) ;
	VOID				DeleteNode( MailNode* pNode ) ;
	
	MailNode*			GetNode(UINT Index);
protected :
	MailNode*			m_pMailNode ;
	
	INT					m_nMailNodeNumber ;
	INT					m_nCurrentOffset ;
};

extern MailNodePool* g_pMailNodePool ;


//�û��ڵ��
class UserNodePool
{
public :
	UserNodePool( ) ;
	~UserNodePool( ) ;

	BOOL				Init( ) ;
	VOID				CleanUp( ) ;

	UserNode*			NewNode( ) ;
	VOID				DeleteNode( UserNode* pNode ) ;

protected :
	UserNode*			m_pUserNode ;

	INT					m_nUserNodeNumber ;
	INT					m_nCurrentOffset ;

};

extern UserNodePool* g_pUserNodePool ;

#endif
