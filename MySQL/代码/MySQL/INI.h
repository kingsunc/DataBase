#pragma once
#include <winbase.h>
#include "MySQLExport.h"

class MYSQL_API INI
{
public:
	INI(void)
	{
	}
	~INI(void)
	{
	}

public:

	static LPCTSTR GetStrValue(LPCTSTR lpFileName, LPCTSTR lpAppName, LPCTSTR lpKeyName)
	{
		TCHAR lpValue[255] = {0};
		auto nSize = GetPrivateProfileString(lpAppName, lpKeyName, NULL, lpValue, 255, lpFileName);
		if (nSize < 0)
		{
			return FALSE;
		}

		return lpValue;
	}

	static BOOL Write(LPCTSTR lpFileName, LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpValue)
	{
		return WritePrivateProfileString(lpAppName, lpKeyName, lpValue, lpFileName);
	}
};

