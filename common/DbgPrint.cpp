#include "common/sys_incl.h"
#include "common/DbgPrint.h"
#include "common/debug.h"
#include "common/tolists.h"

namespace BaseLib
{
	int
	CDbgPrint::GetDebugLevel (DWORD id)
	{
		if (id & DBG_ERROR)
		{
			return 1;
		}
		else
		{
			BASELIB_LOCK (m_lock);
			for (std::list<DWORD>::iterator iter = m_list.begin();
				iter != m_list.end();
				iter++)
			{
				if (id == *iter) return 0;
			}
		}

		return -1;
	}

	void
	CDbgPrint::SetDebug (LPCSTR lpString)
	{
		BASELIB_LOCK (m_lock);

		m_list.clear();
		m_textlist.clear();

		if (!lpString) return ;
		std::string szString = lpString;
		std::replace (szString.begin(),szString.end(), ',', ';');

		BaseLib::LIST_STRING lists = ToLists (szString, "|");

		int iCount = 0;
		for (BaseLib::LIST_STRING::iterator iter = lists.begin();
			iter != lists.end();
			iter++, iCount++)
		{
			if (0 == iCount)
			{
				m_list = ToHexNumLists (*iter, ";");
			}
			else if (1 == iCount)
			{
				m_textlist = ToLists (*iter, ";");
				break;
			}
		}
	}

    void
    CDbgPrint::DbgPrint(DWORD nLevel, LPCSTR lpFile, UINT nLine, LPCSTR lpFunc, LPCSTR lpInfo)
    {
		int iLevel = GetDebugLevel(nLevel);
		if (this->m_DbgFunction)
		{
			if (0 == iLevel)
			{
				if (m_textlist.size())
				{
					BOOL bFind = FALSE;
					for (std::list<std::string>::iterator iter = m_textlist.begin();
						iter != m_textlist.end();
						iter++)
					{
						if (strstr (lpInfo, (*iter).c_str())) 
						{
							bFind = TRUE;
							break;
						}
					}

					if (FALSE == bFind)
					{
						return ;
					}
				}

				this->m_DbgFunction(nLevel, lpFile, nLine, lpFunc, lpInfo);
			}
			else if (1 == iLevel)
			{
				this->m_DbgFunction(nLevel, lpFile, nLine, lpFunc, lpInfo);
			}
		}
    }

    void
    CDbgPrint::DefaultDbgPrint(DWORD nLevel, LPCSTR lpFile, UINT nLine, LPCSTR lpFunc, LPCSTR lpInfo)
    {
        printf ("[%d|%s|%d|%s]%s\r\n", nLevel, lpFile, nLine, lpFunc, lpInfo);
    }


}
