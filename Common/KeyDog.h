#ifdef DLL_GETMAC
#else
#define DLL_GETMAC _declspec(dllexport)
#endif
BOOL GetMacByCmd(char *lpszMac);

/*
���÷���

��һ�ַ���:(���lib)
#define DLL_GETMAC _declspec(dllimport)
#include "DllNet.h"

char lpszMac[128];
memset(lpszMac, 0x00, sizeof(lpszMac));
GetMacByCmd(lpszMac);

�ڶ��ַ���:

char lpszMac[128];
HINSTANCE hinst;
hinst=LoadLibrary("DllNet.dll");
if(hinst==NULL)
{
  return 0;
}
typedef void (*GETMAC)(char *lpszMac);
GETMAC GetMacByCmd=(GETMAC)GetProcAddress(hinst,"GetMacByCmd");
if(!GetMacByCmd)
{
  return 0;
}
GetMacByCmd(lpszMac);



*/