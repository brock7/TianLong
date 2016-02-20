/****************************************\
*										*
*			  ��Դ�ṩϵͳ				*
*										*
\****************************************/
#pragma once
#include "TDNode.h"

//
//��Դ�ṩϵͳ
//
class tResourceProvider : public tNode
{
public:
	virtual DWORD loadResource(LPCTSTR szFileName, LPSTR& szMemAddr, LPCTSTR szResourceGroup = NULL) = 0;
	virtual BOOL unloadResource(LPSTR szMemAddr, DWORD dwSize) = 0;
	WX_DECLARE_DYNAMIC(tResourceProvider);
};
