#ifndef STRINGTOOL_H    
#define STRINGTOOL_H   

#include <Windows.h>
#include <string>
#include <codecvt>

namespace StringTool
{
	//Encode编码(gb2312编码)
	inline BOOL EncodeGB2312(const char* szSrc, char* pBuf, int cbBufLen, BOOL bUpperCase)
	{
		if(szSrc == NULL || pBuf == NULL || cbBufLen <= 0)
			return FALSE;

		size_t len_ascii = strlen(szSrc);
		if(len_ascii == 0)
		{
			pBuf[0] = 0;
			return TRUE;
		}

		char baseChar = bUpperCase ? 'A' : 'a';

		unsigned char c;
		int cbDest = 0; //累加
		unsigned char *pSrc = (unsigned char*)szSrc;
		unsigned char *pDest = (unsigned char*)pBuf;
		while(*pSrc && cbDest < cbBufLen - 1)
		{
			c = *pSrc;
			if(isalpha(c) || isdigit(c) || c == '-' || c == '.' || c == '~')//isalpha判断是否为英文字母，为小写返回2，为大写返回1；isdigit判断是否为0-9
			{
				*pDest = c;
				++pDest;
				++cbDest;
			}
			else if(c == ' ')
			{
				*pDest = '+';
				++pDest;
				++cbDest;
			}
			else
			{
				//检查缓冲区大小是否够用？
				if(cbDest + 3 > cbBufLen - 1)
					break;
				pDest[0] = '%';
				pDest[1] = (c >= 0xA0) ? ((c >> 4) - 10 + baseChar) : ((c >> 4) + '0');
				pDest[2] = ((c & 0xF) >= 0xA)? ((c & 0xF) - 10 + baseChar) : ((c & 0xF) + '0');
				pDest += 3;
				cbDest += 3;
			}
			++pSrc;
		}
		//null-terminator
		*pDest = '\0';

		return TRUE;
	}
	inline void UrlEncode(IN const char *szUrl,OUT char *buf)//Encode编码
	{
	
		WCHAR* pWString = NULL;
		char * pString = NULL;

		//先将字符串由多字节转换成UTF-8编码
		int nLength = MultiByteToWideChar(CP_ACP, 0, szUrl, -1, NULL, 0);

		//分配Unicode空间
		pWString = new WCHAR[nLength];

		//先转换成Unicode
		MultiByteToWideChar(CP_ACP, 0, szUrl, -1, pWString, nLength);

		//分配UTF-8空间
		nLength = WideCharToMultiByte(CP_UTF8, 0, pWString, -1, NULL, 0, NULL, NULL);
		pString = new char[nLength];

		//Unicode转到UTF-8
		nLength = WideCharToMultiByte(CP_UTF8, 0, pWString, -1, pString, nLength, NULL, NULL);

		static char hex[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
		
		int nIndex = 0;
		for( int i = 0; i < nLength-1; i++ )
		{
			unsigned char c = pString[i];
			if( c > 0x20 && c < 0x7f )	// 数字或字母
			{
				buf[nIndex]=c;// += (c);
				nIndex++;
			}
			else if( c == 0x20 )		// 包含空格
			{
				buf[nIndex]= '+';
				nIndex++;
			}
			else						// 进行编码
			{
				buf[nIndex++] = ('%');
				buf[nIndex++] = (hex[c / 16]);
				buf[nIndex++]= (hex[c % 16]);
			}
		}	
		buf[nIndex] = '\0';

		delete pString;
		delete pWString;
		
	}

	
	inline void  Utf8ToGb2312(IN const char* utf8,OUT char *szGB)//UTF-8到GB2312的转换
	{
		int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
		wchar_t* wstr = new wchar_t[len+1];
		memset(wstr, 0, len+1);
		MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
		len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, wstr, -1, szGB, len, NULL, NULL);
		szGB[len] = '\0';


		delete[] wstr;
		
		
	}


	
	inline BOOL MByteToWChar(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize)//多字节转成宽字节
	{
		DWORD dwMinSize;
		dwMinSize = MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, NULL, 0);

		if(dwSize < dwMinSize+1)
		{
			return FALSE;
		}

		MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, lpwszStr, dwMinSize);
		lpwszStr[dwMinSize] = _T('\0');
		return TRUE;
	}

	
	inline BOOL WCharToMByte(LPCWSTR lpcwszStr, LPSTR lpszStr, DWORD dwSize)//宽字节转成多字节
	{
		DWORD dwMinSize;
		dwMinSize = WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,NULL,0,NULL,FALSE);

		if(dwSize < dwMinSize+1)
		{
			return FALSE;
		}

		WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,lpszStr,dwSize,NULL,FALSE);
		lpszStr[dwMinSize] = _T('\0');
		return TRUE;
	}
	
	inline std::string WS_2_S( const std::wstring& src )
	{
		std::locale sys_locale("");

		const wchar_t* data_from = src.c_str();
		const wchar_t* data_from_end = src.c_str() + src.size();
		const wchar_t* data_from_next = 0;

		int wchar_size = 4;
		char* data_to = new char[(src.size() + 1) * wchar_size];
		char* data_to_end = data_to + (src.size() + 1) * wchar_size;
		char* data_to_next = 0;

		memset(data_to, 0, (src.size() + 1) * wchar_size );

		typedef std::codecvt<wchar_t, char, mbstate_t> convert_facet;
		mbstate_t out_state = 0;
		auto result = std::use_facet<convert_facet>(sys_locale).out(
			out_state, data_from, data_from_end, data_from_next,
			data_to, data_to_end, data_to_next );
		if( result == convert_facet::ok )
		{
			std::string dst = data_to;
			delete[] data_to;
			return dst;
		}
		else
		{
			printf( "convert error!\n" );
			delete[] data_to;
			return std::string("");
		}
	}

	inline std::wstring S_2_WS( const std::string& src )
	{
		std::locale sys_locale("");

		const char* data_from = src.c_str();
		const char* data_from_end = src.c_str() + src.size();
		const char* data_from_next = 0;

		wchar_t* data_to = new wchar_t[src.size() + 1];
		wchar_t* data_to_end = data_to + src.size() + 1;
		wchar_t* data_to_next = 0;

		wmemset( data_to, 0, src.size() + 1 );

		typedef std::codecvt<wchar_t, char, mbstate_t> convert_facet;
		mbstate_t in_state = 0;
		auto result = std::use_facet<convert_facet>(sys_locale).in(
			in_state, data_from, data_from_end, data_from_next,
			data_to, data_to_end, data_to_next );
		if( result == convert_facet::ok )
		{
			std::wstring dst = data_to;
			delete[] data_to;
			return dst;
		}
		else
		{
			delete[] data_to;
			return std::wstring(L"");
		}
	}

	inline std::string WS_2_UTF8( const std::wstring& src )
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return conv.to_bytes( src );
	}

	inline  std::wstring UTF8_2_WS( const std::string& src )
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
		return conv.from_bytes( src );
	}
}

#endif