#ifndef BASELIB_INIFILE_H
#define BASELIB_INIFILE_H

namespace BaseLib
{
typedef std::vector<std::string> INIVECTOR;

class CIniFile 
{
public :
	/// 必须使用绝对路径,因为 api 有 bug
	CIniFile (LPCSTR lpFileName);

	std::string ReadSection (LPCSTR lpSection);
	BOOL ReadBool (LPCSTR lpSection, LPCSTR lpIdent, BOOL bDefault);
	std::string ReadString (LPCSTR lpSection, LPCSTR lpIdent, LPCSTR lpszDefault);
	int ReadInteger (LPCSTR lpSection, LPCSTR lpIdent, int nDefault);

	void WriteBool (LPCSTR lpSection, LPCSTR lpIdent, BOOL bValue);
	void WriteString (LPCSTR lpSection, LPCSTR lpIdent, LPCSTR lpValue);
	void WriteInteger (LPCSTR lpSection, LPCSTR lpIdent, int nValue);

protected:
#ifndef _MACRO_OS_WIN32
    BOOL Open(void);
    BOOL Save(void);
    void RemoveComment(std::string &strLine);
	BOOL GetSection(const std::string &strSectName, std::string &strSectValue);
    BOOL GetSectionName(const std::string &strLine, std::string &strSect);
    BOOL GetKeyValue(const std::string &strLine, 
                     std::string &strKey, 
                     int &nValuePos, 
                     int &nValueLen);
    BOOL Search(const std::string &strSect, 
                const std::string &strKey, 
                BOOL &bFindSect,
                INIVECTOR::iterator &itor,
                int &nValuePos, 
                int &nValueLen);
    void AddSection(const std::string &strSect);
    void AddKey(INIVECTOR::iterator itor, 
                const std::string &strKey,
                const std::string &strValue);
    BOOL GetValue(const std::string &strSect, 
                  const std::string &strKey, 
                  const std::string &lpszDefault,
                  std::string &strOut);
    void SetValue (const std::string &strSect, 
                   const std::string &strKey, 
                   const std::string &strValue);
#endif    

    std::string	m_szFileName;

#ifndef _MACRO_OS_WIN32
    INIVECTOR m_vectIni;
    BOOL m_bOpened;
#endif
};

DWORD GetPrivateProfileString(LPCTSTR lpAppName, 
                              LPCTSTR lpKeyName, 
                              LPCTSTR lpDefault, 
                              LPSTR lpReturnedString,
                              DWORD nSize, 
                              LPCTSTR lpFileName);

#ifndef _MACRO_OS_WIN32
#define GetPrivateProfileStringA    GetPrivateProfileString
#endif

int  GetPrivateProfileInt(LPCTSTR lpAppName, 
                          LPCTSTR lpKeyName, 
                          INT     nDefault, 
                          LPCTSTR lpFileName);

void WritePrivateProfileString(LPCTSTR lpAppName, 
                               LPCTSTR lpKeyName, 
                               LPCTSTR lpString, 
                               LPCTSTR lpFileName);
}
#endif
