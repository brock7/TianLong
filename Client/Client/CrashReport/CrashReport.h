// CrashReport.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CCrashReportApp:
// �йش����ʵ�֣������ CrashReport.cpp
//

class CCrashReportApp : public CWinApp
{
public:
	CCrashReportApp();

// ��д
	public:
	virtual BOOL InitInstance();
private:
	//
	VOID SendReport();
// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCrashReportApp theApp;
