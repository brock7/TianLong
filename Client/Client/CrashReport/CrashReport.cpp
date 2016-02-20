// CrashReport.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "CrashReport.h"
#include "CrashReportDlg.h"
#include "FtpTrans.h"
#include "xzip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCrashReportApp

BEGIN_MESSAGE_MAP(CCrashReportApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCrashReportApp ����

CCrashReportApp::CCrashReportApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CCrashReportApp ����

CCrashReportApp theApp;

CHAR g_szCurrentDir[MAX_PATH] = {0};
CHAR g_szSmallInfoFile[MAX_PATH] = {0};
CHAR g_szBigInfoFile[MAX_PATH] = {0};
CHAR g_szDumpFile[MAX_PATH] = {0};

// CCrashReportApp ��ʼ��

BOOL CCrashReportApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	/*SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CCrashReportDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȷ�������ر�
		//�Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//�Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�*/
	LPTSTR lpCmdLine = this->m_lpCmdLine;

	CHAR szFileName[MAX_PATH] = {0};
	GetModuleFileName(GetModuleHandle(NULL), szFileName, MAX_PATH);
	PathRemoveFileSpec(szFileName);
	PathAddBackslash(szFileName);
	strcpy(g_szCurrentDir, szFileName);

	//this->m_pMainWnd->MessageBox(g_szCurrentDir);

	//����������
	char* pMark1 = strchr(lpCmdLine, '"'); if(!pMark1) return 0;
	char* pMark2 = strchr(pMark1+1, '"');  if(!pMark2) return 0;

	char* pMark3 = strchr(pMark2+1, '"');  if(!pMark3) return 0;
	char* pMark4 = strchr(pMark3+1, '"');  if(!pMark4) return 0;

	char* pMark5 = strchr(pMark4+1, '"');  if(!pMark5) return 0;
	char* pMark6 = strchr(pMark5+1, '"');  if(!pMark6) return 0;

	*pMark2 = 0; _snprintf(g_szSmallInfoFile, MAX_PATH, "%s", pMark1+1);
	*pMark4 = 0; _snprintf(g_szBigInfoFile, MAX_PATH, "%s", pMark3+1);
	*pMark6 = 0; _snprintf(g_szDumpFile, MAX_PATH, "%s", pMark5+1);


	//GetCommandLine();
	SendReport();
	return FALSE;
}
void CCrashReportApp::SendReport(void)
{
	//-------����------------------------------------------------
	CHAR szCurrentDir[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szCurrentDir);

	CHAR szCrashFile[MAX_PATH] = {0};
	SYSTEMTIME timeNow;
	::GetLocalTime(&timeNow);
	_snprintf(szCrashFile, MAX_PATH, "crash%02d_%02d_%02d_%02d.zip",
		timeNow.wMonth, timeNow.wDay, timeNow.wHour, timeNow.wMinute,timeNow.wSecond);

	//-------------------------------------------------------
	CHAR szLocalFile[MAX_PATH] = {0};
	strncpy(szLocalFile, szCurrentDir, MAX_PATH);
	PathAppend(szLocalFile, szCrashFile);
	HZIP hz = CreateZip((void *)szLocalFile, 0, ZIP_FILENAME);
	if(!hz) 
	{

		this->m_pMainWnd->MessageBox("Create Crash.zip error");
		return;
	}

	ZipAdd(hz, "dump.dmp", (void *)g_szDumpFile, 0, ZIP_FILENAME);
	ZipAdd(hz, "info.txt", (void *)g_szBigInfoFile, 0, ZIP_FILENAME);

	CHAR szLogFile[MAX_PATH];

	strncpy(szLogFile, szCurrentDir, MAX_PATH);
	PathAppend(szLogFile, "WXRender.log");
	ZipAdd(hz, "WXRender.log", (void *)szLogFile, 0, ZIP_FILENAME);

	strncpy(szLogFile, szCurrentDir, MAX_PATH);
	PathAppend(szLogFile, "UISystem.log");
	ZipAdd(hz, "UISystem.log", (void *)szLogFile, 0, ZIP_FILENAME);

	strncpy(szLogFile, szCurrentDir, MAX_PATH);
	PathAppend(szLogFile, "../wxversion");
	ZipAdd(hz, "wxversion", (void *)szLogFile, 0, ZIP_FILENAME);

	CloseZip(hz);

	//--------------------------����
	CFtpTrans CMyFtpTrans;
	CMyFtpTrans.SetAccessRight("wxsj","up");
	if(CMyFtpTrans.OpenConnection("ftp.hopecool.com "))
	{
		CMyFtpTrans.PutFile(szLocalFile,szCrashFile);
		CMyFtpTrans.CloseConnection();
		//this->m_pMainWnd->MessageBox("OK");
	}
}
