#pragma once

#ifdef MYSQL_EXPORTS
#define MYSQL_API __declspec(dllexport)
#else
#define MYSQL_API __declspec(dllimport)
#endif