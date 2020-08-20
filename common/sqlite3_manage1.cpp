// sqlite3_manage1.cpp : 定义控制台应用程序的入口点。
//

#include "sys_incl.h"
#include <string>
#include <iostream>
#include "baselib.h"
#include "RunSqlite3.h"
#include "DBConnector.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	// 测试
	// 建数据库
	std::string strDbName = "c:\\数据库测试\\dbTest2.db3";

	// 定义数据库操作对象
	ISynchDBConnector m_db(strDbName.c_str());

	// 打开数据库
	bool bResult = m_db.Open();
	
	// 查看表是否存在
	int nResult = m_db.IsExistTable ("dbTable");

	// 创建表
	if (nResult == CT_OK)
	{
		bResult = m_db.ExecSql ("create table dbTable(id integer, col_string string);");
	}
	else
	{
		// 重复建表，测试用
		//bResult = m_db.ExecSql ("create table dbTable(id integer, col_string string);");
	}

	// 插入数据
	CRunSqlTransaction m_tran = m_db.Begin();

	{
		// 单行插入
		bResult = m_db.ExecSql ("insert into dbTable values(1,'aa');");	

		// 多行插入 (列的序号从1开始)
		CRunSqlCommand cmd(m_db.Conn(),
			"insert into dbTable values(?,?);");
		bResult = cmd.Bind(2, "foobar", 6);

		for(int i=0; i<3; i++) {
			bResult = cmd.Bind(1, (int)i);
			bResult = cmd.ExecutenonQuery();
		}

	}
	
	m_db.Commit (m_tran);

	int nResult1 = m_db.ExecSqlGetId("select * from dbTable where col_string='aa';");

	// 查询数据
	{
		CRunSqlCommand cmd(m_db.Conn(), "select * from dbTable;");
		CRunSqlRead reader = cmd.ExecuteReader();

		int nResult;
		std::string strColName;

		bool bResult1 = false;
		while (SQLITE3_READ_ROW == reader.Read())
		{
			nResult = 0;
			strColName = "";
			
			int nColIndex = 0;
			int nColIndex1 = 0;

			// 得到数据库中列号
			bResult = reader.GetColindex("id", nColIndex);
			bResult = reader.GetColindex("col_string", nColIndex1);

			// 根据列号得到数据
			bResult1 = reader.GetInt(nColIndex, nResult);
			bResult = reader.GetString(nColIndex1, strColName);

			if (bResult && bResult1)
			{
				cout << strColName.c_str() << ": " 
					<< nResult << endl;
			}
		}
	}
	
	// 删除表中指定的内容
	bResult = m_db.ExecSql ("delete from dbTable where id=1;");	
	// 删除表中所有的内容
	bResult = m_db.ClearTable("dbTable");
	// 关闭数据库
	m_db.Close();
	return 0;
}

