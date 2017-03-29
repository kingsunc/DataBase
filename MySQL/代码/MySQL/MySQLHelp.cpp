#include "StdAfx.h"
#include "MySQLHelp.h"

#define VERIFY_NULL(strTemp)	(	(!strTemp.empty()) ? fmt::format("'{}'", strTemp) : "NULL"	)

CMySQLHelp::CMySQLHelp()
{
	// 连接本机测试()
	m_myConnect.Connecte("127.0.0.1:3306","test_mysql","root","123456");
}

CMySQLHelp::~CMySQLHelp()
{
}

bool CMySQLHelp::Execute(const std::string &strSQL)
{
	if(!m_myConnect.Execute(strSQL))
	{
	/*	CString strError = m_myConnect.GetErrorMessage().c_str();
		LOG_ERROR(strError);

#ifdef _DEBUG
		AfxERPMessageBox( NULL, strError);
#endif*/
		return false;
	}

	return true;
}

bool CMySQLHelp::ExecuteQuery(const std::string &strSQL, MySQLData &vecResult)
{

	if(!m_myConnect.ExecuteQuery(strSQL, vecResult))
	{
		/*CString strError = m_myConnect.GetErrorMessage().c_str();
		LOG_ERROR(strError);

#ifdef _DEBUG
		AfxERPMessageBox( NULL, strError);
#endif*/
		return false;
	}

	return true;
}

TString CMySQLHelp::GetLastError()
{
	return m_myConnect.GetErrorMessage(); 
}

void CMySQLHelp::Commit()
{
	m_myConnect.Commit();
}

void CMySQLHelp::Rollback()
{
	m_myConnect.Rollback();
}

// 用户信息;
bool CMySQLHelp::InsertUserInfo(IN UserInfo& userInfo)
{
	std::string strSQL = fmt::format("insert into user_info(Name, Address) value ('{}', '{}')", 
		WS_2_S(userInfo.strName), WS_2_S(userInfo.strAddress) );
	bool bResult = Execute(strSQL);
	return bResult;
}

bool CMySQLHelp::SelectUserInfo(IN std::vector<UserInfo>& vecUserInfo, 
	IN TString strID,
	IN TString strName )
{
	vecUserInfo.clear();

	std::string strSQL = fmt::format("select Id, Name, Address from user_info where 1=1 ");
	if (!strID.empty())
	{
		strSQL = strSQL + fmt::format(" and Id={} ", WS_2_S(strID));
	}
	if (!strName.empty())
	{
		strSQL = strSQL + fmt::format(" and Name='{}' ", WS_2_S(strName));
	}

	MySQLData vecResult;
	bool bResult = ExecuteQuery(strSQL, vecResult);
	if(bResult)
	{
		for (int i = 0; i < TypeToInt(vecResult.size()); i++)
		{
			UserInfo tempUserInfo;
			int iCol = 0;
			tempUserInfo.strID		= vecResult[i][iCol++];
			tempUserInfo.strName	= vecResult[i][iCol++];
			tempUserInfo.strID		= vecResult[i][iCol++];

			vecUserInfo.push_back(tempUserInfo);
		}
	}
	return bResult;
}

bool CMySQLHelp::UpdateUserInfo(IN UserInfo& userInfo)
{
	std::string strSQL = fmt::format("update user_info set Name='{}', Address='{}' where Id={} ", 
		WS_2_S(userInfo.strName), WS_2_S(userInfo.strAddress), WS_2_S(userInfo.strID));
	bool bResult = Execute(strSQL);
	return bResult;
}

bool CMySQLHelp::DeleteUserInfo(IN TString strID)
{
	std::string strSQL = fmt::format("delete from user_info where id={}", WS_2_S(strID) );
	bool bResult = Execute(strSQL);
	return bResult;
}