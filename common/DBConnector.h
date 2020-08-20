/*----------------------------------------------------------------------------*
* DBConnector.h 
*
* Data:             2010.8.23
*
* Company:          YuanWang
* 
* Copyright:        www.sqlite3.org
*
* Describe:         完成对数据库的一系列常用操作，包括建立数据库，建立表，
*                   插入数据，删除数据，更改数据，对事务的操作。
*
*                   数据库操作范例详见同一文件夹下的 sqlite3_manage1.cpp 文件
*
*----------------------------------------------------------------------------*/

#ifndef  __baselib__dbconnector__
#define  __baselib__dbconnector__

#include "common/RunSqlite3.h"
#include "common/CodeConver.h"

using namespace Sqlite3Db;

#define DB_SQL_EXCEPTION(SQL)	\
	do \
	{	\
		static BOOL g_bHasError = FALSE;	\
		if (FALSE == g_bHasError)	\
		{	\
			g_bHasError = TRUE;	\
			DBGPRINT(DBG_ERROR,("数据库 ")<<m_szFile<<(" 异常: ")<<Conn().GetLastError()<<" SQL:"<<SQL);	\
		}	\
	} while (0);

#define DB_EXCEPTION	\
	do \
	{	\
		static BOOL g_bHasError = FALSE;	\
		if (FALSE == g_bHasError)	\
		{	\
			g_bHasError = TRUE;	\
			DBGPRINT(DBG_ERROR,("数据库 ")<<m_szFile<<(" 异常: ")<<Conn().GetLastError());	\
		}	\
	} while (0);

#define SQL_EQUAL(I,V) I<<"="<<"'"<<V<<"'"
#define SQL_NOT_EQUAL(I,V) I<<"!="<<"'"<<V<<"'"
#define SQL_GREATE(I,V) I<<">"<<V
#define SQL_GREATE_EQUAL(I,V) I<<">="<<V
#define SQL_LESS(I,V) I<<"<"<<V
#define SQL_LESS_EQUAL(I,V) I<<"<="<<V

/************************************************************************/
/* 
/************************************************************************/

#define CT_ERROR	-1	///< 失败
#define CT_OK		0	///< 表创建成功
#define CT_EXIST	1	///< 表存在

#define FIELD_ID "id"

class IDBConnector
{
public:

	IDBConnector (LPCSTR lpFile)
		: m_bOpen (FALSE)
	{
		if (INVALID_PARAM_LPSTR (lpFile))
		{
			DBGPRINT (DBG_ERROR, "变量错误");
			return ;
		}

		m_szFile = lpFile;
	}


	//////////////virtual 导致问题，暂时屏掉///////////////////
	/*virtual*/ ~IDBConnector ()
	{
		Close();
	}

	/// 打开数据库文件///////同上//////////
	/*virtual*/ BOOL Open ()
	{
		if (m_bOpen)
		{
			return TRUE;
		}

#ifdef _MACRO_OS_WIN32
		m_szUtf8File.clear();
		CCodeConver codeConver;
		int nSize = -1;
		LPSTR lpUtf8Char = (LPSTR)codeConver.AnsiToUTF8 (m_szFile.c_str(), &nSize);
		m_szUtf8File.append (lpUtf8Char, nSize);
#else
		m_szUtf8File = m_szFile;
#endif
		if (m_szUtf8File.empty())
		{
			return FALSE;
		}

		BOOL bResult = FALSE;

		if (m_conn.Open (m_szUtf8File.c_str()))
		{
			m_bOpen = TRUE;
		}
		else
		{
			DB_EXCEPTION;
			return FALSE;
		}

		return TRUE;
	}

	/// 关闭数据库文件
	VOID Close ()
	{
		if (m_bOpen) {
			m_conn.Close ();
			m_bOpen = FALSE;
		}
	}

	/**
	* 查询表是否存在
	*
	* @param LPCSTR lpTable 表名称
	* @Return INT
	*	- @see CT_ERROR
	*	- @see CT_OK	
	*	- @see CT_EXIST
	*/
	INT IsExistTable (LPCSTR lpTable)
	{
		if (INVALID_PARAM_LPSTR (lpTable))
		{
			DBGPRINT (DBG_ERROR, "变量错误");
			return CT_ERROR;
		}

		std::ostringstream o;
		o << _T("select count(*) from sqlite_master where name='") << lpTable << _T("';");

		if (Open ())
		{

			int nCount = 0;

			if (m_conn.ExecuteInt (o.str().c_str(), nCount))
			{
				return nCount == 0 ? CT_OK : CT_EXIST;
			}
			else
			{
				DB_EXCEPTION;
			}
		}

		return CT_ERROR;
	}

	/**
	* 查询表字段是否存在
	*
	* @param LPCSTR lpTable 表名称
	* @param LPCSTR lpField 字段名称
	* @Return INT
	*	- @see CT_ERROR
	*	- @see CT_OK	
	*	- @see CT_EXIST
	*/
	INT IsExistField (LPCSTR lpTable, LPCSTR lpField)
	{
		if (INVALID_PARAM_LPSTR (lpTable))
		{
			DBGPRINT (DBG_ERROR, "变量错误");
			return CT_ERROR;
		}

		std::ostringstream o;
		o << _T("PRAGMA table_info(") << lpTable << _T(");");

		if (Open ())
		{
			CRunSqlCommand cmd (Conn(), o.str().c_str());
			CRunSqlRead reader = cmd.ExecuteReader ();
			int nCount = 0;

			while (SQLITE3_READ_ROW == reader.Read())
			{
				std::string strFieldName = reader.GetString (reader.GetColindex ("name"));
				if (strcmp(strFieldName.c_str(), lpField) == 0)
				{
					nCount = 1;
					break;
				}
			}

			return nCount == 0 ? CT_OK : CT_EXIST;
		}

		return CT_ERROR;
	}

	/// 清除表,不删除表
	BOOL ClearTable (LPCSTR lpTable)
	{
		if (INVALID_PARAM_LPSTR (lpTable))
		{
			DBGPRINT (DBG_ERROR, "变量错误");
			return FALSE;
		}

		std::ostringstream o;
		o << "delete from " << lpTable;

		if (Open ())
		{
			return ExecSql (o.str().c_str());
		}

		return FALSE;
	}

	/// 获取纪录个数
	int GetRecordCount (LPCSTR lpTable)
	{
		if (INVALID_PARAM_LPSTR (lpTable))
		{
			DBGPRINT (DBG_ERROR, "变量错误");
			return FALSE;
		}

		std::ostringstream o;
		o << "select count(*) as " FIELD_ID " from " << lpTable;

		return ExecSqlGetId (o.str().c_str());
	}

	/// 获取记录的最后一个id
	int GetRecordMaxId (LPCSTR lpTable)
	{
		if (INVALID_PARAM_LPSTR (lpTable))
		{
			DBGPRINT (DBG_ERROR, "变量错误");
			return 0;
		}

		std::ostringstream o;
		o << "select max(" FIELD_ID ") as " FIELD_ID " from " << lpTable;

		return ExecSqlGetId (o.str().c_str());
	}

	/// 根据id从表删除记录
	BOOL Delete (LPCSTR lpTable, int iId)
	{
		return Delete (lpTable, &iId, 1);
	}

	BOOL Delete (LPCSTR lpTable, int* lpId, int iCount)
	{
		if (INVALID_PARAM_LPSTR (lpTable) || iCount <= 0 || NULL == lpId)
		{
			DBGPRINT (DBG_ERROR, "变量错误");
			return FALSE;
		}

		if (Open ())	
		{
			std::ostringstream o;
			o << "delete from " << lpTable << " where ";
			for (int i = 0; i < iCount; i++)
			{
				if (0 != i)
				{
					o << " or ";
				}
				
				o << "id=" << lpId[i];
			}

			return ExecSql (o.str().c_str());
		}

		return FALSE;
	}

	/// 执行 sql 语句
	BOOL ExecSql (LPCSTR lpSql)
	{
		if (m_conn.ExecutenonQuery (lpSql))
 		{
 			return TRUE;
 		}

		DB_SQL_EXCEPTION (lpSql);
		return FALSE;
	}

	int ExecSqlGetId (LPCSTR lpSql)
	{
		if (Open())
		{
			CRunSqlCommand cmd (Conn(), lpSql);
			CRunSqlRead reader = cmd.ExecuteReader ();

			if (reader.Read () == SQLITE3_READ_ROW)
			{
				int nResult = 0;
				int nColIndex = 0;

				// 得到 id 在数据库中的列的序号
				if (!reader.GetColindex (FIELD_ID, nColIndex))
				{
					DB_EXCEPTION;
					return -1;
				}
				// 根据列的序号得到结果
				if (reader.GetInt (nColIndex, nResult)) 
				{
					return nResult;
				}
				else
				{
					DB_EXCEPTION;
					return -1;
				}
			}
		}
		else
		{
			//DBGPRINT (DBG_ERROR, "未打开");
			return -1;
		}

		return 0;
	}

	/// 开始事务
	CRunSqlTransaction Begin()
	{
		return CRunSqlTransaction (m_conn);
	}

	/// 提交事务
	void Commit (CRunSqlTransaction &tran)
	{
		tran.commit();
	}

	/// 回滚事务
	void RollBack (CRunSqlTransaction &tran)
	{
		tran.rollback();
	}

	CRunSqlConnect& Conn() { return m_conn; }

protected:

	std::string m_szUtf8File;
	std::string m_szFile;
	BOOL m_bOpen;
	CRunSqlConnect m_conn;
};

// 多线程 异步
typedef IDBConnector ISynchDBConnector;

#endif