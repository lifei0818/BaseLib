#include "common/sys_incl.h"
#include "common/stdfile.h"
#include "common/inifile.h"
#include "common/function.h"
#include "common/tolists.h"
//#include "common/MD5.h"
//#include "common/RC4-LIB.h"
#include "common/os.h"

namespace BaseLib
{

template<typename TYPE>
BOOL
ReadLineToArray (LPCSTR lpFileName, 
				 TYPE &t,
				 BOOL bDellete)
{
	if (!lpFileName || '\0' == *lpFileName)
	{
		DBGPRINT (DBG_ERROR, "参数错误");
		return FALSE;
	}

	FILE *lpFile = fopen (lpFileName, "r");
	if (NULL == lpFile)
	{
		return FALSE;
	}

	char szBuffer[PAGE_SIZE];
	memset (szBuffer, 0, sizeof (szBuffer));
	while (!feof (lpFile))
	{
		szBuffer[0] = 0;
		fgets (szBuffer, sizeof (szBuffer) - 1, lpFile);

		std::string szLine = szBuffer;
//		boost::trim_right (szLine);
        BaseLib::StringTrimRight(szLine);
		if (szLine.empty()) 
		{
			continue;
		}
		t.push_back (szLine);
	}

	fclose (lpFile);

	if (bDellete) 
	{
		BaseLib::CxxDeleteFile (lpFileName);
	}

	return TRUE;
}

BOOL
ReadFileToList (LPCSTR lpFileName, 
				LIST_STRING &List,
				BOOL bDelete)
{
	return ReadLineToArray (lpFileName, List, bDelete);
}

std::string
ReadFileToString (LPCSTR lpFile)
{
	if (!lpFile || '\0' == *lpFile)
	{
		DBGPRINT (DBG_ERROR, "参数错误");
		return NULL_STRING;
	}

	CAutoFile autoFile (fopen (lpFile, "rb"));
	if (autoFile.Get())
	{
		fseek (autoFile.Get(), 0, SEEK_END);
		DWORD dwSize = ftell (autoFile.Get());
		fseek (autoFile.Get(), 0, SEEK_SET);

		LPSTR lpChar = new char [dwSize];
		if (lpChar)
		{
			fread (lpChar, dwSize, 1, autoFile.Get());
			std::string strValue;
			strValue.append (lpChar, dwSize);

			delete []lpChar;
			return strValue;
		}
	}

	return NULL_STRING;
}

BOOL ReadFileData (LPCSTR lpFile, TArrayString &arrayString)
{
	CAutoFile autoFile (fopen (lpFile, "rb"));
	if (*autoFile)
	{
		fseek (*autoFile, 0, SEEK_END);
		DWORD dwSize = ftell (*autoFile);
		fseek (*autoFile, 0, SEEK_SET);

		arrayString.Reset (dwSize);
		if (arrayString.IsRight())
		{
			fread (*arrayString, dwSize, 1, *autoFile);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL WriteFileData (LPCSTR lpFile, LPCSTR lpData)
{
	CAutoFile autoFile (fopen (lpFile, "wb"));
	if (autoFile.Get())
	{
		fwrite (lpData, strlen (lpData), 1, autoFile.Get());
		return TRUE;
	}

	return FALSE;
}

BOOL AppendFileBinData (LPCSTR lpFile, LPVOID lpData, int iLen)
{
	CAutoFile autoFile (fopen (lpFile, "a+b"));
	if (autoFile.Get())
	{
		fwrite (lpData, iLen, 1, autoFile.Get());
		return TRUE;
	}

	return FALSE;
}

BOOL AppendFileData (LPCSTR lpFile, LPCSTR lpData)
{
	return AppendFileBinData (lpFile, (LPVOID)lpData, strlen (lpData));
}

#define DEFAULT_FILE_PASSWORD 0x02

void XorData (UCHAR* lpBuffer, int iLen)
{
	for (int i = 0; i < iLen; i++)
	{
		lpBuffer[i] ^= DEFAULT_FILE_PASSWORD;
	}
}

BOOL AppendFileDataNeedEncrypTion (LPCSTR lpFile, LPCSTR lpData)
{
	if (INVALID_PARAM_LPSTR (lpData))
	{
		return FALSE;
	}

	int iLen = strlen (lpData);
	UCHAR* lpBuffer = new UCHAR [iLen];
	memcpy (lpBuffer, lpData, iLen);

	XorData (lpBuffer, iLen);

	BOOL bResult = AppendFileBinData (lpFile, lpBuffer, iLen);
	
	delete []lpBuffer;
	return bResult;
}
#if 0
std::string Md5File (LPCSTR lpFile, unsigned int interval)
{
	CHAR cFilePath[128] = {0};
	if (INVALID_PARAM_LPSTR (lpFile))
	{
		return "";
	}

	int iLen = strlen (lpFile);
	if (iLen >= sizeof (cFilePath))
	{
		iLen = sizeof (cFilePath) - 1;
	}

	strncpy (cFilePath, lpFile, iLen);
	DBGPRINT(DBG_PROCESS,"开始计算"<< cFilePath <<" 的MD5值");  //为方便dmp堆栈中存放文件信息

	UCHAR uResult [16] = {0};
	TMD5 md5;
	MD5_CTX context1;
	md5.Init(&context1);
	CAutoFile autoFile (fopen (lpFile, "rb"));	
	if (*autoFile)
	{
		fseek (*autoFile, 0, SEEK_END);
		DWORD dwSize = ftell (*autoFile);
		fseek (*autoFile, 0, SEEK_SET);
		DWORD dwTempSize = 1*1024*1024;     //为防止占用内存过多，边读取边计算，每次读取1M，计算MD5；

		if (dwSize > dwTempSize)
		{
			int n = dwSize/dwTempSize;
			TArrayString arrayString (dwTempSize);
			if (arrayString.IsRight())
			{
				for (int i = 0; i < n; i++)
				{
					fread (*arrayString, dwTempSize, 1, *autoFile);
					md5.Update(&context1,(UCHAR*)arrayString.Get(),arrayString.Size(),interval);
					BaseLib::OS::msleep(1);
				}
			}
			else
			{
				return NULL_STRING;
			}

			DWORD dwLastSize = dwSize - n * dwTempSize;
			if (dwLastSize > 0)
			{
				fread (*arrayString, dwLastSize, 1, *autoFile);
				md5.Update (&context1,(UCHAR*)arrayString.Get(), dwLastSize,interval);
			}			
		} 
		else
		{
			TArrayString arrayString (dwSize);
			if (arrayString.IsRight())
			{
				fread (*arrayString, dwSize, 1, *autoFile);
				md5.Update(&context1,(UCHAR*)arrayString.Get(),arrayString.Size(),interval);
			}
			else
			{
				return NULL_STRING;
			}

		}			
		md5.Final(uResult, &context1);
		
		char strResult [32];
		std::string strMd5;
		for (int i = 0; i < sizeof (uResult); i++)
		{
			sprintf (strResult, "%.2X", uResult[i]);
			strMd5 += strResult;
		}

		return strMd5;
	}
	return NULL_STRING;
}

BOOL Md5VerityFile (LPCSTR lpIniFile, LPCSTR lpFile)
{
	CHAR strFileName[MAX_PATH];
	CHAR strFileExt[MAX_PATH];

#ifdef _MACRO_OS_WIN32
	_splitpath (lpFile, NULL, NULL, strFileName, strFileExt);
	strcat (strFileName, strFileExt);
#else
    strcpy(strFileName, lpFile);
    strcpy(strFileName, ::basename(strFileName));
#endif

	CIniFile iniFile (lpIniFile);
	std::string strMd5 = iniFile.ReadString ("FILE", strFileName, NULL_STRING);
	if (strMd5.empty())
	{
		// INI 里没有这个文件
		return TRUE;
	}

	if (FileAccess (lpFile, 0) == -1)
	{
		// 文件不存在
		return TRUE;
	}

	std::string strNowMd5 = Md5File (lpFile);
	if (stricmp (strNowMd5.c_str(), strMd5.c_str()) == 0)
	{
		return TRUE;
	}

	DBGPRINT (DBG_ERROR, lpFile << " 校验错误");
	return FALSE;
}

BOOL Md5VerityFile (LPCSTR lpFile)
{
	CHAR strDrive[MAX_PATH];
	CHAR strDir[MAX_PATH];
	CHAR strIniFile[STANDARD_SIZE];

#ifdef _MACRO_OS_WIN32
	_splitpath (lpFile, strDrive, strDir, NULL, NULL);

	strcpy (strIniFile, strDrive);
	strcat (strIniFile, strDir);
#else
    strcpy(strIniFile, lpFile);
    ::dirname(strIniFile);
#endif
	strcat (strIniFile, MD5_INI_FILE);

	return Md5VerityFile (strIniFile, lpFile);
}

BOOL Md5VerityIni (LPCSTR lpDirectory)
{
	if (INVALID_PARAM_LPSTR (lpDirectory))
	{
		DBGPRINT (DBG_ERROR, "参数错误");
		return FALSE;
	}

	CHAR strIniFile[STANDARD_SIZE] = {0};
	strcpy (strIniFile, lpDirectory);
	if (strIniFile[strlen(strIniFile) - 1] != PATH_SEPARATOR)
	{
	    strcat (strIniFile, PATH_SEP_STRING);
    }
    strcat (strIniFile, MD5_CRC_FILE);
	std::string strCrcFile = strIniFile;
	std::string strMd5 = ReadFileToString (strCrcFile.c_str());
	if (strMd5.empty())
	{
		DBGPRINT (DBG_ERROR, "忽略校验");
		return TRUE;
	}
	
	memset (strIniFile, 0, sizeof (strIniFile));
	strcpy (strIniFile, lpDirectory);
	strcat (strIniFile, PATH_SEP_STRING MD5_INI_FILE);
	std::string strMd5Crc = Md5File (strIniFile, 5000);
	if (stricmp (strMd5Crc.c_str(), strMd5.c_str()) != 0)
	{
		DBGPRINT (DBG_ERROR, "校验错误, " << strCrcFile << " 的校验码: " << strMd5 << " " << strIniFile << " 的校验码: " << strMd5Crc);
		return FALSE;
	}

	return TRUE;
}

BOOL Md5VerityOrgDirectory (LPCSTR lpDirectory)
{
	if (INVALID_PARAM_LPSTR (lpDirectory))
	{
		DBGPRINT (DBG_ERROR, "参数错误");
		return FALSE;
	}

	LIST_STRING fileList;
	CxxFileList (lpDirectory, fileList);

	for (LIST_STRING::iterator iter = fileList.begin(); iter != fileList.end(); iter++)
	{
		std::string strFile = lpDirectory;
		strFile += PATH_SEP_STRING;
		strFile += *iter;

		if (!Md5VerityFile (strFile.c_str()))
		{
			return FALSE;
		}
	}

	fileList.clear();
	CxxDirectoryList (lpDirectory, fileList);

	for (LIST_STRING::iterator iter = fileList.begin(); iter != fileList.end(); iter++)
	{
		std::string strFile = lpDirectory;
		strFile += PATH_SEP_STRING;
		strFile += *iter;
		if (!Md5VerityOrgDirectory (strFile.c_str()))
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL Md5VerityDirectory (LPCSTR lpDirectory)
{
	if (INVALID_PARAM_LPSTR (lpDirectory))
	{
		DBGPRINT (DBG_ERROR, "参数错误");
		return FALSE;
	}

	if (!Md5VerityIni (lpDirectory))
	{
		return FALSE;
	}

	return Md5VerityOrgDirectory (lpDirectory);
}


BOOL Md5VerityDirectory (LPCSTR lpDirectory, std::string &strBadFile)
{
	if (INVALID_PARAM_LPSTR (lpDirectory))
	{
		DBGPRINT (DBG_ERROR, "参数错误");
		return FALSE;
	}

	if (!Md5VerityIni (lpDirectory))
	{
		return FALSE;
	}

	LIST_STRING fileList;
	CxxFileList (lpDirectory, fileList);

	for (LIST_STRING::iterator iter = fileList.begin(); iter != fileList.end(); iter++)
	{
		std::string strFile = lpDirectory;
		if (lpDirectory[strlen(lpDirectory) - 1] != PATH_SEPARATOR)
		{
			strFile += PATH_SEP_STRING;
		}
		strFile += *iter;

		if (!Md5VerityFile (strFile.c_str()))
		{
			if (!strBadFile.empty())
			{
				strBadFile += ";";
			}
			strBadFile += iter->c_str();
		}
	}

	fileList.clear();

	return TRUE;
}

#endif

INT FileAccess ( LPCSTR lpFilePath, INT nAccess )
{
#ifdef _MACRO_OS_WIN32    
	return ::_access ( lpFilePath, nAccess );
#else
	return ::access ( lpFilePath, nAccess );
#endif
}

BOOL IsFileExist (LPCSTR lpDirectory)
{
	if ( lpDirectory == NULL 
		|| *lpDirectory == 0 )
	{
		return FALSE;
	}

	if ( -1 != FileAccess ( lpDirectory, FILE_EXIST_ACCESS_EX ) )
	{
		return TRUE;
	}

	return FALSE;
}

//BOOL AnalyFileSuffix (
//					  IN LPCSTR lpFileName, 
//					  OUT std::string& strFilePrifix,
//					  OUT std::string& strFileSuffix
//					  )
//{
//	if ( lpFileName == NULL 
//		|| *lpFileName == 0 )
//	{
//		return FALSE;
//	}
//	
//	std::string strFileName = lpFileName;
//	size_t nPos = 0;
//
//	// 解析 "c:\\aa.txt" 还是 "aa.txt"
//	nPos = strFileName.find_last_of ( "\\" );
//
//	if ( nPos != std::string::npos )
//	{
//		strFileName = strFileName.substr ( nPos + 1, strFileName.size () - ( nPos + 1 )  );	
//	}
//	
//	// 解析 aa.txt
//	nPos = strFileName.find_last_of ( "." );
//	
//	if ( nPos != std::string::npos )
//	{
//		strFilePrifix = strFileName.substr ( 0, nPos );
//		strFileSuffix = strFileName.substr ( nPos + 1, strFileName.size () - ( nPos + 1 ) );
//	}
//	else
//	{
//		strFilePrifix = strFileName;
//		strFileSuffix = "";
//	}
//
//	return TRUE;
//}

}
