/////////////////////////////////////////////////////////////////////////////////
//�ļ�����SceneInfo.h
//����������ͳ�Ƴ�����������Ϣ
//
/////////////////////////////////////////////////////////////////////////////////


#ifndef __SCENEINFO_H__
#define __SCENEINFO_H__


#include "Type.h"



class SceneInfo
{
public :
	SceneInfo( ) ;
	~SceneInfo( ) ;

	BOOL				Init( ) ;
	VOID				CleanUp( ) ;

	VOID				IncScenePlayerCount(SceneID_t sceneid){
		m_aScenePlayerCount[sceneid] ++ ;
	}
	VOID				DecScenePlayerCount(SceneID_t sceneid){
		m_aScenePlayerCount[sceneid] -- ;
	}


	BOOL				HeartBeat( UINT uTime ) ;


protected :
	INT					m_aScenePlayerCount[MAX_SCENE] ;	//��ǰ�ĳ���ʵ������
	INT					m_aScenePlayerCount_Bak[MAX_SCENE] ;//�ϴ��߼��ĳ�������


	CMyTimer			m_LoginTimer ;


};

extern SceneInfo*		g_pSceneInfo ;



#endif
