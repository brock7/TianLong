/****************************************\
*										*
* 			   ��Ϸ�¼�������			*
*										*
\****************************************/
#pragma once

#include <queue>
#include "TDEventSystem.h"
#include "TDEventDefine.h"

//��Ϸ�¼�������
class CEventSystem : public tEventSystem
{
public:
	//�����¼�
	VOID	PushEvent(GAME_EVENT_ID id, const std::vector< STRING >& vParam);

	virtual VOID	PushEvent(GAME_EVENT_ID id);
	virtual VOID	PushEvent(GAME_EVENT_ID id, INT iArg0);
	virtual VOID	PushEvent(GAME_EVENT_ID id, FLOAT fArg0);
	virtual VOID	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0);
	virtual VOID	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1);
	virtual VOID	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1, INT nArg2 );
	virtual VOID	PushEvent(GAME_EVENT_ID id, INT iArg0, INT iArg1);
	virtual VOID	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1, INT iArg2, INT iArg3);
	//ע���¼�������
	virtual VOID	RegisterEventHandle(const STRING& nameEvent, FUNC_EVENT_HANDLE funHandle, UINT uOwnerData=NULL);
	//�����¼�
	virtual VOID	ProcessAllEvent(VOID);

public:
	virtual VOID	Initial(VOID*);
	virtual VOID	Release(VOID);
	virtual VOID	Tick(VOID);

protected:
	void	_PushEvent(const EVENT& event);
	void	_ProcessEvent(const EVENT& event);

	//ͨ���¼����Ƽ�����
	std::map< STRING, EVENT_DEFINE* >					m_mapEventIndex_AsName;
	//ͨ���¼�ID������
	std::map< GAME_EVENT_ID, EVENT_DEFINE* >			m_mapEventIndex_AsID;
	
	//�¼�����
	std::list< EVENT >		m_queueEvent;

	//���ٴ������, ÿ��һ������ֹ�������Ϣͬʱӿ��
	std::list< EVENT >		m_delayQueueEvent;
	UINT					m_dwLastTickCount;

public:
	CEventSystem();
	virtual ~CEventSystem();
	static CEventSystem*	GetMe(VOID) { return s_pMe; }

protected:
	static CEventSystem* s_pMe;

protected:
	WX_DECLARE_DYNAMIC(CEventSystem);
};