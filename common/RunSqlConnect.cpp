#include "common/sys_incl.h"
#include "common/RunSqlite3.h"

namespace Sqlite3Db
{

CRunSqlConnect::CRunSqlConnect(void)
: __db (NULL)
{
}

CRunSqlConnect::CRunSqlConnect (const char *db)
: __db (NULL)
{
	Open (db);
}

CRunSqlConnect::CRunSqlConnect(const wchar_t *db) 
: __db (NULL) 
{ 
	Open (db); 
}

CRunSqlConnect::~CRunSqlConnect (void)
{
	Close ();
}

BOOL 
CRunSqlConnect::Open (const char *db)
{
	if (!__db)
	{
		if (sqlite3_open(db, &__db) == SQLITE_OK)
		{
			return TRUE;
		}
		else
		{
			std::ostringstream o;
			o << "´ò¿ª " << db << " ´íÎó";

			this->SetLastError (o.str().c_str());
			return FALSE;
		}
	}

	return FALSE;
}

BOOL 
CRunSqlConnect::Open (const wchar_t *db)
{
	if (sqlite3_open16(db, &__db) == SQLITE_OK)
	{
		return TRUE;
	}
	else
	{
		this->SetLastError ("open db error!");
		return FALSE;
	}
}


VOID
CRunSqlConnect::Close ()
{
	if (__db)
	{
		sqlite3_close (__db);
		__db = NULL;
	}
}

BOOL
CRunSqlConnect::InsertId (long long& nRowId)
{
	if (!__db)
	{
		this->SetLastError ("database is not open!");
		return FALSE;
	}

	nRowId = sqlite3_last_insert_rowid(__db);
	return TRUE;
}

BOOL
CRunSqlConnect::SetBusytimeout (int ms) 
{
	if (!__db)
	{
		this->SetLastError ("database is not open!");
		return FALSE;
	}

	if (sqlite3_busy_timeout(__db, ms)
		== SQLITE_OK)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL 
CRunSqlConnect::ExecutenonQuery (const char *sql)
{
	if (!__db)
	{
		this->SetLastError ("database is not open!");
		return FALSE;
	}

	return CRunSqlCommand(*this, sql).ExecutenonQuery ();
}

BOOL
CRunSqlConnect::ExecutenonQuery (const wchar_t *sql) 
{
	if (!__db)
	{
		this->SetLastError ("database is not open!");
		return FALSE;
	}

	return CRunSqlCommand(*this, sql).ExecutenonQuery ();
}

BOOL
CRunSqlConnect::ExecuteInt (const char *sql, int& nResult)
{
	if (!__db)
	{
		this->SetLastError ("database is not open!");
		return FALSE;
	}

	return CRunSqlCommand(*this, sql).ExecuteInt (nResult);
}

BOOL
CRunSqlConnect::ExecuteInt (const wchar_t *sql, int& nResult)
{
	if (!__db)
	{
		this->SetLastError ("database is not open!");
		return FALSE;
	}

	return CRunSqlCommand(*this, sql).ExecuteInt (nResult);
}

BOOL
CRunSqlConnect::ExecuteInt64 (const char *sql, long long& nResult) 
{
	if (!__db)
	{
		this->SetLastError ("database is not open!");
		return FALSE;
	}

	return CRunSqlCommand(*this, sql).ExecuteInt64 (nResult);
}

BOOL
CRunSqlConnect::ExecuteInt64 (const wchar_t *sql, long long& nResult) 
{
	if (!__db)
	{
		this->SetLastError ("database is not open!");
		return FALSE;
	}

	return CRunSqlCommand(*this, sql).ExecuteInt64 (nResult);
}

BOOL
CRunSqlConnect::ExecuteDouble (const char *sql, double& bResult) 
{
	if (!__db)
	{
		this->SetLastError ("database is not open!");
		return FALSE;
	}

	return CRunSqlCommand(*this, sql).ExecuteDouble (bResult);
}

BOOL
CRunSqlConnect::ExecuteDouble (const wchar_t *sql, double& bResult) 
{
	if (!__db)
	{
		this->SetLastError ("database is not open!");
		return FALSE;
	}
	
	return CRunSqlCommand(*this, sql).ExecuteDouble (bResult);
}

BOOL
CRunSqlConnect::ExecuteString (const char *sql, std::string& strResult) 
{
	if (!__db)
	{
		this->SetLastError ("database is not open!");
		return FALSE;
	}

	return CRunSqlCommand(*this, sql).ExecuteString (strResult);
}

BOOL
CRunSqlConnect::ExecuteString (const wchar_t *sql, std::string& strResult)
{
	if (!__db)
	{
		this->SetLastError ("database is not open!");
		return FALSE;
	}

	return CRunSqlCommand(*this, sql).ExecuteString (strResult);
}

BOOL
CRunSqlConnect::ExecuteString16 (const char *sql, std::wstring& wstrResult)
{
	if (!__db)
	{
		this->SetLastError ("database is not open!");
		return FALSE;
	}

	return CRunSqlCommand(*this, sql).ExecuteString16 (wstrResult);
}

BOOL
CRunSqlConnect::ExecuteString16 (const wchar_t *sql, std::wstring& wstrResult) 
{
	if (!__db)
	{
		this->SetLastError ("database is not open!");
		return FALSE;
	}

	return CRunSqlCommand(*this, sql).ExecuteString16 (wstrResult);
}

BOOL
CRunSqlConnect::ExecuteBlob (const char *sql, std::string& strResult)
{
	if (!__db)
	{
		this->SetLastError ("database is not open!");
		return FALSE;
	}

	return CRunSqlCommand(*this, sql).ExecuteBlob (strResult);
}

BOOL
CRunSqlConnect::ExecuteBlob (const wchar_t *sql, std::string& strResult)
{
	if (!__db)
	{
		this->SetLastError ("database is not open!");
		return FALSE;
	}

	return CRunSqlCommand(*this, sql).ExecuteBlob (strResult);
}

}