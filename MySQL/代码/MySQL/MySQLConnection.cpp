#include "StdAfx.h"
#include "MySQLConnection.h"
//#include <boost/scoped_array.hpp>
#include <fstream>

CMySQLConnection::CMySQLConnection()
{
	mysql_library_init(0, nullptr, nullptr);
	m_bConn = false;
}

CMySQLConnection::~CMySQLConnection()
{
	if (m_bConn)
	{
		mysql_close(&m_conn);
	}
	mysql_library_end();
}

bool CMySQLConnection::Connecte(std::string  strUrl,std::string  DBName,std::string  strUserName,std::string strPwd)
{
	if (NULL == mysql_init(&m_conn)) 
	{
		m_strError = "数据库初变量始化失败!";
		return false;
	} 

	int nPos = strUrl.find(":");
	std::string strIp = strUrl.substr(0,nPos);
	std::string strPort = strUrl.substr(nPos+1);

	if (mysql_real_connect(&m_conn, strIp.c_str(), strUserName.c_str(), strPwd.c_str(), DBName.c_str(), atoi(strPort.c_str()), NULL, 0) == nullptr)
	{
	   int errCode = mysql_errno(&m_conn);
	   const char *err = mysql_error(&m_conn);

	   char errBuf[512] = {0};
	   sprintf_s(errBuf,512,"连接数据库失败!错误码:%d,错误描述:%s",errCode,err);
	   m_strError = errBuf;
	   return false;
	}

	if (mysql_query(&m_conn, "set names utf8") != 0 )
	{
		mysql_close(&m_conn);
		int errCode = mysql_errno(&m_conn);
		const char *err = mysql_error(&m_conn);

		char errBuf[512] = {0};
		sprintf_s(errBuf,512,"设置据库字符集失败!错误码:%d,错误描述:%s",errCode,err);
		m_strError = errBuf;
		return false;
	}
	
	if (mysql_query(&m_conn,"BEGIN") != 0)	//事物;
	{
		mysql_close(&m_conn);
		int errCode = mysql_errno(&m_conn);
		const char *err = mysql_error(&m_conn);

		char errBuf[512] = {0};
		sprintf_s(errBuf,512,"设置据库事物失败!错误码:%d,错误描述:%s",errCode,err);
		m_strError = errBuf;
		return false;
	}
	
	m_bConn = true;

	return true;
}

bool CMySQLConnection::Execute(const std::string  &strSQL)
{
	if (!m_bConn)
	{
		return false;
	}

	if (strSQL.length() == 0)
	{
		return false;
	}

	std::wstring wstrSQL = S_2_WS(strSQL);
	std::string  strUTF8 = WS_2_UTF8(wstrSQL);

	if (mysql_real_query(&m_conn,strUTF8.c_str(),strUTF8.length()) != 0)
	{

		int errCode = mysql_errno(&m_conn);
		const char *err = mysql_error(&m_conn);

		char errBuf[2 * 1024] = {0};
		sprintf_s(errBuf,2 * 1024,"执行SQL语句失败!错误码:%d,错误描述:%s，SQL语句:%s", errCode, err, strSQL.c_str());
		m_strError = errBuf;
		m_nErrorNo = errCode;
		return false;

	}
	return true;
}

bool CMySQLConnection::ExecuteQuery(const std::string  &strSQL,std::vector<std::vector<std::wstring>> &vecResult)
{
	if (!m_bConn)
	{
		return false;
	}

	if (strSQL.length() == 0)
	{
		return false;
	}
	vecResult.clear();

	std::wstring wstrSQL = S_2_WS(strSQL);
	std::string  strUTF8 = WS_2_UTF8(wstrSQL);

	if (mysql_real_query(&m_conn,strUTF8.c_str(),strUTF8.length()) != 0 )
	{
		int errCode = mysql_errno(&m_conn);
		const char *err = mysql_error(&m_conn);

		char errBuf[2048] = {0};
		sprintf_s(errBuf,2048,"执行查询失败!错误码:%d,错误描述:%s，SQL语句:%s", errCode, err, strSQL.c_str());
		m_strError = errBuf;
		return false;
	}

	MYSQL_RES *res = mysql_store_result(&m_conn);	//将结果保存在res结构体中;
	if(res == nullptr)
	{
		int errCode = mysql_errno(&m_conn);
		const char *err = mysql_error(&m_conn);

		char errBuf[2048] = {0};
		sprintf_s(errBuf,2048,"获取结果集失败!错误码:%d,错误描述:%s，SQL语句:%s", errCode, err, strSQL.c_str());
		m_strError = errBuf;

		return false;
	}

	my_ulonglong nRow = mysql_num_rows(res);	// 行数;
	auto nCol = mysql_num_fields(res);			// 列数;

	if (nRow == 0)
	{
		return true;
	}

	MYSQL_ROW row;
	std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
	std::vector<std::wstring> vecRow;
	vecRow.resize(nCol);
	vecResult.resize((unsigned int)nRow);
	std::wstring strValue;

	int nIndex = 0;
	while( (row = mysql_fetch_row(res)) )
	{
		for (unsigned int i = 0; i < nCol; i++) 
		{
			char *pfield =  row[i];
			strValue.clear();

			if (pfield)
			{
				strValue =  conv.from_bytes(pfield);
			}
			vecRow[i] = strValue;
		}
		vecResult[nIndex] = vecRow;	//一行;
		nIndex++;
	}

	mysql_free_result(res);

	return true;
}

// 二进制
void CMySQLConnection::ExecuteBlod(std::string fileName)
{
	//MYSQL_BIND bind;
	//MYSQL_STMT *stmt = mysql_stmt_init(&m_conn);
	//if (stmt == nullptr)
	//{
	//	return;
	//}

	//std::string strSQL = "update userinfo set photo=? where id=4";
	//if ( mysql_stmt_prepare(stmt, strSQL.c_str(),strSQL.length() ))
	//{
	//	//fprintf(stderr, "\n mysql_stmt_prepare(), INSERT failed");
	//	//fprintf(stderr, "\n %s", mysql_stmt_error(stmt));
	//	return;
	//}

	//struct stat file_stat;
	//stat(fileName.c_str(),&file_stat);
	//long fileLen = file_stat.st_size;

	//boost::scoped_array<char> fileBuf(new char[fileLen + 1]);
	////char *fileBuf = new char[fileLen + 1];

	//std::ifstream rFile;

	//rFile.open(fileName.c_str(),std::ios::_Nocreate|std::ios::binary);
	//if (!rFile.is_open())
	//{
	//	return;
	//}
	//rFile.read(fileBuf.get(),fileLen);

	//memset(&bind, 0, sizeof(bind));
	//bind.buffer_type = MYSQL_TYPE_MEDIUM_BLOB;
	//bind.buffer = fileBuf.get();
	////bind.is_null= nullptr;
	//bind.buffer_length = fileLen;

	//if (mysql_stmt_bind_param(stmt, &bind))
	//{
	//	return;
	//}

	//if (mysql_stmt_execute(stmt))
	//{
	//	//fprintf(stderr, "\n mysql_stmt_execute failed");
	//	//fprintf(stderr, "\n %s", mysql_stmt_error(stmt));
	//
	//}

	//Commit();
}

void CMySQLConnection::Blod2(char *file)
{
	std::string strSQL = "select photo from userinfo where id = 4";
	if (mysql_real_query(&m_conn,strSQL.c_str(),strSQL.length()) != 0 )
	{
		return ;
	}

	MYSQL_RES *res = mysql_store_result(&m_conn);	//将结果保存在res结构体中;
	if(res == nullptr)
	{
	   return ;
	}

	my_ulonglong nRow = mysql_num_rows(res);// 行数;
	auto nCol = mysql_num_fields(res);		// 列数;

	MYSQL_ROW row;
	std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
	std::vector<std::wstring> vecRow;
	std::wstring strValue;

	row = mysql_fetch_row(res);
	if (row == nullptr)
	{
		return;
	}
	unsigned long len = * (mysql_fetch_lengths(res));

	std::ofstream out(file,std::ios::binary|std::ios::trunc|std::ios::out);  
	if (!out.is_open())
	{
		return ;
	}

	out.write(row[0],len);
	out.close();
}

std::wstring CMySQLConnection::GetErrorMessage()
{
	std::string strErr = m_strError;
	m_strError.clear();

	return S_2_WS(strErr);
}

int CMySQLConnection::GetErrorCode()
{
	int nCode = m_nErrorNo;
	m_nErrorNo = 0;

	return nCode;
}

MYSQL * CMySQLConnection::GetConnection()
{
	return &m_conn;
}

void CMySQLConnection::Commit()
{
	mysql_commit(&m_conn);
}

void CMySQLConnection::Rollback()
{
	mysql_rollback(&m_conn);
}
