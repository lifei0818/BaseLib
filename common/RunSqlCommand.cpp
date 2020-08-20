#include "common/sys_incl.h"
#include "common/RunSqlite3.h"
#include "common/debug.h"
#include "common/tolists.h"

namespace Sqlite3Db
{

CRunSqlCommand::CRunSqlCommand (CRunSqlConnect& m_dbconn,
								const char* sql)
: __dbconnect (m_dbconn)
, __stmt (NULL)
, __isexiterror (FALSE)
, __refs (0)
, my_argc (0)
{
	if (INVALID_PARAM_LPSTR (sql))
	{
		__dbconnect.SetLastError ("执行的 sql 语句为空!");
		__isexiterror = TRUE;		
		return;
	}	

	const char *tail = NULL;

	// 执行 sql 语句
	if (sqlite3_prepare(
		__dbconnect.Conn(),
		sql,
		-1,
		&__stmt,
		&tail) == SQLITE_OK)
	{
		// 最近一次操作影响到的行数 查询到的行数
		my_argc = sqlite3_column_count(__stmt);
	}
	else
	{
		// 有异常发生
		__dbconnect.SetLastError (sqlite3_errmsg(__dbconnect.Conn()));
		DBGPRINT (DBG_ERROR, __dbconnect.GetLastError() << " SQL:" << sql);
		__isexiterror = TRUE;
	}	
}

CRunSqlCommand::CRunSqlCommand (CRunSqlConnect &m_dbconn,
								const wchar_t *sql) 
: __dbconnect (m_dbconn)
, __stmt (NULL)
, __isexiterror (FALSE)
, __refs (0)
, my_argc (0)
{
	const wchar_t *tail = NULL;

	if (sqlite3_prepare16(
		__dbconnect.Conn(),
		sql,
		-1,
		&__stmt,
		(const void**)&tail) == SQLITE_OK
		)
	{
		my_argc = sqlite3_column_count(__stmt);
	}
	else
	{
		// 有异常发生
		__dbconnect.SetLastError (sqlite3_errmsg(__dbconnect.Conn()));
		DBGPRINT (DBG_ERROR, __dbconnect.GetLastError() << " SQL:" << sql);
		__isexiterror = TRUE;
	}
}

CRunSqlCommand::~CRunSqlCommand (void)
{
	if (__stmt) sqlite3_finalize(__stmt);
}

CRunSqlRead 
CRunSqlCommand::ExecuteReader ()
{
	return CRunSqlRead(this);
}

BOOL
CRunSqlCommand::ExecutenonQuery ()
{
	int nReadResult = 0;
	nReadResult = ExecuteReader().Read();

	if (nReadResult == SQLITE3_READ_ROW
		|| nReadResult == SQLITE3_READ_DOWN)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL 
CRunSqlCommand::Bind (int index)
{
	if (sqlite3_bind_null(__stmt, index) 
		== SQLITE_OK)
	{
		return TRUE;
	}
	else
	{
		// 有异常发生
		__dbconnect.SetLastError (sqlite3_errmsg(__dbconnect.__db));
		return FALSE;
	}
}

BOOL 
CRunSqlCommand::Bind (int index, int data) 
{
	if (sqlite3_bind_int (__stmt, index, data)
		== SQLITE_OK)
	{
		return TRUE;
	}
	else
	{
		// 有异常发生
		__dbconnect.SetLastError (sqlite3_errmsg(__dbconnect.__db));
		return FALSE;
	}
}

BOOL
CRunSqlCommand::Bind (int index, long long data) 
{
	if (sqlite3_bind_int64 (__stmt, index, data) 
		== SQLITE_OK)
	{
		return TRUE;
	}
	else
	{
		// 有异常发生
		__dbconnect.SetLastError (sqlite3_errmsg(__dbconnect.__db));
		return FALSE;
	}
}

BOOL
CRunSqlCommand::Bind (int index, double data)
{
	if (sqlite3_bind_double (__stmt, index, data) 
		== SQLITE_OK)
	{
		return TRUE;
	}
	else
	{
		// 有异常发生
		__dbconnect.SetLastError (sqlite3_errmsg(__dbconnect.__db));
		return FALSE;
	}
}

BOOL
CRunSqlCommand::Bind (int index, const char *data, int datalen)
{
	if (data && datalen > 0)
	{
		/// 处理异常字符，如：单引号
		std::string strNewData = data;
		BaseLib::StringReplaceAll(strNewData, "'", "''");

		if (sqlite3_bind_text (__stmt, index, strNewData.c_str(), strNewData.length(), SQLITE_TRANSIENT) 
			== SQLITE_OK)
		{
			return TRUE;
		}
		else
		{
			// 有异常发生
			__dbconnect.SetLastError (sqlite3_errmsg(__dbconnect.__db));
		}
	}
	return FALSE;
}

BOOL
CRunSqlCommand::Bind (int index, const wchar_t *data, int datalen)
{
	if (data && datalen > 0)
	{
		std::wstring strNewData = data;
		BaseLib::StringReplaceAll(strNewData, L"'", L"''");

		if (sqlite3_bind_text16 (__stmt, index, data, datalen, SQLITE_TRANSIENT)
			== SQLITE_OK)
		{
			return TRUE;
		}
		else
		{
			// 有异常发生
			__dbconnect.SetLastError (sqlite3_errmsg(__dbconnect.__db));	
		}
	}

	return FALSE;
}

BOOL
CRunSqlCommand::Bind (int index, const void *data, int datalen) 
{
	if (sqlite3_bind_blob (__stmt, index, data, datalen, SQLITE_TRANSIENT) 
		== SQLITE_OK)
	{
		return TRUE;
	}
	else
	{
		// 有异常发生
		__dbconnect.SetLastError (sqlite3_errmsg(__dbconnect.__db));
		return FALSE;
	}
}

BOOL
CRunSqlCommand::Bind (int index, const std::string &data)
{
	return Bind(index, data.c_str(), data.length());
}

BOOL
CRunSqlCommand::Bind (int index, const std::wstring &data)
{
	return Bind(index, data.c_str(), data.length());
}

BOOL
CRunSqlCommand::ExecuteInt (int& nResult) 
{
	CRunSqlRead reader = ExecuteReader();

	if (reader.Read() != SQLITE3_READ_ROW) 
	{
		return FALSE;
	}

	return reader.GetInt(0, nResult);
}

BOOL
CRunSqlCommand::ExecuteInt64 (long long& nResult) 
{
	CRunSqlRead reader = ExecuteReader();

	if (reader.Read() != SQLITE3_READ_ROW) 
	{
		return FALSE;
	}

	return  reader.GetInt64 (0, nResult);
}

BOOL
CRunSqlCommand::ExecuteDouble (double& bResult)
{
	CRunSqlRead reader = ExecuteReader();

	if (reader.Read() != SQLITE3_READ_ROW) 
	{
		return FALSE;
	}

	return reader.GetDouble (0, bResult);
}

BOOL
CRunSqlCommand::ExecuteString (std::string& strResult) 
{
	CRunSqlRead reader = ExecuteReader();

	if (reader.Read() != SQLITE3_READ_ROW) 
	{
		return FALSE;
	}

	return reader.GetString (0, strResult);
}

BOOL
CRunSqlCommand::ExecuteString16(std::wstring& wstrResult)
{
	CRunSqlRead reader = ExecuteReader();

	if (reader.Read() != SQLITE3_READ_ROW) 
	{
		return FALSE;
	}

	return reader.GetString16 (0, wstrResult);
}

BOOL
CRunSqlCommand::ExecuteBlob (std::string& strResult)
{
	CRunSqlRead reader = ExecuteReader();

	if (reader.Read() != SQLITE3_READ_ROW) 
	{
		return FALSE;
	}

	return reader.GetBlob (0, strResult);
}

}