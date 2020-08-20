#include "common/sys_incl.h"
#include "common/macro.h"
#include "common/inifile.h"

namespace BaseLib
{
#ifndef _MACRO_OS_WIN32
BOOL 
CIniFile::Open(void)
{
    std::ifstream fin(m_szFileName.c_str());
    if(!fin.is_open())
        return FALSE;

    m_vectIni.clear();
    while(!fin.eof())
    {
        std::string inbuf;
        std::getline(fin, inbuf);
        inbuf.erase(inbuf.find_last_not_of("\r\n") + 1);
        m_vectIni.push_back(inbuf);
    }
    fin.close();
    return TRUE;
}

BOOL
CIniFile::Save(void)
{
    std::ofstream fout(m_szFileName.c_str(), std::ios::out);
    if(!fout.is_open())
        return false;
    copy(m_vectIni.begin(), m_vectIni.end() - 1, std::ostream_iterator<std::string>(fout, "\n"));
    copy(m_vectIni.end() - 1, m_vectIni.end(), std::ostream_iterator<std::string>(fout, ""));
    fout.close();
    return TRUE;
}

void
CIniFile::RemoveComment(std::string &strLine)
{
    // 去除行尾的空白字符
    strLine.erase(strLine.find_last_not_of(" \t") + 1);

    // 内容以;或#开始，则认为整行都是注释
    if (strLine[0] == ';' || strLine[0] == '#')
    {
        strLine = "";
    }

    //  ;或 #之后的内容认为是注释
    if (strLine.find(" ;") != std::string::npos)
    {
        strLine.erase(strLine.find_first_of(" ;"));
    }

    if (strLine.find(" #") != std::string::npos)
    {
        strLine.erase(strLine.find_first_of(" #"));
    }
}

BOOL
CIniFile::GetSectionName(const std::string &strLine, std::string &strSect)
{
    int first, last;

    if ((first = strLine.find_first_of("[")) == std::string::npos)
    {
        return FALSE;
    }

    if ((last = strLine.find_first_of("]", first + 1)) == std::string::npos)
    {
        return FALSE;
    }
    // 取[]之间的内容作为SECTION名
    strSect = strLine.substr(first + 1, last - first - 1);
    return TRUE;
}

BOOL
CIniFile::GetKeyValue(const std::string &strLine,
                      std::string &strKey, 
                      int &nValuePos, 
                      int &nValueLen)
{
    int split;

    if ((split = strLine.find_first_of("=")) == std::string::npos)
    {
        return FALSE;
    }

    strKey = strLine.substr(0, split);
    strKey.erase(0, strKey.find_first_not_of(" \t"));
    strKey.erase(strKey.find_last_not_of(" \t") + 1);
    nValuePos = strLine.find_first_not_of(" \t", split + 1);
    if (nValuePos == std::string::npos)
    {
        nValuePos = split + 1;
    }
    nValueLen = strLine.find_last_not_of(" \t\r\n") - nValuePos + 1;

    return TRUE;
}

BOOL
CIniFile::GetSection(const std::string &strSectName, std::string &strSectValue)
{
	std::string tmpSectName;
	BOOL bFindSect = FALSE;

	for (INIVECTOR::iterator itor = m_vectIni.begin(); itor != m_vectIni.end(); itor++)
	{        
		std::string strLine(*itor);

        RemoveComment(strLine);
		if (bFindSect)
		{
			// 是否到了下一个ECTION
			if (GetSectionName(strLine, tmpSectName))
			{
				return TRUE;
			}
			if (!strLine.empty())
			{
				strSectValue.append(strLine);
				strSectValue.append(NEW_LINE);
			}
		}
		else
		{
			if (GetSectionName(strLine, tmpSectName))
			{
				if (strcasecmp(strSectName.c_str(), tmpSectName.c_str()) == 0)
				{
					bFindSect = TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL
CIniFile::Search(const std::string &strSect, 
                 const std::string &strKey, 
                 BOOL &bFindSect,
                 INIVECTOR::iterator &itor,
                 int &nValuePos, 
                 int &nValueLen)
{
    std::string tmpSectName, tmpKey;
    bFindSect = FALSE;

    for (itor = m_vectIni.begin(); itor != m_vectIni.end(); itor++)
    {        
        std::string strLine(*itor);

        RemoveComment(strLine);
        if (bFindSect)
        {
            // 是否到了下一个ECTION
            if (GetSectionName(strLine, tmpSectName))
            {
                return FALSE;
            }
            if (GetKeyValue(strLine, tmpKey, nValuePos, nValueLen))
            {
                if (strcasecmp(strKey.c_str(), tmpKey.c_str()) == 0)
                {
                    return TRUE;
                }
            }
        }
        else
        {
            if (GetSectionName(strLine, tmpSectName))
            {
                if (strcasecmp(strSect.c_str(), tmpSectName.c_str()) == 0)
                {
                    bFindSect = TRUE;
                }
            }
        }
    }
    return FALSE;
}

void
CIniFile::AddSection(const std::string &strSect)
{
    if (m_vectIni.size() > 0 && !m_vectIni[m_vectIni.size() - 1].empty())
    {
        m_vectIni.push_back("");
    }
    m_vectIni.push_back("[" + strSect + "]");
}

void
CIniFile::AddKey(INIVECTOR::iterator itor, 
                 const std::string &strKey,
                 const std::string &strValue)
{
    while (itor != m_vectIni.begin())
    {
        itor--;

        if (!(*itor).empty())
        {
            itor++;
            break;
        }
    }

    m_vectIni.insert(itor, strKey + "=" + strValue);
}

BOOL
CIniFile::GetValue (const std::string &strSect, 
                    const std::string &strKey, 
                    const std::string &lpszDefault,
                    std::string &strOut)
{
    
    INIVECTOR::iterator itor;
    int nValuePos, nValueLen;
    BOOL bFindSect;

    if (m_bOpened && Search(strSect, strKey, bFindSect, itor, nValuePos, nValueLen))
    {
        strOut = (*itor).substr(nValuePos, nValueLen);
        return TRUE;
    }
    else
    {
        strOut = lpszDefault;
        return FALSE;
    }
};

void 
CIniFile::SetValue (const std::string &strSect, 
                    const std::string &strKey, 
                    const std::string &strValue)
{
    std::string strOut;
    INIVECTOR::iterator itor;
    int nValuePos, nValueLen;
    BOOL bFindSect;

    if (m_bOpened && Search(strSect, strKey, bFindSect, itor, nValuePos, nValueLen))
    {
        (*itor).replace(nValuePos, nValueLen, strValue);
    }
    else
    {
        if (!bFindSect)
        {
            AddSection(strSect);
            Search(strSect, strKey, bFindSect, itor, nValuePos, nValueLen);
        }
        AddKey(itor, strKey, strValue);
    }
    this->Save();
};
#endif    

CIniFile::CIniFile (LPCSTR lpFileName)
{
	m_szFileName = SAFE_STRING (lpFileName);
#ifndef _MACRO_OS_WIN32
    m_bOpened = Open();
#endif
}

std::string
CIniFile::ReadSection (LPCSTR lpSection)
{
	std::string strOut;
#ifdef _MACRO_OS_WIN32
	TCHAR szTmp[8192] = {0};
	DWORD dwLength = sizeof (szTmp) - 1;
	GetPrivateProfileSection(lpSection,
						     szTmp,
							 dwLength,
							 m_szFileName.c_str());

	for (char *lpName = szTmp; *lpName; lpName += strlen(lpName) + 1)
	{
		strOut.append(lpName);
		strOut.append(NEW_LINE);
	}

#else
	GetSection(lpSection, strOut);
#endif
	return strOut;
}

BOOL
CIniFile::ReadBool (LPCSTR lpSection, LPCSTR lpIdent, BOOL bDefault)
{
	return ReadInteger (lpSection, lpIdent, bDefault);
}

std::string
CIniFile::ReadString (LPCSTR lpSection, LPCSTR lpIdent, LPCSTR lpszDefault)
{
	std::string szDefault = SAFE_STRING (lpszDefault);
	if (INVALID_PARAM_LPSTR (lpSection) ||
		INVALID_PARAM_LPSTR (lpIdent)) 
	{
		return szDefault;
	}

	TCHAR szTmp[8192] = {0};
	DWORD dwLength = sizeof (szTmp) - 1;

#ifdef _MACRO_OS_WIN32
	::GetPrivateProfileStringA (lpSection,
		lpIdent,
		szDefault.c_str(),
		szTmp,
		dwLength,
		m_szFileName.c_str());
	return szTmp[0] ? szTmp : szDefault;
#else
    std::string strOut;

    this->GetValue(lpSection, lpIdent, szDefault, strOut);
    return strOut;
#endif

}

int
CIniFile::ReadInteger (LPCSTR lpSection, LPCSTR lpIdent, int nDefault)
{
	char szBuffer[64] = {0};
	sprintf (szBuffer, "%d", nDefault);
	std::string szValue = ReadString (lpSection, lpIdent, szBuffer);

	sscanf (szValue.c_str(), "%d", &nDefault);
	return nDefault;
}

void
CIniFile::WriteString (LPCSTR lpSection, LPCSTR lpIdent, LPCSTR lpValue)
{
	if (INVALID_PARAM_LPSTR (lpSection) || 
		INVALID_PARAM_LPSTR (lpIdent) ||
		!lpValue) 
	{
		return;
	}

#ifdef _MACRO_OS_WIN32
	::WritePrivateProfileString (lpSection,
		lpIdent,
		lpValue,
		m_szFileName.c_str());
#else
    this->SetValue(lpSection, lpIdent, lpValue);
#endif
}

void
CIniFile::WriteBool (LPCSTR lpSection, LPCSTR lpIdent, BOOL bValue)
{
	WriteInteger (lpSection, lpIdent, bValue);
}

void
CIniFile::WriteInteger (LPCSTR lpSection, LPCSTR lpIdent, int nValue)
{
	char szBuffer[64] = {0};
	sprintf (szBuffer, "%d", nValue);

	WriteString (lpSection, lpIdent, szBuffer);
}


DWORD GetPrivateProfileString(LPCSTR lpAppName, 
                              LPCSTR lpKeyName, 
                              LPCSTR lpDefault, 
                              LPSTR lpReturnedString,
                              DWORD nSize, 
                              LPCSTR lpFileName)
{
#ifdef _MACRO_OS_WIN32
	return ::GetPrivateProfileStringA (lpAppName,
		lpKeyName,
		lpDefault,
		lpReturnedString,
		nSize,
		lpFileName);
#else
    CIniFile ini(lpFileName);
    std::string strOut = ini.ReadString(lpAppName, lpKeyName, lpDefault);
    strncpy(lpReturnedString, strOut.c_str(), nSize);
    return strlen(lpReturnedString);
#endif    
}

int  GetPrivateProfileInt(LPCSTR lpAppName, 
                          LPCSTR lpKeyName, 
                          INT     nDefault, 
                          LPCSTR lpFileName)
{
    CIniFile ini(lpFileName);
    return ini.ReadInteger(lpAppName, lpKeyName, nDefault);
}

void WritePrivateProfileString(LPCSTR lpAppName, 
                               LPCSTR lpKeyName, 
                               LPCSTR lpString, 
                               LPCSTR lpFileName)
{
    CIniFile ini(lpFileName);
    ini.WriteString(lpAppName, lpKeyName, lpString);
}

}
