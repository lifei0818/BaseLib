#include "common/sys_incl.h"
#include "common/RunSqlite3.h"
#include "common/debug.h"

namespace Sqlite3Db
{

CRunSqlRead::CRunSqlRead ()
: __command (NULL)
{
}

CRunSqlRead::CRunSqlRead (const CRunSqlRead &copy) 
: __command (copy.__command)
{
	if (__command) 
	{
		// 重置通配符用
		++__command->__refs;
	}
}

CRunSqlRead::CRunSqlRead (CRunSqlCommand *cmd)
: __command (cmd)
{
	if (__command)
	{
		// 重置通配符用
		++__command->__refs;
	}
}

CRunSqlRead::~CRunSqlRead(void)
{
	Close ();
}


INT 
CRunSqlRead::Read ()
{
	if (!__command)
	{
		__command->__dbconnect.SetLastError ("reader is closed");	

		return SQLITE3_READ_ERROR;
	}

	// 真正的执行 sql 语句
	switch (sqlite3_step(__command->__stmt))
	{
	case SQLITE_ROW:    // 返回单行结果集
		return SQLITE3_READ_ROW;
	case SQLITE_DONE:   // 执行成功或者正常
		return SQLITE3_READ_DOWN;
	default:
		{
			__command->__dbconnect.SetLastError (sqlite3_errmsg(__command->__dbconnect.Conn()));
			DBGPRINT (DBG_ERROR, __command->__dbconnect.GetLastError());
			return SQLITE3_READ_ERROR;
		}
	}
}

int
CRunSqlRead::GetInt(int index) 
{
	if (index >= 0)
	{
		int iResult = 0;
		if (GetInt (index, iResult))
		{
			return iResult;
		}
	}

	return 0;
}

BOOL 
CRunSqlRead::GetInt(int index, int& nResult) 
{
	if (!__command)
	{
		__command->__dbconnect.SetLastError ("reader is closed");	

		return FALSE;
	}

	if (index > (__command->my_argc - 1)) 
	{
		__command->__dbconnect.SetLastError ("index out of range");

		return FALSE;
	}

	// 得到查询到的列号的值 (列号从 0 开始)
	nResult = sqlite3_column_int(__command->__stmt, index);
	return TRUE;
}

int
CRunSqlRead::GetByte(int index) 
{
	if (index >= 0)
	{
		char iResult = 0;
		if (GetByte (index, iResult))
		{
			return iResult;
		}
	}

	return 0;
}

BOOL 
CRunSqlRead::GetByte(int index, char& nResult) 
{
	if (!__command)
	{
		__command->__dbconnect.SetLastError ("reader is closed");	

		return FALSE;
	}

	if (index > (__command->my_argc - 1)) 
	{
		__command->__dbconnect.SetLastError ("index out of range");

		return FALSE;
	}

	// 得到查询到的列号的值 (列号从 0 开始)
	nResult = sqlite3_column_bytes(__command->__stmt, index);
	return TRUE;
}

LONGLONG 
CRunSqlRead::GetInt64(int index)
{
	if(index >= 0)
	{
		LONGLONG lResult = 0;
		if (GetInt64(index, lResult))
		{
			return lResult;
		}
	}
	return 0;
}


BOOL
CRunSqlRead::GetInt64 (int index, long long& nResult)
{
	if (!__command)
	{
		__command->__dbconnect.SetLastError ("reader is closed");	

		return FALSE;
	}

	if (index > (__command->my_argc - 1)) 
	{
		__command->__dbconnect.SetLastError ("index out of range");

		return FALSE;
	}

	nResult = sqlite3_column_int64(__command->__stmt, index);
	return TRUE;
}

BOOL
CRunSqlRead::GetDouble (int index, double& dResult)
{
	if (!__command)
	{
		__command->__dbconnect.SetLastError ("reader is closed");	

		return FALSE;
	}

	if (index > (__command->my_argc - 1)) 
	{
		__command->__dbconnect.SetLastError ("index out of range");

		return FALSE;
	}

	dResult =  sqlite3_column_double(__command->__stmt, index);
	return TRUE;
}

std::string
CRunSqlRead::GetString(int index) 
{
	if (index >= 0)
	{
		std::string strResult;
		if (GetString (index, strResult))
		{
			return strResult;
		}
	}

	return NULL_STRING;
}

BOOL
CRunSqlRead::GetString(int index, std::string& strResult) 
{
	if (!__command)
	{
		__command->__dbconnect.SetLastError ("reader is closed");	

		return FALSE;
	}

	if (index > (__command->my_argc - 1)) 
	{
		__command->__dbconnect.SetLastError ("index out of range");

		return FALSE;
	}

	strResult = "";
	const unsigned char *lpValue = sqlite3_column_text(__command->__stmt, index);
	if (lpValue)
	{
		strResult = std::string ((const char *)lpValue, sqlite3_column_bytes (__command->__stmt, index));
	}

	return TRUE;
}

BOOL 
CRunSqlRead::GetString16 (int index, std::wstring& wstrResult) 
{
	if (!__command)
	{
		__command->__dbconnect.SetLastError ("reader is closed");	

		return FALSE;
	}

	if (index > (__command->my_argc - 1)) 
	{
		__command->__dbconnect.SetLastError ("index out of range");

		return FALSE;
	}

	wstrResult = L"";
	const wchar_t* lpChar = (const wchar_t*)sqlite3_column_text16(__command->__stmt, index);
	if (lpChar)
	{
		wstrResult = std::wstring (lpChar, sqlite3_column_bytes16 (__command->__stmt, index) / 2);
	}
	
	return TRUE;
}

BOOL
CRunSqlRead::GetBlob (int index, std::string& strResult)
{
	if (!__command)
	{
		__command->__dbconnect.SetLastError ("reader is closed");	

		return FALSE;
	}

	if (index > (__command->my_argc - 1)) 
	{
		__command->__dbconnect.SetLastError ("index out of range");

		return FALSE;
	}

	strResult = "";
	const void *lpValue = sqlite3_column_blob (__command->__stmt, index);
	if (lpValue)
	{
		strResult = std::string ((const char *)lpValue, sqlite3_column_bytes (__command->__stmt, index));
	}

	return TRUE;
}

BOOL
CRunSqlRead::GetColname (int index, std::string& strResult)
{
	if (!__command)
	{
		__command->__dbconnect.SetLastError ("reader is closed");	

		return FALSE;
	}

	if (index > (__command->my_argc - 1)) 
	{
		__command->__dbconnect.SetLastError ("index out of range");

		return FALSE;
	}

	strResult = sqlite3_column_name (__command->__stmt, index);
	return TRUE;
}

BOOL
CRunSqlRead::GetColname16 (int index, std::wstring& wstrResult) 
{
	if (!__command)
	{
		__command->__dbconnect.SetLastError ("reader is closed");	

		return FALSE;
	}

	if (index > (__command->my_argc - 1)) 
	{
		__command->__dbconnect.SetLastError ("index out of range");

		return FALSE;
	}

	wstrResult = (const wchar_t*)sqlite3_column_name16 (__command->__stmt, index);
	return TRUE;
}

int
CRunSqlRead::GetColindex (std::string colname)
{
	int nResult = 0;
	if (GetColindex (colname, nResult))
	{
		return nResult;
	}

	DBGPRINT (DBG_ERROR, "错误列:" << colname);
	return -1;
}

BOOL
CRunSqlRead::GetColindex (std::string colname, int& nResult)
{
	BOOL bReTurn = FALSE;
	std::map<std::string, int>::iterator iter = colmap.find (colname);
	if (iter != colmap.end())
	{
		bReTurn = TRUE;
		nResult  = (*iter).second;
		return bReTurn;
	}

	nResult = -1;
	int nColCount = 0;
	GetColcount(nColCount);

	for (int i = 0; i <= nColCount; i++)
	{
		std::string name;
		if (!GetColname (i, name)) continue;

		colmap.insert (std::map<std::string, int>::value_type (name, i));
		if (name == colname)
		{
			bReTurn = TRUE;
			nResult = i;
		}
	}

	return bReTurn;
}

BOOL
CRunSqlRead::GetColcount(int& nResult)
{
	if (!__command)
	{
		__command->__dbconnect.SetLastError ("reader is closed");	

		return FALSE;
	}

	nResult = __command->my_argc - 1;

	return TRUE;
}

BOOL
CRunSqlRead::Reset()
{
	if (!__command)
	{
		__command->__dbconnect.SetLastError ("reader is closed");	

		return FALSE;
	}

	if (sqlite3_reset(__command->__stmt) == SQLITE_OK)
	{
		return TRUE;
	}
	else
	{
		__command->__dbconnect.SetLastError(sqlite3_errmsg(__command->__dbconnect.__db));

		return FALSE;
	}
}

CRunSqlRead& CRunSqlRead::operator=(const CRunSqlRead &copy) 
{
	this->Close();

	this->__command = copy.__command;

	if (this->__command)
	{
		++this->__command->__refs;
	}

	return *this;
}

void
CRunSqlRead::Close () 
{
	if (__command)
	{
		if (--__command->__refs == 0) 
		{
			sqlite3_reset (__command->__stmt);
		}
		__command = NULL;
	}
}

}