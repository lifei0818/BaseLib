/*----------------------------------------------------------------------------*
* RunSqlite3.h 
*
* Data:             2010.8.23
*
* Company:          YuanWang
* 
* Copyright:        www.sqlite3.org
*
* Describe:         ���� sqlite3 ���ݿ�������ඨ��
*
*----------------------------------------------------------------------------*/
#ifndef __baselib__runsqlite3__
#define __baselib__runsqlite3__

#include "common/sys_incl.h"
#include "sqlite3/sqlite3.h"

/// ERROR CODES
#define SQLITE3_READ_ROW				 (DWORD)1		///< ִ�� sql ���ʱ��������
#define SQLITE3_READ_DOWN				 (DWORD)0		///< ִ�� sql ���ʱ�ɹ����
#define SQLITE3_READ_ERROR				 (DWORD)-1		///< ִ�� sql ���ʱ��������

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

	// �����ݿ�
	BOOL Open (const char *db);
	BOOL Open (const wchar_t *db);
	// �ر����ݿ�
	VOID Close ();
	// ���һ������� RowID
	BOOL InsertId (long long& nRowId);
	// ����æ�ȴ�ʱ��
	BOOL SetBusytimeout (int ms);

	// ִ�� sql ���
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
	// ������Ϣ
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

	// �õ� sql ���Ĳ�ѯ����
	CRunSqlRead ExecuteReader ();
	// ִ�� sql ���
	BOOL ExecutenonQuery ();
	// ������������ (��ȷ�����ַ�ֵ���� Bind �ӿ��������Щֵ)
	// �е���Ŵ� 1 ��ʼ����ѯʱ�е����кŴ� 0 ��ʼ
	BOOL Bind (int index);
	BOOL Bind (int index, int data);
	BOOL Bind (int index, long long data);
	BOOL Bind (int index, double data);
	BOOL Bind (int index, const char *data, int datalen);
	BOOL Bind (int index, const wchar_t *data, int datalen);
	BOOL Bind (int index, const void *data, int datalen);
	BOOL Bind (int index, const std::string &data);
	BOOL Bind (int index, const std::wstring &data);
	// ���в�ѯ���
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
	* �ж� sql ���ִ�н��
	* 
	* @Warning ����ǰ�İ汾��ͬ������ʹ�����µķ���ֵ�����ж�
	* @Return 
	*	- @see SQLITE3_READ_ROW 
	*	- @see SQLITE3_READ_DOWN
	*	- @see SQLITE3_READ_ERROR
	*/
	INT Read ();

	// ����ͨ���
	BOOL Reset ();
	// �õ���ѯ�����кŵĽ�� (index �кŴ� 0 ��ʼ)
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
	// �õ�����
	BOOL GetColname (int index, std::string& strResult);
	BOOL GetColname16 (int index, std::wstring& wstrResult);
	// ���������õ��е����
	int GetColindex (std::string colname);
	BOOL GetColindex (std::string colname, int& nResult);
	// �õ��е�����
	BOOL GetColcount (int& nResult);
	// �ر�ͨ���
	void Close ();
	// ��ֵ����������
	CRunSqlRead& operator=(const CRunSqlRead &copy);

private:
	friend class CRunSqlCommand;
	CRunSqlRead (CRunSqlCommand* m_read);

	CRunSqlCommand* __command;

	// ���� -- �в�ѯ���
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