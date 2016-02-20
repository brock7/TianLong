// ������Ϣ�ṹ

#ifndef __GAMESTRUCT_MINORPASSWD_H__
#define __GAMESTRUCT_MINORPASSWD_H__

#include "Type.h"

class SocketInputStream;
class SocketOutputStream;

/////////////////////////////////////////////////////////////////////////////////

enum MINORPASSWD_REQUEST_TYPE
{
	MREQT_NONE							= 0,
	MREQT_PASSWDSETUP,									// ѯ�ʶ��������Ƿ��Ѿ�����
	MREQT_DELETEPASSWDTIME,								// ѯ���Ƿ���ǿ�ƽ���׶�
	MREQT_SETPASSWD,									// ���ö�������
	MREQT_MODIFYPASSWD,									// �޸Ķ�������
	MREQT_UNLOCKPASSWD,									// �����������
	MREQT_DELETEPASSWD,									// ǿ�ƽ����������
};

enum MINORPASSWD_RETURN_TYPE
{
	MRETT_NONE							= 0,
	MRETT_PASSWDSETUP,									// ���������Ƿ��Ѿ�����
	MRETT_DELETEPASSWDTIME,								// ����������ʣ��ʱ�䣨���߲��ڽ���׶Σ�
	MRETT_SETPASSWDSUCC,								// �����������óɹ�
	MRETT_MODIFYPASSWDSUCC,								// ���������޸ĳɹ�
	MRETT_UNLOCKPASSWDSUCC,								// ������������ɹ�
	MRETT_DELETEPASSWDCANCEL,							// ǿ�ƽ����������ʧЧ
	MRETT_DELETEPASSWDSUCC,								// ��������ǿ�ƽ���ɹ�

	MRETT_ERR_START,
	MRETT_ERR_SETPASSWDFAIL,							// ������������ʧ��
	MRETT_ERR_MODIFYPASSWDFAIL,							// ���������޸�ʧ��
	MRETT_ERR_UNLOCKPASSWDFAIL,							// �����������ʧ��
	MRETT_ERR_DELETEPASSWDFAIL,							// ��������ǿ�ƽ��ʧ��
};

// �޲���
// MREQT_PASSWDSETUP
// MREQT_DELETEPASSWDTIME
// MREQT_DELETEPASSWD

// ����һ������
// MREQT_SETPASSWD
// MREQT_UNLOCKPASSWD
struct MINORPASSWD_PWD
{
	//����
	UCHAR								m_uPasswdSize;
	CHAR								m_Passwd[MAX_PWD];		//��������

	//�����ӿ�
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	UCHAR								GetPasswdSize( ) { return m_uPasswdSize; }

	const CHAR*							GetPasswd( ) { return m_Passwd; }
	VOID								SetPasswd( const CHAR* szPasswd )
	{
		Assert( szPasswd );
		strncpy(m_Passwd, szPasswd, sizeof(m_Passwd) - 1);
		m_uPasswdSize = (UCHAR)strlen(m_Passwd);
	}
};

// MREQT_MODIFYPASSWD
struct REQUEST_MINORPASSWD_MODIFY
{
	//����
	UCHAR								m_uOldPasswdSize;
	CHAR								m_OldPasswd[MAX_PWD];	// �ɶ�������
	UCHAR								m_uNewPasswdSize;
	CHAR								m_NewPasswd[MAX_PWD];	// �¶�������

	//�����ӿ�
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	UCHAR								GetOldPasswdSize( ) { return m_uOldPasswdSize; }

	const CHAR*							GetOldPasswd( ) { return m_OldPasswd; }
	VOID								SetOldPasswd( const CHAR* szPasswd )
	{
		Assert( m_OldPasswd );
		strncpy(m_OldPasswd, szPasswd, sizeof(m_OldPasswd) - 1);
		m_uOldPasswdSize = (UCHAR)strlen(m_OldPasswd);
	}

	UCHAR								GetPasswdSize( ) { return m_uNewPasswdSize; }

	const CHAR*							GetNewPasswd( ) { return m_NewPasswd; }
	VOID								SetNewPasswd( const CHAR* szPasswd )
	{
		Assert( m_NewPasswd );
		strncpy(m_NewPasswd, szPasswd, sizeof(m_NewPasswd) - 1);
		m_uNewPasswdSize = (UCHAR)strlen(m_NewPasswd);
	}
};

// e.g.
// set minor password
// CGMinorPasswd Msg;
// CG_MINORPASSWD* pMinorPasswd = Msg.GetMinorPassword();
// pMinorPasswd->CleanUp();
// pMinorPasswd->m_Type = MREQT_SETPASSWD;
// pMinorPasswd->m_OnePasswd.CleanUp();
// pMinorPasswd->m_OnePasswd.SetPasswd( szPasswd );

struct CG_MINORPASSWD
{
	UCHAR								m_Type;				// enum MINORPASSWD_REQUEST_TYPE

	union
	{
		MINORPASSWD_PWD					m_OnePasswd;
		REQUEST_MINORPASSWD_MODIFY		m_ModifyPasswd;
	};

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

};

/////////////////////////////////////////////////////////////////////////////////

// �޲���

// MRETT_SETPASSWDSUCC
// MRETT_MODIFYPASSWDSUCC
// MRETT_UNLOCKPASSWDSUCC
// MRETT_DELETEPASSWDCANCEL
// MRETT_DELETEPASSWDSUCC

// MRETT_ERR_SETPASSWDFAIL
// MRETT_ERR_MODIFYPASSWDFAIL
// MRETT_ERR_UNLOCKPASSWDFAIL
// MRETT_ERR_DELETEPASSWDFAIL

// MRETT_PASSWDSETUP
struct RETURN_MINORPASSWD_SETUP
{
	UCHAR								m_uFlag;			// 0 Ϊδ���ã�1 Ϊ������

	//�����ӿ�
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	UCHAR								GetSetupFlag( ) { return m_uFlag; }
	VOID								SetSetupFlag( UCHAR uFlag ) { m_uFlag = uFlag; }
};

// MRETT_DELETEPASSWDTIME
struct RETURN_DELETE_MINORPASSWD_TIME
{
	UINT								m_uTime;			// 0 Ϊδ����ǿ�ƽ���׶Σ�����Ϊǿ�ƽ����ʱ��

	//�����ӿ�
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	UINT								GetDeleteTime( ) { return m_uTime; }
	VOID								SetDeleteTime( UINT uTime ) { m_uTime = uTime; }
};

struct GC_MINORPASSWD
{
	UCHAR								m_Type;				// enum MINORPASSWD_RETURN_TYPE

	union
	{
		RETURN_MINORPASSWD_SETUP		m_ReturnSetup;
		RETURN_DELETE_MINORPASSWD_TIME	m_ReturnDeleteTime;
	};

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;
};

//////////////////////////////////////////////////////////////////////////

#endif // __GAMESTRUCT_MINORPASSWD_H__
