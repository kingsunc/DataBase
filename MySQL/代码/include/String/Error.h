#pragma once
#include "StringHeader.h"
#include <Strsafe.h>
#pragma comment(lib,"Strsafe.lib")

inline TString ErrorMessage()   
{   

	LPVOID lpMsgBuf;  
	DWORD dw = GetLastError();   
	TCHAR szBuf[128] = {0};

	FormatMessage(  
		FORMAT_MESSAGE_ALLOCATE_BUFFER |   
		FORMAT_MESSAGE_FROM_SYSTEM |  
		FORMAT_MESSAGE_IGNORE_INSERTS,  
		NULL,  
		dw,  
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  
		(LPTSTR) &lpMsgBuf,  
		0, NULL );  

	StringCbPrintf(szBuf,128,_T(" 出错信息 (出错码:%d): %s"), dw, lpMsgBuf);
	LocalFree(lpMsgBuf);
	TString strErr = szBuf;
	return strErr;
}