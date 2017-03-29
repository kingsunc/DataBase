#pragma once

class INI
{
public:
	INI(void)
	{
	}
	~INI(void)
	{
	}

public:
	static BOOL Read(const CString& strFile,const CString& strSection,const  CString& srtKey, CString &strValue)
	{
		TCHAR buf[256] = {0};
		auto nSize = GetPrivateProfileString(strSection,srtKey,NULL,buf,256,strFile);
		if (nSize < 0)
		{
			return FALSE;
		}

		strValue.Empty();
		strValue.Append(buf,nSize);
		return TRUE;
	}

	static CString GetValue(const CString strFile,const CString strSection,const  CString srtKey)
	{
		CString strValue;
		TCHAR buf[256] = {0};
		auto nSize = GetPrivateProfileString(strSection,srtKey,NULL,buf,256,strFile);
		if (nSize < 0)
		{
			return FALSE;
		}

		strValue.Empty();
		strValue.Append(buf,nSize);

		return strValue;
	}

	static BOOL Write(const CString& strFile,const CString& strSection,const CString& strKey,const CString &strValue)
	{
		return WritePrivateProfileString(strSection,strKey,strValue,strFile);
	}

};

