// sqlite3_manage1.cpp : �������̨Ӧ�ó������ڵ㡣
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
	// ����
	// �����ݿ�
	std::string strDbName = "c:\\���ݿ����\\dbTest2.db3";

	// �������ݿ��������
	ISynchDBConnector m_db(strDbName.c_str());

	// �����ݿ�
	bool bResult = m_db.Open();
	
	// �鿴���Ƿ����
	int nResult = m_db.IsExistTable ("dbTable");

	// ������
	if (nResult == CT_OK)
	{
		bResult = m_db.ExecSql ("create table dbTable(id integer, col_string string);");
	}
	else
	{
		// �ظ�����������
		//bResult = m_db.ExecSql ("create table dbTable(id integer, col_string string);");
	}

	// ��������
	CRunSqlTransaction m_tran = m_db.Begin();

	{
		// ���в���
		bResult = m_db.ExecSql ("insert into dbTable values(1,'aa');");	

		// ���в��� (�е���Ŵ�1��ʼ)
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

	// ��ѯ����
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

			// �õ����ݿ����к�
			bResult = reader.GetColindex("id", nColIndex);
			bResult = reader.GetColindex("col_string", nColIndex1);

			// �����кŵõ�����
			bResult1 = reader.GetInt(nColIndex, nResult);
			bResult = reader.GetString(nColIndex1, strColName);

			if (bResult && bResult1)
			{
				cout << strColName.c_str() << ": " 
					<< nResult << endl;
			}
		}
	}
	
	// ɾ������ָ��������
	bResult = m_db.ExecSql ("delete from dbTable where id=1;");	
	// ɾ���������е�����
	bResult = m_db.ClearTable("dbTable");
	// �ر����ݿ�
	m_db.Close();
	return 0;
}

