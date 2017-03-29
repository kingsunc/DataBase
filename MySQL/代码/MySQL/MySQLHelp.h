#pragma once
#include "MySQLExport.h"
#include "MySQLConnection.h"
#include "StringTool.h"

typedef std::vector<std::vector<TString>> MySQLData;

class MYSQL_API UserInfo
{
public:

	TString strID;
	TString strName;
	TString strAddress;
};

class MYSQL_API CMySQLHelp
{
public:
	CMySQLHelp();
	~CMySQLHelp();

public:
	// 用户信息;
	bool InsertUserInfo(IN UserInfo& userInfo);
	bool SelectUserInfo(IN std::vector<UserInfo>& vecUserInfo, 
		IN TString strID = L"",
		IN TString strName = L"" );
	bool UpdateUserInfo(IN UserInfo& userInfo);
	bool DeleteUserInfo(IN TString strID);

public:
	// 执行;
	void Commit();
	// 撤销;
	void Rollback();

protected:
	// 执行操作SQL，增加、修改、删除;
	bool Execute(const std::string  &strSQL);

	// 执行查询SQL，查询结果集保存在vecResult中;
	bool ExecuteQuery(const std::string  &strSQL, MySQLData &vecResult);

	// 获取错误信息;
	TString GetLastError();

private:

	CMySQLConnection m_myConnect;
};

