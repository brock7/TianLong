

#ifndef __LOGINPLAYER_H__
#define __LOGINPLAYER_H__

#include "Type.h"
#include "Player.h"




class LoginPlayer : public Player
{
public :
	LoginPlayer( ) ;
	~LoginPlayer( ) ;

	//��Ϣִ�нӿ�
	virtual BOOL		ProcessCommand( BOOL Option = TRUE ) ;
	//���ݽ��սӿ�
	virtual BOOL		ProcessInput( ) ;
	//���ݷ��ͽӿ�
	virtual BOOL		ProcessOutput( ) ;

//�����ӿڣ����������߼�
	virtual BOOL		HeartBeat( UINT uTime=0 ) ;

	//�������
	virtual VOID		CleanUp( ) ;


public :
	//Ӧ�ýӿ�
	virtual BOOL		IsLoginPlayer( ){ return TRUE; } ;
	virtual BOOL		IsServerPlayer( ){ return FALSE ; } ;

	//���ӳɹ����ʼ����������
	VOID				Init( ) ;

	//���Player����һ����Ϣ��
	//�˽ӿ�ֻ���ڱ�ִ���߳��ڴ���������ͬ��������
	virtual BOOL		SendPacket( Packet* pPacket ) ;

	virtual VOID		Encrypt_SC(CHAR* header, UINT uLen, UINT uBeginPlace){ENCRYPT(header, uLen, LOGIN_TO_CLIENT_KEY, uBeginPlace)}

	virtual VOID		DecryptHead_CS(CHAR* header){ENCRYPT_HEAD(header, CLIENT_TO_LOGIN_KEY)}

	virtual VOID		Decrypt_CS(CHAR* header, UINT uLen, UINT uBeginPlace){ENCRYPT(header, uLen, CLIENT_TO_LOGIN_KEY, uBeginPlace)}

	//���״̬���á���ȡ�ӿ�
	VOID				SetPlayerStatus( UINT status ){ m_Status = status ; } ;
	UINT				GetPlayerStatus( ) { return m_Status ; } ;
	
	//�Ͽ��������ӣ����һ���Player����
	//�����ݱ����պ�������ϻᱻPlayerPool�����ȥʹ��
	BOOL				FreeOwn( ) ;
	virtual VOID		ResetKick( ) ;
	virtual VOID		Disconnect( ) ;
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
public :
	const CHAR*				GetAccount();
	VOID					SetAccount(const CHAR*  pAccount);
	
	UINT					GetVersion();
	VOID					SetVersion(UINT uVersion);

	UINT					GetQueuePos();
	VOID					SetQueuePos(UINT QueuePos);

	INT						GetCharNumber();
	VOID					SetCharNumber(INT CharNumber);

	VOID					SetCharGUID(GUID_t	guid,UINT uIndex);

	BOOL					IsGUIDOwner(GUID_t guid);

	VOID					SetUserKey(UINT key);
	UINT					GetUserKey();

	VOID					SetDBOperating(BOOL bOp);
	BOOL					GetDBOperating();

	VOID					SetReadyKickCount(INT KickCount);	
	INT						GetReadyKickCount();

	UINT					GetLastSendTurnTime();
	VOID					SetLastSendTurnTime(UINT uTime);

	INT						GetWrongPWCount();
	VOID					SetWrongPWCount(INT Count);
	
	SceneID_t				GetChooseSceneID() const;
	VOID					SetChooseSceneID(SceneID_t sid);

	WORLD_POS				GetWorldPos()	const;
	VOID					SetWorldPos(WORLD_POS& pos);

	CampID_t				GetPlayerCamp();
	VOID					SetPlayerCamp(CampID_t id);

	BYTE					GetPlayerAge();
	VOID					SetPlayerAge(BYTE age);

	UINT					m_LastDBOpTime;		//���һ�����ݿ����ʱ��
private :
	UINT					m_Status ;
	GUID_t					m_AccountGuid;				//�˺�guid ��billingsystem ���أ�ͨ����֤�Ժ�
														//�Ŀͻ����Ժ����������
	UINT					m_Version;					//�ͻ��˰汾
	UINT					m_LastSendProcessTurn;		//������߼��Ŷ���Ϣʱ��
	UINT					m_QueuePos;					//�ڶ����е�λ��
	INT						m_CharNumber;				//��ɫ����
	CHAR					szAccount[MAX_ACCOUNT+1] ;	//�û�����
	GUID_t					m_GUIDList[DB_CHAR_NUMBER]; //�û���ɫGUID�б�
	UINT					m_uKey;						//��ɫ��¼Key
	BOOL					m_bDBOprating;				//�Ƿ�ǰ�������ݿ����
	INT						m_ReadyKickCount;
	INT						m_WrongPWCount;				//������֤�������
	SceneID_t				m_SceneID;					//������
	WORLD_POS				m_WorldPos;					//����λ��
	CampID_t				m_Camp;						//��ǰ�˻�����Ӫ
	BYTE					m_Age;
	//MyLock					m_Lock;	
public :
	//��Ϸ����
	UINT					m_KickTime ;		//�ж��Ƿ���Ҫ�ߵ���ҵļ�ʱ��
	UINT					m_LastSendTime ;	//�ϴη������ݵ�ʱ��
	UINT					m_ConnectTime;
	UINT					m_CurrentTime ;		//��ǰ�߼�ʱ��
	INT						m_LeftTimeToQuit ;	//ʣ�౻����˳���ʱ��
	BOOL					m_Dirty ;			//�˱�־��ʾ��ǰ�����Ѿ���Ч��
												//����Ҫ�����κ�״̬��Ϣ���������ݷ�����
	
	
};





#endif
