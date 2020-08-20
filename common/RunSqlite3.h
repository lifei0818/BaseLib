/*----------------------------------------------------------------------------*
* RunSqlite3.h 
*
* Data:             2010.8.23
*
* Company:          YuanWang
* 
* Copyright:        www.sqlite3.org
*
* Describe:         管理 sqlite3 数据库操作的类定义
*
*----------------------------------------------------------------------------*/
#ifndef __baselib__runsqlite3__
#define __baselib__runsqlite3__

#include "common/sys_incl.h"
#include "sqlite3/sqlite3.h"

/// ERROR CODES
#define SQLITE3_READ_ROW				 (DWORD)1		///< 执行 sql 语句时读到数据
#define SQLITE3_READ_DOWN				 (DWORD)0		///< 执行 sql 语句时成功完成
#define SQLITE3_READ_ERROR				 (DWORD)-1		///< 执行 sql 语句时发生错误

namespace Sqlite3Db
{

class CRunSqlCommand;
class CRunSqlRead;

class CRunSqlConnect
{
public:
	CRunSqlConnect (void);
	CRunSqlConnect (const char *db);
	CRunSqlConnect (const wchar_t *db); 
	~CRunSqlConnect (void);

	// 打开数据库
	BOOL Open (const char *db);
	BOOL Open (const wchar_t *db);
	// 关闭数据库
	VOID Close ();
	// 最后一个插入的 RowID
	BOOL InsertId (long long& nRowId);
	// 设置忙等待时间
	BOOL SetBusytimeout (int ms);

	// 执行 sql 语句
	BOOL ExecutenonQuery (const char *sql);
	BOOL ExecutenonQuery (const wchar_t *sql);

	BOOL ExecuteInt (const char *sql, int& nResult);
	BOOL ExecuteInt (const wchar_t *sql, int& nResult);

	BOOL ExecuteInt64 (const char *sql, long long& nResult);
	BOOL ExecuteInt64 (const wchar_t *sql, long long& nResult);

	BOOL ExecuteDouble (const char *sql, double& bResult);
	BOOL ExecuteDouble (const wchar_t *sql, double& bResult);

	BOOL ExecuteString (const char *sql, std::string& strResult);
	BOOL ExecuteString (const wchar_t *sql, std::string& strResult);

	BOOL ExecuteString16 (const char *sql, std::wstring& wstrResult);
	BOOL ExecuteString16 (const wchar_t *sql, std::wstring& wstrResult);

	BOOL ExecuteBlob (const char *sql, std::string& strResult);
	BOOL ExecuteBlob (const wchar_t *sql, std::string& strResult);

	std::string& GetLastError () { return __strErrMsg; }
	
	struct sqlite3* Conn() { return __db; }
private:
	// 错误信息
	void SetLastError (const char * lpErrorCode) { if (lpErrorCode) __strErrMsg = lpErrorCode; }	
private:
	friend class CRunSqlCommand;
	friend class CRunSqlRead;

	std::string __strErrMsg;
	struct sqlite3 *__db;
};

class CRunSqlCommand
{
public:
	CRunSqlCommand (CRunSqlConnect& m_dbconn, const char* sql);
	CRunSqlCommand (CRunSqlConnect& m_dbconn, const wchar_t *sql);

	~CRunSqlCommand (void);

	// 得到 sql 语句的查询对象
	CRunSqlRead ExecuteReader ();
	// 执行 sql 语句
	BOOL ExecutenonQuery ();
	// 批量插入数据 (不确定的字符值，用 Bind 接口来填充这些值)
	// 列的序号从 1 开始，查询时列的序列号从 0 开始
	BOOL Bind (int index);
	BOOL Bind (int index, int data);
	BOOL Bind (int index, long long data);
	BOOL Bind (int index, double data);
	BOOL Bind (int index, const char *data, int datalen);
	BOOL Bind (int index, const wchar_t *data, int datalen);
	BOOL Bind (int index, const void *data, int datalen);
	BOOL Bind (int index, const std::string &data);
	BOOL Bind (int index, const std::wstring &data);
	// 单行查询结果
	BOOL ExecuteInt (int& nResult);
	BOOL ExecuteInt64 (long long& nResult);
	BOOL ExecuteDouble (double& bResult);
	BOOL ExecuteString (std::string& strResult);
	BOOL ExecuteString16 (std::wstring& wstrResult);
	BOOL ExecuteBlob (std::string& nResult);
private:
	friend class CRunSqlRead;

	CRunSqlConnect& __dbconnect;
	struct sqlite3_stmt *__stmt;
	unsigned int __refs;
	int my_argc;
	bool __isexiterror;
};

class CRunSqlRead
{
public:
	CRunSqlRead ();
	CRunSqlRead (const CRunSqlRead& m_read);

	~CRunSqlRead(void);

	/**
	*
	* 判断 sql 语句执行结果
	* 
	* @Warning 与以前的版本不同，必须使用以下的返回值进行判断
	* @Return 
	*	- @see SQLITE3_READ_ROW 
	*	- @see SQLITE3_READ_DOWN
	*	- @see SQLITE3_READ_ERROR
	*/
	INT Read ();

	// 重置通配符
	BOOL Reset ();
	// 得到查询到的列号的结果 (index 列号从 0 开始)
	int	GetInt(int index);
	BOOL GetInt (int index, int& nResult);

	int	GetByte(int index);
	BOOL GetByte (int index, char& nResult);

	LONGLONG GetInt64 (int index);
	BOOL GetInt64 (int index, long long& nResult);
	BOOL GetDouble (int index, double& bResult);

	std::string GetString (int index);
	BOOL GetString (int index, std::string& strResult);
	
	BOOL GetString16 (int index, std::wstring& wstrResult);
	BOOL GetBlob (int index, std::string& strResult);
	// 得到列名
	BOOL GetColname (int index, std::string& strResult);
	BOOL GetColname16 (int index, std::wstring& wstrResult);
	// 根据列名得到列的序号
	int GetColindex (std::string colname);
	BOOL GetColindex (std::string colname, int& nResult);
	// 得到列的总数
	BOOL GetColcount (int& nResult);
	// 关闭通配符
	void Close ();
	// 赋值操作符重载
	CRunSqlRead& operator=(const CRunSqlRead &copy);

private:
	friend class CRunSqlCommand;
	CRunSqlRead (CRunSqlCommand* m_read);

	CRunSqlCommand* __command;

	// 列名 -- 列查询序号
	std::map<std::string, int> colmap;
};

class CRunSqlTransaction
{
private:
	CRunSqlConnect& con;
	bool intrans;

public:
	CRunSqlTransaction (CRunSqlConnect &con, bool start=true);
	~CRunSqlTransaction ();

	void begin();
	void commit();
	void rollback();
};

}

#endif