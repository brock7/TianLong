
#ifndef __LOG_H__
#define __LOG_H__

#include "Type.h"
#include "File.h"
#include "LogDefine.h"


enum LOG_FILE_NAME_ID
{
	LOG_FILE_0 =0 ,
	LOG_FILE_1 =1 ,
	LOG_FILE_2 =2 ,
	LOG_FILE_3 =3 ,
	LOG_FILE_4 =4 ,
	LOG_FILE_5 =5 ,
	LOG_FILE_6 =6 ,
	LOG_FILE_7 =7 ,
	LOG_FILE_8 =8 ,
	LOG_FILE_9 =9 ,
	LOG_FILE_10 =10,
	LOG_FILE_11 =11,
	LOG_FILE_12 =12,
	LOG_FILE_13 =13,

	LOG_FILE_NUMBER ,
};

#define LOGIN_LOGFILE					"./Log/Login.log"
#define BILLING_LOGFILE					"./Log/Billing.log"
#define SERVER_LOGFILE					"./Log/Debug.log"
#define SERVER_ERRORFILE				"./Log/Error.log"
#define SERVER_FUNCTIONFILE				"./Log/Functions.log"
#define WORLD_LOGFILE					"./Log/world.log"
#define CONFIG_LOGFILE					"./Log/config.log"
#define ASSERT_LOGFILE					"./Log/assert.log"
#define RECYCLEPLAYER_LOGFILE			"./Log/RecyclePlayer.log"


#define DEFAULT_LOG_CACHE_SIZE 1024*1024*4
class Log
{
public :
	Log( ) ;
	~Log( ) ;

	BOOL			Init( ) ;

	//����־����д����־��Ϣ
	VOID			FastSaveLog( INT logid, CHAR* msg, ... ) ;

	//����־�ڴ�����д���ļ�
	VOID			FlushLog( INT logid ) ;

	//ȡ����־��Ч���ݴ�С
	INT				GetLogSize( INT logid ){ return m_LogPos[logid] ; }

	//ȡ�ñ�����־���ļ�����
	VOID			GetLogName( INT logid, CHAR* szName ) ;

	//ˢ��ÿ����־�ļ�
	VOID			FlushLog_All( ) ;

	//ȡ����������
	UINT			GetDayTime( ){ return m_DayTime ; }
	//������������
	VOID			SetDayTime( UINT daytime ){ m_DayTime = daytime ; }


	//֧���첽д���������־д��
	static VOID		SaveLog( CHAR* filename, CHAR* msg, ... ) ;	
	//ɾ����־����
	static VOID		RemoveLog( CHAR* filename ) ;


private :
	CHAR*			m_LogCache[LOG_FILE_NUMBER] ;	//��־�ڴ���
	INT				m_LogPos[LOG_FILE_NUMBER] ;		//��־��ǰ��Ч����λ��
	MyLock			m_LogLock[LOG_FILE_NUMBER] ;	//��־��д��
	INT				m_CacheSize ;
	UINT			m_DayTime ;
};

#if defined __LINUX__
	#define SaveCodeLog()	(Log::SaveLog( SERVER_ERRORFILE, "%s %d %s", __FILE__,__LINE__,__PRETTY_FUNCTION__ ))
#else
	#define SaveCodeLog()	(Log::SaveLog( SERVER_ERRORFILE, "%s %d %s", __FILE__,__LINE__,__FUNCTION__ ))
#endif

extern Log* g_pLog ;

#endif
