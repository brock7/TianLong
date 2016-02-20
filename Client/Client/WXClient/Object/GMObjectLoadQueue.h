#pragma once

#include <map>

class Packet;
class SocketOutputStream;
//������ض��У���ֹһ����ͬʱ���ع�������
class CObjectLoadQueue
{
public:
	//��һ����ҪAsk��obj����
	BOOL		TryAddLoadTask(INT idObj, INT ObjType);
	//������
	void		Tick(void);
	//ǿ��ִ��
	void		ExcuteTask(INT idObj);
	//���ĳһ��ָ��Object�Ķ���
	void		ClearTask(INT idObj);
	//��չ������У��л�����ʱ��
	void		ClearAllTask(void);
	//�Ƿ���������NPC
	void		SetLoadNPCDirect(BOOL b);

protected:
	//���վ���Զ���Ĺ�������
	typedef std::multimap< FLOAT, INT > LOAD_TASK_MAP;
	LOAD_TASK_MAP	m_mapLoadTask;
	BOOL			m_LoadDirect;

public:
	CObjectLoadQueue();
	virtual ~CObjectLoadQueue();
};