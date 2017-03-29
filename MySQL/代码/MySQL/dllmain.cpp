// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <atlstr.h>

TString g_strDirPath;
BOOL APIENTRY DllMain(	HMODULE	hModule,
						DWORD	ul_reason_for_call,
						LPVOID	lpReserved
						)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	CString strDirPath;
	GetModuleFileName(NULL, strDirPath.GetBufferSetLength(MAX_PATH+1), MAX_PATH);  
	strDirPath.ReleaseBuffer();  
	strDirPath = strDirPath.Left(strDirPath.ReverseFind(_T('\\')));

	g_strDirPath = strDirPath;

	return TRUE;
}

