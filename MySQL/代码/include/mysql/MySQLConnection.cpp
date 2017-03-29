#include "StdAfx.h"
#include "MySQLConnection.h"
#include <format.h>

CMySQLConnection::CMySQLConnection(void)
{
	m_pConn = nullptr;
}

CMySQLConnection::~CMySQLConnection(void)
{

	if (m_pConn)
	{
		if (!m_pConn->isClosed())
		{
			m_pConn->close();
		}
		delete m_pConn;
	}
}

bool CMySQLConnection::Connecte(std::string  strUrl,std::string  DBName,std::string  strUserName,std::string strPwd)
{
	try
	{
		sql::Driver *driver;
		driver = sql::mysql::get_mysql_driver_instance();
		m_pConn = driver->connect(strUrl.c_str(),strUserName.c_str(),strPwd.c_str());
		m_pConn->setSchema(DBName.c_str());//设置数据库
	}
	catch (sql::SQLException &e)
	{
		m_strError = fmt::format("数据库连接失败! ErrorCode:{},SQLState:{}",e.getErrorCode(),e.what());
		return false;
	}
	catch(std::runtime_error &e)
	{
		m_strError = e.what();
		return false;
	}
	
	if (m_pConn->isClosed())
	{
		return false;
	}

	m_pConn->setAutoCommit(false);//设置为手动提交模式
	return true;
}

bool CMySQLConnection::Execute(const std::string  &strSQL)
{
	if (!m_pConn)
		return false;
	if (m_pConn->isClosed())
	{
		return false;
	}
	//sql::Statement *stmt = nullptr;
	boost::shared_ptr<sql::Statement> pStmt;
	if (strSQL.length() == 0)
	{
		return false;
	}

	try
	{
		std::wstring wstrSQL = StringTool::S_2_WS(strSQL);
		std::string  strUTF8 = StringTool::WS_2_UTF8(wstrSQL);
		pStmt.reset(m_pConn->createStatement());
		pStmt->execute(strUTF8.c_str());
	}
	catch (sql::SQLException &e)
	{
		m_strError = fmt::format("Excute failed! ErrorCode:{},SQLState:{},SQL:{}",e.getErrorCode(),e.what(),strSQL.c_str());
		return false;
	}
	catch (std::runtime_error &e)
	{
		m_strError = e.what();
		return false;
	}
	
	return true;
}

bool CMySQLConnection::ExecuteQuery(const std::string  &strSQL,std::vector<std::vector<std::wstring>> &vecResult)
{
	if (!m_pConn)
		return false;
	if (m_pConn->isClosed())
	{
		return false;
	}
	if (strSQL.length() == 0)
	{
		return false;
	}

	vecResult.clear();

	boost::shared_ptr<sql::Statement> pStmt;
	boost::shared_ptr<sql::ResultSet> pRes;
	boost::scoped_ptr<int> pArrType;

	try
	{
		std::wstring wstrSQL = StringTool::S_2_WS(strSQL);
		std::string  strUTF8 = StringTool::WS_2_UTF8(wstrSQL);

		pStmt.reset(m_pConn->createStatement());
		pRes.reset(pStmt->executeQuery(strUTF8.c_str()));

		size_t iRowCount = pRes->rowsCount();//多少行
		size_t iCulCount = pRes->getMetaData()->getColumnCount();//列

		/*pArrType.reset(new int[iCulCount]);

		for (size_t i = 1;i<iCulCount+1;i++)//获取列的数据类型
		{
			int type = pRes->getMetaData()->getColumnType(i);
			(pArrType.get())[i-1] = type;
		}*/
		std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
		std::vector<std::wstring> vecRow;
		std::wstring strValue;


		while (pRes->next()) 
		{
			//std::vector<std::wstring> vecRow;
			vecRow.clear();
			for (size_t n = 1;n<iCulCount+1;n++)
			{
				//std::wstring strValue;
				strValue.clear();
				//int type = pRes->getMetaData()->getColumnType(n);
				//if (type != sql::DataType::SQLNULL)
				//{
					
					strValue =  conv.from_bytes( pRes->getString(n).c_str() );
				//}
				
				 /*switch((pArrType.get())[n-1])
				 {
				 case sql::DataType::BIGINT:
				 case  sql::DataType::INTEGER :
					 strValue = fmt::format(L"{}",pRes->getString(n).c_str());
					 break;
				 case  sql::DataType::DOUBLE :
					 strValue = fmt::format(L"{}",pRes->getDouble(n));
					 break;
				 case  sql::DataType::VARCHAR :
					 {
						 //strValue = pRes->getString(n).c_str();
						 std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
						 strValue =  conv.from_bytes( pRes->getString(n).c_str() );
					 }
					 break;
				 case sql::DataType::SQLNULL:
					 break;
				 default:
					 {
						 strValue = fmt::format(L"{}",pRes->getString(n).c_str());
					 }
					 break;
				 }*/
				vecRow.push_back(strValue);//一列
			}

			vecResult.push_back(vecRow);//一行
		}

	}
	catch (sql::SQLException &e)
	{
		m_strError = fmt::format("ExcuteQuery failed! ErrorCode:{},SQLState:{},SQL:{}",e.getErrorCode(),e.what(),strSQL.c_str());
		return false;
	}
	catch (std::exception &e)
	{
		m_strError = e.what();
		return false;
	}


	return true;

}

boost::shared_ptr<sql::ResultSet> CMySQLConnection::ExecuteQuery(const std::string  &strSQL)
{
	if (!m_pConn)
		return false;
	if (m_pConn->isClosed())
	{
		return false;
	}
	if (strSQL.length() == 0)
	{
		return nullptr;
	}
	boost::shared_ptr<sql::Statement> pStmt;
	boost::shared_ptr<sql::ResultSet> pRes;

	try
	{
		pStmt.reset(m_pConn->createStatement());
		pRes.reset(pStmt->executeQuery(strSQL));
	}
	catch (sql::SQLException &e)
	{
		m_strError = fmt::format("ExcuteQuery failed! ErrorCode:{},SQLState:",e.getErrorCode(),e.what());
		return nullptr;
	}
	catch (std::exception & e)
	{
		m_strError = e.what();
		return nullptr;
	}

	return pRes;
}
