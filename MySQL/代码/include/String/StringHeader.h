#pragma once
#include <string>

//#ifdef UNICODE
//typedef std::wstring  TString;
//#else
//typedef std::string   TString;
//#endif 

#ifdef UNICODE
typedef std::basic_string<wchar_t> TString;
#else
typedef std::basic_string<char> TString;
#endif 