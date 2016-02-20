/****************************************\
*										*
*				�쳣���				*
*										*
\****************************************/
#pragma once
#include "TDBasicType.h"

// ref from http://www.codeproject.com/tools/minidump.asp 
//generate stack frame pointers for all functions - same as /Oy- in the project
#pragma optimize("y", off)


//---------------------------------------------------------
//����SEH�쳣
VOID tProcessException(PEXCEPTION_POINTERS pException, bool bRunCrashReport);

//---------------------------------------------------------
// �쳣Ӧ��

//�׳��ַ�����ʽ�쳣
VOID tThrowStringException(LPCTSTR szFmt, ...);
//�׳�������ʽ�쳣
VOID tThrowAssertException(LPCTSTR szFileName, INT nFileLine, LPCTSTR pExpr);
//��ʾһ���ڲ��쳣
INT tProcessInnerException(PEXCEPTION_POINTERS pException, HWND hParentWnd, LPCTSTR szTitle);

#if 1
	//�׳�һ���ַ��������쳣
	#define TDThrow	tThrowStringException

	//�����ж�
	#define TDAssert( bCondition) \
		do { \
				if( (bCondition) == 0 ) \
				{ \
					tThrowAssertException(_T(__FILE__), __LINE__, _T(#bCondition)); \
				} \
			} while(0);

#else
	//�׳�һ���ַ��������쳣
	#define TDThrow	tThrowStringException

	//�����ж� Release�治���κ�����
	#define TDAssert(bCondition) (VOID)(0)
#endif

				
