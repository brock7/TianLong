// Render.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Global.h"
#include "RenderSystem.h"
#include "ResourceProvider.h"
#include "TDPlugin.h"
#include "TDKernel.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hInstance = (HINSTANCE)hModule;
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

//Magic Code
//extern "C" UINT DllMagicCode = DLL_MAGIC_CODE|TD_ENGINE_VERSION;

//�����ʼ��
extern "C" VOID __stdcall DllInit( tKernel* pKernel )
{
	g_pKernel = pKernel;

	//ע����
	g_pKernel->ReisgerClass(GETCLASS(CRenderSystem));
	g_pKernel->ReisgerClass(GETCLASS(CResourceProvider));
}

//����ͷ�
extern "C" VOID __stdcall DllRelease( VOID )
{
}
