

#ifndef __IDTABLE_H__
#define __IDTABLE_H__

#include "Type.h"


struct _TABLEITEM
{
	UINT	m_ID ;
	VOID*	m_pPtr ;
	UINT	m_Status ;
};

class IDTable
{
public :
	IDTable( ) ;
	~IDTable( ) ;

	//��ʼ����
	VOID		InitTable( UINT MaxItem ) ;
	//����һ������
	BOOL		Add( UINT id, VOID* pPtr ) ;
	//��ȡ��Ϣ
	VOID*		Get( UINT id ) ;
	//ɾ������
	VOID		Remove( UINT id ) ;
	//�����������
	VOID		CleanUp( ) ;

protected :
	enum {
		IDTS_EMPTY = 0 ,
		IDTS_SET = 1 ,
		IDTS_USE = 2 ,
	};

	UINT		toIndex( UINT id )
	{
		return (UINT)((id&0xffff)+(id>>6))%m_Count ;
	};

private :

	_TABLEITEM*		m_pTable ;
	UINT			m_Count ;


};

#endif
