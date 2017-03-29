#pragma once
#include "MySQLExport.h"
#include <WinSock2.h>
#include <vector>
#include "mysql.h"

#pragma comment(lib,"libmysql.lib")

#ifndef BUF_SIZE
#define BUF_SIZE	(1024*8)
#endif

// 适用于UTF-8编码的数据库,手动提交模式;
class MYSQL_API CMySQLConnection
{
public:
	CMySQLConnection();
	~CMySQLConnection();

public:

	/*--------------------------------------------------------------------
	 Method:    Connecte
	 FullName:  CMySQLConnection::Connecte
	 Access:    public 
	 Returns:   bool 连接成功返回ture;
	 Parameter: string strUrl MYSQL地址
	 Parameter: string strUserName 用户名;
	 Parameter: string strPwd 密码;
	---------------------------------------------------------------------*/
	bool Connecte(std::string strUrl,std::string  DBName,std::string  strUserName,std::string  strPwd);

	/*--------------------------------------------------------------------
	 Method:    Execute
	 FullName:  CMySQLConnection::Execute 执行增，删，改操作;
	 Access:    public 
	 Returns:   bool 执行成功返回ture;
	 Parameter: const string & strSQL SQL操作语句;
	---------------------------------------------------------------------*/
	bool Execute(const std::string  &strSQL);

	/*--------------------------------------------------------------------
	Method:    ExecuteQuery
	FullName:  CMySQLConnection::ExecuteQuery,执行查询操作
	Access:    public 
	Returns:   bool 查询成功返回true;
	Qualifier:
	Parameter: const sql::SQLString & strSQL ,查询语句
	Parameter: vector<string> & vecResult  查询结果集保存数组;
	---------------------------------------------------------------------*/
	bool ExecuteQuery(const std::string  &strSQL,std::vector<std::vector<std::wstring>> &vecResult);

	// 二进制读写;
	void ExecuteBlod(std::string fileName);
	void Blod2(char *file);

	/*--------------------------------------------------------------------
	 Method:	GetErrorMessage
	 FullName:	CMySQLConnection::GetErrorMessage 获取MYSQL 执行错误 信息
	 Access:	public 
	 Returns:	string错误信息;
	--------------------------------------------------------------------*/
	std::wstring GetErrorMessage();

	/*--------------------------------------------------------------------
	 Method:    GetErrorCode
	 FullName:  CMySQLConnection::GetErrorCode 获取MYSQL 执行错误代码
	 Access:    public 
	 Returns:   string 错误代码;
	--------------------------------------------------------------------*/
	int GetErrorCode();

	/*--------------------------------------------------------------------
	Method:    GetConnection
	FullName:  CMySQLConnection::GetConnection,获取MySQL Connector/C++连接原始指针
	Access:    public 
	Returns:   sql::Connection
	---------------------------------------------------------------------*/
	MYSQL *GetConnection();

	// 执行;
	void Commit();
	// 撤销;
	void Rollback();

private:
	MYSQL			m_conn;
	bool			m_bConn;
	std::string		m_strError;
	int				m_nErrorNo;
	// dll导出模版时报警告，暂时忽略;
};