// stdafx.h : 标准系统包含文件的包含文件;
// 或是经常使用但不常更改的;
// 特定于项目的包含文件;

#pragma once

#define WIN32_LEAN_AND_MEAN		// 从 Windows 头文件中排除极少使用的信息;
// Windows 头文件;
#include <windows.h>

#include "StringTool.h"
using namespace StringTool;
#include "INI.h"
#include "format.h"

extern TString g_strDirPath;

// TypeX->TypeY 减少类型转换的警告;
#define TypeToInt(tValue)				(int(tValue))
#define TypeToFloat(tValue)				(float(tValue))
#define TypeToDouble(tValue)			(double(tValue))