/****************************************\
*										*
* 		 ��Ϸ�߼��¼�������				*
*										*
\****************************************/

#pragma once
#include "TDNode.h"

//�¼�����ص�����
enum GAME_EVENT_ID;
struct EVENT;
typedef VOID (__stdcall* FUNC_EVENT_HANDLE)(const EVENT* pEvent, UINT dwOwnerData);

struct EVENT_DEFINE
{
	typedef std::list< std::pair< FUNC_EVENT_HANDLE, UINT > > REGISTER_STRUCT;
	GAME_EVENT_ID		idEvent;
	LPCTSTR				szEvent;
	BOOL				delayProcess;
	REGISTER_STRUCT		listFuncNotify;
};

struct EVENT
{
	EVENT_DEFINE*			pEventDef;
	std::vector< STRING >	vArg;
	bool operator == (const EVENT& other)
	{
		if(other.pEventDef != pEventDef) return false;
		if(other.vArg.size() != vArg.size()) return false;
		for(register size_t i=0; i<vArg.size(); i++)
		{
			if(vArg[i] != other.vArg[i]) return false;
		}
		return true;
	}
};


//=====================================================
//��Ϸ�¼�ϵͳ
class tEventSystem : public tNode
{
public:
	//�����¼�
	/*
		!!!! �˺���������Ϊ��ӿ�  !!!
		���ܻ������ڴ��ڲ�ͬģ��֮������ͷ�
	
		virtual VOID	PushEvent(GAME_EVENT_ID id, const std::vector< STRING >& vParam) = 0;
	*/

	virtual VOID	PushEvent(GAME_EVENT_ID id)	= 0;
	virtual VOID	PushEvent(GAME_EVENT_ID id, INT iArg0)	= 0;
	virtual VOID	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0)	= 0;
	virtual VOID	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1)	= 0;
	virtual VOID	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1, INT nArg2 )	= 0;
	virtual VOID	PushEvent(GAME_EVENT_ID id, INT iArg0, INT iArg1)	= 0;
	virtual VOID	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1, INT iArg2, INT iArg3)	= 0;

	//ע���¼�������
	virtual VOID	RegisterEventHandle(const STRING& nameEvent, FUNC_EVENT_HANDLE funHandle, UINT dwOwnerData=NULL) = 0;
	//�����¼�
	virtual VOID	ProcessAllEvent(VOID) = 0;
protected:
	WX_DECLARE_DYNAMIC(tEventSystem);
};