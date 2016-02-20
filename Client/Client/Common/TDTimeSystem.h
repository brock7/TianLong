#pragma once

#include "TDNode.h"

//ʱ��ϵͳ,����ϵͳʱ���ϵļ�������ٿ���
class tTimeSystem : public tNode
{
public:
	//ȡ�����20ms����ÿ��ƽ�����ѵ�ʱ��
	FLOAT			GetLoopTime(VOID) const { return m_fLoopTime; }
	//ȡ����һ�����ѵ�ʱ��
	FLOAT			GetDeltaTime(VOID) const { return m_fDeltaTime; }
	//ȡ�ø���m_fLoopTime�ۻ���ϵͳ����ʱ��
	FLOAT			GetRealTime(VOID) const { return m_fRealTime; }
	//ȡ�õ�ǰʱ��
	UINT			GetTimeNow(VOID) const { return m_dwTimeNow; }
	//ȡ������
	FLOAT			GetFPS(VOID) const { return m_fFPS; }
	//����ʱ���,����ʱ����(49.71��)
	UINT			CalSubTime(UINT timeStart, UINT timeEnd);
	//�õ�����
	UINT			GetTickCount(VOID) { return m_dwTickCount; }

public:
	//�ڵ��ʼ��
	virtual VOID						Initial(VOID*);
	//�߼���ѭ����
	virtual VOID						Tick(VOID);

private:
	UINT		m_dwTimeNow;		//��ǰʱ��
	FLOAT		m_fLoopTime;		//���20ms����ÿ��ƽ�����ѵ�ʱ��
	FLOAT		m_fDeltaTime;		//��һ�������ѵ�ʱ��
	FLOAT		m_fRealTime;		//����m_fLoopTime�ۻ���ϵͳ����ʱ��
	FLOAT		m_fFPS;				//��ǰ������
	UINT		m_dwTickCount;		//������

	WX_DECLARE_DYNAMIC(tTimeSystem);
};