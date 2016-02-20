// UI_CEGUI.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "UIGlobal.h"
#include "UISystem.h"
#include "TDPlugin.h"
#include "TDException.h"
#include "TDKernel.h"

//Magic Code
//extern "C" UINT DllMagicCode = DLL_MAGIC_CODE|TD_ENGINE_VERSION;

//�����ʼ��
extern "C" VOID __stdcall DllInit( tKernel* pKernel )
{
	TDAssert(pKernel);

	g_pKernel = pKernel;

	pKernel->ReisgerClass(GETCLASS(CUISystem));
}

//����ͷ�
extern "C" VOID __stdcall DllRelease(VOID)
{
}