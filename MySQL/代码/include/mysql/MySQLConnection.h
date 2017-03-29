#pragma once

#include<mysql_connection.h>
#include<mysql_driver.h>
#include<cppconn/exception.h>
#include<cppconn/driver.h>
#include<cppconn/connection.h>
#include<cppconn/resultset.h>
#include<cppconn/prepared_statement.h>
#include<cppconn/statement.h>

#include <vector>
#include <string>
#include <codecvt>
#include "StringHeader.h"
#include "StringTool.hpp"

#pragma comment(lib,"mysqlcppconn.lib")



//适用于UTF-8编码的数据库,手动提交模式
class CMySQLConnection
{
public:
	CMySQLConnection(void);
	~CMySQLConnection(void);
private:
	sql::Connection *m_pConn;
	std::string          m_strError;

public:
	/*
	 Method:    GetErrorMessage
	 FullName:  CMySQLConnection::GetErrorMessage 获取MYSQL 执行错误 信息
	 Access:    public 
	 Returns:   sql::SQLString 错误 信息
	*/
	TString GetErrorMessage()
	{
		std::string strErr = m_strError;
		m_strError.clear();

		TString str;
#ifdef UNICODE
		TCHAR buf[512] = {0};
		StringTool::MByteToWChar(strErr.c_str(),buf,512);
		str = buf;
#else
		str = strErr;
#endif
		return str;
	}

	/*
	 Method:    Connecte
	 FullName:  CMySQLConnection::Connecte
	 Access:    public 
	 Returns:   bool 连接成功返回ture
	 Parameter: sql::SQLString strUrl MYSQL地址
	 Parameter: sql::SQLString strUserName 用户名
	 Parameter: sql::SQLString strPwd 密码
	*/
	bool Connecte(std::string strUrl,std::string  DBName,std::string  strUserName,std::string  strPwd);

	/*
	 Method:    Execute
	 FullName:  CMySQLConnection::Execute 执行增，删，改操作
	 Access:    public 
	 Returns:   bool 执行成功返回ture
	 Parameter: const sql::SQLString & strSQL SQL操作语句
	*/
	bool Execute(const std::string  &strSQL);

    /*
     Method:    ExecuteQuery
     FullName:  CMySQLConnection::ExecuteQuery,执行查询操作
     Access:    public 
     Returns:   bool 查询成功返回true
     Qualifier:
     Parameter: const sql::SQLString & strSQL ,查询语句
     Parameter: vector<string> & vecResult  查询结果集保存数组
    */
    bool ExecuteQuery(const std::string  &strSQL,std::vector<std::vector<std::wstring>> &vecResult);

	/*
     Method:    ExecuteQuery
     FullName:  CMySQLConnection::ExecuteQuery,执行查询操作
     Access:    public 
     Returns:   std::shared_ptr<sql::ResultSet> 
     Qualifier:
     Parameter: const sql::SQLString & strSQL ,查询语句
     Parameter: vector<string> & vecResult  查询结果集指针
    */
    boost::shared_ptr<sql::ResultSet> ExecuteQuery(const std::string  &strSQL);


	/*
     Method:    GetConnection
     FullName:  CMySQLConnection::GetConnection,获取MySQL Connector/C++连接原始指针
     Access:    public 
     Returns:   sql::Connection * 
    */
	sql::Connection *GetConnection()
	{
		return m_pConn;
	}

	void Commit()
	{
		m_pConn->commit();
	}
	void Rollback()
	{
		m_pConn->rollback();
	}
};

